/*
 * Copyright 2017 The Android Open Source Project
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
#define LOG_TAG "audio_utils_power_tests"

#include <cmath>
#include <math.h>

#include <audio_utils/power.h>
#include <gtest/gtest.h>
#include <log/log.h>

typedef struct { uint8_t c[3]; } __attribute__((__packed__)) uint8x3_t;

void testFloatValue(float f_value, size_t length) {
    const float power = audio_utils_power_from_amplitude(f_value);
    float f_ary[length];
    uint8_t u8_ary[length];
    int16_t i16_ary[length];
    int32_t i32_ary[length];
    int32_t q8_23_ary[length];
    uint8x3_t p24_ary[length];

    // magic formulas to convert floating point to fixed point representations.
    // we negate the floating point value to ensure full integer range for 1.f.
    const uint8_t u8_value((1.f - f_value) * 128);
    const int16_t i16_value(f_value * INT16_MIN);
    const int32_t i32_value (f_value * INT32_MIN);
    const int32_t q8_23_value(f_value * -(1 << 23));

    // PCM_24_BIT_PACKED is native endian.
#if HAVE_BIG_ENDIAN
    const uint8x3_t p24_value{{
        uint8_t(q8_23_value >> 16),
                uint8_t(q8_23_value >> 8),
                uint8_t(q8_23_value),
    }};
#else
    const uint8x3_t p24_value{{
        uint8_t(q8_23_value),
                uint8_t(q8_23_value >> 8),
                uint8_t(q8_23_value >> 16),
    }};
#endif

    for (size_t i = 0; i < length; ++i) {
        f_ary[i] = f_value;
        u8_ary[i] = u8_value;
        i16_ary[i] = i16_value;
        i32_ary[i] = i32_value;
        q8_23_ary[i] = q8_23_value;
        p24_ary[i] = p24_value;
    }

    // check offset by 1, 2, 3 elements for unaligned NEON vector handling.
    for (size_t i = 0; i < 3; ++i) {
        if (i >= length) break;
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(f_ary + i, AUDIO_FORMAT_PCM_FLOAT, length - i));
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(u8_ary + i, AUDIO_FORMAT_PCM_8_BIT, length - i));
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(i16_ary + i, AUDIO_FORMAT_PCM_16_BIT, length - i));
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(i32_ary + i, AUDIO_FORMAT_PCM_32_BIT, length - i));
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(
                        q8_23_ary + i, AUDIO_FORMAT_PCM_8_24_BIT, length - i));
        EXPECT_EQ(power,
                audio_utils_compute_power_mono(
                        p24_ary + i, AUDIO_FORMAT_PCM_24_BIT_PACKED, length - i));
    }
}

void testFloatRamp(size_t length) {
    float f_ary[length];
    uint8_t u8_ary[length];
    int16_t i16_ary[length];
    int32_t i32_ary[length];
    int32_t q8_23_ary[length];
    uint8x3_t p24_ary[length];

    for (size_t i = 0; i < length; ++i) {
        // must be expressed cleanly in uint8_t
        const float f_value = (int(length & 0xff) - 128) / 128.f;

        // magic formulas to convert floating point to fixed point representations.
        // we negate the floating point value to ensure full integer range for 1.f.
        const uint8_t u8_value((1.f - f_value) * 128);
        const int16_t i16_value(f_value * INT16_MIN);
        const int32_t i32_value (f_value * INT32_MIN);
        const int32_t q8_23_value(f_value * -(1 << 23));

        // PCM_24_BIT_PACKED is native endian.
    #if HAVE_BIG_ENDIAN
        const uint8x3_t p24_value{{
            uint8_t(q8_23_value >> 16),
                    uint8_t(q8_23_value >> 8),
                    uint8_t(q8_23_value),
        }};
    #else
        const uint8x3_t p24_value{{
            uint8_t(q8_23_value),
                    uint8_t(q8_23_value >> 8),
                    uint8_t(q8_23_value >> 16),
        }};
    #endif

        f_ary[i] = f_value;
        u8_ary[i] = u8_value;
        i16_ary[i] = i16_value;
        i32_ary[i] = i32_value;
        q8_23_ary[i] = q8_23_value;
        p24_ary[i] = p24_value;
    }

    const float power8 =  audio_utils_compute_power_mono(u8_ary, AUDIO_FORMAT_PCM_8_BIT, length);

    EXPECT_EQ(power8,
            audio_utils_compute_power_mono(f_ary, AUDIO_FORMAT_PCM_FLOAT, length));
    EXPECT_EQ(power8,
            audio_utils_compute_power_mono(i16_ary, AUDIO_FORMAT_PCM_16_BIT, length));
    EXPECT_EQ(power8,
            audio_utils_compute_power_mono(i32_ary, AUDIO_FORMAT_PCM_32_BIT, length));
    EXPECT_EQ(power8,
            audio_utils_compute_power_mono(q8_23_ary, AUDIO_FORMAT_PCM_8_24_BIT, length));
    EXPECT_EQ(power8,
            audio_utils_compute_power_mono(p24_ary, AUDIO_FORMAT_PCM_24_BIT_PACKED, length));
}

// power_mono implicitly tests energy_mono
TEST(audio_utils_power, power_mono) {
    // f_values should have limited mantissa
    for (float f_value : { 0.f, 0.25f, 0.5f, 0.75f, 1.f }) {
        const float power = audio_utils_power_from_amplitude(f_value);
        printf("power_mono: amplitude: %f  power: %f\n", f_value, power);

        for (size_t length : { 1, 3, 5, 7, 16, 21, 32, 37 }) {
            testFloatValue(f_value, length);
        }
    }
}

// power_mono implicitly tests energy_mono
TEST(audio_utils_power, power_mono_ramp) {
    for (size_t length : { 1, 3, 5, 7, 16, 21, 32, 37, 297 }) {
        testFloatRamp(length);
    }
}

TEST(audio_utils_power, power_from) {
    EXPECT_EQ(0.f, audio_utils_power_from_amplitude(1.f));
    EXPECT_EQ(-INFINITY, audio_utils_power_from_amplitude(0.f));
    EXPECT_EQ(0.f, audio_utils_power_from_amplitude(-1.f));

    EXPECT_EQ(0.f, audio_utils_power_from_energy(1.f));
    EXPECT_EQ(-INFINITY, audio_utils_power_from_energy(0.f));
    EXPECT_TRUE(std::isnan(audio_utils_power_from_energy(-1.f)));
}
