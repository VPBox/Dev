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

#include <memory>

#include <base/macros.h>

#include <android/bluetooth/IBluetoothLowEnergyCallback.h>
#include "android/bluetooth/BnBluetoothLowEnergy.h"

#include "service/common/bluetooth/low_energy_constants.h"
#include "service/ipc/binder/interface_with_instances_base.h"
#include "service/low_energy_client.h"

using android::binder::Status;
using android::String16;

using android::bluetooth::BnBluetoothLowEnergy;
using android::bluetooth::IBluetoothLowEnergyCallback;

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

// Implements the server side of the IBluetoothLowEnergy interface.
class BluetoothLowEnergyBinderServer
    : public BnBluetoothLowEnergy,
      public InterfaceWithInstancesBase,
      public bluetooth::LowEnergyClient::Delegate {
 public:
  explicit BluetoothLowEnergyBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothLowEnergyBinderServer() override;

  // IBluetoothLowEnergy overrides:
  Status RegisterClient(
      const android::sp<IBluetoothLowEnergyCallback>& callback,
      bool* _aidl_return) override;
  Status UnregisterClient(int client_id) override;
  Status UnregisterAll() override;
  Status Connect(int client_id, const String16& address, bool is_direct,
                 bool* _aidl_return) override;
  Status Disconnect(int client_id, const String16& address,
                    bool* _aidl_return) override;
  Status SetMtu(int client_id, const String16& address, int mtu,
                bool* _aidl_return) override;

  // bluetooth::LowEnergyClient::Delegate overrides:
  void OnConnectionState(bluetooth::LowEnergyClient* client, int status,
                         const char* address, bool connected) override;
  void OnMtuChanged(bluetooth::LowEnergyClient* client, int status,
                    const char* address, int mtu) override;

 private:
  // Returns a pointer to the IBluetoothLowEnergyCallback instance associated
  // with |client_id|. Returns NULL if such a callback cannot be found.
  android::sp<IBluetoothLowEnergyCallback> GetLECallback(int client_id);

  // Returns a pointer to the LowEnergyClient instance associated with
  // |client_id|. Returns NULL if such a client cannot be found.
  std::shared_ptr<bluetooth::LowEnergyClient> GetLEClient(int client_id);

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothLowEnergyBinderServer);
};

}  // namespace binder
}  // namespace ipc
