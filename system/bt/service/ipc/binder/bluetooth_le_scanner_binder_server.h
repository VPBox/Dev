//
//  Copyright 2016 The Android Open Source Project
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

#include <android/bluetooth/IBluetoothLeScannerCallback.h>
#include "android/bluetooth/BnBluetoothLeScanner.h"

#include "service/common/bluetooth/low_energy_constants.h"
#include "service/ipc/binder/interface_with_instances_base.h"
#include "service/low_energy_scanner.h"

using android::binder::Status;
using android::String16;

using android::bluetooth::BnBluetoothLeScanner;
using android::bluetooth::IBluetoothLeScannerCallback;

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

// Implements the server side of the IBluetoothLowEnergy interface.
class BluetoothLeScannerBinderServer
    : public BnBluetoothLeScanner,
      public InterfaceWithInstancesBase,
      public bluetooth::LowEnergyScanner::Delegate {
 public:
  explicit BluetoothLeScannerBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothLeScannerBinderServer() override;

  // IBluetoothLowEnergy overrides:
  Status RegisterScanner(
      const android::sp<IBluetoothLeScannerCallback>& callback,
      bool* _aidl_return) override;
  Status UnregisterScanner(int scanner_id) override;
  Status UnregisterAll() override;
  Status StartScan(int scanner_id,
                   const android::bluetooth::ScanSettings& settings,
                   const std::vector<android::bluetooth::ScanFilter>& filters,
                   bool* _aidl_return) override;
  Status StopScan(int scanner_id, bool* _aidl_return) override;

  void OnScanResult(bluetooth::LowEnergyScanner* scanner,
                    const bluetooth::ScanResult& result) override;

 private:
  // Returns a pointer to the IBluetoothLowEnergyCallback instance associated
  // with |scanner_id|. Returns NULL if such a callback cannot be found.
  android::sp<IBluetoothLeScannerCallback> GetLECallback(int scanner_id);

  // Returns a pointer to the LowEnergyScanner instance associated with
  // |scanner_id|. Returns NULL if such a scanner cannot be found.
  std::shared_ptr<bluetooth::LowEnergyScanner> GetLEScanner(int scanner_id);

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(bluetooth::BLEStatus status,
                              android::sp<IInterface> callback,
                              bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothLeScannerBinderServer);
};

}  // namespace binder
}  // namespace ipc
