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
 *  Utility functions to help build and parse the LDAC Codec Information
 *  Element and Media Payload.
 *
 ******************************************************************************/

#define LOG_TAG "a2dp_vendor_ldac"

#include "bt_target.h"

#include "a2dp_vendor_ldac.h"

#include <string.h>

#include <base/logging.h>
#include "a2dp_vendor.h"
#include "a2dp_vendor_ldac_decoder.h"
#include "a2dp_vendor_ldac_encoder.h"
#include "bt_utils.h"
#include "btif_av_co.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

// data type for the LDAC Codec Information Element */
// NOTE: bits_per_sample is needed only for LDAC encoder initialization.
typedef struct {
  uint32_t vendorId;
  uint16_t codecId;    /* Codec ID for LDAC */
  uint8_t sampleRate;  /* Sampling Frequency */
  uint8_t channelMode; /* STEREO/DUAL/MONO */
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
} tA2DP_LDAC_CIE;

/* LDAC Source codec capabilities */
static const tA2DP_LDAC_CIE a2dp_ldac_source_caps = {
    A2DP_LDAC_VENDOR_ID,  // vendorId
    A2DP_LDAC_CODEC_ID,   // codecId
    // sampleRate
    (A2DP_LDAC_SAMPLING_FREQ_44100 | A2DP_LDAC_SAMPLING_FREQ_48000 |
     A2DP_LDAC_SAMPLING_FREQ_88200 | A2DP_LDAC_SAMPLING_FREQ_96000),
    // channelMode
    (A2DP_LDAC_CHANNEL_MODE_DUAL | A2DP_LDAC_CHANNEL_MODE_STEREO),
    // bits_per_sample
    (BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16 | BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24 |
     BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32)};

/* LDAC Sink codec capabilities */
static const tA2DP_LDAC_CIE a2dp_ldac_sink_caps = {
    A2DP_LDAC_VENDOR_ID,  // vendorId
    A2DP_LDAC_CODEC_ID,   // codecId
    // sampleRate
    (A2DP_LDAC_SAMPLING_FREQ_44100 | A2DP_LDAC_SAMPLING_FREQ_48000 |
     A2DP_LDAC_SAMPLING_FREQ_88200 | A2DP_LDAC_SAMPLING_FREQ_96000),
    // channelMode
    (A2DP_LDAC_CHANNEL_MODE_DUAL | A2DP_LDAC_CHANNEL_MODE_STEREO),
    // bits_per_sample
    (BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16 | BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24 |
     BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32)};

/* Default LDAC codec configuration */
static const tA2DP_LDAC_CIE a2dp_ldac_default_config = {
    A2DP_LDAC_VENDOR_ID,                // vendorId
    A2DP_LDAC_CODEC_ID,                 // codecId
    A2DP_LDAC_SAMPLING_FREQ_96000,      // sampleRate
    A2DP_LDAC_CHANNEL_MODE_STEREO,      // channelMode
    BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32  // bits_per_sample
};

static const tA2DP_ENCODER_INTERFACE a2dp_encoder_interface_ldac = {
    a2dp_vendor_ldac_encoder_init,
    a2dp_vendor_ldac_encoder_cleanup,
    a2dp_vendor_ldac_feeding_reset,
    a2dp_vendor_ldac_feeding_flush,
    a2dp_vendor_ldac_get_encoder_interval_ms,
    a2dp_vendor_ldac_send_frames,
    a2dp_vendor_ldac_set_transmit_queue_length};

static const tA2DP_DECODER_INTERFACE a2dp_decoder_interface_ldac = {
    a2dp_vendor_ldac_decoder_init,
    a2dp_vendor_ldac_decoder_cleanup,
    a2dp_vendor_ldac_decoder_decode_packet,
};

UNUSED_ATTR static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilityLdac(
    const tA2DP_LDAC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_peer_codec_info);

// Builds the LDAC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. |media_type| is the media type |AVDT_MEDIA_TYPE_*|.
// |p_ie| is a pointer to the LDAC Codec Information Element information.
// The result is stored in |p_result|. Returns A2DP_SUCCESS on success,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_BuildInfoLdac(uint8_t media_type,
                                       const tA2DP_LDAC_CIE* p_ie,
                                       uint8_t* p_result) {
  if (p_ie == NULL || p_result == NULL) {
    return A2DP_INVALID_PARAMS;
  }

  *p_result++ = A2DP_LDAC_CODEC_LEN;
  *p_result++ = (media_type << 4);
  *p_result++ = A2DP_MEDIA_CT_NON_A2DP;

  // Vendor ID and Codec ID
  *p_result++ = (uint8_t)(p_ie->vendorId & 0x000000FF);
  *p_result++ = (uint8_t)((p_ie->vendorId & 0x0000FF00) >> 8);
  *p_result++ = (uint8_t)((p_ie->vendorId & 0x00FF0000) >> 16);
  *p_result++ = (uint8_t)((p_ie->vendorId & 0xFF000000) >> 24);
  *p_result++ = (uint8_t)(p_ie->codecId & 0x00FF);
  *p_result++ = (uint8_t)((p_ie->codecId & 0xFF00) >> 8);

  // Sampling Frequency
  *p_result = (uint8_t)(p_ie->sampleRate & A2DP_LDAC_SAMPLING_FREQ_MASK);
  if (*p_result == 0) return A2DP_INVALID_PARAMS;
  p_result++;

  // Channel Mode
  *p_result = (uint8_t)(p_ie->channelMode & A2DP_LDAC_CHANNEL_MODE_MASK);
  if (*p_result == 0) return A2DP_INVALID_PARAMS;

  return A2DP_SUCCESS;
}

// Parses the LDAC Media Codec Capabilities byte sequence beginning from the
// LOSC octet. The result is stored in |p_ie|. The byte sequence to parse is
// |p_codec_info|. If |is_capability| is true, the byte sequence is
// codec capabilities, otherwise is codec configuration.
// Returns A2DP_SUCCESS on success, otherwise the corresponding A2DP error
// status code.
static tA2DP_STATUS A2DP_ParseInfoLdac(tA2DP_LDAC_CIE* p_ie,
                                       const uint8_t* p_codec_info,
                                       bool is_capability) {
  uint8_t losc;
  uint8_t media_type;
  tA2DP_CODEC_TYPE codec_type;

  if (p_ie == NULL || p_codec_info == NULL) return A2DP_INVALID_PARAMS;

  // Check the codec capability length
  losc = *p_codec_info++;
  if (losc != A2DP_LDAC_CODEC_LEN) return A2DP_WRONG_CODEC;

  media_type = (*p_codec_info++) >> 4;
  codec_type = *p_codec_info++;
  /* Check the Media Type and Media Codec Type */
  if (media_type != AVDT_MEDIA_TYPE_AUDIO ||
      codec_type != A2DP_MEDIA_CT_NON_A2DP) {
    return A2DP_WRONG_CODEC;
  }

  // Check the Vendor ID and Codec ID */
  p_ie->vendorId = (*p_codec_info & 0x000000FF) |
                   (*(p_codec_info + 1) << 8 & 0x0000FF00) |
                   (*(p_codec_info + 2) << 16 & 0x00FF0000) |
                   (*(p_codec_info + 3) << 24 & 0xFF000000);
  p_codec_info += 4;
  p_ie->codecId =
      (*p_codec_info & 0x00FF) | (*(p_codec_info + 1) << 8 & 0xFF00);
  p_codec_info += 2;
  if (p_ie->vendorId != A2DP_LDAC_VENDOR_ID ||
      p_ie->codecId != A2DP_LDAC_CODEC_ID) {
    return A2DP_WRONG_CODEC;
  }

  p_ie->sampleRate = *p_codec_info++ & A2DP_LDAC_SAMPLING_FREQ_MASK;
  p_ie->channelMode = *p_codec_info++ & A2DP_LDAC_CHANNEL_MODE_MASK;

  if (is_capability) {
    // NOTE: The checks here are very liberal. We should be using more
    // pedantic checks specific to the SRC or SNK as specified in the spec.
    if (A2DP_BitsSet(p_ie->sampleRate) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_SAMP_FREQ;
    if (A2DP_BitsSet(p_ie->channelMode) == A2DP_SET_ZERO_BIT)
      return A2DP_BAD_CH_MODE;

    return A2DP_SUCCESS;
  }

  if (A2DP_BitsSet(p_ie->sampleRate) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_SAMP_FREQ;
  if (A2DP_BitsSet(p_ie->channelMode) != A2DP_SET_ONE_BIT)
    return A2DP_BAD_CH_MODE;

  return A2DP_SUCCESS;
}

// Build the LDAC Media Payload Header.
// |p_dst| points to the location where the header should be written to.
// If |frag| is true, the media payload frame is fragmented.
// |start| is true for the first packet of a fragmented frame.
// |last| is true for the last packet of a fragmented frame.
// If |frag| is false, |num| is the number of number of frames in the packet,
// otherwise is the number of remaining fragments (including this one).
static void A2DP_BuildMediaPayloadHeaderLdac(uint8_t* p_dst, bool frag,
                                             bool start, bool last,
                                             uint8_t num) {
  if (p_dst == NULL) return;

  *p_dst = 0;
  if (frag) *p_dst |= A2DP_LDAC_HDR_F_MSK;
  if (start) *p_dst |= A2DP_LDAC_HDR_S_MSK;
  if (last) *p_dst |= A2DP_LDAC_HDR_L_MSK;
  *p_dst |= (A2DP_LDAC_HDR_NUM_MSK & num);
}

bool A2DP_IsVendorSourceCodecValidLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsVendorSinkCodecValidLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsVendorPeerSourceCodecValidLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsVendorPeerSinkCodecValidLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE cfg_cie;

  /* Use a liberal check when parsing the codec info */
  return (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, false) == A2DP_SUCCESS) ||
         (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, true) == A2DP_SUCCESS);
}

bool A2DP_IsVendorSinkCodecSupportedLdac(const uint8_t* p_codec_info) {
  return A2DP_CodecInfoMatchesCapabilityLdac(&a2dp_ldac_sink_caps, p_codec_info,
                                             false) == A2DP_SUCCESS;
}
bool A2DP_IsPeerSourceCodecSupportedLdac(const uint8_t* p_codec_info) {
  return A2DP_CodecInfoMatchesCapabilityLdac(&a2dp_ldac_sink_caps, p_codec_info,
                                             true) == A2DP_SUCCESS;
}

// Checks whether A2DP LDAC codec configuration matches with a device's codec
// capabilities. |p_cap| is the LDAC codec configuration. |p_codec_info| is
// the device's codec capabilities.
// If |is_capability| is true, the byte sequence is codec capabilities,
// otherwise is codec configuration.
// |p_codec_info| contains the codec capabilities for a peer device that
// is acting as an A2DP source.
// Returns A2DP_SUCCESS if the codec configuration matches with capabilities,
// otherwise the corresponding A2DP error status code.
static tA2DP_STATUS A2DP_CodecInfoMatchesCapabilityLdac(
    const tA2DP_LDAC_CIE* p_cap, const uint8_t* p_codec_info,
    bool is_capability) {
  tA2DP_STATUS status;
  tA2DP_LDAC_CIE cfg_cie;

  /* parse configuration */
  status = A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: parsing failed %d", __func__, status);
    return status;
  }

  /* verify that each parameter is in range */

  LOG_VERBOSE(LOG_TAG, "%s: FREQ peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.sampleRate, p_cap->sampleRate);
  LOG_VERBOSE(LOG_TAG, "%s: CH_MODE peer: 0x%x, capability 0x%x", __func__,
              cfg_cie.channelMode, p_cap->channelMode);

  /* sampling frequency */
  if ((cfg_cie.sampleRate & p_cap->sampleRate) == 0) return A2DP_NS_SAMP_FREQ;

  /* channel mode */
  if ((cfg_cie.channelMode & p_cap->channelMode) == 0) return A2DP_NS_CH_MODE;

  return A2DP_SUCCESS;
}

bool A2DP_VendorUsesRtpHeaderLdac(UNUSED_ATTR bool content_protection_enabled,
                                  UNUSED_ATTR const uint8_t* p_codec_info) {
  // TODO: Is this correct? The RTP header is always included?
  return true;
}

const char* A2DP_VendorCodecNameLdac(UNUSED_ATTR const uint8_t* p_codec_info) {
  return "LDAC";
}

bool A2DP_VendorCodecTypeEqualsLdac(const uint8_t* p_codec_info_a,
                                    const uint8_t* p_codec_info_b) {
  tA2DP_LDAC_CIE ldac_cie_a;
  tA2DP_LDAC_CIE ldac_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoLdac(&ldac_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoLdac(&ldac_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  return true;
}

bool A2DP_VendorCodecEqualsLdac(const uint8_t* p_codec_info_a,
                                const uint8_t* p_codec_info_b) {
  tA2DP_LDAC_CIE ldac_cie_a;
  tA2DP_LDAC_CIE ldac_cie_b;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status =
      A2DP_ParseInfoLdac(&ldac_cie_a, p_codec_info_a, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }
  a2dp_status = A2DP_ParseInfoLdac(&ldac_cie_b, p_codec_info_b, true);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return false;
  }

  return (ldac_cie_a.sampleRate == ldac_cie_b.sampleRate) &&
         (ldac_cie_a.channelMode == ldac_cie_b.channelMode);
}

int A2DP_VendorGetBitRateLdac(const uint8_t* p_codec_info) {
  A2dpCodecConfig* current_codec = bta_av_get_a2dp_current_codec();
  btav_a2dp_codec_config_t codec_config_ = current_codec->getCodecConfig();
  int samplerate = A2DP_GetTrackSampleRate(p_codec_info);
  switch (codec_config_.codec_specific_1 % 10) {
    case 0:
      if (samplerate == 44100 || samplerate == 88200)
        return 909000;
      else
        return 990000;
    case 1:
      if (samplerate == 44100 || samplerate == 88200)
        return 606000;
      else
        return 660000;
    case 2:
      if (samplerate == 44100 || samplerate == 88200)
        return 303000;
      else
        return 330000;
    case 3:
    default:
      if (samplerate == 44100 || samplerate == 88200)
        return 909000;
      else
        return 990000;
  }
  return 0;
}

int A2DP_VendorGetTrackSampleRateLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE ldac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (ldac_cie.sampleRate) {
    case A2DP_LDAC_SAMPLING_FREQ_44100:
      return 44100;
    case A2DP_LDAC_SAMPLING_FREQ_48000:
      return 48000;
    case A2DP_LDAC_SAMPLING_FREQ_88200:
      return 88200;
    case A2DP_LDAC_SAMPLING_FREQ_96000:
      return 96000;
    case A2DP_LDAC_SAMPLING_FREQ_176400:
      return 176400;
    case A2DP_LDAC_SAMPLING_FREQ_192000:
      return 192000;
  }

  return -1;
}

int A2DP_VendorGetTrackBitsPerSampleLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE ldac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

#if 1
  return 32;
#else
  // TODO : Implement proc to care about bit per sample in A2DP_ParseInfoLdac()

  switch (ldac_cie.bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      return 16;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
      return 24;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
      return 32;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE:
      return -1;
  }
#endif
}

int A2DP_VendorGetTrackChannelCountLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE ldac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (ldac_cie.channelMode) {
    case A2DP_LDAC_CHANNEL_MODE_MONO:
      return 1;
    case A2DP_LDAC_CHANNEL_MODE_DUAL:
      return 2;
    case A2DP_LDAC_CHANNEL_MODE_STEREO:
      return 2;
  }

  return -1;
}

int A2DP_VendorGetSinkTrackChannelTypeLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE ldac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (ldac_cie.channelMode) {
    case A2DP_LDAC_CHANNEL_MODE_MONO:
      return 1;
    case A2DP_LDAC_CHANNEL_MODE_DUAL:
      return 3;
    case A2DP_LDAC_CHANNEL_MODE_STEREO:
      return 3;
  }

  return -1;
}

int A2DP_VendorGetChannelModeCodeLdac(const uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE ldac_cie;

  // Check whether the codec info contains valid data
  tA2DP_STATUS a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, false);
  if (a2dp_status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: cannot decode codec information: %d", __func__,
              a2dp_status);
    return -1;
  }

  switch (ldac_cie.channelMode) {
    case A2DP_LDAC_CHANNEL_MODE_MONO:
    case A2DP_LDAC_CHANNEL_MODE_DUAL:
    case A2DP_LDAC_CHANNEL_MODE_STEREO:
      return ldac_cie.channelMode;
    default:
      break;
  }

  return -1;
}

bool A2DP_VendorGetPacketTimestampLdac(UNUSED_ATTR const uint8_t* p_codec_info,
                                       const uint8_t* p_data,
                                       uint32_t* p_timestamp) {
  // TODO: Is this function really codec-specific?
  *p_timestamp = *(const uint32_t*)p_data;
  return true;
}

bool A2DP_VendorBuildCodecHeaderLdac(UNUSED_ATTR const uint8_t* p_codec_info,
                                     BT_HDR* p_buf,
                                     uint16_t frames_per_packet) {
  uint8_t* p;

  p_buf->offset -= A2DP_LDAC_MPL_HDR_LEN;
  p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  p_buf->len += A2DP_LDAC_MPL_HDR_LEN;
  A2DP_BuildMediaPayloadHeaderLdac(p, false, false, false,
                                   (uint8_t)frames_per_packet);

  return true;
}

std::string A2DP_VendorCodecInfoStringLdac(const uint8_t* p_codec_info) {
  std::stringstream res;
  std::string field;
  tA2DP_STATUS a2dp_status;
  tA2DP_LDAC_CIE ldac_cie;

  a2dp_status = A2DP_ParseInfoLdac(&ldac_cie, p_codec_info, true);
  if (a2dp_status != A2DP_SUCCESS) {
    res << "A2DP_ParseInfoLdac fail: " << loghex(a2dp_status);
    return res.str();
  }

  res << "\tname: LDAC\n";

  // Sample frequency
  field.clear();
  AppendField(&field, (ldac_cie.sampleRate == 0), "NONE");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100),
              "44100");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000),
              "48000");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200),
              "88200");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000),
              "96000");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400),
              "176400");
  AppendField(&field, (ldac_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000),
              "192000");
  res << "\tsamp_freq: " << field << " (" << loghex(ldac_cie.sampleRate)
      << ")\n";

  // Channel mode
  field.clear();
  AppendField(&field, (ldac_cie.channelMode == 0), "NONE");
  AppendField(&field, (ldac_cie.channelMode & A2DP_LDAC_CHANNEL_MODE_MONO),
              "Mono");
  AppendField(&field, (ldac_cie.channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL),
              "Dual");
  AppendField(&field, (ldac_cie.channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO),
              "Stereo");
  res << "\tch_mode: " << field << " (" << loghex(ldac_cie.channelMode)
      << ")\n";

  return res.str();
}

const tA2DP_ENCODER_INTERFACE* A2DP_VendorGetEncoderInterfaceLdac(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsVendorSourceCodecValidLdac(p_codec_info)) return NULL;

  return &a2dp_encoder_interface_ldac;
}

const tA2DP_DECODER_INTERFACE* A2DP_VendorGetDecoderInterfaceLdac(
    const uint8_t* p_codec_info) {
  if (!A2DP_IsVendorSinkCodecValidLdac(p_codec_info)) return NULL;

  return &a2dp_decoder_interface_ldac;
}

bool A2DP_VendorAdjustCodecLdac(uint8_t* p_codec_info) {
  tA2DP_LDAC_CIE cfg_cie;

  // Nothing to do: just verify the codec info is valid
  if (A2DP_ParseInfoLdac(&cfg_cie, p_codec_info, true) != A2DP_SUCCESS)
    return false;

  return true;
}

btav_a2dp_codec_index_t A2DP_VendorSourceCodecIndexLdac(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SOURCE_LDAC;
}

btav_a2dp_codec_index_t A2DP_VendorSinkCodecIndexLdac(
    UNUSED_ATTR const uint8_t* p_codec_info) {
  return BTAV_A2DP_CODEC_INDEX_SINK_LDAC;
}

const char* A2DP_VendorCodecIndexStrLdac(void) { return "LDAC"; }

const char* A2DP_VendorCodecIndexStrLdacSink(void) { return "LDAC SINK"; }

bool A2DP_VendorInitCodecConfigLdac(AvdtpSepConfig* p_cfg) {
  if (A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &a2dp_ldac_source_caps,
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

bool A2DP_VendorInitCodecConfigLdacSink(AvdtpSepConfig* p_cfg) {
  return A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &a2dp_ldac_sink_caps,
                            p_cfg->codec_info) == A2DP_SUCCESS;
}

UNUSED_ATTR static void build_codec_config(const tA2DP_LDAC_CIE& config_cie,
                                           btav_a2dp_codec_config_t* result) {
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
  if (config_cie.sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000)
    result->sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_192000;

  result->bits_per_sample = config_cie.bits_per_sample;

  if (config_cie.channelMode & A2DP_LDAC_CHANNEL_MODE_MONO)
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  if (config_cie.channelMode &
      (A2DP_LDAC_CHANNEL_MODE_DUAL | A2DP_LDAC_CHANNEL_MODE_STEREO)) {
    result->channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigLdacSource::A2dpCodecConfigLdacSource(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigLdacBase(BTAV_A2DP_CODEC_INDEX_SOURCE_LDAC,
                              A2DP_VendorCodecIndexStrLdac(), codec_priority,
                              true) {
  // Compute the local capability
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  }
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
  }
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
  }
  if (a2dp_ldac_source_caps.sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
    codec_local_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_192000;
  }
  codec_local_capability_.bits_per_sample =
      a2dp_ldac_source_caps.bits_per_sample;
  if (a2dp_ldac_source_caps.channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (a2dp_ldac_source_caps.channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (a2dp_ldac_source_caps.channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
    codec_local_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
}

A2dpCodecConfigLdacSource::~A2dpCodecConfigLdacSource() {}

bool A2dpCodecConfigLdacSource::init() {
  if (!isValid()) return false;

  // Load the encoder
  if (!A2DP_VendorLoadEncoderLdac()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the encoder", __func__);
    return false;
  }

  return true;
}

bool A2dpCodecConfigLdacSource::useRtpHeaderMarkerBit() const { return false; }

//
// Selects the best sample rate from |sampleRate|.
// The result is stored in |p_result| and |p_codec_config|.
// Returns true if a selection was made, otherwise false.
//
static bool select_best_sample_rate(uint8_t sampleRate,
                                    tA2DP_LDAC_CIE* p_result,
                                    btav_a2dp_codec_config_t* p_codec_config) {
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_192000;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_192000;
    return true;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_176400;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
    return true;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_96000;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
    return true;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_88200;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    return true;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_48000;
    p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    return true;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
    p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_44100;
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
    const btav_a2dp_codec_config_t* p_codec_audio_config, uint8_t sampleRate,
    tA2DP_LDAC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_44100;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_48000;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_88200;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_96000;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_176400;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
        p_result->sampleRate = A2DP_LDAC_SAMPLING_FREQ_192000;
        p_codec_config->sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_192000;
        return true;
      }
      break;
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
    btav_a2dp_codec_bits_per_sample_t bits_per_sample, tA2DP_LDAC_CIE* p_result,
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
    btav_a2dp_codec_bits_per_sample_t bits_per_sample, tA2DP_LDAC_CIE* p_result,
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
                                     tA2DP_LDAC_CIE* p_result,
                                     btav_a2dp_codec_config_t* p_codec_config) {
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
    p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_STEREO;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
    p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_DUAL;
    p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    return true;
  }
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
    p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_MONO;
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
    tA2DP_LDAC_CIE* p_result, btav_a2dp_codec_config_t* p_codec_config) {
  switch (p_codec_audio_config->channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
        p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_MONO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
        p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_STEREO;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
        p_result->channelMode = A2DP_LDAC_CHANNEL_MODE_DUAL;
        p_codec_config->channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
        return true;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_NONE:
      break;
  }

  return false;
}

bool A2dpCodecConfigLdacBase::setCodecConfig(const uint8_t* p_peer_codec_info,
                                             bool is_capability,
                                             uint8_t* p_result_codec_config) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_LDAC_CIE peer_info_cie;
  tA2DP_LDAC_CIE result_config_cie;
  uint8_t channelMode;
  uint8_t sampleRate;
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
  const tA2DP_LDAC_CIE* p_a2dp_ldac_caps =
      (is_source_) ? &a2dp_ldac_source_caps : &a2dp_ldac_sink_caps;

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
      A2DP_ParseInfoLdac(&peer_info_cie, p_peer_codec_info, is_capability);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }

  //
  // Build the preferred configuration
  //
  memset(&result_config_cie, 0, sizeof(result_config_cie));
  result_config_cie.vendorId = p_a2dp_ldac_caps->vendorId;
  result_config_cie.codecId = p_a2dp_ldac_caps->codecId;

  //
  // Select the sample frequency
  //
  sampleRate = p_a2dp_ldac_caps->sampleRate & peer_info_cie.sampleRate;
  codec_config_.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
  switch (codec_user_config_.sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_44100;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_48000;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_88200;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_96000;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_176400;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
      if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
        result_config_cie.sampleRate = A2DP_LDAC_SAMPLING_FREQ_192000;
        codec_capability_.sample_rate = codec_user_config_.sample_rate;
        codec_config_.sample_rate = codec_user_config_.sample_rate;
      }
      break;
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
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    }
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    }
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    }
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
    }
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
    }
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
      codec_selectable_capability_.sample_rate |=
          BTAV_A2DP_CODEC_SAMPLE_RATE_192000;
    }

    if (codec_config_.sample_rate != BTAV_A2DP_CODEC_SAMPLE_RATE_NONE) break;

    // Compute the common capability
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
    if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000)
      codec_capability_.sample_rate |= BTAV_A2DP_CODEC_SAMPLE_RATE_192000;

    // No user preference - try the codec audio config
    if (select_audio_sample_rate(&codec_audio_config_, sampleRate,
                                 &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_sample_rate(
            a2dp_ldac_default_config.sampleRate & peer_info_cie.sampleRate,
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
              "%s: cannot match sample frequency: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_ldac_caps->sampleRate, peer_info_cie.sampleRate);
    goto fail;
  }

  //
  // Select the bits per sample
  //
  // NOTE: this information is NOT included in the LDAC A2DP codec description
  // that is sent OTA.
  bits_per_sample = p_a2dp_ldac_caps->bits_per_sample;
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
        p_a2dp_ldac_caps->bits_per_sample;

    if (codec_config_.bits_per_sample != BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE)
      break;

    // Compute the common capability
    codec_capability_.bits_per_sample = bits_per_sample;

    // No user preference - the the codec audio config
    if (select_audio_bits_per_sample(&codec_audio_config_,
                                     p_a2dp_ldac_caps->bits_per_sample,
                                     &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_bits_per_sample(a2dp_ldac_default_config.bits_per_sample,
                                    &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - use the best match
    if (select_best_bits_per_sample(p_a2dp_ldac_caps->bits_per_sample,
                                    &result_config_cie, &codec_config_)) {
      break;
    }
  } while (false);
  if (codec_config_.bits_per_sample == BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot match bits per sample: default = 0x%x "
              "user preference = 0x%x",
              __func__, a2dp_ldac_default_config.bits_per_sample,
              codec_user_config_.bits_per_sample);
    goto fail;
  }

  //
  // Select the channel mode
  //
  channelMode = p_a2dp_ldac_caps->channelMode & peer_info_cie.channelMode;
  codec_config_.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;
  switch (codec_user_config_.channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
        result_config_cie.channelMode = A2DP_LDAC_CHANNEL_MODE_MONO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
      }
      break;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
        result_config_cie.channelMode = A2DP_LDAC_CHANNEL_MODE_STEREO;
        codec_capability_.channel_mode = codec_user_config_.channel_mode;
        codec_config_.channel_mode = codec_user_config_.channel_mode;
        break;
      }
      if (channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
        result_config_cie.channelMode = A2DP_LDAC_CHANNEL_MODE_DUAL;
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
    if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    }
    if (channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }
    if (channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
      codec_selectable_capability_.channel_mode |=
          BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    if (codec_config_.channel_mode != BTAV_A2DP_CODEC_CHANNEL_MODE_NONE) break;

    // Compute the common capability
    if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO)
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
    if (channelMode &
        (A2DP_LDAC_CHANNEL_MODE_STEREO | A2DP_LDAC_CHANNEL_MODE_DUAL)) {
      codec_capability_.channel_mode |= BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
    }

    // No user preference - try the codec audio config
    if (select_audio_channel_mode(&codec_audio_config_, channelMode,
                                  &result_config_cie, &codec_config_)) {
      break;
    }

    // No user preference - try the default config
    if (select_best_channel_mode(
            a2dp_ldac_default_config.channelMode & peer_info_cie.channelMode,
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
              "%s: cannot match channel mode: local caps = 0x%x "
              "peer info = 0x%x",
              __func__, p_a2dp_ldac_caps->channelMode,
              peer_info_cie.channelMode);
    goto fail;
  }

  if (A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
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

  // Create a local copy of the peer codec capability, and the
  // result codec config.
  if (is_capability) {
    status = A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                                ota_codec_peer_capability_);
  } else {
    status = A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
                                ota_codec_peer_config_);
  }
  CHECK(status == A2DP_SUCCESS);
  status = A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &result_config_cie,
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

bool A2dpCodecConfigLdacBase::setPeerCodecCapabilities(
    const uint8_t* p_peer_codec_capabilities) {
  std::lock_guard<std::recursive_mutex> lock(codec_mutex_);
  tA2DP_LDAC_CIE peer_info_cie;
  uint8_t channelMode;
  uint8_t sampleRate;
  const tA2DP_LDAC_CIE* p_a2dp_ldac_caps =
      (is_source_) ? &a2dp_ldac_source_caps : &a2dp_ldac_sink_caps;

  // Save the internal state
  btav_a2dp_codec_config_t saved_codec_selectable_capability =
      codec_selectable_capability_;
  uint8_t saved_ota_codec_peer_capability[AVDT_CODEC_SIZE];
  memcpy(saved_ota_codec_peer_capability, ota_codec_peer_capability_,
         sizeof(ota_codec_peer_capability_));

  tA2DP_STATUS status =
      A2DP_ParseInfoLdac(&peer_info_cie, p_peer_codec_capabilities, true);
  if (status != A2DP_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: can't parse peer's capabilities: error = %d",
              __func__, status);
    goto fail;
  }

  // Compute the selectable capability - sample rate
  sampleRate = p_a2dp_ldac_caps->sampleRate & peer_info_cie.sampleRate;
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_44100) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_44100;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_48000) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_48000;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_88200) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_88200;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_96000) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_96000;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_176400) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_176400;
  }
  if (sampleRate & A2DP_LDAC_SAMPLING_FREQ_192000) {
    codec_selectable_capability_.sample_rate |=
        BTAV_A2DP_CODEC_SAMPLE_RATE_192000;
  }

  // Compute the selectable capability - bits per sample
  codec_selectable_capability_.bits_per_sample =
      p_a2dp_ldac_caps->bits_per_sample;

  // Compute the selectable capability - channel mode
  channelMode = p_a2dp_ldac_caps->channelMode & peer_info_cie.channelMode;
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_MONO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_MONO;
  }
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_STEREO) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }
  if (channelMode & A2DP_LDAC_CHANNEL_MODE_DUAL) {
    codec_selectable_capability_.channel_mode |=
        BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
  }

  status = A2DP_BuildInfoLdac(AVDT_MEDIA_TYPE_AUDIO, &peer_info_cie,
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

A2dpCodecConfigLdacSink::A2dpCodecConfigLdacSink(
    btav_a2dp_codec_priority_t codec_priority)
    : A2dpCodecConfigLdacBase(BTAV_A2DP_CODEC_INDEX_SINK_LDAC,
                              A2DP_VendorCodecIndexStrLdacSink(),
                              codec_priority, false) {}

A2dpCodecConfigLdacSink::~A2dpCodecConfigLdacSink() {}

bool A2dpCodecConfigLdacSink::init() {
  if (!isValid()) return false;

  // Load the decoder
  if (!A2DP_VendorLoadDecoderLdac()) {
    LOG_ERROR(LOG_TAG, "%s: cannot load the decoder", __func__);
    return false;
  }

  return true;
}

uint64_t A2dpCodecConfigLdacSink::encoderIntervalMs() const {
  // TODO: This method applies only to Source codecs
  return 0;
}

int A2dpCodecConfigLdacSink::getEffectiveMtu() const {
  // TODO: This method applies only to Source codecs
  return 0;
}

bool A2dpCodecConfigLdacSink::useRtpHeaderMarkerBit() const {
  // TODO: This method applies only to Source codecs
  return false;
}

bool A2dpCodecConfigLdacSink::updateEncoderUserConfig(
    UNUSED_ATTR const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
    UNUSED_ATTR bool* p_restart_input, UNUSED_ATTR bool* p_restart_output,
    UNUSED_ATTR bool* p_config_updated) {
  // TODO: This method applies only to Source codecs
  return false;
}
