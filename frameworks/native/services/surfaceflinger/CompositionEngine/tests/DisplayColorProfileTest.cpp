/*
 * Copyright 2019 The Android Open Source Project
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

#include <compositionengine/DisplayColorProfileCreationArgs.h>
#include <compositionengine/impl/DisplayColorProfile.h>
#include <compositionengine/mock/CompositionEngine.h>
#include <gtest/gtest.h>

namespace android::hardware::graphics::common::V1_1 {

// Note: These operator overloads need to be defined in the same namespace as
// the values they print.

std::ostream& operator<<(std::ostream& os, const RenderIntent& value) {
    return os << toString(value) << " (" << static_cast<std::underlying_type_t<Dataspace>>(value)
              << ")";
}

} // namespace android::hardware::graphics::common::V1_1

namespace android::hardware::graphics::common::V1_2 {

// Note: These operator overloads need to be defined in the same namespace as
// the values they print.

std::ostream& operator<<(std::ostream& os, const Dataspace& value) {
    return os << toString(value) << " (" << static_cast<std::underlying_type_t<Dataspace>>(value)
              << ")";
}

std::ostream& operator<<(std::ostream& os, const ColorMode& value) {
    return os << toString(value) << " (" << static_cast<std::underlying_type_t<Dataspace>>(value)
              << ")";
}

} // namespace android::hardware::graphics::common::V1_2

namespace android::compositionengine {
namespace {

using testing::_;
using testing::Contains;
using testing::IsEmpty;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::SizeIs;
using testing::StrictMock;

using ui::ColorMode;
using ui::Dataspace;
using ui::Hdr;
using ui::RenderIntent;

// This allows us to simulate a vendor-specified intent being used.
constexpr RenderIntent VendorRenderIntent = static_cast<RenderIntent>(0x100);

class DisplayColorProfileTest : public testing::Test {
public:
    ~DisplayColorProfileTest() override = default;

    StrictMock<mock::CompositionEngine> mCompositionEngine;
};

class ProfileFactory {
public:
    impl::DisplayColorProfile build() const {
        return impl::DisplayColorProfile{DisplayColorProfileCreationArgs{
                mHasWideColorGamut,
                HdrCapabilities(mSupportedHdrTypes, mMaxLuminance, mMaxAverageLuminance,
                                mMinLuminance),
                mSupportedPerFrameMetadata,
                mSupportedColorModes,
        }};
    }

    ProfileFactory& setHasWideColorGamut(bool value) {
        mHasWideColorGamut = value;
        return *this;
    }

    ProfileFactory& setPerFrameMetadata(int32_t value) {
        mSupportedPerFrameMetadata = value;
        return *this;
    }

    ProfileFactory& addHdrType(Hdr value) {
        mSupportedHdrTypes.emplace_back(value);
        return *this;
    }

    ProfileFactory& addHdrTypes(std::initializer_list<Hdr> values) {
        for (auto value : values) {
            mSupportedHdrTypes.emplace_back(value);
        }
        return *this;
    }

    ProfileFactory& setMaxLuminance(float value) {
        mMaxLuminance = value;
        return *this;
    }

    ProfileFactory& setMaxAverageLuminance(float value) {
        mMaxAverageLuminance = value;
        return *this;
    }

    ProfileFactory& setMinLuminance(float value) {
        mMinLuminance = value;
        return *this;
    }

    ProfileFactory& addColorModeRenderIntent(ColorMode colorMode, RenderIntent renderIntent) {
        mSupportedColorModes[colorMode].emplace_back(renderIntent);
        return *this;
    }

    ProfileFactory& addColorModeRenderIntents(ColorMode colorMode,
                                              std::initializer_list<RenderIntent> renderIntents) {
        auto& profileedRenderIntents = mSupportedColorModes[colorMode];
        for (auto renderIntent : renderIntents) {
            profileedRenderIntents.emplace_back(renderIntent);
        }
        return *this;
    }

    static impl::DisplayColorProfile createProfileWithNoColorModeSupport() {
        return ProfileFactory().build();
    }

    static impl::DisplayColorProfile createProfileWithBT2020ColorModeSupport() {
        return ProfileFactory()
                .setHasWideColorGamut(true)
                .addHdrType(Hdr::HDR10)
                .addColorModeRenderIntent(ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC)
                .addColorModeRenderIntent(ColorMode::DISPLAY_BT2020, RenderIntent::ENHANCE)
                .addColorModeRenderIntent(ColorMode::DISPLAY_BT2020, VendorRenderIntent)
                .build();
    }

    static impl::DisplayColorProfile createProfileWithSRGBColorModeSupport() {
        return ProfileFactory()
                .setHasWideColorGamut(true)
                .addHdrType(Hdr::HDR10)
                .addColorModeRenderIntent(ColorMode::SRGB, RenderIntent::COLORIMETRIC)
                .addColorModeRenderIntent(ColorMode::SRGB, RenderIntent::ENHANCE)
                .addColorModeRenderIntent(ColorMode::SRGB, VendorRenderIntent)
                .build();
    }

    static impl::DisplayColorProfile createProfileWithBT2100PQSupport() {
        return ProfileFactory()
                .setHasWideColorGamut(true)
                .addHdrType(Hdr::HLG)
                .addColorModeRenderIntent(ColorMode::BT2100_PQ, VendorRenderIntent)
                .build();
    }

    static impl::DisplayColorProfile createProfileWithDisplayP3ColorModeSupport() {
        return ProfileFactory()
                .setHasWideColorGamut(true)
                .addHdrType(Hdr::HLG)
                .addColorModeRenderIntent(ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC)
                .addColorModeRenderIntent(ColorMode::DISPLAY_P3, RenderIntent::ENHANCE)
                .addColorModeRenderIntent(ColorMode::DISPLAY_P3, VendorRenderIntent)
                .build();
    }

private:
    bool mHasWideColorGamut = false;
    std::vector<Hdr> mSupportedHdrTypes;
    float mMaxLuminance = -1.f;
    float mMaxAverageLuminance = -1.f;
    float mMinLuminance = -1.f;
    int32_t mSupportedPerFrameMetadata = 0;
    std::unordered_map<ColorMode, std::vector<RenderIntent>> mSupportedColorModes;
};

/* ------------------------------------------------------------------------
 * RenderSurface Construction
 */

TEST_F(DisplayColorProfileTest, ctorSetsHasWideColorGamutFromInputArgs) {
    {
        auto profile = ProfileFactory().setHasWideColorGamut(false).build();

        EXPECT_FALSE(profile.hasWideColorGamut());
    }

    {
        auto profile = ProfileFactory().setHasWideColorGamut(true).build();

        EXPECT_TRUE(profile.hasWideColorGamut());
    }
}

TEST_F(DisplayColorProfileTest, ctorSetsSupportedPerFrameMetadataFromInputArgs) {
    {
        auto profile = ProfileFactory().setPerFrameMetadata(0).build();

        EXPECT_EQ(0, profile.getSupportedPerFrameMetadata());
    }

    {
        impl::DisplayColorProfile profile = ProfileFactory().setPerFrameMetadata(123).build();

        EXPECT_EQ(123, profile.getSupportedPerFrameMetadata());
    }
}

TEST_F(DisplayColorProfileTest, ctorDetectsSupportedHdrTypesFromInputArgs) {
    {
        // The constructor will set the internal state to not indicate any
        // profile for HDR modes if none are profileed.
        auto profile = ProfileFactory().build();

        EXPECT_FALSE(profile.hasHDR10PlusSupport());
        EXPECT_FALSE(profile.hasHDR10Support());
        EXPECT_FALSE(profile.hasHLGSupport());
        EXPECT_FALSE(profile.hasDolbyVisionSupport());
    }

    {
        // The constructor will set the intenral state to indicate HDR10Plus
        // profile if the input arguments indicate it is profileed.
        auto profile = ProfileFactory().addHdrType(Hdr::HDR10_PLUS).build();

        EXPECT_TRUE(profile.hasHDR10PlusSupport());
        EXPECT_FALSE(profile.hasHDR10Support());
        EXPECT_FALSE(profile.hasHLGSupport());
        EXPECT_FALSE(profile.hasDolbyVisionSupport());
    }

    {
        // The constructor will set the intenral state to indicate HDR10 profile
        // if the input arguments indicate it is profileed.
        auto profile = ProfileFactory().addHdrType(Hdr::HDR10).build();

        EXPECT_FALSE(profile.hasHDR10PlusSupport());
        EXPECT_TRUE(profile.hasHDR10Support());
        EXPECT_FALSE(profile.hasHLGSupport());
        EXPECT_FALSE(profile.hasDolbyVisionSupport());
    }

    {
        // The constructor will set the intenral state to indicate HLG profile
        // if the input arguments indicate it is profileed.
        auto profile = ProfileFactory().addHdrType(Hdr::HLG).build();

        EXPECT_FALSE(profile.hasHDR10PlusSupport());
        EXPECT_FALSE(profile.hasHDR10Support());
        EXPECT_TRUE(profile.hasHLGSupport());
        EXPECT_FALSE(profile.hasDolbyVisionSupport());
    }

    {
        // The constructor will set the intenral state to indicate Dolbyvision profile
        // if the input arguments indicate it is profileed.
        auto profile = ProfileFactory().addHdrType(Hdr::DOLBY_VISION).build();

        EXPECT_FALSE(profile.hasHDR10Support());
        EXPECT_FALSE(profile.hasHLGSupport());
        EXPECT_TRUE(profile.hasDolbyVisionSupport());
    }
}

TEST_F(DisplayColorProfileTest, ctorUsesOrDefaultsLuminanceValuesFromInputArgs) {
    {
        // The constructor will use a default value for each luminance setting
        // that is negative.
        auto profile = ProfileFactory()
                               .setMaxLuminance(-1.f)
                               .setMaxAverageLuminance(-1.f)
                               .setMinLuminance(-1.f)
                               .build();

        EXPECT_EQ(DisplayColorProfile::sDefaultMaxLumiance,
                  profile.getHdrCapabilities().getDesiredMaxLuminance());
        EXPECT_EQ(DisplayColorProfile::sDefaultMaxLumiance,
                  profile.getHdrCapabilities().getDesiredMaxAverageLuminance());
        EXPECT_EQ(DisplayColorProfile::sDefaultMinLumiance,
                  profile.getHdrCapabilities().getDesiredMinLuminance());
    }

    {
        // The constructor will otherwise take and use a positive value for each
        // of the luminance settings.
        auto profile = ProfileFactory()
                               .setMaxLuminance(1001.f)
                               .setMaxAverageLuminance(1002.f)
                               .setMinLuminance(1003.f)
                               .build();

        EXPECT_EQ(1001.f, profile.getHdrCapabilities().getDesiredMaxLuminance());
        EXPECT_EQ(1002.f, profile.getHdrCapabilities().getDesiredMaxAverageLuminance());
        EXPECT_EQ(1003.f, profile.getHdrCapabilities().getDesiredMinLuminance());
    }
}

TEST_F(DisplayColorProfileTest, ctorSignalsHdrSupportForAnyWideColorGamutDevice) {
    {
        // If the output does not profile wide color gamut, then no HDR modes
        // will be profileed in the generated HDR capabilities.
        auto profile = ProfileFactory().setHasWideColorGamut(false).build();

        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), IsEmpty());
    }

    {
        // If the HWC does not show profile for certain HDR modes, then the
        // generated HDR capabilities will indicate profile anyway.
        auto profile = ProfileFactory().setHasWideColorGamut(true).build();

        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), SizeIs(2));
        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), Contains(Hdr::HDR10));
        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), Contains(Hdr::HLG));
    }

    {
        // If the HWC profiles the HDR modes, then the generated capabilities
        // still has one entry for each HDR type.
        auto profile = ProfileFactory()
                               .setHasWideColorGamut(true)
                               .addHdrTypes({Hdr::HLG, Hdr::HDR10})
                               .build();

        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), SizeIs(2));
        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), Contains(Hdr::HDR10));
        EXPECT_THAT(profile.getHdrCapabilities().getSupportedHdrTypes(), Contains(Hdr::HLG));
    }
}

/* ------------------------------------------------------------------------
 * DisplayColorProfile::hasRenderIntent
 */

TEST_F(DisplayColorProfileTest, hasRenderIntentReturnsExpectedValueWhenOutputHasNoSupport) {
    auto profile = ProfileFactory::createProfileWithNoColorModeSupport();

    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(VendorRenderIntent));
}

TEST_F(DisplayColorProfileTest, hasRenderIntentReturnsExpectedValueWhenOutputHasBT2020upport) {
    auto profile = ProfileFactory::createProfileWithBT2020ColorModeSupport();

    EXPECT_TRUE(profile.hasRenderIntent(RenderIntent::COLORIMETRIC));
    EXPECT_TRUE(profile.hasRenderIntent(RenderIntent::ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(VendorRenderIntent));
}

TEST_F(DisplayColorProfileTest, hasRenderIntentReturnsExpectedValueWhenOutputHasSRGBSupport) {
    auto profile = ProfileFactory::createProfileWithSRGBColorModeSupport();

    EXPECT_TRUE(profile.hasRenderIntent(RenderIntent::COLORIMETRIC));
    EXPECT_TRUE(profile.hasRenderIntent(RenderIntent::ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_ENHANCE));
    EXPECT_TRUE(profile.hasRenderIntent(VendorRenderIntent));
}

TEST_F(DisplayColorProfileTest, hasRenderIntentReturnsExpectedValueWhenOutputHasBTG2100PQSupport) {
    auto profile = ProfileFactory::createProfileWithBT2100PQSupport();

    EXPECT_TRUE(profile.hasRenderIntent(RenderIntent::COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_COLORIMETRIC));
    EXPECT_FALSE(profile.hasRenderIntent(RenderIntent::TONE_MAP_ENHANCE));
    EXPECT_FALSE(profile.hasRenderIntent(VendorRenderIntent));
}

/* ------------------------------------------------------------------------
 * DisplayColorProfile::hasLegacyHdrSupport
 */

TEST_F(DisplayColorProfileTest, hasLegacyHdrSupport) {
    {
        auto profile = ProfileFactory::createProfileWithNoColorModeSupport();

        EXPECT_FALSE(profile.hasLegacyHdrSupport(Dataspace::BT2020_PQ));
        EXPECT_FALSE(profile.hasLegacyHdrSupport(Dataspace::BT2020_HLG));
    }

    {
        auto profile = ProfileFactory::createProfileWithBT2020ColorModeSupport();

        EXPECT_TRUE(profile.hasLegacyHdrSupport(Dataspace::BT2020_PQ));
        EXPECT_FALSE(profile.hasLegacyHdrSupport(Dataspace::BT2020_HLG));
    }

    {
        auto profile = ProfileFactory::createProfileWithSRGBColorModeSupport();

        EXPECT_TRUE(profile.hasLegacyHdrSupport(Dataspace::BT2020_PQ));
        EXPECT_FALSE(profile.hasLegacyHdrSupport(Dataspace::BT2020_HLG));
    }

    {
        auto profile = ProfileFactory::createProfileWithBT2100PQSupport();

        EXPECT_FALSE(profile.hasLegacyHdrSupport(Dataspace::BT2020_PQ));
        EXPECT_TRUE(profile.hasLegacyHdrSupport(Dataspace::BT2020_HLG));
    }
}

/* ------------------------------------------------------------------------
 * RenderSurface::getBestColorMode()
 */

void checkGetBestColorMode(
        DisplayColorProfile& profile,
        const std::array<std::tuple<Dataspace, ColorMode, RenderIntent>, 15>& expected) {
    using ArgsType = std::tuple<Dataspace, RenderIntent>;

    // These are the combinations of dataspaces and render intents that could be
    // passed to RenderSurface::getBestColorMode()
    const std::array<std::tuple<Dataspace, RenderIntent>, 15> kArgs = {
            /* clang-format off */

            // Non-HDR combinations

            /*  0 */ ArgsType{Dataspace::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /*  1 */ ArgsType{Dataspace::DISPLAY_BT2020, RenderIntent::ENHANCE},
            /*  2 */ ArgsType{Dataspace::DISPLAY_BT2020, VendorRenderIntent},                  // Vendor explicit setting

            /*  3 */ ArgsType{Dataspace::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /*  4 */ ArgsType{Dataspace::DISPLAY_P3, RenderIntent::ENHANCE},
            /*  5 */ ArgsType{Dataspace::DISPLAY_P3, VendorRenderIntent},                  // Vendor explicit setting

            /*  6 */ ArgsType{Dataspace::V0_SRGB, RenderIntent::COLORIMETRIC},
            /*  7 */ ArgsType{Dataspace::V0_SRGB, RenderIntent::ENHANCE},
            /*  8 */ ArgsType{Dataspace::V0_SRGB, VendorRenderIntent},                  // Vendor explicit setting

            // HDR combinations

            /*  9 */ ArgsType{Dataspace::BT2020_PQ, RenderIntent::TONE_MAP_COLORIMETRIC},
            /* 10 */ ArgsType{Dataspace::BT2020_PQ, RenderIntent::TONE_MAP_ENHANCE},
            /* 11 */ ArgsType{Dataspace::BT2020_PQ, VendorRenderIntent},               // Vendor explicit setting

            /* 12 */ ArgsType{Dataspace::BT2020_HLG, RenderIntent::TONE_MAP_COLORIMETRIC},
            /* 13 */ ArgsType{Dataspace::BT2020_HLG, RenderIntent::TONE_MAP_ENHANCE},
            /* 14 */ ArgsType{Dataspace::BT2020_HLG, VendorRenderIntent},               // Vendor explicit setting
            /* clang-format on */
    };

    for (size_t i = 0; i < kArgs.size(); i++) {
        std::tuple<Dataspace, ColorMode, RenderIntent> actual;
        profile.getBestColorMode(std::get<0>(kArgs[i]), std::get<1>(kArgs[i]), &std::get<0>(actual),
                                 &std::get<1>(actual), &std::get<2>(actual));

        EXPECT_EQ(expected[i], actual) << " for index " << i;
    }
}

TEST_F(DisplayColorProfileTest, getBestColorModeReturnsExpectedModesWhenOutputHasNoSupport) {
    auto profile = ProfileFactory::createProfileWithNoColorModeSupport();

    // Note: This table of expected values goes with the table of arguments
    // used in checkGetBestColorMode.
    using Result = std::tuple<Dataspace, ColorMode, RenderIntent>;
    std::array<Result, 15> expectedResults = {
            /* clang-format off */
            /*  0 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  1 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  2 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  3 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  4 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  5 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  6 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  7 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  8 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  9 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* 10 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* 11 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /* 12 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* 13 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* 14 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /* clang-format on */
    };

    checkGetBestColorMode(profile, expectedResults);
}

TEST_F(DisplayColorProfileTest, getBestColorModeReturnsExpectedModesWhenOutputHasBT2020Support) {
    auto profile = ProfileFactory::createProfileWithBT2020ColorModeSupport();

    // Note: This table of expected values goes with the table of arguments
    // used in checkGetBestColorMode.
    using Result = std::tuple<Dataspace, ColorMode, RenderIntent>;
    std::array<Result, 15> expectedResults = {
            /* clang-format off */
            /*  0 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /*  1 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::ENHANCE},
            /*  2 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  3 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /*  4 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::ENHANCE},
            /*  5 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  6 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /*  7 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::ENHANCE},
            /*  8 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  9 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /* 10 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /* 11 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /* 12 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /* 13 */ Result{Dataspace::DISPLAY_BT2020, ColorMode::DISPLAY_BT2020, RenderIntent::COLORIMETRIC},
            /* 14 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* clang-format on */
    };

    checkGetBestColorMode(profile, expectedResults);
}

TEST_F(DisplayColorProfileTest, getBestColorModeReturnsExpectedModesWhenOutputHasSRGBSupport) {
    auto profile = ProfileFactory::createProfileWithSRGBColorModeSupport();

    // Note: This table of expected values goes with the table of arguments
    // used in checkGetBestColorMode.
    using Result = std::tuple<Dataspace, ColorMode, RenderIntent>;
    std::array<Result, 15> expectedResults = {
            /* clang-format off */
            /*  0 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /*  1 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::ENHANCE},
            /*  2 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, VendorRenderIntent},

            /*  3 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /*  4 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::ENHANCE},
            /*  5 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, VendorRenderIntent},

            /*  6 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /*  7 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::ENHANCE},
            /*  8 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, VendorRenderIntent},

            /*  9 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /* 10 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /* 11 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /* 12 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /* 13 */ Result{Dataspace::V0_SRGB, ColorMode::SRGB, RenderIntent::COLORIMETRIC},
            /* 14 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* clang-format on */
    };

    checkGetBestColorMode(profile, expectedResults);
}

TEST_F(DisplayColorProfileTest, getBestColorModeReturnsExpectedModesWhenOutputHasDisplayP3Support) {
    auto profile = ProfileFactory::createProfileWithDisplayP3ColorModeSupport();

    // Note: This table of expected values goes with the table of arguments
    // used in checkGetBestColorMode.
    using Result = std::tuple<Dataspace, ColorMode, RenderIntent>;
    std::array<Result, 15> expectedResults = {
            /* clang-format off */
            /*  0 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /*  1 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::ENHANCE},
            // TODO(b/124317977): There is bug here.
            /*  2 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  3 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /*  4 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::ENHANCE},
            /*  5 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  6 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /*  7 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::ENHANCE},
            /*  8 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  9 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /* 10 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /* 11 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /* 12 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /* 13 */ Result{Dataspace::DISPLAY_P3, ColorMode::DISPLAY_P3, RenderIntent::COLORIMETRIC},
            /* 14 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /* clang-format on */
    };

    checkGetBestColorMode(profile, expectedResults);
}

TEST_F(DisplayColorProfileTest, getBestColorModeReturnsExpectedModesWhenOutputHasBT2100PQSupport) {
    auto profile = ProfileFactory::createProfileWithBT2100PQSupport();

    // Note: This table of expected values goes with the table of arguments
    // used in checkGetBestColorMode.
    using Result = std::tuple<Dataspace, ColorMode, RenderIntent>;
    std::array<Result, 15> expectedResults = {
            /* clang-format off */
            /*  0 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  1 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  2 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  3 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  4 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  5 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  6 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  7 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},
            /*  8 */ Result{Dataspace::UNKNOWN, ColorMode::NATIVE, RenderIntent::COLORIMETRIC},

            /*  9 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, RenderIntent::COLORIMETRIC},
            /* 10 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, RenderIntent::COLORIMETRIC},
            /* 11 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, VendorRenderIntent},

            /* 12 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, RenderIntent::COLORIMETRIC},
            /* 13 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, RenderIntent::COLORIMETRIC},
            /* 14 */ Result{Dataspace::BT2020_PQ, ColorMode::BT2100_PQ, VendorRenderIntent},
            /* clang-format on */
    };

    checkGetBestColorMode(profile, expectedResults);
}

} // namespace
} // namespace android::compositionengine
