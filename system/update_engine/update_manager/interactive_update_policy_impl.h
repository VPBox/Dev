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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_INTERACTIVE_UPDATE_POLICY_IMPL_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_INTERACTIVE_UPDATE_POLICY_IMPL_H_

#include <string>

#include "update_engine/common/error_code.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/update_manager/policy_utils.h"

namespace chromeos_update_manager {

// Check to see if an interactive update was requested.
class InteractiveUpdatePolicyImpl : public PolicyImplBase {
 public:
  InteractiveUpdatePolicyImpl() = default;
  ~InteractiveUpdatePolicyImpl() override = default;

  // Policy overrides.
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                std::string* error,
                                UpdateCheckParams* result) const override;

  EvalStatus UpdateCanBeApplied(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      chromeos_update_engine::ErrorCode* result,
      chromeos_update_engine::InstallPlan* install_plan) const override;

 protected:
  std::string PolicyName() const override {
    return "InteractiveUpdatePolicyImpl";
  }

 private:
  // Checks whether a forced update was requested. If there is a forced update,
  // return true and set |interactive_out| to true if the forced update is
  // interactive, and false otherwise. If there are no forced updates, return
  // true and don't modify |interactive_out|.
  bool CheckInteractiveUpdateRequested(EvaluationContext* ec,
                                       UpdaterProvider* const updater_provider,
                                       bool* interactive_out) const;

  DISALLOW_COPY_AND_ASSIGN(InteractiveUpdatePolicyImpl);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_INTERACTIVE_UPDATE_POLICY_IMPL_H_
