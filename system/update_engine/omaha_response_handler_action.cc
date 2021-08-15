//
// Copyright (C) 2011 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/omaha_response_handler_action.h"

#include <limits>
#include <string>

#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <policy/device_policy.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/utils.h"
#include "update_engine/connection_manager_interface.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/payload_consumer/delta_performer.h"
#include "update_engine/payload_state_interface.h"
#include "update_engine/update_manager/policy.h"
#include "update_engine/update_manager/update_manager.h"

using chromeos_update_manager::Policy;
using chromeos_update_manager::UpdateManager;
using std::numeric_limits;
using std::string;

namespace chromeos_update_engine {

OmahaResponseHandlerAction::OmahaResponseHandlerAction(
    SystemState* system_state)
    : system_state_(system_state),
      deadline_file_(constants::kOmahaResponseDeadlineFile) {}

void OmahaResponseHandlerAction::PerformAction() {
  CHECK(HasInputObject());
  ScopedActionCompleter completer(processor_, this);
  const OmahaResponse& response = GetInputObject();
  if (!response.update_exists) {
    LOG(INFO) << "There are no updates. Aborting.";
    completer.set_code(ErrorCode::kNoUpdate);
    return;
  }

  // All decisions as to which URL should be used have already been done. So,
  // make the current URL as the download URL.
  string current_url = system_state_->payload_state()->GetCurrentUrl();
  if (current_url.empty()) {
    // This shouldn't happen as we should always supply the HTTPS backup URL.
    // Handling this anyway, just in case.
    LOG(ERROR) << "There are no suitable URLs in the response to use.";
    completer.set_code(ErrorCode::kOmahaResponseInvalid);
    return;
  }

  // This is the url to the first package, not all packages.
  install_plan_.download_url = current_url;
  install_plan_.version = response.version;
  install_plan_.system_version = response.system_version;

  OmahaRequestParams* const params = system_state_->request_params();
  PayloadStateInterface* const payload_state = system_state_->payload_state();

  // If we're using p2p to download and there is a local peer, use it.
  if (payload_state->GetUsingP2PForDownloading() &&
      !payload_state->GetP2PUrl().empty()) {
    LOG(INFO) << "Replacing URL " << install_plan_.download_url
              << " with local URL " << payload_state->GetP2PUrl()
              << " since p2p is enabled.";
    install_plan_.download_url = payload_state->GetP2PUrl();
    payload_state->SetUsingP2PForDownloading(true);
  }

  // Fill up the other properties based on the response.
  string update_check_response_hash;
  for (const auto& package : response.packages) {
    brillo::Blob raw_hash;
    if (!base::HexStringToBytes(package.hash, &raw_hash)) {
      LOG(ERROR) << "Failed to convert payload hash from hex string to bytes: "
                 << package.hash;
      completer.set_code(ErrorCode::kOmahaResponseInvalid);
      return;
    }
    install_plan_.payloads.push_back(
        {.size = package.size,
         .metadata_size = package.metadata_size,
         .metadata_signature = package.metadata_signature,
         .hash = raw_hash,
         .type = package.is_delta ? InstallPayloadType::kDelta
                                  : InstallPayloadType::kFull});
    update_check_response_hash += package.hash + ":";
  }
  install_plan_.public_key_rsa = response.public_key_rsa;
  install_plan_.hash_checks_mandatory = AreHashChecksMandatory(response);
  install_plan_.is_resume = DeltaPerformer::CanResumeUpdate(
      system_state_->prefs(), update_check_response_hash);
  if (install_plan_.is_resume) {
    payload_state->UpdateResumed();
  } else {
    payload_state->UpdateRestarted();
    LOG_IF(WARNING,
           !DeltaPerformer::ResetUpdateProgress(system_state_->prefs(), false))
        << "Unable to reset the update progress.";
    LOG_IF(WARNING,
           !system_state_->prefs()->SetString(kPrefsUpdateCheckResponseHash,
                                              update_check_response_hash))
        << "Unable to save the update check response hash.";
  }

  if (params->is_install()) {
    install_plan_.target_slot = system_state_->boot_control()->GetCurrentSlot();
    install_plan_.source_slot = BootControlInterface::kInvalidSlot;
  } else {
    install_plan_.source_slot = system_state_->boot_control()->GetCurrentSlot();
    install_plan_.target_slot = install_plan_.source_slot == 0 ? 1 : 0;
  }

  // The Omaha response doesn't include the channel name for this image, so we
  // use the download_channel we used during the request to tag the target slot.
  // This will be used in the next boot to know the channel the image was
  // downloaded from.
  string current_channel_key =
      kPrefsChannelOnSlotPrefix + std::to_string(install_plan_.target_slot);
  system_state_->prefs()->SetString(current_channel_key,
                                    params->download_channel());

  // Checking whether device is able to boot up the returned rollback image.
  if (response.is_rollback) {
    if (!params->rollback_allowed()) {
      LOG(ERROR) << "Received rollback image but rollback is not allowed.";
      completer.set_code(ErrorCode::kOmahaResponseInvalid);
      return;
    }
    auto min_kernel_key_version = static_cast<uint32_t>(
        system_state_->hardware()->GetMinKernelKeyVersion());
    auto min_firmware_key_version = static_cast<uint32_t>(
        system_state_->hardware()->GetMinFirmwareKeyVersion());
    uint32_t kernel_key_version =
        static_cast<uint32_t>(response.rollback_key_version.kernel_key) << 16 |
        static_cast<uint32_t>(response.rollback_key_version.kernel);
    uint32_t firmware_key_version =
        static_cast<uint32_t>(response.rollback_key_version.firmware_key)
            << 16 |
        static_cast<uint32_t>(response.rollback_key_version.firmware);

    // Don't attempt a rollback if the versions are incompatible or the
    // target image does not specify the version information.
    if (kernel_key_version == numeric_limits<uint32_t>::max() ||
        firmware_key_version == numeric_limits<uint32_t>::max() ||
        kernel_key_version < min_kernel_key_version ||
        firmware_key_version < min_firmware_key_version) {
      LOG(ERROR) << "Device won't be able to boot up the rollback image.";
      completer.set_code(ErrorCode::kRollbackNotPossible);
      return;
    }
    install_plan_.is_rollback = true;
  }

  if (response.powerwash_required || params->ShouldPowerwash())
    install_plan_.powerwash_required = true;

  TEST_AND_RETURN(HasOutputPipe());
  if (HasOutputPipe())
    SetOutputObject(install_plan_);
  LOG(INFO) << "Using this install plan:";
  install_plan_.Dump();

  // Send the deadline data (if any) to Chrome through a file. This is a pretty
  // hacky solution but should be OK for now.
  //
  // TODO(petkov): Re-architect this to avoid communication through a
  // file. Ideally, we would include this information in D-Bus's GetStatus
  // method and UpdateStatus signal. A potential issue is that update_engine may
  // be unresponsive during an update download.
  if (!deadline_file_.empty()) {
    if (payload_state->GetRollbackHappened()) {
      // Don't do forced update if rollback has happened since the last update
      // check where policy was present.
      LOG(INFO) << "Not forcing update because a rollback happened.";
      utils::WriteFile(deadline_file_.c_str(), nullptr, 0);
    } else {
      utils::WriteFile(deadline_file_.c_str(),
                       response.deadline.data(),
                       response.deadline.size());
    }
    chmod(deadline_file_.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  }

  // Check the generated install-plan with the Policy to confirm that
  // it can be applied at this time (or at all).
  UpdateManager* const update_manager = system_state_->update_manager();
  CHECK(update_manager);
  auto ec = ErrorCode::kSuccess;
  update_manager->PolicyRequest(
      &Policy::UpdateCanBeApplied, &ec, &install_plan_);
  completer.set_code(ec);
}

bool OmahaResponseHandlerAction::AreHashChecksMandatory(
    const OmahaResponse& response) {
  // We sometimes need to waive the hash checks in order to download from
  // sources that don't provide hashes, such as dev server.
  // At this point UpdateAttempter::IsAnyUpdateSourceAllowed() has already been
  // checked, so an unofficial update URL won't get this far unless it's OK to
  // use without a hash. Additionally, we want to always waive hash checks on
  // unofficial builds (i.e. dev/test images).
  // The end result is this:
  //  * Base image:
  //    - Official URLs require a hash.
  //    - Unofficial URLs only get this far if the IsAnyUpdateSourceAllowed()
  //      devmode/debugd checks pass, in which case the hash is waived.
  //  * Dev/test image:
  //    - Any URL is allowed through with no hash checking.
  if (!system_state_->request_params()->IsUpdateUrlOfficial() ||
      !system_state_->hardware()->IsOfficialBuild()) {
    // Still do a hash check if a public key is included.
    if (!response.public_key_rsa.empty()) {
      // The autoupdate_CatchBadSignatures test checks for this string
      // in log-files. Keep in sync.
      LOG(INFO) << "Mandating payload hash checks since Omaha Response "
                << "for unofficial build includes public RSA key.";
      return true;
    } else {
      LOG(INFO) << "Waiving payload hash checks for unofficial update URL.";
      return false;
    }
  }

  LOG(INFO) << "Mandating hash checks for official URL on official build.";
  return true;
}

}  // namespace chromeos_update_engine
