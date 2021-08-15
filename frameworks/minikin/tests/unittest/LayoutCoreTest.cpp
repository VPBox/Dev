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

#include "minikin/LayoutCore.h"

#include <gtest/gtest.h>

#include "minikin/FontCollection.h"
#include "minikin/LayoutPieces.h"

#include "FontTestUtils.h"
#include "UnicodeUtils.h"

namespace minikin {
namespace {

static LayoutPiece buildLayout(const std::string& text, const MinikinPaint& paint) {
    auto utf16 = utf8ToUtf16(text);
    return LayoutPiece(utf16, Range(0, utf16.size()), false /* rtl */, paint,
                       StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT);
}

static LayoutPiece buildLayout(const std::string& text, const std::vector<std::string>& fonts) {
    std::vector<std::shared_ptr<FontFamily>> families;
    for (const auto& fontPath : fonts) {
        families.push_back(buildFontFamily(fontPath));
    }
    auto fc = std::make_shared<FontCollection>(families);
    MinikinPaint paint(fc);
    paint.size = 10.0f;  // make 1em = 10px
    return buildLayout(text, paint);
}

static LayoutPiece buildLayout(const std::string& text, const std::vector<std::string>& fonts,
                               const std::string fontFeaturesSettings) {
    std::vector<std::shared_ptr<FontFamily>> families;
    for (const auto& fontPath : fonts) {
        families.push_back(buildFontFamily(fontPath));
    }
    auto fc = std::make_shared<FontCollection>(families);
    MinikinPaint paint(fc);
    paint.size = 10.0f;  // make 1em = 10px
    paint.fontFeatureSettings = fontFeaturesSettings;
    return buildLayout(text, paint);
}

TEST(LayoutPieceTest, doLayoutTest) {
    // The LayoutTestFont.ttf has following coverage, extent, width and bbox.
    // Ascender: 10em, Descender: -2em
    // U+0020: 10em, (0, 0) - (10, 10)
    // U+002E (.): 10em, (0, 0) - (10, 10)
    // U+0043 (C): 100em, (0, 0) - (100, 100)
    // U+0049 (I): 1em, (0, 0) - (1, 1)
    // U+004C (L): 50em, (0, 0) - (50, 50)
    // U+0056 (V): 5em, (0, 0) - (5, 5)
    // U+0058 (X): 10em, (0, 0) - (10, 10)
    // U+005F (_): 0em, (0, 0) - (0, 0)
    // U+FFFD (invalid surrogate will be replaced to this): 7em, (0, 0) - (7, 7)
    // U+10331 (\uD800\uDF31): 10em, (0, 0) - (10, 10)
    {
        auto layout = buildLayout("I", {"LayoutTestFont.ttf"});
        EXPECT_EQ(1u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-100.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_EQ(1u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(10.0f, layout.advance());
    }
    {
        auto layout = buildLayout("II", {"LayoutTestFont.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(Point(10.0f, 0), layout.pointAt(1));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-100.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_EQ(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(10.0f, layout.advances()[1]);
        EXPECT_EQ(20.0f, layout.advance());
    }
    {
        auto layout = buildLayout("IV", {"LayoutTestFont.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(Point(10.0f, 0), layout.pointAt(1));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 60.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-100.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_EQ(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(50.0f, layout.advances()[1]);
        EXPECT_EQ(60.0f, layout.advance());
    }
}

TEST(LayoutPieceTest, doLayoutTest_MultiFont) {
    // See doLayoutTest for the details of LayoutTestFont.ttf
    // The Hiragana.ttf has following coverage, extent, width and bbox.
    // Ascender: 16em, Descender: -4em
    // U+3042: 2em, (0, 0) - (2, 2)
    // U+3044: 2em, (0, 0) - (2, 2)
    // U+3046: 2em, (0, 0) - (2, 2)
    // U+3048: 2em, (0, 0) - (2, 2)
    // U+304A: 2em, (0, 0) - (2, 2)
    {
        auto layout = buildLayout("I\u3042", {"LayoutTestFont.ttf", "Hiragana.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(Point(10.0f, 0), layout.pointAt(1));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 30.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-160.0f, 40.0f), layout.extent());
        EXPECT_EQ(2u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_NE(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(20.0f, layout.advances()[1]);
        EXPECT_EQ(30.0f, layout.advance());
    }
    {
        auto layout = buildLayout("\u3042I", {"LayoutTestFont.ttf", "Hiragana.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(Point(20.0f, 0), layout.pointAt(1));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 30.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-160.0f, 40.0f), layout.extent());
        EXPECT_EQ(2u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_NE(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(20.0f, layout.advances()[0]);
        EXPECT_EQ(10.0f, layout.advances()[1]);
        EXPECT_EQ(30.0f, layout.advance());
    }
}

TEST(LayoutPieceTest, doLayoutTest_Ligature) {
    // Ligature.ttf support all ASCII characters.
    // Ascender: 8em, Descender: -2em
    // U+0020..U+007E: 1em, (0, 0) - (1, 1)
    // Also this has ligature entry for fi as "ccmp" feature, ff as "liga" feature.
    {
        auto layout = buildLayout("fi", {"Ligature.ttf"});
        EXPECT_EQ(1u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(0.0f, layout.advances()[1]);  // Ligature assigns all width to the first char.
        EXPECT_EQ(10.0f, layout.advance());
    }
    {
        auto layout = buildLayout("ff", {"Ligature.ttf"});
        EXPECT_EQ(1u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(0.0f, layout.advances()[1]);  // Ligature assigns all width to the first char.
        EXPECT_EQ(10.0f, layout.advance());
    }
    {
        auto layout = buildLayout("fi", {"Ligature.ttf"}, "'liga' off");
        EXPECT_EQ(1u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 10.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(0.0f, layout.advances()[1]);  // Ligature assigns all width to the first char.
        EXPECT_EQ(10.0f, layout.advance());
    }
    {
        auto layout = buildLayout("ff", {"Ligature.ttf"}, "'liga' off");
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(10.0f, layout.advances()[1]);
        EXPECT_EQ(20.0f, layout.advance());
    }
    {
        auto layout = buildLayout("fii", {"Ligature.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_EQ(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(3u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(0.0f, layout.advances()[1]);  // Ligature assigns all width to the first char.
        EXPECT_EQ(10.0f, layout.advances()[2]);
        EXPECT_EQ(20.0f, layout.advance());
    }
    {
        auto layout = buildLayout("if", {"Ligature.ttf"});
        EXPECT_EQ(2u, layout.glyphCount());
        EXPECT_EQ(Point(0, 0), layout.pointAt(0));
        EXPECT_EQ(MinikinRect(0.0f, 10.0f, 20.0f, 0.0f), layout.bounds());
        EXPECT_EQ(MinikinExtent(-80.0f, 20.0f), layout.extent());
        EXPECT_EQ(1u, layout.fonts().size());
        EXPECT_TRUE(layout.fontAt(0).font);
        EXPECT_TRUE(layout.fontAt(1).font);
        EXPECT_EQ(layout.fontAt(0), layout.fontAt(1));
        EXPECT_EQ(2u, layout.advances().size());
        EXPECT_EQ(10.0f, layout.advances()[0]);
        EXPECT_EQ(10.0f, layout.advances()[1]);
        EXPECT_EQ(20.0f, layout.advance());
    }
}

}  // namespace
}  // namespace minikin
