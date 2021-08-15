/******************************************************************************
 *
 *  Copyright 2003-2016 Broadcom Corporation
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

/*****************************************************************************
 *
 *  Name:           avct_bcb_act.cc
 *
 *  Description:    This module contains action functions of the browsing
 *                  control state machine.
 *
 *****************************************************************************/

#include <log/log.h>
#include <string.h>
#include "avct_api.h"
#include "avct_int.h"
#include "bt_target.h"
#include "bt_utils.h"
#include "btm_api.h"
#include "osi/include/osi.h"

/* action function list */
const tAVCT_BCB_ACTION avct_bcb_action[] = {
    avct_bcb_chnl_open,   /* AVCT_LCB_CHNL_OPEN */
    avct_bcb_chnl_disc,   /* AVCT_LCB_CHNL_DISC */
    avct_bcb_send_msg,    /* AVCT_LCB_SEND_MSG */
    avct_bcb_open_ind,    /* AVCT_LCB_OPEN_IND */
    avct_bcb_open_fail,   /* AVCT_LCB_OPEN_FAIL */
    avct_bcb_close_ind,   /* AVCT_LCB_CLOSE_IND */
    avct_bcb_close_cfm,   /* AVCT_LCB_CLOSE_CFM */
    avct_bcb_msg_ind,     /* AVCT_LCB_MSG_IND */
    avct_bcb_cong_ind,    /* AVCT_LCB_CONG_IND */
    avct_bcb_bind_conn,   /* AVCT_LCB_BIND_CONN */
    avct_bcb_bind_fail,   /* AVCT_LCB_BIND_FAIL */
    avct_bcb_unbind_disc, /* AVCT_LCB_UNBIND_DISC */
    avct_bcb_chk_disc,    /* AVCT_LCB_CHK_DISC */
    avct_bcb_discard_msg, /* AVCT_LCB_DISCARD_MSG */
    avct_bcb_dealloc,     /* AVCT_LCB_DEALLOC */
    avct_bcb_free_msg_ind /* AVCT_LCB_FREE_MSG_IND */
};

/*******************************************************************************
 *
 * Function         avct_bcb_msg_asmbl
 *
 * Description      Reassemble incoming message.
 *
 *
 * Returns          Pointer to reassembled message;  NULL if no message
 *                  available.
 *
 ******************************************************************************/
static BT_HDR* avct_bcb_msg_asmbl(UNUSED_ATTR tAVCT_BCB* p_bcb, BT_HDR* p_buf) {
  uint8_t* p;
  uint8_t pkt_type;

  if (p_buf->len == 0) {
    osi_free_and_reset((void**)&p_buf);
    android_errorWriteLog(0x534e4554, "79944113");
    return nullptr;
  }

  /* parse the message header */
  p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  pkt_type = AVCT_PKT_TYPE(p);

  /* must be single packet - can not fragment */
  if (pkt_type != AVCT_PKT_TYPE_SINGLE) {
    osi_free_and_reset((void**)&p_buf);
    AVCT_TRACE_WARNING("Pkt type=%d - fragmentation not allowed. drop it",
                       pkt_type);
  }
  return p_buf;
}

/*******************************************************************************
 *
 * Function         avct_bcb_chnl_open
 *
 * Description      Open L2CAP channel to peer
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_chnl_open(tAVCT_BCB* p_bcb, UNUSED_ATTR tAVCT_LCB_EVT* p_data) {
  uint16_t result = AVCT_RESULT_FAIL;
  tAVCT_LCB* p_lcb = avct_lcb_by_bcb(p_bcb);
  tL2CAP_ERTM_INFO ertm_info;

  BTM_SetOutService(p_lcb->peer_addr, BTM_SEC_SERVICE_AVCTP_BROWSE, 0);

  /* Set the FCR options: Browsing channel mandates ERTM */
  ertm_info.preferred_mode = avct_l2c_br_fcr_opts_def.mode;
  ertm_info.allowed_modes = L2CAP_FCR_CHAN_OPT_ERTM;
  ertm_info.user_rx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.user_tx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.fcr_rx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.fcr_tx_buf_size = BT_DEFAULT_BUFFER_SIZE;

  /* call l2cap connect req */
  p_bcb->ch_state = AVCT_CH_CONN;
  p_bcb->ch_lcid =
      L2CA_ErtmConnectReq(AVCT_BR_PSM, p_lcb->peer_addr, &ertm_info);
  if (p_bcb->ch_lcid == 0) {
    /* if connect req failed, send ourselves close event */
    tAVCT_LCB_EVT avct_lcb_evt;
    avct_lcb_evt.result = result;
    avct_bcb_event(p_bcb, AVCT_LCB_LL_CLOSE_EVT, &avct_lcb_evt);
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_unbind_disc
 *
 * Description      call callback with disconnect event.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_unbind_disc(UNUSED_ATTR tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  p_data->p_ccb->p_bcb = NULL;
  (*p_data->p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_data->p_ccb),
                                    AVCT_BROWSE_DISCONN_CFM_EVT, 0, NULL);
}

/*******************************************************************************
 *
 * Function         avct_bcb_open_ind
 *
 * Description      Handle an LL_OPEN event.
 *                  For the allocated ccb already bound to the bcb, send a
 *                  connect event. For the unbound ccb with a new PID, bind that
 *                  ccb to the bcb with the same bd_addr and send a connect
 *                  event.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_open_ind(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];
  tAVCT_CCB* p_ccb_bind = NULL;
  bool bind = false;
  tAVCT_UL_MSG ul_msg;

  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    /* if ccb allocated and */
    if (p_ccb->allocated) {
      /* if bound to this bcb send connect confirm event */
      if (p_ccb->p_bcb == p_bcb) {
        bind = true;
        p_ccb_bind = p_ccb;
        p_ccb->cc.p_ctrl_cback(avct_ccb_to_idx(p_ccb), AVCT_BROWSE_CONN_CFM_EVT,
                               0, &p_ccb->p_lcb->peer_addr);
      }
      /* if unbound acceptor and lcb allocated and bd_addr are the same for bcb
         and lcb */
      else if ((p_ccb->p_bcb == NULL) && (p_ccb->cc.role == AVCT_ACP) &&
               (p_ccb->p_lcb != NULL) &&
               p_bcb->peer_addr == p_ccb->p_lcb->peer_addr) {
        /* bind bcb to ccb and send connect ind event */
        bind = true;
        p_ccb_bind = p_ccb;
        p_ccb->p_bcb = p_bcb;
        p_ccb->cc.p_ctrl_cback(avct_ccb_to_idx(p_ccb), AVCT_BROWSE_CONN_IND_EVT,
                               0, &p_ccb->p_lcb->peer_addr);
      }
    }
  }

  /* if no ccbs bound to this lcb, disconnect */
  if (!bind) {
    avct_bcb_event(p_bcb, AVCT_LCB_INT_CLOSE_EVT, p_data);
    return;
  }

  if (!p_bcb->p_tx_msg || !p_ccb_bind) {
    return;
  }

  ul_msg.p_buf = p_bcb->p_tx_msg;
  ul_msg.p_ccb = p_ccb_bind;
  ul_msg.label = (uint8_t)(p_bcb->p_tx_msg->layer_specific & 0xFF);
  ul_msg.cr = (uint8_t)((p_bcb->p_tx_msg->layer_specific & 0xFF00) >> 8);
  p_bcb->p_tx_msg->layer_specific = AVCT_DATA_BROWSE;
  p_bcb->p_tx_msg = NULL;

  /* send msg event to bcb */
  tAVCT_LCB_EVT avct_lcb_evt;
  avct_lcb_evt.ul_msg = ul_msg;
  avct_bcb_event(p_bcb, AVCT_LCB_UL_MSG_EVT, &avct_lcb_evt);
}

/*******************************************************************************
 *
 * Function         avct_bcb_open_fail
 *
 * Description      L2CAP channel open attempt failed.  Mark the ccbs
 *                  as NULL bcb.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_open_fail(tAVCT_BCB* p_bcb, UNUSED_ATTR tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];

  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    if (p_ccb->allocated && (p_ccb->p_bcb == p_bcb)) {
      p_ccb->p_bcb = NULL;
    }
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_close_ind
 *
 * Description      L2CAP channel closed by peer.  Deallocate any initiator
 *                  ccbs on this lcb and send disconnect ind event.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_close_ind(tAVCT_BCB* p_bcb, UNUSED_ATTR tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];
  tAVCT_LCB* p_lcb = avct_lcb_by_bcb(p_bcb);

  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    if (p_ccb->allocated && (p_ccb->p_bcb == p_bcb)) {
      if (p_ccb->cc.role == AVCT_INT) {
        (*p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_ccb),
                                  AVCT_BROWSE_DISCONN_CFM_EVT, 0,
                                  &p_lcb->peer_addr);
      } else {
        (*p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_ccb),
                                  AVCT_BROWSE_DISCONN_IND_EVT, 0, NULL);
      }
      p_ccb->p_bcb = NULL;
    }
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_close_cfm
 *
 * Description      L2CAP channel closed by us.  Deallocate any initiator
 *                  ccbs on this lcb and send disconnect ind or cfm event.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_close_cfm(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];
  uint8_t event = 0;
  /* Whether BCB initiated channel close */
  bool ch_close = p_bcb->ch_close;
  tAVCT_CTRL_CBACK* p_cback;

  p_bcb->ch_close = false;
  p_bcb->allocated = 0;
  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    if (p_ccb->allocated && (p_ccb->p_bcb == p_bcb)) {
      /* if this ccb initiated close send disconnect cfm otherwise ind */
      if (ch_close) {
        event = AVCT_BROWSE_DISCONN_CFM_EVT;
      } else {
        event = AVCT_BROWSE_DISCONN_IND_EVT;
      }

      p_cback = p_ccb->cc.p_ctrl_cback;
      p_ccb->p_bcb = NULL;
      if (p_ccb->p_lcb == NULL) avct_ccb_dealloc(p_ccb, AVCT_NO_EVT, 0, NULL);
      (*p_cback)(avct_ccb_to_idx(p_ccb), event, p_data->result,
                 &p_bcb->peer_addr);
    }
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_bind_conn
 *
 * Description      Bind ccb to lcb and send connect cfm event.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_bind_conn(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  tAVCT_LCB* p_lcb = avct_lcb_by_bcb(p_bcb);
  p_data->p_ccb->p_bcb = p_bcb;
  (*p_data->p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_data->p_ccb),
                                    AVCT_BROWSE_CONN_CFM_EVT, 0,
                                    &p_lcb->peer_addr);
}

/*******************************************************************************
 *
 * Function         avct_bcb_chk_disc
 *
 * Description      A ccb wants to close; if it is the last ccb on this lcb,
 *                  close channel.  Otherwise just deallocate and call
 *                  callback.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_chk_disc(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  p_bcb->ch_close = avct_bcb_get_last_ccb_index(p_bcb, p_data->p_ccb);
  if (p_bcb->ch_close) {
    avct_bcb_event(p_bcb, AVCT_LCB_INT_CLOSE_EVT, p_data);
    return;
  }

  avct_bcb_unbind_disc(p_bcb, p_data);
}

/*******************************************************************************
 *
 * Function         avct_bcb_chnl_disc
 *
 * Description      Disconnect L2CAP channel.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_chnl_disc(tAVCT_BCB* p_bcb, UNUSED_ATTR tAVCT_LCB_EVT* p_data) {
  L2CA_DisconnectReq(p_bcb->ch_lcid);
}

/*******************************************************************************
 *
 * Function         avct_bcb_bind_fail
 *
 * Description      Deallocate ccb and call callback with connect event
 *                  with failure result.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_bind_fail(UNUSED_ATTR tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  p_data->p_ccb->p_bcb = NULL;
  (*p_data->p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_data->p_ccb),
                                    AVCT_BROWSE_CONN_CFM_EVT, AVCT_RESULT_FAIL,
                                    NULL);
}

/*******************************************************************************
 *
 * Function         avct_bcb_cong_ind
 *
 * Description      Handle congestion indication from L2CAP.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_cong_ind(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];
  uint8_t event;
  tAVCT_LCB* p_lcb = avct_lcb_by_bcb(p_bcb);

  /* set event */
  event =
      (p_data->cong) ? AVCT_BROWSE_CONG_IND_EVT : AVCT_BROWSE_UNCONG_IND_EVT;

  /* send event to all ccbs on this lcb */
  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    if (p_ccb->allocated && (p_ccb->p_bcb == p_bcb)) {
      (*p_ccb->cc.p_ctrl_cback)(avct_ccb_to_idx(p_ccb), event, 0,
                                &p_lcb->peer_addr);
    }
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_discard_msg
 *
 * Description      Discard a message sent in from the API.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_discard_msg(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  osi_free_and_reset((void**)&p_bcb->p_tx_msg);

  /* if control channel is up, save the message and open the browsing channel */
  if (p_data->ul_msg.p_ccb->p_lcb == NULL) {
    osi_free_and_reset((void**)&p_data->ul_msg.p_buf);
    return;
  }
  p_bcb->p_tx_msg = p_data->ul_msg.p_buf;

  if (p_bcb->p_tx_msg) {
    p_bcb->p_tx_msg->layer_specific =
        (p_data->ul_msg.cr << 8) + p_data->ul_msg.label;

    /* the channel is closed, opening or closing - open it again */
    AVCT_TRACE_DEBUG("ch_state: %d, allocated:%d->%d", p_bcb->ch_state,
                     p_bcb->allocated, p_data->ul_msg.p_ccb->p_lcb->allocated);
    p_bcb->allocated = p_data->ul_msg.p_ccb->p_lcb->allocated;
    avct_bcb_event(p_bcb, AVCT_LCB_UL_BIND_EVT,
                   (tAVCT_LCB_EVT*)p_data->ul_msg.p_ccb);
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_send_msg
 *
 * Description      Build and send an AVCTP message.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_send_msg(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  uint16_t curr_msg_len;
  uint8_t pkt_type = AVCT_PKT_TYPE_SINGLE;
  uint8_t hdr_len;
  BT_HDR* p_buf;
  uint8_t* p;

  /* store msg len */
  curr_msg_len = p_data->ul_msg.p_buf->len;

  /* initialize packet type and other stuff */
  if (curr_msg_len > (p_bcb->peer_mtu - AVCT_HDR_LEN_SINGLE)) {
    AVCT_TRACE_ERROR("%s msg len (%d) exceeds peer mtu(%d-%d)!!", __func__,
                     curr_msg_len, p_bcb->peer_mtu, AVCT_HDR_LEN_SINGLE);
    osi_free_and_reset((void**)&p_data->ul_msg.p_buf);
    return;
  }

  /* set header len */
  hdr_len = avct_lcb_pkt_type_len[pkt_type];
  p_buf = p_data->ul_msg.p_buf;

  /* set up to build header */
  p_buf->len += hdr_len;
  p_buf->offset -= hdr_len;
  p = (uint8_t*)(p_buf + 1) + p_buf->offset;

  /* build header */
  AVCT_BUILD_HDR(p, p_data->ul_msg.label, pkt_type, p_data->ul_msg.cr);
  UINT16_TO_BE_STREAM(p, p_data->ul_msg.p_ccb->cc.pid);

  p_buf->layer_specific = AVCT_DATA_BROWSE;

  /* send message to L2CAP */
  L2CA_DataWrite(p_bcb->ch_lcid, p_buf);
}

/*******************************************************************************
 *
 * Function         avct_bcb_free_msg_ind
 *
 * Description      Discard an incoming AVCTP message.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_free_msg_ind(UNUSED_ATTR tAVCT_BCB* p_bcb,
                           tAVCT_LCB_EVT* p_data) {
  if (p_data) osi_free_and_reset((void**)&p_data->p_buf);
}

/*******************************************************************************
 *
 * Function         avct_bcb_msg_ind
 *
 * Description      Handle an incoming AVCTP message.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_bcb_msg_ind(tAVCT_BCB* p_bcb, tAVCT_LCB_EVT* p_data) {
  uint8_t* p;
  uint8_t label, type, cr_ipid;
  uint16_t pid;
  tAVCT_CCB* p_ccb;
  tAVCT_LCB* p_lcb = avct_lcb_by_bcb(p_bcb);

  if ((p_data == NULL) || (p_data->p_buf == NULL)) {
    AVCT_TRACE_WARNING("%s p_data is NULL, returning!", __func__);
    return;
  }

  /* this p_buf is to be reported through p_msg_cback. The layer_specific
   * needs to be set properly to indicate that it is received through
   * browsing channel */
  p_data->p_buf->layer_specific = AVCT_DATA_BROWSE;

  /* reassemble message; if no message available (we received a fragment) return
   */
  p_data->p_buf = avct_bcb_msg_asmbl(p_bcb, p_data->p_buf);
  if (p_data->p_buf == NULL) {
    return;
  }

  if (p_data->p_buf->len < AVCT_HDR_LEN_SINGLE) {
    AVCT_TRACE_WARNING("Invalid AVCTP packet length %d: must be at least %d",
                       p_data->p_buf->len, AVCT_HDR_LEN_SINGLE);
    osi_free_and_reset((void**)&p_data->p_buf);
    android_errorWriteLog(0x534e4554, "79944113");
    return;
  }

  p = (uint8_t*)(p_data->p_buf + 1) + p_data->p_buf->offset;

  /* parse header byte */
  AVCT_PARSE_HDR(p, label, type, cr_ipid);

  /* check for invalid cr_ipid */
  if (cr_ipid == AVCT_CR_IPID_INVALID) {
    AVCT_TRACE_WARNING("Invalid cr_ipid", cr_ipid);
    osi_free_and_reset((void**)&p_data->p_buf);
    return;
  }

  /* parse and lookup PID */
  BE_STREAM_TO_UINT16(pid, p);
  p_ccb = avct_lcb_has_pid(p_lcb, pid);
  if (p_ccb) {
    /* PID found; send msg up, adjust bt hdr and call msg callback */
    p_data->p_buf->offset += AVCT_HDR_LEN_SINGLE;
    p_data->p_buf->len -= AVCT_HDR_LEN_SINGLE;
    (*p_ccb->cc.p_msg_cback)(avct_ccb_to_idx(p_ccb), label, cr_ipid,
                             p_data->p_buf);
    return;
  }

  /* PID not found; drop message */
  AVCT_TRACE_WARNING("No ccb for PID=%x", pid);
  osi_free_and_reset((void**)&p_data->p_buf);

  /* if command send reject */
  if (cr_ipid == AVCT_CMD) {
    BT_HDR* p_buf = (BT_HDR*)osi_malloc(AVRC_CMD_BUF_SIZE);
    p_buf->len = AVCT_HDR_LEN_SINGLE;
    p_buf->offset = AVCT_MSG_OFFSET - AVCT_HDR_LEN_SINGLE;
    p = (uint8_t*)(p_buf + 1) + p_buf->offset;
    AVCT_BUILD_HDR(p, label, AVCT_PKT_TYPE_SINGLE, AVCT_REJ);
    UINT16_TO_BE_STREAM(p, pid);
    p_buf->layer_specific = AVCT_DATA_BROWSE;
    L2CA_DataWrite(p_bcb->ch_lcid, p_buf);
  }
}

/*******************************************************************************
 *
 * Function         avct_bcb_dealloc
 *
 * Description      Deallocate a browse control block.
 *
 *
 * Returns          void.
 *
 ******************************************************************************/
void avct_bcb_dealloc(tAVCT_BCB* p_bcb, UNUSED_ATTR tAVCT_LCB_EVT* p_data) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];

  AVCT_TRACE_DEBUG("%s %d", __func__, p_bcb->allocated);

  for (int idx = 0; idx < AVCT_NUM_CONN; idx++, p_ccb++) {
    /* if ccb allocated and */
    if ((p_ccb->allocated) && (p_ccb->p_bcb == p_bcb)) {
      p_ccb->p_bcb = NULL;
      AVCT_TRACE_DEBUG("%s used by ccb: %d", __func__, idx);
      break;
    }
  }

  /* the browsing channel is down. Check if we have pending messages */
  osi_free_and_reset((void**)&p_bcb->p_tx_msg);
  memset(p_bcb, 0, sizeof(tAVCT_BCB));
}

/*******************************************************************************
 *
 * Function         avct_close_bcb
 *
 * Description      this function is called right before LCB disconnects.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avct_close_bcb(tAVCT_LCB* p_lcb, tAVCT_LCB_EVT* p_data) {
  tAVCT_BCB* p_bcb = avct_bcb_by_lcb(p_lcb);
  if (p_bcb->allocated) {
    avct_bcb_event(p_bcb, AVCT_LCB_UL_UNBIND_EVT, p_data);
  }
}

/*******************************************************************************
 *
 * Function         avct_lcb_by_bcb
 *
 * Description      This lookup function finds the lcb for a bcb.
 *
 * Returns          pointer to the lcb.
 *
 ******************************************************************************/
tAVCT_LCB* avct_lcb_by_bcb(tAVCT_BCB* p_bcb) {
  return &avct_cb.lcb[p_bcb->allocated - 1];
}

/*******************************************************************************
 *
 * Function         avct_bcb_by_lcb
 *
 * Description      This lookup function finds the bcb for a lcb.
 *
 * Returns          pointer to the lcb.
 *
 ******************************************************************************/
tAVCT_BCB* avct_bcb_by_lcb(tAVCT_LCB* p_lcb) {
  return &avct_cb.bcb[p_lcb->allocated - 1];
}

/*******************************************************************************
 *
 * Function         avct_bcb_get_last_ccb_index
 *
 * Description      See if given ccb is only one on the bcb.
 *
 *
 * Returns          0, if ccb is last,  (ccb index + 1) otherwise.
 *
 ******************************************************************************/
uint8_t avct_bcb_get_last_ccb_index(tAVCT_BCB* p_bcb, tAVCT_CCB* p_ccb_last) {
  tAVCT_CCB* p_ccb = &avct_cb.ccb[0];
  uint8_t idx = 0;

  for (int i = 0; i < AVCT_NUM_CONN; i++, p_ccb++) {
    if (p_ccb->allocated && (p_ccb->p_bcb == p_bcb)) {
      if (p_ccb != p_ccb_last) return 0;
      idx = (uint8_t)(i + 1);
    }
  }
  return idx;
}

/*******************************************************************************
 *
 * Function         avct_bcb_by_lcid
 *
 * Description      Find the BCB associated with the L2CAP LCID
 *
 *
 * Returns          pointer to the lcb, or NULL if none found.
 *
 ******************************************************************************/
tAVCT_BCB* avct_bcb_by_lcid(uint16_t lcid) {
  tAVCT_BCB* p_bcb = &avct_cb.bcb[0];
  int idx;

  for (idx = 0; idx < AVCT_NUM_LINKS; idx++, p_bcb++) {
    if (p_bcb->allocated && (p_bcb->ch_lcid == lcid)) {
      return p_bcb;
    }
  }

  /* out of lcbs */
  AVCT_TRACE_WARNING("No bcb for lcid %x", lcid);
  return NULL;
}
