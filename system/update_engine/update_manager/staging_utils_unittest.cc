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

#include "update_engine/update_manager/staging_utils.h"

#include <memory>
#include <utility>

#include <base/time/time.h>
#include <gtest/gtest.h>
#include <policy/mock_device_policy.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_prefs.h"

using base::TimeDelta;
using chromeos_update_engine::FakePrefs;
using chromeos_update_engine::kPrefsWallClockStagingWaitPeriod;
using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace chromeos_update_manager {

constexpr TimeDelta kDay = TimeDelta::FromDays(1);
constexpr int kMaxDays = 28;
constexpr int kValidDaySum = 14;
const StagingSchedule valid_schedule = {{2, 0}, {7, 50}, {9, 80}, {14, 100}};

class StagingUtilsScheduleTest : public testing::Test {
 protected:
  void SetUp() override {
    test_wait_time_ = TimeDelta();
    test_staging_schedule_ = StagingSchedule();
  }

  void SetStagingSchedule(const StagingSchedule& staging_schedule) {
    EXPECT_CALL(device_policy_, GetDeviceUpdateStagingSchedule(_))
        .WillRepeatedly(
            DoAll(SetArgPointee<0>(staging_schedule), Return(true)));
  }

  void SetPersistedStagingVal(int64_t wait_time) {
    EXPECT_TRUE(
        fake_prefs_.SetInt64(kPrefsWallClockStagingWaitPeriod, wait_time));
  }

  void TestStagingCase(const StagingCase& expected) {
    EXPECT_EQ(expected,
              CalculateStagingCase(&device_policy_,
                                   &fake_prefs_,
                                   &test_wait_time_,
                                   &test_staging_schedule_));
  }

  void ExpectNoChanges() {
    EXPECT_EQ(TimeDelta(), test_wait_time_);
    EXPECT_EQ(StagingSchedule(), test_staging_schedule_);
  }

  policy::MockDevicePolicy device_policy_;
  TimeDelta test_wait_time_;
  StagingSchedule test_staging_schedule_;
  FakePrefs fake_prefs_;
};

// Last element should be 100, if not return false.
TEST_F(StagingUtilsScheduleTest, GetStagingScheduleInvalidLastElem) {
  SetStagingSchedule(StagingSchedule{{2, 10}, {4, 20}, {5, 40}});
  EXPECT_EQ(0, GetStagingSchedule(&device_policy_, &test_staging_schedule_));
  ExpectNoChanges();
}

// Percentage should be monotonically increasing.
TEST_F(StagingUtilsScheduleTest, GetStagingScheduleNonMonotonic) {
  SetStagingSchedule(StagingSchedule{{2, 10}, {6, 20}, {11, 20}, {12, 100}});
  EXPECT_EQ(0, GetStagingSchedule(&device_policy_, &test_staging_schedule_));
  ExpectNoChanges();
}

// The days should be monotonically increasing.
TEST_F(StagingUtilsScheduleTest, GetStagingScheduleOverMaxDays) {
  SetStagingSchedule(StagingSchedule{{2, 10}, {4, 20}, {15, 30}, {10, 100}});
  EXPECT_EQ(0, GetStagingSchedule(&device_policy_, &test_staging_schedule_));
  ExpectNoChanges();
}

TEST_F(StagingUtilsScheduleTest, GetStagingScheduleValid) {
  SetStagingSchedule(valid_schedule);
  EXPECT_EQ(kValidDaySum,
            GetStagingSchedule(&device_policy_, &test_staging_schedule_));
  EXPECT_EQ(test_staging_schedule_, valid_schedule);
}

TEST_F(StagingUtilsScheduleTest, StagingOffNoSchedule) {
  // If the function returns false, the schedule shouldn't get used.
  EXPECT_CALL(device_policy_, GetDeviceUpdateStagingSchedule(_))
      .WillRepeatedly(DoAll(SetArgPointee<0>(valid_schedule), Return(false)));
  TestStagingCase(StagingCase::kOff);
  ExpectNoChanges();
}

TEST_F(StagingUtilsScheduleTest, StagingOffEmptySchedule) {
  SetStagingSchedule(StagingSchedule());
  TestStagingCase(StagingCase::kOff);
  ExpectNoChanges();
}

TEST_F(StagingUtilsScheduleTest, StagingOffInvalidSchedule) {
  // Any invalid schedule should return |StagingCase::kOff|.
  SetStagingSchedule(StagingSchedule{{3, 30}, {6, 40}});
  TestStagingCase(StagingCase::kOff);
  ExpectNoChanges();
}

TEST_F(StagingUtilsScheduleTest, StagingOnNoAction) {
  test_wait_time_ = kDay;
  // Same as valid schedule, just using std::pair types.
  StagingSchedule valid_schedule_pairs = {{2, 0}, {7, 50}, {9, 80}, {14, 100}};
  test_staging_schedule_ = valid_schedule_pairs;
  SetStagingSchedule(valid_schedule);
  TestStagingCase(StagingCase::kNoAction);
  // Vars should not be changed.
  EXPECT_EQ(kDay, test_wait_time_);
  EXPECT_EQ(test_staging_schedule_, valid_schedule_pairs);
}

TEST_F(StagingUtilsScheduleTest, StagingNoSavedValueChangePolicy) {
  test_wait_time_ = kDay;
  SetStagingSchedule(valid_schedule);
  TestStagingCase(StagingCase::kNoSavedValue);
  // Vars should change since < 2 days should not be possible due to
  // valid_schedule's value.
  EXPECT_NE(kDay, test_wait_time_);
  EXPECT_EQ(test_staging_schedule_, valid_schedule);
  EXPECT_LE(test_wait_time_, kDay * kMaxDays);
}

// Tests the case where there was a reboot and there is no persisted value.
TEST_F(StagingUtilsScheduleTest, StagingNoSavedValueNoPersisted) {
  SetStagingSchedule(valid_schedule);
  TestStagingCase(StagingCase::kNoSavedValue);
  // Vars should change since there are no preset values and there is a new
  // staging schedule.
  EXPECT_NE(TimeDelta(), test_wait_time_);
  EXPECT_EQ(test_staging_schedule_, valid_schedule);
  EXPECT_LE(test_wait_time_, kDay * kMaxDays);
}

// If there is a pref set and its value is less than the day count, use that
// pref.
TEST_F(StagingUtilsScheduleTest, StagingSetFromPref) {
  SetStagingSchedule(valid_schedule);
  SetPersistedStagingVal(5);
  TestStagingCase(StagingCase::kSetStagingFromPref);
  // Vars should change.
  EXPECT_EQ(kDay * 5, test_wait_time_);
  EXPECT_EQ(test_staging_schedule_, valid_schedule);
}

}  // namespace chromeos_update_manager
