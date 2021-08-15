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

#include <gtest/gtest.h>

#include "minikin/FontCollection.h"
#include "minikin/LayoutPieces.h"

#include "FontTestUtils.h"
#include "LayoutSplitter.h"
#include "UnicodeUtils.h"

namespace minikin {
namespace {

std::pair<std::vector<uint16_t>, Range> parseTestString(const std::string& text) {
    auto utf16 = utf8ToUtf16(text);
    Range range;
    std::vector<uint16_t> outText;
    for (uint16_t c : utf16) {
        switch (c) {
            case '(':
                range.setStart(outText.size());
                break;
            case ')':
                range.setEnd(outText.size());
                break;
            default:
                outText.push_back(c);
        }
    }
    return std::make_pair(outText, range);
}

std::pair<Range, Range> parseExpectString(const std::string& text) {
    auto utf16 = utf8ToUtf16(text);
    Range context;
    Range piece;
    uint32_t textPos = 0;
    for (uint16_t c : utf16) {
        switch (c) {
            case '[':
                context.setStart(textPos);
                break;
            case ']':
                context.setEnd(textPos);
                break;
            case '(':
                piece.setStart(textPos);
                break;
            case ')':
                piece.setEnd(textPos);
                break;
            default:
                textPos++;
        }
    }
    return std::make_pair(context, piece);
}

std::string buildDebugString(const U16StringPiece& textBuf, const Range& context,
                             const Range& piece) {
    std::vector<uint16_t> out;
    out.reserve(textBuf.size() + 4);
    for (uint32_t i = 0; i < textBuf.size() + 1; ++i) {
        if (i == context.getStart()) {
            out.push_back('[');
        }
        if (i == piece.getStart()) {
            out.push_back('(');
        }
        if (i == piece.getEnd()) {
            out.push_back(')');
        }
        if (i == context.getEnd()) {
            out.push_back(']');
        }
        if (i != textBuf.size()) {
            out.push_back(textBuf[i]);
        }
    }
    return utf16ToUtf8(out);
}

TEST(LayoutSplitterTest, LTR_Latin) {
    struct TestCase {
        std::string testStr;
        std::vector<std::string> expects;
    } testCases[] = {
            {"(This is an example text.)",
             {
                     "[(This)] is an example text.", "This[( )]is an example text.",
                     "This [(is)] an example text.", "This is[( )]an example text.",
                     "This is [(an)] example text.", "This is an[( )]example text.",
                     "This is an [(example)] text.", "This is an example[( )]text.",
                     "This is an example [(text.)]",
             }},
            {"This( is an example )text.",
             {
                     "This[( )]is an example text.", "This [(is)] an example text.",
                     "This is[( )]an example text.", "This is [(an)] example text.",
                     "This is an[( )]example text.", "This is an [(example)] text.",
                     "This is an example[( )]text.",
             }},
            {"This (is an example) text.",
             {
                     "This [(is)] an example text.", "This is[( )]an example text.",
                     "This is [(an)] example text.", "This is an[( )]example text.",
                     "This is an [(example)] text.",
             }},
            {"Th(is is an example te)xt.",
             {
                     "[Th(is)] is an example text.", "This[( )]is an example text.",
                     "This [(is)] an example text.", "This is[( )]an example text.",
                     "This is [(an)] example text.", "This is an[( )]example text.",
                     "This is an [(example)] text.", "This is an example[( )]text.",
                     "This is an example [(te)xt.]",
             }},
            {"This is an ex(amp)le text.",
             {
                     "This is an [ex(amp)le] text.",
             }},
            {"There are (three   spaces.)",
             {
                     "There are [(three)]   spaces.", "There are three[( )]  spaces.",
                     "There are three [( )] spaces.", "There are three  [( )]spaces.",
                     "There are three   [(spaces.)]",
             }},
    };

    for (const auto& testCase : testCases) {
        auto[text, range] = parseTestString(testCase.testStr);
        uint32_t expectationIndex = 0;
        for (auto[acContext, acPiece] : LayoutSplitter(text, range, false /* isRtl */)) {
            ASSERT_NE(expectationIndex, testCase.expects.size());
            const std::string expectString = testCase.expects[expectationIndex++];
            auto[exContext, exPiece] = parseExpectString(expectString);
            EXPECT_EQ(acContext, exContext)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
            EXPECT_EQ(acPiece, exPiece)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
        }
        EXPECT_EQ(expectationIndex, testCase.expects.size()) << "Expectations Remains";
    }
}

TEST(LayoutSplitterTest, RTL_Latin) {
    struct TestCase {
        std::string testStr;
        std::vector<std::string> expects;
    } testCases[] = {
            {"(This is an example text.)",
             {
                     "This is an example [(text.)]", "This is an example[( )]text.",
                     "This is an [(example)] text.", "This is an[( )]example text.",
                     "This is [(an)] example text.", "This is[( )]an example text.",
                     "This [(is)] an example text.", "This[( )]is an example text.",
                     "[(This)] is an example text.",
             }},
            {"This( is an example )text.",
             {
                     "This is an example[( )]text.", "This is an [(example)] text.",
                     "This is an[( )]example text.", "This is [(an)] example text.",
                     "This is[( )]an example text.", "This [(is)] an example text.",
                     "This[( )]is an example text.",
             }},
            {"This (is an example) text.",
             {
                     "This is an [(example)] text.", "This is an[( )]example text.",
                     "This is [(an)] example text.", "This is[( )]an example text.",
                     "This [(is)] an example text.",
             }},
            {"Th(is is an example te)xt.",
             {
                     "This is an example [(te)xt.]", "This is an example[( )]text.",
                     "This is an [(example)] text.", "This is an[( )]example text.",
                     "This is [(an)] example text.", "This is[( )]an example text.",
                     "This [(is)] an example text.", "This[( )]is an example text.",
                     "[Th(is)] is an example text.",
             }},
            {"This is an ex(amp)le text.",
             {
                     "This is an [ex(amp)le] text.",
             }},
            {"There are (three   spaces.)",
             {
                     "There are three   [(spaces.)]", "There are three  [( )]spaces.",
                     "There are three [( )] spaces.", "There are three[( )]  spaces.",
                     "There are [(three)]   spaces.",
             }},
    };

    for (const auto& testCase : testCases) {
        auto[text, range] = parseTestString(testCase.testStr);
        uint32_t expectationIndex = 0;
        for (auto[acContext, acPiece] : LayoutSplitter(text, range, true /* isRtl */)) {
            ASSERT_NE(expectationIndex, testCase.expects.size());
            const std::string expectString = testCase.expects[expectationIndex++];
            auto[exContext, exPiece] = parseExpectString(expectString);
            EXPECT_EQ(acContext, exContext)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
            EXPECT_EQ(acPiece, exPiece)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
        }
        EXPECT_EQ(expectationIndex, testCase.expects.size()) << "Expectations Remains";
    }
}

TEST(LayoutSplitterTest, LTR_CJK) {
    struct TestCase {
        std::string testStr;
        std::vector<std::string> expects;
    } testCases[] = {
            {// All Kanji text
             "(\u6614\u8005\u8358\u5468\u5922\u70BA\u80E1\u8776)",
             {
                     "[(\u6614)]\u8005\u8358\u5468\u5922\u70BA\u80E1\u8776",
                     "\u6614[(\u8005)]\u8358\u5468\u5922\u70BA\u80E1\u8776",
                     "\u6614\u8005[(\u8358)]\u5468\u5922\u70BA\u80E1\u8776",
                     "\u6614\u8005\u8358[(\u5468)]\u5922\u70BA\u80E1\u8776",
                     "\u6614\u8005\u8358\u5468[(\u5922)]\u70BA\u80E1\u8776",
                     "\u6614\u8005\u8358\u5468\u5922[(\u70BA)]\u80E1\u8776",
                     "\u6614\u8005\u8358\u5468\u5922\u70BA[(\u80E1)]\u8776",
                     "\u6614\u8005\u8358\u5468\u5922\u70BA\u80E1[(\u8776)]",
             }},
            {// Japanese text like as follows
             // [Kanji][Kanji][Kana][Kanji][Kanji][Kana][Kana][Kana]
             "(\u672C\u65E5\u306F\u6674\u5929\u306A\u308A\u3002)",
             {
                     "[(\u672C)]\u65E5\u306F\u6674\u5929\u306A\u308A\u3002",
                     "\u672C[(\u65E5\u306F)]\u6674\u5929\u306A\u308A\u3002",
                     "\u672C\u65E5\u306F[(\u6674)]\u5929\u306A\u308A\u3002",
                     "\u672C\u65E5\u306F\u6674[(\u5929\u306A\u308A\u3002)]",
             }},
            {// Japanese text like as follows
             // [Kanji][Kanji][Kana][Kanji][Kanji][Kana][Kana][Kana]
             "\u672C\u65E5(\u306F\u6674\u5929\u306A)\u308A\u3002",
             {
                     "\u672C[\u65E5(\u306F)]\u6674\u5929\u306A\u308A\u3002",
                     "\u672C\u65E5\u306F[(\u6674)]\u5929\u306A\u308A\u3002",
                     "\u672C\u65E5\u306F\u6674[(\u5929\u306A)\u308A\u3002]",
             }},
    };

    for (const auto& testCase : testCases) {
        auto[text, range] = parseTestString(testCase.testStr);
        uint32_t expectationIndex = 0;
        for (auto[acContext, acPiece] : LayoutSplitter(text, range, false /* isRtl */)) {
            ASSERT_NE(expectationIndex, testCase.expects.size());
            const std::string expectString = testCase.expects[expectationIndex++];
            auto[exContext, exPiece] = parseExpectString(expectString);
            EXPECT_EQ(acContext, exContext)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
            EXPECT_EQ(acPiece, exPiece)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
        }
        EXPECT_EQ(expectationIndex, testCase.expects.size()) << "Expectations Remains";
    }
}

TEST(LayoutSplitterTest, RTL_CJK) {
    struct TestCase {
        std::string testStr;
        std::vector<std::string> expects;
    } testCases[] = {
            {// All Kanji text
             "(\u6614\u8005\u8358\u5468\u5922\u70BA\u80E1\u8776)",
             {
                     "\u6614\u8005\u8358\u5468\u5922\u70BA\u80E1[(\u8776)]",
                     "\u6614\u8005\u8358\u5468\u5922\u70BA[(\u80E1)]\u8776",
                     "\u6614\u8005\u8358\u5468\u5922[(\u70BA)]\u80E1\u8776",
                     "\u6614\u8005\u8358\u5468[(\u5922)]\u70BA\u80E1\u8776",
                     "\u6614\u8005\u8358[(\u5468)]\u5922\u70BA\u80E1\u8776",
                     "\u6614\u8005[(\u8358)]\u5468\u5922\u70BA\u80E1\u8776",
                     "\u6614[(\u8005)]\u8358\u5468\u5922\u70BA\u80E1\u8776",
                     "[(\u6614)]\u8005\u8358\u5468\u5922\u70BA\u80E1\u8776",
             }},
            {// Japanese text like as follows
             // [Kanji][Kanji][Kana][Kanji][Kanji][Kana][Kana][Kana]
             "(\u672C\u65E5\u306F\u6674\u5929\u306A\u308A\u3002)",
             {
                     "\u672C\u65E5\u306F\u6674[(\u5929\u306A\u308A\u3002)]",
                     "\u672C\u65E5\u306F[(\u6674)]\u5929\u306A\u308A\u3002",
                     "\u672C[(\u65E5\u306F)]\u6674\u5929\u306A\u308A\u3002",
                     "[(\u672C)]\u65E5\u306F\u6674\u5929\u306A\u308A\u3002",
             }},
            {// Japanese text like as follows
             // [Kanji][Kanji][Kana][Kanji][Kanji][Kana][Kana][Kana]
             "\u672C\u65E5(\u306F\u6674\u5929\u306A)\u308A\u3002",
             {
                     "\u672C\u65E5\u306F\u6674[(\u5929\u306A)\u308A\u3002]",
                     "\u672C\u65E5\u306F[(\u6674)]\u5929\u306A\u308A\u3002",
                     "\u672C[\u65E5(\u306F)]\u6674\u5929\u306A\u308A\u3002",
             }},
    };

    for (const auto& testCase : testCases) {
        auto[text, range] = parseTestString(testCase.testStr);
        uint32_t expectationIndex = 0;
        for (auto[acContext, acPiece] : LayoutSplitter(text, range, true /* isRtl */)) {
            ASSERT_NE(expectationIndex, testCase.expects.size());
            const std::string expectString = testCase.expects[expectationIndex++];
            auto[exContext, exPiece] = parseExpectString(expectString);
            EXPECT_EQ(acContext, exContext)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
            EXPECT_EQ(acPiece, exPiece)
                    << expectString << " vs " << buildDebugString(text, acContext, acPiece);
        }
        EXPECT_EQ(expectationIndex, testCase.expects.size()) << "Expectations Remains";
    }
}

}  // namespace
}  // namespace minikin
