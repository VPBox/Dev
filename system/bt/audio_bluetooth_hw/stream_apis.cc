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

#define LOG_TAG "BTAudioHalStream"

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <cutils/properties.h>
#include <errno.h>
#include <inttypes.h>
#include <log/log.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "stream_apis.h"
#include "utils.h"

using ::android::base::StringPrintf;
using ::android::bluetooth::audio::BluetoothAudioPortOut;
using ::android::bluetooth::audio::utils::GetAudioParamString;
using ::android::bluetooth::audio::utils::ParseAudioParams;

namespace {

constexpr unsigned int kMinimumDelayMs = 100;
constexpr unsigned int kMaximumDelayMs = 1000;
constexpr int kExtraAudioSyncMs = 200;

std::ostream& operator<<(std::ostream& os, const audio_config& config) {
  return os << "audio_config[sample_rate=" << config.sample_rate
            << ", channels=" << StringPrintf("%#x", config.channel_mask) << ", format=" << config.format << "]";
}

}  // namespace

std::ostream& operator<<(std::ostream& os, const BluetoothStreamState& state) {
  switch (state) {
    case BluetoothStreamState::DISABLED:
      return os << "DISABLED";
    case BluetoothStreamState::STANDBY:
      return os << "STANDBY";
    case BluetoothStreamState::STARTING:
      return os << "STARTING";
    case BluetoothStreamState::STARTED:
      return os << "STARTED";
    case BluetoothStreamState::SUSPENDING:
      return os << "SUSPENDING";
    case BluetoothStreamState::UNKNOWN:
      return os << "UNKNOWN";
    default:
      return os << StringPrintf("%#hhx", state);
  }
}

static uint32_t out_get_sample_rate(const struct audio_stream* stream) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  audio_config_t audio_cfg;
  if (out->bluetooth_output_.LoadAudioConfig(&audio_cfg)) {
    LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " audio_cfg=" << audio_cfg;
    return audio_cfg.sample_rate;
  } else {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << ", sample_rate=" << out->sample_rate_ << " failed";
    return out->sample_rate_;
  }
}

static int out_set_sample_rate(struct audio_stream* stream, uint32_t rate) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", sample_rate=" << out->sample_rate_;
  return (rate == out->sample_rate_ ? 0 : -1);
}

static size_t out_get_buffer_size(const struct audio_stream* stream) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  size_t buffer_size =
      out->frames_count_ * audio_stream_out_frame_size(&out->stream_out_);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", buffer_size=" << buffer_size;
  return buffer_size;
}

static audio_channel_mask_t out_get_channels(
    const struct audio_stream* stream) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  audio_config_t audio_cfg;
  if (out->bluetooth_output_.LoadAudioConfig(&audio_cfg)) {
    LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " audio_cfg=" << audio_cfg;
    return audio_cfg.channel_mask;
  } else {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << ", channels=" << StringPrintf("%#x", out->channel_mask_) << " failure";
    return out->channel_mask_;
  }
}

static audio_format_t out_get_format(const struct audio_stream* stream) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  audio_config_t audio_cfg;
  if (out->bluetooth_output_.LoadAudioConfig(&audio_cfg)) {
    LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " audio_cfg=" << audio_cfg;
    return audio_cfg.format;
  } else {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << ", format=" << out->format_ << " failure";
    return out->format_;
  }
}

static int out_set_format(struct audio_stream* stream, audio_format_t format) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", format=" << out->format_;
  return (format == out->format_ ? 0 : -1);
}

static int out_standby(struct audio_stream* stream) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  int retval = 0;

  // out->last_write_time_us_ = 0; unnecessary as a stale write time has same
  // effect
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " being standby (suspend)";
  if (out->bluetooth_output_.GetState() == BluetoothStreamState::STARTED) {
    out->frames_rendered_ = 0;
    retval = (out->bluetooth_output_.Suspend() ? 0 : -EIO);
  } else if (out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::STARTING ||
             out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::SUSPENDING) {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " NOT ready to be standby";
    retval = -EBUSY;
  } else {
    LOG(DEBUG) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " standby already";
  }
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " standby (suspend) retval=" << retval;

  return retval;
}

static int out_dump(const struct audio_stream* stream, int fd) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState();
  return 0;
}

static int out_set_parameters(struct audio_stream* stream,
                              const char* kvpairs) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  int retval = 0;

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", kvpairs=[" << kvpairs << "]";

  std::unordered_map<std::string, std::string> params =
      ParseAudioParams(kvpairs);
  if (params.empty()) return retval;

  LOG(VERBOSE) << __func__ << ": ParamsMap=[" << GetAudioParamString(params)
               << "]";

  audio_config_t audio_cfg;
  if (params.find(AUDIO_PARAMETER_STREAM_SUP_SAMPLING_RATES) != params.end() ||
      params.find(AUDIO_PARAMETER_STREAM_SUP_CHANNELS) != params.end() ||
      params.find(AUDIO_PARAMETER_STREAM_SUP_FORMATS) != params.end()) {
    if (out->bluetooth_output_.LoadAudioConfig(&audio_cfg)) {
      out->sample_rate_ = audio_cfg.sample_rate;
      out->channel_mask_ = audio_cfg.channel_mask;
      out->format_ = audio_cfg.format;
      LOG(VERBOSE) << "state=" << out->bluetooth_output_.GetState() << ", sample_rate=" << out->sample_rate_
                   << ", channels=" << StringPrintf("%#x", out->channel_mask_) << ", format=" << out->format_;
    } else {
      LOG(WARNING) << __func__
                   << ": state=" << out->bluetooth_output_.GetState()
                   << " failed to get audio config";
    }
  }

  if (params.find("routing") != params.end()) {
    auto routing_param = params.find("routing");
    LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
              << ", stream param '" << routing_param->first.c_str() << "="
              << routing_param->second.c_str() << "'";
  }

  if (params.find("A2dpSuspended") != params.end()) {
    if (params["A2dpSuspended"] == "true") {
      LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                << " stream param stopped";
      if (out->bluetooth_output_.GetState() != BluetoothStreamState::DISABLED) {
        out->frames_rendered_ = 0;
        out->bluetooth_output_.Stop();
      }
    } else {
      LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                << " stream param standby";
      if (out->bluetooth_output_.GetState() == BluetoothStreamState::DISABLED) {
        out->bluetooth_output_.SetState(BluetoothStreamState::STANDBY);
      }
    }
  }

  if (params.find("closing") != params.end()) {
    if (params["closing"] == "true") {
      LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                << " stream param closing, disallow any writes?";
      if (out->bluetooth_output_.GetState() != BluetoothStreamState::DISABLED) {
        out->frames_rendered_ = 0;
        out->frames_presented_ = 0;
        out->bluetooth_output_.Stop();
      }
    }
  }

  if (params.find("exiting") != params.end()) {
    if (params["exiting"] == "1") {
      LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                << " stream param exiting";
      if (out->bluetooth_output_.GetState() != BluetoothStreamState::DISABLED) {
        out->frames_rendered_ = 0;
        out->frames_presented_ = 0;
        out->bluetooth_output_.Stop();
      }
    }
  }

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", kvpairs=[" << kvpairs << "], retval=" << retval;
  return retval;
}

static char* out_get_parameters(const struct audio_stream* stream,
                                const char* keys) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", keys=[" << keys << "]";

  std::unordered_map<std::string, std::string> params = ParseAudioParams(keys);
  if (params.empty()) return strdup("");

  audio_config_t audio_cfg;
  if (out->bluetooth_output_.LoadAudioConfig(&audio_cfg)) {
    LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " audio_cfg=" << audio_cfg;
  } else {
    LOG(ERROR) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " failed to get audio config";
  }

  std::unordered_map<std::string, std::string> return_params;
  if (params.find(AUDIO_PARAMETER_STREAM_SUP_SAMPLING_RATES) != params.end()) {
    std::string param;
    if (audio_cfg.sample_rate == 16000) {
      param = "16000";
    }
    if (audio_cfg.sample_rate == 24000) {
      param = "24000";
    }
    if (audio_cfg.sample_rate == 44100) {
      param = "44100";
    }
    if (audio_cfg.sample_rate == 48000) {
      param = "48000";
    }
    if (audio_cfg.sample_rate == 88200) {
      param = "88200";
    }
    if (audio_cfg.sample_rate == 96000) {
      param = "96000";
    }
    if (audio_cfg.sample_rate == 176400) {
      param = "176400";
    }
    if (audio_cfg.sample_rate == 192000) {
      param = "192000";
    }
    return_params[AUDIO_PARAMETER_STREAM_SUP_SAMPLING_RATES] = param;
  }

  if (params.find(AUDIO_PARAMETER_STREAM_SUP_CHANNELS) != params.end()) {
    std::string param;
    if (audio_cfg.channel_mask == AUDIO_CHANNEL_OUT_MONO) {
      param = "AUDIO_CHANNEL_OUT_MONO";
    }
    if (audio_cfg.channel_mask == AUDIO_CHANNEL_OUT_STEREO) {
      param = "AUDIO_CHANNEL_OUT_STEREO";
    }
    return_params[AUDIO_PARAMETER_STREAM_SUP_CHANNELS] = param;
  }

  if (params.find(AUDIO_PARAMETER_STREAM_SUP_FORMATS) != params.end()) {
    std::string param;
    if (audio_cfg.format == AUDIO_FORMAT_PCM_16_BIT) {
      param = "AUDIO_FORMAT_PCM_16_BIT";
    }
    if (audio_cfg.format == AUDIO_FORMAT_PCM_24_BIT_PACKED) {
      param = "AUDIO_FORMAT_PCM_24_BIT_PACKED";
    }
    if (audio_cfg.format == AUDIO_FORMAT_PCM_32_BIT) {
      param = "AUDIO_FORMAT_PCM_32_BIT";
    }
    return_params[AUDIO_PARAMETER_STREAM_SUP_FORMATS] = param;
  }

  std::string result;
  for (const auto& ptr : return_params) {
    result += ptr.first + "=" + ptr.second + ";";
  }

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", result=[" << result << "]";
  return strdup(result.c_str());
}

static uint32_t out_get_latency_ms(const struct audio_stream_out* stream) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  /***
   * audio_a2dp_hw:
   *   frames_count = buffer_size / frame_size
   *   latency (sec.) = frames_count / sample_rate
   */
  uint32_t latency_ms = out->frames_count_ * 1000 / out->sample_rate_;
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", latency_ms=" << latency_ms;
  // Sync from audio_a2dp_hw to add extra +200ms
  return latency_ms + kExtraAudioSyncMs;
}

static int out_set_volume(struct audio_stream_out* stream, float left,
                          float right) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", Left=" << left << ", Right=" << right;
  return -1;
}

static ssize_t out_write(struct audio_stream_out* stream, const void* buffer,
                         size_t bytes) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  size_t totalWritten = 0;

  if (out->bluetooth_output_.GetState() != BluetoothStreamState::STARTED) {
    LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState()
              << " first time bytes=" << bytes;
    lock.unlock();
    if (stream->resume(stream)) {
      LOG(ERROR) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " failed to resume";
      usleep(kBluetoothDefaultOutputBufferMs * 1000);
      return totalWritten;
    }
    lock.lock();
  }
  lock.unlock();
  totalWritten = out->bluetooth_output_.WriteData(buffer, bytes);
  lock.lock();

  struct timespec ts = {.tv_sec = 0, .tv_nsec = 0};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  if (totalWritten) {
    const size_t frames = bytes / audio_stream_out_frame_size(stream);
    out->frames_rendered_ += frames;
    out->frames_presented_ += frames;
    out->last_write_time_us_ = (ts.tv_sec * 1000000000LL + ts.tv_nsec) / 1000;
  } else {
    const int64_t now = (ts.tv_sec * 1000000000LL + ts.tv_nsec) / 1000;
    const int64_t elapsed_time_since_last_write =
        now - out->last_write_time_us_;
    // frames_count = written_data / frame_size
    // play_time (ms) = frames_count / (sample_rate (Sec.) / 1000000)
    // sleep_time (ms) = play_time - elapsed_time
    int64_t sleep_time = bytes * 1000000LL /
                             audio_stream_out_frame_size(stream) /
                             out_get_sample_rate(&stream->common) -
                         elapsed_time_since_last_write;
    if (sleep_time > 0) {
      LOG(VERBOSE) << __func__ << ": sleep " << (sleep_time / 1000)
                   << " ms when writting FMQ datapath";
      lock.unlock();
      usleep(sleep_time);
      lock.lock();
    } else {
      // we don't sleep when we exit standby (this is typical for a real alsa
      // buffer).
      sleep_time = 0;
    }
    out->last_write_time_us_ = now + sleep_time;
  }
  return totalWritten;
}

static int out_get_render_position(const struct audio_stream_out* stream,
                                   uint32_t* dsp_frames) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);

  if (dsp_frames == nullptr) return -EINVAL;

  /* frames = (latency (ms) / 1000) * sample_per_seconds */
  uint64_t latency_frames =
      (uint64_t)out_get_latency_ms(stream) * out->sample_rate_ / 1000;
  if (out->frames_rendered_ >= latency_frames) {
    *dsp_frames = (uint32_t)(out->frames_rendered_ - latency_frames);
  } else {
    *dsp_frames = 0;
  }

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", dsp_frames=" << *dsp_frames;
  return 0;
}

static int out_add_audio_effect(const struct audio_stream* stream,
                                effect_handle_t effect) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", effect=" << effect;
  return 0;
}

static int out_remove_audio_effect(const struct audio_stream* stream,
                                   effect_handle_t effect) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", effect=" << effect;
  return 0;
}

static int out_get_next_write_timestamp(const struct audio_stream_out* stream,
                                        int64_t* timestamp) {
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  *timestamp = 0;
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", timestamp=" << *timestamp;
  return -EINVAL;
}

static int out_pause(struct audio_stream_out* stream) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  int retval = 0;
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", pausing (suspend)";
  if (out->bluetooth_output_.GetState() == BluetoothStreamState::STARTED) {
    out->frames_rendered_ = 0;
    retval = (out->bluetooth_output_.Suspend() ? 0 : -EIO);
  } else if (out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::STARTING ||
             out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::SUSPENDING) {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " NOT ready to pause?!";
    retval = -EBUSY;
  } else {
    LOG(DEBUG) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " paused already";
  }
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", pausing (suspend) retval=" << retval;

  return retval;
}

static int out_resume(struct audio_stream_out* stream) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  int retval = 0;

  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", resuming (start)";
  if (out->bluetooth_output_.GetState() == BluetoothStreamState::STANDBY) {
    retval = (out->bluetooth_output_.Start() ? 0 : -EIO);
  } else if (out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::STARTING ||
             out->bluetooth_output_.GetState() ==
                 BluetoothStreamState::SUSPENDING) {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " NOT ready to resume?!";
    retval = -EBUSY;
  } else if (out->bluetooth_output_.GetState() ==
             BluetoothStreamState::DISABLED) {
    LOG(WARNING) << __func__ << ": state=" << out->bluetooth_output_.GetState()
                 << " NOT allow to resume?!";
    retval = -EINVAL;
  } else {
    LOG(DEBUG) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " resumed already";
  }
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", resuming (start) retval=" << retval;

  return retval;
}

static int out_get_presentation_position(const struct audio_stream_out* stream,
                                         uint64_t* frames,
                                         struct timespec* timestamp) {
  if (frames == nullptr || timestamp == nullptr) {
    return -EINVAL;
  }

  // bytes is the total number of bytes sent by the Bluetooth stack to a
  // remote headset
  uint64_t bytes = 0;
  // delay_report is the audio delay from the remote headset receiving data to
  // the headset playing sound in units of nanoseconds
  uint64_t delay_report_ns = 0;
  const auto* out = reinterpret_cast<const BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);

  if (out->bluetooth_output_.GetPresentationPosition(&delay_report_ns, &bytes,
                                                     timestamp)) {
    // assume kMinimumDelayMs (100ms) < delay_report_ns < kMaximumDelayMs
    // (1000ms), or it is invalid / ignored and use old delay calculated
    // by ourselves.
    if (delay_report_ns > kMinimumDelayMs * 1000000 &&
        delay_report_ns < kMaximumDelayMs * 1000000) {
      *frames = bytes / audio_stream_out_frame_size(stream);
      timestamp->tv_nsec += delay_report_ns;
      if (timestamp->tv_nsec > 1000000000) {
        timestamp->tv_sec += static_cast<int>(timestamp->tv_nsec / 1000000000);
        timestamp->tv_nsec %= 1000000000;
      }
      LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState() << ", frames=" << *frames << " ("
                   << bytes << " bytes), timestamp=" << timestamp->tv_sec << "."
                   << StringPrintf("%09ld", timestamp->tv_nsec) << "s";
      return 0;
    } else if (delay_report_ns >= kMaximumDelayMs * 1000000) {
      LOG(WARNING) << __func__
                   << ": state=" << out->bluetooth_output_.GetState()
                   << ", delay_report=" << delay_report_ns << "ns abnormal";
    }
  }

  // default to old delay if any failure is found when fetching from ports
  if (out->frames_presented_ >= out->frames_count_) {
    clock_gettime(CLOCK_MONOTONIC, timestamp);
    *frames = out->frames_presented_ - out->frames_count_;
    LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState() << ", frames=" << *frames << " ("
                 << bytes << " bytes), timestamp=" << timestamp->tv_sec << "."
                 << StringPrintf("%09ld", timestamp->tv_nsec) << "s";
    return 0;
  }

  *frames = 0;
  *timestamp = {};
  return -EWOULDBLOCK;
}

static void out_update_source_metadata(
    struct audio_stream_out* stream,
    const struct source_metadata* source_metadata) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  std::unique_lock<std::mutex> lock(out->mutex_);
  if (source_metadata == nullptr || source_metadata->track_count == 0) {
    return;
  }
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", " << source_metadata->track_count << " track(s)";
  out->bluetooth_output_.UpdateMetadata(source_metadata);
}

static size_t samples_per_ticks(size_t milliseconds, uint32_t sample_rate,
                                size_t channel_count) {
  return milliseconds * sample_rate * channel_count / 1000;
}

int adev_open_output_stream(struct audio_hw_device* dev,
                            audio_io_handle_t handle, audio_devices_t devices,
                            audio_output_flags_t flags,
                            struct audio_config* config,
                            struct audio_stream_out** stream_out,
                            const char* address __unused) {
  *stream_out = nullptr;
  auto* out = new BluetoothStreamOut;
  if (!out->bluetooth_output_.SetUp(devices)) {
    delete out;
    return -EINVAL;
  }
  LOG(VERBOSE) << __func__ << ": device=" << StringPrintf("%#x", devices);

  out->stream_out_.common.get_sample_rate = out_get_sample_rate;
  out->stream_out_.common.set_sample_rate = out_set_sample_rate;
  out->stream_out_.common.get_buffer_size = out_get_buffer_size;
  out->stream_out_.common.get_channels = out_get_channels;
  out->stream_out_.common.get_format = out_get_format;
  out->stream_out_.common.set_format = out_set_format;
  out->stream_out_.common.standby = out_standby;
  out->stream_out_.common.dump = out_dump;
  out->stream_out_.common.set_parameters = out_set_parameters;
  out->stream_out_.common.get_parameters = out_get_parameters;
  out->stream_out_.common.add_audio_effect = out_add_audio_effect;
  out->stream_out_.common.remove_audio_effect = out_remove_audio_effect;
  out->stream_out_.get_latency = out_get_latency_ms;
  out->stream_out_.set_volume = out_set_volume;
  out->stream_out_.write = out_write;
  out->stream_out_.get_render_position = out_get_render_position;
  out->stream_out_.get_next_write_timestamp = out_get_next_write_timestamp;
  out->stream_out_.pause = out_pause;
  out->stream_out_.resume = out_resume;
  out->stream_out_.get_presentation_position = out_get_presentation_position;
  out->stream_out_.update_source_metadata = out_update_source_metadata;

  if (!out->bluetooth_output_.LoadAudioConfig(config)) {
    LOG(ERROR) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << " failed to get audio config";
  }
  // WAR to support Mono / 16 bits per sample as the Bluetooth stack required
  if (config->channel_mask == AUDIO_CHANNEL_OUT_MONO && config->format == AUDIO_FORMAT_PCM_16_BIT) {
    LOG(INFO) << __func__ << ": force channels=" << StringPrintf("%#x", out->channel_mask_)
              << " to be AUDIO_CHANNEL_OUT_STEREO";
    out->bluetooth_output_.ForcePcmStereoToMono(true);
    config->channel_mask = AUDIO_CHANNEL_OUT_STEREO;
  }
  out->sample_rate_ = config->sample_rate;
  out->channel_mask_ = config->channel_mask;
  out->format_ = config->format;
  // frame is number of samples per channel
  out->frames_count_ =
      samples_per_ticks(kBluetoothDefaultOutputBufferMs, out->sample_rate_, 1);
  out->frames_rendered_ = 0;
  out->frames_presented_ = 0;

  *stream_out = &out->stream_out_;
  LOG(INFO) << __func__ << ": state=" << out->bluetooth_output_.GetState() << ", sample_rate=" << out->sample_rate_
            << ", channels=" << StringPrintf("%#x", out->channel_mask_) << ", format=" << out->format_
            << ", frames=" << out->frames_count_;
  return 0;
}

void adev_close_output_stream(struct audio_hw_device* dev,
                              struct audio_stream_out* stream) {
  auto* out = reinterpret_cast<BluetoothStreamOut*>(stream);
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", stopping";
  if (out->bluetooth_output_.GetState() != BluetoothStreamState::DISABLED) {
    out->frames_rendered_ = 0;
    out->frames_presented_ = 0;
    out->bluetooth_output_.Stop();
  }
  out->bluetooth_output_.TearDown();
  LOG(VERBOSE) << __func__ << ": state=" << out->bluetooth_output_.GetState()
               << ", stopped";
  delete out;
}

size_t adev_get_input_buffer_size(const struct audio_hw_device* dev,
                                  const struct audio_config* config) {
  return 320;
}

int adev_open_input_stream(struct audio_hw_device* dev,
                           audio_io_handle_t handle, audio_devices_t devices,
                           struct audio_config* config,
                           struct audio_stream_in** stream_in,
                           audio_input_flags_t flags __unused,
                           const char* address __unused,
                           audio_source_t source __unused) {
  return -EINVAL;
}

void adev_close_input_stream(struct audio_hw_device* dev,
                             struct audio_stream_in* stream_in) {}
