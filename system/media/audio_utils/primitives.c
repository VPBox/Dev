/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <cutils/bitops.h>  /* for popcount() */
#include <audio_utils/primitives.h>
#include "private/private.h"

void ditherAndClamp(int32_t *out, const int32_t *sums, size_t pairs)
{
    for (; pairs > 0; --pairs) {
        const int32_t l = clamp16(*sums++ >> 12);
        const int32_t r = clamp16(*sums++ >> 12);
        *out++ = (r << 16) | (l & 0xFFFF);
    }
}

void memcpy_to_i16_from_q4_27(int16_t *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp16(*src++ >> 12);
    }
}

void memcpy_to_i16_from_u8(int16_t *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = (int16_t)(*--src - 0x80) << 8;
    }
}

void memcpy_to_u8_from_i16(uint8_t *dst, const int16_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = (*src++ >> 8) + 0x80;
    }
}

void memcpy_to_u8_from_float(uint8_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp8_from_float(*src++);
    }
}

void memcpy_to_i16_from_i32(int16_t *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = *src++ >> 16;
    }
}

void memcpy_to_i16_from_float(int16_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp16_from_float(*src++);
    }
}

void memcpy_to_float_from_q4_27(float *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = float_from_q4_27(*src++);
    }
}

void memcpy_to_float_from_i16(float *dst, const int16_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = float_from_i16(*--src);
    }
}

void memcpy_to_float_from_u8(float *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = float_from_u8(*--src);
    }
}

void memcpy_to_float_from_p24(float *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count * 3;
    for (; count > 0; --count) {
        src -= 3;
        *--dst = float_from_p24(src);
    }
}

void memcpy_to_i16_from_p24(int16_t *dst, const uint8_t *src, size_t count)
{
    for (; count > 0; --count) {
#if HAVE_BIG_ENDIAN
        *dst++ = src[1] | (src[0] << 8);
#else
        *dst++ = src[1] | (src[2] << 8);
#endif
        src += 3;
    }
}

void memcpy_to_i32_from_p24(int32_t *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count * 3;
    for (; count > 0; --count) {
        src -= 3;
#if HAVE_BIG_ENDIAN
        *--dst = (src[2] << 8) | (src[1] << 16) | (src[0] << 24);
#else
        *--dst = (src[0] << 8) | (src[1] << 16) | (src[2] << 24);
#endif
    }
}

void memcpy_to_p24_from_i16(uint8_t *dst, const int16_t *src, size_t count)
{
    dst += count * 3;
    src += count;
    for (; count > 0; --count) {
        dst -= 3;
        const int16_t sample = *--src;
#if HAVE_BIG_ENDIAN
        dst[0] = sample >> 8;
        dst[1] = sample;
        dst[2] = 0;
#else
        dst[0] = 0;
        dst[1] = sample;
        dst[2] = sample >> 8;
#endif
    }
}

void memcpy_to_p24_from_float(uint8_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        int32_t ival = clamp24_from_float(*src++);

#if HAVE_BIG_ENDIAN
        *dst++ = ival >> 16;
        *dst++ = ival >> 8;
        *dst++ = ival;
#else
        *dst++ = ival;
        *dst++ = ival >> 8;
        *dst++ = ival >> 16;
#endif
    }
}

void memcpy_to_p24_from_q8_23(uint8_t *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        int32_t ival = clamp24_from_q8_23(*src++);

#if HAVE_BIG_ENDIAN
        *dst++ = ival >> 16;
        *dst++ = ival >> 8;
        *dst++ = ival;
#else
        *dst++ = ival;
        *dst++ = ival >> 8;
        *dst++ = ival >> 16;
#endif
    }
}

void memcpy_to_p24_from_i32(uint8_t *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        int32_t ival = *src++ >> 8;

#if HAVE_BIG_ENDIAN
        *dst++ = ival >> 16;
        *dst++ = ival >> 8;
        *dst++ = ival;
#else
        *dst++ = ival;
        *dst++ = ival >> 8;
        *dst++ = ival >> 16;
#endif
    }
}

void memcpy_to_q8_23_from_i16(int32_t *dst, const int16_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = (int32_t)*--src << 8;
    }
}

void memcpy_to_q8_23_from_float_with_clamp(int32_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp24_from_float(*src++);
    }
}

void memcpy_to_q8_23_from_p24(int32_t *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count * 3;
    for (; count > 0; --count) {
        src -= 3;
#if HAVE_BIG_ENDIAN
        *--dst = (int8_t)src[0] << 16 | src[1] << 8 | src[2];
#else
        *--dst = (int8_t)src[2] << 16 | src[1] << 8 | src[0];
#endif
    }
}

void memcpy_to_q4_27_from_float(int32_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clampq4_27_from_float(*src++);
    }
}

void memcpy_to_i16_from_q8_23(int16_t *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp16(*src++ >> 8);
    }
}

void memcpy_to_float_from_q8_23(float *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = float_from_q8_23(*src++);
    }
}

void memcpy_to_i32_from_u8(int32_t *dst, const uint8_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = ((int32_t)(*--src) - 0x80) << 24;
    }
}

void memcpy_to_i32_from_i16(int32_t *dst, const int16_t *src, size_t count)
{
    dst += count;
    src += count;
    for (; count > 0; --count) {
        *--dst = (int32_t)*--src << 16;
    }
}

void memcpy_to_i32_from_float(int32_t *dst, const float *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = clamp32_from_float(*src++);
    }
}

void memcpy_to_float_from_i32(float *dst, const int32_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = float_from_i32(*src++);
    }
}

void memcpy_to_float_from_float_with_clamping(float *dst, const float *src, size_t count,
                                              float absMax) {
    // Note: using NEON intrinsics (vminq_f32, vld1q_f32...) did NOT accelerate
    // the function when benchmarked. The compiler already vectorize using FMINNM f32x4 & similar.
    // Note: clamping induce a ~20% overhead compared to memcpy for count in [64, 512]
    //       See primitives_benchmark
    for (; count > 0; --count) {
        const float sample = *src++;
        *dst++ = fmax(-absMax, fmin(absMax, sample));
    }
}

void downmix_to_mono_i16_from_stereo_i16(int16_t *dst, const int16_t *src, size_t count)
{
    for (; count > 0; --count) {
        *dst++ = (int16_t)(((int32_t)src[0] + (int32_t)src[1]) >> 1);
        src += 2;
    }
}

void upmix_to_stereo_i16_from_mono_i16(int16_t *dst, const int16_t *src, size_t count)
{
    dst += count * 2;
    src += count;
    for (; count > 0; --count) {
        const int32_t temp = *--src;
        dst -= 2;
        dst[0] = temp;
        dst[1] = temp;
    }
}

void downmix_to_mono_float_from_stereo_float(float *dst, const float *src, size_t frames)
{
    for (; frames > 0; --frames) {
        *dst++ = (src[0] + src[1]) * 0.5;
        src += 2;
    }
}

void upmix_to_stereo_float_from_mono_float(float *dst, const float *src, size_t frames)
{
    dst += frames * 2;
    src += frames;
    for (; frames > 0; --frames) {
        const float temp = *--src;
        dst -= 2;
        dst[0] = temp;
        dst[1] = temp;
    }
}

size_t nonZeroMono32(const int32_t *samples, size_t count)
{
    size_t nonZero = 0;
    for (; count > 0; --count) {
        nonZero += *samples++ != 0;
    }
    return nonZero;
}

size_t nonZeroMono16(const int16_t *samples, size_t count)
{
    size_t nonZero = 0;
    for (; count > 0; --count) {
        nonZero += *samples++ != 0;
    }
    return nonZero;
}

size_t nonZeroStereo32(const int32_t *frames, size_t count)
{
    size_t nonZero = 0;
    for (; count > 0; --count) {
        nonZero += frames[0] != 0 || frames[1] != 0;
        frames += 2;
    }
    return nonZero;
}

size_t nonZeroStereo16(const int16_t *frames, size_t count)
{
    size_t nonZero = 0;
    for (; count > 0; --count) {
        nonZero += frames[0] != 0 || frames[1] != 0;
        frames += 2;
    }
    return nonZero;
}

/*
 * C macro to do channel mask copying independent of dst/src sample type.
 * Don't pass in any expressions for the macro arguments here.
 */
#define copy_frame_by_mask(dst, dmask, src, smask, count, zero) \
{ \
    uint32_t bit, ormask; \
    for (; (count) > 0; --(count)) { \
        ormask = (dmask) | (smask); \
        while (ormask) { \
            bit = ormask & -ormask; /* get lowest bit */ \
            ormask ^= bit; /* remove lowest bit */ \
            if ((dmask) & bit) { \
                *(dst)++ = (smask) & bit ? *(src)++ : (zero); \
            } else { /* source channel only */ \
                ++(src); \
            } \
        } \
    } \
}

void memcpy_by_channel_mask(void *dst, uint32_t dst_mask,
        const void *src, uint32_t src_mask, size_t sample_size, size_t count)
{
#if 0
    /* alternate way of handling memcpy_by_channel_mask by using the idxary */
    int8_t idxary[32];
    uint32_t src_channels = popcount(src_mask);
    uint32_t dst_channels =
            memcpy_by_index_array_initialization(idxary, 32, dst_mask, src_mask);

    memcpy_by_idxary(dst, dst_channels, src, src_channels, idxary, sample_size, count);
#else
    if (dst_mask == src_mask) {
        memcpy(dst, src, sample_size * popcount(dst_mask) * count);
        return;
    }
    switch (sample_size) {
    case 1: {
        uint8_t *udst = (uint8_t*)dst;
        const uint8_t *usrc = (const uint8_t*)src;

        copy_frame_by_mask(udst, dst_mask, usrc, src_mask, count, 0);
    } break;
    case 2: {
        uint16_t *udst = (uint16_t*)dst;
        const uint16_t *usrc = (const uint16_t*)src;

        copy_frame_by_mask(udst, dst_mask, usrc, src_mask, count, 0);
    } break;
    case 3: { /* could be slow.  use a struct to represent 3 bytes of data. */
        uint8x3_t *udst = (uint8x3_t*)dst;
        const uint8x3_t *usrc = (const uint8x3_t*)src;
        static const uint8x3_t zero; /* tricky - we use this to zero out a sample */

        copy_frame_by_mask(udst, dst_mask, usrc, src_mask, count, zero);
    } break;
    case 4: {
        uint32_t *udst = (uint32_t*)dst;
        const uint32_t *usrc = (const uint32_t*)src;

        copy_frame_by_mask(udst, dst_mask, usrc, src_mask, count, 0);
    } break;
    default:
        abort(); /* illegal value */
        break;
    }
#endif
}

/*
 * C macro to do copying by index array, to rearrange samples
 * within a frame.  This is independent of src/dst sample type.
 * Don't pass in any expressions for the macro arguments here.
 */
#define copy_frame_by_idx(dst, dst_channels, src, src_channels, idxary, count, zero) \
{ \
    unsigned i; \
    int index; \
    for (; (count) > 0; --(count)) { \
        for (i = 0; i < (dst_channels); ++i) { \
            index = (idxary)[i]; \
            *(dst)++ = index < 0 ? (zero) : (src)[index]; \
        } \
        (src) += (src_channels); \
    } \
}

void memcpy_by_index_array(void *dst, uint32_t dst_channels,
        const void *src, uint32_t src_channels,
        const int8_t *idxary, size_t sample_size, size_t count)
{
    switch (sample_size) {
    case 1: {
        uint8_t *udst = (uint8_t*)dst;
        const uint8_t *usrc = (const uint8_t*)src;

        copy_frame_by_idx(udst, dst_channels, usrc, src_channels, idxary, count, 0);
    } break;
    case 2: {
        uint16_t *udst = (uint16_t*)dst;
        const uint16_t *usrc = (const uint16_t*)src;

        copy_frame_by_idx(udst, dst_channels, usrc, src_channels, idxary, count, 0);
    } break;
    case 3: { /* could be slow.  use a struct to represent 3 bytes of data. */
        uint8x3_t *udst = (uint8x3_t*)dst;
        const uint8x3_t *usrc = (const uint8x3_t*)src;
        static const uint8x3_t zero;

        copy_frame_by_idx(udst, dst_channels, usrc, src_channels, idxary, count, zero);
    } break;
    case 4: {
        uint32_t *udst = (uint32_t*)dst;
        const uint32_t *usrc = (const uint32_t*)src;

        copy_frame_by_idx(udst, dst_channels, usrc, src_channels, idxary, count, 0);
    } break;
    default:
        abort(); /* illegal value */
        break;
    }
}

size_t memcpy_by_index_array_initialization(int8_t *idxary, size_t idxcount,
        uint32_t dst_mask, uint32_t src_mask)
{
    size_t n = 0;
    int srcidx = 0;
    uint32_t bit, ormask = src_mask | dst_mask;

    while (ormask && n < idxcount) {
        bit = ormask & -ormask;          /* get lowest bit */
        ormask ^= bit;                   /* remove lowest bit */
        if (src_mask & dst_mask & bit) { /* matching channel */
            idxary[n++] = srcidx++;
        } else if (src_mask & bit) {     /* source channel only */
            ++srcidx;
        } else {                         /* destination channel only */
            idxary[n++] = -1;
        }
    }
    return n + popcount(ormask & dst_mask);
}

size_t memcpy_by_index_array_initialization_src_index(int8_t *idxary, size_t idxcount,
        uint32_t dst_mask, uint32_t src_mask) {
    size_t dst_count = popcount(dst_mask);
    if (idxcount == 0) {
        return dst_count;
    }
    if (dst_count > idxcount) {
        dst_count = idxcount;
    }

    size_t src_idx, dst_idx;
    for (src_idx = 0, dst_idx = 0; dst_idx < dst_count; ++dst_idx) {
        if (src_mask & 1) {
            idxary[dst_idx] = src_idx++;
        } else {
            idxary[dst_idx] = -1;
        }
        src_mask >>= 1;
    }
    return dst_idx;
}

size_t memcpy_by_index_array_initialization_dst_index(int8_t *idxary, size_t idxcount,
        uint32_t dst_mask, uint32_t src_mask) {
    size_t src_idx, dst_idx;
    size_t dst_count = __builtin_popcount(dst_mask);
    size_t src_count = __builtin_popcount(src_mask);
    if (idxcount == 0) {
        return dst_count;
    }
    if (dst_count > idxcount) {
        dst_count = idxcount;
    }
    for (src_idx = 0, dst_idx = 0; dst_idx < dst_count; ++src_idx) {
        if (dst_mask & 1) {
            idxary[dst_idx++] = src_idx < src_count ? (signed)src_idx : -1;
        }
        dst_mask >>= 1;
    }
    return dst_idx;
}

void accumulate_i16(int16_t *dst, const int16_t *src, size_t count) {
    while (count--) {
        *dst = clamp16((int32_t)*dst + *src++);
        ++dst;
    }
}

void accumulate_u8(uint8_t *dst, const uint8_t *src, size_t count) {
    int32_t sum;
    for (; count > 0; --count) {
        // 8-bit samples are centered around 0x80.
        sum = *dst + *src++ - 0x80;
        // Clamp to [0, 0xff].
        *dst++ = (sum & 0x100) ? (~sum >> 9) : sum;
    }
}

void accumulate_p24(uint8_t *dst, const uint8_t *src, size_t count) {
    for (; count > 0; --count) {
        // Unpack.
        int32_t dst_q8_23 = 0;
        int32_t src_q8_23 = 0;
        memcpy_to_q8_23_from_p24(&dst_q8_23, dst, 1);
        memcpy_to_q8_23_from_p24(&src_q8_23, src, 1);

        // Accumulate and overwrite.
        dst_q8_23 += src_q8_23;
        memcpy_to_p24_from_q8_23(dst, &dst_q8_23, 1);

        // Move on to next sample.
        dst += 3;
        src += 3;
  }
}

void accumulate_q8_23(int32_t *dst, const int32_t *src, size_t count) {
    for (; count > 0; --count) {
        *dst = clamp24_from_q8_23(*dst + *src++);
        ++dst;
    }
}

void accumulate_i32(int32_t *dst, const int32_t *src, size_t count) {
    for (; count > 0; --count) {
        *dst = clamp32((int64_t)*dst + *src++);
        ++dst;
    }
}

void accumulate_float(float *dst, const float *src, size_t count) {
    for (; count > 0; --count) {
        *dst++ += *src++;
    }
}
