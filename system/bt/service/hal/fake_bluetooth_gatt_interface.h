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

#pragma once

#include <vector>

#include <base/macros.h>
#include <base/observer_list.h>

#include "service/hal/bluetooth_gatt_interface.h"

namespace bluetooth {
namespace hal {

class FakeBluetoothGattInterface : public BluetoothGattInterface {
 public:
  // Handles HAL Bluetooth GATT client API calls for testing. Test code can
  // provide a fake or mock implementation of this and all calls will be routed
  // to it.
  class TestClientHandler {
   public:
    virtual ~TestClientHandler() = default;

    virtual bt_status_t RegisterClient(const bluetooth::Uuid& app_uuid) = 0;
    virtual bt_status_t UnregisterClient(int client_if) = 0;

    virtual bt_status_t Connect(int client_if, const RawAddress& bd_addr,
                                bool is_direct, int transport) = 0;
    virtual bt_status_t Disconnect(int client_if, const RawAddress& bd_addr,
                                   int conn_id) = 0;
  };

  // Handles HAL Bluetooth GATT server API calls for testing. Test code can
  // provide a fake or mock implementation of this and all calls will be routed
  // to it.
  class TestServerHandler {
   public:
    virtual ~TestServerHandler() = default;

    virtual bt_status_t RegisterServer(const bluetooth::Uuid& app_uuid) = 0;
    virtual bt_status_t UnregisterServer(int server_if) = 0;
    virtual bt_status_t AddService(
        int server_if, std::vector<btgatt_db_element_t> service) = 0;
    virtual bt_status_t DeleteService(int server_if, int srvc_handle) = 0;
    virtual bt_status_t SendIndication(int server_if, int attribute_handle,
                                       int conn_id, int confirm,
                                       std::vector<uint8_t> value) = 0;
    virtual bt_status_t SendResponse(int conn_id, int trans_id, int status,
                                     const btgatt_response_t& response) = 0;
  };

  // Constructs the fake with the given handlers. Implementations can
  // provide their own handlers or simply pass "nullptr" for the default
  // behavior in which BT_STATUS_FAIL will be returned from all calls.
  FakeBluetoothGattInterface(
      std::shared_ptr<BleAdvertiserInterface> advertiser_handler,
      std::shared_ptr<BleScannerInterface> scanner_handler,
      std::shared_ptr<TestClientHandler> client_handler,
      std::shared_ptr<TestServerHandler> server_handler);
  ~FakeBluetoothGattInterface();

  // The methods below can be used to notify observers with certain events and
  // given parameters.

  void NotifyRegisterScannerCallback(int status, int client_if,
                                     const bluetooth::Uuid& app_uuid);
  void NotifyScanResultCallback(const RawAddress& bda, int rssi,
                                std::vector<uint8_t> adv_data);

  // Client callbacks:
  void NotifyRegisterClientCallback(int status, int client_if,
                                    const bluetooth::Uuid& app_uuid);
  void NotifyConnectCallback(int conn_id, int status, int client_if,
                             const RawAddress& bda);
  void NotifyDisconnectCallback(int conn_id, int status, int client_if,
                                const RawAddress& bda);

  // Server callbacks:
  void NotifyRegisterServerCallback(int status, int server_if,
                                    const bluetooth::Uuid& app_uuid);
  void NotifyServerConnectionCallback(int conn_id, int server_if, int connected,
                                      const RawAddress& bda);
  void NotifyServiceAddedCallback(int status, int server_if,
                                  std::vector<btgatt_db_element_t> srvc);
  void NotifyCharacteristicAddedCallback(int status, int server_if,
                                         const bluetooth::Uuid& uuid,
                                         int srvc_handle, int char_handle);
  void NotifyDescriptorAddedCallback(int status, int server_if,
                                     const bluetooth::Uuid& uuid,
                                     int srvc_handle, int desc_handle);
  void NotifyServiceStartedCallback(int status, int server_if, int srvc_handle);
  void NotifyRequestReadCharacteristicCallback(int conn_id, int trans_id,
                                               const RawAddress& bda,
                                               int attr_handle, int offset,
                                               bool is_long);
  void NotifyRequestReadDescriptorCallback(int conn_id, int trans_id,
                                           const RawAddress& bda,
                                           int attr_handle, int offset,
                                           bool is_long);
  void NotifyRequestWriteCharacteristicCallback(int conn_id, int trans_id,
                                                const RawAddress& bda,
                                                int attr_handle, int offset,
                                                bool need_rsp, bool is_prep,
                                                std::vector<uint8_t> value);
  void NotifyRequestWriteDescriptorCallback(int conn_id, int trans_id,
                                            const RawAddress& bda,
                                            int attr_handle, int offset,
                                            bool need_rsp, bool is_prep,
                                            std::vector<uint8_t> value);
  void NotifyRequestExecWriteCallback(int conn_id, int trans_id,
                                      const RawAddress& bda, int exec_write);
  void NotifyIndicationSentCallback(int conn_id, int status);

  // BluetoothGattInterface overrides:
  void AddScannerObserver(ScannerObserver* observer) override;
  void RemoveScannerObserver(ScannerObserver* observer) override;
  void AddClientObserver(ClientObserver* observer) override;
  void RemoveClientObserver(ClientObserver* observer) override;
  void AddServerObserver(ServerObserver* observer) override;
  void RemoveServerObserver(ServerObserver* observer) override;
  BleAdvertiserInterface* GetAdvertiserHALInterface() const override;
  BleScannerInterface* GetScannerHALInterface() const override;
  const btgatt_client_interface_t* GetClientHALInterface() const override;
  const btgatt_server_interface_t* GetServerHALInterface() const override;

 private:
  base::ObserverList<ScannerObserver> scanner_observers_;
  base::ObserverList<ClientObserver> client_observers_;
  base::ObserverList<ServerObserver> server_observers_;
  std::shared_ptr<BleScannerInterface> scanner_handler_;
  std::shared_ptr<TestClientHandler> client_handler_;
  std::shared_ptr<TestServerHandler> server_handler_;

  DISALLOW_COPY_AND_ASSIGN(FakeBluetoothGattInterface);
};

}  // namespace hal
}  // namespace bluetooth
