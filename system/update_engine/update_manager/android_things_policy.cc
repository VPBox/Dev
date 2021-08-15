//
// Copyright (C) 2017 The Android Open Source Project
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

#include "update_engine/update_manager/android_things_policy.h"

#include <string>
#include <vector>

#include <base/logging.h>
#include <base/time/time.h>

#include "update_engine/update_manager/api_restricted_downloads_policy_impl.h"
#include "update_engine/update_manager/enough_slots_ab_updates_policy_impl.h"
#include "update_engine/update_manager/interactive_update_policy_impl.h"
#include "update_engine/update_manager/official_build_check_policy_impl.h"

using base::Time;
using chromeos_update_engine::ErrorCode;
using std::string;
using std::vector;

namespace chromeos_update_manager {

const NextUpdateCheckPolicyConstants
    AndroidThingsPolicy::kNextUpdateCheckPolicyConstants = {
        .timeout_initial_interval = 7 * 60,
        .timeout_periodic_interval = 5 * 60 * 60,
        .timeout_max_backoff_interval = 26 * 60 * 60,
        .timeout_regular_fuzz = 10 * 60,
        .attempt_backoff_max_interval_in_days = 16,
        .attempt_backoff_fuzz_in_hours = 12,
};

EvalStatus AndroidThingsPolicy::UpdateCheckAllowed(
    EvaluationContext* ec,
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

  // Build a list of policies to consult.  Note that each policy may modify the
  // result structure, even if it signals kContinue.
  EnoughSlotsAbUpdatesPolicyImpl enough_slots_ab_updates_policy;
  OnlyUpdateOfficialBuildsPolicyImpl only_update_official_builds_policy;
  InteractiveUpdatePolicyImpl interactive_update_policy;
  NextUpdateCheckTimePolicyImpl next_update_check_time_policy(
      kNextUpdateCheckPolicyConstants);

  vector<Policy const*> policies_to_consult = {
      // Do not perform any updates if there are not enough slots to do
      // A/B updates
      &enough_slots_ab_updates_policy,

      // Check to see if an interactive update was requested.
      &interactive_update_policy,

      // Unofficial builds should not perform periodic update checks.
      &only_update_official_builds_policy,

      // Ensure that periodic update checks are timed properly.
      &next_update_check_time_policy,
  };

  // Now that the list of policy implementations, and the order to consult them,
  // as been setup, do that.  If none of the policies make a definitive
  // decisions about whether or not to check for updates, then allow the update
  // check to happen.
  EvalStatus status = ConsultPolicies(policies_to_consult,
                                      &Policy::UpdateCheckAllowed,
                                      ec,
                                      state,
                                      error,
                                      result);
  if (status != EvalStatus::kContinue) {
    return status;
  } else {
    // It is time to check for an update.
    LOG(INFO) << "Allowing update check.";
    return EvalStatus::kSucceeded;
  }
}

// Uses the |UpdateRestrictions| to determine if the download and apply can
// occur at this time.
EvalStatus AndroidThingsPolicy::UpdateCanBeApplied(
    EvaluationContext* ec,
    State* state,
    string* error,
    ErrorCode* result,
    chromeos_update_engine::InstallPlan* install_plan) const {
  // Build a list of policies to consult.  Note that each policy may modify the
  // result structure, even if it signals kContinue.
  ApiRestrictedDownloadsPolicyImpl api_restricted_downloads_policy;

  vector<Policy const*> policies_to_consult = {
      // Do not apply the update if all updates are restricted by the API.
      &api_restricted_downloads_policy,
  };

  // Now that the list of policy implementations, and the order to consult them,
  // as been setup, do that.  If none of the policies make a definitive
  // decisions about whether or not to check for updates, then allow the update
  // check to happen.
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

// Always returns |EvalStatus::kSucceeded|
EvalStatus AndroidThingsPolicy::UpdateCanStart(EvaluationContext* ec,
                                               State* state,
                                               string* error,
                                               UpdateDownloadParams* result,
                                               UpdateState update_state) const {
  // Update is good to go.
  result->update_can_start = true;
  return EvalStatus::kSucceeded;
}

// Always returns |EvalStatus::kSucceeded|
EvalStatus AndroidThingsPolicy::UpdateDownloadAllowed(EvaluationContext* ec,
                                                      State* state,
                                                      string* error,
                                                      bool* result) const {
  // By default, we allow updates.
  *result = true;
  return EvalStatus::kSucceeded;
}

// P2P is always disabled.  Returns |result|==|false| and
// |EvalStatus::kSucceeded|
EvalStatus AndroidThingsPolicy::P2PEnabled(EvaluationContext* ec,
                                           State* state,
                                           string* error,
                                           bool* result) const {
  *result = false;
  return EvalStatus::kSucceeded;
}

// This will return immediately with |EvalStatus::kSucceeded| and set
// |result|==|false|
EvalStatus AndroidThingsPolicy::P2PEnabledChanged(EvaluationContext* ec,
                                                  State* state,
                                                  string* error,
                                                  bool* result,
                                                  bool prev_result) const {
  *result = false;
  return EvalStatus::kSucceeded;
}

}  // namespace chromeos_update_manager
