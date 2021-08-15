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

#include "service/low_energy_client.h"

#include <base/logging.h>

#include "service/adapter.h"
#include "service/logging_helpers.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"

using std::lock_guard;
using std::mutex;

namespace bluetooth {

// LowEnergyClient implementation
// ========================================================

LowEnergyClient::LowEnergyClient(Adapter& adapter, const Uuid& uuid,
                                 int client_id)
    : adapter_(adapter),
      app_identifier_(uuid),
      client_id_(client_id),
      delegate_(nullptr) {}

LowEnergyClient::~LowEnergyClient() {
  // Automatically unregister the client.
  VLOG(1) << "LowEnergyClient unregistering client: " << client_id_;

  // Unregister as observer so we no longer receive any callbacks.
  hal::BluetoothGattInterface::Get()->RemoveClientObserver(this);

  hal::BluetoothGattInterface::Get()
      ->GetClientHALInterface()
      ->unregister_client(client_id_);
}

bool LowEnergyClient::Connect(const std::string& address, bool is_direct) {
  VLOG(2) << __func__ << "Address: " << address << " is_direct: " << is_direct;

  RawAddress bda;
  RawAddress::FromString(address, bda);

  bt_status_t status =
      hal::BluetoothGattInterface::Get()->GetClientHALInterface()->connect(
          client_id_, bda, is_direct, BT_TRANSPORT_LE, false, PHY_LE_1M_MASK);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "HAL call to connect failed";
    return false;
  }

  return true;
}

bool LowEnergyClient::Disconnect(const std::string& address) {
  VLOG(2) << __func__ << "Address: " << address;

  RawAddress bda;
  RawAddress::FromString(address, bda);

  std::map<const RawAddress, int>::iterator conn_id;
  {
    lock_guard<mutex> lock(connection_fields_lock_);
    conn_id = connection_ids_.find(bda);
    if (conn_id == connection_ids_.end()) {
      LOG(WARNING) << "Can't disconnect, no existing connection to " << address;
      return false;
    }
  }

  bt_status_t status =
      hal::BluetoothGattInterface::Get()->GetClientHALInterface()->disconnect(
          client_id_, bda, conn_id->second);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "HAL call to disconnect failed";
    return false;
  }

  return true;
}

bool LowEnergyClient::SetMtu(const std::string& address, int mtu) {
  VLOG(2) << __func__ << "Address: " << address << " MTU: " << mtu;

  RawAddress bda;
  RawAddress::FromString(address, bda);

  std::map<const RawAddress, int>::iterator conn_id;
  {
    lock_guard<mutex> lock(connection_fields_lock_);
    conn_id = connection_ids_.find(bda);
    if (conn_id == connection_ids_.end()) {
      LOG(WARNING) << "Can't set MTU, no existing connection to " << address;
      return false;
    }
  }

  bt_status_t status = hal::BluetoothGattInterface::Get()
                           ->GetClientHALInterface()
                           ->configure_mtu(conn_id->second, mtu);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "HAL call to set MTU failed";
    return false;
  }

  return true;
}

void LowEnergyClient::SetDelegate(Delegate* delegate) {
  lock_guard<mutex> lock(delegate_mutex_);
  delegate_ = delegate;
}

const Uuid& LowEnergyClient::GetAppIdentifier() const {
  return app_identifier_;
}

int LowEnergyClient::GetInstanceId() const { return client_id_; }

void LowEnergyClient::ConnectCallback(hal::BluetoothGattInterface* gatt_iface,
                                      int conn_id, int status, int client_id,
                                      const RawAddress& bda) {
  if (client_id != client_id_) return;

  VLOG(1) << __func__ << "client_id: " << client_id << " status: " << status;

  {
    lock_guard<mutex> lock(connection_fields_lock_);
    auto success = connection_ids_.emplace(bda, conn_id);
    if (!success.second) {
      LOG(ERROR) << __func__ << " Insertion into connection_ids_ failed!";
    }
  }

  if (delegate_)
    delegate_->OnConnectionState(this, status, BtAddrString(&bda).c_str(),
                                 true);
}

void LowEnergyClient::DisconnectCallback(
    hal::BluetoothGattInterface* gatt_iface, int conn_id, int status,
    int client_id, const RawAddress& bda) {
  if (client_id != client_id_) return;

  VLOG(1) << __func__ << " client_id: " << client_id << " status: " << status;
  {
    lock_guard<mutex> lock(connection_fields_lock_);
    if (!connection_ids_.erase(bda)) {
      LOG(ERROR) << __func__ << " Erasing from connection_ids_ failed!";
    }
  }

  if (delegate_)
    delegate_->OnConnectionState(this, status, BtAddrString(&bda).c_str(),
                                 false);
}

void LowEnergyClient::MtuChangedCallback(
    hal::BluetoothGattInterface* gatt_iface, int conn_id, int status, int mtu) {
  VLOG(1) << __func__ << " conn_id: " << conn_id << " status: " << status
          << " mtu: " << mtu;

  const RawAddress* bda = nullptr;
  {
    lock_guard<mutex> lock(connection_fields_lock_);
    for (auto& connection : connection_ids_) {
      if (connection.second == conn_id) {
        bda = &connection.first;
        break;
      }
    }
  }

  if (!bda) return;

  std::string addr = BtAddrString(bda);
  if (delegate_) delegate_->OnMtuChanged(this, status, addr.c_str(), mtu);
}

// LowEnergyClientFactory implementation
// ========================================================

LowEnergyClientFactory::LowEnergyClientFactory(Adapter& adapter)
    : adapter_(adapter) {
  hal::BluetoothGattInterface::Get()->AddClientObserver(this);
}

LowEnergyClientFactory::~LowEnergyClientFactory() {
  hal::BluetoothGattInterface::Get()->RemoveClientObserver(this);
}

bool LowEnergyClientFactory::RegisterInstance(
    const Uuid& uuid, const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  if (pending_calls_.find(uuid) != pending_calls_.end()) {
    LOG(ERROR) << "Low-Energy client with given Uuid already registered - "
               << "Uuid: " << uuid.ToString();
    return false;
  }

  const btgatt_client_interface_t* hal_iface =
      hal::BluetoothGattInterface::Get()->GetClientHALInterface();

  if (hal_iface->register_client(uuid) != BT_STATUS_SUCCESS) return false;

  pending_calls_[uuid] = callback;

  return true;
}

void LowEnergyClientFactory::RegisterClientCallback(
    hal::BluetoothGattInterface* gatt_iface, int status, int client_id,
    const bluetooth::Uuid& app_uuid) {
  Uuid uuid(app_uuid);

  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  auto iter = pending_calls_.find(uuid);
  if (iter == pending_calls_.end()) {
    VLOG(1) << "Ignoring callback for unknown app_id: " << uuid.ToString();
    return;
  }

  // No need to construct a client if the call wasn't successful.
  std::unique_ptr<LowEnergyClient> client;
  BLEStatus result = BLE_STATUS_FAILURE;
  if (status == BT_STATUS_SUCCESS) {
    client.reset(new LowEnergyClient(adapter_, uuid, client_id));

    gatt_iface->AddClientObserver(client.get());

    result = BLE_STATUS_SUCCESS;
  }

  // Notify the result via the result callback.
  iter->second(result, uuid, std::move(client));

  pending_calls_.erase(iter);
}

}  // namespace bluetooth
