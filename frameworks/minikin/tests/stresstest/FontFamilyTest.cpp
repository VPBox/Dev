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

#include "minikin/FontFamily.h"

#include <gtest/gtest.h>
#include <hb.h>

#include "minikin/FontCollection.h"

#include "FontTestUtils.h"
#include "FreeTypeMinikinFontForTest.h"
#include "MinikinInternal.h"

namespace minikin {

typedef std::pair<std::string, int> TestParam;

class FontFamilyHarfBuzzCompatibilityTest : public ::testing::TestWithParam<TestParam> {};

TEST_P(FontFamilyHarfBuzzCompatibilityTest, CoverageTest) {
    const std::string& fontPath = GetParam().first;
    int ttcIndex = GetParam().second;

    auto font = std::make_shared<FreeTypeMinikinFontForTest>(fontPath);
    std::vector<Font> fonts;
    fonts.push_back(Font::Builder(font).build());
    std::shared_ptr<FontFamily> family = std::make_shared<FontFamily>(std::move(fonts));

    hb_font_t* hbFont = family->getFont(0)->baseFont().get();

    for (uint32_t codePoint = 0; codePoint < MAX_UNICODE_CODE_POINT; ++codePoint) {
        uint32_t unusedGlyph;
        EXPECT_EQ(family->hasGlyph(codePoint, 0 /* variation selector */),
                  static_cast<bool>(hb_font_get_glyph(hbFont, codePoint, 0 /* variation selector */,
                                                      &unusedGlyph)));
    }

    for (uint32_t vs = VS1; vs < VS256; ++vs) {
        // Move to variation selectors supplements after variation selectors.
        if (vs == VS16 + 1) {
            vs = VS17;
        }
        for (uint32_t codePoint = 0; codePoint < MAX_UNICODE_CODE_POINT; ++codePoint) {
            uint32_t unusedGlyph;
            ASSERT_EQ(family->hasGlyph(codePoint, vs),
                      static_cast<bool>(hb_font_get_glyph(hbFont, codePoint, vs, &unusedGlyph)))
                    << "Inconsistent Result: " << fontPath << "#" << ttcIndex << ": U+" << std::hex
                    << codePoint << " U+" << std::hex << vs
                    << " Minikin: " << family->hasGlyph(codePoint, vs) << " HarfBuzz: "
                    << static_cast<bool>(hb_font_get_glyph(hbFont, codePoint, vs, &unusedGlyph));
        }
    }
    hb_font_destroy(hbFont);
}

INSTANTIATE_TEST_CASE_P(FontFamilyTest, FontFamilyHarfBuzzCompatibilityTest,
                        ::testing::Values(TestParam("/system/fonts/NotoSansCJK-Regular.ttc", 0),
                                          TestParam("/system/fonts/NotoColorEmoji.ttf", 0)));
}  // namespace minikin
