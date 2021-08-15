/******************************************************************************
 *
 *  Copyright 1999-2012 Broadcom Corporation
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
 *  This file contains L2CAP interface functions
 *
 ******************************************************************************/

#include <stddef.h>
#include "bt_target.h"

#include "bt_common.h"
#include "common/time_util.h"
#include "osi/include/osi.h"

#include "bt_utils.h"
#include "hci/include/btsnoop.h"
#include "l2c_api.h"
#include "l2cdefs.h"
#include "port_api.h"
#include "port_int.h"
#include "rfc_int.h"
#include "rfcdefs.h"

/*
 * Define Callback functions to be called by L2CAP
*/
static void RFCOMM_ConnectInd(const RawAddress& bd_addr, uint16_t lcid,
                              uint16_t psm, uint8_t id);
static void RFCOMM_ConnectCnf(uint16_t lcid, uint16_t err);
static void RFCOMM_ConfigInd(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg);
static void RFCOMM_ConfigCnf(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg);
static void RFCOMM_DisconnectInd(uint16_t lcid, bool is_clear);
static void RFCOMM_QoSViolationInd(UNUSED_ATTR const RawAddress& bd_addr);
static void RFCOMM_BufDataInd(uint16_t lcid, BT_HDR* p_buf);
static void RFCOMM_CongestionStatusInd(uint16_t lcid, bool is_congested);

/*******************************************************************************
 *
 * Function         rfcomm_l2cap_if_init
 *
 * Description      This function is called during the RFCOMM task startup
 *                  to register interface functions with L2CAP.
 *
 ******************************************************************************/
void rfcomm_l2cap_if_init(void) {
  tL2CAP_APPL_INFO* p_l2c = &rfc_cb.rfc.reg_info;

  p_l2c->pL2CA_ConnectInd_Cb = RFCOMM_ConnectInd;
  p_l2c->pL2CA_ConnectCfm_Cb = RFCOMM_ConnectCnf;
  p_l2c->pL2CA_ConnectPnd_Cb = NULL;
  p_l2c->pL2CA_ConfigInd_Cb = RFCOMM_ConfigInd;
  p_l2c->pL2CA_ConfigCfm_Cb = RFCOMM_ConfigCnf;
  p_l2c->pL2CA_DisconnectInd_Cb = RFCOMM_DisconnectInd;
  p_l2c->pL2CA_DisconnectCfm_Cb = NULL;
  p_l2c->pL2CA_QoSViolationInd_Cb = RFCOMM_QoSViolationInd;
  p_l2c->pL2CA_DataInd_Cb = RFCOMM_BufDataInd;
  p_l2c->pL2CA_CongestionStatus_Cb = RFCOMM_CongestionStatusInd;
  p_l2c->pL2CA_TxComplete_Cb = NULL;

  L2CA_Register(BT_PSM_RFCOMM, p_l2c, true /* enable_snoop */);
}

/*******************************************************************************
 *
 * Function         RFCOMM_ConnectInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_ConnectInd received.  Allocate multiplexer control
 *                  block and dispatch the event to it.
 *
 ******************************************************************************/
void RFCOMM_ConnectInd(const RawAddress& bd_addr, uint16_t lcid,
                       UNUSED_ATTR uint16_t psm, uint8_t id) {
  tRFC_MCB* p_mcb = rfc_alloc_multiplexer_channel(bd_addr, false);

  if ((p_mcb) && (p_mcb->state != RFC_MX_STATE_IDLE)) {
    /* if this is collision case */
    if ((p_mcb->is_initiator) && (p_mcb->state == RFC_MX_STATE_WAIT_CONN_CNF)) {
      p_mcb->pending_lcid = lcid;
      p_mcb->pending_id = id;

      /* wait random timeout (2 - 12) to resolve collision */
      /* if peer gives up then local device rejects incoming connection and
       * continues as initiator */
      /* if timeout, local device disconnects outgoing connection and continues
       * as acceptor */
      RFCOMM_TRACE_DEBUG(
          "RFCOMM_ConnectInd start timer for collision, initiator's "
          "LCID(0x%x), acceptor's LCID(0x%x)",
          p_mcb->lcid, p_mcb->pending_lcid);

      rfc_timer_start(
          p_mcb,
          (uint16_t)(bluetooth::common::time_get_os_boottime_ms() % 10 + 2));
      return;
    } else {
      /* we cannot accept connection request from peer at this state */
      /* don't update lcid */
      p_mcb = nullptr;
    }
  } else {
    /* store mcb even if null */
    rfc_save_lcid_mcb(p_mcb, lcid);
  }

  if (p_mcb == nullptr) {
    L2CA_ConnectRsp(bd_addr, id, lcid, L2CAP_CONN_NO_RESOURCES, 0);
    return;
  }
  p_mcb->lcid = lcid;

  rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, &id);
}

/*******************************************************************************
 *
 * Function         RFCOMM_ConnectCnf
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_ConnectCnf received.  Save L2CAP handle and dispatch
 *                  event to the FSM.
 *
 ******************************************************************************/
void RFCOMM_ConnectCnf(uint16_t lcid, uint16_t result) {
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);

  if (!p_mcb) {
    RFCOMM_TRACE_ERROR("RFCOMM_ConnectCnf LCID:0x%x", lcid);
    return;
  }

  if (p_mcb->pending_lcid) {
    /* if peer rejects our connect request but peer's connect request is pending
     */
    if (result != L2CAP_CONN_OK) {
      RFCOMM_TRACE_DEBUG(
          "RFCOMM_ConnectCnf retry as acceptor on pending LCID(0x%x)",
          p_mcb->pending_lcid);

      /* remove mcb from mapping table */
      rfc_save_lcid_mcb(NULL, p_mcb->lcid);

      p_mcb->lcid = p_mcb->pending_lcid;
      p_mcb->is_initiator = false;
      p_mcb->state = RFC_MX_STATE_IDLE;

      /* store mcb into mapping table */
      rfc_save_lcid_mcb(p_mcb, p_mcb->lcid);

      /* update direction bit */
      for (int i = 0; i < RFCOMM_MAX_DLCI; i += 2) {
        uint8_t handle = p_mcb->port_handles[i];
        if (handle != 0) {
          p_mcb->port_handles[i] = 0;
          p_mcb->port_handles[i + 1] = handle;
          rfc_cb.port.port[handle - 1].dlci += 1;
          RFCOMM_TRACE_DEBUG("RFCOMM MX, port_handle=%d, DLCI[%d->%d]", handle,
                             i, rfc_cb.port.port[handle - 1].dlci);
        }
      }

      rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, &(p_mcb->pending_id));
      return;
    } else {
      RFCOMM_TRACE_DEBUG("RFCOMM_ConnectCnf peer gave up pending LCID(0x%x)",
                         p_mcb->pending_lcid);

      /* Peer gave up his connection request, make sure cleaning up L2CAP
       * channel */
      L2CA_ConnectRsp(p_mcb->bd_addr, p_mcb->pending_id, p_mcb->pending_lcid,
                      L2CAP_CONN_NO_RESOURCES, 0);

      p_mcb->pending_lcid = 0;
    }
  }

  /* Save LCID to be used in all consecutive calls to L2CAP */
  p_mcb->lcid = lcid;

  rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_CNF, &result);
}

/*******************************************************************************
 *
 * Function         RFCOMM_ConfigInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_ConfigInd received.  Save parameters in the control
 *                  block and dispatch event to the FSM.
 *
 ******************************************************************************/
void RFCOMM_ConfigInd(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg) {
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);

  if (!p_mcb) {
    RFCOMM_TRACE_ERROR("RFCOMM_ConfigInd LCID:0x%x", lcid);
    return;
  }

  rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONF_IND, (void*)p_cfg);
}

/*******************************************************************************
 *
 * Function         RFCOMM_ConfigCnf
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_ConfigCnf received.  Save L2CAP handle and dispatch
 *                  event to the FSM.
 *
 ******************************************************************************/
void RFCOMM_ConfigCnf(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg) {
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);

  if (!p_mcb) {
    RFCOMM_TRACE_ERROR("RFCOMM_ConfigCnf no MCB LCID:0x%x", lcid);
    return;
  }

  rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONF_CNF, (void*)p_cfg);
}

/*******************************************************************************
 *
 * Function         RFCOMM_QoSViolationInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_QoSViolationIndInd received.  Dispatch event to the
 *                  FSM.
 *
 ******************************************************************************/
void RFCOMM_QoSViolationInd(UNUSED_ATTR const RawAddress& bd_addr) {}

/*******************************************************************************
 *
 * Function         RFCOMM_DisconnectInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  L2CA_DisconnectInd received.  Dispatch event to the FSM.
 *
 ******************************************************************************/
void RFCOMM_DisconnectInd(uint16_t lcid, bool is_conf_needed) {
  VLOG(1) << __func__ << ": lcid=" << loghex(lcid)
          << ", is_conf_needed=" << is_conf_needed;
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);
  if (is_conf_needed) {
    L2CA_DisconnectRsp(lcid);
  }
  if (!p_mcb) {
    LOG(WARNING) << __func__ << ": no mcb for lcid " << loghex(lcid);
    return;
  }
  rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_DISC_IND, nullptr);
}

/*******************************************************************************
 *
 * Function         RFCOMM_BufDataInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  data RFCOMM frame is received.  Parse the frames, check
 *                  the checksum and dispatch event to multiplexer or port
 *                  state machine depending on the frame destination.
 *
 ******************************************************************************/
void RFCOMM_BufDataInd(uint16_t lcid, BT_HDR* p_buf) {
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);

  if (!p_mcb) {
    LOG(WARNING) << __func__ << ": Cannot find RFCOMM multiplexer for lcid "
                 << loghex(lcid);
    osi_free(p_buf);
    return;
  }

  uint8_t event = rfc_parse_data(p_mcb, &rfc_cb.rfc.rx_frame, p_buf);

  /* If the frame did not pass validation just ignore it */
  if (event == RFC_EVENT_BAD_FRAME) {
    LOG(WARNING) << __func__ << ": Bad RFCOMM frame from lcid=" << loghex(lcid)
                 << ", bd_addr=" << p_mcb->bd_addr << ", p_mcb=" << p_mcb;
    osi_free(p_buf);
    return;
  }

  if (rfc_cb.rfc.rx_frame.dlci == RFCOMM_MX_DLCI) {
    RFCOMM_TRACE_DEBUG("%s: handle multiplexer event %d, p_mcb=%p", __func__,
                       event, p_mcb);
    /* Take special care of the Multiplexer Control Messages */
    if (event == RFC_EVENT_UIH) {
      rfc_process_mx_message(p_mcb, p_buf);
      return;
    }

    /* Other multiplexer events go to state machine */
    rfc_mx_sm_execute(p_mcb, event, nullptr);
    osi_free(p_buf);
    return;
  }

  /* The frame was received on the data channel DLCI, verify that DLC exists */
  tPORT* p_port = port_find_mcb_dlci_port(p_mcb, rfc_cb.rfc.rx_frame.dlci);
  if (p_port == nullptr || !p_port->rfc.p_mcb) {
    /* If this is a SABME on new port, check if any app is waiting for it */
    if (event != RFC_EVENT_SABME) {
      LOG(WARNING) << __func__
                   << ": no for none-SABME event, lcid=" << loghex(lcid)
                   << ", bd_addr=" << p_mcb->bd_addr << ", p_mcb=" << p_mcb;
      if ((p_mcb->is_initiator && !rfc_cb.rfc.rx_frame.cr) ||
          (!p_mcb->is_initiator && rfc_cb.rfc.rx_frame.cr)) {
        LOG(ERROR) << __func__
                   << ": Disconnecting RFCOMM, lcid=" << loghex(lcid)
                   << ", bd_addr=" << p_mcb->bd_addr << ", p_mcb=" << p_mcb;
        rfc_send_dm(p_mcb, rfc_cb.rfc.rx_frame.dlci, rfc_cb.rfc.rx_frame.pf);
      }
      osi_free(p_buf);
      return;
    }

    p_port = port_find_dlci_port(rfc_cb.rfc.rx_frame.dlci);
    if (p_port == nullptr) {
      LOG(ERROR) << __func__ << ":Disconnecting RFCOMM, no port for dlci "
                 << +rfc_cb.rfc.rx_frame.dlci << ", lcid=" << loghex(lcid)
                 << ", bd_addr=" << p_mcb->bd_addr << ", p_mcb=" << p_mcb;
      rfc_send_dm(p_mcb, rfc_cb.rfc.rx_frame.dlci, true);
      osi_free(p_buf);
      return;
    }
    RFCOMM_TRACE_DEBUG("%s: port_handles[dlci=%d]:%d->%d, p_mcb=%p", __func__,
                       rfc_cb.rfc.rx_frame.dlci,
                       p_mcb->port_handles[rfc_cb.rfc.rx_frame.dlci],
                       p_port->handle);
    p_mcb->port_handles[rfc_cb.rfc.rx_frame.dlci] = p_port->handle;
    p_port->rfc.p_mcb = p_mcb;
  }

  if (event == RFC_EVENT_UIH) {
    RFCOMM_TRACE_DEBUG("%s: Handling UIH event, buf_len=%u, credit=%u",
                       __func__, p_buf->len, rfc_cb.rfc.rx_frame.credit);
    if (p_buf->len > 0) {
      rfc_port_sm_execute(p_port, event, p_buf);
    } else {
      osi_free(p_buf);
    }

    if (rfc_cb.rfc.rx_frame.credit != 0) {
      rfc_inc_credit(p_port, rfc_cb.rfc.rx_frame.credit);
    }

    return;
  }
  rfc_port_sm_execute(p_port, event, nullptr);
  osi_free(p_buf);
}

/*******************************************************************************
 *
 * Function         RFCOMM_CongestionStatusInd
 *
 * Description      This is a callback function called by L2CAP when
 *                  data RFCOMM L2CAP congestion status changes
 *
 ******************************************************************************/
void RFCOMM_CongestionStatusInd(uint16_t lcid, bool is_congested) {
  tRFC_MCB* p_mcb = rfc_find_lcid_mcb(lcid);

  if (!p_mcb) {
    RFCOMM_TRACE_ERROR("RFCOMM_CongestionStatusInd dropped LCID:0x%x", lcid);
    return;
  } else {
    RFCOMM_TRACE_EVENT("RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
  }
  rfc_process_l2cap_congestion(p_mcb, is_congested);
}

/*******************************************************************************
 *
 * Function         rfc_find_lcid_mcb
 *
 * Description      This function returns MCB block supporting local cid
 *
 ******************************************************************************/
tRFC_MCB* rfc_find_lcid_mcb(uint16_t lcid) {
  if (lcid - L2CAP_BASE_APPL_CID >= MAX_L2CAP_CHANNELS) {
    RFCOMM_TRACE_ERROR("rfc_find_lcid_mcb LCID:0x%x", lcid);
    return nullptr;
  } else {
    tRFC_MCB* p_mcb = rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID];
    if (p_mcb != nullptr) {
      if (p_mcb->lcid != lcid) {
        LOG(WARNING) << __func__ << "LCID reused lcid=:" << loghex(lcid)
                     << ", current_lcid=" << loghex(p_mcb->lcid);
        return nullptr;
      }
    }
    return p_mcb;
  }
}

/*******************************************************************************
 *
 * Function         rfc_save_lcid_mcb
 *
 * Description      This function returns MCB block supporting local cid
 *
 ******************************************************************************/
void rfc_save_lcid_mcb(tRFC_MCB* p_mcb, uint16_t lcid) {
  if (lcid < L2CAP_BASE_APPL_CID) {
    LOG(ERROR) << __func__ << ": LCID " << lcid << " is too small";
    return;
  }
  auto mcb_index = static_cast<size_t>(lcid - L2CAP_BASE_APPL_CID);
  if (mcb_index >= MAX_L2CAP_CHANNELS) {
    LOG(ERROR) << __func__ << ": LCID " << lcid << " is too large";
    return;
  }
  rfc_cb.rfc.p_rfc_lcid_mcb[mcb_index] = p_mcb;
}
