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

#include <gtest/gtest.h>

#include "minikin/Hyphenator.h"

#include "GreedyLineBreaker.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"

namespace minikin {
namespace {

const LocaleList& getLocaleList(const std::string& localeStr) {
    return LocaleListCache::getById(LocaleListCache::getId(localeStr));
}

TEST(LocaleListTest, basicTest) {
    std::string langTag = "en-US";
    EXPECT_EQ(1U, getLocaleList(langTag).size());
    EXPECT_EQ("en-Latn-US", getLocaleList(langTag)[0].getString());

    langTag = "en-US,fr-FR";
    EXPECT_EQ(2U, getLocaleList(langTag).size());
    EXPECT_EQ("en-Latn-US", getLocaleList(langTag)[0].getString());
    EXPECT_EQ("fr-Latn-FR", getLocaleList(langTag)[1].getString());

    langTag = "en-US,en-US,fr-FR";
    EXPECT_EQ(2U, getLocaleList(langTag).size());
    EXPECT_EQ("en-Latn-US", getLocaleList(langTag)[0].getString());
    EXPECT_EQ("fr-Latn-FR", getLocaleList(langTag)[1].getString());

    EXPECT_EQ(0U, getLocaleList("").size());
}

TEST(LocaleListTest, bogusLanguageTest) {
    std::string langTag = "en-US,THISISBOGUSLANGUAGE";
    EXPECT_EQ(1U, getLocaleList(langTag).size());
    EXPECT_EQ("en-Latn-US", getLocaleList(langTag)[0].getString());

    langTag = "THISISBOGUSLANGUAGE,en-US";
    EXPECT_EQ(1U, getLocaleList(langTag).size());
    EXPECT_EQ("en-Latn-US", getLocaleList(langTag)[0].getString());

    langTag = "THISISBOGUSLANGUAGE,THISISANOTHERBOGUSLANGUAGE";
    EXPECT_EQ(0U, getLocaleList(langTag).size());
}

}  // namespace
}  // namespace minikin
