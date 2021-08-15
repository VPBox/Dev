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

#include "service/hal/fake_bluetooth_gatt_interface.h"

namespace bluetooth {
namespace hal {
namespace {

// The global test handler instances. We have to have globals since the HAL
// interface methods all have to be global and their signatures don't allow us
// to pass in user_data.
std::shared_ptr<BleAdvertiserInterface> g_advertiser_handler;
std::shared_ptr<BleScannerInterface> g_scanner_handler;
std::shared_ptr<FakeBluetoothGattInterface::TestClientHandler> g_client_handler;
std::shared_ptr<FakeBluetoothGattInterface::TestServerHandler> g_server_handler;

bt_status_t FakeRegisterClient(const bluetooth::Uuid& app_uuid) {
  if (g_client_handler) return g_client_handler->RegisterClient(app_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeUnregisterClient(int client_if) {
  if (g_client_handler) return g_client_handler->UnregisterClient(client_if);

  return BT_STATUS_FAIL;
}

bt_status_t FakeConnect(int client_if, const RawAddress& bd_addr,
                        bool is_direct, int transport, bool opportunistic,
                        int phy) {
  if (g_client_handler)
    return g_client_handler->Connect(client_if, bd_addr, is_direct, transport);

  return BT_STATUS_FAIL;
}

bt_status_t FakeDisconnect(int client_if, const RawAddress& bd_addr,
                           int conn_id) {
  if (g_client_handler)
    return g_client_handler->Disconnect(client_if, bd_addr, conn_id);

  return BT_STATUS_FAIL;
}

bt_status_t FakeRegisterServer(const bluetooth::Uuid& app_uuid) {
  if (g_server_handler) return g_server_handler->RegisterServer(app_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeUnregisterServer(int server_if) {
  if (g_server_handler) return g_server_handler->UnregisterServer(server_if);

  return BT_STATUS_FAIL;
}

bt_status_t FakeAddService(int server_if,
                           std::vector<btgatt_db_element_t> service) {
  if (g_server_handler)
    return g_server_handler->AddService(server_if, std::move(service));

  return BT_STATUS_FAIL;
}

bt_status_t FakeDeleteService(int server_if, int srvc_handle) {
  if (g_server_handler)
    return g_server_handler->DeleteService(server_if, srvc_handle);

  return BT_STATUS_FAIL;
}

bt_status_t FakeSendIndication(int server_if, int attribute_handle, int conn_id,
                               int confirm, std::vector<uint8_t> value) {
  if (g_server_handler)
    return g_server_handler->SendIndication(server_if, attribute_handle,
                                            conn_id, confirm, std::move(value));

  return BT_STATUS_FAIL;
}

bt_status_t FakeSendResponse(int conn_id, int trans_id, int status,
                             const btgatt_response_t& response) {
  if (g_server_handler)
    return g_server_handler->SendResponse(conn_id, trans_id, status, response);

  return BT_STATUS_FAIL;
}

btgatt_client_interface_t fake_btgattc_iface = {
    FakeRegisterClient,
    FakeUnregisterClient,
    FakeConnect,
    FakeDisconnect,
    nullptr,  // refresh
    nullptr,  // search_service
    nullptr,  // discover_service_by_uuid
    nullptr,  // read_characteristic
    nullptr,  // read_using_characteristic_uuid
    nullptr,  // write_characteristic
    nullptr,  // read_descriptor
    nullptr,  // write_descriptor
    nullptr,  // execute_write
    nullptr,  // register_for_notification
    nullptr,  // deregister_for_notification
    nullptr,  // read_remote_rssi
    nullptr,  // get_device_type
    nullptr,  // configure_mtu
    nullptr,  // conn_parameter_update
    nullptr,  // set_phy
    nullptr,  // read_phy
    nullptr,  // test_command
    nullptr,  // get_gatt_db
};

btgatt_server_interface_t fake_btgatts_iface = {
    FakeRegisterServer,
    FakeUnregisterServer,
    nullptr,  // connect
    nullptr,  // disconnect
    FakeAddService,
    nullptr,  // stop_service
    FakeDeleteService,
    FakeSendIndication,
    FakeSendResponse,
    nullptr,  // set_phy
    nullptr,  // read_phy
};

}  // namespace

FakeBluetoothGattInterface::FakeBluetoothGattInterface(
    std::shared_ptr<BleAdvertiserInterface> advertiser_handler,
    std::shared_ptr<BleScannerInterface> scanner_handler,
    std::shared_ptr<TestClientHandler> client_handler,
    std::shared_ptr<TestServerHandler> server_handler)
    : client_handler_(client_handler) {
  CHECK(!g_advertiser_handler);
  CHECK(!g_scanner_handler);
  CHECK(!g_client_handler);
  CHECK(!g_server_handler);

  // We allow passing NULL. In this case all calls we fail by default.
  if (advertiser_handler) g_advertiser_handler = advertiser_handler;

  if (scanner_handler) g_scanner_handler = scanner_handler;

  if (client_handler) g_client_handler = client_handler;

  if (server_handler) g_server_handler = server_handler;
}

FakeBluetoothGattInterface::~FakeBluetoothGattInterface() {
  if (g_advertiser_handler) g_advertiser_handler = nullptr;

  if (g_scanner_handler) g_scanner_handler = nullptr;

  if (g_client_handler) g_client_handler = nullptr;

  if (g_server_handler) g_server_handler = nullptr;
}

// The methods below can be used to notify observers with certain events and
// given parameters.
void FakeBluetoothGattInterface::NotifyScanResultCallback(
    const RawAddress& bda, int rssi, std::vector<uint8_t> adv_data) {
  for (auto& observer : scanner_observers_) {
    observer.ScanResultCallback(this, bda, rssi, adv_data);
  }
}

void FakeBluetoothGattInterface::NotifyRegisterClientCallback(
    int status, int client_if, const bluetooth::Uuid& app_uuid) {
  for (auto& observer : client_observers_) {
    observer.RegisterClientCallback(this, status, client_if, app_uuid);
  }
}

void FakeBluetoothGattInterface::NotifyConnectCallback(int conn_id, int status,
                                                       int client_if,
                                                       const RawAddress& bda) {
  for (auto& observer : client_observers_) {
    observer.ConnectCallback(this, conn_id, status, client_if, bda);
  }
}

void FakeBluetoothGattInterface::NotifyDisconnectCallback(
    int conn_id, int status, int client_if, const RawAddress& bda) {
  for (auto& observer : client_observers_) {
    observer.DisconnectCallback(this, conn_id, status, client_if, bda);
  }
}

void FakeBluetoothGattInterface::NotifyRegisterServerCallback(
    int status, int server_if, const Uuid& app_uuid) {
  for (auto& observer : server_observers_) {
    observer.RegisterServerCallback(this, status, server_if, app_uuid);
  }
}

void FakeBluetoothGattInterface::NotifyServerConnectionCallback(
    int conn_id, int server_if, int connected, const RawAddress& bda) {
  for (auto& observer : server_observers_) {
    observer.ConnectionCallback(this, conn_id, server_if, connected, bda);
  }
}

void FakeBluetoothGattInterface::NotifyServiceAddedCallback(
    int status, int server_if, std::vector<btgatt_db_element_t> service) {
  for (auto& observer : server_observers_) {
    observer.ServiceAddedCallback(this, status, server_if, service);
  }
}

void FakeBluetoothGattInterface::NotifyRequestReadCharacteristicCallback(
    int conn_id, int trans_id, const RawAddress& bda, int attr_handle,
    int offset, bool is_long) {
  for (auto& observer : server_observers_) {
    observer.RequestReadCharacteristicCallback(this, conn_id, trans_id, bda,
                                               attr_handle, offset, is_long);
  }
}

void FakeBluetoothGattInterface::NotifyRequestReadDescriptorCallback(
    int conn_id, int trans_id, const RawAddress& bda, int attr_handle,
    int offset, bool is_long) {
  for (auto& observer : server_observers_) {
    observer.RequestReadDescriptorCallback(this, conn_id, trans_id, bda,
                                           attr_handle, offset, is_long);
  }
}

void FakeBluetoothGattInterface::NotifyRequestWriteCharacteristicCallback(
    int conn_id, int trans_id, const RawAddress& bda, int attr_handle,
    int offset, bool need_rsp, bool is_prep, std::vector<uint8_t> value) {
  for (auto& observer : server_observers_) {
    observer.RequestWriteCharacteristicCallback(this, conn_id, trans_id, bda,
                                                attr_handle, offset, need_rsp,
                                                is_prep, value);
  }
}

void FakeBluetoothGattInterface::NotifyRequestWriteDescriptorCallback(
    int conn_id, int trans_id, const RawAddress& bda, int attr_handle,
    int offset, bool need_rsp, bool is_prep, std::vector<uint8_t> value) {
  for (auto& observer : server_observers_) {
    observer.RequestWriteDescriptorCallback(this, conn_id, trans_id, bda,
                                            attr_handle, offset, need_rsp,
                                            is_prep, value);
  }
}

void FakeBluetoothGattInterface::NotifyRequestExecWriteCallback(
    int conn_id, int trans_id, const RawAddress& bda, int exec_write) {
  for (auto& observer : server_observers_) {
    observer.RequestExecWriteCallback(this, conn_id, trans_id, bda, exec_write);
  }
}

void FakeBluetoothGattInterface::NotifyIndicationSentCallback(int conn_id,
                                                              int status) {
  for (auto& observer : server_observers_) {
    observer.IndicationSentCallback(this, conn_id, status);
  }
}

void FakeBluetoothGattInterface::AddScannerObserver(ScannerObserver* observer) {
  CHECK(observer);
  scanner_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveScannerObserver(
    ScannerObserver* observer) {
  CHECK(observer);
  scanner_observers_.RemoveObserver(observer);
}

void FakeBluetoothGattInterface::AddClientObserver(ClientObserver* observer) {
  CHECK(observer);
  client_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveClientObserver(
    ClientObserver* observer) {
  CHECK(observer);
  client_observers_.RemoveObserver(observer);
}

void FakeBluetoothGattInterface::AddServerObserver(ServerObserver* observer) {
  CHECK(observer);
  server_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveServerObserver(
    ServerObserver* observer) {
  CHECK(observer);
  server_observers_.RemoveObserver(observer);
}

BleAdvertiserInterface* FakeBluetoothGattInterface::GetAdvertiserHALInterface()
    const {
  return g_advertiser_handler.get();
}

BleScannerInterface* FakeBluetoothGattInterface::GetScannerHALInterface()
    const {
  return g_scanner_handler.get();
}

const btgatt_client_interface_t*
FakeBluetoothGattInterface::GetClientHALInterface() const {
  return &fake_btgattc_iface;
}

const btgatt_server_interface_t*
FakeBluetoothGattInterface::GetServerHALInterface() const {
  return &fake_btgatts_iface;
}

}  // namespace hal
}  // namespace bluetooth
