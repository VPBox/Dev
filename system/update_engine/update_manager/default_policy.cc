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

#include "update_engine/update_manager/default_policy.h"

using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;

namespace {

// A fixed minimum interval between consecutive allowed update checks. This
// needs to be long enough to prevent busywork and/or DDoS attacks on Omaha, but
// at the same time short enough to allow the machine to update itself
// reasonably soon.
const int kCheckIntervalInSeconds = 15 * 60;

}  // namespace

namespace chromeos_update_manager {

DefaultPolicy::DefaultPolicy(chromeos_update_engine::ClockInterface* clock)
    : clock_(clock), aux_state_(new DefaultPolicyState()) {}

EvalStatus DefaultPolicy::UpdateCheckAllowed(EvaluationContext* ec,
                                             State* state,
                                             std::string* error,
                                             UpdateCheckParams* result) const {
  result->updates_enabled = true;
  result->target_channel.clear();
  result->target_version_prefix.clear();
  result->rollback_allowed = false;
  result->rollback_allowed_milestones = -1;  // No version rolls should happen.
  result->interactive = false;

  // Ensure that the minimum interval is set. If there's no clock, this defaults
  // to always allowing the update.
  if (!aux_state_->IsLastCheckAllowedTimeSet() ||
      ec->IsMonotonicTimeGreaterThan(
          aux_state_->last_check_allowed_time() +
          base::TimeDelta::FromSeconds(kCheckIntervalInSeconds))) {
    if (clock_)
      aux_state_->set_last_check_allowed_time(clock_->GetMonotonicTime());
    return EvalStatus::kSucceeded;
  }

  return EvalStatus::kAskMeAgainLater;
}

EvalStatus DefaultPolicy::UpdateCanBeApplied(EvaluationContext* ec,
                                             State* state,
                                             std::string* error,
                                             ErrorCode* result,
                                             InstallPlan* install_plan) const {
  *result = ErrorCode::kSuccess;
  return EvalStatus::kSucceeded;
}

EvalStatus DefaultPolicy::UpdateCanStart(EvaluationContext* ec,
                                         State* state,
                                         std::string* error,
                                         UpdateDownloadParams* result,
                                         const UpdateState update_state) const {
  result->update_can_start = true;
  result->cannot_start_reason = UpdateCannotStartReason::kUndefined;
  result->download_url_idx = 0;
  result->download_url_allowed = true;
  result->download_url_num_errors = 0;
  result->p2p_downloading_allowed = false;
  result->p2p_sharing_allowed = false;
  result->do_increment_failures = false;
  result->backoff_expiry = base::Time();
  result->scatter_wait_period = base::TimeDelta();
  result->scatter_check_threshold = 0;
  return EvalStatus::kSucceeded;
}

EvalStatus DefaultPolicy::UpdateDownloadAllowed(EvaluationContext* ec,
                                                State* state,
                                                std::string* error,
                                                bool* result) const {
  *result = true;
  return EvalStatus::kSucceeded;
}

EvalStatus DefaultPolicy::P2PEnabled(EvaluationContext* ec,
                                     State* state,
                                     std::string* error,
                                     bool* result) const {
  *result = false;
  return EvalStatus::kSucceeded;
}

EvalStatus DefaultPolicy::P2PEnabledChanged(EvaluationContext* ec,
                                            State* state,
                                            std::string* error,
                                            bool* result,
                                            bool prev_result) const {
  // This policy will always prohibit P2P, so this is signaling to the caller
  // that the decision is final (because the current value is the same as the
  // previous one) and there's no need to issue another call.
  *result = false;
  return EvalStatus::kSucceeded;
}

}  // namespace chromeos_update_manager
