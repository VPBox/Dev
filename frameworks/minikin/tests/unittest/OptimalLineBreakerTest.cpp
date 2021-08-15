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

#include <memory>

#include <gtest/gtest.h>

#include "minikin/Hyphenator.h"

#include "FileUtils.h"
#include "FontTestUtils.h"
#include "HyphenatorMap.h"
#include "LineBreakerTestHelper.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"
#include "OptimalLineBreaker.h"
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

class OptimalLineBreakerTest : public testing::Test {
public:
    OptimalLineBreakerTest() {}

    virtual ~OptimalLineBreakerTest() {}

    virtual void SetUp() override {
        mHyphenationPattern = readWholeFile("/system/usr/hyphen-data/hyph-en-us.hyb");
        Hyphenator* hyphenator = Hyphenator::loadBinary(
                mHyphenationPattern.data(), 2 /* min prefix */, 2 /* min suffix */, "en-US");
        HyphenatorMap::add("en-US", hyphenator);
        HyphenatorMap::add("pl", Hyphenator::loadBinary(nullptr, 0, 0, "pl"));
    }

    virtual void TearDown() override { HyphenatorMap::clear(); }

protected:
    LineBreakResult doLineBreak(const U16StringPiece& textBuffer, BreakStrategy strategy,
                                HyphenationFrequency frequency, float lineWidth) {
        return doLineBreak(textBuffer, strategy, frequency, "en-US", lineWidth);
    }

    LineBreakResult doLineBreak(const U16StringPiece& textBuffer, BreakStrategy strategy,
                                HyphenationFrequency frequency, const std::string& lang,
                                float lineWidth) {
        MeasuredTextBuilder builder;
        auto family1 = buildFontFamily("Ascii.ttf");
        auto family2 = buildFontFamily("CustomExtent.ttf");
        std::vector<std::shared_ptr<FontFamily>> families = {family1, family2};
        auto fc = std::make_shared<FontCollection>(families);
        MinikinPaint paint(fc);
        paint.size = 10.0f;  // Make 1em=1px
        paint.localeListId = LocaleListCache::getId(lang);
        builder.addStyleRun(0, textBuffer.size(), std::move(paint), false);
        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuffer, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        return doLineBreak(textBuffer, *measuredText, strategy, frequency, lineWidth);
    }

    LineBreakResult doLineBreak(const U16StringPiece& textBuffer, const MeasuredText& measuredText,
                                BreakStrategy strategy, HyphenationFrequency frequency,
                                float lineWidth) {
        RectangleLineWidth rectangleLineWidth(lineWidth);
        return breakLineOptimal(textBuffer, measuredText, rectangleLineWidth, strategy, frequency,
                                false /* justified */);
    }

private:
    std::vector<uint8_t> mHyphenationPattern;
};

TEST_F(OptimalLineBreakerTest, testBreakWithoutHyphenation) {
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr BreakStrategy BALANCED = BreakStrategy::Balanced;
    constexpr HyphenationFrequency NO_HYPHENATION = HyphenationFrequency::None;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;
    const std::vector<uint16_t> textBuf = utf8ToUtf16("This is an example text.");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit END_HYPHEN = EndHyphenEdit::INSERT_HYPHEN;
    // Note that disable clang-format everywhere since aligned expectation is more readable.
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 240;
        std::vector<LineBreakExpectation> expect = {
                {"This is an example text.", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 230;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an example " , 180, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."               ,  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);

        // clang-format off
        expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an ex-" , 140, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample text."    , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 170;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an exam-" , 160, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple text."        ,  90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an ex-", 140, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample text."   , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 160;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an exam-" , 160, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple text."        ,  90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an ex-", 140, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample text."   , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 150;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an ex-", 140, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample text."   , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is an ex-", 140, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample text."   , 110, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 130;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an "   , 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example text." , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 120;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This is an ", 100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example "   ,  70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."      ,  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is " ,  70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an exam-" ,  80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple text.",  90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This "   , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is " , 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an exam-" , 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple text.", 90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This "   , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is " , 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an exam-" , 80, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple text.", 90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 80;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This "   , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an ex-"  , 60, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 70;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                { "This "   , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This is ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an ex-"  , 60, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ample "  , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text."   , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
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
                // TODO: Is this desperate break working correctly?
                { "exa"   , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mple " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "exam-" , 50, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple "  , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
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
                // TODO: Is this desperate break working correctly?
                { "exa"   , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mple " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is an ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "exam-" , 50, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple "  , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text." , 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
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
                // TODO: Is this desperate break working correctly?
                { "exa"   , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mple " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text"  , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."     , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "exa"   , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mple " , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "t"     , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ext."  , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This ", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ex-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "am-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple " , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "text" , 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."    , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "This ", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is "  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an "  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ex-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "am-"  , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple " , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "te"   , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xt."  , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 30;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                // TODO: Is this desperate break working correctly?
                { "T"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "his ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "e"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xam" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ple ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "tex" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t."  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                // TODO: Is this desperate break working correctly?
                { "T"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "his ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "e"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xam" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ple ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "te"  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xt." , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                // TODO: Is this desperate break working correctly?
                { "T"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "his ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ex-" , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "am-" , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT },
                { "ple ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "tex" , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t."  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                // TODO: Is this desperate break working correctly?
                {"T"   , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"his ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"is " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"an " , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ex-" , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT},
                {"am-" , 30, NO_START_HYPHEN, END_HYPHEN, ASCENT, DESCENT},
                {"ple ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                // TODO: Is this desperate break working correctly?
                {"te"  , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"xt." , 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
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
                // TODO: Is this desperate break working correctly?
                { "e"  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xa" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mp" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "le ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "te" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xt" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "."  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                { "Th" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "an ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "e"  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "xa" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "mp" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "le ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                // TODO: Is this desperate break working correctly?
                { "t"  , 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "ex" , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
                { "t." , 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT },
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
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

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testHyphenationStartLineChange) {
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;
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

        const auto actual =
                doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, "pl", LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 180;
        std::vector<LineBreakExpectation> expect = {
                {"czerwono-niebieska", 180, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        const auto actual =
                doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, "pl", LINE_WIDTH);
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

        const auto actual =
                doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, "pl", LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testZeroWidthLine) {
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;
    constexpr float LINE_WIDTH = 0;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    {
        const auto textBuf = utf8ToUtf16("");
        std::vector<LineBreakExpectation> expect = {};
        const auto actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        const auto textBuf = utf8ToUtf16("A");
        std::vector<LineBreakExpectation> expect = {
                {"A", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        const auto actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
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
        const auto actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testZeroWidthCharacter) {
    constexpr float CHAR_WIDTH = 0.0;
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;

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
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);

        const auto actual =
                doLineBreak(textBuf, *measuredText, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
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
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);

        const auto actual =
                doLineBreak(textBuf, *measuredText, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testLocaleSwitchTest) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;

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
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);

        const auto actual =
                doLineBreak(textBuf, *measuredText, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
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
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        const auto actual =
                doLineBreak(textBuf, *measuredText, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testEmailOrUrl) {
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr BreakStrategy BALANCED = BreakStrategy::Balanced;
    constexpr HyphenationFrequency NO_HYPHENATION = HyphenationFrequency::None;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 240;
        const auto textBuf = utf8ToUtf16("This is an url: http://a.b");
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                // TODO: Fix this. Prefer not to break inside URL.
                {"This is an url: http://a", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".b",                        20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        // clang-format off
        expect = {
                {"This is an url: ", 150, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"http://a.b",       100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 240;
        const auto textBuf = utf8ToUtf16("This is an email: a@example.com");
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is an email: ", 170, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a@example.com"     , 130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testLocaleSwitch_InEmailOrUrl) {
    constexpr float CHAR_WIDTH = 10.0;
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr BreakStrategy BALANCED = BreakStrategy::Balanced;
    constexpr HyphenationFrequency NO_HYPHENATION = HyphenationFrequency::None;
    constexpr HyphenationFrequency NORMAL_HYPHENATION = HyphenationFrequency::Normal;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    constexpr float LINE_WIDTH = 240;
    {
        const auto textBuf = utf8ToUtf16("This is an url: http://a.b");
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "fr-FR", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measured =
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);

        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                // TODO: Fix this. Prefer not to break inside URL.
                {"This is an url: http://a", 240, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".b",                        20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, *measured, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, *measured, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);

        // clang-format off
        expect = {
                {"This is an url: ", 150, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"http://a.b",       100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on

        actual = doLineBreak(textBuf, *measured, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, *measured, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        const auto textBuf = utf8ToUtf16("This is an email: a@example.com");
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 18), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addCustomRun<ConstantRun>(Range(18, textBuf.size()), "fr-FR", CHAR_WIDTH, ASCENT,
                                          DESCENT);
        std::unique_ptr<MeasuredText> measured =
                builder.build(textBuf, true /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);

        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is an email: ", 170, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"a@example.com",      130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on

        auto actual = doLineBreak(textBuf, *measured, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, *measured, HIGH_QUALITY, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, *measured, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, *measured, BALANCED, NORMAL_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, ExtentTest) {
    constexpr HyphenationFrequency NO_HYPHEN = HyphenationFrequency::None;
    const std::vector<uint16_t> textBuf = utf8ToUtf16("The \u3042\u3044\u3046 is Japanese.");

    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 1000;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042\u3044\u3046 is Japanese.", 200, NO_START_HYPHEN, NO_END_HYPHEN,
                 CUSTOM_ASCENT, CUSTOM_DESCENT},
        };

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
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

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
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
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 90;
        std::vector<LineBreakExpectation> expect = {
                {"The \u3042", 50, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3044\u3046 is ", 50, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"Japanese.", 90, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
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
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 40;
        std::vector<LineBreakExpectation> expect = {
                {"The ", 30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u3042\u3044", 20, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3046 is ", 40, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"Japa", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"nese", 40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 20;
        std::vector<LineBreakExpectation> expect = {
                {"T", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"he ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u3042", 10, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT, CUSTOM_DESCENT},
                {"\u3044\u3046 ", 20, NO_START_HYPHEN, NO_END_HYPHEN, CUSTOM_ASCENT,
                 CUSTOM_DESCENT},
                {"is ", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"Ja", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"pa", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ne", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"se", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".", 10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
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
        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHEN, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testReplacementSpanNotBreakTest_SingleChar) {
    constexpr float CHAR_WIDTH = 10.0;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    const auto textBuf = utf8ToUtf16("This is an example \u2639 text.");

    // In this test case, assign a replacement run for "U+2639" with 5 times of CHAR_WIDTH.
    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addCustomRun<ConstantRun>(Range(0, 19), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(19, 21, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(21, textBuf.size()), "en-US", CHAR_WIDTH, ASCENT,
                                          DESCENT);

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineOptimal(textBuf, *measuredText, rectangleLineWidth,
                                BreakStrategy::HighQuality, HyphenationFrequency::None,
                                false /* justified */);
    };

    {
        constexpr float LINE_WIDTH = 100;
        // "is an" is a single replacement span. Do not break.
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"This is an ",   100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ",       70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2639 text.",  100, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
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
                {"is an ",  50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"example ",70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2639 ", 50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
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
                {"\u2639 ",50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
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

TEST_F(OptimalLineBreakerTest, testReplacementSpanNotBreakTest_MultipleChars) {
    constexpr float CHAR_WIDTH = 10.0;

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
        return breakLineOptimal(textBuf, *measuredText, rectangleLineWidth,
                                BreakStrategy::HighQuality, HyphenationFrequency::None,
                                false /* justified */);
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

TEST_F(OptimalLineBreakerTest, testReplacementSpanNotBreakTest_CJK) {
    constexpr float CHAR_WIDTH = 10.0;

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
        return breakLineOptimal(textBuf, *measuredText, rectangleLineWidth,
                                BreakStrategy::HighQuality, HyphenationFrequency::None,
                                false /* justified */);
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

// http://b/119657685
// Following test case is for verifying that the ReplacementSpan should not be broken into multiple
// pieces. The actual break point is not a part of expectation. For example, it would be good to
// break the starting offset of the ReplacementSpan for some case.
TEST_F(OptimalLineBreakerTest, testReplacementSpan_GraphemeLineBreakWithMultipleRepalcementSpans) {
    constexpr float CHAR_WIDTH = 10.0;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;

    const auto textBuf = utf8ToUtf16("ab de\u00A0\u00A0fg ij\u00A0\u00A0kl no\u00A0\u00A0pq st");

    auto doLineBreak = [=](float width) {
        MeasuredTextBuilder builder;
        builder.addReplacementRun(0, 5, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(5, 7), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(7, 12, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(12, 14), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(14, 19, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));
        builder.addCustomRun<ConstantRun>(Range(19, 21), "en-US", CHAR_WIDTH, ASCENT, DESCENT);
        builder.addReplacementRun(21, 26, 5 * CHAR_WIDTH, LocaleListCache::getId("en-US"));

        std::unique_ptr<MeasuredText> measuredText =
                builder.build(textBuf, false /* compute hyphenation */,
                              false /* compute full layout */, nullptr /* no hint */);
        RectangleLineWidth rectangleLineWidth(width);
        TabStops tabStops(nullptr, 0, 0);
        return breakLineOptimal(textBuf, *measuredText, rectangleLineWidth,
                                BreakStrategy::HighQuality, HyphenationFrequency::None,
                                false /* justified */);
    };

    {
        constexpr float LINE_WIDTH = 1000;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0\u00A0fg ij\u00A0\u00A0kl no\u00A0\u00A0pq st",
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
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0\u00A0fg ij\u00A0\u00A0kl no\u00A0\u00A0",
                  210, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"pq st",
                   50, NO_START_HYPHEN, NO_END_HYPHEN,       0,      0},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 180;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0\u00A0fg ij\u00A0\u00A0",
                  140, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"kl no\u00A0\u00A0pq st",
                  120, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 130;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0\u00A0fg ij\u00A0",
                  130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u00A0kl no\u00A0\u00A0pq st",
                  130, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 110;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0",             60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u00A0fg ij\u00A0\u00A0", 80, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"kl no\u00A0\u00A0",       70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"pq st",                   50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 60;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de\u00A0",  60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u00A0fg ij",  60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u00A0\u00A0", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"kl no\u00A0",  60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u00A0pq st",  60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
    {
        constexpr float LINE_WIDTH = 50;
        // clang-format off
        std::vector<LineBreakExpectation> expect = {
                {"ab de",        50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u00A0\u00A0", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"fg ij",        50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u00A0\u00A0", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"kl no",        50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
                {"\u00A0\u00A0", 20, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"pq st",        50, NO_START_HYPHEN, NO_END_HYPHEN,      0,       0},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testReplacementSpanNotBreakTest_with_punctuation) {
    constexpr float CHAR_WIDTH = 10.0;

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
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
        return breakLineOptimal(textBuf, *measuredText, rectangleLineWidth,
                                BreakStrategy::HighQuality, HyphenationFrequency::Normal,
                                false /* justified */);
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
                {") ex",   40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ample ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
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
                {") ex",   40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"ample ", 50, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
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
                {"This ",  40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                // TODO(nona): This might be wrongly broken. "(is an" should be broken into "(" and
                // "is an" as the desperate break.
                {"(is an", 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {") ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"exa",    30, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"mple ",  40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"text",   40, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {".",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
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
                // TODO(nona): This might be wrongly broken. "(is an" should be broken into "(" and
                // "is an" as the desperate break.
                {"(is an", 60, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {") ",     10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
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
                {".",      10, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };
        // clang-format on
        const auto actual = doLineBreak(LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}

TEST_F(OptimalLineBreakerTest, testControllCharAfterSpace) {
    constexpr BreakStrategy HIGH_QUALITY = BreakStrategy::HighQuality;
    constexpr BreakStrategy BALANCED = BreakStrategy::Balanced;
    constexpr HyphenationFrequency NO_HYPHENATION = HyphenationFrequency::None;
    const std::vector<uint16_t> textBuf = utf8ToUtf16("example \u2066example");

    constexpr StartHyphenEdit NO_START_HYPHEN = StartHyphenEdit::NO_EDIT;
    constexpr EndHyphenEdit NO_END_HYPHEN = EndHyphenEdit::NO_EDIT;
    {
        constexpr float LINE_WIDTH = 90;
        // Note that HarfBuzz assigns 0px for control characters regardless of glyph existence in
        // the font.
        std::vector<LineBreakExpectation> expect = {
                {"example ", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
                {"\u2066example", 70, NO_START_HYPHEN, NO_END_HYPHEN, ASCENT, DESCENT},
        };

        auto actual = doLineBreak(textBuf, HIGH_QUALITY, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
        actual = doLineBreak(textBuf, BALANCED, NO_HYPHENATION, LINE_WIDTH);
        EXPECT_TRUE(sameLineBreak(expect, actual)) << toString(expect) << std::endl
                                                   << " vs " << std::endl
                                                   << toString(textBuf, actual);
    }
}
}  // namespace
}  // namespace minikin
