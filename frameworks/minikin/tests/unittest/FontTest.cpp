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

#include "minikin/Font.h"

#include <gtest/gtest.h>

#include "FontTestUtils.h"
#include "FreeTypeMinikinFontForTest.h"

namespace minikin {

TEST(FontTest, CopyTest) {
    auto minikinFont = std::make_shared<FreeTypeMinikinFontForTest>(getTestFontPath("Ascii.ttf"));
    {
        Font font = Font::Builder(minikinFont).build();
        {
            Font copied(font);
            EXPECT_EQ(font.typeface(), copied.typeface());
            EXPECT_EQ(font.style(), copied.style());
            EXPECT_EQ(font.baseFont(), copied.baseFont());
        }
        {
            Font copied = font;
            EXPECT_EQ(font.typeface(), copied.typeface());
            EXPECT_EQ(font.style(), copied.style());
            EXPECT_EQ(font.baseFont(), copied.baseFont());
        }
    }
}

}  // namespace minikin
