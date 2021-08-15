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

#undef LOG_TAG
#define LOG_TAG "RegionSamplingTest"

#include <ui/Transform.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <array>
#include <limits>

#include "RegionSamplingThread.h"

namespace android {

struct RegionSamplingTest : testing::Test {
public:
    static uint32_t constexpr kBlack = 0;
    static uint32_t constexpr kWhite = std::numeric_limits<uint32_t>::max();
    static int constexpr kWidth = 98;
    static int constexpr kStride = 100;
    static int constexpr kHeight = 29;
    static int constexpr kOrientation = ui::Transform::ROT_0;
    std::array<uint32_t, kHeight * kStride> buffer;
    Rect const whole_area{0, 0, kWidth, kHeight};
};

TEST_F(RegionSamplingTest, calculate_mean_white) {
    std::fill(buffer.begin(), buffer.end(), kWhite);
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, whole_area),
                testing::FloatEq(1.0f));
}

TEST_F(RegionSamplingTest, calculate_mean_black) {
    std::fill(buffer.begin(), buffer.end(), kBlack);
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, whole_area),
                testing::FloatEq(0.0f));
}

TEST_F(RegionSamplingTest, calculate_mean_partial_region) {
    auto const halfway_down = kHeight >> 1;
    auto const half = halfway_down * kStride;
    Rect const partial_region = {whole_area.left, whole_area.top, whole_area.right,
                                 whole_area.top + halfway_down};
    std::fill(buffer.begin(), buffer.begin() + half, 0);
    std::fill(buffer.begin() + half, buffer.end(), kWhite);
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, partial_region),
                testing::FloatEq(0.0f));
}

TEST_F(RegionSamplingTest, calculate_mean_mixed_values) {
    std::generate(buffer.begin(), buffer.end(), [n = 0]() mutable {
        uint32_t const pixel = (n % std::numeric_limits<uint8_t>::max()) << ((n % 3) * CHAR_BIT);
        n++;
        return pixel;
    });
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, whole_area),
                testing::FloatNear(0.083f, 0.01f));
}

TEST_F(RegionSamplingTest, bimodal_tiebreaker) {
    std::generate(buffer.begin(), buffer.end(),
                  [n = 0]() mutable { return (n++ % 2) ? kBlack : kWhite; });
    // presently there's no tiebreaking strategy in place, accept either of the means
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, whole_area),
                testing::AnyOf(testing::FloatEq(1.0), testing::FloatEq(0.0f)));
}

TEST_F(RegionSamplingTest, bounds_checking) {
    std::generate(buffer.begin(), buffer.end(),
                  [n = 0]() mutable { return (n++ > (kStride * kHeight >> 1)) ? kBlack : kWhite; });

    Rect invalid_region{0, 0, 4, kHeight + 1};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, invalid_region),
                testing::Eq(0.0));

    invalid_region = Rect{0, 0, -4, kHeight};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, invalid_region),
                testing::Eq(0.0));

    invalid_region = Rect{3, 0, 2, 0};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, invalid_region),
                testing::Eq(0.0));

    invalid_region = Rect{0, 3, 0, 2};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, kOrientation, invalid_region),
                testing::Eq(0.0));
}

// workaround for b/133849373
TEST_F(RegionSamplingTest, orientation_90) {
    std::generate(buffer.begin(), buffer.end(),
                  [n = 0]() mutable { return (n++ > (kStride * kHeight >> 1)) ? kBlack : kWhite; });

    Rect tl_region{0, 0, 4, 4};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_0,
                           tl_region),
                testing::Eq(1.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_180,
                           tl_region),
                testing::Eq(1.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_90,
                           tl_region),
                testing::Eq(0.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_270,
                           tl_region),
                testing::Eq(0.0));

    Rect br_region{kWidth - 4, kHeight - 4, kWidth, kHeight};
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_0,
                           br_region),
                testing::Eq(0.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_180,
                           br_region),
                testing::Eq(0.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_90,
                           br_region),
                testing::Eq(1.0));
    EXPECT_THAT(sampleArea(buffer.data(), kWidth, kHeight, kStride, ui::Transform::ROT_270,
                           br_region),
                testing::Eq(1.0));
}

} // namespace android
