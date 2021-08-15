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

//
// A2DP Codec API for AAC
//

#ifndef A2DP_AAC_H
#define A2DP_AAC_H

#include "a2dp_aac_constants.h"
#include "a2dp_codec_api.h"
#include "avdt_api.h"

class A2dpCodecConfigAacBase : public A2dpCodecConfig {
 protected:
  A2dpCodecConfigAacBase(btav_a2dp_codec_index_t codec_index,
                         const std::string& name,
                         btav_a2dp_codec_priority_t codec_priority,
                         bool is_source)
      : A2dpCodecConfig(codec_index, name, codec_priority),
        is_source_(is_source) {}
  bool setCodecConfig(const uint8_t* p_peer_codec_info, bool is_capability,
                      uint8_t* p_result_codec_config) override;
  bool setPeerCodecCapabilities(
      const uint8_t* p_peer_codec_capabilities) override;

 private:
  bool is_source_;  // True if local is Source
};

class A2dpCodecConfigAacSource : public A2dpCodecConfigAacBase {
 public:
  A2dpCodecConfigAacSource(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigAacSource();

  bool init() override;
  uint64_t encoderIntervalMs() const override;
  int getEffectiveMtu() const override;

 private:
  bool useRtpHeaderMarkerBit() const override;
  bool updateEncoderUserConfig(
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      bool* p_restart_input, bool* p_restart_output,
      bool* p_config_updated) override;
  void debug_codec_dump(int fd) override;
};

class A2dpCodecConfigAacSink : public A2dpCodecConfigAacBase {
 public:
  A2dpCodecConfigAacSink(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigAacSink();

  bool init() override;
  uint64_t encoderIntervalMs() const override;
  int getEffectiveMtu() const override;

 private:
  bool useRtpHeaderMarkerBit() const override;
  bool updateEncoderUserConfig(
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      bool* p_restart_input, bool* p_restart_output,
      bool* p_config_updated) override;
};

// Checks whether the codec capabilities contain a valid A2DP AAC Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid AAC
// codec, otherwise false.
bool A2DP_IsSourceCodecValidAac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP AAC Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid AAC codec,
// otherwise false.
bool A2DP_IsSinkCodecValidAac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP AAC Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid AAC codec,
// otherwise false.
bool A2DP_IsPeerSourceCodecValidAac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP AAC Sink
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid AAC
// codec, otherwise false.
bool A2DP_IsPeerSinkCodecValidAac(const uint8_t* p_codec_info);

// Checks whether A2DP AAC Sink codec is supported.
// |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP AAC Sink codec is supported, otherwise false.
bool A2DP_IsSinkCodecSupportedAac(const uint8_t* p_codec_info);

// Checks whether an A2DP AAC Source codec for a peer Source device is
// supported.
// |p_codec_info| contains information about the codec capabilities of the
// peer device.
// Returns true if the A2DP AAC Source codec for a peer Source device is
// supported, otherwise false.
bool A2DP_IsPeerSourceCodecSupportedAac(const uint8_t* p_codec_info);

// Checks whether the A2DP data packets should contain RTP header.
// |content_protection_enabled| is true if Content Protection is
// enabled. |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP data packets should contain RTP header, otherwise
// false.
bool A2DP_UsesRtpHeaderAac(bool content_protection_enabled,
                           const uint8_t* p_codec_info);

// Gets the A2DP AAC codec name for a given |p_codec_info|.
const char* A2DP_CodecNameAac(const uint8_t* p_codec_info);

// Checks whether two A2DP AAC codecs |p_codec_info_a| and |p_codec_info_b|
// have the same type.
// Returns true if the two codecs have the same type, otherwise false.
bool A2DP_CodecTypeEqualsAac(const uint8_t* p_codec_info_a,
                             const uint8_t* p_codec_info_b);

// Checks whether two A2DP AAC codecs |p_codec_info_a| and |p_codec_info_b|
// are exactly the same.
// Returns true if the two codecs are exactly the same, otherwise false.
// If the codec type is not AAC, the return value is false.
bool A2DP_CodecEqualsAac(const uint8_t* p_codec_info_a,
                         const uint8_t* p_codec_info_b);

// Gets the track sample rate value for the A2DP AAC codec.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackSampleRateAac(const uint8_t* p_codec_info);

// Gets the track bits per sample value for the A2DP AAC codec.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the track bits per sample on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackBitsPerSampleAac(const uint8_t* p_codec_info);

// Gets the channel count for the A2DP AAC codec.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackChannelCountAac(const uint8_t* p_codec_info);

// Gets the channel type for the A2DP AAC Sink codec:
// 1 for mono, or 3 for dual/stereo/joint.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the channel type on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetSinkTrackChannelTypeAac(const uint8_t* p_codec_info);

// Gets the object type code for the A2DP AAC codec.
// The actual value is codec-specific - see |A2DP_AAC_OBJECT_TYPE_*|.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the object type code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetObjectTypeCodeAac(const uint8_t* p_codec_info);

// Gets the channel mode code for the A2DP AAC codec.
// The actual value is codec-specific - see |A2DP_AAC_CHANNEL_MODE_*|.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the channel mode code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetChannelModeCodeAac(const uint8_t* p_codec_info);

// Gets the variable bit rate support for the A2DP AAC codec.
// The actual value is codec-specific - see |A2DP_AAC_VARIABLE_BIT_RATE_*|.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the variable bit rate support on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetVariableBitRateSupportAac(const uint8_t* p_codec_info);

// Gets the bit rate for the A2DP AAC codec.
// The actual value is codec-specific - for AAC it is in bits per second.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns the bit rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetBitRateAac(const uint8_t* p_codec_info);

// Computes the maximum bit rate for the A2DP AAC codec based on the MTU.
// The actual value is codec-specific - for AAC it is in bits per second.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// |mtu| is the MTU to use for the computation.
// Returns the maximum bit rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_ComputeMaxBitRateAac(const uint8_t* p_codec_info, uint16_t mtu);

// Gets the A2DP AAC audio data timestamp from an audio packet.
// |p_codec_info| contains the codec information.
// |p_data| contains the audio data.
// The timestamp is stored in |p_timestamp|.
// Returns true on success, otherwise false.
bool A2DP_GetPacketTimestampAac(const uint8_t* p_codec_info,
                                const uint8_t* p_data, uint32_t* p_timestamp);

// Builds A2DP AAC codec header for audio data.
// |p_codec_info| contains the codec information.
// |p_buf| contains the audio data.
// |frames_per_packet| is the number of frames in this packet.
// Returns true on success, otherwise false.
bool A2DP_BuildCodecHeaderAac(const uint8_t* p_codec_info, BT_HDR* p_buf,
                              uint16_t frames_per_packet);

// Decodes A2DP AAC codec info into a human readable string.
// |p_codec_info| is a pointer to the AAC codec_info to decode.
// Returns a string describing the codec information.
std::string A2DP_CodecInfoStringAac(const uint8_t* p_codec_info);

// Gets the A2DP AAC encoder interface that can be used to encode and prepare
// A2DP packets for transmission - see |tA2DP_ENCODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP AAC encoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_ENCODER_INTERFACE* A2DP_GetEncoderInterfaceAac(
    const uint8_t* p_codec_info);

// Gets the current A2DP AAC decoder interface that can be used to decode
// received A2DP packets - see |tA2DP_DECODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP AAC decoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_DECODER_INTERFACE* A2DP_GetDecoderInterfaceAac(
    const uint8_t* p_codec_info);

// Adjusts the A2DP AAC codec, based on local support and Bluetooth
// specification.
// |p_codec_info| contains the codec information to adjust.
// Returns true if |p_codec_info| is valid and supported, otherwise false.
bool A2DP_AdjustCodecAac(uint8_t* p_codec_info);

// Gets the A2DP AAC Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SourceCodecIndexAac(const uint8_t* p_codec_info);

// Gets the A2DP AAC Sink codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SinkCodecIndexAac(const uint8_t* p_codec_info);

// Gets the A2DP AAC Source codec name.
const char* A2DP_CodecIndexStrAac(void);

// Gets the A2DP AAC Sink codec name.
const char* A2DP_CodecIndexStrAacSink(void);

// Initializes A2DP AAC Source codec information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|.
bool A2DP_InitCodecConfigAac(AvdtpSepConfig* p_cfg);

// Initializes A2DP AAC Sink codec information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|.
bool A2DP_InitCodecConfigAacSink(AvdtpSepConfig* p_cfg);

#endif  // A2DP_AAC_H
