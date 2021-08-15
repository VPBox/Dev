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

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

#include <base/macros.h>

#include "service/bluetooth_instance.h"
#include "service/hal/bluetooth_av_interface.h"

namespace bluetooth {

class A2dpSink : public BluetoothInstance,
                 private hal::BluetoothAvInterface::A2dpSinkObserver {
 public:
  // We only allow one instance of this object at a time.
  static const int kSingletonInstanceId;

  class Delegate {
   public:
    virtual void OnConnectionState(const std::string& device_address,
                                   int state) = 0;
    virtual void OnAudioState(const std::string& device_address, int state) = 0;
    virtual void OnAudioConfig(const std::string& device_address,
                               uint32_t sample_rate, uint8_t channel_count) = 0;

   protected:
    virtual ~Delegate() = default;
  };

  ~A2dpSink() override;

  void SetDelegate(Delegate* delegate);

  // BluetoothInstance implementation:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

  bool Enable();
  void Disable();
  bool Connect(const std::string& device_address);
  bool Disconnect(const std::string& device_address);
  void SetAudioFocusState(int focus_state);
  void SetAudioTrackGain(float gain);

 private:
  friend class A2dpSinkFactory;

  explicit A2dpSink(const Uuid& uuid);

  // hal::bluetooth::hal::BluetoothAvInterface::Observer implementation:
  void ConnectionStateCallback(bluetooth::hal::BluetoothAvInterface* iface,
                               const RawAddress& bd_addr,
                               btav_connection_state_t state) override;
  void AudioStateCallback(bluetooth::hal::BluetoothAvInterface* iface,
                          const RawAddress& bd_addr,
                          btav_audio_state_t state) override;
  void AudioConfigCallback(bluetooth::hal::BluetoothAvInterface* iface,
                           const RawAddress& bd_addr, uint32_t sample_rate,
                           uint8_t channel_count) override;

  // See getters above for documentation.
  const Uuid app_identifier_;

  std::mutex mutex_;
  std::mutex delegate_mutex_;
  Delegate* delegate_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(A2dpSink);
};

class A2dpSinkFactory : public BluetoothInstanceFactory {
 public:
  A2dpSinkFactory();
  ~A2dpSinkFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& uuid,
                        const RegisterCallback& callback) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(A2dpSinkFactory);
};

}  // namespace bluetooth
