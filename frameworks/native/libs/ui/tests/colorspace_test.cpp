/*
 * Copyright 2013 The Android Open Source Project
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

#define LOG_TAG "ColorSpaceTest"

#include <math.h>
#include <stdlib.h>

#include <ui/ColorSpace.h>

#include <gtest/gtest.h>

namespace android {

class ColorSpaceTest : public testing::Test {
protected:
};

TEST_F(ColorSpaceTest, XYZ) {
    mat3 sRGBToXYZ(transpose(mat3{
        0.412391f, 0.357584f, 0.180481f,
        0.212639f, 0.715169f, 0.072192f,
        0.019331f, 0.119195f, 0.950532f
    }));

    mat3 XYZtoSRGB(inverse(sRGBToXYZ));

    ColorSpace sRGB("sRGB", sRGBToXYZ);

    EXPECT_EQ(sRGBToXYZ, sRGB.getRGBtoXYZ());
    EXPECT_EQ(XYZtoSRGB, sRGB.getXYZtoRGB());
}

TEST_F(ColorSpaceTest, XYZPrimaries) {
    mat3 sRGBToXYZ(transpose(mat3{
        0.412391f, 0.357584f, 0.180481f,
        0.212639f, 0.715169f, 0.072192f,
        0.019331f, 0.119195f, 0.950532f
    }));

    ColorSpace sRGB("sRGB", sRGBToXYZ);

    EXPECT_NEAR(0.640f, sRGB.getPrimaries()[0].x, 1e-5f);
    EXPECT_NEAR(0.330f, sRGB.getPrimaries()[0].y, 1e-5f);

    EXPECT_NEAR(0.300f, sRGB.getPrimaries()[1].x, 1e-5f);
    EXPECT_NEAR(0.600f, sRGB.getPrimaries()[1].y, 1e-5f);

    EXPECT_NEAR(0.150f, sRGB.getPrimaries()[2].x, 1e-5f);
    EXPECT_NEAR(0.060f, sRGB.getPrimaries()[2].y, 1e-5f);
}

TEST_F(ColorSpaceTest, XYZWhitePoint) {
    mat3 sRGBToXYZ(transpose(mat3{
        0.412391f, 0.357584f, 0.180481f,
        0.212639f, 0.715169f, 0.072192f,
        0.019331f, 0.119195f, 0.950532f
    }));

    ColorSpace sRGB("sRGB", sRGBToXYZ);

    EXPECT_NEAR(0.3127f, sRGB.getWhitePoint().x, 1e-5f);
    EXPECT_NEAR(0.3290f, sRGB.getWhitePoint().y, 1e-5f);
}

TEST_F(ColorSpaceTest, XYZFromPrimaries) {
    mat3 sRGBToXYZ(transpose(mat3{
        0.412391f, 0.357584f, 0.180481f,
        0.212639f, 0.715169f, 0.072192f,
        0.019331f, 0.119195f, 0.950532f
    }));

    ColorSpace sRGB1("sRGB", sRGBToXYZ);
    ColorSpace sRGB2(
          "sRGB",
          {{float2{0.640f, 0.330f}, {0.300f, 0.600f}, {0.150f, 0.060f}}},
          {0.3127f, 0.3290f}
    );

    for (size_t i = 0; i < 3; i++) {
        for (size_t j= 0; j < 3; j++) {
            ASSERT_NEAR(sRGB1.getRGBtoXYZ()[i][j], sRGB2.getRGBtoXYZ()[i][j], 1e-5f);
        }
    }

    for (size_t i = 0; i < 3; i++) {
        for (size_t j= 0; j < 3; j++) {
            ASSERT_NEAR(sRGB2.getXYZtoRGB()[i][j], sRGB2.getXYZtoRGB()[i][j], 1e-5f);
        }
    }
}

TEST_F(ColorSpaceTest, TransferFunctions) {
    ColorSpace sRGB = ColorSpace::sRGB();

    EXPECT_NEAR(0.0f, sRGB.getEOTF()(0.0f), 1e-6f);
    EXPECT_NEAR(0.0f, sRGB.getOETF()(0.0f), 1e-6f);
    EXPECT_NEAR(1.0f, sRGB.getEOTF()(1.0f), 1e-6f);
    EXPECT_NEAR(1.0f, sRGB.getOETF()(1.0f), 1e-6f);

    for (float v = 0.0f; v <= 0.5f; v += 1e-3f) {
        ASSERT_TRUE(v >= sRGB.getEOTF()(v));
        ASSERT_TRUE(v <= sRGB.getOETF()(v));
    }

    float previousEOTF = std::numeric_limits<float>::lowest();
    float previousOETF = std::numeric_limits<float>::lowest();
    for (float v = 0.0f; v <= 1.0f; v += 1e-3f) {
        ASSERT_TRUE(previousEOTF < sRGB.getEOTF()(v));
        previousEOTF = sRGB.getEOTF()(v);
        ASSERT_TRUE(previousOETF < sRGB.getOETF()(v));
        previousOETF = sRGB.getOETF()(v);
    }

    ColorSpace sRGB2(
          "sRGB",
          {{float2{0.640f, 0.330f}, {0.300f, 0.600f}, {0.150f, 0.060f}}},
          {0.3127f, 0.3290f}
          // linear transfer functions
    );
    for (float v = 0.0f; v <= 1.0f; v += 1e-3f) {
        ASSERT_EQ(v, sRGB2.getEOTF()(v));
        ASSERT_EQ(v, sRGB2.getOETF()(v));
    }
}

TEST_F(ColorSpaceTest, Clamping) {
    // Pick a color outside of sRGB
    float3 c(ColorSpace::BT2020().rgbToXYZ(float3{0, 1, 0}));

    // The color will be clamped
    float3 sRGB(ColorSpace::sRGB().xyzToRGB(c));
    EXPECT_TRUE(sRGB > float3{0.0} && sRGB < float3{1.0});

    // The color will not be clamped
    float3 extendedSRGB(ColorSpace::linearExtendedSRGB().xyzToRGB(c));
    EXPECT_TRUE(extendedSRGB.g > 1.0f);
}

TEST_F(ColorSpaceTest, Connect) {
    // No chromatic adaptation
    auto r = ColorSpaceConnector(ColorSpace::sRGB(), ColorSpace::AdobeRGB())
            .transform({1.0f, 0.5f, 0.0f});
    EXPECT_TRUE(all(lessThan(abs(r - float3{0.8912f, 0.4962f, 0.1164f}), float3{1e-4f})));

    // Test with chromatic adaptation
    r = ColorSpaceConnector(ColorSpace::sRGB(), ColorSpace::ProPhotoRGB())
            .transform({1.0f, 0.0f, 0.0f});
    EXPECT_TRUE(all(lessThan(abs(r - float3{0.70226f, 0.2757f, 0.1036f}), float3{1e-4f})));
}

TEST_F(ColorSpaceTest, LUT) {
    auto lut = ColorSpace::createLUT(17, ColorSpace::sRGB(), ColorSpace::AdobeRGB());
    EXPECT_TRUE(lut != nullptr);

    // {1.0f, 0.5f, 0.0f}
    auto r = lut.get()[0 * 17 * 17 + 8 * 17 + 16];
    EXPECT_TRUE(all(lessThan(abs(r - float3{0.8912f, 0.4962f, 0.1164f}), float3{1e-4f})));

    // {1.0f, 1.0f, 0.5f}
    r = lut.get()[8 * 17 * 17 + 0 * 17 + 16]; // y (G) is flipped
    EXPECT_TRUE(all(lessThan(abs(r - float3{1.0f, 1.0f, 0.5290f}), float3{1e-4f})));

    // {1.0f, 1.0f, 1.0f}
    r = lut.get()[16 * 17 * 17 + 0 * 17 + 16]; // y (G) is flipped
    EXPECT_TRUE(all(lessThan(abs(r - float3{1.0f, 1.0f, 1.0f}), float3{1e-4f})));

}

}; // namespace android
