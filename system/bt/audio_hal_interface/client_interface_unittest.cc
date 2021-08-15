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

#include <gtest/gtest.h>

#include "client_interface.h"

namespace {

using ::android::hardware::bluetooth::audio::V2_0::AacObjectType;
using ::android::hardware::bluetooth::audio::V2_0::AacParameters;
using ::android::hardware::bluetooth::audio::V2_0::AacVariableBitRate;
using ::android::hardware::bluetooth::audio::V2_0::AptxParameters;
using ::android::hardware::bluetooth::audio::V2_0::CodecCapabilities;
using ::android::hardware::bluetooth::audio::V2_0::CodecConfiguration;
using ::android::hardware::bluetooth::audio::V2_0::CodecType;
using ::android::hardware::bluetooth::audio::V2_0::LdacChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::LdacParameters;
using ::android::hardware::bluetooth::audio::V2_0::LdacQualityIndex;
using ::android::hardware::bluetooth::audio::V2_0::SbcAllocMethod;
using ::android::hardware::bluetooth::audio::V2_0::SbcBlockLength;
using ::android::hardware::bluetooth::audio::V2_0::SbcChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::SbcNumSubbands;
using ::android::hardware::bluetooth::audio::V2_0::SbcParameters;

using ::bluetooth::audio::AudioCapabilities;
using ::bluetooth::audio::AudioConfiguration;
using ::bluetooth::audio::BitsPerSample;
using ::bluetooth::audio::BluetoothAudioClientInterface;
using ::bluetooth::audio::BluetoothAudioStatus;
using ::bluetooth::audio::ChannelMode;
using ::bluetooth::audio::PcmParameters;
using ::bluetooth::audio::SampleRate;
using ::bluetooth::audio::SessionType;
using ::testing::Test;

constexpr SampleRate kSampleRates[9] = {
    SampleRate::RATE_UNKNOWN, SampleRate::RATE_44100, SampleRate::RATE_48000,
    SampleRate::RATE_88200,   SampleRate::RATE_96000, SampleRate::RATE_176400,
    SampleRate::RATE_192000,  SampleRate::RATE_16000, SampleRate::RATE_24000};
constexpr BitsPerSample kBitsPerSamples[4] = {
    BitsPerSample::BITS_UNKNOWN, BitsPerSample::BITS_16, BitsPerSample::BITS_24,
    BitsPerSample::BITS_32};
constexpr ChannelMode kChannelModes[3] = {
    ChannelMode::UNKNOWN, ChannelMode::MONO, ChannelMode::STEREO};
constexpr uint16_t kPeerMtus[5] = {660, 663, 883, 1005, 1500};

class TestTransport : public bluetooth::audio::IBluetoothTransportInstance {
 private:
  static constexpr uint64_t kRemoteDelayReportMs = 200;

 public:
  TestTransport(SessionType session_type)
      : bluetooth::audio::IBluetoothTransportInstance(session_type, {}){};
  bluetooth::audio::BluetoothAudioCtrlAck StartRequest() {
    return bluetooth::audio::BluetoothAudioCtrlAck::SUCCESS_FINISHED;
  }
  bluetooth::audio::BluetoothAudioCtrlAck SuspendRequest() {
    return bluetooth::audio::BluetoothAudioCtrlAck::SUCCESS_FINISHED;
  }
  void StopRequest() {}
  bool GetPresentationPosition(uint64_t* remote_delay_report_ns,
                               uint64_t* total_bytes_readed,
                               timespec* data_position) {
    if (remote_delay_report_ns) {
      *remote_delay_report_ns = kRemoteDelayReportMs * 1000000;
    }
    if (total_bytes_readed) {
      *total_bytes_readed = 0;
    }
    if (data_position) {
      clock_gettime(CLOCK_MONOTONIC, data_position);
    }
    return true;
  }
  void MetadataChanged(const source_metadata_t& source_metadata __unused) {}
  void ResetPresentationPosition(){};
  void LogBytesRead(size_t bytes_readed __unused){};
};

class BluetoothAudioClientInterfaceTest : public Test {
 protected:
  TestTransport* test_transport_;
  BluetoothAudioClientInterface* clientif_;

  static constexpr int kClientIfReturnSuccess = 0;

  virtual void SetUp() override {}

  virtual void TearDown() override {
    clientif_ = nullptr;
    test_transport_ = nullptr;
  }

  bool IsSoftwarePcmParametersSupported(const PcmParameters& pcm_config) {
    const std::vector<AudioCapabilities>& capabilities =
        clientif_->GetAudioCapabilities();
    PcmParameters pcm_capabilities = capabilities[0].pcmCapabilities();
    bool is_pcm_config_valid =
        (pcm_config.sampleRate != SampleRate::RATE_UNKNOWN &&
         pcm_config.bitsPerSample != BitsPerSample::BITS_UNKNOWN &&
         pcm_config.channelMode != ChannelMode::UNKNOWN);
    bool is_pcm_config_supported =
        (pcm_config.sampleRate & pcm_capabilities.sampleRate &&
         pcm_config.bitsPerSample & pcm_capabilities.bitsPerSample &&
         pcm_config.channelMode & pcm_capabilities.channelMode);
    return (is_pcm_config_valid && is_pcm_config_supported);
  }

  bool IsOffloadCodecConfigurationSupported(
      const CodecConfiguration& codec_config) {
    CodecCapabilities codec_capability = {};
    for (auto audio_capability : clientif_->GetAudioCapabilities()) {
      if (audio_capability.codecCapabilities().codecType ==
          codec_config.codecType) {
        codec_capability = audio_capability.codecCapabilities();
      }
    }
    if (codec_capability.codecType != codec_config.codecType) {
      // codec is unsupported
      return false;
    }
    bool is_codec_config_supported = false;
    switch (codec_config.codecType) {
      case CodecType::SBC: {
        SbcParameters sbc_config = codec_config.config.sbcConfig();
        SbcParameters sbc_capability =
            codec_capability.capabilities.sbcCapabilities();
        is_codec_config_supported =
            (sbc_config.sampleRate & sbc_capability.sampleRate &&
             sbc_config.channelMode & sbc_capability.channelMode &&
             sbc_config.blockLength & sbc_capability.blockLength &&
             sbc_config.numSubbands & sbc_capability.numSubbands &&
             sbc_config.allocMethod & sbc_capability.allocMethod &&
             sbc_config.bitsPerSample & sbc_capability.bitsPerSample &&
             (sbc_capability.minBitpool <= sbc_config.minBitpool &&
              sbc_config.minBitpool <= sbc_config.maxBitpool &&
              sbc_config.maxBitpool <= sbc_capability.maxBitpool));
        return is_codec_config_supported;
      }
      case CodecType::AAC: {
        AacParameters aac_config = codec_config.config.aacConfig();
        AacParameters aac_capability =
            codec_capability.capabilities.aacCapabilities();
        is_codec_config_supported =
            (aac_config.objectType & aac_capability.objectType &&
             aac_config.sampleRate & aac_capability.sampleRate &&
             aac_config.channelMode & aac_capability.channelMode &&
             (aac_config.variableBitRateEnabled ==
                  AacVariableBitRate::DISABLED ||
              aac_capability.variableBitRateEnabled ==
                  AacVariableBitRate::ENABLED) &&
             aac_config.bitsPerSample & aac_capability.bitsPerSample);
        return is_codec_config_supported;
      }
      case CodecType::LDAC: {
        LdacParameters ldac_config = codec_config.config.ldacConfig();
        LdacParameters ldac_capability =
            codec_capability.capabilities.ldacCapabilities();
        is_codec_config_supported =
            (ldac_config.sampleRate & ldac_capability.sampleRate &&
             ldac_config.channelMode & ldac_capability.channelMode &&
             ldac_config.bitsPerSample & ldac_capability.bitsPerSample);
        return is_codec_config_supported;
      }
      case CodecType::APTX:
        [[fallthrough]];
      case CodecType::APTX_HD: {
        AptxParameters aptx_config = codec_config.config.aptxConfig();
        AptxParameters aptx_capability =
            codec_capability.capabilities.aptxCapabilities();
        is_codec_config_supported =
            (aptx_config.sampleRate & aptx_capability.sampleRate &&
             aptx_config.channelMode & aptx_capability.channelMode &&
             aptx_config.bitsPerSample & aptx_capability.bitsPerSample);
        return is_codec_config_supported;
      }
      case CodecType::UNKNOWN:
        return false;
    }
  }
};

}  // namespace

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpSoftwareSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  PcmParameters pcm_config = {};
  for (auto sample_rate : kSampleRates) {
    pcm_config.sampleRate = sample_rate;
    for (auto bits_per_sample : kBitsPerSamples) {
      pcm_config.bitsPerSample = bits_per_sample;
      for (auto channel_mode : kChannelModes) {
        pcm_config.channelMode = channel_mode;
        audio_config.pcmConfig(pcm_config);
        clientif_->UpdateAudioConfig(audio_config);
        if (IsSoftwarePcmParametersSupported(pcm_config)) {
          EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
        } else {
          EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
        }
        EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
      }  // ChannelMode
    }    // BitsPerSampple
  }      // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpOffloadSbcSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  SbcBlockLength block_lengths[4] = {
      SbcBlockLength::BLOCKS_4, SbcBlockLength::BLOCKS_8,
      SbcBlockLength::BLOCKS_12, SbcBlockLength::BLOCKS_16};
  SbcNumSubbands num_subbands[2] = {SbcNumSubbands::SUBBAND_4,
                                    SbcNumSubbands::SUBBAND_8};
  SbcAllocMethod alloc_methods[2] = {SbcAllocMethod::ALLOC_MD_S,
                                     SbcAllocMethod::ALLOC_MD_L};
  for (auto sample_rate : kSampleRates) {
    for (auto bits_per_sample : kBitsPerSamples) {
      for (auto channel_mode : kChannelModes) {
        for (auto peer_mtu : kPeerMtus) {
          for (auto block_length : block_lengths) {
            for (auto num_subband : num_subbands) {
              for (auto alloc_method : alloc_methods) {
                codec_config.codecType = CodecType::SBC;
                codec_config.peerMtu = peer_mtu;
                codec_config.isScmstEnabled = false;
                // A2DP_SBC_DEFAULT_BITRATE
                codec_config.encodedAudioBitrate = 328000;
                SbcParameters sbc = {
                    .sampleRate = sample_rate,
                    .channelMode = (channel_mode == ChannelMode::MONO
                                        ? SbcChannelMode::MONO
                                        : SbcChannelMode::JOINT_STEREO),
                    .blockLength = block_length,
                    .numSubbands = num_subband,
                    .allocMethod = alloc_method,
                    .bitsPerSample = bits_per_sample,
                    .minBitpool = 2,
                    .maxBitpool = 53};
                codec_config.config.sbcConfig(sbc);
                audio_config.codecConfig(codec_config);
                clientif_->UpdateAudioConfig(audio_config);
                if (IsOffloadCodecConfigurationSupported(codec_config)) {
                  EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
                } else {
                  EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
                }
                EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
              }  // SbcAllocMethod
            }    // SbcNumSubbands
          }      // SbcBlockLength
        }        // peerMtu
      }          // ChannelMode
    }            // BitsPerSampple
  }              // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpOffloadAacSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  AacObjectType object_types[4] = {
      AacObjectType::MPEG2_LC, AacObjectType::MPEG4_LC,
      AacObjectType::MPEG4_LTP, AacObjectType::MPEG4_SCALABLE};
  AacVariableBitRate variable_bitrates[2] = {AacVariableBitRate::DISABLED,
                                             AacVariableBitRate::ENABLED};
  for (auto sample_rate : kSampleRates) {
    for (auto bits_per_sample : kBitsPerSamples) {
      for (auto channel_mode : kChannelModes) {
        for (auto peer_mtu : kPeerMtus) {
          for (auto object_type : object_types) {
            for (auto variable_bitrate : variable_bitrates) {
              codec_config.codecType = CodecType::AAC;
              codec_config.peerMtu = peer_mtu;
              codec_config.isScmstEnabled = false;
              // A2DP_AAC_DEFAULT_BITRATE
              codec_config.encodedAudioBitrate = 320000;
              AacParameters aac = {.objectType = object_type,
                                   .sampleRate = sample_rate,
                                   .channelMode = channel_mode,
                                   .variableBitRateEnabled = variable_bitrate,
                                   .bitsPerSample = bits_per_sample};
              codec_config.config.aacConfig(aac);
              audio_config.codecConfig(codec_config);
              clientif_->UpdateAudioConfig(audio_config);
              if (IsOffloadCodecConfigurationSupported(codec_config)) {
                EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
              } else {
                EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
              }
              EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
            }  // AacVariableBitRate
          }    // AacObjectType
        }      // peerMtu
      }        // ChannelMode
    }          // BitsPerSampple
  }            // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpOffloadLdacSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  LdacQualityIndex quality_indexes[4] = {
      LdacQualityIndex::QUALITY_HIGH, LdacQualityIndex::QUALITY_MID,
      LdacQualityIndex::QUALITY_LOW, LdacQualityIndex::QUALITY_ABR};
  for (auto sample_rate : kSampleRates) {
    for (auto bits_per_sample : kBitsPerSamples) {
      for (auto channel_mode : kChannelModes) {
        for (auto peer_mtu : kPeerMtus) {
          for (auto quality_index : quality_indexes) {
            codec_config.codecType = CodecType::LDAC;
            codec_config.peerMtu = peer_mtu;
            codec_config.isScmstEnabled = false;
            codec_config.encodedAudioBitrate = 990000;
            LdacParameters ldac = {
                .sampleRate = sample_rate,
                .channelMode = (channel_mode == ChannelMode::MONO
                                    ? LdacChannelMode::MONO
                                    : LdacChannelMode::STEREO),
                .qualityIndex = quality_index,
                .bitsPerSample = bits_per_sample};
            codec_config.config.ldacConfig(ldac);
            audio_config.codecConfig(codec_config);
            clientif_->UpdateAudioConfig(audio_config);
            if (IsOffloadCodecConfigurationSupported(codec_config)) {
              EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
            } else {
              EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
            }
            EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
          }  // LdacQualityIndex
        }    // peerMtu
      }      // ChannelMode
    }        // BitsPerSampple
  }          // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpOffloadAptxSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  for (auto sample_rate : kSampleRates) {
    for (auto bits_per_sample : kBitsPerSamples) {
      for (auto channel_mode : kChannelModes) {
        for (auto peer_mtu : kPeerMtus) {
          codec_config.codecType = CodecType::APTX;
          codec_config.peerMtu = peer_mtu;
          codec_config.isScmstEnabled = false;
          codec_config.encodedAudioBitrate = 352000;
          AptxParameters aptx = {.sampleRate = sample_rate,
                                 .channelMode = channel_mode,
                                 .bitsPerSample = bits_per_sample};
          codec_config.config.aptxConfig(aptx);
          audio_config.codecConfig(codec_config);
          clientif_->UpdateAudioConfig(audio_config);
          if (IsOffloadCodecConfigurationSupported(codec_config)) {
            EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
          } else {
            EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
          }
          EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
        }  // peerMtu
      }    // ChannelMode
    }      // BitsPerSampple
  }        // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest, StartAndEndA2dpOffloadAptxHdSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  for (auto sample_rate : kSampleRates) {
    for (auto bits_per_sample : kBitsPerSamples) {
      for (auto channel_mode : kChannelModes) {
        for (auto peer_mtu : kPeerMtus) {
          codec_config.codecType = CodecType::APTX_HD;
          codec_config.peerMtu = peer_mtu;
          codec_config.isScmstEnabled = false;
          codec_config.encodedAudioBitrate = 576000;
          AptxParameters aptx = {.sampleRate = sample_rate,
                                 .channelMode = channel_mode,
                                 .bitsPerSample = bits_per_sample};
          codec_config.config.aptxConfig(aptx);
          audio_config.codecConfig(codec_config);
          clientif_->UpdateAudioConfig(audio_config);
          if (IsOffloadCodecConfigurationSupported(codec_config)) {
            EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
          } else {
            EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
          }
          EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
        }  // peerMtu
      }    // ChannelMode
    }      // BitsPerSampple
  }        // SampleRate
}

TEST_F(BluetoothAudioClientInterfaceTest,
       StartAndEndA2dpOffloadUnknownSession) {
  test_transport_ =
      new TestTransport(SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  CodecConfiguration codec_config = {};
  codec_config.codecType = CodecType::UNKNOWN;
  codec_config.peerMtu = 1005;
  codec_config.isScmstEnabled = false;
  codec_config.encodedAudioBitrate = 328000;
  codec_config.config = {};
  audio_config.codecConfig(codec_config);
  clientif_->UpdateAudioConfig(audio_config);
  if (IsOffloadCodecConfigurationSupported(codec_config)) {
    EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
  } else {
    EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
  }
  EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
}

TEST_F(BluetoothAudioClientInterfaceTest,
       StartAndEndHearingAidSoftwareSession) {
  test_transport_ =
      new TestTransport(SessionType::HEARING_AID_SOFTWARE_ENCODING_DATAPATH);
  clientif_ = new BluetoothAudioClientInterface(test_transport_, nullptr);
  AudioConfiguration audio_config = {};
  PcmParameters pcm_config = {};
  for (auto sample_rate : kSampleRates) {
    pcm_config.sampleRate = sample_rate;
    for (auto bits_per_sample : kBitsPerSamples) {
      pcm_config.bitsPerSample = bits_per_sample;
      for (auto channel_mode : kChannelModes) {
        pcm_config.channelMode = channel_mode;
        audio_config.pcmConfig(pcm_config);
        clientif_->UpdateAudioConfig(audio_config);
        if (IsSoftwarePcmParametersSupported(pcm_config)) {
          EXPECT_EQ(clientif_->StartSession(), kClientIfReturnSuccess);
        } else {
          EXPECT_NE(clientif_->StartSession(), kClientIfReturnSuccess);
        }
        EXPECT_EQ(clientif_->EndSession(), kClientIfReturnSuccess);
      }  // ChannelMode
    }    // BitsPerSampple
  }      // SampleRate
}
