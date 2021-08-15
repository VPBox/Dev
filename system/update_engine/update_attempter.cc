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

#include "update_engine/update_attempter.h"

#include <stdint.h>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/rand_util.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/data_encoding.h>
#include <brillo/errors/error_codes.h>
#include <brillo/message_loops/message_loop.h>
#include <policy/device_policy.h>
#include <policy/libpolicy.h>
#include <update_engine/dbus-constants.h>

#include "update_engine/certificate_checker.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/clock_interface.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/dlcservice_interface.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/subprocess.h"
#include "update_engine/common/utils.h"
#include "update_engine/libcurl_http_fetcher.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/omaha_request_action.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/omaha_response_handler_action.h"
#include "update_engine/p2p_manager.h"
#include "update_engine/payload_consumer/download_action.h"
#include "update_engine/payload_consumer/filesystem_verifier_action.h"
#include "update_engine/payload_consumer/postinstall_runner_action.h"
#include "update_engine/payload_state_interface.h"
#include "update_engine/power_manager_interface.h"
#include "update_engine/system_state.h"
#include "update_engine/update_boot_flags_action.h"
#include "update_engine/update_manager/policy.h"
#include "update_engine/update_manager/policy_utils.h"
#include "update_engine/update_manager/update_manager.h"
#include "update_engine/update_status_utils.h"

using base::Bind;
using base::Callback;
using base::Time;
using base::TimeDelta;
using base::TimeTicks;
using brillo::MessageLoop;
using chromeos_update_manager::CalculateStagingCase;
using chromeos_update_manager::EvalStatus;
using chromeos_update_manager::Policy;
using chromeos_update_manager::StagingCase;
using chromeos_update_manager::UpdateCheckParams;
using std::string;
using std::vector;
using update_engine::UpdateAttemptFlags;
using update_engine::UpdateEngineStatus;

namespace chromeos_update_engine {

const int UpdateAttempter::kMaxDeltaUpdateFailures = 3;

namespace {
const int kMaxConsecutiveObeyProxyRequests = 20;

// Minimum threshold to broadcast an status update in progress and time.
const double kBroadcastThresholdProgress = 0.01;  // 1%
const int kBroadcastThresholdSeconds = 10;

// By default autest bypasses scattering. If we want to test scattering,
// use kScheduledAUTestURLRequest. The URL used is same in both cases, but
// different params are passed to CheckForUpdate().
const char kAUTestURLRequest[] = "autest";
const char kScheduledAUTestURLRequest[] = "autest-scheduled";
}  // namespace

ErrorCode GetErrorCodeForAction(AbstractAction* action, ErrorCode code) {
  if (code != ErrorCode::kError)
    return code;

  const string type = action->Type();
  if (type == OmahaRequestAction::StaticType())
    return ErrorCode::kOmahaRequestError;
  if (type == OmahaResponseHandlerAction::StaticType())
    return ErrorCode::kOmahaResponseHandlerError;
  if (type == FilesystemVerifierAction::StaticType())
    return ErrorCode::kFilesystemVerifierError;
  if (type == PostinstallRunnerAction::StaticType())
    return ErrorCode::kPostinstallRunnerError;

  return code;
}

UpdateAttempter::UpdateAttempter(SystemState* system_state,
                                 CertificateChecker* cert_checker)
    : processor_(new ActionProcessor()),
      system_state_(system_state),
      cert_checker_(cert_checker),
      is_install_(false) {}

UpdateAttempter::~UpdateAttempter() {
  // CertificateChecker might not be initialized in unittests.
  if (cert_checker_)
    cert_checker_->SetObserver(nullptr);
  // Release ourselves as the ActionProcessor's delegate to prevent
  // re-scheduling the updates due to the processing stopped.
  processor_->set_delegate(nullptr);
}

void UpdateAttempter::Init() {
  // Pulling from the SystemState can only be done after construction, since
  // this is an aggregate of various objects (such as the UpdateAttempter),
  // which requires them all to be constructed prior to it being used.
  prefs_ = system_state_->prefs();
  omaha_request_params_ = system_state_->request_params();

  if (cert_checker_)
    cert_checker_->SetObserver(this);

  // In case of update_engine restart without a reboot we need to restore the
  // reboot needed state.
  if (GetBootTimeAtUpdate(nullptr))
    status_ = UpdateStatus::UPDATED_NEED_REBOOT;
  else
    status_ = UpdateStatus::IDLE;
}

bool UpdateAttempter::ScheduleUpdates() {
  if (IsUpdateRunningOrScheduled())
    return false;

  chromeos_update_manager::UpdateManager* const update_manager =
      system_state_->update_manager();
  CHECK(update_manager);
  Callback<void(EvalStatus, const UpdateCheckParams&)> callback =
      Bind(&UpdateAttempter::OnUpdateScheduled, base::Unretained(this));
  // We limit the async policy request to a reasonably short time, to avoid a
  // starvation due to a transient bug.
  update_manager->AsyncPolicyRequest(callback, &Policy::UpdateCheckAllowed);
  waiting_for_scheduled_check_ = true;
  return true;
}

void UpdateAttempter::CertificateChecked(ServerToCheck server_to_check,
                                         CertificateCheckResult result) {
  system_state_->metrics_reporter()->ReportCertificateCheckMetrics(
      server_to_check, result);
}

bool UpdateAttempter::CheckAndReportDailyMetrics() {
  int64_t stored_value;
  Time now = system_state_->clock()->GetWallclockTime();
  if (system_state_->prefs()->Exists(kPrefsDailyMetricsLastReportedAt) &&
      system_state_->prefs()->GetInt64(kPrefsDailyMetricsLastReportedAt,
                                       &stored_value)) {
    Time last_reported_at = Time::FromInternalValue(stored_value);
    TimeDelta time_reported_since = now - last_reported_at;
    if (time_reported_since.InSeconds() < 0) {
      LOG(WARNING) << "Last reported daily metrics "
                   << utils::FormatTimeDelta(time_reported_since) << " ago "
                   << "which is negative. Either the system clock is wrong or "
                   << "the kPrefsDailyMetricsLastReportedAt state variable "
                   << "is wrong.";
      // In this case, report daily metrics to reset.
    } else {
      if (time_reported_since.InSeconds() < 24 * 60 * 60) {
        LOG(INFO) << "Last reported daily metrics "
                  << utils::FormatTimeDelta(time_reported_since) << " ago.";
        return false;
      }
      LOG(INFO) << "Last reported daily metrics "
                << utils::FormatTimeDelta(time_reported_since) << " ago, "
                << "which is more than 24 hours ago.";
    }
  }

  LOG(INFO) << "Reporting daily metrics.";
  system_state_->prefs()->SetInt64(kPrefsDailyMetricsLastReportedAt,
                                   now.ToInternalValue());

  ReportOSAge();

  return true;
}

void UpdateAttempter::ReportOSAge() {
  struct stat sb;

  if (system_state_ == nullptr)
    return;

  if (stat("/etc/lsb-release", &sb) != 0) {
    PLOG(ERROR) << "Error getting file status for /etc/lsb-release "
                << "(Note: this may happen in some unit tests)";
    return;
  }

  Time lsb_release_timestamp = Time::FromTimeSpec(sb.st_ctim);
  Time now = system_state_->clock()->GetWallclockTime();
  TimeDelta age = now - lsb_release_timestamp;
  if (age.InSeconds() < 0) {
    LOG(ERROR) << "The OS age (" << utils::FormatTimeDelta(age)
               << ") is negative. Maybe the clock is wrong? "
               << "(Note: this may happen in some unit tests.)";
    return;
  }

  system_state_->metrics_reporter()->ReportDailyMetrics(age);
}

void UpdateAttempter::Update(const string& app_version,
                             const string& omaha_url,
                             const string& target_channel,
                             const string& target_version_prefix,
                             bool rollback_allowed,
                             bool obey_proxies,
                             bool interactive) {
  // This is normally called frequently enough so it's appropriate to use as a
  // hook for reporting daily metrics.
  // TODO(garnold) This should be hooked to a separate (reliable and consistent)
  // timeout event.
  CheckAndReportDailyMetrics();

  fake_update_success_ = false;
  if (status_ == UpdateStatus::UPDATED_NEED_REBOOT) {
    // Although we have applied an update, we still want to ping Omaha
    // to ensure the number of active statistics is accurate.
    //
    // Also convey to the UpdateEngine.Check.Result metric that we're
    // not performing an update check because of this.
    LOG(INFO) << "Not updating b/c we already updated and we're waiting for "
              << "reboot, we'll ping Omaha instead";
    system_state_->metrics_reporter()->ReportUpdateCheckMetrics(
        system_state_,
        metrics::CheckResult::kRebootPending,
        metrics::CheckReaction::kUnset,
        metrics::DownloadErrorCode::kUnset);
    PingOmaha();
    return;
  }
  if (status_ != UpdateStatus::IDLE) {
    // Update in progress. Do nothing
    return;
  }

  if (!CalculateUpdateParams(app_version,
                             omaha_url,
                             target_channel,
                             target_version_prefix,
                             rollback_allowed,
                             obey_proxies,
                             interactive)) {
    return;
  }

  BuildUpdateActions(interactive);

  SetStatusAndNotify(UpdateStatus::CHECKING_FOR_UPDATE);

  // Update the last check time here; it may be re-updated when an Omaha
  // response is received, but this will prevent us from repeatedly scheduling
  // checks in the case where a response is not received.
  UpdateLastCheckedTime();

  ScheduleProcessingStart();
}

void UpdateAttempter::RefreshDevicePolicy() {
  // Lazy initialize the policy provider, or reload the latest policy data.
  if (!policy_provider_.get())
    policy_provider_.reset(new policy::PolicyProvider());
  policy_provider_->Reload();

  const policy::DevicePolicy* device_policy = nullptr;
  if (policy_provider_->device_policy_is_loaded())
    device_policy = &policy_provider_->GetDevicePolicy();

  if (device_policy)
    LOG(INFO) << "Device policies/settings present";
  else
    LOG(INFO) << "No device policies/settings present.";

  system_state_->set_device_policy(device_policy);
  system_state_->p2p_manager()->SetDevicePolicy(device_policy);
}

void UpdateAttempter::CalculateP2PParams(bool interactive) {
  bool use_p2p_for_downloading = false;
  bool use_p2p_for_sharing = false;

  // Never use p2p for downloading in interactive checks unless the developer
  // has opted in for it via a marker file.
  //
  // (Why would a developer want to opt in? If they are working on the
  // update_engine or p2p codebases so they can actually test their code.)

  if (system_state_ != nullptr) {
    if (!system_state_->p2p_manager()->IsP2PEnabled()) {
      LOG(INFO) << "p2p is not enabled - disallowing p2p for both"
                << " downloading and sharing.";
    } else {
      // Allow p2p for sharing, even in interactive checks.
      use_p2p_for_sharing = true;
      if (!interactive) {
        LOG(INFO) << "Non-interactive check - allowing p2p for downloading";
        use_p2p_for_downloading = true;
      } else {
        LOG(INFO) << "Forcibly disabling use of p2p for downloading "
                  << "since this update attempt is interactive.";
      }
    }
  }

  PayloadStateInterface* const payload_state = system_state_->payload_state();
  payload_state->SetUsingP2PForDownloading(use_p2p_for_downloading);
  payload_state->SetUsingP2PForSharing(use_p2p_for_sharing);
}

bool UpdateAttempter::CalculateUpdateParams(const string& app_version,
                                            const string& omaha_url,
                                            const string& target_channel,
                                            const string& target_version_prefix,
                                            bool rollback_allowed,
                                            bool obey_proxies,
                                            bool interactive) {
  http_response_code_ = 0;
  PayloadStateInterface* const payload_state = system_state_->payload_state();

  // Refresh the policy before computing all the update parameters.
  RefreshDevicePolicy();

  // Check whether we need to clear the rollback-happened preference after
  // policy is available again.
  UpdateRollbackHappened();

  // Update the target version prefix.
  omaha_request_params_->set_target_version_prefix(target_version_prefix);

  // Set whether rollback is allowed.
  omaha_request_params_->set_rollback_allowed(rollback_allowed);

  CalculateStagingParams(interactive);
  // If staging_wait_time_ wasn't set, staging is off, use scattering instead.
  if (staging_wait_time_.InSeconds() == 0) {
    CalculateScatteringParams(interactive);
  }

  CalculateP2PParams(interactive);
  if (payload_state->GetUsingP2PForDownloading() ||
      payload_state->GetUsingP2PForSharing()) {
    // OK, p2p is to be used - start it and perform housekeeping.
    if (!StartP2PAndPerformHousekeeping()) {
      // If this fails, disable p2p for this attempt
      LOG(INFO) << "Forcibly disabling use of p2p since starting p2p or "
                << "performing housekeeping failed.";
      payload_state->SetUsingP2PForDownloading(false);
      payload_state->SetUsingP2PForSharing(false);
    }
  }

  if (!omaha_request_params_->Init(app_version, omaha_url, interactive)) {
    LOG(ERROR) << "Unable to initialize Omaha request params.";
    return false;
  }

  // Set the target channel, if one was provided.
  if (target_channel.empty()) {
    LOG(INFO) << "No target channel mandated by policy.";
  } else {
    LOG(INFO) << "Setting target channel as mandated: " << target_channel;
    // Pass in false for powerwash_allowed until we add it to the policy
    // protobuf.
    string error_message;
    if (!omaha_request_params_->SetTargetChannel(
            target_channel, false, &error_message)) {
      LOG(ERROR) << "Setting the channel failed: " << error_message;
    }

    // Since this is the beginning of a new attempt, update the download
    // channel. The download channel won't be updated until the next attempt,
    // even if target channel changes meanwhile, so that how we'll know if we
    // should cancel the current download attempt if there's such a change in
    // target channel.
    omaha_request_params_->UpdateDownloadChannel();
  }
  // Set the DLC module ID list.
  omaha_request_params_->set_dlc_module_ids(dlc_module_ids_);
  omaha_request_params_->set_is_install(is_install_);

  LOG(INFO) << "target_version_prefix = "
            << omaha_request_params_->target_version_prefix()
            << ", rollback_allowed = "
            << omaha_request_params_->rollback_allowed()
            << ", scatter_factor_in_seconds = "
            << utils::FormatSecs(scatter_factor_.InSeconds());

  LOG(INFO) << "Wall Clock Based Wait Enabled = "
            << omaha_request_params_->wall_clock_based_wait_enabled()
            << ", Update Check Count Wait Enabled = "
            << omaha_request_params_->update_check_count_wait_enabled()
            << ", Waiting Period = "
            << utils::FormatSecs(
                   omaha_request_params_->waiting_period().InSeconds());

  LOG(INFO) << "Use p2p For Downloading = "
            << payload_state->GetUsingP2PForDownloading()
            << ", Use p2p For Sharing = "
            << payload_state->GetUsingP2PForSharing();

  obeying_proxies_ = true;
  if (obey_proxies || proxy_manual_checks_ == 0) {
    LOG(INFO) << "forced to obey proxies";
    // If forced to obey proxies, every 20th request will not use proxies
    proxy_manual_checks_++;
    LOG(INFO) << "proxy manual checks: " << proxy_manual_checks_;
    if (proxy_manual_checks_ >= kMaxConsecutiveObeyProxyRequests) {
      proxy_manual_checks_ = 0;
      obeying_proxies_ = false;
    }
  } else if (base::RandInt(0, 4) == 0) {
    obeying_proxies_ = false;
  }
  LOG_IF(INFO, !obeying_proxies_)
      << "To help ensure updates work, this update check we are ignoring the "
      << "proxy settings and using direct connections.";

  DisableDeltaUpdateIfNeeded();
  return true;
}

void UpdateAttempter::CalculateScatteringParams(bool interactive) {
  // Take a copy of the old scatter value before we update it, as
  // we need to update the waiting period if this value changes.
  TimeDelta old_scatter_factor = scatter_factor_;
  const policy::DevicePolicy* device_policy = system_state_->device_policy();
  if (device_policy) {
    int64_t new_scatter_factor_in_secs = 0;
    device_policy->GetScatterFactorInSeconds(&new_scatter_factor_in_secs);
    if (new_scatter_factor_in_secs < 0)  // sanitize input, just in case.
      new_scatter_factor_in_secs = 0;
    scatter_factor_ = TimeDelta::FromSeconds(new_scatter_factor_in_secs);
  }

  bool is_scatter_enabled = false;
  if (scatter_factor_.InSeconds() == 0) {
    LOG(INFO) << "Scattering disabled since scatter factor is set to 0";
  } else if (interactive) {
    LOG(INFO) << "Scattering disabled as this is an interactive update check";
  } else if (system_state_->hardware()->IsOOBEEnabled() &&
             !system_state_->hardware()->IsOOBEComplete(nullptr)) {
    LOG(INFO) << "Scattering disabled since OOBE is enabled but not complete "
                 "yet";
  } else {
    is_scatter_enabled = true;
    LOG(INFO) << "Scattering is enabled";
  }

  if (is_scatter_enabled) {
    // This means the scattering policy is turned on.
    // Now check if we need to update the waiting period. The two cases
    // in which we'd need to update the waiting period are:
    // 1. First time in process or a scheduled check after a user-initiated one.
    //    (omaha_request_params_->waiting_period will be zero in this case).
    // 2. Admin has changed the scattering policy value.
    //    (new scattering value will be different from old one in this case).
    int64_t wait_period_in_secs = 0;
    if (omaha_request_params_->waiting_period().InSeconds() == 0) {
      // First case. Check if we have a suitable value to set for
      // the waiting period.
      if (prefs_->GetInt64(kPrefsWallClockScatteringWaitPeriod,
                           &wait_period_in_secs) &&
          wait_period_in_secs > 0 &&
          wait_period_in_secs <= scatter_factor_.InSeconds()) {
        // This means:
        // 1. There's a persisted value for the waiting period available.
        // 2. And that persisted value is still valid.
        // So, in this case, we should reuse the persisted value instead of
        // generating a new random value to improve the chances of a good
        // distribution for scattering.
        omaha_request_params_->set_waiting_period(
            TimeDelta::FromSeconds(wait_period_in_secs));
        LOG(INFO) << "Using persisted wall-clock waiting period: "
                  << utils::FormatSecs(
                         omaha_request_params_->waiting_period().InSeconds());
      } else {
        // This means there's no persisted value for the waiting period
        // available or its value is invalid given the new scatter_factor value.
        // So, we should go ahead and regenerate a new value for the
        // waiting period.
        LOG(INFO) << "Persisted value not present or not valid ("
                  << utils::FormatSecs(wait_period_in_secs)
                  << ") for wall-clock waiting period.";
        GenerateNewWaitingPeriod();
      }
    } else if (scatter_factor_ != old_scatter_factor) {
      // This means there's already a waiting period value, but we detected
      // a change in the scattering policy value. So, we should regenerate the
      // waiting period to make sure it's within the bounds of the new scatter
      // factor value.
      GenerateNewWaitingPeriod();
    } else {
      // Neither the first time scattering is enabled nor the scattering value
      // changed. Nothing to do.
      LOG(INFO) << "Keeping current wall-clock waiting period: "
                << utils::FormatSecs(
                       omaha_request_params_->waiting_period().InSeconds());
    }

    // The invariant at this point is that omaha_request_params_->waiting_period
    // is non-zero no matter which path we took above.
    LOG_IF(ERROR, omaha_request_params_->waiting_period().InSeconds() == 0)
        << "Waiting Period should NOT be zero at this point!!!";

    // Since scattering is enabled, wall clock based wait will always be
    // enabled.
    omaha_request_params_->set_wall_clock_based_wait_enabled(true);

    // If we don't have any issues in accessing the file system to update
    // the update check count value, we'll turn that on as well.
    bool decrement_succeeded = DecrementUpdateCheckCount();
    omaha_request_params_->set_update_check_count_wait_enabled(
        decrement_succeeded);
  } else {
    // This means the scattering feature is turned off or disabled for
    // this particular update check. Make sure to disable
    // all the knobs and artifacts so that we don't invoke any scattering
    // related code.
    omaha_request_params_->set_wall_clock_based_wait_enabled(false);
    omaha_request_params_->set_update_check_count_wait_enabled(false);
    omaha_request_params_->set_waiting_period(TimeDelta::FromSeconds(0));
    prefs_->Delete(kPrefsWallClockScatteringWaitPeriod);
    prefs_->Delete(kPrefsUpdateCheckCount);
    // Don't delete the UpdateFirstSeenAt file as we don't want manual checks
    // that result in no-updates (e.g. due to server side throttling) to
    // cause update starvation by having the client generate a new
    // UpdateFirstSeenAt for each scheduled check that follows a manual check.
  }
}

void UpdateAttempter::GenerateNewWaitingPeriod() {
  omaha_request_params_->set_waiting_period(
      TimeDelta::FromSeconds(base::RandInt(1, scatter_factor_.InSeconds())));

  LOG(INFO) << "Generated new wall-clock waiting period: "
            << utils::FormatSecs(
                   omaha_request_params_->waiting_period().InSeconds());

  // Do a best-effort to persist this in all cases. Even if the persistence
  // fails, we'll still be able to scatter based on our in-memory value.
  // The persistence only helps in ensuring a good overall distribution
  // across multiple devices if they tend to reboot too often.
  system_state_->payload_state()->SetScatteringWaitPeriod(
      omaha_request_params_->waiting_period());
}

void UpdateAttempter::CalculateStagingParams(bool interactive) {
  bool oobe_complete = system_state_->hardware()->IsOOBEEnabled() &&
                       system_state_->hardware()->IsOOBEComplete(nullptr);
  auto device_policy = system_state_->device_policy();
  StagingCase staging_case = StagingCase::kOff;
  if (device_policy && !interactive && oobe_complete) {
    staging_wait_time_ = omaha_request_params_->waiting_period();
    staging_case = CalculateStagingCase(
        device_policy, prefs_, &staging_wait_time_, &staging_schedule_);
  }
  switch (staging_case) {
    case StagingCase::kOff:
      // Staging is off, get rid of persisted value.
      prefs_->Delete(kPrefsWallClockStagingWaitPeriod);
      // Set |staging_wait_time_| to its default value so scattering can still
      // be turned on
      staging_wait_time_ = TimeDelta();
      break;
    // Let the cases fall through since they just add, and never remove, steps
    // to turning staging on.
    case StagingCase::kNoSavedValue:
      prefs_->SetInt64(kPrefsWallClockStagingWaitPeriod,
                       staging_wait_time_.InDays());
    case StagingCase::kSetStagingFromPref:
      omaha_request_params_->set_waiting_period(staging_wait_time_);
    case StagingCase::kNoAction:
      // Staging is on, enable wallclock based wait so that its values get used.
      omaha_request_params_->set_wall_clock_based_wait_enabled(true);
      // Use UpdateCheckCount if possible to prevent devices updating all at
      // once.
      omaha_request_params_->set_update_check_count_wait_enabled(
          DecrementUpdateCheckCount());
      // Scattering should not be turned on if staging is on, delete the
      // existing scattering configuration.
      prefs_->Delete(kPrefsWallClockScatteringWaitPeriod);
      scatter_factor_ = TimeDelta();
  }
}

void UpdateAttempter::BuildUpdateActions(bool interactive) {
  CHECK(!processor_->IsRunning());
  processor_->set_delegate(this);

  // Actions:
  auto update_check_fetcher = std::make_unique<LibcurlHttpFetcher>(
      GetProxyResolver(), system_state_->hardware());
  update_check_fetcher->set_server_to_check(ServerToCheck::kUpdate);
  // Try harder to connect to the network, esp when not interactive.
  // See comment in libcurl_http_fetcher.cc.
  update_check_fetcher->set_no_network_max_retries(interactive ? 1 : 3);
  auto update_check_action = std::make_unique<OmahaRequestAction>(
      system_state_, nullptr, std::move(update_check_fetcher), false);
  auto response_handler_action =
      std::make_unique<OmahaResponseHandlerAction>(system_state_);
  auto update_boot_flags_action =
      std::make_unique<UpdateBootFlagsAction>(system_state_->boot_control());
  auto download_started_action = std::make_unique<OmahaRequestAction>(
      system_state_,
      new OmahaEvent(OmahaEvent::kTypeUpdateDownloadStarted),
      std::make_unique<LibcurlHttpFetcher>(GetProxyResolver(),
                                           system_state_->hardware()),
      false);

  LibcurlHttpFetcher* download_fetcher =
      new LibcurlHttpFetcher(GetProxyResolver(), system_state_->hardware());
  download_fetcher->set_server_to_check(ServerToCheck::kDownload);
  if (interactive)
    download_fetcher->set_max_retry_count(kDownloadMaxRetryCountInteractive);
  auto download_action =
      std::make_unique<DownloadAction>(prefs_,
                                       system_state_->boot_control(),
                                       system_state_->hardware(),
                                       system_state_,
                                       download_fetcher,  // passes ownership
                                       interactive);
  download_action->set_delegate(this);

  auto download_finished_action = std::make_unique<OmahaRequestAction>(
      system_state_,
      new OmahaEvent(OmahaEvent::kTypeUpdateDownloadFinished),
      std::make_unique<LibcurlHttpFetcher>(GetProxyResolver(),
                                           system_state_->hardware()),
      false);
  auto filesystem_verifier_action =
      std::make_unique<FilesystemVerifierAction>();
  auto update_complete_action = std::make_unique<OmahaRequestAction>(
      system_state_,
      new OmahaEvent(OmahaEvent::kTypeUpdateComplete),
      std::make_unique<LibcurlHttpFetcher>(GetProxyResolver(),
                                           system_state_->hardware()),
      false);

  auto postinstall_runner_action = std::make_unique<PostinstallRunnerAction>(
      system_state_->boot_control(), system_state_->hardware());
  postinstall_runner_action->set_delegate(this);

  // Bond them together. We have to use the leaf-types when calling
  // BondActions().
  BondActions(update_check_action.get(), response_handler_action.get());
  BondActions(response_handler_action.get(), download_action.get());
  BondActions(download_action.get(), filesystem_verifier_action.get());
  BondActions(filesystem_verifier_action.get(),
              postinstall_runner_action.get());

  processor_->EnqueueAction(std::move(update_check_action));
  processor_->EnqueueAction(std::move(response_handler_action));
  processor_->EnqueueAction(std::move(update_boot_flags_action));
  processor_->EnqueueAction(std::move(download_started_action));
  processor_->EnqueueAction(std::move(download_action));
  processor_->EnqueueAction(std::move(download_finished_action));
  processor_->EnqueueAction(std::move(filesystem_verifier_action));
  processor_->EnqueueAction(std::move(postinstall_runner_action));
  processor_->EnqueueAction(std::move(update_complete_action));
}

bool UpdateAttempter::Rollback(bool powerwash) {
  is_install_ = false;
  if (!CanRollback()) {
    return false;
  }

  // Extra check for enterprise-enrolled devices since they don't support
  // powerwash.
  if (powerwash) {
    // Enterprise-enrolled devices have an empty owner in their device policy.
    string owner;
    RefreshDevicePolicy();
    const policy::DevicePolicy* device_policy = system_state_->device_policy();
    if (device_policy && (!device_policy->GetOwner(&owner) || owner.empty())) {
      LOG(ERROR) << "Enterprise device detected. "
                 << "Cannot perform a powerwash for enterprise devices.";
      return false;
    }
  }

  processor_->set_delegate(this);

  // Initialize the default request params.
  if (!omaha_request_params_->Init("", "", true)) {
    LOG(ERROR) << "Unable to initialize Omaha request params.";
    return false;
  }

  LOG(INFO) << "Setting rollback options.";
  install_plan_.reset(new InstallPlan());
  install_plan_->target_slot = GetRollbackSlot();
  install_plan_->source_slot = system_state_->boot_control()->GetCurrentSlot();

  TEST_AND_RETURN_FALSE(
      install_plan_->LoadPartitionsFromSlots(system_state_->boot_control()));
  install_plan_->powerwash_required = powerwash;

  LOG(INFO) << "Using this install plan:";
  install_plan_->Dump();

  auto install_plan_action =
      std::make_unique<InstallPlanAction>(*install_plan_);
  auto postinstall_runner_action = std::make_unique<PostinstallRunnerAction>(
      system_state_->boot_control(), system_state_->hardware());
  postinstall_runner_action->set_delegate(this);
  BondActions(install_plan_action.get(), postinstall_runner_action.get());
  processor_->EnqueueAction(std::move(install_plan_action));
  processor_->EnqueueAction(std::move(postinstall_runner_action));

  // Update the payload state for Rollback.
  system_state_->payload_state()->Rollback();

  SetStatusAndNotify(UpdateStatus::ATTEMPTING_ROLLBACK);

  ScheduleProcessingStart();
  return true;
}

bool UpdateAttempter::CanRollback() const {
  // We can only rollback if the update_engine isn't busy and we have a valid
  // rollback partition.
  return (status_ == UpdateStatus::IDLE &&
          GetRollbackSlot() != BootControlInterface::kInvalidSlot);
}

BootControlInterface::Slot UpdateAttempter::GetRollbackSlot() const {
  LOG(INFO) << "UpdateAttempter::GetRollbackSlot";
  const unsigned int num_slots = system_state_->boot_control()->GetNumSlots();
  const BootControlInterface::Slot current_slot =
      system_state_->boot_control()->GetCurrentSlot();

  LOG(INFO) << "  Installed slots: " << num_slots;
  LOG(INFO) << "  Booted from slot: "
            << BootControlInterface::SlotName(current_slot);

  if (current_slot == BootControlInterface::kInvalidSlot || num_slots < 2) {
    LOG(INFO) << "Device is not updateable.";
    return BootControlInterface::kInvalidSlot;
  }

  vector<BootControlInterface::Slot> bootable_slots;
  for (BootControlInterface::Slot slot = 0; slot < num_slots; slot++) {
    if (slot != current_slot &&
        system_state_->boot_control()->IsSlotBootable(slot)) {
      LOG(INFO) << "Found bootable slot "
                << BootControlInterface::SlotName(slot);
      return slot;
    }
  }
  LOG(INFO) << "No other bootable slot found.";
  return BootControlInterface::kInvalidSlot;
}

bool UpdateAttempter::CheckForUpdate(const string& app_version,
                                     const string& omaha_url,
                                     UpdateAttemptFlags flags) {
  dlc_module_ids_.clear();
  is_install_ = false;
  bool interactive = !(flags & UpdateAttemptFlags::kFlagNonInteractive);

  if (interactive && status_ != UpdateStatus::IDLE) {
    // An update check is either in-progress, or an update has completed and the
    // system is in UPDATED_NEED_REBOOT.  Either way, don't do an interactive
    // update at this time
    LOG(INFO) << "Refusing to do an interactive update with an update already "
                 "in progress";
    return false;
  }

  LOG(INFO) << "Forced update check requested.";
  forced_app_version_.clear();
  forced_omaha_url_.clear();

  // Certain conditions must be met to allow setting custom version and update
  // server URLs. However, kScheduledAUTestURLRequest and kAUTestURLRequest are
  // always allowed regardless of device state.
  if (IsAnyUpdateSourceAllowed()) {
    forced_app_version_ = app_version;
    forced_omaha_url_ = omaha_url;
  }
  if (omaha_url == kScheduledAUTestURLRequest) {
    forced_omaha_url_ = constants::kOmahaDefaultAUTestURL;
    // Pretend that it's not user-initiated even though it is,
    // so as to test scattering logic, etc. which get kicked off
    // only in scheduled update checks.
    interactive = false;
  } else if (omaha_url == kAUTestURLRequest) {
    forced_omaha_url_ = constants::kOmahaDefaultAUTestURL;
  }

  if (interactive) {
    // Use the passed-in update attempt flags for this update attempt instead
    // of the previously set ones.
    current_update_attempt_flags_ = flags;
    // Note: The caching for non-interactive update checks happens in
    // OnUpdateScheduled().
  }

  if (forced_update_pending_callback_.get()) {
    if (!system_state_->dlcservice()->GetInstalled(&dlc_module_ids_)) {
      dlc_module_ids_.clear();
    }
    // Make sure that a scheduling request is made prior to calling the forced
    // update pending callback.
    ScheduleUpdates();
    forced_update_pending_callback_->Run(true, interactive);
  }

  return true;
}

bool UpdateAttempter::CheckForInstall(const vector<string>& dlc_module_ids,
                                      const string& omaha_url) {
  dlc_module_ids_ = dlc_module_ids;
  is_install_ = true;
  forced_omaha_url_.clear();

  // Certain conditions must be met to allow setting custom version and update
  // server URLs. However, kScheduledAUTestURLRequest and kAUTestURLRequest are
  // always allowed regardless of device state.
  if (IsAnyUpdateSourceAllowed()) {
    forced_omaha_url_ = omaha_url;
  }
  if (omaha_url == kScheduledAUTestURLRequest) {
    forced_omaha_url_ = constants::kOmahaDefaultAUTestURL;
  } else if (omaha_url == kAUTestURLRequest) {
    forced_omaha_url_ = constants::kOmahaDefaultAUTestURL;
  }

  if (!ScheduleUpdates()) {
    if (forced_update_pending_callback_.get()) {
      // Make sure that a scheduling request is made prior to calling the forced
      // update pending callback.
      ScheduleUpdates();
      forced_update_pending_callback_->Run(true, true);
      return true;
    }
    return false;
  }
  return true;
}

bool UpdateAttempter::RebootIfNeeded() {
#ifdef __ANDROID__
  if (status_ != UpdateStatus::UPDATED_NEED_REBOOT) {
    LOG(INFO) << "Reboot requested, but status is "
              << UpdateStatusToString(status_) << ", so not rebooting.";
    return false;
  }
#endif  // __ANDROID__

  if (system_state_->power_manager()->RequestReboot())
    return true;

  return RebootDirectly();
}

void UpdateAttempter::WriteUpdateCompletedMarker() {
  string boot_id;
  if (!utils::GetBootId(&boot_id))
    return;
  prefs_->SetString(kPrefsUpdateCompletedOnBootId, boot_id);

  int64_t value = system_state_->clock()->GetBootTime().ToInternalValue();
  prefs_->SetInt64(kPrefsUpdateCompletedBootTime, value);
}

bool UpdateAttempter::RebootDirectly() {
  vector<string> command;
  command.push_back("/sbin/shutdown");
  command.push_back("-r");
  command.push_back("now");
  LOG(INFO) << "Running \"" << base::JoinString(command, " ") << "\"";
  int rc = 0;
  Subprocess::SynchronousExec(command, &rc, nullptr);
  return rc == 0;
}

void UpdateAttempter::OnUpdateScheduled(EvalStatus status,
                                        const UpdateCheckParams& params) {
  waiting_for_scheduled_check_ = false;

  if (status == EvalStatus::kSucceeded) {
    if (!params.updates_enabled) {
      LOG(WARNING) << "Updates permanently disabled.";
      // Signal disabled status, then switch right back to idle. This is
      // necessary for ensuring that observers waiting for a signal change will
      // actually notice one on subsequent calls. Note that we don't need to
      // re-schedule a check in this case as updates are permanently disabled;
      // further (forced) checks may still initiate a scheduling call.
      SetStatusAndNotify(UpdateStatus::DISABLED);
      SetStatusAndNotify(UpdateStatus::IDLE);
      return;
    }

    LOG(INFO) << "Running " << (params.interactive ? "interactive" : "periodic")
              << " update.";

    if (!params.interactive) {
      // Cache the update attempt flags that will be used by this update attempt
      // so that they can't be changed mid-way through.
      current_update_attempt_flags_ = update_attempt_flags_;
    }

    LOG(INFO) << "Update attempt flags in use = 0x" << std::hex
              << current_update_attempt_flags_;

    Update(forced_app_version_,
           forced_omaha_url_,
           params.target_channel,
           params.target_version_prefix,
           params.rollback_allowed,
           /*obey_proxies=*/false,
           params.interactive);
    // Always clear the forced app_version and omaha_url after an update attempt
    // so the next update uses the defaults.
    forced_app_version_.clear();
    forced_omaha_url_.clear();
  } else {
    LOG(WARNING)
        << "Update check scheduling failed (possibly timed out); retrying.";
    ScheduleUpdates();
  }

  // This check ensures that future update checks will be or are already
  // scheduled. The check should never fail. A check failure means that there's
  // a bug that will most likely prevent further automatic update checks. It
  // seems better to crash in such cases and restart the update_engine daemon
  // into, hopefully, a known good state.
  CHECK(IsUpdateRunningOrScheduled());
}

void UpdateAttempter::UpdateLastCheckedTime() {
  last_checked_time_ = system_state_->clock()->GetWallclockTime().ToTimeT();
}

void UpdateAttempter::UpdateRollbackHappened() {
  DCHECK(system_state_);
  DCHECK(system_state_->payload_state());
  DCHECK(policy_provider_);
  if (system_state_->payload_state()->GetRollbackHappened() &&
      (policy_provider_->device_policy_is_loaded() ||
       policy_provider_->IsConsumerDevice())) {
    // Rollback happened, but we already went through OOBE and policy is
    // present or it's a consumer device.
    system_state_->payload_state()->SetRollbackHappened(false);
  }
}

// Delegate methods:
void UpdateAttempter::ProcessingDone(const ActionProcessor* processor,
                                     ErrorCode code) {
  LOG(INFO) << "Processing Done.";

  // Reset cpu shares back to normal.
  cpu_limiter_.StopLimiter();

  // reset the state that's only valid for a single update pass
  current_update_attempt_flags_ = UpdateAttemptFlags::kNone;

  if (forced_update_pending_callback_.get())
    // Clear prior interactive requests once the processor is done.
    forced_update_pending_callback_->Run(false, false);

  if (status_ == UpdateStatus::REPORTING_ERROR_EVENT) {
    LOG(INFO) << "Error event sent.";

    // Inform scheduler of new status;
    SetStatusAndNotify(UpdateStatus::IDLE);
    ScheduleUpdates();

    if (!fake_update_success_) {
      return;
    }
    LOG(INFO) << "Booted from FW B and tried to install new firmware, "
                 "so requesting reboot from user.";
  }

  attempt_error_code_ = utils::GetBaseErrorCode(code);

  if (code == ErrorCode::kSuccess) {
    // For install operation, we do not mark update complete since we do not
    // need reboot.
    if (!is_install_)
      WriteUpdateCompletedMarker();
    ReportTimeToUpdateAppliedMetric();

    prefs_->SetInt64(kPrefsDeltaUpdateFailures, 0);
    prefs_->SetString(kPrefsPreviousVersion,
                      omaha_request_params_->app_version());
    DeltaPerformer::ResetUpdateProgress(prefs_, false);

    system_state_->payload_state()->UpdateSucceeded();

    // Since we're done with scattering fully at this point, this is the
    // safest point delete the state files, as we're sure that the status is
    // set to reboot (which means no more updates will be applied until reboot)
    // This deletion is required for correctness as we want the next update
    // check to re-create a new random number for the update check count.
    // Similarly, we also delete the wall-clock-wait period that was persisted
    // so that we start with a new random value for the next update check
    // after reboot so that the same device is not favored or punished in any
    // way.
    prefs_->Delete(kPrefsUpdateCheckCount);
    system_state_->payload_state()->SetScatteringWaitPeriod(TimeDelta());
    system_state_->payload_state()->SetStagingWaitPeriod(TimeDelta());
    prefs_->Delete(kPrefsUpdateFirstSeenAt);

    if (is_install_) {
      LOG(INFO) << "DLC successfully installed, no reboot needed.";
      SetStatusAndNotify(UpdateStatus::IDLE);
      ScheduleUpdates();
      return;
    }

    SetStatusAndNotify(UpdateStatus::UPDATED_NEED_REBOOT);
    ScheduleUpdates();
    LOG(INFO) << "Update successfully applied, waiting to reboot.";

    // |install_plan_| is null during rollback operations, and the stats don't
    // make much sense then anyway.
    if (install_plan_) {
      // Generate an unique payload identifier.
      string target_version_uid;
      for (const auto& payload : install_plan_->payloads) {
        target_version_uid +=
            brillo::data_encoding::Base64Encode(payload.hash) + ":" +
            payload.metadata_signature + ":";
      }

      // If we just downloaded a rollback image, we should preserve this fact
      // over the following powerwash.
      if (install_plan_->is_rollback) {
        system_state_->payload_state()->SetRollbackHappened(true);
        system_state_->metrics_reporter()->ReportEnterpriseRollbackMetrics(
            /*success=*/true, install_plan_->version);
      }

      // Expect to reboot into the new version to send the proper metric during
      // next boot.
      system_state_->payload_state()->ExpectRebootInNewVersion(
          target_version_uid);
    } else {
      // If we just finished a rollback, then we expect to have no Omaha
      // response. Otherwise, it's an error.
      if (system_state_->payload_state()->GetRollbackVersion().empty()) {
        LOG(ERROR) << "Can't send metrics because there was no Omaha response";
      }
    }
    return;
  }

  if (ScheduleErrorEventAction()) {
    return;
  }
  LOG(INFO) << "No update.";
  SetStatusAndNotify(UpdateStatus::IDLE);
  ScheduleUpdates();
}

void UpdateAttempter::ProcessingStopped(const ActionProcessor* processor) {
  // Reset cpu shares back to normal.
  cpu_limiter_.StopLimiter();
  download_progress_ = 0.0;
  if (forced_update_pending_callback_.get())
    // Clear prior interactive requests once the processor is done.
    forced_update_pending_callback_->Run(false, false);
  SetStatusAndNotify(UpdateStatus::IDLE);
  ScheduleUpdates();
  error_event_.reset(nullptr);
}

// Called whenever an action has finished processing, either successfully
// or otherwise.
void UpdateAttempter::ActionCompleted(ActionProcessor* processor,
                                      AbstractAction* action,
                                      ErrorCode code) {
  // Reset download progress regardless of whether or not the download
  // action succeeded. Also, get the response code from HTTP request
  // actions (update download as well as the initial update check
  // actions).
  const string type = action->Type();
  if (type == DownloadAction::StaticType()) {
    download_progress_ = 0.0;
    DownloadAction* download_action = static_cast<DownloadAction*>(action);
    http_response_code_ = download_action->GetHTTPResponseCode();
  } else if (type == OmahaRequestAction::StaticType()) {
    OmahaRequestAction* omaha_request_action =
        static_cast<OmahaRequestAction*>(action);
    // If the request is not an event, then it's the update-check.
    if (!omaha_request_action->IsEvent()) {
      http_response_code_ = omaha_request_action->GetHTTPResponseCode();

      // Record the number of consecutive failed update checks.
      if (http_response_code_ == kHttpResponseInternalServerError ||
          http_response_code_ == kHttpResponseServiceUnavailable) {
        consecutive_failed_update_checks_++;
      } else {
        consecutive_failed_update_checks_ = 0;
      }

      const OmahaResponse& omaha_response =
          omaha_request_action->GetOutputObject();
      // Store the server-dictated poll interval, if any.
      server_dictated_poll_interval_ =
          std::max(0, omaha_response.poll_interval);

      // This update is ignored by omaha request action because update over
      // cellular connection is not allowed. Needs to ask for user's permissions
      // to update.
      if (code == ErrorCode::kOmahaUpdateIgnoredOverCellular) {
        new_version_ = omaha_response.version;
        new_payload_size_ = 0;
        for (const auto& package : omaha_response.packages) {
          new_payload_size_ += package.size;
        }
        SetStatusAndNotify(UpdateStatus::NEED_PERMISSION_TO_UPDATE);
      }
    }
  } else if (type == OmahaResponseHandlerAction::StaticType()) {
    // Depending on the returned error code, note that an update is available.
    if (code == ErrorCode::kOmahaUpdateDeferredPerPolicy ||
        code == ErrorCode::kSuccess) {
      // Note that the status will be updated to DOWNLOADING when some bytes
      // get actually downloaded from the server and the BytesReceived
      // callback is invoked. This avoids notifying the user that a download
      // has started in cases when the server and the client are unable to
      // initiate the download.
      auto omaha_response_handler_action =
          static_cast<OmahaResponseHandlerAction*>(action);
      install_plan_.reset(
          new InstallPlan(omaha_response_handler_action->install_plan()));
      UpdateLastCheckedTime();
      new_version_ = install_plan_->version;
      new_system_version_ = install_plan_->system_version;
      new_payload_size_ = 0;
      for (const auto& payload : install_plan_->payloads)
        new_payload_size_ += payload.size;
      cpu_limiter_.StartLimiter();
      SetStatusAndNotify(UpdateStatus::UPDATE_AVAILABLE);
    }
  }
  // General failure cases.
  if (code != ErrorCode::kSuccess) {
    // If the current state is at or past the download phase, count the failure
    // in case a switch to full update becomes necessary. Ignore network
    // transfer timeouts and failures.
    if (code != ErrorCode::kDownloadTransferError) {
      switch (status_) {
        case UpdateStatus::IDLE:
        case UpdateStatus::CHECKING_FOR_UPDATE:
        case UpdateStatus::UPDATE_AVAILABLE:
        case UpdateStatus::NEED_PERMISSION_TO_UPDATE:
          break;
        case UpdateStatus::DOWNLOADING:
        case UpdateStatus::VERIFYING:
        case UpdateStatus::FINALIZING:
        case UpdateStatus::UPDATED_NEED_REBOOT:
        case UpdateStatus::REPORTING_ERROR_EVENT:
        case UpdateStatus::ATTEMPTING_ROLLBACK:
        case UpdateStatus::DISABLED:
          MarkDeltaUpdateFailure();
          break;
      }
    }
    if (code != ErrorCode::kNoUpdate) {
      // On failure, schedule an error event to be sent to Omaha.
      CreatePendingErrorEvent(action, code);
    }
    return;
  }
  // Find out which action completed (successfully).
  if (type == DownloadAction::StaticType()) {
    SetStatusAndNotify(UpdateStatus::FINALIZING);
  } else if (type == FilesystemVerifierAction::StaticType()) {
    // Log the system properties before the postinst and after the file system
    // is verified. It used to be done in the postinst itself. But postinst
    // cannot do this anymore. On the other hand, these logs are frequently
    // looked at and it is preferable not to scatter them in random location in
    // the log and rather log it right before the postinst. The reason not do
    // this in the |PostinstallRunnerAction| is to prevent dependency from
    // libpayload_consumer to libupdate_engine.
    LogImageProperties();
  }
}

void UpdateAttempter::BytesReceived(uint64_t bytes_progressed,
                                    uint64_t bytes_received,
                                    uint64_t total) {
  // The PayloadState keeps track of how many bytes were actually downloaded
  // from a given URL for the URL skipping logic.
  system_state_->payload_state()->DownloadProgress(bytes_progressed);

  double progress = 0;
  if (total)
    progress = static_cast<double>(bytes_received) / static_cast<double>(total);
  if (status_ != UpdateStatus::DOWNLOADING || bytes_received == total) {
    download_progress_ = progress;
    SetStatusAndNotify(UpdateStatus::DOWNLOADING);
  } else {
    ProgressUpdate(progress);
  }
}

void UpdateAttempter::DownloadComplete() {
  system_state_->payload_state()->DownloadComplete();
}

void UpdateAttempter::ProgressUpdate(double progress) {
  // Self throttle based on progress. Also send notifications if progress is
  // too slow.
  if (progress == 1.0 ||
      progress - download_progress_ >= kBroadcastThresholdProgress ||
      TimeTicks::Now() - last_notify_time_ >=
          TimeDelta::FromSeconds(kBroadcastThresholdSeconds)) {
    download_progress_ = progress;
    BroadcastStatus();
  }
}

bool UpdateAttempter::ResetStatus() {
  LOG(INFO) << "Attempting to reset state from "
            << UpdateStatusToString(status_) << " to UpdateStatus::IDLE";

  switch (status_) {
    case UpdateStatus::IDLE:
      // no-op.
      return true;

    case UpdateStatus::UPDATED_NEED_REBOOT: {
      bool ret_value = true;
      status_ = UpdateStatus::IDLE;

      // Remove the reboot marker so that if the machine is rebooted
      // after resetting to idle state, it doesn't go back to
      // UpdateStatus::UPDATED_NEED_REBOOT state.
      ret_value = prefs_->Delete(kPrefsUpdateCompletedOnBootId) && ret_value;
      ret_value = prefs_->Delete(kPrefsUpdateCompletedBootTime) && ret_value;

      // Update the boot flags so the current slot has higher priority.
      BootControlInterface* boot_control = system_state_->boot_control();
      if (!boot_control->SetActiveBootSlot(boot_control->GetCurrentSlot()))
        ret_value = false;

      // Mark the current slot as successful again, since marking it as active
      // may reset the successful bit. We ignore the result of whether marking
      // the current slot as successful worked.
      if (!boot_control->MarkBootSuccessfulAsync(Bind([](bool successful) {})))
        ret_value = false;

      // Notify the PayloadState that the successful payload was canceled.
      system_state_->payload_state()->ResetUpdateStatus();

      // The previous version is used to report back to omaha after reboot that
      // we actually rebooted into the new version from this "prev-version". We
      // need to clear out this value now to prevent it being sent on the next
      // updatecheck request.
      ret_value = prefs_->SetString(kPrefsPreviousVersion, "") && ret_value;

      LOG(INFO) << "Reset status " << (ret_value ? "successful" : "failed");
      return ret_value;
    }

    default:
      LOG(ERROR) << "Reset not allowed in this state.";
      return false;
  }
}

bool UpdateAttempter::GetStatus(UpdateEngineStatus* out_status) {
  out_status->last_checked_time = last_checked_time_;
  out_status->status = status_;
  out_status->current_version = omaha_request_params_->app_version();
  out_status->current_system_version = omaha_request_params_->system_version();
  out_status->progress = download_progress_;
  out_status->new_size_bytes = new_payload_size_;
  out_status->new_version = new_version_;
  out_status->new_system_version = new_system_version_;
  return true;
}

void UpdateAttempter::BroadcastStatus() {
  UpdateEngineStatus broadcast_status;
  // Use common method for generating the current status.
  GetStatus(&broadcast_status);

  for (const auto& observer : service_observers_) {
    observer->SendStatusUpdate(broadcast_status);
  }
  last_notify_time_ = TimeTicks::Now();
}

uint32_t UpdateAttempter::GetErrorCodeFlags() {
  uint32_t flags = 0;

  if (!system_state_->hardware()->IsNormalBootMode())
    flags |= static_cast<uint32_t>(ErrorCode::kDevModeFlag);

  if (install_plan_ && install_plan_->is_resume)
    flags |= static_cast<uint32_t>(ErrorCode::kResumedFlag);

  if (!system_state_->hardware()->IsOfficialBuild())
    flags |= static_cast<uint32_t>(ErrorCode::kTestImageFlag);

  if (!omaha_request_params_->IsUpdateUrlOfficial()) {
    flags |= static_cast<uint32_t>(ErrorCode::kTestOmahaUrlFlag);
  }

  return flags;
}

bool UpdateAttempter::ShouldCancel(ErrorCode* cancel_reason) {
  // Check if the channel we're attempting to update to is the same as the
  // target channel currently chosen by the user.
  OmahaRequestParams* params = system_state_->request_params();
  if (params->download_channel() != params->target_channel()) {
    LOG(ERROR) << "Aborting download as target channel: "
               << params->target_channel()
               << " is different from the download channel: "
               << params->download_channel();
    *cancel_reason = ErrorCode::kUpdateCanceledByChannelChange;
    return true;
  }

  return false;
}

void UpdateAttempter::SetStatusAndNotify(UpdateStatus status) {
  status_ = status;
  BroadcastStatus();
}

void UpdateAttempter::CreatePendingErrorEvent(AbstractAction* action,
                                              ErrorCode code) {
  if (error_event_.get() || status_ == UpdateStatus::REPORTING_ERROR_EVENT) {
    // This shouldn't really happen.
    LOG(WARNING) << "There's already an existing pending error event.";
    return;
  }

  // Classify the code to generate the appropriate result so that
  // the Borgmon charts show up the results correctly.
  // Do this before calling GetErrorCodeForAction which could potentially
  // augment the bit representation of code and thus cause no matches for
  // the switch cases below.
  OmahaEvent::Result event_result;
  switch (code) {
    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
    case ErrorCode::kOmahaUpdateDeferredForBackoff:
      event_result = OmahaEvent::kResultUpdateDeferred;
      break;
    default:
      event_result = OmahaEvent::kResultError;
      break;
  }

  code = GetErrorCodeForAction(action, code);
  fake_update_success_ = code == ErrorCode::kPostinstallBootedFromFirmwareB;

  // Compute the final error code with all the bit flags to be sent to Omaha.
  code =
      static_cast<ErrorCode>(static_cast<uint32_t>(code) | GetErrorCodeFlags());
  error_event_.reset(
      new OmahaEvent(OmahaEvent::kTypeUpdateComplete, event_result, code));
}

bool UpdateAttempter::ScheduleErrorEventAction() {
  if (error_event_.get() == nullptr)
    return false;

  LOG(ERROR) << "Update failed.";
  system_state_->payload_state()->UpdateFailed(error_event_->error_code);

  // Send metrics if it was a rollback.
  if (install_plan_ && install_plan_->is_rollback) {
    system_state_->metrics_reporter()->ReportEnterpriseRollbackMetrics(
        /*success=*/false, install_plan_->version);
  }

  // Send it to Omaha.
  LOG(INFO) << "Reporting the error event";
  auto error_event_action = std::make_unique<OmahaRequestAction>(
      system_state_,
      error_event_.release(),  // Pass ownership.
      std::make_unique<LibcurlHttpFetcher>(GetProxyResolver(),
                                           system_state_->hardware()),
      false);
  processor_->EnqueueAction(std::move(error_event_action));
  SetStatusAndNotify(UpdateStatus::REPORTING_ERROR_EVENT);
  processor_->StartProcessing();
  return true;
}

void UpdateAttempter::ScheduleProcessingStart() {
  LOG(INFO) << "Scheduling an action processor start.";
  MessageLoop::current()->PostTask(
      FROM_HERE,
      Bind([](ActionProcessor* processor) { processor->StartProcessing(); },
           base::Unretained(processor_.get())));
}

void UpdateAttempter::DisableDeltaUpdateIfNeeded() {
  int64_t delta_failures;
  if (omaha_request_params_->delta_okay() &&
      prefs_->GetInt64(kPrefsDeltaUpdateFailures, &delta_failures) &&
      delta_failures >= kMaxDeltaUpdateFailures) {
    LOG(WARNING) << "Too many delta update failures, forcing full update.";
    omaha_request_params_->set_delta_okay(false);
  }
}

void UpdateAttempter::MarkDeltaUpdateFailure() {
  // Don't try to resume a failed delta update.
  DeltaPerformer::ResetUpdateProgress(prefs_, false);
  int64_t delta_failures;
  if (!prefs_->GetInt64(kPrefsDeltaUpdateFailures, &delta_failures) ||
      delta_failures < 0) {
    delta_failures = 0;
  }
  prefs_->SetInt64(kPrefsDeltaUpdateFailures, ++delta_failures);
}

void UpdateAttempter::PingOmaha() {
  if (!processor_->IsRunning()) {
    auto ping_action = std::make_unique<OmahaRequestAction>(
        system_state_,
        nullptr,
        std::make_unique<LibcurlHttpFetcher>(GetProxyResolver(),
                                             system_state_->hardware()),
        true);
    processor_->set_delegate(nullptr);
    processor_->EnqueueAction(std::move(ping_action));
    // Call StartProcessing() synchronously here to avoid any race conditions
    // caused by multiple outstanding ping Omaha requests.  If we call
    // StartProcessing() asynchronously, the device can be suspended before we
    // get a chance to callback to StartProcessing().  When the device resumes
    // (assuming the device sleeps longer than the next update check period),
    // StartProcessing() is called back and at the same time, the next update
    // check is fired which eventually invokes StartProcessing().  A crash
    // can occur because StartProcessing() checks to make sure that the
    // processor is idle which it isn't due to the two concurrent ping Omaha
    // requests.
    processor_->StartProcessing();
  } else {
    LOG(WARNING) << "Action processor running, Omaha ping suppressed.";
  }

  // Update the last check time here; it may be re-updated when an Omaha
  // response is received, but this will prevent us from repeatedly scheduling
  // checks in the case where a response is not received.
  UpdateLastCheckedTime();

  // Update the status which will schedule the next update check
  SetStatusAndNotify(UpdateStatus::UPDATED_NEED_REBOOT);
  ScheduleUpdates();
}

bool UpdateAttempter::DecrementUpdateCheckCount() {
  int64_t update_check_count_value;

  if (!prefs_->Exists(kPrefsUpdateCheckCount)) {
    // This file does not exist. This means we haven't started our update
    // check count down yet, so nothing more to do. This file will be created
    // later when we first satisfy the wall-clock-based-wait period.
    LOG(INFO) << "No existing update check count. That's normal.";
    return true;
  }

  if (prefs_->GetInt64(kPrefsUpdateCheckCount, &update_check_count_value)) {
    // Only if we're able to read a proper integer value, then go ahead
    // and decrement and write back the result in the same file, if needed.
    LOG(INFO) << "Update check count = " << update_check_count_value;

    if (update_check_count_value == 0) {
      // It could be 0, if, for some reason, the file didn't get deleted
      // when we set our status to waiting for reboot. so we just leave it
      // as is so that we can prevent another update_check wait for this client.
      LOG(INFO) << "Not decrementing update check count as it's already 0.";
      return true;
    }

    if (update_check_count_value > 0)
      update_check_count_value--;
    else
      update_check_count_value = 0;

    // Write out the new value of update_check_count_value.
    if (prefs_->SetInt64(kPrefsUpdateCheckCount, update_check_count_value)) {
      // We successfully wrote out the new value, so enable the
      // update check based wait.
      LOG(INFO) << "New update check count = " << update_check_count_value;
      return true;
    }
  }

  LOG(INFO) << "Deleting update check count state due to read/write errors.";

  // We cannot read/write to the file, so disable the update check based wait
  // so that we don't get stuck in this OS version by any chance (which could
  // happen if there's some bug that causes to read/write incorrectly).
  // Also attempt to delete the file to do our best effort to cleanup.
  prefs_->Delete(kPrefsUpdateCheckCount);
  return false;
}

void UpdateAttempter::UpdateEngineStarted() {
  // If we just booted into a new update, keep the previous OS version
  // in case we rebooted because of a crash of the old version, so we
  // can do a proper crash report with correct information.
  // This must be done before calling
  // system_state_->payload_state()->UpdateEngineStarted() since it will
  // delete SystemUpdated marker file.
  if (system_state_->system_rebooted() &&
      prefs_->Exists(kPrefsSystemUpdatedMarker)) {
    if (!prefs_->GetString(kPrefsPreviousVersion, &prev_version_)) {
      // If we fail to get the version string, make sure it stays empty.
      prev_version_.clear();
    }
  }

  system_state_->payload_state()->UpdateEngineStarted();
  StartP2PAtStartup();
}

bool UpdateAttempter::StartP2PAtStartup() {
  if (system_state_ == nullptr ||
      !system_state_->p2p_manager()->IsP2PEnabled()) {
    LOG(INFO) << "Not starting p2p at startup since it's not enabled.";
    return false;
  }

  if (system_state_->p2p_manager()->CountSharedFiles() < 1) {
    LOG(INFO) << "Not starting p2p at startup since our application "
              << "is not sharing any files.";
    return false;
  }

  return StartP2PAndPerformHousekeeping();
}

bool UpdateAttempter::StartP2PAndPerformHousekeeping() {
  if (system_state_ == nullptr)
    return false;

  if (!system_state_->p2p_manager()->IsP2PEnabled()) {
    LOG(INFO) << "Not starting p2p since it's not enabled.";
    return false;
  }

  LOG(INFO) << "Ensuring that p2p is running.";
  if (!system_state_->p2p_manager()->EnsureP2PRunning()) {
    LOG(ERROR) << "Error starting p2p.";
    return false;
  }

  LOG(INFO) << "Performing p2p housekeeping.";
  if (!system_state_->p2p_manager()->PerformHousekeeping()) {
    LOG(ERROR) << "Error performing housekeeping for p2p.";
    return false;
  }

  LOG(INFO) << "Done performing p2p housekeeping.";
  return true;
}

bool UpdateAttempter::GetBootTimeAtUpdate(Time* out_boot_time) {
  // In case of an update_engine restart without a reboot, we stored the boot_id
  // when the update was completed by setting a pref, so we can check whether
  // the last update was on this boot or a previous one.
  string boot_id;
  TEST_AND_RETURN_FALSE(utils::GetBootId(&boot_id));

  string update_completed_on_boot_id;
  if (!prefs_->Exists(kPrefsUpdateCompletedOnBootId) ||
      !prefs_->GetString(kPrefsUpdateCompletedOnBootId,
                         &update_completed_on_boot_id) ||
      update_completed_on_boot_id != boot_id)
    return false;

  // Short-circuit avoiding the read in case out_boot_time is nullptr.
  if (out_boot_time) {
    int64_t boot_time = 0;
    // Since the kPrefsUpdateCompletedOnBootId was correctly set, this pref
    // should not fail.
    TEST_AND_RETURN_FALSE(
        prefs_->GetInt64(kPrefsUpdateCompletedBootTime, &boot_time));
    *out_boot_time = Time::FromInternalValue(boot_time);
  }
  return true;
}

bool UpdateAttempter::IsUpdateRunningOrScheduled() {
  return ((status_ != UpdateStatus::IDLE &&
           status_ != UpdateStatus::UPDATED_NEED_REBOOT) ||
          waiting_for_scheduled_check_);
}

bool UpdateAttempter::IsAnyUpdateSourceAllowed() const {
  // We allow updates from any source if either of these are true:
  //  * The device is running an unofficial (dev/test) image.
  //  * The debugd dev features are accessible (i.e. in devmode with no owner).
  // This protects users running a base image, while still allowing a specific
  // window (gated by the debug dev features) where `cros flash` is usable.
  if (!system_state_->hardware()->IsOfficialBuild()) {
    LOG(INFO) << "Non-official build; allowing any update source.";
    return true;
  }

  if (system_state_->hardware()->AreDevFeaturesEnabled()) {
    LOG(INFO) << "Developer features enabled; allowing custom update sources.";
    return true;
  }

  LOG(INFO)
      << "Developer features disabled; disallowing custom update sources.";
  return false;
}

void UpdateAttempter::ReportTimeToUpdateAppliedMetric() {
  const policy::DevicePolicy* device_policy = system_state_->device_policy();
  if (device_policy && device_policy->IsEnterpriseEnrolled()) {
    vector<policy::DevicePolicy::WeeklyTimeInterval> parsed_intervals;
    bool has_time_restrictions =
        device_policy->GetDisallowedTimeIntervals(&parsed_intervals);

    int64_t update_first_seen_at_int;
    if (system_state_->prefs()->Exists(kPrefsUpdateFirstSeenAt)) {
      if (system_state_->prefs()->GetInt64(kPrefsUpdateFirstSeenAt,
                                           &update_first_seen_at_int)) {
        TimeDelta update_delay =
            system_state_->clock()->GetWallclockTime() -
            Time::FromInternalValue(update_first_seen_at_int);
        system_state_->metrics_reporter()
            ->ReportEnterpriseUpdateSeenToDownloadDays(has_time_restrictions,
                                                       update_delay.InDays());
      }
    }
  }
}

}  // namespace chromeos_update_engine
