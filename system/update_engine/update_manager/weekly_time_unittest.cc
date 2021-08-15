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
#include "update_engine/update_manager/weekly_time.h"

#include <tuple>

#include <base/time/time.h>
#include <gtest/gtest.h>

using base::TimeDelta;
using std::tuple;

namespace chromeos_update_manager {

namespace {

enum {
  kSunday = 0,
  kMonday,
  kTuesday,
  kWednesday,
  kThursday,
  kFriday,
  kSaturday
};

}  // namespace

class WeeklyTimeDurationTest
    : public testing::TestWithParam<tuple<int /* start_day_of_week */,
                                          TimeDelta /* start_time */,
                                          int /* end_day_of_week */,
                                          TimeDelta /* end_time */,
                                          TimeDelta /* expected result */>> {
 protected:
  int start_day_of_week() { return std::get<0>(GetParam()); }
  TimeDelta start_time() { return std::get<1>(GetParam()); }
  int end_day_of_week() { return std::get<2>(GetParam()); }
  TimeDelta end_time() { return std::get<3>(GetParam()); }
  TimeDelta result() { return std::get<4>(GetParam()); }
};

TEST_P(WeeklyTimeDurationTest, GetDurationTo) {
  WeeklyTime start = WeeklyTime(start_day_of_week(), start_time());
  WeeklyTime end = WeeklyTime(end_day_of_week(), end_time());

  EXPECT_EQ(result(), start.GetDurationTo(end));
}

INSTANTIATE_TEST_CASE_P(
    SameMinutes,
    WeeklyTimeDurationTest,
    testing::Values(std::make_tuple(kThursday,
                                    TimeDelta::FromMinutes(30),
                                    kSaturday,
                                    TimeDelta::FromMinutes(30),
                                    TimeDelta::FromDays(2))));

INSTANTIATE_TEST_CASE_P(
    DifferentMinutes,
    WeeklyTimeDurationTest,
    testing::Values(std::make_tuple(kMonday,
                                    TimeDelta::FromMinutes(10),
                                    kWednesday,
                                    TimeDelta::FromMinutes(30),
                                    TimeDelta::FromDays(2) +
                                        TimeDelta::FromMinutes(20))));

INSTANTIATE_TEST_CASE_P(
    EndLessThanStartSameMinutes,
    WeeklyTimeDurationTest,
    testing::Values(std::make_tuple(kSaturday,
                                    TimeDelta::FromMinutes(100),
                                    kTuesday,
                                    TimeDelta::FromMinutes(100),
                                    TimeDelta::FromDays(3))));

INSTANTIATE_TEST_CASE_P(
    EndLessThanStartDifferentMinutes,
    WeeklyTimeDurationTest,
    testing::Values(std::make_tuple(kSaturday,
                                    TimeDelta::FromMinutes(150),
                                    kMonday,
                                    TimeDelta::FromMinutes(10),
                                    TimeDelta::FromDays(2) -
                                        TimeDelta::FromMinutes(140))));

class WeeklyTimeOffsetTest
    : public testing::TestWithParam<tuple<int /* day_of_week */,
                                          TimeDelta /* time */,
                                          TimeDelta /* offset */,
                                          WeeklyTime /* expected result */>> {
 protected:
  int day_of_week() { return std::get<0>(GetParam()); }
  TimeDelta time() { return std::get<1>(GetParam()); }
  TimeDelta offset() { return std::get<2>(GetParam()); }
  WeeklyTime result() { return std::get<3>(GetParam()); }
};

TEST_P(WeeklyTimeOffsetTest, WeekTimeAddTime) {
  WeeklyTime test_time = WeeklyTime(day_of_week(), time());
  test_time.AddTime(offset());

  EXPECT_EQ(result(), test_time);
}

INSTANTIATE_TEST_CASE_P(
    SameDayTest,
    WeeklyTimeOffsetTest,
    testing::Values(std::make_tuple(kTuesday,
                                    TimeDelta::FromMinutes(200),
                                    TimeDelta::FromMinutes(400),
                                    WeeklyTime(kTuesday,
                                               TimeDelta::FromMinutes(600)))));

INSTANTIATE_TEST_CASE_P(DayChangeTest,
                        WeeklyTimeOffsetTest,
                        testing::Values(std::make_tuple(
                            kThursday,
                            TimeDelta::FromHours(23),
                            TimeDelta::FromHours(2),
                            WeeklyTime(kFriday, TimeDelta::FromHours(1)))));

INSTANTIATE_TEST_CASE_P(DayChangeTestOver7,
                        WeeklyTimeOffsetTest,
                        testing::Values(std::make_tuple(
                            kSunday,
                            TimeDelta::FromHours(20),
                            TimeDelta::FromDays(3),
                            WeeklyTime(kWednesday, TimeDelta::FromHours(20)))));

class WeeklyTimeIntervalRangeTest
    : public testing::TestWithParam<tuple<int /* test_day_of_week */,
                                          int /* test_time */,
                                          bool /* in regular interval */,
                                          bool /* in short interval */,
                                          bool /* |start| < | */>> {
 protected:
  int day_of_week() { return std::get<0>(GetParam()); }
  int minutes() { return std::get<1>(GetParam()); }
  bool regular_result() { return std::get<2>(GetParam()); }
  bool short_result() { return std::get<3>(GetParam()); }
  bool wraparound_result() { return std::get<4>(GetParam()); }
};

TEST_P(WeeklyTimeIntervalRangeTest, InRange) {
  WeeklyTime test =
      WeeklyTime(day_of_week(), TimeDelta::FromMinutes(minutes()));
  WeeklyTimeInterval interval_regular =
      WeeklyTimeInterval(WeeklyTime(kMonday, TimeDelta::FromMinutes(10)),
                         WeeklyTime(kWednesday, TimeDelta::FromMinutes(30)));
  WeeklyTimeInterval interval_short =
      WeeklyTimeInterval(WeeklyTime(kThursday, TimeDelta::FromMinutes(10)),
                         WeeklyTime(kThursday, TimeDelta::FromMinutes(11)));

  WeeklyTimeInterval interval_wraparound =
      WeeklyTimeInterval(WeeklyTime(kFriday, TimeDelta::FromMinutes(10)),
                         WeeklyTime(kTuesday, TimeDelta::FromMinutes(30)));

  EXPECT_EQ(regular_result(), interval_regular.InRange(test));
  EXPECT_EQ(short_result(), interval_short.InRange(test));
  EXPECT_EQ(wraparound_result(), interval_wraparound.InRange(test));
}

// Test the left side of the range being inclusive.
INSTANTIATE_TEST_CASE_P(
    InclusiveSuccessLeft,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kThursday, 10, false, true, false)));

// Test the right side of the range being exclusive.
INSTANTIATE_TEST_CASE_P(
    ExclusiveSuccessRight,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kThursday, 11, false, false, false)));

// Test falling out of the interval by a small amount.
INSTANTIATE_TEST_CASE_P(
    FailOutsideRangeSmall,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kThursday, 12, false, false, false)));

// These test cases check that intervals wrap around properly.
INSTANTIATE_TEST_CASE_P(
    WraparoundOutside,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kWednesday, 10, true, false, false)));

INSTANTIATE_TEST_CASE_P(
    WraparoundInsideRight,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kSaturday, 10, false, false, true)));

INSTANTIATE_TEST_CASE_P(
    WraparoundInsideLeft,
    WeeklyTimeIntervalRangeTest,
    testing::Values(std::make_tuple(kMonday, 0, false, false, true)));

}  // namespace chromeos_update_manager
