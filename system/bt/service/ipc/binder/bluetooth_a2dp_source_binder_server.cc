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
#include "service/ipc/binder/bluetooth_a2dp_source_binder_server.h"

#include <base/logging.h>

#include "service/adapter.h"

#define AIDL_RET(value)      \
  do {                       \
    *_aidl_return = (value); \
    return Status::ok();     \
  } while (0)

#define TRY_GET_SOURCE()                                   \
  ({                                                       \
    auto source = GetA2dpSource();                         \
    if (!source) {                                         \
      LOG(ERROR) << __func__ << ": "                       \
                 << "Failed to get A2DP source interface"; \
      AIDL_RET(false);                                     \
    }                                                      \
    source;                                                \
  })

#define TRY_GET_CB()                                          \
  ({                                                          \
    auto cb = GetA2dpSourceCallback();                        \
    if (!cb.get()) {                                          \
      LOG(WARNING) << "Callback for A2DP SOURCE was deleted"; \
      return;                                                 \
    }                                                         \
    cb;                                                       \
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
using android::bluetooth::BluetoothA2dpCodecConfig;
using android::bluetooth::IBluetoothA2dpSourceCallback;

using LockGuard = std::lock_guard<std::mutex>;

namespace ipc {
namespace binder {

namespace {

std::vector<bluetooth::A2dpCodecConfig> A2dpCodecsFromBinder(
    const std::vector<BluetoothA2dpCodecConfig>& codecs) {
  std::vector<bluetooth::A2dpCodecConfig> ret;
  ret.reserve(codecs.size());
  for (const auto& config : codecs) {
    ret.push_back(config);
  }

  return ret;
}

std::vector<BluetoothA2dpCodecConfig> A2dpCodecsToBinder(
    const std::vector<bluetooth::A2dpCodecConfig>& codecs) {
  std::vector<BluetoothA2dpCodecConfig> ret;
  ret.reserve(codecs.size());
  for (const auto& config : codecs) {
    ret.push_back(config);
  }

  return ret;
}

}  // namespace

BluetoothA2dpSourceBinderServer::BluetoothA2dpSourceBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter);
}

BluetoothA2dpSourceBinderServer::~BluetoothA2dpSourceBinderServer() = default;

bool BluetoothA2dpSourceBinderServer::HasInstance() {
  return GetA2dpSource() != nullptr;
}

Status BluetoothA2dpSourceBinderServer::Register(
    const android::sp<IBluetoothA2dpSourceCallback>& callback,
    bool* _aidl_return) {
  auto factory = adapter_->GetA2dpSourceFactory();
  *_aidl_return = RegisterInstanceBase(callback, factory);
  return Status::ok();
}

Status BluetoothA2dpSourceBinderServer::Unregister() {
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothA2dpSourceBinderServer::Enable(
    const std::vector<android::bluetooth::BluetoothA2dpCodecConfig>&
        codec_priorities,
    bool* _aidl_return) {
  auto codec_priorities_non_binder = A2dpCodecsFromBinder(codec_priorities);

  LockGuard lock(*maps_lock());
  auto a2dp_source = TRY_GET_SOURCE();
  TRY_RET_FUNC(a2dp_source->Enable(codec_priorities_non_binder));
}

Status BluetoothA2dpSourceBinderServer::Disable(bool* _aidl_return) {
  LockGuard lock(*maps_lock());
  auto a2dp_source = TRY_GET_SOURCE();
  a2dp_source->Disable();
  AIDL_RET(true);
}

Status BluetoothA2dpSourceBinderServer::Connect(const String16& device_address,
                                                bool* _aidl_return) {
  LockGuard lock(*maps_lock());
  auto a2dp_source = TRY_GET_SOURCE();
  TRY_RET_FUNC(a2dp_source->Connect(String8(device_address).string()));
}

Status BluetoothA2dpSourceBinderServer::Disconnect(
    const String16& device_address, bool* _aidl_return) {
  LockGuard lock(*maps_lock());
  auto a2dp_source = TRY_GET_SOURCE();
  TRY_RET_FUNC(a2dp_source->Disconnect(String8(device_address).string()));
}

Status BluetoothA2dpSourceBinderServer::ConfigCodec(
    const android::String16& device_address,
    const std::vector<android::bluetooth::BluetoothA2dpCodecConfig>&
        codec_preferences,
    bool* _aidl_return) {
  auto codec_preferences_non_binder = A2dpCodecsFromBinder(codec_preferences);

  LockGuard lock(*maps_lock());
  auto a2dp_source = TRY_GET_SOURCE();
  TRY_RET_FUNC(a2dp_source->ConfigCodec(String8(device_address).string(),
                                        codec_preferences_non_binder));
}

void BluetoothA2dpSourceBinderServer::OnConnectionState(
    const std::string& device_address, int state) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnConnectionState(String16(device_address.c_str()), state);
}

void BluetoothA2dpSourceBinderServer::OnAudioState(
    const std::string& device_address, int state) {
  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnAudioState(String16(device_address.c_str()), state);
}

void BluetoothA2dpSourceBinderServer::OnAudioConfig(
    const std::string& device_address, bluetooth::A2dpCodecConfig codec_config,
    const std::vector<bluetooth::A2dpCodecConfig>& codecs_local_capabilities,
    const std::vector<bluetooth::A2dpCodecConfig>&
        codecs_selectable_capabilities) {
  auto binder_codecs_local_capabilities =
      A2dpCodecsToBinder(codecs_local_capabilities);
  auto binder_codecs_selectable_capabilities =
      A2dpCodecsToBinder(codecs_selectable_capabilities);

  LockGuard lock(*maps_lock());
  auto cb = TRY_GET_CB();
  cb->OnAudioConfig(String16(device_address.c_str()), codec_config,
                    binder_codecs_local_capabilities,
                    binder_codecs_selectable_capabilities);
}

android::sp<IBluetoothA2dpSourceCallback>
BluetoothA2dpSourceBinderServer::GetA2dpSourceCallback() {
  auto cb = GetCallback(bluetooth::A2dpSource::kSingletonInstanceId);
  return android::sp<IBluetoothA2dpSourceCallback>(
      static_cast<IBluetoothA2dpSourceCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::A2dpSource>
BluetoothA2dpSourceBinderServer::GetA2dpSource() {
  return std::static_pointer_cast<bluetooth::A2dpSource>(
      GetInstance(bluetooth::A2dpSource::kSingletonInstanceId));
}

void BluetoothA2dpSourceBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " instance ID: " << instance->GetInstanceId()
          << " status: " << status;
  bluetooth::A2dpSource* a2dp_source =
      static_cast<bluetooth::A2dpSource*>(instance);
  a2dp_source->SetDelegate(this);

  android::sp<IBluetoothA2dpSourceCallback> cb(
      static_cast<IBluetoothA2dpSourceCallback*>(callback.get()));
  cb->OnRegistered(status);
}

}  // namespace binder
}  // namespace ipc
