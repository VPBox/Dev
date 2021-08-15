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

#define LOG_TAG "SizeTest"

#include <cmath>
#include <cstdlib>

#include <ui/Size.h>

#include <gtest/gtest.h>

namespace android {
namespace ui {

TEST(SizeTest, BasicConstructionAndEqualityComparison) {
    Size s(123, 456);

    EXPECT_EQ(123, s.width);
    EXPECT_EQ(123, s.getWidth());

    EXPECT_EQ(456, s.height);
    EXPECT_EQ(456, s.getHeight());

    EXPECT_EQ(Size(123, 456), s);
    EXPECT_NE(Size(456, 123), s);
}

TEST(SizeTest, BasicLessThanComparison) {
    EXPECT_TRUE(Size(0, 1) < Size(2, 3));
    EXPECT_FALSE(Size(2, 3) < Size(0, 1));

    EXPECT_TRUE(Size(0, 3) < Size(2, 1));
    EXPECT_FALSE(Size(2, 1) < Size(0, 3));

    EXPECT_TRUE(Size(0, 1) < Size(0, 3));
    EXPECT_FALSE(Size(0, 3) < Size(0, 1));

    EXPECT_FALSE(Size(1, 1) < Size(1, 1));
}

TEST(SizeTest, ValidAndEmpty) {
    {
        Size s;
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(-1, -1);
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(1, -1000);
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(-1000, 1);
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(-1000, -1000);
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        const auto& s = Size::INVALID;
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(123, 456);
        s.makeInvalid();
        EXPECT_FALSE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }

    {
        Size s(0, 0);
        EXPECT_TRUE(s.isValid());
        EXPECT_TRUE(s.isEmpty());
    }

    {
        const auto& s = Size::EMPTY;
        EXPECT_TRUE(s.isValid());
        EXPECT_TRUE(s.isEmpty());
    }

    {
        Size s(123, 456);
        s.clear();
        EXPECT_TRUE(s.isValid());
        EXPECT_TRUE(s.isEmpty());
    }

    {
        Size s(123, 456);
        EXPECT_TRUE(s.isValid());
        EXPECT_FALSE(s.isEmpty());
    }
}

TEST(SizeTest, Set) {
    {
        Size s;
        s.setWidth(0);
        EXPECT_EQ(Size(0, -1), s);
    }

    {
        Size s;
        s.setHeight(0);
        EXPECT_EQ(Size(-1, 0), s);
    }

    {
        Size s;
        s.set(123, 456);
        EXPECT_EQ(Size(123, 456), s);
    }
}

template <typename T, typename U>
void ClampTest(T input, U expected) {
    // The constructor, set(), setWidth() and setHeight() all allow arbitrary
    // conversions from other numeric types, and implement clamping if necessary.

    EXPECT_EQ(Size(expected, expected), Size(input, input));

    {
        Size s;
        s.set(input, input);
        EXPECT_EQ(Size(expected, expected), s);
    }

    {
        Size s;
        s.setWidth(input);
        EXPECT_EQ(expected, s.width);
    }

    {
        Size s;
        s.setHeight(input);
        EXPECT_EQ(expected, s.height);
    }
}

TEST(SizeTest, Int8RangeIsNotClamped) {
    ClampTest(std::numeric_limits<int8_t>::max(), std::numeric_limits<int8_t>::max());
    ClampTest(int8_t(0), int8_t(0));
    ClampTest(std::numeric_limits<int8_t>::lowest(), std::numeric_limits<int8_t>::lowest());
}

TEST(SizeTest, FloatRangeIsClamped) {
    ClampTest(std::numeric_limits<float>::max(), std::numeric_limits<int32_t>::max());
    ClampTest(float(0), int32_t(0));
    ClampTest(std::numeric_limits<float>::lowest(), std::numeric_limits<int32_t>::lowest());
}

} // namespace ui
} // namespace android
