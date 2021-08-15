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

#include "minikin/Layout.h"

#include <gtest/gtest.h>

#include "minikin/LayoutCache.h"

#include "FontTestUtils.h"
#include "LocaleListCache.h"
#include "UnicodeUtils.h"

namespace minikin {

class TestableLayoutCache : public LayoutCache {
public:
    TestableLayoutCache(uint32_t maxEntries) : LayoutCache(maxEntries) {}
    using LayoutCache::getCacheSize;
};

class LayoutCapture {
public:
    LayoutCapture() {}

    void operator()(const LayoutPiece& layout, const MinikinPaint& /* dir */) { mLayout = &layout; }

    const LayoutPiece* get() const { return mLayout; }

private:
    const LayoutPiece* mLayout;
};

TEST(LayoutCacheTest, cacheHitTest) {
    auto text = utf8ToUtf16("android");
    Range range(0, text.size());
    MinikinPaint paint(buildFontCollection("Ascii.ttf"));

    TestableLayoutCache layoutCache(10);

    LayoutCapture layout1;
    layoutCache.getOrCreate(text, range, paint, false /* LTR */, StartHyphenEdit::NO_EDIT,
                            EndHyphenEdit::NO_EDIT, layout1);

    LayoutCapture layout2;
    layoutCache.getOrCreate(text, range, paint, false /* LTR */, StartHyphenEdit::NO_EDIT,
                            EndHyphenEdit::NO_EDIT, layout2);

    EXPECT_EQ(layout1.get(), layout2.get());
}

TEST(LayoutCacheTest, cacheMissTest) {
    auto text1 = utf8ToUtf16("android");
    auto text2 = utf8ToUtf16("ANDROID");
    MinikinPaint paint(buildFontCollection("Ascii.ttf"));

    TestableLayoutCache layoutCache(10);

    LayoutCapture layout1;
    LayoutCapture layout2;

    {
        SCOPED_TRACE("Different text");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text2, Range(0, text2.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different range");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text1, Range(1, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different text");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text2, Range(0, text2.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different direction");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, true /* RTL */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different start hyphenation");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::INSERT_HYPHEN, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different end hyphen");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::INSERT_HYPHEN, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different collection");
        MinikinPaint paint1(buildFontCollection("Ascii.ttf"));
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(buildFontCollection("Emoji.ttf"));
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different size");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.size = 10.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.size = 20.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different scale X");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.scaleX = 1.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.scaleX = 2.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different skew X");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.skewX = 1.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.skewX = 2.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different letter spacing");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.letterSpacing = 0.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.letterSpacing = 1.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different word spacing");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.wordSpacing = 0.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.wordSpacing = 1.0f;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different paint flags");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.fontFlags = 0;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.fontFlags = LinearMetrics_Flag;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different locale list ID");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.localeListId = LocaleListCache::getId("en-US");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.localeListId = LocaleListCache::getId("ja-JP");
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different family variant");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.familyVariant = FamilyVariant::DEFAULT;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.familyVariant = FamilyVariant::COMPACT;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
    {
        SCOPED_TRACE("Different font feature settings");
        auto collection = buildFontCollection("Ascii.ttf");
        MinikinPaint paint1(collection);
        paint1.fontFeatureSettings = "";
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint1, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout1);
        MinikinPaint paint2(collection);
        paint2.fontFeatureSettings = "'liga' on";
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint2, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
        EXPECT_NE(layout1.get(), layout2.get());
    }
}

TEST(LayoutCacheTest, cacheOverflowTest) {
    auto text = utf8ToUtf16("android");
    Range range(0, text.size());
    MinikinPaint paint(buildFontCollection("Ascii.ttf"));

    TestableLayoutCache layoutCache(5);

    LayoutCapture layout1;
    layoutCache.getOrCreate(text, range, paint, false /* LTR */, StartHyphenEdit::NO_EDIT,
                            EndHyphenEdit::NO_EDIT, layout1);

    for (char c = 'a'; c <= 'z'; c++) {
        auto text1 = utf8ToUtf16(std::string(10, c));
        LayoutCapture layout2;
        layoutCache.getOrCreate(text1, Range(0, text1.size()), paint, false /* LTR */,
                                StartHyphenEdit::NO_EDIT, EndHyphenEdit::NO_EDIT, layout2);
    }

    LayoutCapture layout3;
    layoutCache.getOrCreate(text, range, paint, false /* LTR */, StartHyphenEdit::NO_EDIT,
                            EndHyphenEdit::NO_EDIT, layout3);
    EXPECT_NE(layout1.get(), layout3.get());
}

TEST(LayoutCacheTest, cacheLengthLimitTest) {
    auto text = utf8ToUtf16(std::string(130, 'a'));
    Range range(0, text.size());
    MinikinPaint paint(buildFontCollection("Ascii.ttf"));

    TestableLayoutCache layoutCache(140);

    LayoutCapture layout;
    layoutCache.getOrCreate(text, range, paint, false /* LTR */, StartHyphenEdit::NO_EDIT,
                            EndHyphenEdit::NO_EDIT, layout);

    EXPECT_EQ(layoutCache.getCacheSize(), 0u);
}

}  // namespace minikin
