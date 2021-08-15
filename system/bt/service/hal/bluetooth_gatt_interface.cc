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

#include "service/hal/bluetooth_gatt_interface.h"

#include <mutex>
#include <shared_mutex>

#include <base/logging.h>
#include <base/observer_list.h>

#include "service/hal/bluetooth_interface.h"
#include "service/logging_helpers.h"

using std::lock_guard;
using std::unique_lock;
using std::shared_lock;
using std::mutex;
#if defined(OS_GENERIC) && defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION < 3500)
using shared_mutex_impl = std::shared_mutex;
#else
using shared_mutex_impl = std::shared_timed_mutex;
#endif

namespace bluetooth {
namespace hal {

namespace {

// The global BluetoothGattInterface instance.
BluetoothGattInterface* g_interface = nullptr;

// Mutex used by callbacks to access |g_interface|. If we initialize or clean it
// use unique_lock. If only accessing |g_interface| use shared lock.
// TODO(jpawlowski): this should be just shared_mutex, as we currently don't use
// timed methods. Change to shared_mutex when we upgrade to C++14
shared_mutex_impl g_instance_lock;

// Helper for obtaining the observer lists. This is forward declared here
// and defined below since it depends on BluetoothInterfaceImpl.
base::ObserverList<BluetoothGattInterface::ScannerObserver>*
GetScannerObservers();
base::ObserverList<BluetoothGattInterface::ClientObserver>*
GetClientObservers();
base::ObserverList<BluetoothGattInterface::ServerObserver>*
GetServerObservers();

#define FOR_EACH_SCANNER_OBSERVER(func)           \
  for (auto& observer : *GetScannerObservers()) { \
    observer.func;                                \
  }

#define FOR_EACH_CLIENT_OBSERVER(func)           \
  for (auto& observer : *GetClientObservers()) { \
    observer.func;                               \
  }

#define FOR_EACH_SERVER_OBSERVER(func)           \
  for (auto& observer : *GetServerObservers()) { \
    observer.func;                               \
  }

#define VERIFY_INTERFACE_OR_RETURN()                                   \
  do {                                                                 \
    if (!g_interface) {                                                \
      LOG(WARNING) << "Callback received while |g_interface| is NULL"; \
      return;                                                          \
    }                                                                  \
  } while (0)

void RegisterClientCallback(int status, int client_if,
                            const bluetooth::Uuid& app_uuid) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " client_if: " << client_if;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_CLIENT_OBSERVER(
      RegisterClientCallback(g_interface, status, client_if, app_uuid));
}

void ScanResultCallback(
    uint16_t ble_evt_type, uint8_t addr_type, RawAddress* bda,
    uint8_t ble_primary_phy, uint8_t ble_secondary_phy,
    uint8_t ble_advertising_sid, int8_t ble_tx_power, int8_t rssi,
    uint16_t ble_periodic_adv_int,
    std::vector<uint8_t> adv_data) {  // NOLINT(pass-by-value)
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  CHECK(bda);

  VLOG(2) << __func__ << " - BD_ADDR: " << BtAddrString(bda)
          << " RSSI: " << rssi;
  FOR_EACH_SCANNER_OBSERVER(
      ScanResultCallback(g_interface, *bda, rssi, adv_data));
}

void ConnectCallback(int conn_id, int status, int client_if,
                     const RawAddress& bda) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - status: " << status << " client_if: " << client_if
          << " - BD_ADDR: " << BtAddrString(&bda) << " - conn_id: " << conn_id;

  FOR_EACH_CLIENT_OBSERVER(
      ConnectCallback(g_interface, conn_id, status, client_if, bda));
}

void DisconnectCallback(int conn_id, int status, int client_if,
                        const RawAddress& bda) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id << " - status: " << status
          << " client_if: " << client_if
          << " - BD_ADDR: " << BtAddrString(&bda);
  FOR_EACH_CLIENT_OBSERVER(
      DisconnectCallback(g_interface, conn_id, status, client_if, bda));
}

void SearchCompleteCallback(int conn_id, int status) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id << " - status: " << status;
  FOR_EACH_CLIENT_OBSERVER(
      SearchCompleteCallback(g_interface, conn_id, status));
}

void RegisterForNotificationCallback(int conn_id, int registered, int status,
                                     uint16_t handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  LOG(INFO) << __func__ << " - conn_id: " << conn_id << " - status: " << status
            << " - registered: " << registered << " - handle: " << handle;
  FOR_EACH_CLIENT_OBSERVER(RegisterForNotificationCallback(
      g_interface, conn_id, registered, status, handle));
}

void NotifyCallback(int conn_id, const btgatt_notify_params_t& p_data) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id
          << " - address: " << BtAddrString(&p_data.bda)
          << " - handle: " << p_data.handle << " - len: " << p_data.len
          << " - is_notify: " << p_data.is_notify;

  FOR_EACH_CLIENT_OBSERVER(NotifyCallback(g_interface, conn_id, p_data));
}

void WriteCharacteristicCallback(int conn_id, int status, uint16_t handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id << " - status: " << status;

  FOR_EACH_CLIENT_OBSERVER(
      WriteCharacteristicCallback(g_interface, conn_id, status, handle));
}

void WriteDescriptorCallback(int conn_id, int status, uint16_t handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id << " - status: " << status;

  FOR_EACH_CLIENT_OBSERVER(
      WriteDescriptorCallback(g_interface, conn_id, status, handle));
}

void MtuChangedCallback(int conn_id, int status, int mtu) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  VLOG(2) << __func__ << " - conn_id: " << conn_id << " status: " << status
          << " mtu: " << mtu;

  FOR_EACH_CLIENT_OBSERVER(
      MtuChangedCallback(g_interface, conn_id, status, mtu));
}

void GetGattDbCallback(int conn_id, const btgatt_db_element_t* db, int size) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " size: " << size;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_CLIENT_OBSERVER(GetGattDbCallback(g_interface, conn_id, db, size));
}

void ServicesRemovedCallback(int conn_id, uint16_t start_handle,
                             uint16_t end_handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id
          << " start_handle: " << start_handle << " end_handle: " << end_handle;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_CLIENT_OBSERVER(
      ServicesRemovedCallback(g_interface, conn_id, start_handle, end_handle));
}

void ServicesAddedCallback(int conn_id, const btgatt_db_element_t& added,
                           int added_count) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id
          << " added_count: " << added_count;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_CLIENT_OBSERVER(
      ServicesAddedCallback(g_interface, conn_id, added, added_count));
}

void RegisterServerCallback(int status, int server_if,
                            const bluetooth::Uuid& app_uuid) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " server_if: " << server_if;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      RegisterServerCallback(g_interface, status, server_if, app_uuid));
}

void ConnectionCallback(int conn_id, int server_if, int connected,
                        const RawAddress& bda) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id
          << " server_if: " << server_if << " connected: " << connected;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      ConnectionCallback(g_interface, conn_id, server_if, connected, bda));
}

void ServiceAddedCallback(
    int status, int server_if,
    std::vector<btgatt_db_element_t> service) {  // NOLINT(pass-by-value)
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " server_if: " << server_if
          << " count: " << service.size();
  VERIFY_INTERFACE_OR_RETURN();
  CHECK(service.size());

  FOR_EACH_SERVER_OBSERVER(
      ServiceAddedCallback(g_interface, status, server_if, service));
}

void ServiceStoppedCallback(int status, int server_if, int srvc_handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " server_if: " << server_if
          << " handle: " << srvc_handle;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      ServiceStoppedCallback(g_interface, status, server_if, srvc_handle));
}

void ServiceDeletedCallback(int status, int server_if, int srvc_handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " server_if: " << server_if
          << " handle: " << srvc_handle;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      ServiceDeletedCallback(g_interface, status, server_if, srvc_handle));
}

void RequestReadCharacteristicCallback(int conn_id, int trans_id,
                                       const RawAddress& bda, int attr_handle,
                                       int offset, bool is_long) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " attr_handle: " << attr_handle << " offset: " << offset
          << " is_long: " << is_long;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(RequestReadCharacteristicCallback(
      g_interface, conn_id, trans_id, bda, attr_handle, offset, is_long));
}

void RequestReadDescriptorCallback(int conn_id, int trans_id,
                                   const RawAddress& bda, int attr_handle,
                                   int offset, bool is_long) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " attr_handle: " << attr_handle << " offset: " << offset
          << " is_long: " << is_long;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(RequestReadDescriptorCallback(
      g_interface, conn_id, trans_id, bda, attr_handle, offset, is_long));
}

void RequestWriteCharacteristicCallback(int conn_id, int trans_id,
                                        const RawAddress& bda, int attr_handle,
                                        int offset, bool need_rsp, bool is_prep,
                                        std::vector<uint8_t> value) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " attr_handle: " << attr_handle << " offset: " << offset
          << " length: " << value.size() << " need_rsp: " << need_rsp
          << " is_prep: " << is_prep;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(RequestWriteCharacteristicCallback(
      g_interface, conn_id, trans_id, bda, attr_handle, offset, need_rsp,
      is_prep, value));
}

void RequestWriteDescriptorCallback(
    int conn_id, int trans_id, const RawAddress& bda, int attr_handle,
    int offset, bool need_rsp, bool is_prep,
    std::vector<uint8_t> value) {  // NOLINT(pass-by-value)
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " attr_handle: " << attr_handle << " offset: " << offset
          << " length: " << value.size() << " need_rsp: " << need_rsp
          << " is_prep: " << is_prep;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(RequestWriteDescriptorCallback(
      g_interface, conn_id, trans_id, bda, attr_handle, offset, need_rsp,
      is_prep, value));
}

void RequestExecWriteCallback(int conn_id, int trans_id, const RawAddress& bda,
                              int exec_write) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " trans_id: " << trans_id
          << " exec_write: " << exec_write;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(RequestExecWriteCallback(g_interface, conn_id,
                                                    trans_id, bda, exec_write));
}

void ResponseConfirmationCallback(int status, int handle) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - status: " << status << " handle: " << handle;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      ResponseConfirmationCallback(g_interface, status, handle));
}

void IndicationSentCallback(int conn_id, int status) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " status: " << status;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(
      IndicationSentCallback(g_interface, conn_id, status));
}

void MtuChangedCallback(int conn_id, int mtu) {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VLOG(2) << __func__ << " - conn_id: " << conn_id << " mtu: " << mtu;
  VERIFY_INTERFACE_OR_RETURN();

  FOR_EACH_SERVER_OBSERVER(MtuChangedCallback(g_interface, conn_id, mtu));
}

// The HAL Bluetooth GATT client interface callbacks. These signal a mixture of
// GATT client-role and GAP events.

const btgatt_scanner_callbacks_t gatt_scanner_callbacks = {
    ScanResultCallback,
    nullptr,  // batchscan_reports_cb
    nullptr,  // batchscan_threshold_cb
    nullptr,  // track_adv_event_cb
};

const btgatt_client_callbacks_t gatt_client_callbacks = {
    RegisterClientCallback,
    ConnectCallback,
    DisconnectCallback,
    SearchCompleteCallback,
    RegisterForNotificationCallback,
    NotifyCallback,
    nullptr,  // read_characteristic_cb
    WriteCharacteristicCallback,
    nullptr,  // read_descriptor_cb
    WriteDescriptorCallback,
    nullptr,  // execute_write_cb
    nullptr,  // read_remote_rssi_cb
    MtuChangedCallback,
    nullptr,  // congestion_cb
    GetGattDbCallback,
    ServicesRemovedCallback,
    ServicesAddedCallback,
    nullptr,
    nullptr,
};

const btgatt_server_callbacks_t gatt_server_callbacks = {
    RegisterServerCallback,
    ConnectionCallback,
    ServiceAddedCallback,
    ServiceStoppedCallback,
    ServiceDeletedCallback,
    RequestReadCharacteristicCallback,
    RequestReadDescriptorCallback,
    RequestWriteCharacteristicCallback,
    RequestWriteDescriptorCallback,
    RequestExecWriteCallback,
    ResponseConfirmationCallback,
    IndicationSentCallback,
    nullptr,  // congestion_cb
    MtuChangedCallback,
    nullptr,
    nullptr,
};

const btgatt_callbacks_t gatt_callbacks = {
    sizeof(btgatt_callbacks_t), &gatt_client_callbacks, &gatt_server_callbacks,
    &gatt_scanner_callbacks,
};

}  // namespace

// BluetoothGattInterface implementation for production.
class BluetoothGattInterfaceImpl : public BluetoothGattInterface {
 public:
  BluetoothGattInterfaceImpl() : hal_iface_(nullptr) {}

  ~BluetoothGattInterfaceImpl() override {
    if (hal_iface_) hal_iface_->cleanup();
  }

  void AddScannerObserver(ScannerObserver* observer) override {
    scanner_observers_.AddObserver(observer);
  }

  void RemoveScannerObserver(ScannerObserver* observer) override {
    scanner_observers_.RemoveObserver(observer);
  }

  void AddClientObserver(ClientObserver* observer) override {
    client_observers_.AddObserver(observer);
  }

  void RemoveClientObserver(ClientObserver* observer) override {
    client_observers_.RemoveObserver(observer);
  }

  void AddServerObserver(ServerObserver* observer) override {
    server_observers_.AddObserver(observer);
  }

  void RemoveServerObserver(ServerObserver* observer) override {
    server_observers_.RemoveObserver(observer);
  }

  BleAdvertiserInterface* GetAdvertiserHALInterface() const override {
    return hal_iface_->advertiser;
  }

  BleScannerInterface* GetScannerHALInterface() const override {
    return hal_iface_->scanner;
  }

  const btgatt_client_interface_t* GetClientHALInterface() const override {
    return hal_iface_->client;
  }

  const btgatt_server_interface_t* GetServerHALInterface() const override {
    return hal_iface_->server;
  }

  // Initialize the interface.
  bool Initialize() {
    const bt_interface_t* bt_iface =
        BluetoothInterface::Get()->GetHALInterface();
    CHECK(bt_iface);

    const btgatt_interface_t* gatt_iface =
        reinterpret_cast<const btgatt_interface_t*>(
            bt_iface->get_profile_interface(BT_PROFILE_GATT_ID));
    if (!gatt_iface) {
      LOG(ERROR) << "Failed to obtain HAL GATT interface handle";
      return false;
    }

    bt_status_t status = gatt_iface->init(&gatt_callbacks);
    if (status != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to initialize HAL GATT interface";
      return false;
    }

    hal_iface_ = gatt_iface;

    return true;
  }

  base::ObserverList<ScannerObserver>* scanner_observers() {
    return &scanner_observers_;
  }

  base::ObserverList<ClientObserver>* client_observers() {
    return &client_observers_;
  }

  base::ObserverList<ServerObserver>* server_observers() {
    return &server_observers_;
  }

 private:
  // List of observers that are interested in notifications from us.
  // We're not using a base::ObserverListThreadSafe, which it posts observer
  // events automatically on the origin threads, as we want to avoid that
  // overhead and simply forward the events to the upper layer.
  base::ObserverList<ScannerObserver> scanner_observers_;
  base::ObserverList<ClientObserver> client_observers_;
  base::ObserverList<ServerObserver> server_observers_;

  // The HAL handle obtained from the shared library. We hold a weak reference
  // to this since the actual data resides in the shared Bluetooth library.
  const btgatt_interface_t* hal_iface_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothGattInterfaceImpl);
};

namespace {

base::ObserverList<BluetoothGattInterface::ScannerObserver>*
GetScannerObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothGattInterfaceImpl*>(g_interface)
      ->scanner_observers();
}

base::ObserverList<BluetoothGattInterface::ClientObserver>*
GetClientObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothGattInterfaceImpl*>(g_interface)
      ->client_observers();
}

base::ObserverList<BluetoothGattInterface::ServerObserver>*
GetServerObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothGattInterfaceImpl*>(g_interface)
      ->server_observers();
}

}  // namespace

// Default observer implementations. These are provided so that the methods
// themselves are optional.

void BluetoothGattInterface::ScannerObserver::ScanResultCallback(
    BluetoothGattInterface* /* gatt_iface */, const RawAddress& /* bda */,
    int /* rssi */,
    std::vector<uint8_t> /* adv_data */) {  // NOLINT(pass-by-value)
  // Do Nothing.
}

void BluetoothGattInterface::ClientObserver::RegisterClientCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* client_if */, const bluetooth::Uuid& /* app_uuid */) {
  // Do nothing.
}

void BluetoothGattInterface::ClientObserver::ConnectCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */, int /* client_if */, const RawAddress& /* bda */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::DisconnectCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */, int /* client_if */, const RawAddress& /* bda */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::SearchCompleteCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::RegisterForNotificationCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* registered */, int /* status */, uint16_t /* handle */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::NotifyCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    const btgatt_notify_params_t& /* p_data */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::WriteCharacteristicCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */, uint16_t /* handle */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::WriteDescriptorCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */, uint16_t /* handle */) {
  // Do nothing
}

void BluetoothGattInterface::ClientObserver::MtuChangedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* statis*/, int /* mtu */) {
  // Do nothing.
}

void BluetoothGattInterface::ClientObserver::GetGattDbCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    const btgatt_db_element_t* /* gatt_db */, int /* size */) {
  // Do nothing.
}

void BluetoothGattInterface::ClientObserver::ServicesRemovedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    uint16_t /* start_handle */, uint16_t /* end_handle */) {
  // Do nothing.
}

void BluetoothGattInterface::ClientObserver::ServicesAddedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    const btgatt_db_element_t& /* added */, int /* added_count */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RegisterServerCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* server_if */, const bluetooth::Uuid& /* app_uuid */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::ConnectionCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* server_if */, int /* connected */, const RawAddress& /* bda */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::ServiceAddedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* server_if */,
    std::vector<btgatt_db_element_t> /* service */) {  // NOLINT(pass-by-value)
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::ServiceStoppedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* server_if */, int /* srvc_handle */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::ServiceDeletedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* server_if */, int /* srvc_handle */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RequestReadCharacteristicCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* trans_id */, const RawAddress& /* bda */, int /* attr_handle */,
    int /* offset */, bool /* is_long */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RequestReadDescriptorCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* trans_id */, const RawAddress& /* bda */, int /* attr_handle */,
    int /* offset */, bool /* is_long */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RequestWriteCharacteristicCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* trans_id */, const RawAddress& /* bda */, int /* attr_handle */,
    int /* offset */, bool /* need_rsp */, bool /* is_prep */,
    std::vector<uint8_t> /* value */) {  // NOLINT(pass-by-value)
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RequestWriteDescriptorCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* trans_id */, const RawAddress& /* bda */, int /* attr_handle */,
    int /* offset */, bool /* need_rsp */, bool /* is_prep */,
    std::vector<uint8_t> /* value */) {  // NOLINT(pass-by-value)
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::RequestExecWriteCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* trans_id */, const RawAddress& /* bda */, int /* exec_write */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::ResponseConfirmationCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* status */,
    int /* handle */) {
  // Do nothing
}

void BluetoothGattInterface::ServerObserver::IndicationSentCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* status */) {
  // Do nothing.
}

void BluetoothGattInterface::ServerObserver::MtuChangedCallback(
    BluetoothGattInterface* /* gatt_iface */, int /* conn_id */,
    int /* mtu */) {
  // Do nothing.
}

// static
bool BluetoothGattInterface::Initialize() {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(!g_interface);

  std::unique_ptr<BluetoothGattInterfaceImpl> impl(
      new BluetoothGattInterfaceImpl());
  if (!impl->Initialize()) {
    LOG(ERROR) << "Failed to initialize BluetoothGattInterface";
    return false;
  }

  g_interface = impl.release();

  return true;
}

// static
void BluetoothGattInterface::CleanUp() {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);

  delete g_interface;
  g_interface = nullptr;
}

// static
bool BluetoothGattInterface::IsInitialized() {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);

  return g_interface != nullptr;
}

// static
BluetoothGattInterface* BluetoothGattInterface::Get() {
  shared_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);
  return g_interface;
}

// static
void BluetoothGattInterface::InitializeForTesting(
    BluetoothGattInterface* test_instance) {
  unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(test_instance);
  CHECK(!g_interface);

  g_interface = test_instance;
}

bt_status_t BluetoothGattInterface::StartScan(int client_id) {
  lock_guard<mutex> lock(scan_clients_lock_);

  // Scan already initiated for this client.
  if (scan_client_set_.find(client_id) != scan_client_set_.end()) {
    // Assume starting scan multiple times is not error, but warn user.
    LOG(WARNING) << "Scan already initiated for client";
    return BT_STATUS_SUCCESS;
  }

  // If this is the first scan client, then make a call into the stack. We
  // only do this when the reference count changes to or from 0.
  if (scan_client_set_.empty()) {
    GetScannerHALInterface()->Scan(true);
  }

  scan_client_set_.insert(client_id);

  return BT_STATUS_SUCCESS;
}

bt_status_t BluetoothGattInterface::StopScan(int client_id) {
  lock_guard<mutex> lock(scan_clients_lock_);

  // Scan not initiated for this client.
  auto iter = scan_client_set_.find(client_id);
  if (iter == scan_client_set_.end()) {
    // Assume stopping scan multiple times is not error, but warn user.
    LOG(WARNING) << "Scan already stopped or not initiated for client";
    return BT_STATUS_SUCCESS;
  }

  if (scan_client_set_.size() == 1) {
    GetScannerHALInterface()->Scan(false);
  }

  scan_client_set_.erase(iter);
  return BT_STATUS_SUCCESS;
}

}  // namespace hal
}  // namespace bluetooth
