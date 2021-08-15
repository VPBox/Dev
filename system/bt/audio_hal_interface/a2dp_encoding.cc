/*
 * Copyright 2019 The Android Open Source Project
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

#include "a2dp_encoding.h"
#include "client_interface.h"

#include "a2dp_aac_constants.h"
#include "a2dp_sbc_constants.h"
#include "a2dp_vendor_ldac_constants.h"
#include "bta/av/bta_av_int.h"
#include "btif_a2dp_source.h"
#include "btif_av.h"
#include "btif_av_co.h"
#include "btif_hf.h"
#include "osi/include/properties.h"

namespace {

using ::android::hardware::bluetooth::audio::V2_0::AacObjectType;
using ::android::hardware::bluetooth::audio::V2_0::AacVariableBitRate;
using ::android::hardware::bluetooth::audio::V2_0::CodecType;
using ::android::hardware::bluetooth::audio::V2_0::LdacChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::LdacQualityIndex;
using ::android::hardware::bluetooth::audio::V2_0::SbcAllocMethod;
using ::android::hardware::bluetooth::audio::V2_0::SbcBlockLength;
using ::android::hardware::bluetooth::audio::V2_0::SbcChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::SbcNumSubbands;
using ::bluetooth::audio::AudioConfiguration;
using ::bluetooth::audio::BitsPerSample;
using ::bluetooth::audio::BluetoothAudioCtrlAck;
using ::bluetooth::audio::ChannelMode;
using ::bluetooth::audio::CodecConfiguration;
using ::bluetooth::audio::PcmParameters;
using ::bluetooth::audio::SampleRate;
using ::bluetooth::audio::SessionType;

const CodecConfiguration kInvalidCodecConfiguration = {
    .codecType = CodecType::UNKNOWN,
    .encodedAudioBitrate = 0x00000000,
    .peerMtu = 0xffff,
    .isScmstEnabled = false,
    .config = {}};

BluetoothAudioCtrlAck a2dp_ack_to_bt_audio_ctrl_ack(tA2DP_CTRL_ACK ack);

// Provide call-in APIs for the Bluetooth Audio HAL
class A2dpTransport : public ::bluetooth::audio::IBluetoothTransportInstance {
 public:
  A2dpTransport(SessionType sessionType)
      : IBluetoothTransportInstance(sessionType, {}),
        a2dp_pending_cmd_(A2DP_CTRL_CMD_NONE),
        remote_delay_report_(0),
        total_bytes_read_(0),
        data_position_({}){};

  BluetoothAudioCtrlAck StartRequest() override {
    // Check if a previous request is not finished
    if (a2dp_pending_cmd_ == A2DP_CTRL_CMD_START) {
      LOG(INFO) << __func__ << ": A2DP_CTRL_CMD_START in progress";
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_PENDING);
    } else if (a2dp_pending_cmd_ != A2DP_CTRL_CMD_NONE) {
      LOG(WARNING) << __func__ << ": busy in pending_cmd=" << a2dp_pending_cmd_;
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_FAILURE);
    }

    // Don't send START request to stack while we are in a call
    if (!bluetooth::headset::IsCallIdle()) {
      LOG(ERROR) << __func__ << ": call state is busy";
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_INCALL_FAILURE);
    }

    if (btif_a2dp_source_is_streaming()) {
      LOG(ERROR) << __func__ << ": source is busy streaming";
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_FAILURE);
    }

    if (btif_av_stream_ready()) {
      /*
       * Post start event and wait for audio path to open.
       * If we are the source, the ACK will be sent after the start
       * procedure is completed, othewise send it now.
       */
      a2dp_pending_cmd_ = A2DP_CTRL_CMD_START;
      btif_av_stream_start();
      if (btif_av_get_peer_sep() != AVDT_TSEP_SRC) {
        LOG(INFO) << __func__ << ": accepted";
        return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_PENDING);
      }
      a2dp_pending_cmd_ = A2DP_CTRL_CMD_NONE;
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_SUCCESS);
    }

    if (btif_av_stream_started_ready()) {
      // Already started, ACK back immediately.
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_SUCCESS);
    }
    LOG(ERROR) << __func__ << ": AV stream is not ready to start";
    return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_FAILURE);
  }

  BluetoothAudioCtrlAck SuspendRequest() override {
    // Previous request is not finished
    if (a2dp_pending_cmd_ == A2DP_CTRL_CMD_SUSPEND) {
      LOG(INFO) << __func__ << ": A2DP_CTRL_CMD_SUSPEND in progress";
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_PENDING);
    } else if (a2dp_pending_cmd_ != A2DP_CTRL_CMD_NONE) {
      LOG(WARNING) << __func__ << ": busy in pending_cmd=" << a2dp_pending_cmd_;
      return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_FAILURE);
    }
    // Local suspend
    if (btif_av_stream_started_ready()) {
      LOG(INFO) << __func__ << ": accepted";
      a2dp_pending_cmd_ = A2DP_CTRL_CMD_SUSPEND;
      btif_av_stream_suspend();
      return BluetoothAudioCtrlAck::PENDING;
    }
    /* If we are not in started state, just ack back ok and let
     * audioflinger close the channel. This can happen if we are
     * remotely suspended, clear REMOTE SUSPEND flag.
     */
    btif_av_clear_remote_suspend_flag();
    return a2dp_ack_to_bt_audio_ctrl_ack(A2DP_CTRL_ACK_SUCCESS);
  }

  void StopRequest() override {
    if (btif_av_get_peer_sep() == AVDT_TSEP_SNK &&
        !btif_a2dp_source_is_streaming()) {
      return;
    }
    LOG(INFO) << __func__ << ": handling";
    a2dp_pending_cmd_ = A2DP_CTRL_CMD_STOP;
    btif_av_stream_stop(RawAddress::kEmpty);
  }

  bool GetPresentationPosition(uint64_t* remote_delay_report_ns,
                               uint64_t* total_bytes_read,
                               timespec* data_position) override {
    *remote_delay_report_ns = remote_delay_report_ * 100000u;
    *total_bytes_read = total_bytes_read_;
    *data_position = data_position_;
    VLOG(2) << __func__ << ": delay=" << remote_delay_report_
            << "/10ms, data=" << total_bytes_read_
            << " byte(s), timestamp=" << data_position_.tv_sec << "."
            << data_position_.tv_nsec << "s";
    return true;
  }

  void MetadataChanged(const source_metadata_t& source_metadata) override {
    auto track_count = source_metadata.track_count;
    auto tracks = source_metadata.tracks;
    VLOG(1) << __func__ << ": " << track_count << " track(s) received";
    while (track_count) {
      VLOG(2) << __func__ << ": usage=" << tracks->usage
              << ", content_type=" << tracks->content_type
              << ", gain=" << tracks->gain;
      --track_count;
      ++tracks;
    }
  }

  tA2DP_CTRL_CMD GetPendingCmd() const { return a2dp_pending_cmd_; }

  void ResetPendingCmd() { a2dp_pending_cmd_ = A2DP_CTRL_CMD_NONE; }

  void ResetPresentationPosition() override {
    remote_delay_report_ = 0;
    total_bytes_read_ = 0;
    data_position_ = {};
  }

  void LogBytesRead(size_t bytes_read) override {
    if (bytes_read != 0) {
      total_bytes_read_ += bytes_read;
      clock_gettime(CLOCK_MONOTONIC, &data_position_);
    }
  }

  // delay reports from AVDTP is based on 1/10 ms (100us)
  void SetRemoteDelay(uint16_t delay_report) {
    remote_delay_report_ = delay_report;
  }

 private:
  tA2DP_CTRL_CMD a2dp_pending_cmd_;
  uint16_t remote_delay_report_;
  uint64_t total_bytes_read_;
  timespec data_position_;
};

A2dpTransport* a2dp_sink = nullptr;

// Common interface to call-out into Bluetooth Audio HAL
bluetooth::audio::BluetoothAudioClientInterface* a2dp_hal_clientif = nullptr;
auto session_type = SessionType::UNKNOWN;

// Save the value if the remote reports its delay before a2dp_sink is
// initialized
uint16_t remote_delay = 0;

bool btaudio_a2dp_disabled = false;
bool is_configured = false;

BluetoothAudioCtrlAck a2dp_ack_to_bt_audio_ctrl_ack(tA2DP_CTRL_ACK ack) {
  switch (ack) {
    case A2DP_CTRL_ACK_SUCCESS:
      return BluetoothAudioCtrlAck::SUCCESS_FINISHED;
    case A2DP_CTRL_ACK_PENDING:
      return BluetoothAudioCtrlAck::PENDING;
    case A2DP_CTRL_ACK_INCALL_FAILURE:
      return BluetoothAudioCtrlAck::FAILURE_BUSY;
    case A2DP_CTRL_ACK_DISCONNECT_IN_PROGRESS:
      return BluetoothAudioCtrlAck::FAILURE_DISCONNECTING;
    case A2DP_CTRL_ACK_UNSUPPORTED: /* Offloading but resource failure */
      return BluetoothAudioCtrlAck::FAILURE_UNSUPPORTED;
    case A2DP_CTRL_ACK_FAILURE:
      return BluetoothAudioCtrlAck::FAILURE;
    default:
      return BluetoothAudioCtrlAck::FAILURE;
  }
}

SampleRate a2dp_codec_to_hal_sample_rate(
    const btav_a2dp_codec_config_t& a2dp_codec_config) {
  switch (a2dp_codec_config.sample_rate) {
    case BTAV_A2DP_CODEC_SAMPLE_RATE_44100:
      return SampleRate::RATE_44100;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_48000:
      return SampleRate::RATE_48000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_88200:
      return SampleRate::RATE_88200;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_96000:
      return SampleRate::RATE_96000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_176400:
      return SampleRate::RATE_176400;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_192000:
      return SampleRate::RATE_192000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_16000:
      return SampleRate::RATE_16000;
    case BTAV_A2DP_CODEC_SAMPLE_RATE_24000:
      return SampleRate::RATE_24000;
    default:
      return SampleRate::RATE_UNKNOWN;
  }
}

BitsPerSample a2dp_codec_to_hal_bits_per_sample(
    const btav_a2dp_codec_config_t& a2dp_codec_config) {
  switch (a2dp_codec_config.bits_per_sample) {
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16:
      return BitsPerSample::BITS_16;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_24:
      return BitsPerSample::BITS_24;
    case BTAV_A2DP_CODEC_BITS_PER_SAMPLE_32:
      return BitsPerSample::BITS_32;
    default:
      return BitsPerSample::BITS_UNKNOWN;
  }
}

ChannelMode a2dp_codec_to_hal_channel_mode(
    const btav_a2dp_codec_config_t& a2dp_codec_config) {
  switch (a2dp_codec_config.channel_mode) {
    case BTAV_A2DP_CODEC_CHANNEL_MODE_MONO:
      return ChannelMode::MONO;
    case BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO:
      return ChannelMode::STEREO;
    default:
      return ChannelMode::UNKNOWN;
  }
}

bool a2dp_get_selected_hal_codec_config(CodecConfiguration* codec_config) {
  A2dpCodecConfig* a2dp_codec_configs = bta_av_get_a2dp_current_codec();
  if (a2dp_codec_configs == nullptr) {
    LOG(WARNING) << __func__ << ": failure to get A2DP codec config";
    *codec_config = kInvalidCodecConfiguration;
    return false;
  }
  btav_a2dp_codec_config_t current_codec = a2dp_codec_configs->getCodecConfig();
  tBT_A2DP_OFFLOAD a2dp_offload;
  a2dp_codec_configs->getCodecSpecificConfig(&a2dp_offload);
  switch (current_codec.codec_type) {
    case BTAV_A2DP_CODEC_INDEX_SOURCE_SBC:
      [[fallthrough]];
    case BTAV_A2DP_CODEC_INDEX_SINK_SBC: {
      codec_config->codecType = CodecType::SBC;
      codec_config->config.sbcConfig({});
      auto sbc_config = codec_config->config.sbcConfig();
      sbc_config.sampleRate = a2dp_codec_to_hal_sample_rate(current_codec);
      if (sbc_config.sampleRate == SampleRate::RATE_UNKNOWN) {
        LOG(ERROR) << __func__
                   << ": Unknown SBC sample_rate=" << current_codec.sample_rate;
        return false;
      }
      uint8_t channel_mode = a2dp_offload.codec_info[3] & A2DP_SBC_IE_CH_MD_MSK;
      switch (channel_mode) {
        case A2DP_SBC_IE_CH_MD_JOINT:
          sbc_config.channelMode = SbcChannelMode::JOINT_STEREO;
          break;
        case A2DP_SBC_IE_CH_MD_STEREO:
          sbc_config.channelMode = SbcChannelMode::STEREO;
          break;
        case A2DP_SBC_IE_CH_MD_DUAL:
          sbc_config.channelMode = SbcChannelMode::DUAL;
          break;
        case A2DP_SBC_IE_CH_MD_MONO:
          sbc_config.channelMode = SbcChannelMode::MONO;
          break;
        default:
          LOG(ERROR) << __func__
                     << ": Unknown SBC channel_mode=" << channel_mode;
          sbc_config.channelMode = SbcChannelMode::UNKNOWN;
          return false;
      }
      uint8_t block_length =
          a2dp_offload.codec_info[0] & A2DP_SBC_IE_BLOCKS_MSK;
      switch (block_length) {
        case A2DP_SBC_IE_BLOCKS_4:
          sbc_config.blockLength = SbcBlockLength::BLOCKS_4;
          break;
        case A2DP_SBC_IE_BLOCKS_8:
          sbc_config.blockLength = SbcBlockLength::BLOCKS_8;
          break;
        case A2DP_SBC_IE_BLOCKS_12:
          sbc_config.blockLength = SbcBlockLength::BLOCKS_12;
          break;
        case A2DP_SBC_IE_BLOCKS_16:
          sbc_config.blockLength = SbcBlockLength::BLOCKS_16;
          break;
        default:
          LOG(ERROR) << __func__
                     << ": Unknown SBC block_length=" << block_length;
          return false;
      }
      uint8_t sub_bands = a2dp_offload.codec_info[0] & A2DP_SBC_IE_SUBBAND_MSK;
      switch (sub_bands) {
        case A2DP_SBC_IE_SUBBAND_4:
          sbc_config.numSubbands = SbcNumSubbands::SUBBAND_4;
          break;
        case A2DP_SBC_IE_SUBBAND_8:
          sbc_config.numSubbands = SbcNumSubbands::SUBBAND_8;
          break;
        default:
          LOG(ERROR) << __func__ << ": Unknown SBC Subbands=" << sub_bands;
          return false;
      }
      uint8_t alloc_method =
          a2dp_offload.codec_info[0] & A2DP_SBC_IE_ALLOC_MD_MSK;
      switch (alloc_method) {
        case A2DP_SBC_IE_ALLOC_MD_S:
          sbc_config.allocMethod = SbcAllocMethod::ALLOC_MD_S;
          break;
        case A2DP_SBC_IE_ALLOC_MD_L:
          sbc_config.allocMethod = SbcAllocMethod::ALLOC_MD_L;
          break;
        default:
          LOG(ERROR) << __func__
                     << ": Unknown SBC alloc_method=" << alloc_method;
          return false;
      }
      sbc_config.minBitpool = a2dp_offload.codec_info[1];
      sbc_config.maxBitpool = a2dp_offload.codec_info[2];
      sbc_config.bitsPerSample =
          a2dp_codec_to_hal_bits_per_sample(current_codec);
      if (sbc_config.bitsPerSample == BitsPerSample::BITS_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown SBC bits_per_sample="
                   << current_codec.bits_per_sample;
        return false;
      }
      codec_config->config.sbcConfig(sbc_config);
      break;
    }
    case BTAV_A2DP_CODEC_INDEX_SOURCE_AAC:
      [[fallthrough]];
    case BTAV_A2DP_CODEC_INDEX_SINK_AAC: {
      codec_config->codecType = CodecType::AAC;
      codec_config->config.aacConfig({});
      auto aac_config = codec_config->config.aacConfig();
      uint8_t object_type = a2dp_offload.codec_info[0];
      switch (object_type) {
        case A2DP_AAC_OBJECT_TYPE_MPEG2_LC:
          aac_config.objectType = AacObjectType::MPEG2_LC;
          break;
        case A2DP_AAC_OBJECT_TYPE_MPEG4_LC:
          aac_config.objectType = AacObjectType::MPEG4_LC;
          break;
        case A2DP_AAC_OBJECT_TYPE_MPEG4_LTP:
          aac_config.objectType = AacObjectType::MPEG4_LTP;
          break;
        case A2DP_AAC_OBJECT_TYPE_MPEG4_SCALABLE:
          aac_config.objectType = AacObjectType::MPEG4_SCALABLE;
          break;
        default:
          LOG(ERROR) << __func__
                     << ": Unknown AAC object_type=" << +object_type;
          return false;
      }
      aac_config.sampleRate = a2dp_codec_to_hal_sample_rate(current_codec);
      if (aac_config.sampleRate == SampleRate::RATE_UNKNOWN) {
        LOG(ERROR) << __func__
                   << ": Unknown AAC sample_rate=" << current_codec.sample_rate;
        return false;
      }
      aac_config.channelMode = a2dp_codec_to_hal_channel_mode(current_codec);
      if (aac_config.channelMode == ChannelMode::UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown AAC channel_mode="
                   << current_codec.channel_mode;
        return false;
      }
      uint8_t vbr_enabled =
          a2dp_offload.codec_info[1] & A2DP_AAC_VARIABLE_BIT_RATE_MASK;
      switch (vbr_enabled) {
        case A2DP_AAC_VARIABLE_BIT_RATE_ENABLED:
          aac_config.variableBitRateEnabled = AacVariableBitRate::ENABLED;
          break;
        case A2DP_AAC_VARIABLE_BIT_RATE_DISABLED:
          aac_config.variableBitRateEnabled = AacVariableBitRate::DISABLED;
          break;
        default:
          LOG(ERROR) << __func__ << ": Unknown AAC VBR=" << +vbr_enabled;
          return false;
      }
      aac_config.bitsPerSample =
          a2dp_codec_to_hal_bits_per_sample(current_codec);
      if (aac_config.bitsPerSample == BitsPerSample::BITS_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown AAC bits_per_sample="
                   << current_codec.bits_per_sample;
        return false;
      }
      codec_config->config.aacConfig(aac_config);
      break;
    }
    case BTAV_A2DP_CODEC_INDEX_SOURCE_APTX:
      [[fallthrough]];
    case BTAV_A2DP_CODEC_INDEX_SOURCE_APTX_HD: {
      if (current_codec.codec_type == BTAV_A2DP_CODEC_INDEX_SOURCE_APTX) {
        codec_config->codecType = CodecType::APTX;
      } else {
        codec_config->codecType = CodecType::APTX_HD;
      }
      codec_config->config.aptxConfig({});
      auto aptx_config = codec_config->config.aptxConfig();
      aptx_config.sampleRate = a2dp_codec_to_hal_sample_rate(current_codec);
      if (aptx_config.sampleRate == SampleRate::RATE_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown aptX sample_rate="
                   << current_codec.sample_rate;
        return false;
      }
      aptx_config.channelMode = a2dp_codec_to_hal_channel_mode(current_codec);
      if (aptx_config.channelMode == ChannelMode::UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown aptX channel_mode="
                   << current_codec.channel_mode;
        return false;
      }
      aptx_config.bitsPerSample =
          a2dp_codec_to_hal_bits_per_sample(current_codec);
      if (aptx_config.bitsPerSample == BitsPerSample::BITS_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown aptX bits_per_sample="
                   << current_codec.bits_per_sample;
        return false;
      }
      codec_config->config.aptxConfig(aptx_config);
      break;
    }
    case BTAV_A2DP_CODEC_INDEX_SOURCE_LDAC: {
      codec_config->codecType = CodecType::LDAC;
      codec_config->config.ldacConfig({});
      auto ldac_config = codec_config->config.ldacConfig();
      ldac_config.sampleRate = a2dp_codec_to_hal_sample_rate(current_codec);
      if (ldac_config.sampleRate == SampleRate::RATE_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown LDAC sample_rate="
                   << current_codec.sample_rate;
        return false;
      }
      switch (a2dp_offload.codec_info[7]) {
        case A2DP_LDAC_CHANNEL_MODE_STEREO:
          ldac_config.channelMode = LdacChannelMode::STEREO;
          break;
        case A2DP_LDAC_CHANNEL_MODE_DUAL:
          ldac_config.channelMode = LdacChannelMode::DUAL;
          break;
        case A2DP_LDAC_CHANNEL_MODE_MONO:
          ldac_config.channelMode = LdacChannelMode::MONO;
          break;
        default:
          LOG(ERROR) << __func__ << ": Unknown LDAC channel_mode="
                     << a2dp_offload.codec_info[7];
          ldac_config.channelMode = LdacChannelMode::UNKNOWN;
          return false;
      }
      switch (a2dp_offload.codec_info[6]) {
        case A2DP_LDAC_QUALITY_HIGH:
          ldac_config.qualityIndex = LdacQualityIndex::QUALITY_HIGH;
          break;
        case A2DP_LDAC_QUALITY_MID:
          ldac_config.qualityIndex = LdacQualityIndex::QUALITY_MID;
          break;
        case A2DP_LDAC_QUALITY_LOW:
          ldac_config.qualityIndex = LdacQualityIndex::QUALITY_LOW;
          break;
        case A2DP_LDAC_QUALITY_ABR_OFFLOAD:
          ldac_config.qualityIndex = LdacQualityIndex::QUALITY_ABR;
          break;
        default:
          LOG(ERROR) << __func__ << ": Unknown LDAC QualityIndex="
                     << a2dp_offload.codec_info[6];
          return false;
      }
      ldac_config.bitsPerSample =
          a2dp_codec_to_hal_bits_per_sample(current_codec);
      if (ldac_config.bitsPerSample == BitsPerSample::BITS_UNKNOWN) {
        LOG(ERROR) << __func__ << ": Unknown LDAC bits_per_sample="
                   << current_codec.bits_per_sample;
        return false;
      }
      codec_config->config.ldacConfig(ldac_config);
      break;
    }
    case BTAV_A2DP_CODEC_INDEX_MAX:
      [[fallthrough]];
    default:
      LOG(ERROR) << __func__
                 << ": Unknown codec_type=" << current_codec.codec_type;
      codec_config->codecType = CodecType::UNKNOWN;
      codec_config->config = {};
      return false;
  }
  codec_config->encodedAudioBitrate = a2dp_codec_configs->getTrackBitRate();
  // Obtain the MTU
  RawAddress peer_addr = btif_av_source_active_peer();
  tA2DP_ENCODER_INIT_PEER_PARAMS peer_param;
  bta_av_co_get_peer_params(peer_addr, &peer_param);
  int effectiveMtu = a2dp_codec_configs->getEffectiveMtu();
  if (effectiveMtu > 0 && effectiveMtu < peer_param.peer_mtu) {
    codec_config->peerMtu = effectiveMtu;
  } else {
    codec_config->peerMtu = peer_param.peer_mtu;
  }
  LOG(INFO) << __func__ << ": CodecConfiguration=" << toString(*codec_config);
  return true;
}

bool a2dp_get_selected_hal_pcm_config(PcmParameters* pcm_config) {
  if (pcm_config == nullptr) return false;
  A2dpCodecConfig* a2dp_codec_configs = bta_av_get_a2dp_current_codec();
  if (a2dp_codec_configs == nullptr) {
    LOG(WARNING) << __func__ << ": failure to get A2DP codec config";
    *pcm_config = ::bluetooth::audio::BluetoothAudioClientInterface::
        kInvalidPcmConfiguration;
    return false;
  }

  btav_a2dp_codec_config_t current_codec = a2dp_codec_configs->getCodecConfig();
  pcm_config->sampleRate = a2dp_codec_to_hal_sample_rate(current_codec);
  pcm_config->bitsPerSample = a2dp_codec_to_hal_bits_per_sample(current_codec);
  pcm_config->channelMode = a2dp_codec_to_hal_channel_mode(current_codec);
  return (pcm_config->sampleRate != SampleRate::RATE_UNKNOWN &&
          pcm_config->bitsPerSample != BitsPerSample::BITS_UNKNOWN &&
          pcm_config->channelMode != ChannelMode::UNKNOWN);
}

// Checking if new bluetooth_audio is supported
bool is_hal_2_0_force_disabled() {
  if (!is_configured) {
    btaudio_a2dp_disabled = osi_property_get_bool(BLUETOOTH_AUDIO_HAL_PROP_DISABLED, false);
    is_configured = true;
  }
  return btaudio_a2dp_disabled;
}

}  // namespace

namespace bluetooth {
namespace audio {
namespace a2dp {

// Checking if new bluetooth_audio is enabled
bool is_hal_2_0_enabled() { return a2dp_hal_clientif != nullptr; }

// Initialize BluetoothAudio HAL: openProvider
bool init(bluetooth::common::MessageLoopThread* message_loop) {
  LOG(INFO) << __func__;

  if (is_hal_2_0_force_disabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is disabled";
    return false;
  }

  if (btif_av_is_a2dp_offload_enabled()) {
    session_type = SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH;
  } else {
    session_type = SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH;
  }
  a2dp_sink = new A2dpTransport(session_type);
  a2dp_hal_clientif = new bluetooth::audio::BluetoothAudioClientInterface(
      a2dp_sink, message_loop);
  if (!a2dp_hal_clientif->IsValid()) {
    LOG(WARNING) << __func__ << ": BluetoothAudio HAL for A2DP session=" << toString(session_type) << " is invalid?!";
    delete a2dp_hal_clientif;
    a2dp_hal_clientif = nullptr;
    delete a2dp_sink;
    a2dp_sink = nullptr;
    return false;
  }

  if (remote_delay != 0) {
    LOG(INFO) << __func__ << ": restore DELAY "
              << static_cast<float>(remote_delay / 10.0) << " ms";
    a2dp_sink->SetRemoteDelay(remote_delay);
    remote_delay = 0;
  }
  return true;
}

// Clean up BluetoothAudio HAL
void cleanup() {
  if (!is_hal_2_0_enabled()) return;
  end_session();
  delete a2dp_hal_clientif;
  a2dp_hal_clientif = nullptr;
  delete a2dp_sink;
  a2dp_sink = nullptr;
  session_type = SessionType::UNKNOWN;
  remote_delay = 0;
}

// Set up the codec into BluetoothAudio HAL
bool setup_codec() {
  if (!is_hal_2_0_enabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is not enabled";
    return false;
  }
  AudioConfiguration audio_config{};
  if (session_type == SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH) {
    CodecConfiguration codec_config{};
    if (!a2dp_get_selected_hal_codec_config(&codec_config)) {
      LOG(ERROR) << __func__ << ": Failed to get CodecConfiguration";
      return false;
    }
    audio_config.codecConfig(codec_config);
  } else if (session_type == SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH) {
    PcmParameters pcm_config{};
    if (!a2dp_get_selected_hal_pcm_config(&pcm_config)) {
      LOG(ERROR) << __func__ << ": Failed to get PcmConfiguration";
      return false;
    }
    audio_config.pcmConfig(pcm_config);
  }
  return a2dp_hal_clientif->UpdateAudioConfig(audio_config);
}

void start_session() {
  if (!is_hal_2_0_enabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is not enabled";
    return;
  }
  a2dp_hal_clientif->StartSession();
}

void end_session() {
  if (!is_hal_2_0_enabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is not enabled";
    return;
  }
  a2dp_hal_clientif->EndSession();
}

void ack_stream_started(const tA2DP_CTRL_ACK& ack) {
  auto ctrl_ack = a2dp_ack_to_bt_audio_ctrl_ack(ack);
  LOG(INFO) << __func__ << ": result=" << ctrl_ack;
  auto pending_cmd = a2dp_sink->GetPendingCmd();
  if (pending_cmd == A2DP_CTRL_CMD_START) {
    a2dp_hal_clientif->StreamStarted(ctrl_ack);
  } else {
    LOG(WARNING) << __func__ << ": pending=" << pending_cmd
                 << " ignore result=" << ctrl_ack;
    return;
  }
  if (ctrl_ack != bluetooth::audio::BluetoothAudioCtrlAck::PENDING) {
    a2dp_sink->ResetPendingCmd();
  }
}

void ack_stream_suspended(const tA2DP_CTRL_ACK& ack) {
  auto ctrl_ack = a2dp_ack_to_bt_audio_ctrl_ack(ack);
  LOG(INFO) << __func__ << ": result=" << ctrl_ack;
  auto pending_cmd = a2dp_sink->GetPendingCmd();
  if (pending_cmd == A2DP_CTRL_CMD_SUSPEND) {
    a2dp_hal_clientif->StreamSuspended(ctrl_ack);
  } else if (pending_cmd == A2DP_CTRL_CMD_STOP) {
    LOG(INFO) << __func__ << ": A2DP_CTRL_CMD_STOP result=" << ctrl_ack;
  } else {
    LOG(WARNING) << __func__ << ": pending=" << pending_cmd
                 << " ignore result=" << ctrl_ack;
    return;
  }
  if (ctrl_ack != bluetooth::audio::BluetoothAudioCtrlAck::PENDING) {
    a2dp_sink->ResetPendingCmd();
  }
}

// Read from the FMQ of BluetoothAudio HAL
size_t read(uint8_t* p_buf, uint32_t len) {
  if (!is_hal_2_0_enabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is not enabled";
    return 0;
  } else if (session_type != SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH) {
    LOG(ERROR) << __func__ << ": session_type=" << toString(session_type)
               << " is not A2DP_SOFTWARE_ENCODING_DATAPATH";
    return 0;
  }
  return a2dp_hal_clientif->ReadAudioData(p_buf, len);
}

// Update A2DP delay report to BluetoothAudio HAL
void set_remote_delay(uint16_t delay_report) {
  if (!is_hal_2_0_enabled()) {
    LOG(INFO) << __func__ << ":  not ready for DelayReport "
              << static_cast<float>(delay_report / 10.0) << " ms";
    remote_delay = delay_report;
    return;
  }
  LOG(INFO) << __func__ << ": DELAY " << static_cast<float>(delay_report / 10.0)
            << " ms";
  a2dp_sink->SetRemoteDelay(delay_report);
}

}  // namespace a2dp
}  // namespace audio
}  // namespace bluetooth
