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

#include <array>
#include <climits>
#include <math.h>

#include <audio_utils/sample.h>
#include <gtest/gtest.h>

static_assert(sizeof(sample_minifloat_t) == sizeof(uint16_t),
        "sizeof(sample_minifloat_t != sizeof(uint16_t");

static constexpr int signum(float f)
{
    return (f > 0) - (f < 0);
}

TEST(audio_utils_sample, Convert)
{
    std::vector<float> fvec;
    // verify minifloat <-> float is a bijection, and monotonic as float
    for (int i = 0; i <= 0xFFFF; i++) {
        // construct floats in order
        const int val = i < 0x8000 ? 0xFFFF - i : i ^ 0x8000;
        // TODO shouldn't depend on representation in order to skip negative zero
        if (val == 0x8000) {
            // This is an undefined value and so we won't test its behavior
            continue;
        }
        // TODO reinterpret_cast<sample_minifloat_t>(val) fails
        const sample_minifloat_t in = (sample_minifloat_t) val;
        const float f = float_from_sample(in);
        const sample_minifloat_t out = sample_from_float(f);
        ASSERT_EQ(in, out);
        fvec.push_back(f);
    }
    // no longer needed since we construct floats in order
    // #include <algorithm>
    // std::sort(fvec.begin(), fvec.end());
    float prev = -2.0f;
    for (auto curr : fvec) {
        // LT instead of LE because no negative zero
        ASSERT_LT(prev, curr);
        int signum_prev = signum(prev);
        int signum_curr = signum(curr);
        ASSERT_LE(signum_prev, signum_curr);
        if (signum_prev == signum_curr) {
            // confirm ratio between adjacent values (3:45 of "Will it float?" video)
            float ratio = curr / prev;
            float lower, upper;
            // normal
            if (fabsf(curr) >= 0.001f) {
                upper = 1.005f;
                lower = 0.995f;
            // denormal
            } else {
                upper = 2.0f;
                lower = 0.5f;
            }
            ASSERT_GE(ratio, lower) << "prev " << prev << " curr " << curr;
            ASSERT_LE(ratio, upper) << "prev " << prev << " curr " << curr;
        }
        prev = curr;
    }
    ASSERT_LT(prev, 2.0f);
}
