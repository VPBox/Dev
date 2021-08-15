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

#include <base/macros.h>

#include <android/bluetooth/BnBluetoothA2dpSink.h>
#include <android/bluetooth/IBluetoothA2dpSinkCallback.h>

#include "service/a2dp_sink.h"
#include "service/ipc/binder/interface_with_instances_base.h"

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

class BluetoothA2dpSinkBinderServer
    : public InterfaceWithInstancesBase,
      public android::bluetooth::BnBluetoothA2dpSink,
      public bluetooth::A2dpSink::Delegate {
 public:
  explicit BluetoothA2dpSinkBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothA2dpSinkBinderServer() override = default;

  // IBluetoothA2dpSink implementation:
  android::binder::Status Register(
      const android::sp<android::bluetooth::IBluetoothA2dpSinkCallback>&
          callback,
      bool* _aidl_return) override;
  android::binder::Status Enable(bool* _aidl_return) override;
  android::binder::Status Disable(bool* _aidl_return) override;
  android::binder::Status Unregister() override;
  android::binder::Status Connect(const android::String16& device_address,
                                  bool* _aidl_return) override;
  android::binder::Status Disconnect(const android::String16& device_address,
                                     bool* _aidl_return) override;
  android::binder::Status SetAudioFocusState(int state,
                                             bool* _aidl_return) override;
  android::binder::Status SetAudioTrackGain(float gain,
                                            bool* _aidl_return) override;

  // bluetooth::bluetooth::A2dpSink::Delegate implementation:
  void OnConnectionState(const std::string& device_address, int state) override;
  void OnAudioState(const std::string& device_address, int state) override;
  void OnAudioConfig(const std::string& device_address, uint32_t sample_rate,
                     uint8_t channel_count) override;

  bool HasInstance();

 private:
  android::sp<android::bluetooth::IBluetoothA2dpSinkCallback>
  GetA2dpSinkCallback();
  std::shared_ptr<bluetooth::A2dpSink> GetA2dpSink();

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothA2dpSinkBinderServer);
};

}  // namespace binder
}  // namespace ipc
