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

#include "update_engine/update_manager/api_restricted_downloads_policy_impl.h"

using chromeos_update_engine::ErrorCode;
using std::string;
using std::vector;

namespace chromeos_update_manager {

// Allow the API to restrict the downloading of updates.
EvalStatus ApiRestrictedDownloadsPolicyImpl::UpdateCanBeApplied(
    EvaluationContext* ec,
    State* state,
    std::string* error,
    ErrorCode* result,
    chromeos_update_engine::InstallPlan* install_plan) const {
  // Next, check to see if updates can be applied (in general).
  const UpdateRestrictions* update_restrictions_p =
      ec->GetValue(state->updater_provider()->var_update_restrictions());
  if (update_restrictions_p) {
    if (*update_restrictions_p & UpdateRestrictions::kRestrictDownloading) {
      *result = ErrorCode::kOmahaUpdateDeferredPerPolicy;
      return EvalStatus::kSucceeded;
    }
  }

  // The API isn't restricting downloads, so implicitly allow them to happen
  // but don't explicitly return success from this policy implementation.
  return EvalStatus::kContinue;
}

}  // namespace chromeos_update_manager
