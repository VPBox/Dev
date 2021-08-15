/******************************************************************************
 *
 *  Copyright 2010-2012 Broadcom Corporation
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

/******************************************************************************
 *
 *  This is the implementation of the API for GATT module of BTA.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <string.h>

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/callback.h>
#include "bt_common.h"
#include "bta_gatt_api.h"
#include "bta_gattc_int.h"
#include "bta_sys.h"
#include "device/include/controller.h"
#include "stack/include/btu.h"

using bluetooth::Uuid;

/*****************************************************************************
 *  Constants
 ****************************************************************************/

static const tBTA_SYS_REG bta_gattc_reg = {bta_gattc_hdl_event,
                                           BTA_GATTC_Disable};

/*******************************************************************************
 *
 * Function         BTA_GATTC_Disable
 *
 * Description      This function is called to disable GATTC module
 *
 * Parameters       None.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_Disable(void) {
  if (!bta_sys_is_register(BTA_ID_GATTC)) {
    LOG(WARNING) << "GATTC Module not enabled/already disabled";
    return;
  }

  do_in_main_thread(FROM_HERE, base::Bind(&bta_gattc_disable));
  bta_sys_deregister(BTA_ID_GATTC);
}

/**
 * This function is called to register application callbacks with BTA GATTC
 * module. |client_cb| pointer to the application callback function.
 * |cb| one time callback when registration is finished
 */
void BTA_GATTC_AppRegister(tBTA_GATTC_CBACK* p_client_cb,
                           BtaAppRegisterCallback cb) {
  if (!bta_sys_is_register(BTA_ID_GATTC))
    bta_sys_register(BTA_ID_GATTC, &bta_gattc_reg);

  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_gattc_register, Uuid::GetRandom(), p_client_cb,
                            std::move(cb)));
}

static void app_deregister_impl(tGATT_IF client_if) {
  bta_gattc_deregister(bta_gattc_cl_get_regcb(client_if));
}
/*******************************************************************************
 *
 * Function         BTA_GATTC_AppDeregister
 *
 * Description      This function is called to deregister an application
 *                  from BTA GATTC module.
 *
 * Parameters       client_if - client interface identifier.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_AppDeregister(tGATT_IF client_if) {
  do_in_main_thread(FROM_HERE, base::Bind(&app_deregister_impl, client_if));
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_Open
 *
 * Description      Open a direct connection or add a background auto connection
 *                  bd address
 *
 * Parameters       client_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *                  transport: Transport to be used for GATT connection
 *                             (BREDR/LE)
 *                  initiating_phys: LE PHY to use, optional
 *                  opportunistic: wether the connection shall be opportunistic,
 *                                 and don't impact the disconnection timer
 *
 ******************************************************************************/
void BTA_GATTC_Open(tGATT_IF client_if, const RawAddress& remote_bda,
                    bool is_direct, tGATT_TRANSPORT transport,
                    bool opportunistic) {
  uint8_t phy = controller_get_interface()->get_le_all_initiating_phys();
  BTA_GATTC_Open(client_if, remote_bda, is_direct, transport, opportunistic,
                 phy);
}

void BTA_GATTC_Open(tGATT_IF client_if, const RawAddress& remote_bda,
                    bool is_direct, tGATT_TRANSPORT transport,
                    bool opportunistic, uint8_t initiating_phys) {
  tBTA_GATTC_API_OPEN* p_buf =
      (tBTA_GATTC_API_OPEN*)osi_malloc(sizeof(tBTA_GATTC_API_OPEN));

  p_buf->hdr.event = BTA_GATTC_API_OPEN_EVT;
  p_buf->client_if = client_if;
  p_buf->is_direct = is_direct;
  p_buf->transport = transport;
  p_buf->initiating_phys = initiating_phys;
  p_buf->opportunistic = opportunistic;
  p_buf->remote_bda = remote_bda;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_CancelOpen
 *
 * Description      Cancel a direct open connection or remove a background auto
 *                  connection
 *                  bd address
 *
 * Parameters       client_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTC_CancelOpen(tGATT_IF client_if, const RawAddress& remote_bda,
                          bool is_direct) {
  tBTA_GATTC_API_CANCEL_OPEN* p_buf = (tBTA_GATTC_API_CANCEL_OPEN*)osi_malloc(
      sizeof(tBTA_GATTC_API_CANCEL_OPEN));

  p_buf->hdr.event = BTA_GATTC_API_CANCEL_OPEN_EVT;
  p_buf->client_if = client_if;
  p_buf->is_direct = is_direct;
  p_buf->remote_bda = remote_bda;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_Close
 *
 * Description      Close a connection to a GATT server.
 *
 * Parameters       conn_id: connectino ID to be closed.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTC_Close(uint16_t conn_id) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_GATTC_API_CLOSE_EVT;
  p_buf->layer_specific = conn_id;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ConfigureMTU
 *
 * Description      Configure the MTU size in the GATT channel. This can be done
 *                  only once per connection.
 *
 * Parameters       conn_id: connection ID.
 *                  mtu: desired MTU size to use.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTC_ConfigureMTU(uint16_t conn_id, uint16_t mtu) {
  tBTA_GATTC_API_CFG_MTU* p_buf =
      (tBTA_GATTC_API_CFG_MTU*)osi_malloc(sizeof(tBTA_GATTC_API_CFG_MTU));

  p_buf->hdr.event = BTA_GATTC_API_CFG_MTU_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->mtu = mtu;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ServiceSearchRequest
 *
 * Description      This function is called to request a GATT service discovery
 *                  on a GATT server. This function report service search
 *                  result by a callback event, and followed by a service search
 *                  complete event.
 *
 * Parameters       conn_id: connection ID.
 *                  p_srvc_uuid: a UUID of the service application is interested
 *                               in.
 *                              If Null, discover for all services.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ServiceSearchRequest(uint16_t conn_id, Uuid* p_srvc_uuid) {
  const size_t len = sizeof(tBTA_GATTC_API_SEARCH) + sizeof(Uuid);
  tBTA_GATTC_API_SEARCH* p_buf = (tBTA_GATTC_API_SEARCH*)osi_calloc(len);

  p_buf->hdr.event = BTA_GATTC_API_SEARCH_EVT;
  p_buf->hdr.layer_specific = conn_id;
  if (p_srvc_uuid) {
    p_buf->p_srvc_uuid = (Uuid*)(p_buf + 1);
    *p_buf->p_srvc_uuid = *p_srvc_uuid;
  } else {
    p_buf->p_srvc_uuid = NULL;
  }

  bta_sys_sendmsg(p_buf);
}

void BTA_GATTC_DiscoverServiceByUuid(uint16_t conn_id, const Uuid& srvc_uuid) {
  do_in_main_thread(
      FROM_HERE,
      base::Bind(
          base::IgnoreResult<tGATT_STATUS (*)(uint16_t, tGATT_DISC_TYPE,
                                              uint16_t, uint16_t, const Uuid&)>(
              &GATTC_Discover),
          conn_id, GATT_DISC_SRVC_BY_UUID, 0x0001, 0xFFFF, srvc_uuid));
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetServices
 *
 * Description      This function is called to find the services on the given
 *                  server.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *
 * Returns          returns list of gatt::Service or NULL.
 *
 ******************************************************************************/
const std::list<gatt::Service>* BTA_GATTC_GetServices(uint16_t conn_id) {
  return bta_gattc_get_services(conn_id);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetCharacteristic
 *
 * Description      This function is called to find the characteristic on the
 *                  given server.
 *
 * Parameters       conn_id - connection ID which identify the server.
 *                  handle - characteristic handle
 *
 * Returns          returns pointer to gatt::Characteristic or NULL.
 *
 ******************************************************************************/
const gatt::Characteristic* BTA_GATTC_GetCharacteristic(uint16_t conn_id,
                                                        uint16_t handle) {
  return bta_gattc_get_characteristic(conn_id, handle);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetDescriptor
 *
 * Description      This function is called to find the characteristic on the
 *                  given server.
 *
 * Parameters       conn_id - connection ID which identify the server.
 *                  handle - descriptor handle
 *
 * Returns          returns pointer to gatt::Descriptor or NULL.
 *
 ******************************************************************************/
const gatt::Descriptor* BTA_GATTC_GetDescriptor(uint16_t conn_id,
                                                uint16_t handle) {
  return bta_gattc_get_descriptor(conn_id, handle);
}

/* Return characteristic that owns descriptor with handle equal to |handle|, or
 * NULL */
const gatt::Characteristic* BTA_GATTC_GetOwningCharacteristic(uint16_t conn_id,
                                                              uint16_t handle) {
  return bta_gattc_get_owning_characteristic(conn_id, handle);
}

/* Return service that owns descriptor or characteristic with handle equal to
 * |handle|, or NULL */
const gatt::Service* BTA_GATTC_GetOwningService(uint16_t conn_id,
                                                uint16_t handle) {
  return bta_gattc_get_service_for_handle(conn_id, handle);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetGattDb
 *
 * Description      This function is called to get the GATT database.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *                  db: output parameter which will contain the GATT database
 *                      copy. Caller is responsible for freeing it.
 *                  count: number of elements in database.
 *
 ******************************************************************************/
void BTA_GATTC_GetGattDb(uint16_t conn_id, uint16_t start_handle,
                         uint16_t end_handle, btgatt_db_element_t** db,
                         int* count) {
  bta_gattc_get_gatt_db(conn_id, start_handle, end_handle, db, count);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadCharacteristic
 *
 * Description      This function is called to read a characteristics value
 *
 * Parameters       conn_id - connection ID.
 *                  handle - characteritic handle to read.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ReadCharacteristic(uint16_t conn_id, uint16_t handle,
                                  tGATT_AUTH_REQ auth_req,
                                  GATT_READ_OP_CB callback, void* cb_data) {
  tBTA_GATTC_API_READ* p_buf =
      (tBTA_GATTC_API_READ*)osi_calloc(sizeof(tBTA_GATTC_API_READ));

  p_buf->hdr.event = BTA_GATTC_API_READ_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = handle;
  p_buf->read_cb = callback;
  p_buf->read_cb_data = cb_data;

  bta_sys_sendmsg(p_buf);
}

/**
 * This function is called to read a value of characteristic with uuid equal to
 * |uuid|
 */
void BTA_GATTC_ReadUsingCharUuid(uint16_t conn_id, const Uuid& uuid,
                                 uint16_t s_handle, uint16_t e_handle,
                                 tGATT_AUTH_REQ auth_req,
                                 GATT_READ_OP_CB callback, void* cb_data) {
  tBTA_GATTC_API_READ* p_buf =
      (tBTA_GATTC_API_READ*)osi_calloc(sizeof(tBTA_GATTC_API_READ));

  p_buf->hdr.event = BTA_GATTC_API_READ_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = 0;
  p_buf->uuid = uuid;
  p_buf->s_handle = s_handle;
  p_buf->e_handle = e_handle;
  p_buf->read_cb = callback;
  p_buf->read_cb_data = cb_data;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadCharDescr
 *
 * Description      This function is called to read a descriptor value.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - descriptor handle to read.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ReadCharDescr(uint16_t conn_id, uint16_t handle,
                             tGATT_AUTH_REQ auth_req, GATT_READ_OP_CB callback,
                             void* cb_data) {
  tBTA_GATTC_API_READ* p_buf =
      (tBTA_GATTC_API_READ*)osi_calloc(sizeof(tBTA_GATTC_API_READ));

  p_buf->hdr.event = BTA_GATTC_API_READ_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = handle;
  p_buf->read_cb = callback;
  p_buf->read_cb_data = cb_data;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadMultiple
 *
 * Description      This function is called to read multiple characteristic or
 *                  characteristic descriptors.
 *
 * Parameters       conn_id - connectino ID.
 *                    p_read_multi - pointer to the read multiple parameter.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ReadMultiple(uint16_t conn_id, tBTA_GATTC_MULTI* p_read_multi,
                            tGATT_AUTH_REQ auth_req) {
  tBTA_GATTC_API_READ_MULTI* p_buf =
      (tBTA_GATTC_API_READ_MULTI*)osi_calloc(sizeof(tBTA_GATTC_API_READ_MULTI));

  p_buf->hdr.event = BTA_GATTC_API_READ_MULTI_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->num_attr = p_read_multi->num_attr;

  if (p_buf->num_attr > 0)
    memcpy(p_buf->handles, p_read_multi->handles,
           sizeof(uint16_t) * p_read_multi->num_attr);

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_WriteCharValue
 *
 * Description      This function is called to write characteristic value.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - characteristic handle to write.
 *                  write_type - type of write.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_WriteCharValue(uint16_t conn_id, uint16_t handle,
                              tGATT_WRITE_TYPE write_type,
                              std::vector<uint8_t> value,
                              tGATT_AUTH_REQ auth_req,
                              GATT_WRITE_OP_CB callback, void* cb_data) {
  tBTA_GATTC_API_WRITE* p_buf = (tBTA_GATTC_API_WRITE*)osi_calloc(
      sizeof(tBTA_GATTC_API_WRITE) + value.size());

  p_buf->hdr.event = BTA_GATTC_API_WRITE_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = handle;
  p_buf->write_type = write_type;
  p_buf->len = value.size();
  p_buf->write_cb = callback;
  p_buf->write_cb_data = cb_data;

  if (value.size() > 0) {
    p_buf->p_value = (uint8_t*)(p_buf + 1);
    memcpy(p_buf->p_value, value.data(), value.size());
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_WriteCharDescr
 *
 * Description      This function is called to write descriptor value.
 *
 * Parameters       conn_id - connection ID
 *                  handle - descriptor hadle to write.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_WriteCharDescr(uint16_t conn_id, uint16_t handle,
                              std::vector<uint8_t> value,
                              tGATT_AUTH_REQ auth_req,
                              GATT_WRITE_OP_CB callback, void* cb_data) {
  tBTA_GATTC_API_WRITE* p_buf = (tBTA_GATTC_API_WRITE*)osi_calloc(
      sizeof(tBTA_GATTC_API_WRITE) + value.size());

  p_buf->hdr.event = BTA_GATTC_API_WRITE_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = handle;
  p_buf->write_type = GATT_WRITE;
  p_buf->write_cb = callback;
  p_buf->write_cb_data = cb_data;

  if (value.size() != 0) {
    p_buf->p_value = (uint8_t*)(p_buf + 1);
    p_buf->len = value.size();
    memcpy(p_buf->p_value, value.data(), value.size());
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_PrepareWrite
 *
 * Description      This function is called to prepare write a characteristic
 *                  value.
 *
 * Parameters       conn_id - connection ID.
 *                  p_char_id - GATT characteritic ID of the service.
 *                  offset - offset of the write value.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_PrepareWrite(uint16_t conn_id, uint16_t handle, uint16_t offset,
                            std::vector<uint8_t> value, tGATT_AUTH_REQ auth_req,
                            GATT_WRITE_OP_CB callback, void* cb_data) {
  tBTA_GATTC_API_WRITE* p_buf = (tBTA_GATTC_API_WRITE*)osi_calloc(
      sizeof(tBTA_GATTC_API_WRITE) + value.size());

  p_buf->hdr.event = BTA_GATTC_API_WRITE_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->auth_req = auth_req;
  p_buf->handle = handle;
  p_buf->write_cb = callback;
  p_buf->write_cb_data = cb_data;

  p_buf->write_type = BTA_GATTC_WRITE_PREPARE;
  p_buf->offset = offset;
  p_buf->len = value.size();

  if (value.size() > 0) {
    p_buf->p_value = (uint8_t*)(p_buf + 1);
    memcpy(p_buf->p_value, value.data(), value.size());
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_ExecuteWrite
 *
 * Description      This function is called to execute write a prepare write
 *                  sequence.
 *
 * Parameters       conn_id - connection ID.
 *                    is_execute - execute or cancel.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ExecuteWrite(uint16_t conn_id, bool is_execute) {
  tBTA_GATTC_API_EXEC* p_buf =
      (tBTA_GATTC_API_EXEC*)osi_calloc(sizeof(tBTA_GATTC_API_EXEC));

  p_buf->hdr.event = BTA_GATTC_API_EXEC_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->is_execute = is_execute;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_SendIndConfirm
 *
 * Description      This function is called to send handle value confirmation.
 *
 * Parameters       conn_id - connection ID.
 *                    p_char_id - characteristic ID to confirm.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_SendIndConfirm(uint16_t conn_id, uint16_t handle) {
  tBTA_GATTC_API_CONFIRM* p_buf =
      (tBTA_GATTC_API_CONFIRM*)osi_calloc(sizeof(tBTA_GATTC_API_CONFIRM));

  VLOG(1) << __func__ << ": conn_id=" << +conn_id << " handle=0x" << std::hex
          << +handle;

  p_buf->hdr.event = BTA_GATTC_API_CONFIRM_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->handle = handle;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_RegisterForNotifications
 *
 * Description      This function is called to register for notification of a
 *                  service.
 *
 * Parameters       client_if - client interface.
 *                  bda - target GATT server.
 *                  handle - GATT characteristic handle.
 *
 * Returns          OK if registration succeed, otherwise failed.
 *
 ******************************************************************************/
tGATT_STATUS BTA_GATTC_RegisterForNotifications(tGATT_IF client_if,
                                                const RawAddress& bda,
                                                uint16_t handle) {
  tBTA_GATTC_RCB* p_clreg;
  tGATT_STATUS status = GATT_ILLEGAL_PARAMETER;
  uint8_t i;

  if (!handle) {
    LOG(ERROR) << __func__ << ": registration failed, handle is 0";
    return status;
  }

  p_clreg = bta_gattc_cl_get_regcb(client_if);
  if (p_clreg != NULL) {
    for (i = 0; i < BTA_GATTC_NOTIF_REG_MAX; i++) {
      if (p_clreg->notif_reg[i].in_use &&
          p_clreg->notif_reg[i].remote_bda == bda &&
          p_clreg->notif_reg[i].handle == handle) {
        LOG(WARNING) << "notification already registered";
        status = GATT_SUCCESS;
        break;
      }
    }
    if (status != GATT_SUCCESS) {
      for (i = 0; i < BTA_GATTC_NOTIF_REG_MAX; i++) {
        if (!p_clreg->notif_reg[i].in_use) {
          memset((void*)&p_clreg->notif_reg[i], 0,
                 sizeof(tBTA_GATTC_NOTIF_REG));

          p_clreg->notif_reg[i].in_use = true;
          p_clreg->notif_reg[i].remote_bda = bda;

          p_clreg->notif_reg[i].handle = handle;
          status = GATT_SUCCESS;
          break;
        }
      }
      if (i == BTA_GATTC_NOTIF_REG_MAX) {
        status = GATT_NO_RESOURCES;
        LOG(ERROR) << "Max Notification Reached, registration failed.";
      }
    }
  } else {
    LOG(ERROR) << "client_if=" << +client_if << " Not Registered";
  }

  return status;
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_DeregisterForNotifications
 *
 * Description      This function is called to de-register for notification of a
 *                  service.
 *
 * Parameters       client_if - client interface.
 *                  remote_bda - target GATT server.
 *                  handle - GATT characteristic handle.
 *
 * Returns          OK if deregistration succeed, otherwise failed.
 *
 ******************************************************************************/
tGATT_STATUS BTA_GATTC_DeregisterForNotifications(tGATT_IF client_if,
                                                  const RawAddress& bda,
                                                  uint16_t handle) {
  if (!handle) {
    LOG(ERROR) << __func__ << ": deregistration failed, handle is 0";
    return GATT_ILLEGAL_PARAMETER;
  }

  tBTA_GATTC_RCB* p_clreg = bta_gattc_cl_get_regcb(client_if);
  if (p_clreg == NULL) {
    LOG(ERROR) << __func__ << " client_if=" << +client_if
               << " not registered bd_addr=" << bda;
    return GATT_ILLEGAL_PARAMETER;
  }

  for (int i = 0; i < BTA_GATTC_NOTIF_REG_MAX; i++) {
    if (p_clreg->notif_reg[i].in_use &&
        p_clreg->notif_reg[i].remote_bda == bda &&
        p_clreg->notif_reg[i].handle == handle) {
      VLOG(1) << __func__ << " deregistered bd_addr=" << bda;
      memset(&p_clreg->notif_reg[i], 0, sizeof(tBTA_GATTC_NOTIF_REG));
      return GATT_SUCCESS;
    }
  }

  LOG(ERROR) << __func__ << " registration not found bd_addr=" << bda;
  return GATT_ERROR;
}

/*******************************************************************************
 *
 * Function         BTA_GATTC_Refresh
 *
 * Description      Refresh the server cache of the remote device
 *
 * Parameters       remote_bda: remote device BD address.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTC_Refresh(const RawAddress& remote_bda) {
  do_in_main_thread(FROM_HERE,
                    base::Bind(&bta_gattc_process_api_refresh, remote_bda));
}
