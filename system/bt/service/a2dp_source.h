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
#include "service/common/bluetooth/a2dp_codec_config.h"
#include "service/hal/bluetooth_av_interface.h"

namespace bluetooth {

class A2dpSource : public BluetoothInstance,
                   private hal::BluetoothAvInterface::A2dpSourceObserver {
 public:
  // We only allow one instance of this object at a time.
  static const int kSingletonInstanceId;

  class Delegate {
   public:
    virtual void OnConnectionState(const std::string& device_address,
                                   int state) = 0;
    virtual void OnAudioState(const std::string& device_address, int state) = 0;
    virtual void OnAudioConfig(
        const std::string& device_address, A2dpCodecConfig codec_config,
        const std::vector<A2dpCodecConfig>& codecs_local_capabilities,
        const std::vector<A2dpCodecConfig>& codecs_selectable_capabilities) = 0;

   protected:
    virtual ~Delegate() = default;
  };

  ~A2dpSource() override;

  void SetDelegate(Delegate* delegate);

  // BluetoothInstance implementation:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

  bool Enable(const std::vector<A2dpCodecConfig>& codec_priorities);
  void Disable();
  bool Connect(const std::string& device_address);
  bool Disconnect(const std::string& device_address);
  bool ConfigCodec(const std::string& device_address,
                   const std::vector<A2dpCodecConfig>& codec_preferences);

 private:
  friend class A2dpSourceFactory;

  explicit A2dpSource(const Uuid& uuid);

  // hal::bluetooth::hal::BluetoothAvInterface::Observer implementation:
  void ConnectionStateCallback(hal::BluetoothAvInterface* iface,
                               const RawAddress& bd_addr,
                               btav_connection_state_t state) override;
  void AudioStateCallback(hal::BluetoothAvInterface* iface,
                          const RawAddress& bd_addr,
                          btav_audio_state_t state) override;
  void AudioConfigCallback(
      hal::BluetoothAvInterface* iface, const RawAddress& bd_addr,
      const btav_a2dp_codec_config_t& codec_config,
      const std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities,
      const std::vector<btav_a2dp_codec_config_t>
          codecs_selectable_capabilities) override;

  // For |GetAppIdentifier|.
  const Uuid app_identifier_;

  std::mutex mutex_;

  // A second mutex is used only for |delegate_|. We cannot use |mutex_| because
  // it may cause a deadlock if the caller and Delegate both take the same lock
  // 'clock'.
  // In that scenario, the caller may take 'clock' first and will try to take
  // |mutex_| second. The callback will take |mutex_| first and invoke a
  // delegate function which attempts to take 'clock'.
  std::mutex delegate_mutex_;
  Delegate* delegate_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(A2dpSource);
};

class A2dpSourceFactory : public BluetoothInstanceFactory {
 public:
  A2dpSourceFactory();
  ~A2dpSourceFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& uuid,
                        const RegisterCallback& callback) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(A2dpSourceFactory);
};

}  // namespace bluetooth
