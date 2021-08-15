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

#include <base/strings/stringprintf.h>
#include <string.h>
#include "bt_target.h"
#include "bt_utils.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "gap_api.h"
#include "l2c_int.h"
#include "l2cdefs.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/mutex.h"

using base::StringPrintf;

/* Define the GAP Connection Control Block */
typedef struct {
#define GAP_CCB_STATE_IDLE 0
#define GAP_CCB_STATE_LISTENING 1
#define GAP_CCB_STATE_CONN_SETUP 2
#define GAP_CCB_STATE_CFG_SETUP 3
#define GAP_CCB_STATE_WAIT_SEC 4
#define GAP_CCB_STATE_CONNECTED 5
  uint8_t con_state;

#define GAP_CCB_FLAGS_IS_ORIG 0x01
#define GAP_CCB_FLAGS_HIS_CFG_DONE 0x02
#define GAP_CCB_FLAGS_MY_CFG_DONE 0x04
#define GAP_CCB_FLAGS_SEC_DONE 0x08
#define GAP_CCB_FLAGS_CONN_DONE 0x0E
  uint8_t con_flags;

  uint8_t service_id;     /* Used by BTM */
  uint16_t gap_handle;    /* GAP handle */
  uint16_t connection_id; /* L2CAP CID */
  bool rem_addr_specified;
  uint8_t chan_mode_mask; /* Supported channel modes (FCR) */
  RawAddress rem_dev_address;
  uint16_t psm;
  uint16_t rem_mtu_size;

  bool is_congested;
  fixed_queue_t* tx_queue; /* Queue of buffers waiting to be sent */
  fixed_queue_t* rx_queue; /* Queue of buffers waiting to be read */

  uint32_t rx_queue_size; /* Total data count in rx_queue */

  tGAP_CONN_CALLBACK* p_callback; /* Users callback function */

  tL2CAP_CFG_INFO cfg;              /* Configuration */
  tL2CAP_ERTM_INFO ertm_info;       /* Pools and modes for ertm */
  tBT_TRANSPORT transport;          /* Transport channel BR/EDR or BLE */
  tL2CAP_LE_CFG_INFO local_coc_cfg; /* local configuration for LE Coc */
  tL2CAP_LE_CFG_INFO peer_coc_cfg;  /* local configuration for LE Coc */
} tGAP_CCB;

typedef struct {
  tL2CAP_APPL_INFO reg_info; /* L2CAP Registration info */
  tGAP_CCB ccb_pool[GAP_MAX_CONNECTIONS];
} tGAP_CONN;

namespace {
tGAP_CONN conn;
}

/******************************************************************************/
/*            L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/******************************************************************************/
static void gap_connect_ind(const RawAddress& bd_addr, uint16_t l2cap_cid,
                            uint16_t psm, uint8_t l2cap_id);
static void gap_connect_cfm(uint16_t l2cap_cid, uint16_t result);
static void gap_config_ind(uint16_t l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void gap_config_cfm(uint16_t l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void gap_disconnect_ind(uint16_t l2cap_cid, bool ack_needed);
static void gap_data_ind(uint16_t l2cap_cid, BT_HDR* p_msg);
static void gap_congestion_ind(uint16_t lcid, bool is_congested);
static void gap_tx_complete_ind(uint16_t l2cap_cid, uint16_t sdu_sent);
static void gap_credits_received_cb(uint16_t l2cap_cid,
                                    uint16_t credits_received,
                                    uint16_t credit_count);

static tGAP_CCB* gap_find_ccb_by_cid(uint16_t cid);
static tGAP_CCB* gap_find_ccb_by_handle(uint16_t handle);
static tGAP_CCB* gap_allocate_ccb(void);
static void gap_release_ccb(tGAP_CCB* p_ccb);
static void gap_checks_con_flags(tGAP_CCB* p_ccb);

/*******************************************************************************
 *
 * Function         gap_conn_init
 *
 * Description      This function is called to initialize GAP connection
 *                  management
 *
 * Returns          void
 *
 ******************************************************************************/
void gap_conn_init(void) {
  memset(&conn, 0, sizeof(tGAP_CONN));
  conn.reg_info.pL2CA_ConnectInd_Cb = gap_connect_ind;
  conn.reg_info.pL2CA_ConnectCfm_Cb = gap_connect_cfm;
  conn.reg_info.pL2CA_ConnectPnd_Cb = NULL;
  conn.reg_info.pL2CA_ConfigInd_Cb = gap_config_ind;
  conn.reg_info.pL2CA_ConfigCfm_Cb = gap_config_cfm;
  conn.reg_info.pL2CA_DisconnectInd_Cb = gap_disconnect_ind;
  conn.reg_info.pL2CA_DisconnectCfm_Cb = NULL;
  conn.reg_info.pL2CA_QoSViolationInd_Cb = NULL;
  conn.reg_info.pL2CA_DataInd_Cb = gap_data_ind;
  conn.reg_info.pL2CA_CongestionStatus_Cb = gap_congestion_ind;
  conn.reg_info.pL2CA_TxComplete_Cb = gap_tx_complete_ind;
  conn.reg_info.pL2CA_CreditsReceived_Cb = gap_credits_received_cb;
}

/*******************************************************************************
 *
 * Function         GAP_ConnOpen
 *
 * Description      This function is called to open an L2CAP connection.
 *
 * Parameters:      is_server   - If true, the connection is not created
 *                                but put into a "listen" mode waiting for
 *                                the remote side to connect.
 *
 *                  service_id  - Unique service ID from
 *                                BTM_SEC_SERVICE_FIRST_EMPTY (6)
 *                                to BTM_SEC_MAX_SERVICE_RECORDS (32)
 *
 *                  p_rem_bda   - Pointer to remote BD Address.
 *                                If a server, and we don't care about the
 *                                remote BD Address, then NULL should be passed.
 *
 *                  psm         - the PSM used for the connection
 *                  le_mps      - Maximum PDU Size for LE CoC
 *
 *                  p_config    - Optional pointer to configuration structure.
 *                                If NULL, the default GAP configuration will
 *                                be used.
 *
 *                  security    - security flags
 *                  chan_mode_mask - (GAP_FCR_CHAN_OPT_BASIC,
 *                                    GAP_FCR_CHAN_OPT_ERTM,
 *                                    GAP_FCR_CHAN_OPT_STREAM)
 *
 *                  p_cb        - Pointer to callback function for events.
 *
 * Returns          handle of the connection if successful, else
 *                            GAP_INVALID_HANDLE
 *
 ******************************************************************************/
uint16_t GAP_ConnOpen(const char* p_serv_name, uint8_t service_id,
                      bool is_server, const RawAddress* p_rem_bda, uint16_t psm,
                      uint16_t le_mps, tL2CAP_CFG_INFO* p_cfg,
                      tL2CAP_ERTM_INFO* ertm_info, uint16_t security,
                      uint8_t chan_mode_mask, tGAP_CONN_CALLBACK* p_cb,
                      tBT_TRANSPORT transport) {
  tGAP_CCB* p_ccb;
  uint16_t cid;

  DVLOG(1) << "GAP_CONN - Open Request";

  /* Allocate a new CCB. Return if none available. */
  p_ccb = gap_allocate_ccb();
  if (p_ccb == NULL) return (GAP_INVALID_HANDLE);

  /* update the transport */
  p_ccb->transport = transport;

  /* The service_id must be set before calling gap_release_ccb(). */
  p_ccb->service_id = service_id;

  /* If caller specified a BD address, save it */
  if (p_rem_bda) {
    /* the bd addr is not RawAddress::kAny, then a bd address was specified */
    if (*p_rem_bda != RawAddress::kAny) p_ccb->rem_addr_specified = true;

    p_ccb->rem_dev_address = *p_rem_bda;
  } else if (!is_server) {
    /* remore addr is not specified and is not a server -> bad */
    return (GAP_INVALID_HANDLE);
  }

  /* A client MUST have specified a bd addr to connect with */
  if (!p_ccb->rem_addr_specified && !is_server) {
    gap_release_ccb(p_ccb);
    LOG(ERROR)
        << "GAP ERROR: Client must specify a remote BD ADDR to connect to!";
    return (GAP_INVALID_HANDLE);
  }

  /* Check if configuration was specified */
  if (p_cfg) p_ccb->cfg = *p_cfg;

  /* Configure L2CAP COC, if transport is LE */
  if (transport == BT_TRANSPORT_LE) {
    p_ccb->local_coc_cfg.credits = L2CAP_LE_CREDIT_DEFAULT;
    p_ccb->local_coc_cfg.mtu = p_cfg->mtu;

    uint16_t max_mps = controller_get_interface()->get_acl_data_size_ble();
    if (le_mps > max_mps) {
      LOG(INFO) << "Limiting MPS to one buffer size - " << max_mps;
      le_mps = max_mps;
    }
    p_ccb->local_coc_cfg.mps = le_mps;

    VLOG(2) << __func__ << ": credits=" << p_ccb->local_coc_cfg.credits
            << ", mps=" << p_ccb->local_coc_cfg.mps
            << ", mtu=" << p_ccb->local_coc_cfg.mtu;
  }

  p_ccb->p_callback = p_cb;

  /* If originator, use a dynamic PSM */
  if (!is_server)
    conn.reg_info.pL2CA_ConnectInd_Cb = NULL;
  else
    conn.reg_info.pL2CA_ConnectInd_Cb = gap_connect_ind;

  /* Register the PSM with L2CAP */
  if (transport == BT_TRANSPORT_BR_EDR) {
    p_ccb->psm = L2CA_REGISTER(psm, &conn.reg_info, false /* enable_snoop */);
    if (p_ccb->psm == 0) {
      LOG(ERROR) << StringPrintf("%s: Failure registering PSM 0x%04x", __func__,
                                 psm);
      gap_release_ccb(p_ccb);
      return (GAP_INVALID_HANDLE);
    }
  }

  if (transport == BT_TRANSPORT_LE) {
    p_ccb->psm = L2CA_REGISTER_COC(
        psm, &conn.reg_info, AMP_AUTOSWITCH_ALLOWED | AMP_USE_AMP_IF_POSSIBLE);
    if (p_ccb->psm == 0) {
      LOG(ERROR) << StringPrintf("%s: Failure registering PSM 0x%04x", __func__,
                                 psm);
      gap_release_ccb(p_ccb);
      return (GAP_INVALID_HANDLE);
    }
  }

  /* Register with Security Manager for the specific security level */
  if (!BTM_SetSecurityLevel((uint8_t)!is_server, p_serv_name, p_ccb->service_id,
                            security, p_ccb->psm, 0, 0)) {
    LOG(ERROR) << "GAP_CONN - Security Error";
    gap_release_ccb(p_ccb);
    return (GAP_INVALID_HANDLE);
  }

  /* Fill in eL2CAP parameter data */
  if (p_ccb->cfg.fcr_present) {
    if (ertm_info == NULL) {
      p_ccb->ertm_info.preferred_mode = p_ccb->cfg.fcr.mode;
      p_ccb->ertm_info.user_rx_buf_size = GAP_DATA_BUF_SIZE;
      p_ccb->ertm_info.user_tx_buf_size = GAP_DATA_BUF_SIZE;
      p_ccb->ertm_info.fcr_rx_buf_size = L2CAP_INVALID_ERM_BUF_SIZE;
      p_ccb->ertm_info.fcr_tx_buf_size = L2CAP_INVALID_ERM_BUF_SIZE;
    } else {
      p_ccb->ertm_info = *ertm_info;
    }
  }

  /* optional FCR channel modes */
  if (ertm_info != NULL) {
    p_ccb->ertm_info.allowed_modes =
        (chan_mode_mask) ? chan_mode_mask : (uint8_t)L2CAP_FCR_CHAN_OPT_BASIC;
  }

  if (is_server) {
    p_ccb->con_flags |=
        GAP_CCB_FLAGS_SEC_DONE; /* assume btm/l2cap would handle it */
    p_ccb->con_state = GAP_CCB_STATE_LISTENING;
    return (p_ccb->gap_handle);
  } else {
    /* We are the originator of this connection */
    p_ccb->con_flags = GAP_CCB_FLAGS_IS_ORIG;

    /* Transition to the next appropriate state, waiting for connection confirm.
     */
    p_ccb->con_state = GAP_CCB_STATE_CONN_SETUP;

    /* mark security done flag, when security is not required */
    if ((security & (BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_AUTHENTICATE |
                     BTM_SEC_OUT_ENCRYPT)) == 0)
      p_ccb->con_flags |= GAP_CCB_FLAGS_SEC_DONE;

    /* Check if L2CAP started the connection process */
    if (p_rem_bda && (transport == BT_TRANSPORT_BR_EDR)) {
      cid = L2CA_CONNECT_REQ(p_ccb->psm, *p_rem_bda, &p_ccb->ertm_info);
      if (cid != 0) {
        p_ccb->connection_id = cid;
        return (p_ccb->gap_handle);
      }
    }

    if (p_rem_bda && (transport == BT_TRANSPORT_LE)) {
      cid = L2CA_CONNECT_COC_REQ(p_ccb->psm, *p_rem_bda, &p_ccb->local_coc_cfg);
      if (cid != 0) {
        p_ccb->connection_id = cid;
        return (p_ccb->gap_handle);
      }
    }

    gap_release_ccb(p_ccb);
    return (GAP_INVALID_HANDLE);
  }
}

/*******************************************************************************
 *
 * Function         GAP_ConnClose
 *
 * Description      This function is called to close a connection.
 *
 * Parameters:      handle - Handle of the connection returned by GAP_ConnOpen
 *
 * Returns          BT_PASS             - closed OK
 *                  GAP_ERR_BAD_HANDLE  - invalid handle
 *
 ******************************************************************************/
uint16_t GAP_ConnClose(uint16_t gap_handle) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

  DVLOG(1) << StringPrintf("GAP_CONN - close  handle: 0x%x", gap_handle);

  if (p_ccb) {
    /* Check if we have a connection ID */
    if (p_ccb->con_state != GAP_CCB_STATE_LISTENING)
      L2CA_DISCONNECT_REQ(p_ccb->connection_id);

    gap_release_ccb(p_ccb);

    return (BT_PASS);
  }

  return (GAP_ERR_BAD_HANDLE);
}

/*******************************************************************************
 *
 * Function         GAP_ConnReadData
 *
 * Description      Normally not GKI aware application will call this function
 *                  after receiving GAP_EVT_RXDATA event.
 *
 * Parameters:      handle      - Handle of the connection returned in the Open
 *                  p_data      - Data area
 *                  max_len     - Byte count requested
 *                  p_len       - Byte count received
 *
 * Returns          BT_PASS             - data read
 *                  GAP_ERR_BAD_HANDLE  - invalid handle
 *                  GAP_NO_DATA_AVAIL   - no data available
 *
 ******************************************************************************/
uint16_t GAP_ConnReadData(uint16_t gap_handle, uint8_t* p_data,
                          uint16_t max_len, uint16_t* p_len) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);
  uint16_t copy_len;

  if (!p_ccb) return (GAP_ERR_BAD_HANDLE);

  *p_len = 0;

  if (fixed_queue_is_empty(p_ccb->rx_queue)) return (GAP_NO_DATA_AVAIL);

  mutex_global_lock();

  while (max_len) {
    BT_HDR* p_buf =
        static_cast<BT_HDR*>(fixed_queue_try_peek_first(p_ccb->rx_queue));
    if (p_buf == NULL) break;

    copy_len = (p_buf->len > max_len) ? max_len : p_buf->len;
    max_len -= copy_len;
    *p_len += copy_len;
    if (p_data) {
      memcpy(p_data, (uint8_t*)(p_buf + 1) + p_buf->offset, copy_len);
      p_data += copy_len;
    }

    if (p_buf->len > copy_len) {
      p_buf->offset += copy_len;
      p_buf->len -= copy_len;
      break;
    }
    osi_free(fixed_queue_try_dequeue(p_ccb->rx_queue));
  }

  p_ccb->rx_queue_size -= *p_len;

  mutex_global_unlock();

  DVLOG(1) << StringPrintf(
      "GAP_ConnReadData - rx_queue_size left=%d, *p_len=%d",
      p_ccb->rx_queue_size, *p_len);

  return (BT_PASS);
}

/*******************************************************************************
 *
 * Function         GAP_GetRxQueueCnt
 *
 * Description      This function return number of bytes on the rx queue.
 *
 * Parameters:      handle     - Handle returned in the GAP_ConnOpen
 *                  p_rx_queue_count - Pointer to return queue count in.
 *
 *
 ******************************************************************************/
int GAP_GetRxQueueCnt(uint16_t handle, uint32_t* p_rx_queue_count) {
  tGAP_CCB* p_ccb;
  int rc = BT_PASS;

  /* Check that handle is valid */
  if (handle < GAP_MAX_CONNECTIONS) {
    p_ccb = &conn.ccb_pool[handle];

    if (p_ccb->con_state == GAP_CCB_STATE_CONNECTED) {
      *p_rx_queue_count = p_ccb->rx_queue_size;
    } else
      rc = GAP_INVALID_HANDLE;
  } else
    rc = GAP_INVALID_HANDLE;

  DVLOG(1) << StringPrintf("GAP_GetRxQueueCnt - rc = 0x%04x, rx_queue_count=%d",
                           rc, *p_rx_queue_count);

  return (rc);
}

/*******************************************************************************
 *
 * Function         GAP_ConnBTRead
 *
 * Description      Bluetooth-aware applications will call this function after
 *                  receiving GAP_EVT_RXDATA event.
 *
 * Parameters:      handle      - Handle of the connection returned in the Open
 *                  pp_buf      - pointer to address of buffer with data,
 *
 * Returns          BT_PASS             - data read
 *                  GAP_ERR_BAD_HANDLE  - invalid handle
 *                  GAP_NO_DATA_AVAIL   - no data available
 *
 ******************************************************************************/
uint16_t GAP_ConnBTRead(uint16_t gap_handle, BT_HDR** pp_buf) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);
  BT_HDR* p_buf;

  if (!p_ccb) return (GAP_ERR_BAD_HANDLE);

  p_buf = (BT_HDR*)fixed_queue_try_dequeue(p_ccb->rx_queue);

  if (p_buf) {
    *pp_buf = p_buf;

    p_ccb->rx_queue_size -= p_buf->len;
    return (BT_PASS);
  } else {
    *pp_buf = NULL;
    return (GAP_NO_DATA_AVAIL);
  }
}

/* Try to write the queued data to l2ca. Return true on success, or if queue is
 * congested. False if error occured when writing. */
static bool gap_try_write_queued_data(tGAP_CCB* p_ccb) {
  if (p_ccb->is_congested) return true;

  /* Send the buffer through L2CAP */
  BT_HDR* p_buf;
  while ((p_buf = (BT_HDR*)fixed_queue_try_dequeue(p_ccb->tx_queue)) != NULL) {
    uint8_t status = L2CA_DATA_WRITE(p_ccb->connection_id, p_buf);

    if (status == L2CAP_DW_CONGESTED) {
      p_ccb->is_congested = true;
      return true;
    } else if (status != L2CAP_DW_SUCCESS)
      return false;
  }
  return true;
}

/*******************************************************************************
 *
 * Function         GAP_ConnWriteData
 *
 * Description      Normally not GKI aware application will call this function
 *                  to send data to the connection.
 *
 * Parameters:      handle      - Handle of the connection returned in the Open
 *                  msg         - pointer to single SDU to send. This function
 *                                will take ownership of it.
 *
 * Returns          BT_PASS                 - data read
 *                  GAP_ERR_BAD_HANDLE      - invalid handle
 *                  GAP_ERR_BAD_STATE       - connection not established
 *                  GAP_CONGESTION          - system is congested
 *
 ******************************************************************************/
uint16_t GAP_ConnWriteData(uint16_t gap_handle, BT_HDR* msg) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

  if (!p_ccb) {
    osi_free(msg);
    return GAP_ERR_BAD_HANDLE;
  }

  if (p_ccb->con_state != GAP_CCB_STATE_CONNECTED) {
    osi_free(msg);
    return GAP_ERR_BAD_STATE;
  }

  if (msg->len > p_ccb->rem_mtu_size) {
    osi_free(msg);
    return GAP_ERR_ILL_PARM;
  }

  DVLOG(1) << StringPrintf("GAP_WriteData %d bytes", msg->len);

  fixed_queue_enqueue(p_ccb->tx_queue, msg);

  if (!gap_try_write_queued_data(p_ccb)) return GAP_ERR_BAD_STATE;

  return (BT_PASS);
}

/*******************************************************************************
 *
 * Function         GAP_ConnReconfig
 *
 * Description      Applications can call this function to reconfigure the
 *                  connection.
 *
 * Parameters:      handle      - Handle of the connection
 *                  p_cfg       - Pointer to new configuration
 *
 * Returns          BT_PASS                 - config process started
 *                  GAP_ERR_BAD_HANDLE      - invalid handle
 *
 ******************************************************************************/
uint16_t GAP_ConnReconfig(uint16_t gap_handle, tL2CAP_CFG_INFO* p_cfg) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

  if (!p_ccb) return (GAP_ERR_BAD_HANDLE);

  p_ccb->cfg = *p_cfg;

  if (p_ccb->con_state == GAP_CCB_STATE_CONNECTED)
    L2CA_CONFIG_REQ(p_ccb->connection_id, p_cfg);

  return (BT_PASS);
}

/*******************************************************************************
 *
 * Function         GAP_ConnSetIdleTimeout
 *
 * Description      Higher layers call this function to set the idle timeout for
 *                  a connection, or for all future connections. The "idle
 *                  timeout" is the amount of time that a connection can remain
 *                  up with no L2CAP channels on it. A timeout of zero means
 *                  that the connection will be torn down immediately when the
 *                  last channel is removed. A timeout of 0xFFFF means no
 *                  timeout. Values are in seconds.
 *
 * Parameters:      handle      - Handle of the connection
 *                  timeout     - in secs
 *                                0 = immediate disconnect when last channel is
 *                                    removed
 *                                0xFFFF = no idle timeout
 *
 * Returns          BT_PASS                 - config process started
 *                  GAP_ERR_BAD_HANDLE      - invalid handle
 *
 ******************************************************************************/
uint16_t GAP_ConnSetIdleTimeout(uint16_t gap_handle, uint16_t timeout) {
  tGAP_CCB* p_ccb;

  p_ccb = gap_find_ccb_by_handle(gap_handle);
  if (p_ccb == NULL) return (GAP_ERR_BAD_HANDLE);

  if (L2CA_SetIdleTimeout(p_ccb->connection_id, timeout, false))
    return (BT_PASS);
  else
    return (GAP_ERR_BAD_HANDLE);
}

/*******************************************************************************
 *
 * Function         GAP_ConnGetRemoteAddr
 *
 * Description      This function is called to get the remote BD address
 *                  of a connection.
 *
 * Parameters:      handle - Handle of the connection returned by GAP_ConnOpen
 *
 * Returns          BT_PASS             - closed OK
 *                  GAP_ERR_BAD_HANDLE  - invalid handle
 *
 ******************************************************************************/
const RawAddress* GAP_ConnGetRemoteAddr(uint16_t gap_handle) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

  DVLOG(1) << __func__ << " gap_handle = " << gap_handle;

  if ((p_ccb) && (p_ccb->con_state > GAP_CCB_STATE_LISTENING)) {
    DVLOG(1) << __func__ << " BDA: " << p_ccb->rem_dev_address;
    return &p_ccb->rem_dev_address;
  } else {
    DVLOG(1) << __func__ << " return Error ";
    return nullptr;
  }
}

/*******************************************************************************
 *
 * Function         GAP_ConnGetRemMtuSize
 *
 * Description      Returns the remote device's MTU size
 *
 * Parameters:      handle      - Handle of the connection
 *
 * Returns          uint16_t    - maximum size buffer that can be transmitted to
 *                                the peer
 *
 ******************************************************************************/
uint16_t GAP_ConnGetRemMtuSize(uint16_t gap_handle) {
  tGAP_CCB* p_ccb;

  p_ccb = gap_find_ccb_by_handle(gap_handle);
  if (p_ccb == NULL) return (0);

  return (p_ccb->rem_mtu_size);
}

/*******************************************************************************
 *
 * Function         GAP_ConnGetL2CAPCid
 *
 * Description      Returns the L2CAP channel id
 *
 * Parameters:      handle      - Handle of the connection
 *
 * Returns          uint16_t    - The L2CAP channel id
 *                  0, if error
 *
 ******************************************************************************/
uint16_t GAP_ConnGetL2CAPCid(uint16_t gap_handle) {
  tGAP_CCB* p_ccb;

  p_ccb = gap_find_ccb_by_handle(gap_handle);
  if (p_ccb == NULL) return (0);

  return (p_ccb->connection_id);
}

/*******************************************************************************
 *
 * Function         gap_tx_connect_ind
 *
 * Description      Sends out GAP_EVT_TX_EMPTY when transmission has been
 *                  completed.
 *
 * Returns          void
 *
 ******************************************************************************/
void gap_tx_complete_ind(uint16_t l2cap_cid, uint16_t sdu_sent) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) return;

  if ((p_ccb->con_state == GAP_CCB_STATE_CONNECTED) && (sdu_sent == 0xFFFF)) {
    DVLOG(1) << StringPrintf("%s: GAP_EVT_TX_EMPTY", __func__);
    p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_TX_EMPTY, nullptr);
  }
}

void gap_credits_received_cb(uint16_t l2cap_cid, uint16_t credits_received,
                             uint16_t credit_count) {
  tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (!p_ccb) return;

  tGAP_CB_DATA data{.coc_credits = {.credits_received = credits_received,
                                    .credit_count = credit_count}};
  p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_LE_COC_CREDITS, &data);
}

/*******************************************************************************
 *
 * Function         gap_connect_ind
 *
 * Description      This function handles an inbound connection indication
 *                  from L2CAP. This is the case where we are acting as a
 *                  server.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_connect_ind(const RawAddress& bd_addr, uint16_t l2cap_cid,
                            uint16_t psm, uint8_t l2cap_id) {
  uint16_t xx;
  tGAP_CCB* p_ccb;

  /* See if we have a CCB listening for the connection */
  for (xx = 0, p_ccb = conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
    if ((p_ccb->con_state == GAP_CCB_STATE_LISTENING) && (p_ccb->psm == psm) &&
        (!p_ccb->rem_addr_specified || (bd_addr == p_ccb->rem_dev_address)))
      break;
  }

  if (xx == GAP_MAX_CONNECTIONS) {
    LOG(WARNING) << "*******";
    LOG(WARNING) << "WARNING: GAP Conn Indication for Unexpected Bd "
                    "Addr...Disconnecting";
    LOG(WARNING) << "*******";

    /* Disconnect because it is an unexpected connection */
    L2CA_DISCONNECT_REQ(l2cap_cid);
    return;
  }

  /* Transition to the next appropriate state, waiting for config setup. */
  if (p_ccb->transport == BT_TRANSPORT_BR_EDR)
    p_ccb->con_state = GAP_CCB_STATE_CFG_SETUP;

  /* Save the BD Address and Channel ID. */
  p_ccb->rem_dev_address = bd_addr;
  p_ccb->connection_id = l2cap_cid;

  /* Send response to the L2CAP layer. */
  if (p_ccb->transport == BT_TRANSPORT_BR_EDR)
    L2CA_CONNECT_RSP(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_OK, L2CAP_CONN_OK,
                     &p_ccb->ertm_info);

  if (p_ccb->transport == BT_TRANSPORT_LE) {
    L2CA_CONNECT_COC_RSP(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_OK,
                         L2CAP_CONN_OK, &p_ccb->local_coc_cfg);

    /* get the remote coc configuration */
    L2CA_GET_PEER_COC_CONFIG(l2cap_cid, &p_ccb->peer_coc_cfg);
    p_ccb->rem_mtu_size = p_ccb->peer_coc_cfg.mtu;

    /* configuration is not required for LE COC */
    p_ccb->con_flags |= GAP_CCB_FLAGS_HIS_CFG_DONE;
    p_ccb->con_flags |= GAP_CCB_FLAGS_MY_CFG_DONE;
    gap_checks_con_flags(p_ccb);
  }

  DVLOG(1) << StringPrintf("GAP_CONN - Rcvd L2CAP conn ind, CID: 0x%x",
                           p_ccb->connection_id);

  /* Send a Configuration Request. */
  if (p_ccb->transport == BT_TRANSPORT_BR_EDR)
    L2CA_CONFIG_REQ(l2cap_cid, &p_ccb->cfg);
}

/*******************************************************************************
 *
 * Function         gap_checks_con_flags
 *
 * Description      This function processes the L2CAP configuration indication
 *                  event.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_checks_con_flags(tGAP_CCB* p_ccb) {
  DVLOG(1) << __func__ << " conn_flags:0x" << +p_ccb->con_flags;
  /* if all the required con_flags are set, report the OPEN event now */
  if ((p_ccb->con_flags & GAP_CCB_FLAGS_CONN_DONE) == GAP_CCB_FLAGS_CONN_DONE) {
    p_ccb->con_state = GAP_CCB_STATE_CONNECTED;

    p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_CONN_OPENED, nullptr);
  }
}

/*******************************************************************************
 *
 * Function         gap_sec_check_complete
 *
 * Description      The function called when Security Manager finishes
 *                  verification of the service side connection
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_sec_check_complete(const RawAddress*, tBT_TRANSPORT,
                                   void* p_ref_data, uint8_t res) {
  tGAP_CCB* p_ccb = (tGAP_CCB*)p_ref_data;

  DVLOG(1) << StringPrintf(
      "gap_sec_check_complete conn_state:%d, conn_flags:0x%x, status:%d",
      p_ccb->con_state, p_ccb->con_flags, res);
  if (p_ccb->con_state == GAP_CCB_STATE_IDLE) return;

  if (res == BTM_SUCCESS) {
    p_ccb->con_flags |= GAP_CCB_FLAGS_SEC_DONE;
    gap_checks_con_flags(p_ccb);
  } else {
    /* security failed - disconnect the channel */
    L2CA_DISCONNECT_REQ(p_ccb->connection_id);
  }
}

/*******************************************************************************
 *
 * Function         gap_connect_cfm
 *
 * Description      This function handles the connect confirm events
 *                  from L2CAP. This is the case when we are acting as a
 *                  client and have sent a connect request.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_connect_cfm(uint16_t l2cap_cid, uint16_t result) {
  tGAP_CCB* p_ccb;

  /* Find CCB based on CID */
  p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) return;

  /* initiate security process, if needed */
  if ((p_ccb->con_flags & GAP_CCB_FLAGS_SEC_DONE) == 0 &&
      p_ccb->transport != BT_TRANSPORT_LE) {
    btm_sec_mx_access_request(p_ccb->rem_dev_address, p_ccb->psm, true, 0, 0,
                              &gap_sec_check_complete, p_ccb);
  }

  /* If the connection response contains success status, then */
  /* Transition to the next state and startup the timer.      */
  if ((result == L2CAP_CONN_OK) &&
      (p_ccb->con_state == GAP_CCB_STATE_CONN_SETUP)) {
    if (p_ccb->transport == BT_TRANSPORT_BR_EDR) {
      p_ccb->con_state = GAP_CCB_STATE_CFG_SETUP;

      /* Send a Configuration Request. */
      L2CA_CONFIG_REQ(l2cap_cid, &p_ccb->cfg);
    }

    if (p_ccb->transport == BT_TRANSPORT_LE) {
      /* get the remote coc configuration */
      L2CA_GET_PEER_COC_CONFIG(l2cap_cid, &p_ccb->peer_coc_cfg);
      p_ccb->rem_mtu_size = p_ccb->peer_coc_cfg.mtu;

      /* configuration is not required for LE COC */
      p_ccb->con_flags |= GAP_CCB_FLAGS_HIS_CFG_DONE;
      p_ccb->con_flags |= GAP_CCB_FLAGS_MY_CFG_DONE;
      p_ccb->con_flags |= GAP_CCB_FLAGS_SEC_DONE;
      gap_checks_con_flags(p_ccb);
    }
  } else {
    /* Tell the user if he has a callback */
    if (p_ccb->p_callback)
      (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED, nullptr);

    gap_release_ccb(p_ccb);
  }
}

/*******************************************************************************
 *
 * Function         gap_config_ind
 *
 * Description      This function processes the L2CAP configuration indication
 *                  event.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_config_ind(uint16_t l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
  tGAP_CCB* p_ccb;
  uint16_t local_mtu_size;

  /* Find CCB based on CID */
  p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) return;

  /* Remember the remote MTU size */

  if (p_ccb->cfg.fcr.mode == L2CAP_FCR_ERTM_MODE) {
    local_mtu_size =
        p_ccb->ertm_info.user_tx_buf_size - sizeof(BT_HDR) - L2CAP_MIN_OFFSET;
  } else
    local_mtu_size = L2CAP_MTU_SIZE;

  if ((!p_cfg->mtu_present) || (p_cfg->mtu > local_mtu_size)) {
    p_ccb->rem_mtu_size = local_mtu_size;
  } else
    p_ccb->rem_mtu_size = p_cfg->mtu;

  /* For now, always accept configuration from the other side */
  p_cfg->flush_to_present = false;
  p_cfg->mtu_present = false;
  p_cfg->result = L2CAP_CFG_OK;
  p_cfg->fcs_present = false;

  L2CA_CONFIG_RSP(l2cap_cid, p_cfg);

  p_ccb->con_flags |= GAP_CCB_FLAGS_HIS_CFG_DONE;

  gap_checks_con_flags(p_ccb);
}

/*******************************************************************************
 *
 * Function         gap_config_cfm
 *
 * Description      This function processes the L2CAP configuration confirmation
 *                  event.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_config_cfm(uint16_t l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
  tGAP_CCB* p_ccb;

  /* Find CCB based on CID */
  p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) return;

  if (p_cfg->result == L2CAP_CFG_OK) {
    p_ccb->con_flags |= GAP_CCB_FLAGS_MY_CFG_DONE;

    if (p_ccb->cfg.fcr_present)
      p_ccb->cfg.fcr.mode = p_cfg->fcr.mode;
    else
      p_ccb->cfg.fcr.mode = L2CAP_FCR_BASIC_MODE;

    gap_checks_con_flags(p_ccb);
  } else {
    p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED, nullptr);
    gap_release_ccb(p_ccb);
  }
}

/*******************************************************************************
 *
 * Function         gap_disconnect_ind
 *
 * Description      This function handles a disconnect event from L2CAP. If
 *                  requested to, we ack the disconnect before dropping the CCB
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_disconnect_ind(uint16_t l2cap_cid, bool ack_needed) {
  tGAP_CCB* p_ccb;

  DVLOG(1) << StringPrintf("GAP_CONN - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);

  /* Find CCB based on CID */
  p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) return;

  if (ack_needed) L2CA_DISCONNECT_RSP(l2cap_cid);

  p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED, nullptr);
  gap_release_ccb(p_ccb);
}

/*******************************************************************************
 *
 * Function         gap_data_ind
 *
 * Description      This function is called when data is received from L2CAP.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_data_ind(uint16_t l2cap_cid, BT_HDR* p_msg) {
  tGAP_CCB* p_ccb;

  /* Find CCB based on CID */
  p_ccb = gap_find_ccb_by_cid(l2cap_cid);
  if (p_ccb == NULL) {
    osi_free(p_msg);
    return;
  }

  if (p_ccb->con_state == GAP_CCB_STATE_CONNECTED) {
    fixed_queue_enqueue(p_ccb->rx_queue, p_msg);

    p_ccb->rx_queue_size += p_msg->len;
    /*
    DVLOG(1) << StringPrintf ("gap_data_ind - rx_queue_size=%d, msg len=%d",
                                   p_ccb->rx_queue_size, p_msg->len);
     */

    p_ccb->p_callback(p_ccb->gap_handle, GAP_EVT_CONN_DATA_AVAIL, nullptr);
  } else {
    osi_free(p_msg);
  }
}

/*******************************************************************************
 *
 * Function         gap_congestion_ind
 *
 * Description      This is a callback function called by L2CAP when
 *                  data L2CAP congestion status changes
 *
 ******************************************************************************/
static void gap_congestion_ind(uint16_t lcid, bool is_congested) {
  DVLOG(1) << StringPrintf("GAP_CONN - Rcvd L2CAP Is Congested (%d), CID: 0x%x",
                           is_congested, lcid);

  tGAP_CCB* p_ccb = gap_find_ccb_by_cid(lcid); /* Find CCB based on CID */
  if (!p_ccb) return;

  p_ccb->is_congested = is_congested;

  p_ccb->p_callback(
      p_ccb->gap_handle,
      (is_congested) ? GAP_EVT_CONN_CONGESTED : GAP_EVT_CONN_UNCONGESTED,
      nullptr);

  gap_try_write_queued_data(p_ccb);
}

/*******************************************************************************
 *
 * Function         gap_find_ccb_by_cid
 *
 * Description      This function searches the CCB table for an entry with the
 *                  passed CID.
 *
 * Returns          the CCB address, or NULL if not found.
 *
 ******************************************************************************/
static tGAP_CCB* gap_find_ccb_by_cid(uint16_t cid) {
  uint16_t xx;
  tGAP_CCB* p_ccb;

  /* Look through each connection control block */
  for (xx = 0, p_ccb = conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
    if ((p_ccb->con_state != GAP_CCB_STATE_IDLE) &&
        (p_ccb->connection_id == cid))
      return (p_ccb);
  }

  /* If here, not found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         gap_find_ccb_by_handle
 *
 * Description      This function searches the CCB table for an entry with the
 *                  passed handle.
 *
 * Returns          the CCB address, or NULL if not found.
 *
 ******************************************************************************/
static tGAP_CCB* gap_find_ccb_by_handle(uint16_t handle) {
  tGAP_CCB* p_ccb;

  /* Check that handle is valid */
  if (handle < GAP_MAX_CONNECTIONS) {
    p_ccb = &conn.ccb_pool[handle];

    if (p_ccb->con_state != GAP_CCB_STATE_IDLE) return (p_ccb);
  }

  /* If here, handle points to invalid connection */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         gap_allocate_ccb
 *
 * Description      This function allocates a new CCB.
 *
 * Returns          CCB address, or NULL if none available.
 *
 ******************************************************************************/
static tGAP_CCB* gap_allocate_ccb(void) {
  uint16_t xx;
  tGAP_CCB* p_ccb;

  /* Look through each connection control block for a free one */
  for (xx = 0, p_ccb = conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
    if (p_ccb->con_state == GAP_CCB_STATE_IDLE) {
      memset(p_ccb, 0, sizeof(tGAP_CCB));
      p_ccb->tx_queue = fixed_queue_new(SIZE_MAX);
      p_ccb->rx_queue = fixed_queue_new(SIZE_MAX);

      p_ccb->gap_handle = xx;
      p_ccb->rem_mtu_size = L2CAP_MTU_SIZE;

      return (p_ccb);
    }
  }

  /* If here, no free CCB found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         gap_release_ccb
 *
 * Description      This function releases a CCB.
 *
 * Returns          void
 *
 ******************************************************************************/
static void gap_release_ccb(tGAP_CCB* p_ccb) {
  /* Drop any buffers we may be holding */
  p_ccb->rx_queue_size = 0;

  while (!fixed_queue_is_empty(p_ccb->rx_queue))
    osi_free(fixed_queue_try_dequeue(p_ccb->rx_queue));
  fixed_queue_free(p_ccb->rx_queue, NULL);
  p_ccb->rx_queue = NULL;

  while (!fixed_queue_is_empty(p_ccb->tx_queue))
    osi_free(fixed_queue_try_dequeue(p_ccb->tx_queue));
  fixed_queue_free(p_ccb->tx_queue, NULL);
  p_ccb->tx_queue = NULL;

  p_ccb->con_state = GAP_CCB_STATE_IDLE;

  /* If no-one else is using the PSM, deregister from L2CAP */
  tGAP_CCB* p_ccb_local = conn.ccb_pool;
  for (uint16_t i = 0; i < GAP_MAX_CONNECTIONS; i++, p_ccb_local++) {
    if ((p_ccb_local->con_state != GAP_CCB_STATE_IDLE) &&
        (p_ccb_local->psm == p_ccb->psm)) {
      DVLOG(1) << __func__ << " : " << +p_ccb_local->psm
               << " PSM is still in use, do not deregister";
      return;
    }
  }

  /* Free the security record for this PSM */
  BTM_SecClrService(p_ccb->service_id);
  if (p_ccb->transport == BT_TRANSPORT_BR_EDR) L2CA_DEREGISTER(p_ccb->psm);
  if (p_ccb->transport == BT_TRANSPORT_LE) L2CA_DEREGISTER_COC(p_ccb->psm);
}

extern void gap_attr_db_init(void);

/*
 * This routine should not be called except once per stack invocation.
 */
void GAP_Init(void) {
  gap_conn_init();
  gap_attr_db_init();
}
