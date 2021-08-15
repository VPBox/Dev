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

#include <string.h>
#include <audio_utils/channels.h>
#include "private/private.h"

/*
 * Clamps a 24-bit value from a 32-bit sample
 */
static inline int32_t clamp24(int32_t sample)
{
    if ((sample>>23) ^ (sample>>31)) {
        sample = 0x007FFFFF ^ (sample>>31);
    }
    return sample;
}

/*
 * Converts a uint8x3_t into an int32_t
 */
static inline int32_t uint8x3_to_int32(uint8x3_t val) {
#if HAVE_BIG_ENDIAN
    int32_t temp = (val.c[0] << 24 | val.c[1] << 16 | val.c[2] << 8) >> 8;
#else
    int32_t temp = (val.c[2] << 24 | val.c[1] << 16 | val.c[0] << 8) >> 8;
#endif
    return clamp24(temp);
}

/*
 * Converts an int32_t to a uint8x3_t
 */
static inline uint8x3_t int32_to_uint8x3(int32_t in) {
    uint8x3_t out;
#if HAVE_BIG_ENDIAN
    out.c[2] = in;
    out.c[1] = in >> 8;
    out.c[0] = in >> 16;
#else
    out.c[0] = in;
    out.c[1] = in >> 8;
    out.c[2] = in >> 16;
#endif
    return out;
}

/* This is written as a C macro because it operates on generic types,
 * which in a C++ file could be alternatively achieved by a "template"
 * or an "auto" declaration.
 * TODO: convert this from a C file to a C++ file.
 */

/* Channel expands (adds zeroes to audio frame end) from an input buffer to an output buffer.
 * See expand_channels() function below for parameter definitions.
 *
 * Move from back to front so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Macro has a return statement.
 */
#define EXPAND_CHANNELS(in_buff, in_buff_chans, out_buff, out_buff_chans, num_in_bytes, zero) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    typeof(out_buff) dst_ptr = (out_buff) + num_out_samples - 1; \
    size_t src_index; \
    typeof(in_buff) src_ptr = (in_buff) + num_in_samples - 1; \
    size_t num_zero_chans = (out_buff_chans) - (in_buff_chans); \
    for (src_index = 0; src_index < num_in_samples; src_index += (in_buff_chans)) { \
        size_t dst_offset; \
        for (dst_offset = 0; dst_offset < num_zero_chans; dst_offset++) { \
            *dst_ptr-- = zero; \
        } \
        for (; dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr-- = *src_ptr--; \
        } \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Channel expands from an input buffer to an output buffer.
 * See expand_selected_channels() function below for parameter definitions.
 * Selected channels are replaced in the output buffer, with any extra channels
 * per frame left alone.
 *
 * Move from back to front so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Macro has a return statement.
 */
#define EXPAND_SELECTED_CHANNELS( \
        in_buff, in_buff_chans, out_buff, out_buff_chans, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    typeof(out_buff) dst_ptr = (out_buff) + num_out_samples - 1; \
    size_t src_index; \
    typeof(in_buff) src_ptr = (in_buff) + num_in_samples - 1; \
    size_t num_extra_chans = (out_buff_chans) - (in_buff_chans); \
    for (src_index = 0; src_index < num_in_samples; src_index += (in_buff_chans)) { \
        dst_ptr -= num_extra_chans; \
        for (size_t dst_offset = num_extra_chans; dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr-- = *src_ptr--; \
        } \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Expand number of channels from an input buffer to an output buffer.
 * See expand_channels_non_destructive() function below for parameter definitions.
 *
 * Input channels are copied to the output buffer, with extra output
 * channels interleaved from back of input buffer.
 *
 * So for in_chans = 2, out_chans = 4: [1|2|1|2...|3|4|3|4] => [1|2|3|4|1|2|3|4...]
 *
 * NOTE: in_buff must be same size as out_buff and num_in_bytes must
 * be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Uses a temporary buffer so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 *
 * Macro has a return statement.
 */
#define EXPAND_CHANNELS_NON_DESTRUCTIVE(in_buff, in_buff_chans, \
        out_buff, out_buff_chans, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    typeof(out_buff) dst_ptr = (out_buff); \
    typeof(in_buff) src_ptr = (in_buff); \
    typeof(*out_buff) temp_buff[num_in_samples]; \
    typeof(out_buff) temp_ptr = temp_buff; \
    /* if in-place, copy input channels to a temp buffer */ \
    if ((in_buff) == (out_buff)) { \
        memcpy(temp_buff, src_ptr, (num_in_bytes)); \
        src_ptr += num_in_samples; \
    } else { \
        temp_ptr = (typeof(out_buff)) src_ptr; \
        src_ptr += num_in_samples; \
    } \
    /* interleave channels from end of source buffer with those from front */ \
    size_t src_index; \
    for (src_index = 0; src_index < num_out_samples; src_index += (out_buff_chans)) { \
        size_t dst_offset; \
        for (dst_offset = 0; dst_offset < (in_buff_chans); dst_offset++) { \
            *dst_ptr++ = *temp_ptr++; \
        } \
        for (;dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr++ = *src_ptr++; \
        } \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Channel expands from a MONO input buffer to a MULTICHANNEL output buffer by duplicating the
 * single input channel to the first 2 output channels and 0-filling the remaining.
 * See expand_channels() function below for parameter definitions.
 *
 * in_buff_chans MUST be 1 and out_buff_chans MUST be >= 2
 *
 * Move from back to front so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Macro has a return statement.
 */
#define EXPAND_MONO_TO_MULTI(in_buff, in_buff_chans, out_buff, out_buff_chans, num_in_bytes, zero) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    typeof(out_buff) dst_ptr = (out_buff) + num_out_samples - 1; \
    size_t src_index; \
    typeof(in_buff) src_ptr = (in_buff) + num_in_samples - 1; \
    size_t num_zero_chans = (out_buff_chans) - (in_buff_chans) - 1; \
    for (src_index = 0; src_index < num_in_samples; src_index += (in_buff_chans)) { \
        size_t dst_offset; \
        for (dst_offset = 0; dst_offset < num_zero_chans; dst_offset++) { \
            *dst_ptr-- = zero; \
        } \
        for (; dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr-- = *src_ptr; \
        } \
        src_ptr--; \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Channel contracts (removes from audio frame end) from an input buffer to an output buffer.
 * See contract_channels() function below for parameter definitions.
 *
 * Move from front to back so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Macro has a return statement.
 */
#define CONTRACT_CHANNELS(in_buff, in_buff_chans, out_buff, out_buff_chans, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    size_t num_skip_samples = (in_buff_chans) - (out_buff_chans); \
    typeof(out_buff) dst_ptr = out_buff; \
    typeof(in_buff) src_ptr = in_buff; \
    size_t src_index; \
    for (src_index = 0; src_index < num_in_samples; src_index += (in_buff_chans)) { \
        size_t dst_offset; \
        for (dst_offset = 0; dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr++ = *src_ptr++; \
        } \
        src_ptr += num_skip_samples; \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Contract number of channels from an input buffer to an output buffer,
 * storing removed channels at end of buffer.
 *
 * See contract_channels_non_destructive() function below for parameter definitions.
 *
 * So for in_chans = 4, out_chans = 2: [1|2|3|4|1|2|3|4...] => [1|2|1|2...|3|4|3|4]
 *
 * NOTE: in_buff must be same size as out_buff and num_in_bytes must
 * be a multiple of in_buff_channels * in_buff_sample_size.
 *
 * Uses a temporary buffer so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 *
 * Macro has a return statement.
 */
#define CONTRACT_CHANNELS_NON_DESTRUCTIVE(in_buff, in_buff_chans, out_buff, \
        out_buff_chans, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * (out_buff_chans)) / (in_buff_chans); \
    typeof(out_buff) dst_ptr = (out_buff); \
    typeof(in_buff) src_ptr = (in_buff); \
    size_t num_temp_samples = num_in_samples - num_out_samples; \
    typeof(*out_buff) temp_buff[num_temp_samples]; \
    typeof(out_buff) temp_ptr; \
    /* if in-place, copy input channels to a temp buffer instead of out buffer */ \
    if ((in_buff) == (out_buff)) { \
        temp_ptr = temp_buff; \
    } else { \
        temp_ptr = dst_ptr + num_out_samples; \
    } \
    size_t src_index; \
    for (src_index = 0; src_index < num_in_samples; src_index += (in_buff_chans)) { \
        size_t dst_offset; \
        for (dst_offset = 0; dst_offset < (out_buff_chans); dst_offset++) { \
            *dst_ptr++ = *src_ptr++; \
        } \
        for (;dst_offset < (in_buff_chans); dst_offset++) { \
            *temp_ptr++ = *src_ptr++; \
        } \
    } \
    /* if in-place, interleave channels from the temp buffer */ \
    if ((in_buff) == (out_buff)) { \
        temp_ptr = temp_buff; \
        memcpy(dst_ptr, temp_ptr, num_temp_samples * sizeof(*(in_buff))); \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Channel contracts from a MULTICHANNEL input buffer to a MONO output buffer by mixing the
 * first two input channels into the single output channel (and skipping the rest).
 * See contract_channels() function below for parameter definitions.
 *
 * in_buff_chans MUST be >= 2 and out_buff_chans MUST be 1
 *
 * Move from front to back so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 * NOTE: Overload of the summed channels is avoided by averaging the two input channels.
 * NOTE: Can not be used for uint8x3_t samples, see CONTRACT_TO_MONO_24() below.
 *
 * Macro has a return statement.
 */
#define CONTRACT_TO_MONO(in_buff, out_buff, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * out_buff_chans) / in_buff_chans; \
    size_t num_skip_samples = in_buff_chans - 2; \
    typeof(out_buff) dst_ptr = out_buff; \
    typeof(in_buff) src_ptr = in_buff; \
    int32_t temp0, temp1; \
    size_t src_index; \
    for (src_index = 0; src_index < num_in_samples; src_index += in_buff_chans) { \
        temp0 = *src_ptr++; \
        temp1 = *src_ptr++; \
        /* *dst_ptr++ = temp >> 1; */ \
        /* This bit of magic adds and normalizes without overflow (or so claims hunga@) */ \
        /* Bitwise half adder trick, see http://en.wikipedia.org/wiki/Adder_(electronics) */ \
        /* Hacker's delight, p. 19 http://www.hackersdelight.org/basics2.pdf */ \
        *dst_ptr++ = (temp0 & temp1) + ((temp0 ^ temp1) >> 1); \
        src_ptr += num_skip_samples; \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/* Channel contracts from a MULTICHANNEL uint8x3_t input buffer to a MONO uint8x3_t output buffer
 * by mixing the first two input channels into the single output channel (and skipping the rest).
 * See contract_channels() function below for parameter definitions.
 *
 * Move from front to back so that the conversion can be done in-place
 * i.e. in_buff == out_buff
 * NOTE: num_in_bytes must be a multiple of in_buff_channels * in_buff_sample_size.
 * NOTE: Overload of the summed channels is avoided by averaging the two input channels.
 * NOTE: Can not be used for normal, scalar samples, see CONTRACT_TO_MONO() above.
 *
 * Macro has a return statement.
 */
#define CONTRACT_TO_MONO_24(in_buff, out_buff, num_in_bytes) \
{ \
    size_t num_in_samples = (num_in_bytes) / sizeof(*(in_buff)); \
    size_t num_out_samples = (num_in_samples * out_buff_chans) / in_buff_chans; \
    size_t num_skip_samples = in_buff_chans - 2; \
    typeof(out_buff) dst_ptr = out_buff; \
    typeof(in_buff) src_ptr = in_buff; \
    int32_t temp; \
    size_t src_index; \
    for (src_index = 0; src_index < num_in_samples; src_index += in_buff_chans) { \
        temp = uint8x3_to_int32(*src_ptr++); \
        temp += uint8x3_to_int32(*src_ptr++); \
        *dst_ptr = int32_to_uint8x3(temp >> 1); \
        src_ptr += num_skip_samples; \
    } \
    /* return number of *bytes* generated */ \
    return num_out_samples * sizeof(*(out_buff)); \
}

/*
 * Convert a buffer of N-channel, interleaved samples to M-channel
 * (where N > M).
 *   in_buff points to the buffer of samples
 *   in_buff_channels Specifies the number of channels in the input buffer.
 *   out_buff points to the buffer to receive converted samples.
 *   out_buff_channels Specifies the number of channels in the output buffer.
 *   sample_size_in_bytes Specifies the number of bytes per sample.
 *   num_in_bytes size of input buffer in BYTES
 * returns
 *   the number of BYTES of output data.
 * NOTE
 *   channels > M are thrown away.
 *   The out and sums buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
static size_t contract_channels(const void* in_buff, size_t in_buff_chans,
                                void* out_buff, size_t out_buff_chans,
                                unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    switch (sample_size_in_bytes) {
    case 1:
        if (out_buff_chans == 1) {
            /* Special case Multi to Mono */
            CONTRACT_TO_MONO((const uint8_t*)in_buff, (uint8_t*)out_buff, num_in_bytes);
            // returns in macro
        } else {
            CONTRACT_CHANNELS((const uint8_t*)in_buff, in_buff_chans,
                              (uint8_t*)out_buff, out_buff_chans,
                              num_in_bytes);
            // returns in macro
        }
    case 2:
        if (out_buff_chans == 1) {
            /* Special case Multi to Mono */
            CONTRACT_TO_MONO((const int16_t*)in_buff, (int16_t*)out_buff, num_in_bytes);
            // returns in macro
        } else {
            CONTRACT_CHANNELS((const int16_t*)in_buff, in_buff_chans,
                              (int16_t*)out_buff, out_buff_chans,
                              num_in_bytes);
            // returns in macro
        }
    case 3:
        if (out_buff_chans == 1) {
            /* Special case Multi to Mono */
            CONTRACT_TO_MONO_24((const uint8x3_t*)in_buff,
                                       (uint8x3_t*)out_buff, num_in_bytes);
            // returns in macro
        } else {
            CONTRACT_CHANNELS((const uint8x3_t*)in_buff, in_buff_chans,
                              (uint8x3_t*)out_buff, out_buff_chans,
                              num_in_bytes);
            // returns in macro
        }
    case 4:
        if (out_buff_chans == 1) {
            /* Special case Multi to Mono */
            CONTRACT_TO_MONO((const int32_t*)in_buff, (int32_t*)out_buff, num_in_bytes);
            // returns in macro
        } else {
            CONTRACT_CHANNELS((const int32_t*)in_buff, in_buff_chans,
                              (int32_t*)out_buff, out_buff_chans,
                              num_in_bytes);
            // returns in macro
        }
    default:
        return 0;
    }
}

/*
 * Convert a buffer of N-channel, interleaved samples to M-channel
 * (where N > M).
 *   in_buff points to the buffer of samples
 *   in_buff_channels specifies the number of channels in the input buffer.
 *   out_buff points to the buffer to receive converted samples.
 *   out_buff_channels specifies the number of channels in the output buffer.
 *   sample_size_in_bytes specifies the number of bytes per sample.
 *   num_in_bytes size of input buffer in BYTES
 * returns
 *   the number of BYTES of output data.
 * NOTE
 *   channels > M are stored at the end of the output buffer.
 *   The output and input buffers must be the same length.
 *   The output and input buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
static size_t contract_channels_non_destructive(const void* in_buff, size_t in_buff_chans,
                                void* out_buff, size_t out_buff_chans,
                                unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    switch (sample_size_in_bytes) {
    case 1:
        CONTRACT_CHANNELS_NON_DESTRUCTIVE((const uint8_t*)in_buff, in_buff_chans,
                          (uint8_t*)out_buff, out_buff_chans,
                          num_in_bytes);
        // returns in macro
    case 2:
        CONTRACT_CHANNELS_NON_DESTRUCTIVE((const int16_t*)in_buff, in_buff_chans,
                          (int16_t*)out_buff, out_buff_chans,
                          num_in_bytes);
        // returns in macro
    case 3:
        CONTRACT_CHANNELS_NON_DESTRUCTIVE((const uint8x3_t*)in_buff, in_buff_chans,
                          (uint8x3_t*)out_buff, out_buff_chans,
                          num_in_bytes);
        // returns in macro
    case 4:
        CONTRACT_CHANNELS_NON_DESTRUCTIVE((const int32_t*)in_buff, in_buff_chans,
                          (int32_t*)out_buff, out_buff_chans,
                          num_in_bytes);
        // returns in macro
    default:
        return 0;
    }
}

/*
 * Convert a buffer of N-channel, interleaved samples to M-channel
 * (where N < M).
 *   in_buff points to the buffer of samples
 *   in_buff_channels Specifies the number of channels in the input buffer.
 *   out_buff points to the buffer to receive converted samples.
 *   out_buff_channels Specifies the number of channels in the output buffer.
 *   sample_size_in_bytes Specifies the number of bytes per sample.
 *   num_in_bytes size of input buffer in BYTES
 * returns
 *   the number of BYTES of output data.
 * NOTE
 *   channels > N are filled with silence.
 *   The out and sums buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
static size_t expand_channels(const void* in_buff, size_t in_buff_chans,
                              void* out_buff, size_t out_buff_chans,
                              unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    static const uint8x3_t packed24_zero; /* zero 24 bit sample */

    switch (sample_size_in_bytes) {
    case 1:
        if (in_buff_chans == 1) {
            /* special case of mono source to multi-channel */
            EXPAND_MONO_TO_MULTI((const uint8_t*)in_buff, in_buff_chans,
                            (uint8_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        } else {
            EXPAND_CHANNELS((const uint8_t*)in_buff, in_buff_chans,
                            (uint8_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        }
    case 2:
        if (in_buff_chans == 1) {
            /* special case of mono source to multi-channel */
            EXPAND_MONO_TO_MULTI((const int16_t*)in_buff, in_buff_chans,
                            (int16_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        } else {
            EXPAND_CHANNELS((const int16_t*)in_buff, in_buff_chans,
                            (int16_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        }
    case 3:
        if (in_buff_chans == 1) {
            /* special case of mono source to multi-channel */
            EXPAND_MONO_TO_MULTI((const uint8x3_t*)in_buff, in_buff_chans,
                            (uint8x3_t*)out_buff, out_buff_chans,
                            num_in_bytes, packed24_zero);
            // returns in macro
        } else {
            EXPAND_CHANNELS((const uint8x3_t*)in_buff, in_buff_chans,
                            (uint8x3_t*)out_buff, out_buff_chans,
                            num_in_bytes, packed24_zero);
            // returns in macro
        }
    case 4:
        if (in_buff_chans == 1) {
            /* special case of mono source to multi-channel */
            EXPAND_MONO_TO_MULTI((const int32_t*)in_buff, in_buff_chans,
                            (int32_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        } else {
           EXPAND_CHANNELS((const int32_t*)in_buff, in_buff_chans,
                            (int32_t*)out_buff, out_buff_chans,
                            num_in_bytes, 0);
            // returns in macro
        }
    default:
        return 0;
    }
}

/*
 * Convert a buffer of N-channel, interleaved samples to M-channel
 * (where N < M).
 *   in_buff points to the buffer of samples
 *   in_buff_channels Specifies the number of channels in the input buffer.
 *   out_buff points to the buffer to receive converted samples.
 *   out_buff_channels Specifies the number of channels in the output buffer.
 *   sample_size_in_bytes Specifies the number of bytes per sample.
 *   num_in_bytes size of input buffer in BYTES
 * returns
 *   the number of BYTES of output data.
 * NOTE
 *   channels > N are left alone in out_buff.
 *   The out and in buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
static size_t expand_selected_channels(const void* in_buff, size_t in_buff_chans,
                              void* out_buff, size_t out_buff_chans,
                              unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    switch (sample_size_in_bytes) {
    case 1:

        EXPAND_SELECTED_CHANNELS((const uint8_t*)in_buff, in_buff_chans,
                        (uint8_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 2:

        EXPAND_SELECTED_CHANNELS((const int16_t*)in_buff, in_buff_chans,
                        (int16_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 3:

        EXPAND_SELECTED_CHANNELS((const uint8x3_t*)in_buff, in_buff_chans,
                        (uint8x3_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 4:

        EXPAND_SELECTED_CHANNELS((const int32_t*)in_buff, in_buff_chans,
                        (int32_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    default:
        return 0;
    }
}

/*
 * Convert a buffer of N-channel, interleaved samples to M-channel
 * (where N < M).
 *   in_buff points to the buffer of samples
 *   in_buff_channels Specifies the number of channels in the input buffer.
 *   out_buff points to the buffer to receive converted samples.
 *   out_buff_channels Specifies the number of channels in the output buffer.
 *   sample_size_in_bytes Specifies the number of bytes per sample.
 *   num_in_bytes size of input buffer in BYTES
 * returns
 *   the number of BYTES of output data.
 * NOTE
 *   channels > N are interleaved with data from the end of the input buffer.
 *   The output and input buffers must be the same length.
 *   The output and input buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
static size_t expand_channels_non_destructive(const void* in_buff, size_t in_buff_chans,
                              void* out_buff, size_t out_buff_chans,
                              unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    switch (sample_size_in_bytes) {
    case 1:

        EXPAND_CHANNELS_NON_DESTRUCTIVE((const uint8_t*)in_buff, in_buff_chans,
                        (uint8_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 2:

        EXPAND_CHANNELS_NON_DESTRUCTIVE((const int16_t*)in_buff, in_buff_chans,
                        (int16_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 3:

        EXPAND_CHANNELS_NON_DESTRUCTIVE((const uint8x3_t*)in_buff, in_buff_chans,
                        (uint8x3_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    case 4:

        EXPAND_CHANNELS_NON_DESTRUCTIVE((const int32_t*)in_buff, in_buff_chans,
                        (int32_t*)out_buff, out_buff_chans,
                        num_in_bytes);
        // returns in macro

    default:
        return 0;
    }
}

size_t adjust_channels(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    if (out_buff_chans > in_buff_chans) {
        return expand_channels(in_buff, in_buff_chans, out_buff,  out_buff_chans,
                               sample_size_in_bytes, num_in_bytes);
    } else if (out_buff_chans < in_buff_chans) {
        return contract_channels(in_buff, in_buff_chans, out_buff,  out_buff_chans,
                                 sample_size_in_bytes, num_in_bytes);
    } else if (in_buff != out_buff) {
        memcpy(out_buff, in_buff, num_in_bytes);
    }

    return num_in_bytes;
}

size_t adjust_selected_channels(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    if (out_buff_chans > in_buff_chans) {
        return expand_selected_channels(in_buff, in_buff_chans, out_buff, out_buff_chans,
                               sample_size_in_bytes, num_in_bytes);
    } else if (out_buff_chans < in_buff_chans) {
        return contract_channels(in_buff, in_buff_chans, out_buff, out_buff_chans,
                                 sample_size_in_bytes, num_in_bytes);
    } else if (in_buff != out_buff) {
        memcpy(out_buff, in_buff, num_in_bytes);
    }

    return num_in_bytes;
}

size_t adjust_channels_non_destructive(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes)
{
    if (out_buff_chans > in_buff_chans) {
        return expand_channels_non_destructive(in_buff, in_buff_chans, out_buff, out_buff_chans,
                               sample_size_in_bytes, num_in_bytes);
    } else if (out_buff_chans < in_buff_chans) {
        return contract_channels_non_destructive(in_buff, in_buff_chans, out_buff, out_buff_chans,
                                 sample_size_in_bytes, num_in_bytes);
    } else if (in_buff != out_buff) {
        memcpy(out_buff, in_buff, num_in_bytes);
    }

    return num_in_bytes;
}
