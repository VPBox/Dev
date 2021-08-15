/*
 * Copyright (C) 2014 The Android Open Source Project
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
#define LOG_TAG "audio_utils_primitives_tests"

#include <math.h>
#include <vector>

#include <gtest/gtest.h>
#include <log/log.h>

#include <audio_utils/primitives.h>
#include <audio_utils/format.h>
#include <audio_utils/channels.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const int32_t lim8pos = 255;
static const int32_t lim8neg = 0;
static const int32_t lim16pos = (1 << 15) - 1;
static const int32_t lim16neg = -(1 << 15);
static const int32_t lim24pos = (1 << 23) - 1;
static const int32_t lim24neg = -(1 << 23);
static const int64_t lim32pos = 0x000000007fffffff;
static const int64_t lim32neg = 0xffffffff80000000;

// Use memset here since it is generally the fastest method of clearing data,
// but could be changed to std::fill or assignment should those prove faster.
template <typename T>
static void zeroFill(T &container) {
    memset(container.data(), 0, container.size() * sizeof(container[0]));
}

inline void testClamp8(float f)
{
    // f is in native u8 scaling to test rounding
    uint8_t uval = clamp8_from_float((f - 128) / (1 << 7));

    // test clamping
    ALOGV("clamp8_from_float(%f) = %u\n", f, uval);
    if (f > lim8pos) {
        EXPECT_EQ(lim8pos, uval);
    } else if (f < lim8neg) {
        EXPECT_EQ(lim8neg, uval);
    }

    // if in range, make sure round trip clamp and conversion is correct.
    if (f < lim8pos - 1. && f > lim8neg + 1.) {
        uint8_t uval2 = clamp8_from_float(float_from_u8(uval));
        int diff = abs(uval - uval2);
        EXPECT_LE(diff, 1);
    }
}

inline void testClamp16(float f)
{
    int16_t ival = clamp16_from_float(f / (1 << 15));

    // test clamping
    ALOGV("clamp16_from_float(%f) = %d\n", f, ival);
    if (f > lim16pos) {
        EXPECT_EQ(lim16pos, ival);
    } else if (f < lim16neg) {
        EXPECT_EQ(lim16neg, ival);
    }

    // if in range, make sure round trip clamp and conversion is correct.
    if (f < lim16pos - 1. && f > lim16neg + 1.) {
        int ival2 = clamp16_from_float(float_from_i16(ival));
        int diff = abs(ival - ival2);
        EXPECT_LE(diff, 1);
    }
}

inline void testClamp24(float f)
{
    int32_t ival = clamp24_from_float(f / (1 << 23));

    // test clamping
    ALOGV("clamp24_from_float(%f) = %d\n", f, ival);
    if (f > lim24pos) {
        EXPECT_EQ(lim24pos, ival);
    } else if (f < lim24neg) {
        EXPECT_EQ(lim24neg, ival);
    }

    // if in range, make sure round trip clamp and conversion is correct.
    if (f < lim24pos - 1. && f > lim24neg + 1.) {
        int ival2 = clamp24_from_float(float_from_q8_23(ival));
        int diff = abs(ival - ival2);
        EXPECT_LE(diff, 1);
    }
}

template<typename T>
void checkMonotone(const T *ary, size_t size)
{
    for (size_t i = 1; i < size; ++i) {
        EXPECT_LT(ary[i-1], ary[i]);
    }
}

void checkMonotonep24(uint8_t * pary, size_t size)
{
    size_t frames = size/3;
    for (size_t i = 1; i < frames; ++i) {
        EXPECT_LT(i32_from_p24(pary + 3*(i-1)), i32_from_p24(pary + 3*i));
    }
}

TEST(audio_utils_primitives, clamp_to_int) {
    static const float testArray[] = {
            -NAN, -INFINITY,
            -1.e20, -32768., 63.9,
            -3.5, -3.4, -2.5, 2.4, -1.5, -1.4, -0.5, -0.2, 0., 0.2, 0.5, 0.8,
            1.4, 1.5, 1.8, 2.4, 2.5, 2.6, 3.4, 3.5,
            32767., 32768., 1.e20,
            INFINITY, NAN };

    for (size_t i = 0; i < ARRAY_SIZE(testArray); ++i) {
        testClamp8(testArray[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(testArray); ++i) {
        testClamp16(testArray[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(testArray); ++i) {
        testClamp24(testArray[i]);
    }

    // used for ULP testing (tweaking the lsb of the float)
    union {
        int32_t i;
        float f;
    } val;
    int32_t res;

    // check clampq4_27_from_float()
    val.f = 16.;
    res = clampq4_27_from_float(val.f);
    EXPECT_EQ(0x7fffffff, res);
    val.i--;
    res = clampq4_27_from_float(val.f);
    EXPECT_LE(res, 0x7fffffff);
    EXPECT_GE(res, 0x7fff0000);
    val.f = -16.;
    res = clampq4_27_from_float(val.f);
    EXPECT_EQ((int32_t)0x80000000, res); // negative
    val.i++;
    res = clampq4_27_from_float(val.f);
    EXPECT_GE(res, (int32_t)0x80000000); // negative
    EXPECT_LE(res, (int32_t)0x80008000); // negative

    // check u4_28_from_float and u4_12_from_float
    uint32_t ures;
    uint16_t ures16;
    val.f = 16.;
    ures = u4_28_from_float(val.f);
    EXPECT_EQ(0xffffffff, ures);
    ures16 = u4_12_from_float(val.f);
    EXPECT_EQ(0xffff, ures16);

    val.f = -1.;
    ures = u4_28_from_float(val.f);
    EXPECT_EQ((uint32_t)0, ures);
    ures16 = u4_12_from_float(val.f);
    EXPECT_EQ(0, ures16);

    // check float_from_u4_28 and float_from_u4_12 (roundtrip)
    for (uint32_t v = 0x100000; v <= 0xff000000; v += 0x100000) {
        ures = u4_28_from_float(float_from_u4_28(v));
        EXPECT_EQ(ures, v);
    }
    for (uint32_t v = 0; v <= 0xffff; ++v) { // uint32_t prevents overflow
        ures16 = u4_12_from_float(float_from_u4_12(v));
        EXPECT_EQ(ures16, v);
    }

    // check infinity
    EXPECT_EQ(0, clamp8_from_float(-INFINITY));
    EXPECT_EQ(255, clamp8_from_float(INFINITY));
}

TEST(audio_utils_primitives, memcpy) {
    // test round-trip.
    constexpr size_t size = 65536;
    std::vector<int16_t> i16ref(size);
    std::vector<int16_t> i16ary(size);
    std::vector<int32_t> i32ary(size);
    std::vector<float> fary(size);
    std::vector<uint8_t> pary(size * 3);


    // set signed reference monotonic array from -32768 to 32767
    for (size_t i = 0; i < i16ref.size(); ++i) {
        i16ref[i] = i16ary[i] = i - 32768;
    }

    // do round-trip testing i16 and float
    memcpy_to_float_from_i16(fary.data(), i16ary.data(), fary.size());
    zeroFill(i16ary);
    checkMonotone(fary.data(), fary.size());

    memcpy_to_i16_from_float(i16ary.data(), fary.data(), i16ary.size());
    zeroFill(fary);
    checkMonotone(i16ary.data(), i16ary.size());

    // TODO make a template case for the following?

    // do round-trip testing p24 to i16 and float
    memcpy_to_p24_from_i16(pary.data(), i16ary.data(), size /* note pary elem is 3 bytes */);
    zeroFill(i16ary);

    // check an intermediate format at a position(???)
#if 0
    printf("pary[%d].0 = %u  pary[%d].1 = %u  pary[%d].2 = %u\n",
            1025, (unsigned) pary[1025*3],
            1025, (unsigned) pary[1025*3+1],
            1025, (unsigned) pary[1025*3+2]
    );
#endif

    memcpy_to_float_from_p24(fary.data(), pary.data(), fary.size());
    zeroFill(pary);
    checkMonotone(fary.data(), fary.size());

    memcpy_to_p24_from_float(pary.data(), fary.data(), size /* note pary elem is 3 bytes */);
    zeroFill(fary);
    checkMonotonep24(pary.data(), pary.size() /* this is * 3*/);

    memcpy_to_i16_from_p24(i16ary.data(), pary.data(), i16ary.size());
    zeroFill(pary);
    checkMonotone(i16ary.data(), i16ary.size());

    // do round-trip testing q8_23 to i16 and float
    memcpy_to_q8_23_from_i16(i32ary.data(), i16ary.data(), i32ary.size());
    zeroFill(i16ary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_float_from_q8_23(fary.data(), i32ary.data(), fary.size());
    zeroFill(i32ary);
    checkMonotone(fary.data(), fary.size());

    memcpy_to_q8_23_from_float_with_clamp(i32ary.data(), fary.data(), i32ary.size());
    zeroFill(fary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_i16_from_q8_23(i16ary.data(), i32ary.data(), i16ary.size());
    zeroFill(i32ary);
    checkMonotone(i16ary.data(), i16ary.size());

    // do round-trip testing i32 to i16 and float
    memcpy_to_i32_from_i16(i32ary.data(), i16ary.data(), i32ary.size());
    zeroFill(i16ary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_float_from_i32(fary.data(), i32ary.data(), fary.size());
    zeroFill(i32ary);
    checkMonotone(fary.data(), fary.size());

    memcpy_to_i32_from_float(i32ary.data(), fary.data(), i32ary.size());
    zeroFill(fary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_i16_from_i32(i16ary.data(), i32ary.data(), i16ary.size());
    zeroFill(i32ary);
    checkMonotone(i16ary.data(), i16ary.size());

    // do round-trip test i16 -> p24 -> i32 -> p24 -> q8_23 -> p24 -> i16
    memcpy_to_p24_from_i16(pary.data(), i16ary.data(), size /* note pary elem is 3 bytes */);
    zeroFill(i16ary);
    checkMonotonep24(pary.data(), pary.size() /* this is * 3*/);

    memcpy_to_i32_from_p24(i32ary.data(), pary.data(), i32ary.size());
    zeroFill(pary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_p24_from_i32(pary.data(), i32ary.data(), size /* note pary elem is 3 bytes */);
    zeroFill(i32ary);
    checkMonotonep24(pary.data(), pary.size() /* this is * 3*/);

    memcpy_to_q8_23_from_p24(i32ary.data(), pary.data(), i32ary.size());
    zeroFill(pary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_p24_from_q8_23(pary.data(), i32ary.data(), size /* note pary elem is 3 bytes */);
    zeroFill(i32ary);
    checkMonotonep24(pary.data(), pary.size() /* this is * 3*/);

    memcpy_to_i16_from_p24(i16ary.data(), pary.data(), i16ary.size());
    zeroFill(pary);
    checkMonotone(i16ary.data(), i16ary.size());

    // do partial round-trip testing q4_27 to i16 and float
    memcpy_to_float_from_i16(fary.data(), i16ary.data(), fary.size());
    zeroFill(i16ary);

    memcpy_to_q4_27_from_float(i32ary.data(), fary.data(), i32ary.size());
    zeroFill(fary);
    checkMonotone(i32ary.data(), i32ary.size());

    memcpy_to_i16_from_q4_27(i16ary.data(), i32ary.data(), i16ary.size());
    checkMonotone(i16ary.data(), i16ary.size());
    EXPECT_EQ(0, memcmp(i16ary.data(), i16ref.data(), i16ary.size() * sizeof(i16ary[0])));

    zeroFill(i16ary);

    // ditherAndClamp() has non-standard parameters - memcpy_to_float_from_q4_27() is preferred
    ditherAndClamp(reinterpret_cast<int32_t *>(i16ary.data()),
            i32ary.data(), i16ary.size() / 2);
    checkMonotone(i16ary.data(), i16ary.size());
    EXPECT_EQ(0, memcmp(i16ary.data(), i16ref.data(), i16ary.size() * sizeof(i16ary[0])));

    memcpy_to_float_from_q4_27(fary.data(), i32ary.data(), fary.size());
    zeroFill(i32ary);
    checkMonotone(fary.data(), fary.size());

    // at the end, our i16ary must be the same. (Monotone should be equivalent to this)
    EXPECT_EQ(0, memcmp(i16ary.data(), i16ref.data(), i16ary.size() * sizeof(i16ary[0])));

    // test round-trip for u8 and float.
    constexpr size_t u8size = 256;
    std::vector<uint8_t> u8ref(u8size);
    std::vector<uint8_t> u8ary(u8size);

    for (size_t i = 0; i < u8ref.size(); ++i) {
        u8ref[i] = i;
    }

    constexpr size_t testsize = std::min(u8size, size);
    zeroFill(fary);
    memcpy_to_float_from_u8(fary.data(), u8ref.data(), testsize);
    memcpy_to_u8_from_float(u8ary.data(), fary.data(), testsize);

    EXPECT_EQ(0, memcmp(u8ary.data(), u8ref.data(), u8ary.size() * sizeof(u8ary[0])));

    // test conversion from u8 to i32
    zeroFill(i32ary);
    memcpy_to_i32_from_u8(i32ary.data(), u8ref.data(), testsize);
    checkMonotone(i32ary.data(), testsize);
}

template<typename T>
void checkMonotoneOrZero(const T *ary, size_t size)
{
    T least = 0;

    for (size_t i = 1; i < size; ++i) {
        if (ary[i]) {
            EXPECT_LT(least, ary[i]);
            least = ary[i];
        }
    }
}

TEST(audio_utils_primitives, memcpy_by_channel_mask) {
    uint32_t dst_mask;
    uint32_t src_mask;
    uint16_t *u16ref = new uint16_t[65536];
    uint16_t *u16ary = new uint16_t[65536];

    for (size_t i = 0; i < 65536; ++i) {
        u16ref[i] = i;
    }

    // Test when src mask is 0.  Everything copied is zero.
    src_mask = 0;
    dst_mask = 0x8d;
    memset(u16ary, 0x99, 65536 * sizeof(u16ref[0]));
    memcpy_by_channel_mask(u16ary, dst_mask, u16ref, src_mask, sizeof(u16ref[0]),
            65536 / popcount(dst_mask));
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when dst_mask is 0.  Nothing should be copied.
    src_mask = 0;
    dst_mask = 0;
    memset(u16ary, 0, 65536 * sizeof(u16ref[0]));
    memcpy_by_channel_mask(u16ary, dst_mask, u16ref, src_mask, sizeof(u16ref[0]),
            65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when masks are the same.  One to one copy.
    src_mask = dst_mask = 0x8d;
    memset(u16ary, 0x99, 65536 * sizeof(u16ref[0]));
    memcpy_by_channel_mask(u16ary, dst_mask, u16ref, src_mask, sizeof(u16ref[0]), 555);
    EXPECT_EQ(0, memcmp(u16ary, u16ref, 555 * sizeof(u16ref[0]) * popcount(dst_mask)));

    // Test with a gap in source:
    // Input 3 samples, output 4 samples, one zero inserted.
    src_mask = 0x8c;
    dst_mask = 0x8d;
    memset(u16ary, 0x9, 65536 * sizeof(u16ary[0]));
    memcpy_by_channel_mask(u16ary, dst_mask, u16ref, src_mask, sizeof(u16ref[0]),
            65536 / popcount(dst_mask));
    checkMonotoneOrZero(u16ary, 65536);
    EXPECT_EQ((size_t)(65536 * 3 / 4 - 1), nonZeroMono16((int16_t*)u16ary, 65536));

    // Test with a gap in destination:
    // Input 4 samples, output 3 samples, one deleted
    src_mask = 0x8d;
    dst_mask = 0x8c;
    memset(u16ary, 0x9, 65536 * sizeof(u16ary[0]));
    memcpy_by_channel_mask(u16ary, dst_mask, u16ref, src_mask, sizeof(u16ref[0]),
            65536 / popcount(src_mask));
    checkMonotone(u16ary, 65536 * 3 / 4);

    delete[] u16ref;
    delete[] u16ary;
}

void memcpy_by_channel_mask2(void *dst, uint32_t dst_mask,
        const void *src, uint32_t src_mask, size_t sample_size, size_t count)
{
    int8_t idxary[32];
    uint32_t src_channels = popcount(src_mask);
    uint32_t dst_channels =
            memcpy_by_index_array_initialization(idxary, 32, dst_mask, src_mask);

    memcpy_by_index_array(dst, dst_channels, src, src_channels, idxary, sample_size, count);
}

// a modified version of the memcpy_by_channel_mask test
// but using 24 bit type and memcpy_by_index_array()
TEST(audio_utils_primitives, memcpy_by_index_array) {
    uint32_t dst_mask;
    uint32_t src_mask;
    typedef struct {uint8_t c[3];} __attribute__((__packed__)) uint8x3_t;
    uint8x3_t *u24ref = new uint8x3_t[65536];
    uint8x3_t *u24ary = new uint8x3_t[65536];
    uint16_t *u16ref = new uint16_t[65536];
    uint16_t *u16ary = new uint16_t[65536];

    EXPECT_EQ((size_t)3, sizeof(uint8x3_t)); // 3 bytes per struct

    // tests prepare_index_array_from_masks()
    EXPECT_EQ((size_t)4, memcpy_by_index_array_initialization(NULL, 0, 0x8d, 0x8c));
    EXPECT_EQ((size_t)3, memcpy_by_index_array_initialization(NULL, 0, 0x8c, 0x8d));

    for (size_t i = 0; i < 65536; ++i) {
        u16ref[i] = i;
    }
    memcpy_to_p24_from_i16((uint8_t*)u24ref, (int16_t*)u16ref, 65536);

    // Test when src mask is 0.  Everything copied is zero.
    src_mask = 0;
    dst_mask = 0x8d;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask2(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when dst_mask is 0.  Nothing should be copied.
    src_mask = 0;
    dst_mask = 0;
    memset(u24ary, 0, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask2(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536);
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when masks are the same.  One to one copy.
    src_mask = dst_mask = 0x8d;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask2(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]), 555);
    EXPECT_EQ(0, memcmp(u24ary, u24ref, 555 * sizeof(u24ref[0]) * popcount(dst_mask)));

    // Test with a gap in source:
    // Input 3 samples, output 4 samples, one zero inserted.
    src_mask = 0x8c;
    dst_mask = 0x8d;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask2(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotoneOrZero(u16ary, 65536);
    EXPECT_EQ((size_t)(65536 * 3 / 4 - 1), nonZeroMono16((int16_t*)u16ary, 65536));

    // Test with a gap in destination:
    // Input 4 samples, output 3 samples, one deleted
    src_mask = 0x8d;
    dst_mask = 0x8c;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask2(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(src_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotone(u16ary, 65536 * 3 / 4);

    delete[] u16ref;
    delete[] u16ary;
    delete[] u24ref;
    delete[] u24ary;
}

void memcpy_by_channel_mask_dst_index(void *dst, uint32_t dst_mask,
        const void *src, uint32_t src_mask, size_t sample_size, size_t count)
{
    int8_t idxary[32];
    uint32_t src_channels = popcount(src_mask);
    uint32_t dst_channels =
            memcpy_by_index_array_initialization_dst_index(idxary, 32, dst_mask, src_mask);

    memcpy_by_index_array(dst, dst_channels, src, src_channels, idxary, sample_size, count);
}

// a modified version of the memcpy_by_channel_mask test
// but using 24 bit type and memcpy_by_index_array()
TEST(audio_utils_primitives, memcpy_by_index_array_dst_index) {
    uint32_t dst_mask;
    uint32_t src_mask;
    typedef struct {uint8_t c[3];} __attribute__((__packed__)) uint8x3_t;
    uint8x3_t *u24ref = new uint8x3_t[65536];
    uint8x3_t *u24ary = new uint8x3_t[65536];
    uint16_t *u16ref = new uint16_t[65536];
    uint16_t *u16ary = new uint16_t[65536];

    EXPECT_EQ((size_t)3, sizeof(uint8x3_t)); // 3 bytes per struct

    // tests prepare_index_array_from_masks()
    EXPECT_EQ((size_t)4, memcpy_by_index_array_initialization_dst_index(NULL, 0, 0x8d, 0x8c));
    EXPECT_EQ((size_t)3, memcpy_by_index_array_initialization_dst_index(NULL, 0, 0x8c, 0x8d));

    for (size_t i = 0; i < 65536; ++i) {
        u16ref[i] = i;
    }
    memcpy_to_p24_from_i16((uint8_t*)u24ref, (int16_t*)u16ref, 65536);

    // Test when src mask is 0.  Everything copied is zero.
    src_mask = 0;
    dst_mask = 0x8d;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_dst_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when dst_mask is 0.  Nothing should be copied.
    src_mask = 0;
    dst_mask = 0;
    memset(u24ary, 0, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_dst_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536);
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when dst mask equals source count size.  One to one copy.
    src_mask = 0x8d;
    dst_mask = 0x0f;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_dst_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]), 555);
    EXPECT_EQ(0, memcmp(u24ary, u24ref, 555 * sizeof(u24ref[0]) * popcount(dst_mask)));

    // Test with a gap in source:
    // Input 3 samples, output 4 samples, one zero inserted.
    src_mask = 0x8c;
    dst_mask = 0x0f;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_dst_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotoneOrZero(u16ary, 65536);
    EXPECT_EQ((size_t)(65536 * 3 / 4 - 1), nonZeroMono16((int16_t*)u16ary, 65536));

    // Test with a gap in destination:
    // Input 4 samples, output 3 samples, one deleted
    src_mask = 0x8d;
    dst_mask = 0x07;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_dst_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(src_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotone(u16ary, 65536 * 3 / 4);

    delete[] u16ref;
    delete[] u16ary;
    delete[] u24ref;
    delete[] u24ary;
}

void memcpy_by_channel_mask_src_index(void *dst, uint32_t dst_mask,
        const void *src, uint32_t src_mask, size_t sample_size, size_t count)
{
    int8_t idxary[32];
    uint32_t src_channels = popcount(src_mask);
    uint32_t dst_channels =
            memcpy_by_index_array_initialization_src_index(idxary, 32, dst_mask, src_mask);

    memcpy_by_index_array(dst, dst_channels, src, src_channels, idxary, sample_size, count);
}

// a modified version of the memcpy_by_channel_mask test
// but using 24 bit type and memcpy_by_index_array()
TEST(audio_utils_primitives, memcpy_by_index_array_src_index) {
    uint32_t dst_mask;
    uint32_t src_mask;
    typedef struct {uint8_t c[3];} __attribute__((__packed__)) uint8x3_t;
    uint8x3_t *u24ref = new uint8x3_t[65536];
    uint8x3_t *u24ary = new uint8x3_t[65536];
    uint16_t *u16ref = new uint16_t[65536];
    uint16_t *u16ary = new uint16_t[65536];

    EXPECT_EQ((size_t)3, sizeof(uint8x3_t)); // 3 bytes per struct

    // tests prepare_index_array_from_masks()
    EXPECT_EQ((size_t)4, memcpy_by_index_array_initialization_src_index(NULL, 0, 0x8d, 0x8c));
    EXPECT_EQ((size_t)3, memcpy_by_index_array_initialization_src_index(NULL, 0, 0x8c, 0x8d));

    for (size_t i = 0; i < 65536; ++i) {
        u16ref[i] = i;
    }
    memcpy_to_p24_from_i16((uint8_t*)u24ref, (int16_t*)u16ref, 65536);

    // Test when src mask is 0.  Everything copied is zero.
    src_mask = 0;
    dst_mask = 0x8d;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when dst_mask is 0.  Nothing should be copied.
    src_mask = 0;
    dst_mask = 0;
    memset(u24ary, 0, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536);
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    EXPECT_EQ((size_t)0, nonZeroMono16((int16_t*)u16ary, 65530));

    // Test when source mask must copy to dst mask.  One to one copy.
    src_mask = 0xf;
    dst_mask = 0xf;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]), 555);
    EXPECT_EQ(0, memcmp(u24ary, u24ref, 555 * sizeof(u24ref[0]) * popcount(dst_mask)));

    // Test when source mask must copy to dst mask.  One to one copy.
    src_mask = 0xf;
    dst_mask = 0x8d;
    memset(u24ary, 0x99, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]), 555);
    EXPECT_EQ(0, memcmp(u24ary, u24ref, 555 * sizeof(u24ref[0]) * popcount(dst_mask)));

    // Test with a gap in source:
    // Input 3 samples, output 4 samples, one zero inserted.
    src_mask = 0x07;
    dst_mask = 0x8d;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(dst_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotoneOrZero(u16ary, 65536);
    EXPECT_EQ((size_t)(65536 * 3 / 4 - 1), nonZeroMono16((int16_t*)u16ary, 65536));

    // Test with a gap in destination:
    // Input 4 samples, output 3 samples, one deleted
    src_mask = 0x0f;
    dst_mask = 0x8c;
    memset(u24ary, 0x9, 65536 * sizeof(u24ary[0]));
    memcpy_by_channel_mask_src_index(u24ary, dst_mask, u24ref, src_mask, sizeof(u24ref[0]),
            65536 / popcount(src_mask));
    memcpy_to_i16_from_p24((int16_t*)u16ary, (uint8_t*)u24ary, 65536);
    checkMonotone(u16ary, 65536 * 3 / 4);

    delete[] u16ref;
    delete[] u16ary;
    delete[] u24ref;
    delete[] u24ary;
}

TEST(audio_utils_primitives, updown_mix) {
    const size_t size = 32767;
    std::vector<int16_t> i16ref(size * 2);
    std::vector<int16_t> i16ary(size * 2);

    for (size_t i = 0; i < size; ++i) {
        i16ref[i] = i;
    }
    upmix_to_stereo_i16_from_mono_i16(i16ary.data(), i16ref.data(), size);
    downmix_to_mono_i16_from_stereo_i16(i16ary.data(), i16ary.data(), size);

    EXPECT_EQ(0, memcmp(i16ary.data(), i16ref.data(), sizeof(i16ref[0]) * size));
}

template<typename T, typename TComparison>
void checkAddedClamped(T *out, const T *in1, const T *in2, size_t size,
        TComparison limNeg, TComparison limPos)
{
    for (size_t i = 0; i < size; ++i) {
        TComparison added = (TComparison)in1[i] + in2[i];
        if (added <= limNeg) {
            EXPECT_EQ(limNeg, out[i]);
        } else if (added >= limPos) {
            EXPECT_EQ(limPos, out[i]);
        } else {
            EXPECT_EQ(added, out[i]);
        }
    }
}

void checkAddedClampedp24(uint8_t *pary, const uint8_t *in1,
        const uint8_t *in2, size_t size) {
    // Convert to q8_23 for comparison.
    int32_t *outi32ary = new int32_t[size];
    int32_t *in1i32ary = new int32_t[size];
    int32_t *in2i32ary = new int32_t[size];
    memcpy_to_q8_23_from_p24(outi32ary, pary, size);
    memcpy_to_q8_23_from_p24(in1i32ary, in1, size);
    memcpy_to_q8_23_from_p24(in2i32ary, in2, size);
    checkAddedClamped(
            outi32ary, in1i32ary, in2i32ary, size, lim24neg, lim24pos);
    delete[] in2i32ary;
    delete[] in1i32ary;
    delete[] outi32ary;
}

void checkAddedClampedu8(uint8_t *out, const uint8_t *in1,
        const uint8_t *in2, size_t size) {
    // uint8_t data is centered around 0x80, not 0, so checkAddedClamped
    // won't work. Convert to i16 first.
    int16_t *outi16ary = new int16_t[size];
    int16_t *in1i16ary = new int16_t[size];
    int16_t *in2i16ary = new int16_t[size];
    memcpy_to_i16_from_u8(outi16ary, out, size);
    memcpy_to_i16_from_u8(in1i16ary, in1, size);
    memcpy_to_i16_from_u8(in2i16ary, in2, size);
    // Only the higher order bits are used.
    checkAddedClamped(outi16ary, in1i16ary, in2i16ary, size,
            -0x8000, 0x7f00);
    delete[] in2i16ary;
    delete[] in1i16ary;
    delete[] outi16ary;
}

TEST(audio_utils_primitives, accumulate) {
    int16_t *i16ref = new int16_t[65536];
    int16_t *i16add = new int16_t[65536];
    int16_t *i16ary = new int16_t[65536];

    for (size_t i = 0; i < 65536; ++i) {
        i16ref[i] = i16ary[i] = i16add[(i+1) % 65536] = i - 32768;
    }

    // Test i16.
    accumulate_i16(i16ary, i16add, 65536);
    checkAddedClamped(i16ary, i16ref, i16add, 65536, lim16neg,
            lim16pos);

    // Test i32.
    int32_t *i32ary = new int32_t[65536];
    int32_t *i32add = new int32_t[65536];
    int32_t *i32ref = new int32_t[65536];
    // Convert sample data to i32 to perform accumulate function.
    memcpy_to_i32_from_i16(i32ary, i16ref, 65536);
    memcpy_to_i32_from_i16(i32add, i16add, 65536);
    // Ensure the reference matches the inital output after conversion.
    memcpy(i32ref, i32ary, 65536 * sizeof(i32ary[0]));
    // Accumulate and check.
    accumulate_i32(i32ary, i32add, 65536);
    checkAddedClamped(
            i32ary, i32ref, i32add, 65536, lim32neg, lim32pos);
    // Cleanup
    delete[] i32ref;
    delete[] i32add;
    delete[] i32ary;

    // Test u8.
    uint8_t *u8ary = new uint8_t[65536];
    uint8_t *u8add = new uint8_t[65536];
    uint8_t *u8ref = new uint8_t[65536];
    // Convert sample data to u8 to perform accumulate function.
    memcpy_to_u8_from_i16(u8ary, i16ref, 65536);
    memcpy_to_u8_from_i16(u8add, i16add, 65536);
    // Ensure the reference matches the inital output after conversion.
    memcpy(u8ref, u8ary, 65536 * sizeof(u8ary[0]));
    // Accumulate and check.
    accumulate_u8(u8ary, u8add, 65536);
    checkAddedClampedu8(u8ary, u8ref, u8add, 65536);
    // Cleanup.
    delete[] u8ref;
    delete[] u8add;
    delete[] u8ary;

    // Test 24 bit packed.
    uint8_t *pary = new uint8_t[65536 * 3];
    uint8_t *padd = new uint8_t[65536 * 3];
    uint8_t *pref = new uint8_t[65536 * 3];
    // Convert sample data to p24 to perform accumulate function.
    memcpy_to_p24_from_i16(pary, i16ref, 65536);
    memcpy_to_p24_from_i16(padd, i16add, 65536);
    // Ensure the reference matches the inital output after conversion.
    memcpy(pref, pary, 65536 * sizeof(pary[0]) * 3);
    // Accumulate and check.
    accumulate_p24(pary, padd, 65536);
    checkAddedClampedp24(pary, pref, padd, 65536);
    // Cleanup.
    delete[] pref;
    delete[] padd;
    delete[] pary;

    // Test 24 bit unpacked.
    int32_t *q8_23ary = new int32_t[65536];
    int32_t *q8_23add = new int32_t[65536];
    int32_t *q8_23ref = new int32_t[65536];
    // Convert sample data to q8_23 to perform accumulate function.
    memcpy_to_q8_23_from_i16(q8_23ary, i16ref, 65536);
    memcpy_to_q8_23_from_i16(q8_23add, i16add, 65536);
    // Ensure the reference matches the inital output after conversion.
    memcpy(q8_23ref, q8_23ary, 65536 * sizeof(q8_23ary[0]));
    // Accumulate and check.
    accumulate_q8_23(q8_23ary, q8_23add, 65536);
    checkAddedClamped(
            q8_23ary, q8_23ref, q8_23add, 65536, lim24neg, lim24pos);
    // Cleanup.
    delete[] q8_23ref;
    delete[] q8_23add;
    delete[] q8_23ary;

    // Test float.
    float *fary = new float[65536];
    float *fadd = new float[65536];
    float *fref = new float[65536];
    // Convert sample data to float to perform accumulate function.
    memcpy_to_float_from_i16(fary, i16ref, 65536);
    memcpy_to_float_from_i16(fadd, i16add, 65536);
    // Ensure the reference matches the inital output after conversion.
    memcpy(fref, fary, 65536 * sizeof(fary[0]));
    // Accumulate and check. Floats aren't clamped by accumulate,
    // but given the input is in the [-1.0, 1.0) range output should be in
    // [-2.0, 2.0) range.
    accumulate_float(fary, fadd, 65536);
    checkAddedClamped(fary, fref, fadd, 65536, -2.0f, 2.0f);
    // Cleanup.
    delete[] fref;
    delete[] fadd;
    delete[] fary;

    delete[] i16ary;
    delete[] i16add;
    delete[] i16ref;
}


TEST(audio_utils_primitives, MemcpyToFloatFromFloatWithClamping) {
    std::vector<float> src = {-INFINITY, -2, -1, -0, 0, 0.009, 1.000001, 9999999, INFINITY, NAN};
    std::vector<float> dst(src.size());
    float absMax = 1;
    std::vector<float> expected = {-1, -1, -1, -0, 0, 0.009, 1, 1, 1, 1};
    ASSERT_EQ(expected.size(), src.size());

    memcpy_to_float_from_float_with_clamping(dst.data(), src.data(), src.size(), absMax);

    ASSERT_EQ(dst, expected) << "src=" << testing::PrintToString(src);
}
