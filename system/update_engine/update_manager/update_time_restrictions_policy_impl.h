//
// Copyright (C) 2018 The Android Open Source Project
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
#ifndef UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_TIME_RESTRICTIONS_POLICY_IMPL_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_TIME_RESTRICTIONS_POLICY_IMPL_H_

#include <string>

#include <base/time/time.h>

#include "update_engine/common/error_code.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/update_manager/policy_utils.h"

namespace chromeos_update_manager {

// Policy that allows administrators to set time intervals during which
// automatic update checks are disallowed. This implementation then checks if
// the current time falls in the range spanned by the time intervals. If the
// current time falls in one of the intervals then the update check is
// blocked by this policy.
class UpdateTimeRestrictionsPolicyImpl : public PolicyImplBase {
 public:
  UpdateTimeRestrictionsPolicyImpl() = default;
  ~UpdateTimeRestrictionsPolicyImpl() override = default;

  // When the current time is inside one of the intervals returns
  // kSucceeded and sets |result| to kOmahaUpdateDeferredPerPolicy. If the
  // current time is not inside any intervals returns kContinue. In case of
  // errors, i.e. cannot access intervals or time, return kContinue.
  EvalStatus UpdateCanBeApplied(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      chromeos_update_engine::ErrorCode* result,
      chromeos_update_engine::InstallPlan* install_plan) const override;

 protected:
  std::string PolicyName() const override {
    return "UpdateTimeRestrictionsPolicyImpl";
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(UpdateTimeRestrictionsPolicyImpl);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_TIME_RESTRICTIONS_POLICY_IMPL_H_
