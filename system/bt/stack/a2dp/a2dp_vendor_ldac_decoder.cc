/*
 * Copyright 2016 The Android Open Source Project
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

#define LOG_TAG "a2dp_vendor_ldac_decoder"
#define ATRACE_TAG ATRACE_TAG_AUDIO

#include "a2dp_vendor_ldac_decoder.h"

#ifndef OS_GENERIC
#include <cutils/trace.h>
#endif
#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <ldacBT.h>

#include "a2dp_vendor.h"
#include "a2dp_vendor_ldac.h"
#include "bt_common.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

//
// Decoder for LDAC Source Codec
//

//
// The LDAC decoder shared library, and the functions to use
//
static const char* LDAC_DECODER_LIB_NAME = "libldacBT_dec.so";
static void* ldac_decoder_lib_handle = NULL;

static const char* LDAC_GET_HANDLE_NAME = "ldacBT_get_handle";
typedef HANDLE_LDAC_BT (*tLDAC_GET_HANDLE)(void);

static const char* LDAC_FREE_HANDLE_NAME = "ldacBT_free_handle";
typedef void (*tLDAC_FREE_HANDLE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_CLOSE_HANDLE_NAME = "ldacBT_close_handle";
typedef void (*tLDAC_CLOSE_HANDLE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_GET_VERSION_NAME = "ldacBT_get_version";
typedef int (*tLDAC_GET_VERSION)(void);

static const char* LDAC_GET_BITRATE_NAME = "ldacBT_get_bitrate";
typedef int (*tLDAC_GET_BITRATE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_GET_SAMPLING_FREQ_NAME = "ldacBT_get_sampling_freq";
typedef int (*tLDAC_GET_SAMPLING_FREQ)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_INIT_HANDLE_DECODE_NAME = "ldacBT_init_handle_decode";
typedef int (*tLDAC_INIT_HANDLE_DECODE)(HANDLE_LDAC_BT hLdacParam, int cm,
                                        int sf, int var0, int var1, int var2);

static const char* LDAC_DECODE_NAME = "ldacBT_decode";
typedef int (*tLDAC_DECODE)(HANDLE_LDAC_BT hLdacBt, unsigned char* p_bs,
                            unsigned char* p_pcm, LDACBT_SMPL_FMT_T fmt,
                            int bs_bytes, int* used_bytes, int* wrote_bytes);

static const char* LDAC_GET_ERROR_CODE_NAME = "ldacBT_get_error_code";
typedef int (*tLDAC_GET_ERROR_CODE)(HANDLE_LDAC_BT hLdacParam);

static tLDAC_GET_HANDLE ldac_get_handle_func;
static tLDAC_FREE_HANDLE ldac_free_handle_func;
static tLDAC_CLOSE_HANDLE ldac_close_handle_func;
static tLDAC_GET_VERSION ldac_get_version_func;
static tLDAC_GET_BITRATE ldac_get_bitrate_func;
static tLDAC_GET_SAMPLING_FREQ ldac_get_sampling_freq_func;
static tLDAC_INIT_HANDLE_DECODE ldac_init_handle_decode_func;
static tLDAC_DECODE ldac_decode_func;
static tLDAC_GET_ERROR_CODE ldac_get_error_code_func;

// offset
#if (BTA_AV_CO_CP_SCMS_T == TRUE)
#define A2DP_LDAC_OFFSET (AVDT_MEDIA_OFFSET + A2DP_LDAC_MPL_HDR_LEN + 1)
#else
#define A2DP_LDAC_OFFSET (AVDT_MEDIA_OFFSET + A2DP_LDAC_MPL_HDR_LEN)
#endif

typedef struct {
  uint32_t sample_rate;
  uint8_t channel_mode;
  uint8_t bits_per_sample;
  int pcm_wlength;
  LDACBT_SMPL_FMT_T pcm_fmt;
} tA2DP_LDAC_DECODER_PARAMS;

typedef struct {
  bool use_SCMS_T;
  bool is_peer_edr;          // True if the peer device supports EDR
  bool peer_supports_3mbps;  // True if the peer device supports 3Mbps EDR
  uint16_t peer_mtu;         // MTU of the A2DP peer
  uint32_t timestamp;        // Timestamp for the A2DP frames

  HANDLE_LDAC_BT ldac_handle;
  bool has_ldac_handle;  // True if ldac_handle is valid
  unsigned char* decode_buf;
  decoded_data_callback_t decode_callback;
} tA2DP_LDAC_DECODER_CB;

static tA2DP_LDAC_DECODER_CB a2dp_ldac_decoder_cb;

static void* load_func(const char* func_name) {
  void* func_ptr = dlsym(ldac_decoder_lib_handle, func_name);
  if (func_ptr == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the decoder library: %s",
              __func__, func_name, dlerror());
    A2DP_VendorUnloadDecoderLdac();
    return NULL;
  }
  return func_ptr;
}

bool A2DP_VendorLoadDecoderLdac(void) {
  if (ldac_decoder_lib_handle != NULL) return true;  // Already loaded

  // Initialize the control block
  memset(&a2dp_ldac_decoder_cb, 0, sizeof(a2dp_ldac_decoder_cb));

  // Open the decoder library
  ldac_decoder_lib_handle = dlopen(LDAC_DECODER_LIB_NAME, RTLD_NOW);
  if (ldac_decoder_lib_handle == NULL) {
    LOG_ERROR(LOG_TAG, "%s: cannot open LDAC decoder library %s: %s", __func__,
              LDAC_DECODER_LIB_NAME, dlerror());
    return false;
  }

  // Load all functions
  ldac_get_handle_func = (tLDAC_GET_HANDLE)load_func(LDAC_GET_HANDLE_NAME);
  if (ldac_get_handle_func == NULL) return false;
  ldac_free_handle_func = (tLDAC_FREE_HANDLE)load_func(LDAC_FREE_HANDLE_NAME);
  if (ldac_free_handle_func == NULL) return false;
  ldac_close_handle_func =
      (tLDAC_CLOSE_HANDLE)load_func(LDAC_CLOSE_HANDLE_NAME);
  if (ldac_close_handle_func == NULL) return false;
  ldac_get_version_func = (tLDAC_GET_VERSION)load_func(LDAC_GET_VERSION_NAME);
  if (ldac_get_version_func == NULL) return false;
  ldac_get_bitrate_func = (tLDAC_GET_BITRATE)load_func(LDAC_GET_BITRATE_NAME);
  if (ldac_get_bitrate_func == NULL) return false;
  ldac_get_sampling_freq_func =
      (tLDAC_GET_SAMPLING_FREQ)load_func(LDAC_GET_SAMPLING_FREQ_NAME);
  if (ldac_get_sampling_freq_func == NULL) return false;
  ldac_init_handle_decode_func =
      (tLDAC_INIT_HANDLE_DECODE)load_func(LDAC_INIT_HANDLE_DECODE_NAME);
  if (ldac_init_handle_decode_func == NULL) return false;
  ldac_decode_func = (tLDAC_DECODE)load_func(LDAC_DECODE_NAME);
  if (ldac_decode_func == NULL) return false;
  ldac_get_error_code_func =
      (tLDAC_GET_ERROR_CODE)load_func(LDAC_GET_ERROR_CODE_NAME);
  if (ldac_get_error_code_func == NULL) return false;

  return true;
}

void A2DP_VendorUnloadDecoderLdac(void) {
  // Cleanup any LDAC-related state
  if (a2dp_ldac_decoder_cb.has_ldac_handle && ldac_free_handle_func != NULL)
    ldac_free_handle_func(a2dp_ldac_decoder_cb.ldac_handle);
  memset(&a2dp_ldac_decoder_cb, 0, sizeof(a2dp_ldac_decoder_cb));

  ldac_get_handle_func = NULL;
  ldac_free_handle_func = NULL;
  ldac_close_handle_func = NULL;
  ldac_get_version_func = NULL;
  ldac_get_bitrate_func = NULL;
  ldac_get_sampling_freq_func = NULL;
  ldac_init_handle_decode_func = NULL;
  ldac_decode_func = NULL;
  ldac_get_error_code_func = NULL;

  if (ldac_decoder_lib_handle != NULL) {
    dlclose(ldac_decoder_lib_handle);
    ldac_decoder_lib_handle = NULL;
  }
}

bool a2dp_vendor_ldac_decoder_init(decoded_data_callback_t decode_callback) {
  if (a2dp_ldac_decoder_cb.has_ldac_handle)
    ldac_free_handle_func(a2dp_ldac_decoder_cb.ldac_handle);
  memset(&a2dp_ldac_decoder_cb, 0, sizeof(a2dp_ldac_decoder_cb));

  a2dp_vendor_ldac_decoder_cleanup();

  a2dp_ldac_decoder_cb.ldac_handle = ldac_get_handle_func();
  a2dp_ldac_decoder_cb.has_ldac_handle = true;
  a2dp_ldac_decoder_cb.decode_buf = static_cast<unsigned char*>(
      osi_malloc(sizeof(a2dp_ldac_decoder_cb.decode_buf[0]) * LDACBT_MAX_LSU *
                 LDAC_PRCNCH * sizeof(int)));
  a2dp_ldac_decoder_cb.decode_callback = decode_callback;

  // initialize
  ldac_init_handle_decode_func(a2dp_ldac_decoder_cb.ldac_handle,
                               LDACBT_CHANNEL_MODE_STEREO, 96000, 0, 0, 0);
  return true;
}

void a2dp_vendor_ldac_decoder_cleanup(void) {
  if (a2dp_ldac_decoder_cb.has_ldac_handle)
    ldac_free_handle_func(a2dp_ldac_decoder_cb.ldac_handle);
  memset(&a2dp_ldac_decoder_cb, 0, sizeof(a2dp_ldac_decoder_cb));
}

bool a2dp_vendor_ldac_decoder_decode_packet(BT_HDR* p_buf) {
  unsigned char* pBuffer =
      reinterpret_cast<unsigned char*>(p_buf->data + p_buf->offset);
  //  unsigned int bufferSize = p_buf->len;
  unsigned int bytesValid = p_buf->len;
  int err;
  LDACBT_SMPL_FMT_T fmt;
  int bs_bytes, used_bytes, wrote_bytes, frame_number;

  fmt = LDACBT_SMPL_FMT_S32;
  frame_number = (int)pBuffer[0];
  pBuffer++;
  bs_bytes = (int)bytesValid - 1;
  bytesValid -= 1;
  LOG_VERBOSE(LOG_TAG, "%s:INPUT size : %d, frame : %d", __func__, bs_bytes,
              frame_number);

  while (bytesValid > 0) {
#if 0
    err = ldacDecoder_Fill(a2dp_ldac_decoder_cb.ldac_handle,
                               &pBuffer, &bufferSize, &bytesValid);
    if (err != LDACBT_ERR_NONE) {
      LOG_ERROR(LOG_TAG, "%s: ldacDecoder_Fill failed: 0x%x", __func__,
                static_cast<unsigned>(err));
      return false;
    }
#endif
    while (true) {
      // Todo : implement LDAC Buffer Control Operation instead of
      // ldac_decode_func().
      err = ldac_decode_func(a2dp_ldac_decoder_cb.ldac_handle, pBuffer,
                             a2dp_ldac_decoder_cb.decode_buf, fmt, bs_bytes,
                             &used_bytes, &wrote_bytes);
      //      if (err == LDAC_DEC_NOT_ENOUGH_FRAMES) {
      //        break;
      //      }
      if (LDACBT_ERROR(err)) {
        err = ldac_get_error_code_func(a2dp_ldac_decoder_cb.ldac_handle);
        LOG_ERROR(LOG_TAG, "%s: ldacDecoder_DecodeFrame failed: %d:%d:%d",
                  __func__, LDACBT_API_ERR(err), LDACBT_HANDLE_ERR(err),
                  LDACBT_BLOCK_ERR(err));
        if (LDACBT_FATAL(err)) {
          break;
        }
      }

      if (wrote_bytes > 0) {
        size_t frame_len = (size_t)wrote_bytes;
        a2dp_ldac_decoder_cb.decode_callback(
            reinterpret_cast<uint8_t*>(a2dp_ldac_decoder_cb.decode_buf),
            frame_len);
      }
      pBuffer += used_bytes;
      bs_bytes -= used_bytes;
      if (bs_bytes <= 1) {
        bytesValid = 0;
        break;
      }
    }
  }

  return true;
}
