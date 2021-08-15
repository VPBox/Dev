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

#ifndef ANDROID_AUDIO_FORMAT_H
#define ANDROID_AUDIO_FORMAT_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <system/audio.h>

/** \cond */
__BEGIN_DECLS
/** \endcond */

/**
 * Copy buffers with conversion between buffer sample formats.
 *
 *  \param dst        Destination buffer
 *  \param dst_format Destination buffer format
 *  \param src        Source buffer
 *  \param src_format Source buffer format
 *  \param count      Number of samples to copy
 *
 * Allowed format conversions are given by either case 1 or 2 below:
 *
 * 1) One of src_format or dst_format is AUDIO_FORMAT_PCM_16_BIT or
 * AUDIO_FORMAT_PCM_FLOAT, and the other format type is one of:
 *
 * AUDIO_FORMAT_PCM_16_BIT
 * <BR>
 * AUDIO_FORMAT_PCM_FLOAT
 * <BR>
 * AUDIO_FORMAT_PCM_8_BIT
 * <BR>
 * AUDIO_FORMAT_PCM_24_BIT_PACKED
 * <BR>
 * AUDIO_FORMAT_PCM_32_BIT
 * <BR>
 * AUDIO_FORMAT_PCM_8_24_BIT
 *
 * 2) Both dst_format and src_format are identical and of the list given
 * in (1). This is a straight copy.
 *
 * The destination and source buffers must be completely separate
 * or point to the same starting buffer address. These routines call functions
 * in primitives.h, so descriptions of detailed behavior can be reviewed there.
 *
 * Logs a fatal error if dst or src format is not allowed by the conversion rules above.
 */
void memcpy_by_audio_format(void *dst, audio_format_t dst_format,
        const void *src, audio_format_t src_format, size_t count);


/**
 * This function creates an index array for converting audio data with different
 * channel position and index masks, used by memcpy_by_index_array().
 *
 * Note that idxary is a caller allocated array
 * of at least as many channels as present in the dst_mask.
 *
 * Parameters:
 *  \param idxary            Updated array of indices of channels in the src frame for the dst frame
 *  \param arysize           Number of caller allocated elements in idxary
 *  \param dst_channel_mask  Bit mask corresponding to destination channels present
 *  \param src_channel_mask  Bit mask corresponding to source channels present
 *
 * \return the number of array elements required.
 * This may be greater than idxcount, so the return value should be checked
 * if idxary size is less than 32. Returns zero if the input masks are unrecognized.
 */
size_t memcpy_by_index_array_initialization_from_channel_mask(int8_t *idxary, size_t arysize,
        audio_channel_mask_t dst_channel_mask, audio_channel_mask_t src_channel_mask);

/** \cond */
__END_DECLS
/** \endcond */

#endif  // ANDROID_AUDIO_FORMAT_H
