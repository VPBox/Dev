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

#include "minikin/SystemFonts.h"

#include <gtest/gtest.h>

#include "minikin/FontCollection.h"

#include "FontTestUtils.h"

namespace minikin {
namespace {

class TestableSystemFonts : public SystemFonts {
public:
    TestableSystemFonts() : SystemFonts() {}
    virtual ~TestableSystemFonts() {}

    std::shared_ptr<FontCollection> findFontCollection(const std::string& familyName) const {
        return findFontCollectionInternal(familyName);
    }

    void registerFallback(const std::string& familyName,
                          const std::shared_ptr<FontCollection>& fc) {
        registerFallbackInternal(familyName, fc);
    }

    void registerDefault(const std::shared_ptr<FontCollection>& fc) { registerDefaultInternal(fc); }
};

TEST(SystemFontsTest, registerAndLookup) {
    TestableSystemFonts systemFonts;
    auto fc = buildFontCollection("Ascii.ttf");
    systemFonts.registerFallback("sans", fc);
    EXPECT_EQ(fc, systemFonts.findFontCollection("sans"));
}

TEST(SystemFontsTest, registerDefaultAndLookup) {
    TestableSystemFonts systemFonts;
    auto fc = buildFontCollection("Ascii.ttf");
    systemFonts.registerDefault(fc);
    EXPECT_EQ(fc, systemFonts.findFontCollection("unknown-name"));
}

TEST(SystemFontsTest, registerDefaultAndFallback) {
    TestableSystemFonts systemFonts;
    auto fc1 = buildFontCollection("Ascii.ttf");
    auto fc2 = buildFontCollection("Bold.ttf");
    systemFonts.registerDefault(fc1);
    systemFonts.registerFallback("sans", fc2);
    EXPECT_EQ(fc1, systemFonts.findFontCollection("unknown-name"));
    EXPECT_EQ(fc2, systemFonts.findFontCollection("sans"));
}

}  // namespace
}  // namespace minikin
