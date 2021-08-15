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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <cstddef>

#include "android-base/file.h"
#include "android-base/test_utils.h"
#include <gtest/gtest.h>

#include <binder/Parcel.h>
#include <binder/TextOutput.h>
#include <binder/Debug.h>

static void CheckMessage(CapturedStderr& cap,
                         const char* expected,
                         bool singleline) {
    cap.Stop();
    std::string output = cap.str();
    if (singleline)
        output.erase(std::remove(output.begin(), output.end(), '\n'));
    ASSERT_EQ(output, expected);
}

#define CHECK_LOG_(input, expect, singleline)    \
{                                                \
    CapturedStderr cap;                          \
    android::aerr << input << android::endl;     \
    CheckMessage(cap, expect, singleline);       \
}                                                \

#define CHECK_VAL_(val, singleline)              \
{                                                \
    std::stringstream ss;                        \
    ss << val;                                   \
    std::string s = ss.str();                    \
    CHECK_LOG_(val, s.c_str(), singleline);      \
}                                                \

#define CHECK_LOG(input, expect) CHECK_LOG_(input, expect, true)
#define CHECK_VAL(val) CHECK_VAL_(val, true)

TEST(TextOutput, HandlesStdEndl) {
    CapturedStderr cap;
    android::aerr << "foobar" << std::endl;
    cap.Stop();
    ASSERT_EQ(cap.str(), "foobar\n");
}

TEST(TextOutput, HandlesCEndl) {
    CapturedStderr cap;
    android::aerr << "foobar" << "\n";
    cap.Stop();
    ASSERT_EQ(cap.str(), "foobar\n");
}

TEST(TextOutput, HandlesAndroidEndl) {
    CapturedStderr cap;
    android::aerr << "foobar" << android::endl;
    cap.Stop();
    ASSERT_EQ(cap.str(), "foobar\n");
}

TEST(TextOutput, HandleEmptyString) {
    CHECK_LOG("", "");
}

TEST(TextOutput, HandleString) {
    CHECK_LOG("foobar", "foobar");
}

TEST(TextOutput, HandleNum) {
    CHECK_LOG(12345, "12345");
}

TEST(TextOutput, HandleBool) {
    CHECK_LOG(false, "false");
}

TEST(TextOutput, HandleChar) {
    CHECK_LOG('T', "T");
}

TEST(TextOutput, HandleParcel) {
    android::Parcel val;
    CHECK_LOG(val, "Parcel(NULL)");
}

TEST(TextOutput, HandleHexDump) {
    const char buf[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    android::HexDump val(buf, sizeof(buf));
    CHECK_LOG(val, "03020100 07060504 0b0a0908 0f0e0d0c '................'");
}

TEST(TextOutput, HandleHexDumpCustom) {
    const char buf[4] = {0x11,0x22,0x33,0x44};
    android::HexDump val(buf, sizeof(buf), 4);
    CHECK_LOG(val, "11 22 33 44 '.\"3D'");
}

TEST(TextOutput, HandleTypeCode) {
    android::TypeCode val(1234);
    CHECK_LOG(val, "'\\x04\\xd2'");
}

TEST(TextOutput, HandleCookie) {
    int32_t val = 321; //0x141
    CHECK_LOG((void*)(long)val, "0x141");
}

TEST(TextOutput, HandleString8) {
    android::String8 val("foobar");
    CHECK_LOG(val, "foobar");
}

TEST(TextOutput, HandleString16) {
    android::String16 val("foobar");
    CHECK_LOG(val, "foobar");
}

template <typename T>
class TextTest : public testing::Test {};

typedef testing::Types<short, unsigned short,
                       int, unsigned int,
                       long, unsigned long,
                       long long, unsigned long long,
                       float, double, long double> TestTypes;
TYPED_TEST_CASE(TextTest, TestTypes);

TYPED_TEST(TextTest, TextMax)
{
    TypeParam max = std::numeric_limits<TypeParam>::max();
    CHECK_VAL(max);
}

TYPED_TEST(TextTest, TestMin)
{
    TypeParam min = std::numeric_limits<TypeParam>::min();
    CHECK_VAL(min);
}

TYPED_TEST(TextTest, TestDenom)
{
    TypeParam min = std::numeric_limits<TypeParam>::denorm_min();
    CHECK_VAL(min);
}

TYPED_TEST(TextTest, TestEpsilon)
{
    TypeParam eps = std::numeric_limits<TypeParam>::epsilon();
    CHECK_VAL(eps);
}
