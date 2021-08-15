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

#define LOG_TAG "bt_gatts"

#include "gatt_server_old.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/callback.h>
#include <algorithm>
#include <array>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <hardware/bluetooth.h>
#include <hardware/bt_gatt.h>

#include "service/hal/bluetooth_interface.h"
#include "service/logging_helpers.h"

#include "osi/include/log.h"
#include "osi/include/osi.h"

namespace {

const size_t kMaxGattAttributeSize = 512;
std::vector<btgatt_db_element_t> pending_svc_decl;
std::unordered_set<int> blob_index;

// TODO(icoolidge): Support multiple instances
// TODO(armansito): Remove this variable. No point of having this if
// each bluetooth::gatt::Server instance already keeps a pointer to the
// ServerInternals that is associated with it (which is much cleaner). It looks
// like this variable exists because the btif callbacks don't allow the
// upper-layer to pass user data to them. We could:
//
//    1. Fix the btif callbacks so that some sort of continuation can be
//    attached to a callback. This might be a long shot since the callback
//    interface doesn't allow more than one caller to register its own callbacks
//    (which might be what we want though, since this would make the API more
//    flexible).
//
//    2. Allow creation of Server objects using a factory method that returns
//    the result asynchronously in a base::Callback. The RegisterServerCallback
//    provides an |app_uuid|, which can be used to store callback structures in
//    a map and lazily instantiate the Server and invoke the correct callback.
//    This is a general pattern that we should use throughout the daemon, since
//    all operations can timeout or fail and this is best reported in an
//    asynchronous base::Callback.
//
static bluetooth::gatt::ServerInternals* g_internal = nullptr;

enum { kPipeReadEnd = 0, kPipeWriteEnd = 1, kPipeNumEnds = 2 };

}  // namespace

namespace bluetooth {
namespace gatt {

struct Characteristic {
  Uuid uuid;
  int blob_section;
  std::vector<uint8_t> blob;

  // Support synchronized blob updates by latching under mutex.
  std::vector<uint8_t> next_blob;
  bool next_blob_pending;
  bool notify;
};

struct ServerInternals {
  ServerInternals();
  ~ServerInternals();
  int Initialize();
  bt_status_t AddCharacteristic(const Uuid& uuid, uint8_t properties,
                                uint16_t permissions);

  // This maps API attribute Uuids to BlueDroid handles.
  std::map<Uuid, int> uuid_to_attribute;

  // The attribute cache, indexed by BlueDroid handles.
  std::unordered_map<int, Characteristic> characteristics;

  // Associate a control attribute with its value attribute.
  std::unordered_map<int, int> controlled_blobs;

  ScanResults scan_results;

  Uuid last_write;
  const btgatt_interface_t* gatt;
  int server_if;
  int client_if;
  int service_handle;
  std::set<int> connections;

  std::mutex lock;
  std::condition_variable api_synchronize;
  int pipefd[kPipeNumEnds];
};

}  // namespace gatt
}  // namespace bluetooth

namespace {

/** Callback invoked in response to register_server */
void RegisterServerCallback(int status, int server_if,
                            const bluetooth::Uuid& app_uuid) {
  LOG_INFO(LOG_TAG, "%s: status:%d server_if:%d app_uuid:%p", __func__, status,
           server_if, &app_uuid);

  g_internal->server_if = server_if;

  pending_svc_decl.push_back(
      {.type = BTGATT_DB_PRIMARY_SERVICE, .uuid = app_uuid});
}

void ServiceAddedCallback(int status, int server_if,
                          std::vector<btgatt_db_element_t> service) {
  LOG_INFO(LOG_TAG, "%s: status:%d server_if:%d count:%zu svc_handle:%d",
           __func__, status, server_if, service.size(),
           service[0].attribute_handle);

  std::lock_guard<std::mutex> lock(g_internal->lock);
  g_internal->server_if = server_if;

  g_internal->service_handle = service[0].attribute_handle;

  uint16_t prev_char_handle = 0;
  uint16_t prev_char_properties = 0;
  for (size_t i = 1; i < service.size(); i++) {
    const btgatt_db_element_t& el = service[i];
    if (el.type == BTGATT_DB_DESCRIPTOR) {
      LOG_INFO(LOG_TAG, "%s: descr_handle:%d", __func__, el.attribute_handle);
    } else if (el.type == BTGATT_DB_CHARACTERISTIC) {
      bluetooth::Uuid id(el.uuid);
      uint16_t char_handle = el.attribute_handle;

      LOG_INFO(LOG_TAG, "%s: char_handle:%d", __func__, char_handle);

      g_internal->uuid_to_attribute[id] = char_handle;
      g_internal->characteristics[char_handle].uuid = id;
      g_internal->characteristics[char_handle].blob_section = 0;

      // If the added characteristic is blob
      if (blob_index.find(i) != blob_index.end()) {
        // Finally, associate the control attribute with the value attribute.
        // Also, initialize the control attribute to a readable zero.
        const uint16_t control_attribute = char_handle;
        const uint16_t blob_attribute = prev_char_handle;
        g_internal->controlled_blobs[control_attribute] = blob_attribute;
        g_internal->characteristics[blob_attribute].notify =
            prev_char_properties & bluetooth::gatt::kPropertyNotify;

        bluetooth::gatt::Characteristic& ctrl =
            g_internal->characteristics[control_attribute];
        ctrl.next_blob.clear();
        ctrl.next_blob.push_back(0);
        ctrl.next_blob_pending = true;
        ctrl.blob_section = 0;
        ctrl.notify = false;
      }
      prev_char_handle = char_handle;
      prev_char_properties = el.properties;
    }
  }

  pending_svc_decl.clear();
  blob_index.clear();

  // The Uuid provided here is unimportant, and is only used to satisfy
  // BlueDroid.
  // It must be different than any other registered Uuid.
  bluetooth::Uuid client_id = bluetooth::Uuid::GetRandom();

  bt_status_t btstat = g_internal->gatt->client->register_client(client_id);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "%s: Failed to register client", __func__);
  }
}

void RequestReadCallback(int conn_id, int trans_id, const RawAddress& bda,
                         int attr_handle, int attribute_offset_octets,
                         bool is_long) {
  std::lock_guard<std::mutex> lock(g_internal->lock);

  bluetooth::gatt::Characteristic& ch =
      g_internal->characteristics[attr_handle];

  // Latch next_blob to blob on a 'fresh' read.
  if (ch.next_blob_pending && attribute_offset_octets == 0 &&
      ch.blob_section == 0) {
    std::swap(ch.blob, ch.next_blob);
    ch.next_blob_pending = false;
  }

  const size_t blob_offset_octets =
      std::min(ch.blob.size(), ch.blob_section * kMaxGattAttributeSize);
  const size_t blob_remaining = ch.blob.size() - blob_offset_octets;
  const size_t attribute_size = std::min(kMaxGattAttributeSize, blob_remaining);

  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG,
           "%s: connection:%d (%s) reading attr:%d attribute_offset_octets:%d "
           "blob_section:%u (is_long:%u)",
           __func__, conn_id, addr.c_str(), attr_handle,
           attribute_offset_octets, ch.blob_section, is_long);

  btgatt_response_t response;
  response.attr_value.len = 0;

  if (attribute_offset_octets < static_cast<int>(attribute_size)) {
    std::copy(ch.blob.begin() + blob_offset_octets + attribute_offset_octets,
              ch.blob.begin() + blob_offset_octets + attribute_size,
              response.attr_value.value);
    response.attr_value.len = attribute_size - attribute_offset_octets;
  }

  response.attr_value.handle = attr_handle;
  response.attr_value.offset = attribute_offset_octets;
  response.attr_value.auth_req = 0;
  g_internal->gatt->server->send_response(conn_id, trans_id, 0, response);
}

void RequestWriteCallback(int conn_id, int trans_id, const RawAddress& bda,
                          int attr_handle, int attribute_offset, bool need_rsp,
                          bool is_prep, std::vector<uint8_t> value) {
  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG,
           "%s: connection:%d (%s:trans:%d) write attr:%d attribute_offset:%d "
           "length:%zu "
           "need_resp:%u is_prep:%u",
           __func__, conn_id, addr.c_str(), trans_id, attr_handle,
           attribute_offset, value.size(), need_rsp, is_prep);

  std::lock_guard<std::mutex> lock(g_internal->lock);

  bluetooth::gatt::Characteristic& ch =
      g_internal->characteristics[attr_handle];

  ch.blob.resize(attribute_offset + value.size());

  std::copy(value.begin(), value.end(), ch.blob.begin() + attribute_offset);

  auto target_blob = g_internal->controlled_blobs.find(attr_handle);
  // If this is a control attribute, adjust offset of the target blob.
  if (target_blob != g_internal->controlled_blobs.end() &&
      ch.blob.size() == 1u) {
    g_internal->characteristics[target_blob->second].blob_section = ch.blob[0];
    LOG_INFO(LOG_TAG, "%s: updating attribute %d blob_section to %u", __func__,
             target_blob->second, ch.blob[0]);
  } else if (!is_prep) {
    // This is a single frame characteristic write.
    // Notify upwards because we're done now.
    const bluetooth::Uuid::UUID128Bit& attr_uuid = ch.uuid.To128BitBE();
    ssize_t status;
    OSI_NO_INTR(status = write(g_internal->pipefd[kPipeWriteEnd],
                               attr_uuid.data(), attr_uuid.size()));
    if (-1 == status)
      LOG_ERROR(LOG_TAG, "%s: write failed: %s", __func__, strerror(errno));
  } else {
    // This is a multi-frame characteristic write.
    // Wait for an 'RequestExecWriteCallback' to notify completion.
    g_internal->last_write = ch.uuid;
  }

  // Respond only if needed.
  if (!need_rsp) return;

  btgatt_response_t response;
  response.attr_value.handle = attr_handle;
  response.attr_value.offset = attribute_offset;
  response.attr_value.len = value.size();
  response.attr_value.auth_req = 0;
  // Provide written data back to sender for the response.
  // Remote stacks use this to validate the success of the write.
  std::copy(value.begin(), value.end(), response.attr_value.value);
  g_internal->gatt->server->send_response(conn_id, trans_id, 0, response);
}

void RequestExecWriteCallback(int conn_id, int trans_id, const RawAddress& bda,
                              int exec_write) {
  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG, "%s: connection:%d (%s:trans:%d) exec_write:%d", __func__,
           conn_id, addr.c_str(), trans_id, exec_write);

  // This 'response' data is unused for ExecWriteResponses.
  // It is only used to pass BlueDroid argument validation.
  btgatt_response_t response = {};
  g_internal->gatt->server->send_response(conn_id, trans_id, 0, response);

  if (!exec_write) return;

  std::lock_guard<std::mutex> lock(g_internal->lock);
  // Communicate the attribute Uuid as notification of a write update.
  const bluetooth::Uuid::UUID128Bit uuid = g_internal->last_write.To128BitBE();
  ssize_t status;
  OSI_NO_INTR(status = write(g_internal->pipefd[kPipeWriteEnd], uuid.data(),
                             uuid.size()));
  if (-1 == status)
    LOG_ERROR(LOG_TAG, "%s: write failed: %s", __func__, strerror(errno));
}

void ConnectionCallback(int conn_id, int server_if, int connected,
                        const RawAddress& bda) {
  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG, "%s: connection:%d server_if:%d connected:%d addr:%s",
           __func__, conn_id, server_if, connected, addr.c_str());
  if (connected == 1) {
    g_internal->connections.insert(conn_id);
  } else if (connected == 0) {
    g_internal->connections.erase(conn_id);
  }
}

void EnableAdvertisingCallback(uint8_t status) {
  LOG_INFO(LOG_TAG, "%s: status:%d", __func__, status);
  // This terminates a Start call.
  std::lock_guard<std::mutex> lock(g_internal->lock);
  g_internal->api_synchronize.notify_one();
}

void RegisterClientCallback(int status, int client_if,
                            const bluetooth::Uuid& app_uuid) {
  LOG_INFO(LOG_TAG, "%s: status:%d client_if:%d uuid[0]:%s", __func__, status,
           client_if, app_uuid.ToString().c_str());
  g_internal->client_if = client_if;

  // Setup our advertisement. This has no callback.
  g_internal->gatt->advertiser->SetData(0 /* std_inst */, false,
                                        {/*TODO: put inverval 2,2 here*/},
                                        base::DoNothing());

  g_internal->gatt->advertiser->Enable(
      0 /* std_inst */, true, base::Bind(&EnableAdvertisingCallback),
      0 /* no duration */, 0 /* no maxExtAdvEvent*/, base::DoNothing());
}

void ServiceStoppedCallback(int status, int server_if, int srvc_handle) {
  LOG_INFO(LOG_TAG, "%s: status:%d server_if:%d srvc_handle:%d", __func__,
           status, server_if, srvc_handle);
  // This terminates a Stop call.
  // TODO(icoolidge): make this symmetric with start
  std::lock_guard<std::mutex> lock(g_internal->lock);
  g_internal->api_synchronize.notify_one();
}

void ScanResultCallback(uint16_t ble_evt_type, uint8_t addr_type,
                        RawAddress* bda, uint8_t ble_primary_phy,
                        uint8_t ble_secondary_phy, uint8_t ble_advertising_sid,
                        int8_t ble_tx_power, int8_t rssi,
                        uint16_t ble_periodic_adv_int,
                        std::vector<uint8_t> adv_data) {
  std::string addr(BtAddrString(bda));
  std::lock_guard<std::mutex> lock(g_internal->lock);
  g_internal->scan_results[addr] = rssi;
}

void ClientConnectCallback(int conn_id, int status, int client_if,
                           const RawAddress& bda) {
  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG, "%s: conn_id:%d status:%d client_if:%d %s", __func__,
           conn_id, status, client_if, addr.c_str());
}

void ClientDisconnectCallback(int conn_id, int status, int client_if,
                              const RawAddress& bda) {
  std::string addr(BtAddrString(&bda));
  LOG_INFO(LOG_TAG, "%s: conn_id:%d status:%d client_if:%d %s", __func__,
           conn_id, status, client_if, addr.c_str());
}

void IndicationSentCallback(UNUSED_ATTR int conn_id, UNUSED_ATTR int status) {
  // TODO(icoolidge): what to do
}

void ResponseConfirmationCallback(UNUSED_ATTR int status,
                                  UNUSED_ATTR int handle) {
  // TODO(icoolidge): what to do
}

const btgatt_server_callbacks_t gatt_server_callbacks = {
    RegisterServerCallback,
    ConnectionCallback,
    ServiceAddedCallback,
    ServiceStoppedCallback,
    nullptr, /* service_deleted_cb */
    RequestReadCallback,
    RequestReadCallback,
    RequestWriteCallback,
    RequestWriteCallback,
    RequestExecWriteCallback,
    ResponseConfirmationCallback,
    IndicationSentCallback,
    nullptr, /* congestion_cb*/
    nullptr, /* mtu_changed_cb */
    nullptr, /* phy_update_cb */
    nullptr, /* conn_update_cb */
};

// TODO(eisenbach): Refactor GATT interface to not require servers
// to refer to the client interface.
const btgatt_client_callbacks_t gatt_client_callbacks = {
    RegisterClientCallback,
    ClientConnectCallback,
    ClientDisconnectCallback,
    nullptr, /* search_complete_cb; */
    nullptr, /* register_for_notification_cb; */
    nullptr, /* notify_cb; */
    nullptr, /* read_characteristic_cb; */
    nullptr, /* write_characteristic_cb; */
    nullptr, /* read_descriptor_cb; */
    nullptr, /* write_descriptor_cb; */
    nullptr, /* execute_write_cb; */
    nullptr, /* read_remote_rssi_cb; */
    nullptr, /* configure_mtu_cb; */
    nullptr, /* congestion_cb; */
    nullptr, /* get_gatt_db_cb; */
    nullptr, /* services_removed_cb */
    nullptr, /* services_added_cb */
    nullptr, /* phy_update_cb */
    nullptr, /* conn_update_cb */
};

const btgatt_scanner_callbacks_t gatt_scanner_callbacks = {
    ScanResultCallback,
    nullptr, /* batchscan_reports_cb; */
    nullptr, /* batchscan_threshold_cb; */
    nullptr, /* track_adv_event_cb; */
};

const btgatt_callbacks_t gatt_callbacks = {
    /** Set to sizeof(btgatt_callbacks_t) */
    sizeof(btgatt_callbacks_t),

    /** GATT Client callbacks */
    &gatt_client_callbacks,

    /** GATT Server callbacks */
    &gatt_server_callbacks,

    /** GATT Server callbacks */
    &gatt_scanner_callbacks,
};

}  // namespace

namespace bluetooth {
namespace gatt {

int ServerInternals::Initialize() {
  // Get the interface to the GATT profile.
  const bt_interface_t* bt_iface =
      hal::BluetoothInterface::Get()->GetHALInterface();
  gatt = reinterpret_cast<const btgatt_interface_t*>(
      bt_iface->get_profile_interface(BT_PROFILE_GATT_ID));
  if (!gatt) {
    LOG_ERROR(LOG_TAG, "Error getting GATT interface");
    return -1;
  }

  bt_status_t btstat = gatt->init(&gatt_callbacks);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to initialize gatt interface");
    return -1;
  }

  int status = pipe(pipefd);
  if (status == -1) {
    LOG_ERROR(LOG_TAG, "pipe creation failed: %s", strerror(errno));
    return -1;
  }

  return 0;
}

bt_status_t ServerInternals::AddCharacteristic(const Uuid& uuid,
                                               uint8_t properties,
                                               uint16_t permissions) {
  pending_svc_decl.push_back({.type = BTGATT_DB_CHARACTERISTIC,
                              .uuid = uuid,
                              .properties = properties,
                              .permissions = permissions});
  return BT_STATUS_SUCCESS;
}

ServerInternals::ServerInternals()
    : gatt(nullptr),
      server_if(0),
      client_if(0),
      service_handle(0),
      pipefd{INVALID_FD, INVALID_FD} {}

ServerInternals::~ServerInternals() {
  if (pipefd[0] != INVALID_FD) close(pipefd[0]);
  if (pipefd[1] != INVALID_FD) close(pipefd[1]);

  gatt->server->delete_service(server_if, service_handle);
  gatt->server->unregister_server(server_if);
  gatt->client->unregister_client(client_if);
}

Server::Server() : internal_(nullptr) {}

Server::~Server() {}

bool Server::Initialize(const Uuid& service_id, int* gatt_pipe) {
  internal_.reset(new ServerInternals);
  if (!internal_) {
    LOG_ERROR(LOG_TAG, "Error creating internals");
    return false;
  }
  g_internal = internal_.get();

  std::unique_lock<std::mutex> lock(internal_->lock);
  int status = internal_->Initialize();
  if (status) {
    LOG_ERROR(LOG_TAG, "Error initializing internals");
    return false;
  }

  bt_status_t btstat = internal_->gatt->server->register_server(service_id);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to register server");
    return false;
  }

  internal_->api_synchronize.wait(lock);
  // TODO(icoolidge): Better error handling.
  if (internal_->server_if == 0) {
    LOG_ERROR(LOG_TAG, "Initialization of server failed");
    return false;
  }

  *gatt_pipe = internal_->pipefd[kPipeReadEnd];
  LOG_INFO(LOG_TAG, "Server Initialize succeeded");
  return true;
}

bool Server::SetAdvertisement(const std::vector<Uuid>& ids,
                              const std::vector<uint8_t>& service_data,
                              const std::vector<uint8_t>& manufacturer_data,
                              bool transmit_name) {
  // std::vector<uint8_t> id_data;
  // const auto& mutable_manufacturer_data = manufacturer_data;
  // const auto& mutable_service_data = service_data;

  // for (const Uuid &id : ids) {
  //   const auto le_id = id.To128BitLE();
  //   id_data.insert(id_data.end(), le_id.begin(), le_id.end());
  // }

  std::lock_guard<std::mutex> lock(internal_->lock);

  // Setup our advertisement. This has no callback.
  internal_->gatt->advertiser->SetData(0, false, /* beacon, not scan response */
                                       {}, base::DoNothing());
  // transmit_name,               /* name */
  // 2, 2,                         interval
  // mutable_manufacturer_data,
  // mutable_service_data,
  // id_data);
  return true;
}

bool Server::SetScanResponse(const std::vector<Uuid>& ids,
                             const std::vector<uint8_t>& service_data,
                             const std::vector<uint8_t>& manufacturer_data,
                             bool transmit_name) {
  // std::vector<uint8_t> id_data;
  // const auto& mutable_manufacturer_data = manufacturer_data;
  // const auto& mutable_service_data = service_data;

  // for (const Uuid &id : ids) {
  //   const auto le_id = id.To128BitLE();
  //   id_data.insert(id_data.end(), le_id.begin(), le_id.end());
  // }

  std::lock_guard<std::mutex> lock(internal_->lock);

  // Setup our advertisement. This has no callback.
  internal_->gatt->advertiser->SetData(0, true, /* scan response */
                                       {}, base::DoNothing());
  // transmit_name,              /* name */
  // false,                      /* no txpower */
  // 2, 2,                        interval
  // 0,                          /* appearance */
  // mutable_manufacturer_data,
  // mutable_service_data,
  // id_data);
  return true;
}

bool Server::AddCharacteristic(const Uuid& id, int properties,
                               int permissions) {
  std::unique_lock<std::mutex> lock(internal_->lock);
  bt_status_t btstat =
      internal_->AddCharacteristic(id, properties, permissions);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to add characteristic to service: 0x%04x",
              internal_->service_handle);
    return false;
  }
  internal_->api_synchronize.wait(lock);
  const int handle = internal_->uuid_to_attribute[id];
  internal_->characteristics[handle].notify = properties & kPropertyNotify;
  return true;
}

bool Server::AddBlob(const Uuid& id, const Uuid& control_id, int properties,
                     int permissions) {
  std::unique_lock<std::mutex> lock(internal_->lock);

  // First, add the primary attribute (characteristic value)
  bt_status_t btstat =
      internal_->AddCharacteristic(id, properties, permissions);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to set scan response data");
    return false;
  }

  // Next, add the secondary attribute (blob control).
  // Control attributes have fixed permissions/properties.
  // Remember position at which blob was added.
  blob_index.insert(pending_svc_decl.size());
  btstat =
      internal_->AddCharacteristic(control_id, kPropertyRead | kPropertyWrite,
                                   kPermissionRead | kPermissionWrite);

  return true;
}

bool Server::Start() {
  std::unique_lock<std::mutex> lock(internal_->lock);
  bt_status_t btstat = internal_->gatt->server->add_service(
      internal_->server_if, pending_svc_decl);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to start service with handle: 0x%04x",
              internal_->service_handle);
    return false;
  }
  internal_->api_synchronize.wait(lock);
  return true;
}

bool Server::Stop() {
  std::unique_lock<std::mutex> lock(internal_->lock);
  bt_status_t btstat = internal_->gatt->server->stop_service(
      internal_->server_if, internal_->service_handle);
  if (btstat != BT_STATUS_SUCCESS) {
    LOG_ERROR(LOG_TAG, "Failed to stop service with handle: 0x%04x",
              internal_->service_handle);
    return false;
  }
  internal_->api_synchronize.wait(lock);
  return true;
}

bool Server::ScanEnable() {
  internal_->gatt->scanner->Scan(true);
  return true;
}

bool Server::ScanDisable() {
  internal_->gatt->scanner->Scan(false);
  return true;
}

bool Server::GetScanResults(ScanResults* results) {
  std::lock_guard<std::mutex> lock(internal_->lock);
  *results = internal_->scan_results;
  return true;
}

bool Server::SetCharacteristicValue(const Uuid& id,
                                    const std::vector<uint8_t>& value) {
  std::lock_guard<std::mutex> lock(internal_->lock);
  const int attribute_id = internal_->uuid_to_attribute[id];
  Characteristic& ch = internal_->characteristics[attribute_id];
  ch.next_blob = value;
  ch.next_blob_pending = true;

  if (!ch.notify) return true;

  for (auto connection : internal_->connections) {
    internal_->gatt->server->send_indication(internal_->server_if, attribute_id,
                                             connection, true, {0});
  }
  return true;
}

bool Server::GetCharacteristicValue(const Uuid& id,
                                    std::vector<uint8_t>* value) {
  std::lock_guard<std::mutex> lock(internal_->lock);
  const int attribute_id = internal_->uuid_to_attribute[id];
  *value = internal_->characteristics[attribute_id].blob;
  return true;
}

}  // namespace gatt
}  // namespace bluetooth
