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

#include "service/ipc/binder/bluetooth_low_energy_binder_server.h"

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

BluetoothLowEnergyBinderServer::BluetoothLowEnergyBinderServer(
    bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
}

BluetoothLowEnergyBinderServer::~BluetoothLowEnergyBinderServer() {}

Status BluetoothLowEnergyBinderServer::RegisterClient(
    const android::sp<IBluetoothLowEnergyCallback>& callback,
    bool* _aidl_return) {
  VLOG(2) << __func__;
  bluetooth::LowEnergyClientFactory* ble_factory =
      adapter_->GetLowEnergyClientFactory();

  *_aidl_return = RegisterInstanceBase(callback, ble_factory);
  return Status::ok();
}

Status BluetoothLowEnergyBinderServer::UnregisterClient(int client_id) {
  VLOG(2) << __func__;
  UnregisterInstanceBase(client_id);
  return Status::ok();
}

Status BluetoothLowEnergyBinderServer::UnregisterAll() {
  VLOG(2) << __func__;
  UnregisterAllBase();
  return Status::ok();
}

Status BluetoothLowEnergyBinderServer::Connect(int client_id,
                                               const String16& address,
                                               bool is_direct,
                                               bool* _aidl_return) {
  VLOG(2) << __func__ << " client_id: " << client_id << " address: " << address
          << " is_direct: " << is_direct;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto client = GetLEClient(client_id);
  if (!client) {
    LOG(ERROR) << "Unknown client_id: " << client_id;
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return =
      client->Connect(std::string(String8(address).string()), is_direct);
  return Status::ok();
}

Status BluetoothLowEnergyBinderServer::Disconnect(int client_id,
                                                  const String16& address,
                                                  bool* _aidl_return) {
  VLOG(2) << __func__ << " client_id: " << client_id << " address: " << address;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto client = GetLEClient(client_id);
  if (!client) {
    LOG(ERROR) << "Unknown client_id: " << client_id;
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = client->Disconnect(std::string(String8(address).string()));
  return Status::ok();
}

Status BluetoothLowEnergyBinderServer::SetMtu(int client_id,
                                              const String16& address, int mtu,
                                              bool* _aidl_return) {
  VLOG(2) << __func__ << " client_id: " << client_id << " address: " << address
          << " mtu: " << mtu;
  std::lock_guard<std::mutex> lock(*maps_lock());

  auto client = GetLEClient(client_id);
  if (!client) {
    LOG(ERROR) << "Unknown client_id: " << client_id;
    *_aidl_return = false;
    return Status::ok();
  }

  *_aidl_return = client->SetMtu(std::string(String8(address).string()), mtu);
  return Status::ok();
}

void BluetoothLowEnergyBinderServer::OnConnectionState(
    bluetooth::LowEnergyClient* client, int status, const char* address,
    bool connected) {
  VLOG(2) << __func__ << " address: " << address << " connected: " << connected;

  int client_id = client->GetInstanceId();
  auto cb = GetLECallback(client->GetInstanceId());
  if (!cb.get()) {
    VLOG(2) << "Client was unregistered - client_id: " << client_id;
    return;
  }

  cb->OnConnectionState(status, client_id,
                        String16(address, std::strlen(address)), connected);
}

void BluetoothLowEnergyBinderServer::OnMtuChanged(
    bluetooth::LowEnergyClient* client, int status, const char* address,
    int mtu) {
  VLOG(2) << __func__ << " address: " << address << " status: " << status
          << " mtu: " << mtu;

  int client_id = client->GetInstanceId();
  auto cb = GetLECallback(client_id);
  if (!cb.get()) {
    VLOG(2) << "Client was unregistered - client_id: " << client_id;
    return;
  }

  cb->OnMtuChanged(status, String16(address, std::strlen(address)), mtu);
}

android::sp<IBluetoothLowEnergyCallback>
BluetoothLowEnergyBinderServer::GetLECallback(int client_id) {
  auto cb = GetCallback(client_id);
  return android::sp<IBluetoothLowEnergyCallback>(
      static_cast<IBluetoothLowEnergyCallback*>(cb.get()));
}

std::shared_ptr<bluetooth::LowEnergyClient>
BluetoothLowEnergyBinderServer::GetLEClient(int client_id) {
  return std::static_pointer_cast<bluetooth::LowEnergyClient>(
      GetInstance(client_id));
}

void BluetoothLowEnergyBinderServer::OnRegisterInstanceImpl(
    bluetooth::BLEStatus status, android::sp<IInterface> callback,
    bluetooth::BluetoothInstance* instance) {
  VLOG(1) << __func__ << " status: " << status;
  bluetooth::LowEnergyClient* le_client =
      static_cast<bluetooth::LowEnergyClient*>(instance);
  le_client->SetDelegate(this);

  android::sp<IBluetoothLowEnergyCallback> cb(
      static_cast<IBluetoothLowEnergyCallback*>(callback.get()));
  cb->OnClientRegistered(status, (status == bluetooth::BLE_STATUS_SUCCESS)
                                     ? instance->GetInstanceId()
                                     : kInvalidInstanceId);
}

}  // namespace binder
}  // namespace ipc
