/*
 * Copyright 2018 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_string_tests"

#include <audio_utils/string.h>
#include <gtest/gtest.h>

// fills the string buffer with a increasing ramp of values from start.
template <size_t size>
void fill(char (&s)[size], int start) {
    for (size_t i = 0; i < size - 1; ++i) {
        s[i] = start++;
    }
    s[size - 1] = 0;
}

// checks that the fill counts from start, as expected to actual chars,
// whereupon the rest is expected to be zeroes.
template <size_t size>
void check(char (&s)[size], int start, size_t actual) {
    size_t lim = std::min(actual, size);
    size_t i = 0;

    if (lim > 0) {
        for (; i < lim - 1; ++i) {
            EXPECT_EQ(start, s[i]);
            ++start;
        }
    }
    for (; i < size; ++i) {
        EXPECT_EQ(0, s[i]);
    }
}

TEST(audio_utils_string, check_zero_fill) {
    // we use string arrays whose size is known by compiler, not vectors
    constexpr size_t STRING_SIZE = 50;
    union {
        char dst[STRING_SIZE];
        char dst_mirror[STRING_SIZE + 10]; // verifier that we don't overwrite
    };
    char over[sizeof(dst) + 5];
    char under[sizeof(dst) - 5];

    // fill with a value ramp
    constexpr int DST_START = 1;
    constexpr int OVER_START = 2;
    constexpr int UNDER_START = 3;
    fill(dst_mirror, DST_START);
    fill(over, OVER_START);
    fill(under, UNDER_START);

    // union should overlay dst and dst_mirror.
    dst[sizeof(dst) - 1] = 0;
    check(dst, DST_START, sizeof(dst));
    EXPECT_EQ(sizeof(dst) + DST_START, dst_mirror[sizeof(dst)]);

    // make sure we truncate when copying a larger string.
    audio_utils_strlcpy_zerofill(dst, over);
    check(dst, OVER_START, sizeof(dst));

    // check we didn't overwrite
    EXPECT_EQ(sizeof(dst) + DST_START, dst_mirror[sizeof(dst)]);

    // make sure we fill remaining buffer with zeros.
    audio_utils_strlcpy_zerofill(dst, under);
    check(dst, UNDER_START, sizeof(under));

    // check we didn't overwrite
    EXPECT_EQ(sizeof(dst) + DST_START, dst_mirror[sizeof(dst)]);
}
