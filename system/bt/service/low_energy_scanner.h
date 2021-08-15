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

#include <atomic>
#include <functional>
#include <map>
#include <mutex>

#include <base/macros.h>
#include <bluetooth/uuid.h>

#include "service/bluetooth_instance.h"
#include "service/common/bluetooth/advertise_data.h"
#include "service/common/bluetooth/advertise_settings.h"
#include "service/common/bluetooth/low_energy_constants.h"
#include "service/common/bluetooth/scan_filter.h"
#include "service/common/bluetooth/scan_result.h"
#include "service/common/bluetooth/scan_settings.h"
#include "service/hal/bluetooth_gatt_interface.h"

namespace bluetooth {

class Adapter;

// A LowEnergyScanner represents an application's handle to perform various
// Bluetooth Low Energy GAP operations. Instances cannot be created directly and
// should be obtained through the factory.
class LowEnergyScanner : private hal::BluetoothGattInterface::ScannerObserver,
                         public BluetoothInstance {
 public:
  // The Delegate interface is used to notify asynchronous events related to LE
  // scan.
  class Delegate {
   public:
    Delegate() = default;
    virtual ~Delegate() = default;

    // Called asynchronously to notify the delegate of nearby BLE advertisers
    // found during a device scan.
    virtual void OnScanResult(LowEnergyScanner* client,
                              const ScanResult& scan_result) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  // The destructor automatically unregisters this client instance from the
  // stack.
  ~LowEnergyScanner() override;

  // Assigns a delegate to this instance. |delegate| must out-live this
  // LowEnergyClient instance.
  void SetDelegate(Delegate* delegate);

  // Initiates a BLE device scan for this client using the given |settings| and
  // |filters|. See the documentation for ScanSettings and ScanFilter for how
  // these parameters can be configured. Return true on success, false
  // otherwise. Please see logs for details in case of error.
  bool StartScan(const ScanSettings& settings,
                 const std::vector<ScanFilter>& filters);

  // Stops an ongoing BLE device scan for this client.
  bool StopScan();

  // Returns the current scan settings.
  const ScanSettings& scan_settings() const { return scan_settings_; }

  // BluetoothInstace overrides:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

  void ScanResultCallback(hal::BluetoothGattInterface* gatt_iface,
                          const RawAddress& bda, int rssi,
                          std::vector<uint8_t> adv_data) override;

 private:
  friend class LowEnergyScannerFactory;

  // Constructor shouldn't be called directly as instances are meant to be
  // obtained from the factory.
  LowEnergyScanner(Adapter& adapter, const Uuid& uuid, int scanner_id);

  // Calls and clears the pending callbacks.
  void InvokeAndClearStartCallback(BLEStatus status);
  void InvokeAndClearStopCallback(BLEStatus status);

  // Raw pointer to the Bluetooth Adapter.
  Adapter& adapter_;

  // See getters above for documentation.
  Uuid app_identifier_;
  int scanner_id_;

  // Protects device scan related members below.
  std::mutex scan_fields_lock_;

  // Current scan settings.
  ScanSettings scan_settings_;

  // If true, then this client have a BLE device scan in progress.
  std::atomic_bool scan_started_;

  // Raw handle to the Delegate, which must outlive this LowEnergyScanner
  // instance.
  std::mutex delegate_mutex_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyScanner);
};

// LowEnergyScannerFactory is used to register and obtain a per-application
// LowEnergyScanner instance. Users should call RegisterInstance to obtain their
// own unique LowEnergyScanner instance that has been registered with the
// Bluetooth stack.
class LowEnergyScannerFactory
    : private hal::BluetoothGattInterface::ScannerObserver,
      public BluetoothInstanceFactory {
 public:
  // Don't construct/destruct directly except in tests. Instead, obtain a handle
  // from an Adapter instance.
  explicit LowEnergyScannerFactory(Adapter& adapter);
  ~LowEnergyScannerFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& app_uuid,
                        const RegisterCallback& callback) override;

 private:
  friend class LowEnergyScanner;

  // BluetoothGattInterface::ScannerObserver overrides:
  void RegisterScannerCallback(const RegisterCallback& callback,
                               const Uuid& app_uuid, uint8_t scanner_id,
                               uint8_t status);

  // Map of pending calls to register.
  std::mutex pending_calls_lock_;
  std::unordered_set<Uuid> pending_calls_;

  // Raw pointer to the Adapter that owns this factory.
  Adapter& adapter_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyScannerFactory);
};

}  // namespace bluetooth
