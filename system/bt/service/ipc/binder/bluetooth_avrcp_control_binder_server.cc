//
//  Copyright 2017 Google, Inc.
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
#include "service/ipc/binder/bluetooth_avrcp_control_binder_server.h"

#include <string>

#include "base/logging.h"

#include "service/adapter.h"

using android::String16;
using android::String8;
using android::binder::Status;
using android::bluetooth::IBluetoothAvrcpControlCallback;

namespace ipc {
namespace binder {

namespace {
const int kInvalidInstanceId = -1;
}  // namespace

BluetoothAvrcpControlBinderServer::BluetoothAvrcpControlBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
}

Status BluetoothAvrcpControlBinderServer::Register(
    const android::sp<IBluetoothAvrcpControlCallback>& callback,
    bool* _aidl_return) {
  VLOG(2) << __func__;

  bluetooth::AvrcpControlFactory* gatt_client_factory =
      adapter_->GetAvrcpControlFactory();

  *_aidl_return = RegisterInstanceBase(callback, gatt_client_factory);
  return Status::ok();
}

Status BluetoothAvrcpControlBinderServer::Unregister(int32_t id) {
  VLOG(2) << __func__;
  UnregisterInstanceBase(id);
  return Status::ok();
}

Status BluetoothAvrcpControlBinderServer::UnregisterAll() {
  VLOG(2) << __func__;
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothAvrcpControlBinderServer::Enable(int32_t id,
                                                 bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto avrcp_control = GetAvrcpControl(id);
  if (!avrcp_control) {
    LOG(ERROR) << "Failed to get avrcp control instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!avrcp_control->Enable()) {
    LOG(ERROR) << "Failed to enable";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothAvrcpControlBinderServer::Disable(int32_t id,
                                                  bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto avrcp_control = GetAvrcpControl(id);
  if (!avrcp_control) {
    LOG(ERROR) << "Failed to get avrcp control instance";
    *_aidl_return = false;
    return Status::ok();
  }

  avrcp_control->Disable();
  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothAvrcpControlBinderServer::SendPassThroughCommand(
    int32_t id, const String16& device_address, int32_t key_code,
    bool key_pressed, bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto avrcp_control = GetAvrcpControl(id);
  if (!avrcp_control) {
    LOG(ERROR) << "Failed to get avrcp control instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!avrcp_control->SendPassThroughCommand(String8(device_address).string(),
                                             key_code, key_pressed)) {
    LOG(ERROR) << "Failed to send pass through command";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

android::binder::Status BluetoothAvrcpControlBinderServer::SetAbsVolumeResponse(
    int32_t id, const android::String16& device_address, int32_t abs_vol,
    int32_t label, bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto avrcp_control = GetAvrcpControl(id);
  if (!avrcp_control) {
    LOG(ERROR) << "Failed to get avrcp control instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!avrcp_control->SetAbsVolumeResponse(String8(device_address).string(),
                                           abs_vol, label)) {
    LOG(ERROR) << "Failed to send set absolute volume response";
    *_aidl_return = false;
    return Status::ok();
  }
  *_aidl_return = true;
  return Status::ok();
}

android::binder::Status
BluetoothAvrcpControlBinderServer::RegisterForAbsVolumeCallbackResponse(
    int32_t id, const android::String16& device_address, int32_t response_type,
    int32_t abs_vol, int32_t label, bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto avrcp_control = GetAvrcpControl(id);
  if (!avrcp_control) {
    LOG(ERROR) << "Failed to get avrcp control instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!avrcp_control->RegisterForAbsVolumeCallbackResponse(
          String8(device_address).string(), response_type, abs_vol, label)) {
    LOG(ERROR)
        << "Failed to send register for absolute volume callback response";
    *_aidl_return = false;
    return Status::ok();
  }
  *_aidl_return = true;
  return Status::ok();
}

void BluetoothAvrcpControlBinderServer::OnConnectionState(
    bool rc_connect, bool bt_connect, const std::string& device_address) {
  auto func = [rc_connect, bt_connect, &device_address](IInterface* cb) {
    auto avrcp_control_cb = static_cast<IBluetoothAvrcpControlCallback*>(cb);
    avrcp_control_cb->OnConnectionState(rc_connect, bt_connect,
                                        String16(device_address.c_str()));
  };

  ForEachCallback(func);
}

void BluetoothAvrcpControlBinderServer::OnTrackChanged(
    const std::string& device_address, const bluetooth::AvrcpMediaAttr& attr) {
  auto binder_attr = android::bluetooth::BluetoothAvrcpMediaAttr(attr);

  auto func = [&device_address, &binder_attr](IInterface* cb) {
    auto avrcp_control_cb = static_cast<IBluetoothAvrcpControlCallback*>(cb);
    avrcp_control_cb->OnTrackChanged(String16(device_address.c_str()),
                                     binder_attr);
  };

  ForEachCallback(func);
}

void BluetoothAvrcpControlBinderServer::OnSetAbsVolumeRequest(
    const std::string& device_address, int32_t abs_vol, int32_t label) {
  auto addr_s16 = String16(device_address.c_str(), device_address.size());
  auto func = [&addr_s16, abs_vol, label](IInterface* cb) {
    auto avrcp_control_cb = static_cast<IBluetoothAvrcpControlCallback*>(cb);
    avrcp_control_cb->OnSetAbsVolumeRequest(addr_s16, abs_vol, label);
  };

  ForEachCallback(func);
}

void BluetoothAvrcpControlBinderServer::OnRegisterForAbsVolumeCallbackRequest(
    const std::string& device_address, int32_t label) {
  auto addr_s16 = String16(device_address.c_str(), device_address.size());
  auto func = [&addr_s16, label](IInterface* cb) {
    auto avrcp_control_cb = static_cast<IBluetoothAvrcpControlCallback*>(cb);
    avrcp_control_cb->OnRegisterForAbsVolumeCallbackRequest(addr_s16, label);
  };

  ForEachCallback(func);
}

std::shared_ptr<bluetooth::AvrcpControl>
BluetoothAvrcpControlBinderServer::GetAvrcpControl(int id) {
  return std::static_pointer_cast<bluetooth::AvrcpControl>(GetInstance(id));
}

void BluetoothAvrcpControlBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " client ID: " << instance->GetInstanceId()
          << " status: " << status;

  bluetooth::AvrcpControl* avrcp_control =
      static_cast<bluetooth::AvrcpControl*>(instance);
  avrcp_control->SetDelegate(this);

  android::sp<IBluetoothAvrcpControlCallback> cb(
      static_cast<IBluetoothAvrcpControlCallback*>(callback.get()));
  cb->OnRegistered(status, (status == bluetooth::BLE_STATUS_SUCCESS)
                               ? instance->GetInstanceId()
                               : kInvalidInstanceId);
}

}  // namespace binder
}  // namespace ipc
