/*
 * Copyright 2018 The Android Open Source Project
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

#undef LOG_TAG
#define LOG_TAG "SchedulerUnittests"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>

#include "Scheduler/SchedulerUtils.h"

namespace android {
namespace scheduler {

class SchedulerUtilsTest : public testing::Test {
public:
    SchedulerUtilsTest() = default;
    ~SchedulerUtilsTest() override = default;
};

namespace {
TEST_F(SchedulerUtilsTest, calculate_mean) {
    std::array<int64_t, 30> testArray{};
    // Calling the function on empty array returns 0.
    EXPECT_EQ(0, calculate_mean(testArray));

    testArray[0] = 33;
    EXPECT_EQ(1, calculate_mean(testArray));
    testArray[1] = 33;
    testArray[2] = 33;
    EXPECT_EQ(3, calculate_mean(testArray));
    testArray[3] = 42;
    EXPECT_EQ(4, calculate_mean(testArray));
    testArray[4] = 33;
    EXPECT_EQ(5, calculate_mean(testArray));
    testArray[5] = 42;
    EXPECT_EQ(7, calculate_mean(testArray));
    for (int i = 6; i < 30; i++) {
        testArray[i] = 33;
    }
    EXPECT_EQ(33, calculate_mean(testArray));
}

TEST_F(SchedulerUtilsTest, calculate_median) {
    std::vector<int64_t> testVector;
    // Calling the function on empty vector returns 0.
    EXPECT_EQ(0, calculate_median(&testVector));

    testVector.push_back(33);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(33);
    testVector.push_back(33);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(33);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_median(&testVector));
    testVector.push_back(42);
    EXPECT_EQ(42, calculate_median(&testVector));
    testVector.push_back(60);
    EXPECT_EQ(42, calculate_median(&testVector));
    testVector.push_back(60);
    EXPECT_EQ(42, calculate_median(&testVector));
    testVector.push_back(33);
    EXPECT_EQ(42, calculate_median(&testVector));
    testVector.push_back(33);
    EXPECT_EQ(42, calculate_median(&testVector));
    testVector.push_back(33);
    EXPECT_EQ(33, calculate_median(&testVector));
}

TEST_F(SchedulerUtilsTest, calculate_mode) {
    std::vector<int64_t> testVector;
    // Calling the function on empty vector returns 0.
    EXPECT_EQ(0, calculate_mode(testVector));

    testVector.push_back(33);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(33);
    testVector.push_back(33);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(33);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(60);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(60);
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(33);
    // 5 occurences of 33.
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    // 5 occurences of 33, 5 occurences of 42. We choose the first one.
    EXPECT_EQ(33, calculate_mode(testVector));
    testVector.push_back(42);
    // 5 occurences of 33, 6 occurences of 42.
    EXPECT_EQ(42, calculate_mode(testVector));
    testVector.push_back(42);
    // 5 occurences of 33, 7 occurences of 42.
    EXPECT_EQ(42, calculate_mode(testVector));
}

} // namespace
} // namespace scheduler
} // namespace android