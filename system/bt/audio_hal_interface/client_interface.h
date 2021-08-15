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

#pragma once

#include <time.h>
#include <mutex>

#include <android/hardware/bluetooth/audio/2.0/IBluetoothAudioProvider.h>
#include <android/hardware/bluetooth/audio/2.0/types.h>
#include <fmq/MessageQueue.h>
#include <hardware/audio.h>

#include "common/message_loop_thread.h"

#define BLUETOOTH_AUDIO_HAL_PROP_DISABLED "persist.bluetooth.bluetooth_audio_hal.disabled"

namespace bluetooth {
namespace audio {

using ::android::hardware::bluetooth::audio::V2_0::AudioCapabilities;
using ::android::hardware::bluetooth::audio::V2_0::AudioConfiguration;
using ::android::hardware::bluetooth::audio::V2_0::BitsPerSample;
using ::android::hardware::bluetooth::audio::V2_0::ChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::CodecConfiguration;
using ::android::hardware::bluetooth::audio::V2_0::CodecType;
using ::android::hardware::bluetooth::audio::V2_0::IBluetoothAudioProvider;
using ::android::hardware::bluetooth::audio::V2_0::PcmParameters;
using ::android::hardware::bluetooth::audio::V2_0::SampleRate;
using ::android::hardware::bluetooth::audio::V2_0::SessionType;
using ::android::hardware::bluetooth::audio::V2_0::TimeSpec;
using BluetoothAudioStatus =
    ::android::hardware::bluetooth::audio::V2_0::Status;

enum class BluetoothAudioCtrlAck : uint8_t {
  SUCCESS_FINISHED = 0,
  PENDING,
  FAILURE_UNSUPPORTED,
  FAILURE_BUSY,
  FAILURE_DISCONNECTING,
  FAILURE
};

std::ostream& operator<<(std::ostream& os, const BluetoothAudioCtrlAck& ack);

inline BluetoothAudioStatus BluetoothAudioCtrlAckToHalStatus(
    const BluetoothAudioCtrlAck& ack) {
  switch (ack) {
    case BluetoothAudioCtrlAck::SUCCESS_FINISHED:
      return BluetoothAudioStatus::SUCCESS;
    case BluetoothAudioCtrlAck::FAILURE_UNSUPPORTED:
      return BluetoothAudioStatus::UNSUPPORTED_CODEC_CONFIGURATION;
    case BluetoothAudioCtrlAck::PENDING:
      return BluetoothAudioStatus::FAILURE;
    case BluetoothAudioCtrlAck::FAILURE_BUSY:
      return BluetoothAudioStatus::FAILURE;
    case BluetoothAudioCtrlAck::FAILURE_DISCONNECTING:
      return BluetoothAudioStatus::FAILURE;
    default:
      return BluetoothAudioStatus::FAILURE;
  }
}

// An IBluetoothTransportInstance needs to be implemented by a Bluetooth audio
// transport, such as A2DP or Hearing Aid, to handle callbacks from Audio HAL.
class IBluetoothTransportInstance {
 public:
  IBluetoothTransportInstance(SessionType sessionType,
                              AudioConfiguration audioConfig)
      : session_type_(sessionType), audio_config_(std::move(audioConfig)){};
  virtual ~IBluetoothTransportInstance() = default;

  SessionType GetSessionType() const { return session_type_; }

  AudioConfiguration GetAudioConfiguration() const { return audio_config_; }

  void UpdateAudioConfiguration(const AudioConfiguration& audio_config) {
    audio_config_ = audio_config;
  }

  virtual BluetoothAudioCtrlAck StartRequest() = 0;

  virtual BluetoothAudioCtrlAck SuspendRequest() = 0;

  virtual void StopRequest() = 0;

  virtual bool GetPresentationPosition(uint64_t* remote_delay_report_ns,
                                       uint64_t* total_bytes_readed,
                                       timespec* data_position) = 0;

  virtual void MetadataChanged(const source_metadata_t& source_metadata) = 0;

  // Invoked when the transport is requested to reset presentation position
  virtual void ResetPresentationPosition() = 0;

  // Invoked when the transport is requested to log bytes read
  virtual void LogBytesRead(size_t bytes_readed) = 0;

 private:
  const SessionType session_type_;
  AudioConfiguration audio_config_;
};

// common object is shared between different kind of SessionType
class BluetoothAudioDeathRecipient;

// The client interface connects an IBluetoothTransportInstance to
// IBluetoothAudioProvider and helps to route callbacks to
// IBluetoothTransportInstance
class BluetoothAudioClientInterface {
 public:
  // Constructs an BluetoothAudioClientInterface to communicate to
  // BluetoothAudio HAL. |sink| is the implementation for the transport, and
  // |message_loop| is the thread where callbacks are invoked.
  BluetoothAudioClientInterface(
      IBluetoothTransportInstance* sink,
      bluetooth::common::MessageLoopThread* message_loop);

  ~BluetoothAudioClientInterface();

  bool IsValid() const {
    return provider_ != nullptr;
  }

  std::vector<AudioCapabilities> GetAudioCapabilities() const;

  bool UpdateAudioConfig(const AudioConfiguration& audioConfig);

  int StartSession();

  void StreamStarted(const BluetoothAudioCtrlAck& ack);

  void StreamSuspended(const BluetoothAudioCtrlAck& ack);

  int EndSession();

  // Read data from audio  HAL through fmq
  size_t ReadAudioData(uint8_t* p_buf, uint32_t len);

  // Write data to audio HAL through fmq
  size_t WriteAudioData(uint8_t* p_buf, uint32_t len);

  // Renew the connection and usually is used when HIDL restarted
  void RenewAudioProviderAndSession();

  static constexpr PcmParameters kInvalidPcmConfiguration = {
      .sampleRate = SampleRate::RATE_UNKNOWN,
      .bitsPerSample = BitsPerSample::BITS_UNKNOWN,
      .channelMode = ChannelMode::UNKNOWN};

 private:
  // Helper function to connect to an IBluetoothAudioProvider
  void fetch_audio_provider();

  mutable std::mutex internal_mutex_;
  IBluetoothTransportInstance* sink_;
  android::sp<IBluetoothAudioProvider> provider_;
  std::vector<AudioCapabilities> capabilities_;
  bool session_started_;
  std::unique_ptr<::android::hardware::MessageQueue<
      uint8_t, ::android::hardware::kSynchronizedReadWrite>>
      mDataMQ;
  android::sp<BluetoothAudioDeathRecipient> death_recipient_;
};

}  // namespace audio
}  // namespace bluetooth
