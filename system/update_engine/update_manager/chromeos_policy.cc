//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/chromeos_policy.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/time/time.h>

#include "update_engine/common/error_code.h"
#include "update_engine/common/error_code_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/update_manager/device_policy_provider.h"
#include "update_engine/update_manager/enough_slots_ab_updates_policy_impl.h"
#include "update_engine/update_manager/enterprise_device_policy_impl.h"
#include "update_engine/update_manager/interactive_update_policy_impl.h"
#include "update_engine/update_manager/official_build_check_policy_impl.h"
#include "update_engine/update_manager/out_of_box_experience_policy_impl.h"
#include "update_engine/update_manager/policy_utils.h"
#include "update_engine/update_manager/shill_provider.h"
#include "update_engine/update_manager/update_time_restrictions_policy_impl.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ConnectionTethering;
using chromeos_update_engine::ConnectionType;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;
using std::get;
using std::min;
using std::set;
using std::string;
using std::vector;

namespace {

// Examines |err_code| and decides whether the URL index needs to be advanced,
// the error count for the URL incremented, or none of the above. In the first
// case, returns true; in the second case, increments |*url_num_error_p| and
// returns false; otherwise just returns false.
//
// TODO(garnold) Adapted from PayloadState::UpdateFailed() (to be retired).
bool HandleErrorCode(ErrorCode err_code, int* url_num_error_p) {
  err_code = chromeos_update_engine::utils::GetBaseErrorCode(err_code);
  switch (err_code) {
    // Errors which are good indicators of a problem with a particular URL or
    // the protocol used in the URL or entities in the communication channel
    // (e.g. proxies). We should try the next available URL in the next update
    // check to quickly recover from these errors.
    case ErrorCode::kPayloadHashMismatchError:
    case ErrorCode::kPayloadSizeMismatchError:
    case ErrorCode::kDownloadPayloadVerificationError:
    case ErrorCode::kDownloadPayloadPubKeyVerificationError:
    case ErrorCode::kSignedDeltaPayloadExpectedError:
    case ErrorCode::kDownloadInvalidMetadataMagicString:
    case ErrorCode::kDownloadSignatureMissingInManifest:
    case ErrorCode::kDownloadManifestParseError:
    case ErrorCode::kDownloadMetadataSignatureError:
    case ErrorCode::kDownloadMetadataSignatureVerificationError:
    case ErrorCode::kDownloadMetadataSignatureMismatch:
    case ErrorCode::kDownloadOperationHashVerificationError:
    case ErrorCode::kDownloadOperationExecutionError:
    case ErrorCode::kDownloadOperationHashMismatch:
    case ErrorCode::kDownloadInvalidMetadataSize:
    case ErrorCode::kDownloadInvalidMetadataSignature:
    case ErrorCode::kDownloadOperationHashMissingError:
    case ErrorCode::kDownloadMetadataSignatureMissingError:
    case ErrorCode::kPayloadMismatchedType:
    case ErrorCode::kUnsupportedMajorPayloadVersion:
    case ErrorCode::kUnsupportedMinorPayloadVersion:
    case ErrorCode::kPayloadTimestampError:
    case ErrorCode::kVerityCalculationError:
      LOG(INFO) << "Advancing download URL due to error "
                << chromeos_update_engine::utils::ErrorCodeToString(err_code)
                << " (" << static_cast<int>(err_code) << ")";
      return true;

    // Errors which seem to be just transient network/communication related
    // failures and do not indicate any inherent problem with the URL itself.
    // So, we should keep the current URL but just increment the
    // failure count to give it more chances. This way, while we maximize our
    // chances of downloading from the URLs that appear earlier in the response
    // (because download from a local server URL that appears earlier in a
    // response is preferable than downloading from the next URL which could be
    // an Internet URL and thus could be more expensive).
    case ErrorCode::kError:
    case ErrorCode::kDownloadTransferError:
    case ErrorCode::kDownloadWriteError:
    case ErrorCode::kDownloadStateInitializationError:
    case ErrorCode::kOmahaErrorInHTTPResponse:  // Aggregate for HTTP errors.
      LOG(INFO) << "Incrementing URL failure count due to error "
                << chromeos_update_engine::utils::ErrorCodeToString(err_code)
                << " (" << static_cast<int>(err_code) << ")";
      *url_num_error_p += 1;
      return false;

    // Errors which are not specific to a URL and hence shouldn't result in
    // the URL being penalized. This can happen in two cases:
    // 1. We haven't started downloading anything: These errors don't cost us
    // anything in terms of actual payload bytes, so we should just do the
    // regular retries at the next update check.
    // 2. We have successfully downloaded the payload: In this case, the
    // payload attempt number would have been incremented and would take care
    // of the back-off at the next update check.
    // In either case, there's no need to update URL index or failure count.
    case ErrorCode::kOmahaRequestError:
    case ErrorCode::kOmahaResponseHandlerError:
    case ErrorCode::kPostinstallRunnerError:
    case ErrorCode::kFilesystemCopierError:
    case ErrorCode::kInstallDeviceOpenError:
    case ErrorCode::kKernelDeviceOpenError:
    case ErrorCode::kDownloadNewPartitionInfoError:
    case ErrorCode::kNewRootfsVerificationError:
    case ErrorCode::kNewKernelVerificationError:
    case ErrorCode::kPostinstallBootedFromFirmwareB:
    case ErrorCode::kPostinstallFirmwareRONotUpdatable:
    case ErrorCode::kOmahaRequestEmptyResponseError:
    case ErrorCode::kOmahaRequestXMLParseError:
    case ErrorCode::kOmahaResponseInvalid:
    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
    case ErrorCode::kNonCriticalUpdateInOOBE:
    case ErrorCode::kOmahaUpdateDeferredForBackoff:
    case ErrorCode::kPostinstallPowerwashError:
    case ErrorCode::kUpdateCanceledByChannelChange:
    case ErrorCode::kOmahaRequestXMLHasEntityDecl:
    case ErrorCode::kFilesystemVerifierError:
    case ErrorCode::kUserCanceled:
    case ErrorCode::kOmahaUpdateIgnoredOverCellular:
    case ErrorCode::kUpdatedButNotActive:
    case ErrorCode::kNoUpdate:
    case ErrorCode::kRollbackNotPossible:
    case ErrorCode::kFirstActiveOmahaPingSentPersistenceError:
      LOG(INFO) << "Not changing URL index or failure count due to error "
                << chromeos_update_engine::utils::ErrorCodeToString(err_code)
                << " (" << static_cast<int>(err_code) << ")";
      return false;

    case ErrorCode::kSuccess:                       // success code
    case ErrorCode::kUmaReportedMax:                // not an error code
    case ErrorCode::kOmahaRequestHTTPResponseBase:  // aggregated already
    case ErrorCode::kDevModeFlag:                   // not an error code
    case ErrorCode::kResumedFlag:                   // not an error code
    case ErrorCode::kTestImageFlag:                 // not an error code
    case ErrorCode::kTestOmahaUrlFlag:              // not an error code
    case ErrorCode::kSpecialFlags:                  // not an error code
      // These shouldn't happen. Enumerating these  explicitly here so that we
      // can let the compiler warn about new error codes that are added to
      // action_processor.h but not added here.
      LOG(WARNING) << "Unexpected error "
                   << chromeos_update_engine::utils::ErrorCodeToString(err_code)
                   << " (" << static_cast<int>(err_code) << ")";
      // Note: Not adding a default here so as to let the compiler warn us of
      // any new enums that were added in the .h but not listed in this switch.
  }
  return false;
}

// Checks whether |url| can be used under given download restrictions.
bool IsUrlUsable(const string& url, bool http_allowed) {
  return http_allowed ||
         !base::StartsWith(
             url, "http://", base::CompareCase::INSENSITIVE_ASCII);
}

}  // namespace

namespace chromeos_update_manager {

const NextUpdateCheckPolicyConstants
    ChromeOSPolicy::kNextUpdateCheckPolicyConstants = {
        .timeout_initial_interval = 7 * 60,
        .timeout_periodic_interval = 45 * 60,
        .timeout_max_backoff_interval = 4 * 60 * 60,
        .timeout_regular_fuzz = 10 * 60,
        .attempt_backoff_max_interval_in_days = 16,
        .attempt_backoff_fuzz_in_hours = 12,
};

const int ChromeOSPolicy::kMaxP2PAttempts = 10;
const int ChromeOSPolicy::kMaxP2PAttemptsPeriodInSeconds = 5 * 24 * 60 * 60;

EvalStatus ChromeOSPolicy::UpdateCheckAllowed(EvaluationContext* ec,
                                              State* state,
                                              string* error,
                                              UpdateCheckParams* result) const {
  // Set the default return values.
  result->updates_enabled = true;
  result->target_channel.clear();
  result->target_version_prefix.clear();
  result->rollback_allowed = false;
  result->rollback_allowed_milestones = -1;
  result->interactive = false;

  EnoughSlotsAbUpdatesPolicyImpl enough_slots_ab_updates_policy;
  EnterpriseDevicePolicyImpl enterprise_device_policy;
  OnlyUpdateOfficialBuildsPolicyImpl only_update_official_builds_policy;
  InteractiveUpdatePolicyImpl interactive_update_policy;
  OobePolicyImpl oobe_policy;
  NextUpdateCheckTimePolicyImpl next_update_check_time_policy(
      kNextUpdateCheckPolicyConstants);

  vector<Policy const*> policies_to_consult = {
      // Do not perform any updates if there are not enough slots to do A/B
      // updates.
      &enough_slots_ab_updates_policy,

      // Check to see if Enterprise-managed (has DevicePolicy) and/or
      // Kiosk-mode.  If so, then defer to those settings.
      &enterprise_device_policy,

      // Check to see if an interactive update was requested.
      &interactive_update_policy,

      // Unofficial builds should not perform periodic update checks.
      &only_update_official_builds_policy,

      // If OOBE is enabled, wait until it is completed.
      &oobe_policy,

      // Ensure that periodic update checks are timed properly.
      &next_update_check_time_policy,
  };

  // Now that the list of policy implementations, and the order to consult them,
  // has been setup, consult the policies. If none of the policies make a
  // definitive decisions about whether or not to check for updates, then allow
  // the update check to happen.
  EvalStatus status = ConsultPolicies(policies_to_consult,
                                      &Policy::UpdateCheckAllowed,
                                      ec,
                                      state,
                                      error,
                                      result);
  if (EvalStatus::kContinue != status) {
    return status;
  } else {
    // It is time to check for an update.
    LOG(INFO) << "Allowing update check.";
    return EvalStatus::kSucceeded;
  }
}

EvalStatus ChromeOSPolicy::UpdateCanBeApplied(EvaluationContext* ec,
                                              State* state,
                                              std::string* error,
                                              ErrorCode* result,
                                              InstallPlan* install_plan) const {
  UpdateTimeRestrictionsPolicyImpl update_time_restrictions_policy;
  InteractiveUpdatePolicyImpl interactive_update_policy;

  vector<Policy const*> policies_to_consult = {
      // Check to see if an interactive update has been requested.
      &interactive_update_policy,

      // Do not apply or download an update if we are inside one of the
      // restricted times.
      &update_time_restrictions_policy,
  };

  EvalStatus status = ConsultPolicies(policies_to_consult,
                                      &Policy::UpdateCanBeApplied,
                                      ec,
                                      state,
                                      error,
                                      result,
                                      install_plan);
  if (EvalStatus::kContinue != status) {
    return status;
  } else {
    // The update can proceed.
    LOG(INFO) << "Allowing update to be applied.";
    *result = ErrorCode::kSuccess;
    return EvalStatus::kSucceeded;
  }
}

EvalStatus ChromeOSPolicy::UpdateCanStart(
    EvaluationContext* ec,
    State* state,
    string* error,
    UpdateDownloadParams* result,
    const UpdateState update_state) const {
  // Set the default return values. Note that we set persisted values (backoff,
  // scattering) to the same values presented in the update state. The reason is
  // that preemptive returns, such as the case where an update check is due,
  // should not clear off the said values; rather, it is the deliberate
  // inference of new values that should cause them to be reset.
  result->update_can_start = false;
  result->cannot_start_reason = UpdateCannotStartReason::kUndefined;
  result->download_url_idx = -1;
  result->download_url_allowed = true;
  result->download_url_num_errors = 0;
  result->p2p_downloading_allowed = false;
  result->p2p_sharing_allowed = false;
  result->do_increment_failures = false;
  result->backoff_expiry = update_state.backoff_expiry;
  result->scatter_wait_period = update_state.scatter_wait_period;
  result->scatter_check_threshold = update_state.scatter_check_threshold;

  // Make sure that we're not due for an update check.
  UpdateCheckParams check_result;
  EvalStatus check_status = UpdateCheckAllowed(ec, state, error, &check_result);
  if (check_status == EvalStatus::kFailed)
    return EvalStatus::kFailed;
  bool is_check_due = (check_status == EvalStatus::kSucceeded &&
                       check_result.updates_enabled == true);

  // Check whether backoff applies, and if not then which URL can be used for
  // downloading. These require scanning the download error log, and so they are
  // done together.
  UpdateBackoffAndDownloadUrlResult backoff_url_result;
  EvalStatus backoff_url_status = UpdateBackoffAndDownloadUrl(
      ec, state, error, &backoff_url_result, update_state);
  if (backoff_url_status == EvalStatus::kFailed)
    return EvalStatus::kFailed;
  result->download_url_idx = backoff_url_result.url_idx;
  result->download_url_num_errors = backoff_url_result.url_num_errors;
  result->do_increment_failures = backoff_url_result.do_increment_failures;
  result->backoff_expiry = backoff_url_result.backoff_expiry;
  bool is_backoff_active =
      (backoff_url_status == EvalStatus::kAskMeAgainLater) ||
      !backoff_url_result.backoff_expiry.is_null();

  DevicePolicyProvider* const dp_provider = state->device_policy_provider();
  bool is_scattering_active = false;
  EvalStatus scattering_status = EvalStatus::kSucceeded;

  const bool* device_policy_is_loaded_p =
      ec->GetValue(dp_provider->var_device_policy_is_loaded());
  if (device_policy_is_loaded_p && *device_policy_is_loaded_p) {
    // Check whether scattering applies to this update attempt. We should not be
    // scattering if this is an interactive update check, or if OOBE is enabled
    // but not completed.
    //
    // Note: current code further suppresses scattering if a "deadline"
    // attribute is found in the Omaha response. However, it appears that the
    // presence of this attribute is merely indicative of an OOBE update, during
    // which we suppress scattering anyway.
    bool is_scattering_applicable = false;
    result->scatter_wait_period = kZeroInterval;
    result->scatter_check_threshold = 0;
    if (!update_state.interactive) {
      const bool* is_oobe_enabled_p =
          ec->GetValue(state->config_provider()->var_is_oobe_enabled());
      if (is_oobe_enabled_p && !(*is_oobe_enabled_p)) {
        is_scattering_applicable = true;
      } else {
        const bool* is_oobe_complete_p =
            ec->GetValue(state->system_provider()->var_is_oobe_complete());
        is_scattering_applicable = (is_oobe_complete_p && *is_oobe_complete_p);
      }
    }

    // Compute scattering values.
    if (is_scattering_applicable) {
      UpdateScatteringResult scatter_result;
      scattering_status =
          UpdateScattering(ec, state, error, &scatter_result, update_state);
      if (scattering_status == EvalStatus::kFailed) {
        return EvalStatus::kFailed;
      } else {
        result->scatter_wait_period = scatter_result.wait_period;
        result->scatter_check_threshold = scatter_result.check_threshold;
        if (scattering_status == EvalStatus::kAskMeAgainLater ||
            scatter_result.is_scattering)
          is_scattering_active = true;
      }
    }
  }

  // Find out whether P2P is globally enabled.
  bool p2p_enabled;
  EvalStatus p2p_enabled_status = P2PEnabled(ec, state, error, &p2p_enabled);
  if (p2p_enabled_status != EvalStatus::kSucceeded)
    return EvalStatus::kFailed;

  // Is P2P is enabled, consider allowing it for downloading and/or sharing.
  if (p2p_enabled) {
    // Sharing via P2P is allowed if not disabled by Omaha.
    if (update_state.p2p_sharing_disabled) {
      LOG(INFO) << "Blocked P2P sharing because it is disabled by Omaha.";
    } else {
      result->p2p_sharing_allowed = true;
    }

    // Downloading via P2P is allowed if not disabled by Omaha, an update is not
    // interactive, and other limits haven't been reached.
    if (update_state.p2p_downloading_disabled) {
      LOG(INFO) << "Blocked P2P downloading because it is disabled by Omaha.";
    } else if (update_state.interactive) {
      LOG(INFO) << "Blocked P2P downloading because update is interactive.";
    } else if (update_state.p2p_num_attempts >= kMaxP2PAttempts) {
      LOG(INFO) << "Blocked P2P downloading as it was attempted too many "
                   "times.";
    } else if (!update_state.p2p_first_attempted.is_null() &&
               ec->IsWallclockTimeGreaterThan(
                   update_state.p2p_first_attempted +
                   TimeDelta::FromSeconds(kMaxP2PAttemptsPeriodInSeconds))) {
      LOG(INFO) << "Blocked P2P downloading as its usage timespan exceeds "
                   "limit.";
    } else {
      // P2P download is allowed; if backoff or scattering are active, be sure
      // to suppress them, yet prevent any download URL from being used.
      result->p2p_downloading_allowed = true;
      if (is_backoff_active || is_scattering_active) {
        is_backoff_active = is_scattering_active = false;
        result->download_url_allowed = false;
      }
    }
  }

  // Check for various deterrents.
  if (is_check_due) {
    result->cannot_start_reason = UpdateCannotStartReason::kCheckDue;
    return EvalStatus::kSucceeded;
  }
  if (is_backoff_active) {
    result->cannot_start_reason = UpdateCannotStartReason::kBackoff;
    return backoff_url_status;
  }
  if (is_scattering_active) {
    result->cannot_start_reason = UpdateCannotStartReason::kScattering;
    return scattering_status;
  }
  if (result->download_url_idx < 0 && !result->p2p_downloading_allowed) {
    result->cannot_start_reason = UpdateCannotStartReason::kCannotDownload;
    return EvalStatus::kSucceeded;
  }

  // Update is good to go.
  result->update_can_start = true;
  return EvalStatus::kSucceeded;
}

// TODO(garnold) Logic in this method is based on
// ConnectionManager::IsUpdateAllowedOver(); be sure to deprecate the latter.
//
// TODO(garnold) The current logic generally treats the list of allowed
// connections coming from the device policy as a whitelist, meaning that it
// can only be used for enabling connections, but not disable them. Further,
// certain connection types (like Bluetooth) cannot be enabled even by policy.
// In effect, the only thing that device policy can change is to enable
// updates over a cellular network (disabled by default). We may want to
// revisit this semantics, allowing greater flexibility in defining specific
// permissions over all types of networks.
EvalStatus ChromeOSPolicy::UpdateDownloadAllowed(EvaluationContext* ec,
                                                 State* state,
                                                 string* error,
                                                 bool* result) const {
  // Get the current connection type.
  ShillProvider* const shill_provider = state->shill_provider();
  const ConnectionType* conn_type_p =
      ec->GetValue(shill_provider->var_conn_type());
  POLICY_CHECK_VALUE_AND_FAIL(conn_type_p, error);
  ConnectionType conn_type = *conn_type_p;

  // If we're tethering, treat it as a cellular connection.
  if (conn_type != ConnectionType::kCellular) {
    const ConnectionTethering* conn_tethering_p =
        ec->GetValue(shill_provider->var_conn_tethering());
    POLICY_CHECK_VALUE_AND_FAIL(conn_tethering_p, error);
    if (*conn_tethering_p == ConnectionTethering::kConfirmed)
      conn_type = ConnectionType::kCellular;
  }

  // By default, we allow updates for all connection types, with exceptions as
  // noted below. This also determines whether a device policy can override the
  // default.
  *result = true;
  bool device_policy_can_override = false;
  switch (conn_type) {
    case ConnectionType::kBluetooth:
      *result = false;
      break;

    case ConnectionType::kCellular:
      *result = false;
      device_policy_can_override = true;
      break;

    case ConnectionType::kUnknown:
      if (error)
        *error = "Unknown connection type";
      return EvalStatus::kFailed;

    default:
      break;  // Nothing to do.
  }

  // If update is allowed, we're done.
  if (*result)
    return EvalStatus::kSucceeded;

  // Check whether the device policy specifically allows this connection.
  if (device_policy_can_override) {
    DevicePolicyProvider* const dp_provider = state->device_policy_provider();
    const bool* device_policy_is_loaded_p =
        ec->GetValue(dp_provider->var_device_policy_is_loaded());
    if (device_policy_is_loaded_p && *device_policy_is_loaded_p) {
      const set<ConnectionType>* allowed_conn_types_p =
          ec->GetValue(dp_provider->var_allowed_connection_types_for_update());
      if (allowed_conn_types_p) {
        if (allowed_conn_types_p->count(conn_type)) {
          *result = true;
          return EvalStatus::kSucceeded;
        }
      } else if (conn_type == ConnectionType::kCellular) {
        // Local user settings can allow updates over cellular iff a policy was
        // loaded but no allowed connections were specified in it.
        const bool* update_over_cellular_allowed_p =
            ec->GetValue(state->updater_provider()->var_cellular_enabled());
        if (update_over_cellular_allowed_p && *update_over_cellular_allowed_p)
          *result = true;
      }
    }
  }

  return (*result ? EvalStatus::kSucceeded : EvalStatus::kAskMeAgainLater);
}

EvalStatus ChromeOSPolicy::P2PEnabled(EvaluationContext* ec,
                                      State* state,
                                      string* error,
                                      bool* result) const {
  bool enabled = false;

  // Determine whether use of P2P is allowed by policy. Even if P2P is not
  // explicitly allowed, we allow it if the device is enterprise enrolled (that
  // is, missing or empty owner string).
  DevicePolicyProvider* const dp_provider = state->device_policy_provider();
  const bool* device_policy_is_loaded_p =
      ec->GetValue(dp_provider->var_device_policy_is_loaded());
  if (device_policy_is_loaded_p && *device_policy_is_loaded_p) {
    const bool* policy_au_p2p_enabled_p =
        ec->GetValue(dp_provider->var_au_p2p_enabled());
    if (policy_au_p2p_enabled_p) {
      enabled = *policy_au_p2p_enabled_p;
    } else {
      const string* policy_owner_p = ec->GetValue(dp_provider->var_owner());
      if (!policy_owner_p || policy_owner_p->empty())
        enabled = true;
    }
  }

  // Enable P2P, if so mandated by the updater configuration. This is additive
  // to whether or not P2P is enabled by device policy.
  if (!enabled) {
    const bool* updater_p2p_enabled_p =
        ec->GetValue(state->updater_provider()->var_p2p_enabled());
    enabled = updater_p2p_enabled_p && *updater_p2p_enabled_p;
  }

  *result = enabled;
  return EvalStatus::kSucceeded;
}

EvalStatus ChromeOSPolicy::P2PEnabledChanged(EvaluationContext* ec,
                                             State* state,
                                             string* error,
                                             bool* result,
                                             bool prev_result) const {
  EvalStatus status = P2PEnabled(ec, state, error, result);
  if (status == EvalStatus::kSucceeded && *result == prev_result)
    return EvalStatus::kAskMeAgainLater;
  return status;
}

EvalStatus ChromeOSPolicy::UpdateBackoffAndDownloadUrl(
    EvaluationContext* ec,
    State* state,
    string* error,
    UpdateBackoffAndDownloadUrlResult* result,
    const UpdateState& update_state) const {
  // Sanity checks.
  DCHECK_GE(update_state.download_errors_max, 0);

  // Set default result values.
  result->do_increment_failures = false;
  result->backoff_expiry = update_state.backoff_expiry;
  result->url_idx = -1;
  result->url_num_errors = 0;

  const bool* is_official_build_p =
      ec->GetValue(state->system_provider()->var_is_official_build());
  bool is_official_build = (is_official_build_p ? *is_official_build_p : true);

  // Check whether backoff is enabled.
  bool may_backoff = false;
  if (update_state.is_backoff_disabled) {
    LOG(INFO) << "Backoff disabled by Omaha.";
  } else if (update_state.interactive) {
    LOG(INFO) << "No backoff for interactive updates.";
  } else if (update_state.is_delta_payload) {
    LOG(INFO) << "No backoff for delta payloads.";
  } else if (!is_official_build) {
    LOG(INFO) << "No backoff for unofficial builds.";
  } else {
    may_backoff = true;
  }

  // If previous backoff still in effect, block.
  if (may_backoff && !update_state.backoff_expiry.is_null() &&
      !ec->IsWallclockTimeGreaterThan(update_state.backoff_expiry)) {
    LOG(INFO) << "Previous backoff has not expired, waiting.";
    return EvalStatus::kAskMeAgainLater;
  }

  // Determine whether HTTP downloads are forbidden by policy. This only
  // applies to official system builds; otherwise, HTTP is always enabled.
  bool http_allowed = true;
  if (is_official_build) {
    DevicePolicyProvider* const dp_provider = state->device_policy_provider();
    const bool* device_policy_is_loaded_p =
        ec->GetValue(dp_provider->var_device_policy_is_loaded());
    if (device_policy_is_loaded_p && *device_policy_is_loaded_p) {
      const bool* policy_http_downloads_enabled_p =
          ec->GetValue(dp_provider->var_http_downloads_enabled());
      http_allowed = (!policy_http_downloads_enabled_p ||
                      *policy_http_downloads_enabled_p);
    }
  }

  int url_idx = update_state.last_download_url_idx;
  if (url_idx < 0)
    url_idx = -1;
  bool do_advance_url = false;
  bool is_failure_occurred = false;
  Time err_time;

  // Scan the relevant part of the download error log, tracking which URLs are
  // being used, and accounting the number of errors for each URL. Note that
  // this process may not traverse all errors provided, as it may decide to bail
  // out midway depending on the particular errors exhibited, the number of
  // failures allowed, etc. When this ends, |url_idx| will point to the last URL
  // used (-1 if starting fresh), |do_advance_url| will determine whether the
  // URL needs to be advanced, and |err_time| the point in time when the last
  // reported error occurred.  Additionally, if the error log indicates that an
  // update attempt has failed (abnormal), then |is_failure_occurred| will be
  // set to true.
  const int num_urls = update_state.download_urls.size();
  int prev_url_idx = -1;
  int url_num_errors = update_state.last_download_url_num_errors;
  Time prev_err_time;
  bool is_first = true;
  for (const auto& err_tuple : update_state.download_errors) {
    // Do some sanity checks.
    int used_url_idx = get<0>(err_tuple);
    if (is_first && url_idx >= 0 && used_url_idx != url_idx) {
      LOG(WARNING) << "First URL in error log (" << used_url_idx
                   << ") not as expected (" << url_idx << ")";
    }
    is_first = false;
    url_idx = used_url_idx;
    if (url_idx < 0 || url_idx >= num_urls) {
      LOG(ERROR) << "Download error log contains an invalid URL index ("
                 << url_idx << ")";
      return EvalStatus::kFailed;
    }
    err_time = get<2>(err_tuple);
    if (!(prev_err_time.is_null() || err_time >= prev_err_time)) {
      // TODO(garnold) Monotonicity cannot really be assumed when dealing with
      // wallclock-based timestamps. However, we're making a simplifying
      // assumption so as to keep the policy implementation straightforward, for
      // now. In general, we should convert all timestamp handling in the
      // UpdateManager to use monotonic time (instead of wallclock), including
      // the computation of various expiration times (backoff, scattering, etc).
      // The client will do whatever conversions necessary when
      // persisting/retrieving these values across reboots. See chromium:408794.
      LOG(ERROR) << "Download error timestamps not monotonically increasing.";
      return EvalStatus::kFailed;
    }
    prev_err_time = err_time;

    // Ignore errors that happened before the last known failed attempt.
    if (!update_state.failures_last_updated.is_null() &&
        err_time <= update_state.failures_last_updated)
      continue;

    if (prev_url_idx >= 0) {
      if (url_idx < prev_url_idx) {
        LOG(ERROR) << "The URLs in the download error log have wrapped around ("
                   << prev_url_idx << "->" << url_idx
                   << "). This should not have happened and means that there's "
                      "a bug. To be conservative, we record a failed attempt "
                      "(invalidating the rest of the error log) and resume "
                      "download from the first usable URL.";
        url_idx = -1;
        is_failure_occurred = true;
        break;
      }

      if (url_idx > prev_url_idx) {
        url_num_errors = 0;
        do_advance_url = false;
      }
    }

    if (HandleErrorCode(get<1>(err_tuple), &url_num_errors) ||
        url_num_errors > update_state.download_errors_max)
      do_advance_url = true;

    prev_url_idx = url_idx;
  }

  // If required, advance to the next usable URL. If the URLs wraparound, we
  // mark an update attempt failure. Also be sure to set the download error
  // count to zero.
  if (url_idx < 0 || do_advance_url) {
    url_num_errors = 0;
    int start_url_idx = -1;
    do {
      if (++url_idx == num_urls) {
        url_idx = 0;
        // We only mark failure if an actual advancing of a URL was required.
        if (do_advance_url)
          is_failure_occurred = true;
      }

      if (start_url_idx < 0)
        start_url_idx = url_idx;
      else if (url_idx == start_url_idx)
        url_idx = -1;  // No usable URL.
    } while (url_idx >= 0 &&
             !IsUrlUsable(update_state.download_urls[url_idx], http_allowed));
  }

  // If we have a download URL but a failure was observed, compute a new backoff
  // expiry (if allowed). The backoff period is generally 2 ^ (num_failures - 1)
  // days, bounded by the size of int and kAttemptBackoffMaxIntervalInDays, and
  // fuzzed by kAttemptBackoffFuzzInHours hours. Backoff expiry is computed from
  // the latest recorded time of error.
  Time backoff_expiry;
  if (url_idx >= 0 && is_failure_occurred && may_backoff) {
    CHECK(!err_time.is_null())
        << "We must have an error timestamp if a failure occurred!";
    const uint64_t* seed = ec->GetValue(state->random_provider()->var_seed());
    POLICY_CHECK_VALUE_AND_FAIL(seed, error);
    PRNG prng(*seed);
    int exp =
        min(update_state.num_failures, static_cast<int>(sizeof(int)) * 8 - 2);
    TimeDelta backoff_interval = TimeDelta::FromDays(min(
        1 << exp,
        kNextUpdateCheckPolicyConstants.attempt_backoff_max_interval_in_days));
    TimeDelta backoff_fuzz = TimeDelta::FromHours(
        kNextUpdateCheckPolicyConstants.attempt_backoff_fuzz_in_hours);
    TimeDelta wait_period = NextUpdateCheckTimePolicyImpl::FuzzedInterval(
        &prng, backoff_interval.InSeconds(), backoff_fuzz.InSeconds());
    backoff_expiry = err_time + wait_period;

    // If the newly computed backoff already expired, nullify it.
    if (ec->IsWallclockTimeGreaterThan(backoff_expiry))
      backoff_expiry = Time();
  }

  result->do_increment_failures = is_failure_occurred;
  result->backoff_expiry = backoff_expiry;
  result->url_idx = url_idx;
  result->url_num_errors = url_num_errors;
  return EvalStatus::kSucceeded;
}

EvalStatus ChromeOSPolicy::UpdateScattering(
    EvaluationContext* ec,
    State* state,
    string* error,
    UpdateScatteringResult* result,
    const UpdateState& update_state) const {
  // Preconditions. These stem from the postconditions and usage contract.
  DCHECK(update_state.scatter_wait_period >= kZeroInterval);
  DCHECK_GE(update_state.scatter_check_threshold, 0);

  // Set default result values.
  result->is_scattering = false;
  result->wait_period = kZeroInterval;
  result->check_threshold = 0;

  DevicePolicyProvider* const dp_provider = state->device_policy_provider();

  // Ensure that a device policy is loaded.
  const bool* device_policy_is_loaded_p =
      ec->GetValue(dp_provider->var_device_policy_is_loaded());
  if (!(device_policy_is_loaded_p && *device_policy_is_loaded_p))
    return EvalStatus::kSucceeded;

  // Is scattering enabled by policy?
  const TimeDelta* scatter_factor_p =
      ec->GetValue(dp_provider->var_scatter_factor());
  if (!scatter_factor_p || *scatter_factor_p == kZeroInterval)
    return EvalStatus::kSucceeded;

  // Obtain a pseudo-random number generator.
  const uint64_t* seed = ec->GetValue(state->random_provider()->var_seed());
  POLICY_CHECK_VALUE_AND_FAIL(seed, error);
  PRNG prng(*seed);

  // Step 1: Maintain the scattering wait period.
  //
  // If no wait period was previously determined, or it no longer fits in the
  // scatter factor, then generate a new one. Otherwise, keep the one we have.
  TimeDelta wait_period = update_state.scatter_wait_period;
  if (wait_period == kZeroInterval || wait_period > *scatter_factor_p) {
    wait_period = TimeDelta::FromSeconds(
        prng.RandMinMax(1, scatter_factor_p->InSeconds()));
  }

  // If we surpassed the wait period or the max scatter period associated with
  // the update, then no wait is needed.
  Time wait_expires = (update_state.first_seen +
                       min(wait_period, update_state.scatter_wait_period_max));
  if (ec->IsWallclockTimeGreaterThan(wait_expires))
    wait_period = kZeroInterval;

  // Step 2: Maintain the update check threshold count.
  //
  // If an update check threshold is not specified then generate a new
  // one.
  int check_threshold = update_state.scatter_check_threshold;
  if (check_threshold == 0) {
    check_threshold = prng.RandMinMax(update_state.scatter_check_threshold_min,
                                      update_state.scatter_check_threshold_max);
  }

  // If the update check threshold is not within allowed range then nullify it.
  // TODO(garnold) This is compliant with current logic found in
  // OmahaRequestAction::IsUpdateCheckCountBasedWaitingSatisfied(). We may want
  // to change it so that it behaves similarly to the wait period case, namely
  // if the current value exceeds the maximum, we set a new one within range.
  if (check_threshold > update_state.scatter_check_threshold_max)
    check_threshold = 0;

  // If the update check threshold is non-zero and satisfied, then nullify it.
  if (check_threshold > 0 && update_state.num_checks >= check_threshold)
    check_threshold = 0;

  bool is_scattering = (wait_period != kZeroInterval || check_threshold);
  EvalStatus ret = EvalStatus::kSucceeded;
  if (is_scattering && wait_period == update_state.scatter_wait_period &&
      check_threshold == update_state.scatter_check_threshold)
    ret = EvalStatus::kAskMeAgainLater;
  result->is_scattering = is_scattering;
  result->wait_period = wait_period;
  result->check_threshold = check_threshold;
  return ret;
}

}  // namespace chromeos_update_manager
