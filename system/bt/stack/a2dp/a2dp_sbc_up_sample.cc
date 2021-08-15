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
 *  This module contains utility functions for dealing with SBC data frames
 *  and codec capabilities.
 *
 ******************************************************************************/

#include "a2dp_sbc_up_sample.h"

typedef int(tA2DP_SBC_ACT)(void* p_src, void* p_dst, uint32_t src_samples,
                           uint32_t dst_samples, uint32_t* p_ret);

typedef struct {
  int32_t cur_pos;      /* current position */
  uint32_t src_sps;     /* samples per second (source audio data) */
  uint32_t dst_sps;     /* samples per second (converted audio data) */
  tA2DP_SBC_ACT* p_act; /* the action function to do the conversion */
  uint8_t bits;         /* number of bits per pcm sample */
  uint8_t n_channels;   /* number of channels (i.e. mono(1), stereo(2)...) */
  int16_t worker1;
  int16_t worker2;
  uint8_t div;
} tA2DP_SBC_UPS_CB;

tA2DP_SBC_UPS_CB a2dp_sbc_ups_cb;

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
                             uint8_t n_channels) {
  a2dp_sbc_ups_cb.cur_pos = -1;
  a2dp_sbc_ups_cb.src_sps = src_sps;
  a2dp_sbc_ups_cb.dst_sps = dst_sps;
  a2dp_sbc_ups_cb.bits = bits;
  a2dp_sbc_ups_cb.n_channels = n_channels;

  if (n_channels == 1) {
    /* mono */
    if (bits == 8) {
      a2dp_sbc_ups_cb.p_act = a2dp_sbc_up_sample_8m;
      a2dp_sbc_ups_cb.div = 1;
    } else {
      a2dp_sbc_ups_cb.p_act = a2dp_sbc_up_sample_16m;
      a2dp_sbc_ups_cb.div = 2;
    }
  } else {
    /* stereo */
    if (bits == 8) {
      a2dp_sbc_ups_cb.p_act = a2dp_sbc_up_sample_8s;
      a2dp_sbc_ups_cb.div = 2;
    } else {
      a2dp_sbc_ups_cb.p_act = a2dp_sbc_up_sample_16s;
      a2dp_sbc_ups_cb.div = 4;
    }
  }
}

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
                       uint32_t dst_samples, uint32_t* p_ret) {
  uint32_t src;
  uint32_t dst;

  if (a2dp_sbc_ups_cb.p_act) {
    src = src_samples / a2dp_sbc_ups_cb.div;
    dst = dst_samples / a2dp_sbc_ups_cb.div;
    return (*a2dp_sbc_ups_cb.p_act)(p_src, p_dst, src, dst, p_ret);
  } else {
    *p_ret = 0;
    return 0;
  }
}

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
                           uint32_t dst_samples, uint32_t* p_ret) {
  int16_t* p_src_tmp = (int16_t*)p_src;
  int16_t* p_dst_tmp = (int16_t*)p_dst;
  int16_t* p_worker1 = &a2dp_sbc_ups_cb.worker1;
  int16_t* p_worker2 = &a2dp_sbc_ups_cb.worker2;
  uint32_t src_sps = a2dp_sbc_ups_cb.src_sps;
  uint32_t dst_sps = a2dp_sbc_ups_cb.dst_sps;

  while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples) {
    *p_dst_tmp++ = *p_worker1;
    *p_dst_tmp++ = *p_worker2;

    a2dp_sbc_ups_cb.cur_pos -= src_sps;
    dst_samples--;
  }

  a2dp_sbc_ups_cb.cur_pos = dst_sps;

  while (src_samples-- && dst_samples) {
    *p_worker1 = *p_src_tmp++;
    *p_worker2 = *p_src_tmp++;

    do {
      *p_dst_tmp++ = *p_worker1;
      *p_dst_tmp++ = *p_worker2;

      a2dp_sbc_ups_cb.cur_pos -= src_sps;
      dst_samples--;
    } while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples);

    a2dp_sbc_ups_cb.cur_pos += dst_sps;
  }

  if (a2dp_sbc_ups_cb.cur_pos == (int32_t)dst_sps) a2dp_sbc_ups_cb.cur_pos = 0;

  *p_ret = ((char*)p_src_tmp - (char*)p_src);
  return ((char*)p_dst_tmp - (char*)p_dst);
}

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
                           uint32_t dst_samples, uint32_t* p_ret) {
  int16_t* p_src_tmp = (int16_t*)p_src;
  int16_t* p_dst_tmp = (int16_t*)p_dst;
  int16_t* p_worker = &a2dp_sbc_ups_cb.worker1;
  uint32_t src_sps = a2dp_sbc_ups_cb.src_sps;
  uint32_t dst_sps = a2dp_sbc_ups_cb.dst_sps;

  while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples) {
    *p_dst_tmp++ = *p_worker;
    *p_dst_tmp++ = *p_worker;

    a2dp_sbc_ups_cb.cur_pos -= src_sps;
    dst_samples--;
    dst_samples--;
  }

  a2dp_sbc_ups_cb.cur_pos = dst_sps;

  while (src_samples-- && dst_samples) {
    *p_worker = *p_src_tmp++;

    do {
      *p_dst_tmp++ = *p_worker;
      *p_dst_tmp++ = *p_worker;

      a2dp_sbc_ups_cb.cur_pos -= src_sps;
      dst_samples--;
      dst_samples--;

    } while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples);

    a2dp_sbc_ups_cb.cur_pos += dst_sps;
  }

  if (a2dp_sbc_ups_cb.cur_pos == (int32_t)dst_sps) a2dp_sbc_ups_cb.cur_pos = 0;

  *p_ret = ((char*)p_src_tmp - (char*)p_src);
  return ((char*)p_dst_tmp - (char*)p_dst);
}

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
                          uint32_t dst_samples, uint32_t* p_ret) {
  uint8_t* p_src_tmp = (uint8_t*)p_src;
  int16_t* p_dst_tmp = (int16_t*)p_dst;
  int16_t* p_worker1 = &a2dp_sbc_ups_cb.worker1;
  int16_t* p_worker2 = &a2dp_sbc_ups_cb.worker2;
  uint32_t src_sps = a2dp_sbc_ups_cb.src_sps;
  uint32_t dst_sps = a2dp_sbc_ups_cb.dst_sps;

  while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples) {
    *p_dst_tmp++ = *p_worker1;
    *p_dst_tmp++ = *p_worker2;

    a2dp_sbc_ups_cb.cur_pos -= src_sps;
    dst_samples--;
    dst_samples--;
  }

  a2dp_sbc_ups_cb.cur_pos = dst_sps;

  while (src_samples-- && dst_samples) {
    *p_worker1 = *(uint8_t*)p_src_tmp++;
    *p_worker1 -= 0x80;
    *p_worker1 <<= 8;
    *p_worker2 = *(uint8_t*)p_src_tmp++;
    *p_worker2 -= 0x80;
    *p_worker2 <<= 8;

    do {
      *p_dst_tmp++ = *p_worker1;
      *p_dst_tmp++ = *p_worker2;

      a2dp_sbc_ups_cb.cur_pos -= src_sps;
      dst_samples--;
      dst_samples--;
    } while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples);

    a2dp_sbc_ups_cb.cur_pos += dst_sps;
  }

  if (a2dp_sbc_ups_cb.cur_pos == (int32_t)dst_sps) a2dp_sbc_ups_cb.cur_pos = 0;

  *p_ret = ((char*)p_src_tmp - (char*)p_src);
  return ((char*)p_dst_tmp - (char*)p_dst);
}

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
                          uint32_t dst_samples, uint32_t* p_ret) {
  uint8_t* p_src_tmp = (uint8_t*)p_src;
  int16_t* p_dst_tmp = (int16_t*)p_dst;
  int16_t* p_worker = &a2dp_sbc_ups_cb.worker1;
  uint32_t src_sps = a2dp_sbc_ups_cb.src_sps;
  uint32_t dst_sps = a2dp_sbc_ups_cb.dst_sps;

  while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples) {
    *p_dst_tmp++ = *p_worker;
    *p_dst_tmp++ = *p_worker;

    a2dp_sbc_ups_cb.cur_pos -= src_sps;
    dst_samples -= 4;
  }

  a2dp_sbc_ups_cb.cur_pos = dst_sps;

  while (src_samples-- && dst_samples) {
    *p_worker = *(uint8_t*)p_src_tmp++;
    *p_worker -= 0x80;
    *p_worker <<= 8;

    do {
      *p_dst_tmp++ = *p_worker;
      *p_dst_tmp++ = *p_worker;

      a2dp_sbc_ups_cb.cur_pos -= src_sps;
      dst_samples -= 4;

    } while (a2dp_sbc_ups_cb.cur_pos > 0 && dst_samples);

    a2dp_sbc_ups_cb.cur_pos += dst_sps;
  }

  if (a2dp_sbc_ups_cb.cur_pos == (int32_t)dst_sps) a2dp_sbc_ups_cb.cur_pos = 0;

  *p_ret = ((char*)p_src_tmp - (char*)p_src);
  return ((char*)p_dst_tmp - (char*)p_dst);
}
