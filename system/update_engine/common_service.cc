//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common_service.h"

#include <string>

#include <base/bind.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/strings/stringprintf.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/strings/string_utils.h>
#include <policy/device_policy.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs.h"
#include "update_engine/common/utils.h"
#include "update_engine/connection_manager_interface.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/omaha_utils.h"
#include "update_engine/p2p_manager.h"
#include "update_engine/payload_state_interface.h"
#include "update_engine/update_attempter.h"

using base::StringPrintf;
using brillo::ErrorPtr;
using brillo::string_utils::ToString;
using std::string;
using std::vector;
using update_engine::UpdateAttemptFlags;
using update_engine::UpdateEngineStatus;

namespace chromeos_update_engine {

namespace {
// Log and set the error on the passed ErrorPtr.
void LogAndSetError(ErrorPtr* error,
#if BASE_VER < 576279
                    const tracked_objects::Location& location,
#else
                    const base::Location& location,
#endif
                    const string& reason) {
  brillo::Error::AddTo(error,
                       location,
                       UpdateEngineService::kErrorDomain,
                       UpdateEngineService::kErrorFailed,
                       reason);
  LOG(ERROR) << "Sending Update Engine Failure: " << location.ToString() << ": "
             << reason;
}
}  // namespace

const char* const UpdateEngineService::kErrorDomain = "update_engine";
const char* const UpdateEngineService::kErrorFailed =
    "org.chromium.UpdateEngine.Error.Failed";

UpdateEngineService::UpdateEngineService(SystemState* system_state)
    : system_state_(system_state) {}

// org::chromium::UpdateEngineInterfaceInterface methods implementation.

bool UpdateEngineService::SetUpdateAttemptFlags(ErrorPtr* /* error */,
                                                int32_t in_flags_as_int) {
  auto flags = static_cast<UpdateAttemptFlags>(in_flags_as_int);
  LOG(INFO) << "Setting Update Attempt Flags: "
            << "flags=0x" << std::hex << flags << " "
            << "RestrictDownload="
            << ((flags & UpdateAttemptFlags::kFlagRestrictDownload) ? "yes"
                                                                    : "no");
  system_state_->update_attempter()->SetUpdateAttemptFlags(flags);
  return true;
}

bool UpdateEngineService::AttemptUpdate(ErrorPtr* /* error */,
                                        const string& in_app_version,
                                        const string& in_omaha_url,
                                        int32_t in_flags_as_int,
                                        bool* out_result) {
  auto flags = static_cast<UpdateAttemptFlags>(in_flags_as_int);
  bool interactive = !(flags & UpdateAttemptFlags::kFlagNonInteractive);
  bool restrict_downloads = (flags & UpdateAttemptFlags::kFlagRestrictDownload);

  LOG(INFO) << "Attempt update: app_version=\"" << in_app_version << "\" "
            << "omaha_url=\"" << in_omaha_url << "\" "
            << "flags=0x" << std::hex << flags << " "
            << "interactive=" << (interactive ? "yes " : "no ")
            << "RestrictDownload=" << (restrict_downloads ? "yes " : "no ");

  *out_result = system_state_->update_attempter()->CheckForUpdate(
      in_app_version, in_omaha_url, flags);
  return true;
}

bool UpdateEngineService::AttemptInstall(brillo::ErrorPtr* error,
                                         const string& omaha_url,
                                         const vector<string>& dlc_module_ids) {
  if (!system_state_->update_attempter()->CheckForInstall(dlc_module_ids,
                                                          omaha_url)) {
    // TODO(xiaochu): support more detailed error messages.
    LogAndSetError(error, FROM_HERE, "Could not schedule install operation.");
    return false;
  }
  return true;
}

bool UpdateEngineService::AttemptRollback(ErrorPtr* error, bool in_powerwash) {
  LOG(INFO) << "Attempting rollback to non-active partitions.";

  if (!system_state_->update_attempter()->Rollback(in_powerwash)) {
    // TODO(dgarrett): Give a more specific error code/reason.
    LogAndSetError(error, FROM_HERE, "Rollback attempt failed.");
    return false;
  }
  return true;
}

bool UpdateEngineService::CanRollback(ErrorPtr* /* error */,
                                      bool* out_can_rollback) {
  bool can_rollback = system_state_->update_attempter()->CanRollback();
  LOG(INFO) << "Checking to see if we can rollback . Result: " << can_rollback;
  *out_can_rollback = can_rollback;
  return true;
}

bool UpdateEngineService::ResetStatus(ErrorPtr* error) {
  if (!system_state_->update_attempter()->ResetStatus()) {
    // TODO(dgarrett): Give a more specific error code/reason.
    LogAndSetError(error, FROM_HERE, "ResetStatus failed.");
    return false;
  }
  return true;
}

bool UpdateEngineService::GetStatus(ErrorPtr* error,
                                    UpdateEngineStatus* out_status) {
  if (!system_state_->update_attempter()->GetStatus(out_status)) {
    LogAndSetError(error, FROM_HERE, "GetStatus failed.");
    return false;
  }
  return true;
}

bool UpdateEngineService::RebootIfNeeded(ErrorPtr* error) {
  if (!system_state_->update_attempter()->RebootIfNeeded()) {
    // TODO(dgarrett): Give a more specific error code/reason.
    LogAndSetError(error, FROM_HERE, "Reboot not needed, or attempt failed.");
    return false;
  }
  return true;
}

bool UpdateEngineService::SetChannel(ErrorPtr* error,
                                     const string& in_target_channel,
                                     bool in_is_powerwash_allowed) {
  const policy::DevicePolicy* device_policy = system_state_->device_policy();

  // The device_policy is loaded in a lazy way before an update check. Load it
  // now from the libbrillo cache if it wasn't already loaded.
  if (!device_policy) {
    UpdateAttempter* update_attempter = system_state_->update_attempter();
    if (update_attempter) {
      update_attempter->RefreshDevicePolicy();
      device_policy = system_state_->device_policy();
    }
  }

  bool delegated = false;
  if (device_policy && device_policy->GetReleaseChannelDelegated(&delegated) &&
      !delegated) {
    LogAndSetError(error,
                   FROM_HERE,
                   "Cannot set target channel explicitly when channel "
                   "policy/settings is not delegated");
    return false;
  }

  LOG(INFO) << "Setting destination channel to: " << in_target_channel;
  string error_message;
  if (!system_state_->request_params()->SetTargetChannel(
          in_target_channel, in_is_powerwash_allowed, &error_message)) {
    LogAndSetError(error, FROM_HERE, error_message);
    return false;
  }
  return true;
}

bool UpdateEngineService::GetChannel(ErrorPtr* /* error */,
                                     bool in_get_current_channel,
                                     string* out_channel) {
  OmahaRequestParams* rp = system_state_->request_params();
  *out_channel =
      (in_get_current_channel ? rp->current_channel() : rp->target_channel());
  return true;
}

bool UpdateEngineService::SetCohortHint(ErrorPtr* error,
                                        string in_cohort_hint) {
  PrefsInterface* prefs = system_state_->prefs();

  // It is ok to override the cohort hint with an invalid value since it is
  // stored in stateful partition. The code reading it should sanitize it
  // anyway.
  if (!prefs->SetString(kPrefsOmahaCohortHint, in_cohort_hint)) {
    LogAndSetError(
        error,
        FROM_HERE,
        StringPrintf("Error setting the cohort hint value to \"%s\".",
                     in_cohort_hint.c_str()));
    return false;
  }
  return true;
}

bool UpdateEngineService::GetCohortHint(ErrorPtr* error,
                                        string* out_cohort_hint) {
  PrefsInterface* prefs = system_state_->prefs();

  *out_cohort_hint = "";
  if (prefs->Exists(kPrefsOmahaCohortHint) &&
      !prefs->GetString(kPrefsOmahaCohortHint, out_cohort_hint)) {
    LogAndSetError(error, FROM_HERE, "Error getting the cohort hint.");
    return false;
  }
  return true;
}

bool UpdateEngineService::SetP2PUpdatePermission(ErrorPtr* error,
                                                 bool in_enabled) {
  PrefsInterface* prefs = system_state_->prefs();

  if (!prefs->SetBoolean(kPrefsP2PEnabled, in_enabled)) {
    LogAndSetError(
        error,
        FROM_HERE,
        StringPrintf("Error setting the update via p2p permission to %s.",
                     ToString(in_enabled).c_str()));
    return false;
  }
  return true;
}

bool UpdateEngineService::GetP2PUpdatePermission(ErrorPtr* error,
                                                 bool* out_enabled) {
  PrefsInterface* prefs = system_state_->prefs();

  bool p2p_pref = false;  // Default if no setting is present.
  if (prefs->Exists(kPrefsP2PEnabled) &&
      !prefs->GetBoolean(kPrefsP2PEnabled, &p2p_pref)) {
    LogAndSetError(error, FROM_HERE, "Error getting the P2PEnabled setting.");
    return false;
  }

  *out_enabled = p2p_pref;
  return true;
}

bool UpdateEngineService::SetUpdateOverCellularPermission(ErrorPtr* error,
                                                          bool in_allowed) {
  ConnectionManagerInterface* connection_manager =
      system_state_->connection_manager();

  // Check if this setting is allowed by the device policy.
  if (connection_manager->IsAllowedConnectionTypesForUpdateSet()) {
    LogAndSetError(error,
                   FROM_HERE,
                   "Ignoring the update over cellular setting since there's "
                   "a device policy enforcing this setting.");
    return false;
  }

  // If the policy wasn't loaded yet, then it is still OK to change the local
  // setting because the policy will be checked again during the update check.

  PrefsInterface* prefs = system_state_->prefs();

  if (!prefs ||
      !prefs->SetBoolean(kPrefsUpdateOverCellularPermission, in_allowed)) {
    LogAndSetError(error,
                   FROM_HERE,
                   string("Error setting the update over cellular to ") +
                       (in_allowed ? "true" : "false"));
    return false;
  }
  return true;
}

bool UpdateEngineService::SetUpdateOverCellularTarget(
    brillo::ErrorPtr* error,
    const std::string& target_version,
    int64_t target_size) {
  ConnectionManagerInterface* connection_manager =
      system_state_->connection_manager();

  // Check if this setting is allowed by the device policy.
  if (connection_manager->IsAllowedConnectionTypesForUpdateSet()) {
    LogAndSetError(error,
                   FROM_HERE,
                   "Ignoring the update over cellular setting since there's "
                   "a device policy enforcing this setting.");
    return false;
  }

  // If the policy wasn't loaded yet, then it is still OK to change the local
  // setting because the policy will be checked again during the update check.

  PrefsInterface* prefs = system_state_->prefs();

  if (!prefs ||
      !prefs->SetString(kPrefsUpdateOverCellularTargetVersion,
                        target_version) ||
      !prefs->SetInt64(kPrefsUpdateOverCellularTargetSize, target_size)) {
    LogAndSetError(
        error, FROM_HERE, "Error setting the target for update over cellular.");
    return false;
  }
  return true;
}

bool UpdateEngineService::GetUpdateOverCellularPermission(ErrorPtr* error,
                                                          bool* out_allowed) {
  ConnectionManagerInterface* connection_manager =
      system_state_->connection_manager();

  if (connection_manager->IsAllowedConnectionTypesForUpdateSet()) {
    // We have device policy, so ignore the user preferences.
    *out_allowed = connection_manager->IsUpdateAllowedOver(
        ConnectionType::kCellular, ConnectionTethering::kUnknown);
  } else {
    PrefsInterface* prefs = system_state_->prefs();

    if (!prefs || !prefs->Exists(kPrefsUpdateOverCellularPermission)) {
      // Update is not allowed as user preference is not set or not available.
      *out_allowed = false;
      return true;
    }

    bool is_allowed;

    if (!prefs->GetBoolean(kPrefsUpdateOverCellularPermission, &is_allowed)) {
      LogAndSetError(error,
                     FROM_HERE,
                     "Error getting the update over cellular preference.");
      return false;
    }
    *out_allowed = is_allowed;
  }
  return true;
}

bool UpdateEngineService::GetDurationSinceUpdate(ErrorPtr* error,
                                                 int64_t* out_usec_wallclock) {
  base::Time time;
  if (!system_state_->update_attempter()->GetBootTimeAtUpdate(&time)) {
    LogAndSetError(error, FROM_HERE, "No pending update.");
    return false;
  }

  ClockInterface* clock = system_state_->clock();
  *out_usec_wallclock = (clock->GetBootTime() - time).InMicroseconds();
  return true;
}

bool UpdateEngineService::GetPrevVersion(ErrorPtr* /* error */,
                                         string* out_prev_version) {
  *out_prev_version = system_state_->update_attempter()->GetPrevVersion();
  return true;
}

bool UpdateEngineService::GetRollbackPartition(
    ErrorPtr* /* error */, string* out_rollback_partition_name) {
  BootControlInterface::Slot rollback_slot =
      system_state_->update_attempter()->GetRollbackSlot();

  if (rollback_slot == BootControlInterface::kInvalidSlot) {
    out_rollback_partition_name->clear();
    return true;
  }

  string name;
  if (!system_state_->boot_control()->GetPartitionDevice(
          "KERNEL", rollback_slot, &name)) {
    LOG(ERROR) << "Invalid rollback device";
    return false;
  }

  LOG(INFO) << "Getting rollback partition name. Result: " << name;
  *out_rollback_partition_name = name;
  return true;
}

bool UpdateEngineService::GetLastAttemptError(ErrorPtr* /* error */,
                                              int32_t* out_last_attempt_error) {
  ErrorCode error_code =
      system_state_->update_attempter()->GetAttemptErrorCode();
  *out_last_attempt_error = static_cast<int>(error_code);
  return true;
}

bool UpdateEngineService::GetEolStatus(ErrorPtr* error,
                                       int32_t* out_eol_status) {
  PrefsInterface* prefs = system_state_->prefs();

  string str_eol_status;
  if (prefs->Exists(kPrefsOmahaEolStatus) &&
      !prefs->GetString(kPrefsOmahaEolStatus, &str_eol_status)) {
    LogAndSetError(error, FROM_HERE, "Error getting the end-of-life status.");
    return false;
  }

  // StringToEolStatus will return kSupported for invalid values.
  *out_eol_status = static_cast<int32_t>(StringToEolStatus(str_eol_status));
  return true;
}

}  // namespace chromeos_update_engine
