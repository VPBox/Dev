/******************************************************************************
 *
 *  Copyright 2004-2012 Broadcom Corporation
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
 *  This is the advanced audio/video call-out function implementation for
 *  BTIF.
 *
 ******************************************************************************/

#include <mutex>

#include <base/bind.h>
#include <base/logging.h>
#include <string.h>

#include "bt_target.h"

#include "a2dp_api.h"
#include "a2dp_sbc.h"
#include "bta_av_api.h"
#include "bta_av_ci.h"
#include "bta_av_co.h"
#include "bta_sys.h"

#include "btif_av.h"
#include "btif_av_co.h"
#include "btif_util.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"

// Macro to retrieve the number of elements in a statically allocated array
#define BTA_AV_CO_NUM_ELEMENTS(__a) (sizeof(__a) / sizeof((__a)[0]))

// Macro to convert BTA AV audio handle to index and vice versa
#define BTA_AV_CO_AUDIO_HANDLE_TO_INDEX(bta_av_handle) \
  (((bta_av_handle) & (~BTA_AV_CHNL_MSK)) - 1)
#define BTA_AV_CO_AUDIO_INDEX_TO_HANDLE(index) \
  (((index) + 1) | BTA_AV_CHNL_AUDIO)

class BtaAvCoSep {
 public:
  BtaAvCoSep()
      : sep_info_idx(0), seid(0), codec_caps{}, num_protect(0), protect_info{} {
    Reset();
  }

  /**
   * Reset the state.
   */
  void Reset() {
    sep_info_idx = 0;
    seid = 0;
    memset(codec_caps, 0, sizeof(codec_caps));
    num_protect = 0;
    memset(protect_info, 0, sizeof(protect_info));
  }

  uint8_t sep_info_idx;                    // Local SEP index (in BTA tables)
  uint8_t seid;                            // Peer SEP index (in peer tables)
  uint8_t codec_caps[AVDT_CODEC_SIZE];     // Peer SEP codec capabilities
  uint8_t num_protect;                     // Peer SEP number of CP elements
  uint8_t protect_info[AVDT_CP_INFO_LEN];  // Peer SEP content protection info
};

class BtaAvCoPeer {
 public:
  BtaAvCoPeer()
      : addr(RawAddress::kEmpty),
        num_sinks(0),
        num_sources(0),
        num_seps(0),
        num_rx_sinks(0),
        num_rx_sources(0),
        num_sup_sinks(0),
        num_sup_sources(0),
        p_sink(nullptr),
        p_source(nullptr),
        codec_config{},
        acceptor(false),
        reconfig_needed(false),
        opened(false),
        mtu(0),
        uuid_to_connect(0),
        bta_av_handle_(0),
        codecs_(nullptr),
        content_protect_active_(false) {
    Reset(0);
  }

  /**
   * Initialize the state.
   *
   * @param codec_priorities the codec priorities to use for the initialization
   */
  void Init(const std::vector<btav_a2dp_codec_config_t>& codec_priorities);

  /**
   * Reset the state.
   *
   * @param bta_av_handle the BTA AV handle to use
   */
  void Reset(tBTA_AV_HNDL bta_av_handle);

  /**
   * Get the BTA AV handle.
   *
   * @return the BTA AV handle
   */
  tBTA_AV_HNDL BtaAvHandle() const { return bta_av_handle_; }

  /**
   * Get the A2DP codecs.
   *
   * @return the A2DP codecs
   */
  A2dpCodecs* GetCodecs() { return codecs_; }

  bool ContentProtectActive() const { return content_protect_active_; }
  void SetContentProtectActive(bool cp_active) {
    content_protect_active_ = cp_active;
  }

  RawAddress addr;                                // Peer address
  BtaAvCoSep sinks[BTAV_A2DP_CODEC_INDEX_MAX];    // Supported sinks
  BtaAvCoSep sources[BTAV_A2DP_CODEC_INDEX_MAX];  // Supported sources
  uint8_t num_sinks;                      // Total number of sinks at peer
  uint8_t num_sources;                    // Total number of sources at peer
  uint8_t num_seps;                       // Total number of SEPs at peer
  uint8_t num_rx_sinks;                   // Number of received sinks
  uint8_t num_rx_sources;                 // Number of received sources
  uint8_t num_sup_sinks;                  // Number of supported sinks
  uint8_t num_sup_sources;                // Number of supported sources
  const BtaAvCoSep* p_sink;               // Currently selected sink
  const BtaAvCoSep* p_source;             // Currently selected source
  uint8_t codec_config[AVDT_CODEC_SIZE];  // Current codec configuration
  bool acceptor;                          // True if acceptor
  bool reconfig_needed;                   // True if reconfiguration is needed
  bool opened;                            // True if opened
  uint16_t mtu;                           // Maximum Transmit Unit size
  uint16_t uuid_to_connect;               // UUID of peer device

 private:
  tBTA_AV_HNDL bta_av_handle_;   // BTA AV handle to use
  A2dpCodecs* codecs_;           // Locally supported codecs
  bool content_protect_active_;  // True if Content Protect is active
};

class BtaAvCo {
 public:
  BtaAvCo(bool content_protect_enabled)
      : active_peer_(nullptr),
        codec_config_{},
        content_protect_enabled_(content_protect_enabled),
        content_protect_flag_(0) {
    Reset();
  }

  /**
   * Initialize the state.
   *
   * @param codec_priorities the codec priorities to use for the initialization
   */
  void Init(const std::vector<btav_a2dp_codec_config_t>& codec_priorities);

  /**
   * Checks whether a codec is supported.
   *
   * @param codec_index the index of the codec to check
   * @return true if the codec is supported, otherwise false
   */
  bool IsSupportedCodec(btav_a2dp_codec_index_t codec_index);

  /**
   * Get the current codec configuration for the active peer.
   *
   * @return the current codec configuration if found, otherwise nullptr
   */
  A2dpCodecConfig* GetActivePeerCurrentCodec();

  /**
   * Get the current codec configuration for a peer.
   *
   * @param peer_address the peer address
   * @return the current codec configuration if found, otherwise nullptr
   */
  A2dpCodecConfig* GetPeerCurrentCodec(const RawAddress& peer_address);

  /**
   * Find the peer UUID for a given BTA AV handle.
   *
   * @param bta_av_handle the BTA AV handle to use
   * @return the peer UUID if found, otherwise 0
   */
  uint16_t FindPeerUuid(tBTA_AV_HNDL bta_av_handle);

  /**
   * Process the AVDTP discovery result: number of Stream End Points (SEP)
   * found during the AVDTP stream discovery process.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param num_seps the number of discovered SEPs
   * @param num_sinks number of discovered Sink SEPs
   * @param num_sources number of discovered Source SEPs
   * @param uuid_local local UUID
   */
  void ProcessDiscoveryResult(tBTA_AV_HNDL bta_av_handle,
                              const RawAddress& peer_address, uint8_t num_seps,
                              uint8_t num_sinks, uint8_t num_sources,
                              uint16_t uuid_local);

  /**
   * Process retrieved codec configuration and content protection from
   * Peer Sink SEP.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param p_codec_info the peer sink capability filled-in by the caller.
   * On success, it will contain the current codec configuration for the peer.
   * @param p_sep_info_idx the peer SEP index for the corresponding peer
   * sink capability filled-in by the caller. On success, it will contain
   * the SEP index for the current codec configuration for the peer.
   * @param seid the peer SEP index in peer tables
   * @param p_num_protect the peer SEP number of content protection elements
   * filled-in by the caller. On success, it will contain the SEP number of
   * content protection elements for the current codec configuration for the
   * peer.
   * @param p_protect_info the peer SEP content protection info filled-in by
   * the caller. On success, it will contain the SEP content protection info
   * for the current codec configuration for the peer.
   * @return A2DP_SUCCESS on success, otherwise A2DP_FAIL
   */
  tA2DP_STATUS ProcessSourceGetConfig(tBTA_AV_HNDL bta_av_handle,
                                      const RawAddress& peer_address,
                                      uint8_t* p_codec_info,
                                      uint8_t* p_sep_info_idx, uint8_t seid,
                                      uint8_t* p_num_protect,
                                      uint8_t* p_protect_info);

  /**
   * Process retrieved codec configuration and content protection from
   * Peer Source SEP.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param p_codec_info the peer source capability filled-in by the caller.
   * On success, it will contain the current codec configuration for the peer.
   * @param p_sep_info_idx the peer SEP index for the corresponding peer
   * source capability filled-in by the caller. On success, it will contain
   * the SEP index for the current codec configuration for the peer.
   * @param seid the peer SEP index in peer tables
   * @param p_num_protect the peer SEP number of content protection elements
   * filled-in by the caller. On success, it will contain the SEP number of
   * content protection elements for the current codec configuration for the
   * peer.
   * @param p_protect_info the peer SEP content protection info filled-in by
   * the caller. On success, it will contain the SEP content protection info
   * for the current codec configuration for the peer.
   * @return A2DP_SUCCESS on success, otherwise A2DP_FAIL
   */
  tA2DP_STATUS ProcessSinkGetConfig(tBTA_AV_HNDL bta_av_handle,
                                    const RawAddress& peer_address,
                                    uint8_t* p_codec_info,
                                    uint8_t* p_sep_info_idx, uint8_t seid,
                                    uint8_t* p_num_protect,
                                    uint8_t* p_protect_info);

  /**
   * Process AVDTP Set Config to set the codec and content protection
   * configuration of the audio stream.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param p_codec_info the codec configuration to set
   * @param seid stream endpoint ID of stream initiating the operation
   * @param peer_address the peer address
   * @param num_protect the peer SEP number of content protection elements
   * @param p_protect_info the peer SEP conntent protection info
   * @param t_local_sep the local SEP: AVDT_TSEP_SRC or AVDT_TSEP_SNK
   * @param avdt_handle the AVDTP handle
   */
  void ProcessSetConfig(tBTA_AV_HNDL bta_av_handle,
                        const RawAddress& peer_address,
                        const uint8_t* p_codec_info, uint8_t seid,
                        uint8_t num_protect, const uint8_t* p_protect_info,
                        uint8_t t_local_sep, uint8_t avdt_handle);

  /**
   * Process AVDTP Open when the stream connection is opened.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param mtu the MTU of the connection
   */
  void ProcessOpen(tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address,
                   uint16_t mtu);

  /**
   * Process AVDTP Close when the stream connection is closed.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   */
  void ProcessClose(tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address);

  /**
   * Process AVDTP Start when the audio data streaming is started.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param p_codec_info the codec configuration
   * @param p_no_rtp_header on return, set to true if the audio data packets
   * should not contain RTP header
   */
  void ProcessStart(tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address,
                    const uint8_t* p_codec_info, bool* p_no_rtp_header);

  /**
   * Process AVDTP Stop when the audio data streaming is stopped.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   */
  void ProcessStop(tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address);

  /**
   * Get the next encoded audio data packet to send.
   *
   * @param p_codec_info the codec configuration
   * @param p_timestamp on return, set to the timestamp of the data packet
   * @return the next encoded data packet or nullptr if no encoded data to send
   */
  BT_HDR* GetNextSourceDataPacket(const uint8_t* p_codec_info,
                                  uint32_t* p_timestamp);

  /**
   * An audio packet has been dropped.
   * This signal can be used by the encoder to reduce the encoder bit rate
   * setting.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   */
  void DataPacketWasDropped(tBTA_AV_HNDL bta_av_handle,
                            const RawAddress& peer_address);

  /**
   * Process AVDTP Audio Delay when the initial delay report is received by
   * the Source.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param delay the reported delay in 1/10th of a millisecond
   */
  void ProcessAudioDelay(tBTA_AV_HNDL bta_av_handle,
                         const RawAddress& peer_address, uint16_t delay);

  /**
   * Update the MTU of the audio data connection.
   *
   * @param bta_av_handle the BTA AV handle to identify the peer
   * @param peer_address the peer address
   * @param mtu the new MTU of the audio data connection
   */
  void UpdateMtu(tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address,
                 uint16_t mtu);

  /**
   * Set the active peer.
   *
   * @param peer_address the peer address
   * @return true on success, otherwise false
   */
  bool SetActivePeer(const RawAddress& peer_address);

  /**
   * Get the encoder parameters for a peer.
   *
   * @param peer_address the peer address
   * @param p_peer_params on return, set to the peer's encoder parameters
   */
  void GetPeerEncoderParameters(const RawAddress& peer_address,
                                tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params);

  /**
   * Get the Source encoder interface for the current codec.
   *
   * @return the Source encoder interface for the current codec
   */
  const tA2DP_ENCODER_INTERFACE* GetSourceEncoderInterface();

  /**
   * Get the Sink decoder interface for the current codec.
   *
   * @return the Sink decoder interface for the current codec
   */
  const tA2DP_DECODER_INTERFACE* GetSinkDecoderInterface();

  /**
   * Set the codec user configuration.
   *
   * @param peer_address the peer address
   * @param codec_user_config the codec user configuration to set
   * @return true on success, otherwise false
   */
  bool SetCodecUserConfig(const RawAddress& peer_address,
                          const btav_a2dp_codec_config_t& codec_user_config);

  /**
   * Set the codec audio configuration.
   *
   * @param codec_audio_config the codec audio configuration to set
   * @return true on success, otherwise false
   */
  bool SetCodecAudioConfig(const btav_a2dp_codec_config_t& codec_audio_config);

  /**
   * Report the source codec state for a peer
   *
   * @param p_peer the peer to report
   * @return true on success, otherwise false
   */
  bool ReportSourceCodecState(BtaAvCoPeer* p_peer);

  /**
   * Report the sink codec state for a peer
   *
   * @param p_peer the peer to report
   * @return true on success, otherwise false
   */
  bool ReportSinkCodecState(BtaAvCoPeer* p_peer);

  /**
   * Get the content protection flag.
   *
   * @return the content protection flag. It should be one of the following:
   * AVDT_CP_SCMS_COPY_NEVER, AVDT_CP_SCMS_COPY_ONCE, AVDT_CP_SCMS_COPY_FREE
   */
  uint8_t ContentProtectFlag() const { return content_protect_flag_; }

  /**
   * Set the content protection flag.
   *
   * @param cp_flag the content protection flag. It should be one of the
   * following:
   * AVDT_CP_SCMS_COPY_NEVER, AVDT_CP_SCMS_COPY_ONCE, AVDT_CP_SCMS_COPY_FREE
   * NOTE: If Content Protection is not enabled on the system, then
   * the only acceptable vailue is AVDT_CP_SCMS_COPY_FREE.
   */
  void SetContentProtectFlag(uint8_t cp_flag) {
    if (!ContentProtectEnabled() && (cp_flag != AVDT_CP_SCMS_COPY_FREE)) {
      return;
    }
    content_protect_flag_ = cp_flag;
  }

  /**
   * Dump debug-related information.
   *
   * @param fd the file descritor to use for writing the ASCII formatted
   * information
   */
  void DebugDump(int fd);

  /**
   * Find the peer entry for a given peer address.
   *
   * @param peer_address the peer address to use
   * @return the peer entry if found, otherwise nullptr
   */
  BtaAvCoPeer* FindPeer(const RawAddress& peer_address);

  /**
   * Find the peer Sink SEP entry for a given codec index.
   *
   * @param p_peer the peer to use
   * @param codec_index the codec index to use
   * @return the peer Sink SEP for the codec index if found, otherwise nullptr
   */
  BtaAvCoSep* FindPeerSink(BtaAvCoPeer* p_peer,
                           btav_a2dp_codec_index_t codec_index);

  /**
   * Find the peer Source SEP entry for a given codec index.
   *
   * @param p_peer the peer to use
   * @param codec_config the codec index to use
   * @return the peer Source SEP for the codec index if found, otherwise nullptr
   */
  BtaAvCoSep* FindPeerSource(BtaAvCoPeer* p_peer,
                             btav_a2dp_codec_index_t codec_index);

 private:
  /**
   * Reset the state.
   */
  void Reset();

  /**
   * Find the peer entry for a given BTA AV handle.
   *
   * @param bta_av_handle the BTA AV handle to use
   * @return the peer entry if found, otherwise nullptr
   */
  BtaAvCoPeer* FindPeer(tBTA_AV_HNDL bta_av_handle);

  /**
   * Find the peer entry for a given BTA AV handle and update it with the
   * peer address.
   *
   * @param bta_av_handle the BTA AV handle to use
   * @param peer_address the peer address
   * @return the peer entry if found, otherwise nullptr
   */
  BtaAvCoPeer* FindPeerAndUpdate(tBTA_AV_HNDL bta_av_handle,
                                 const RawAddress& peer_address);

  /**
   * Select the Source codec configuration based on peer codec support.
   *
   * Furthermore, the local state for the remaining non-selected codecs is
   * updated to reflect whether the codec is selectable.
   *
   * @param p_peer the peer to use
   * @return a pointer to the corresponding SEP Sink entry on success,
   * otherwise nullptr
   */
  const BtaAvCoSep* SelectSourceCodec(BtaAvCoPeer* p_peer);

  /**
   * Select the Sink codec configuration based on peer codec support.
   *
   * Furthermore, the local state for the remaining non-selected codecs is
   * updated to reflect whether the codec is selectable.
   *
   * @param p_peer the peer to use
   * @return a pointer to the corresponding SEP Source entry on success,
   * otherwise nullptr
   */
  const BtaAvCoSep* SelectSinkCodec(BtaAvCoPeer* p_peer);

  /**
   * Save new codec configuration.
   *
   * @param p_peer the peer to use
   * @param new_codec_config the new codec configuration to use
   * @param num_protect the number of content protection elements
   * @param p_protect_info the content protection info to use
   */
  void SaveNewCodecConfig(BtaAvCoPeer* p_peer, const uint8_t* new_codec_config,
                          uint8_t num_protect, const uint8_t* p_protect_info);

  /**
   * Set the Over-The-Air preferred codec configuration.
   *
   * The OTA prefered codec configuration is ignored if the current
   * codec configuration contains explicit user configuration, or if the
   * codec configuration for the same codec contains explicit user
   * configuration.
   *
   * @param p_peer is the peer device that sent the OTA codec configuration
   * @param p_ota_codec_config contains the received OTA A2DP codec
   * configuration from the remote peer. Note: this is not the peer codec
   * capability, but the codec configuration that the peer would like to use.
   * @param num_protect is the number of content protection methods to use
   * @param p_protect_info contains the content protection information to use.
   * @param p_restart_output if there is a change in the encoder configuration
   * that requires restarting of the A2DP connection, flag |p_restart_output|
   * is set to true.
   * @return true on success, otherwise false
   */
  bool SetCodecOtaConfig(BtaAvCoPeer* p_peer, const uint8_t* p_ota_codec_config,
                         uint8_t num_protect, const uint8_t* p_protect_info,
                         bool* p_restart_output);

  /**
   * Update all selectable Source codecs with the corresponding codec
   * information from a Sink peer.
   *
   * @param p_peer the peer Sink SEP to use
   * @return the number of codecs that have been updated
   */
  size_t UpdateAllSelectableSourceCodecs(BtaAvCoPeer* p_peer);

  /**
   * Update a selectable Source codec with the corresponding codec information
   * from a Sink peer.
   *
   * @param codec_config the codec config info to identify the codec to update
   * @param p_peer the peer Sink SEP to use
   * @return true if the codec is updated, otherwise false
   */
  bool UpdateSelectableSourceCodec(const A2dpCodecConfig& codec_config,
                                   BtaAvCoPeer* p_peer);

  /**
   * Update all selectable Sink codecs with the corresponding codec
   * information from a Source peer.
   *
   * @param p_peer the peer Source SEP to use
   * @return the number of codecs that have been updated
   */
  size_t UpdateAllSelectableSinkCodecs(BtaAvCoPeer* p_peer);

  /**
   * Update a selectable Sink codec with the corresponding codec information
   * from a Source peer.
   *
   * @param codec_config the codec config info to identify the codec to update
   * @param p_peer the peer Source SEP to use
   * @return true if the codec is updated, otherwise false
   */
  bool UpdateSelectableSinkCodec(const A2dpCodecConfig& codec_config,
                                 BtaAvCoPeer* p_peer);

  /**
   * Attempt to select Source codec configuration for a Sink peer.
   *
   * @param codec_config the codec configuration to use
   * @param p_peer the Sink peer to use
   * @return a pointer to the corresponding SEP Sink entry on success,
   * otnerwise nullptr
   */
  const BtaAvCoSep* AttemptSourceCodecSelection(
      const A2dpCodecConfig& codec_config, BtaAvCoPeer* p_peer);

  /**
   * Attempt to select Sink codec configuration for a Source peer.
   *
   * @param codec_config the codec configuration to use
   * @param p_peer the Source peer to use
   * @return a pointer to the corresponding SEP Source entry on success,
   * otnerwise nullptr
   */
  const BtaAvCoSep* AttemptSinkCodecSelection(
      const A2dpCodecConfig& codec_config, BtaAvCoPeer* p_peer);

  /**
   * Check if a peer SEP has content protection enabled.
   *
   * @param p_sep the peer SEP to check
   * @return true if the peer SEP has content protection enabled,
   * otherwise false
   */
  bool AudioSepHasContentProtection(const BtaAvCoSep* p_sep);

  /**
   * Check if a content protection service is SCMS-T.
   *
   * @param p_orotect_info the content protection info to check
   * @return true if the Contention Protection in @param p_protect_info
   * is SCMS-T, otherwise false
   */
  static bool ContentProtectIsScmst(const uint8_t* p_protect_info);

  /**
   * Check if audio protect info contains SCMS-T Content Protection.
   *
   * @param num_protect number of protect schemes
   * @param p_protect_info the protect info to check
   * @return true if @param p_protect_info contains SCMS-T, otherwise false
   */
  static bool AudioProtectHasScmst(uint8_t num_protect,
                                   const uint8_t* p_protect_info);

  bool ContentProtectEnabled() const { return content_protect_enabled_; }

  std::recursive_mutex codec_lock_;  // Protect access to the codec state
  std::vector<btav_a2dp_codec_config_t> codec_priorities_;  // Configured
  BtaAvCoPeer peers_[BTA_AV_NUM_STRS];     // Connected peer information
  BtaAvCoPeer* active_peer_;               // The current active peer
  uint8_t codec_config_[AVDT_CODEC_SIZE];  // Current codec configuration
  const bool content_protect_enabled_;     // True if Content Protect is enabled
  uint8_t content_protect_flag_;           // Content Protect flag
};

// SCMS-T protect info
const uint8_t bta_av_co_cp_scmst[AVDT_CP_INFO_LEN] = {0x02, 0x02, 0x00};

// Control block instance
#if (BTA_AV_CO_CP_SCMS_T == TRUE)
static const bool kContentProtectEnabled = true;
#else
static const bool kContentProtectEnabled = false;
#endif
static BtaAvCo bta_av_co_cb(kContentProtectEnabled);

void BtaAvCoPeer::Init(
    const std::vector<btav_a2dp_codec_config_t>& codec_priorities) {
  Reset(bta_av_handle_);
  // Reset the current config
  codecs_ = new A2dpCodecs(codec_priorities);
  codecs_->init();
  A2DP_InitDefaultCodec(codec_config);
}

void BtaAvCoPeer::Reset(tBTA_AV_HNDL bta_av_handle) {
  addr = RawAddress::kEmpty;
  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(sinks); i++) {
    BtaAvCoSep& sink = sinks[i];
    sink.Reset();
  }
  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(sources); i++) {
    BtaAvCoSep& source = sources[i];
    source.Reset();
  }
  num_sinks = 0;
  num_sources = 0;
  num_seps = 0;
  num_rx_sinks = 0;
  num_rx_sources = 0;
  num_sup_sinks = 0;
  num_sup_sources = 0;
  p_sink = nullptr;
  p_source = nullptr;
  memset(codec_config, 0, sizeof(codec_config));
  acceptor = false;
  reconfig_needed = false;
  opened = false;
  mtu = 0;
  uuid_to_connect = 0;

  bta_av_handle_ = bta_av_handle;
  delete codecs_;
  codecs_ = nullptr;
  content_protect_active_ = false;
}

void BtaAvCo::Init(
    const std::vector<btav_a2dp_codec_config_t>& codec_priorities) {
  APPL_TRACE_DEBUG("%s", __func__);

  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  // Reset the control block
  Reset();
  codec_priorities_ = codec_priorities;

  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(peers_); i++) {
    BtaAvCoPeer* p_peer = &peers_[i];
    p_peer->Init(codec_priorities);
  }
}

void BtaAvCo::Reset() {
  codec_priorities_.clear();
  active_peer_ = nullptr;
  content_protect_flag_ = 0;
  memset(codec_config_, 0, sizeof(codec_config_));

  if (ContentProtectEnabled()) {
    SetContentProtectFlag(AVDT_CP_SCMS_COPY_NEVER);
  } else {
    SetContentProtectFlag(AVDT_CP_SCMS_COPY_FREE);
  }

  // Reset the peers and initialize the handles
  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(peers_); i++) {
    BtaAvCoPeer* p_peer = &peers_[i];
    p_peer->Reset(BTA_AV_CO_AUDIO_INDEX_TO_HANDLE(i));
  }
}

bool BtaAvCo::IsSupportedCodec(btav_a2dp_codec_index_t codec_index) {
  // All peer state is initialized with the same local codec config,
  // hence we check only the first peer.
  A2dpCodecs* codecs = peers_[0].GetCodecs();
  CHECK(codecs != nullptr);
  return codecs->isSupportedCodec(codec_index);
}

A2dpCodecConfig* BtaAvCo::GetActivePeerCurrentCodec() {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  if (active_peer_ == nullptr || active_peer_->GetCodecs() == nullptr) {
    return nullptr;
  }
  return active_peer_->GetCodecs()->getCurrentCodecConfig();
}

A2dpCodecConfig* BtaAvCo::GetPeerCurrentCodec(const RawAddress& peer_address) {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  BtaAvCoPeer* peer = FindPeer(peer_address);
  if (peer == nullptr || peer->GetCodecs() == nullptr) {
    return nullptr;
  }
  return peer->GetCodecs()->getCurrentCodecConfig();
}

BtaAvCoPeer* BtaAvCo::FindPeer(const RawAddress& peer_address) {
  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(peers_); i++) {
    BtaAvCoPeer* p_peer = &peers_[i];
    if (p_peer->addr == peer_address) {
      return p_peer;
    }
  }
  return nullptr;
}

BtaAvCoPeer* BtaAvCo::FindPeer(tBTA_AV_HNDL bta_av_handle) {
  uint8_t index;

  index = BTA_AV_CO_AUDIO_HANDLE_TO_INDEX(bta_av_handle);

  APPL_TRACE_DEBUG("%s: bta_av_handle = 0x%x index = %d", __func__,
                   bta_av_handle, index);

  // Sanity check
  if (index >= BTA_AV_CO_NUM_ELEMENTS(peers_)) {
    APPL_TRACE_ERROR(
        "%s: peer index %d for BTA AV handle 0x%x is out of bounds", __func__,
        index, bta_av_handle);
    return nullptr;
  }

  return &peers_[index];
}

BtaAvCoPeer* BtaAvCo::FindPeerAndUpdate(tBTA_AV_HNDL bta_av_handle,
                                        const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle = 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle);

  BtaAvCoPeer* p_peer = FindPeer(bta_av_handle);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR("%s: peer entry for BTA AV handle 0x%x peer %s not found",
                     __func__, bta_av_handle, peer_address.ToString().c_str());
    return nullptr;
  }

  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle = 0x%x previous address %s",
                   __func__, peer_address.ToString().c_str(), bta_av_handle,
                   p_peer->addr.ToString().c_str());
  p_peer->addr = peer_address;
  return p_peer;
}

uint16_t BtaAvCo::FindPeerUuid(tBTA_AV_HNDL bta_av_handle) {
  BtaAvCoPeer* p_peer = FindPeer(bta_av_handle);
  if (p_peer == nullptr) {
    return 0;
  }
  return p_peer->uuid_to_connect;
}

void BtaAvCo::ProcessDiscoveryResult(tBTA_AV_HNDL bta_av_handle,
                                     const RawAddress& peer_address,
                                     uint8_t num_seps, uint8_t num_sinks,
                                     uint8_t num_sources, uint16_t uuid_local) {
  APPL_TRACE_DEBUG(
      "%s: peer %s bta_av_handle:0x%x num_seps:%d num_sinks:%d num_sources:%d",
      __func__, peer_address.ToString().c_str(), bta_av_handle, num_seps,
      num_sinks, num_sources);

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return;
  }

  /* Sanity check : this should never happen */
  if (p_peer->opened) {
    APPL_TRACE_ERROR("%s: peer %s already opened", __func__,
                     peer_address.ToString().c_str());
  }

  /* Copy the discovery results */
  p_peer->addr = peer_address;
  p_peer->num_sinks = num_sinks;
  p_peer->num_sources = num_sources;
  p_peer->num_seps = num_seps;
  p_peer->num_rx_sinks = 0;
  p_peer->num_rx_sources = 0;
  p_peer->num_sup_sinks = 0;
  p_peer->num_sup_sources = 0;
  if (uuid_local == UUID_SERVCLASS_AUDIO_SINK) {
    p_peer->uuid_to_connect = UUID_SERVCLASS_AUDIO_SOURCE;
  } else if (uuid_local == UUID_SERVCLASS_AUDIO_SOURCE) {
    p_peer->uuid_to_connect = UUID_SERVCLASS_AUDIO_SINK;
  }
}

tA2DP_STATUS BtaAvCo::ProcessSourceGetConfig(
    tBTA_AV_HNDL bta_av_handle, const RawAddress& peer_address,
    uint8_t* p_codec_info, uint8_t* p_sep_info_idx, uint8_t seid,
    uint8_t* p_num_protect, uint8_t* p_protect_info) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle:0x%x codec:%s seid:%d", __func__,
                   peer_address.ToString().c_str(), bta_av_handle,
                   A2DP_CodecName(p_codec_info), seid);
  APPL_TRACE_DEBUG("%s: num_protect:0x%02x protect_info:0x%02x%02x%02x",
                   __func__, *p_num_protect, p_protect_info[0],
                   p_protect_info[1], p_protect_info[2]);
  APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                   A2DP_CodecInfoString(p_codec_info).c_str());

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return A2DP_FAIL;
  }
  APPL_TRACE_DEBUG("%s: peer(o=%d, n_sinks=%d, n_rx_sinks=%d, n_sup_sinks=%d)",
                   __func__, p_peer->opened, p_peer->num_sinks,
                   p_peer->num_rx_sinks, p_peer->num_sup_sinks);

  p_peer->num_rx_sinks++;

  // Check the peer's Sink codec
  if (A2DP_IsPeerSinkCodecValid(p_codec_info)) {
    // If there is room for a new one
    if (p_peer->num_sup_sinks < BTA_AV_CO_NUM_ELEMENTS(p_peer->sinks)) {
      BtaAvCoSep* p_sink = &p_peer->sinks[p_peer->num_sup_sinks++];

      APPL_TRACE_DEBUG("%s: saved caps[%x:%x:%x:%x:%x:%x]", __func__,
                       p_codec_info[1], p_codec_info[2], p_codec_info[3],
                       p_codec_info[4], p_codec_info[5], p_codec_info[6]);

      memcpy(p_sink->codec_caps, p_codec_info, AVDT_CODEC_SIZE);
      p_sink->sep_info_idx = *p_sep_info_idx;
      p_sink->seid = seid;
      p_sink->num_protect = *p_num_protect;
      memcpy(p_sink->protect_info, p_protect_info, AVDT_CP_INFO_LEN);
    } else {
      APPL_TRACE_ERROR("%s: peer %s : no more room for Sink info", __func__,
                       p_peer->addr.ToString().c_str());
    }
  }

  // Check if this is the last Sink get capabilities or all supported codec
  // capabilities are retrieved.
  if ((p_peer->num_rx_sinks != p_peer->num_sinks) &&
      (p_peer->num_sup_sinks != BTA_AV_CO_NUM_ELEMENTS(p_peer->sinks))) {
    return A2DP_FAIL;
  }
  APPL_TRACE_DEBUG("%s: last Sink codec reached for peer %s", __func__,
                   p_peer->addr.ToString().c_str());

  // Select the Source codec
  const BtaAvCoSep* p_sink = nullptr;
  if (p_peer->acceptor) {
    UpdateAllSelectableSourceCodecs(p_peer);
    if (p_peer->p_sink == nullptr) {
      // Update the selected codec
      p_peer->p_sink =
          FindPeerSink(p_peer, A2DP_SourceCodecIndex(p_peer->codec_config));
    }
    p_sink = p_peer->p_sink;
    if (p_sink == nullptr) {
      APPL_TRACE_ERROR("%s: cannot find the selected codec for peer %s",
                       __func__, p_peer->addr.ToString().c_str());
      return A2DP_FAIL;
    }
  } else {
    p_sink = SelectSourceCodec(p_peer);
    if (p_sink == nullptr) {
      APPL_TRACE_ERROR("%s: cannot set up codec for peer %s", __func__,
                       p_peer->addr.ToString().c_str());
      return A2DP_FAIL;
    }
  }

  // By default, no content protection
  *p_num_protect = 0;
  if (ContentProtectEnabled() && p_peer->ContentProtectActive()) {
    *p_num_protect = AVDT_CP_INFO_LEN;
    memcpy(p_protect_info, bta_av_co_cp_scmst, AVDT_CP_INFO_LEN);
  }

  // If acceptor -> reconfig otherwise reply for configuration
  *p_sep_info_idx = p_sink->sep_info_idx;
  APPL_TRACE_EVENT("%s: peer %s acceptor:%s reconfig_needed:%s", __func__,
                   p_peer->addr.ToString().c_str(),
                   (p_peer->acceptor) ? "true" : "false",
                   (p_peer->reconfig_needed) ? "true" : "false");
  if (p_peer->acceptor) {
    if (p_peer->reconfig_needed) {
      APPL_TRACE_DEBUG("%s: call BTA_AvReconfig(0x%x) for peer %s", __func__,
                       bta_av_handle, p_peer->addr.ToString().c_str());
      BTA_AvReconfig(bta_av_handle, true, p_sink->sep_info_idx,
                     p_peer->codec_config, *p_num_protect, bta_av_co_cp_scmst);
    }
  } else {
    memcpy(p_codec_info, p_peer->codec_config, AVDT_CODEC_SIZE);
  }

  return A2DP_SUCCESS;
}

tA2DP_STATUS BtaAvCo::ProcessSinkGetConfig(tBTA_AV_HNDL bta_av_handle,
                                           const RawAddress& peer_address,
                                           uint8_t* p_codec_info,
                                           uint8_t* p_sep_info_idx,
                                           uint8_t seid, uint8_t* p_num_protect,
                                           uint8_t* p_protect_info) {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle:0x%x codec:%s seid:%d", __func__,
                   peer_address.ToString().c_str(), bta_av_handle,
                   A2DP_CodecName(p_codec_info), seid);
  APPL_TRACE_DEBUG("%s: num_protect:0x%02x protect_info:0x%02x%02x%02x",
                   __func__, *p_num_protect, p_protect_info[0],
                   p_protect_info[1], p_protect_info[2]);
  APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                   A2DP_CodecInfoString(p_codec_info).c_str());

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return A2DP_FAIL;
  }
  APPL_TRACE_DEBUG(
      "%s: peer %s found (o=%d, n_sources=%d, n_rx_sources=%d, "
      "n_sup_sources=%d)",
      __func__, p_peer->addr.ToString().c_str(), p_peer->opened,
      p_peer->num_sources, p_peer->num_rx_sources, p_peer->num_sup_sources);

  p_peer->num_rx_sources++;

  // Check the peer's Source codec
  if (A2DP_IsPeerSourceCodecValid(p_codec_info)) {
    // If there is room for a new one
    if (p_peer->num_sup_sources < BTA_AV_CO_NUM_ELEMENTS(p_peer->sources)) {
      BtaAvCoSep* p_source = &p_peer->sources[p_peer->num_sup_sources++];

      APPL_TRACE_DEBUG("%s: saved caps[%x:%x:%x:%x:%x:%x]", __func__,
                       p_codec_info[1], p_codec_info[2], p_codec_info[3],
                       p_codec_info[4], p_codec_info[5], p_codec_info[6]);

      memcpy(p_source->codec_caps, p_codec_info, AVDT_CODEC_SIZE);
      p_source->sep_info_idx = *p_sep_info_idx;
      p_source->seid = seid;
      p_source->num_protect = *p_num_protect;
      memcpy(p_source->protect_info, p_protect_info, AVDT_CP_INFO_LEN);
    } else {
      APPL_TRACE_ERROR("%s: peer %s : no more room for Source info", __func__,
                       p_peer->addr.ToString().c_str());
    }
  }

  // Check if this is the last Source get capabilities or all supported codec
  // capabilities are retrieved.
  if ((p_peer->num_rx_sources != p_peer->num_sources) &&
      (p_peer->num_sup_sources != BTA_AV_CO_NUM_ELEMENTS(p_peer->sources))) {
    return A2DP_FAIL;
  }
  APPL_TRACE_DEBUG("%s: last Source codec reached for peer %s", __func__,
                   p_peer->addr.ToString().c_str());

  // Select the Sink codec
  const BtaAvCoSep* p_source = nullptr;
  if (p_peer->acceptor) {
    UpdateAllSelectableSinkCodecs(p_peer);
    if (p_peer->p_source == nullptr) {
      // Update the selected codec
      p_peer->p_source =
          FindPeerSource(p_peer, A2DP_SinkCodecIndex(p_peer->codec_config));
    }
    p_source = p_peer->p_source;
    if (p_source == nullptr) {
      APPL_TRACE_ERROR("%s: cannot find the selected codec for peer %s",
                       __func__, p_peer->addr.ToString().c_str());
      return A2DP_FAIL;
    }
  } else {
    p_source = SelectSinkCodec(p_peer);
    if (p_source == nullptr) {
      APPL_TRACE_ERROR("%s: cannot set up codec for the peer %s", __func__,
                       p_peer->addr.ToString().c_str());
      return A2DP_FAIL;
    }
  }

  // By default, no content protection
  *p_num_protect = 0;
  if (ContentProtectEnabled() && p_peer->ContentProtectActive()) {
    *p_num_protect = AVDT_CP_INFO_LEN;
    memcpy(p_protect_info, bta_av_co_cp_scmst, AVDT_CP_INFO_LEN);
  }

  // If acceptor -> reconfig otherwise reply for configuration
  *p_sep_info_idx = p_source->sep_info_idx;
  APPL_TRACE_EVENT("%s: peer %s acceptor:%s reconfig_needed:%s", __func__,
                   p_peer->addr.ToString().c_str(),
                   (p_peer->acceptor) ? "true" : "false",
                   (p_peer->reconfig_needed) ? "true" : "false");
  if (p_peer->acceptor) {
    if (p_peer->reconfig_needed) {
      APPL_TRACE_DEBUG("%s: call BTA_AvReconfig(0x%x) for peer %s", __func__,
                       bta_av_handle, p_peer->addr.ToString().c_str());
      BTA_AvReconfig(bta_av_handle, true, p_source->sep_info_idx,
                     p_peer->codec_config, *p_num_protect, bta_av_co_cp_scmst);
    }
  } else {
    memcpy(p_codec_info, p_peer->codec_config, AVDT_CODEC_SIZE);
  }

  return A2DP_SUCCESS;
}

void BtaAvCo::ProcessSetConfig(tBTA_AV_HNDL bta_av_handle,
                               UNUSED_ATTR const RawAddress& peer_address,
                               const uint8_t* p_codec_info,
                               UNUSED_ATTR uint8_t seid, uint8_t num_protect,
                               const uint8_t* p_protect_info,
                               uint8_t t_local_sep, uint8_t avdt_handle) {
  tA2DP_STATUS status = A2DP_SUCCESS;
  uint8_t category = A2DP_SUCCESS;
  bool reconfig_needed = false;

  APPL_TRACE_DEBUG(
      "%s: bta_av_handle=0x%x peer_address=%s seid=%d "
      "num_protect=%d t_local_sep=%d avdt_handle=%d",
      __func__, bta_av_handle, peer_address.ToString().c_str(), seid,
      num_protect, t_local_sep, avdt_handle);
  APPL_TRACE_DEBUG("%s: p_codec_info[%x:%x:%x:%x:%x:%x]", __func__,
                   p_codec_info[1], p_codec_info[2], p_codec_info[3],
                   p_codec_info[4], p_codec_info[5], p_codec_info[6]);
  APPL_TRACE_DEBUG("%s: num_protect:0x%02x protect_info:0x%02x%02x%02x",
                   __func__, num_protect, p_protect_info[0], p_protect_info[1],
                   p_protect_info[2]);
  APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                   A2DP_CodecInfoString(p_codec_info).c_str());

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    // Call call-in rejecting the configuration
    bta_av_ci_setconfig(bta_av_handle, A2DP_BUSY, AVDT_ASC_CODEC, 0, nullptr,
                        false, avdt_handle);
    return;
  }

  APPL_TRACE_DEBUG(
      "%s: peer %s found (o=%d, n_sinks=%d, n_rx_sinks=%d, "
      "n_sup_sinks=%d)",
      __func__, p_peer->addr.ToString().c_str(), p_peer->opened,
      p_peer->num_sinks, p_peer->num_rx_sinks, p_peer->num_sup_sinks);

  // Sanity check: should not be opened at this point
  if (p_peer->opened) {
    APPL_TRACE_ERROR("%s: peer %s already in use", __func__,
                     p_peer->addr.ToString().c_str());
  }

  if (num_protect != 0) {
    if (ContentProtectEnabled()) {
      if ((num_protect != 1) ||
          !BtaAvCo::ContentProtectIsScmst(p_protect_info)) {
        APPL_TRACE_ERROR("%s: wrong CP configuration for peer %s", __func__,
                         p_peer->addr.ToString().c_str());
        status = A2DP_BAD_CP_TYPE;
        category = AVDT_ASC_PROTECT;
      }
    } else {
      // Do not support content protection for the time being
      APPL_TRACE_ERROR("%s: wrong CP configuration for peer %s", __func__,
                       p_peer->addr.ToString().c_str());
      status = A2DP_BAD_CP_TYPE;
      category = AVDT_ASC_PROTECT;
    }
  }

  if (status == A2DP_SUCCESS) {
    bool codec_config_supported = false;

    if (t_local_sep == AVDT_TSEP_SNK) {
      APPL_TRACE_DEBUG("%s: peer %s is A2DP Source", __func__,
                       p_peer->addr.ToString().c_str());
      codec_config_supported = A2DP_IsSinkCodecSupported(p_codec_info);
      if (codec_config_supported) {
        // If Peer is Source, and our config subset matches with what is
        // requested by peer, then just accept what peer wants.
        SaveNewCodecConfig(p_peer, p_codec_info, num_protect, p_protect_info);
      }
    }
    if (t_local_sep == AVDT_TSEP_SRC) {
      APPL_TRACE_DEBUG("%s: peer %s is A2DP SINK", __func__,
                       p_peer->addr.ToString().c_str());
      // Ignore the restart_output flag: accepting the remote device's
      // codec selection should not trigger codec reconfiguration.
      bool dummy_restart_output = false;
      if ((p_peer->GetCodecs() == nullptr) ||
          !SetCodecOtaConfig(p_peer, p_codec_info, num_protect, p_protect_info,
                             &dummy_restart_output)) {
        APPL_TRACE_ERROR("%s: cannot set source codec %s for peer %s", __func__,
                         A2DP_CodecName(p_codec_info),
                         p_peer->addr.ToString().c_str());
      } else {
        codec_config_supported = true;
        // Check if reconfiguration is needed
        if (((num_protect == 1) && !p_peer->ContentProtectActive())) {
          reconfig_needed = true;
        }
      }
    }

    // Check if codec configuration is supported
    if (!codec_config_supported) {
      category = AVDT_ASC_CODEC;
      status = A2DP_WRONG_CODEC;
    }
  }

  if (status != A2DP_SUCCESS) {
    APPL_TRACE_DEBUG("%s: peer %s reject s=%d c=%d", __func__,
                     p_peer->addr.ToString().c_str(), status, category);
    // Call call-in rejecting the configuration
    bta_av_ci_setconfig(bta_av_handle, status, category, 0, nullptr, false,
                        avdt_handle);
    return;
  }

  // Mark that this is an acceptor peer
  p_peer->acceptor = true;
  p_peer->reconfig_needed = reconfig_needed;
  APPL_TRACE_DEBUG("%s: peer %s accept reconf=%d", __func__,
                   p_peer->addr.ToString().c_str(), reconfig_needed);
  // Call call-in accepting the configuration
  bta_av_ci_setconfig(bta_av_handle, A2DP_SUCCESS, A2DP_SUCCESS, 0, nullptr,
                      reconfig_needed, avdt_handle);
}

void BtaAvCo::ProcessOpen(tBTA_AV_HNDL bta_av_handle,
                          const RawAddress& peer_address, uint16_t mtu) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x mtu:%d", __func__,
                   peer_address.ToString().c_str(), bta_av_handle, mtu);

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return;
  }
  p_peer->opened = true;
  p_peer->mtu = mtu;

  // The first connected peer becomes the active peer
  if (active_peer_ == nullptr) {
    active_peer_ = p_peer;
  }
}

void BtaAvCo::ProcessClose(tBTA_AV_HNDL bta_av_handle,
                           const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle);
  btif_av_reset_audio_delay();

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return;
  }
  // Reset the active peer
  if (active_peer_ == p_peer) {
    active_peer_ = nullptr;
  }
  // Mark the peer closed and clean the peer info
  p_peer->Init(codec_priorities_);
}

void BtaAvCo::ProcessStart(tBTA_AV_HNDL bta_av_handle,
                           const RawAddress& peer_address,
                           const uint8_t* p_codec_info, bool* p_no_rtp_header) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle);

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return;
  }

  bool add_rtp_header =
      A2DP_UsesRtpHeader(p_peer->ContentProtectActive(), p_codec_info);

  APPL_TRACE_DEBUG("%s: bta_av_handle: 0x%x add_rtp_header: %s", __func__,
                   bta_av_handle, add_rtp_header ? "true" : "false");
  *p_no_rtp_header = !add_rtp_header;
}

void BtaAvCo::ProcessStop(tBTA_AV_HNDL bta_av_handle,
                          const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle);
  // Nothing to do
}

BT_HDR* BtaAvCo::GetNextSourceDataPacket(const uint8_t* p_codec_info,
                                         uint32_t* p_timestamp) {
  BT_HDR* p_buf;

  APPL_TRACE_DEBUG("%s: codec: %s", __func__, A2DP_CodecName(p_codec_info));

  p_buf = btif_a2dp_source_audio_readbuf();
  if (p_buf == nullptr) return nullptr;

  /*
   * Retrieve the timestamp information from the media packet,
   * and set up the packet header.
   *
   * In media packet, the following information is available:
   * p_buf->layer_specific : number of audio frames in the packet
   * p_buf->word[0] : timestamp
   */
  if (!A2DP_GetPacketTimestamp(p_codec_info, (const uint8_t*)(p_buf + 1),
                               p_timestamp) ||
      !A2DP_BuildCodecHeader(p_codec_info, p_buf, p_buf->layer_specific)) {
    APPL_TRACE_ERROR("%s: unsupported codec type (%d)", __func__,
                     A2DP_GetCodecType(p_codec_info));
  }

  if (ContentProtectEnabled() && (active_peer_ != nullptr) &&
      active_peer_->ContentProtectActive()) {
    p_buf->len++;
    p_buf->offset--;
    uint8_t* p = (uint8_t*)(p_buf + 1) + p_buf->offset;
    *p = ContentProtectFlag();
  }

  return p_buf;
}

void BtaAvCo::DataPacketWasDropped(tBTA_AV_HNDL bta_av_handle,
                                   const RawAddress& peer_address) {
  APPL_TRACE_ERROR("%s: peer %s dropped audio packet on handle 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle);
}

void BtaAvCo::ProcessAudioDelay(tBTA_AV_HNDL bta_av_handle,
                                const RawAddress& peer_address,
                                uint16_t delay) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x delay:0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle, delay);

  btif_av_set_audio_delay(delay);
}

void BtaAvCo::UpdateMtu(tBTA_AV_HNDL bta_av_handle,
                        const RawAddress& peer_address, uint16_t mtu) {
  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle: 0x%x mtu: %d", __func__,
                   peer_address.ToString().c_str(), bta_av_handle, mtu);

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeerAndUpdate(bta_av_handle, peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR(
        "%s: could not find peer entry for bta_av_handle 0x%x peer %s",
        __func__, bta_av_handle, peer_address.ToString().c_str());
    return;
  }
  p_peer->mtu = mtu;
}

bool BtaAvCo::SetActivePeer(const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer_address=%s", __func__,
                   peer_address.ToString().c_str());

  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  if (peer_address.IsEmpty()) {
    // Reset the active peer;
    active_peer_ = nullptr;
    memset(codec_config_, 0, sizeof(codec_config_));
    return true;
  }

  // Find the peer
  BtaAvCoPeer* p_peer = FindPeer(peer_address);
  if (p_peer == nullptr) {
    return false;
  }

  active_peer_ = p_peer;
  memcpy(codec_config_, active_peer_->codec_config, AVDT_CODEC_SIZE);
  APPL_TRACE_DEBUG("%s: codec = %s", __func__,
                   A2DP_CodecInfoString(codec_config_).c_str());
  ReportSourceCodecState(active_peer_);
  return true;
}

void BtaAvCo::GetPeerEncoderParameters(
    const RawAddress& peer_address,
    tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params) {
  uint16_t min_mtu = 0xFFFF;
  CHECK(p_peer_params != nullptr) << "Peer address " << peer_address;

  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  // Compute the MTU
  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(peers_); i++) {
    const BtaAvCoPeer* p_peer = &peers_[i];
    if (!p_peer->opened) continue;
    if (p_peer->addr != peer_address) continue;
    if (p_peer->mtu < min_mtu) min_mtu = p_peer->mtu;
  }
  p_peer_params->peer_mtu = min_mtu;
  p_peer_params->is_peer_edr = btif_av_is_peer_edr(peer_address);
  p_peer_params->peer_supports_3mbps =
      btif_av_peer_supports_3mbps(peer_address);
  APPL_TRACE_DEBUG(
      "%s: peer_address=%s peer_mtu=%d is_peer_edr=%s peer_supports_3mbps=%s",
      __func__, peer_address.ToString().c_str(), p_peer_params->peer_mtu,
      logbool(p_peer_params->is_peer_edr).c_str(),
      logbool(p_peer_params->peer_supports_3mbps).c_str());
}

const tA2DP_ENCODER_INTERFACE* BtaAvCo::GetSourceEncoderInterface() {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  return A2DP_GetEncoderInterface(codec_config_);
}

const tA2DP_DECODER_INTERFACE* BtaAvCo::GetSinkDecoderInterface() {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  return A2DP_GetDecoderInterface(codec_config_);
}

bool BtaAvCo::SetCodecUserConfig(
    const RawAddress& peer_address,
    const btav_a2dp_codec_config_t& codec_user_config) {
  uint8_t result_codec_config[AVDT_CODEC_SIZE];
  const BtaAvCoSep* p_sink = nullptr;
  bool restart_input = false;
  bool restart_output = false;
  bool config_updated = false;
  bool success = true;

  APPL_TRACE_DEBUG("%s: peer_address=%s codec_user_config=%s", __func__,
                   peer_address.ToString().c_str(),
                   codec_user_config.ToString().c_str());

  BtaAvCoPeer* p_peer = FindPeer(peer_address);
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR("%s: cannot find peer %s to configure", __func__,
                     peer_address.ToString().c_str());
    success = false;
    goto done;
  }

  // Find the peer SEP codec to use
  if (codec_user_config.codec_type < BTAV_A2DP_CODEC_INDEX_MAX) {
    p_sink = FindPeerSink(p_peer, codec_user_config.codec_type);
  } else {
    // Use the current sink codec
    p_sink = p_peer->p_sink;
  }
  if (p_sink == nullptr) {
    APPL_TRACE_ERROR(
        "%s: peer %s : cannot find peer SEP to configure for codec type %d",
        __func__, p_peer->addr.ToString().c_str(),
        codec_user_config.codec_type);
    success = false;
    goto done;
  }

  tA2DP_ENCODER_INIT_PEER_PARAMS peer_params;
  GetPeerEncoderParameters(p_peer->addr, &peer_params);
  if (!p_peer->GetCodecs()->setCodecUserConfig(
          codec_user_config, &peer_params, p_sink->codec_caps,
          result_codec_config, &restart_input, &restart_output,
          &config_updated)) {
    success = false;
    goto done;
  }

  if (restart_output) {
    uint8_t num_protect = 0;
    if (ContentProtectEnabled() && p_peer->ContentProtectActive()) {
      num_protect = AVDT_CP_INFO_LEN;
    }

    p_sink = SelectSourceCodec(p_peer);
    if (p_sink == nullptr) {
      APPL_TRACE_ERROR("%s: peer %s : cannot set up codec for the peer SINK",
                       __func__, p_peer->addr.ToString().c_str());
      success = false;
      goto done;
    }
    // Don't call BTA_AvReconfig() prior to retrieving all peer's capabilities
    if ((p_peer->num_rx_sinks != p_peer->num_sinks) &&
        (p_peer->num_sup_sinks != BTA_AV_CO_NUM_ELEMENTS(p_peer->sinks))) {
      APPL_TRACE_WARNING(
          "%s: peer %s : not all peer's capabilities have been retrieved",
          __func__, p_peer->addr.ToString().c_str());
      success = false;
      goto done;
    }

    p_peer->acceptor = false;
    APPL_TRACE_DEBUG("%s: call BTA_AvReconfig(0x%x)", __func__,
                     p_peer->BtaAvHandle());
    BTA_AvReconfig(p_peer->BtaAvHandle(), true, p_sink->sep_info_idx,
                   p_peer->codec_config, num_protect, bta_av_co_cp_scmst);
  }

done:
  // NOTE: We unconditionally send the upcall even if there is no change
  // or the user config failed. Thus, the caller would always know whether the
  // request succeeded or failed.
  // NOTE: Currently, the input is restarted by sending an upcall
  // and informing the Media Framework about the change.
  if (p_peer != nullptr) {
    return ReportSourceCodecState(p_peer);
  }

  return success;
}

bool BtaAvCo::SetCodecAudioConfig(
    const btav_a2dp_codec_config_t& codec_audio_config) {
  uint8_t result_codec_config[AVDT_CODEC_SIZE];
  bool restart_output = false;
  bool config_updated = false;

  APPL_TRACE_DEBUG("%s: codec_audio_config: %s", __func__,
                   codec_audio_config.ToString().c_str());

  // Find the peer that is currently open
  BtaAvCoPeer* p_peer = active_peer_;
  if (p_peer == nullptr) {
    APPL_TRACE_ERROR("%s: no active peer to configure", __func__);
    return false;
  }

  // Use the current sink codec
  const BtaAvCoSep* p_sink = p_peer->p_sink;
  if (p_sink == nullptr) {
    APPL_TRACE_ERROR("%s: peer %s : cannot find peer SEP to configure",
                     __func__, p_peer->addr.ToString().c_str());
    return false;
  }

  tA2DP_ENCODER_INIT_PEER_PARAMS peer_params;
  GetPeerEncoderParameters(p_peer->addr, &peer_params);
  if (!p_peer->GetCodecs()->setCodecAudioConfig(
          codec_audio_config, &peer_params, p_sink->codec_caps,
          result_codec_config, &restart_output, &config_updated)) {
    return false;
  }

  if (restart_output) {
    uint8_t num_protect = 0;
    if (ContentProtectEnabled() && p_peer->ContentProtectActive()) {
      num_protect = AVDT_CP_INFO_LEN;
    }

    SaveNewCodecConfig(p_peer, result_codec_config, p_sink->num_protect,
                       p_sink->protect_info);

    // Don't call BTA_AvReconfig() prior to retrieving all peer's capabilities
    if ((p_peer->num_rx_sinks != p_peer->num_sinks) &&
        (p_peer->num_sup_sinks != BTA_AV_CO_NUM_ELEMENTS(p_peer->sinks))) {
      APPL_TRACE_WARNING(
          "%s: peer %s : not all peer's capabilities have been retrieved",
          __func__, p_peer->addr.ToString().c_str());
    } else {
      p_peer->acceptor = false;
      APPL_TRACE_DEBUG("%s: call BTA_AvReconfig(0x%x)", __func__,
                       p_peer->BtaAvHandle());
      BTA_AvReconfig(p_peer->BtaAvHandle(), true, p_sink->sep_info_idx,
                     p_peer->codec_config, num_protect, bta_av_co_cp_scmst);
    }
  }

  if (config_updated) {
    // NOTE: Currently, the input is restarted by sending an upcall
    // and informing the Media Framework about the change.
    return ReportSourceCodecState(p_peer);
  }

  return true;
}

bool BtaAvCo::ReportSourceCodecState(BtaAvCoPeer* p_peer) {
  btav_a2dp_codec_config_t codec_config;
  std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities;
  std::vector<btav_a2dp_codec_config_t> codecs_selectable_capabilities;

  APPL_TRACE_DEBUG("%s: peer_address=%s", __func__,
                   p_peer->addr.ToString().c_str());
  A2dpCodecs* codecs = p_peer->GetCodecs();
  CHECK(codecs != nullptr);
  if (!codecs->getCodecConfigAndCapabilities(&codec_config,
                                             &codecs_local_capabilities,
                                             &codecs_selectable_capabilities)) {
    APPL_TRACE_WARNING(
        "%s: Peer %s : error reporting audio source codec state: "
        "cannot get codec config and capabilities",
        __func__, p_peer->addr.ToString().c_str());
    return false;
  }
  APPL_TRACE_DEBUG("%s: peer %s codec_config=%s", __func__,
                   p_peer->addr.ToString().c_str(),
                   codec_config.ToString().c_str());
  btif_av_report_source_codec_state(p_peer->addr, codec_config,
                                    codecs_local_capabilities,
                                    codecs_selectable_capabilities);
  return true;
}

bool BtaAvCo::ReportSinkCodecState(BtaAvCoPeer* p_peer) {
  APPL_TRACE_DEBUG("%s: peer_address=%s", __func__,
                   p_peer->addr.ToString().c_str());
  // Nothing to do (for now)
  return true;
}

void BtaAvCo::DebugDump(int fd) {
  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  //
  // Active peer codec-specific stats
  //
  if (active_peer_ != nullptr) {
    A2dpCodecs* a2dp_codecs = active_peer_->GetCodecs();
    if (a2dp_codecs != nullptr) {
      a2dp_codecs->debug_codec_dump(fd);
    }
  }

  if (appl_trace_level < BT_TRACE_LEVEL_DEBUG) return;

  dprintf(fd, "\nA2DP Peers State:\n");
  dprintf(fd, "  Active peer: %s\n",
          (active_peer_ != nullptr) ? active_peer_->addr.ToString().c_str()
                                    : "null");

  for (size_t i = 0; i < BTA_AV_CO_NUM_ELEMENTS(peers_); i++) {
    const BtaAvCoPeer& peer = peers_[i];
    if (peer.addr.IsEmpty()) {
      continue;
    }
    dprintf(fd, "  Peer: %s\n", peer.addr.ToString().c_str());
    dprintf(fd, "    Number of sinks: %u\n", peer.num_sinks);
    dprintf(fd, "    Number of sources: %u\n", peer.num_sources);
    dprintf(fd, "    Number of SEPs: %u\n", peer.num_seps);
    dprintf(fd, "    Number of received sinks: %u\n", peer.num_rx_sinks);
    dprintf(fd, "    Number of received sources: %u\n", peer.num_rx_sources);
    dprintf(fd, "    Number of supported sinks: %u\n", peer.num_sup_sinks);
    dprintf(fd, "    Number of supported sources: %u\n", peer.num_sup_sources);
    dprintf(fd, "    Acceptor: %s\n", (peer.acceptor) ? "true" : "false");
    dprintf(fd, "    Reconfig needed: %s\n",
            (peer.reconfig_needed) ? "true" : "false");
    dprintf(fd, "    Opened: %s\n", (peer.opened) ? "true" : "false");
    dprintf(fd, "    MTU: %u\n", peer.mtu);
    dprintf(fd, "    UUID to connect: 0x%x\n", peer.uuid_to_connect);
    dprintf(fd, "    BTA AV handle: %u\n", peer.BtaAvHandle());
  }
}

bool BtaAvCo::ContentProtectIsScmst(const uint8_t* p_protect_info) {
  APPL_TRACE_DEBUG("%s", __func__);

  if (*p_protect_info >= AVDT_CP_LOSC) {
    uint16_t cp_id;
    p_protect_info++;
    STREAM_TO_UINT16(cp_id, p_protect_info);
    if (cp_id == AVDT_CP_SCMS_T_ID) {
      APPL_TRACE_DEBUG("%s: SCMS-T found", __func__);
      return true;
    }
  }
  return false;
}

bool BtaAvCo::AudioProtectHasScmst(uint8_t num_protect,
                                   const uint8_t* p_protect_info) {
  APPL_TRACE_DEBUG("%s", __func__);
  while (num_protect--) {
    if (BtaAvCo::ContentProtectIsScmst(p_protect_info)) return true;
    // Move to the next Content Protect schema
    p_protect_info += *p_protect_info + 1;
  }
  APPL_TRACE_DEBUG("%s: SCMS-T not found", __func__);
  return false;
}

bool BtaAvCo::AudioSepHasContentProtection(const BtaAvCoSep* p_sep) {
  APPL_TRACE_DEBUG("%s", __func__);

  // Check if content protection is enabled for this stream
  if (ContentProtectFlag() != AVDT_CP_SCMS_COPY_FREE) {
    return BtaAvCo::AudioProtectHasScmst(p_sep->num_protect,
                                         p_sep->protect_info);
  }

  APPL_TRACE_DEBUG("%s: not required", __func__);
  return true;
}

const BtaAvCoSep* BtaAvCo::SelectSourceCodec(BtaAvCoPeer* p_peer) {
  const BtaAvCoSep* p_sink = nullptr;

  // Update all selectable codecs.
  // This is needed to update the selectable parameters for each codec.
  // NOTE: The selectable codec info is used only for informational purpose.
  UpdateAllSelectableSourceCodecs(p_peer);

  // Select the codec
  for (const auto& iter : p_peer->GetCodecs()->orderedSourceCodecs()) {
    APPL_TRACE_DEBUG("%s: trying codec %s", __func__, iter->name().c_str());
    p_sink = AttemptSourceCodecSelection(*iter, p_peer);
    if (p_sink != nullptr) {
      APPL_TRACE_DEBUG("%s: selected codec %s", __func__, iter->name().c_str());
      break;
    }
    APPL_TRACE_DEBUG("%s: cannot use codec %s", __func__, iter->name().c_str());
  }

  // NOTE: Unconditionally dispatch the event to make sure a callback with
  // the most recent codec info is generated.
  ReportSourceCodecState(p_peer);

  return p_sink;
}

const BtaAvCoSep* BtaAvCo::SelectSinkCodec(BtaAvCoPeer* p_peer) {
  const BtaAvCoSep* p_source = nullptr;

  // Update all selectable codecs.
  // This is needed to update the selectable parameters for each codec.
  // NOTE: The selectable codec info is used only for informational purpose.
  UpdateAllSelectableSinkCodecs(p_peer);

  // Select the codec
  for (const auto& iter : p_peer->GetCodecs()->orderedSinkCodecs()) {
    APPL_TRACE_DEBUG("%s: trying codec %s", __func__, iter->name().c_str());
    p_source = AttemptSinkCodecSelection(*iter, p_peer);
    if (p_source != nullptr) {
      APPL_TRACE_DEBUG("%s: selected codec %s", __func__, iter->name().c_str());
      break;
    }
    APPL_TRACE_DEBUG("%s: cannot use codec %s", __func__, iter->name().c_str());
  }

  // NOTE: Unconditionally dispatch the event to make sure a callback with
  // the most recent codec info is generated.
  ReportSinkCodecState(p_peer);

  return p_source;
}

BtaAvCoSep* BtaAvCo::FindPeerSink(BtaAvCoPeer* p_peer,
                                  btav_a2dp_codec_index_t codec_index) {
  if (codec_index == BTAV_A2DP_CODEC_INDEX_MAX) {
    APPL_TRACE_WARNING("%s: invalid codec index for peer %s", __func__,
                       p_peer->addr.ToString().c_str());
    return nullptr;
  }

  // Find the peer Sink for the codec
  for (size_t index = 0; index < p_peer->num_sup_sinks; index++) {
    BtaAvCoSep* p_sink = &p_peer->sinks[index];
    btav_a2dp_codec_index_t peer_codec_index =
        A2DP_SourceCodecIndex(p_sink->codec_caps);
    if (peer_codec_index != codec_index) {
      continue;
    }
    if (!AudioSepHasContentProtection(p_sink)) {
      APPL_TRACE_DEBUG(
          "%s: peer Sink for codec %s does not support "
          "Content Protection",
          __func__, A2DP_CodecIndexStr(codec_index));
      continue;
    }
    return p_sink;
  }
  return nullptr;
}

BtaAvCoSep* BtaAvCo::FindPeerSource(BtaAvCoPeer* p_peer,
                                    btav_a2dp_codec_index_t codec_index) {
  if (codec_index == BTAV_A2DP_CODEC_INDEX_MAX) {
    APPL_TRACE_WARNING("%s: invalid codec index for peer %s", __func__,
                       p_peer->addr.ToString().c_str());
    return nullptr;
  }

  // Find the peer Source for the codec
  for (size_t index = 0; index < p_peer->num_sup_sources; index++) {
    BtaAvCoSep* p_source = &p_peer->sources[index];
    btav_a2dp_codec_index_t peer_codec_index =
        A2DP_SinkCodecIndex(p_source->codec_caps);
    if (peer_codec_index != codec_index) {
      continue;
    }
    if (!AudioSepHasContentProtection(p_source)) {
      APPL_TRACE_DEBUG(
          "%s: peer Source for codec %s does not support "
          "Content Protection",
          __func__, A2DP_CodecIndexStr(codec_index));
      continue;
    }
    return p_source;
  }
  return nullptr;
}

const BtaAvCoSep* BtaAvCo::AttemptSourceCodecSelection(
    const A2dpCodecConfig& codec_config, BtaAvCoPeer* p_peer) {
  uint8_t new_codec_config[AVDT_CODEC_SIZE];

  APPL_TRACE_DEBUG("%s", __func__);

  // Find the peer Sink for the codec
  BtaAvCoSep* p_sink = FindPeerSink(p_peer, codec_config.codecIndex());
  if (p_sink == nullptr) {
    APPL_TRACE_DEBUG("%s: peer Sink for codec %s not found", __func__,
                     codec_config.name().c_str());
    return nullptr;
  }
  if (!p_peer->GetCodecs()->setCodecConfig(
          p_sink->codec_caps, true /* is_capability */, new_codec_config,
          true /* select_current_codec */)) {
    APPL_TRACE_DEBUG("%s: cannot set source codec %s", __func__,
                     codec_config.name().c_str());
    return nullptr;
  }
  p_peer->p_sink = p_sink;

  SaveNewCodecConfig(p_peer, new_codec_config, p_sink->num_protect,
                     p_sink->protect_info);

  return p_sink;
}

const BtaAvCoSep* BtaAvCo::AttemptSinkCodecSelection(
    const A2dpCodecConfig& codec_config, BtaAvCoPeer* p_peer) {
  uint8_t new_codec_config[AVDT_CODEC_SIZE];

  APPL_TRACE_DEBUG("%s", __func__);

  // Find the peer Source for the codec
  BtaAvCoSep* p_source = FindPeerSource(p_peer, codec_config.codecIndex());
  if (p_source == nullptr) {
    APPL_TRACE_DEBUG("%s: peer Source for codec %s not found", __func__,
                     codec_config.name().c_str());
    return nullptr;
  }
  if (!p_peer->GetCodecs()->setSinkCodecConfig(
          p_source->codec_caps, true /* is_capability */, new_codec_config,
          true /* select_current_codec */)) {
    APPL_TRACE_DEBUG("%s: cannot set sink codec %s", __func__,
                     codec_config.name().c_str());
    return nullptr;
  }
  p_peer->p_source = p_source;

  SaveNewCodecConfig(p_peer, new_codec_config, p_source->num_protect,
                     p_source->protect_info);

  return p_source;
}

size_t BtaAvCo::UpdateAllSelectableSourceCodecs(BtaAvCoPeer* p_peer) {
  APPL_TRACE_DEBUG("%s: peer %s", __func__, p_peer->addr.ToString().c_str());

  size_t updated_codecs = 0;
  for (const auto& iter : p_peer->GetCodecs()->orderedSourceCodecs()) {
    APPL_TRACE_DEBUG("%s: updating selectable codec %s", __func__,
                     iter->name().c_str());
    if (UpdateSelectableSourceCodec(*iter, p_peer)) {
      updated_codecs++;
    }
  }
  return updated_codecs;
}

bool BtaAvCo::UpdateSelectableSourceCodec(const A2dpCodecConfig& codec_config,
                                          BtaAvCoPeer* p_peer) {
  APPL_TRACE_DEBUG("%s: peer %s", __func__, p_peer->addr.ToString().c_str());

  // Find the peer Sink for the codec
  const BtaAvCoSep* p_sink = FindPeerSink(p_peer, codec_config.codecIndex());
  if (p_sink == nullptr) {
    // The peer Sink device does not support this codec
    return false;
  }
  if (!p_peer->GetCodecs()->setPeerSinkCodecCapabilities(p_sink->codec_caps)) {
    APPL_TRACE_WARNING("%s: cannot update peer %s codec capabilities for %s",
                       __func__, p_peer->addr.ToString().c_str(),
                       A2DP_CodecName(p_sink->codec_caps));
    return false;
  }
  return true;
}

size_t BtaAvCo::UpdateAllSelectableSinkCodecs(BtaAvCoPeer* p_peer) {
  APPL_TRACE_DEBUG("%s: peer %s", __func__, p_peer->addr.ToString().c_str());

  size_t updated_codecs = 0;
  for (const auto& iter : p_peer->GetCodecs()->orderedSinkCodecs()) {
    APPL_TRACE_DEBUG("%s: updating selectable codec %s", __func__,
                     iter->name().c_str());
    if (UpdateSelectableSinkCodec(*iter, p_peer)) {
      updated_codecs++;
    }
  }
  return updated_codecs;
}

bool BtaAvCo::UpdateSelectableSinkCodec(const A2dpCodecConfig& codec_config,
                                        BtaAvCoPeer* p_peer) {
  APPL_TRACE_DEBUG("%s: peer %s", __func__, p_peer->addr.ToString().c_str());

  // Find the peer Source for the codec
  const BtaAvCoSep* p_source =
      FindPeerSource(p_peer, codec_config.codecIndex());
  if (p_source == nullptr) {
    // The peer Source device does not support this codec
    return false;
  }
  if (!p_peer->GetCodecs()->setPeerSourceCodecCapabilities(
          p_source->codec_caps)) {
    APPL_TRACE_WARNING("%s: cannot update peer %s codec capabilities for %s",
                       __func__, p_peer->addr.ToString().c_str(),
                       A2DP_CodecName(p_source->codec_caps));
    return false;
  }
  return true;
}

void BtaAvCo::SaveNewCodecConfig(BtaAvCoPeer* p_peer,
                                 const uint8_t* new_codec_config,
                                 uint8_t num_protect,
                                 const uint8_t* p_protect_info) {
  APPL_TRACE_DEBUG("%s: peer %s", __func__, p_peer->addr.ToString().c_str());
  APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                   A2DP_CodecInfoString(new_codec_config).c_str());

  std::lock_guard<std::recursive_mutex> lock(codec_lock_);

  memcpy(codec_config_, new_codec_config, sizeof(codec_config_));
  memcpy(p_peer->codec_config, new_codec_config, AVDT_CODEC_SIZE);

  if (ContentProtectEnabled()) {
    // Check if this Sink supports SCMS
    bool cp_active = BtaAvCo::AudioProtectHasScmst(num_protect, p_protect_info);
    p_peer->SetContentProtectActive(cp_active);
  }
}

bool BtaAvCo::SetCodecOtaConfig(BtaAvCoPeer* p_peer,
                                const uint8_t* p_ota_codec_config,
                                uint8_t num_protect,
                                const uint8_t* p_protect_info,
                                bool* p_restart_output) {
  uint8_t result_codec_config[AVDT_CODEC_SIZE];
  bool restart_input = false;
  bool restart_output = false;
  bool config_updated = false;

  APPL_TRACE_DEBUG("%s: peer_address=%s", __func__,
                   p_peer->addr.ToString().c_str());
  APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                   A2DP_CodecInfoString(p_ota_codec_config).c_str());

  *p_restart_output = false;

  // Find the peer SEP codec to use
  const BtaAvCoSep* p_sink =
      FindPeerSink(p_peer, A2DP_SourceCodecIndex(p_ota_codec_config));
  if ((p_peer->num_sup_sinks > 0) && (p_sink == nullptr)) {
    // There are no peer SEPs if we didn't do the discovery procedure yet.
    // We have all the information we need from the peer, so we can
    // proceed with the OTA codec configuration.
    APPL_TRACE_ERROR("%s: peer %s : cannot find peer SEP to configure",
                     __func__, p_peer->addr.ToString().c_str());
    return false;
  }

  tA2DP_ENCODER_INIT_PEER_PARAMS peer_params;
  GetPeerEncoderParameters(p_peer->addr, &peer_params);
  if (!p_peer->GetCodecs()->setCodecOtaConfig(
          p_ota_codec_config, &peer_params, result_codec_config, &restart_input,
          &restart_output, &config_updated)) {
    APPL_TRACE_ERROR("%s: peer %s : cannot set OTA config", __func__,
                     p_peer->addr.ToString().c_str());
    return false;
  }

  if (restart_output) {
    APPL_TRACE_DEBUG("%s: restart output", __func__);
    APPL_TRACE_DEBUG("%s: codec: %s", __func__,
                     A2DP_CodecInfoString(result_codec_config).c_str());

    *p_restart_output = true;
    p_peer->p_sink = p_sink;
    SaveNewCodecConfig(p_peer, result_codec_config, num_protect,
                       p_protect_info);
  }

  if (restart_input || config_updated) {
    // NOTE: Currently, the input is restarted by sending an upcall
    // and informing the Media Framework about the change.
    ReportSourceCodecState(p_peer);
  }

  return true;
}

void bta_av_co_init(
    const std::vector<btav_a2dp_codec_config_t>& codec_priorities) {
  bta_av_co_cb.Init(codec_priorities);
}

bool bta_av_co_is_supported_codec(btav_a2dp_codec_index_t codec_index) {
  return bta_av_co_cb.IsSupportedCodec(codec_index);
}

A2dpCodecConfig* bta_av_get_a2dp_current_codec(void) {
  return bta_av_co_cb.GetActivePeerCurrentCodec();
}

A2dpCodecConfig* bta_av_get_a2dp_peer_current_codec(
    const RawAddress& peer_address) {
  return bta_av_co_cb.GetPeerCurrentCodec(peer_address);
}

bool bta_av_co_audio_init(btav_a2dp_codec_index_t codec_index,
                          AvdtpSepConfig* p_cfg) {
  return A2DP_InitCodecConfig(codec_index, p_cfg);
}

void bta_av_co_audio_disc_res(tBTA_AV_HNDL bta_av_handle,
                              const RawAddress& peer_address, uint8_t num_seps,
                              uint8_t num_sinks, uint8_t num_sources,
                              uint16_t uuid_local) {
  bta_av_co_cb.ProcessDiscoveryResult(bta_av_handle, peer_address, num_seps,
                                      num_sinks, num_sources, uuid_local);
}

tA2DP_STATUS bta_av_co_audio_getconfig(tBTA_AV_HNDL bta_av_handle,
                                       const RawAddress& peer_address,
                                       uint8_t* p_codec_info,
                                       uint8_t* p_sep_info_idx, uint8_t seid,
                                       uint8_t* p_num_protect,
                                       uint8_t* p_protect_info) {
  uint16_t peer_uuid = bta_av_co_cb.FindPeerUuid(bta_av_handle);

  APPL_TRACE_DEBUG("%s: peer %s bta_av_handle=0x%x peer_uuid=0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_handle, peer_uuid);

  switch (peer_uuid) {
    case UUID_SERVCLASS_AUDIO_SOURCE:
      return bta_av_co_cb.ProcessSinkGetConfig(
          bta_av_handle, peer_address, p_codec_info, p_sep_info_idx, seid,
          p_num_protect, p_protect_info);
    case UUID_SERVCLASS_AUDIO_SINK:
      return bta_av_co_cb.ProcessSourceGetConfig(
          bta_av_handle, peer_address, p_codec_info, p_sep_info_idx, seid,
          p_num_protect, p_protect_info);
    default:
      break;
  }
  APPL_TRACE_ERROR(
      "%s: peer %s : Invalid peer UUID: 0x%x for bta_av_handle 0x%x",
      peer_address.ToString().c_str(), peer_uuid, bta_av_handle);
  return A2DP_FAIL;
}

void bta_av_co_audio_setconfig(tBTA_AV_HNDL bta_av_handle,
                               const RawAddress& peer_address,
                               const uint8_t* p_codec_info, uint8_t seid,
                               uint8_t num_protect,
                               const uint8_t* p_protect_info,
                               uint8_t t_local_sep, uint8_t avdt_handle) {
  bta_av_co_cb.ProcessSetConfig(bta_av_handle, peer_address, p_codec_info, seid,
                                num_protect, p_protect_info, t_local_sep,
                                avdt_handle);
}

void bta_av_co_audio_open(tBTA_AV_HNDL bta_av_handle,
                          const RawAddress& peer_address, uint16_t mtu) {
  bta_av_co_cb.ProcessOpen(bta_av_handle, peer_address, mtu);
}

void bta_av_co_audio_close(tBTA_AV_HNDL bta_av_handle,
                           const RawAddress& peer_address) {
  bta_av_co_cb.ProcessClose(bta_av_handle, peer_address);
}

void bta_av_co_audio_start(tBTA_AV_HNDL bta_av_handle,
                           const RawAddress& peer_address,
                           const uint8_t* p_codec_info, bool* p_no_rtp_header) {
  bta_av_co_cb.ProcessStart(bta_av_handle, peer_address, p_codec_info,
                            p_no_rtp_header);
}

void bta_av_co_audio_stop(tBTA_AV_HNDL bta_av_handle,
                          const RawAddress& peer_address) {
  bta_av_co_cb.ProcessStop(bta_av_handle, peer_address);
}

BT_HDR* bta_av_co_audio_source_data_path(const uint8_t* p_codec_info,
                                         uint32_t* p_timestamp) {
  return bta_av_co_cb.GetNextSourceDataPacket(p_codec_info, p_timestamp);
}

void bta_av_co_audio_drop(tBTA_AV_HNDL bta_av_handle,
                          const RawAddress& peer_address) {
  bta_av_co_cb.DataPacketWasDropped(bta_av_handle, peer_address);
}

void bta_av_co_audio_delay(tBTA_AV_HNDL bta_av_handle,
                           const RawAddress& peer_address, uint16_t delay) {
  bta_av_co_cb.ProcessAudioDelay(bta_av_handle, peer_address, delay);
}

void bta_av_co_audio_update_mtu(tBTA_AV_HNDL bta_av_handle,
                                const RawAddress& peer_address, uint16_t mtu) {
  bta_av_co_cb.UpdateMtu(bta_av_handle, peer_address, mtu);
}

bool bta_av_co_set_active_peer(const RawAddress& peer_address) {
  return bta_av_co_cb.SetActivePeer(peer_address);
}

void bta_av_co_get_peer_params(const RawAddress& peer_address,
                               tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params) {
  bta_av_co_cb.GetPeerEncoderParameters(peer_address, p_peer_params);
}

const tA2DP_ENCODER_INTERFACE* bta_av_co_get_encoder_interface(void) {
  return bta_av_co_cb.GetSourceEncoderInterface();
}

const tA2DP_DECODER_INTERFACE* bta_av_co_get_decoder_interface(void) {
  return bta_av_co_cb.GetSinkDecoderInterface();
}

bool bta_av_co_set_codec_user_config(
    const RawAddress& peer_address,
    const btav_a2dp_codec_config_t& codec_user_config) {
  return bta_av_co_cb.SetCodecUserConfig(peer_address, codec_user_config);
}

bool bta_av_co_set_codec_audio_config(
    const btav_a2dp_codec_config_t& codec_audio_config) {
  return bta_av_co_cb.SetCodecAudioConfig(codec_audio_config);
}

bool bta_av_co_content_protect_is_active(const RawAddress& peer_address) {
  BtaAvCoPeer* p_peer = bta_av_co_cb.FindPeer(peer_address);
  CHECK(p_peer != nullptr);
  return p_peer->ContentProtectActive();
}

void btif_a2dp_codec_debug_dump(int fd) { bta_av_co_cb.DebugDump(fd); }
