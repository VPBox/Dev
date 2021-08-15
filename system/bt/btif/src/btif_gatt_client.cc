/******************************************************************************
 *
 *  Copyright 2009-2014 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *  Filename:      btif_gatt_client.c
 *
 *  Description:   GATT client implementation
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_gattc"

#include <base/at_exit.h>
#include <base/bind.h>
#include <base/threading/thread.h>
#include <errno.h>
#include <hardware/bluetooth.h>
#include <stdlib.h>
#include <string.h>
#include "device/include/controller.h"

#include "btif_common.h"
#include "btif_util.h"

#include <hardware/bt_gatt.h>

#include "bta_api.h"
#include "bta_gatt_api.h"
#include "btif_config.h"
#include "btif_dm.h"
#include "btif_gatt.h"
#include "btif_gatt_util.h"
#include "btif_storage.h"
#include "osi/include/log.h"
#include "stack/include/btu.h"
#include "vendor_api.h"

using base::Bind;
using base::Owned;
using bluetooth::Uuid;
using std::vector;

extern bt_status_t btif_gattc_test_command_impl(
    int command, const btgatt_test_params_t* params);
extern const btgatt_callbacks_t* bt_gatt_callbacks;

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/

#define CLI_CBACK_IN_JNI(P_CBACK, ...)                                         \
  do {                                                                         \
    if (bt_gatt_callbacks && bt_gatt_callbacks->client->P_CBACK) {             \
      BTIF_TRACE_API("HAL bt_gatt_callbacks->client->%s", #P_CBACK);           \
      do_in_jni_thread(Bind(bt_gatt_callbacks->client->P_CBACK, __VA_ARGS__)); \
    } else {                                                                   \
      ASSERTC(0, "Callback is NULL", 0);                                       \
    }                                                                          \
  } while (0)

#define CHECK_BTGATT_INIT()                                      \
  do {                                                           \
    if (bt_gatt_callbacks == NULL) {                             \
      LOG_WARN(LOG_TAG, "%s: BTGATT not initialized", __func__); \
      return BT_STATUS_NOT_READY;                                \
    } else {                                                     \
      LOG_VERBOSE(LOG_TAG, "%s", __func__);                      \
    }                                                            \
  } while (0)

#define BLE_RESOLVE_ADDR_MSB                                                   \
  0x40                             /* bit7, bit6 is 01 to be resolvable random \
                                      */
#define BLE_RESOLVE_ADDR_MASK 0xc0 /* bit 6, and bit7 */
inline bool BTM_BLE_IS_RESOLVE_BDA(const RawAddress& x) {
  return ((x.address)[0] & BLE_RESOLVE_ADDR_MASK) == BLE_RESOLVE_ADDR_MSB;
}
namespace {

uint8_t rssi_request_client_if;

void btif_gattc_upstreams_evt(uint16_t event, char* p_param) {
  LOG_VERBOSE(LOG_TAG, "%s: Event %d", __func__, event);

  tBTA_GATTC* p_data = (tBTA_GATTC*)p_param;
  switch (event) {
    case BTA_GATTC_DEREG_EVT:
      break;

    case BTA_GATTC_EXEC_EVT: {
      HAL_CBACK(bt_gatt_callbacks, client->execute_write_cb,
                p_data->exec_cmpl.conn_id, p_data->exec_cmpl.status);
      break;
    }

    case BTA_GATTC_SEARCH_CMPL_EVT: {
      HAL_CBACK(bt_gatt_callbacks, client->search_complete_cb,
                p_data->search_cmpl.conn_id, p_data->search_cmpl.status);
      break;
    }

    case BTA_GATTC_NOTIF_EVT: {
      btgatt_notify_params_t data;

      data.bda = p_data->notify.bda;
      memcpy(data.value, p_data->notify.value, p_data->notify.len);

      data.handle = p_data->notify.handle;
      data.is_notify = p_data->notify.is_notify;
      data.len = p_data->notify.len;

      HAL_CBACK(bt_gatt_callbacks, client->notify_cb, p_data->notify.conn_id,
                data);

      if (!p_data->notify.is_notify)
        BTA_GATTC_SendIndConfirm(p_data->notify.conn_id, p_data->notify.handle);

      break;
    }

    case BTA_GATTC_OPEN_EVT: {
      DVLOG(1) << "BTA_GATTC_OPEN_EVT " << p_data->open.remote_bda;
      HAL_CBACK(bt_gatt_callbacks, client->open_cb, p_data->open.conn_id,
                p_data->open.status, p_data->open.client_if,
                p_data->open.remote_bda);

      if (GATT_DEF_BLE_MTU_SIZE != p_data->open.mtu && p_data->open.mtu) {
        HAL_CBACK(bt_gatt_callbacks, client->configure_mtu_cb,
                  p_data->open.conn_id, p_data->open.status, p_data->open.mtu);
      }

      if (p_data->open.status == GATT_SUCCESS)
        btif_gatt_check_encrypted_link(p_data->open.remote_bda,
                                       p_data->open.transport);
      break;
    }

    case BTA_GATTC_CLOSE_EVT: {
      HAL_CBACK(bt_gatt_callbacks, client->close_cb, p_data->close.conn_id,
                p_data->status, p_data->close.client_if,
                p_data->close.remote_bda);
      break;
    }

    case BTA_GATTC_ACL_EVT:
      LOG_DEBUG(LOG_TAG, "BTA_GATTC_ACL_EVT: status = %d", p_data->status);
      /* Ignore for now */
      break;

    case BTA_GATTC_SEARCH_RES_EVT:
      break;

    case BTA_GATTC_CANCEL_OPEN_EVT:
      break;

    case BTA_GATTC_CFG_MTU_EVT: {
      HAL_CBACK(bt_gatt_callbacks, client->configure_mtu_cb,
                p_data->cfg_mtu.conn_id, p_data->cfg_mtu.status,
                p_data->cfg_mtu.mtu);
      break;
    }

    case BTA_GATTC_CONGEST_EVT:
      HAL_CBACK(bt_gatt_callbacks, client->congestion_cb,
                p_data->congest.conn_id, p_data->congest.congested);
      break;

    case BTA_GATTC_PHY_UPDATE_EVT:
      HAL_CBACK(bt_gatt_callbacks, client->phy_updated_cb,
                p_data->phy_update.conn_id, p_data->phy_update.tx_phy,
                p_data->phy_update.rx_phy, p_data->phy_update.status);
      break;

    case BTA_GATTC_CONN_UPDATE_EVT:
      HAL_CBACK(bt_gatt_callbacks, client->conn_updated_cb,
                p_data->conn_update.conn_id, p_data->conn_update.interval,
                p_data->conn_update.latency, p_data->conn_update.timeout,
                p_data->conn_update.status);
      break;

    default:
      LOG_ERROR(LOG_TAG, "%s: Unhandled event (%d)!", __func__, event);
      break;
  }
}

void bta_gattc_cback(tBTA_GATTC_EVT event, tBTA_GATTC* p_data) {
  bt_status_t status =
      btif_transfer_context(btif_gattc_upstreams_evt, (uint16_t)event,
                            (char*)p_data, sizeof(tBTA_GATTC), NULL);
  ASSERTC(status == BT_STATUS_SUCCESS, "Context transfer failed!", status);
}

void btm_read_rssi_cb(void* p_void) {
  tBTM_RSSI_RESULT* p_result = (tBTM_RSSI_RESULT*)p_void;

  if (!p_result) return;

  CLI_CBACK_IN_JNI(read_remote_rssi_cb, rssi_request_client_if,
                   p_result->rem_bda, p_result->rssi, p_result->status);
}

/*******************************************************************************
 *  Client API Functions
 ******************************************************************************/

bt_status_t btif_gattc_register_app(const Uuid& uuid) {
  CHECK_BTGATT_INIT();

  return do_in_jni_thread(Bind(
      [](const Uuid& uuid) {
        BTA_GATTC_AppRegister(
            bta_gattc_cback,
            base::Bind(
                [](const Uuid& uuid, uint8_t client_id, uint8_t status) {
                  do_in_jni_thread(Bind(
                      [](const Uuid& uuid, uint8_t client_id, uint8_t status) {
                        HAL_CBACK(bt_gatt_callbacks, client->register_client_cb,
                                  status, client_id, uuid);
                      },
                      uuid, client_id, status));
                },
                uuid));
      },
      uuid));
}

void btif_gattc_unregister_app_impl(int client_if) {
  BTA_GATTC_AppDeregister(client_if);
}

bt_status_t btif_gattc_unregister_app(int client_if) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&btif_gattc_unregister_app_impl, client_if));
}

void btif_gattc_open_impl(int client_if, RawAddress address, bool is_direct,
                          int transport_p, bool opportunistic,
                          int initiating_phys) {
  // Ensure device is in inquiry database
  int addr_type = 0;
  int device_type = 0;
  tGATT_TRANSPORT transport = (tGATT_TRANSPORT)GATT_TRANSPORT_LE;

  if (btif_get_address_type(address, &addr_type) &&
      btif_get_device_type(address, &device_type) &&
      device_type != BT_DEVICE_TYPE_BREDR) {
    BTA_DmAddBleDevice(address, addr_type, device_type);
  }

  // Check for background connections
  if (!is_direct) {
    // Check for privacy 1.0 and 1.1 controller and do not start background
    // connection if RPA offloading is not supported, since it will not
    // connect after change of random address
    if (!controller_get_interface()->supports_ble_privacy() &&
        (addr_type == BLE_ADDR_RANDOM) && BTM_BLE_IS_RESOLVE_BDA(address)) {
      tBTM_BLE_VSC_CB vnd_capabilities;
      BTM_BleGetVendorCapabilities(&vnd_capabilities);
      if (!vnd_capabilities.rpa_offloading) {
        HAL_CBACK(bt_gatt_callbacks, client->open_cb, 0, BT_STATUS_UNSUPPORTED,
                  client_if, address);
        return;
      }
    }
  }

  // Determine transport
  if (transport_p != GATT_TRANSPORT_AUTO) {
    transport = transport_p;
  } else {
    switch (device_type) {
      case BT_DEVICE_TYPE_BREDR:
        transport = GATT_TRANSPORT_BR_EDR;
        break;

      case BT_DEVICE_TYPE_BLE:
        transport = GATT_TRANSPORT_LE;
        break;

      case BT_DEVICE_TYPE_DUMO:
        if (transport_p == GATT_TRANSPORT_LE)
          transport = GATT_TRANSPORT_LE;
        else
          transport = GATT_TRANSPORT_BR_EDR;
        break;
    }
  }

  // Connect!
  BTIF_TRACE_DEBUG("%s Transport=%d, device type=%d, phy=%d", __func__,
                   transport, device_type, initiating_phys);
  BTA_GATTC_Open(client_if, address, is_direct, transport, opportunistic,
                 initiating_phys);
}

bt_status_t btif_gattc_open(int client_if, const RawAddress& bd_addr,
                            bool is_direct, int transport, bool opportunistic,
                            int initiating_phys) {
  CHECK_BTGATT_INIT();
  // Closure will own this value and free it.
  return do_in_jni_thread(Bind(&btif_gattc_open_impl, client_if, bd_addr,
                               is_direct, transport, opportunistic,
                               initiating_phys));
}

void btif_gattc_close_impl(int client_if, RawAddress address, int conn_id) {
  // Disconnect established connections
  if (conn_id != 0)
    BTA_GATTC_Close(conn_id);
  else
    BTA_GATTC_CancelOpen(client_if, address, true);

  // Cancel pending background connections (remove from whitelist)
  BTA_GATTC_CancelOpen(client_if, address, false);
}

bt_status_t btif_gattc_close(int client_if, const RawAddress& bd_addr,
                             int conn_id) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      Bind(&btif_gattc_close_impl, client_if, bd_addr, conn_id));
}

bt_status_t btif_gattc_refresh(int client_if, const RawAddress& bd_addr) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTC_Refresh, bd_addr));
}

bt_status_t btif_gattc_search_service(int conn_id, const Uuid* filter_uuid) {
  CHECK_BTGATT_INIT();

  if (filter_uuid) {
    Uuid* uuid = new Uuid(*filter_uuid);
    return do_in_jni_thread(
        Bind(&BTA_GATTC_ServiceSearchRequest, conn_id, base::Owned(uuid)));
  } else {
    return do_in_jni_thread(
        Bind(&BTA_GATTC_ServiceSearchRequest, conn_id, nullptr));
  }
}

void btif_gattc_discover_service_by_uuid(int conn_id, const Uuid& uuid) {
  do_in_jni_thread(Bind(&BTA_GATTC_DiscoverServiceByUuid, conn_id, uuid));
}

void btif_gattc_get_gatt_db_impl(int conn_id) {
  btgatt_db_element_t* db = NULL;
  int count = 0;
  BTA_GATTC_GetGattDb(conn_id, 0x0000, 0xFFFF, &db, &count);

  HAL_CBACK(bt_gatt_callbacks, client->get_gatt_db_cb, conn_id, db, count);
  osi_free(db);
}

bt_status_t btif_gattc_get_gatt_db(int conn_id) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&btif_gattc_get_gatt_db_impl, conn_id));
}

void read_char_cb(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                  uint16_t len, uint8_t* value, void* data) {
  btgatt_read_params_t* params = new btgatt_read_params_t;
  params->value_type = 0x00 /* GATTC_READ_VALUE_TYPE_VALUE */;
  params->status = status;
  params->handle = handle;
  params->value.len = len;
  CHECK(len <= BTGATT_MAX_ATTR_LEN);
  if (len > 0) memcpy(params->value.value, value, len);

  // clang-tidy analyzer complains about |params| is leaked.  It doesn't know
  // that |param| will be freed by the callback function.
  CLI_CBACK_IN_JNI(read_characteristic_cb, conn_id, status, /* NOLINT */
                   base::Owned(params));
}

bt_status_t btif_gattc_read_char(int conn_id, uint16_t handle, int auth_req) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTC_ReadCharacteristic, conn_id, handle,
                               auth_req, read_char_cb, nullptr));
}

void read_using_char_uuid_cb(uint16_t conn_id, tGATT_STATUS status,
                             uint16_t handle, uint16_t len, uint8_t* value,
                             void* data) {
  btgatt_read_params_t* params = new btgatt_read_params_t;
  params->value_type = 0x00 /* GATTC_READ_VALUE_TYPE_VALUE */;
  params->status = status;
  params->handle = handle;
  params->value.len = len;
  CHECK(len <= BTGATT_MAX_ATTR_LEN);
  if (len > 0) memcpy(params->value.value, value, len);

  // clang-tidy analyzer complains about |params| is leaked.  It doesn't know
  // that |param| will be freed by the callback function.
  CLI_CBACK_IN_JNI(read_characteristic_cb, conn_id, status, /* NOLINT */
                   base::Owned(params));
}

bt_status_t btif_gattc_read_using_char_uuid(int conn_id, const Uuid& uuid,
                                            uint16_t s_handle,
                                            uint16_t e_handle, int auth_req) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTC_ReadUsingCharUuid, conn_id, uuid,
                               s_handle, e_handle, auth_req,
                               read_using_char_uuid_cb, nullptr));
}

void read_desc_cb(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                  uint16_t len, uint8_t* value, void* data) {
  btgatt_read_params_t params;
  params.value_type = 0x00 /* GATTC_READ_VALUE_TYPE_VALUE */;
  params.status = status;
  params.handle = handle;
  params.value.len = len;
  CHECK(len <= BTGATT_MAX_ATTR_LEN);
  if (len > 0) memcpy(params.value.value, value, len);

  CLI_CBACK_IN_JNI(read_descriptor_cb, conn_id, status, params);
}

bt_status_t btif_gattc_read_char_descr(int conn_id, uint16_t handle,
                                       int auth_req) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTC_ReadCharDescr, conn_id, handle,
                               auth_req, read_desc_cb, nullptr));
}

void write_char_cb(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                   void* data) {
  CLI_CBACK_IN_JNI(write_characteristic_cb, conn_id, status, handle);
}

bt_status_t btif_gattc_write_char(int conn_id, uint16_t handle, int write_type,
                                  int auth_req, vector<uint8_t> value) {
  CHECK_BTGATT_INIT();

  if (value.size() > BTGATT_MAX_ATTR_LEN) value.resize(BTGATT_MAX_ATTR_LEN);

  return do_in_jni_thread(Bind(&BTA_GATTC_WriteCharValue, conn_id, handle,
                               write_type, std::move(value), auth_req,
                               write_char_cb, nullptr));
}

void write_descr_cb(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                    void* data) {
  CLI_CBACK_IN_JNI(write_descriptor_cb, conn_id, status, handle);
}

bt_status_t btif_gattc_write_char_descr(int conn_id, uint16_t handle,
                                        int auth_req, vector<uint8_t> value) {
  CHECK_BTGATT_INIT();

  if (value.size() > BTGATT_MAX_ATTR_LEN) value.resize(BTGATT_MAX_ATTR_LEN);

  return do_in_jni_thread(Bind(&BTA_GATTC_WriteCharDescr, conn_id, handle,
                               std::move(value), auth_req, write_descr_cb,
                               nullptr));
}

bt_status_t btif_gattc_execute_write(int conn_id, int execute) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      Bind(&BTA_GATTC_ExecuteWrite, conn_id, (uint8_t)execute));
}

void btif_gattc_reg_for_notification_impl(tGATT_IF client_if,
                                          const RawAddress& bda,
                                          uint16_t handle) {
  tGATT_STATUS status =
      BTA_GATTC_RegisterForNotifications(client_if, bda, handle);

  // TODO(jpawlowski): conn_id is currently unused
  HAL_CBACK(bt_gatt_callbacks, client->register_for_notification_cb,
            /* conn_id */ 0, 1, status, handle);
}

bt_status_t btif_gattc_reg_for_notification(int client_if,
                                            const RawAddress& bd_addr,
                                            uint16_t handle) {
  CHECK_BTGATT_INIT();

  return do_in_jni_thread(
      Bind(base::IgnoreResult(&btif_gattc_reg_for_notification_impl), client_if,
           bd_addr, handle));
}

void btif_gattc_dereg_for_notification_impl(tGATT_IF client_if,
                                            const RawAddress& bda,
                                            uint16_t handle) {
  tGATT_STATUS status =
      BTA_GATTC_DeregisterForNotifications(client_if, bda, handle);

  // TODO(jpawlowski): conn_id is currently unused
  HAL_CBACK(bt_gatt_callbacks, client->register_for_notification_cb,
            /* conn_id */ 0, 0, status, handle);
}

bt_status_t btif_gattc_dereg_for_notification(int client_if,
                                              const RawAddress& bd_addr,
                                              uint16_t handle) {
  CHECK_BTGATT_INIT();

  return do_in_jni_thread(
      Bind(base::IgnoreResult(&btif_gattc_dereg_for_notification_impl),
           client_if, bd_addr, handle));
}

bt_status_t btif_gattc_read_remote_rssi(int client_if,
                                        const RawAddress& bd_addr) {
  CHECK_BTGATT_INIT();
  rssi_request_client_if = client_if;

  return do_in_jni_thread(
      Bind(base::IgnoreResult(&BTM_ReadRSSI), bd_addr, btm_read_rssi_cb));
}

bt_status_t btif_gattc_configure_mtu(int conn_id, int mtu) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      Bind(base::IgnoreResult(&BTA_GATTC_ConfigureMTU), conn_id, mtu));
}

void btif_gattc_conn_parameter_update_impl(RawAddress addr, int min_interval,
                                           int max_interval, int latency,
                                           int timeout, uint16_t min_ce_len,
                                           uint16_t max_ce_len) {
  if (BTA_DmGetConnectionState(addr))
    BTA_DmBleUpdateConnectionParams(addr, min_interval, max_interval, latency,
                                    timeout, min_ce_len, max_ce_len);
  else
    BTA_DmSetBlePrefConnParams(addr, min_interval, max_interval, latency,
                               timeout);
}

bt_status_t btif_gattc_conn_parameter_update(const RawAddress& bd_addr,
                                             int min_interval, int max_interval,
                                             int latency, int timeout,
                                             uint16_t min_ce_len,
                                             uint16_t max_ce_len) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(
      base::IgnoreResult(&btif_gattc_conn_parameter_update_impl), bd_addr,
      min_interval, max_interval, latency, timeout, min_ce_len, max_ce_len));
}

bt_status_t btif_gattc_set_preferred_phy(const RawAddress& bd_addr,
                                         uint8_t tx_phy, uint8_t rx_phy,
                                         uint16_t phy_options) {
  CHECK_BTGATT_INIT();
  do_in_main_thread(FROM_HERE,
                    Bind(&BTM_BleSetPhy, bd_addr, tx_phy, rx_phy, phy_options));
  return BT_STATUS_SUCCESS;
}

bt_status_t btif_gattc_read_phy(
    const RawAddress& bd_addr,
    base::Callback<void(uint8_t tx_phy, uint8_t rx_phy, uint8_t status)> cb) {
  CHECK_BTGATT_INIT();
  do_in_main_thread(FROM_HERE, Bind(&BTM_BleReadPhy, bd_addr,
                                    jni_thread_wrapper(FROM_HERE, cb)));
  return BT_STATUS_SUCCESS;
}

int btif_gattc_get_device_type(const RawAddress& bd_addr) {
  int device_type = 0;

  if (btif_config_get_int(bd_addr.ToString().c_str(), "DevType", &device_type))
    return device_type;
  return 0;
}

bt_status_t btif_gattc_test_command(int command,
                                    const btgatt_test_params_t& params) {
  return btif_gattc_test_command_impl(command, &params);
}

}  // namespace

const btgatt_client_interface_t btgattClientInterface = {
    btif_gattc_register_app,
    btif_gattc_unregister_app,
    btif_gattc_open,
    btif_gattc_close,
    btif_gattc_refresh,
    btif_gattc_search_service,
    btif_gattc_discover_service_by_uuid,
    btif_gattc_read_char,
    btif_gattc_read_using_char_uuid,
    btif_gattc_write_char,
    btif_gattc_read_char_descr,
    btif_gattc_write_char_descr,
    btif_gattc_execute_write,
    btif_gattc_reg_for_notification,
    btif_gattc_dereg_for_notification,
    btif_gattc_read_remote_rssi,
    btif_gattc_get_device_type,
    btif_gattc_configure_mtu,
    btif_gattc_conn_parameter_update,
    btif_gattc_set_preferred_phy,
    btif_gattc_read_phy,
    btif_gattc_test_command,
    btif_gattc_get_gatt_db};
