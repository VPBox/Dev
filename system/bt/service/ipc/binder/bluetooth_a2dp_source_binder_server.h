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

#include <map>
#include <string>
#include <vector>

#include <base/macros.h>

#include <android/bluetooth/BnBluetoothA2dpSource.h>
#include <android/bluetooth/IBluetoothA2dpSourceCallback.h>

#include "service/a2dp_source.h"
#include "service/ipc/binder/interface_with_instances_base.h"

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

class BluetoothA2dpSourceBinderServer
    : public InterfaceWithInstancesBase,
      public android::bluetooth::BnBluetoothA2dpSource,
      public bluetooth::A2dpSource::Delegate {
 public:
  explicit BluetoothA2dpSourceBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothA2dpSourceBinderServer() override;

  bool HasInstance();

  // IBluetoothA2dpSource implementation:
  android::binder::Status Register(
      const android::sp<android::bluetooth::IBluetoothA2dpSourceCallback>&
          callback,
      bool* _aidl_return) override;
  android::binder::Status Unregister() override;
  android::binder::Status Enable(
      const std::vector<android::bluetooth::BluetoothA2dpCodecConfig>&
          codec_priorities,
      bool* _aidl_return) override;
  android::binder::Status Disable(bool* _aidl_return) override;
  android::binder::Status Connect(const android::String16& device_address,
                                  bool* _aidl_return) override;
  android::binder::Status Disconnect(const android::String16& device_address,
                                     bool* _aidl_return) override;
  android::binder::Status ConfigCodec(
      const android::String16& device_address,
      const std::vector<android::bluetooth::BluetoothA2dpCodecConfig>&
          codec_preferences,
      bool* _aidl_return) override;

 private:
  // bluetooth::bluetooth::A2dpSource::Delegate implementation:
  void OnConnectionState(const std::string& device_address, int state) override;
  void OnAudioState(const std::string& device_address, int state) override;
  void OnAudioConfig(
      const std::string& device_address,
      bluetooth::A2dpCodecConfig codec_config,
      const std::vector<bluetooth::A2dpCodecConfig>& codecs_local_capabilities,
      const std::vector<bluetooth::A2dpCodecConfig>&
          codecs_selectable_capabilities) override;

  android::sp<android::bluetooth::IBluetoothA2dpSourceCallback>
  GetA2dpSourceCallback();
  std::shared_ptr<bluetooth::A2dpSource> GetA2dpSource();

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* const adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothA2dpSourceBinderServer);
};

}  // namespace binder
}  // namespace ipc
