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
#include <utils/Log.h>

#include "gtest/gtest.h"

#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/AStringUtils.h>
#include <media/stagefright/foundation/base64.h>

#include <utils/RefBase.h>
#include <utils/String8.h>

namespace {
const android::String8 kBase64Padding("=");
};

namespace android {

class Base64Test : public ::testing::Test {
};

void verifyDecode(const AString* expected, const AString* in) {
    size_t numTests = 0;
    while (!expected[numTests].empty())
        ++numTests;

    for (size_t i = 0; i < numTests; ++i) {
        // Since android::decodeBase64() requires padding characters,
        // add them so length of encoded text is exactly a multiple of 4.
        int remainder = in[i].size() % 4;
        String8 paddedText(in[i].c_str());
        if (remainder > 0) {
            for (int i = 0; i < 4 - remainder; ++i) {
                paddedText.append(kBase64Padding);
            }
        }
        sp<ABuffer> result = decodeBase64(AString(paddedText.string()));

        ASSERT_EQ(AStringUtils::Compare(expected[i].c_str(),
                reinterpret_cast<char*>(result->data()),
                expected[i].size(), false), 0);
    }
}

void verifyEncode(const AString* expected, const AString* in) {
    size_t numTests = 0;
    while (!expected[numTests].empty())
        ++numTests;

    AString out = AString("");
    for (size_t i = 0; i < numTests; ++i) {
        encodeBase64Url(in[i].c_str(), in[i].size(), &out);

        ASSERT_EQ(AStringUtils::Compare(expected[i].c_str(), out.c_str(),
                expected[i].size(), false), 0);
    }
}

TEST_F(Base64Test, TestDecodeBase64) {
    const AString base64[] = {
        AString("SGVsbG8gRnJpZW5kIQ"),
        AString("R29vZCBkYXkh"),
        AString("")  // string to signal end of array
    };

    const AString clearText[] = {
        AString("Hello Friend!"),
        AString("Good day!"),
        AString("")
    };

    verifyDecode(clearText, base64);
}

TEST_F(Base64Test, TestDecodeBase64Url) {
    const AString base64Url[] = {
        AString("SGVsbG8gRnJpZW5kICE-Pw"),
        AString("SGVsbG8gRnJpZW5kICE_"),
        AString("SGVsbG8gPz4-IEZyaWVuZCA_Pg"),
        AString("")
    };

    const AString clearText[] = {
        AString("Hello Friend !>?"),
        AString("Hello Friend !?"),
        AString("Hello ?>> Friend ?>"),
        AString("")
    };

    verifyDecode(clearText, base64Url);
}

TEST_F(Base64Test, TestDecodeMalformedBase64) {
    const AString base64Url[] = {
        AString("1?GawgguFyGrWKav7AX4VKUg"),  // fail on parsing
        AString("GawgguFyGrWKav7AX4V???"),    // fail on length not multiple of 4
        AString("GawgguFyGrWKav7AX4VKUg"),    // ditto
    };

    for (size_t i = 0; i < 3; ++i) {
        sp<ABuffer> result = decodeBase64(AString(base64Url[i]));
        EXPECT_TRUE(result == nullptr);
    }
}

TEST_F(Base64Test, TestEncodeBase64) {
    const AString clearText[] = {
        AString("Hello Friend!"),
        AString("Good day!"),
        AString("")
    };

    const AString base64[] = {
        AString("SGVsbG8gRnJpZW5kIQ=="),
        AString("R29vZCBkYXkh"),
        AString("")
    };

    verifyEncode(base64, clearText);
}

TEST_F(Base64Test, TestEncodeBase64Url) {
    const AString clearText[] = {
        AString("Hello Friend !>?"),
        AString("Hello Friend !?"),
        AString("Hello ?>> Friend ?>"),
        AString("")
    };

    const AString base64Url[] = {
        AString("SGVsbG8gRnJpZW5kICE-Pw=="),
        AString("SGVsbG8gRnJpZW5kICE_"),
        AString("SGVsbG8gPz4-IEZyaWVuZCA_Pg"),
        AString("")
    };

    verifyEncode(base64Url, clearText);
}

} // namespace android
