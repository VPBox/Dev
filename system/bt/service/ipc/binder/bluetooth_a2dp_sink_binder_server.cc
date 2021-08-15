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
#include "service/ipc/binder/bluetooth_a2dp_sink_binder_server.h"

#include <base/logging.h>

#include "service/adapter.h"

using android::String16;
using android::String8;
using android::binder::Status;

using android::bluetooth::IBluetoothA2dpSinkCallback;

namespace ipc {
namespace binder {

BluetoothA2dpSinkBinderServer::BluetoothA2dpSinkBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter);
}

Status BluetoothA2dpSinkBinderServer::Register(
    const android::sp<IBluetoothA2dpSinkCallback>& callback,
    bool* _aidl_return) {
  auto factory = adapter_->GetA2dpSinkFactory();
  *_aidl_return = RegisterInstanceBase(callback, factory);
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::Unregister() {
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::Enable(bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP sink instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!a2dp_sink->Enable()) {
    LOG(ERROR) << "Failed to enable";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::Disable(bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP sink instance";
    *_aidl_return = false;
    return Status::ok();
  }

  a2dp_sink->Disable();
  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::Connect(const String16& device_address,
                                              bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP sink instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!a2dp_sink->Connect(String8(device_address).string())) {
    LOG(ERROR) << "Failed to connect";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::Disconnect(const String16& device_address,
                                                 bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP sink instance";
    *_aidl_return = false;
    return Status::ok();
  }

  if (!a2dp_sink->Disconnect(String8(device_address).string())) {
    LOG(ERROR) << "Failed to connect";
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::SetAudioFocusState(int focus_state,
                                                         bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP instance";
    *_aidl_return = false;
    return Status::ok();
  }

  a2dp_sink->SetAudioFocusState(focus_state);
  *_aidl_return = true;
  return Status::ok();
}

Status BluetoothA2dpSinkBinderServer::SetAudioTrackGain(float gain,
                                                        bool* _aidl_return) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto a2dp_sink = GetA2dpSink();
  if (!a2dp_sink) {
    LOG(ERROR) << "Failed to get A2DP instance";
    *_aidl_return = false;
    return Status::ok();
  }

  a2dp_sink->SetAudioTrackGain(gain);
  *_aidl_return = true;
  return Status::ok();
}

void BluetoothA2dpSinkBinderServer::OnConnectionState(
    const std::string& device_address, int state) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto cb = GetA2dpSinkCallback();
  if (!cb.get()) {
    LOG(WARNING) << "Callback for this GattServer was deleted.";
    return;
  }

  cb->OnConnectionState(String16(device_address.c_str()), state);
}

void BluetoothA2dpSinkBinderServer::OnAudioState(
    const std::string& device_address, int state) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto cb = GetA2dpSinkCallback();
  if (!cb.get()) {
    LOG(WARNING) << "Callback for this GattServer was deleted.";
    return;
  }

  cb->OnAudioState(String16(device_address.c_str()), state);
}

void BluetoothA2dpSinkBinderServer::OnAudioConfig(
    const std::string& device_address, uint32_t sample_rate,
    uint8_t channel_count) {
  std::lock_guard<std::mutex> lock(*maps_lock());
  auto cb = GetA2dpSinkCallback();
  if (!cb.get()) {
    LOG(WARNING) << "Callback for this GattServer was deleted.";
    return;
  }

  cb->OnAudioConfig(String16(device_address.c_str()), sample_rate,
                    channel_count);
}

bool BluetoothA2dpSinkBinderServer::HasInstance() {
  return GetA2dpSink() != nullptr;
}

android::sp<IBluetoothA2dpSinkCallback>
BluetoothA2dpSinkBinderServer::GetA2dpSinkCallback() {
  auto cb = GetCallback(bluetooth::A2dpSink::kSingletonInstanceId);
  return android::sp<IBluetoothA2dpSinkCallback>(
      static_cast<IBluetoothA2dpSinkCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::A2dpSink>
BluetoothA2dpSinkBinderServer::GetA2dpSink() {
  return std::static_pointer_cast<bluetooth::A2dpSink>(
      GetInstance(bluetooth::A2dpSink::kSingletonInstanceId));
}

void BluetoothA2dpSinkBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " instance ID: " << instance->GetInstanceId()
          << " status: " << status;
  bluetooth::A2dpSink* a2dp_sink = static_cast<bluetooth::A2dpSink*>(instance);
  a2dp_sink->SetDelegate(this);

  android::sp<IBluetoothA2dpSinkCallback> cb(
      static_cast<IBluetoothA2dpSinkCallback*>(callback.get()));
  cb->OnRegistered(status);
}

}  // namespace binder
}  // namespace ipc
