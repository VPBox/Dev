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

#include "service/low_energy_scanner.h"

#include "service/adapter.h"
#include "service/logging_helpers.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"

#include <base/bind.h>
#include <base/callback.h>
#include <base/logging.h>

using std::lock_guard;
using std::mutex;

namespace bluetooth {

namespace {

// 31 + 31 for advertising data and scan response. This is the maximum length
// TODO(armansito): Fix the HAL to return a concatenated blob that contains the
// true length of each field and also provide a length parameter so that we
// can support advertising length extensions in the future.
const size_t kScanRecordLength = 62;

// Returns the length of the given scan record array. We have to calculate this
// based on the maximum possible data length and the TLV data. See TODO above
// |kScanRecordLength|.
size_t GetScanRecordLength(std::vector<uint8_t> bytes) {
  for (size_t i = 0, field_len = 0; i < kScanRecordLength;
       i += (field_len + 1)) {
    field_len = bytes[i];

    // Assert here that the data returned from the stack is correctly formatted
    // in TLV form and that the length of the current field won't exceed the
    // total data length.
    CHECK(i + field_len < kScanRecordLength);

    // If the field length is zero and we haven't reached the maximum length,
    // then we have found the length, as the stack will pad the data with zeros
    // accordingly.
    if (field_len == 0) return i;
  }

  // We have reached the end.
  return kScanRecordLength;
}

}  // namespace

// LowEnergyScanner implementation
// ========================================================

LowEnergyScanner::LowEnergyScanner(Adapter& adapter, const Uuid& uuid,
                                   int scanner_id)
    : adapter_(adapter),
      app_identifier_(uuid),
      scanner_id_(scanner_id),
      scan_started_(false),
      delegate_(nullptr) {}

LowEnergyScanner::~LowEnergyScanner() {
  // Automatically unregister the scanner.
  VLOG(1) << "LowEnergyScanner unregistering scanner: " << scanner_id_;

  // Unregister as observer so we no longer receive any callbacks.
  hal::BluetoothGattInterface::Get()->RemoveScannerObserver(this);

  hal::BluetoothGattInterface::Get()->GetScannerHALInterface()->Unregister(
      scanner_id_);

  // Stop any scans started by this client.
  if (scan_started_.load()) StopScan();
}

void LowEnergyScanner::SetDelegate(Delegate* delegate) {
  lock_guard<mutex> lock(delegate_mutex_);
  delegate_ = delegate;
}

bool LowEnergyScanner::StartScan(const ScanSettings& settings,
                                 const std::vector<ScanFilter>& filters) {
  VLOG(2) << __func__;

  // Cannot start a scan if the adapter is not enabled.
  if (!adapter_.IsEnabled()) {
    LOG(ERROR) << "Cannot scan while Bluetooth is disabled";
    return false;
  }

  // TODO(jpawlowski): Push settings and filtering logic below the HAL.
  bt_status_t status =
      hal::BluetoothGattInterface::Get()->StartScan(scanner_id_);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to initiate scanning for client: " << scanner_id_;
    return false;
  }

  scan_started_ = true;
  return true;
}

bool LowEnergyScanner::StopScan() {
  VLOG(2) << __func__;

  // TODO(armansito): We don't support batch scanning yet so call
  // StopRegularScanForClient directly. In the future we will need to
  // conditionally call a batch scan API here.
  bt_status_t status =
      hal::BluetoothGattInterface::Get()->StopScan(scanner_id_);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to stop scan for client: " << scanner_id_;
    return false;
  }

  scan_started_ = false;
  return true;
}

const Uuid& LowEnergyScanner::GetAppIdentifier() const {
  return app_identifier_;
}

int LowEnergyScanner::GetInstanceId() const { return scanner_id_; }

void LowEnergyScanner::ScanResultCallback(
    hal::BluetoothGattInterface* gatt_iface, const RawAddress& bda, int rssi,
    std::vector<uint8_t> adv_data) {
  // Ignore scan results if this client didn't start a scan.
  if (!scan_started_.load()) return;

  lock_guard<mutex> lock(delegate_mutex_);
  if (!delegate_) return;

  // TODO(armansito): Apply software filters here.

  size_t record_len = GetScanRecordLength(adv_data);
  std::vector<uint8_t> scan_record(adv_data.begin(),
                                   adv_data.begin() + record_len);

  ScanResult result(BtAddrString(&bda), scan_record, rssi);

  delegate_->OnScanResult(this, result);
}

// LowEnergyScannerFactory implementation
// ========================================================

LowEnergyScannerFactory::LowEnergyScannerFactory(Adapter& adapter)
    : adapter_(adapter) {
  hal::BluetoothGattInterface::Get()->AddScannerObserver(this);
}

LowEnergyScannerFactory::~LowEnergyScannerFactory() {
  hal::BluetoothGattInterface::Get()->RemoveScannerObserver(this);
}

bool LowEnergyScannerFactory::RegisterInstance(
    const Uuid& uuid, const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  if (pending_calls_.find(uuid) != pending_calls_.end()) {
    LOG(ERROR) << "Low-Energy scanner with given Uuid already registered - "
               << "Uuid: " << uuid.ToString();
    return false;
  }

  BleScannerInterface* hal_iface =
      hal::BluetoothGattInterface::Get()->GetScannerHALInterface();

  hal_iface->RegisterScanner(
      base::Bind(&LowEnergyScannerFactory::RegisterScannerCallback,
                 base::Unretained(this), callback, uuid));

  pending_calls_.insert(uuid);

  return true;
}

void LowEnergyScannerFactory::RegisterScannerCallback(
    const RegisterCallback& callback, const Uuid& app_uuid, uint8_t scanner_id,
    uint8_t status) {
  Uuid uuid(app_uuid);

  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  auto iter = pending_calls_.find(uuid);
  if (iter == pending_calls_.end()) {
    VLOG(1) << "Ignoring callback for unknown app_id: " << uuid.ToString();
    return;
  }

  // No need to construct a scanner if the call wasn't successful.
  std::unique_ptr<LowEnergyScanner> scanner;
  BLEStatus result = BLE_STATUS_FAILURE;
  if (status == BT_STATUS_SUCCESS) {
    scanner.reset(new LowEnergyScanner(adapter_, uuid, scanner_id));

    hal::BluetoothGattInterface::Get()->AddScannerObserver(scanner.get());

    result = BLE_STATUS_SUCCESS;
  }

  // Notify the result via the result callback.
  callback(result, app_uuid, std::move(scanner));

  pending_calls_.erase(iter);
}

}  // namespace bluetooth
