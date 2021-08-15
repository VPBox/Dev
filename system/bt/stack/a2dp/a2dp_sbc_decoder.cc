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

#define LOG_TAG "a2dp_sbc_decoder"

#include "a2dp_sbc_decoder.h"

#include <base/logging.h>

#include "embdrv/sbc/decoder/include/oi_codec_sbc.h"
#include "embdrv/sbc/decoder/include/oi_status.h"
#include "osi/include/log.h"

typedef struct {
  OI_CODEC_SBC_DECODER_CONTEXT decoder_context;
  uint32_t context_data[CODEC_DATA_WORDS(2, SBC_CODEC_FAST_FILTER_BUFFERS)];
  int16_t decode_buf[15 * SBC_MAX_SAMPLES_PER_FRAME * SBC_MAX_CHANNELS];
  decoded_data_callback_t decode_callback;
} tA2DP_SBC_DECODER_CB;

static tA2DP_SBC_DECODER_CB a2dp_sbc_decoder_cb;

bool A2DP_LoadDecoderSbc(void) {
  // Nothing to do - the library is statically linked
  return true;
}

void A2DP_UnloadDecoderSbc(void) { a2dp_sbc_decoder_cleanup(); }

bool a2dp_sbc_decoder_init(decoded_data_callback_t decode_callback) {
  OI_STATUS status = OI_CODEC_SBC_DecoderReset(
      &a2dp_sbc_decoder_cb.decoder_context, a2dp_sbc_decoder_cb.context_data,
      sizeof(a2dp_sbc_decoder_cb.context_data), 2, 2, false);
  if (!OI_SUCCESS(status)) {
    LOG_ERROR(LOG_TAG,
              "%s: OI_CODEC_SBC_DecoderReset failed with error code %d",
              __func__, status);
    return false;
  }

  a2dp_sbc_decoder_cb.decode_callback = decode_callback;
  return true;
}

void a2dp_sbc_decoder_cleanup(void) {
  // Do nothing.
}

bool a2dp_sbc_decoder_decode_packet(BT_HDR* p_buf) {
  uint8_t* data = p_buf->data + p_buf->offset;
  size_t data_size = p_buf->len;

  if (data_size == 0) {
    LOG_ERROR(LOG_TAG, "%s: Empty packet", __func__);
    return false;
  }
  size_t num_frames = data[0] & 0xf;
  data += 1;
  data_size -= 1;

  const OI_BYTE* oi_data = data;
  uint32_t oi_size = data_size;
  size_t out_avail = sizeof(a2dp_sbc_decoder_cb.decode_buf);
  int16_t* out_ptr = a2dp_sbc_decoder_cb.decode_buf;

  for (size_t i = 0; i < num_frames; ++i) {
    uint32_t out_size = out_avail;
    OI_STATUS status =
        OI_CODEC_SBC_DecodeFrame(&a2dp_sbc_decoder_cb.decoder_context, &oi_data,
                                 &oi_size, out_ptr, &out_size);
    if (!OI_SUCCESS(status)) {
      LOG_ERROR(LOG_TAG, "%s: Decoding failure: %d", __func__, status);
      return false;
    }
    out_avail -= out_size;
    out_ptr += out_size / sizeof(*out_ptr);
  }

  size_t out_used =
      (out_ptr - a2dp_sbc_decoder_cb.decode_buf) * sizeof(*out_ptr);
  a2dp_sbc_decoder_cb.decode_callback(
      reinterpret_cast<uint8_t*>(a2dp_sbc_decoder_cb.decode_buf), out_used);
  return true;
}
