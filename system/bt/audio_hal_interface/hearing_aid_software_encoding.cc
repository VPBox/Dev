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

#define LOG_TAG "BTAudioClientHearingAid"

#include "hearing_aid_software_encoding.h"
#include "client_interface.h"

#include "audio_hearing_aid_hw/include/audio_hearing_aid_hw.h"
#include "osi/include/log.h"
#include "osi/include/properties.h"

namespace {

using ::android::hardware::bluetooth::audio::V2_0::CodecType;
using ::bluetooth::audio::AudioConfiguration;
using ::bluetooth::audio::BitsPerSample;
using ::bluetooth::audio::BluetoothAudioCtrlAck;
using ::bluetooth::audio::ChannelMode;
using ::bluetooth::audio::PcmParameters;
using ::bluetooth::audio::SampleRate;
using ::bluetooth::audio::SessionType;
using ::bluetooth::audio::hearing_aid::StreamCallbacks;

// Transport implementation for Hearing Aids
class HearingAidTransport
    : public bluetooth::audio::IBluetoothTransportInstance {
 public:
  HearingAidTransport(StreamCallbacks stream_cb)
      : IBluetoothTransportInstance(
            SessionType::HEARING_AID_SOFTWARE_ENCODING_DATAPATH, {}),
        stream_cb_(std::move(stream_cb)),
        remote_delay_report_ms_(0),
        total_bytes_read_(0),
        data_position_({}){};

  BluetoothAudioCtrlAck StartRequest() override {
    LOG(INFO) << __func__;
    if (stream_cb_.on_resume_(true)) {
      return BluetoothAudioCtrlAck::SUCCESS_FINISHED;
    }
    return BluetoothAudioCtrlAck::FAILURE;
  }

  BluetoothAudioCtrlAck SuspendRequest() override {
    LOG(INFO) << __func__;
    if (stream_cb_.on_suspend_()) {
      uint8_t p_buf[AUDIO_STREAM_OUTPUT_BUFFER_SZ * 2];
      ::bluetooth::audio::hearing_aid::read(p_buf, sizeof(p_buf));
      return BluetoothAudioCtrlAck::SUCCESS_FINISHED;
    } else {
      return BluetoothAudioCtrlAck::FAILURE;
    }
  }

  void StopRequest() override {
    LOG(INFO) << __func__;
    if (stream_cb_.on_suspend_()) {
      // flush
      uint8_t p_buf[AUDIO_STREAM_OUTPUT_BUFFER_SZ * 2];
      ::bluetooth::audio::hearing_aid::read(p_buf, sizeof(p_buf));
    }
  }

  bool GetPresentationPosition(uint64_t* remote_delay_report_ns,
                               uint64_t* total_bytes_read,
                               timespec* data_position) override {
    VLOG(2) << __func__ << ": data=" << total_bytes_read_
            << " byte(s), timestamp=" << data_position_.tv_sec << "."
            << data_position_.tv_nsec
            << "s, delay report=" << remote_delay_report_ms_ << " msec.";
    if (remote_delay_report_ns != nullptr) {
      *remote_delay_report_ns = remote_delay_report_ms_ * 1000000u;
    }
    if (total_bytes_read != nullptr) *total_bytes_read = total_bytes_read_;
    if (data_position != nullptr) *data_position = data_position_;

    return true;
  }

  void MetadataChanged(const source_metadata_t& source_metadata) override {
    auto track_count = source_metadata.track_count;
    auto tracks = source_metadata.tracks;
    LOG(INFO) << __func__ << ": " << track_count << " track(s) received";
    while (track_count) {
      VLOG(1) << __func__ << ": usage=" << tracks->usage
              << ", content_type=" << tracks->content_type
              << ", gain=" << tracks->gain;
      --track_count;
      ++tracks;
    }
  }

  void ResetPresentationPosition() override {
    VLOG(2) << __func__ << ": called.";
    remote_delay_report_ms_ = 0;
    total_bytes_read_ = 0;
    data_position_ = {};
  }

  void LogBytesRead(size_t bytes_read) override {
    if (bytes_read) {
      total_bytes_read_ += bytes_read;
      clock_gettime(CLOCK_MONOTONIC, &data_position_);
    }
  }

  void SetRemoteDelay(uint16_t delay_report_ms) {
    LOG(INFO) << __func__ << ": delay_report=" << delay_report_ms << " msec";
    remote_delay_report_ms_ = delay_report_ms;
  }

 private:
  StreamCallbacks stream_cb_;
  uint16_t remote_delay_report_ms_;
  uint64_t total_bytes_read_;
  timespec data_position_;
};

bool HearingAidGetSelectedHalPcmConfig(PcmParameters* hal_pcm_config) {
  if (hal_pcm_config == nullptr) return false;
  // TODO: we only support one config for now!
  hal_pcm_config->sampleRate = SampleRate::RATE_16000;
  hal_pcm_config->bitsPerSample = BitsPerSample::BITS_16;
  hal_pcm_config->channelMode = ChannelMode::STEREO;
  return true;
}

// Sink instance of Hearing Aids to provide call-in APIs for Bluetooth Audio Hal
HearingAidTransport* hearing_aid_sink = nullptr;
// Common interface to call-out into Bluetooth Audio Hal
bluetooth::audio::BluetoothAudioClientInterface*
    hearing_aid_hal_clientinterface = nullptr;
bool btaudio_hearing_aid_disabled = false;
bool is_configured = false;

// Save the value if the remote reports its delay before hearing_aid_sink is
// initialized
uint16_t remote_delay_ms = 0;

bool is_hal_2_0_force_disabled() {
  if (!is_configured) {
    btaudio_hearing_aid_disabled = osi_property_get_bool(BLUETOOTH_AUDIO_HAL_PROP_DISABLED, false);
    is_configured = true;
  }
  return btaudio_hearing_aid_disabled;
}

}  // namespace

namespace bluetooth {
namespace audio {
namespace hearing_aid {

bool is_hal_2_0_enabled() { return hearing_aid_hal_clientinterface != nullptr; }

bool init(StreamCallbacks stream_cb,
          bluetooth::common::MessageLoopThread* message_loop) {
  LOG(INFO) << __func__;

  if (is_hal_2_0_force_disabled()) {
    LOG(ERROR) << __func__ << ": BluetoothAudio HAL is disabled";
    return false;
  }

  hearing_aid_sink = new HearingAidTransport(std::move(stream_cb));
  hearing_aid_hal_clientinterface =
      new bluetooth::audio::BluetoothAudioClientInterface(hearing_aid_sink,
                                                          message_loop);
  if (!hearing_aid_hal_clientinterface->IsValid()) {
    LOG(WARNING) << __func__ << ": BluetoothAudio HAL for Hearing Aid is invalid?!";
    delete hearing_aid_hal_clientinterface;
    hearing_aid_hal_clientinterface = nullptr;
    delete hearing_aid_sink;
    hearing_aid_sink = nullptr;
    return false;
  }

  if (remote_delay_ms != 0) {
    LOG(INFO) << __func__ << ": restore DELAY " << remote_delay_ms << " ms";
    hearing_aid_sink->SetRemoteDelay(remote_delay_ms);
    remote_delay_ms = 0;
  }

  return true;
}

void cleanup() {
  LOG(INFO) << __func__;
  if (!is_hal_2_0_enabled()) return;
  end_session();
  delete hearing_aid_hal_clientinterface;
  hearing_aid_hal_clientinterface = nullptr;
  delete hearing_aid_sink;
  hearing_aid_sink = nullptr;
  remote_delay_ms = 0;
}

void start_session() {
  LOG(INFO) << __func__;
  if (!is_hal_2_0_enabled()) return;
  AudioConfiguration audio_config;
  PcmParameters pcm_config{};
  if (!HearingAidGetSelectedHalPcmConfig(&pcm_config)) {
    LOG(ERROR) << __func__ << ": cannot get PCM config";
    return;
  }
  audio_config.pcmConfig(pcm_config);
  if (!hearing_aid_hal_clientinterface->UpdateAudioConfig(audio_config)) {
    LOG(ERROR) << __func__ << ": cannot update audio config to HAL";
    return;
  }
  hearing_aid_hal_clientinterface->StartSession();
}

void end_session() {
  LOG(INFO) << __func__;
  if (!is_hal_2_0_enabled()) return;
  hearing_aid_hal_clientinterface->EndSession();
}

size_t read(uint8_t* p_buf, uint32_t len) {
  if (!is_hal_2_0_enabled()) return 0;
  return hearing_aid_hal_clientinterface->ReadAudioData(p_buf, len);
}

// Update Hearing Aids delay report to BluetoothAudio HAL
void set_remote_delay(uint16_t delay_report_ms) {
  if (!is_hal_2_0_enabled()) {
    LOG(INFO) << __func__ << ":  not ready for DelayReport " << delay_report_ms
              << " ms";
    remote_delay_ms = delay_report_ms;
    return;
  }
  LOG(INFO) << __func__ << ": delay_report_ms=" << delay_report_ms << " ms";
  hearing_aid_sink->SetRemoteDelay(delay_report_ms);
}

}  // namespace hearing_aid
}  // namespace audio
}  // namespace bluetooth
