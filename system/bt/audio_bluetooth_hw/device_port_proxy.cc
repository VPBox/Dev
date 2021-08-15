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

#define LOG_TAG "BTAudioHalDeviceProxy"

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <audio_utils/primitives.h>
#include <inttypes.h>
#include <log/log.h>
#include <stdlib.h>

#include "BluetoothAudioSessionControl.h"
#include "device_port_proxy.h"
#include "stream_apis.h"
#include "utils.h"

namespace android {
namespace bluetooth {
namespace audio {

using ::android::base::StringPrintf;
using ::android::bluetooth::audio::BluetoothAudioSessionControl;
using ::android::hardware::bluetooth::audio::V2_0::BitsPerSample;
using ::android::hardware::bluetooth::audio::V2_0::ChannelMode;
using ::android::hardware::bluetooth::audio::V2_0::PcmParameters;
using ::android::hardware::bluetooth::audio::V2_0::SampleRate;
using ::android::hardware::bluetooth::audio::V2_0::SessionType;
using BluetoothAudioStatus =
    ::android::hardware::bluetooth::audio::V2_0::Status;
using ControlResultCallback = std::function<void(
    uint16_t cookie, bool start_resp, const BluetoothAudioStatus& status)>;
using SessionChangedCallback = std::function<void(uint16_t cookie)>;

namespace {

unsigned int SampleRateToAudioFormat(SampleRate sample_rate) {
  switch (sample_rate) {
    case SampleRate::RATE_16000:
      return 16000;
    case SampleRate::RATE_24000:
      return 24000;
    case SampleRate::RATE_44100:
      return 44100;
    case SampleRate::RATE_48000:
      return 48000;
    case SampleRate::RATE_88200:
      return 88200;
    case SampleRate::RATE_96000:
      return 96000;
    case SampleRate::RATE_176400:
      return 176400;
    case SampleRate::RATE_192000:
      return 192000;
    default:
      return kBluetoothDefaultSampleRate;
  }
}
audio_channel_mask_t ChannelModeToAudioFormat(ChannelMode channel_mode) {
  switch (channel_mode) {
    case ChannelMode::MONO:
      return AUDIO_CHANNEL_OUT_MONO;
    case ChannelMode::STEREO:
      return AUDIO_CHANNEL_OUT_STEREO;
    default:
      return kBluetoothDefaultOutputChannelModeMask;
  }
}

audio_format_t BitsPerSampleToAudioFormat(BitsPerSample bits_per_sample) {
  switch (bits_per_sample) {
    case BitsPerSample::BITS_16:
      return AUDIO_FORMAT_PCM_16_BIT;
    case BitsPerSample::BITS_24:
      return AUDIO_FORMAT_PCM_24_BIT_PACKED;
    case BitsPerSample::BITS_32:
      return AUDIO_FORMAT_PCM_32_BIT;
    default:
      return kBluetoothDefaultAudioFormatBitsPerSample;
  }
}

// The maximum time to wait in std::condition_variable::wait_for()
constexpr unsigned int kMaxWaitingTimeMs = 4500;

}  // namespace

BluetoothAudioPortOut::BluetoothAudioPortOut()
    : state_(BluetoothStreamState::DISABLED),
      session_type_(SessionType::UNKNOWN),
      cookie_(android::bluetooth::audio::kObserversCookieUndefined) {}

bool BluetoothAudioPortOut::SetUp(audio_devices_t devices) {
  if (!init_session_type(devices)) return false;

  state_ = BluetoothStreamState::STANDBY;

  auto control_result_cb = [port = this](uint16_t cookie, bool start_resp,
                                         const BluetoothAudioStatus& status) {
    if (!port->in_use()) {
      LOG(ERROR) << "control_result_cb: BluetoothAudioPortOut is not in use";
      return;
    }
    if (port->cookie_ != cookie) {
      LOG(ERROR) << "control_result_cb: proxy of device port (cookie=" << StringPrintf("%#hx", cookie)
                 << ") is corrupted";
      return;
    }
    port->ControlResultHandler(status);
  };
  auto session_changed_cb = [port = this](uint16_t cookie) {
    if (!port->in_use()) {
      LOG(ERROR) << "session_changed_cb: BluetoothAudioPortOut is not in use";
      return;
    }
    if (port->cookie_ != cookie) {
      LOG(ERROR) << "session_changed_cb: proxy of device port (cookie=" << StringPrintf("%#hx", cookie)
                 << ") is corrupted";
      return;
    }
    port->SessionChangedHandler();
  };
  ::android::bluetooth::audio::PortStatusCallbacks cbacks = {
      .control_result_cb_ = control_result_cb,
      .session_changed_cb_ = session_changed_cb};
  cookie_ = BluetoothAudioSessionControl::RegisterControlResultCback(
      session_type_, cbacks);
  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_);

  return (cookie_ != android::bluetooth::audio::kObserversCookieUndefined);
}

bool BluetoothAudioPortOut::init_session_type(audio_devices_t device) {
  switch (device) {
    case AUDIO_DEVICE_OUT_BLUETOOTH_A2DP:
    case AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES:
    case AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER:
      LOG(VERBOSE) << __func__ << ": device=AUDIO_DEVICE_OUT_BLUETOOTH_A2DP (HEADPHONES/SPEAKER) ("
                   << StringPrintf("%#x", device) << ")";
      session_type_ = SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH;
      break;
    case AUDIO_DEVICE_OUT_HEARING_AID:
      LOG(VERBOSE) << __func__ << ": device=AUDIO_DEVICE_OUT_HEARING_AID (MEDIA/VOICE) (" << StringPrintf("%#x", device)
                   << ")";
      session_type_ = SessionType::HEARING_AID_SOFTWARE_ENCODING_DATAPATH;
      break;
    default:
      LOG(ERROR) << __func__ << ": unknown device=" << StringPrintf("%#x", device);
      return false;
  }

  if (!BluetoothAudioSessionControl::IsSessionReady(session_type_)) {
    LOG(ERROR) << __func__ << ": device=" << StringPrintf("%#x", device) << ", session_type=" << toString(session_type_)
               << " is not ready";
    return false;
  }
  return true;
}

void BluetoothAudioPortOut::TearDown() {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": session_type=" << toString(session_type_)
               << ", cookie=" << StringPrintf("%#hx", cookie_) << " unknown monitor";
    return;
  }

  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_);
  BluetoothAudioSessionControl::UnregisterControlResultCback(session_type_,
                                                             cookie_);
  cookie_ = android::bluetooth::audio::kObserversCookieUndefined;
}

void BluetoothAudioPortOut::ControlResultHandler(
    const BluetoothAudioStatus& status) {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return;
  }
  std::unique_lock<std::mutex> port_lock(cv_mutex_);
  BluetoothStreamState previous_state = state_;
  LOG(INFO) << "control_result_cb: session_type=" << toString(session_type_)
            << ", cookie=" << StringPrintf("%#hx", cookie_) << ", previous_state=" << previous_state
            << ", status=" << toString(status);

  switch (previous_state) {
    case BluetoothStreamState::STARTING:
      if (status == BluetoothAudioStatus::SUCCESS) {
        state_ = BluetoothStreamState::STARTED;
      } else {
        // Set to standby since the stack may be busy switching between outputs
        LOG(WARNING) << "control_result_cb: status=" << toString(status)
                     << " failure for session_type=" << toString(session_type_)
                     << ", cookie=" << StringPrintf("%#hx", cookie_) << ", previous_state=" << previous_state;
        state_ = BluetoothStreamState::STANDBY;
      }
      break;
    case BluetoothStreamState::SUSPENDING:
      if (status == BluetoothAudioStatus::SUCCESS) {
        state_ = BluetoothStreamState::STANDBY;
      } else {
        // It will be failed if the headset is disconnecting, and set to disable
        // to wait for re-init again
        LOG(WARNING) << "control_result_cb: status=" << toString(status)
                     << " failure for session_type=" << toString(session_type_)
                     << ", cookie=" << StringPrintf("%#hx", cookie_) << ", previous_state=" << previous_state;
        state_ = BluetoothStreamState::DISABLED;
      }
      break;
    default:
      LOG(ERROR) << "control_result_cb: unexpected status=" << toString(status)
                 << " for session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
                 << ", previous_state=" << previous_state;
      return;
  }
  port_lock.unlock();
  internal_cv_.notify_all();
}

void BluetoothAudioPortOut::SessionChangedHandler() {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return;
  }
  std::unique_lock<std::mutex> port_lock(cv_mutex_);
  BluetoothStreamState previous_state = state_;
  LOG(INFO) << "session_changed_cb: session_type=" << toString(session_type_)
            << ", cookie=" << StringPrintf("%#hx", cookie_) << ", previous_state=" << previous_state;
  if (previous_state != BluetoothStreamState::DISABLED) {
    state_ = BluetoothStreamState::DISABLED;
  } else {
    state_ = BluetoothStreamState::STANDBY;
  }
  port_lock.unlock();
  internal_cv_.notify_all();
}

bool BluetoothAudioPortOut::in_use() const {
  return (cookie_ != android::bluetooth::audio::kObserversCookieUndefined);
}

bool BluetoothAudioPortOut::LoadAudioConfig(audio_config_t* audio_cfg) const {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    audio_cfg->sample_rate = kBluetoothDefaultSampleRate;
    audio_cfg->channel_mask = kBluetoothDefaultOutputChannelModeMask;
    audio_cfg->format = kBluetoothDefaultAudioFormatBitsPerSample;
    return false;
  }

  const AudioConfiguration& hal_audio_cfg =
      BluetoothAudioSessionControl::GetAudioConfig(session_type_);
  if (hal_audio_cfg.getDiscriminator() !=
      AudioConfiguration::hidl_discriminator::pcmConfig) {
    audio_cfg->sample_rate = kBluetoothDefaultSampleRate;
    audio_cfg->channel_mask = kBluetoothDefaultOutputChannelModeMask;
    audio_cfg->format = kBluetoothDefaultAudioFormatBitsPerSample;
    return false;
  }
  const PcmParameters& pcm_cfg = hal_audio_cfg.pcmConfig();
  LOG(VERBOSE) << __func__ << ": session_type=" << toString(session_type_)
               << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << ", PcmConfig=["
               << toString(pcm_cfg) << "]";
  if (pcm_cfg.sampleRate == SampleRate::RATE_UNKNOWN ||
      pcm_cfg.channelMode == ChannelMode::UNKNOWN ||
      pcm_cfg.bitsPerSample == BitsPerSample::BITS_UNKNOWN) {
    return false;
  }
  audio_cfg->sample_rate = SampleRateToAudioFormat(pcm_cfg.sampleRate);
  audio_cfg->channel_mask =
      (is_stereo_to_mono_ ? AUDIO_CHANNEL_OUT_STEREO : ChannelModeToAudioFormat(pcm_cfg.channelMode));
  audio_cfg->format = BitsPerSampleToAudioFormat(pcm_cfg.bitsPerSample);
  return true;
}

bool BluetoothAudioPortOut::CondwaitState(BluetoothStreamState state) {
  bool retval;
  std::unique_lock<std::mutex> port_lock(cv_mutex_);
  switch (state) {
    case BluetoothStreamState::STARTING:
      LOG(VERBOSE) << __func__ << ": session_type=" << toString(session_type_)
                   << ", cookie=" << StringPrintf("%#hx", cookie_) << " waiting for STARTED";
      retval = internal_cv_.wait_for(
          port_lock, std::chrono::milliseconds(kMaxWaitingTimeMs),
          [this] { return this->state_ != BluetoothStreamState::STARTING; });
      retval = retval && state_ == BluetoothStreamState::STARTED;
      break;
    case BluetoothStreamState::SUSPENDING:
      LOG(VERBOSE) << __func__ << ": session_type=" << toString(session_type_)
                   << ", cookie=" << StringPrintf("%#hx", cookie_) << " waiting for SUSPENDED";
      retval = internal_cv_.wait_for(
          port_lock, std::chrono::milliseconds(kMaxWaitingTimeMs),
          [this] { return this->state_ != BluetoothStreamState::SUSPENDING; });
      retval = retval && state_ == BluetoothStreamState::STANDBY;
      break;
    default:
      LOG(WARNING) << __func__ << ": session_type=" << toString(session_type_)
                   << ", cookie=" << StringPrintf("%#hx", cookie_) << " waiting for KNOWN";
      return false;
  }

  return retval;  // false if any failure like timeout
}

bool BluetoothAudioPortOut::Start() {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return false;
  }

  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
            << ", state=" << state_ << ", mono=" << (is_stereo_to_mono_ ? "true" : "false") << " request";
  bool retval = false;
  if (state_ == BluetoothStreamState::STANDBY) {
    state_ = BluetoothStreamState::STARTING;
    if (BluetoothAudioSessionControl::StartStream(session_type_)) {
      retval = CondwaitState(BluetoothStreamState::STARTING);
    } else {
      LOG(ERROR) << __func__ << ": session_type=" << toString(session_type_)
                 << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << " Hal fails";
    }
  }

  if (retval) {
    LOG(INFO) << __func__ << ": session_type=" << toString(session_type_)
              << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_
              << ", mono=" << (is_stereo_to_mono_ ? "true" : "false") << " done";
  } else {
    LOG(ERROR) << __func__ << ": session_type=" << toString(session_type_)
               << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << " failure";
  }

  return retval;  // false if any failure like timeout
}

bool BluetoothAudioPortOut::Suspend() {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return false;
  }

  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
            << ", state=" << state_ << " request";
  bool retval = false;
  if (state_ == BluetoothStreamState::STARTED) {
    state_ = BluetoothStreamState::SUSPENDING;
    if (BluetoothAudioSessionControl::SuspendStream(session_type_)) {
      retval = CondwaitState(BluetoothStreamState::SUSPENDING);
    } else {
      LOG(ERROR) << __func__ << ": session_type=" << toString(session_type_)
                 << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << " Hal fails";
    }
  }

  if (retval) {
    LOG(INFO) << __func__ << ": session_type=" << toString(session_type_)
              << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << " done";
  } else {
    LOG(ERROR) << __func__ << ": session_type=" << toString(session_type_)
               << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << " failure";
  }

  return retval;  // false if any failure like timeout
}

void BluetoothAudioPortOut::Stop() {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return;
  }
  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
            << ", state=" << state_ << " request";
  state_ = BluetoothStreamState::DISABLED;
  BluetoothAudioSessionControl::StopStream(session_type_);
  LOG(INFO) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
            << ", state=" << state_ << " done";
}

size_t BluetoothAudioPortOut::WriteData(const void* buffer, size_t bytes) const {
  if (!in_use()) return 0;
  if (!is_stereo_to_mono_) {
    return BluetoothAudioSessionControl::OutWritePcmData(session_type_, buffer, bytes);
  }

  // WAR to mix the stereo into Mono (16 bits per sample)
  const size_t write_frames = bytes >> 2;
  if (write_frames == 0) return 0;
  auto src = static_cast<const int16_t*>(buffer);
  std::unique_ptr<int16_t[]> dst{new int16_t[write_frames]};
  downmix_to_mono_i16_from_stereo_i16(dst.get(), src, write_frames);
  // a frame is 16 bits, and the size of a mono frame is equal to half a stereo.
  return BluetoothAudioSessionControl::OutWritePcmData(session_type_, dst.get(), write_frames * 2) * 2;
}

bool BluetoothAudioPortOut::GetPresentationPosition(uint64_t* delay_ns,
                                                    uint64_t* bytes,
                                                    timespec* timestamp) const {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return false;
  }
  bool retval = BluetoothAudioSessionControl::GetPresentationPosition(
      session_type_, delay_ns, bytes, timestamp);
  LOG(VERBOSE) << __func__ << ": session_type=" << StringPrintf("%#hhx", session_type_)
               << ", cookie=" << StringPrintf("%#hx", cookie_) << ", state=" << state_ << ", delay=" << *delay_ns
               << "ns, data=" << *bytes << " bytes, timestamp=" << timestamp->tv_sec << "."
               << StringPrintf("%09ld", timestamp->tv_nsec) << "s";

  return retval;
}

void BluetoothAudioPortOut::UpdateMetadata(
    const source_metadata* source_metadata) const {
  if (!in_use()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioPortOut is not in use";
    return;
  }
  LOG(DEBUG) << __func__ << ": session_type=" << toString(session_type_) << ", cookie=" << StringPrintf("%#hx", cookie_)
             << ", state=" << state_ << ", " << source_metadata->track_count << " track(s)";
  if (source_metadata->track_count == 0) return;
  BluetoothAudioSessionControl::UpdateTracksMetadata(session_type_,
                                                     source_metadata);
}

BluetoothStreamState BluetoothAudioPortOut::GetState() const { return state_; }

void BluetoothAudioPortOut::SetState(BluetoothStreamState state) {
  state_ = state;
}

}  // namespace audio
}  // namespace bluetooth
}  // namespace android
