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

#include "FontTestUtils.h"
#include "FreeTypeMinikinFontForTest.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"

namespace minikin {

static const LocaleList& createLocaleList(const std::string& input) {
    uint32_t localeListId = LocaleListCache::getId(input);
    return LocaleListCache::getById(localeListId);
}

static Locale createLocale(const std::string& input) {
    uint32_t localeListId = LocaleListCache::getId(input);
    return LocaleListCache::getById(localeListId)[0];
}

static Locale createLocaleWithoutICUSanitization(const std::string& input) {
    return Locale(input);
}

TEST(LocaleTest, basicTests) {
    Locale defaultLocale;
    Locale emptyLocale("");
    Locale english = createLocale("en");
    Locale french = createLocale("fr");
    Locale und = createLocale("und");
    Locale undZsye = createLocale("und-Zsye");

    EXPECT_EQ(english, english);
    EXPECT_EQ(french, french);

    EXPECT_TRUE(defaultLocale != defaultLocale);
    EXPECT_TRUE(emptyLocale != emptyLocale);
    EXPECT_TRUE(defaultLocale != emptyLocale);
    EXPECT_TRUE(defaultLocale != und);
    EXPECT_TRUE(emptyLocale != und);
    EXPECT_TRUE(english != defaultLocale);
    EXPECT_TRUE(english != emptyLocale);
    EXPECT_TRUE(english != french);
    EXPECT_TRUE(english != undZsye);
    EXPECT_TRUE(und != undZsye);
    EXPECT_TRUE(english != und);
    EXPECT_TRUE(createLocale("de-1901") != createLocale("de-1996"));

    EXPECT_TRUE(defaultLocale.isUnsupported());
    EXPECT_TRUE(emptyLocale.isUnsupported());

    EXPECT_FALSE(english.isUnsupported());
    EXPECT_FALSE(french.isUnsupported());
    EXPECT_FALSE(und.isUnsupported());
    EXPECT_FALSE(undZsye.isUnsupported());
}

TEST(LocaleTest, getStringTest) {
    EXPECT_EQ("en-Latn-US", createLocale("en").getString());
    EXPECT_EQ("en-Latn-US", createLocale("en-Latn").getString());

    // Capitalized language code or lowercased script should be normalized.
    EXPECT_EQ("en-Latn-US", createLocale("EN-LATN").getString());
    EXPECT_EQ("en-Latn-US", createLocale("EN-latn").getString());
    EXPECT_EQ("en-Latn-US", createLocale("en-latn").getString());

    // Invalid script should be kept.
    EXPECT_EQ("en-Xyzt-US", createLocale("en-xyzt").getString());

    EXPECT_EQ("en-Latn-US", createLocale("en-Latn-US").getString());
    EXPECT_EQ("ja-Jpan-JP", createLocale("ja").getString());
    EXPECT_EQ("zh-Hant-TW", createLocale("zh-TW").getString());
    EXPECT_EQ("zh-Hant-HK", createLocale("zh-HK").getString());
    EXPECT_EQ("zh-Hant-MO", createLocale("zh-MO").getString());
    EXPECT_EQ("zh-Hans-CN", createLocale("zh").getString());
    EXPECT_EQ("zh-Hans-CN", createLocale("zh-CN").getString());
    EXPECT_EQ("zh-Hans-SG", createLocale("zh-SG").getString());
    EXPECT_EQ("und", createLocale("und").getString());
    EXPECT_EQ("und", createLocale("UND").getString());
    EXPECT_EQ("und", createLocale("Und").getString());
    EXPECT_EQ("und-Zsye", createLocale("und-Zsye").getString());
    EXPECT_EQ("und-Zsye", createLocale("Und-ZSYE").getString());
    EXPECT_EQ("und-Zsye", createLocale("Und-zsye").getString());

    EXPECT_EQ("es-Latn-419", createLocale("es-Latn-419").getString());

    // Variant
    EXPECT_EQ("de-Latn-DE", createLocale("de").getString());
    EXPECT_EQ("de-Latn-DE-1901", createLocale("de-1901").getString());
    EXPECT_EQ("de-Latn-DE-1996", createLocale("de-DE-1996").getString());

    // Line Break subtag
    EXPECT_EQ("ja-Jpan-JP-u-lb-loose", createLocale("ja-JP-u-lb-loose").getString());
    EXPECT_EQ("ja-Jpan-JP-u-lb-normal", createLocale("ja-JP-u-lb-normal").getString());
    EXPECT_EQ("ja-Jpan-JP-u-lb-strict", createLocale("ja-JP-u-lb-strict").getString());
    EXPECT_EQ("ja-Jpan-JP-u-lb-loose", createLocale("ja-JP-u-lb-loose-em-emoji").getString());
    EXPECT_EQ("ja-Jpan-JP-u-lb-strict", createLocale("ja-JP-u-em-default-lb-strict").getString());
    EXPECT_EQ("ja-Jpan-JP", createLocale("ja-JP-u-lb-bogus").getString());

    // Emoji subtag is dropped from getString().
    EXPECT_EQ("es-Latn-419", createLocale("es-419-u-em-emoji").getString());
    EXPECT_EQ("es-Latn-419", createLocale("es-Latn-419-u-em-emoji").getString());

    // This is not a necessary desired behavior, just known behavior.
    EXPECT_EQ("en-Latn-US", createLocale("und-Abcdefgh").getString());
}

TEST(LocaleTest, invalidLanguageTagTest) {  // just make sure no crash happens
    LocaleListCache::getId("ja-JP-u-lb-lb-strict");
}

TEST(LocaleTest, testReconstruction) {
    EXPECT_EQ("en", createLocaleWithoutICUSanitization("en").getString());
    EXPECT_EQ("fil", createLocaleWithoutICUSanitization("fil").getString());
    EXPECT_EQ("und", createLocaleWithoutICUSanitization("und").getString());

    EXPECT_EQ("en-Latn", createLocaleWithoutICUSanitization("en-Latn").getString());
    EXPECT_EQ("fil-Taga", createLocaleWithoutICUSanitization("fil-Taga").getString());
    EXPECT_EQ("und-Zsye", createLocaleWithoutICUSanitization("und-Zsye").getString());

    EXPECT_EQ("en-US", createLocaleWithoutICUSanitization("en-US").getString());
    EXPECT_EQ("fil-PH", createLocaleWithoutICUSanitization("fil-PH").getString());
    EXPECT_EQ("es-419", createLocaleWithoutICUSanitization("es-419").getString());

    EXPECT_EQ("en-Latn-US", createLocaleWithoutICUSanitization("en-Latn-US").getString());
    EXPECT_EQ("fil-Taga-PH", createLocaleWithoutICUSanitization("fil-Taga-PH").getString());
    EXPECT_EQ("es-Latn-419", createLocaleWithoutICUSanitization("es-Latn-419").getString());

    // Possible minimum/maximum values.
    EXPECT_EQ("aa", createLocaleWithoutICUSanitization("aa").getString());
    EXPECT_EQ("zz", createLocaleWithoutICUSanitization("zz").getString());
    EXPECT_EQ("aa-Aaaa", createLocaleWithoutICUSanitization("aa-Aaaa").getString());
    EXPECT_EQ("zz-Zzzz", createLocaleWithoutICUSanitization("zz-Zzzz").getString());
    EXPECT_EQ("aaa-Aaaa-AA", createLocaleWithoutICUSanitization("aaa-Aaaa-AA").getString());
    EXPECT_EQ("zzz-Zzzz-ZZ", createLocaleWithoutICUSanitization("zzz-Zzzz-ZZ").getString());
    EXPECT_EQ("aaa-Aaaa-000", createLocaleWithoutICUSanitization("aaa-Aaaa-000").getString());
    EXPECT_EQ("zzz-Zzzz-999", createLocaleWithoutICUSanitization("zzz-Zzzz-999").getString());
}

TEST(LocaleTest, ScriptEqualTest) {
    EXPECT_TRUE(createLocale("en").isEqualScript(createLocale("en")));
    EXPECT_TRUE(createLocale("en-Latn").isEqualScript(createLocale("en")));
    EXPECT_TRUE(createLocale("jp-Latn").isEqualScript(createLocale("en-Latn")));
    EXPECT_TRUE(createLocale("en-Jpan").isEqualScript(createLocale("en-Jpan")));

    EXPECT_FALSE(createLocale("en-Jpan").isEqualScript(createLocale("en-Hira")));
    EXPECT_FALSE(createLocale("en-Jpan").isEqualScript(createLocale("en-Hani")));
}

TEST(LocaleTest, ScriptMatchTest) {
    const bool SUPPORTED = true;
    const bool NOT_SUPPORTED = false;

    struct TestCase {
        const std::string baseScript;
        const std::string requestedScript;
        bool isSupported;
    } testCases[] = {
            // Same scripts
            {"en-Latn", "Latn", SUPPORTED},
            {"ja-Jpan", "Jpan", SUPPORTED},
            {"ja-Hira", "Hira", SUPPORTED},
            {"ja-Kana", "Kana", SUPPORTED},
            {"ja-Hrkt", "Hrkt", SUPPORTED},
            {"zh-Hans", "Hans", SUPPORTED},
            {"zh-Hant", "Hant", SUPPORTED},
            {"zh-Hani", "Hani", SUPPORTED},
            {"ko-Kore", "Kore", SUPPORTED},
            {"ko-Hang", "Hang", SUPPORTED},
            {"zh-Hanb", "Hanb", SUPPORTED},

            // Japanese supports Hiragana, Katakanara, etc.
            {"ja-Jpan", "Hira", SUPPORTED},
            {"ja-Jpan", "Kana", SUPPORTED},
            {"ja-Jpan", "Hrkt", SUPPORTED},
            {"ja-Hrkt", "Hira", SUPPORTED},
            {"ja-Hrkt", "Kana", SUPPORTED},

            // Chinese supports Han.
            {"zh-Hans", "Hani", SUPPORTED},
            {"zh-Hant", "Hani", SUPPORTED},
            {"zh-Hanb", "Hani", SUPPORTED},

            // Hanb supports Bopomofo.
            {"zh-Hanb", "Bopo", SUPPORTED},

            // Korean supports Hangul.
            {"ko-Kore", "Hang", SUPPORTED},

            // Different scripts
            {"ja-Jpan", "Latn", NOT_SUPPORTED},
            {"en-Latn", "Jpan", NOT_SUPPORTED},
            {"ja-Jpan", "Hant", NOT_SUPPORTED},
            {"zh-Hant", "Jpan", NOT_SUPPORTED},
            {"ja-Jpan", "Hans", NOT_SUPPORTED},
            {"zh-Hans", "Jpan", NOT_SUPPORTED},
            {"ja-Jpan", "Kore", NOT_SUPPORTED},
            {"ko-Kore", "Jpan", NOT_SUPPORTED},
            {"zh-Hans", "Hant", NOT_SUPPORTED},
            {"zh-Hant", "Hans", NOT_SUPPORTED},
            {"zh-Hans", "Kore", NOT_SUPPORTED},
            {"ko-Kore", "Hans", NOT_SUPPORTED},
            {"zh-Hant", "Kore", NOT_SUPPORTED},
            {"ko-Kore", "Hant", NOT_SUPPORTED},

            // Hiragana doesn't support Japanese, etc.
            {"ja-Hira", "Jpan", NOT_SUPPORTED},
            {"ja-Kana", "Jpan", NOT_SUPPORTED},
            {"ja-Hrkt", "Jpan", NOT_SUPPORTED},
            {"ja-Hani", "Jpan", NOT_SUPPORTED},
            {"ja-Hira", "Hrkt", NOT_SUPPORTED},
            {"ja-Kana", "Hrkt", NOT_SUPPORTED},
            {"ja-Hani", "Hrkt", NOT_SUPPORTED},
            {"ja-Hani", "Hira", NOT_SUPPORTED},
            {"ja-Hani", "Kana", NOT_SUPPORTED},

            // Kanji doesn't support Chinese, etc.
            {"zh-Hani", "Hant", NOT_SUPPORTED},
            {"zh-Hani", "Hans", NOT_SUPPORTED},
            {"zh-Hani", "Hanb", NOT_SUPPORTED},

            // Hangul doesn't support Korean, etc.
            {"ko-Hang", "Kore", NOT_SUPPORTED},
            {"ko-Hani", "Kore", NOT_SUPPORTED},
            {"ko-Hani", "Hang", NOT_SUPPORTED},
            {"ko-Hang", "Hani", NOT_SUPPORTED},

            // Han with botomofo doesn't support simplified Chinese, etc.
            {"zh-Hanb", "Hant", NOT_SUPPORTED},
            {"zh-Hanb", "Hans", NOT_SUPPORTED},
            {"zh-Hanb", "Jpan", NOT_SUPPORTED},
            {"zh-Hanb", "Kore", NOT_SUPPORTED},
    };

    for (const auto& testCase : testCases) {
        hb_script_t script = hb_script_from_iso15924_tag(
                HB_TAG(testCase.requestedScript[0], testCase.requestedScript[1],
                       testCase.requestedScript[2], testCase.requestedScript[3]));
        if (testCase.isSupported) {
            EXPECT_TRUE(createLocale(testCase.baseScript).supportsHbScript(script))
                    << testCase.baseScript << " should support " << testCase.requestedScript;
        } else {
            EXPECT_FALSE(createLocale(testCase.baseScript).supportsHbScript(script))
                    << testCase.baseScript << " shouldn't support " << testCase.requestedScript;
        }
    }
}

TEST(LocaleListTest, basicTests) {
    LocaleList emptyLocales;
    EXPECT_EQ(0u, emptyLocales.size());

    Locale english = createLocale("en");
    const LocaleList& singletonLocales = createLocaleList("en");
    EXPECT_EQ(1u, singletonLocales.size());
    EXPECT_EQ(english, singletonLocales[0]);

    Locale french = createLocale("fr");
    const LocaleList& twoLocales = createLocaleList("en,fr");
    EXPECT_EQ(2u, twoLocales.size());
    EXPECT_EQ(english, twoLocales[0]);
    EXPECT_EQ(french, twoLocales[1]);
}

TEST(LocaleListTest, unsupportedLocaleuageTests) {
    const LocaleList& oneUnsupported = createLocaleList("abcd-example");
    EXPECT_TRUE(oneUnsupported.empty());

    const LocaleList& twoUnsupporteds = createLocaleList("abcd-example,abcd-example");
    EXPECT_TRUE(twoUnsupporteds.empty());

    Locale english = createLocale("en");
    const LocaleList& firstUnsupported = createLocaleList("abcd-example,en");
    EXPECT_EQ(1u, firstUnsupported.size());
    EXPECT_EQ(english, firstUnsupported[0]);

    const LocaleList& lastUnsupported = createLocaleList("en,abcd-example");
    EXPECT_EQ(1u, lastUnsupported.size());
    EXPECT_EQ(english, lastUnsupported[0]);
}

TEST(LocaleListTest, repeatedLocaleuageTests) {
    Locale english = createLocale("en");
    Locale french = createLocale("fr");
    Locale canadianFrench = createLocale("fr-CA");
    Locale englishInLatn = createLocale("en-Latn");
    ASSERT_TRUE(english == englishInLatn);

    const LocaleList& locales = createLocaleList("en,en-Latn");
    EXPECT_EQ(1u, locales.size());
    EXPECT_EQ(english, locales[0]);

    const LocaleList& fr = createLocaleList("fr,fr-FR,fr-Latn-FR");
    EXPECT_EQ(1u, fr.size());
    EXPECT_EQ(french, fr[0]);

    // ICU appends FR to fr. The third language is dropped which is same as the first language.
    const LocaleList& fr2 = createLocaleList("fr,fr-CA,fr-FR");
    EXPECT_EQ(2u, fr2.size());
    EXPECT_EQ(french, fr2[0]);
    EXPECT_EQ(canadianFrench, fr2[1]);

    // The order should be kept.
    const LocaleList& locales2 = createLocaleList("en,fr,en-Latn");
    EXPECT_EQ(2u, locales2.size());
    EXPECT_EQ(english, locales2[0]);
    EXPECT_EQ(french, locales2[1]);
}

TEST(LocaleListTest, identifierTest) {
    EXPECT_EQ(createLocale("en-Latn-US"), createLocale("en-Latn-US"));
    EXPECT_EQ(createLocale("zh-Hans-CN"), createLocale("zh-Hans-CN"));
    EXPECT_EQ(createLocale("en-Zsye-US"), createLocale("en-Zsye-US"));

    EXPECT_NE(createLocale("en-Latn-US"), createLocale("en-Latn-GB"));
    EXPECT_NE(createLocale("en-Latn-US"), createLocale("en-Zsye-US"));
    EXPECT_NE(createLocale("es-Latn-US"), createLocale("en-Latn-US"));
    EXPECT_NE(createLocale("zh-Hant-HK"), createLocale("zh-Hant-TW"));
}

TEST(LocaleListTest, undEmojiTests) {
    Locale emoji = createLocale("und-Zsye");
    EXPECT_EQ(EmojiStyle::EMOJI, emoji.getEmojiStyle());

    Locale und = createLocale("und");
    EXPECT_EQ(EmojiStyle::EMPTY, und.getEmojiStyle());
    EXPECT_FALSE(emoji == und);

    Locale undExample = createLocale("und-example");
    EXPECT_EQ(EmojiStyle::EMPTY, undExample.getEmojiStyle());
    EXPECT_FALSE(emoji == undExample);
}

TEST(LocaleListTest, subtagEmojiTest) {
    std::string subtagEmojiStrings[] = {
            // Duplicate subtag case.
            "und-Latn-u-em-emoji-u-em-text",

            // Strings that contain language.
            "und-u-em-emoji", "en-u-em-emoji",

            // Strings that contain the script.
            "und-Jpan-u-em-emoji", "en-Latn-u-em-emoji", "und-Zsym-u-em-emoji",
            "und-Zsye-u-em-emoji", "en-Zsym-u-em-emoji", "en-Zsye-u-em-emoji",

            // Strings that contain the country.
            "und-US-u-em-emoji", "en-US-u-em-emoji", "es-419-u-em-emoji", "und-Latn-US-u-em-emoji",
            "en-Zsym-US-u-em-emoji", "en-Zsye-US-u-em-emoji", "es-Zsye-419-u-em-emoji",

            // Strings that contain the variant.
            "de-Latn-DE-1901-u-em-emoji",
    };

    for (const auto& subtagEmojiString : subtagEmojiStrings) {
        SCOPED_TRACE("Test for \"" + subtagEmojiString + "\"");
        Locale subtagEmoji = createLocale(subtagEmojiString);
        EXPECT_EQ(EmojiStyle::EMOJI, subtagEmoji.getEmojiStyle());
    }
}

TEST(LocaleListTest, subtagTextTest) {
    std::string subtagTextStrings[] = {
            // Duplicate subtag case.
            "und-Latn-u-em-text-u-em-emoji",

            // Strings that contain language.
            "und-u-em-text", "en-u-em-text",

            // Strings that contain the script.
            "und-Latn-u-em-text", "en-Jpan-u-em-text", "und-Zsym-u-em-text", "und-Zsye-u-em-text",
            "en-Zsym-u-em-text", "en-Zsye-u-em-text",

            // Strings that contain the country.
            "und-US-u-em-text", "en-US-u-em-text", "es-419-u-em-text", "und-Latn-US-u-em-text",
            "en-Zsym-US-u-em-text", "en-Zsye-US-u-em-text", "es-Zsye-419-u-em-text",

            // Strings that contain the variant.
            "de-Latn-DE-1901-u-em-text",
    };

    for (const auto& subtagTextString : subtagTextStrings) {
        SCOPED_TRACE("Test for \"" + subtagTextString + "\"");
        Locale subtagText = createLocale(subtagTextString);
        EXPECT_EQ(EmojiStyle::TEXT, subtagText.getEmojiStyle());
    }
}

// TODO: add more "und" language cases whose language and script are
//       unexpectedly translated to en-Latn by ICU.
TEST(LocaleListTest, subtagDefaultTest) {
    std::string subtagDefaultStrings[] = {
            // Duplicate subtag case.
            "en-Latn-u-em-default-u-em-emoji", "en-Latn-u-em-default-u-em-text",

            // Strings that contain language.
            "und-u-em-default", "en-u-em-default",

            // Strings that contain the script.
            "en-Latn-u-em-default", "en-Zsym-u-em-default", "en-Zsye-u-em-default",

            // Strings that contain the country.
            "en-US-u-em-default", "en-Latn-US-u-em-default", "es-Latn-419-u-em-default",
            "en-Zsym-US-u-em-default", "en-Zsye-US-u-em-default", "es-Zsye-419-u-em-default",

            // Strings that contain the variant.
            "de-Latn-DE-1901-u-em-default",
    };

    for (const auto& subtagDefaultString : subtagDefaultStrings) {
        SCOPED_TRACE("Test for \"" + subtagDefaultString + "\"");
        Locale subtagDefault = createLocale(subtagDefaultString);
        EXPECT_EQ(EmojiStyle::DEFAULT, subtagDefault.getEmojiStyle());
    }
}

TEST(LocaleListTest, subtagEmptyTest) {
    std::string subtagEmptyStrings[] = {
            "und",
            "jp",
            "en-US",
            "en-Latn",
            "en-Latn-US",
            "en-Latn-US-u-em",
            "en-Latn-US-u-em-defaultemoji",
            "de-Latn-DE-1901",
    };

    for (const auto& subtagEmptyString : subtagEmptyStrings) {
        SCOPED_TRACE("Test for \"" + subtagEmptyString + "\"");
        Locale subtagEmpty = createLocale(subtagEmptyString);
        EXPECT_EQ(EmojiStyle::EMPTY, subtagEmpty.getEmojiStyle());
    }
}

TEST(LocaleListTest, registerLocaleListTest) {
    EXPECT_EQ(0UL, registerLocaleList(""));
    EXPECT_NE(0UL, registerLocaleList("en"));
    EXPECT_NE(0UL, registerLocaleList("jp"));
    EXPECT_NE(0UL, registerLocaleList("en,zh-Hans"));

    EXPECT_EQ(registerLocaleList("en"), registerLocaleList("en"));
    EXPECT_NE(registerLocaleList("en"), registerLocaleList("jp"));
    EXPECT_NE(registerLocaleList("de"), registerLocaleList("de-1901"));

    EXPECT_EQ(registerLocaleList("en,zh-Hans"), registerLocaleList("en,zh-Hans"));
    EXPECT_NE(registerLocaleList("en,zh-Hans"), registerLocaleList("zh-Hans,en"));
    EXPECT_NE(registerLocaleList("en,zh-Hans"), registerLocaleList("jp"));
    EXPECT_NE(registerLocaleList("en,zh-Hans"), registerLocaleList("en"));
    EXPECT_NE(registerLocaleList("en,zh-Hans"), registerLocaleList("en,zh-Hant"));
    EXPECT_NE(registerLocaleList("de,de-1901"), registerLocaleList("de-1901,de"));
}

// The test font has following glyphs.
// U+82A6
// U+82A6 U+FE00 (VS1)
// U+82A6 U+E0100 (VS17)
// U+82A6 U+E0101 (VS18)
// U+82A6 U+E0102 (VS19)
// U+845B
// U+845B U+FE00 (VS2)
// U+845B U+E0101 (VS18)
// U+845B U+E0102 (VS19)
// U+845B U+E0103 (VS20)
// U+537F
// U+717D U+FE02 (VS3)
// U+717D U+E0102 (VS19)
// U+717D U+E0103 (VS20)
const char kVsTestFont[] = "VariationSelectorTest-Regular.ttf";

class FontFamilyTest : public testing::Test {
public:
    virtual void SetUp() override {
        if (access(getTestFontPath(kVsTestFont).c_str(), R_OK) != 0) {
            FAIL() << "Unable to read " << kVsTestFont << ". "
                   << "Please prepare the test data directory. "
                   << "For more details, please see how_to_run.txt.";
        }
    }
};

// Asserts that the font family has glyphs for and only for specified codepoint
// and variationSelector pairs.
void expectVSGlyphs(FontFamily* family, uint32_t codepoint, const std::set<uint32_t>& vs) {
    for (uint32_t i = 0xFE00; i <= 0xE01EF; ++i) {
        // Move to variation selectors supplements after variation selectors.
        if (i == 0xFF00) {
            i = 0xE0100;
        }
        if (vs.find(i) == vs.end()) {
            EXPECT_FALSE(family->hasGlyph(codepoint, i))
                    << "Glyph for U+" << std::hex << codepoint << " U+" << i;
        } else {
            EXPECT_TRUE(family->hasGlyph(codepoint, i))
                    << "Glyph for U+" << std::hex << codepoint << " U+" << i;
        }
    }
}

TEST_F(FontFamilyTest, hasVariationSelectorTest) {
    std::shared_ptr<FontFamily> family = buildFontFamily(kVsTestFont);

    const uint32_t kVS1 = 0xFE00;
    const uint32_t kVS2 = 0xFE01;
    const uint32_t kVS3 = 0xFE02;
    const uint32_t kVS17 = 0xE0100;
    const uint32_t kVS18 = 0xE0101;
    const uint32_t kVS19 = 0xE0102;
    const uint32_t kVS20 = 0xE0103;

    const uint32_t kSupportedChar1 = 0x82A6;
    EXPECT_TRUE(family->getCoverage().get(kSupportedChar1));
    expectVSGlyphs(family.get(), kSupportedChar1, std::set<uint32_t>({kVS1, kVS17, kVS18, kVS19}));

    const uint32_t kSupportedChar2 = 0x845B;
    EXPECT_TRUE(family->getCoverage().get(kSupportedChar2));
    expectVSGlyphs(family.get(), kSupportedChar2, std::set<uint32_t>({kVS2, kVS18, kVS19, kVS20}));

    const uint32_t kNoVsSupportedChar = 0x537F;
    EXPECT_TRUE(family->getCoverage().get(kNoVsSupportedChar));
    expectVSGlyphs(family.get(), kNoVsSupportedChar, std::set<uint32_t>());

    const uint32_t kVsOnlySupportedChar = 0x717D;
    EXPECT_FALSE(family->getCoverage().get(kVsOnlySupportedChar));
    expectVSGlyphs(family.get(), kVsOnlySupportedChar, std::set<uint32_t>({kVS3, kVS19, kVS20}));

    const uint32_t kNotSupportedChar = 0x845C;
    EXPECT_FALSE(family->getCoverage().get(kNotSupportedChar));
    expectVSGlyphs(family.get(), kNotSupportedChar, std::set<uint32_t>());
}

TEST_F(FontFamilyTest, hasVSTableTest) {
    struct TestCase {
        const std::string fontPath;
        bool hasVSTable;
    } testCases[] = {
            {"Ja.ttf", true},      {"ZhHant.ttf", true}, {"ZhHans.ttf", true},
            {"Italic.ttf", false}, {"Bold.ttf", false},  {"BoldItalic.ttf", false},
    };

    for (const auto& testCase : testCases) {
        SCOPED_TRACE(testCase.hasVSTable ? "Font " + testCase.fontPath +
                                                   " should have a variation sequence table."
                                         : "Font " + testCase.fontPath +
                                                   " shouldn't have a variation sequence table.");

        std::shared_ptr<FontFamily> family = buildFontFamily(testCase.fontPath);
        EXPECT_EQ(testCase.hasVSTable, family->hasVSTable());
    }
}

TEST_F(FontFamilyTest, createFamilyWithVariationTest) {
    // This font has 'wdth' and 'wght' axes.
    const char kMultiAxisFont[] = "MultiAxis.ttf";
    const char kNoAxisFont[] = "Regular.ttf";

    std::shared_ptr<FontFamily> multiAxisFamily = buildFontFamily(kMultiAxisFont);
    std::shared_ptr<FontFamily> noAxisFamily = buildFontFamily(kNoAxisFont);

    {
        // Do not ceate new instance if none of variations are specified.
        EXPECT_EQ(nullptr,
                  multiAxisFamily->createFamilyWithVariation(std::vector<FontVariation>()));
        EXPECT_EQ(nullptr, noAxisFamily->createFamilyWithVariation(std::vector<FontVariation>()));
    }
    {
        // New instance should be used for supported variation.
        std::vector<FontVariation> variations = {{MinikinFont::MakeTag('w', 'd', 't', 'h'), 1.0f}};
        std::shared_ptr<FontFamily> newFamily(
                multiAxisFamily->createFamilyWithVariation(variations));
        EXPECT_NE(nullptr, newFamily.get());
        EXPECT_NE(multiAxisFamily.get(), newFamily.get());
        EXPECT_EQ(nullptr, noAxisFamily->createFamilyWithVariation(variations));
    }
    {
        // New instance should be used for supported variation. (multiple variations case)
        std::vector<FontVariation> variations = {{MinikinFont::MakeTag('w', 'd', 't', 'h'), 1.0f},
                                                 {MinikinFont::MakeTag('w', 'g', 'h', 't'), 1.0f}};
        std::shared_ptr<FontFamily> newFamily(
                multiAxisFamily->createFamilyWithVariation(variations));
        EXPECT_NE(nullptr, newFamily.get());
        EXPECT_NE(multiAxisFamily.get(), newFamily.get());
        EXPECT_EQ(nullptr, noAxisFamily->createFamilyWithVariation(variations));
    }
    {
        // Do not ceate new instance if none of variations are supported.
        std::vector<FontVariation> variations = {{MinikinFont::MakeTag('Z', 'Z', 'Z', 'Z'), 1.0f}};
        EXPECT_EQ(nullptr, multiAxisFamily->createFamilyWithVariation(variations));
        EXPECT_EQ(nullptr, noAxisFamily->createFamilyWithVariation(variations));
    }
    {
        // At least one axis is supported, should create new instance.
        std::vector<FontVariation> variations = {{MinikinFont::MakeTag('w', 'd', 't', 'h'), 1.0f},
                                                 {MinikinFont::MakeTag('Z', 'Z', 'Z', 'Z'), 1.0f}};
        std::shared_ptr<FontFamily> newFamily(
                multiAxisFamily->createFamilyWithVariation(variations));
        EXPECT_NE(nullptr, newFamily.get());
        EXPECT_NE(multiAxisFamily.get(), newFamily.get());
        EXPECT_EQ(nullptr, noAxisFamily->createFamilyWithVariation(variations));
    }
}

TEST_F(FontFamilyTest, coverageTableSelectionTest) {
    // This font supports U+0061. The cmap subtable is format 4 and its platform ID is 0 and
    // encoding ID is 1.
    const char kUnicodeEncoding1Font[] = "UnicodeBMPOnly.ttf";

    // This font supports U+0061. The cmap subtable is format 4 and its platform ID is 0 and
    // encoding ID is 3.
    const char kUnicodeEncoding3Font[] = "UnicodeBMPOnly2.ttf";

    // This font has both cmap format 4 subtable which platform ID is 0 and encoding ID is 1
    // and cmap format 14 subtable which platform ID is 0 and encoding ID is 10.
    // U+0061 is listed in both subtable but U+1F926 is only listed in latter.
    const char kUnicodeEncoding4Font[] = "UnicodeUCS4.ttf";

    std::shared_ptr<FontFamily> unicodeEnc1Font = buildFontFamily(kUnicodeEncoding1Font);
    std::shared_ptr<FontFamily> unicodeEnc3Font = buildFontFamily(kUnicodeEncoding3Font);
    std::shared_ptr<FontFamily> unicodeEnc4Font = buildFontFamily(kUnicodeEncoding4Font);

    EXPECT_TRUE(unicodeEnc1Font->hasGlyph(0x0061, 0));
    EXPECT_TRUE(unicodeEnc3Font->hasGlyph(0x0061, 0));
    EXPECT_TRUE(unicodeEnc4Font->hasGlyph(0x0061, 0));

    EXPECT_TRUE(unicodeEnc4Font->hasGlyph(0x1F926, 0));
}

const char* slantToString(FontStyle::Slant slant) {
    if (slant == FontStyle::Slant::ITALIC) {
        return "ITALIC";
    } else {
        return "UPRIGHT";
    }
}

std::string fontStyleToString(const FontStyle& style) {
    char buf[64] = {};
    snprintf(buf, sizeof(buf), "FontStyle(weight=%d, slant=%s)", style.weight(),
             slantToString(style.slant()));
    return buf;
}

TEST_F(FontFamilyTest, closestMatch) {
    constexpr char kTestFont[] = "Ascii.ttf";

    constexpr FontStyle::Weight THIN = FontStyle::Weight::THIN;
    constexpr FontStyle::Weight LIGHT = FontStyle::Weight::LIGHT;
    constexpr FontStyle::Weight NORMAL = FontStyle::Weight::NORMAL;
    constexpr FontStyle::Weight MEDIUM = FontStyle::Weight::MEDIUM;
    constexpr FontStyle::Weight BOLD = FontStyle::Weight::BOLD;
    constexpr FontStyle::Weight BLACK = FontStyle::Weight::BLACK;

    constexpr FontStyle::Slant UPRIGHT = FontStyle::Slant::UPRIGHT;
    constexpr FontStyle::Slant ITALIC = FontStyle::Slant::ITALIC;

    const std::vector<FontStyle> STANDARD_SET = {
            FontStyle(NORMAL, UPRIGHT),  // 0
            FontStyle(BOLD, UPRIGHT),    // 1
            FontStyle(NORMAL, ITALIC),   // 2
            FontStyle(BOLD, ITALIC),     // 3
    };

    const std::vector<FontStyle> FULL_SET = {
            FontStyle(THIN, UPRIGHT),    // 0
            FontStyle(LIGHT, UPRIGHT),   // 1
            FontStyle(NORMAL, UPRIGHT),  // 2
            FontStyle(MEDIUM, UPRIGHT),  // 3
            FontStyle(BOLD, UPRIGHT),    // 4
            FontStyle(BLACK, UPRIGHT),   // 5
            FontStyle(THIN, ITALIC),     // 6
            FontStyle(LIGHT, ITALIC),    // 7
            FontStyle(NORMAL, ITALIC),   // 8
            FontStyle(MEDIUM, ITALIC),   // 9
            FontStyle(BOLD, ITALIC),     // 10
            FontStyle(BLACK, ITALIC),    // 11
    };
    struct TestCase {
        FontStyle wantedStyle;
        std::vector<FontStyle> familyStyles;
        size_t expectedIndex;
    } testCases[] = {
            {FontStyle(), {FontStyle()}, 0},

            // Exact matches
            {FontStyle(BOLD), {FontStyle(NORMAL), FontStyle(BOLD)}, 1},
            {FontStyle(BOLD), {FontStyle(LIGHT), FontStyle(BOLD)}, 1},
            {FontStyle(LIGHT), {FontStyle(NORMAL), FontStyle(LIGHT)}, 1},
            {FontStyle(LIGHT), {FontStyle(BOLD), FontStyle(LIGHT)}, 1},
            {FontStyle(NORMAL), {FontStyle(NORMAL), FontStyle(LIGHT)}, 0},
            {FontStyle(NORMAL), {FontStyle(NORMAL), FontStyle(BOLD)}, 0},
            {FontStyle(LIGHT), {FontStyle(LIGHT), FontStyle(NORMAL), FontStyle(BOLD)}, 0},
            {FontStyle(NORMAL), {FontStyle(LIGHT), FontStyle(NORMAL), FontStyle(BOLD)}, 1},
            {FontStyle(BOLD), {FontStyle(LIGHT), FontStyle(NORMAL), FontStyle(BOLD)}, 2},

            {FontStyle(UPRIGHT), {FontStyle(UPRIGHT), FontStyle(ITALIC)}, 0},
            {FontStyle(ITALIC), {FontStyle(UPRIGHT), FontStyle(ITALIC)}, 1},

            {FontStyle(NORMAL, UPRIGHT), STANDARD_SET, 0},
            {FontStyle(BOLD, UPRIGHT), STANDARD_SET, 1},
            {FontStyle(NORMAL, ITALIC), STANDARD_SET, 2},
            {FontStyle(BOLD, ITALIC), STANDARD_SET, 3},

            {FontStyle(NORMAL, UPRIGHT), FULL_SET, 2},
            {FontStyle(BOLD, UPRIGHT), FULL_SET, 4},
            {FontStyle(NORMAL, ITALIC), FULL_SET, 8},
            {FontStyle(BOLD, ITALIC), FULL_SET, 10},

            // TODO: Add fallback expectations. (b/68814338)
    };

    for (const TestCase& testCase : testCases) {
        std::vector<std::shared_ptr<MinikinFont>> dummyFonts;
        std::vector<Font> fonts;
        for (auto familyStyle : testCase.familyStyles) {
            std::shared_ptr<MinikinFont> dummyFont(
                    new FreeTypeMinikinFontForTest(getTestFontPath(kTestFont)));
            dummyFonts.push_back(dummyFont);
            fonts.push_back(Font::Builder(dummyFont).setStyle(familyStyle).build());
        }

        FontFamily family(std::move(fonts));
        FakedFont closest = family.getClosestMatch(testCase.wantedStyle);

        size_t idx = dummyFonts.size();
        for (size_t i = 0; i < dummyFonts.size(); i++) {
            if (dummyFonts[i].get() == closest.font->typeface().get()) {
                idx = i;
                break;
            }
        }
        ASSERT_NE(idx, dummyFonts.size()) << "The selected font is unknown.";
        EXPECT_EQ(testCase.expectedIndex, idx)
                << "Input Style: " << fontStyleToString(testCase.wantedStyle) << std::endl
                << "Actual Families' Style: " << fontStyleToString(testCase.familyStyles[idx])
                << std::endl
                << "Expected Families' Style: "
                << fontStyleToString(testCase.familyStyles[testCase.expectedIndex]) << std::endl;
    }
}

}  // namespace minikin
