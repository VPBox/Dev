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

#define LOG_TAG "HalfTest"

#include <math.h>
#include <stdlib.h>

#include <math/half.h>
#include <math/vec4.h>

#include <gtest/gtest.h>

namespace android {

class HalfTest : public testing::Test {
protected:
};

TEST_F(HalfTest, Basics) {

    EXPECT_EQ(2UL, sizeof(half));

    // test +/- zero
    EXPECT_EQ(0x0000, half( 0.0f).getBits());
    EXPECT_EQ(0x8000, half(-0.0f).getBits());

    // test nan
    EXPECT_EQ(0x7e00, half(NAN).getBits());

    // test +/- infinity
    EXPECT_EQ(0x7C00, half( std::numeric_limits<float>::infinity()).getBits());
    EXPECT_EQ(0xFC00, half(-std::numeric_limits<float>::infinity()).getBits());

    // test a few known values
    EXPECT_EQ(0x3C01, half(1.0009765625).getBits());
    EXPECT_EQ(0xC000, half(-2).getBits());
    EXPECT_EQ(0x0400, half(6.10352e-5).getBits());
    EXPECT_EQ(0x7BFF, half(65504).getBits());
    EXPECT_EQ(0x3555, half(1.0f/3).getBits());

    // numeric limits
    EXPECT_EQ(0x7C00, std::numeric_limits<half>::infinity().getBits());
    EXPECT_EQ(0x0400, std::numeric_limits<half>::min().getBits());
    EXPECT_EQ(0x7BFF, std::numeric_limits<half>::max().getBits());
    EXPECT_EQ(0xFBFF, std::numeric_limits<half>::lowest().getBits());

    // denormals (flushed to zero)
    EXPECT_EQ(0x0000, half( 6.09756e-5).getBits());      // if handled, should be: 0x03FF
    EXPECT_EQ(0x0000, half( 5.96046e-8).getBits());      // if handled, should be: 0x0001
    EXPECT_EQ(0x8000, half(-6.09756e-5).getBits());      // if handled, should be: 0x83FF
    EXPECT_EQ(0x8000, half(-5.96046e-8).getBits());      // if handled, should be: 0x8001

    // test all exactly representable integers
    for (int i=-2048 ; i<= 2048 ; ++i) {
        half h = i;
        EXPECT_EQ(i, float(h));
    }
}

TEST_F(HalfTest, Literals) {
    half one = 1.0_hf;
    half pi = 3.1415926_hf;
    half minusTwo = -2.0_hf;

    EXPECT_EQ(half(1.0f), one);
    EXPECT_EQ(half(3.1415926), pi);
    EXPECT_EQ(half(-2.0f), minusTwo);
}


TEST_F(HalfTest, Vec) {
    float4 f4(1,2,3,4);
    half4 h4(f4);
    half3 h3(f4.xyz);
    half2 h2(f4.xy);

    EXPECT_EQ(f4, h4);
    EXPECT_EQ(f4.xyz, h3);
    EXPECT_EQ(f4.xy, h2);
}

}; // namespace android
