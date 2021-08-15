/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/util/time.h"

using chre::Seconds;
using chre::Milliseconds;
using chre::Microseconds;
using chre::Nanoseconds;
using chre::kOneSecondInMilliseconds;
using chre::kOneSecondInNanoseconds;
using chre::kOneMillisecondInMicroseconds;
using chre::kOneMillisecondInNanoseconds;
using chre::kOneMicrosecondInNanoseconds;

// Tests for Time constants
TEST(Time, CheckTimeConversionConstants) {
  EXPECT_EQ(kOneSecondInNanoseconds, 1e9);
  EXPECT_EQ(kOneMillisecondInNanoseconds, 1e6);
  EXPECT_EQ(kOneMicrosecondInNanoseconds, 1e3);
  EXPECT_EQ(kOneMillisecondInMicroseconds, 1e3);
}

// Tests for Seconds
TEST(Time, ConvertSecToNanosec) {
  Seconds t(5);
  EXPECT_EQ(t.toRawNanoseconds(), 5 * kOneSecondInNanoseconds);
}

TEST(Time, ConvertSecToNanosecOverflowIsUint64Max) {
  Seconds t(UINT64_MAX / kOneSecondInNanoseconds + 1);
  EXPECT_EQ(t.toRawNanoseconds(), UINT64_MAX);
}

TEST(Time, ConvertSecToMillisec) {
  Seconds t(5);
  EXPECT_EQ(t.getMilliseconds(), 5 * kOneSecondInMilliseconds);
}

TEST(Time, ConvertSecToMillisecOverflowIsUint64Max) {
  Seconds t(UINT64_MAX / kOneSecondInMilliseconds + 1);
  EXPECT_EQ(t.getMilliseconds(), UINT64_MAX);
}

// Tests for Milliseconds
TEST(Time, DefaultMillisecIsZero) {
  Milliseconds t;
  EXPECT_EQ(t.getMilliseconds(), 0);
}

TEST(Time, GetInitialMillisec) {
  Milliseconds t(5);
  EXPECT_EQ(t.getMilliseconds(), 5);
}

TEST(Time, InitializeMillisecFromNanosec) {
  Nanoseconds tNano(5 * kOneMillisecondInNanoseconds);
  Milliseconds tMilli(tNano);
  EXPECT_EQ(tMilli.getMilliseconds(), 5);
}

TEST(Time, ConcertMillisecToMicrosec) {
  Milliseconds t(5);
  EXPECT_EQ(t.getMicroseconds(), 5 * kOneMillisecondInMicroseconds);
}

TEST(Time, ConvertMillisecToMicrosecOverflowIsUint64Max) {
  Milliseconds t(UINT64_MAX / kOneMillisecondInMicroseconds + 1);
  EXPECT_EQ(t.getMicroseconds(), UINT64_MAX);
}

TEST(Time, ConvertMillisecToNanosec) {
  Milliseconds t(5);
  EXPECT_EQ(t.toRawNanoseconds(), 5 * kOneMillisecondInNanoseconds);
}

TEST(Time, ConvertMillisecToNanosecOverflowIsUint64Max) {
  Milliseconds t(UINT64_MAX / kOneMillisecondInNanoseconds + 1);
  EXPECT_EQ(t.toRawNanoseconds(), UINT64_MAX);
}

TEST(Time, TestMillisecEquals) {
  Milliseconds t1(5), t2(5), t3(6);
  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 == t3);
}

// Tests for Microseconds
TEST(Time, GetInitialMicrosec) {
  Microseconds t(5);
  EXPECT_EQ(t.getMicroseconds(), 5);
}

TEST(Time, InitializeMicrosecFromNanosec) {
  Nanoseconds tNano(5 * kOneMicrosecondInNanoseconds);
  Microseconds tMicro(tNano);
  EXPECT_EQ(tMicro.getMicroseconds(), 5);
}

TEST(Time, ConvertMicrosecToNanosec) {
  Microseconds t(5);
  EXPECT_EQ(t.toRawNanoseconds(), 5 * kOneMicrosecondInNanoseconds);
}

TEST(Time, ConvertMicrosecToNanosecOverflowIsUint64Max) {
  Milliseconds t(UINT64_MAX / kOneMicrosecondInNanoseconds + 1);
  EXPECT_EQ(t.toRawNanoseconds(), UINT64_MAX);
}

TEST(Time, ConvertMicrosecToMillisec) {
  Microseconds t(5120);
  EXPECT_EQ(t.getMilliseconds(), 5);
}

// Tests for Nanoseconds
TEST(Time, DefaultNanosecIsZero) {
  Nanoseconds t;
  EXPECT_EQ(t.toRawNanoseconds(), 0);
}

TEST(Time, GetInitialNanosec) {
  Nanoseconds t(5);
  EXPECT_EQ(t.toRawNanoseconds(), 5);
}

TEST(Time, InitializeNanosecFromSec) {
  Seconds tSec(5);
  Nanoseconds tNano(tSec);
  EXPECT_EQ(tNano.toRawNanoseconds(), 5 * kOneSecondInNanoseconds);
}

TEST(Time, InitializeNanosecFromMillisec) {
  Milliseconds tMilli(5);
  Nanoseconds tNano(tMilli);
  EXPECT_EQ(tNano.toRawNanoseconds(), 5 * kOneMillisecondInNanoseconds);
}

TEST(Time, InitializeNanosecFromMicrosec) {
  Microseconds tMicro(5);
  Nanoseconds tNano(tMicro);
  EXPECT_EQ(tNano.toRawNanoseconds(), 5 * kOneMicrosecondInNanoseconds);
}

TEST(Time, NanosecEquals) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 == t3);
}

TEST(Time, NanosecNotEquals) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_FALSE(t1 != t2);
  EXPECT_TRUE(t1 != t3);
}

TEST(Time, AddSecToNanosec) {
  Nanoseconds t = Seconds(5) + Nanoseconds(6);
  EXPECT_EQ(t.toRawNanoseconds(), 5 * kOneSecondInNanoseconds + 6);
}

TEST(Time, AddSecToNanosecOverflow) {
  Nanoseconds t = Seconds(5) + Nanoseconds(UINT64_MAX);
  EXPECT_EQ(t.toRawNanoseconds(), 5 * kOneSecondInNanoseconds - 1);
}

TEST(Time, AddNanosecToNanosec) {
  Nanoseconds t = Nanoseconds(6) + Nanoseconds(5);
  EXPECT_EQ(t.toRawNanoseconds(), 11);
}

TEST(Time, AddNanosecToNanosecOverflow) {
  Nanoseconds t = Nanoseconds(6) + Nanoseconds(UINT64_MAX);
  EXPECT_EQ(t.toRawNanoseconds(), 5);
}

TEST(Time, SubtractNanosecFromNanosec) {
  Nanoseconds t = Nanoseconds(11) - Nanoseconds(5);
  EXPECT_EQ(t.toRawNanoseconds(), 6);
}

TEST(Time, SubtractNanosecFromNanosecOverflow) {
  Nanoseconds t = Nanoseconds(5) - Nanoseconds(11);
  EXPECT_EQ(t.toRawNanoseconds(), UINT64_MAX - 5);
}

TEST(Time, NanosecGreaterThanEqual) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_TRUE(t2 >= t1);
  EXPECT_TRUE(t3 >= t1);
  EXPECT_FALSE(t1 >= t3);
}

TEST(Time, NanosecLessThanEqual) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_TRUE(t2 <= t1);
  EXPECT_TRUE(t1 <= t3);
  EXPECT_FALSE(t3 <= t1);
}

TEST(Time, NanosecLessThan) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_FALSE(t2 < t1);
  EXPECT_FALSE(t3 < t1);
  EXPECT_TRUE(t1 < t3);
}

TEST(Time, NanosecGreaterThan) {
  Nanoseconds t1(5), t2(5), t3(6);
  EXPECT_FALSE(t2 > t1);
  EXPECT_TRUE(t3 > t1);
  EXPECT_FALSE(t1 > t3);
}
