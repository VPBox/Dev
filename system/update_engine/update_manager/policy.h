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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_POLICY_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_POLICY_H_

#include <string>
#include <tuple>
#include <vector>

#include "update_engine/common/error_code.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/update_manager/evaluation_context.h"
#include "update_engine/update_manager/rollback_prefs.h"
#include "update_engine/update_manager/state.h"

namespace chromeos_update_manager {

// The three different results of a policy request.
enum class EvalStatus {
  kFailed,
  kSucceeded,
  kAskMeAgainLater,
  kContinue,
};

std::string ToString(EvalStatus status);

// Parameters of an update check. These parameters are determined by the
// UpdateCheckAllowed policy.
struct UpdateCheckParams {
  bool updates_enabled;  // Whether the auto-updates are enabled on this build.

  // Attributes pertaining to the case where update checks are allowed.
  //
  // A target version prefix, if imposed by policy; otherwise, an empty string.
  std::string target_version_prefix;
  // Specifies whether rollback images are allowed by device policy.
  bool rollback_allowed;
  // Specifies the number of Chrome milestones rollback should be allowed,
  // starting from the stable version at any time. Value is -1 if unspecified
  // (e.g. no device policy is available yet), in this case no version
  // roll-forward should happen.
  int rollback_allowed_milestones;
  // A target channel, if so imposed by policy; otherwise, an empty string.
  std::string target_channel;

  // Whether the allowed update is interactive (user-initiated) or periodic.
  bool interactive;
};

// Input arguments to UpdateCanStart.
//
// A snapshot of the state of the current update process. This includes
// everything that a policy might need and that occurred since the first time
// the current payload was first seen and attempted (consecutively).
struct UpdateState {
  // Information pertaining to the current update payload and/or check.
  //
  // Whether the current update check is an interactive one. The caller should
  // feed the value returned by the preceding call to UpdateCheckAllowed().
  bool interactive;
  // Whether it is a delta payload.
  bool is_delta_payload;
  // Wallclock time when payload was first (consecutively) offered by Omaha.
  base::Time first_seen;
  // Number of consecutive update checks returning the current update.
  int num_checks;
  // Number of update payload failures and the wallclock time when it was last
  // updated by the updater. These should both be nullified whenever a new
  // update is seen; they are updated at the policy's descretion (via
  // UpdateDownloadParams.do_increment_failures) once all of the usable download
  // URLs for the payload have been used without success. They should be
  // persisted across reboots.
  int num_failures;
  base::Time failures_last_updated;

  // Information pertaining to downloading and applying of the current update.
  //
  // An array of download URLs provided by Omaha.
  std::vector<std::string> download_urls;
  // Max number of errors allowed per download URL.
  int download_errors_max;
  // The index of the URL to download from, as determined in the previous call
  // to the policy. For a newly seen payload, this should be -1.
  int last_download_url_idx;
  // The number of successive download errors pertaining to this last URL, as
  // determined in the previous call to the policy. For a newly seen payload,
  // this should be zero.
  int last_download_url_num_errors;
  // An array of errors that occurred while trying to download this update since
  // the previous call to this policy has returned, or since this payload was
  // first seen, or since the updater process has started (whichever is later).
  // Includes the URL index attempted, the error code, and the wallclock-based
  // timestamp when it occurred.
  std::vector<std::tuple<int, chromeos_update_engine::ErrorCode, base::Time>>
      download_errors;
  // Whether Omaha forbids use of P2P for downloading and/or sharing.
  bool p2p_downloading_disabled;
  bool p2p_sharing_disabled;
  // The number of P2P download attempts and wallclock-based time when P2P
  // download was first attempted.
  int p2p_num_attempts;
  base::Time p2p_first_attempted;

  // Information pertaining to update backoff mechanism.
  //
  // The currently known (persisted) wallclock-based backoff expiration time;
  // zero if none.
  base::Time backoff_expiry;
  // Whether backoff is disabled by Omaha.
  bool is_backoff_disabled;

  // Information pertaining to update scattering.
  //
  // The currently known (persisted) scattering wallclock-based wait period and
  // update check threshold; zero if none.
  base::TimeDelta scatter_wait_period;
  int scatter_check_threshold;
  // Maximum wait period allowed for this update, as determined by Omaha.
  base::TimeDelta scatter_wait_period_max;
  // Minimum/maximum check threshold values.
  // TODO(garnold) These appear to not be related to the current update and so
  // should probably be obtained as variables via UpdaterProvider.
  int scatter_check_threshold_min;
  int scatter_check_threshold_max;
};

// Results regarding the downloading and applying of an update, as determined by
// UpdateCanStart.
//
// An enumerator for the reasons of not allowing an update to start.
enum class UpdateCannotStartReason {
  kUndefined,
  kCheckDue,
  kScattering,
  kBackoff,
  kCannotDownload,
};

struct UpdateDownloadParams {
  // Whether the update attempt is allowed to proceed.
  bool update_can_start;
  // If update cannot proceed, a reason code for why it cannot do so.
  UpdateCannotStartReason cannot_start_reason;

  // Download related attributes. The update engine uses them to choose the
  // means for downloading and applying an update.
  //
  // The index of the download URL to use (-1 means no suitable URL was found)
  // and whether it can be used. Even if there's no URL or its use is not
  // allowed (backoff, scattering) there may still be other means for download
  // (like P2P).  The URL index needs to be persisted and handed back to the
  // policy on the next time it is called.
  int download_url_idx;
  bool download_url_allowed;
  // The number of download errors associated with this download URL. This value
  // needs to be persisted and handed back to the policy on the next time it is
  // called.
  int download_url_num_errors;
  // Whether P2P download and sharing are allowed.
  bool p2p_downloading_allowed;
  bool p2p_sharing_allowed;

  // Other values that need to be persisted and handed to the policy as need on
  // the next call.
  //
  // Whether an update failure has been identified by the policy. The client
  // should increment and persist its update failure count, and record the time
  // when this was done; it needs to hand these values back to the policy
  // (UpdateState.{num_failures,failures_last_updated}) on the next time it is
  // called.
  bool do_increment_failures;
  // The current backof expiry.
  base::Time backoff_expiry;
  // The scattering wait period and check threshold.
  base::TimeDelta scatter_wait_period;
  int scatter_check_threshold;
};

// The Policy class is an interface to the ensemble of policy requests that the
// client can make. A derived class includes the policy implementations of
// these.
//
// When compile-time selection of the policy is required due to missing or extra
// parts in a given platform, a different Policy subclass can be used.
class Policy {
 public:
  virtual ~Policy() {}

  // Returns the name of a public policy request.
  // IMPORTANT: Be sure to add a conditional for each new public policy that is
  // being added to this class in the future.
  template <typename R, typename... Args>
  std::string PolicyRequestName(EvalStatus (Policy::*policy_method)(
      EvaluationContext*, State*, std::string*, R*, Args...) const) const {
    std::string class_name = PolicyName() + "::";

    if (reinterpret_cast<typeof(&Policy::UpdateCheckAllowed)>(policy_method) ==
        &Policy::UpdateCheckAllowed)
      return class_name + "UpdateCheckAllowed";
    if (reinterpret_cast<typeof(&Policy::UpdateCanBeApplied)>(policy_method) ==
        &Policy::UpdateCanBeApplied)
      return class_name + "UpdateCanBeApplied";
    if (reinterpret_cast<typeof(&Policy::UpdateCanStart)>(policy_method) ==
        &Policy::UpdateCanStart)
      return class_name + "UpdateCanStart";
    if (reinterpret_cast<typeof(&Policy::UpdateDownloadAllowed)>(
            policy_method) == &Policy::UpdateDownloadAllowed)
      return class_name + "UpdateDownloadAllowed";
    if (reinterpret_cast<typeof(&Policy::P2PEnabled)>(policy_method) ==
        &Policy::P2PEnabled)
      return class_name + "P2PEnabled";
    if (reinterpret_cast<typeof(&Policy::P2PEnabledChanged)>(policy_method) ==
        &Policy::P2PEnabledChanged)
      return class_name + "P2PEnabledChanged";

    NOTREACHED();
    return class_name + "(unknown)";
  }

  // List of policy requests. A policy request takes an EvaluationContext as the
  // first argument, a State instance, a returned error message, a returned
  // value and optionally followed by one or more arbitrary constant arguments.
  //
  // When the implementation fails, the method returns EvalStatus::kFailed and
  // sets the |error| string.

  // UpdateCheckAllowed returns whether it is allowed to request an update check
  // to Omaha.
  virtual EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                        State* state,
                                        std::string* error,
                                        UpdateCheckParams* result) const = 0;

  // UpdateCanBeApplied returns whether the given |install_plan| can be acted
  // on at this time.  The reason for not applying is returned in |result|.
  // The Policy may modify the passed-in |install_plan|, based on the
  // implementation in the Policy and values provided by the EvaluationContext.
  virtual EvalStatus UpdateCanBeApplied(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      chromeos_update_engine::ErrorCode* result,
      chromeos_update_engine::InstallPlan* install_plan) const = 0;

  // Returns EvalStatus::kSucceeded if either an update can start being
  // processed, or the attempt needs to be aborted. In cases where the update
  // needs to wait for some condition to be satisfied, but none of the values
  // that need to be persisted has changed, returns
  // EvalStatus::kAskMeAgainLater. Arguments include an |update_state| that
  // encapsulates data pertaining to the current ongoing update process.
  virtual EvalStatus UpdateCanStart(EvaluationContext* ec,
                                    State* state,
                                    std::string* error,
                                    UpdateDownloadParams* result,
                                    UpdateState update_state) const = 0;

  // Checks whether downloading of an update is allowed; currently, this checks
  // whether the network connection type is suitable for updating over.  May
  // consult the shill provider as well as the device policy (if available).
  // Returns |EvalStatus::kSucceeded|, setting |result| according to whether or
  // not the current connection can be used; on error, returns
  // |EvalStatus::kFailed| and sets |error| accordingly.
  virtual EvalStatus UpdateDownloadAllowed(EvaluationContext* ec,
                                           State* state,
                                           std::string* error,
                                           bool* result) const = 0;

  // Checks whether P2P is enabled. This may consult device policy and other
  // global settings.
  virtual EvalStatus P2PEnabled(EvaluationContext* ec,
                                State* state,
                                std::string* error,
                                bool* result) const = 0;

  // Checks whether P2P is enabled, but blocks (returns
  // |EvalStatus::kAskMeAgainLater|) until it is different from |prev_result|.
  // If the P2P enabled status is not expected to change, will return
  // immediately with |EvalStatus::kSucceeded|. This internally uses the
  // P2PEnabled() policy above.
  virtual EvalStatus P2PEnabledChanged(EvaluationContext* ec,
                                       State* state,
                                       std::string* error,
                                       bool* result,
                                       bool prev_result) const = 0;

 protected:
  Policy() {}

  // Returns the name of the actual policy class.
  virtual std::string PolicyName() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Policy);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_POLICY_H_
