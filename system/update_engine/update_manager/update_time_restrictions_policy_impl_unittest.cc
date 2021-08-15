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

#include "update_engine/update_manager/update_time_restrictions_policy_impl.h"

#include <memory>

#include <base/time/time.h>

#include "update_engine/update_manager/policy_test_utils.h"
#include "update_engine/update_manager/weekly_time.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;
using std::string;

namespace chromeos_update_manager {

constexpr TimeDelta kHour = TimeDelta::FromHours(1);
constexpr TimeDelta kMinute = TimeDelta::FromMinutes(1);

const WeeklyTimeIntervalVector kTestIntervals{
    // Monday 10:15 AM to Monday 3:30 PM.
    WeeklyTimeInterval(WeeklyTime(1, kHour * 10 + kMinute * 15),
                       WeeklyTime(1, kHour * 15 + kMinute * 30)),
    // Wednesday 8:30 PM to Thursday 8:40 AM.
    WeeklyTimeInterval(WeeklyTime(3, kHour * 20 + kMinute * 30),
                       WeeklyTime(4, kHour * 8 + kMinute * 40)),
};

class UmUpdateTimeRestrictionsPolicyImplTest : public UmPolicyTestBase {
 protected:
  UmUpdateTimeRestrictionsPolicyImplTest() {
    policy_ = std::make_unique<UpdateTimeRestrictionsPolicyImpl>();
  }

  void TestPolicy(const Time::Exploded& exploded,
                  const WeeklyTimeIntervalVector& test_intervals,
                  const EvalStatus& expected_value,
                  bool kiosk) {
    if (kiosk)
      fake_state_.device_policy_provider()
          ->var_auto_launched_kiosk_app_id()
          ->reset(new string("myapp"));

    Time time;
    EXPECT_TRUE(Time::FromLocalExploded(exploded, &time));
    fake_clock_.SetWallclockTime(time);
    SetUpDefaultTimeProvider();
    fake_state_.device_policy_provider()
        ->var_disallowed_time_intervals()
        ->reset(new WeeklyTimeIntervalVector(test_intervals));
    ErrorCode result;
    InstallPlan install_plan;
    ExpectPolicyStatus(
        expected_value, &Policy::UpdateCanBeApplied, &result, &install_plan);
    if (expected_value == EvalStatus::kSucceeded)
      EXPECT_EQ(result, ErrorCode::kOmahaUpdateDeferredPerPolicy);
  }
};

// If there are no intervals, then the check should always return kContinue.
TEST_F(UmUpdateTimeRestrictionsPolicyImplTest, NoIntervalsSetTest) {
  Time::Exploded random_time{2018, 7, 1, 9, 12, 30, 0, 0};
  TestPolicy(random_time,
             WeeklyTimeIntervalVector(),
             EvalStatus::kContinue,
             /* kiosk = */ true);
}

// Check that all intervals are checked.
TEST_F(UmUpdateTimeRestrictionsPolicyImplTest, TimeInRange) {
  // Monday, July 9th 2018 12:30 PM.
  Time::Exploded first_interval_time{2018, 7, 1, 9, 12, 30, 0, 0};
  TestPolicy(first_interval_time,
             kTestIntervals,
             EvalStatus::kSucceeded,
             /* kiosk = */ true);

  // Check second interval.
  // Thursday, July 12th 2018 4:30 AM.
  Time::Exploded second_interval_time{2018, 7, 4, 12, 4, 30, 0, 0};
  TestPolicy(second_interval_time,
             kTestIntervals,
             EvalStatus::kSucceeded,
             /* kiosk = */ true);
}

TEST_F(UmUpdateTimeRestrictionsPolicyImplTest, TimeOutOfRange) {
  // Monday, July 9th 2018 6:30 PM.
  Time::Exploded out_of_range_time{2018, 7, 1, 9, 18, 30, 0, 0};
  TestPolicy(out_of_range_time,
             kTestIntervals,
             EvalStatus::kContinue,
             /* kiosk = */ true);
}

TEST_F(UmUpdateTimeRestrictionsPolicyImplTest, NoKioskDisablesPolicy) {
  Time::Exploded in_range_time{2018, 7, 1, 9, 12, 30, 0, 0};
  TestPolicy(in_range_time,
             kTestIntervals,
             EvalStatus::kContinue,
             /* kiosk = */ false);
}
}  // namespace chromeos_update_manager
