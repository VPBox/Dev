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

#include "minikin/MeasuredText.h"

#include <gtest/gtest.h>

#include "minikin/LineBreaker.h"

#include "FontTestUtils.h"
#include "UnicodeUtils.h"

namespace minikin {

constexpr float CHAR_WIDTH = 10.0;  // Mock implementation always returns 10.0 for advance.

TEST(MeasuredTextTest, RunTests) {
    constexpr uint32_t CHAR_COUNT = 6;
    constexpr float REPLACEMENT_WIDTH = 20.0f;
    auto font = buildFontCollection("Ascii.ttf");

    MeasuredTextBuilder builder;

    MinikinPaint paint1(font);
    paint1.size = 10.0f;  // make 1em = 10px
    builder.addStyleRun(0, 2, std::move(paint1), false /* is RTL */);
    builder.addReplacementRun(2, 4, REPLACEMENT_WIDTH, 0 /* locale list id */);
    MinikinPaint paint2(font);
    paint2.size = 10.0f;  // make 1em = 10px
    builder.addStyleRun(4, 6, std::move(paint2), false /* is RTL */);

    std::vector<uint16_t> text(CHAR_COUNT, 'a');

    std::unique_ptr<MeasuredText> measuredText =
            builder.build(text, true /* compute hyphenation */, false /* compute full layout */,
                          nullptr /* no hint */);

    ASSERT_TRUE(measuredText);

    // ReplacementRun assigns all width to the first character and leave zeros others.
    std::vector<float> expectedWidths = {CHAR_WIDTH, CHAR_WIDTH, REPLACEMENT_WIDTH,
                                         0,          CHAR_WIDTH, CHAR_WIDTH};

    EXPECT_EQ(expectedWidths, measuredText->widths);
}

TEST(MeasuredTextTest, getBoundsTest) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, text.size(), std::move(paint), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    EXPECT_EQ(MinikinRect(0.0f, 0.0f, 0.0f, 0.0f), mt->getBounds(text, Range(0, 0)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), mt->getBounds(text, Range(0, 1)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), mt->getBounds(text, Range(0, 2)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), mt->getBounds(text, Range(1, 2)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 130.0f, 0.0f), mt->getBounds(text, Range(0, text.size())));
}

TEST(MeasuredTextTest, getBoundsTest_multiStyle) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    uint32_t helloLength = 7;  // length of "Hello, "

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, helloLength, std::move(paint), false /* is RTL */);
    MinikinPaint paint2(font);
    paint2.size = 20.0f;
    builder.addStyleRun(helloLength, text.size(), std::move(paint2), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    EXPECT_EQ(MinikinRect(0.0f, 0.0f, 0.0f, 0.0f), mt->getBounds(text, Range(0, 0)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), mt->getBounds(text, Range(0, 1)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), mt->getBounds(text, Range(0, 2)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), mt->getBounds(text, Range(1, 2)));
    EXPECT_EQ(MinikinRect(0.0f, 0.0f, 0.0f, 0.0f), mt->getBounds(text, Range(7, 7)));
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 20.0f, 0.0f), mt->getBounds(text, Range(7, 8)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 30.0f, 0.0f), mt->getBounds(text, Range(6, 8)));
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 190.0f, 0.0f), mt->getBounds(text, Range(0, text.size())));
}

TEST(MeasuredTextTest, getExtentTest) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, text.size(), std::move(paint), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    EXPECT_EQ(MinikinExtent(0.0f, 0.0f), mt->getExtent(text, Range(0, 0)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(0, 1)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(0, 2)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(1, 2)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(0, text.size())));
}

TEST(MeasuredTextTest, getExtentTest_multiStyle) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    uint32_t helloLength = 7;  // length of "Hello, "

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, helloLength, std::move(paint), false /* is RTL */);
    MinikinPaint paint2(font);
    paint2.size = 20.0f;
    builder.addStyleRun(helloLength, text.size(), std::move(paint2), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    EXPECT_EQ(MinikinExtent(0.0f, 0.0f), mt->getExtent(text, Range(0, 0)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(0, 1)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(0, 2)));
    EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), mt->getExtent(text, Range(1, 2)));
    EXPECT_EQ(MinikinExtent(0.0f, 0.0f), mt->getExtent(text, Range(7, 7)));
    EXPECT_EQ(MinikinExtent(-160.0f, 40.0f), mt->getExtent(text, Range(7, 8)));
    EXPECT_EQ(MinikinExtent(-160.0f, 40.0f), mt->getExtent(text, Range(6, 8)));
    EXPECT_EQ(MinikinExtent(-160.0f, 40.0f), mt->getExtent(text, Range(0, text.size())));
}

TEST(MeasuredTextTest, buildLayoutTest) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    Range fullContext(0, text.size());

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, text.size(), std::move(paint), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    MinikinPaint samePaint(font);
    samePaint.size = 10.0f;

    Layout layout = mt->buildLayout(text, Range(0, 0), fullContext, samePaint,
                                    StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    layout = mt->buildLayout(text, Range(0, 1), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, 2), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(2u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0) && layout.getFont(0) == layout.getFont(1));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getX(1));
    EXPECT_EQ(0.0f, layout.getY(1));
    EXPECT_EQ(20.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(10.0f, layout.getCharAdvance(1));
    EXPECT_EQ(2u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(1, 2), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, text.size()), fullContext, samePaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(text.size(), layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    for (uint32_t i = 0; i < text.size(); ++i) {
        EXPECT_EQ(layout.getFont(0), layout.getFont(i)) << i;
        EXPECT_EQ(10.0f * i, layout.getX(i)) << i;
        EXPECT_EQ(0.0f, layout.getY(i)) << i;
        EXPECT_EQ(10.0f, layout.getCharAdvance(i)) << i;
    }
    EXPECT_EQ(130.0f, layout.getAdvance());
    EXPECT_EQ(text.size(), layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 130.0f, 0.0f), layout.getBounds());
}

TEST(MeasuredTextTest, buildLayoutTest_multiStyle) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    uint32_t helloLength = 7;  // length of "Hello, "
    Range fullContext(0, text.size());

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, helloLength, std::move(paint), false /* is RTL */);
    MinikinPaint paint2(font);
    paint2.size = 20.0f;
    builder.addStyleRun(helloLength, text.size(), std::move(paint2), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    MinikinPaint samePaint(font);
    samePaint.size = 10.0f;

    Layout layout = mt->buildLayout(text, Range(0, 0), fullContext, samePaint,
                                    StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    layout = mt->buildLayout(text, Range(0, 1), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, 2), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(2u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0) && layout.getFont(0) == layout.getFont(1));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getX(1));
    EXPECT_EQ(0.0f, layout.getY(1));
    EXPECT_EQ(20.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(10.0f, layout.getCharAdvance(1));
    EXPECT_EQ(2u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(1, 2), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(10.0f, layout.getAdvance());
    EXPECT_EQ(10.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(7, 7), fullContext, samePaint, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    MinikinPaint samePaint2(font);
    samePaint2.size = 20.0f;
    layout = mt->buildLayout(text, Range(7, 8), fullContext, samePaint2, StartHyphenEdit::NO_EDIT,
                             EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(20.0f, layout.getAdvance());
    EXPECT_EQ(20.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 20.0f, 0.0f), layout.getBounds());
}

TEST(MeasuredTextTest, buildLayoutTest_differentPaint) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    Range fullContext(0, text.size());

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, text.size(), std::move(paint), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    MinikinPaint differentPaint(font);
    differentPaint.size = 20.0f;

    Layout layout = mt->buildLayout(text, Range(0, 0), fullContext, differentPaint,
                                    StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    layout = mt->buildLayout(text, Range(0, 1), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(20.0f, layout.getAdvance());
    EXPECT_EQ(20.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 20.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, 2), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(2u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0) && layout.getFont(0) == layout.getFont(1));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(20.0f, layout.getX(1));
    EXPECT_EQ(0.0f, layout.getY(1));
    EXPECT_EQ(40.0f, layout.getAdvance());
    EXPECT_EQ(20.0f, layout.getCharAdvance(0));
    EXPECT_EQ(20.0f, layout.getCharAdvance(1));
    EXPECT_EQ(2u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 40.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(1, 2), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(20.0f, layout.getAdvance());
    EXPECT_EQ(20.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 20.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, text.size()), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(text.size(), layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    for (uint32_t i = 0; i < text.size(); ++i) {
        EXPECT_EQ(layout.getFont(0), layout.getFont(i)) << i;
        EXPECT_EQ(20.0f * i, layout.getX(i)) << i;
        EXPECT_EQ(0.0f, layout.getY(i)) << i;
        EXPECT_EQ(20.0f, layout.getCharAdvance(i)) << i;
    }
    EXPECT_EQ(260.0f, layout.getAdvance());
    EXPECT_EQ(text.size(), layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 20.0f, 260.0f, 0.0f), layout.getBounds());
}

TEST(MeasuredTextTest, buildLayoutTest_multiStyle_differentPaint) {
    auto text = utf8ToUtf16("Hello, World!");
    auto font = buildFontCollection("Ascii.ttf");
    uint32_t helloLength = 7;  // length of "Hello, "
    Range fullContext(0, text.size());

    MeasuredTextBuilder builder;
    MinikinPaint paint(font);
    paint.size = 10.0f;
    builder.addStyleRun(0, helloLength, std::move(paint), false /* is RTL */);
    MinikinPaint paint2(font);
    paint2.size = 20.0f;
    builder.addStyleRun(helloLength, text.size(), std::move(paint2), false /* is RTL */);
    auto mt = builder.build(text, true /* hyphenation */, true /* full layout */,
                            nullptr /* no hint */);

    MinikinPaint differentPaint(font);
    differentPaint.size = 30.0f;

    Layout layout = mt->buildLayout(text, Range(0, 0), fullContext, differentPaint,
                                    StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    layout = mt->buildLayout(text, Range(0, 1), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(30.0f, layout.getAdvance());
    EXPECT_EQ(30.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 30.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, 2), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(2u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0) && layout.getFont(0) == layout.getFont(1));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(30.0f, layout.getX(1));
    EXPECT_EQ(0.0f, layout.getY(1));
    EXPECT_EQ(60.0f, layout.getAdvance());
    EXPECT_EQ(30.0f, layout.getCharAdvance(0));
    EXPECT_EQ(30.0f, layout.getCharAdvance(1));
    EXPECT_EQ(2u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 60.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(1, 2), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(30.0f, layout.getAdvance());
    EXPECT_EQ(30.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 30.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(7, 7), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    EXPECT_EQ(0u, layout.nGlyphs());

    layout = mt->buildLayout(text, Range(7, 8), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(1u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(30.0f, layout.getAdvance());
    EXPECT_EQ(30.0f, layout.getCharAdvance(0));
    EXPECT_EQ(1u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 30.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(6, 8), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(2u, layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0) && layout.getFont(0) == layout.getFont(1));
    EXPECT_EQ(0.0f, layout.getX(0));
    EXPECT_EQ(0.0f, layout.getY(0));
    EXPECT_EQ(30.0f, layout.getX(1));
    EXPECT_EQ(0.0f, layout.getY(1));
    EXPECT_EQ(60.0f, layout.getAdvance());
    EXPECT_EQ(30.0f, layout.getCharAdvance(0));
    EXPECT_EQ(30.0f, layout.getCharAdvance(1));
    EXPECT_EQ(2u, layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 60.0f, 0.0f), layout.getBounds());

    layout = mt->buildLayout(text, Range(0, text.size()), fullContext, differentPaint,
                             StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
    ASSERT_EQ(text.size(), layout.nGlyphs());
    EXPECT_TRUE(layout.getFont(0));
    for (uint32_t i = 0; i < text.size(); ++i) {
        EXPECT_EQ(layout.getFont(0), layout.getFont(i)) << i;
        EXPECT_EQ(30.0f * i, layout.getX(i)) << i;
        EXPECT_EQ(0.0f, layout.getY(i)) << i;
        EXPECT_EQ(30.0f, layout.getCharAdvance(i)) << i;
    }
    EXPECT_EQ(390.0f, layout.getAdvance());
    EXPECT_EQ(text.size(), layout.getAdvances().size());
    EXPECT_EQ(MinikinRect(0.0f, 30.0f, 390.0f, 0.0f), layout.getBounds());
}

}  // namespace minikin
