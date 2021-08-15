/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "BidiUtils.h"

#include <gtest/gtest.h>

#include "minikin/Range.h"

#include "UnicodeUtils.h"

namespace minikin {

const char LTR_1[] = "Hello, World";
const char RTL_1[] = "\u0627\u0644\u0633\u0644\u0627\u0645\u0020\u0639\u0644\u064A\u0643\u0645";
const char LTR_2[] = "Hello, Android";
const char RTL_2[] = "\u0639\u0644\u064A\u0643\u0645\u0020\u0627\u0644\u0633\u0644\u0627\u0645";

TEST(BidiUtilsTest, AllLTRCharText) {
    auto text = utf8ToUtf16(LTR_1);
    uint32_t ltrLength = text.size();
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, ltrLength), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

TEST(BidiUtilsTest, AllRTLCharText) {
    auto text = utf8ToUtf16(RTL_1);
    uint32_t rtlLength = text.size();
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        BidiText bidiText(text, Range(0, rtlLength), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

TEST(BidiUtilsTest, LTR_RTL_CharText) {
    auto text = utf8ToUtf16(std::string(LTR_1) + RTL_1);
    uint32_t ltrLength = utf8ToUtf16(LTR_1).size();
    uint32_t rtlLength = utf8ToUtf16(RTL_1).size();
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltrLength, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : R2 R1 L1 L2
        BidiText bidiText(text, Range(0, text.size()), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltrLength, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltrLength, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltrLength, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : L1 L2 R1 R2
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2
        // Visual Run : R2 R1 L2 L1
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

TEST(BidiUtilsTest, RTL_LTR_CharText) {
    auto text = utf8ToUtf16(std::string(RTL_1) + LTR_1);
    uint32_t ltrLength = utf8ToUtf16(LTR_1).size();
    uint32_t rtlLength = utf8ToUtf16(RTL_1).size();
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : R2 R1 L1 L2
        BidiText bidiText(text, Range(0, text.size()), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtlLength, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtlLength, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtlLength, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtlLength, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : R1 R2 L1 L2
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength + rtlLength), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2
        // Visual Run : L2 L1 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltrLength + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

TEST(BidiUtilsTest, LTR_RTL_LTR_CharText) {
    auto text = utf8ToUtf16(std::string(LTR_1) + RTL_1 + LTR_2);
    uint32_t ltr1Length = utf8ToUtf16(LTR_1).size();
    uint32_t ltr2Length = utf8ToUtf16(LTR_2).size();
    uint32_t rtlLength = utf8ToUtf16(RTL_1).size();
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L1 L2 R2 R1 L3 L4
        BidiText bidiText(text, Range(0, text.size()), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length, ltr1Length + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length + rtlLength, ltr1Length + rtlLength + ltr2Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L3 L4 R2 R1 L1 2L
        BidiText bidiText(text, Range(0, text.size()), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length + rtlLength, text.size()), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length, ltr1Length + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L1 L2 R2 R1 L3 L4
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length, ltr1Length + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length + rtlLength, ltr1Length + rtlLength + ltr2Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L1 L2 R2 R1 L3 L4
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length, ltr1Length + rtlLength), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(ltr1Length + rtlLength, ltr1Length + rtlLength + ltr2Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L1 L2 R2 R1 L3 L4
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length + rtlLength + ltr2Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: L1 L2 R1 R2 L3 L4
        // Visual Run : L1 L2 R2 R1 L3 L4
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, ltr1Length + rtlLength + ltr2Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

TEST(BidiUtilsTest, RTL_LTR_RTL_CharText) {
    auto text = utf8ToUtf16(std::string(RTL_1) + LTR_1 + RTL_2);
    uint32_t ltrLength = utf8ToUtf16(LTR_1).size();
    uint32_t rtl1Length = utf8ToUtf16(RTL_1).size();
    uint32_t rtl2Length = utf8ToUtf16(RTL_2).size();
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R2 R1 L1 L2 R4 R3
        BidiText bidiText(text, Range(0, text.size()), Bidi::LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length, ltrLength + rtl1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length + ltrLength, text.size()), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R4 R3 L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length + ltrLength, text.size()), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length, ltrLength + rtl1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R4 R3 L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length + ltrLength, text.size()), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length, ltrLength + rtl1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R4 R3 L1 L2 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::DEFAULT_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length + ltrLength, text.size()), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(rtl1Length, ltrLength + rtl1Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R1 R2 L1 L2 R3 R4
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_LTR);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length + ltrLength + rtl2Length), (*it).range);
        EXPECT_FALSE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
    {
        // Logical Run: R1 R2 L1 L2 R3 R4
        // Visual Run : R4 R3 L2 L1 R2 R1
        BidiText bidiText(text, Range(0, text.size()), Bidi::FORCE_RTL);
        auto it = bidiText.begin();
        EXPECT_NE(bidiText.end(), it);
        EXPECT_EQ(Range(0, rtl1Length + ltrLength + rtl2Length), (*it).range);
        EXPECT_TRUE((*it).isRtl);
        ++it;
        EXPECT_EQ(bidiText.end(), it);
    }
}

}  // namespace minikin
