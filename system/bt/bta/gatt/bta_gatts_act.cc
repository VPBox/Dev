/******************************************************************************
 *
 *  Copyright 2003-2012 Broadcom Corporation
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
 *  This file contains the GATT Server action functions for the state
 *  machine.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <base/logging.h>
#include <string.h>
#include "bt_common.h"
#include "bta_gatts_co.h"
#include "bta_gatts_int.h"
#include "bta_sys.h"
#include "btif/include/btif_debug_conn.h"
#include "btm_ble_api.h"
#include "osi/include/osi.h"
#include "utl.h"

using base::StringPrintf;

static void bta_gatts_nv_save_cback(bool is_saved,
                                    tGATTS_HNDL_RANGE* p_hndl_range);
static bool bta_gatts_nv_srv_chg_cback(tGATTS_SRV_CHG_CMD cmd,
                                       tGATTS_SRV_CHG_REQ* p_req,
                                       tGATTS_SRV_CHG_RSP* p_rsp);

static void bta_gatts_conn_cback(tGATT_IF gatt_if, const RawAddress& bda,
                                 uint16_t conn_id, bool connected,
                                 tGATT_DISCONN_REASON reason,
                                 tGATT_TRANSPORT transport);
static void bta_gatts_send_request_cback(uint16_t conn_id, uint32_t trans_id,
                                         tGATTS_REQ_TYPE req_type,
                                         tGATTS_DATA* p_data);
static void bta_gatts_cong_cback(uint16_t conn_id, bool congested);
static void bta_gatts_phy_update_cback(tGATT_IF gatt_if, uint16_t conn_id,
                                       uint8_t tx_phy, uint8_t rx_phy,
                                       uint8_t status);
static void bta_gatts_conn_update_cback(tGATT_IF gatt_if, uint16_t conn_id,
                                        uint16_t interval, uint16_t latency,
                                        uint16_t timeout, uint8_t status);

static tGATT_CBACK bta_gatts_cback = {bta_gatts_conn_cback,
                                      NULL,
                                      NULL,
                                      NULL,
                                      bta_gatts_send_request_cback,
                                      NULL,
                                      bta_gatts_cong_cback,
                                      bta_gatts_phy_update_cback,
                                      bta_gatts_conn_update_cback};

tGATT_APPL_INFO bta_gatts_nv_cback = {bta_gatts_nv_save_cback,
                                      bta_gatts_nv_srv_chg_cback};

/*******************************************************************************
 *
 * Function         bta_gatts_nv_save_cback
 *
 * Description      NV save callback function.
 *
 * Parameter        is_add: true is to add a handle range; otherwise is to
 *                          delete.
 * Returns          none.
 *
 ******************************************************************************/
static void bta_gatts_nv_save_cback(bool is_add,
                                    tGATTS_HNDL_RANGE* p_hndl_range) {
  bta_gatts_co_update_handle_range(is_add,
                                   (tBTA_GATTS_HNDL_RANGE*)p_hndl_range);
}

/*******************************************************************************
 *
 * Function         bta_gatts_nv_srv_chg_cback
 *
 * Description      NV save callback function.
 *
 * Parameter        is_add: true is to add a handle range; otherwise is to
 *                          delete.
 * Returns          none.
 *
 ******************************************************************************/
static bool bta_gatts_nv_srv_chg_cback(tGATTS_SRV_CHG_CMD cmd,
                                       tGATTS_SRV_CHG_REQ* p_req,
                                       tGATTS_SRV_CHG_RSP* p_rsp) {
  return bta_gatts_co_srv_chg((tGATTS_SRV_CHG_CMD)cmd,
                              (tGATTS_SRV_CHG_REQ*)p_req,
                              (tGATTS_SRV_CHG_RSP*)p_rsp);
}

/*******************************************************************************
 *
 * Function         bta_gatts_enable
 *
 * Description      enable BTA GATTS module.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_enable(tBTA_GATTS_CB* p_cb) {
  uint8_t index = 0;
  tBTA_GATTS_HNDL_RANGE handle_range;

  if (p_cb->enabled) {
    VLOG(1) << "GATTS already enabled.";
  } else {
    memset(p_cb, 0, sizeof(tBTA_GATTS_CB));

    p_cb->enabled = true;

    while (bta_gatts_co_load_handle_range(index, &handle_range)) {
      GATTS_AddHandleRange((tGATTS_HNDL_RANGE*)&handle_range);
      memset(&handle_range, 0, sizeof(tGATTS_HNDL_RANGE));
      index++;
    }

    VLOG(1) << __func__ << ": num of handle range added:" << +index;

    if (!GATTS_NVRegister(&bta_gatts_nv_cback)) {
      LOG(ERROR) << "BTA GATTS NV register failed.";
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_api_disable
 *
 * Description      disable BTA GATTS module.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_api_disable(tBTA_GATTS_CB* p_cb) {
  uint8_t i;

  if (p_cb->enabled) {
    for (i = 0; i < BTA_GATTS_MAX_APP_NUM; i++) {
      if (p_cb->rcb[i].in_use) {
        GATT_Deregister(p_cb->rcb[i].gatt_if);
      }
    }
    memset(p_cb, 0, sizeof(tBTA_GATTS_CB));
  } else {
    LOG(ERROR) << "GATTS not enabled";
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_register
 *
 * Description      register an application.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_register(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS cb_data;
  tGATT_STATUS status = GATT_SUCCESS;
  uint8_t i, first_unuse = 0xff;

  if (!p_cb->enabled) {
    bta_gatts_enable(p_cb);
  }

  for (i = 0; i < BTA_GATTS_MAX_APP_NUM; i++) {
    if (p_cb->rcb[i].in_use) {
      if (p_cb->rcb[i].app_uuid == p_msg->api_reg.app_uuid) {
        LOG(ERROR) << "application already registered.";
        status = GATT_DUP_REG;
        break;
      }
    }
  }

  if (status == GATT_SUCCESS) {
    for (i = 0; i < BTA_GATTS_MAX_APP_NUM; i++) {
      if (first_unuse == 0xff && !p_cb->rcb[i].in_use) {
        first_unuse = i;
        break;
      }
    }

    cb_data.reg_oper.server_if = BTA_GATTS_INVALID_IF;
    cb_data.reg_oper.uuid = p_msg->api_reg.app_uuid;
    if (first_unuse != 0xff) {
      LOG(INFO) << "register application first_unuse rcb_idx=" << +first_unuse;

      p_cb->rcb[first_unuse].in_use = true;
      p_cb->rcb[first_unuse].p_cback = p_msg->api_reg.p_cback;
      p_cb->rcb[first_unuse].app_uuid = p_msg->api_reg.app_uuid;
      cb_data.reg_oper.server_if = p_cb->rcb[first_unuse].gatt_if =
          GATT_Register(p_msg->api_reg.app_uuid, &bta_gatts_cback);
      if (!p_cb->rcb[first_unuse].gatt_if) {
        status = GATT_NO_RESOURCES;
      } else {
        tBTA_GATTS_INT_START_IF* p_buf = (tBTA_GATTS_INT_START_IF*)osi_malloc(
            sizeof(tBTA_GATTS_INT_START_IF));
        p_buf->hdr.event = BTA_GATTS_INT_START_IF_EVT;
        p_buf->server_if = p_cb->rcb[first_unuse].gatt_if;

        bta_sys_sendmsg(p_buf);
      }
    } else {
      status = GATT_NO_RESOURCES;
    }
  }
  cb_data.reg_oper.status = status;
  if (p_msg->api_reg.p_cback)
    (*p_msg->api_reg.p_cback)(BTA_GATTS_REG_EVT, &cb_data);
}

/*******************************************************************************
 *
 * Function         bta_gatts_start_if
 *
 * Description      start an application interface.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_start_if(UNUSED_ATTR tBTA_GATTS_CB* p_cb,
                        tBTA_GATTS_DATA* p_msg) {
  if (bta_gatts_find_app_rcb_by_app_if(p_msg->int_start_if.server_if)) {
    GATT_StartIf(p_msg->int_start_if.server_if);
  } else {
    LOG(ERROR) << "Unable to start app.: Unknown interface="
               << +p_msg->int_start_if.server_if;
  }
}
/*******************************************************************************
 *
 * Function         bta_gatts_deregister
 *
 * Description      deregister an application.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_deregister(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg) {
  tGATT_STATUS status = GATT_ERROR;
  tBTA_GATTS_CBACK* p_cback = NULL;
  uint8_t i;
  tBTA_GATTS cb_data;

  cb_data.reg_oper.server_if = p_msg->api_dereg.server_if;
  cb_data.reg_oper.status = status;

  for (i = 0; i < BTA_GATTS_MAX_APP_NUM; i++) {
    if (p_cb->rcb[i].in_use &&
        p_cb->rcb[i].gatt_if == p_msg->api_dereg.server_if) {
      p_cback = p_cb->rcb[i].p_cback;
      status = GATT_SUCCESS;

      /* deregister the app */
      GATT_Deregister(p_cb->rcb[i].gatt_if);

      /* reset cb */
      memset(&p_cb->rcb[i], 0, sizeof(tBTA_GATTS_RCB));
      cb_data.reg_oper.status = status;
      break;
    }
  }

  if (p_cback) {
    (*p_cback)(BTA_GATTS_DEREG_EVT, &cb_data);
  } else {
    LOG(ERROR) << "application not registered.";
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_delete_service
 *
 * Description      action function to delete a service.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_delete_service(tBTA_GATTS_SRVC_CB* p_srvc_cb,
                              tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_RCB* p_rcb = &bta_gatts_cb.rcb[p_srvc_cb->rcb_idx];
  tBTA_GATTS cb_data;

  cb_data.srvc_oper.server_if = p_rcb->gatt_if;
  cb_data.srvc_oper.service_id = p_srvc_cb->service_id;

  if (GATTS_DeleteService(p_rcb->gatt_if, &p_srvc_cb->service_uuid,
                          p_srvc_cb->service_id)) {
    cb_data.srvc_oper.status = GATT_SUCCESS;
    memset(p_srvc_cb, 0, sizeof(tBTA_GATTS_SRVC_CB));
  } else {
    cb_data.srvc_oper.status = GATT_ERROR;
  }

  if (p_rcb->p_cback) (*p_rcb->p_cback)(BTA_GATTS_DELELTE_EVT, &cb_data);
}

/*******************************************************************************
 *
 * Function         bta_gatts_stop_service
 *
 * Description      action function to stop a service.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_stop_service(tBTA_GATTS_SRVC_CB* p_srvc_cb,
                            UNUSED_ATTR tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_RCB* p_rcb = &bta_gatts_cb.rcb[p_srvc_cb->rcb_idx];
  tBTA_GATTS cb_data;

  GATTS_StopService(p_srvc_cb->service_id);
  cb_data.srvc_oper.server_if = p_rcb->gatt_if;
  cb_data.srvc_oper.service_id = p_srvc_cb->service_id;
  cb_data.srvc_oper.status = GATT_SUCCESS;
  LOG(ERROR) << __func__ << " service_id=" << +p_srvc_cb->service_id;

  if (p_rcb->p_cback) (*p_rcb->p_cback)(BTA_GATTS_STOP_EVT, &cb_data);
}
/*******************************************************************************
 *
 * Function         bta_gatts_send_rsp
 *
 * Description      GATTS send response.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_send_rsp(UNUSED_ATTR tBTA_GATTS_CB* p_cb,
                        tBTA_GATTS_DATA* p_msg) {
  if (GATTS_SendRsp(p_msg->api_rsp.hdr.layer_specific, p_msg->api_rsp.trans_id,
                    p_msg->api_rsp.status,
                    (tGATTS_RSP*)p_msg->api_rsp.p_rsp) != GATT_SUCCESS) {
    LOG(ERROR) << "Sending response failed";
  }
}
/*******************************************************************************
 *
 * Function         bta_gatts_indicate_handle
 *
 * Description      GATTS send handle value indication or notification.
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_indicate_handle(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_SRVC_CB* p_srvc_cb;
  tBTA_GATTS_RCB* p_rcb = NULL;
  tGATT_STATUS status = GATT_ILLEGAL_PARAMETER;
  tGATT_IF gatt_if;
  RawAddress remote_bda;
  tBTA_TRANSPORT transport;
  tBTA_GATTS cb_data;

  p_srvc_cb =
      bta_gatts_find_srvc_cb_by_attr_id(p_cb, p_msg->api_indicate.attr_id);

  if (p_srvc_cb) {
    if (GATT_GetConnectionInfor(p_msg->api_indicate.hdr.layer_specific,
                                &gatt_if, remote_bda, &transport)) {
      p_rcb = bta_gatts_find_app_rcb_by_app_if(gatt_if);

      if (p_msg->api_indicate.need_confirm)

        status = GATTS_HandleValueIndication(
            p_msg->api_indicate.hdr.layer_specific, p_msg->api_indicate.attr_id,
            p_msg->api_indicate.len, p_msg->api_indicate.value);
      else
        status = GATTS_HandleValueNotification(
            p_msg->api_indicate.hdr.layer_specific, p_msg->api_indicate.attr_id,
            p_msg->api_indicate.len, p_msg->api_indicate.value);

      /* if over BR_EDR, inform PM for mode change */
      if (transport == BTA_TRANSPORT_BR_EDR) {
        bta_sys_busy(BTA_ID_GATTS, BTA_ALL_APP_ID, remote_bda);
        bta_sys_idle(BTA_ID_GATTS, BTA_ALL_APP_ID, remote_bda);
      }
    } else {
      LOG(ERROR) << "Unknown connection_id="
                 << loghex(p_msg->api_indicate.hdr.layer_specific)
                 << " fail sending notification";
    }

    if ((status != GATT_SUCCESS || !p_msg->api_indicate.need_confirm) &&
        p_rcb && p_cb->rcb[p_srvc_cb->rcb_idx].p_cback) {
      cb_data.req_data.status = status;
      cb_data.req_data.conn_id = p_msg->api_indicate.hdr.layer_specific;

      (*p_rcb->p_cback)(BTA_GATTS_CONF_EVT, &cb_data);
    }
  } else {
    LOG(ERROR) << "Not an registered servce attribute ID: "
               << loghex(p_msg->api_indicate.attr_id);
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_open
 *
 * Description
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_open(UNUSED_ATTR tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_RCB* p_rcb = NULL;
  tGATT_STATUS status = GATT_ERROR;
  uint16_t conn_id;

  p_rcb = bta_gatts_find_app_rcb_by_app_if(p_msg->api_open.server_if);
  if (p_rcb != NULL) {
    /* should always get the connection ID */
    if (GATT_Connect(p_rcb->gatt_if, p_msg->api_open.remote_bda,
                     p_msg->api_open.is_direct, p_msg->api_open.transport,
                     false)) {
      status = GATT_SUCCESS;

      if (GATT_GetConnIdIfConnected(p_rcb->gatt_if, p_msg->api_open.remote_bda,
                                    &conn_id, p_msg->api_open.transport)) {
        status = GATT_ALREADY_OPEN;
      }
    }
  } else {
    LOG(ERROR) << "Inavlid server_if=" << p_msg->api_open.server_if;
  }

  if (p_rcb && p_rcb->p_cback) {
    tBTA_GATTS bta_gatts;
    bta_gatts.status = status;
    (*p_rcb->p_cback)(BTA_GATTS_OPEN_EVT, &bta_gatts);
  }
}
/*******************************************************************************
 *
 * Function         bta_gatts_cancel_open
 *
 * Description
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_cancel_open(UNUSED_ATTR tBTA_GATTS_CB* p_cb,
                           tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_RCB* p_rcb;
  tGATT_STATUS status = GATT_ERROR;

  p_rcb = bta_gatts_find_app_rcb_by_app_if(p_msg->api_cancel_open.server_if);
  if (p_rcb != NULL) {
    if (!GATT_CancelConnect(p_rcb->gatt_if, p_msg->api_cancel_open.remote_bda,
                            p_msg->api_cancel_open.is_direct)) {
      LOG(ERROR) << __func__ << ": failed for open request";
    } else {
      status = GATT_SUCCESS;
    }
  } else {
    LOG(ERROR) << "Inavlid server_if=" << +p_msg->api_cancel_open.server_if;
  }

  if (p_rcb && p_rcb->p_cback) {
    tBTA_GATTS bta_gatts;
    bta_gatts.status = status;
    (*p_rcb->p_cback)(BTA_GATTS_CANCEL_OPEN_EVT, &bta_gatts);
  }
}
/*******************************************************************************
 *
 * Function         bta_gatts_close
 *
 * Description
 *
 * Returns          none.
 *
 ******************************************************************************/
void bta_gatts_close(UNUSED_ATTR tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg) {
  tBTA_GATTS_RCB* p_rcb;
  tGATT_STATUS status = GATT_ERROR;
  tGATT_IF gatt_if;
  RawAddress remote_bda;
  tGATT_TRANSPORT transport;

  if (GATT_GetConnectionInfor(p_msg->hdr.layer_specific, &gatt_if, remote_bda,
                              &transport)) {
    if (GATT_Disconnect(p_msg->hdr.layer_specific) != GATT_SUCCESS) {
      LOG(ERROR) << __func__
                 << ": fail conn_id=" << loghex(p_msg->hdr.layer_specific);
    } else {
      status = GATT_SUCCESS;
    }

    p_rcb = bta_gatts_find_app_rcb_by_app_if(gatt_if);

    if (p_rcb && p_rcb->p_cback) {
      if (transport == BTA_TRANSPORT_BR_EDR)
        bta_sys_conn_close(BTA_ID_GATTS, BTA_ALL_APP_ID, remote_bda);

      tBTA_GATTS bta_gatts;
      bta_gatts.status = status;
      (*p_rcb->p_cback)(BTA_GATTS_CLOSE_EVT, &bta_gatts);
    }
  } else {
    LOG(ERROR) << "Unknown connection_id=" << loghex(p_msg->hdr.layer_specific);
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_request_cback
 *
 * Description      GATTS attribute request callback.
 *
 * Returns          none.
 *
 ******************************************************************************/
static void bta_gatts_send_request_cback(uint16_t conn_id, uint32_t trans_id,
                                         tGATTS_REQ_TYPE req_type,
                                         tGATTS_DATA* p_data) {
  tBTA_GATTS cb_data;
  tBTA_GATTS_RCB* p_rcb;
  tGATT_IF gatt_if;
  tGATT_TRANSPORT transport;

  memset(&cb_data, 0, sizeof(tBTA_GATTS));

  if (GATT_GetConnectionInfor(conn_id, &gatt_if, cb_data.req_data.remote_bda,
                              &transport)) {
    p_rcb = bta_gatts_find_app_rcb_by_app_if(gatt_if);

    VLOG(1) << __func__ << ": conn_id=" << loghex(conn_id)
            << ", trans_id=" << +trans_id << ", req_type=" << +req_type;

    if (p_rcb && p_rcb->p_cback) {
      /* if over BR_EDR, inform PM for mode change */
      if (transport == BTA_TRANSPORT_BR_EDR) {
        bta_sys_busy(BTA_ID_GATTS, BTA_ALL_APP_ID, cb_data.req_data.remote_bda);
        bta_sys_idle(BTA_ID_GATTS, BTA_ALL_APP_ID, cb_data.req_data.remote_bda);
      }

      cb_data.req_data.conn_id = conn_id;
      cb_data.req_data.trans_id = trans_id;
      cb_data.req_data.p_data = (tGATTS_DATA*)p_data;

      (*p_rcb->p_cback)(req_type, &cb_data);
    } else {
      LOG(ERROR) << "connection request on gatt_if=" << +gatt_if
                 << " is not interested";
    }
  } else {
    LOG(ERROR) << "request received on unknown conn_id=" << loghex(conn_id);
  }
}

/*******************************************************************************
 *
 * Function         bta_gatts_conn_cback
 *
 * Description      connection callback.
 *
 * Returns          none.
 *
 ******************************************************************************/
static void bta_gatts_conn_cback(tGATT_IF gatt_if, const RawAddress& bdaddr,
                                 uint16_t conn_id, bool connected,
                                 tGATT_DISCONN_REASON reason,
                                 tGATT_TRANSPORT transport) {
  tBTA_GATTS cb_data;
  uint8_t evt = connected ? BTA_GATTS_CONNECT_EVT : BTA_GATTS_DISCONNECT_EVT;
  tBTA_GATTS_RCB* p_reg;

  VLOG(1) << __func__ << "  bda=" << bdaddr << " gatt_if= " << gatt_if
          << ", conn_id=" << loghex(conn_id) << " connected=" << connected
          << ", reason=" << loghex(reason);

  if (connected)
    btif_debug_conn_state(bdaddr, BTIF_DEBUG_CONNECTED, GATT_CONN_UNKNOWN);
  else
    btif_debug_conn_state(bdaddr, BTIF_DEBUG_DISCONNECTED, reason);

  p_reg = bta_gatts_find_app_rcb_by_app_if(gatt_if);

  if (p_reg && p_reg->p_cback) {
    /* there is no RM for GATT */
    if (transport == BTA_TRANSPORT_BR_EDR) {
      if (connected)
        bta_sys_conn_open(BTA_ID_GATTS, BTA_ALL_APP_ID, bdaddr);
      else
        bta_sys_conn_close(BTA_ID_GATTS, BTA_ALL_APP_ID, bdaddr);
    }

    cb_data.conn.conn_id = conn_id;
    cb_data.conn.server_if = gatt_if;
    cb_data.conn.reason = reason;
    cb_data.conn.transport = transport;
    cb_data.conn.remote_bda = bdaddr;
    (*p_reg->p_cback)(evt, &cb_data);
  } else {
    LOG(ERROR) << __func__ << " server_if=" << +gatt_if << " not found";
  }
}

static void bta_gatts_phy_update_cback(tGATT_IF gatt_if, uint16_t conn_id,
                                       uint8_t tx_phy, uint8_t rx_phy,
                                       uint8_t status) {
  tBTA_GATTS_RCB* p_reg = bta_gatts_find_app_rcb_by_app_if(gatt_if);
  if (!p_reg || !p_reg->p_cback) {
    LOG(ERROR) << __func__ << ": server_if=" << +gatt_if << " not found";
    return;
  }

  tBTA_GATTS cb_data;
  cb_data.phy_update.conn_id = conn_id;
  cb_data.phy_update.server_if = gatt_if;
  cb_data.phy_update.tx_phy = tx_phy;
  cb_data.phy_update.rx_phy = rx_phy;
  cb_data.phy_update.status = status;
  (*p_reg->p_cback)(BTA_GATTS_PHY_UPDATE_EVT, &cb_data);
}

static void bta_gatts_conn_update_cback(tGATT_IF gatt_if, uint16_t conn_id,
                                        uint16_t interval, uint16_t latency,
                                        uint16_t timeout, uint8_t status) {
  tBTA_GATTS_RCB* p_reg = bta_gatts_find_app_rcb_by_app_if(gatt_if);
  if (!p_reg || !p_reg->p_cback) {
    LOG(ERROR) << __func__ << ": server_if=" << +gatt_if << " not found";
    return;
  }

  tBTA_GATTS cb_data;
  cb_data.conn_update.conn_id = conn_id;
  cb_data.conn_update.server_if = gatt_if;
  cb_data.conn_update.interval = interval;
  cb_data.conn_update.latency = latency;
  cb_data.conn_update.timeout = timeout;
  cb_data.conn_update.status = status;
  (*p_reg->p_cback)(BTA_GATTS_CONN_UPDATE_EVT, &cb_data);
}

/*******************************************************************************
 *
 * Function         bta_gatts_cong_cback
 *
 * Description      congestion callback.
 *
 * Returns          none.
 *
 ******************************************************************************/
static void bta_gatts_cong_cback(uint16_t conn_id, bool congested) {
  tBTA_GATTS_RCB* p_rcb;
  tGATT_IF gatt_if;
  tGATT_TRANSPORT transport;
  tBTA_GATTS cb_data;

  if (GATT_GetConnectionInfor(conn_id, &gatt_if, cb_data.req_data.remote_bda,
                              &transport)) {
    p_rcb = bta_gatts_find_app_rcb_by_app_if(gatt_if);

    if (p_rcb && p_rcb->p_cback) {
      cb_data.congest.conn_id = conn_id;
      cb_data.congest.congested = congested;

      (*p_rcb->p_cback)(BTA_GATTS_CONGEST_EVT, &cb_data);
    }
  }
}
