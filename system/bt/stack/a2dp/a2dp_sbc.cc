/******************************************************************************
 *
 *  Copyright 2002-2012 Broadcom Corporation
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
 *  Utility functions to help build and parse SBC Codec Information Element
 *  and Media Payload.
 *
 ******************************************************************************/

#define LOG_TAG "a2dp_sbc"

#include "bt_target.h"

#include "a2dp_sbc.h"

#include <string.h>

#include <base/logging.h>
#include "a2dp_sbc_decoder.h"
#include "a2dp_sbc_encoder.h"
#include "bt_utils.h"
#include "embdrv/sbc/encoder/include/sbc_encoder.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

#define A2DP_SBC_MAX_BITPOOL 53

/* data type for the SBC Codec Information Element */
typedef struct {
  uint8_t samp_freq;    /* Sampling frequency */
  uint8_t ch_mode;      /* Channel mode */
  uint8_t block_len;    /* Block length */
  uint8_t num_subbands; /* Number of subbands */
  uint8_t alloc_method; /* Allocation method */
  uint8_t min_bitpool;  /* Minimum bitpool */
  uint8_t max_bitpool;  /* Maximum bitpool */
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
} tA2DP_SBC_CIE;

/* SBC Source codec capabilities */
static const tA2DP_SBC_CIE a2dp_sbc_source_caps = {
    (A2DP_SBC_IE_SAMP_FREQ_44),                         /* samp_freq */
    (A2DP_SBC_IE_CH_MD_MONO | A2DP_SBC_IE_CH_MD_JOINT), /* ch_mode */
    (A2DP_SBC_IE_BLOCKS_16 | A2DP_SBC_IE_BLOCKS_12 | A2DP_SBC_IE_BLOCKS_8 |
     A2DP_SBC_IE_BLOCKS_4),            /* block_len */
    A2DP_SBC_IE_SUBBAND_8,             /* num_subbands */
    A2DP_SBC_IE_ALLOC_MD_L,            /* alloc_method */
    A2DP_SBC_IE_MIN_BITPOOL,           /* min_bitpool */
    A2DP_SBC_MAX_BITPOOL,              /* max_bitpool */
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16 /* bits_per_sample */
};

/* SBC Sink codec capabilities */
static const tA2DP_SBC_CIE a2dp_sbc_sink_caps = {
    (A2DP_SBC_IE_SAMP_FREQ_48 | A2DP_SBC_IE_SAMP_FREQ_44), /* samp_freq */
    (A2DP_SBC_IE_CH_MD_MONO | A2DP_SBC_IE_CH_MD_STEREO |
     A2DP_SBC_IE_CH_MD_JOINT | A2DP_SBC_IE_CH_MD_DUAL), /* ch_mode */
    (A2DP_SBC_IE_BLOCKS_16 | A2DP_SBC_IE_BLOCKS_12 | A2DP_SBC_IE_BLOCKS_8 |
     A2DP_SBC_IE_BLOCKS_4),                            /* block_len */
    (A2DP_SBC_IE_SUBBAND_4 | A2DP_SBC_IE_SUBBAND_8),   /* num_subbands */
    (A2DP_SBC_IE_ALLOC_MD_L | A2DP_SBC_IE_ALLOC_MD_S), /* alloc_method */
    A2DP_SBC_IE_MIN_BITPOOL,                           /* min_bitpool */
    A2DP_SBC_MAX_BITPOOL,                              /* max_bitpool */
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16                 /* bits_per_sample */
};

/* Default SBC codec configuration */
const tA2DP_SBC_CIE a2dp_sbc_default_config = {
    A2DP_SBC_IE_SAMP_FREQ_44,          /* samp_freq */
    A2DP_SBC_IE_CH_MD_JOINT,           /* ch_mode */
    A2DP_SBC_IE_BLOCKS_16,             /* block_len */
    A2DP_SBC_IE_SUBBAND_8,             /* num_subbands */
    A2DP_SBC_IE_ALLOC_MD_L,            /* alloc_method */
    A2DP_SBC_IE_MIN_BITPOOL,           /* min_bitpool */
    A2DP_SBC_MAX_BITPOOL,              /* max_bitpool */
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16 /* bits_per_sample */
};

static const tA2DP_ENCODER_INTERFACE a2dp_encoder_interface_sbc = {
    a2dp_sbc_encoder_init,
    a2dp_sbc_encoder_cleanup,
    a2dp_sbc_feeding_reset,
    a2dp_sbc_feeding_flush,
    a2dp_sbc_get_encoder_interval_ms,
    a2dp_sbc_send_frames,
    nullptr  // set_transmit_queue_length
};

static const tA2DP_DECODER_INTERFACE a2dp_decoder_interface_sbc = {
    a2dp_sbc_decoder_init, a2dp_sbc_decoder_cleanup,
    a2dp_sbc_decoder_decode_packet,
};

static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilitySbc(
    const tA2DP_SBC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_capability);
static void A2DP_ParseMplHeaderSbc(uint8_t* p_src, bool* p_frag, bool* p_start,
                                   bool* p_last, uint8_t* p_num);

// Builds the SBC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. |media_type| is the media type |AVDT_MEDIA_TYPE_*|.
// |p_ie| is a pointer to the SBC Codec Information Element information.
// The result is stored in |p_result|. Returns A2DP_SUCCESS on success,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_BuildInfoSbc(uint8_t media_type,
                                      const tA2DP_SBC_CIE* p_ie,
                                      uint8_t* p_result) {
  if (p_ie == NULL || p_result == NULL ||
      (p_ie->samp_freq & ~A2DP_SBC_IE_SAMP_FREQ_MSK) ||
      (p_ie->ch_mode & ~A2DP_SBC_IE_CH_MD_MSK) ||
      (p_ie->block_len & ~A2DP_SBC_IE_BLOCKS_MSK) ||
      (p_ie->num_subbands & ~A2DP_SBC_IE_SUBBAND_MSK) ||
      (p_ie->alloc_method & ~A2DP_SBC_IE_ALLOC_MD_MSK) ||
      (p_ie->min_bitpool > p_ie->max_bitpool) ||
      (p_ie->min_bitpool < A2DP_SBC_IE_MIN_BITPOOL) ||
      (p_ie->min_bitpool > A2DP_SBC_IE_MAX_BITPOOL) ||
      (p_ie->max_bitpool < A2DP_SBC_IE_MIN_BITPOOL) ||
      (p_ie->max_bitpool > A2DP_SBC_IE_MAX_BITPOOL)) {
    /* if any unused bit is set */
    return A2DP_INVALID_PARAMS;
  }

  *p_result++ = A2DP_SBC_INFO_LEN;
  *p_result++ = (media_type << 4);
  *p_result++ = A2DP_MEDIA_CT_SBC;

  /* Media Codec Specific Information Element */
  *p_result++ = p_ie->samp_freq | p_ie->ch_mode;

  *p_result++ = p_ie->block_len | p_ie->num_subbands | p_ie->alloc_method;

  *p_result++ = p_ie->min_bitpool;
  *p_result = p_ie->max_bitpool;

  return A2DP_SUCCESS;
}

// Parses the SBC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. The result is stored in |p_ie|. The byte sequence to parse is
// |p_codec_info|. If |is_capability| is true, the byte sequence contains
// codec capability.
// Returns A2DP_SUCCESS on success, otherwise the corresponding A2DP error
// status code.
static tA2DP_STATUS A2DP_ParseInfoSbc(tA2DP_SBC_CIE* p_ie,
                                      const uint8_t* p_codec_info,
                                      bool is_capability) {
  uint8_t losc;
  uint8_t media_type;
  tA2DP_CODEC_TYPE codec_type;

  if (p_ie == NULL || p_codec_info == NULL) return A2DP_INVALID_PARAMS;

  // Check the codec capability length
  losc = *p_codec_info++;
  if (losc != A2DP_SBC_INFO_LEN) return A2DP_WRONG_CODEC;

  media_type = (*p_codec_info++) >> 4;
  codec_type = *p_codec_info++;
  /* Check the Media Type and Media Codec Type */
  if (media_type != AVDT_MEDIA_TYPE_AUDIO || codec_type != A2DP_MEDIA_CT_SBC) {
    return A2DP_WRONG_CODEC;
  }

  p_ie->samp_freq = *p_codec_info & A2DP_SBC_IE_SAMP_FREQ_MSK;
  p_ie->ch_mode = *p_codec_info & A2DP_SBC_IE_CH_MD_MSK;
  p_codec_info++;
  p_ie->block_len = *p_codec_info & A2DP_SBC_IE_BLOCKS_MSK;
  p_ie->num_subbands = *p_codec_info & A2DP_SBC_IE_SUBBAND_MSK;
  p_ie->alloc_method = *p_codec_info & A2DP_SBC_IE_ALLOC_MD_MSK;
  p_codec_info++;
  p_ie->min_bitpool = *p_codec_info++;
  p_ie->max_bitpool = *p_codec_info++;
  if (p_ie->min_bitpool < A2DP_SBC_IE_MIN_BITPOOL ||
      p_ie->min_bitpool > A2DP_SBC_IE_MAX_BITPOOL) {
    return A2DP_BAD_MIN_BITPOOL;
  }

  if (p_ie->max_bitpool < A2DP_SBC_IE_MIN_BITPOOL ||
      p_ie->max_bitpool > A2DP_SBC_IE_MAX_BITPOOL ||
      p_ie->max_bitpool < p_ie->min_bitpool) {
    return A2DP_BAD_MAX_BITPOOL;
  }

  if (is_capability) {
    // NOTE: The checks here are very liberal. We should be using more
    // pedantic checks specific to the SRC or SNK as specified in the spec.
    if (A2DP_BitsSet(p_ie->samp_freq) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_SAMP_FREQ;
    if (A2DP_BitsSet(p_ie->ch_mode) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_CH_MODE;
    if (A2DP_BitsSet(p_ie->block_len) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_BLOCK_LEN;
    if (A2DP_BitsSet(p_ie->num_subbands) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_SUBBANDS;
    if (A2DP_BitsSet(p_ie->alloc_method) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_ALLOC_METHOD;

    return A2DP_SUCCESS;
  }

  if (A2DP_BitsSet(p_ie->samp_freq) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_SAMP_FREQ;
  if (A2DP_BitsSet(p_ie->ch_mode) != A2DP_SET_ONE_BIT) return A2DP_BAD_CH_MODE;
  if (A2DP_BitsSet(p_ie->block_len) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_BLOCK_LEN;
  if (A2DP_BitsSet(p_ie->num_subbands) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_SUBBANDS;
  if (A2DP_BitsSet(p_ie->alloc_method) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_ALLOC_METHOD;

  return A2DP_SUCCESS;
}

// Build the SBC Media Payload Header.
// |p_dst| points to the location where the header should be written to.
// If |frag| is true, the media payload frame is fragmented.
// |start| is true for the first packet of a fragmented frame.
// |last| is true for the last packet of a fragmented frame.
// If |frag| is false, |num| is the number of number of frames in the packet,
// otherwise is the number of remaining fragments (including this one).
static void A2DP_BuildMediaPayloadHeaderSbc(uint8_t* p_dst, bool frag,
                                            bool start, bool last,
                                            uint8_t num) {
  if (p_dst == NULL) return;

  *p_dst = 0;
  if (frag) *p_dst |= A2DP_SBC_HDR_F_MSK;
  if (start) *p_dst |= A2DP_SBC_HDR_S_MSK;
  if (last) *p_dst |= A2DP_SBC_HDR_L_MSK;
  *p_dst |= (A2DP_SBC_HDR_NUM_MSK & num);
}

/******************************************************************************
 *
 * Function         A2DP_ParseMplHeaderSbc
 *
 * Description      This function is called by an application to parse
 *                  the SBC Media Payload header.
 *                  Input Parameters:
 *                      p_src:  the byte sequence to parse..
 *
 *                  Output Parameters:
 *                      frag:  1, if fragmented. 0, otherwise.
 *
 *                      start:  1, if the starting packet of a fragmented frame.
 *
 *                      last:  1, if the last packet of a fragmented frame.
 *
 *                      num:  If frag is 1, this is the number of remaining
 *                            fragments
 *                            (including this fragment) of this frame.
 *                            If frag is 0, this is the number of frames in
 *                            this packet.
 *
 * Returns          void.
 *****************************************************************************/
UNUSED_ATTR static void A2DP_ParseMplHeaderSbc(uint8_t* p_src, bool* p_frag,
                                               bool* p_start, bool* p_last,
                                               uint8_t* p_num) {
  if (p_src && p_frag && p_start && p_last && p_num) {
    *p_frag = (*p_src & A2DP_SBC_HDR_F_MSK) ? true : false;
    *p_start = (*p_src & A2DP_SBC_HDR_S_MSK) ? true : false;
    *p_last = (*p_src & A2DP_SBC_HDR_L_MSK) ? true : false;
    *p_num = (*p_src & A2DP_SBC_HDR_NUM_MSK);
  }
}

const char* A2DP_CodecNameSbc(UNUSED_ATTR const uint8_t* p_codec_info) {
  return "SBC";
}

bool A2DP_IsSourceCodecValidSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsSinkCodecValidSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsPeerSourceCodecValidSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsPeerSinkCodecValidSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsSinkCodecSupportedSbc(const uint8_t* p_codec_info) {
  return (A2DP_CodecInfoMatchesCapabilitySbc(&a2dp_sbc_sink_caps, p_codec_info,
                                             false) == A2DP_SUCCESS);
}

bool A2DP_IsPeerSourceCodecSupportedSbc(const uint8_t* p_codec_info) {
  return (A2DP_CodecInfoMatchesCapabilitySbc(&a2dp_sbc_sink_caps, p_codec_info,
                                             true) == A2DP_SUCCESS);
}

void A2DP_InitDefaultCodecSbc(uint8_t* p_codec_info) {
  if (A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &a2dp_sbc_default_config,
                        p_codec_info) != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: A2DP_BuildInfoSbc failed", __func__);
  }
}

// Checks whether A2DP SBC codec configuration matches with a device's codec
// capabilities. |p_cap| is the SBC codec configuration. |p_codec_info| is
// the device's codec capabilities. |is_capability| is true if
// |p_codec_info| contains A2DP codec capability.
// Returns A2DP_SUCCESS if the codec configuration matches with capabilities,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilitySbc(
    const tA2DP_SBC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_capability) {
  tA2DP_STATUS status;
  tA2DP_SBC_CIE cfg_cie;

  /* parse configuration */
  status = A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: parsing failed %d", __func__, status);
    return status;
  }

  /* verify that each parameter is in range */

  LOG_VERBOSE(LOG_TAG, "%s: FREQ peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.samp_freq, p_cap->samp_freq);
  LOG_VERBOSE(LOG_TAG, "%s: CH_MODE peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.ch_mode, p_cap->ch_mode);
  LOG_VERBOSE(LOG_TAG, "%s: BLOCK_LEN peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.block_len, p_cap->block_len);
  LOG_VERBOSE(LOG_TAG, "%s: SUB_BAND peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.num_subbands, p_cap->num_subbands);
  LOG_VERBOSE(LOG_TAG, "%s: ALLOC_METHOD peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.alloc_method, p_cap->alloc_method);
  LOG_VERBOSE(LOG_TAG, "%s: MIN_BitPool peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.min_bitpool, p_cap->min_bitpool);
  LOG_VERBOSE(LOG_TAG, "%s: MAX_BitPool peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.max_bitpool, p_cap->max_bitpool);

  /* sampling frequency */
  if ((cfg_cie.samp_freq & p_cap->samp_freq) == 0) return A2DP_NS_SAMP_FREQ;

  /* channel mode */
  if ((cfg_cie.ch_mode & p_cap->ch_mode) == 0) return A2DP_NS_CH_MODE;

  /* block length */
  if ((cfg_cie.block_len & p_cap->block_len) == 0) return A2DP_BAD_BLOCK_LEN;

  /* subbands */
  if ((cfg_cie.num_subbands & p_cap->num_subbands) == 0)
    return A2DP_NS_SUBBANDS;

  /* allocation method */
  if ((cfg_cie.alloc_method & p_cap->alloc_method) == 0)
    return A2DP_NS_ALLOC_METHOD;

  /* min bitpool */
  if (cfg_cie.min_bitpool > p_cap->max_bitpool) return A2DP_NS_MIN_BITPOOL;

  /* max bitpool */
  if (cfg_cie.max_bitpool < p_cap->min_bitpool) return A2DP_NS_MAX_BITPOOL;

  return A2DP_SUCCESS;
}

bool A2DP_CodecTypeEqualsSbc(const uint8_t* p_codec_info_a,
                             const uint8_t* p_codec_info_b) {
  tA2DP_SBC_CIE sbc_cie_a;
  tA2DP_SBC_CIE sbc_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoSbc(&sbc_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoSbc(&sbc_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  tA2DP_CODEC_TYPE codec_type_a = A2DP_GetCodecType(p_codec_info_a);
  tA2DP_CODEC_TYPE codec_type_b = A2DP_GetCodecType(p_codec_info_b);

  return (codec_type_a == codec_type_b) && (codec_type_a == A2DP_MEDIA_CT_SBC);
}

bool A2DP_CodecEqualsSbc(const uint8_t* p_codec_info_a,
                         const uint8_t* p_codec_info_b) {
  tA2DP_SBC_CIE sbc_cie_a;
  tA2DP_SBC_CIE sbc_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoSbc(&sbc_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoSbc(&sbc_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  tA2DP_CODEC_TYPE codec_type_a = A2DP_GetCodecType(p_codec_info_a);
  tA2DP_CODEC_TYPE codec_type_b = A2DP_GetCodecType(p_codec_info_b);

  if ((codec_type_a != codec_type_b) || (codec_type_a != A2DP_MEDIA_CT_SBC))
    return false;

  return (sbc_cie_a.samp_freq == sbc_cie_b.samp_freq) &&
         (sbc_cie_a.ch_mode == sbc_cie_b.ch_mode) &&
         (sbc_cie_a.block_len == sbc_cie_b.block_len) &&
         (sbc_cie_a.num_subbands == sbc_cie_b.num_subbands) &&
         (sbc_cie_a.alloc_method == sbc_cie_b.alloc_method) &&
         (sbc_cie_a.min_bitpool == sbc_cie_b.min_bitpool) &&
         (sbc_cie_a.max_bitpool == sbc_cie_b.max_bitpool);
}

int A2DP_GetTrackSampleRateSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.samp_freq) {
    case A2DP_SBC_IE_SAMP_FREQ_16:
      return 16000;
    case A2DP_SBC_IE_SAMP_FREQ_32:
      return 32000;
    case A2DP_SBC_IE_SAMP_FREQ_44:
      return 44100;
    case A2DP_SBC_IE_SAMP_FREQ_48:
      return 48000;
    default:
      break;
  }

  return -1;
}

int A2DP_GetTrackBitsPerSampleSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  // NOTE: The bits per sample never changes for SBC
  return 16;
}

int A2DP_GetTrackChannelCountSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.ch_mode) {
    case A2DP_SBC_IE_CH_MD_MONO:
      return 1;
    case A2DP_SBC_IE_CH_MD_DUAL:
    case A2DP_SBC_IE_CH_MD_STEREO:
    case A2DP_SBC_IE_CH_MD_JOINT:
      return 2;
    default:
      break;
  }

  return -1;
}

int A2DP_GetNumberOfSubbandsSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.num_subbands) {
    case A2DP_SBC_IE_SUBBAND_4:
      return 4;
    case A2DP_SBC_IE_SUBBAND_8:
      return 8;
    default:
      break;
  }

  return -1;
}

int A2DP_GetNumberOfBlocksSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.block_len) {
    case A2DP_SBC_IE_BLOCKS_4:
      return 4;
    case A2DP_SBC_IE_BLOCKS_8:
      return 8;
    case A2DP_SBC_IE_BLOCKS_12:
      return 12;
    case A2DP_SBC_IE_BLOCKS_16:
      return 16;
    default:
      break;
  }

  return -1;
}

int A2DP_GetAllocationMethodCodeSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.alloc_method) {
    case A2DP_SBC_IE_ALLOC_MD_S:
      return SBC_SNR;
    case A2DP_SBC_IE_ALLOC_MD_L:
      return SBC_LOUDNESS;
    default:
      break;
  }

  return -1;
}

int A2DP_GetChannelModeCodeSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.ch_mode) {
    case A2DP_SBC_IE_CH_MD_MONO:
      return SBC_MONO;
    case A2DP_SBC_IE_CH_MD_DUAL:
      return SBC_DUAL;
    case A2DP_SBC_IE_CH_MD_STEREO:
      return SBC_STEREO;
    case A2DP_SBC_IE_CH_MD_JOINT:
      return SBC_JOINT_STEREO;
    default:
      break;
  }

  return -1;
}

int A2DP_GetSamplingFrequencyCodeSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.samp_freq) {
    case A2DP_SBC_IE_SAMP_FREQ_16:
      return SBC_sf16000;
    case A2DP_SBC_IE_SAMP_FREQ_32:
      return SBC_sf32000;
    case A2DP_SBC_IE_SAMP_FREQ_44:
      return SBC_sf44100;
    case A2DP_SBC_IE_SAMP_FREQ_48:
      return SBC_sf48000;
    default:
      break;
  }

  return -1;
}

int A2DP_GetMinBitpoolSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  return sbc_cie.min_bitpool;
}

int A2DP_GetMaxBitpoolSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  return sbc_cie.max_bitpool;
}

uint32_t A2DP_GetBitrateSbc() { return a2dp_sbc_get_bitrate(); }
int A2DP_GetSinkTrackChannelTypeSbc(const uint8_t* p_codec_info) {
  tA2DP_SBC_CIE sbc_cie;

  tA2DP_STATUS a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (sbc_cie.ch_mode) {
    case A2DP_SBC_IE_CH_MD_MONO:
      return 1;
    case A2DP_SBC_IE_CH_MD_DUAL:
    case A2DP_SBC_IE_CH_MD_STEREO:
    case A2DP_SBC_IE_CH_MD_JOINT:
      return 3;
    default:
      break;
  }

  return -1;
}

bool A2DP_GetPacketTimestampSbc(UNUSED_ATTR const uint8_t* p_codec_info,
                                const uint8_t* p_data, uint32_t* p_timestamp) {
  *p_timestamp = *(const uint32_t*)p_data;
  return true;
}

bool A2DP_BuildCodecHeaderSbc(UNUSED_ATTR const uint8_t* p_codec_info,
                              BT_HDR* p_buf, uint16_t frames_per_packet) {
  uint8_t* p;

  p_buf->offset -= A2DP_SBC_MPL_HDR_LEN;
  p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  p_buf->len += A2DP_SBC_MPL_HDR_LEN;
  A2DP_BuildMediaPayloadHeaderSbc(p, false, false, false,
                                  (uint8_t)frames_per_packet);

  return true;
}

std::string A2DP_CodecInfoStringSbc(const uint8_t* p_codec_info) {
  std::stringstream res;
  std::string field;
  tA2DP_STATUS a2dp_status;
  tA2DP_SBC_CIE sbc_cie;

  a2dp_status = A2DP_ParseInfoSbc(&sbc_cie, p_codec_info, true);
  if (a2dp_status != A2DP_SUCCESS) {
    res << "A2DP_ParseInfoSbc fail: " << loghex(a2dp_status);
    return res.str();
  }

  res << "\tname: SBC\n";

  // Sample frequency
  field.clear();
  AppendField(&field, (sbc_cie.samp_freq == 0), "NONE");
  AppendField(&field, (sbc_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_16), "16000");
  AppendField(&field, (sbc_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_32), "32000");
  AppendField(&field, (sbc_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_44), "44100");
  AppendField(&field, (sbc_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_48), "48000");
  res << "\tsamp_freq: " << field << " (" << loghex(sbc_cie.samp_freq) << ")\n";

  // Channel mode
  field.clear();
  AppendField(&field, (sbc_cie.ch_mode == 0), "NONE");
  AppendField(&field, (sbc_cie.ch_mode & A2DP_SBC_IE_CH_MD_MONO), "Mono");
  AppendField(&field, (sbc_cie.ch_mode & A2DP_SBC_IE_CH_MD_DUAL), "Dual");
  AppendField(&field, (sbc_cie.ch_mode & A2DP_SBC_IE_CH_MD_STEREO), "Stereo");
  AppendField(&field, (sbc_cie.ch_mode & A2DP_SBC_IE_CH_MD_JOINT), "Joint");
  res << "\tch_mode: " << field << " (" << loghex(sbc_cie.ch_mode) << ")\n";

  // Block length
  field.clear();
  AppendField(&field, (sbc_cie.block_len == 0), "NONE");
  AppendField(&field, (sbc_cie.block_len & A2DP_SBC_IE_BLOCKS_4), "4");
  AppendField(&field, (sbc_cie.block_len & A2DP_SBC_IE_BLOCKS_8), "8");
  AppendField(&field, (sbc_cie.block_len & A2DP_SBC_IE_BLOCKS_12), "12");
  AppendField(&field, (sbc_cie.block_len & A2DP_SBC_IE_BLOCKS_16), "16");
  res << "\tblock_len: " << field << " (" << loghex(sbc_cie.block_len) << ")\n";

  // Number of subbands
  field.clear();
  AppendField(&field, (sbc_cie.num_subbands == 0), "NONE");
  AppendField(&field, (sbc_cie.num_subbands & A2DP_SBC_IE_SUBBAND_4), "4");
  AppendField(&field, (sbc_cie.num_subbands & A2DP_SBC_IE_SUBBAND_8), "8");
  res << "\tnum_subbands: " << field << " (" << loghex(sbc_cie.num_subbands)
      << ")\n";

  // Allocation method
  field.clear();
  AppendField(&field, (sbc_cie.alloc_method == 0), "NONE");
  AppendField(&field, (sbc_cie.alloc_method & A2DP_SBC_IE_ALLOC_MD_S), "SNR");
  AppendField(&field, (sbc_cie.alloc_method & A2DP_SBC_IE_ALLOC_MD_L),
              "Loundess");
  res << "\talloc_method: " << field << " (" << loghex(sbc_cie.alloc_method)
      << ")\n";

  // Min/max bitloop
  res << "\tBit pool Min: " << std::to_string(sbc_cie.min_bitpool)
      << " Max: " << std::to_string(sbc_cie.max_bitpool);

  return res.str();
}

const tA2DP_ENCODER_INTERFACE* A2DP_GetEncoderInterfaceSbc(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsSourceCodecValidSbc(p_codec_info)) return NULL;

  return &a2dp_encoder_interface_sbc;
}

const tA2DP_DECODER_INTERFACE* A2DP_GetDecoderInterfaceSbc(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsSinkCodecValidSbc(p_codec_info)) return NULL;

  return &a2dp_decoder_interface_sbc;
}

bool A2DP_AdjustCodecSbc(uint8_t* p_codec_info) {
  tA2DP_SBC_CIE cfg_cie;

  if (A2DP_ParseInfoSbc(&cfg_cie, p_codec_info, true) != A2DP_SUCCESS)
    return false;

  // Updated the max bitpool
  if (cfg_cie.max_bitpool > A2DP_SBC_MAX_BITPOOL) {
    LOG_WARN(LOG_TAG, "%s: Updated the SBC codec max bitpool from %d to %d",
             __func__, cfg_cie.max_bitpool, A2DP_SBC_MAX_BITPOOL);
    cfg_cie.max_bitpool = A2DP_SBC_MAX_BITPOOL;
  }

  return (A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &cfg_cie, p_codec_info) ==
          A2DP_SUCCESS);
}

btav_a2dp_codec_index_t A2DP_SourceCodecIndexSbc(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SOURCE_SBC;
}

btav_a2dp_codec_index_t A2DP_SinkCodecIndexSbc(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SINK_SBC;
}

const char* A2DP_CodecIndexStrSbc(void) { return "SBC"; }

const char* A2DP_CodecIndexStrSbcSink(void) { return "SBC SINK"; }

bool A2DP_InitCodecConfigSbc(AvdtpSepConfig* p_cfg) {
  if (A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &a2dp_sbc_source_caps,
                        p_cfg->codec_info) != A2DP_SUCCESS) {
    return false;
  }

#if (BTA_AV_CO_CP_SCMS_T == TRUE)
  /* Content protection info - support SCMS-T */
  uint8_t* p = p_cfg->protect_info;
  *p++ = AVDT_CP_LOSC;
  UINT16_TO_STREAM(p, AVDT_CP_SCMS_T_ID);
  p_cfg->num_protect = 1;
#endif

  return true;
}

bool A2DP_InitCodecConfigSbcSink(AvdtpSepConfig* p_cfg) {
  if (A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &a2dp_sbc_sink_caps,
                        p_cfg->codec_info) != A2DP_SUCCESS) {
    return false;
  }

  return true;
}

UNUSED_ATTR static void build_codec_config(const tA2DP_SBC_CIE& config_cie,
                                           btav_a2dp_codec_config_t* result) {
  if (config_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_44)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  if (config_cie.samp_freq & A2DP_SBC_IE_SAMP_FREQ_48)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;

  result->bits_per_sample = config_cie.bits_per_sample;

  if (config_cie.ch_mode & A2DP_SBC_IE_CH_MD_MONO)
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;

  if (config_cie.ch_mode & (A2DP_SBC_IE_CH_MD_STEREO | A2DP_SBC_IE_CH_MD_JOINT |
                            A2DP_SBC_IE_CH_MD_DUAL)) {
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigSbcSource::A2dpCodecConfigSbcSource(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigSbcBase(BTAV_A2DP_CODEC_INDEX_SOURCE_SBC,
                             A2DP_CodecIndexStrSbc(), codec_priority, true) {
  // Compute the local capability
  if (a2dp_sbc_source_caps.samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (a2dp_sbc_source_caps.samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }
  codec_local_capability_.bits_per_sample =
      a2dp_sbc_source_caps.bits_per_sample;
  if (a2dp_sbc_source_caps.ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (a2dp_sbc_source_caps.ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (a2dp_sbc_source_caps.ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (a2dp_sbc_source_caps.ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigSbcSource::~A2dpCodecConfigSbcSource() {}

bool A2dpCodecConfigSbcSource::init() {
  if (!isValid()) return false;

  // Load the encoder
  if (!A2DP_LoadEncoderSbc()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the encoder", __func__);
    return false;
  }

  return true;
}

bool A2dpCodecConfigSbcSource::useRtpHeaderMarkerBit() const { return false; }

//
// Selects the best sample rate from |samp_freq|.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_sample_rate(uint8_t samp_freq, tA2DP_SBC_CIE* p_result,
                                    btav_a2dp_codec_config_t* p_codec_config) {
  if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
    p_result->samp_freq = A2DP_SBC_IE_SAMP_FREQ_48;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    return true;
  }
  if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
    p_result->samp_freq = A2DP_SBC_IE_SAMP_FREQ_44;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    return true;
  }
  return false;
}

//
// Selects the audio sample rate from |p_codec_audio_config|.
// |samp_freq| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_sample_rate(
    const btav_a2dp_codec_config_t* p_codec_audio_config, uint8_t samp_freq,
    tA2DP_SBC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
        p_result->samp_freq = A2DP_SBC_IE_SAMP_FREQ_44;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
        p_result->samp_freq = A2DP_SBC_IE_SAMP_FREQ_48;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_16000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_24000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_NONE:
      break;
  }

  return false;
}

//
// Selects the best bits per sample.
// The result is stored in |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_bits_per_sample(
    btav_a2dp_codec_config_t* p_codec_config) {
  p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
  return true;
}

//
// Selects the audio bits per sample from |p_codec_audio_config|.
// The result is stored in |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_bits_per_sample(
    const btav_a2dp_codec_config_t* p_codec_audio_config,
    btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
      return true;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      break;
  }
  return false;
}

//
// Selects the best channel mode from |ch_mode|.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_channel_mode(uint8_t ch_mode, tA2DP_SBC_CIE* p_result,
                                     btav_a2dp_codec_config_t* p_codec_config) {
  if (ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
    p_result->ch_mode = A2DP_SBC_IE_CH_MD_JOINT;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
    p_result->ch_mode = A2DP_SBC_IE_CH_MD_STEREO;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
    p_result->ch_mode = A2DP_SBC_IE_CH_MD_DUAL;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
    p_result->ch_mode = A2DP_SBC_IE_CH_MD_MONO;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    return true;
  }
  return false;
}

//
// Selects the audio channel mode from |p_codec_audio_config|.
// |ch_mode| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_channel_mode(
    const btav_a2dp_codec_config_t* p_codec_audio_config, uint8_t ch_mode,
    tA2DP_SBC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
        p_result->ch_mode = A2DP_SBC_IE_CH_MD_MONO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
        p_result->ch_mode = A2DP_SBC_IE_CH_MD_JOINT;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      if (ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
        p_result->ch_mode = A2DP_SBC_IE_CH_MD_STEREO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      if (ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
        p_result->ch_mode = A2DP_SBC_IE_CH_MD_DUAL;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_NONE:
      break;
  }

  return false;
}

bool A2dpCodecConfigSbcBase::setCodecConfig(const uint8_t* p_peer_codec_info,
                                            bool is_capability,
                                            uint8_t* p_result_codec_config) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_SBC_CIE peer_info_cie;
  tA2DP_SBC_CIE result_config_cie;
  uint8_t samp_freq;
  uint8_t ch_mode;
  uint8_t block_len;
  uint8_t num_subbands;
  uint8_t alloc_method;
  const tA2DP_SBC_CIE* p_a2dp_sbc_caps =
      (is_source_) ? &a2dp_sbc_source_caps : &a2dp_sbc_sink_caps;

  // Save the internal state
  btav_a2dp_codec_config_t saved_codec_config = codec_config_;
  btav_a2dp_codec_config_t saved_codec_capability = codec_capability_;
  btav_a2dp_codec_config_t saved_codec_selectable_capability =
      codec_selectable_capability_;
  btav_a2dp_codec_config_t saved_codec_user_config = codec_user_config_;
  btav_a2dp_codec_config_t saved_codec_audio_config = codec_audio_config_;
  uint8_t saved_ota_codec_config[AVDT_CODEC_SIZE];
  uint8_t saved_ota_codec_peer_capability[AVDT_CODEC_SIZE];
  uint8_t saved_ota_codec_peer_config[AVDT_CODEC_SIZE];
  memcpy(saved_ota_codec_config, ota_codec_config_, sizeof(ota_codec_config_));
  memcpy(saved_ota_codec_peer_capability, ota_codec_peer_capability_,
         sizeof(ota_codec_peer_capability_));
  memcpy(saved_ota_codec_peer_config, ota_codec_peer_config_,
         sizeof(ota_codec_peer_config_));

  tA2DP_STATUS status =
      A2DP_ParseInfoSbc(&peer_info_cie, p_peer_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }
  // Try using the prefered peer codec config (if valid), instead of the peer
  // capability.
  if (is_capability) {
    if (is_source_) {
      if (A2DP_IsPeerSinkCodecValidSbc(ota_codec_peer_config_)) {
        status =
            A2DP_ParseInfoSbc(&peer_info_cie, ota_codec_peer_config_, false);
      }
    } else {
      if (A2DP_IsPeerSourceCodecValidSbc(ota_codec_peer_config_)) {
        status =
            A2DP_ParseInfoSbc(&peer_info_cie, ota_codec_peer_config_, false);
      }
    }
    if (status != A2DP_SUCCESS) {
      // Use the peer codec capability
      status =
          A2DP_ParseInfoSbc(&peer_info_cie, p_peer_codec_info, is_capability);
      CHECK(status == A2DP_SUCCESS);
    }
  }

  //
  // Build the preferred configuration
  //
  memset(&result_config_cie, 0, sizeof(result_config_cie));

  //
  // Select the sample frequency
  //
  samp_freq = p_a2dp_sbc_caps->samp_freq & peer_info_cie.samp_freq;
  codec_config_.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
  switch (codec_user_config_.sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
        result_config_cie.samp_freq = A2DP_SBC_IE_SAMP_FREQ_44;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
        result_config_cie.samp_freq = A2DP_SBC_IE_SAMP_FREQ_48;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_16000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_24000:
    case BTAV_A2DP_CODEC_SAMPLE_RATE_NONE:
      codec_capability_.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
      codec_config_.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
      break;
  }

  // Select the sample frequency if there is no user preference
  do {
    // Compute the selectable capability
    if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    }
    if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    }

    if (codec_config_.sample_rate != BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) break;

    // Compute the common capability
    if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;

    // No user preference - try the codec audio config
    if (select_audio_sample_rate(&codec_audio_config_, samp_freq,
                                 &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_sample_rate(
            a2dp_sbc_default_config.samp_freq & peer_info_cie.samp_freq,
            &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_sample_rate(samp_freq, &result_config_cie,
                                &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.sample_rate == BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match sample frequency: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_sbc_caps->samp_freq, peer_info_cie.samp_freq);
    goto fail;
  }

  //
  // Select the bits per sample
  //
  // NOTE: this information is NOT included in the SBC A2DP codec description
  // that is sent OTA.
  codec_config_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
  switch (codec_user_config_.bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      codec_capability_.bits_per_sample = codec_user_config_.bits_per_sample;
      codec_config_.bits_per_sample = codec_user_config_.bits_per_sample;
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      codec_capability_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      codec_config_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      break;
  }

  // Select the bits per sample if there is no user preference
  do {
    // Compute the selectable capability
    codec_selectable_capability_.bits_per_sample =
        p_a2dp_sbc_caps->bits_per_sample;

    if (codec_config_.bits_per_sample != BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE)
      break;

    // Compute the common capability
    codec_capability_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;

    // No user preference - try the codec audio config
    if (select_audio_bits_per_sample(&codec_audio_config_, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_bits_per_sample(&codec_config_)) {
      break;
    }

    // No user preference - use the best match
    // TODO: no-op - temporary kept here for consistency
    if (select_best_bits_per_sample(&codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.bits_per_sample == BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match bits per sample: user preference = 0x%x",
              __func__, codec_user_config_.bits_per_sample);
    goto fail;
  }

  //
  // Select the channel mode
  //
  ch_mode = p_a2dp_sbc_caps->ch_mode & peer_info_cie.ch_mode;
  codec_config_.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;
  switch (codec_user_config_.channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
        result_config_cie.ch_mode = A2DP_SBC_IE_CH_MD_MONO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
        result_config_cie.ch_mode = A2DP_SBC_IE_CH_MD_JOINT;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
        break;
      }
      if (ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
        result_config_cie.ch_mode = A2DP_SBC_IE_CH_MD_STEREO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
        break;
      }
      if (ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
        result_config_cie.ch_mode = A2DP_SBC_IE_CH_MD_DUAL;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
        break;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_NONE:
      codec_capability_.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;
      codec_config_.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;
      break;
  }

  // Select the channel mode if there is no user preference
  do {
    // Compute the selectable capability
    if (ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    }
    if (ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }
    if (ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }
    if (ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    if (codec_config_.channel_mode != BTAV_A2DP_CODEC_CHANNEL_MODE_NONE) break;

    // Compute the common capability
    if (ch_mode & A2DP_SBC_IE_CH_MD_MONO)
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    if (ch_mode & (A2DP_SBC_IE_CH_MD_JOINT | A2DP_SBC_IE_CH_MD_STEREO |
                   A2DP_SBC_IE_CH_MD_DUAL)) {
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    // No user preference - use the codec audio config
    if (select_audio_channel_mode(&codec_audio_config_, ch_mode,
                                  &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_channel_mode(
            a2dp_sbc_default_config.ch_mode & peer_info_cie.ch_mode,
            &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_channel_mode(ch_mode, &result_config_cie, &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.channel_mode == BTAV_A2DP_CODEC_CHANNEL_MODE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match channel mode: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_sbc_caps->ch_mode, peer_info_cie.ch_mode);
    goto fail;
  }

  //
  // Select the block length
  //
  block_len = p_a2dp_sbc_caps->block_len & peer_info_cie.block_len;
  if (block_len & A2DP_SBC_IE_BLOCKS_16) {
    result_config_cie.block_len = A2DP_SBC_IE_BLOCKS_16;
  } else if (block_len & A2DP_SBC_IE_BLOCKS_12) {
    result_config_cie.block_len = A2DP_SBC_IE_BLOCKS_12;
  } else if (block_len & A2DP_SBC_IE_BLOCKS_8) {
    result_config_cie.block_len = A2DP_SBC_IE_BLOCKS_8;
  } else if (block_len & A2DP_SBC_IE_BLOCKS_4) {
    result_config_cie.block_len = A2DP_SBC_IE_BLOCKS_4;
  } else {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match block length: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_sbc_caps->block_len, peer_info_cie.block_len);
    goto fail;
  }

  //
  // Select the number of sub-bands
  //
  num_subbands = p_a2dp_sbc_caps->num_subbands & peer_info_cie.num_subbands;
  if (num_subbands & A2DP_SBC_IE_SUBBAND_8) {
    result_config_cie.num_subbands = A2DP_SBC_IE_SUBBAND_8;
  } else if (num_subbands & A2DP_SBC_IE_SUBBAND_4) {
    result_config_cie.num_subbands = A2DP_SBC_IE_SUBBAND_4;
  } else {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match number of sub-bands: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_sbc_caps->num_subbands,
              peer_info_cie.num_subbands);
    goto fail;
  }

  //
  // Select the allocation method
  //
  alloc_method = p_a2dp_sbc_caps->alloc_method & peer_info_cie.alloc_method;
  if (alloc_method & A2DP_SBC_IE_ALLOC_MD_L) {
    result_config_cie.alloc_method = A2DP_SBC_IE_ALLOC_MD_L;
  } else if (alloc_method & A2DP_SBC_IE_ALLOC_MD_S) {
    result_config_cie.alloc_method = A2DP_SBC_IE_ALLOC_MD_S;
  } else {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match allocation method: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_sbc_caps->alloc_method,
              peer_info_cie.alloc_method);
    goto fail;
  }

  //
  // Select the min/max bitpool
  //
  result_config_cie.min_bitpool = p_a2dp_sbc_caps->min_bitpool;
  if (result_config_cie.min_bitpool < peer_info_cie.min_bitpool)
    result_config_cie.min_bitpool = peer_info_cie.min_bitpool;
  result_config_cie.max_bitpool = p_a2dp_sbc_caps->max_bitpool;
  if (result_config_cie.max_bitpool > peer_info_cie.max_bitpool)
    result_config_cie.max_bitpool = peer_info_cie.max_bitpool;
  if (result_config_cie.min_bitpool > result_config_cie.max_bitpool) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match min/max bitpool: "
              "local caps min/max = 0x%x/0x%x peer info min/max = 0x%x/0x%x",
              __func__, p_a2dp_sbc_caps->min_bitpool,
              p_a2dp_sbc_caps->max_bitpool, peer_info_cie.min_bitpool,
              peer_info_cie.max_bitpool);
    goto fail;
  }

  if (A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
                        p_result_codec_config) != A2DP_SUCCESS) {
    goto fail;
  }

  //
  // Copy the codec-specific fields if they are not zero
  //
  if (codec_user_config_.codec_specific_1 != 0)
    codec_config_.codec_specific_1 = codec_user_config_.codec_specific_1;
  if (codec_user_config_.codec_specific_2 != 0)
    codec_config_.codec_specific_2 = codec_user_config_.codec_specific_2;
  if (codec_user_config_.codec_specific_3 != 0)
    codec_config_.codec_specific_3 = codec_user_config_.codec_specific_3;
  if (codec_user_config_.codec_specific_4 != 0)
    codec_config_.codec_specific_4 = codec_user_config_.codec_specific_4;

  // Create a local copy of the peer codec capability/config, and the
  // result codec config.
  if (is_capability) {
    status = A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                               ota_codec_peer_capability_);
  } else {
    status = A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                               ota_codec_peer_config_);
  }
  CHECK(status == A2DP_SUCCESS);
  status = A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
                             ota_codec_config_);
  CHECK(status == A2DP_SUCCESS);
  return true;

fail:
  // Restore the internal state
  codec_config_ = saved_codec_config;
  codec_capability_ = saved_codec_capability;
  codec_selectable_capability_ = saved_codec_selectable_capability;
  codec_user_config_ = saved_codec_user_config;
  codec_audio_config_ = saved_codec_audio_config;
  memcpy(ota_codec_config_, saved_ota_codec_config, sizeof(ota_codec_config_));
  memcpy(ota_codec_peer_capability_, saved_ota_codec_peer_capability,
         sizeof(ota_codec_peer_capability_));
  memcpy(ota_codec_peer_config_, saved_ota_codec_peer_config,
         sizeof(ota_codec_peer_config_));
  return false;
}

bool A2dpCodecConfigSbcBase::setPeerCodecCapabilities(
    const uint8_t* p_peer_codec_capabilities) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_SBC_CIE peer_info_cie;
  uint8_t samp_freq;
  uint8_t ch_mode;
  const tA2DP_SBC_CIE* p_a2dp_sbc_caps =
      (is_source_) ? &a2dp_sbc_source_caps : &a2dp_sbc_sink_caps;

  // Save the internal state
  btav_a2dp_codec_config_t saved_codec_selectable_capability =
      codec_selectable_capability_;
  uint8_t saved_ota_codec_peer_capability[AVDT_CODEC_SIZE];
  memcpy(saved_ota_codec_peer_capability, ota_codec_peer_capability_,
         sizeof(ota_codec_peer_capability_));

  tA2DP_STATUS status =
      A2DP_ParseInfoSbc(&peer_info_cie, p_peer_codec_capabilities, true);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }

  // Compute the selectable capability - sample rate
  samp_freq = p_a2dp_sbc_caps->samp_freq & peer_info_cie.samp_freq;
  if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_44) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (samp_freq & A2DP_SBC_IE_SAMP_FREQ_48) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }

  // Compute the selectable capability - bits per sample
  codec_selectable_capability_.bits_per_sample =
      p_a2dp_sbc_caps->bits_per_sample;

  // Compute the selectable capability - channel mode
  ch_mode = p_a2dp_sbc_caps->ch_mode & peer_info_cie.ch_mode;
  if (ch_mode & A2DP_SBC_IE_CH_MD_MONO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_JOINT) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_STEREO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (ch_mode & A2DP_SBC_IE_CH_MD_DUAL) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }

  status = A2DP_BuildInfoSbc(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                             ota_codec_peer_capability_);
  CHECK(status == A2DP_SUCCESS);
  return true;

fail:
  // Restore the internal state
  codec_selectable_capability_ = saved_codec_selectable_capability;
  memcpy(ota_codec_peer_capability_, saved_ota_codec_peer_capability,
         sizeof(ota_codec_peer_capability_));
  return false;
}

A2dpCodecConfigSbcSink::A2dpCodecConfigSbcSink(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigSbcBase(BTAV_A2DP_CODEC_INDEX_SINK_SBC,
                             A2DP_CodecIndexStrSbcSink(), codec_priority,
                             false) {}

A2dpCodecConfigSbcSink::~A2dpCodecConfigSbcSink() {}

bool A2dpCodecConfigSbcSink::init() {
  if (!isValid()) return false;

  // Load the decoder
  if (!A2DP_LoadDecoderSbc()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the decoder", __func__);
    return false;
  }

  return true;
}

bool A2dpCodecConfigSbcSink::useRtpHeaderMarkerBit() const {
  // TODO: This method applies only to Source codecs
  return false;
}

bool A2dpCodecConfigSbcSink::updateEncoderUserConfig(
    UNUSED_ATTR const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
    UNUSED_ATTR bool* p_restart_input, UNUSED_ATTR bool* p_restart_output,
    UNUSED_ATTR bool* p_config_updated) {
  // TODO: This method applies only to Source codecs
  return false;
}

uint64_t A2dpCodecConfigSbcSink::encoderIntervalMs() const {
  // TODO: This method applies only to Source codecs
  return 0;
}

int A2dpCodecConfigSbcSink::getEffectiveMtu() const {
  // TODO: This method applies only to Source codecs
  return 0;
}
