/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "a2dp_aac_decoder"

#include "a2dp_aac_decoder.h"

#include <aacdecoder_lib.h>
#include <base/logging.h>

#include "a2dp_aac.h"
#include "osi/include/allocator.h"
#include "osi/include/log.h"

#define DECODE_BUF_LEN (8 * 2 * 1024)

typedef struct {
  HANDLE_AACDECODER aac_handle;
  bool has_aac_handle;  // True if aac_handle is valid
  INT_PCM* decode_buf;
  decoded_data_callback_t decode_callback;
} tA2DP_AAC_DECODER_CB;

static tA2DP_AAC_DECODER_CB a2dp_aac_decoder_cb;

bool A2DP_LoadDecoderAac(void) {
  // Nothing to do - the library is statically linked
  return true;
}

void A2DP_UnloadDecoderAac(void) { a2dp_aac_decoder_cleanup(); }

bool a2dp_aac_decoder_init(decoded_data_callback_t decode_callback) {
  a2dp_aac_decoder_cleanup();

  a2dp_aac_decoder_cb.aac_handle =
      aacDecoder_Open(TT_MP4_LATM_MCP1, 1 /* nrOfLayers */);
  a2dp_aac_decoder_cb.has_aac_handle = true;
  a2dp_aac_decoder_cb.decode_buf = static_cast<INT_PCM*>(
      osi_malloc(sizeof(a2dp_aac_decoder_cb.decode_buf[0]) * DECODE_BUF_LEN));
  a2dp_aac_decoder_cb.decode_callback = decode_callback;
  return true;
}

void a2dp_aac_decoder_cleanup(void) {
  if (a2dp_aac_decoder_cb.has_aac_handle)
    aacDecoder_Close(a2dp_aac_decoder_cb.aac_handle);
  free(a2dp_aac_decoder_cb.decode_buf);
  memset(&a2dp_aac_decoder_cb, 0, sizeof(a2dp_aac_decoder_cb));
}

bool a2dp_aac_decoder_decode_packet(BT_HDR* p_buf) {
  auto* pBuffer = reinterpret_cast<UCHAR*>(p_buf->data + p_buf->offset);
  UINT bufferSize = p_buf->len;
  UINT bytesValid = p_buf->len;
  while (bytesValid > 0) {
    AAC_DECODER_ERROR err = aacDecoder_Fill(a2dp_aac_decoder_cb.aac_handle,
                                            &pBuffer, &bufferSize, &bytesValid);
    if (err != AAC_DEC_OK) {
      LOG_ERROR(LOG_TAG, "%s: aacDecoder_Fill failed: 0x%x", __func__,
                static_cast<unsigned>(err));
      return false;
    }

    while (true) {
      err = aacDecoder_DecodeFrame(a2dp_aac_decoder_cb.aac_handle,
                                   a2dp_aac_decoder_cb.decode_buf,
                                   DECODE_BUF_LEN, 0 /* flags */);
      if (err == AAC_DEC_NOT_ENOUGH_BITS) {
        break;
      }
      if (err != AAC_DEC_OK) {
        LOG_ERROR(LOG_TAG, "%s: aacDecoder_DecodeFrame failed: 0x%x", __func__,
                  static_cast<int>(err));
        break;
      }

      CStreamInfo* info =
          aacDecoder_GetStreamInfo(a2dp_aac_decoder_cb.aac_handle);
      if (!info || info->sampleRate <= 0) {
        LOG_ERROR(LOG_TAG, "%s: Invalid stream info", __func__);
        break;
      }

      size_t frame_len = info->frameSize * info->numChannels *
                         sizeof(a2dp_aac_decoder_cb.decode_buf[0]);
      a2dp_aac_decoder_cb.decode_callback(
          reinterpret_cast<uint8_t*>(a2dp_aac_decoder_cb.decode_buf),
          frame_len);
    }
  }

  return true;
}
