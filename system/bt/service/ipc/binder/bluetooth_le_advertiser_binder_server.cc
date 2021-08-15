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

#include "service/ipc/binder/bluetooth_le_advertiser_binder_server.h"

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

BluetoothLeAdvertiserBinderServer::BluetoothLeAdvertiserBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
}

BluetoothLeAdvertiserBinderServer::~BluetoothLeAdvertiserBinderServer() {}

Status BluetoothLeAdvertiserBinderServer::RegisterAdvertiser(
    const android::sp<IBluetoothLeAdvertiserCallback>& callback,
    bool* _aidl_return) {
  VLOG(2) << __func__;
  bluetooth::LowEnergyAdvertiserFactory* adv_factory =
      adapter_->GetLeAdvertiserFactory();

  *_aidl_return = RegisterInstanceBase(callback, adv_factory);
  return Status::ok();
}

Status BluetoothLeAdvertiserBinderServer::UnregisterAdvertiser(
    int advertiser_id) {
  VLOG(2) << __func__;
  UnregisterInstanceBase(advertiser_id);
  return Status::ok();
}

Status BluetoothLeAdvertiserBinderServer::UnregisterAll() {
  VLOG(2) << __func__;
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothLeAdvertiserBinderServer::StartMultiAdvertising(
    int advertiser_id, const android::bluetooth::AdvertiseData& advertise_data,
    const android::bluetooth::AdvertiseData& scan_response,
    const android::bluetooth::AdvertiseSettings& settings, bool* _aidl_return) {
  VLOG(2) << __func__ << " advertiser_id: " << advertiser_id;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto advertiser = GetLEAdvertiser(advertiser_id);
  if (!advertiser) {
    LOG(ERROR) << "Unknown advertiser_id: " << advertiser_id;
    *_aidl_return = false;
    return Status::ok();
  }

  // Create a weak pointer and pass that to the callback to prevent a potential
  // use after free.
  android::wp<BluetoothLeAdvertiserBinderServer> weak_ptr_to_this(this);
  auto settings_copy = settings;
  auto callback = [=](bluetooth::BLEStatus status) {
    auto sp_to_this = weak_ptr_to_this.promote();
    if (!sp_to_this.get()) {
      VLOG(2) << "BluetoothLeAdvertiserBinderServer was deleted";
      return;
    }

    std::lock_guard<std::mutex> lock(*maps_lock());

    auto cb = GetLECallback(advertiser_id);
    if (!cb.get()) {
      VLOG(1) << "Advertiser was removed before callback: " << advertiser_id;
      return;
    }

    cb->OnMultiAdvertiseCallback(status, true /* is_start */, settings_copy);
  };

  if (!advertiser->StartAdvertising(settings, advertise_data, scan_response,
                                    callback)) {
    LOG(ERROR) << "Failed to initiate call to start advertising";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothLeAdvertiserBinderServer::StopMultiAdvertising(
    int advertiser_id, bool* _aidl_return) {
  VLOG(2) << __func__;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto advertiser = GetLEAdvertiser(advertiser_id);
  if (!advertiser) {
    LOG(ERROR) << "Unknown advertiser_id: " << advertiser_id;
    *_aidl_return = false;
    return Status::ok();
  }

  // Create a weak pointer and pass that to the callback to prevent a potential
  // use after free.
  android::wp<BluetoothLeAdvertiserBinderServer> weak_ptr_to_this(this);
  auto settings_copy = advertiser->advertise_settings();
  auto callback = [=](bluetooth::BLEStatus status) {
    auto sp_to_this = weak_ptr_to_this.promote();
    if (!sp_to_this.get()) {
      VLOG(2) << "BluetoothLeAdvertiserBinderServer was deleted";
      return;
    }

    auto cb = GetLECallback(advertiser_id);
    if (!cb.get()) {
      VLOG(2) << "Advertiser was unregistered - advertiser_id: "
              << advertiser_id;
      return;
    }

    std::lock_guard<std::mutex> lock(*maps_lock());

    cb->OnMultiAdvertiseCallback(status, false /* is_start */, settings_copy);
  };

  if (!advertiser->StopAdvertising(callback)) {
    LOG(ERROR) << "Failed to initiate call to start advertising";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

android::sp<IBluetoothLeAdvertiserCallback>
BluetoothLeAdvertiserBinderServer::GetLECallback(int advertiser_id) {
  auto cb = GetCallback(advertiser_id);
  return android::sp<IBluetoothLeAdvertiserCallback>(
      static_cast<IBluetoothLeAdvertiserCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::LowEnergyAdvertiser>
BluetoothLeAdvertiserBinderServer::GetLEAdvertiser(int advertiser_id) {
  return std::static_pointer_cast<bluetooth::LowEnergyAdvertiser>(
      GetInstance(advertiser_id));
}

void BluetoothLeAdvertiserBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " status: " << status;

  android::sp<IBluetoothLeAdvertiserCallback> cb(
      static_cast<IBluetoothLeAdvertiserCallback*>(callback.get()));
  cb->OnAdvertiserRegistered(status, (status == bluetooth::BLE_STATUS_SUCCESS)
                                         ? instance->GetInstanceId()
                                         : kInvalidInstanceId);
}

}  // namespace binder
}  // namespace ipc
