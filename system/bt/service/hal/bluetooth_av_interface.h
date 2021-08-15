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
#include <hardware/bluetooth.h>
#include <hardware/bt_av.h>

#include <vector>

namespace bluetooth {
namespace hal {

class BluetoothAvInterface {
 public:
  class A2dpSourceObserver {
   public:
    virtual void ConnectionStateCallback(BluetoothAvInterface* iface,
                                         const RawAddress& bd_addr,
                                         btav_connection_state_t state);
    virtual void AudioStateCallback(BluetoothAvInterface* iface,
                                    const RawAddress& bd_addr,
                                    btav_audio_state_t state);
    virtual void AudioConfigCallback(
        BluetoothAvInterface* iface, const RawAddress& bd_addr,
        const btav_a2dp_codec_config_t& codec_config,
        const std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities,
        const std::vector<btav_a2dp_codec_config_t>
            codecs_selectable_capabilities);

   protected:
    virtual ~A2dpSourceObserver() = default;
  };

  class A2dpSinkObserver {
   public:
    virtual void ConnectionStateCallback(BluetoothAvInterface* iface,
                                         const RawAddress& bd_addr,
                                         btav_connection_state_t state);
    virtual void AudioStateCallback(BluetoothAvInterface* iface,
                                    const RawAddress& bd_addr,
                                    btav_audio_state_t state);
    virtual void AudioConfigCallback(BluetoothAvInterface* iface,
                                     const RawAddress& bd_addr,
                                     uint32_t sample_rate,
                                     uint8_t channel_count);

   protected:
    virtual ~A2dpSinkObserver() = default;
  };

  static bool Initialize();
  static void CleanUp();
  static bool IsInitialized();
  static void InitializeForTesting(BluetoothAvInterface* test_instance);

  static BluetoothAvInterface* Get();

  virtual bool A2dpSourceEnable(
      std::vector<btav_a2dp_codec_config_t> codec_priorities) = 0;
  virtual void A2dpSourceDisable() = 0;
  virtual bool A2dpSinkEnable() = 0;
  virtual void A2dpSinkDisable() = 0;

  virtual void AddA2dpSourceObserver(A2dpSourceObserver* observer) = 0;
  virtual void RemoveA2dpSourceObserver(A2dpSourceObserver* observer) = 0;
  virtual void AddA2dpSinkObserver(A2dpSinkObserver* observer) = 0;
  virtual void RemoveA2dpSinkObserver(A2dpSinkObserver* observer) = 0;

  virtual const btav_source_interface_t* GetA2dpSourceHALInterface() = 0;
  virtual const btav_sink_interface_t* GetA2dpSinkHALInterface() = 0;

 protected:
  BluetoothAvInterface() = default;
  virtual ~BluetoothAvInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothAvInterface);
};

}  // namespace hal
}  // namespace bluetooth
