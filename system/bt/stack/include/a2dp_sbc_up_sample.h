/******************************************************************************
 *
 *  Copyright 2004-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  This is the interface to utility functions for dealing with SBC data
 *  frames and codec capabilities.
 *
 ******************************************************************************/
#ifndef A2DP_SBC_UP_SAMPLE_H
#define A2DP_SBC_UP_SAMPLE_H

#include <stdint.h>

/*******************************************************************************
 *
 * Function         a2dp_sbc_init_up_sample
 *
 * Description      initialize the up sample
 *
 *                  src_sps: samples per second (source audio data)
 *                  dst_sps: samples per second (converted audio data)
 *                  bits: number of bits per pcm sample
 *                  n_channels: number of channels (i.e. mono(1), stereo(2)...)
 *
 * Returns          none
 *
 ******************************************************************************/
void a2dp_sbc_init_up_sample(uint32_t src_sps, uint32_t dst_sps, uint8_t bits,
                             uint8_t n_channels);

/*******************************************************************************
 *
 * Function         a2dp_sbc_up_sample
 *
 * Description      Given the source (p_src) audio data and
 *                  source speed (src_sps, samples per second),
 *                  This function converts it to audio data in the desired
 *                  format
 *
 *                  p_src: the data buffer that holds the source audio data
 *                  p_dst: the data buffer to hold the converted audio data
 *                  src_samples: The number of source samples (number of bytes)
 *                  dst_samples: The size of p_dst (number of bytes)
 *
 * Note:            An AE reported an issue with this function.
 *                  When called with a2dp_sbc_up_sample(src, uint8_array_dst..)
 *                  the byte before uint8_array_dst may get overwritten.
 *                  Using uint16_array_dst avoids the problem.
 *                  This issue is related to endian-ness and is hard to resolve
 *                  in a generic manner.
 * **************** Please use uint16 array as dst.
 *
 * Returns          The number of bytes used in p_dst
 *                  The number of bytes used in p_src (in *p_ret)
 *
 ******************************************************************************/
int a2dp_sbc_up_sample(void* p_src, void* p_dst, uint32_t src_samples,
                       uint32_t dst_samples, uint32_t* p_ret);

/*******************************************************************************
 *
 * Function         a2dp_sbc_up_sample_16s (16bits-stereo)
 *
 * Description      Given the source (p_src) audio data and
 *                  source speed (src_sps, samples per second),
 *                  This function converts it to audio data in the desired
 *                  format
 *
 *                  p_src: the data buffer that holds the source audio data
 *                  p_dst: the data buffer to hold the converted audio data
 *                  src_samples: The number of source samples (in uint of 4
 *                               bytes)
 *                  dst_samples: The size of p_dst (in uint of 4 bytes)
 *
 * Returns          The number of bytes used in p_dst
 *                  The number of bytes used in p_src (in *p_ret)
 *
 ******************************************************************************/
int a2dp_sbc_up_sample_16s(void* p_src, void* p_dst, uint32_t src_samples,
                           uint32_t dst_samples, uint32_t* p_ret);

/*******************************************************************************
 *
 * Function         a2dp_sbc_up_sample_16m (16bits-mono)
 *
 * Description      Given the source (p_src) audio data and
 *                  source speed (src_sps, samples per second),
 *                  This function converts it to audio data in the desired
 *                  format
 *
 *                  p_src: the data buffer that holds the source audio data
 *                  p_dst: the data buffer to hold the converted audio data
 *                  src_samples: The number of source samples (in uint of 2
 *                               bytes)
 *                  dst_samples: The size of p_dst (in uint of 2 bytes)
 *
 * Returns          The number of bytes used in p_dst
 *                  The number of bytes used in p_src (in *p_ret)
 *
 ******************************************************************************/
int a2dp_sbc_up_sample_16m(void* p_src, void* p_dst, uint32_t src_samples,
                           uint32_t dst_samples, uint32_t* p_ret);

/*******************************************************************************
 *
 * Function         a2dp_sbc_up_sample_8s (8bits-stereo)
 *
 * Description      Given the source (p_src) audio data and
 *                  source speed (src_sps, samples per second),
 *                  This function converts it to audio data in the desired
 *                  format
 *
 *                  p_src: the data buffer that holds the source audio data
 *                  p_dst: the data buffer to hold the converted audio data
 *                  src_samples: The number of source samples (in uint of 2
 *                               bytes)
 *                  dst_samples: The size of p_dst (in uint of 2 bytes)
 *
 * Returns          The number of bytes used in p_dst
 *                  The number of bytes used in p_src (in *p_ret)
 *
 ******************************************************************************/
int a2dp_sbc_up_sample_8s(void* p_src, void* p_dst, uint32_t src_samples,
                          uint32_t dst_samples, uint32_t* p_ret);

/*******************************************************************************
 *
 * Function         a2dp_sbc_up_sample_8m (8bits-mono)
 *
 * Description      Given the source (p_src) audio data and
 *                  source speed (src_sps, samples per second),
 *                  This function converts it to audio data in the desired
 *                  format
 *
 *                  p_src: the data buffer that holds the source audio data
 *                  p_dst: the data buffer to hold the converted audio data
 *                  src_samples: The number of source samples (number of bytes)
 *                  dst_samples: The size of p_dst (number of bytes)
 *
 * Returns          The number of bytes used in p_dst
 *                  The number of bytes used in p_src (in *p_ret)
 *
 ******************************************************************************/
int a2dp_sbc_up_sample_8m(void* p_src, void* p_dst, uint32_t src_samples,
                          uint32_t dst_samples, uint32_t* p_ret);

#endif  // A2DP_SBC_UP_SAMPLE_H
