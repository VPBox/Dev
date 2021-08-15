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

#include <memory>

#include <gtest/gtest.h>

#include "minikin/Hyphenator.h"

#include "FileUtils.h"
#include "FontTestUtils.h"
#include "GreedyLineBreaker.h"
#include "HyphenatorMap.h"
#include "LineBreakerTestHelper.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"
#include "UnicodeUtils.h"
#include "WordBreaker.h"

namespace minikin {
namespace {

using line_breaker_test_helper::ConstantRun;
using line_breaker_test_helper::LineBreakExpectation;
using line_breaker_test_helper::RectangleLineWidth;
using line_breaker_test_helper::sameLineBreak;
using line_breaker_test_helper::toString;

// The ascent/descent of Ascii.ttf with text size = 10.
constexpr float ASCENT = -80.0f;
constexpr float DESCENT = 20.0f;

// The ascent/descent of CustomExtent.ttf with text size = 10.
constexpr float CUSTOM_ASCENT = -160.0f;
constexpr float CUSTOM_DESCENT = 40.0f;

class GreedyLineBreakerTest : public testing::Test {
public:
    GreedyLineBreakerTest() {}

    virtual ~GreedyLineBreakerTest() {}

    virtual void SetUp() override {
        mHyphenationPattern = readWholeFile("/system/usr/hyphen-data/hyph-en-us.hyb");
        Hyphenator* hyphenator = Hyphenator::loadBinary(
                mHyphenationPattern.data(), 2 /* min prefix */, 2 /* min suffix */, "en-US");
        HyphenatorMap::add("en-US", hyphenator);
        HyphenatorMap::add("pl", Hyphenator::loadBinary(nullptr, 0, 0, "pl"));
    }

    virtual void TearDown() override { HyphenatorMap::clear(); }

protected:
    LineBreakResult doLineBreak(const U16StringPiece& textBuffer, bool doHyphenation,
                                float lineWidth) {
        return doLineBreak(textBuffer, doHyphenation, "en-US", lineWidth);
    }

    LineBreakResult doLineBreak(const U16StringPiece& textBuffer, bool doHyphenation,
                                const std::string& lang, float lineWidth) {
        MeasuredTextBuilder builder;
        auto family1 = buildFontFamily("Ascii.ttf");
        auto family2 = buildFontFamily("CustomExtent.ttf");
        std::vector<std::shared_ptr<FontFamily>> families = {family1, family2};
        auto fc = std::make_shared<FontCollection>(families);
        MinikinPaint paint(fc);
        paint.size = 10.0f;  // Make 1em=10px
        paint.localeListId = LocaleListCache::getId(lang);
        builder.addStyleRun(0, textBuffer.size(), std::move(paint), false);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuffer, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(lineWidth);
        TabStops tabStops(nullptr, 0, 10);
        return breakLineGreedy(textBuffer, *measuredText, rectangleLineWidth, tabStops,
                               doHyphenation);
    }

private:
    std::vector<uint8_t> mHyphenationPattern;
};

TEST_F(GreedyLineBreakerTest, testBreakWithoutHyphenation) {
    constexpr bool NO_HYPHEN = false;  // No hyphenation in this test case.
    const std::vector<uint16_t> textBuf = utf8ToUtf16("This is an example text.");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    // Note that disable clang-format everywhere since aligned expectation is more readable.
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 240;
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 230;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an example ", 180, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."              ,  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 80;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "     , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 70;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "     , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 60;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "exampl", 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e "    , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "examp" , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "le "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "exam"  , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ple "  , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text"  , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."     , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 30;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Thi" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "s "  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "exa" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mpl" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e "  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "tex" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t."  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 20;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Th" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ex" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "am" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "pl" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e " , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "te" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xt" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "T" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "h" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "i" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "s ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "i" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "s ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "x" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "m" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "p" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "l" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "x" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "." , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testBreakWithHyphenation) {
    constexpr bool NO_HYPHEN = true;  // Do hyphenation in this test case.
    // "hyphenation" is hyphnated to "hy-phen-a-tion".
    const std::vector<uint16_t> textBuf = utf8ToUtf16("Hyphenation is hyphenation.");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit END_HYPHEN = EndHyphenEdit::INSERT_HYPHEN;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    // Note that disable clang-format everywhere since aligned expectation is more readable.
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"Hyphenation is hyphenation.", 270, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT,
                 DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 270;
        std::vector<LineBreakExpectation> expect = {
                {"Hyphenation is hyphenation.", 270, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT,
                 DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 260;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphenation is " , 140, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphenation."    , 120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 170;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphenation is " , 140, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphenation."    , 120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 120;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphenation " , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "          , 20 , NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphenation." , 120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 100;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphena-", 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphena-", 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 80;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphena-", 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphena-", 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 70;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hyphen-", 70, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ation " , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "    , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hyphen-", 70, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ation." , 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 60;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hy-"   , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phena-", 60, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hy-"   , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phena-", 60, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hy-"   , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phen-" , 50, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ation ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hy-"   , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phen-" , 50, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "a-"    , 20, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hy-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phen" , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a-"   , 20, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion ", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hy-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phen" , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a-"   , 20, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tion" , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."    , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 30;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hy-", 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phe", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "na-", 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tio", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n " , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hy-", 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "phe", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "na-", 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "tio", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n." , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 20;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "Hy" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ph" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "en" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a-" , 20, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ti" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "on ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "hy" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ph" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "en" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a-" , 20, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ti" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "on" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "H" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "y" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "p" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "h" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "i" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "o" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "i" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "s ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "h" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "y" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "p" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "h" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "e" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "a" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "i" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "o" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "n" , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "." , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testHyphenationStartLineChange) {
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.
    // "hyphenation" is hyphnated to "hy-phen-a-tion".
    const std::vector<uint16_t> textBuf = utf8ToUtf16("czerwono-niebieska");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    constexpr StartHyphenEdit START_HYPHEN = StartHyphenEdit::INSERT_HYPHEN;

    // Note that disable clang-format everywhere since aligned expectation is more readable.
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"czerwono-niebieska", 180, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, DO_HYPHEN, "pl", LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 180;
        std::vector<LineBreakExpectation> expect = {
                {"czerwono-niebieska", 180, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, DO_HYPHEN, "pl", LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 130;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"czerwono-" ,  90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"-niebieska", 100,    START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on

        const auto actual = doLineBreak(textBuf, DO_HYPHEN, "pl", LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testZeroWidthLine) {
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.
    constexpr float LINE_WIDTH = 0;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    {
        const auto textBuf = utf8ToUtf16("");
        std::vector<LineBreakExpectation> expect = {};
        const auto actual = doLineBreak(textBuf, DO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        const auto textBuf = utf8ToUtf16("A");
        std::vector<LineBreakExpectation> expect = {
                {"A", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        const auto actual = doLineBreak(textBuf, DO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        const auto textBuf = utf8ToUtf16("AB");
        std::vector<LineBreakExpectation> expect = {
                {"A", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"B", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        const auto actual = doLineBreak(textBuf, DO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testZeroWidthCharacter) {
    constexpr float CHAR_WIDTH = 0.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 1.0;
        const auto textBuf = utf8ToUtf16("This is an example text.");
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 0, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 10);
        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 0.0;
        const auto textBuf = utf8ToUtf16("This is an example text.");
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 0, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 10);
        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testLocaleSwitchTest) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    constexpr float LINE_WIDTH = 240;
    const auto textBuf = utf8ToUtf16("This is an example text.");
    {
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 0);

        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "fr-FR", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 0);

        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testEmailOrUrl) {
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 240;
        const auto textBuf = utf8ToUtf16("This is an url: http://a.b");
        std::vector<LineBreakExpectation> expect = {
                {"This is an url: ", 150, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"http://a.b", 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        const auto actual = doLineBreak(textBuf, DO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 240;
        const auto textBuf = utf8ToUtf16("This is an email: a@example.com");
        std::vector<LineBreakExpectation> expect = {
                {"This is an email: ", 170, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a@example.com", 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        const auto actual = doLineBreak(textBuf, DO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testLocaleSwitch_InEmailOrUrl) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    constexpr float LINE_WIDTH = 240;
    {
        const auto textBuf = utf8ToUtf16("This is an url: http://a.b");
        std::vector<LineBreakExpectation> expect = {
                {"This is an url: ", 150, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"http://a.b", 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "fr-FR", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 0);

        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        const auto textBuf = utf8ToUtf16("This is an email: a@example.com");
        std::vector<LineBreakExpectation> expect = {
                {"This is an email: ", 170, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a@example.com", 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "fr-FR", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, 0);

        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

// b/68669534
TEST_F(GreedyLineBreakerTest, CrashFix_Space_Tab) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 50;
        const auto textBuf = utf8ToUtf16("a \tb");
        std::vector<LineBreakExpectation> expect = {
                {"a \tb", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(LINE_WIDTH);
        TabStops tabStops(nullptr, 0, CHAR_WIDTH);

        const auto actual =
                breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, ExtentTest) {
    constexpr bool NO_HYPHEN = false;  // No hyphenation in this test case.
    const std::vector<uint16_t> textBuf = utf8ToUtf16("The \u3042\u3044\u3046 is Japanese.");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042\u3044\u3046 is Japanese.", 200, NO_START_HYPHEN, NO_END_HYPHEN,
                 CUSTOM_ASCENT, CUSTOM_DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 200;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042\u3044\u3046 is Japanese.", 200, NO_START_HYPHEN, NO_END_HYPHEN,
                 CUSTOM_ASCENT, CUSTOM_DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 190;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042\u3044\u3046 is ", 100, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"Japanese.", 90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042\u3044\u3046 ", 70, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"Japanese.", 90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042", 50, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3044\u3046 is ", 50, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"Japan", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ese.", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        std::vector<LineBreakExpectation> expect = {
                {"The ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u3042\u3044\u3046 ", 30, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"Japa", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"nese", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 20;
        std::vector<LineBreakExpectation> expect = {
                {"Th", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u3042\u3044", 20, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3046 ", 10, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"Ja", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"pa", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ne", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"se", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        std::vector<LineBreakExpectation> expect = {
                {"T", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"h", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u3042", 10, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3044", 10, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3046 ", 10, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"i", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s ", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"J", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"p", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"n", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testReplacementSpanNotBreakTest_SingleChar) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    const auto textBuf = utf8ToUtf16("This is an example \u2639 text.");

    // In this test case, assign a replacement run for "U+2639" with 5 times of CHAR_WIDTH.
    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 19), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(19, 20, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(20, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
    };

    {
        constexpr float LINE_WIDTH = 100;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is an ",   100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ",       70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2639 ",        50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",          50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"an ",      20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2639 ",  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",    50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"T",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"h",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"i",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"i",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"n ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"m",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"p",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"l",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                // TODO: This should be "\u2639 " since should not count the trailing line end space
                {"\u2639", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {" ",       0, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testReplacementSpanNotBreakTest_MultipleChars) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    const auto textBuf = utf8ToUtf16("This is an example text.");

    // In this test case, assign a replacement run for "is an " with 5 times of CHAR_WIDTH.
    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 5), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(5, 11, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(11, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
    };

    {
        constexpr float LINE_WIDTH = 100;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is an ",   100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ",       70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",          50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This ",   40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is an ",  50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"example ",70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",   50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"T",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"h",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"i",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is an ", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"e",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"m",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"p",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"l",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testReplacementSpanNotBreakTest_CJK) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    // Example string: "Today is a sunny day." in Japanese.
    const auto textBuf = utf8ToUtf16("\u672C\u65E5\u306F\u6674\u5929\u306A\u308A");

    // In this test case, assign a replacement run for "\u6674\u5929" with 5 times of CHAR_WIDTH.
    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 3), "ja-JP", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(3, 5, 5 * CHAR_WIDTH, LocaleListCache::getId("ja-JP"));
        builder.addCustomRun<ConstantRun>(Range(5, textBuf.size()), "ja-JP", CHAR_WIDTH, ASCENT,
                                          DESCENT);

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
    };

    {
        constexpr float LINE_WIDTH = 100;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F\u6674\u5929\u306A\u308A",
                  100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F\u6674\u5929\u306A",
                  90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u308A",
                  10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 80;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F\u6674\u5929",
                  80, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u306A\u308A",
                  20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 70;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u6674\u5929\u306A\u308A", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 60;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u6674\u5929\u306A", 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u308A",             10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u6674\u5929",       50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u306A\u308A",       20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C\u65E5\u306F", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u6674\u5929",       50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u306A\u308A",       20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // "\u6674\u5929" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"\u672C",       10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u65E5",       10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u306F",       10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u6674\u5929", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u306A",       10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u308A",       10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testReplacementSpanNotBreakTest_with_punctuation) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr bool DO_HYPHEN = true;  // Do hyphenation in this test case.

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit END_HYPHEN = EndHyphenEdit::INSERT_HYPHEN;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    const auto textBuf = utf8ToUtf16("This (is an) example text.");

    // In this test case, assign a replacement run for "U+2639" with 5 times of CHAR_WIDTH.
    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 6), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(6, 11, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(11, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineGreedy(textBuf, *measuredText, rectangleLineWidth, tabStops, DO_HYPHEN);
    };

    {
        constexpr float LINE_WIDTH = 1000;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This (is an) example text.",
                  260, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 250;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This (is an) example ", 200, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",                  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 190;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This (is an) ", 120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example text.", 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 120;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This (is an) ", 120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ",       70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",          50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 110;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This ",    40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"(is an) ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",    50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 60;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This ",  40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"(is an", 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {") ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"exam-",  50, NO_START_HYPHEN,    END_HYPHEN, ASCENT, DESCENT},
                {"ple ",   30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This ",  40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"(",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is an",  50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {") ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"exam-",  50, NO_START_HYPHEN,    END_HYPHEN, ASCENT, DESCENT},
                {"ple ",   30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text.",  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This ", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"(",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is an", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {") ",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ex-",   30, NO_START_HYPHEN,    END_HYPHEN, ASCENT, DESCENT},
                {"am-",   30, NO_START_HYPHEN,    END_HYPHEN, ASCENT, DESCENT},
                {"ple ",  30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text",  40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 10;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"T",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"h",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"i",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"s ",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"(",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is an", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {") ",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"m",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"p",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"l",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e ",    10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"e",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"x",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"t",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(GreedyLineBreakerTest, testControllCharAfterSpace) {
    constexpr bool NO_HYPHEN = false;  // No hyphenation in this test case.
    const std::vector<uint16_t> textBuf = utf8ToUtf16("example \u2066example");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 90;
        std::vector<LineBreakExpectation> expect = {
                {"example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2066example", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual = doLineBreak(textBuf, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}
}  // namespace
}  // namespace minikin
