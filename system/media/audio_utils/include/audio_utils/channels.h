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

#ifndef ANDROID_AUDIO_CHANNELS_H
#define ANDROID_AUDIO_CHANNELS_H

/** \cond */
__BEGIN_DECLS
/** \endcond */

/**
 * Expands or contracts sample data from one interleaved channel format to another.
 * Expanded channels are filled with zeros and put at the end of each audio frame.
 * Contracted channels are omitted from the end of each audio frame.
 *
 *   \param in_buff              points to the buffer of samples
 *   \param in_buff_chans        Specifies the number of channels in the input buffer.
 *   \param out_buff             points to the buffer to receive converted samples.
 *   \param out_buff_chans       Specifies the number of channels in the output buffer.
 *   \param sample_size_in_bytes Specifies the number of bytes per sample. 1, 2, 3, 4 are
 *     currently valid.
 *   \param num_in_bytes         size of input buffer in bytes
 *
 * \return
 *   the number of bytes of output data or 0 if an error occurs.
 *
 * \note
 *   The out and sums buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
size_t adjust_channels(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes);

/**
 * Expands or contracts sample data from one interleaved channel format to another.
 * Extra expanded channels are left alone in the output buffer.
 * Contracted channels are omitted from the end of each audio frame.
 *
 *   \param in_buff              points to the buffer of samples
 *   \param in_buff_chans        Specifies the number of channels in the input buffer.
 *   \param out_buff             points to the buffer to receive converted samples.
 *   \param out_buff_chans       Specifies the number of channels in the output buffer.
 *   \param sample_size_in_bytes Specifies the number of bytes per sample. 1, 2, 3, 4 are
 *     currently valid.
 *   \param num_in_bytes         size of input buffer in bytes
 *
 * \return
 *   the number of bytes of output data or 0 if an error occurs.
 *
 * \note
 *   The out and in buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
size_t adjust_selected_channels(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes);

/**
 * Expands or contracts sample data from one interleaved channel format to another.
 * Extra expanded channels are interleaved in from the end of the input buffer.
 * Contracted channels are copied to the end of the output buffer.
 *
 *   \param in_buff              points to the buffer of samples.
 *   \param in_buff_chans        Specifies the number of channels in the input buffer.
 *   \param out_buff             points to the buffer to receive converted samples.
 *   \param out_buff_chans       Specifies the number of channels in the output buffer.
 *   \param sample_size_in_bytes Specifies the number of bytes per sample. 1, 2, 3, 4 are
 *     currently valid.
 *   \param num_in_bytes         size of input buffer in bytes.
 *
 * \return
 *   the number of bytes of output data or 0 if an error occurs.
 *
 * \note
 *   The out and in buffers must be the same length.
 *   The out and in buffers must either be completely separate (non-overlapping), or
 *   they must both start at the same address. Partially overlapping buffers are not supported.
 */
size_t adjust_channels_non_destructive(const void* in_buff, size_t in_buff_chans,
                       void* out_buff, size_t out_buff_chans,
                       unsigned sample_size_in_bytes, size_t num_in_bytes);

/** \cond */
__END_DECLS
/** \endcond */

#endif  // !ANDROID_AUDIO_CHANNELS_H
