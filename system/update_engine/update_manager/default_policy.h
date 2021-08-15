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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_DEFAULT_POLICY_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_DEFAULT_POLICY_H_

#include <memory>
#include <string>

#include <base/time/time.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/update_manager/policy.h"

namespace chromeos_update_manager {

// Auxiliary state class for DefaultPolicy evaluations.
//
// IMPORTANT: The use of a state object in policies is generally forbidden, as
// it was a design decision to keep policy calls side-effect free. We make an
// exception here to ensure that DefaultPolicy indeed serves as a safe (and
// secure) fallback option. This practice should be avoided when imlpementing
// other policies.
class DefaultPolicyState {
 public:
  DefaultPolicyState() {}

  bool IsLastCheckAllowedTimeSet() const {
    return last_check_allowed_time_ != base::Time::Max();
  }

  // Sets/returns the point time on the monotonic time scale when the latest
  // check allowed was recorded.
  void set_last_check_allowed_time(base::Time timestamp) {
    last_check_allowed_time_ = timestamp;
  }
  base::Time last_check_allowed_time() const {
    return last_check_allowed_time_;
  }

 private:
  base::Time last_check_allowed_time_ = base::Time::Max();
};

// The DefaultPolicy is a safe Policy implementation that doesn't fail. The
// values returned by this policy are safe default in case of failure of the
// actual policy being used by the UpdateManager.
class DefaultPolicy : public Policy {
 public:
  explicit DefaultPolicy(chromeos_update_engine::ClockInterface* clock);
  DefaultPolicy() : DefaultPolicy(nullptr) {}
  ~DefaultPolicy() override {}

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

  EvalStatus UpdateCanStart(EvaluationContext* ec,
                            State* state,
                            std::string* error,
                            UpdateDownloadParams* result,
                            UpdateState update_state) const override;

  EvalStatus UpdateDownloadAllowed(EvaluationContext* ec,
                                   State* state,
                                   std::string* error,
                                   bool* result) const override;

  EvalStatus P2PEnabled(EvaluationContext* ec,
                        State* state,
                        std::string* error,
                        bool* result) const override;

  EvalStatus P2PEnabledChanged(EvaluationContext* ec,
                               State* state,
                               std::string* error,
                               bool* result,
                               bool prev_result) const override;

 protected:
  // Policy override.
  std::string PolicyName() const override { return "DefaultPolicy"; }

 private:
  // A clock interface.
  chromeos_update_engine::ClockInterface* clock_;

  // An auxiliary state object.
  std::unique_ptr<DefaultPolicyState> aux_state_;

  DISALLOW_COPY_AND_ASSIGN(DefaultPolicy);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_DEFAULT_POLICY_H_
