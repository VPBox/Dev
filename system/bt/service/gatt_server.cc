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

#include "service/gatt_server.h"

#include <base/logging.h>

#include "service/logging_helpers.h"
#include "stack/include/bt_types.h"

using std::lock_guard;
using std::mutex;

namespace bluetooth {

// GattServer implementation
// ========================================================

GattServer::GattServer(const Uuid& uuid, int server_id)
    : app_identifier_(uuid), server_id_(server_id), delegate_(nullptr) {}

GattServer::~GattServer() {
  // Automatically unregister the server.
  VLOG(1) << "GattServer unregistering: " << server_id_;

  // Unregister as observer so we no longer receive any callbacks.
  hal::BluetoothGattInterface::Get()->RemoveServerObserver(this);

  // Unregister this server, stop all services, and ignore the result.
  // TODO(armansito): stop and remove all services here? unregister_server
  // should really take care of that.
  hal::BluetoothGattInterface::Get()
      ->GetServerHALInterface()
      ->unregister_server(server_id_);
}

void GattServer::SetDelegate(Delegate* delegate) {
  lock_guard<mutex> lock(mutex_);
  delegate_ = delegate;
}

const Uuid& GattServer::GetAppIdentifier() const { return app_identifier_; }

int GattServer::GetInstanceId() const { return server_id_; }

bool GattServer::AddService(const bluetooth::Service& service,
                            const ResultCallback& callback) {
  VLOG(1) << __func__ << " server_id: " << server_id_;
  lock_guard<mutex> lock(mutex_);

  if (!callback) {
    LOG(ERROR) << "|callback| cannot be NULL";
    return false;
  }

  std::vector<btgatt_db_element_t> svc;

  svc.push_back({.type = (service.primary() ? BTGATT_DB_PRIMARY_SERVICE
                                            : BTGATT_DB_SECONDARY_SERVICE),
                 .uuid = service.uuid()});

  for (const auto& characteristic : service.characteristics()) {
    svc.push_back({.type = BTGATT_DB_CHARACTERISTIC,
                   .uuid = characteristic.uuid(),
                   .properties = characteristic.properties(),
                   .permissions = characteristic.permissions()});
    for (const auto& descriptor : characteristic.descriptors())
      svc.push_back({.type = BTGATT_DB_DESCRIPTOR,
                     .uuid = descriptor.uuid(),
                     .permissions = descriptor.permissions()});
  }

  for (const auto& incl_svc : service.included_services())
    svc.push_back({.type = BTGATT_DB_INCLUDED_SERVICE,
                   .attribute_handle = incl_svc.handle()});

  pending_end_decl_cb_ = callback;

  bt_status_t status =
      hal::BluetoothGattInterface::Get()->GetServerHALInterface()->add_service(
          server_id_, svc);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to initiate call to populate GATT service";
    CleanUpPendingData();
    return false;
  }

  return true;
}

bool GattServer::SendResponse(const std::string& device_address, int request_id,
                              GATTError error, int offset,
                              const std::vector<uint8_t>& value) {
  VLOG(1) << __func__ << " - server_id: " << server_id_
          << " device_address: " << device_address
          << " request_id: " << request_id << " error: " << error
          << " offset: " << offset;
  lock_guard<mutex> lock(mutex_);

  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  if (value.size() + offset > BTGATT_MAX_ATTR_LEN) {
    LOG(ERROR) << "Value is too large";
    return false;
  }

  // Find the correct connection ID for |device_address| and |request_id|.
  auto iter = conn_addr_map_.find(device_address);
  if (iter == conn_addr_map_.end()) {
    LOG(ERROR) << "No known connections for device address: " << device_address;
    return false;
  }

  std::shared_ptr<Connection> connection;
  for (const auto& tmp : iter->second) {
    if (tmp->request_id_to_handle.find(request_id) ==
        tmp->request_id_to_handle.end())
      continue;

    connection = tmp;
  }

  if (!connection) {
    LOG(ERROR) << "Pending request with ID " << request_id
               << " not found for device with BD_ADDR: " << device_address;
    return false;
  }

  btgatt_response_t response;
  memset(&response, 0, sizeof(response));

  // We keep -1 as the handle for "Execute Write Request". In that case,
  // there is no need to populate the response data. Just send zeros back.
  int handle = connection->request_id_to_handle[request_id];
  response.handle = handle;
  response.attr_value.handle = handle;
  if (handle != -1) {
    memcpy(response.attr_value.value, value.data(), value.size());
    response.attr_value.offset = offset;
    response.attr_value.len = value.size();
  }

  bt_status_t result =
      hal::BluetoothGattInterface::Get()
          ->GetServerHALInterface()
          ->send_response(connection->conn_id, request_id, error, response);
  if (result != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to initiate call to send GATT response";
    return false;
  }

  connection->request_id_to_handle.erase(request_id);

  return true;
}

bool GattServer::SendNotification(const std::string& device_address,
                                  const uint16_t handle, bool confirm,
                                  const std::vector<uint8_t>& value,
                                  const GattCallback& callback) {
  VLOG(1) << " - server_id: " << server_id_
          << " device_address: " << device_address << " confirm: " << confirm;
  lock_guard<mutex> lock(mutex_);

  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  // Get the connection IDs for which we will send this notification.
  auto conn_iter = conn_addr_map_.find(device_address);
  if (conn_iter == conn_addr_map_.end()) {
    LOG(ERROR) << "No known connections for device with address: "
               << device_address;
    return false;
  }

  std::shared_ptr<PendingIndication> pending_ind(
      new PendingIndication(callback));

  // Send the notification/indication on all matching connections.
  int send_count = 0;
  for (const auto& conn : conn_iter->second) {
    // Make sure that one isn't already pending for this connection.
    if (pending_indications_.find(conn->conn_id) !=
        pending_indications_.end()) {
      VLOG(1) << "A" << (confirm ? "n indication" : " notification")
              << " is already pending for connection: " << conn->conn_id;
      continue;
    }

    // The HAL API takes char* rather const char* for |value|, so we have to
    // cast away the const.
    // TODO(armansito): Make HAL accept const char*.
    bt_status_t status = hal::BluetoothGattInterface::Get()
                             ->GetServerHALInterface()
                             ->send_indication(server_id_, handle,
                                               conn->conn_id, confirm, value);

    // Increment the send count if this was successful. We don't immediately
    // fail if the HAL returned an error. It's better to report success as long
    // as we sent out at least one notification to this device as
    // multi-transport GATT connections from the same BD_ADDR will be rare
    // enough already.
    if (status != BT_STATUS_SUCCESS) continue;

    send_count++;
    pending_indications_[conn->conn_id] = pending_ind;
  }

  if (send_count == 0) {
    LOG(ERROR) << "Failed to send notifications/indications to device: "
               << device_address;
    return false;
  }

  return true;
}

void GattServer::ConnectionCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int server_id,
    int connected, const RawAddress& bda) {
  lock_guard<mutex> lock(mutex_);

  if (server_id != server_id_) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " conn_id: " << conn_id << " connected: " << connected
          << " BD_ADDR: " << device_address;

  if (!connected) {
    // Erase the entry if we were connected to it.
    VLOG(1) << "No longer connected: " << device_address;
    conn_id_map_.erase(conn_id);
    auto iter = conn_addr_map_.find(device_address);
    if (iter == conn_addr_map_.end()) return;

    // Remove the appropriate connection objects in the address.
    for (auto conn_iter = iter->second.begin(); conn_iter != iter->second.end();
         ++conn_iter) {
      if ((*conn_iter)->conn_id != conn_id) continue;

      iter->second.erase(conn_iter);
      break;
    }

    if (delegate_)
      delegate_->OnConnectionStateChanged(this, device_address, false);

    return;
  }

  if (conn_id_map_.find(conn_id) != conn_id_map_.end()) {
    LOG(WARNING) << "Connection entry already exists; "
                 << "ignoring ConnectionCallback";
    return;
  }

  LOG(INFO) << "Added connection entry for conn_id: " << conn_id
            << " device address: " << device_address;
  std::shared_ptr<Connection> connection(new Connection(conn_id, bda));
  conn_id_map_[conn_id] = connection;
  conn_addr_map_[device_address].push_back(connection);

  if (delegate_)
    delegate_->OnConnectionStateChanged(this, device_address, true);
}

void GattServer::ServiceAddedCallback(hal::BluetoothGattInterface* gatt_iface,
                                      int status, int server_id,
                                      std::vector<btgatt_db_element_t> svc) {
  lock_guard<mutex> lock(mutex_);

  if (server_id != server_id_) return;

  VLOG(1) << __func__ << " - status: " << status << " server_id: " << server_id
          << " first handle: " << svc[0].attribute_handle
          << " service Uuid: " << Uuid(svc[0].uuid).ToString()
          << " count: " << svc.size();

  Service service(svc[0].attribute_handle, true, Uuid(svc[0].uuid), {}, {});

  for (size_t i = 1; i < svc.size(); i++) {
    const btgatt_db_element_t& curr = svc[i];
    VLOG(1) << " - processing item no: " << i
            << " handle: " << curr.attribute_handle;
    if (curr.type == BTGATT_DB_CHARACTERISTIC) {
      service.characteristics().push_back({curr.attribute_handle,
                                           Uuid(curr.uuid),
                                           curr.properties,
                                           curr.permissions,
                                           {}});
    } else if (curr.type == BTGATT_DB_DESCRIPTOR) {
      service.characteristics().back().descriptors().push_back(
          {curr.attribute_handle, Uuid(curr.uuid), curr.permissions});
    } else if (svc[i].type == BTGATT_DB_INCLUDED_SERVICE) {
    }
  }

  pending_end_decl_cb_((bluetooth::BLEStatus)status, service);

  CleanUpPendingData();
}

void GattServer::ServiceStoppedCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* server_id */, int /* service_handle */) {
  // TODO(armansito): Support stopping a service.
}

void GattServer::RequestReadCharacteristicCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int trans_id,
    const RawAddress& bda, int attribute_handle, int offset, bool is_long) {
  lock_guard<mutex> lock(mutex_);

  // Check to see if we know about this connection. Otherwise ignore the
  // request.
  auto conn = GetConnection(conn_id, bda, trans_id);
  if (!conn) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " BD_ADDR: " << device_address
          << " attribute_handle: " << attribute_handle << " offset: " << offset
          << " is_long: " << is_long;

  conn->request_id_to_handle[trans_id] = attribute_handle;

  // If there is no delegate then there is nobody to handle request. The request
  // will eventually timeout and we should get a connection update that
  // terminates the connection.
  if (!delegate_) {
    // TODO(armansito): Require a delegate at server registration so that this
    // is never possible.
    LOG(WARNING) << "No delegate was assigned to GattServer. Incoming request "
                 << "will time out.";
    return;
  }

  delegate_->OnCharacteristicReadRequest(this, device_address, trans_id, offset,
                                         is_long, attribute_handle);
}
void GattServer::RequestReadDescriptorCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int trans_id,
    const RawAddress& bda, int attribute_handle, int offset, bool is_long) {
  lock_guard<mutex> lock(mutex_);

  // Check to see if we know about this connection. Otherwise ignore the
  // request.
  auto conn = GetConnection(conn_id, bda, trans_id);
  if (!conn) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " BD_ADDR: " << device_address
          << " attribute_handle: " << attribute_handle << " offset: " << offset
          << " is_long: " << is_long;

  conn->request_id_to_handle[trans_id] = attribute_handle;

  // If there is no delegate then there is nobody to handle request. The request
  // will eventually timeout and we should get a connection update that
  // terminates the connection.
  if (!delegate_) {
    // TODO(armansito): Require a delegate at server registration so that this
    // is never possible.
    LOG(WARNING) << "No delegate was assigned to GattServer. Incoming request "
                 << "will time out.";
    return;
  }

  delegate_->OnDescriptorReadRequest(this, device_address, trans_id, offset,
                                     is_long, attribute_handle);
}

void GattServer::RequestWriteCharacteristicCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int trans_id,
    const RawAddress& bda, int attr_handle, int offset, bool need_rsp,
    bool is_prep, std::vector<uint8_t> value) {
  lock_guard<mutex> lock(mutex_);

  // Check to see if we know about this connection. Otherwise ignore the
  // request.
  auto conn = GetConnection(conn_id, bda, trans_id);
  if (!conn) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " BD_ADDR: " << device_address << " attr_handle: " << attr_handle
          << " offset: " << offset << " length: " << value.size()
          << " need_rsp: " << need_rsp << " is_prep: " << is_prep;

  // Store the request ID only if this is not a write-without-response. If
  // another request occurs after this with the same request ID, then we'll
  // simply process it normally, though that shouldn't ever happen.
  if (need_rsp) conn->request_id_to_handle[trans_id] = attr_handle;

  // If there is no delegate then there is nobody to handle request. The request
  // will eventually timeout and we should get a connection update that
  // terminates the connection.
  if (!delegate_) {
    // TODO(armansito): Require a delegate at server registration so that this
    // is never possible.
    LOG(WARNING) << "No delegate was assigned to GattServer. Incoming request "
                 << "will time out.";
    return;
  }

  delegate_->OnCharacteristicWriteRequest(this, device_address, trans_id,
                                          offset, is_prep, need_rsp,
                                          std::move(value), attr_handle);
}

void GattServer::RequestWriteDescriptorCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int trans_id,
    const RawAddress& bda, int attr_handle, int offset, bool need_rsp,
    bool is_prep, std::vector<uint8_t> value) {
  lock_guard<mutex> lock(mutex_);

  // Check to see if we know about this connection. Otherwise ignore the
  // request.
  auto conn = GetConnection(conn_id, bda, trans_id);
  if (!conn) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " BD_ADDR: " << device_address << " attr_handle: " << attr_handle
          << " offset: " << offset << " length: " << value.size()
          << " need_rsp: " << need_rsp << " is_prep: " << is_prep;

  // Store the request ID only if this is not a write-without-response. If
  // another request occurs after this with the same request ID, then we'll
  // simply process it normally, though that shouldn't ever happen.
  if (need_rsp) conn->request_id_to_handle[trans_id] = attr_handle;

  // If there is no delegate then there is nobody to handle request. The request
  // will eventually timeout and we should get a connection update that
  // terminates the connection.
  if (!delegate_) {
    // TODO(armansito): Require a delegate at server registration so that this
    // is never possible.
    LOG(WARNING) << "No delegate was assigned to GattServer. Incoming request "
                 << "will time out.";
    return;
  }

  delegate_->OnDescriptorWriteRequest(this, device_address, trans_id, offset,
                                      is_prep, need_rsp, std::move(value),
                                      attr_handle);
}

void GattServer::RequestExecWriteCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int trans_id,
    const RawAddress& bda, int exec_write) {
  lock_guard<mutex> lock(mutex_);

  // Check to see if we know about this connection. Otherwise ignore the
  // request.
  auto conn = GetConnection(conn_id, bda, trans_id);
  if (!conn) return;

  std::string device_address = BtAddrString(&bda);

  VLOG(1) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " BD_ADDR: " << device_address << " exec_write: " << exec_write;

  // Just store a dummy invalid handle as this request doesn't apply to a
  // specific handle.
  conn->request_id_to_handle[trans_id] = -1;

  // If there is no delegate then there is nobody to handle request. The request
  // will eventually timeout and we should get a connection update that
  // terminates the connection.
  if (!delegate_) {
    // TODO(armansito): Require a delegate at server registration so that this
    // is never possible.
    LOG(WARNING) << "No delegate was assigned to GattServer. Incoming request "
                 << "will time out.";
    return;
  }

  delegate_->OnExecuteWriteRequest(this, device_address, trans_id, exec_write);
}

void GattServer::IndicationSentCallback(
    hal::BluetoothGattInterface* /* gatt_iface */, int conn_id, int status) {
  VLOG(1) << __func__ << " conn_id: " << conn_id << " status: " << status;
  lock_guard<mutex> lock(mutex_);

  const auto& pending_ind_iter = pending_indications_.find(conn_id);
  if (pending_ind_iter == pending_indications_.end()) {
    VLOG(1) << "Unknown connection: " << conn_id;
    return;
  }

  std::shared_ptr<PendingIndication> pending_ind = pending_ind_iter->second;
  pending_indications_.erase(pending_ind_iter);

  if (status == BT_STATUS_SUCCESS) pending_ind->has_success = true;

  // Invoke it if this was the last reference to the confirmation callback.
  if (pending_ind.unique() && pending_ind->callback) {
    pending_ind->callback(pending_ind->has_success
                              ? GATT_ERROR_NONE
                              : static_cast<GATTError>(status));
  }
}

void GattServer::CleanUpPendingData() {
  pending_end_decl_cb_ = ResultCallback();
}

std::shared_ptr<GattServer::Connection> GattServer::GetConnection(
    int conn_id, const RawAddress& bda, int request_id) {
  auto iter = conn_id_map_.find(conn_id);
  if (iter == conn_id_map_.end()) {
    VLOG(1) << "Connection doesn't belong to this server";
    return nullptr;
  }

  auto conn = iter->second;
  if (conn->bdaddr != bda) {
    LOG(WARNING) << "BD_ADDR: " << BtAddrString(&bda) << " doesn't match "
                 << "connection ID: " << conn_id;
    return nullptr;
  }

  if (conn->request_id_to_handle.find(request_id) !=
      conn->request_id_to_handle.end()) {
    VLOG(1) << "Request with ID: " << request_id << " already exists for "
            << " connection: " << conn_id;
    return nullptr;
  }

  return conn;
}

// GattServerFactory implementation
// ========================================================

GattServerFactory::GattServerFactory() {
  hal::BluetoothGattInterface::Get()->AddServerObserver(this);
}

GattServerFactory::~GattServerFactory() {
  hal::BluetoothGattInterface::Get()->RemoveServerObserver(this);
}

bool GattServerFactory::RegisterInstance(const Uuid& uuid,
                                         const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  if (pending_calls_.find(uuid) != pending_calls_.end()) {
    LOG(ERROR) << "GATT-server client with given Uuid already being registered "
               << " - Uuid: " << uuid.ToString();
    return false;
  }

  const btgatt_server_interface_t* hal_iface =
      hal::BluetoothGattInterface::Get()->GetServerHALInterface();

  if (hal_iface->register_server(uuid) != BT_STATUS_SUCCESS) return false;

  pending_calls_[uuid] = callback;

  return true;
}

void GattServerFactory::RegisterServerCallback(
    hal::BluetoothGattInterface* gatt_iface, int status, int server_id,
    const Uuid& app_uuid) {
  Uuid uuid(app_uuid);

  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();
  lock_guard<mutex> lock(pending_calls_lock_);

  auto iter = pending_calls_.find(uuid);
  if (iter == pending_calls_.end()) {
    VLOG(1) << "Ignoring callback for unknown app_id: " << uuid.ToString();
    return;
  }

  // No need to construct a server if the call wasn't successful.
  std::unique_ptr<GattServer> server;
  BLEStatus result = BLE_STATUS_FAILURE;
  if (status == BT_STATUS_SUCCESS) {
    server.reset(new GattServer(uuid, server_id));

    gatt_iface->AddServerObserver(server.get());

    result = BLE_STATUS_SUCCESS;
  }

  // Notify the result via the result callback.
  iter->second(result, uuid, std::move(server));

  pending_calls_.erase(iter);
}

}  // namespace bluetooth
