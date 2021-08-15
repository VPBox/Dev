/******************************************************************************
 *
 *  Copyright 2015 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "common/time_util.h"

// Generous upper bound: 10 seconds
static const uint32_t TEST_TIME_DELTA_UPPER_BOUND_MS = 10 * 1000;

//
// Test that the return value of bluetooth::common::time_get_os_boottime_ms() is
// not zero.
//
TEST(TimeTest, test_time_get_os_boottime_ms_not_zero) {
  uint64_t t1 = bluetooth::common::time_get_os_boottime_ms();
  ASSERT_GT(t1, uint64_t(0));
}

//
// Test that the return value of bluetooth::common::time_get_os_boottime_us() is
// not zero.
//
TEST(TimeTest, test_time_get_os_boottime_us_not_zero) {
  uint64_t t1 = bluetooth::common::time_get_os_boottime_us();
  ASSERT_GT(t1, uint64_t(0));
}

//
// Test that the return value of bluetooth::common::time_get_os_boottime_ms()
// is monotonically increasing within reasonable boundries.
//
TEST(TimeTest, test_time_get_os_boottime_ms_increases_upper_bound) {
  uint64_t t1 = bluetooth::common::time_get_os_boottime_ms();
  uint64_t t2 = bluetooth::common::time_get_os_boottime_ms();
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS);
}

//
// Test that the return value of bluetooth::common::time_get_os_boottime_us()
// is monotonically increasing within reasonable boundries.
//
TEST(TimeTest, test_time_get_os_boottime_us_increases_upper_bound) {
  uint64_t t1 = bluetooth::common::time_get_os_boottime_us();
  uint64_t t2 = bluetooth::common::time_get_os_boottime_us();
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS * 1000);
}

//
// Test that the return value of bluetooth::common::time_get_os_boottime_ms()
// is increasing.
//
TEST(TimeTest, test_time_get_os_boottime_ms_increases_lower_bound) {
  static const uint32_t TEST_TIME_SLEEP_MS = 100;
  struct timespec delay = {};

  delay.tv_sec = TEST_TIME_SLEEP_MS / 1000;
  delay.tv_nsec = 1000 * 1000 * (TEST_TIME_SLEEP_MS % 1000);

  // Take two timestamps with sleep in-between
  uint64_t t1 = bluetooth::common::time_get_os_boottime_ms();
  int err = nanosleep(&delay, &delay);
  uint64_t t2 = bluetooth::common::time_get_os_boottime_ms();

  ASSERT_EQ(err, 0);
  ASSERT_TRUE((t2 - t1) >= TEST_TIME_SLEEP_MS);
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS);
}

//
// Test that the return value of bluetooth::common::time_get_os_boottime_us()
// is increasing.
//
TEST(TimeTest, test_time_get_os_boottime_us_increases_lower_bound) {
  static const uint64_t TEST_TIME_SLEEP_US = 100 * 1000;
  struct timespec delay = {};

  delay.tv_sec = TEST_TIME_SLEEP_US / (1000 * 1000);
  delay.tv_nsec = 1000 * (TEST_TIME_SLEEP_US % (1000 * 1000));

  // Take two timestamps with sleep in-between
  uint64_t t1 = bluetooth::common::time_get_os_boottime_us();
  int err = nanosleep(&delay, &delay);
  uint64_t t2 = bluetooth::common::time_get_os_boottime_us();

  ASSERT_EQ(err, 0);
  ASSERT_GT(t2, t1);
  ASSERT_TRUE((t2 - t1) >= TEST_TIME_SLEEP_US);
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS * 1000);
}

//
// Test that the return value of bluetooth::common::time_gettimeofday_us() is
// not zero.
//
TEST(TimeTest, test_time_gettimeofday_us_not_zero) {
  uint64_t t1 = bluetooth::common::time_gettimeofday_us();
  ASSERT_GT(t1, uint64_t(0));
}

//
// Test that the return value of bluetooth::common::time_gettimeofday_us()
// is monotonically increasing within reasonable boundaries.
//
TEST(TimeTest, test_time_gettimeofday_us_increases_upper_bound) {
  uint64_t t1 = bluetooth::common::time_gettimeofday_us();
  uint64_t t2 = bluetooth::common::time_gettimeofday_us();
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS * 1000);
}

//
// Test that the return value of bluetooth::common::time_gettimeofday_us()
// is increasing.
//
TEST(TimeTest, test_time_gettimeofday_us_increases_lower_bound) {
  static const uint64_t TEST_TIME_SLEEP_US = 100 * 1000;
  struct timespec delay = {};

  delay.tv_sec = TEST_TIME_SLEEP_US / (1000 * 1000);
  delay.tv_nsec = 1000 * (TEST_TIME_SLEEP_US % (1000 * 1000));

  // Take two timestamps with sleep in-between
  uint64_t t1 = bluetooth::common::time_gettimeofday_us();
  int err = nanosleep(&delay, &delay);
  uint64_t t2 = bluetooth::common::time_gettimeofday_us();

  ASSERT_EQ(err, 0);
  ASSERT_GT(t2, t1);
  ASSERT_TRUE((t2 - t1) >= TEST_TIME_SLEEP_US);
  ASSERT_TRUE((t2 - t1) < TEST_TIME_DELTA_UPPER_BOUND_MS * 1000);
}
