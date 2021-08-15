/******************************************************************************
 *
 *  Copyright 2009-2013 Broadcom Corporation
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
 *  Filename:      btif_gatt_server.c
 *
 *  Description:   GATT server implementation
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_gatt"

#include <base/bind.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/bluetooth.h>
#include <hardware/bt_gatt.h>

#include "btif_common.h"
#include "btif_util.h"

#include "bt_common.h"
#include "bta_api.h"
#include "bta_gatt_api.h"
#include "btif_config.h"
#include "btif_dm.h"
#include "btif_gatt.h"
#include "btif_gatt_util.h"
#include "btif_storage.h"
#include "osi/include/log.h"
#include "stack/include/btu.h"

using base::Bind;
using base::Owned;
using bluetooth::Uuid;
using std::vector;

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/

#define CHECK_BTGATT_INIT()                                      \
  do {                                                           \
    if (bt_gatt_callbacks == NULL) {                             \
      LOG_WARN(LOG_TAG, "%s: BTGATT not initialized", __func__); \
      return BT_STATUS_NOT_READY;                                \
    } else {                                                     \
      LOG_VERBOSE(LOG_TAG, "%s", __func__);                      \
    }                                                            \
  } while (0)

/*******************************************************************************
 *  Static variables
 ******************************************************************************/

extern const btgatt_callbacks_t* bt_gatt_callbacks;

/*******************************************************************************
 *  Static functions
 ******************************************************************************/

static void btapp_gatts_copy_req_data(uint16_t event, char* p_dest,
                                      char* p_src) {
  tBTA_GATTS* p_dest_data = (tBTA_GATTS*)p_dest;
  tBTA_GATTS* p_src_data = (tBTA_GATTS*)p_src;

  if (!p_src_data || !p_dest_data) return;

  // Copy basic structure first
  maybe_non_aligned_memcpy(p_dest_data, p_src_data, sizeof(*p_src_data));

  // Allocate buffer for request data if necessary
  switch (event) {
    case BTA_GATTS_READ_CHARACTERISTIC_EVT:
    case BTA_GATTS_READ_DESCRIPTOR_EVT:
    case BTA_GATTS_WRITE_CHARACTERISTIC_EVT:
    case BTA_GATTS_WRITE_DESCRIPTOR_EVT:
    case BTA_GATTS_EXEC_WRITE_EVT:
    case BTA_GATTS_MTU_EVT:
      p_dest_data->req_data.p_data =
          (tGATTS_DATA*)osi_malloc(sizeof(tGATTS_DATA));
      memcpy(p_dest_data->req_data.p_data, p_src_data->req_data.p_data,
             sizeof(tGATTS_DATA));
      break;

    default:
      break;
  }
}

static void btapp_gatts_free_req_data(uint16_t event, tBTA_GATTS* p_data) {
  switch (event) {
    case BTA_GATTS_READ_CHARACTERISTIC_EVT:
    case BTA_GATTS_READ_DESCRIPTOR_EVT:
    case BTA_GATTS_WRITE_CHARACTERISTIC_EVT:
    case BTA_GATTS_WRITE_DESCRIPTOR_EVT:
    case BTA_GATTS_EXEC_WRITE_EVT:
    case BTA_GATTS_MTU_EVT:
      if (p_data != NULL) osi_free_and_reset((void**)&p_data->req_data.p_data);
      break;

    default:
      break;
  }
}

static void btapp_gatts_handle_cback(uint16_t event, char* p_param) {
  LOG_VERBOSE(LOG_TAG, "%s: Event %d", __func__, event);

  tBTA_GATTS* p_data = (tBTA_GATTS*)p_param;
  switch (event) {
    case BTA_GATTS_REG_EVT: {
      HAL_CBACK(bt_gatt_callbacks, server->register_server_cb,
                p_data->reg_oper.status, p_data->reg_oper.server_if,
                p_data->reg_oper.uuid);
      break;
    }

    case BTA_GATTS_DEREG_EVT:
      break;

    case BTA_GATTS_CONNECT_EVT: {
      btif_gatt_check_encrypted_link(p_data->conn.remote_bda,
                                     p_data->conn.transport);

      HAL_CBACK(bt_gatt_callbacks, server->connection_cb, p_data->conn.conn_id,
                p_data->conn.server_if, true, p_data->conn.remote_bda);
      break;
    }

    case BTA_GATTS_DISCONNECT_EVT: {
      HAL_CBACK(bt_gatt_callbacks, server->connection_cb, p_data->conn.conn_id,
                p_data->conn.server_if, false, p_data->conn.remote_bda);
      break;
    }

    case BTA_GATTS_STOP_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->service_stopped_cb,
                p_data->srvc_oper.status, p_data->srvc_oper.server_if,
                p_data->srvc_oper.service_id);
      break;

    case BTA_GATTS_DELELTE_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->service_deleted_cb,
                p_data->srvc_oper.status, p_data->srvc_oper.server_if,
                p_data->srvc_oper.service_id);
      break;

    case BTA_GATTS_READ_CHARACTERISTIC_EVT: {
      HAL_CBACK(bt_gatt_callbacks, server->request_read_characteristic_cb,
                p_data->req_data.conn_id, p_data->req_data.trans_id,
                p_data->req_data.remote_bda,
                p_data->req_data.p_data->read_req.handle,
                p_data->req_data.p_data->read_req.offset,
                p_data->req_data.p_data->read_req.is_long);
      break;
    }

    case BTA_GATTS_READ_DESCRIPTOR_EVT: {
      HAL_CBACK(bt_gatt_callbacks, server->request_read_descriptor_cb,
                p_data->req_data.conn_id, p_data->req_data.trans_id,
                p_data->req_data.remote_bda,
                p_data->req_data.p_data->read_req.handle,
                p_data->req_data.p_data->read_req.offset,
                p_data->req_data.p_data->read_req.is_long);
      break;
    }

    case BTA_GATTS_WRITE_CHARACTERISTIC_EVT: {
      const auto& req = p_data->req_data.p_data->write_req;
      vector<uint8_t> value(req.value, req.value + req.len);
      HAL_CBACK(bt_gatt_callbacks, server->request_write_characteristic_cb,
                p_data->req_data.conn_id, p_data->req_data.trans_id,
                p_data->req_data.remote_bda, req.handle, req.offset,
                req.need_rsp, req.is_prep, value);
      break;
    }

    case BTA_GATTS_WRITE_DESCRIPTOR_EVT: {
      const auto& req = p_data->req_data.p_data->write_req;
      vector<uint8_t> value(req.value, req.value + req.len);
      HAL_CBACK(bt_gatt_callbacks, server->request_write_descriptor_cb,
                p_data->req_data.conn_id, p_data->req_data.trans_id,
                p_data->req_data.remote_bda, req.handle, req.offset,
                req.need_rsp, req.is_prep, value);
      break;
    }

    case BTA_GATTS_EXEC_WRITE_EVT: {
      HAL_CBACK(bt_gatt_callbacks, server->request_exec_write_cb,
                p_data->req_data.conn_id, p_data->req_data.trans_id,
                p_data->req_data.remote_bda,
                p_data->req_data.p_data->exec_write);
      break;
    }

    case BTA_GATTS_CONF_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->indication_sent_cb,
                p_data->req_data.conn_id, p_data->req_data.status);
      break;

    case BTA_GATTS_CONGEST_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->congestion_cb,
                p_data->congest.conn_id, p_data->congest.congested);
      break;

    case BTA_GATTS_MTU_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->mtu_changed_cb,
                p_data->req_data.conn_id, p_data->req_data.p_data->mtu);
      break;

    case BTA_GATTS_OPEN_EVT:
    case BTA_GATTS_CANCEL_OPEN_EVT:
    case BTA_GATTS_CLOSE_EVT:
      LOG_DEBUG(LOG_TAG, "%s: Empty event (%d)!", __func__, event);
      break;

    case BTA_GATTS_PHY_UPDATE_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->phy_updated_cb,
                p_data->phy_update.conn_id, p_data->phy_update.tx_phy,
                p_data->phy_update.rx_phy, p_data->phy_update.status);
      break;

    case BTA_GATTS_CONN_UPDATE_EVT:
      HAL_CBACK(bt_gatt_callbacks, server->conn_updated_cb,
                p_data->conn_update.conn_id, p_data->conn_update.interval,
                p_data->conn_update.latency, p_data->conn_update.timeout,
                p_data->conn_update.status);
      break;

    default:
      LOG_ERROR(LOG_TAG, "%s: Unhandled event (%d)!", __func__, event);
      break;
  }

  btapp_gatts_free_req_data(event, p_data);
}

static void btapp_gatts_cback(tBTA_GATTS_EVT event, tBTA_GATTS* p_data) {
  bt_status_t status;
  status = btif_transfer_context(btapp_gatts_handle_cback, (uint16_t)event,
                                 (char*)p_data, sizeof(tBTA_GATTS),
                                 btapp_gatts_copy_req_data);
  ASSERTC(status == BT_STATUS_SUCCESS, "Context transfer failed!", status);
}

/*******************************************************************************
 *  Server API Functions
 ******************************************************************************/
static bt_status_t btif_gatts_register_app(const Uuid& bt_uuid) {
  CHECK_BTGATT_INIT();

  return do_in_jni_thread(
      Bind(&BTA_GATTS_AppRegister, bt_uuid, &btapp_gatts_cback));
}

static bt_status_t btif_gatts_unregister_app(int server_if) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTS_AppDeregister, server_if));
}

static void btif_gatts_open_impl(int server_if, const RawAddress& address,
                                 bool is_direct, int transport_param) {
  // Ensure device is in inquiry database
  int addr_type = 0;
  int device_type = 0;
  tGATT_TRANSPORT transport = GATT_TRANSPORT_LE;

  if (btif_get_address_type(address, &addr_type) &&
      btif_get_device_type(address, &device_type) &&
      device_type != BT_DEVICE_TYPE_BREDR) {
    BTA_DmAddBleDevice(address, addr_type, device_type);
  }

  // Determine transport
  if (transport_param != GATT_TRANSPORT_AUTO) {
    transport = transport_param;
  } else {
    switch (device_type) {
      case BT_DEVICE_TYPE_BREDR:
        transport = GATT_TRANSPORT_BR_EDR;
        break;

      case BT_DEVICE_TYPE_BLE:
        transport = GATT_TRANSPORT_LE;
        break;

      case BT_DEVICE_TYPE_DUMO:
        if (transport_param == GATT_TRANSPORT_LE)
          transport = GATT_TRANSPORT_LE;
        else
          transport = GATT_TRANSPORT_BR_EDR;
        break;
    }
  }

  // Connect!
  BTA_GATTS_Open(server_if, address, is_direct, transport);
}

static bt_status_t btif_gatts_open(int server_if, const RawAddress& bd_addr,
                                   bool is_direct, int transport) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      Bind(&btif_gatts_open_impl, server_if, bd_addr, is_direct, transport));
}

static void btif_gatts_close_impl(int server_if, const RawAddress& address,
                                  int conn_id) {
  // Close active connection
  if (conn_id != 0)
    BTA_GATTS_Close(conn_id);
  else
    BTA_GATTS_CancelOpen(server_if, address, true);

  // Cancel pending background connections
  BTA_GATTS_CancelOpen(server_if, address, false);
}

static bt_status_t btif_gatts_close(int server_if, const RawAddress& bd_addr,
                                    int conn_id) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      Bind(&btif_gatts_close_impl, server_if, bd_addr, conn_id));
}

static void on_service_added_cb(uint8_t status, int server_if,
                                vector<btgatt_db_element_t> service) {
  HAL_CBACK(bt_gatt_callbacks, server->service_added_cb, status, server_if,
            std::move(service));
}

static void add_service_impl(int server_if,
                             vector<btgatt_db_element_t> service) {
  // TODO(jpawlowski): btif should be a pass through layer, and no checks should
  // be made here. This exception is added only until GATT server code is
  // refactored, and one can distinguish stack-internal aps from external apps
  if (service[0].uuid == Uuid::From16Bit(UUID_SERVCLASS_GATT_SERVER) ||
      service[0].uuid == Uuid::From16Bit(UUID_SERVCLASS_GAP_SERVER)) {
    LOG_ERROR(LOG_TAG, "%s: Attept to register restricted service", __func__);
    HAL_CBACK(bt_gatt_callbacks, server->service_added_cb, BT_STATUS_FAIL,
              server_if, std::move(service));
    return;
  }

  BTA_GATTS_AddService(
      server_if, service,
      jni_thread_wrapper(FROM_HERE, base::Bind(&on_service_added_cb)));
}

static bt_status_t btif_gatts_add_service(int server_if,
                                          vector<btgatt_db_element_t> service) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(
      FROM_HERE, Bind(&add_service_impl, server_if, std::move(service)));
}

static bt_status_t btif_gatts_stop_service(int server_if, int service_handle) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTS_StopService, service_handle));
}

static bt_status_t btif_gatts_delete_service(int server_if,
                                             int service_handle) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&BTA_GATTS_DeleteService, service_handle));
}

static bt_status_t btif_gatts_send_indication(int server_if,
                                              int attribute_handle, int conn_id,
                                              int confirm,
                                              vector<uint8_t> value) {
  CHECK_BTGATT_INIT();

  if (value.size() > BTGATT_MAX_ATTR_LEN) value.resize(BTGATT_MAX_ATTR_LEN);

  return do_in_jni_thread(Bind(&BTA_GATTS_HandleValueIndication, conn_id,
                               attribute_handle, std::move(value), confirm));
  // TODO: Might need to send an ACK if handle value indication is
  //       invoked without need for confirmation.
}

static void btif_gatts_send_response_impl(int conn_id, int trans_id, int status,
                                          btgatt_response_t response) {
  tGATTS_RSP rsp_struct;
  btif_to_bta_response(&rsp_struct, &response);

  BTA_GATTS_SendRsp(conn_id, trans_id, status, &rsp_struct);

  HAL_CBACK(bt_gatt_callbacks, server->response_confirmation_cb, 0,
            rsp_struct.attr_value.handle);
}

static bt_status_t btif_gatts_send_response(int conn_id, int trans_id,
                                            int status,
                                            const btgatt_response_t& response) {
  CHECK_BTGATT_INIT();
  return do_in_jni_thread(Bind(&btif_gatts_send_response_impl, conn_id,
                               trans_id, status, response));
}

static bt_status_t btif_gatts_set_preferred_phy(const RawAddress& bd_addr,
                                                uint8_t tx_phy, uint8_t rx_phy,
                                                uint16_t phy_options) {
  CHECK_BTGATT_INIT();
  do_in_main_thread(FROM_HERE,
                    Bind(&BTM_BleSetPhy, bd_addr, tx_phy, rx_phy, phy_options));
  return BT_STATUS_SUCCESS;
}

static bt_status_t btif_gatts_read_phy(
    const RawAddress& bd_addr,
    base::Callback<void(uint8_t tx_phy, uint8_t rx_phy, uint8_t status)> cb) {
  CHECK_BTGATT_INIT();
  do_in_main_thread(FROM_HERE, Bind(&BTM_BleReadPhy, bd_addr,
                                    jni_thread_wrapper(FROM_HERE, cb)));
  return BT_STATUS_SUCCESS;
}

const btgatt_server_interface_t btgattServerInterface = {
    btif_gatts_register_app,   btif_gatts_unregister_app,
    btif_gatts_open,           btif_gatts_close,
    btif_gatts_add_service,    btif_gatts_stop_service,
    btif_gatts_delete_service, btif_gatts_send_indication,
    btif_gatts_send_response,  btif_gatts_set_preferred_phy,
    btif_gatts_read_phy};
