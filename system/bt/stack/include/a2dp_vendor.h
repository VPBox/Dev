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
// Vendor Specific A2DP Codecs Support
//

#ifndef A2DP_VENDOR_H
#define A2DP_VENDOR_H

#include <stdbool.h>
#include "a2dp_codec_api.h"

/* Offset for A2DP vendor codec */
#define A2DP_VENDOR_CODEC_START_IDX 3

/* Offset for Vendor ID for A2DP vendor codec */
#define A2DP_VENDOR_CODEC_VENDOR_ID_START_IDX A2DP_VENDOR_CODEC_START_IDX

/* Offset for Codec ID for A2DP vendor codec */
#define A2DP_VENDOR_CODEC_CODEC_ID_START_IDX \
  (A2DP_VENDOR_CODEC_VENDOR_ID_START_IDX + sizeof(uint32_t))

// Checks whether the codec capabilities contain a valid A2DP vendor-specific
// Source codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid
// vendor-specific codec, otherwise false.
bool A2DP_IsVendorSourceCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP vendor-specific
// Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid
// vendor-specific codec, otherwise false.
bool A2DP_IsVendorSinkCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP
// vendor-specific Source codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid
// vendor-specific codec, otherwise false.
bool A2DP_IsVendorPeerSourceCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP
// vendor-specific Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid
// vendor-specific codec, otherwise false.
bool A2DP_IsVendorPeerSinkCodecValid(const uint8_t* p_codec_info);

// Checks whether a vendor-specific A2DP Sink codec is supported.
// |p_codec_info| contains information about the codec capabilities.
// Returns true if the vendor-specific A2DP Sink codec is supported,
// otherwise false.
bool A2DP_IsVendorSinkCodecSupported(const uint8_t* p_codec_info);

// Checks whether a vendor-specific A2DP Source codec for a peer Source device
// is supported.
// |p_codec_info| contains information about the codec capabilities of the
// peer device.
// Returns true if the vendor-specific A2DP Source codec for a peer Source
// device is supported, otherwise false.
bool A2DP_IsVendorPeerSourceCodecSupported(const uint8_t* p_codec_info);

// Gets the Vendor ID for the vendor-specific A2DP codec.
// |p_codec_info| contains information about the codec capabilities.
// Returns the Vendor ID for the vendor-specific A2DP codec.
uint32_t A2DP_VendorCodecGetVendorId(const uint8_t* p_codec_info);

// Gets the Codec ID for the vendor-specific A2DP codec.
// |p_codec_info| contains information about the codec capabilities.
// Returns the Codec ID for the vendor-specific A2DP codec.
uint16_t A2DP_VendorCodecGetCodecId(const uint8_t* p_codec_info);

// Checks whether the A2DP vendor-specific data packets should contain RTP
// header. |content_protection_enabled| is true if Content Protection is
// enabled. |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP vendor-specific data packets should contain RTP
// header, otherwise false.
bool A2DP_VendorUsesRtpHeader(bool content_protection_enabled,
                              const uint8_t* p_codec_info);

// Gets the A2DP vendor-specific codec name for a given |p_codec_info|.
const char* A2DP_VendorCodecName(const uint8_t* p_codec_info);

// Checks whether two A2DP vendor-specific codecs |p_codec_info_a| and
// |p_codec_info_b| have the same type.
// Returns true if the two codecs have the same type, otherwise false.
// If the codec type is not recognized, the return value is false.
bool A2DP_VendorCodecTypeEquals(const uint8_t* p_codec_info_a,
                                const uint8_t* p_codec_info_b);

// Checks whether two A2DP vendor-specific codecs |p_codec_info_a| and
// |p_codec_info_b| are exactly the same.
// Returns true if the two codecs are exactly the same, otherwise false.
// If the codec type is not recognized, the return value is false.
bool A2DP_VendorCodecEquals(const uint8_t* p_codec_info_a,
                            const uint8_t* p_codec_info_b);

// Gets the track sample rate value for the A2DP vendor-specific codec.
// |p_codec_info| is a pointer to the vendor-specific codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackSampleRate(const uint8_t* p_codec_info);

// Gets the track bits per sample value for the A2DP vendor-specific codec.
// |p_codec_info| is a pointer to the vendor-specific codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackBitsPerSample(const uint8_t* p_codec_info);

// Gets the channel count for the A2DP vendor-specific codec.
// |p_codec_info| is a pointer to the vendor-specific codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetTrackChannelCount(const uint8_t* p_codec_info);

// Gets the bitrate for the A2DP vendor-specific codec.
// |p_codec_info| is a pointer to the vendor-specific codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetBitRate(const uint8_t* p_codec_info);

// Gets the channel type for the A2DP vendor-specific Sink codec:
// 1 for mono, or 3 for dual/stereo/joint.
// |p_codec_info| is a pointer to the vendor-specific codec_info to decode.
// Returns the channel type on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_VendorGetSinkTrackChannelType(const uint8_t* p_codec_info);

// Gets the A2DP codec-specific audio data timestamp from an audio packet.
// |p_codec_info| contains the codec information.
// |p_data| contains the audio data.
// The timestamp is stored in |p_timestamp|.
// Returns true on success, otherwise false.
bool A2DP_VendorGetPacketTimestamp(const uint8_t* p_codec_info,
                                   const uint8_t* p_data,
                                   uint32_t* p_timestamp);

// Builds A2DP vendor-specific codec header for audio data.
// |p_codec_info| contains the codec information.
// |p_buf| contains the audio data.
// |frames_per_packet| is the number of frames in this packet.
// Returns true on success, otherwise false.
bool A2DP_VendorBuildCodecHeader(const uint8_t* p_codec_info, BT_HDR* p_buf,
                                 uint16_t frames_per_packet);

// Gets the A2DP vendor encoder interface that can be used to encode and
// prepare A2DP packets for transmission - see |tA2DP_ENCODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP vendor encoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_ENCODER_INTERFACE* A2DP_VendorGetEncoderInterface(
    const uint8_t* p_codec_info);

// Gets the current A2DP vendor decoder interface that can be used to decode
// received A2DP packets - see |tA2DP_DECODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP vendor decoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_DECODER_INTERFACE* A2DP_VendorGetDecoderInterface(
    const uint8_t* p_codec_info);

// Adjusts the A2DP vendor-specific codec, based on local support and Bluetooth
// specification.
// |p_codec_info| contains the codec information to adjust.
// Returns true if |p_codec_info| is valid and supported, otherwise false.
bool A2DP_VendorAdjustCodec(uint8_t* p_codec_info);

// Gets the A2DP vendor Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_VendorSourceCodecIndex(
    const uint8_t* p_codec_info);

// Gets the A2DP vendor Sink codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_VendorSinkCodecIndex(const uint8_t* p_codec_info);

// Gets the A2DP vendor codec name for a given |codec_index|.
const char* A2DP_VendorCodecIndexStr(btav_a2dp_codec_index_t codec_index);

// Initializes A2DP vendor codec-specific information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|. The selected codec is defined by
// |codec_index|.
// Returns true on success, otherwise false.
bool A2DP_VendorInitCodecConfig(btav_a2dp_codec_index_t codec_index,
                                AvdtpSepConfig* p_cfg);

// Decodes A2DP vendor codec info into a human readable string.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns a string describing the codec information.
std::string A2DP_VendorCodecInfoString(const uint8_t* p_codec_info);

#endif  // A2DP_VENDOR_H
