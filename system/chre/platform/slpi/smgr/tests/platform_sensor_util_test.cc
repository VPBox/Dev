/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gtest/gtest.h"

#include "chre/platform/slpi/smgr/platform_sensor_util.h"

using chre::intervalToSmgrSamplingRate;
using chre::intervalToSmgrQ16ReportRate;
using chre::Milliseconds;
using chre::Nanoseconds;
using chre::Seconds;

TEST(SmgrSamplingRateTest, Zero) {
  uint16_t rate = intervalToSmgrSamplingRate(Nanoseconds(0));
  EXPECT_EQ(rate, 0);
}

TEST(SmgrSamplingRateTest, FiftyHertz) {
  constexpr Nanoseconds kFiftyHertzInterval = Milliseconds(20);
  uint16_t rate = intervalToSmgrSamplingRate(kFiftyHertzInterval);
  EXPECT_EQ(rate, 50);
}

TEST(SmgrSamplingRateTest, ZeroPointFiveHertz) {
  constexpr Nanoseconds kZeroPointFiveHertzInterval = Seconds(2);
  uint16_t rate = intervalToSmgrSamplingRate(kZeroPointFiveHertzInterval);
  EXPECT_EQ(rate, 2000);
}

TEST(SmgrSamplingRateTest, MinisculeRate) {
  constexpr Nanoseconds kOneNanoHzInterval = Seconds(1e9);
  uint16_t rate = intervalToSmgrSamplingRate(kOneNanoHzInterval);
  EXPECT_EQ(rate, INT16_MAX);
}

TEST(SmgrSamplingRateTest, HugeRate) {
  constexpr Nanoseconds kTwentyMHzInterval = Nanoseconds(50);
  uint16_t rate = intervalToSmgrSamplingRate(kTwentyMHzInterval);
  EXPECT_EQ(rate, 1000);  // Should clamp to inversion point
}

TEST(SmgrQ16ReportRateTest, Zero) {
  uint32_t rate = intervalToSmgrQ16ReportRate(Nanoseconds(0));
  EXPECT_EQ(rate, INT32_MAX);
}

TEST(SmgrQ16ReportRateTest, FiftyHertz) {
  constexpr Nanoseconds kTenHertzInterval = Milliseconds(20);
  uint32_t rate = intervalToSmgrQ16ReportRate(kTenHertzInterval);
  EXPECT_EQ(rate, 0x10000 * 50);
}

TEST(SmgrQ16ReportRateTest, ZeroPointFiveHertz) {
  constexpr Nanoseconds kZeroPointFiveHertzInterval = Seconds(2);
  uint32_t rate = intervalToSmgrQ16ReportRate(kZeroPointFiveHertzInterval);
  EXPECT_EQ(rate, 0x10000 / 2);
}

TEST(SmgrQ16ReportRateTest, OneNanosecond) {
  uint32_t rate = intervalToSmgrQ16ReportRate(Nanoseconds(1));
  EXPECT_EQ(rate, INT32_MAX);
}
