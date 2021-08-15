/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_INCLUDE_BT_HEARING_AID_H
#define ANDROID_INCLUDE_BT_HEARING_AID_H

#include <hardware/bluetooth.h>

namespace bluetooth {
namespace hearing_aid {

enum class ConnectionState {
  DISCONNECTED = 0,
  CONNECTING,
  CONNECTED,
  DISCONNECTING
};

class HearingAidCallbacks {
 public:
  virtual ~HearingAidCallbacks() = default;

  /** Callback for profile connection state change */
  virtual void OnConnectionState(ConnectionState state,
                                 const RawAddress& address) = 0;

  /** Callback for device being available. Is executed when devices are loaded
   * from storage on stack bringup, and when new device is connected to profile.
   * Main purpose of this callback is to keep its users informed of device
   * capabilities and hiSyncId.
   */
  virtual void OnDeviceAvailable(uint8_t capabilities, uint64_t hiSyncId,
                                 const RawAddress& address) = 0;
};

class HearingAidInterface {
 public:
  virtual ~HearingAidInterface() = default;

  /** Register the Hearing Aid callbacks */
  virtual void Init(HearingAidCallbacks* callbacks) = 0;

  /** Connect to Hearing Aid */
  virtual void Connect(const RawAddress& address) = 0;

  /** Disconnect from Hearing Aid */
  virtual void Disconnect(const RawAddress& address) = 0;

  /** Add a hearing aid device to white list */
  virtual void AddToWhiteList(const RawAddress& address) = 0;

  /** Set the volume */
  virtual void SetVolume(int8_t volume) = 0;

  /** Closes the interface. */
  virtual void Cleanup(void) = 0;

  /* Called when Hearing Aid is unbonded. */
  virtual void RemoveDevice(const RawAddress& address) = 0;
};

}  // namespace hearing_aid
}  // namespace bluetooth

#endif /* ANDROID_INCLUDE_BT_HEARING_AID_H */
