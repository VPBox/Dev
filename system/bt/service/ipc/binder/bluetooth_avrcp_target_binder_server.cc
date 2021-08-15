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
#include "service/ipc/binder/bluetooth_avrcp_target_binder_server.h"

#include <string>

#include "base/logging.h"

#include "service/adapter.h"

#define AIDL_RET(value)      \
  do {                       \
    *_aidl_return = (value); \
    return Status::ok();     \
  } while (0)

#define TRY_GET_TARGET()                                    \
  ({                                                        \
    auto target = GetAvrcpTarget();                         \
    if (!target) {                                          \
      LOG(ERROR) << __func__ << ": "                        \
                 << "Failed to get AVRCP target interface"; \
      AIDL_RET(false);                                      \
    }                                                       \
    target;                                                 \
  })

#define TRY_GET_CB()                                           \
  ({                                                           \
    auto cb = GetAvrcpTargetCallback();                        \
    if (!cb.get()) {                                           \
      LOG(WARNING) << "Callback for AVRCP target was deleted"; \
      return;                                                  \
    }                                                          \
    cb;                                                        \
  })

#define TRY_RET(expr, msg) \
  do {                     \
    if (!(expr)) {         \
      LOG(ERROR) << msg;   \
      AIDL_RET(false);     \
    }                      \
    AIDL_RET(true);        \
  } while (0)

#define TRY_RET_FUNC(expr) TRY_RET(expr, __func__ << " failed")

using android::String16;
using android::String8;
using android::binder::Status;
using android::bluetooth::BluetoothAvrcpIntValue;
using android::bluetooth::IBluetoothAvrcpTargetCallback;

using LockGuard = std::lock_guard<std::mutex>;

namespace ipc {
namespace binder {

BluetoothAvrcpTargetBinderServer::BluetoothAvrcpTargetBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
}

BluetoothAvrcpTargetBinderServer::~BluetoothAvrcpTargetBinderServer() = default;

bool BluetoothAvrcpTargetBinderServer::HasInstance() {
  return GetAvrcpTarget() != nullptr;
}

Status BluetoothAvrcpTargetBinderServer::Register(
    const android::sp<IBluetoothAvrcpTargetCallback>& callback,
    bool* _aidl_return) {
  VLOG(2) << __func__;

  bluetooth::AvrcpTargetFactory* gatt_client_factory =
      adapter_->GetAvrcpTargetFactory();

  *_aidl_return = RegisterInstanceBase(callback, gatt_client_factory);
  return Status::ok();
}

Status BluetoothAvrcpTargetBinderServer::Unregister(int32_t id) {
  VLOG(2) << __func__;
  UnregisterInstanceBase(id);
  return Status::ok();
}

Status BluetoothAvrcpTargetBinderServer::UnregisterAll() {
  VLOG(2) << __func__;
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothAvrcpTargetBinderServer::Enable(bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(avrcp_target->Enable());
}

Status BluetoothAvrcpTargetBinderServer::Disable(bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  avrcp_target->Disable();
  AIDL_RET(true);
}

Status BluetoothAvrcpTargetBinderServer::GetPlayStatusResponse(
    const android::String16& addr, int32_t play_status, int32_t song_len,
    int32_t song_pos, bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(avrcp_target->GetPlayStatusResponse(
      String8(addr).string(), play_status, song_len, song_pos));
}

Status BluetoothAvrcpTargetBinderServer::ListPlayerAppAttrResponse(
    const android::String16& addr, const std::vector<int32_t>& attrs,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(
      avrcp_target->ListPlayerAppAttrResponse(String8(addr).string(), attrs));
}

Status BluetoothAvrcpTargetBinderServer::GetPlayerAppValueResponse(
    const android::String16& addr,
    const std::vector<::android::bluetooth::BluetoothAvrcpIntValue>& values,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  std::vector<bluetooth::AvrcpIntValue> non_binder;
  non_binder.reserve(values.size());
  for (const auto& val : values) {
    non_binder.push_back(val);
  }
  TRY_RET_FUNC(avrcp_target->GetPlayerAppValueResponse(String8(addr).string(),
                                                       non_binder));
}

Status BluetoothAvrcpTargetBinderServer::GetPlayerAppAttrTextResponse(
    const android::String16& addr,
    const std::vector<::android::bluetooth::BluetoothAvrcpStringValue>& attrs,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  std::vector<bluetooth::AvrcpStringValue> non_binder;
  non_binder.reserve(attrs.size());
  for (const auto& val : attrs) {
    non_binder.push_back(val);
  }
  TRY_RET_FUNC(avrcp_target->GetPlayerAppAttrTextResponse(
      String8(addr).string(), non_binder));
}

Status BluetoothAvrcpTargetBinderServer::GetPlayerAppValueTextResponse(
    const android::String16& addr,
    const std::vector<::android::bluetooth::BluetoothAvrcpStringValue>& values,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  std::vector<bluetooth::AvrcpStringValue> non_binder;
  non_binder.reserve(values.size());
  for (const auto& val : values) {
    non_binder.push_back(val);
  }
  TRY_RET_FUNC(avrcp_target->GetPlayerAppValueTextResponse(
      String8(addr).string(), non_binder));
}

Status BluetoothAvrcpTargetBinderServer::GetElementAttrResponse(
    const android::String16& addr,
    const std::vector<::android::bluetooth::BluetoothAvrcpStringValue>& attrs,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  std::vector<bluetooth::AvrcpStringValue> non_binder;
  non_binder.reserve(attrs.size());
  for (const auto& val : attrs) {
    non_binder.push_back(val);
  }
  TRY_RET_FUNC(
      avrcp_target->GetElementAttrResponse(String8(addr).string(), non_binder));
}

Status BluetoothAvrcpTargetBinderServer::SetPlayerAppValueResponse(
    const android::String16& addr, int32_t rsp_status, bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(avrcp_target->SetPlayerAppValueResponse(String8(addr).string(),
                                                       rsp_status));
}

Status BluetoothAvrcpTargetBinderServer::RegisterNotificationResponse(
    int32_t event_id, int32_t type,
    const ::android::bluetooth::BluetoothAvrcpRegisterNotificationResponse&
        param,
    bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(
      avrcp_target->RegisterNotificationResponse(event_id, type, param));
}

Status BluetoothAvrcpTargetBinderServer::SetVolume(int32_t volume,
                                                   bool* _aidl_return) {
  auto avrcp_target = TRY_GET_TARGET();
  TRY_RET_FUNC(avrcp_target->SetVolume(volume));
}

void BluetoothAvrcpTargetBinderServer::OnGetRemoteFeatures(
    const std::string& addr, int32_t features) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetRemoteFeatures(String16(addr.data(), addr.size()), features);
}

void BluetoothAvrcpTargetBinderServer::OnGetPlayStatus(
    const std::string& addr) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetPlayStatus(String16(addr.data(), addr.size()));
}

void BluetoothAvrcpTargetBinderServer::OnListPlayerAppAttr(
    const std::string& addr) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnListPlayerAppAttr(String16(addr.data(), addr.size()));
}

void BluetoothAvrcpTargetBinderServer::OnListPlayerAppValues(
    const std::string& addr, int32_t attr_id) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnListPlayerAppValues(String16(addr.data(), addr.size()), attr_id);
}

void BluetoothAvrcpTargetBinderServer::OnGetPlayerAppValue(
    const std::string& addr, const std::vector<int32_t>& attrs) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetPlayerAppValue(String16(addr.data(), addr.size()), attrs);
}

void BluetoothAvrcpTargetBinderServer::OnGetPlayerAppAttrsText(
    const std::string& addr, const std::vector<int32_t>& attrs) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetPlayerAppAttrsText(String16(addr.data(), addr.size()), attrs);
}

void BluetoothAvrcpTargetBinderServer::OnGetPlayerAppValuesText(
    const std::string& addr, int32_t attr_id,
    const std::vector<int32_t>& values) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetPlayerAppValuesText(String16(addr.data(), addr.size()), attr_id,
                               values);
}

void BluetoothAvrcpTargetBinderServer::OnSetPlayerAppValue(
    const std::string& addr,
    const std::vector<bluetooth::AvrcpIntValue>& values) {
  std::vector<BluetoothAvrcpIntValue> binder_values;
  binder_values.reserve(values.size());
  for (const auto& val : values) {
    binder_values.push_back(val);
  }

  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnSetPlayerAppValue(String16(addr.data(), addr.size()), binder_values);
}

void BluetoothAvrcpTargetBinderServer::OnGetElementAttrs(
    const std::string& addr, const std::vector<int32_t>& attrs) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnGetElementAttrs(String16(addr.data(), addr.size()), attrs);
}

void BluetoothAvrcpTargetBinderServer::OnRegisterNotification(
    const std::string& addr, int32_t event_id, uint32_t param) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnRegisterNotification(String16(addr.data(), addr.size()), event_id,
                             param);
}

void BluetoothAvrcpTargetBinderServer::OnVolumeChange(const std::string& addr,
                                                      int32_t volume,
                                                      int32_t ctype) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnVolumeChange(String16(addr.data(), addr.size()), volume, ctype);
}

void BluetoothAvrcpTargetBinderServer::OnPassThroughCommand(
    const std::string& addr, int32_t id, int32_t key_state) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnPassThroughCommand(String16(addr.data(), addr.size()), id, key_state);
}

android::sp<IBluetoothAvrcpTargetCallback>
BluetoothAvrcpTargetBinderServer::GetAvrcpTargetCallback() {
  auto cb = GetCallback(bluetooth::AvrcpTarget::kSingletonInstanceId);
  return android::sp<IBluetoothAvrcpTargetCallback>(
      static_cast<IBluetoothAvrcpTargetCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::AvrcpTarget>
BluetoothAvrcpTargetBinderServer::GetAvrcpTarget() {
  return std::static_pointer_cast<bluetooth::AvrcpTarget>(
      GetInstance(bluetooth::AvrcpTarget::kSingletonInstanceId));
}

void BluetoothAvrcpTargetBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " client ID: " << instance->GetInstanceId()
          << " status: " << status;

  bluetooth::AvrcpTarget* avrcp_target =
      static_cast<bluetooth::AvrcpTarget*>(instance);
  avrcp_target->SetDelegate(this);

  android::sp<IBluetoothAvrcpTargetCallback> cb(
      static_cast<IBluetoothAvrcpTargetCallback*>(callback.get()));
  cb->OnRegistered(status);
}

}  // namespace binder
}  // namespace ipc
