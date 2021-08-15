//
//  Copyright 2015 Google, Inc.
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

#include <android/bluetooth/BnBluetoothGattServer.h>
#include <android/bluetooth/IBluetoothGattServerCallback.h>

#include "service/gatt_server.h"
#include "service/ipc/binder/interface_with_instances_base.h"

using android::bluetooth::BnBluetoothGattServer;
using android::bluetooth::IBluetoothGattServerCallback;

using ::android::binder::Status;

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

// Implements the server side of the IBluetoothGattServer interface.
class BluetoothGattServerBinderServer : public BnBluetoothGattServer,
                                        public InterfaceWithInstancesBase,
                                        public bluetooth::GattServer::Delegate {
 public:
  explicit BluetoothGattServerBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothGattServerBinderServer() override = default;

  // IBluetoothGattServer overrides:
  Status RegisterServer(
      const ::android::sp<::android::bluetooth::IBluetoothGattServerCallback>&
          callback,
      bool* _aidl_return) override;
  Status UnregisterServer(int32_t server_id) override;
  Status UnregisterAll() override;
  Status AddService(int32_t server_id,
                    const ::android::bluetooth::BluetoothGattService& service,
                    bool* _aidl_return) override;
  Status SendResponse(int32_t server_id,
                      const ::android::String16& device_address,
                      int32_t request_id, int32_t status, int32_t offset,
                      const ::std::vector<uint8_t>& value,
                      bool* _aidl_return) override;
  Status SendNotification(int32_t server_id,
                          const ::android::String16& device_address, int handle,
                          bool confirm, const ::std::vector<uint8_t>& value,
                          bool* _aidl_return) override;

  // bluetooth::GattServer::Delegate overrides:
  void OnCharacteristicReadRequest(bluetooth::GattServer* gatt_server,
                                   const std::string& device_address,
                                   int request_id, int offset, bool is_long,
                                   uint16_t handle) override;
  void OnDescriptorReadRequest(bluetooth::GattServer* gatt_server,
                               const std::string& device_address,
                               int request_id, int offset, bool is_long,
                               uint16_t handle) override;
  void OnCharacteristicWriteRequest(bluetooth::GattServer* gatt_server,
                                    const std::string& device_address,
                                    int request_id, int offset,
                                    bool is_prepare_write, bool need_response,
                                    const std::vector<uint8_t>& value,
                                    uint16_t handle) override;
  void OnDescriptorWriteRequest(bluetooth::GattServer* gatt_server,
                                const std::string& device_address,
                                int request_id, int offset,
                                bool is_prepare_write, bool need_response,
                                const std::vector<uint8_t>& value,
                                uint16_t handle) override;
  void OnExecuteWriteRequest(bluetooth::GattServer* gatt_server,
                             const std::string& device_address, int request_id,
                             bool is_execute) override;
  void OnConnectionStateChanged(bluetooth::GattServer* gatt_server,
                                const std::string& device_addres,
                                bool connected) override;

 private:
  // Returns a pointer to the IBluetoothGattServerCallback instance
  // associated with |server_id|. Returns NULL if such a callback cannot be
  // found.
  android::sp<IBluetoothGattServerCallback> GetGattServerCallback(
      int server_id);

  // Returns a pointer to the GattServer instance associated with |server_id|.
  // Returns NULL if such an instance cannot be found.
  std::shared_ptr<bluetooth::GattServer> GetGattServer(int server_id);

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothGattServerBinderServer);
};

}  // namespace binder
}  // namespace ipc
