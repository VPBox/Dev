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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_POLICY_UTILS_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_POLICY_UTILS_H_

#include <string>
#include <vector>

#include "update_engine/update_manager/policy.h"

// Checks that the passed pointer value is not null, returning kFailed on the
// current context and setting the *error description when it is null. The
// intended use is to validate variable failures while using
// EvaluationContext::GetValue, for example:
//
//   const int* my_value = ec->GetValue(state->my_provider()->var_my_value());
//   POLICY_CHECK_VALUE_AND_FAIL(my_value, error);
//
#define POLICY_CHECK_VALUE_AND_FAIL(ptr, error)    \
  do {                                             \
    if ((ptr) == nullptr) {                        \
      *(error) = #ptr " is required but is null."; \
      return EvalStatus::kFailed;                  \
    }                                              \
  } while (false)

namespace chromeos_update_manager {

// Call the passed-in Policy method on a series of Policy implementations, until
// one of them renders a decision by returning a value other than
// |EvalStatus::kContinue|.
template <typename T, typename R, typename... Args>
EvalStatus ConsultPolicies(const std::vector<Policy const*> policies,
                           T policy_method,
                           EvaluationContext* ec,
                           State* state,
                           std::string* error,
                           R* result,
                           Args... args) {
  for (auto policy : policies) {
    EvalStatus status =
        (policy->*policy_method)(ec, state, error, result, args...);
    if (status != EvalStatus::kContinue) {
      LOG(INFO) << "decision by " << policy->PolicyRequestName(policy_method);
      return status;
    }
  }
  return EvalStatus::kContinue;
}

// Base class implementation that returns |EvalStatus::kContinue| for all
// decisions, to be used as a base-class for various Policy facets that only
// pertain to certain situations. This might be better folded into Policy
// instead of using pure-virtual methods on that class.
class PolicyImplBase : public Policy {
 public:
  // Policy overrides.
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                std::string* error,
                                UpdateCheckParams* result) const override {
    return EvalStatus::kContinue;
  };

  EvalStatus UpdateCanBeApplied(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      chromeos_update_engine::ErrorCode* result,
      chromeos_update_engine::InstallPlan* install_plan) const override {
    return EvalStatus::kContinue;
  };

  EvalStatus UpdateCanStart(EvaluationContext* ec,
                            State* state,
                            std::string* error,
                            UpdateDownloadParams* result,
                            UpdateState update_state) const override {
    return EvalStatus::kContinue;
  };

  EvalStatus UpdateDownloadAllowed(EvaluationContext* ec,
                                   State* state,
                                   std::string* error,
                                   bool* result) const override {
    return EvalStatus::kContinue;
  };

  EvalStatus P2PEnabled(EvaluationContext* ec,
                        State* state,
                        std::string* error,
                        bool* result) const override {
    return EvalStatus::kContinue;
  };

  EvalStatus P2PEnabledChanged(EvaluationContext* ec,
                               State* state,
                               std::string* error,
                               bool* result,
                               bool prev_result) const override {
    return EvalStatus::kContinue;
  };
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_POLICY_UTILS_H_
