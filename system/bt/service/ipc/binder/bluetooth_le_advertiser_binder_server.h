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

#include <android/bluetooth/IBluetoothLeAdvertiserCallback.h>
#include "android/bluetooth/BnBluetoothLeAdvertiser.h"

#include "service/common/bluetooth/low_energy_constants.h"
#include "service/ipc/binder/interface_with_instances_base.h"
#include "service/low_energy_advertiser.h"

using android::binder::Status;
using android::String16;

using android::bluetooth::BnBluetoothLeAdvertiser;
using android::bluetooth::IBluetoothLeAdvertiserCallback;

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

// Implements the server side of the IBluetoothLowEnergy interface.
class BluetoothLeAdvertiserBinderServer : public BnBluetoothLeAdvertiser,
                                          public InterfaceWithInstancesBase {
 public:
  explicit BluetoothLeAdvertiserBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothLeAdvertiserBinderServer() override;

  // IBluetoothLowEnergy overrides:
  Status RegisterAdvertiser(
      const android::sp<IBluetoothLeAdvertiserCallback>& callback,
      bool* _aidl_return) override;
  Status UnregisterAdvertiser(int advertiser_id) override;
  Status UnregisterAll() override;
  Status StartMultiAdvertising(
      int advertiser_id,
      const android::bluetooth::AdvertiseData& advertise_data,
      const android::bluetooth::AdvertiseData& scan_response,
      const android::bluetooth::AdvertiseSettings& settings,
      bool* _aidl_return) override;
  Status StopMultiAdvertising(int advertiser_id, bool* _aidl_return) override;

 private:
  // Returns a pointer to the IBluetoothLeAdvertiserCallback instance associated
  // with |advertiser_id|. Returns NULL if such a callback cannot be found.
  android::sp<IBluetoothLeAdvertiserCallback> GetLECallback(int advertiser_id);

  // Returns a pointer to the LowEnergyAdvertiser instance associated with
  // |advertiser_id|. Returns NULL if such a advertiser cannot be found.
  std::shared_ptr<bluetooth::LowEnergyAdvertiser> GetLEAdvertiser(
      int advertiser_id);

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothLeAdvertiserBinderServer);
};

}  // namespace binder
}  // namespace ipc
