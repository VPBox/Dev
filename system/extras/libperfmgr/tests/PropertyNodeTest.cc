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

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>

#include <gtest/gtest.h>

#include "perfmgr/PropertyNode.h"

namespace android {
namespace perfmgr {

using namespace std::chrono_literals;

constexpr double kTIMING_TOLERANCE_MS = std::chrono::milliseconds(25).count();
constexpr auto kSLEEP_TOLERANCE_MS = 2ms;

static inline void _VerifyPropertyValue(const std::string& path,
                                        const std::string& value) {
    std::string s = android::base::GetProperty(path, "");
    EXPECT_EQ(value, s);
}

static inline const std::string _InitProperty(const std::string& path) {
    EXPECT_TRUE(android::base::SetProperty(path, ""))
        << "failed to clear property";
    return path;
}

// Test init with no default value
TEST(PropertyNodeTest, NoInitDefaultTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 1, false);
    _VerifyPropertyValue(key, "");
}

// Test init with default value
TEST(PropertyNodeTest, InitDefaultTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 1, true);
    _VerifyPropertyValue(key, "value1");
    std::string key2 = _InitProperty("test.libperfmgr.key2");
    PropertyNode t2("t2", key2, {{"value0"}, {"value1"}, {"value2"}}, 0, true);
    _VerifyPropertyValue(key2, "value0");
}

// Test DumpToFd
TEST(PropertyNodeTest, DumpToFdTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("test_dump", key, {{"value0"}, {"value1"}, {"value2"}}, 1,
                   true);
    TemporaryFile dumptf;
    t.DumpToFd(dumptf.fd);
    fsync(dumptf.fd);
    std::string buf(
        android::base::StringPrintf("test_dump\t%s\t1\tvalue1\n", key.c_str()));
    std::string s;
    EXPECT_TRUE(android::base::ReadFileToString(dumptf.path, &s))
        << strerror(errno);
    EXPECT_EQ(buf, s);
}

// Test GetValueIndex
TEST(PropertyNodeTest, GetValueIndexTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 1, false);
    std::size_t index = 0;
    EXPECT_TRUE(t.GetValueIndex("value2", &index));
    EXPECT_EQ(2u, index);
    index = 1234;
    EXPECT_FALSE(t.GetValueIndex("NON_EXIST", &index));
    EXPECT_EQ(1234u, index);
}

// Test GetValues
TEST(PropertyNodeTest, GetValuesTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 1, false);
    std::vector values = t.GetValues();
    EXPECT_EQ(3u, values.size());
    EXPECT_EQ("value0", values[0]);
    EXPECT_EQ("value1", values[1]);
    EXPECT_EQ("value2", values[2]);
}

// Test get more properties
TEST(PropertyNodeTest, GetPropertiesTest) {
    std::string test_name = "TESTREQ_1";
    std::string test_path = "TEST_PATH";
    PropertyNode t(test_name, test_path, {}, 0, false);
    EXPECT_EQ(test_name, t.GetName());
    EXPECT_EQ(test_path, t.GetPath());
    EXPECT_EQ(0u, t.GetValues().size());
    EXPECT_EQ(0u, t.GetDefaultIndex());
    EXPECT_FALSE(t.GetResetOnInit());
}

// Test add request
TEST(PropertyNodeTest, AddRequestTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {""}}, 2, true);
    auto start = std::chrono::steady_clock::now();
    EXPECT_TRUE(t.AddRequest(1, "INTERACTION", start + 500ms));
    std::chrono::milliseconds expire_time = t.Update(true);
    // Add request @ value1
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(500).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Add request @ value0 higher prio than value1
    EXPECT_TRUE(t.AddRequest(0, "LAUNCH", start + 200ms));
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value0");
    EXPECT_NEAR(std::chrono::milliseconds(200).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Let high prio request timeout, now only request @ value1 active
    std::this_thread::sleep_for(expire_time + kSLEEP_TOLERANCE_MS);
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(300).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Let all requests timeout, now default value2
    std::this_thread::sleep_for(expire_time + kSLEEP_TOLERANCE_MS);
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "");
    EXPECT_EQ(std::chrono::milliseconds::max(), expire_time);
}

// Test remove request
TEST(PropertyNodeTest, RemoveRequestTest) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 2, true);
    auto start = std::chrono::steady_clock::now();
    EXPECT_TRUE(t.AddRequest(1, "INTERACTION", start + 500ms));
    std::chrono::milliseconds expire_time = t.Update(true);
    // Add request @ value1
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(500).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Add request @ value0 higher prio than value1
    EXPECT_TRUE(t.AddRequest(0, "LAUNCH", start + 200ms));
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value0");
    EXPECT_NEAR(std::chrono::milliseconds(200).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Remove high prio request, now only request @ value1 active
    t.RemoveRequest("LAUNCH");
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(500).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Remove request, now default value2
    t.RemoveRequest("INTERACTION");
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value2");
    EXPECT_EQ(std::chrono::milliseconds::max(), expire_time);
}

// Test add request
TEST(PropertyNodeTest, AddRequestTestOverride) {
    std::string key = _InitProperty("test.libperfmgr.key");
    PropertyNode t("t", key, {{"value0"}, {"value1"}, {"value2"}}, 2, true);
    auto start = std::chrono::steady_clock::now();
    EXPECT_TRUE(t.AddRequest(1, "INTERACTION", start + 500ms));
    std::chrono::milliseconds expire_time = t.Update(true);
    // Add request @ value1
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(500).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Add request @ value0 higher prio than value1
    EXPECT_TRUE(t.AddRequest(0, "LAUNCH", start + 200ms));
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value0");
    EXPECT_NEAR(std::chrono::milliseconds(200).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Add request @ value0 shorter
    EXPECT_TRUE(t.AddRequest(0, "LAUNCH", start + 100ms));
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value0");
    EXPECT_NEAR(std::chrono::milliseconds(200).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Add request @ value0 longer
    EXPECT_TRUE(t.AddRequest(0, "LAUNCH", start + 300ms));
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value0");
    EXPECT_NEAR(std::chrono::milliseconds(300).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Remove high prio request, now only request @ value1 active
    t.RemoveRequest("LAUNCH");
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value1");
    EXPECT_NEAR(std::chrono::milliseconds(500).count(), expire_time.count(),
                kTIMING_TOLERANCE_MS);
    // Remove request, now default value2
    t.RemoveRequest("INTERACTION");
    expire_time = t.Update(true);
    _VerifyPropertyValue(key, "value2");
    EXPECT_EQ(std::chrono::milliseconds::max(), expire_time);
}

}  // namespace perfmgr
}  // namespace android
