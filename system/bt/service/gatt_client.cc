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

#include "service/gatt_client.h"

#include <base/logging.h>

using std::lock_guard;
using std::mutex;

namespace bluetooth {

// GattClient implementation
// ========================================================

GattClient::GattClient(const Uuid& uuid, int client_id)
    : app_identifier_(uuid), client_id_(client_id) {}

GattClient::~GattClient() {
  // Automatically unregister the client.
  VLOG(1) << "GattClient unregistering client: " << client_id_;

  hal::BluetoothGattInterface::Get()
      ->GetClientHALInterface()
      ->unregister_client(client_id_);
}

const Uuid& GattClient::GetAppIdentifier() const { return app_identifier_; }

int GattClient::GetInstanceId() const { return client_id_; }

// GattClientFactory implementation
// ========================================================

GattClientFactory::GattClientFactory() {
  hal::BluetoothGattInterface::Get()->AddClientObserver(this);
}

GattClientFactory::~GattClientFactory() {
  hal::BluetoothGattInterface::Get()->RemoveClientObserver(this);
}

bool GattClientFactory::RegisterInstance(const Uuid& uuid,
                                         const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  if (pending_calls_.find(uuid) != pending_calls_.end()) {
    LOG(ERROR) << "GATT client with given Uuid already registered - "
               << "Uuid: " << uuid.ToString();
    return false;
  }

  const btgatt_client_interface_t* hal_iface =
      hal::BluetoothGattInterface::Get()->GetClientHALInterface();

  if (hal_iface->register_client(uuid) != BT_STATUS_SUCCESS) return false;

  pending_calls_[uuid] = callback;

  return true;
}

void GattClientFactory::RegisterClientCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int status, int client_id,
    const Uuid& app_uuid) {
  Uuid uuid(app_uuid);

  auto iter = pending_calls_.find(uuid);
  if (iter == pending_calls_.end()) {
    VLOG(1) << "Ignoring callback for unknown app_id: " << uuid.ToString();
    return;
  }

  bool success = (status == BT_STATUS_SUCCESS);
  BLEStatus result = success ? BLE_STATUS_SUCCESS : BLE_STATUS_FAILURE;

  // No need to construct a client if the call wasn't successful.
  std::unique_ptr<GattClient> client;
  if (success) client.reset(new GattClient(uuid, client_id));

  // Notify the result via the result callback.
  iter->second(result, uuid, std::move(client));

  pending_calls_.erase(iter);
}

}  // namespace bluetooth
