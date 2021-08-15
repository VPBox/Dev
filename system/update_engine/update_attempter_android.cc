//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/update_attempter_android.h"

#include <algorithm>
#include <map>
#include <memory>
#include <utility>

#include <android-base/properties.h>
#include <base/bind.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <brillo/data_encoding.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/strings/string_utils.h>
#include <log/log_safetynet.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/error_code_utils.h"
#include "update_engine/common/file_fetcher.h"
#include "update_engine/common/utils.h"
#include "update_engine/daemon_state_interface.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/metrics_utils.h"
#include "update_engine/network_selector.h"
#include "update_engine/payload_consumer/delta_performer.h"
#include "update_engine/payload_consumer/download_action.h"
#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/payload_consumer/file_descriptor_utils.h"
#include "update_engine/payload_consumer/filesystem_verifier_action.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_consumer/payload_metadata.h"
#include "update_engine/payload_consumer/postinstall_runner_action.h"
#include "update_engine/update_boot_flags_action.h"
#include "update_engine/update_status_utils.h"

#ifndef _UE_SIDELOAD
// Do not include support for external HTTP(s) urls when building
// update_engine_sideload.
#include "update_engine/libcurl_http_fetcher.h"
#endif

using base::Bind;
using base::Time;
using base::TimeDelta;
using base::TimeTicks;
using std::shared_ptr;
using std::string;
using std::vector;
using update_engine::UpdateEngineStatus;

namespace chromeos_update_engine {

namespace {

// Minimum threshold to broadcast an status update in progress and time.
const double kBroadcastThresholdProgress = 0.01;  // 1%
const int kBroadcastThresholdSeconds = 10;

const char* const kErrorDomain = "update_engine";
// TODO(deymo): Convert the different errors to a numeric value to report them
// back on the service error.
const char* const kGenericError = "generic_error";

// Log and set the error on the passed ErrorPtr.
bool LogAndSetError(brillo::ErrorPtr* error,
                    const base::Location& location,
                    const string& reason) {
  brillo::Error::AddTo(error, location, kErrorDomain, kGenericError, reason);
  LOG(ERROR) << "Replying with failure: " << location.ToString() << ": "
             << reason;
  return false;
}

bool GetHeaderAsBool(const string& header, bool default_value) {
  int value = 0;
  if (base::StringToInt(header, &value) && (value == 0 || value == 1))
    return value == 1;
  return default_value;
}

}  // namespace

UpdateAttempterAndroid::UpdateAttempterAndroid(
    DaemonStateInterface* daemon_state,
    PrefsInterface* prefs,
    BootControlInterface* boot_control,
    HardwareInterface* hardware)
    : daemon_state_(daemon_state),
      prefs_(prefs),
      boot_control_(boot_control),
      hardware_(hardware),
      processor_(new ActionProcessor()),
      clock_(new Clock()) {
  metrics_reporter_ = metrics::CreateMetricsReporter();
  network_selector_ = network::CreateNetworkSelector();
}

UpdateAttempterAndroid::~UpdateAttempterAndroid() {
  // Release ourselves as the ActionProcessor's delegate to prevent
  // re-scheduling the updates due to the processing stopped.
  processor_->set_delegate(nullptr);
}

void UpdateAttempterAndroid::Init() {
  // In case of update_engine restart without a reboot we need to restore the
  // reboot needed state.
  if (UpdateCompletedOnThisBoot()) {
    SetStatusAndNotify(UpdateStatus::UPDATED_NEED_REBOOT);
  } else {
    SetStatusAndNotify(UpdateStatus::IDLE);
    UpdatePrefsAndReportUpdateMetricsOnReboot();
  }
}

bool UpdateAttempterAndroid::ApplyPayload(
    const string& payload_url,
    int64_t payload_offset,
    int64_t payload_size,
    const vector<string>& key_value_pair_headers,
    brillo::ErrorPtr* error) {
  if (status_ == UpdateStatus::UPDATED_NEED_REBOOT) {
    return LogAndSetError(
        error, FROM_HERE, "An update already applied, waiting for reboot");
  }
  if (processor_->IsRunning()) {
    return LogAndSetError(
        error, FROM_HERE, "Already processing an update, cancel it first.");
  }
  DCHECK(status_ == UpdateStatus::IDLE);

  std::map<string, string> headers;
  for (const string& key_value_pair : key_value_pair_headers) {
    string key;
    string value;
    if (!brillo::string_utils::SplitAtFirst(
            key_value_pair, "=", &key, &value, false)) {
      return LogAndSetError(
          error, FROM_HERE, "Passed invalid header: " + key_value_pair);
    }
    if (!headers.emplace(key, value).second)
      return LogAndSetError(error, FROM_HERE, "Passed repeated key: " + key);
  }

  // Unique identifier for the payload. An empty string means that the payload
  // can't be resumed.
  string payload_id = (headers[kPayloadPropertyFileHash] +
                       headers[kPayloadPropertyMetadataHash]);

  // Setup the InstallPlan based on the request.
  install_plan_ = InstallPlan();

  install_plan_.download_url = payload_url;
  install_plan_.version = "";
  base_offset_ = payload_offset;
  InstallPlan::Payload payload;
  payload.size = payload_size;
  if (!payload.size) {
    if (!base::StringToUint64(headers[kPayloadPropertyFileSize],
                              &payload.size)) {
      payload.size = 0;
    }
  }
  if (!brillo::data_encoding::Base64Decode(headers[kPayloadPropertyFileHash],
                                           &payload.hash)) {
    LOG(WARNING) << "Unable to decode base64 file hash: "
                 << headers[kPayloadPropertyFileHash];
  }
  if (!base::StringToUint64(headers[kPayloadPropertyMetadataSize],
                            &payload.metadata_size)) {
    payload.metadata_size = 0;
  }
  // The |payload.type| is not used anymore since minor_version 3.
  payload.type = InstallPayloadType::kUnknown;
  install_plan_.payloads.push_back(payload);

  // The |public_key_rsa| key would override the public key stored on disk.
  install_plan_.public_key_rsa = "";

  install_plan_.hash_checks_mandatory = hardware_->IsOfficialBuild();
  install_plan_.is_resume = !payload_id.empty() &&
                            DeltaPerformer::CanResumeUpdate(prefs_, payload_id);
  if (!install_plan_.is_resume) {
    if (!DeltaPerformer::ResetUpdateProgress(prefs_, false)) {
      LOG(WARNING) << "Unable to reset the update progress.";
    }
    if (!prefs_->SetString(kPrefsUpdateCheckResponseHash, payload_id)) {
      LOG(WARNING) << "Unable to save the update check response hash.";
    }
  }
  install_plan_.source_slot = boot_control_->GetCurrentSlot();
  install_plan_.target_slot = install_plan_.source_slot == 0 ? 1 : 0;

  install_plan_.powerwash_required =
      GetHeaderAsBool(headers[kPayloadPropertyPowerwash], false);

  install_plan_.switch_slot_on_reboot =
      GetHeaderAsBool(headers[kPayloadPropertySwitchSlotOnReboot], true);

  install_plan_.run_post_install = true;
  // Optionally skip post install if and only if:
  // a) we're resuming
  // b) post install has already succeeded before
  // c) RUN_POST_INSTALL is set to 0.
  if (install_plan_.is_resume && prefs_->Exists(kPrefsPostInstallSucceeded)) {
    bool post_install_succeeded = false;
    if (prefs_->GetBoolean(kPrefsPostInstallSucceeded,
                           &post_install_succeeded) &&
        post_install_succeeded) {
      install_plan_.run_post_install =
          GetHeaderAsBool(headers[kPayloadPropertyRunPostInstall], true);
    }
  }

  // Skip writing verity if we're resuming and verity has already been written.
  install_plan_.write_verity = true;
  if (install_plan_.is_resume && prefs_->Exists(kPrefsVerityWritten)) {
    bool verity_written = false;
    if (prefs_->GetBoolean(kPrefsVerityWritten, &verity_written) &&
        verity_written) {
      install_plan_.write_verity = false;
    }
  }

  NetworkId network_id = kDefaultNetworkId;
  if (!headers[kPayloadPropertyNetworkId].empty()) {
    if (!base::StringToUint64(headers[kPayloadPropertyNetworkId],
                              &network_id)) {
      return LogAndSetError(
          error,
          FROM_HERE,
          "Invalid network_id: " + headers[kPayloadPropertyNetworkId]);
    }
    if (!network_selector_->SetProcessNetwork(network_id)) {
      return LogAndSetError(
          error,
          FROM_HERE,
          "Unable to set network_id: " + headers[kPayloadPropertyNetworkId]);
    }
  }

  LOG(INFO) << "Using this install plan:";
  install_plan_.Dump();

  HttpFetcher* fetcher = nullptr;
  if (FileFetcher::SupportedUrl(payload_url)) {
    DLOG(INFO) << "Using FileFetcher for file URL.";
    fetcher = new FileFetcher();
  } else {
#ifdef _UE_SIDELOAD
    LOG(FATAL) << "Unsupported sideload URI: " << payload_url;
#else
    LibcurlHttpFetcher* libcurl_fetcher =
        new LibcurlHttpFetcher(&proxy_resolver_, hardware_);
    libcurl_fetcher->set_server_to_check(ServerToCheck::kDownload);
    fetcher = libcurl_fetcher;
#endif  // _UE_SIDELOAD
  }
  // Setup extra headers.
  if (!headers[kPayloadPropertyAuthorization].empty())
    fetcher->SetHeader("Authorization", headers[kPayloadPropertyAuthorization]);
  if (!headers[kPayloadPropertyUserAgent].empty())
    fetcher->SetHeader("User-Agent", headers[kPayloadPropertyUserAgent]);

  BuildUpdateActions(fetcher);

  SetStatusAndNotify(UpdateStatus::UPDATE_AVAILABLE);

  UpdatePrefsOnUpdateStart(install_plan_.is_resume);
  // TODO(xunchang) report the metrics for unresumable updates

  ScheduleProcessingStart();
  return true;
}

bool UpdateAttempterAndroid::SuspendUpdate(brillo::ErrorPtr* error) {
  if (!processor_->IsRunning())
    return LogAndSetError(error, FROM_HERE, "No ongoing update to suspend.");
  processor_->SuspendProcessing();
  return true;
}

bool UpdateAttempterAndroid::ResumeUpdate(brillo::ErrorPtr* error) {
  if (!processor_->IsRunning())
    return LogAndSetError(error, FROM_HERE, "No ongoing update to resume.");
  processor_->ResumeProcessing();
  return true;
}

bool UpdateAttempterAndroid::CancelUpdate(brillo::ErrorPtr* error) {
  if (!processor_->IsRunning())
    return LogAndSetError(error, FROM_HERE, "No ongoing update to cancel.");
  processor_->StopProcessing();
  return true;
}

bool UpdateAttempterAndroid::ResetStatus(brillo::ErrorPtr* error) {
  LOG(INFO) << "Attempting to reset state from "
            << UpdateStatusToString(status_) << " to UpdateStatus::IDLE";

  switch (status_) {
    case UpdateStatus::IDLE:
      return true;

    case UpdateStatus::UPDATED_NEED_REBOOT: {
      // Remove the reboot marker so that if the machine is rebooted
      // after resetting to idle state, it doesn't go back to
      // UpdateStatus::UPDATED_NEED_REBOOT state.
      bool ret_value = prefs_->Delete(kPrefsUpdateCompletedOnBootId);
      ClearMetricsPrefs();

      // Update the boot flags so the current slot has higher priority.
      if (!boot_control_->SetActiveBootSlot(boot_control_->GetCurrentSlot()))
        ret_value = false;

      // Mark the current slot as successful again, since marking it as active
      // may reset the successful bit. We ignore the result of whether marking
      // the current slot as successful worked.
      if (!boot_control_->MarkBootSuccessfulAsync(Bind([](bool successful) {})))
        ret_value = false;

      if (!ret_value) {
        return LogAndSetError(
            error, FROM_HERE, "Failed to reset the status to ");
      }

      SetStatusAndNotify(UpdateStatus::IDLE);
      LOG(INFO) << "Reset status successful";
      return true;
    }

    default:
      return LogAndSetError(
          error,
          FROM_HERE,
          "Reset not allowed in this state. Cancel the ongoing update first");
  }
}

bool UpdateAttempterAndroid::VerifyPayloadApplicable(
    const std::string& metadata_filename, brillo::ErrorPtr* error) {
  FileDescriptorPtr fd(new EintrSafeFileDescriptor);
  if (!fd->Open(metadata_filename.c_str(), O_RDONLY)) {
    return LogAndSetError(
        error, FROM_HERE, "Failed to open " + metadata_filename);
  }
  brillo::Blob metadata(kMaxPayloadHeaderSize);
  if (!fd->Read(metadata.data(), metadata.size())) {
    return LogAndSetError(
        error,
        FROM_HERE,
        "Failed to read payload header from " + metadata_filename);
  }
  ErrorCode errorcode;
  PayloadMetadata payload_metadata;
  if (payload_metadata.ParsePayloadHeader(metadata, &errorcode) !=
      MetadataParseResult::kSuccess) {
    return LogAndSetError(error,
                          FROM_HERE,
                          "Failed to parse payload header: " +
                              utils::ErrorCodeToString(errorcode));
  }
  uint64_t metadata_size = payload_metadata.GetMetadataSize() +
                           payload_metadata.GetMetadataSignatureSize();
  if (metadata_size < kMaxPayloadHeaderSize ||
      metadata_size >
          static_cast<uint64_t>(utils::FileSize(metadata_filename))) {
    return LogAndSetError(
        error,
        FROM_HERE,
        "Invalid metadata size: " + std::to_string(metadata_size));
  }
  metadata.resize(metadata_size);
  if (!fd->Read(metadata.data() + kMaxPayloadHeaderSize,
                metadata.size() - kMaxPayloadHeaderSize)) {
    return LogAndSetError(
        error,
        FROM_HERE,
        "Failed to read metadata and signature from " + metadata_filename);
  }
  fd->Close();

  string public_key;
  if (!utils::ReadFile(constants::kUpdatePayloadPublicKeyPath, &public_key)) {
    return LogAndSetError(error, FROM_HERE, "Failed to read public key.");
  }
  errorcode =
      payload_metadata.ValidateMetadataSignature(metadata, "", public_key);
  if (errorcode != ErrorCode::kSuccess) {
    return LogAndSetError(error,
                          FROM_HERE,
                          "Failed to validate metadata signature: " +
                              utils::ErrorCodeToString(errorcode));
  }
  DeltaArchiveManifest manifest;
  if (!payload_metadata.GetManifest(metadata, &manifest)) {
    return LogAndSetError(error, FROM_HERE, "Failed to parse manifest.");
  }

  BootControlInterface::Slot current_slot = boot_control_->GetCurrentSlot();
  for (const PartitionUpdate& partition : manifest.partitions()) {
    if (!partition.has_old_partition_info())
      continue;
    string partition_path;
    if (!boot_control_->GetPartitionDevice(
            partition.partition_name(), current_slot, &partition_path)) {
      return LogAndSetError(
          error,
          FROM_HERE,
          "Failed to get partition device for " + partition.partition_name());
    }
    if (!fd->Open(partition_path.c_str(), O_RDONLY)) {
      return LogAndSetError(
          error, FROM_HERE, "Failed to open " + partition_path);
    }
    for (const InstallOperation& operation : partition.operations()) {
      if (!operation.has_src_sha256_hash())
        continue;
      brillo::Blob source_hash;
      if (!fd_utils::ReadAndHashExtents(fd,
                                        operation.src_extents(),
                                        manifest.block_size(),
                                        &source_hash)) {
        return LogAndSetError(
            error, FROM_HERE, "Failed to hash " + partition_path);
      }
      if (!DeltaPerformer::ValidateSourceHash(
              source_hash, operation, fd, &errorcode)) {
        return false;
      }
    }
    fd->Close();
  }
  return true;
}

void UpdateAttempterAndroid::ProcessingDone(const ActionProcessor* processor,
                                            ErrorCode code) {
  LOG(INFO) << "Processing Done.";

  switch (code) {
    case ErrorCode::kSuccess:
      // Update succeeded.
      WriteUpdateCompletedMarker();
      prefs_->SetInt64(kPrefsDeltaUpdateFailures, 0);

      LOG(INFO) << "Update successfully applied, waiting to reboot.";
      break;

    case ErrorCode::kFilesystemCopierError:
    case ErrorCode::kNewRootfsVerificationError:
    case ErrorCode::kNewKernelVerificationError:
    case ErrorCode::kFilesystemVerifierError:
    case ErrorCode::kDownloadStateInitializationError:
      // Reset the ongoing update for these errors so it starts from the
      // beginning next time.
      DeltaPerformer::ResetUpdateProgress(prefs_, false);
      LOG(INFO) << "Resetting update progress.";
      break;

    case ErrorCode::kPayloadTimestampError:
      // SafetyNet logging, b/36232423
      android_errorWriteLog(0x534e4554, "36232423");
      break;

    default:
      // Ignore all other error codes.
      break;
  }

  TerminateUpdateAndNotify(code);
}

void UpdateAttempterAndroid::ProcessingStopped(
    const ActionProcessor* processor) {
  TerminateUpdateAndNotify(ErrorCode::kUserCanceled);
}

void UpdateAttempterAndroid::ActionCompleted(ActionProcessor* processor,
                                             AbstractAction* action,
                                             ErrorCode code) {
  // Reset download progress regardless of whether or not the download
  // action succeeded.
  const string type = action->Type();
  if (type == DownloadAction::StaticType()) {
    download_progress_ = 0;
  }
  if (type == PostinstallRunnerAction::StaticType()) {
    bool succeeded =
        code == ErrorCode::kSuccess || code == ErrorCode::kUpdatedButNotActive;
    prefs_->SetBoolean(kPrefsPostInstallSucceeded, succeeded);
  }
  if (code != ErrorCode::kSuccess) {
    // If an action failed, the ActionProcessor will cancel the whole thing.
    return;
  }
  if (type == DownloadAction::StaticType()) {
    SetStatusAndNotify(UpdateStatus::FINALIZING);
  } else if (type == FilesystemVerifierAction::StaticType()) {
    prefs_->SetBoolean(kPrefsVerityWritten, true);
  }
}

void UpdateAttempterAndroid::BytesReceived(uint64_t bytes_progressed,
                                           uint64_t bytes_received,
                                           uint64_t total) {
  double progress = 0;
  if (total)
    progress = static_cast<double>(bytes_received) / static_cast<double>(total);
  if (status_ != UpdateStatus::DOWNLOADING || bytes_received == total) {
    download_progress_ = progress;
    SetStatusAndNotify(UpdateStatus::DOWNLOADING);
  } else {
    ProgressUpdate(progress);
  }

  // Update the bytes downloaded in prefs.
  int64_t current_bytes_downloaded =
      metrics_utils::GetPersistedValue(kPrefsCurrentBytesDownloaded, prefs_);
  int64_t total_bytes_downloaded =
      metrics_utils::GetPersistedValue(kPrefsTotalBytesDownloaded, prefs_);
  prefs_->SetInt64(kPrefsCurrentBytesDownloaded,
                   current_bytes_downloaded + bytes_progressed);
  prefs_->SetInt64(kPrefsTotalBytesDownloaded,
                   total_bytes_downloaded + bytes_progressed);
}

bool UpdateAttempterAndroid::ShouldCancel(ErrorCode* cancel_reason) {
  // TODO(deymo): Notify the DownloadAction that it should cancel the update
  // download.
  return false;
}

void UpdateAttempterAndroid::DownloadComplete() {
  // Nothing needs to be done when the download completes.
}

void UpdateAttempterAndroid::ProgressUpdate(double progress) {
  // Self throttle based on progress. Also send notifications if progress is
  // too slow.
  if (progress == 1.0 ||
      progress - download_progress_ >= kBroadcastThresholdProgress ||
      TimeTicks::Now() - last_notify_time_ >=
          TimeDelta::FromSeconds(kBroadcastThresholdSeconds)) {
    download_progress_ = progress;
    SetStatusAndNotify(status_);
  }
}

void UpdateAttempterAndroid::ScheduleProcessingStart() {
  LOG(INFO) << "Scheduling an action processor start.";
  brillo::MessageLoop::current()->PostTask(
      FROM_HERE,
      Bind([](ActionProcessor* processor) { processor->StartProcessing(); },
           base::Unretained(processor_.get())));
}

void UpdateAttempterAndroid::TerminateUpdateAndNotify(ErrorCode error_code) {
  if (status_ == UpdateStatus::IDLE) {
    LOG(ERROR) << "No ongoing update, but TerminatedUpdate() called.";
    return;
  }

  boot_control_->Cleanup();

  download_progress_ = 0;
  UpdateStatus new_status =
      (error_code == ErrorCode::kSuccess ? UpdateStatus::UPDATED_NEED_REBOOT
                                         : UpdateStatus::IDLE);
  SetStatusAndNotify(new_status);

  // The network id is only applicable to one download attempt and once it's
  // done the network id should not be re-used anymore.
  if (!network_selector_->SetProcessNetwork(kDefaultNetworkId)) {
    LOG(WARNING) << "Unable to unbind network.";
  }

  for (auto observer : daemon_state_->service_observers())
    observer->SendPayloadApplicationComplete(error_code);

  CollectAndReportUpdateMetricsOnUpdateFinished(error_code);
  ClearMetricsPrefs();
  if (error_code == ErrorCode::kSuccess) {
    // We should only reset the PayloadAttemptNumber if the update succeeds, or
    // we switch to a different payload.
    prefs_->Delete(kPrefsPayloadAttemptNumber);
    metrics_utils::SetSystemUpdatedMarker(clock_.get(), prefs_);
    // Clear the total bytes downloaded if and only if the update succeeds.
    prefs_->SetInt64(kPrefsTotalBytesDownloaded, 0);
  }
}

void UpdateAttempterAndroid::SetStatusAndNotify(UpdateStatus status) {
  status_ = status;
  size_t payload_size =
      install_plan_.payloads.empty() ? 0 : install_plan_.payloads[0].size;
  UpdateEngineStatus status_to_send = {.status = status_,
                                       .progress = download_progress_,
                                       .new_size_bytes = payload_size};

  for (auto observer : daemon_state_->service_observers()) {
    observer->SendStatusUpdate(status_to_send);
  }
  last_notify_time_ = TimeTicks::Now();
}

void UpdateAttempterAndroid::BuildUpdateActions(HttpFetcher* fetcher) {
  CHECK(!processor_->IsRunning());
  processor_->set_delegate(this);

  // Actions:
  auto update_boot_flags_action =
      std::make_unique<UpdateBootFlagsAction>(boot_control_);
  auto install_plan_action = std::make_unique<InstallPlanAction>(install_plan_);
  auto download_action =
      std::make_unique<DownloadAction>(prefs_,
                                       boot_control_,
                                       hardware_,
                                       nullptr,  // system_state, not used.
                                       fetcher,  // passes ownership
                                       true /* interactive */);
  download_action->set_delegate(this);
  download_action->set_base_offset(base_offset_);
  auto filesystem_verifier_action =
      std::make_unique<FilesystemVerifierAction>();
  auto postinstall_runner_action =
      std::make_unique<PostinstallRunnerAction>(boot_control_, hardware_);
  postinstall_runner_action->set_delegate(this);

  // Bond them together. We have to use the leaf-types when calling
  // BondActions().
  BondActions(install_plan_action.get(), download_action.get());
  BondActions(download_action.get(), filesystem_verifier_action.get());
  BondActions(filesystem_verifier_action.get(),
              postinstall_runner_action.get());

  processor_->EnqueueAction(std::move(update_boot_flags_action));
  processor_->EnqueueAction(std::move(install_plan_action));
  processor_->EnqueueAction(std::move(download_action));
  processor_->EnqueueAction(std::move(filesystem_verifier_action));
  processor_->EnqueueAction(std::move(postinstall_runner_action));
}

bool UpdateAttempterAndroid::WriteUpdateCompletedMarker() {
  string boot_id;
  TEST_AND_RETURN_FALSE(utils::GetBootId(&boot_id));
  prefs_->SetString(kPrefsUpdateCompletedOnBootId, boot_id);
  return true;
}

bool UpdateAttempterAndroid::UpdateCompletedOnThisBoot() {
  // In case of an update_engine restart without a reboot, we stored the boot_id
  // when the update was completed by setting a pref, so we can check whether
  // the last update was on this boot or a previous one.
  string boot_id;
  TEST_AND_RETURN_FALSE(utils::GetBootId(&boot_id));

  string update_completed_on_boot_id;
  return (prefs_->Exists(kPrefsUpdateCompletedOnBootId) &&
          prefs_->GetString(kPrefsUpdateCompletedOnBootId,
                            &update_completed_on_boot_id) &&
          update_completed_on_boot_id == boot_id);
}

// Collect and report the android metrics when we terminate the update.
void UpdateAttempterAndroid::CollectAndReportUpdateMetricsOnUpdateFinished(
    ErrorCode error_code) {
  int64_t attempt_number =
      metrics_utils::GetPersistedValue(kPrefsPayloadAttemptNumber, prefs_);
  PayloadType payload_type = kPayloadTypeFull;
  int64_t payload_size = 0;
  for (const auto& p : install_plan_.payloads) {
    if (p.type == InstallPayloadType::kDelta)
      payload_type = kPayloadTypeDelta;
    payload_size += p.size;
  }

  metrics::AttemptResult attempt_result =
      metrics_utils::GetAttemptResult(error_code);
  Time boot_time_start = Time::FromInternalValue(
      metrics_utils::GetPersistedValue(kPrefsUpdateBootTimestampStart, prefs_));
  Time monotonic_time_start = Time::FromInternalValue(
      metrics_utils::GetPersistedValue(kPrefsUpdateTimestampStart, prefs_));
  TimeDelta duration = clock_->GetBootTime() - boot_time_start;
  TimeDelta duration_uptime = clock_->GetMonotonicTime() - monotonic_time_start;

  metrics_reporter_->ReportUpdateAttemptMetrics(
      nullptr,  // system_state
      static_cast<int>(attempt_number),
      payload_type,
      duration,
      duration_uptime,
      payload_size,
      attempt_result,
      error_code);

  int64_t current_bytes_downloaded =
      metrics_utils::GetPersistedValue(kPrefsCurrentBytesDownloaded, prefs_);
  metrics_reporter_->ReportUpdateAttemptDownloadMetrics(
      current_bytes_downloaded,
      0,
      DownloadSource::kNumDownloadSources,
      metrics::DownloadErrorCode::kUnset,
      metrics::ConnectionType::kUnset);

  if (error_code == ErrorCode::kSuccess) {
    int64_t reboot_count =
        metrics_utils::GetPersistedValue(kPrefsNumReboots, prefs_);
    string build_version;
    prefs_->GetString(kPrefsPreviousVersion, &build_version);

    // For android metrics, we only care about the total bytes downloaded
    // for all sources; for now we assume the only download source is
    // HttpsServer.
    int64_t total_bytes_downloaded =
        metrics_utils::GetPersistedValue(kPrefsTotalBytesDownloaded, prefs_);
    int64_t num_bytes_downloaded[kNumDownloadSources] = {};
    num_bytes_downloaded[DownloadSource::kDownloadSourceHttpsServer] =
        total_bytes_downloaded;

    int download_overhead_percentage = 0;
    if (current_bytes_downloaded > 0) {
      download_overhead_percentage =
          (total_bytes_downloaded - current_bytes_downloaded) * 100ull /
          current_bytes_downloaded;
    }
    metrics_reporter_->ReportSuccessfulUpdateMetrics(
        static_cast<int>(attempt_number),
        0,  // update abandoned count
        payload_type,
        payload_size,
        num_bytes_downloaded,
        download_overhead_percentage,
        duration,
        duration_uptime,
        static_cast<int>(reboot_count),
        0);  // url_switch_count
  }
}

void UpdateAttempterAndroid::UpdatePrefsAndReportUpdateMetricsOnReboot() {
  string current_boot_id;
  TEST_AND_RETURN(utils::GetBootId(&current_boot_id));
  // Example: [ro.build.version.incremental]: [4292972]
  string current_version =
      android::base::GetProperty("ro.build.version.incremental", "");
  TEST_AND_RETURN(!current_version.empty());

  // If there's no record of previous version (e.g. due to a data wipe), we
  // save the info of current boot and skip the metrics report.
  if (!prefs_->Exists(kPrefsPreviousVersion)) {
    prefs_->SetString(kPrefsBootId, current_boot_id);
    prefs_->SetString(kPrefsPreviousVersion, current_version);
    ClearMetricsPrefs();
    return;
  }
  string previous_version;
  // update_engine restarted under the same build.
  // TODO(xunchang) identify and report rollback by checking UpdateMarker.
  if (prefs_->GetString(kPrefsPreviousVersion, &previous_version) &&
      previous_version == current_version) {
    string last_boot_id;
    bool is_reboot = prefs_->Exists(kPrefsBootId) &&
                     (prefs_->GetString(kPrefsBootId, &last_boot_id) &&
                      last_boot_id != current_boot_id);
    // Increment the reboot number if |kPrefsNumReboots| exists. That pref is
    // set when we start a new update.
    if (is_reboot && prefs_->Exists(kPrefsNumReboots)) {
      prefs_->SetString(kPrefsBootId, current_boot_id);
      int64_t reboot_count =
          metrics_utils::GetPersistedValue(kPrefsNumReboots, prefs_);
      metrics_utils::SetNumReboots(reboot_count + 1, prefs_);
    }
    return;
  }

  // Now that the build version changes, report the update metrics.
  // TODO(xunchang) check the build version is larger than the previous one.
  prefs_->SetString(kPrefsBootId, current_boot_id);
  prefs_->SetString(kPrefsPreviousVersion, current_version);

  bool previous_attempt_exists = prefs_->Exists(kPrefsPayloadAttemptNumber);
  // |kPrefsPayloadAttemptNumber| should be cleared upon successful update.
  if (previous_attempt_exists) {
    metrics_reporter_->ReportAbnormallyTerminatedUpdateAttemptMetrics();
  }

  metrics_utils::LoadAndReportTimeToReboot(
      metrics_reporter_.get(), prefs_, clock_.get());
  ClearMetricsPrefs();

  // Also reset the update progress if the build version has changed.
  if (!DeltaPerformer::ResetUpdateProgress(prefs_, false)) {
    LOG(WARNING) << "Unable to reset the update progress.";
  }
}

// Save the update start time. Reset the reboot count and attempt number if the
// update isn't a resume; otherwise increment the attempt number.
void UpdateAttempterAndroid::UpdatePrefsOnUpdateStart(bool is_resume) {
  if (!is_resume) {
    metrics_utils::SetNumReboots(0, prefs_);
    metrics_utils::SetPayloadAttemptNumber(1, prefs_);
  } else {
    int64_t attempt_number =
        metrics_utils::GetPersistedValue(kPrefsPayloadAttemptNumber, prefs_);
    metrics_utils::SetPayloadAttemptNumber(attempt_number + 1, prefs_);
  }
  metrics_utils::SetUpdateTimestampStart(clock_->GetMonotonicTime(), prefs_);
  metrics_utils::SetUpdateBootTimestampStart(clock_->GetBootTime(), prefs_);
}

void UpdateAttempterAndroid::ClearMetricsPrefs() {
  CHECK(prefs_);
  prefs_->Delete(kPrefsCurrentBytesDownloaded);
  prefs_->Delete(kPrefsNumReboots);
  prefs_->Delete(kPrefsSystemUpdatedMarker);
  prefs_->Delete(kPrefsUpdateTimestampStart);
  prefs_->Delete(kPrefsUpdateBootTimestampStart);
}

}  // namespace chromeos_update_engine
