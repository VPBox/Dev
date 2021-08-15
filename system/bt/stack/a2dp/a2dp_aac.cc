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

/******************************************************************************
 *
 *  Utility functions to help build and parse the AAC Codec Information
 *  Element and Media Payload.
 *
 ******************************************************************************/

#define LOG_TAG "a2dp_aac"

#include "bt_target.h"

#include "a2dp_aac.h"

#include <string.h>

#include <base/logging.h>
#include "a2dp_aac_decoder.h"
#include "a2dp_aac_encoder.h"
#include "bt_utils.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

#define A2DP_AAC_DEFAULT_BITRATE 320000  // 320 kbps
#define A2DP_AAC_MIN_BITRATE 64000       // 64 kbps

// data type for the AAC Codec Information Element */
// NOTE: bits_per_sample is needed only for AAC encoder initialization.
typedef struct {
  uint8_t objectType;             /* Object Type */
  uint16_t sampleRate;            /* Sampling Frequency */
  uint8_t channelMode;            /* STEREO/MONO */
  uint8_t variableBitRateSupport; /* Variable Bit Rate Support*/
  uint32_t bitRate;               /* Bit rate */
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
} tA2DP_AAC_CIE;

/* AAC Source codec capabilities */
static const tA2DP_AAC_CIE a2dp_aac_source_caps = {
    // objectType
    A2DP_AAC_OBJECT_TYPE_MPEG2_LC,
    // sampleRate
    // TODO: AAC 48.0kHz sampling rate should be added back - see b/62301376
    A2DP_AAC_SAMPLING_FREQ_44100,
    // channelMode
    A2DP_AAC_CHANNEL_MODE_STEREO,
    // variableBitRateSupport
    A2DP_AAC_VARIABLE_BIT_RATE_DISABLED,
    // bitRate
    A2DP_AAC_DEFAULT_BITRATE,
    // bits_per_sample
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16};

/* AAC Sink codec capabilities */
static const tA2DP_AAC_CIE a2dp_aac_sink_caps = {
    // objectType
    A2DP_AAC_OBJECT_TYPE_MPEG2_LC,
    // sampleRate
    A2DP_AAC_SAMPLING_FREQ_44100 | A2DP_AAC_SAMPLING_FREQ_48000,
    // channelMode
    A2DP_AAC_CHANNEL_MODE_MONO | A2DP_AAC_CHANNEL_MODE_STEREO,
    // variableBitRateSupport
    A2DP_AAC_VARIABLE_BIT_RATE_ENABLED,
    // bitRate
    A2DP_AAC_DEFAULT_BITRATE,
    // bits_per_sample
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16};

/* Default AAC codec configuration */
static const tA2DP_AAC_CIE a2dp_aac_default_config = {
    A2DP_AAC_OBJECT_TYPE_MPEG2_LC,        // objectType
    A2DP_AAC_SAMPLING_FREQ_44100,         // sampleRate
    A2DP_AAC_CHANNEL_MODE_STEREO,         // channelMode
    A2DP_AAC_VARIABLE_BIT_RATE_DISABLED,  // variableBitRateSupport
    A2DP_AAC_DEFAULT_BITRATE,             // bitRate
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16    // bits_per_sample
};

static const tA2DP_ENCODER_INTERFACE a2dp_encoder_interface_aac = {
    a2dp_aac_encoder_init,
    a2dp_aac_encoder_cleanup,
    a2dp_aac_feeding_reset,
    a2dp_aac_feeding_flush,
    a2dp_aac_get_encoder_interval_ms,
    a2dp_aac_send_frames,
    nullptr  // set_transmit_queue_length
};

static const tA2DP_DECODER_INTERFACE a2dp_decoder_interface_aac = {
    a2dp_aac_decoder_init, a2dp_aac_decoder_cleanup,
    a2dp_aac_decoder_decode_packet,
};

UNUSED_ATTR static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilityAac(
    const tA2DP_AAC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_capability);

// Builds the AAC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. |media_type| is the media type |AVDT_MEDIA_TYPE_*|.
// |p_ie| is a pointer to the AAC Codec Information Element information.
// The result is stored in |p_result|. Returns A2DP_SUCCESS on success,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_BuildInfoAac(uint8_t media_type,
                                      const tA2DP_AAC_CIE* p_ie,
                                      uint8_t* p_result) {
  if (p_ie == NULL || p_result == NULL) {
    return A2DP_INVALID_PARAMS;
  }

  *p_result++ = A2DP_AAC_CODEC_LEN;
  *p_result++ = (media_type << 4);
  *p_result++ = A2DP_MEDIA_CT_AAC;

  // Object Type
  if (p_ie->objectType == 0) return A2DP_INVALID_PARAMS;
  *p_result++ = p_ie->objectType;

  // Sampling Frequency
  if (p_ie->sampleRate == 0) return A2DP_INVALID_PARAMS;
  *p_result++ = (uint8_t)(p_ie->sampleRate & A2DP_AAC_SAMPLING_FREQ_MASK0);
  *p_result = (uint8_t)((p_ie->sampleRate & A2DP_AAC_SAMPLING_FREQ_MASK1) >> 8);

  // Channel Mode
  if (p_ie->channelMode == 0) return A2DP_INVALID_PARAMS;
  *p_result++ |= (p_ie->channelMode & A2DP_AAC_CHANNEL_MODE_MASK);

  // Variable Bit Rate Support
  *p_result = (p_ie->variableBitRateSupport & A2DP_AAC_VARIABLE_BIT_RATE_MASK);

  // Bit Rate
  *p_result++ |= (uint8_t)((p_ie->bitRate & A2DP_AAC_BIT_RATE_MASK0) >> 16);
  *p_result++ = (uint8_t)((p_ie->bitRate & A2DP_AAC_BIT_RATE_MASK1) >> 8);
  *p_result++ = (uint8_t)(p_ie->bitRate & A2DP_AAC_BIT_RATE_MASK2);

  return A2DP_SUCCESS;
}

// Parses the AAC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. The result is stored in |p_ie|. The byte sequence to parse is
// |p_codec_info|. If |is_capability| is true, the byte sequence is
// codec capabilities, otherwise is codec configuration.
// Returns A2DP_SUCCESS on success, otherwise the corresponding A2DP error
// status code.
static tA2DP_STATUS A2DP_ParseInfoAac(tA2DP_AAC_CIE* p_ie,
                                      const uint8_t* p_codec_info,
                                      bool is_capability) {
  uint8_t losc;
  uint8_t media_type;
  tA2DP_CODEC_TYPE codec_type;

  if (p_ie == NULL || p_codec_info == NULL) return A2DP_INVALID_PARAMS;

  // Check the codec capability length
  losc = *p_codec_info++;
  if (losc != A2DP_AAC_CODEC_LEN) return A2DP_WRONG_CODEC;

  media_type = (*p_codec_info++) >> 4;
  codec_type = *p_codec_info++;
  /* Check the Media Type and Media Codec Type */
  if (media_type != AVDT_MEDIA_TYPE_AUDIO || codec_type != A2DP_MEDIA_CT_AAC) {
    return A2DP_WRONG_CODEC;
  }

  p_ie->objectType = *p_codec_info++;
  p_ie->sampleRate = (*p_codec_info & A2DP_AAC_SAMPLING_FREQ_MASK0) |
                     (*(p_codec_info + 1) << 8 & A2DP_AAC_SAMPLING_FREQ_MASK1);
  p_codec_info++;
  p_ie->channelMode = *p_codec_info & A2DP_AAC_CHANNEL_MODE_MASK;
  p_codec_info++;

  p_ie->variableBitRateSupport =
      *p_codec_info & A2DP_AAC_VARIABLE_BIT_RATE_MASK;

  p_ie->bitRate = ((*p_codec_info) << 16 & A2DP_AAC_BIT_RATE_MASK0) |
                  (*(p_codec_info + 1) << 8 & A2DP_AAC_BIT_RATE_MASK1) |
                  (*(p_codec_info + 2) & A2DP_AAC_BIT_RATE_MASK2);
  p_codec_info += 3;

  if (is_capability) {
    // NOTE: The checks here are very liberal. We should be using more
    // pedantic checks specific to the SRC or SNK as specified in the spec.
    if (A2DP_BitsSet(p_ie->objectType) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_OBJ_TYPE;
    if (A2DP_BitsSet(p_ie->sampleRate) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_SAMP_FREQ;
    if (A2DP_BitsSet(p_ie->channelMode) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_CH_MODE;

    return A2DP_SUCCESS;
  }

  if (A2DP_BitsSet(p_ie->objectType) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_OBJ_TYPE;
  if (A2DP_BitsSet(p_ie->sampleRate) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_SAMP_FREQ;
  if (A2DP_BitsSet(p_ie->channelMode) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_CH_MODE;

  return A2DP_SUCCESS;
}

bool A2DP_IsSourceCodecValidAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsSinkCodecValidAac(UNUSED_ATTR const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsPeerSourceCodecValidAac(UNUSED_ATTR const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsPeerSinkCodecValidAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsSinkCodecSupportedAac(const uint8_t* p_codec_info) {
  return A2DP_CodecInfoMatchesCapabilityAac(&a2dp_aac_sink_caps, p_codec_info,
                                            false) == A2DP_SUCCESS;
}

bool A2DP_IsPeerSourceCodecSupportedAac(const uint8_t* p_codec_info) {
  return A2DP_CodecInfoMatchesCapabilityAac(&a2dp_aac_sink_caps, p_codec_info,
                                            true) == A2DP_SUCCESS;
}

// Checks whether A2DP AAC codec configuration matches with a device's codec
// capabilities. |p_cap| is the AAC codec configuration. |p_codec_info| is
// the device's codec capabilities. |is_capability| is true if
// |p_codec_info| contains A2DP codec capability.
// Returns A2DP_SUCCESS if the codec configuration matches with capabilities,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilityAac(
    const tA2DP_AAC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_capability) {
  tA2DP_STATUS status;
  tA2DP_AAC_CIE cfg_cie;

  /* parse configuration */
  status = A2DP_ParseInfoAac(&cfg_cie, p_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: parsing failed %d", __func__, status);
    return status;
  }

  /* verify that each parameter is in range */

  LOG_VERBOSE(LOG_TAG, "%s: Object Type peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.objectType, p_cap->objectType);
  LOG_VERBOSE(LOG_TAG, "%s: Sample Rate peer: %u, capability %u", __func__,
              cfg_cie.sampleRate, p_cap->sampleRate);
  LOG_VERBOSE(LOG_TAG, "%s: Channel Mode peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.channelMode, p_cap->channelMode);
  LOG_VERBOSE(
      LOG_TAG, "%s: Variable Bit Rate Support peer: 0x%x, capability 0x%x",
      __func__, cfg_cie.variableBitRateSupport, p_cap->variableBitRateSupport);
  LOG_VERBOSE(LOG_TAG, "%s: Bit Rate peer: %u, capability %u", __func__,
              cfg_cie.bitRate, p_cap->bitRate);

  /* Object Type */
  if ((cfg_cie.objectType & p_cap->objectType) == 0) return A2DP_BAD_OBJ_TYPE;

  /* Sample Rate */
  if ((cfg_cie.sampleRate & p_cap->sampleRate) == 0) return A2DP_BAD_SAMP_FREQ;

  /* Channel Mode */
  if ((cfg_cie.channelMode & p_cap->channelMode) == 0) return A2DP_NS_CH_MODE;

  return A2DP_SUCCESS;
}

bool A2DP_UsesRtpHeaderAac(UNUSED_ATTR bool content_protection_enabled,
                           UNUSED_ATTR const uint8_t* p_codec_info) {
  return true;
}

const char* A2DP_CodecNameAac(UNUSED_ATTR const uint8_t* p_codec_info) {
  return "AAC";
}

bool A2DP_CodecTypeEqualsAac(const uint8_t* p_codec_info_a,
                             const uint8_t* p_codec_info_b) {
  tA2DP_AAC_CIE aac_cie_a;
  tA2DP_AAC_CIE aac_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoAac(&aac_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoAac(&aac_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  return true;
}

bool A2DP_CodecEqualsAac(const uint8_t* p_codec_info_a,
                         const uint8_t* p_codec_info_b) {
  tA2DP_AAC_CIE aac_cie_a;
  tA2DP_AAC_CIE aac_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoAac(&aac_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoAac(&aac_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  return (aac_cie_a.objectType == aac_cie_b.objectType) &&
         (aac_cie_a.sampleRate == aac_cie_b.sampleRate) &&
         (aac_cie_a.channelMode == aac_cie_b.channelMode) &&
         (aac_cie_a.variableBitRateSupport ==
          aac_cie_b.variableBitRateSupport) &&
         (aac_cie_a.bitRate == aac_cie_b.bitRate);
}

int A2DP_GetTrackSampleRateAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.sampleRate) {
    case A2DP_AAC_SAMPLING_FREQ_8000:
      return 8000;
    case A2DP_AAC_SAMPLING_FREQ_11025:
      return 11025;
    case A2DP_AAC_SAMPLING_FREQ_12000:
      return 12000;
    case A2DP_AAC_SAMPLING_FREQ_16000:
      return 16000;
    case A2DP_AAC_SAMPLING_FREQ_22050:
      return 22050;
    case A2DP_AAC_SAMPLING_FREQ_24000:
      return 24000;
    case A2DP_AAC_SAMPLING_FREQ_32000:
      return 32000;
    case A2DP_AAC_SAMPLING_FREQ_44100:
      return 44100;
    case A2DP_AAC_SAMPLING_FREQ_48000:
      return 48000;
    case A2DP_AAC_SAMPLING_FREQ_64000:
      return 64000;
    case A2DP_AAC_SAMPLING_FREQ_88200:
      return 88200;
    case A2DP_AAC_SAMPLING_FREQ_96000:
      return 96000;
  }

  return -1;
}

int A2DP_GetTrackBitsPerSampleAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  // NOTE: The bits per sample never changes for AAC
  return 16;
}

int A2DP_GetTrackChannelCountAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.channelMode) {
    case A2DP_AAC_CHANNEL_MODE_MONO:
      return 1;
    case A2DP_AAC_CHANNEL_MODE_STEREO:
      return 2;
  }

  return -1;
}

int A2DP_GetSinkTrackChannelTypeAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.channelMode) {
    case A2DP_AAC_CHANNEL_MODE_MONO:
      return 1;
    case A2DP_AAC_CHANNEL_MODE_STEREO:
      return 3;
  }

  return -1;
}

int A2DP_GetObjectTypeCodeAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.objectType) {
    case A2DP_AAC_OBJECT_TYPE_MPEG2_LC:
    case A2DP_AAC_OBJECT_TYPE_MPEG4_LC:
    case A2DP_AAC_OBJECT_TYPE_MPEG4_LTP:
    case A2DP_AAC_OBJECT_TYPE_MPEG4_SCALABLE:
      return aac_cie.objectType;
    default:
      break;
  }

  return -1;
}

int A2DP_GetChannelModeCodeAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.channelMode) {
    case A2DP_AAC_CHANNEL_MODE_MONO:
    case A2DP_AAC_CHANNEL_MODE_STEREO:
      return aac_cie.channelMode;
    default:
      break;
  }

  return -1;
}

int A2DP_GetVariableBitRateSupportAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (aac_cie.variableBitRateSupport) {
    case A2DP_AAC_VARIABLE_BIT_RATE_ENABLED:
    case A2DP_AAC_VARIABLE_BIT_RATE_DISABLED:
      return aac_cie.variableBitRateSupport;
    default:
      break;
  }

  return -1;
}

int A2DP_GetBitRateAac(const uint8_t* p_codec_info) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  return aac_cie.bitRate;
}

int A2DP_ComputeMaxBitRateAac(const uint8_t* p_codec_info, uint16_t mtu) {
  tA2DP_AAC_CIE aac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  int sampling_freq = A2DP_GetTrackSampleRateAac(p_codec_info);
  if (sampling_freq == -1) return -1;

  int pcm_channel_samples_per_frame = 0;
  switch (aac_cie.objectType) {
    case A2DP_AAC_OBJECT_TYPE_MPEG2_LC:
    case A2DP_AAC_OBJECT_TYPE_MPEG4_LC:
      pcm_channel_samples_per_frame = 1024;
      break;
    case A2DP_AAC_OBJECT_TYPE_MPEG4_LTP:
    case A2DP_AAC_OBJECT_TYPE_MPEG4_SCALABLE:
      // TODO: The MPEG documentation doesn't specify the value.
      break;
    default:
      break;
  }
  if (pcm_channel_samples_per_frame == 0) return -1;

  // See Section 3.2.1 Estimating Average Frame Size from
  // the aacEncoder.pdf document included with the AAC source code.
  return (8 * mtu * sampling_freq) / pcm_channel_samples_per_frame;
}

bool A2DP_GetPacketTimestampAac(const uint8_t* p_codec_info,
                                const uint8_t* p_data, uint32_t* p_timestamp) {
  // TODO: Is this function really codec-specific?
  *p_timestamp = *(const uint32_t*)p_data;
  return true;
}

bool A2DP_BuildCodecHeaderAac(UNUSED_ATTR const uint8_t* p_codec_info,
                              UNUSED_ATTR BT_HDR* p_buf,
                              UNUSED_ATTR uint16_t frames_per_packet) {
  return true;
}

std::string A2DP_CodecInfoStringAac(const uint8_t* p_codec_info) {
  std::stringstream res;
  std::string field;
  tA2DP_STATUS a2dp_status;
  tA2DP_AAC_CIE aac_cie;

  a2dp_status = A2DP_ParseInfoAac(&aac_cie, p_codec_info, true);
  if (a2dp_status != A2DP_SUCCESS) {
    res << "A2DP_ParseInfoAac fail: " << loghex(a2dp_status);
    return res.str();
  }

  res << "\tname: AAC\n";

  // Object type
  field.clear();
  AppendField(&field, (aac_cie.objectType == 0), "NONE");
  AppendField(&field, (aac_cie.objectType & A2DP_AAC_OBJECT_TYPE_MPEG2_LC),
              "(MPEG-2 AAC LC)");
  AppendField(&field, (aac_cie.objectType & A2DP_AAC_OBJECT_TYPE_MPEG4_LC),
              "(MPEG-4 AAC LC)");
  AppendField(&field, (aac_cie.objectType & A2DP_AAC_OBJECT_TYPE_MPEG4_LTP),
              "(MPEG-4 AAC LTP)");
  AppendField(&field,
              (aac_cie.objectType & A2DP_AAC_OBJECT_TYPE_MPEG4_SCALABLE),
              "(MPEG-4 AAC Scalable)");
  res << "\tobjectType: " << field << " (" << loghex(aac_cie.objectType)
      << ")\n";

  // Sample frequency
  field.clear();
  AppendField(&field, (aac_cie.sampleRate == 0), "NONE");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_8000),
              "8000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_11025),
              "11025");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_12000),
              "12000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_16000),
              "16000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_22050),
              "22050");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_24000),
              "24000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_32000),
              "32000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_44100),
              "44100");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_48000),
              "48000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_64000),
              "64000");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_88200),
              "88200");
  AppendField(&field, (aac_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_96000),
              "96000");
  res << "\tsamp_freq: " << field << " (" << loghex(aac_cie.sampleRate)
      << ")\n";

  // Channel mode
  field.clear();
  AppendField(&field, (aac_cie.channelMode == 0), "NONE");
  AppendField(&field, (aac_cie.channelMode == A2DP_AAC_CHANNEL_MODE_MONO),
              "Mono");
  AppendField(&field, (aac_cie.channelMode == A2DP_AAC_CHANNEL_MODE_STEREO),
              "Stereo");
  res << "\tch_mode: " << field << " (" << loghex(aac_cie.channelMode) << ")\n";

  // Variable bit rate support
  res << "\tvariableBitRateSupport: " << std::boolalpha
      << (aac_cie.variableBitRateSupport != 0) << "\n";

  // Bit rate
  res << "\tbitRate: " << std::to_string(aac_cie.bitRate) << "\n";

  return res.str();
}

const tA2DP_ENCODER_INTERFACE* A2DP_GetEncoderInterfaceAac(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsSourceCodecValidAac(p_codec_info)) return NULL;

  return &a2dp_encoder_interface_aac;
}

const tA2DP_DECODER_INTERFACE* A2DP_GetDecoderInterfaceAac(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsSinkCodecValidAac(p_codec_info)) return NULL;

  return &a2dp_decoder_interface_aac;
}

bool A2DP_AdjustCodecAac(uint8_t* p_codec_info) {
  tA2DP_AAC_CIE cfg_cie;

  // Nothing to do: just verify the codec info is valid
  if (A2DP_ParseInfoAac(&cfg_cie, p_codec_info, true) != A2DP_SUCCESS)
    return false;

  return true;
}

btav_a2dp_codec_index_t A2DP_SourceCodecIndexAac(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SOURCE_AAC;
}

btav_a2dp_codec_index_t A2DP_SinkCodecIndexAac(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SINK_AAC;
}

const char* A2DP_CodecIndexStrAac(void) { return "AAC"; }

const char* A2DP_CodecIndexStrAacSink(void) { return "AAC SINK"; }

bool A2DP_InitCodecConfigAac(AvdtpSepConfig* p_cfg) {
  if (A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &a2dp_aac_source_caps,
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

bool A2DP_InitCodecConfigAacSink(AvdtpSepConfig* p_cfg) {
  return A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &a2dp_aac_sink_caps,
                           p_cfg->codec_info) == A2DP_SUCCESS;
}

UNUSED_ATTR static void build_codec_config(const tA2DP_AAC_CIE& config_cie,
                                           btav_a2dp_codec_config_t* result) {
  if (config_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_44100)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  if (config_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_48000)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  if (config_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_88200)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  if (config_cie.sampleRate & A2DP_AAC_SAMPLING_FREQ_96000)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;

  result->bits_per_sample = config_cie.bits_per_sample;

  if (config_cie.channelMode & A2DP_AAC_CHANNEL_MODE_MONO)
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  if (config_cie.channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigAacSource::A2dpCodecConfigAacSource(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigAacBase(BTAV_A2DP_CODEC_INDEX_SOURCE_AAC,
                             A2DP_CodecIndexStrAac(), codec_priority, true) {
  // Compute the local capability
  if (a2dp_aac_source_caps.sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (a2dp_aac_source_caps.sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }
  if (a2dp_aac_source_caps.sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  }
  if (a2dp_aac_source_caps.sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
  }
  codec_local_capability_.bits_per_sample =
      a2dp_aac_source_caps.bits_per_sample;
  if (a2dp_aac_source_caps.channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (a2dp_aac_source_caps.channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigAacSource::~A2dpCodecConfigAacSource() {}

bool A2dpCodecConfigAacSource::init() {
  if (!isValid()) return false;

  // Load the encoder
  if (!A2DP_LoadEncoderAac()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the encoder", __func__);
    return false;
  }

  return true;
}

bool A2dpCodecConfigAacSource::useRtpHeaderMarkerBit() const { return true; }

//
// Selects the best sample rate from |sampleRate|.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_sample_rate(uint16_t sampleRate,
                                    tA2DP_AAC_CIE* p_result,
                                    btav_a2dp_codec_config_t* p_codec_config) {
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
    p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_96000;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
    return true;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
    p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_88200;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    return true;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
    p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_48000;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    return true;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
    p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_44100;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    return true;
  }
  return false;
}

//
// Selects the audio sample rate from |p_codec_audio_config|.
// |sampleRate| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_sample_rate(
    const btav_a2dp_codec_config_t* p_codec_audio_config, uint16_t sampleRate,
    tA2DP_AAC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
        p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_44100;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
        p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_48000;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
        p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_88200;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
        p_result->sampleRate = A2DP_AAC_SAMPLING_FREQ_96000;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
        return true;
      }
      break;
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
// Selects the best bits per sample from |bits_per_sample|.
// |bits_per_sample| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_bits_per_sample(
    btav_a2dp_codec_bits_per_sample_t bits_per_sample, tA2DP_AAC_CIE* p_result,
    btav_a2dp_codec_config_t* p_codec_config) {
  if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32) {
    p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32;
    p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32;
    return true;
  }
  if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24) {
    p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24;
    p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24;
    return true;
  }
  if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16) {
    p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
    p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
    return true;
  }
  return false;
}

//
// Selects the audio bits per sample from |p_codec_audio_config|.
// |bits_per_sample| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_bits_per_sample(
    const btav_a2dp_codec_config_t* p_codec_audio_config,
    btav_a2dp_codec_bits_per_sample_t bits_per_sample, tA2DP_AAC_CIE* p_result,
    btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16) {
        p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
        p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24) {
        p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24;
        p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32) {
        p_codec_config->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32;
        p_result->bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      break;
  }
  return false;
}

//
// Selects the best channel mode from |channelMode|.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_channel_mode(uint8_t channelMode,
                                     tA2DP_AAC_CIE* p_result,
                                     btav_a2dp_codec_config_t* p_codec_config) {
  if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
    p_result->channelMode = A2DP_AAC_CHANNEL_MODE_STEREO;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
    p_result->channelMode = A2DP_AAC_CHANNEL_MODE_MONO;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    return true;
  }
  return false;
}

//
// Selects the audio channel mode from |p_codec_audio_config|.
// |channelMode| contains the capability.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_audio_channel_mode(
    const btav_a2dp_codec_config_t* p_codec_audio_config, uint8_t channelMode,
    tA2DP_AAC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
        p_result->channelMode = A2DP_AAC_CHANNEL_MODE_MONO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
        p_result->channelMode = A2DP_AAC_CHANNEL_MODE_STEREO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_NONE:
      break;
  }

  return false;
}

bool A2dpCodecConfigAacBase::setCodecConfig(const uint8_t* p_peer_codec_info,
                                            bool is_capability,
                                            uint8_t* p_result_codec_config) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_AAC_CIE peer_info_cie;
  tA2DP_AAC_CIE result_config_cie;
  uint8_t channelMode;
  uint16_t sampleRate;
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
  const tA2DP_AAC_CIE* p_a2dp_aac_caps =
      (is_source_) ? &a2dp_aac_source_caps : &a2dp_aac_sink_caps;

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
      A2DP_ParseInfoAac(&peer_info_cie, p_peer_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }

  //
  // Build the preferred configuration
  //
  memset(&result_config_cie, 0, sizeof(result_config_cie));

  // NOTE: Always assign the Object Type and Variable Bit Rate Support.
  result_config_cie.objectType = p_a2dp_aac_caps->objectType;
  // The Variable Bit Rate Support is disabled if either side disables it
  result_config_cie.variableBitRateSupport =
      p_a2dp_aac_caps->variableBitRateSupport &
      peer_info_cie.variableBitRateSupport;

  // Set the bit rate as follows:
  // 1. If the remote device reports a bogus bit rate
  //    (bitRate < A2DP_AAC_MIN_BITRATE), then use the bit rate from our
  //    configuration. Examples of observed bogus bit rates are zero
  //    and 24576.
  // 2. If the remote device reports valid bit rate
  //    (bitRate >= A2DP_AAC_MIN_BITRATE), then use the smaller
  //    of the remote device's bit rate and the bit rate from our configuration.
  // In either case, the actual streaming bit rate will also consider the MTU.
  if (peer_info_cie.bitRate < A2DP_AAC_MIN_BITRATE) {
    // Bogus bit rate
    result_config_cie.bitRate = p_a2dp_aac_caps->bitRate;
  } else {
    result_config_cie.bitRate =
        std::min(p_a2dp_aac_caps->bitRate, peer_info_cie.bitRate);
  }

  //
  // Select the sample frequency
  //
  sampleRate = p_a2dp_aac_caps->sampleRate & peer_info_cie.sampleRate;
  codec_config_.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
  switch (codec_user_config_.sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
        result_config_cie.sampleRate = A2DP_AAC_SAMPLING_FREQ_44100;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
        result_config_cie.sampleRate = A2DP_AAC_SAMPLING_FREQ_48000;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
        result_config_cie.sampleRate = A2DP_AAC_SAMPLING_FREQ_88200;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
        result_config_cie.sampleRate = A2DP_AAC_SAMPLING_FREQ_96000;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
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
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    }
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    }
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    }
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
    }

    if (codec_config_.sample_rate != BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) break;

    // Compute the common capability
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;

    // No user preference - try the codec audio config
    if (select_audio_sample_rate(&codec_audio_config_, sampleRate,
                                 &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_sample_rate(
            a2dp_aac_default_config.sampleRate & peer_info_cie.sampleRate,
            &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_sample_rate(sampleRate, &result_config_cie,
                                &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.sample_rate == BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match sample frequency: source caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_aac_caps->sampleRate, peer_info_cie.sampleRate);
    goto fail;
  }

  //
  // Select the bits per sample
  //
  // NOTE: this information is NOT included in the AAC A2DP codec description
  // that is sent OTA.
  bits_per_sample = p_a2dp_aac_caps->bits_per_sample;
  codec_config_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
  switch (codec_user_config_.bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16) {
        result_config_cie.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_capability_.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_config_.bits_per_sample = codec_user_config_.bits_per_sample;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24) {
        result_config_cie.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_capability_.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_config_.bits_per_sample = codec_user_config_.bits_per_sample;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
      if (bits_per_sample & BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32) {
        result_config_cie.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_capability_.bits_per_sample = codec_user_config_.bits_per_sample;
        codec_config_.bits_per_sample = codec_user_config_.bits_per_sample;
      }
      break;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      result_config_cie.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      codec_capability_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      codec_config_.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      break;
  }

  // Select the bits per sample if there is no user preference
  do {
    // Compute the selectable capability
    codec_selectable_capability_.bits_per_sample =
        p_a2dp_aac_caps->bits_per_sample;

    if (codec_config_.bits_per_sample != BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE)
      break;

    // Compute the common capability
    codec_capability_.bits_per_sample = bits_per_sample;

    // No user preference - the the codec audio config
    if (select_audio_bits_per_sample(&codec_audio_config_,
                                     p_a2dp_aac_caps->bits_per_sample,
                                     &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_bits_per_sample(a2dp_aac_default_config.bits_per_sample,
                                    &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_bits_per_sample(p_a2dp_aac_caps->bits_per_sample,
                                    &result_config_cie, &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.bits_per_sample == BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match bits per sample: default = 0x%x "
              "user preference = 0x%x",
              __func__, a2dp_aac_default_config.bits_per_sample,
              codec_user_config_.bits_per_sample);
    goto fail;
  }

  //
  // Select the channel mode
  //
  channelMode = p_a2dp_aac_caps->channelMode & peer_info_cie.channelMode;
  codec_config_.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;
  switch (codec_user_config_.channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
        result_config_cie.channelMode = A2DP_AAC_CHANNEL_MODE_MONO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
        result_config_cie.channelMode = A2DP_AAC_CHANNEL_MODE_STEREO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
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
    if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    }
    if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    if (codec_config_.channel_mode != BTAV_A2DP_CODEC_CHANNEL_MODE_NONE) break;

    // Compute the common capability
    if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO)
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    // No user preference - try the codec audio config
    if (select_audio_channel_mode(&codec_audio_config_, channelMode,
                                  &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_channel_mode(
            a2dp_aac_default_config.channelMode & peer_info_cie.channelMode,
            &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_channel_mode(channelMode, &result_config_cie,
                                 &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.channel_mode == BTAV_A2DP_CODEC_CHANNEL_MODE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match channel mode: source caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_aac_caps->channelMode,
              peer_info_cie.channelMode);
    goto fail;
  }

  if (A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
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
    status = A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                               ota_codec_peer_capability_);
  } else {
    status = A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                               ota_codec_peer_config_);
  }
  CHECK(status == A2DP_SUCCESS);
  status = A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
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

bool A2dpCodecConfigAacBase::setPeerCodecCapabilities(
    const uint8_t* p_peer_codec_capabilities) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_AAC_CIE peer_info_cie;
  uint8_t channelMode;
  uint16_t sampleRate;
  const tA2DP_AAC_CIE* p_a2dp_aac_caps =
      (is_source_) ? &a2dp_aac_source_caps : &a2dp_aac_sink_caps;

  // Save the internal state
  btav_a2dp_codec_config_t saved_codec_selectable_capability =
      codec_selectable_capability_;
  uint8_t saved_ota_codec_peer_capability[AVDT_CODEC_SIZE];
  memcpy(saved_ota_codec_peer_capability, ota_codec_peer_capability_,
         sizeof(ota_codec_peer_capability_));

  tA2DP_STATUS status =
      A2DP_ParseInfoAac(&peer_info_cie, p_peer_codec_capabilities, true);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }

  // Compute the selectable capability - sample rate
  sampleRate = p_a2dp_aac_caps->sampleRate & peer_info_cie.sampleRate;
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_44100) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_48000) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_88200) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  }
  if (sampleRate & A2DP_AAC_SAMPLING_FREQ_96000) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
  }

  // Compute the selectable capability - bits per sample
  codec_selectable_capability_.bits_per_sample =
      p_a2dp_aac_caps->bits_per_sample;

  // Compute the selectable capability - channel mode
  channelMode = p_a2dp_aac_caps->channelMode & peer_info_cie.channelMode;
  if (channelMode & A2DP_AAC_CHANNEL_MODE_MONO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (channelMode & A2DP_AAC_CHANNEL_MODE_STEREO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }

  status = A2DP_BuildInfoAac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
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

A2dpCodecConfigAacSink::A2dpCodecConfigAacSink(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigAacBase(BTAV_A2DP_CODEC_INDEX_SINK_AAC,
                             A2DP_CodecIndexStrAacSink(), codec_priority,
                             false) {}

A2dpCodecConfigAacSink::~A2dpCodecConfigAacSink() {}

bool A2dpCodecConfigAacSink::init() {
  if (!isValid()) return false;

  // Load the decoder
  if (!A2DP_LoadDecoderAac()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the decoder", __func__);
    return false;
  }

  return true;
}

uint64_t A2dpCodecConfigAacSink::encoderIntervalMs() const {
  // TODO: This method applies only to Source codecs
  return 0;
}

int A2dpCodecConfigAacSink::getEffectiveMtu() const {
  // TODO: This method applies only to Source codecs
  return 0;
}

bool A2dpCodecConfigAacSink::useRtpHeaderMarkerBit() const {
  // TODO: This method applies only to Source codecs
  return false;
}

bool A2dpCodecConfigAacSink::updateEncoderUserConfig(
    UNUSED_ATTR const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
    UNUSED_ATTR bool* p_restart_input, UNUSED_ATTR bool* p_restart_output,
    UNUSED_ATTR bool* p_config_updated) {
  // TODO: This method applies only to Source codecs
  return false;
}
