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

#include "update_engine/update_manager/enough_slots_ab_updates_policy_impl.h"

namespace chromeos_update_manager {

// Do not perform any updates if booted from removable device. This decision
// is final.
EvalStatus EnoughSlotsAbUpdatesPolicyImpl::UpdateCheckAllowed(
    EvaluationContext* ec,
    State* state,
    std::string* error,
    UpdateCheckParams* result) const {
  const auto* num_slots_p =
      ec->GetValue(state->system_provider()->var_num_slots());
  if (num_slots_p == nullptr || *num_slots_p < 2) {
    LOG(INFO) << "Not enough slots for A/B updates, disabling update checks.";
    result->updates_enabled = false;
    return EvalStatus::kSucceeded;
  }
  return EvalStatus::kContinue;
}

}  // namespace chromeos_update_manager
