/******************************************************************************
 *
 *  Copyright 2004-2012 Broadcom Corporation
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
 *  This file contains the audio gateway functions controlling the RFCOMM
 *  connections.
 *
 ******************************************************************************/

#include <cstring>

#include <base/bind.h>

#include "bt_common.h"
#include "bta_ag_api.h"
#include "bta_ag_int.h"
#include "btm_api.h"
#include "osi/include/osi.h"
#include "port_api.h"
#include "rfcdefs.h"
#include "stack/include/btu.h"
#include "utl.h"

/* Event mask for RfCOMM port callback */
#define BTA_AG_PORT_EV_MASK PORT_EV_RXCHAR

/* each scb has its own rfcomm callbacks */
void bta_ag_port_cback_1(uint32_t code, uint16_t port_handle);
void bta_ag_port_cback_2(uint32_t code, uint16_t port_handle);
void bta_ag_port_cback_3(uint32_t code, uint16_t port_handle);
void bta_ag_port_cback_4(uint32_t code, uint16_t port_handle);
void bta_ag_port_cback_5(uint32_t code, uint16_t port_handle);
void bta_ag_port_cback_6(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_1(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_2(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_3(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_4(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_5(uint32_t code, uint16_t port_handle);
void bta_ag_mgmt_cback_6(uint32_t code, uint16_t port_handle);

/* rfcomm callback function tables */
typedef tPORT_CALLBACK* tBTA_AG_PORT_CBACK;
const tBTA_AG_PORT_CBACK bta_ag_port_cback_tbl[] = {
    bta_ag_port_cback_1, bta_ag_port_cback_2, bta_ag_port_cback_3,
    bta_ag_port_cback_4, bta_ag_port_cback_5, bta_ag_port_cback_6};

const tBTA_AG_PORT_CBACK bta_ag_mgmt_cback_tbl[] = {
    bta_ag_mgmt_cback_1, bta_ag_mgmt_cback_2, bta_ag_mgmt_cback_3,
    bta_ag_mgmt_cback_4, bta_ag_mgmt_cback_5, bta_ag_mgmt_cback_6};

/*******************************************************************************
 *
 * Function         bta_ag_port_cback
 *
 * Description      RFCOMM Port callback
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_port_cback(UNUSED_ATTR uint32_t code, uint16_t port_handle,
                              uint16_t handle) {
  tBTA_AG_SCB* p_scb = bta_ag_scb_by_idx(handle);
  if (p_scb != nullptr) {
    /* ignore port events for port handles other than connected handle */
    if (port_handle != p_scb->conn_handle) {
      APPL_TRACE_ERROR(
          "ag_port_cback ignoring handle:%d conn_handle = %d other handle = %d",
          port_handle, p_scb->conn_handle, handle);
      return;
    }
    if (!bta_ag_scb_open(p_scb)) {
      LOG(ERROR) << __func__ << ": rfcomm data on an unopened control block "
                 << handle << " peer_addr " << p_scb->peer_addr << " state "
                 << std::to_string(p_scb->state);
    }
    do_in_main_thread(FROM_HERE,
                      base::Bind(&bta_ag_sm_execute_by_handle, handle,
                                 BTA_AG_RFC_DATA_EVT, tBTA_AG_DATA::kEmpty));
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_mgmt_cback
 *
 * Description      RFCOMM management callback
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_mgmt_cback(uint32_t code, uint16_t port_handle,
                              uint16_t handle) {
  tBTA_AG_SCB* p_scb = bta_ag_scb_by_idx(handle);
  APPL_TRACE_DEBUG("%s: code=%d, port_handle=%d, scb_handle=%d, p_scb=0x%08x",
                   __func__, code, port_handle, handle, p_scb);
  if (p_scb == nullptr) {
    LOG(WARNING) << __func__ << ": cannot find scb, code=" << code
                 << ", port_handle=" << port_handle << ", handle=" << handle;
    return;
  }
  /* ignore close event for port handles other than connected handle */
  if ((code != PORT_SUCCESS) && (port_handle != p_scb->conn_handle)) {
    LOG(WARNING) << __func__ << ": ignore open failure for unmatched "
                 << "port_handle " << port_handle << ", scb_handle=" << handle;
    return;
  }
  uint16_t event;
  if (code == PORT_SUCCESS) {
    bool found_handle = false;
    if (p_scb->conn_handle) {
      /* Outgoing connection */
      if (port_handle == p_scb->conn_handle) {
        found_handle = true;
      }
    } else {
      /* Incoming connection */
      for (uint16_t service_port_handle : p_scb->serv_handle) {
        if (port_handle == service_port_handle) {
          found_handle = true;
          break;
        }
      }
    }
    if (!found_handle) {
      LOG(ERROR) << __func__ << ": port opened successfully, but port_handle "
                 << port_handle << " is unknown"
                 << ", scb_handle=" << handle;
      return;
    }
    event = BTA_AG_RFC_OPEN_EVT;
  } else if (port_handle == p_scb->conn_handle) {
    /* distinguish server close events */
    event = BTA_AG_RFC_CLOSE_EVT;
  } else {
    event = BTA_AG_RFC_SRV_CLOSE_EVT;
  }

  tBTA_AG_DATA data = {};
  data.rfc.port_handle = port_handle;
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle, event, data));
}

/*******************************************************************************
 *
 * Function         bta_ag_port_cback_1 to 6
 *                  bta_ag_mgmt_cback_1 to 6
 *
 * Description      RFCOMM callback functions.  This is an easy way to
 *                  distinguish scb from the callback.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_mgmt_cback_1(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 1);
}
void bta_ag_mgmt_cback_2(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 2);
}
void bta_ag_mgmt_cback_3(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 3);
}
void bta_ag_mgmt_cback_4(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 4);
}
void bta_ag_mgmt_cback_5(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 5);
}
void bta_ag_mgmt_cback_6(uint32_t code, uint16_t port_handle) {
  bta_ag_mgmt_cback(code, port_handle, 6);
}
void bta_ag_port_cback_1(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 1);
}
void bta_ag_port_cback_2(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 2);
}
void bta_ag_port_cback_3(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 3);
}
void bta_ag_port_cback_4(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 4);
}
void bta_ag_port_cback_5(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 5);
}
void bta_ag_port_cback_6(uint32_t code, uint16_t port_handle) {
  bta_ag_port_cback(code, port_handle, 6);
}

/*******************************************************************************
 *
 * Function         bta_ag_setup_port
 *
 * Description      Setup RFCOMM port for use by AG.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_setup_port(tBTA_AG_SCB* p_scb, uint16_t handle) {
  int port_callback_index = bta_ag_scb_to_idx(p_scb) - 1;
  CHECK_GE(port_callback_index, 0)
      << "invalid callback index, handle=" << handle << ", bd_addr"
      << p_scb->peer_addr;
  CHECK_LT(port_callback_index,
           static_cast<int>(sizeof(bta_ag_port_cback_tbl) /
                            sizeof(bta_ag_port_cback_tbl[0])))
      << "callback index out of bound, handle=" << handle << ", bd_addr"
      << p_scb->peer_addr;
  PORT_SetEventMask(handle, BTA_AG_PORT_EV_MASK);
  PORT_SetEventCallback(handle, bta_ag_port_cback_tbl[port_callback_index]);
}

/*******************************************************************************
 *
 * Function         bta_ag_start_servers
 *
 * Description      Setup RFCOMM servers for use by AG.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_start_servers(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK services) {
  services >>= BTA_HSP_SERVICE_ID;
  for (int i = 0; i < BTA_AG_NUM_IDX && services != 0; i++, services >>= 1) {
    /* if service is set in mask */
    if (services & 1) {
      BTM_SetSecurityLevel(false, "", bta_ag_sec_id[i], p_scb->serv_sec_mask,
                           BT_PSM_RFCOMM, BTM_SEC_PROTO_RFCOMM,
                           bta_ag_cb.profile[i].scn);
      int management_callback_index = bta_ag_scb_to_idx(p_scb) - 1;
      CHECK_GE(management_callback_index, 0)
          << "invalid callback index, services=" << loghex(services)
          << ", bd_addr=" << p_scb->peer_addr;
      CHECK_LT(management_callback_index,
               static_cast<int>(sizeof(bta_ag_mgmt_cback_tbl) /
                                sizeof(bta_ag_mgmt_cback_tbl[0])))
          << "callback index out of bound, services=" << loghex(services)
          << ", bd_addr" << p_scb->peer_addr;
      int status = RFCOMM_CreateConnection(
          bta_ag_uuid[i], bta_ag_cb.profile[i].scn, true, BTA_AG_MTU,
          RawAddress::kAny, &(p_scb->serv_handle[i]),
          bta_ag_mgmt_cback_tbl[management_callback_index]);
      if (status == PORT_SUCCESS) {
        bta_ag_setup_port(p_scb, p_scb->serv_handle[i]);
      } else {
        /* TODO: CR#137125 to handle to error properly */
        LOG(ERROR) << __func__ << ": RFCOMM_CreateConnection ERROR " << status
                   << ", p_scb=" << p_scb << ", services=" << loghex(services)
                   << ", mgmt_cback_index=" << management_callback_index;
      }
      APPL_TRACE_DEBUG("%s: p_scb=0x%08x, services=0x%04x, mgmt_cback_index=%d",
                       __func__, p_scb, services, management_callback_index);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_close_servers
 *
 * Description      Close RFCOMM servers port for use by AG.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_close_servers(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK services) {
  services >>= BTA_HSP_SERVICE_ID;
  for (int i = 0; i < BTA_AG_NUM_IDX && services != 0; i++, services >>= 1) {
    /* if service is set in mask */
    if (services & 1) {
      RFCOMM_RemoveServer(p_scb->serv_handle[i]);
      p_scb->serv_handle[i] = 0;
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_is_server_closed
 *
 * Description      Returns true if all servers are closed.
 *
 *
 * Returns          true if all servers are closed, false otherwise
 *
 ******************************************************************************/
bool bta_ag_is_server_closed(tBTA_AG_SCB* p_scb) {
  uint8_t xx;
  bool is_closed = true;

  for (xx = 0; xx < BTA_AG_NUM_IDX; xx++) {
    if (p_scb->serv_handle[xx] != 0) is_closed = false;
  }

  return is_closed;
}

/*******************************************************************************
 *
 * Function         bta_ag_rfc_do_open
 *
 * Description      Open an RFCOMM connection to the peer device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_rfc_do_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data) {
  BTM_SetSecurityLevel(true, "", bta_ag_sec_id[p_scb->conn_service],
                       p_scb->cli_sec_mask, BT_PSM_RFCOMM, BTM_SEC_PROTO_RFCOMM,
                       p_scb->peer_scn);

  int management_callback_index = bta_ag_scb_to_idx(p_scb) - 1;
  int status = RFCOMM_CreateConnection(
      bta_ag_uuid[p_scb->conn_service], p_scb->peer_scn, false, BTA_AG_MTU,
      p_scb->peer_addr, &(p_scb->conn_handle),
      bta_ag_mgmt_cback_tbl[management_callback_index]);
  APPL_TRACE_DEBUG(
      "%s: p_scb=0x%08x, conn_handle=%d, mgmt_cback_index=%d,"
      " status=%d",
      __func__, p_scb, p_scb->conn_handle, management_callback_index, status);
  if (status == PORT_SUCCESS) {
    bta_ag_setup_port(p_scb, p_scb->conn_handle);
  } else {
    /* RFCOMM create connection failed; send ourselves RFCOMM close event */
    LOG(ERROR) << __func__ << ": RFCOMM_CreateConnection ERROR " << status
               << " for " << p_scb->peer_addr;
    bta_ag_sm_execute(p_scb, BTA_AG_RFC_CLOSE_EVT, data);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_rfc_do_close
 *
 * Description      Close RFCOMM connection.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_rfc_do_close(tBTA_AG_SCB* p_scb,
                         UNUSED_ATTR const tBTA_AG_DATA& data) {
  if (p_scb->conn_handle) {
    RFCOMM_RemoveConnection(p_scb->conn_handle);
  } else {
    /* Close API was called while AG is in Opening state.               */
    /* Need to trigger the state machine to send callback to the app    */
    /* and move back to INIT state.                                     */
    do_in_main_thread(
        FROM_HERE,
        base::Bind(&bta_ag_sm_execute_by_handle, bta_ag_scb_to_idx(p_scb),
                   BTA_AG_RFC_CLOSE_EVT, tBTA_AG_DATA::kEmpty));

    /* Cancel SDP if it had been started. */
    /*
    if(p_scb->p_disc_db)
    {
        (void)SDP_CancelServiceSearch (p_scb->p_disc_db);
    }
    */
  }
}
