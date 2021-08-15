/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "minikin/FontCollection.h"

#include <memory>

#include <gtest/gtest.h>

#include "minikin/FontFamily.h"
#include "minikin/LocaleList.h"
#include "minikin/MinikinPaint.h"

#include "FontTestUtils.h"
#include "FreeTypeMinikinFontForTest.h"
#include "Locale.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"
#include "UnicodeUtils.h"

namespace minikin {

const char kItemizeFontXml[] = "itemize.xml";
const char kCherokeeFont[] = "Cherokee.ttf";
const char kEmojiFont[] = "Emoji.ttf";
const char kJAFont[] = "Ja.ttf";
const char kKOFont[] = "Ko.ttf";
const char kLatinBoldFont[] = "Bold.ttf";
const char kLatinBoldItalicFont[] = "BoldItalic.ttf";
const char kLatinFont[] = "Regular.ttf";
const char kLatinItalicFont[] = "Italic.ttf";
const char kZH_HansFont[] = "ZhHans.ttf";
const char kZH_HantFont[] = "ZhHant.ttf";
const char kAsciiFont[] = "Ascii.ttf";

const char kEmojiXmlFile[] = "emoji.xml";
const char kNoGlyphFont[] = "NoGlyphFont.ttf";
const char kColorEmojiFont[] = "ColorEmojiFont.ttf";
const char kTextEmojiFont[] = "TextEmojiFont.ttf";
const char kMixedEmojiFont[] = "ColorTextMixedEmojiFont.ttf";

const char kHasCmapFormat14Font[] = "NoCmapFormat14.ttf";
const char kNoCmapFormat14Font[] = "VariationSelectorTest-Regular.ttf";

// Utility functions for calling itemize function.
std::vector<FontCollection::Run> itemize(const std::shared_ptr<FontCollection>& collection,
                                         const char* str, FontStyle style,
                                         const std::string& localeList) {
    const size_t BUF_SIZE = 256;
    uint16_t buf[BUF_SIZE];
    size_t len;

    ParseUnicode(buf, BUF_SIZE, str, &len, NULL);
    const uint32_t localeListId = registerLocaleList(localeList);
    auto result = collection->itemize(U16StringPiece(buf, len), style, localeListId,
                                      FamilyVariant::DEFAULT);

    // Check the same result has returned by calling with maxRun.
    for (uint32_t runMax = 1; runMax <= result.size(); runMax++) {
        auto resultWithRunMax = collection->itemize(U16StringPiece(buf, len), style, localeListId,
                                                    FamilyVariant::DEFAULT, runMax);
        EXPECT_EQ(runMax, resultWithRunMax.size());
        for (uint32_t i = 0; i < runMax; ++i) {
            EXPECT_EQ(result[i].start, resultWithRunMax[i].start);
            EXPECT_EQ(result[i].end, resultWithRunMax[i].end);
            EXPECT_EQ(result[i].fakedFont, resultWithRunMax[i].fakedFont);
        }
    }
    return result;
}

// Overloaded version for default font style.
std::vector<FontCollection::Run> itemize(const std::shared_ptr<FontCollection>& collection,
                                         const char* str, const std::string& localeList) {
    return itemize(collection, str, FontStyle(), localeList);
}

// Overloaded version for empty locale list id.
std::vector<FontCollection::Run> itemize(const std::shared_ptr<FontCollection>& collection,
                                         const char* str, FontStyle style) {
    return itemize(collection, str, style, "");
}

// Overloaded version for default font style and empty locale list id.
std::vector<FontCollection::Run> itemize(const std::shared_ptr<FontCollection>& collection,
                                         const char* str) {
    return itemize(collection, str, FontStyle(), "");
}

// Utility function to obtain font path associated with run.
std::string getFontName(const FontCollection::Run& run) {
    EXPECT_NE(nullptr, run.fakedFont.font);
    return getBasename(
            ((FreeTypeMinikinFontForTest*)run.fakedFont.font->typeface().get())->fontPath());
}

// Utility function to obtain LocaleList from string.
const LocaleList& registerAndGetLocaleList(const std::string& locale_string) {
    return LocaleListCache::getById(LocaleListCache::getId(locale_string));
}

TEST(FontCollectionItemizeTest, itemize_latin) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    const FontStyle kRegularStyle = FontStyle();
    const FontStyle kItalicStyle = FontStyle(FontStyle::Slant::ITALIC);
    const FontStyle kBoldStyle = FontStyle(FontStyle::Weight::BOLD);
    const FontStyle kBoldItalicStyle = FontStyle(FontStyle::Weight::BOLD, FontStyle::Slant::ITALIC);

    auto runs = itemize(collection, "'a' 'b' 'c' 'd' 'e'", kRegularStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "'a' 'b' 'c' 'd' 'e'", kItalicStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinItalicFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "'a' 'b' 'c' 'd' 'e'", kBoldStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinBoldFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "'a' 'b' 'c' 'd' 'e'", kBoldItalicStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinBoldItalicFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // Continue if the specific characters (e.g. hyphen, comma, etc.) is
    // followed.
    runs = itemize(collection, "'a' ',' '-' 'd' '!'", kRegularStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "'a' ',' '-' 'd' '!'", kRegularStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // U+0301 (COMBINING ACUTE ACCENT) must be in the same run with preceding
    // chars if the font supports it.
    runs = itemize(collection, "'a' U+0301", kRegularStyle);
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());
}

TEST(FontCollectionItemizeTest, itemize_combining) {
    // The regular font and the Cherokee font both support U+0301 (COMBINING ACUTE ACCENT). Since
    // it's a combining mark, it should come from whatever font the base character comes from.
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    auto runs = itemize(collection, "'a' U+0301");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    // CHEROKEE LETTER A, COMBINING ACUTE ACCENT
    runs = itemize(collection, "U+13A0 U+0301");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kCherokeeFont, getFontName(runs[0]));

    // CHEROKEE LETTER A, COMBINING ACUTE ACCENT, COMBINING ACUTE ACCENT
    runs = itemize(collection, "U+13A0 U+0301 U+0301");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kCherokeeFont, getFontName(runs[0]));

    runs = itemize(collection, "U+0301");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    // COMBINING ACUTE ACCENT, CHEROKEE LETTER A, COMBINING ACUTE ACCENT
    runs = itemize(collection, "U+0301 U+13A0 U+0301");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(3, runs[1].end);
    EXPECT_EQ(kCherokeeFont, getFontName(runs[1]));
}

TEST(FontCollectionItemizeTest, itemize_emoji) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    auto runs = itemize(collection, "U+1F469 U+1F467");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // U+20E3(COMBINING ENCLOSING KEYCAP) must be in the same run with preceding
    // character if the font supports.
    runs = itemize(collection, "'0' U+20E3");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "U+1F470 U+20E3");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    runs = itemize(collection, "U+242EE U+1F470 U+20E3");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(5, runs[1].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[1]));
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeItalic());

    // Currently there is no fonts which has a glyph for 'a' + U+20E3, so they
    // are splitted into two.
    runs = itemize(collection, "'a' U+20E3");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(2, runs[1].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[1]));
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeItalic());
}

TEST(FontCollectionItemizeTest, itemize_non_latin) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    // All Japanese Hiragana characters.
    auto runs = itemize(collection, "U+3042 U+3044 U+3046 U+3048 U+304A", "ja-JP");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // All Korean Hangul characters.
    runs = itemize(collection, "U+B300 U+D55C U+BBFC U+AD6D", "en-US");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kKOFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // All Han characters ja, zh-Hans font having.
    // Japanese font should be selected if the specified language is Japanese.
    runs = itemize(collection, "U+81ED U+82B1 U+5FCD", "ja-JP");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // Simplified Chinese font should be selected if the specified language is Simplified
    // Chinese.
    runs = itemize(collection, "U+81ED U+82B1 U+5FCD", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // Fallbacks to other fonts if there is no glyph in the specified language's
    // font. There is no character U+4F60 in Japanese.
    runs = itemize(collection, "U+81ED U+4F60 U+5FCD", "ja-JP");
    ASSERT_EQ(3U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(2, runs[1].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[1]));
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(2, runs[2].start);
    EXPECT_EQ(3, runs[2].end);
    EXPECT_EQ(kJAFont, getFontName(runs[2]));
    EXPECT_FALSE(runs[2].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[2].fakedFont.fakery.isFakeItalic());

    // Tone mark.
    runs = itemize(collection, "U+4444 U+302D", "");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // Both zh-Hant and ja fonts support U+242EE, but zh-Hans doesn't.
    // Here, ja and zh-Hant font should have the same score but ja should be selected since it is
    // listed before zh-Hant.
    runs = itemize(collection, "U+242EE", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());
}

TEST(FontCollectionItemizeTest, itemize_mixed) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    auto runs = itemize(collection, "'a' U+4F60 'b' U+4F60 'c'", "en-US");
    ASSERT_EQ(5U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(2, runs[1].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[1]));
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[1].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(2, runs[2].start);
    EXPECT_EQ(3, runs[2].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[2]));
    EXPECT_FALSE(runs[2].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[2].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(3, runs[3].start);
    EXPECT_EQ(4, runs[3].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[3]));
    EXPECT_FALSE(runs[3].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[3].fakedFont.fakery.isFakeItalic());

    EXPECT_EQ(4, runs[4].start);
    EXPECT_EQ(5, runs[4].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[4]));
    EXPECT_FALSE(runs[4].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[4].fakedFont.fakery.isFakeItalic());
}

TEST(FontCollectionItemizeTest, itemize_variationSelector) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    // A glyph for U+4FAE is provided by both Japanese font and Simplified
    // Chinese font. Also a glyph for U+242EE is provided by both Japanese and
    // Traditional Chinese font.  To avoid effects of device default locale,
    // explicitly specify the locale.

    // U+4FAE is available in both zh_Hans and ja font, but U+4FAE,U+FE00 is
    // only available in ja font.
    auto runs = itemize(collection, "U+4FAE", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));

    runs = itemize(collection, "U+4FAE U+FE00", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+4FAE U+4FAE U+FE00", "zh-Hans");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(3, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));

    runs = itemize(collection, "U+4FAE U+4FAE U+FE00 U+4FAE", "zh-Hans");
    ASSERT_EQ(3U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(3, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));
    EXPECT_EQ(3, runs[2].start);
    EXPECT_EQ(4, runs[2].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[2]));

    // Validation selector after validation selector.
    runs = itemize(collection, "U+4FAE U+FE00 U+FE00", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    // No font supports U+242EE U+FE0E.
    runs = itemize(collection, "U+4FAE U+FE0E", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));

    // Surrogate pairs handling.
    // U+242EE is available in ja font and zh_Hant font.
    // U+242EE U+FE00 is available only in ja font.
    runs = itemize(collection, "U+242EE", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));

    runs = itemize(collection, "U+242EE U+FE00", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+242EE U+242EE U+FE00", "zh-Hant");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));
    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(5, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));

    runs = itemize(collection, "U+242EE U+242EE U+FE00 U+242EE", "zh-Hant");
    ASSERT_EQ(3U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));
    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(5, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));
    EXPECT_EQ(5, runs[2].start);
    EXPECT_EQ(7, runs[2].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[2]));

    // Validation selector after validation selector.
    runs = itemize(collection, "U+242EE U+FE00 U+FE00", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    // No font supports U+242EE U+FE0E
    runs = itemize(collection, "U+242EE U+FE0E", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));

    // Isolated variation selector supplement.
    runs = itemize(collection, "U+FE00", "");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_TRUE(runs[0].fakedFont.font == nullptr || kLatinFont == getFontName(runs[0]));

    runs = itemize(collection, "U+FE00", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_TRUE(runs[0].fakedFont.font == nullptr || kLatinFont == getFontName(runs[0]));

    // First font family (Regular.ttf) supports U+203C but doesn't support U+203C U+FE0F.
    // Emoji.ttf font supports U+203C U+FE0F.  Emoji.ttf should be selected.
    runs = itemize(collection, "U+203C U+FE0F", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));

    // First font family (Regular.ttf) supports U+203C U+FE0E.
    runs = itemize(collection, "U+203C U+FE0E", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_variationSelectorSupplement) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    // A glyph for U+845B is provided by both Japanese font and Simplified
    // Chinese font. Also a glyph for U+242EE is provided by both Japanese and
    // Traditional Chinese font.  To avoid effects of device default locale,
    // explicitly specify the locale.

    // U+845B is available in both zh_Hans and ja font, but U+845B,U+E0100 is
    // only available in ja font.
    auto runs = itemize(collection, "U+845B", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));

    runs = itemize(collection, "U+845B U+E0100", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+845B U+845B U+E0100", "zh-Hans");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(4, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));

    runs = itemize(collection, "U+845B U+845B U+E0100 U+845B", "zh-Hans");
    ASSERT_EQ(3U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));
    EXPECT_EQ(1, runs[1].start);
    EXPECT_EQ(4, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));
    EXPECT_EQ(4, runs[2].start);
    EXPECT_EQ(5, runs[2].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[2]));

    // Validation selector after validation selector.
    runs = itemize(collection, "U+845B U+E0100 U+E0100", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    // No font supports U+845B U+E01E0.
    runs = itemize(collection, "U+845B U+E01E0", "zh-Hans");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kZH_HansFont, getFontName(runs[0]));

    // Isolated variation selector supplement
    // Surrogate pairs handling.
    // U+242EE is available in ja font and zh_Hant font.
    // U+242EE U+E0100 is available only in ja font.
    runs = itemize(collection, "U+242EE", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));

    runs = itemize(collection, "U+242EE U+E0101", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+242EE U+242EE U+E0101", "zh-Hant");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));
    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(6, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));

    runs = itemize(collection, "U+242EE U+242EE U+E0101 U+242EE", "zh-Hant");
    ASSERT_EQ(3U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));
    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(6, runs[1].end);
    EXPECT_EQ(kJAFont, getFontName(runs[1]));
    EXPECT_EQ(6, runs[2].start);
    EXPECT_EQ(8, runs[2].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[2]));

    // Validation selector after validation selector.
    runs = itemize(collection, "U+242EE U+E0100 U+E0100", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(6, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    // No font supports U+242EE U+E01E0.
    runs = itemize(collection, "U+242EE U+E01E0", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kZH_HantFont, getFontName(runs[0]));

    // Isolated variation selector supplement.
    runs = itemize(collection, "U+E0100", "");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_TRUE(runs[0].fakedFont.font == nullptr || kLatinFont == getFontName(runs[0]));

    runs = itemize(collection, "U+E0100", "zh-Hant");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_TRUE(runs[0].fakedFont.font == nullptr || kLatinFont == getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_no_crash) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    // Broken Surrogate pairs. Check only not crashing.
    auto runs = itemize(collection, "'a' U+D83D 'a'");
    runs = itemize(collection, "'a' U+DC69 'a'");
    runs = itemize(collection, "'a' U+D83D U+D83D 'a'");
    runs = itemize(collection, "'a' U+DC69 U+DC69 'a'");

    // Isolated variation selector. Check only not crashing.
    runs = itemize(collection, "U+FE00 U+FE00");
    runs = itemize(collection, "U+E0100 U+E0100");
    runs = itemize(collection, "U+FE00 U+E0100");
    runs = itemize(collection, "U+E0100 U+FE00");

    // Tone mark only. Check only not crashing.
    runs = itemize(collection, "U+302D");
    runs = itemize(collection, "U+302D U+302D");

    // Tone mark and variation selector mixed. Check only not crashing.
    runs = itemize(collection, "U+FE00 U+302D U+E0100");
}

TEST(FontCollectionItemizeTest, itemize_fakery) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    FontStyle kBoldStyle(FontStyle::Weight::BOLD);
    FontStyle kItalicStyle(FontStyle::Slant::ITALIC);
    FontStyle kBoldItalicStyle(FontStyle::Weight::BOLD, FontStyle::Slant::ITALIC);

    // Currently there is no italic or bold font for Japanese. FontFakery has
    // the differences between desired and actual font style.

    // All Japanese Hiragana characters.
    auto runs = itemize(collection, "U+3042 U+3044 U+3046 U+3048 U+304A", kBoldStyle, "ja-JP");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_TRUE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeItalic());

    // All Japanese Hiragana characters.
    runs = itemize(collection, "U+3042 U+3044 U+3046 U+3048 U+304A", kItalicStyle, "ja-JP");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_FALSE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_TRUE(runs[0].fakedFont.fakery.isFakeItalic());

    // All Japanese Hiragana characters.
    runs = itemize(collection, "U+3042 U+3044 U+3046 U+3048 U+304A", kBoldItalicStyle, "ja-JP");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));
    EXPECT_TRUE(runs[0].fakedFont.fakery.isFakeBold());
    EXPECT_TRUE(runs[0].fakedFont.fakery.isFakeItalic());
}

TEST(FontCollectionItemizeTest, itemize_vs_sequence_but_no_base_char) {
    // kVSTestFont supports U+717D U+FE02 but doesn't support U+717D.
    // kVSTestFont should be selected for U+717D U+FE02 even if it does not support the base code
    // point.
    const std::string kVSTestFont = "VariationSelectorTest-Regular.ttf";

    std::vector<std::shared_ptr<FontFamily>> families;
    families.push_back(buildFontFamily(kLatinFont));
    families.push_back(buildFontFamily(kVSTestFont));

    std::shared_ptr<FontCollection> collection(new FontCollection(families));

    auto runs = itemize(collection, "U+717D U+FE02");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kVSTestFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_format_chars) {
    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    auto runs = itemize(collection, "'a' U+061C 'b'");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "'a' U+200D 'b'");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "U+3042 U+061C U+3042");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+061C 'b'");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "U+061C U+3042");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kJAFont, getFontName(runs[0]));

    runs = itemize(collection, "U+061C");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "U+061C U+061C U+061C");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "U+200D U+20E3");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));

    runs = itemize(collection, "U+200D");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kLatinFont, getFontName(runs[0]));

    runs = itemize(collection, "U+20E3");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kEmojiFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_LocaleScore) {
    struct TestCase {
        std::string userPreferredLocale;
        std::vector<std::string> fontLocales;
        int selectedFontIndex;
    } testCases[] = {
            // Font can specify empty locale.
            {"und", {"", ""}, 0},
            {"und", {"", "en-Latn"}, 0},
            {"en-Latn", {"", ""}, 0},
            {"en-Latn", {"", "en-Latn"}, 1},

            // Single user preferred locale.
            // Exact match case
            {"en-Latn", {"en-Latn", "ja-Jpan"}, 0},
            {"ja-Jpan", {"en-Latn", "ja-Jpan"}, 1},
            {"en-Latn", {"en-Latn", "nl-Latn", "es-Latn"}, 0},
            {"nl-Latn", {"en-Latn", "nl-Latn", "es-Latn"}, 1},
            {"es-Latn", {"en-Latn", "nl-Latn", "es-Latn"}, 2},
            {"es-Latn", {"en-Latn", "en-Latn", "nl-Latn"}, 0},

            // Exact script match case
            {"en-Latn", {"nl-Latn", "e-Latn"}, 0},
            {"en-Arab", {"nl-Latn", "ar-Arab"}, 1},
            {"en-Latn", {"be-Latn", "ar-Arab", "d-Beng"}, 0},
            {"en-Arab", {"be-Latn", "ar-Arab", "d-Beng"}, 1},
            {"en-Beng", {"be-Latn", "ar-Arab", "d-Beng"}, 2},
            {"en-Beng", {"be-Latn", "ar-Beng", "d-Beng"}, 1},
            {"zh-Hant", {"zh-Hant", "zh-Hans"}, 0},
            {"zh-Hans", {"zh-Hant", "zh-Hans"}, 1},

            // Subscript match case, e.g. Jpan supports Hira.
            {"en-Hira", {"ja-Jpan"}, 0},
            {"zh-Hani", {"zh-Hans", "zh-Hant"}, 0},
            {"zh-Hani", {"zh-Hant", "zh-Hans"}, 0},
            {"en-Hira", {"zh-Hant", "ja-Jpan", "ja-Jpan"}, 1},

            // Language match case
            {"ja-Latn", {"zh-Latn", "ja-Latn"}, 1},
            {"zh-Latn", {"zh-Latn", "ja-Latn"}, 0},
            {"ja-Latn", {"zh-Latn", "ja-Latn"}, 1},
            {"ja-Latn", {"zh-Latn", "ja-Latn", "ja-Latn"}, 1},

            // Mixed case
            // Script/subscript match is strongest.
            {"ja-Jpan", {"en-Latn", "ja-Latn", "en-Jpan"}, 2},
            {"ja-Hira", {"en-Latn", "ja-Latn", "en-Jpan"}, 2},
            {"ja-Hira", {"en-Latn", "ja-Latn", "en-Jpan", "en-Jpan"}, 2},

            // Language match only happens if the script matches.
            {"ja-Hira", {"en-Latn", "ja-Latn"}, 0},
            {"ja-Hira", {"en-Jpan", "ja-Jpan"}, 1},

            // Multiple locales.
            // Even if all fonts have the same score, use the 2nd locale for better selection.
            {"en-Latn,ja-Jpan", {"zh-Hant", "zh-Hans", "ja-Jpan"}, 2},
            {"en-Latn,nl-Latn", {"es-Latn", "be-Latn", "nl-Latn"}, 2},
            {"en-Latn,br-Latn,nl-Latn", {"es-Latn", "be-Latn", "nl-Latn"}, 2},
            {"en-Latn,br-Latn,nl-Latn", {"es-Latn", "be-Latn", "nl-Latn", "nl-Latn"}, 2},

            // Script score.
            {"en-Latn,ja-Jpan", {"en-Arab", "en-Jpan"}, 1},
            {"en-Latn,ja-Jpan", {"en-Arab", "en-Jpan", "en-Jpan"}, 1},

            // Language match case
            {"en-Latn,ja-Latn", {"bd-Latn", "ja-Latn"}, 1},
            {"en-Latn,ja-Latn", {"bd-Latn", "ja-Latn", "ja-Latn"}, 1},

            // Language match only happens if the script matches.
            {"en-Latn,ar-Arab", {"en-Beng", "ar-Arab"}, 1},

            // Multiple locales in the font settings.
            {"ko-Jamo", {"ja-Jpan", "ko-Kore", "ko-Kore,ko-Jamo"}, 2},
            {"en-Latn", {"ja-Jpan", "en-Latn,ja-Jpan"}, 1},
            {"en-Latn", {"ja-Jpan", "ja-Jpan,en-Latn"}, 1},
            {"en-Latn", {"ja-Jpan,zh-Hant", "en-Latn,ja-Jpan", "en-Latn"}, 1},
            {"en-Latn", {"zh-Hant,ja-Jpan", "ja-Jpan,en-Latn", "en-Latn"}, 1},

            // Kore = Hang + Hani, etc.
            {"ko-Kore", {"ko-Hang", "ko-Jamo,ko-Hani", "ko-Hang,ko-Hani"}, 2},
            {"ja-Hrkt", {"ja-Hira", "ja-Kana", "ja-Hira,ja-Kana"}, 2},
            {"ja-Jpan", {"ja-Hira", "ja-Kana", "ja-Hani", "ja-Hira,ja-Kana,ja-Hani"}, 3},
            {"zh-Hanb", {"zh-Hant", "zh-Bopo", "zh-Hant,zh-Bopo"}, 2},
            {"zh-Hanb", {"ja-Hanb", "zh-Hant,zh-Bopo"}, 1},

            // Language match with unified subscript bits.
            {"zh-Hanb", {"zh-Hant", "zh-Bopo", "ja-Hant,ja-Bopo", "zh-Hant,zh-Bopo"}, 3},
            {"zh-Hanb", {"zh-Hant", "zh-Bopo", "ja-Hant,zh-Bopo", "zh-Hant,zh-Bopo"}, 3},

            // Two elements subtag matching: language and subtag or language or script.
            {"ja-Kana-u-em-emoji", {"zh-Hant", "ja-Kana"}, 1},
            {"ja-Kana-u-em-emoji", {"zh-Hant", "ja-Kana", "ja-Zsye"}, 2},
            {"ja-Zsym-u-em-emoji", {"ja-Kana", "ja-Zsym", "ja-Zsye"}, 2},

            // One element subtag matching: subtag only or script only.
            {"en-Latn-u-em-emoji", {"ja-Latn", "ja-Zsye"}, 1},
            {"en-Zsym-u-em-emoji", {"ja-Zsym", "ja-Zsye"}, 1},
            {"en-Zsye-u-em-text", {"ja-Zsym", "ja-Zsye"}, 0},

            // Multiple locale list with subtags.
            {"en-Latn,ja-Jpan-u-em-text", {"en-Latn", "en-Zsye", "en-Zsym"}, 0},
            {"en-Latn,en-Zsye,ja-Jpan-u-em-text", {"zh", "en-Zsye", "en-Zsym"}, 1},
    };

    for (auto testCase : testCases) {
        std::string fontLocaleStr = "{";
        for (size_t i = 0; i < testCase.fontLocales.size(); ++i) {
            if (i != 0) {
                fontLocaleStr += ", ";
            }
            fontLocaleStr += "\"" + testCase.fontLocales[i] + "\"";
        }
        fontLocaleStr += "}";
        SCOPED_TRACE("Test of user preferred locale: \"" + testCase.userPreferredLocale +
                     "\" with font locale: " + fontLocaleStr);

        std::vector<std::shared_ptr<FontFamily>> families;

        // Prepare first font which doesn't supports U+9AA8
        auto firstFamilyMinikinFont =
                std::make_shared<FreeTypeMinikinFontForTest>(getTestFontPath(kNoGlyphFont));
        std::vector<Font> fonts;
        fonts.push_back(Font::Builder(firstFamilyMinikinFont).build());
        auto firstFamily =
                std::make_shared<FontFamily>(registerLocaleList("und"), FamilyVariant::DEFAULT,
                                             std::move(fonts), false /* isCustomFallback */);
        families.push_back(firstFamily);

        // Prepare font families
        // Each font family is associated with a specified locale. All font families except for
        // the first font support U+9AA8.
        std::unordered_map<MinikinFont*, int> fontLocaleIdxMap;

        for (size_t i = 0; i < testCase.fontLocales.size(); ++i) {
            auto minikinFont =
                    std::make_shared<FreeTypeMinikinFontForTest>(getTestFontPath(kJAFont));
            std::vector<Font> fonts;
            fonts.push_back(Font::Builder(minikinFont).build());
            auto family = std::make_shared<FontFamily>(registerLocaleList(testCase.fontLocales[i]),
                                                       FamilyVariant::DEFAULT, std::move(fonts),
                                                       false /* isCustomFallback */);
            families.push_back(family);
            fontLocaleIdxMap.insert(std::make_pair(minikinFont.get(), i));
        }
        std::shared_ptr<FontCollection> collection(new FontCollection(families));
        // Do itemize
        auto runs = itemize(collection, "U+9AA8", testCase.userPreferredLocale);
        ASSERT_EQ(1U, runs.size());
        ASSERT_NE(nullptr, runs[0].fakedFont.font);

        // First family doesn't support U+9AA8 and others support it, so the first font should not
        // be selected.
        EXPECT_NE(firstFamilyMinikinFont.get(), runs[0].fakedFont.font->typeface().get());

        // Lookup used font family by MinikinFont*.
        const int usedLocaleIndex = fontLocaleIdxMap[runs[0].fakedFont.font->typeface().get()];
        EXPECT_EQ(testCase.selectedFontIndex, usedLocaleIndex);
    }
}

TEST(FontCollectionItemizeTest, itemize_LocaleAndCoverage) {
    struct TestCase {
        std::string testString;
        std::string requestedLocales;
        std::string expectedFont;
    } testCases[] = {
            // Following test cases verify that following rules in font fallback chain.
            // - If the first font in the collection supports the given character or variation
            // sequence,
            //   it should be selected.
            // - If the font doesn't support the given character, variation sequence or its base
            //   character, it should not be selected.
            // - If two or more fonts match the requested locales, the font matches with the highest
            //   priority locale should be selected.
            // - If two or more fonts get the same score, the font listed earlier in the XML file
            //   (here, kItemizeFontXml) should be selected.

            // Regardless of locale, the first font is always selected if it covers the code point.
            {"'a'", "", kLatinFont},
            {"'a'", "en-Latn", kLatinFont},
            {"'a'", "ja-Jpan", kLatinFont},
            {"'a'", "ja-Jpan,en-Latn", kLatinFont},
            {"'a'", "zh-Hans,zh-Hant,en-Latn,ja-Jpan,fr-Latn", kLatinFont},

            // U+81ED is supported by both the ja font and zh-Hans font.
            {"U+81ED", "", kZH_HansFont},         // zh-Hans font is listed before ja font.
            {"U+81ED", "en-Latn", kZH_HansFont},  // zh-Hans font is listed before ja font.
            {"U+81ED", "ja-Jpan", kJAFont},
            {"U+81ED", "zh-Hans", kZH_HansFont},

            {"U+81ED", "ja-Jpan,en-Latn", kJAFont},
            {"U+81ED", "en-Latn,ja-Jpan", kJAFont},
            {"U+81ED", "en-Latn,zh-Hans", kZH_HansFont},
            {"U+81ED", "zh-Hans,en-Latn", kZH_HansFont},
            {"U+81ED", "ja-Jpan,zh-Hans", kJAFont},
            {"U+81ED", "zh-Hans,ja-Jpan", kZH_HansFont},

            {"U+81ED", "en-Latn,zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+81ED", "en-Latn,ja-Jpan,zh-Hans", kJAFont},
            {"U+81ED", "en-Latn,zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+81ED", "ja-Jpan,en-Latn,zh-Hans", kJAFont},
            {"U+81ED", "ja-Jpan,zh-Hans,en-Latn", kJAFont},
            {"U+81ED", "zh-Hans,en-Latn,ja-Jpan", kZH_HansFont},
            {"U+81ED", "zh-Hans,ja-Jpan,en-Latn", kZH_HansFont},

            // U+304A is only supported by ja font.
            {"U+304A", "", kJAFont},
            {"U+304A", "ja-Jpan", kJAFont},
            {"U+304A", "zh-Hant", kJAFont},
            {"U+304A", "zh-Hans", kJAFont},

            {"U+304A", "ja-Jpan,zh-Hant", kJAFont},
            {"U+304A", "zh-Hant,ja-Jpan", kJAFont},
            {"U+304A", "zh-Hans,zh-Hant", kJAFont},
            {"U+304A", "zh-Hant,zh-Hans", kJAFont},
            {"U+304A", "zh-Hans,ja-Jpan", kJAFont},
            {"U+304A", "ja-Jpan,zh-Hans", kJAFont},

            {"U+304A", "zh-Hans,ja-Jpan,zh-Hant", kJAFont},
            {"U+304A", "zh-Hans,zh-Hant,ja-Jpan", kJAFont},
            {"U+304A", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+304A", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+304A", "zh-Hant,zh-Hans,ja-Jpan", kJAFont},
            {"U+304A", "zh-Hant,ja-Jpan,zh-Hans", kJAFont},

            // U+242EE is supported by both ja font and zh-Hant fonts but not by zh-Hans font.
            {"U+242EE", "", kJAFont},  // ja font is listed before zh-Hant font.
            {"U+242EE", "ja-Jpan", kJAFont},
            {"U+242EE", "zh-Hans", kJAFont},
            {"U+242EE", "zh-Hant", kZH_HantFont},

            {"U+242EE", "ja-Jpan,zh-Hant", kJAFont},
            {"U+242EE", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+242EE", "zh-Hans,zh-Hant", kZH_HantFont},
            {"U+242EE", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+242EE", "zh-Hans,ja-Jpan", kJAFont},
            {"U+242EE", "ja-Jpan,zh-Hans", kJAFont},

            {"U+242EE", "zh-Hans,ja-Jpan,zh-Hant", kJAFont},
            {"U+242EE", "zh-Hans,zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+242EE", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+242EE", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+242EE", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+242EE", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // U+9AA8 is supported by all ja-Jpan, zh-Hans, zh-Hant fonts.
            {"U+9AA8", "", kZH_HansFont},  // zh-Hans font is listed before ja and zh-Hant fonts.
            {"U+9AA8", "ja-Jpan", kJAFont},
            {"U+9AA8", "zh-Hans", kZH_HansFont},
            {"U+9AA8", "zh-Hant", kZH_HantFont},

            {"U+9AA8", "ja-Jpan,zh-Hant", kJAFont},
            {"U+9AA8", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+9AA8", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+9AA8", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+9AA8", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+9AA8", "ja-Jpan,zh-Hans", kJAFont},

            {"U+9AA8", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+9AA8", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+9AA8", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+9AA8", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+9AA8", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+9AA8", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // U+242EE U+FE00 is supported by ja font but not by zh-Hans or zh-Hant fonts.
            {"U+242EE U+FE00", "", kJAFont},
            {"U+242EE U+FE00", "ja-Jpan", kJAFont},
            {"U+242EE U+FE00", "zh-Hant", kJAFont},
            {"U+242EE U+FE00", "zh-Hans", kJAFont},

            {"U+242EE U+FE00", "ja-Jpan,zh-Hant", kJAFont},
            {"U+242EE U+FE00", "zh-Hant,ja-Jpan", kJAFont},
            {"U+242EE U+FE00", "zh-Hans,zh-Hant", kJAFont},
            {"U+242EE U+FE00", "zh-Hant,zh-Hans", kJAFont},
            {"U+242EE U+FE00", "zh-Hans,ja-Jpan", kJAFont},
            {"U+242EE U+FE00", "ja-Jpan,zh-Hans", kJAFont},

            {"U+242EE U+FE00", "zh-Hans,ja-Jpan,zh-Hant", kJAFont},
            {"U+242EE U+FE00", "zh-Hans,zh-Hant,ja-Jpan", kJAFont},
            {"U+242EE U+FE00", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+242EE U+FE00", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+242EE U+FE00", "zh-Hant,zh-Hans,ja-Jpan", kJAFont},
            {"U+242EE U+FE00", "zh-Hant,ja-Jpan,zh-Hans", kJAFont},

            // U+3402 U+E0100 is supported by both zh-Hans and zh-Hant but not by ja font.
            {"U+3402 U+E0100", "", kZH_HansFont},  // zh-Hans font is listed before zh-Hant font.
            {"U+3402 U+E0100", "ja-Jpan",
             kZH_HansFont},  // zh-Hans font is listed before zh-Hant font.
            {"U+3402 U+E0100", "zh-Hant", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hans", kZH_HansFont},

            {"U+3402 U+E0100", "ja-Jpan,zh-Hant", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+3402 U+E0100", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+3402 U+E0100", "ja-Jpan,zh-Hans", kZH_HansFont},

            {"U+3402 U+E0100", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+3402 U+E0100", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+3402 U+E0100", "ja-Jpan,zh-Hans,zh-Hant", kZH_HansFont},
            {"U+3402 U+E0100", "ja-Jpan,zh-Hant,zh-Hans", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+3402 U+E0100", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // No font supports U+4444 U+FE00 but only zh-Hans supports its base character U+4444.
            {"U+4444 U+FE00", "", kZH_HansFont},
            {"U+4444 U+FE00", "ja-Jpan", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hant", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hans", kZH_HansFont},

            {"U+4444 U+FE00", "ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hant,zh-Hans", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+4444 U+FE00", "ja-Jpan,zh-Hans", kZH_HansFont},

            {"U+4444 U+FE00", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+4444 U+FE00", "ja-Jpan,zh-Hans,zh-Hant", kZH_HansFont},
            {"U+4444 U+FE00", "ja-Jpan,zh-Hant,zh-Hans", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hant,zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+4444 U+FE00", "zh-Hant,ja-Jpan,zh-Hans", kZH_HansFont},

            // No font supports U+81ED U+E0100 but ja and zh-Hans support its base character U+81ED.
            // zh-Hans font is listed before ja font.
            {"U+81ED U+E0100", "", kZH_HansFont},
            {"U+81ED U+E0100", "ja-Jpan", kJAFont},
            {"U+81ED U+E0100", "zh-Hant", kZH_HansFont},
            {"U+81ED U+E0100", "zh-Hans", kZH_HansFont},

            {"U+81ED U+E0100", "ja-Jpan,zh-Hant", kJAFont},
            {"U+81ED U+E0100", "zh-Hant,ja-Jpan", kJAFont},
            {"U+81ED U+E0100", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+81ED U+E0100", "zh-Hant,zh-Hans", kZH_HansFont},
            {"U+81ED U+E0100", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+81ED U+E0100", "ja-Jpan,zh-Hans", kJAFont},

            {"U+81ED U+E0100", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+81ED U+E0100", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+81ED U+E0100", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+81ED U+E0100", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+81ED U+E0100", "zh-Hant,zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+81ED U+E0100", "zh-Hant,ja-Jpan,zh-Hans", kJAFont},

            // No font supports U+9AA8 U+E0100 but all zh-Hans zh-hant ja fonts support its base
            // character U+9AA8.
            // zh-Hans font is listed before ja and zh-Hant fonts.
            {"U+9AA8 U+E0100", "", kZH_HansFont},
            {"U+9AA8 U+E0100", "ja-Jpan", kJAFont},
            {"U+9AA8 U+E0100", "zh-Hans", kZH_HansFont},
            {"U+9AA8 U+E0100", "zh-Hant", kZH_HantFont},

            {"U+9AA8 U+E0100", "ja-Jpan,zh-Hant", kJAFont},
            {"U+9AA8 U+E0100", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+9AA8 U+E0100", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+9AA8 U+E0100", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+9AA8 U+E0100", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+9AA8 U+E0100", "ja-Jpan,zh-Hans", kJAFont},

            {"U+9AA8 U+E0100", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+9AA8 U+E0100", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+9AA8 U+E0100", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+9AA8 U+E0100", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+9AA8 U+E0100", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+9AA8 U+E0100", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // All zh-Hans,zh-Hant,ja fonts support U+35A8 U+E0100 and its base character U+35A8.
            // zh-Hans font is listed before ja and zh-Hant fonts.
            {"U+35A8", "", kZH_HansFont},
            {"U+35A8", "ja-Jpan", kJAFont},
            {"U+35A8", "zh-Hans", kZH_HansFont},
            {"U+35A8", "zh-Hant", kZH_HantFont},

            {"U+35A8", "ja-Jpan,zh-Hant", kJAFont},
            {"U+35A8", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+35A8", "zh-Hans,zh-Hant", kZH_HansFont},
            {"U+35A8", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+35A8", "zh-Hans,ja-Jpan", kZH_HansFont},
            {"U+35A8", "ja-Jpan,zh-Hans", kJAFont},

            {"U+35A8", "zh-Hans,ja-Jpan,zh-Hant", kZH_HansFont},
            {"U+35A8", "zh-Hans,zh-Hant,ja-Jpan", kZH_HansFont},
            {"U+35A8", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+35A8", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+35A8", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+35A8", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // All zh-Hans,zh-Hant,ja fonts support U+35B6 U+E0100, but zh-Hant and ja fonts support
            // its
            // base character U+35B6.
            // ja font is listed before zh-Hant font.
            {"U+35B6", "", kJAFont},
            {"U+35B6", "ja-Jpan", kJAFont},
            {"U+35B6", "zh-Hant", kZH_HantFont},
            {"U+35B6", "zh-Hans", kJAFont},

            {"U+35B6", "ja-Jpan,zh-Hant", kJAFont},
            {"U+35B6", "zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+35B6", "zh-Hans,zh-Hant", kZH_HantFont},
            {"U+35B6", "zh-Hant,zh-Hans", kZH_HantFont},
            {"U+35B6", "zh-Hans,ja-Jpan", kJAFont},
            {"U+35B6", "ja-Jpan,zh-Hans", kJAFont},

            {"U+35B6", "zh-Hans,ja-Jpan,zh-Hant", kJAFont},
            {"U+35B6", "zh-Hans,zh-Hant,ja-Jpan", kZH_HantFont},
            {"U+35B6", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+35B6", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+35B6", "zh-Hant,zh-Hans,ja-Jpan", kZH_HantFont},
            {"U+35B6", "zh-Hant,ja-Jpan,zh-Hans", kZH_HantFont},

            // All zh-Hans,zh-Hant,ja fonts support U+35C5 U+E0100, but only ja font supports its
            // base
            // character U+35C5.
            {"U+35C5", "", kJAFont},
            {"U+35C5", "ja-Jpan", kJAFont},
            {"U+35C5", "zh-Hant", kJAFont},
            {"U+35C5", "zh-Hans", kJAFont},

            {"U+35C5", "ja-Jpan,zh-Hant", kJAFont},
            {"U+35C5", "zh-Hant,ja-Jpan", kJAFont},
            {"U+35C5", "zh-Hans,zh-Hant", kJAFont},
            {"U+35C5", "zh-Hant,zh-Hans", kJAFont},
            {"U+35C5", "zh-Hans,ja-Jpan", kJAFont},
            {"U+35C5", "ja-Jpan,zh-Hans", kJAFont},

            {"U+35C5", "zh-Hans,ja-Jpan,zh-Hant", kJAFont},
            {"U+35C5", "zh-Hans,zh-Hant,ja-Jpan", kJAFont},
            {"U+35C5", "ja-Jpan,zh-Hans,zh-Hant", kJAFont},
            {"U+35C5", "ja-Jpan,zh-Hant,zh-Hans", kJAFont},
            {"U+35C5", "zh-Hant,zh-Hans,ja-Jpan", kJAFont},
            {"U+35C5", "zh-Hant,ja-Jpan,zh-Hans", kJAFont},

            // None of ja-Jpan, zh-Hant, zh-Hans font supports U+1F469. Emoji font supports it.
            {"U+1F469", "", kEmojiFont},
            {"U+1F469", "ja-Jpan", kEmojiFont},
            {"U+1F469", "zh-Hant", kEmojiFont},
            {"U+1F469", "zh-Hans", kEmojiFont},

            {"U+1F469", "ja-Jpan,zh-Hant", kEmojiFont},
            {"U+1F469", "zh-Hant,ja-Jpan", kEmojiFont},
            {"U+1F469", "zh-Hans,zh-Hant", kEmojiFont},
            {"U+1F469", "zh-Hant,zh-Hans", kEmojiFont},
            {"U+1F469", "zh-Hans,ja-Jpan", kEmojiFont},
            {"U+1F469", "ja-Jpan,zh-Hans", kEmojiFont},

            {"U+1F469", "zh-Hans,ja-Jpan,zh-Hant", kEmojiFont},
            {"U+1F469", "zh-Hans,zh-Hant,ja-Jpan", kEmojiFont},
            {"U+1F469", "ja-Jpan,zh-Hans,zh-Hant", kEmojiFont},
            {"U+1F469", "ja-Jpan,zh-Hant,zh-Hans", kEmojiFont},
            {"U+1F469", "zh-Hant,zh-Hans,ja-Jpan", kEmojiFont},
            {"U+1F469", "zh-Hant,ja-Jpan,zh-Hans", kEmojiFont},
    };

    auto collection = buildFontCollectionFromXml(kItemizeFontXml);

    for (const auto& testCase : testCases) {
        SCOPED_TRACE("Test for \"" + testCase.testString + "\" with locales " +
                     testCase.requestedLocales);

        auto runs = itemize(collection, testCase.testString.c_str(), testCase.requestedLocales);
        ASSERT_EQ(1U, runs.size());
        EXPECT_EQ(testCase.expectedFont, getFontName(runs[0]));
    }
}

TEST(FontCollectionItemizeTest, itemize_emojiSelection_withFE0E) {
    auto collection = buildFontCollectionFromXml(kEmojiXmlFile);

    // U+00A9 is a text default emoji which is only available in TextEmojiFont.ttf.
    // TextEmojiFont.ttf should be selected.
    auto runs = itemize(collection, "U+00A9 U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+00A9 is a text default emoji which is only available in ColorEmojiFont.ttf.
    // ColorEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+00AE U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    // Text emoji is specified but it is not available. Use color emoji instead.
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+203C is a text default emoji which is available in both TextEmojiFont.ttf and
    // ColorEmojiFont.ttf. TextEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+203C U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+2049 is a text default emoji which is not available either TextEmojiFont.ttf or
    // ColorEmojiFont.ttf. No font should be selected.
    runs = itemize(collection, "U+2049 U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));

    // U+231A is a emoji default emoji which is available only in TextEmojifFont.
    // TextEmojiFont.ttf sohuld be selected.
    runs = itemize(collection, "U+231A U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+231B is a emoji default emoji which is available only in ColorEmojiFont.ttf.
    // ColorEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+231B U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    // Text emoji is specified but it is not available. Use color emoji instead.
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+23E9 is a emoji default emoji which is available in both TextEmojiFont.ttf and
    // ColorEmojiFont.ttf. TextEmojiFont.ttf should be selected even if U+23E9 is emoji default
    // emoji since U+FE0E is appended.
    runs = itemize(collection, "U+23E9 U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+23EA is a emoji default emoji but which is not available in either TextEmojiFont.ttf or
    // ColorEmojiFont.ttf. No font should be selected.
    runs = itemize(collection, "U+23EA U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));

    // U+26FA U+FE0E is specified but ColorTextMixedEmojiFont has a variation sequence U+26F9 U+FE0F
    // in its cmap, so ColorTextMixedEmojiFont should be selected instaed of ColorEmojiFont.
    runs = itemize(collection, "U+26FA U+FE0E");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kMixedEmojiFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_emojiSelection_withFE0F) {
    auto collection = buildFontCollectionFromXml(kEmojiXmlFile);

    // U+00A9 is a text default emoji which is available only in TextEmojiFont.ttf.
    // TextEmojiFont.ttf shoudl be selected.
    auto runs = itemize(collection, "U+00A9 U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    // Color emoji is specified but it is not available. Use text representaion instead.
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+00AE is a text default emoji which is available only in ColorEmojiFont.ttf.
    // ColorEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+00AE U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+203C is a text default emoji which is available in both TextEmojiFont.ttf and
    // ColorEmojiFont.ttf. ColorEmojiFont.ttf should be selected even if U+203C is a text default
    // emoji since U+FF0F is appended.
    runs = itemize(collection, "U+203C U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+2049 is a text default emoji which is not available in either TextEmojiFont.ttf or
    // ColorEmojiFont.ttf. No font should be selected.
    runs = itemize(collection, "U+2049 U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));

    // U+231A is a emoji default emoji which is available only in TextEmojiFont.ttf.
    // TextEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+231A U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    // Color emoji is specified but it is not available. Use text representation instead.
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // U+231B is a emoji default emoji which is available only in ColorEmojiFont.ttf.
    // ColorEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+231B U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+23E9 is a emoji default emoji which is available in both TextEmojiFont.ttf and
    // ColorEmojiFont.ttf. ColorEmojiFont.ttf should be selected.
    runs = itemize(collection, "U+23E9 U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // U+23EA is a emoji default emoji which is not available in either TextEmojiFont.ttf or
    // ColorEmojiFont.ttf. No font should be selected.
    runs = itemize(collection, "U+23EA U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));

    // U+26F9 U+FE0F is specified but ColorTextMixedEmojiFont has a variation sequence U+26F9 U+FE0F
    // in its cmap, so ColorTextMixedEmojiFont should be selected instaed of ColorEmojiFont.
    runs = itemize(collection, "U+26F9 U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kMixedEmojiFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_emojiSelection_with_skinTone) {
    auto collection = buildFontCollectionFromXml(kEmojiXmlFile);

    // TextEmoji font is selected since it is listed before ColorEmoji font.
    auto runs = itemize(collection, "U+261D");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(1, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));

    // If skin tone is specified, it should be colored.
    runs = itemize(collection, "U+261D U+1F3FD");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(3, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // Still color font is selected if an emoji variation selector is specified.
    runs = itemize(collection, "U+261D U+FE0F U+1F3FD");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    // Text font should be selected if a text variation selector is specified and skin tone is
    // rendered by itself.
    runs = itemize(collection, "U+261D U+FE0E U+1F3FD");
    ASSERT_EQ(2U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kTextEmojiFont, getFontName(runs[0]));
    EXPECT_EQ(2, runs[1].start);
    EXPECT_EQ(4, runs[1].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[1]));
}

TEST(FontCollectionItemizeTest, itemize_PrivateUseArea) {
    auto collection = buildFontCollectionFromXml(kEmojiXmlFile);

    // Should not set nullptr to the result run. (Issue 26808815)
    auto runs = itemize(collection, "U+FEE10");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(2, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));

    runs = itemize(collection, "U+FEE40 U+FE4C5");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kNoGlyphFont, getFontName(runs[0]));
}

TEST(FontCollectionItemizeTest, itemize_genderBalancedEmoji) {
    auto collection = buildFontCollectionFromXml(kEmojiXmlFile);

    auto runs = itemize(collection, "U+1F469 U+200D U+1F373");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    runs = itemize(collection, "U+1F469 U+200D U+2695 U+FE0F");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(5, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));

    runs = itemize(collection, "U+1F469 U+200D U+2695");
    ASSERT_EQ(1U, runs.size());
    EXPECT_EQ(0, runs[0].start);
    EXPECT_EQ(4, runs[0].end);
    EXPECT_EQ(kColorEmojiFont, getFontName(runs[0]));
}

// For b/29585939
TEST(FontCollectionItemizeTest, itemizeShouldKeepOrderForVS) {
    std::shared_ptr<FontFamily> dummyFamily = buildFontFamily(kNoGlyphFont);
    std::shared_ptr<FontFamily> familyA = buildFontFamily(kZH_HansFont);
    std::shared_ptr<FontFamily> familyB = buildFontFamily(kZH_HansFont);

    std::vector<std::shared_ptr<FontFamily>> families = {dummyFamily, familyA, familyB};
    std::vector<std::shared_ptr<FontFamily>> reversedFamilies = {dummyFamily, familyB, familyA};

    std::shared_ptr<FontCollection> collection(new FontCollection(families));
    std::shared_ptr<FontCollection> reversedCollection(new FontCollection(reversedFamilies));

    // Both fontA/fontB support U+35A8 but don't support U+35A8 U+E0100. The first font should be
    // selected.
    auto runs = itemize(collection, "U+35A8 U+E0100");
    EXPECT_EQ(familyA->getFont(0), runs[0].fakedFont.font);

    runs = itemize(reversedCollection, "U+35A8 U+E0100");
    EXPECT_EQ(familyB->getFont(0), runs[0].fakedFont.font);
}

// For b/29585939
TEST(FontCollectionItemizeTest, itemizeShouldKeepOrderForVS2) {
    std::shared_ptr<FontFamily> dummyFamily = buildFontFamily(kNoGlyphFont);
    std::shared_ptr<FontFamily> hasCmapFormat14Family = buildFontFamily(kHasCmapFormat14Font);
    std::shared_ptr<FontFamily> noCmapFormat14Family = buildFontFamily(kNoCmapFormat14Font);

    std::vector<std::shared_ptr<FontFamily>> families = {dummyFamily, hasCmapFormat14Family,
                                                         noCmapFormat14Family};
    std::vector<std::shared_ptr<FontFamily>> reversedFamilies = {dummyFamily, noCmapFormat14Family,
                                                                 hasCmapFormat14Family};

    std::shared_ptr<FontCollection> collection(new FontCollection(families));
    std::shared_ptr<FontCollection> reversedCollection(new FontCollection(reversedFamilies));

    // Both hasCmapFormat14Font/noCmapFormat14Font support U+5380 but don't support U+5380 U+E0100.
    // The first font should be selected.
    auto runs = itemize(collection, "U+5380 U+E0100");
    EXPECT_EQ(hasCmapFormat14Family->getFont(0), runs[0].fakedFont.font);

    runs = itemize(reversedCollection, "U+5380 U+E0100");
    EXPECT_EQ(noCmapFormat14Family->getFont(0), runs[0].fakedFont.font);
}

TEST(FontCollectionItemizeTest, colorEmojiSelectionTest) {
    auto dummyFamily = buildFontFamily(kNoGlyphFont);
    auto textEmojiFamily = buildFontFamily(kTextEmojiFont, "ja-JP");
    auto colorEmojiFamily = buildFontFamily(kColorEmojiFont, "und-Zsye");

    std::vector<std::shared_ptr<FontFamily>> families = {dummyFamily, textEmojiFamily,
                                                         colorEmojiFamily};
    auto collection = std::make_shared<FontCollection>(families);
    // Both textEmojiFamily and colorEmojiFamily supports U+203C and U+23E9.
    // U+203C is text default emoji, and U+23E9 is color default emoji.
    auto runs = itemize(collection, "U+203C", "en-US,en-Zsym");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "en-US,en-Zsym");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "en-US,en-Zsye");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "en-US,en-Zsye");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-Zsym-JP");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-Zsym-JP");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-Zsye-JP");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-Zsye-JP");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-JP-u-em-text");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-JP-u-em-text");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-JP-u-em-emoji");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-JP-u-em-emoji");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-JP,und-Zsym");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-JP,und-Zsym");
    EXPECT_EQ(textEmojiFamily->getFont(0), runs[0].fakedFont.font);

    runs = itemize(collection, "U+203C", "ja-JP,und-Zsye");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "U+23E9", "ja-JP,und-Zsye");
    EXPECT_EQ(colorEmojiFamily->getFont(0), runs[0].fakedFont.font);
}

TEST(FontCollectionItemizeTest, customFallbackTest) {
    auto firstFamily = buildFontFamily(kNoGlyphFont);
    auto customFallbackFamily = buildFontFamily(kAsciiFont, "", true /* isCustomFallback */);
    auto languageFamily = buildFontFamily(kAsciiFont, "ja-JP");

    std::vector<std::shared_ptr<FontFamily>> families = {firstFamily, customFallbackFamily,
                                                         languageFamily};

    auto collection = std::make_shared<FontCollection>(families);

    auto runs = itemize(collection, "'a'", "");
    EXPECT_EQ(customFallbackFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "'a'", "en-US");
    EXPECT_EQ(customFallbackFamily->getFont(0), runs[0].fakedFont.font);
    runs = itemize(collection, "'a'", "ja-JP");
    EXPECT_EQ(customFallbackFamily->getFont(0), runs[0].fakedFont.font);
}

}  // namespace minikin
