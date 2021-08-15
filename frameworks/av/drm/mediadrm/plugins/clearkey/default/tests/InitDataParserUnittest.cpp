/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <string.h>

#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/base64.h>
#include <utils/String8.h>
#include <utils/Vector.h>

#include "InitDataParser.h"

namespace clearkeydrm {

using namespace android;

namespace {
    const size_t kKeyIdSize = 16;
    const String8 kCencMimeType("video/mp4");
    const String8 kWebmMimeType("video/webm");
    const String8 kBase64Padding("=");
}

class InitDataParserTest : public ::testing::Test {
  protected:
    status_t attemptParse(const Vector<uint8_t>& initData,
                          const String8& mimeType,
                          Vector<uint8_t>* licenseRequest) {
        InitDataParser parser;
        return parser.parse(initData, mimeType, licenseRequest);
    }

    void attemptParseExpectingSuccess(const Vector<uint8_t>& initData,
                                      const String8& mimeType,
                                      const Vector<String8>& expectedKeys) {
        const String8 kRequestPrefix("{\"kids\":[");
        const String8 kRequestSuffix("],\"type\":\"temporary\"}");
        Vector<uint8_t> request;
        ASSERT_EQ(android::OK, attemptParse(initData, mimeType, &request));

        String8 requestString(reinterpret_cast<const char*>(request.array()),
                              request.size());
        EXPECT_EQ(0, requestString.find(kRequestPrefix));
        EXPECT_EQ(requestString.size() - kRequestSuffix.size(),
                  (size_t)requestString.find(kRequestSuffix));
        for (size_t i = 0; i < expectedKeys.size(); ++i) {
            AString encodedIdAString;
            android::encodeBase64Url(expectedKeys[i], kKeyIdSize,
                                  &encodedIdAString);
            String8 encodedId(encodedIdAString.c_str());
            encodedId.removeAll(kBase64Padding);
            EXPECT_TRUE(requestString.contains(encodedId));
        }
    }

    void attemptParseExpectingFailure(const Vector<uint8_t>& initData,
                                      const String8& mimeType) {
        Vector<uint8_t> request;
        ASSERT_NE(android::OK, attemptParse(initData, mimeType, &request));
        EXPECT_EQ(0u, request.size());
    }
};

TEST_F(InitDataParserTest, ParsesSingleKeyPssh) {
    uint8_t pssh[52] = {
        0, 0, 0, 52,                                    // Total Size
        'p', 's', 's', 'h',                             // PSSH
        1, 0, 0, 0,                                     // Version
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02, // System ID
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b,
        0, 0, 0, 1,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 52);

    Vector<String8> expectedKeys;
    expectedKeys.push(String8("01234567890ABCDE"));

    attemptParseExpectingSuccess(initData, kCencMimeType, expectedKeys);
}

TEST_F(InitDataParserTest, ParsesMultipleKeyPssh) {
    uint8_t pssh[84] = {
        0, 0, 0, 84,                                    // Total Size
        'p', 's', 's', 'h',                             // PSSH
        1, 0, 0, 0,                                     // Version
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02, // System ID
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b,
        0, 0, 0, 3,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0x43, 0x6c, 0x65, 0x61, 0x72, 0x4b, 0x65, 0x79, // Key ID #2
        0x43, 0x6c, 0x65, 0x61, 0x72, 0x4b, 0x65, 0x79, //   "ClearKeyClearKey"
        0x20, 0x47, 0x4f, 0x4f, 0x47, 0x4c, 0x45, 0x20, // Key ID #3
        0x20, 0x47, 0x4f, 0x4f, 0x47, 0x4c, 0x45, 0x20, //   " GOOGLE  GOOGLE "
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 84);

    Vector<String8> expectedKeys;
    expectedKeys.push(String8("01234567890ABCDE"));
    expectedKeys.push(String8("ClearKeyClearKey"));
    expectedKeys.push(String8(" GOOGLE  GOOGLE "));

    attemptParseExpectingSuccess(initData, kCencMimeType, expectedKeys);
}

TEST_F(InitDataParserTest, ParsesWebM) {
    uint8_t initDataRaw[16] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
    };
    Vector<uint8_t> initData;
    initData.appendArray(initDataRaw, 16);

    Vector<String8> expectedKeys;
    expectedKeys.push(String8("01234567890ABCDE"));

    attemptParseExpectingSuccess(initData, kWebmMimeType, expectedKeys);
}

TEST_F(InitDataParserTest, FailsForPsshTooSmall) {
    uint8_t pssh[16] = {
        0, 0, 0, 52,
        'p', 's', 's', 'h',
        1, 0, 0, 0,
        0x10, 0x77, 0xef, 0xec
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 16);

    attemptParseExpectingFailure(initData, kCencMimeType);
}

TEST_F(InitDataParserTest, FailsForWebMTooSmall) {
    uint8_t initDataRaw[8] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
    };
    Vector<uint8_t> initData;
    initData.appendArray(initDataRaw, 8);

    attemptParseExpectingFailure(initData, kWebmMimeType);
}

TEST_F(InitDataParserTest, FailsForPsshBadSystemId) {
    uint8_t pssh[52] = {
        0, 0, 0, 52,                                    // Total Size
        'p', 's', 's', 'h',                             // PSSH
        1, 0, 0, 0,                                     // Version
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b, // System ID
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02,
        0, 0, 0, 1,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 52);

    attemptParseExpectingFailure(initData, kCencMimeType);
}

TEST_F(InitDataParserTest, FailsForPsshBadSize) {
    uint8_t pssh[52] = {
        0, 0, 70, 200,                                  // Total Size
        'p', 's', 's', 'h',                             // PSSH
        1, 0, 0, 0,                                     // Version
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02, // System ID
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b,
        0, 0, 0, 1,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 52);

    attemptParseExpectingFailure(initData, kCencMimeType);
}

TEST_F(InitDataParserTest, FailsForPsshWrongVersion) {
    uint8_t pssh[52] = {
        0, 0, 0, 52,                                    // Total Size
        'p', 's', 's', 'h',                             // PSSH
        0, 0, 0, 0,                                     // Version
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02, // System ID
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b,
        0, 0, 0, 1,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 52);

    attemptParseExpectingFailure(initData, kCencMimeType);
}

TEST_F(InitDataParserTest, FailsForPsshBadKeyCount) {
    uint8_t pssh[52] = {
        0, 0, 0, 52,                                    // Total Size
        'p', 's', 's', 'h',                             // PSSH
        1, 0, 0, 0,                                     // Version
        0x10, 0x77, 0xef, 0xec, 0xc0, 0xb2, 0x4d, 0x02, // System ID
        0xac, 0xe3, 0x3c, 0x1e, 0x52, 0xe2, 0xfb, 0x4b,
        0, 0, 0, 7,                                     // Key Count
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // Key ID #1
        0x38, 0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, //   "01234567890ABCDE"
        0, 0, 0, 0                                      // Data Size (always 0)
    };
    Vector<uint8_t> initData;
    initData.appendArray(pssh, 52);

    attemptParseExpectingFailure(initData, kCencMimeType);
}
}  // namespace clearkeydrm
