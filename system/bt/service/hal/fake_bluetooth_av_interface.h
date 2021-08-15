//
//  Copyright (C) 2017 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#pragma once

#include <base/macros.h>
#include <base/observer_list.h>

#include "service/hal/bluetooth_av_interface.h"

namespace bluetooth {
namespace hal {

class FakeBluetoothAvInterface : public BluetoothAvInterface {
 public:
  // Handles HAL Bluetooth A2DP sink API calls for testing. Test code can
  // provide a fake or mock implementation of this and all calls will be routed
  // to it.
  class TestA2dpSinkHandler {
   public:
    virtual bt_status_t Connect(RawAddress bda) = 0;
    virtual bt_status_t Disconnect(RawAddress bda) = 0;
    virtual void SetAudioFocusState(int focus_state) = 0;
    virtual void SetAudioTrackGain(float gain) = 0;

   protected:
    virtual ~TestA2dpSinkHandler() = default;
  };

  // Constructs the fake with the given handlers. Implementations can
  // provide their own handlers or simply pass "nullptr" for the default
  // behavior in which BT_STATUS_FAIL will be returned from all calls.
  FakeBluetoothAvInterface(
      std::shared_ptr<TestA2dpSinkHandler> a2dp_sink_handler);
  ~FakeBluetoothAvInterface();

  // The methods below can be used to notify observers with certain events and
  // given parameters.

  // A2DP sink callbacks
  void NotifyConnectionState(const RawAddress& bda,
                             btav_connection_state_t state);
  void NotifyAudioState(const RawAddress& bda, btav_audio_state_t state);
  void NotifyAudioConfig(const RawAddress& bda, uint32_t sample_rate,
                         uint8_t channel_count);

  // BluetoothAvInterface overrides:
  bool A2dpSourceEnable(
      std::vector<btav_a2dp_codec_config_t> codec_priorities) override;
  void A2dpSourceDisable() override;
  bool A2dpSinkEnable() override;
  void A2dpSinkDisable() override;
  void AddA2dpSourceObserver(A2dpSourceObserver* observer) override;
  void RemoveA2dpSourceObserver(A2dpSourceObserver* observer) override;
  void AddA2dpSinkObserver(A2dpSinkObserver* observer) override;
  void RemoveA2dpSinkObserver(A2dpSinkObserver* observer) override;
  const btav_source_interface_t* GetA2dpSourceHALInterface() override;
  const btav_sink_interface_t* GetA2dpSinkHALInterface() override;

 private:
  base::ObserverList<A2dpSourceObserver> a2dp_source_observers_;
  base::ObserverList<A2dpSinkObserver> a2dp_sink_observers_;
  std::shared_ptr<TestA2dpSinkHandler> scanner_handler_;

  DISALLOW_COPY_AND_ASSIGN(FakeBluetoothAvInterface);
};

}  // namespace hal
}  // namespace bluetooth
