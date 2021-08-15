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

#include <android/hardware/bluetooth/audio/2.0/types.h>
#include <hardware/audio.h>
#include <condition_variable>
#include <mutex>
#include <unordered_map>

enum class BluetoothStreamState : uint8_t;

namespace android {
namespace bluetooth {
namespace audio {

// Proxy for Bluetooth Audio HW Module to communicate with Bluetooth Audio
// Session Control. All methods are not thread safe, so users must acquire a
// lock. Note: currently, in stream_apis.cc, if GetState() is only used for
// verbose logging, it is not locked, so the state may not be synchronized.
class BluetoothAudioPortOut {
 public:
  BluetoothAudioPortOut();
  ~BluetoothAudioPortOut() = default;

  // Fetch output control / data path of BluetoothAudioPortOut and setup
  // callbacks into BluetoothAudioProvider. If SetUp() returns false, the audio
  // HAL must delete this BluetoothAudioPortOut and return EINVAL to caller
  bool SetUp(audio_devices_t devices);

  // Unregister this BluetoothAudioPortOut from BluetoothAudioSessionControl.
  // Audio HAL must delete this BluetoothAudioPortOut after calling this.
  void TearDown();

  // When the Audio framework / HAL tries to query audio config about format,
  // channel mask and sample rate, it uses this function to fetch from the
  // Bluetooth stack
  bool LoadAudioConfig(audio_config_t* audio_cfg) const;

  // WAR to support Mono mode / 16 bits per sample
  void ForcePcmStereoToMono(bool force) {
    is_stereo_to_mono_ = force;
  }

  // When the Audio framework / HAL wants to change the stream state, it invokes
  // these 3 functions to control the Bluetooth stack (Audio Control Path).
  // Note: Both Start() and Suspend() will return ture when there are no errors.
  // Called by Audio framework / HAL to start the stream
  bool Start();
  // Called by Audio framework / HAL to suspend the stream
  bool Suspend();
  // Called by Audio framework / HAL to stop the stream
  void Stop();

  // The audio data path to the Bluetooth stack (Software encoding)
  size_t WriteData(const void* buffer, size_t bytes) const;

  // Called by the Audio framework / HAL to fetch informaiton about audio frames
  // presented to an external sink.
  bool GetPresentationPosition(uint64_t* delay_ns, uint64_t* bytes,
                               timespec* timestamp) const;

  // Called by the Audio framework / HAL when the metadata of the stream's
  // source has been changed.
  void UpdateMetadata(const source_metadata* source_metadata) const;

  // Return the current BluetoothStreamState
  BluetoothStreamState GetState() const;

  // Set the current BluetoothStreamState
  void SetState(BluetoothStreamState state);

 private:
  BluetoothStreamState state_;
  ::android::hardware::bluetooth::audio::V2_0::SessionType session_type_;
  uint16_t cookie_;
  mutable std::mutex cv_mutex_;
  std::condition_variable internal_cv_;
  // WR to support Mono: True if fetching Stereo and mixing into Mono
  bool is_stereo_to_mono_ = false;

  // Check and initialize session type for |devices| If failed, this
  // BluetoothAudioPortOut is not initialized and must be deleted.
  bool init_session_type(audio_devices_t device);

  bool in_use() const;

  bool CondwaitState(BluetoothStreamState state);

  void ControlResultHandler(
      const ::android::hardware::bluetooth::audio::V2_0::Status& status);
  void SessionChangedHandler();
};

}  // namespace audio
}  // namespace bluetooth
}  // namespace android
