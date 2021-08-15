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

#include "base/macros.h"

#include "android/bluetooth/BnBluetoothAvrcpControl.h"
#include "android/bluetooth/IBluetoothAvrcpControlCallback.h"

#include "service/avrcp_control.h"
#include "service/ipc/binder/interface_with_instances_base.h"

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

class BluetoothAvrcpControlBinderServer
    : public InterfaceWithInstancesBase,
      public android::bluetooth::BnBluetoothAvrcpControl,
      public bluetooth::AvrcpControl::Delegate {
 public:
  explicit BluetoothAvrcpControlBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothAvrcpControlBinderServer() override = default;

  // IBluetoothAvrcpControl implementation:
  android::binder::Status Register(
      const android::sp<::android::bluetooth::IBluetoothAvrcpControlCallback>&
          callback,
      bool* _aidl_return) override;
  android::binder::Status Unregister(int32_t id) override;
  android::binder::Status UnregisterAll() override;
  android::binder::Status Enable(int32_t id, bool* _aidl_return) override;
  android::binder::Status Disable(int32_t id, bool* _aidl_return) override;
  android::binder::Status SendPassThroughCommand(
      int32_t id, const android::String16& device_address, int32_t key_code,
      bool key_pressed, bool* _aidl_return) override;
  android::binder::Status SetAbsVolumeResponse(
      int32_t id, const android::String16& device_address, int32_t abs_vol,
      int32_t label, bool* _aidl_return) override;
  android::binder::Status RegisterForAbsVolumeCallbackResponse(
      int32_t id, const android::String16& device_address,
      int32_t response_type, int32_t abs_vol, int32_t label,
      bool* _aidl_return) override;

 private:
  // bluetooth::bluetooth::AvrcpControl::Delegate implementation:
  void OnConnectionState(bool rc_connect, bool bt_connect,
                         const std::string& device_address) override;
  void OnTrackChanged(const std::string& device_address,
                      const bluetooth::AvrcpMediaAttr& attr) override;
  void OnSetAbsVolumeRequest(const std::string& device_address, int32_t abs_vol,
                             int32_t label) override;
  void OnRegisterForAbsVolumeCallbackRequest(const std::string& device_address,
                                             int32_t label) override;

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  std::shared_ptr<bluetooth::AvrcpControl> GetAvrcpControl(int id);

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothAvrcpControlBinderServer);
};

}  // namespace binder
}  // namespace ipc
