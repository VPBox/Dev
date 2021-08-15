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

#include "update_engine/update_manager/interactive_update_policy_impl.h"

using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;

namespace chromeos_update_manager {

// Check to see if an interactive update was requested.
EvalStatus InteractiveUpdatePolicyImpl::UpdateCheckAllowed(
    EvaluationContext* ec,
    State* state,
    std::string* error,
    UpdateCheckParams* result) const {
  bool interactive;
  if (CheckInteractiveUpdateRequested(
          ec, state->updater_provider(), &interactive)) {
    result->interactive = interactive;
    LOG(INFO) << "Forced update signaled ("
              << (interactive ? "interactive" : "periodic")
              << "), allowing update check.";
    return EvalStatus::kSucceeded;
  }
  return EvalStatus::kContinue;
}

EvalStatus InteractiveUpdatePolicyImpl::UpdateCanBeApplied(
    EvaluationContext* ec,
    State* state,
    std::string* error,
    ErrorCode* result,
    InstallPlan* install_plan) const {
  bool interactive;
  if (CheckInteractiveUpdateRequested(
          ec, state->updater_provider(), &interactive)) {
    LOG(INFO) << "Forced update signaled ("
              << (interactive ? "interactive" : "periodic")
              << "), allowing update to be applied.";
    *result = ErrorCode::kSuccess;
    return EvalStatus::kSucceeded;
  }
  return EvalStatus::kContinue;
}

bool InteractiveUpdatePolicyImpl::CheckInteractiveUpdateRequested(
    EvaluationContext* ec,
    UpdaterProvider* const updater_provider,
    bool* interactive_out) const {
  // First, check to see if an interactive update was requested.
  const UpdateRequestStatus* forced_update_requested_p =
      ec->GetValue(updater_provider->var_forced_update_requested());
  if (forced_update_requested_p != nullptr &&
      *forced_update_requested_p != UpdateRequestStatus::kNone) {
    if (interactive_out)
      *interactive_out =
          (*forced_update_requested_p == UpdateRequestStatus::kInteractive);
    return true;
  }
  return false;
}

}  // namespace chromeos_update_manager
