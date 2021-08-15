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

#include <gtest/gtest.h>

#include <endian.h>
#include <hidl/HidlSupport.h>
#include <keymaster/logger.h>
#include <keymasterV4_0/keymaster_utils.h>

#include <keystore/keymaster_types.h>
#include <keystore/keystore_hidl_support.h>

#include "../auth_token_table.h"

using std::vector;

namespace keystore {

using android::hardware::hidl_array;
using android::hardware::hidl_vec;

namespace test {

namespace {

class StdoutLogger : public ::keymaster::Logger {
  public:
    StdoutLogger() { set_instance(this); }

    int log_msg(LogLevel level, const char* fmt, va_list args) const {
        int output_len = 0;
        switch (level) {
        case DEBUG_LVL:
            output_len = printf("DEBUG: ");
            break;
        case INFO_LVL:
            output_len = printf("INFO: ");
            break;
        case WARNING_LVL:
            output_len = printf("WARNING: ");
            break;
        case ERROR_LVL:
            output_len = printf("ERROR: ");
            break;
        case SEVERE_LVL:
            output_len = printf("SEVERE: ");
            break;
        }

        output_len += vprintf(fmt, args);
        output_len += printf("\n");
        return output_len;
    }
};

StdoutLogger logger;

}  // namespace

constexpr const uint8_t test_token[69] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
    0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44};

constexpr const uint8_t test_hmac_data[] = {
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44};

static const Km3HardwareAuthToken km3_hidl_test_token_little_endian = {
    UINT64_C(0x0807060504030201), UINT64_C(0x100f0e0d0c0b0a09),
    UINT64_C(0x1817161514131211), UINT32_C(0x1c1b1a19),
    UINT64_C(0x24232221201f1e1d), hidl_array<uint8_t, 32>(test_hmac_data)};

static const HardwareAuthToken km4_hidl_test_token = {
    UINT64_C(0x0807060504030201), UINT64_C(0x100f0e0d0c0b0a09),
    UINT64_C(0x1817161514131211), static_cast<HardwareAuthenticatorType>(UINT32_C(0x191a1b1c)),
    UINT64_C(0x1d1e1f2021222324), hidl_vec<uint8_t>(test_hmac_data, test_hmac_data + 32)};

TEST(AuthenticationTokenFormattingTest, hidlVec2Km3AuthToken) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(km3_hidl_test_token_little_endian, hidlVec2Km3AuthToken(hidl_test_token));
}

TEST(AuthenticationTokenFormattingTest, hidlVec2Km4AuthToken) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(km4_hidl_test_token, hidlVec2AuthToken(hidl_test_token));
}

TEST(AuthenticationTokenFormattingTest, km3AuthToken2HidlVec) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(hidl_test_token, authToken2HidlVec(km3_hidl_test_token_little_endian));
}

TEST(AuthenticationTokenFormattingTest, km4AuthToken2HidlVec) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(hidl_test_token, authToken2HidlVec(km4_hidl_test_token));
}

TEST(AuthenticationTokenFormattingTest, backAndForth) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(km3_hidl_test_token_little_endian,
              hidlVec2Km3AuthToken(authToken2HidlVec(km3_hidl_test_token_little_endian)));
    ASSERT_EQ(km4_hidl_test_token, hidlVec2AuthToken(authToken2HidlVec(km4_hidl_test_token)));
}

TEST(AuthenticationTokenFormattingTest, forthAndBack) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    ASSERT_EQ(hidl_test_token, authToken2HidlVec(hidlVec2Km3AuthToken(hidl_test_token)));
    ASSERT_EQ(hidl_test_token, authToken2HidlVec(hidlVec2Km3AuthToken(hidl_test_token)));
}

TEST(AuthenticationTokenFormattingTest, roundAndRound) {
    static_assert(sizeof(hw_auth_token_t) == sizeof(test_token), "test_token has wrong size");
    hidl_vec<uint8_t> hidl_test_token;
    hidl_test_token.setToExternal(const_cast<unsigned char*>(test_token), sizeof(test_token));
    HardwareAuthToken km4_from_hidl = hidlVec2AuthToken(hidl_test_token);
    hidl_vec<uint8_t> hidl_from_km4 = authToken2HidlVec(km4_from_hidl);
    Km3HardwareAuthToken km3_from_hidl = hidlVec2Km3AuthToken(hidl_from_km4);
    hidl_vec<uint8_t> hidl_from_km3 = authToken2HidlVec(km3_from_hidl);

    ASSERT_EQ(hidl_from_km4, hidl_test_token);
    ASSERT_EQ(hidl_from_km3, hidl_test_token);
    ASSERT_NE(km4_from_hidl.timestamp, km3_from_hidl.timestamp);
    ASSERT_NE(static_cast<uint32_t>(km4_from_hidl.authenticatorType),
              km3_from_hidl.authenticatorType);
}

}  // namespace test
}  // namespace keystore
