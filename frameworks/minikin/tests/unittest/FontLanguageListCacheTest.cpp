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

#include "minikin/FontFamily.h"

#include <gtest/gtest.h>

#include "minikin/LocaleList.h"

#include "LocaleListCache.h"
#include "MinikinInternal.h"

namespace minikin {

TEST(LocaleListCacheTest, getId) {
    EXPECT_NE(0UL, registerLocaleList("en"));
    EXPECT_NE(0UL, registerLocaleList("jp"));
    EXPECT_NE(0UL, registerLocaleList("en,zh-Hans"));

    EXPECT_EQ(0UL, LocaleListCache::getId(""));

    EXPECT_EQ(LocaleListCache::getId("en"), LocaleListCache::getId("en"));
    EXPECT_NE(LocaleListCache::getId("en"), LocaleListCache::getId("jp"));

    EXPECT_EQ(LocaleListCache::getId("en,zh-Hans"), LocaleListCache::getId("en,zh-Hans"));
    EXPECT_NE(LocaleListCache::getId("en,zh-Hans"), LocaleListCache::getId("zh-Hans,en"));
    EXPECT_NE(LocaleListCache::getId("en,zh-Hans"), LocaleListCache::getId("jp"));
    EXPECT_NE(LocaleListCache::getId("en,zh-Hans"), LocaleListCache::getId("en"));
    EXPECT_NE(LocaleListCache::getId("en,zh-Hans"), LocaleListCache::getId("en,zh-Hant"));
}

TEST(LocaleListCacheTest, getById) {
    uint32_t enLangId = LocaleListCache::getId("en");
    uint32_t jpLangId = LocaleListCache::getId("jp");
    Locale english = LocaleListCache::getById(enLangId)[0];
    Locale japanese = LocaleListCache::getById(jpLangId)[0];

    const LocaleList& defLangs = LocaleListCache::getById(0);
    EXPECT_TRUE(defLangs.empty());

    const LocaleList& locales = LocaleListCache::getById(LocaleListCache::getId("en"));
    ASSERT_EQ(1UL, locales.size());
    EXPECT_EQ(english, locales[0]);

    const LocaleList& locales2 = LocaleListCache::getById(LocaleListCache::getId("en,jp"));
    ASSERT_EQ(2UL, locales2.size());
    EXPECT_EQ(english, locales2[0]);
    EXPECT_EQ(japanese, locales2[1]);
}

}  // namespace minikin
