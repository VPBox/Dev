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
// A2DP Codec API for LDAC
//

#ifndef A2DP_VENDOR_LDAC_H
#define A2DP_VENDOR_LDAC_H

#include "a2dp_codec_api.h"
#include "a2dp_vendor_ldac_constants.h"
#include "avdt_api.h"

class A2dpCodecConfigLdacBase : public A2dpCodecConfig {
 protected:
  A2dpCodecConfigLdacBase(btav_a2dp_codec_index_t codec_index,
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

class A2dpCodecConfigLdacSource : public A2dpCodecConfigLdacBase {
 public:
  A2dpCodecConfigLdacSource(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigLdacSource();

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

class A2dpCodecConfigLdacSink : public A2dpCodecConfigLdacBase {
 public:
  A2dpCodecConfigLdacSink(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigLdacSink();

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

// Checks whether the codec capabilities contain a valid A2DP LDAC Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid LDAC
// codec, otherwise false.
bool A2DP_IsVendorSourceCodecValidLdac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP LDAC Sink
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid LDAC
// codec, otherwise false.
bool A2DP_IsVendorSinkCodecValidLdac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP LDAC Sink
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid LDAC
// codec, otherwise false.
bool A2DP_IsVendorPeerSinkCodecValidLdac(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP LDAC Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid LDAC
// codec, otherwise false.
bool A2DP_IsVendorPeerSourceCodecValidLdac(const uint8_t* p_codec_info);

// Checks whether A2DP LDAC Sink codec is supported.
// |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP LDAC Sink codec is supported, otherwise false.
bool A2DP_IsVendorSinkCodecSupportedLdac(const uint8_t* p_codec_info);

// Checks whether an A2DP LDAC Source codec for a peer Source device is
// supported.
// |p_codec_info| contains information about the codec capabilities of the
// peer device.
// Returns true if the A2DP LDAC Source codec for a peer Source device is
// supported, otherwise false.
bool A2DP_IsPeerSourceCodecSupportedLdac(const uint8_t* p_codec_info);

// Checks whether the A2DP data packets should contain RTP header.
// |content_protection_enabled| is true if Content Protection is
// enabled. |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP data packets should contain RTP header, otherwise
// false.
bool A2DP_VendorUsesRtpHeaderLdac(bool content_protection_enabled,
                                  const uint8_t* p_codec_info);

// Gets the A2DP LDAC codec name for a given |p_codec_info|.
const char* A2DP_VendorCodecNameLdac(const uint8_t* p_codec_info);

// Checks whether two A2DP LDAC codecs |p_codec_info_a| and |p_codec_info_b|
// have the same type.
// Returns true if the two codecs have the same type, otherwise false.
bool A2DP_VendorCodecTypeEqualsLdac(const uint8_t* p_codec_info_a,
                                    const uint8_t* p_codec_info_b);

// Checks whether two A2DP LDAC codecs |p_codec_info_a| and |p_codec_info_b|
// are exactly the same.
// Returns true if the two codecs are exactly the same, otherwise false.
// If the codec type is not LDAC, the return value is false.
bool A2DP_VendorCodecEqualsLdac(const uint8_t* p_codec_info_a,
                                const uint8_t* p_codec_info_b);

// Gets the track sample rate value for the A2DP LDAC codec.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackSampleRateLdac(const uint8_t* p_codec_info);

// Gets the track bits per sample value for the A2DP LDAC codec.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the track bits per sample on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackBitsPerSampleLdac(const uint8_t* p_codec_info);

// Gets the track bitrate value for the A2DP LDAC codec.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetBitRateLdac(const uint8_t* p_codec_info);

// Gets the channel count for the A2DP LDAC codec.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackChannelCountLdac(const uint8_t* p_codec_info);

// Gets the channel type for the A2DP LDAC codec.
// 1 for mono, or 3 for dual channel/stereo.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetSinkTrackChannelTypeLdac(const uint8_t* p_codec_info);

// Gets the channel mode code for the A2DP LDAC codec.
// The actual value is codec-specific - see |A2DP_LDAC_CHANNEL_MODE_*|.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns the channel mode code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetChannelModeCodeLdac(const uint8_t* p_codec_info);

// Gets the A2DP LDAC audio data timestamp from an audio packet.
// |p_codec_info| contains the codec information.
// |p_data| contains the audio data.
// The timestamp is stored in |p_timestamp|.
// Returns true on success, otherwise false.
bool A2DP_VendorGetPacketTimestampLdac(const uint8_t* p_codec_info,
                                       const uint8_t* p_data,
                                       uint32_t* p_timestamp);

// Builds A2DP LDAC codec header for audio data.
// |p_codec_info| contains the codec information.
// |p_buf| contains the audio data.
// |frames_per_packet| is the number of frames in this packet.
// Returns true on success, otherwise false.
bool A2DP_VendorBuildCodecHeaderLdac(const uint8_t* p_codec_info, BT_HDR* p_buf,
                                     uint16_t frames_per_packet);

// Decodes A2DP LDAC codec info into a human readable string.
// |p_codec_info| is a pointer to the LDAC codec_info to decode.
// Returns a string describing the codec information.
std::string A2DP_VendorCodecInfoStringLdac(const uint8_t* p_codec_info);

// Gets the A2DP LDAC encoder interface that can be used to encode and prepare
// A2DP packets for transmission - see |tA2DP_ENCODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP LDAC encoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_ENCODER_INTERFACE* A2DP_VendorGetEncoderInterfaceLdac(
    const uint8_t* p_codec_info);

// Gets the current A2DP LDAC decoder interface that can be used to decode
// received A2DP packets - see |tA2DP_DECODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP LDAC decoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_DECODER_INTERFACE* A2DP_VendorGetDecoderInterfaceLdac(
    const uint8_t* p_codec_info);

// Adjusts the A2DP LDAC codec, based on local support and Bluetooth
// specification.
// |p_codec_info| contains the codec information to adjust.
// Returns true if |p_codec_info| is valid and supported, otherwise false.
bool A2DP_VendorAdjustCodecLdac(uint8_t* p_codec_info);

// Gets the A2DP LDAC Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_VendorSourceCodecIndexLdac(
    const uint8_t* p_codec_info);

// Gets the A2DP LDAC Sink codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_VendorSinkCodecIndexLdac(
    const uint8_t* p_codec_info);

// Gets the A2DP LDAC Source codec name.
const char* A2DP_VendorCodecIndexStrLdac(void);

// Gets the A2DP LDAC Sink codec name.
const char* A2DP_VendorCodecIndexStrLdacSink(void);

// Initializes A2DP LDAC Source codec information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|.
bool A2DP_VendorInitCodecConfigLdac(AvdtpSepConfig* p_cfg);

// Initializes A2DP LDAC Sink codec information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|.
bool A2DP_VendorInitCodecConfigLdacSink(AvdtpSepConfig* p_cfg);

#endif  // A2DP_VENDOR_LDAC_H
