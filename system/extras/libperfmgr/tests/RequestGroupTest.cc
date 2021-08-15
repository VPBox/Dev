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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <thread>

#include <gtest/gtest.h>

#include "perfmgr/RequestGroup.h"

namespace android {
namespace perfmgr {

using namespace std::chrono_literals;

constexpr double kTIMING_TOLERANCE_MS = std::chrono::milliseconds(25).count();

// Test GetRequestValue()
TEST(RequestGroupTest, GetRequestValueTest) {
    std::string test_str = "TESTREQ_1";
    RequestGroup req(test_str);
    EXPECT_EQ(test_str, req.GetRequestValue());
}

// Test AddRequest()
TEST(RequestGroupTest, AddRequestTest) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration = 500ms;
    bool ret = req.AddRequest("INTERACTION", start + duration);
    EXPECT_EQ(true, ret);
    auto sleep_time = 200ms;
    std::this_thread::sleep_for(sleep_time);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR((duration - sleep_time).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
}

// Test AddRequest() with a huge expire time which could be done in some long
// persist power hint such as VR_MODE
TEST(RequestGroupTest, AddRequestNoExpireTest) {
    RequestGroup req("");
    bool ret = req.AddRequest("INTERACTION", ReqTime::max());
    EXPECT_EQ(true, ret);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    auto expect = std::chrono::duration_cast<std::chrono::milliseconds>(
        ReqTime::max() - std::chrono::steady_clock::now());
    EXPECT_NEAR(expect.count(), expire_time.count(), kTIMING_TOLERANCE_MS);
    // expire time is greater than 1 year
    EXPECT_LE(365 * 24 * 60 * 60 * 1000, expire_time.count());
    EXPECT_EQ(true, active);
}

// Test AddRequest() and expires
TEST(RequestGroupTest, AddRequestTestExpire) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration = 5ms;
    bool ret = req.AddRequest("INTERACTION", start + duration);
    EXPECT_EQ(true, ret);
    ret = req.AddRequest("INTERACTION", start + duration + 1ms);
    EXPECT_EQ(false, ret);
    std::this_thread::sleep_for(duration + 10ms);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_EQ(std::chrono::milliseconds::max(), expire_time);
    EXPECT_EQ(false, active);
}

// Test AddRequest() with new value
TEST(RequestGroupTest, AddRequestNewValue) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration = 5000ms;
    bool ret = req.AddRequest("INTERACTION", start + duration);
    EXPECT_EQ(true, ret);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR(duration.count(), expire_time.count(), kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
    // Add a request shorter than the current outstanding one, expiration time
    // not changed
    auto shorter_duration = 100ms;
    ret = req.AddRequest("INTERACTION", start + shorter_duration);
    EXPECT_EQ(false, ret);
    active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR(duration.count(), expire_time.count(), kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
    // Add a request longer than the current outstanding one, expiration time
    // changed
    duration = 10000ms;
    ret = req.AddRequest("INTERACTION", start + duration);
    EXPECT_EQ(false, ret);
    active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR(duration.count(), expire_time.count(), kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
}

// Test multiple AddRequest() with different hint_type
TEST(RequestGroupTest, AddRequestTestMutiple) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration_interact = 500ms;
    req.AddRequest("INTERACTION", start + duration_interact);
    auto duration_launch = 5000ms;
    req.AddRequest("LAUNCH", start + duration_launch);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR(std::min(duration_interact, duration_launch).count(),
                expire_time.count(), kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
}

// Test RemoveRequest()
TEST(RequestGroupTest, RemoveRequestTest) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration_interact = 500ms;
    req.AddRequest("INTERACTION", start + duration_interact);
    bool ret = req.RemoveRequest("INTERACTION");
    EXPECT_EQ(true, ret);
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_EQ(std::chrono::milliseconds::max(), expire_time);
    EXPECT_EQ(false, active);
    // Test removing an already-removed request
    ret = req.RemoveRequest("INTERACTION");
    EXPECT_EQ(false, ret);
}

// Test multiple RemoveRequest() with different hint_type
TEST(RequestGroupTest, RemoveRequestTestMutiple) {
    RequestGroup req("");
    auto start = std::chrono::steady_clock::now();
    auto duration_interact = 500ms;
    req.AddRequest("INTERACTION", start + duration_interact);
    auto duration_launch = 50000ms;
    req.AddRequest("LAUNCH", start + duration_launch);
    req.RemoveRequest("INTERACTION");
    std::chrono::milliseconds expire_time;
    bool active = req.GetExpireTime(&expire_time);
    EXPECT_NEAR(duration_launch.count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    EXPECT_EQ(true, active);
}

}  // namespace perfmgr
}  // namespace android
