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

#include "update_engine/update_manager/out_of_box_experience_policy_impl.h"

#include "update_engine/common/utils.h"

namespace chromeos_update_manager {

EvalStatus OobePolicyImpl::UpdateCheckAllowed(EvaluationContext* ec,
                                              State* state,
                                              std::string* error,
                                              UpdateCheckParams* result) const {
  SystemProvider* const system_provider = state->system_provider();

  // If OOBE is enabled, wait until it is completed.
  const bool* is_oobe_enabled_p =
      ec->GetValue(state->config_provider()->var_is_oobe_enabled());
  if (is_oobe_enabled_p && *is_oobe_enabled_p) {
    const bool* is_oobe_complete_p =
        ec->GetValue(system_provider->var_is_oobe_complete());
    if (is_oobe_complete_p && !(*is_oobe_complete_p)) {
      LOG(INFO) << "OOBE not completed, blocking update checks.";
      return EvalStatus::kAskMeAgainLater;
    }
  }
  return EvalStatus::kContinue;
}

}  // namespace chromeos_update_manager
