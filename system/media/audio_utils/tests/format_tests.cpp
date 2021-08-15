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
#define LOG_TAG "audio_utils_format_tests"
#include <log/log.h>

#include <audio_utils/format.h>
#include <gtest/gtest.h>

/** returns true if the format is a common source or destination format.
    memcpy_by_audio_format() allows interchange between any PCM format and the
    "common" PCM 16 bit and PCM float formats. */
static bool is_common_format(audio_format_t format) {
    return format == AUDIO_FORMAT_PCM_16_BIT || format == AUDIO_FORMAT_PCM_FLOAT;
}

// Initialize PCM 16 bit ramp for basic data sanity check (generated from PCM 8 bit data).
// TODO: consider creating fillPseudoRandomValue().
template<size_t size>
static void fillRamp(int16_t(&buffer)[size])
{
    // Create PCM 16 bit data based on PCM 8 bit format because PCM 8 bit is convertible
    // to all other audio formats without loss; hence, round trip conversion preserves equality.
    uint8_t bytes[size];
    for (size_t i = 0; i < size; ++i) {
        bytes[i] = i;
    }
    // convert to PCM 16 bit
    memcpy_by_audio_format(
            buffer, AUDIO_FORMAT_PCM_16_BIT,
            bytes, AUDIO_FORMAT_PCM_8_BIT, size);

    uint8_t check[size];
    memcpy_by_audio_format(
            check, AUDIO_FORMAT_PCM_8_BIT,
            buffer, AUDIO_FORMAT_PCM_16_BIT, size);
    EXPECT_EQ(0, memcmp(check, bytes, size));
}

class FormatTest : public testing::TestWithParam<std::tuple<audio_format_t, audio_format_t>>
{
};

TEST_P(FormatTest, memcpy_by_audio_format)
{
    // fetch parameters
    const auto param = GetParam();
    const audio_format_t src_encoding = std::get<0>(param);
    const audio_format_t dst_encoding = std::get<1>(param);

    // either source or destination (or both) need to be a common format
    if (!is_common_format(src_encoding) && !is_common_format(dst_encoding)) {
        printf("skip conversion src:%#x  dst:%#x\n", src_encoding, dst_encoding);
        return;
    }

    constexpr size_t SAMPLES = UINT8_MAX;
    constexpr audio_format_t orig_encoding = AUDIO_FORMAT_PCM_16_BIT;
    int16_t orig_data[SAMPLES];

    fillRamp(orig_data);

    // data buffer for in-place conversion (uint32_t is maximum sample size of 4 bytes)
    uint32_t data[SAMPLES];
    // check buffer is used to compare out-of-place vs in-place conversion.
    uint32_t check[SAMPLES];

    printf("trying conversion src:%#x  dst:%#x\n", src_encoding, dst_encoding);
    fflush(stdout);
    // Copy original data to data buffer at src_encoding.
    memcpy_by_audio_format(
            data, src_encoding,
            orig_data, orig_encoding, SAMPLES);

    // Convert from src encoding to dst encoding.
    memcpy_by_audio_format(
            check, dst_encoding,
            data, src_encoding, SAMPLES);

    // Check in-place is same as out-of-place conversion.
    memcpy_by_audio_format(
            data, dst_encoding,
            data, src_encoding, SAMPLES);
    EXPECT_EQ(0, memcmp(check, data, SAMPLES * audio_bytes_per_sample(dst_encoding)));

    // Go back to the original data encoding for comparison.
    memcpy_by_audio_format(
            data, orig_encoding,
            data, dst_encoding, SAMPLES);

    // Raw byte compare at the original encoding must succeed - our conversions
    // must be lossless for PCM 8 bit representation which orig_data was constructed from.
    EXPECT_EQ(0,
            memcmp(data, orig_data, SAMPLES * audio_bytes_per_sample(orig_encoding)));
}

INSTANTIATE_TEST_CASE_P(FormatVariations, FormatTest, ::testing::Combine(
    ::testing::Values(
        AUDIO_FORMAT_PCM_8_BIT,
        AUDIO_FORMAT_PCM_16_BIT,
        AUDIO_FORMAT_PCM_FLOAT,
        AUDIO_FORMAT_PCM_24_BIT_PACKED,
        AUDIO_FORMAT_PCM_32_BIT,
        AUDIO_FORMAT_PCM_8_24_BIT
    ),
    ::testing::Values(
        AUDIO_FORMAT_PCM_8_BIT,
        AUDIO_FORMAT_PCM_16_BIT,
        AUDIO_FORMAT_PCM_FLOAT,
        AUDIO_FORMAT_PCM_24_BIT_PACKED,
        AUDIO_FORMAT_PCM_32_BIT,
        AUDIO_FORMAT_PCM_8_24_BIT
    )));
