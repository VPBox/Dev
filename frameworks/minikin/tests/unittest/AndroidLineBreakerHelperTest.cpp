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

#include "minikin/AndroidLineBreakerHelper.h"

#include <gtest/gtest.h>

namespace minikin {
namespace android {

TEST(AndroidLineWidth, negativeWidthTest) {
    const int LINE_COUNT = 10;
    const std::vector<float> EMPTY;
    {
        AndroidLineWidth lineWidth(-10 /* first width */, 1 /* first count */, 0 /* rest width */,
                                   EMPTY, 0);

        EXPECT_LE(0.0f, lineWidth.getMin());
        for (int i = 0; i < LINE_COUNT; ++i) {
            EXPECT_LE(0.0f, lineWidth.getAt(i));
        }
    }
    {
        AndroidLineWidth lineWidth(0 /* first width */, 0 /* first count */, -10 /* rest width */,
                                   EMPTY, 0);

        EXPECT_LE(0.0f, lineWidth.getMin());
        for (int i = 0; i < LINE_COUNT; ++i) {
            EXPECT_LE(0.0f, lineWidth.getAt(i));
        }
    }
    {
        std::vector<float> indents = {10};
        AndroidLineWidth lineWidth(0 /* first width */, 0 /* first count */, 0 /* rest width */,
                                   indents, 0);

        EXPECT_LE(0.0f, lineWidth.getMin());
        for (int i = 0; i < LINE_COUNT; ++i) {
            EXPECT_LE(0.0f, lineWidth.getAt(i));
        }
    }
}

}  // namespace android
}  // namespace minikin
