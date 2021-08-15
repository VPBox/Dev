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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_ANDROID_THINGS_POLICY_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_ANDROID_THINGS_POLICY_H_

#include <string>

#include "update_engine/update_manager/next_update_check_policy_impl.h"
#include "update_engine/update_manager/policy_utils.h"

namespace chromeos_update_manager {

// AndroidThingsPolicy implements the policy-related logic used in
// AndroidThings.
class AndroidThingsPolicy : public Policy {
 public:
  AndroidThingsPolicy() = default;
  ~AndroidThingsPolicy() override = default;

  // Policy overrides.
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                std::string* error,
                                UpdateCheckParams* result) const override;

  // Uses the |UpdateRestrictions| to determine if the download and apply can
  // occur at this time.
  EvalStatus UpdateCanBeApplied(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      chromeos_update_engine::ErrorCode* result,
      chromeos_update_engine::InstallPlan* install_plan) const override;

  // Always returns |EvalStatus::kSucceeded|
  EvalStatus UpdateCanStart(EvaluationContext* ec,
                            State* state,
                            std::string* error,
                            UpdateDownloadParams* result,
                            UpdateState update_state) const override;

  // Always returns |EvalStatus::kSucceeded|
  EvalStatus UpdateDownloadAllowed(EvaluationContext* ec,
                                   State* state,
                                   std::string* error,
                                   bool* result) const override;

  // P2P is always disabled.  Returns |result|==|false| and
  // |EvalStatus::kSucceeded|
  EvalStatus P2PEnabled(EvaluationContext* ec,
                        State* state,
                        std::string* error,
                        bool* result) const override;

  // This will return immediately with |EvalStatus::kSucceeded| and set
  // |result|==|false|
  EvalStatus P2PEnabledChanged(EvaluationContext* ec,
                               State* state,
                               std::string* error,
                               bool* result,
                               bool prev_result) const override;

 protected:
  // Policy override.
  std::string PolicyName() const override { return "AndroidThingsPolicy"; }

 private:
  friend class UmAndroidThingsPolicyTest;
  FRIEND_TEST(UmAndroidThingsPolicyTest, UpdateCheckAllowedWaitsForTheTimeout);

  static const NextUpdateCheckPolicyConstants kNextUpdateCheckPolicyConstants;

  DISALLOW_COPY_AND_ASSIGN(AndroidThingsPolicy);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_ANDROID_THINGS_POLICY_H_
