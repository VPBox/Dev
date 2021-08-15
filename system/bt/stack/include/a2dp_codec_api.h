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
// A2DP Codecs API
//

#ifndef A2DP_CODEC_API_H
#define A2DP_CODEC_API_H

#include <stddef.h>
#include <string.h>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <string>

#include <hardware/bt_av.h>

#include "a2dp_api.h"
#include "audio_a2dp_hw/include/audio_a2dp_hw.h"
#include "avdt_api.h"

class tBT_A2DP_OFFLOAD;

/**
 * Structure used to initialize the A2DP encoder with A2DP peer information
 */
typedef struct {
  bool is_peer_edr;          // True if the A2DP peer supports EDR
  bool peer_supports_3mbps;  // True if the A2DP peer supports 3 Mbps EDR
  uint16_t peer_mtu;         // MTU of the A2DP peer
} tA2DP_ENCODER_INIT_PEER_PARAMS;

class A2dpCodecConfig {
  friend class A2dpCodecs;

 public:
  // Creates a codec entry. The selected codec is defined by |codec_index|,
  // Returns the codec entry on success, otherwise nullptr.
  static A2dpCodecConfig* createCodec(
      btav_a2dp_codec_index_t codec_index,
      btav_a2dp_codec_priority_t codec_priority =
          BTAV_A2DP_CODEC_PRIORITY_DEFAULT);

  virtual ~A2dpCodecConfig() = 0;

  // Gets the pre-defined codec index.
  btav_a2dp_codec_index_t codecIndex() const { return codec_index_; }

  // Gets the codec name.
  const std::string& name() const { return name_; }

  // Gets the current priority of the codec.
  btav_a2dp_codec_priority_t codecPriority() const { return codec_priority_; }

  // gets current OTA codec specific config to |p_a2dp_offload->codec_info|.
  // Returns true if the current codec config is valid and copied,
  // otherwise false.
  bool getCodecSpecificConfig(tBT_A2DP_OFFLOAD* p_a2dp_offload);

  // Gets the bitRate for the A2DP codec.
  // Returns the bitrate of current codec configuration, or 0 if not configured
  int getTrackBitRate() const;

  // Copies out the current OTA codec config to |p_codec_info|.
  // Returns true if the current codec config is valid and copied,
  // otherwise false.
  bool copyOutOtaCodecConfig(uint8_t* p_codec_info);

  // Gets the current codec configuration.
  // Returns a copy of the current codec configuration.
  btav_a2dp_codec_config_t getCodecConfig();

  // Gets the current codec capability.
  // The capability is computed by intersecting the local codec's capability
  // and the peer's codec capability. However, if there is an explicit user
  // configuration for some of the parameters, the result codec configuration
  // and capability is restricted to the user's configuration choice.
  // Returns a copy of the current codec capability.
  btav_a2dp_codec_config_t getCodecCapability();

  // Gets the codec local capability.
  // Returns a copy of the codec local capability.
  btav_a2dp_codec_config_t getCodecLocalCapability();

  // Gets the codec selectable capability.
  // The capability is computed by intersecting the local codec's capability
  // and the peer's codec capability. Any explicit user configuration is
  // not included in the result.
  // Returns a copy of the codec selectable capability.
  btav_a2dp_codec_config_t getCodecSelectableCapability();

  // Gets the current codec user configuration.
  // Returns a copy of the current codec user configuration.
  btav_a2dp_codec_config_t getCodecUserConfig();

  // Gets the current codec audio configuration.
  // Returns a copy of the current codec audio configuration.
  btav_a2dp_codec_config_t getCodecAudioConfig();

  // Gets the number of bits per sample of the current codec configuration,
  // or 0 if not configured.
  uint8_t getAudioBitsPerSample();

  // Checks whether the codec uses the RTP Header Marker bit (see RFC 6416).
  // NOTE: Even if the encoded data uses RTP headers, some codecs do not use
  // the Marker bit - that bit is expected to be set to 0.
  // Returns true if the encoded data packets have RTP headers, and
  // the Marker bit in the header is set according to RFC 6416.
  virtual bool useRtpHeaderMarkerBit() const = 0;

  // Gets the effective MTU for the A2DP codec.
  // Returns the effective MTU of current codec configuration, or 0 if not
  // configured.
  virtual int getEffectiveMtu() const = 0;

  // Checks whether |codec_config| is empty and contains no configuration.
  // Returns true if |codec_config| is empty, otherwise false.
  static bool isCodecConfigEmpty(const btav_a2dp_codec_config_t& codec_config);

 protected:
  // Sets the current priority of the codec to |codec_priority|.
  // If |codec_priority| is BTAV_A2DP_CODEC_PRIORITY_DEFAULT, the priority is
  // reset to its default value.
  void setCodecPriority(btav_a2dp_codec_priority_t codec_priority);

  // Sets the current priority of the codec to its default value.
  void setDefaultCodecPriority();

  // Sets the A2DP Source-to-Sink codec configuration to be used
  // with a peer Sink device.
  // |p_peer_codec_info| is the peer's A2DP Sink codec information
  // to use. If |is_capability| is true, then |p_peer_codec_info| contains the
  // peer's A2DP Sink codec capability, otherwise it contains the peer's
  // preferred A2DP codec configuration to use.
  // The result codec configuration is stored in |p_result_codec_config|.
  // See |A2dpCodecs.setCodecConfig| for detailed description of
  // the actual mechanism used to compute the configuration.
  // Returns true on success, othewise false.
  virtual bool setCodecConfig(const uint8_t* p_peer_codec_info,
                              bool is_capability,
                              uint8_t* p_result_codec_config) = 0;

  // Sets the user prefered codec configuration.
  // |codec_user_config| contains the preferred codec user configuration.
  // |codec_audio_config| contains the selected audio feeding configuration.
  // |p_peer_params| contains the A2DP peer information.
  // |p_peer_codec_info| is the peer's A2DP Sink codec information
  // to use. If |is_capability| is true, then |p_peer_codec_info| contains the
  // peer's A2DP Sink codec capability, otherwise it contains the peer's
  // preferred A2DP codec configuration to use.
  // If there is a change in the codec configuration that requires restarting
  // if the audio input stream, flag |p_restart_input| is set to true.
  // If there is a change in the encoder configuration that requires restarting
  // of the A2DP connection, the new codec configuration is stored in
  // |p_result_codec_config|, and flag |p_restart_output| is set to true.
  // If there is any change in the codec configuration, flag |p_config_updated|
  // is set to true.
  // Returns true on success, otherwise false.
  virtual bool setCodecUserConfig(
      const btav_a2dp_codec_config_t& codec_user_config,
      const btav_a2dp_codec_config_t& codec_audio_config,
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      const uint8_t* p_peer_codec_info, bool is_capability,
      uint8_t* p_result_codec_config, bool* p_restart_input,
      bool* p_restart_output, bool* p_config_updated);

  // Updates the encoder with the user prefered codec configuration.
  // |p_peer_params| contains the A2DP peer information.
  // If there is a change in the encoder configuration that requires restarting
  // the audio input stream, flag |p_restart_input| is set to true.
  // If there is a change in the encoder configuration that requires restarting
  // of the A2DP connection, flag |p_restart_output| is set to true.
  // If there is any change in the codec configuration, flag |p_config_updated|
  // is set to true.
  // Returns true on success, otherwise false.
  virtual bool updateEncoderUserConfig(
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      bool* p_restart_input, bool* p_restart_output,
      bool* p_config_updated) = 0;

  // Sets the codec capabilities for a peer.
  // |p_peer_codec_capabiltities| is the peer codec capabilities to set.
  // Returns true on success, otherwise false.
  virtual bool setPeerCodecCapabilities(
      const uint8_t* p_peer_codec_capabilities) = 0;

  // Constructor where |codec_index| is the unique index that identifies the
  // codec. The user-friendly name is |name|.
  // The default codec priority is |codec_priority|. If the value is
  // |BTAV_A2DP_CODEC_PRIORITY_DEFAULT|, the codec priority is computed
  // internally.
  A2dpCodecConfig(btav_a2dp_codec_index_t codec_index, const std::string& name,
                  btav_a2dp_codec_priority_t codec_priority);

  // Initializes the codec entry.
  // Returns true on success, otherwise false.
  virtual bool init() = 0;

  // Checks whether the internal state is valid
  virtual bool isValid() const;

  // Returns the encoder's periodic interval (in milliseconds).
  virtual uint64_t encoderIntervalMs() const = 0;

  // Checks whether the A2DP Codec Configuration is valid.
  // Returns true if A2DP Codec Configuration stored in |codec_config|
  // is valid, otherwise false.
  static bool codecConfigIsValid(const btav_a2dp_codec_config_t& codec_config);

  // Gets the string representation of A2DP Codec Configuration.
  // Returns the string representation of A2DP Codec Configuration stored
  // in |codec_config|. The format is:
  // "Rate=44100|48000 Bits=16|24 Mode=MONO|STEREO"
  static std::string codecConfig2Str(
      const btav_a2dp_codec_config_t& codec_config);

  // Gets the string representation of A2DP Codec Sample Rate.
  // Returns the string representation of A2DP Codec Sample Rate stored
  // in |codec_sample_rate|. If there are multiple values stored in
  // |codec_sample_rate|, the return string format is "rate1|rate2|rate3".
  static std::string codecSampleRate2Str(
      btav_a2dp_codec_sample_rate_t codec_sample_rate);

  // Gets the string representation of A2DP Codec Bits Per Sample.
  // Returns the string representation of A2DP Codec Bits Per Sample stored
  // in |codec_bits_per_sample|. If there are multiple values stored in
  // |codec_bits_per_sample|, the return string format is "bits1|bits2|bits3".
  static std::string codecBitsPerSample2Str(
      btav_a2dp_codec_bits_per_sample_t codec_bits_per_sample);

  // Gets the string representation of A2DP Codec Channel Mode.
  // Returns the string representation of A2DP Channel Mode stored
  // in |codec_channel_mode|. If there are multiple values stored in
  // |codec_channel_mode|, the return string format is "mode1|mode2|mode3".
  static std::string codecChannelMode2Str(
      btav_a2dp_codec_channel_mode_t codec_channel_mode);

  // Dumps codec-related information.
  // The information is written in user-friendly form to file descriptor |fd|.
  virtual void debug_codec_dump(int fd);

  std::recursive_mutex codec_mutex_;
  const btav_a2dp_codec_index_t codec_index_;  // The unique codec index
  const std::string name_;                     // The codec name
  btav_a2dp_codec_priority_t codec_priority_;  // Codec priority: must be unique
  btav_a2dp_codec_priority_t default_codec_priority_;

  btav_a2dp_codec_config_t codec_config_;
  btav_a2dp_codec_config_t codec_capability_;
  btav_a2dp_codec_config_t codec_local_capability_;
  btav_a2dp_codec_config_t codec_selectable_capability_;

  // The optional user configuration. The values (if set) are used
  // as a preference when there is a choice. If a particular value
  // is not supported by the local or remote device, it is ignored.
  btav_a2dp_codec_config_t codec_user_config_;

  // The selected audio feeding configuration.
  btav_a2dp_codec_config_t codec_audio_config_;

  uint8_t ota_codec_config_[AVDT_CODEC_SIZE];
  uint8_t ota_codec_peer_capability_[AVDT_CODEC_SIZE];
  uint8_t ota_codec_peer_config_[AVDT_CODEC_SIZE];
};

class A2dpCodecs {
 public:
  // Constructor for class |A2dpCodecs|.
  // |codec_priorities| contains the codec priorities to use.
  A2dpCodecs(const std::vector<btav_a2dp_codec_config_t>& codec_priorities);
  ~A2dpCodecs();

  // Initializes all supported codecs.
  // Returns true if at least one Source codec and one Sink codec were
  // initialized, otherwise false.
  bool init();

  // Finds the Source codec that corresponds to the A2DP over-the-air
  // |p_codec_info| information.
  // Returns the Source codec if found, otherwise nullptr.
  A2dpCodecConfig* findSourceCodecConfig(const uint8_t* p_codec_info);

  // Finds the Sink codec that corresponds to the A2DP over-the-air
  // |p_codec_info| information.
  // Returns the Sink codec if found, otherwise nullptr.
  A2dpCodecConfig* findSinkCodecConfig(const uint8_t* p_codec_info);

  // Checks whether the codec for |codec_index| is supported.
  // Returns true if the codec is supported, otherwise false.
  bool isSupportedCodec(btav_a2dp_codec_index_t codec_index);

  // Gets the codec config that is currently selected.
  // Returns the codec config that is currently selected, or nullptr if
  // no codec is selected.
  A2dpCodecConfig* getCurrentCodecConfig() const {
    return current_codec_config_;
  }

  // Gets the list of Source codecs ordered by priority: higher priority first.
  const std::list<A2dpCodecConfig*> orderedSourceCodecs() const {
    return ordered_source_codecs_;
  }

  // Gets the list of Sink codecs ordered by priority: higher priority first.
  const std::list<A2dpCodecConfig*> orderedSinkCodecs() const {
    return ordered_sink_codecs_;
  }

  // Sets the A2DP Source-to-Sink codec configuration to be used
  // with a peer Sink device.
  // |p_peer_codec_info| is the peer's A2DP Sink codec information
  // to use. If |is_capability| is true, then |p_peer_codec_info| contains the
  // peer's A2DP Sink codec capability, otherwise it contains the peer's
  // preferred A2DP codec configuration to use.
  // If the codec can be used and |select_current_codec| is true, then
  // this codec is selected as the current one.
  //
  // The codec configuration is built by considering the optional user
  // configuration, the local codec capabilities, the peer's codec
  // capabilities, and the codec's locally-defined default values.
  // For each codec parameter:
  //
  // 1. If it is user-configurable parameter (sample rate, bits per sample,
  //    channel mode, and some codec-specific parameters),
  //    if the user has an explicit preference, and that preference
  //    is supported by both the local and remote device, this is the
  //    parameter value that is used.
  // 2. Otherwise, if the explicit internal default value is supported
  //    by both the local and remote device, this is the parameter value
  //    that is used.
  // 3. Otherwise, the best match is chosen among all values supported by
  //    the local and remote device.
  //
  // In addition, the codec's internal state is updated to reflect
  // the capabilities that are advertised to the upstream audio source
  // (Media Framework) to make run-time audio parameter choices:
  // 4. If the user-configurable parameter was selected, this is the
  //    only parameter value that is advertised to the Media Framework.
  // 5. Otherwise, all values supported by both the local and remote
  //    devices are advertised to the Media Framework.
  //
  // The result codec configuration is stored in |p_result_codec_config|.
  // Returns true on success, othewise false.
  bool setCodecConfig(const uint8_t* p_peer_codec_info, bool is_capability,
                      uint8_t* p_result_codec_config,
                      bool select_current_codec);

  // Sets the A2DP Sink codec configuration to be used with a peer Source
  // device.
  // [See setCodecConfig() for description]
  bool setSinkCodecConfig(const uint8_t* p_peer_codec_info, bool is_capability,
                          uint8_t* p_result_codec_config,
                          bool select_current_codec);

  // Sets the user prefered codec configuration.
  // |codec_user_config| contains the preferred codec configuration.
  // |p_peer_params| contains the A2DP peer information.
  // |p_peer_sink_capabilities| is the peer's A2DP Sink codec capabilities
  // to use.
  // If there is a change in the encoder configuration that requires restarting
  // the audio input stream, flag |p_restart_input| is set to true.
  // If there is a change in the encoder configuration that requires restarting
  // of the A2DP connection, flag |p_restart_output| is set to true, and the
  // new codec is stored in |p_result_codec_config|.
  // If there is any change in the codec configuration, flag |p_config_updated|
  // is set to true.
  // Returns true on success, otherwise false.
  bool setCodecUserConfig(const btav_a2dp_codec_config_t& codec_user_config,
                          const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
                          const uint8_t* p_peer_sink_capabilities,
                          uint8_t* p_result_codec_config, bool* p_restart_input,
                          bool* p_restart_output, bool* p_config_updated);

  // Sets the Audio HAL selected audio feeding parameters.
  // Those parameters are applied only to the currently selected codec.
  // |codec_audio_config| contains the selected audio feeding configuration.
  // |p_peer_params| contains the A2DP peer information.
  // |p_peer_sink_capabilities| is the peer's A2DP Sink codec capabilities
  // to use.
  // If there is a change in the encoder configuration that requires restarting
  // of the A2DP connection, flag |p_restart_output| is set to true, and the
  // new codec is stored in |p_result_codec_config|.
  // If there is any change in the codec configuration, flag |p_config_updated|
  // is set to true.
  // Returns true on success, otherwise false.
  bool setCodecAudioConfig(const btav_a2dp_codec_config_t& codec_audio_config,
                           const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
                           const uint8_t* p_peer_sink_capabilities,
                           uint8_t* p_result_codec_config,
                           bool* p_restart_output, bool* p_config_updated);

  // Sets the Over-The-Air preferred codec configuration.
  // The OTA prefered codec configuration is ignored if the current
  // codec configuration contains explicit user configuration, or if the
  // codec configuration for the same codec contains explicit user
  // configuration.
  // |p_ota_codec_config| contains the received OTA A2DP codec configuration
  // from the remote peer. Note: this is not the peer codec capability,
  // but the codec configuration that the peer would like to use.
  // |p_peer_params| contains the A2DP peer information.
  // If there is a change in the encoder configuration that requires restarting
  // the audio input stream, flag |p_restart_input| is set to true.
  // If there is a change in the encoder configuration that requires restarting
  // of the A2DP connection, flag |p_restart_output| is set to true, and the
  // new codec is stored in |p_result_codec_config|.
  // If there is any change in the codec configuration, flag |p_config_updated|
  // is set to true.
  // Returns true on success, otherwise false.
  bool setCodecOtaConfig(const uint8_t* p_ota_codec_config,
                         const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
                         uint8_t* p_result_codec_config, bool* p_restart_input,
                         bool* p_restart_output, bool* p_config_updated);

  // Sets the codec capabilities for a Sink peer.
  // |p_peer_codec_capabiltities| is the peer codec capabilities to set.
  // Returns true on success, otherwise false.
  bool setPeerSinkCodecCapabilities(const uint8_t* p_peer_codec_capabilities);

  // Sets the codec capabilities for a Source peer.
  // |p_peer_codec_capabiltities| is the peer codec capabilities to set.
  // Returns true on success, otherwise false.
  bool setPeerSourceCodecCapabilities(const uint8_t* p_peer_codec_capabilities);

  // Gets the current codec configuration and the capabilities of
  // all configured codecs.
  // The current codec configuration is stored in |p_codec_config|.
  // Local device's codecs capabilities are stored in
  // |p_codecs_local_capabilities|.
  // The codecs capabilities that can be used between the local device
  // and the remote device are stored in |p_codecs_selectable_capabilities|.
  // Returns true on success, otherwise false.
  bool getCodecConfigAndCapabilities(
      btav_a2dp_codec_config_t* p_codec_config,
      std::vector<btav_a2dp_codec_config_t>* p_codecs_local_capabilities,
      std::vector<btav_a2dp_codec_config_t>* p_codecs_selectable_capabilities);

  // Dumps codec-related information.
  // The information is written in user-friendly form to file descriptor |fd|.
  void debug_codec_dump(int fd);

 private:
  struct CompareBtBdaddr
      : public std::binary_function<RawAddress, RawAddress, bool> {
    bool operator()(const RawAddress& lhs, const RawAddress& rhs) const {
      return (memcmp(&lhs, &rhs, sizeof(lhs)) < 0);
    }
  };
  typedef std::map<btav_a2dp_codec_index_t, A2dpCodecConfig*> IndexedCodecs;

  std::recursive_mutex codec_mutex_;
  A2dpCodecConfig* current_codec_config_;  // Currently selected codec
  std::map<btav_a2dp_codec_index_t, btav_a2dp_codec_priority_t>
      codec_priorities_;

  IndexedCodecs indexed_codecs_;           // The codecs indexed by codec index
  IndexedCodecs disabled_codecs_;          // The disabled codecs

  // A2DP Source codecs ordered by priority
  std::list<A2dpCodecConfig*> ordered_source_codecs_;

  // A2DP Sink codecs ordered by priority
  std::list<A2dpCodecConfig*> ordered_sink_codecs_;

  std::map<RawAddress, IndexedCodecs*, CompareBtBdaddr> peer_codecs_;
};

/**
 * Structure used to configure the A2DP feeding.
 */
typedef struct {
  tA2DP_SAMPLE_RATE sample_rate;          // 44100, 48000, etc
  tA2DP_BITS_PER_SAMPLE bits_per_sample;  // 8, 16, 24, 32
  tA2DP_CHANNEL_COUNT channel_count;      // 1 for mono or 2 for stereo
} tA2DP_FEEDING_PARAMS;

// Prototype for a callback to read audio data for encoding.
// |p_buf| is the buffer to store the data. |len| is the number of octets to
// read.
// Returns the number of octets read.
typedef uint32_t (*a2dp_source_read_callback_t)(uint8_t* p_buf, uint32_t len);

// Prototype for a callback to enqueue A2DP Source packets for transmission.
// |p_buf| is the buffer with the audio data to enqueue. The callback is
// responsible for freeing |p_buf|.
// |frames_n| is the number of audio frames in |p_buf| - it is used for
// statistics purpose.
// |num_bytes| is the number of audio bytes in |p_buf| - it is used for
// delay reporting.
// Returns true if the packet was enqueued, otherwise false.
typedef bool (*a2dp_source_enqueue_callback_t)(BT_HDR* p_buf, size_t frames_n,
                                               uint32_t num_bytes);

//
// A2DP encoder callbacks interface.
//
typedef struct {
  // Initialize the A2DP encoder.
  // |p_peer_params| contains the A2DP peer information
  // The current A2DP codec config is in |a2dp_codec_config|.
  // |read_callback| is the callback for reading the input audio data.
  // |enqueue_callback| is the callback for enqueueing the encoded audio data.
  void (*encoder_init)(const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
                       A2dpCodecConfig* a2dp_codec_config,
                       a2dp_source_read_callback_t read_callback,
                       a2dp_source_enqueue_callback_t enqueue_callback);

  // Cleanup the A2DP encoder.
  void (*encoder_cleanup)(void);

  // Reset the feeding for the A2DP encoder.
  void (*feeding_reset)(void);

  // Flush the feeding for the A2DP encoder.
  void (*feeding_flush)(void);

  // Get the A2DP encoder interval (in milliseconds).
  uint64_t (*get_encoder_interval_ms)(void);

  // Prepare and send A2DP encoded frames.
  // |timestamp_us| is the current timestamp (in microseconds).
  void (*send_frames)(uint64_t timestamp_us);

  // Set transmit queue length for the A2DP encoder.
  void (*set_transmit_queue_length)(size_t transmit_queue_length);
} tA2DP_ENCODER_INTERFACE;

// Prototype for a callback to receive decoded audio data from a
// tA2DP_DECODER_INTERFACE|.
// |buf| is a pointer to the data.
// |len| is the number of octets pointed to by |buf|.
typedef void (*decoded_data_callback_t)(uint8_t* buf, uint32_t len);

//
// A2DP decoder callbacks interface.
//
typedef struct {
  // Initialize the decoder. Can be called multiple times, will reinitalize.
  bool (*decoder_init)(decoded_data_callback_t decode_callback);

  // Cleanup the A2DP decoder.
  void (*decoder_cleanup)();

  // Decodes |p_buf| and calls |decode_callback| passed into init for the
  // decoded data.
  bool (*decode_packet)(BT_HDR* p_buf);
} tA2DP_DECODER_INTERFACE;

// Gets the A2DP codec type.
// |p_codec_info| contains information about the codec capabilities.
tA2DP_CODEC_TYPE A2DP_GetCodecType(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP Source codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid codec,
// otherwise false.
bool A2DP_IsSourceCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid codec,
// otherwise false.
bool A2DP_IsSinkCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid codec,
// otherwise false.
bool A2DP_IsPeerSourceCodecValid(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid codec,
// otherwise false.
bool A2DP_IsPeerSinkCodecValid(const uint8_t* p_codec_info);

// Checks whether an A2DP Sink codec is supported.
// |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP Sink codec is supported, otherwise false.
bool A2DP_IsSinkCodecSupported(const uint8_t* p_codec_info);

// Checks whether an A2DP Source codec for a peer Source device is supported.
// |p_codec_info| contains information about the codec capabilities of the
// peer device.
// Returns true if the A2DP Source codec for a peer Source device is supported,
// otherwise false.
bool A2DP_IsPeerSourceCodecSupported(const uint8_t* p_codec_info);

// Initialize state with the default A2DP codec.
// The initialized state with the codec capabilities is stored in
// |p_codec_info|.
void A2DP_InitDefaultCodec(uint8_t* p_codec_info);

// Checks whether the A2DP data packets should contain RTP header.
// |content_protection_enabled| is true if Content Protection is
// enabled. |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP data packets should contain RTP header, otherwise
// false.
bool A2DP_UsesRtpHeader(bool content_protection_enabled,
                        const uint8_t* p_codec_info);

// Gets the |AVDT_MEDIA_TYPE_*| media type from the codec capability
// in |p_codec_info|.
uint8_t A2DP_GetMediaType(const uint8_t* p_codec_info);

// Gets the A2DP codec name for a given |p_codec_info|.
const char* A2DP_CodecName(const uint8_t* p_codec_info);

// Checks whether two A2DP codecs |p_codec_info_a| and |p_codec_info_b| have
// the same type.
// Returns true if the two codecs have the same type, otherwise false.
// If the codec type is not recognized, the return value is false.
bool A2DP_CodecTypeEquals(const uint8_t* p_codec_info_a,
                          const uint8_t* p_codec_info_b);

// Checks whether two A2DP codecs p_codec_info_a| and |p_codec_info_b| are
// exactly the same.
// Returns true if the two codecs are exactly the same, otherwise false.
// If the codec type is not recognized, the return value is false.
bool A2DP_CodecEquals(const uint8_t* p_codec_info_a,
                      const uint8_t* p_codec_info_b);

// Gets the track sample rate value for the A2DP codec.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackSampleRate(const uint8_t* p_codec_info);

// Gets the track bits per sample value for the A2DP codec.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns the track bits per sample on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackBitsPerSample(const uint8_t* p_codec_info);

// Gets the channel count for the A2DP codec.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackChannelCount(const uint8_t* p_codec_info);

// Gets the channel type for the A2DP Sink codec:
// 1 for mono, or 3 for dual/stereo/joint.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns the channel type on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetSinkTrackChannelType(const uint8_t* p_codec_info);

// Gets the A2DP audio data timestamp from an audio packet.
// |p_codec_info| contains the codec information.
// |p_data| contains the audio data.
// The timestamp is stored in |p_timestamp|.
// Returns true on success, otherwise false.
bool A2DP_GetPacketTimestamp(const uint8_t* p_codec_info, const uint8_t* p_data,
                             uint32_t* p_timestamp);

// Builds A2DP codec header for audio data.
// |p_codec_info| contains the codec information.
// |p_buf| contains the audio data.
// |frames_per_packet| is the number of frames in this packet.
// Returns true on success, otherwise false.
bool A2DP_BuildCodecHeader(const uint8_t* p_codec_info, BT_HDR* p_buf,
                           uint16_t frames_per_packet);

// Gets the A2DP encoder interface that can be used to encode and prepare
// A2DP packets for transmission - see |tA2DP_ENCODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP encoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_ENCODER_INTERFACE* A2DP_GetEncoderInterface(
    const uint8_t* p_codec_info);

// Gets the A2DP decoder interface that can be used to decode received A2DP
// packets - see |tA2DP_DECODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP decoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_DECODER_INTERFACE* A2DP_GetDecoderInterface(
    const uint8_t* p_codec_info);

// Adjusts the A2DP codec, based on local support and Bluetooth specification.
// |p_codec_info| contains the codec information to adjust.
// Returns true if |p_codec_info| is valid and supported, otherwise false.
bool A2DP_AdjustCodec(uint8_t* p_codec_info);

// Gets the A2DP Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SourceCodecIndex(const uint8_t* p_codec_info);

// Gets the A2DP Sink codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SinkCodecIndex(const uint8_t* p_codec_info);

// Gets the A2DP codec name for a given |codec_index|.
const char* A2DP_CodecIndexStr(btav_a2dp_codec_index_t codec_index);

// Initializes A2DP codec-specific information into |AvdtpSepConfig|
// configuration entry pointed by |p_cfg|. The selected codec is defined
// by |codec_index|.
// Returns true on success, otherwise false.
bool A2DP_InitCodecConfig(btav_a2dp_codec_index_t codec_index,
                          AvdtpSepConfig* p_cfg);

// Decodes A2DP codec info into a human readable string.
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns a string describing the codec information.
std::string A2DP_CodecInfoString(const uint8_t* p_codec_info);

// Add enum-based flag operators to the btav_a2dp_codec_config_t fields
#ifndef DEFINE_ENUM_FLAG_OPERATORS
// Use NOLINT to suppress missing parentheses warnings around bitmask.
#define DEFINE_ENUM_FLAG_OPERATORS(bitmask)                                 \
  extern "C++" {                                                            \
  inline constexpr bitmask operator&(bitmask X, bitmask Y) {  /* NOLINT */  \
    return static_cast<bitmask>(static_cast<int>(X) & static_cast<int>(Y)); \
  }                                                                         \
  inline constexpr bitmask operator|(bitmask X, bitmask Y) {  /* NOLINT */  \
    return static_cast<bitmask>(static_cast<int>(X) | static_cast<int>(Y)); \
  }                                                                         \
  inline constexpr bitmask operator^(bitmask X, bitmask Y) {  /* NOLINT */  \
    return static_cast<bitmask>(static_cast<int>(X) ^ static_cast<int>(Y)); \
  }                                                                         \
  inline constexpr bitmask operator~(bitmask X) {             /* NOLINT */  \
    return static_cast<bitmask>(~static_cast<int>(X));                      \
  }                                                                         \
  inline bitmask& operator&=(bitmask& X, bitmask Y) {         /* NOLINT */  \
    X = X & Y;                                                              \
    return X;                                                               \
  }                                                                         \
  inline bitmask& operator|=(bitmask& X, bitmask Y) {         /* NOLINT */  \
    X = X | Y;                                                              \
    return X;                                                               \
  }                                                                         \
  inline bitmask& operator^=(bitmask& X, bitmask Y) {         /* NOLINT */  \
    X = X ^ Y;                                                              \
    return X;                                                               \
  }                                                                         \
  }
#endif  // DEFINE_ENUM_FLAG_OPERATORS
DEFINE_ENUM_FLAG_OPERATORS(btav_a2dp_codec_sample_rate_t);
DEFINE_ENUM_FLAG_OPERATORS(btav_a2dp_codec_bits_per_sample_t);
DEFINE_ENUM_FLAG_OPERATORS(btav_a2dp_codec_channel_mode_t);

#endif  // A2DP_CODEC_API_H
