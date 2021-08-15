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

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_channels_tests"

#include <math.h>
#include <vector>

#include <gtest/gtest.h>
#include <log/log.h>

#include <audio_utils/channels.h>

// TODO: Make a common include file for helper functions.

template<typename T>
void checkMonotone(const T *ary, size_t size)
{
    for (size_t i = 1; i < size; ++i) {
        EXPECT_LT(ary[i-1], ary[i]);
    }
}

template<typename T>
void checkUnsignedMonotoneOrZero(const T *ary, size_t size)
{
    if (size == 0) return;

    T least = ary[0];
    for (size_t i = 1; i < size; ++i) {
        if (ary[i]) {
            EXPECT_LT(least, ary[i]);
            least = ary[i];
        }
    }
}

template<typename T>
void expectEq(const T &c1, const T &c2) {
    EXPECT_EQ(c1.size(), c2.size());
    EXPECT_EQ(0, memcmp(c1.data(), c2.data(), sizeof(c1[0]) * std::min(c1.size(), c2.size())));
}

TEST(audio_utils_channels, adjust_channels) {
    constexpr size_t size = 65536;
    std::vector<uint16_t> u16ref(size);
    std::vector<uint16_t> u16expand(size * 2);
    std::vector<uint16_t> u16ary(size);

    // reference buffer is monotonic.
    for (size_t i = 0; i < u16ref.size(); ++i) {
        u16ref[i] = i;
    }

    // expand channels from stereo to quad.
    adjust_channels(
            u16ref.data() /*in_buff*/,
            2 /*in_channels*/,
            u16expand.data() /*out_buff*/,
            4 /*out_channels*/,
            sizeof(u16ref[0]) /*sample_size_in_bytes*/,
            sizeof(u16ref[0]) * u16ref.size() /*num_in_bytes*/);

    // expanded buffer must increase (or be zero).
    checkUnsignedMonotoneOrZero(u16expand.data(), u16expand.size());

    // contract channels back to stereo.
    adjust_channels(
            u16expand.data() /*in_buff*/,
            4 /*in_channels*/,
            u16ary.data() /*out_buff*/,
            2 /*out_channels*/,
            sizeof(u16expand[0]) /*sample_size_in_bytes*/,
            sizeof(u16expand[0]) * u16expand.size() /*num_in_bytes*/);

    // contracted array must be identical to original.
    expectEq(u16ary, u16ref);
}

TEST(audio_utils_channels, adjust_selected_channels) {
    constexpr size_t size = 65536;
    std::vector<uint16_t> u16ref(size);
    std::vector<uint16_t> u16contract(size / 2);
    std::vector<uint16_t> u16ary(size);

    // reference buffer is monotonic.
    for (size_t i = 0; i < u16ref.size(); ++i) {
        u16ref[i] = i;
    }

    // contract from quad to stereo.
    adjust_selected_channels(
            u16ref.data() /*in_buff*/,
            4 /*in_channels*/,
            u16contract.data() /*out_buff*/,
            2 /*out_channels*/,
            sizeof(u16ref[0]) /*sample_size_in_bytes*/,
            sizeof(u16ref[0]) * u16ref.size() /*num_in_bytes*/);

    // contracted buffer must increase.
    checkMonotone(u16contract.data(), u16contract.size());

    // initialize channels 3 and 4 of final comparison array.
    for (size_t i = 0; i < u16ary.size() / 4; ++i) {
        u16ary[i * 4 + 2] = u16ref[i * 4 + 2];
        u16ary[i * 4 + 3] = u16ref[i * 4 + 3];
    }

    // expand stereo into channels 1 and 2 of quad comparison array.
    adjust_selected_channels(
            u16contract.data() /*in_buff*/,
            2 /*in_channels*/,
            u16ary.data() /*out_buff*/,
            4 /*out_channels*/,
            sizeof(u16contract[0]) /*sample_size_in_bytes*/,
            sizeof(u16contract[0]) * u16contract.size() /*num_in_bytes*/);

    // comparison array must be identical to original.
    expectEq(u16ary, u16ref);
}

TEST(audio_utils_channels, adjust_channels_non_destructive) {
    constexpr size_t size = 65536; /* arbitrary large multiple of 8 */
    std::vector<uint16_t> u16ref(size);
    std::vector<uint16_t> u16contracted(size);
    std::vector<uint16_t> u16expanded(size);
    std::vector<uint16_t> u16inout(size);

    // Reference buffer increases monotonically.
    // For second test, in/out buffer begins identical to ref.
    for (size_t i = 0; i < u16ref.size(); ++i) {
        u16ref[i] = i;
        u16inout[i] = i;
    }

    // *** First test: different in/out buffers ***

    // Contract from quad to stereo.
    adjust_channels_non_destructive(
            u16ref.data() /*in_buff*/,
            4 /*in_channels*/,
            u16contracted.data() /*out_buff*/,
            2 /*out_channels*/,
            sizeof(u16ref[0]) /*sample_size_in_bytes*/,
            sizeof(u16ref[0]) * u16ref.size() /*num_in_bytes*/);

    // Each half of contracted buffer should increase monotonically.
    checkMonotone(u16contracted.data(), u16contracted.size() / 2);
    checkMonotone(&u16contracted[u16contracted.size() / 2], u16contracted.size() / 2);

    // Expand stereo to quad
    adjust_channels_non_destructive(
            u16contracted.data() /*in_buff*/,
            2 /*in_channels*/,
            u16expanded.data() /*out_buff*/,
            4 /*out_channels*/,
            sizeof(u16contracted[0]) /*sample_size_in_bytes*/,
            sizeof(u16contracted[0]) * (u16contracted.size() / 2) /*num_in_bytes*/);

    // Comparison array must be identical to reference.
    expectEq(u16expanded, u16ref);

    // *** Second test: in_buff == out_buff ***

    // Contract from eight channels to stereo.
    adjust_channels_non_destructive(
            u16inout.data() /*in_buff*/,
            8 /*in_channels*/,
            u16inout.data() /*out_buff*/,
            2 /*out_channels*/,
            sizeof(u16inout[0]) /*sample_size_in_bytes*/,
            sizeof(u16inout[0]) * u16inout.size() /*num_in_bytes*/);

    // Each section [1/4][3/4] of contracted buffer should increase monotonically.
    checkMonotone(u16inout.data(), u16inout.size() / 4);
    checkMonotone(&u16inout[u16inout.size() / 4], (u16inout.size() * 3) / 4);

    // Expand stereo to eight channels.
    adjust_channels_non_destructive(
            u16inout.data() /*in_buff*/,
            2 /*in_channels*/,
            u16inout.data() /*out_buff*/,
            8 /*out_channels*/,
            sizeof(u16inout[0]) /*sample_size_in_bytes*/,
            sizeof(u16inout[0]) * (u16inout.size() / 4) /*num_in_bytes*/);

    // Comparison array must be identical to reference.
    expectEq(u16inout, u16ref);
}
