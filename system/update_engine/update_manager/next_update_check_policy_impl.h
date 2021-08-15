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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_NEXT_UPDATE_CHECK_POLICY_IMPL_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_NEXT_UPDATE_CHECK_POLICY_IMPL_H_

#include <string>

#include <base/time/time.h>

#include "update_engine/update_manager/policy_utils.h"
#include "update_engine/update_manager/prng.h"

namespace chromeos_update_manager {

// Constants that are provided to the policy implementation.
struct NextUpdateCheckPolicyConstants {
  // Default update check timeout interval/fuzz values used to compute the
  // NextUpdateCheckTime(), in seconds. Actual fuzz is within +/- half of the
  // indicated value.
  int timeout_initial_interval;
  int timeout_periodic_interval;
  int timeout_max_backoff_interval;
  int timeout_regular_fuzz;

  // Maximum update attempt backoff interval and fuzz.
  int attempt_backoff_max_interval_in_days;
  int attempt_backoff_fuzz_in_hours;
};

// Ensure that periodic update checks are timed properly.
class NextUpdateCheckTimePolicyImpl : public PolicyImplBase {
 public:
  explicit NextUpdateCheckTimePolicyImpl(
      const NextUpdateCheckPolicyConstants& constants);

  // Policy overrides.
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                std::string* error,
                                UpdateCheckParams* result) const override;

  // A private policy implementation returning the wallclock timestamp when
  // the next update check should happen.
  // TODO(garnold) We should probably change that to infer a monotonic
  // timestamp, which will make the update check intervals more resilient to
  // clock skews. Might require switching some of the variables exported by the
  // UpdaterProvider to report monotonic time, as well.
  //
  // NOTE:
  // Exposed as a public static so that it's logic can be used to test
  // Policy implementations that utilize this fragment for their
  // timing, without needing to list them all with FRIEND_TEST (so that
  // those Policy implementations can exist without modifying this
  // class's definition.
  //
  // The output value from this method (|next_update_check|), isn't
  // available via the UpdateCheckParams |result| of the Policy
  // method, and so this timing logic needs to be otherwise exposed.
  static EvalStatus NextUpdateCheckTime(
      EvaluationContext* ec,
      State* state,
      std::string* error,
      base::Time* next_update_check,
      const NextUpdateCheckPolicyConstants& constants);

  // Returns a TimeDelta based on the provided |interval| seconds +/- half
  // |fuzz| seconds. The return value is guaranteed to be a non-negative
  // TimeDelta.
  static base::TimeDelta FuzzedInterval(PRNG* prng, int interval, int fuzz);

 protected:
  std::string PolicyName() const override {
    return "NextUpdateCheckTimePolicyImpl";
  }

 private:
  const NextUpdateCheckPolicyConstants policy_constants_;

  DISALLOW_COPY_AND_ASSIGN(NextUpdateCheckTimePolicyImpl);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_NEXT_UPDATE_CHECK_POLICY_IMPL_H_
