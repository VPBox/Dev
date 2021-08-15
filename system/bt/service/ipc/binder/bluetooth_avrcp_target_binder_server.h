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

#include "base/macros.h"

#include "android/bluetooth/BnBluetoothAvrcpTarget.h"
#include "android/bluetooth/IBluetoothAvrcpTargetCallback.h"

#include "service/avrcp_target.h"
#include "service/ipc/binder/interface_with_instances_base.h"

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

class BluetoothAvrcpTargetBinderServer
    : public InterfaceWithInstancesBase,
      public android::bluetooth::BnBluetoothAvrcpTarget,
      public bluetooth::AvrcpTarget::Delegate {
 public:
  explicit BluetoothAvrcpTargetBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothAvrcpTargetBinderServer() override;

  bool HasInstance();

  // IBluetoothAvrcpTarget implementation:
  android::binder::Status Register(
      const android::sp<android::bluetooth::IBluetoothAvrcpTargetCallback>&
          callback,
      bool* _aidl_return) override;
  android::binder::Status Unregister(int32_t id) override;
  android::binder::Status UnregisterAll() override;
  android::binder::Status Enable(bool* _aidl_return) override;
  android::binder::Status Disable(bool* _aidl_return) override;
  android::binder::Status GetPlayStatusResponse(const android::String16& addr,
                                                int32_t play_status,
                                                int32_t song_len,
                                                int32_t song_pos,
                                                bool* _aidl_return) override;
  android::binder::Status ListPlayerAppAttrResponse(
      const android::String16& addr, const std::vector<int32_t>& attrs,
      bool* _aidl_return) override;
  android::binder::Status GetPlayerAppValueResponse(
      const android::String16& addr,
      const std::vector<android::bluetooth::BluetoothAvrcpIntValue>& values,
      bool* _aidl_return) override;
  android::binder::Status GetPlayerAppAttrTextResponse(
      const android::String16& addr,
      const std::vector<android::bluetooth::BluetoothAvrcpStringValue>& attrs,
      bool* _aidl_return) override;
  android::binder::Status GetPlayerAppValueTextResponse(
      const android::String16& addr,
      const std::vector<android::bluetooth::BluetoothAvrcpStringValue>& values,
      bool* _aidl_return) override;
  android::binder::Status GetElementAttrResponse(
      const android::String16& addr,
      const std::vector<android::bluetooth::BluetoothAvrcpStringValue>& attrs,
      bool* _aidl_return) override;
  android::binder::Status SetPlayerAppValueResponse(
      const android::String16& addr, int32_t rsp_status,
      bool* _aidl_return) override;
  android::binder::Status RegisterNotificationResponse(
      int32_t event_id, int32_t type,
      const android::bluetooth::BluetoothAvrcpRegisterNotificationResponse&
          param,
      bool* _aidl_return) override;
  android::binder::Status SetVolume(int32_t volume,
                                    bool* _aidl_return) override;

 private:
  // bluetooth::bluetooth::AvrcpTarget::Delegate implementation:
  void OnGetRemoteFeatures(const std::string& addr, int32_t features) override;
  void OnGetPlayStatus(const std::string& addr) override;
  void OnListPlayerAppAttr(const std::string& addr) override;
  void OnListPlayerAppValues(const std::string& addr, int32_t attr_id) override;
  void OnGetPlayerAppValue(const std::string& addr,
                           const std::vector<int32_t>& attrs) override;
  void OnGetPlayerAppAttrsText(const std::string& addr,
                               const std::vector<int32_t>& attrs) override;
  void OnGetPlayerAppValuesText(const std::string& addr, int32_t attr_id,
                                const std::vector<int32_t>& values) override;
  void OnSetPlayerAppValue(
      const std::string& addr,
      const std::vector<bluetooth::AvrcpIntValue>& values) override;
  void OnGetElementAttrs(const std::string& addr,
                         const std::vector<int32_t>& attrs) override;
  void OnRegisterNotification(const std::string& addr, int32_t event_id,
                              uint32_t param) override;
  void OnVolumeChange(const std::string& addr, int32_t volume,
                      int32_t ctype) override;
  void OnPassThroughCommand(const std::string& addr, int32_t id,
                            int32_t key_state) override;

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  android::sp<android::bluetooth::IBluetoothAvrcpTargetCallback>
  GetAvrcpTargetCallback();
  std::shared_ptr<bluetooth::AvrcpTarget> GetAvrcpTarget();

  bluetooth::Adapter* const adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothAvrcpTargetBinderServer);
};

}  // namespace binder
}  // namespace ipc
