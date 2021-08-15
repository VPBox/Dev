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
 *  This is the implementation of the API for GATT server of BTA.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <base/bind.h>
#include <string.h>

#include "bt_common.h"
#include "bta_gatt_api.h"
#include "bta_gatts_int.h"
#include "bta_sys.h"
#include "stack/include/btu.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/

static const tBTA_SYS_REG bta_gatts_reg = {bta_gatts_hdl_event,
                                           BTA_GATTS_Disable};

/*******************************************************************************
 *
 * Function         BTA_GATTS_Disable
 *
 * Description      This function is called to disable GATTS module
 *
 * Parameters       None.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTS_Disable(void) {
  if (!bta_sys_is_register(BTA_ID_GATTS)) {
    LOG(WARNING) << "GATTS Module not enabled/already disabled";
    return;
  }

  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));
  p_buf->event = BTA_GATTS_API_DISABLE_EVT;
  bta_sys_sendmsg(p_buf);
  bta_sys_deregister(BTA_ID_GATTS);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_AppRegister
 *
 * Description      This function is called to register application callbacks
 *                    with BTA GATTS module.
 *
 * Parameters       p_app_uuid - applicaiton UUID
 *                  p_cback - pointer to the application callback function.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTS_AppRegister(const bluetooth::Uuid& app_uuid,
                           tBTA_GATTS_CBACK* p_cback) {
  tBTA_GATTS_API_REG* p_buf =
      (tBTA_GATTS_API_REG*)osi_malloc(sizeof(tBTA_GATTS_API_REG));

  /* register with BTA system manager */
  if (!bta_sys_is_register(BTA_ID_GATTS))
    bta_sys_register(BTA_ID_GATTS, &bta_gatts_reg);

  p_buf->hdr.event = BTA_GATTS_API_REG_EVT;
  p_buf->app_uuid = app_uuid;
  p_buf->p_cback = p_cback;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_AppDeregister
 *
 * Description      De-register with GATT Server.
 *
 * Parameters       app_id: applicatino ID.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTS_AppDeregister(tGATT_IF server_if) {
  tBTA_GATTS_API_DEREG* p_buf =
      (tBTA_GATTS_API_DEREG*)osi_malloc(sizeof(tBTA_GATTS_API_DEREG));

  p_buf->hdr.event = BTA_GATTS_API_DEREG_EVT;
  p_buf->server_if = server_if;

  bta_sys_sendmsg(p_buf);
}

void bta_gatts_add_service_impl(tGATT_IF server_if,
                                std::vector<btgatt_db_element_t> service,
                                BTA_GATTS_AddServiceCb cb) {
  uint8_t rcb_idx =
      bta_gatts_find_app_rcb_idx_by_app_if(&bta_gatts_cb, server_if);

  LOG(INFO) << __func__ << ": rcb_idx=" << +rcb_idx;

  if (rcb_idx == BTA_GATTS_INVALID_APP) {
    cb.Run(GATT_ERROR, server_if, std::move(service));
    return;
  }

  uint8_t srvc_idx = bta_gatts_alloc_srvc_cb(&bta_gatts_cb, rcb_idx);
  if (srvc_idx == BTA_GATTS_INVALID_APP) {
    cb.Run(GATT_ERROR, server_if, std::move(service));
    return;
  }

  uint16_t status = GATTS_AddService(server_if, service.data(), service.size());
  if (status != GATT_SERVICE_STARTED) {
    memset(&bta_gatts_cb.srvc_cb[srvc_idx], 0, sizeof(tBTA_GATTS_SRVC_CB));
    LOG(ERROR) << __func__ << ": service creation failed.";
    cb.Run(GATT_ERROR, server_if, std::move(service));
    return;
  }

  bta_gatts_cb.srvc_cb[srvc_idx].service_uuid = service[0].uuid;

  // service_id is equal to service start handle
  bta_gatts_cb.srvc_cb[srvc_idx].service_id = service[0].attribute_handle;
  bta_gatts_cb.srvc_cb[srvc_idx].idx = srvc_idx;

  cb.Run(GATT_SUCCESS, server_if, std::move(service));
  return;
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_AddService
 *
 * Description      Add the given |service| and all included elements to the
 *                  GATT database. a |BTA_GATTS_ADD_SRVC_EVT| is triggered to
 *                  report the status and attribute handles.
 *
 * Parameters       server_if: server interface.
 *                  service: pointer vector describing service.
 *
 * Returns          Returns |GATT_SUCCESS| on success or |GATT_ERROR| if the
 *                  service cannot be added.
 *
 ******************************************************************************/
extern void BTA_GATTS_AddService(tGATT_IF server_if,
                                 std::vector<btgatt_db_element_t> service,
                                 BTA_GATTS_AddServiceCb cb) {
  do_in_main_thread(FROM_HERE,
                    base::Bind(&bta_gatts_add_service_impl, server_if,
                               std::move(service), std::move(cb)));
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_DeleteService
 *
 * Description      This function is called to delete a service. When this is
 *                  done, a callback event BTA_GATTS_DELETE_EVT is report with
 *                  the status.
 *
 * Parameters       service_id: service_id to be deleted.
 *
 * Returns          returns none.
 *
 ******************************************************************************/
void BTA_GATTS_DeleteService(uint16_t service_id) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_GATTS_API_DEL_SRVC_EVT;
  p_buf->layer_specific = service_id;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_StopService
 *
 * Description      This function is called to stop a service.
 *
 * Parameters       service_id - service to be topped.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTS_StopService(uint16_t service_id) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_GATTS_API_STOP_SRVC_EVT;
  p_buf->layer_specific = service_id;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_HandleValueIndication
 *
 * Description      This function is called to read a characteristics
 *                  descriptor.
 *
 * Parameters       bda - remote device bd address to indicate.
 *                  attr_id - attribute ID to indicate.
 *                  value - data to indicate.
 *                  need_confirm - if this indication expects a confirmation or
 *                                 not.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTS_HandleValueIndication(uint16_t conn_id, uint16_t attr_id,
                                     std::vector<uint8_t> value,
                                     bool need_confirm) {
  tBTA_GATTS_API_INDICATION* p_buf =
      (tBTA_GATTS_API_INDICATION*)osi_calloc(sizeof(tBTA_GATTS_API_INDICATION));

  p_buf->hdr.event = BTA_GATTS_API_INDICATION_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->attr_id = attr_id;
  p_buf->need_confirm = need_confirm;
  if (value.size() > 0) {
    p_buf->len = value.size();
    memcpy(p_buf->value, value.data(), value.size());
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_SendRsp
 *
 * Description      This function is called to send a response to a request.
 *
 * Parameters       conn_id - connection identifier.
 *                  trans_id - transaction ID.
 *                  status - response status
 *                  p_msg - response data.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTS_SendRsp(uint16_t conn_id, uint32_t trans_id, tGATT_STATUS status,
                       tGATTS_RSP* p_msg) {
  const size_t len = sizeof(tBTA_GATTS_API_RSP) + sizeof(tGATTS_RSP);
  tBTA_GATTS_API_RSP* p_buf = (tBTA_GATTS_API_RSP*)osi_calloc(len);

  p_buf->hdr.event = BTA_GATTS_API_RSP_EVT;
  p_buf->hdr.layer_specific = conn_id;
  p_buf->trans_id = trans_id;
  p_buf->status = status;
  if (p_msg != NULL) {
    p_buf->p_rsp = (tGATTS_RSP*)(p_buf + 1);
    memcpy(p_buf->p_rsp, p_msg, sizeof(tGATTS_RSP));
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_Open
 *
 * Description      Open a direct open connection or add a background auto
 *                  connection bd address
 *
 * Parameters       server_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *                  transport : Transport on which GATT connection to be opened
 *                              (BR/EDR or LE)
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTS_Open(tGATT_IF server_if, const RawAddress& remote_bda,
                    bool is_direct, tGATT_TRANSPORT transport) {
  tBTA_GATTS_API_OPEN* p_buf =
      (tBTA_GATTS_API_OPEN*)osi_malloc(sizeof(tBTA_GATTS_API_OPEN));

  p_buf->hdr.event = BTA_GATTS_API_OPEN_EVT;
  p_buf->server_if = server_if;
  p_buf->is_direct = is_direct;
  p_buf->transport = transport;
  p_buf->remote_bda = remote_bda;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_CancelOpen
 *
 * Description      Cancel a direct open connection or remove a background auto
 *                  connection bd address
 *
 * Parameters       server_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTS_CancelOpen(tGATT_IF server_if, const RawAddress& remote_bda,
                          bool is_direct) {
  tBTA_GATTS_API_CANCEL_OPEN* p_buf = (tBTA_GATTS_API_CANCEL_OPEN*)osi_malloc(
      sizeof(tBTA_GATTS_API_CANCEL_OPEN));

  p_buf->hdr.event = BTA_GATTS_API_CANCEL_OPEN_EVT;
  p_buf->server_if = server_if;
  p_buf->is_direct = is_direct;
  p_buf->remote_bda = remote_bda;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_GATTS_Close
 *
 * Description      Close a connection  a remote device.
 *
 * Parameters       conn_id: connectino ID to be closed.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_GATTS_Close(uint16_t conn_id) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_GATTS_API_CLOSE_EVT;
  p_buf->layer_specific = conn_id;

  bta_sys_sendmsg(p_buf);
}
