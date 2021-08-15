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

#include "HyphenatorMap.h"

#include <gtest/gtest.h>

#include "LocaleListCache.h"
#include "MinikinInternal.h"

namespace minikin {
namespace {

// Constants used for testing. The address does not need a valid one.
const Hyphenator* FAKE_ADDRESS = reinterpret_cast<const Hyphenator*>(1);
const Hyphenator* AS_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* BG_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* BN_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* CU_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* CY_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* DA_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* DE_1901_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* DE_1996_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* DE_CH_1901_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* EN_GB_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* EN_US_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* ES_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* ET_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* EU_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* FR_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* GA_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* GU_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* HI_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* HR_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* HU_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* HY_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* KN_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* ML_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* MN_CYRL_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* MR_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* NB_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* NN_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* OR_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* PA_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* PT_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* SL_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* TA_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* TE_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* TK_HYPHENATOR = FAKE_ADDRESS++;
const Hyphenator* UND_ETHI_HYPHENATOR = FAKE_ADDRESS++;

class TestableHyphenatorMap : public HyphenatorMap {
public:
    TestableHyphenatorMap() : HyphenatorMap() {}

    using HyphenatorMap::addAliasInternal;
    using HyphenatorMap::addInternal;
    using HyphenatorMap::lookupInternal;
};

class HyphenatorMapTest : public testing::Test {
protected:
    virtual void SetUp() override {
        // Following settings are copied from Hyphenator.java.
        mMap.addInternal("as", AS_HYPHENATOR);
        mMap.addInternal("bg", BG_HYPHENATOR);
        mMap.addInternal("bn", BN_HYPHENATOR);
        mMap.addInternal("cu", CU_HYPHENATOR);
        mMap.addInternal("cy", CY_HYPHENATOR);
        mMap.addInternal("da", DA_HYPHENATOR);
        mMap.addInternal("de-1901", DE_1901_HYPHENATOR);
        mMap.addInternal("de-1996", DE_1996_HYPHENATOR);
        mMap.addInternal("de-CH-1901", DE_CH_1901_HYPHENATOR);
        mMap.addInternal("en-GB", EN_GB_HYPHENATOR);
        mMap.addInternal("en-US", EN_US_HYPHENATOR);
        mMap.addInternal("es", ES_HYPHENATOR);
        mMap.addInternal("et", ET_HYPHENATOR);
        mMap.addInternal("eu", EU_HYPHENATOR);
        mMap.addInternal("fr", FR_HYPHENATOR);
        mMap.addInternal("ga", GA_HYPHENATOR);
        mMap.addInternal("gu", GU_HYPHENATOR);
        mMap.addInternal("hi", HI_HYPHENATOR);
        mMap.addInternal("hr", HR_HYPHENATOR);
        mMap.addInternal("hu", HU_HYPHENATOR);
        mMap.addInternal("hy", HY_HYPHENATOR);
        mMap.addInternal("kn", KN_HYPHENATOR);
        mMap.addInternal("ml", ML_HYPHENATOR);
        mMap.addInternal("mn-Cyrl", MN_CYRL_HYPHENATOR);
        mMap.addInternal("mr", MR_HYPHENATOR);
        mMap.addInternal("nb", NB_HYPHENATOR);
        mMap.addInternal("nn", NN_HYPHENATOR);
        mMap.addInternal("or", OR_HYPHENATOR);
        mMap.addInternal("pa", PA_HYPHENATOR);
        mMap.addInternal("pt", PT_HYPHENATOR);
        mMap.addInternal("sl", SL_HYPHENATOR);
        mMap.addInternal("ta", TA_HYPHENATOR);
        mMap.addInternal("te", TE_HYPHENATOR);
        mMap.addInternal("tk", TK_HYPHENATOR);
        mMap.addInternal("und-Ethi", UND_ETHI_HYPHENATOR);

        mMap.addAliasInternal("en", "en-GB");
        mMap.addAliasInternal("en-AS", "en-US");
        mMap.addAliasInternal("en-GU", "en-US");
        mMap.addAliasInternal("en-MH", "en-US");
        mMap.addAliasInternal("en-MP", "en-US");
        mMap.addAliasInternal("en-PR", "en-US");
        mMap.addAliasInternal("en-UM", "en-US");
        mMap.addAliasInternal("en-VI", "en-US");
        mMap.addAliasInternal("de-LI-1901", "de-CH-1901");
        mMap.addAliasInternal("de", "de-1996");
        mMap.addAliasInternal("no", "nb");
        mMap.addAliasInternal("mn", "mn-Cyrl");
        // am for und-Ethi is removed for testing purposes.
        mMap.addAliasInternal("byn", "und-Ethi");
        mMap.addAliasInternal("gez", "und-Ethi");
        mMap.addAliasInternal("ti", "und-Ethi");
        mMap.addAliasInternal("wal", "und-Ethi");
    }

    const Locale& getLocale(const std::string& localeStr) {
        // In production, we reconstruct the LocaleList from the locale list ID.
        // So, do it here too.
        const uint32_t id = LocaleListCache::getId(localeStr);
        const LocaleList& locales = LocaleListCache::getById(id);
        MINIKIN_ASSERT(locales.size() == 1, "The input must be a single locale");
        return locales[0];
    }

    const Hyphenator* lookup(const std::string& localeStr) {
        return mMap.lookupInternal(getLocale(localeStr));
    }

private:
    TestableHyphenatorMap mMap;
};

TEST_F(HyphenatorMapTest, exactMatch) {
    EXPECT_EQ(AS_HYPHENATOR, lookup("as"));
    EXPECT_EQ(BG_HYPHENATOR, lookup("bg"));
    EXPECT_EQ(BN_HYPHENATOR, lookup("bn"));
    EXPECT_EQ(CU_HYPHENATOR, lookup("cu"));
    EXPECT_EQ(CY_HYPHENATOR, lookup("cy"));
    EXPECT_EQ(DA_HYPHENATOR, lookup("da"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-1901"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-1996"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-CH-1901"));
    EXPECT_EQ(EN_GB_HYPHENATOR, lookup("en-GB"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-US"));
    EXPECT_EQ(ES_HYPHENATOR, lookup("es"));
    EXPECT_EQ(ET_HYPHENATOR, lookup("et"));
    EXPECT_EQ(EU_HYPHENATOR, lookup("eu"));
    EXPECT_EQ(FR_HYPHENATOR, lookup("fr"));
    EXPECT_EQ(GA_HYPHENATOR, lookup("ga"));
    EXPECT_EQ(GU_HYPHENATOR, lookup("gu"));
    EXPECT_EQ(HI_HYPHENATOR, lookup("hi"));
    EXPECT_EQ(HR_HYPHENATOR, lookup("hr"));
    EXPECT_EQ(HU_HYPHENATOR, lookup("hu"));
    EXPECT_EQ(HY_HYPHENATOR, lookup("hy"));
    EXPECT_EQ(KN_HYPHENATOR, lookup("kn"));
    EXPECT_EQ(ML_HYPHENATOR, lookup("ml"));
    EXPECT_EQ(MN_CYRL_HYPHENATOR, lookup("mn-Cyrl"));
    EXPECT_EQ(MR_HYPHENATOR, lookup("mr"));
    EXPECT_EQ(NB_HYPHENATOR, lookup("nb"));
    EXPECT_EQ(NN_HYPHENATOR, lookup("nn"));
    EXPECT_EQ(OR_HYPHENATOR, lookup("or"));
    EXPECT_EQ(PA_HYPHENATOR, lookup("pa"));
    EXPECT_EQ(PT_HYPHENATOR, lookup("pt"));
    EXPECT_EQ(SL_HYPHENATOR, lookup("sl"));
    EXPECT_EQ(TA_HYPHENATOR, lookup("ta"));
    EXPECT_EQ(TE_HYPHENATOR, lookup("te"));
    EXPECT_EQ(TK_HYPHENATOR, lookup("tk"));
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("und-Ethi"));
}

TEST_F(HyphenatorMapTest, aliasMatch) {
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-AS"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-GU"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-MH"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-MP"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-PR"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-UM"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-VI"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-LI-1901"));
    EXPECT_EQ(NB_HYPHENATOR, lookup("no"));
    EXPECT_EQ(MN_CYRL_HYPHENATOR, lookup("mn"));
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("byn"));
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("gez"));
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("ti"));
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("wal"));
    // Amharic is tested in fallbackTest_scriptFallback
}

TEST_F(HyphenatorMapTest, IgnoreScript) {
    // Script should be ignored until the final script-only matching rule.
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Latn-US"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Zsye-US"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Zsym-US"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Jpan-US"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Hans-US"));
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en-Ethi-US"));

    EXPECT_EQ(EN_GB_HYPHENATOR, lookup("en-Zsye-AU"));
    EXPECT_EQ(EN_GB_HYPHENATOR, lookup("en-Zsye-GB"));
}

TEST_F(HyphenatorMapTest, languageFallback) {
    EXPECT_EQ(EN_GB_HYPHENATOR, lookup("en-AU"));
    EXPECT_EQ(EN_GB_HYPHENATOR, lookup("en-NZ"));

    // "en" is expanded to en-Latn-US. So this is equivalent to "en-Latn-US" test case.
    // This expansion also happens in production.
    EXPECT_EQ(EN_US_HYPHENATOR, lookup("en"));
}

TEST_F(HyphenatorMapTest, GermanFallback) {
    // German in general
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-1901"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-1996"));

    // German in Germany
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-DE"));

    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-DE-1901"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-Latn-DE-1901"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-Latn-DE-1901-u-em-emoji"));

    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-DE-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-DE-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-DE-1996-u-em-emoji"));

    // German in Austria
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-AT"));

    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-AT-1901"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-Latn-AT-1901"));
    EXPECT_EQ(DE_1901_HYPHENATOR, lookup("de-Latn-AT-1901-u-em-emoji"));

    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-AT-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-AT-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-AT-1996-u-em-emoji"));

    // German in Switzerland
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-CH"));

    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-CH-1901"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-Latn-CH-1901"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-Latn-CH-1901-u-em-emoji"));

    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-CH-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-CH-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-CH-1996-u-em-emoji"));

    // German in Liechtenstein
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-LI"));

    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-LI-1901"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-Latn-LI-1901"));
    EXPECT_EQ(DE_CH_1901_HYPHENATOR, lookup("de-Latn-LI-1901-u-em-emoji"));

    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-LI-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-LI-1996"));
    EXPECT_EQ(DE_1996_HYPHENATOR, lookup("de-Latn-LI-1996-u-em-emoji"));
}

TEST_F(HyphenatorMapTest, fallbackTest_LanguageFallback) {
    EXPECT_EQ(ES_HYPHENATOR, lookup("es-ES"));
    EXPECT_EQ(ES_HYPHENATOR, lookup("es-AR"));
    EXPECT_EQ(ES_HYPHENATOR, lookup("es-BO"));
    EXPECT_EQ(ES_HYPHENATOR, lookup("es-CL"));

    // Spanish in Great Britain
    EXPECT_EQ(ES_HYPHENATOR, lookup("es-GB"));
}

TEST_F(HyphenatorMapTest, fallbackTest_ScriptFallback) {
    EXPECT_EQ(UND_ETHI_HYPHENATOR, lookup("am"));
}

TEST_F(HyphenatorMapTest, neverReturnNullptrTest) {
    EXPECT_NE(nullptr, lookup("und"));
    EXPECT_NE(nullptr, lookup("ja"));
    EXPECT_NE(nullptr, lookup("ja-JP"));
}

TEST_F(HyphenatorMapTest, CyrlScriptFallback) {
    // mn-Cryl should not match with ru-Cyrl and und-Cyrl
    EXPECT_NE(MN_CYRL_HYPHENATOR, lookup("ru-Cyrl"));
    EXPECT_NE(MN_CYRL_HYPHENATOR, lookup("und-Cyrl"));
}

}  // namespace
}  // namespace minikin
