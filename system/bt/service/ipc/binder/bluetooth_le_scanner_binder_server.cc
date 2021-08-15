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

#include "service/ipc/binder/bluetooth_le_scanner_binder_server.h"

#include <base/logging.h>

#include "service/adapter.h"

using android::String8;
using android::String16;
using android::binder::Status;

namespace ipc {
namespace binder {

namespace {
const int kInvalidInstanceId = -1;
}  // namespace

BluetoothLeScannerBinderServer::BluetoothLeScannerBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
}

BluetoothLeScannerBinderServer::~BluetoothLeScannerBinderServer() {}

Status BluetoothLeScannerBinderServer::RegisterScanner(
    const android::sp<IBluetoothLeScannerCallback>& callback,
    bool* _aidl_return) {
  VLOG(2) << __func__;
  bluetooth::LowEnergyScannerFactory* ble_factory =
      adapter_->GetLeScannerFactory();

  *_aidl_return = RegisterInstanceBase(callback, ble_factory);
  return Status::ok();
}

Status BluetoothLeScannerBinderServer::UnregisterScanner(int scanner_id) {
  VLOG(2) << __func__;
  UnregisterInstanceBase(scanner_id);
  return Status::ok();
}

Status BluetoothLeScannerBinderServer::UnregisterAll() {
  VLOG(2) << __func__;
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothLeScannerBinderServer::StartScan(
    int scanner_id, const android::bluetooth::ScanSettings& settings,
    const std::vector<android::bluetooth::ScanFilter>& filters,
    bool* _aidl_return) {
  VLOG(2) << __func__ << " scanner_id: " << scanner_id;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto scanner = GetLEScanner(scanner_id);
  if (!scanner) {
    LOG(ERROR) << "Unknown scanner_id: " << scanner_id;
    *_aidl_return = false;
    return Status::ok();
  }

  std::vector<bluetooth::ScanFilter> flt;
  for (const auto& filter : filters) {
    flt.push_back(filter);
  }

  *_aidl_return = scanner->StartScan(settings, flt);
  return Status::ok();
}

Status BluetoothLeScannerBinderServer::StopScan(int scanner_id,
                                                bool* _aidl_return) {
  VLOG(2) << __func__ << " scanner_id: " << scanner_id;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto scanner = GetLEScanner(scanner_id);
  if (!scanner) {
    LOG(ERROR) << "Unknown scanner_id: " << scanner_id;
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = scanner->StopScan();
  return Status::ok();
}

void BluetoothLeScannerBinderServer::OnScanResult(
    bluetooth::LowEnergyScanner* scanner, const bluetooth::ScanResult& result) {
  VLOG(2) << __func__;
  std::lock_guard<std::mutex> lock(*maps_lock());

  int scanner_id = scanner->GetInstanceId();
  auto cb = GetLECallback(scanner->GetInstanceId());
  if (!cb.get()) {
    VLOG(2) << "Scanner was unregistered - scanner_id: " << scanner_id;
    return;
  }

  cb->OnScanResult(result);
}

android::sp<IBluetoothLeScannerCallback>
BluetoothLeScannerBinderServer::GetLECallback(int scanner_id) {
  auto cb = GetCallback(scanner_id);
  return android::sp<IBluetoothLeScannerCallback>(
      static_cast<IBluetoothLeScannerCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::LowEnergyScanner>
BluetoothLeScannerBinderServer::GetLEScanner(int scanner_id) {
  return std::static_pointer_cast<bluetooth::LowEnergyScanner>(
      GetInstance(scanner_id));
}

void BluetoothLeScannerBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " status: " << status;
  bluetooth::LowEnergyScanner* le_scanner =
      static_cast<bluetooth::LowEnergyScanner*>(instance);
  le_scanner->SetDelegate(this);

  android::sp<IBluetoothLeScannerCallback> cb(
      static_cast<IBluetoothLeScannerCallback*>(callback.get()));
  cb->OnScannerRegistered(status, (status == bluetooth::BLE_STATUS_SUCCESS)
                                      ? instance->GetInstanceId()
                                      : kInvalidInstanceId);
}

}  // namespace binder
}  // namespace ipc
