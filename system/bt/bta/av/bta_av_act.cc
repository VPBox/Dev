/******************************************************************************
 *
 *  Copyright 2004-2016 Broadcom Corporation
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
 *  This file contains action functions for advanced audio/video main state
 *  machine.
 *
 ******************************************************************************/

#define LOG_TAG "bt_bta_av"

#include "bt_target.h"

#include <base/logging.h>
#include <string.h>

#include "avdt_api.h"
#include "avrcp_service.h"
#include "bta_av_api.h"
#include "bta_av_int.h"
#include "l2c_api.h"
#include "log/log.h"
#include "osi/include/list.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"
#include "utl.h"

#if (BTA_AR_INCLUDED == TRUE)
#include "bta_ar_api.h"
#endif

/*****************************************************************************
 *  Constants
 ****************************************************************************/
/* the timeout to wait for open req after setconfig for incoming connections */
#ifndef BTA_AV_SIGNALLING_TIMEOUT_MS
#define BTA_AV_SIGNALLING_TIMEOUT_MS (8 * 1000) /* 8 seconds */
#endif

/* Time to wait for signalling from SNK when it is initiated from SNK. */
/* If not, we will start signalling from SRC. */
#ifndef BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS
#define BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS (2 * 1000) /* 2 seconds */
#endif

static void bta_av_accept_signalling_timer_cback(void* data);

#ifndef AVRC_MIN_META_CMD_LEN
#define AVRC_MIN_META_CMD_LEN 20
#endif

/*******************************************************************************
 *
 * Function         bta_av_get_rcb_by_shdl
 *
 * Description      find the RCB associated with the given SCB handle.
 *
 * Returns          tBTA_AV_RCB
 *
 ******************************************************************************/
tBTA_AV_RCB* bta_av_get_rcb_by_shdl(uint8_t shdl) {
  tBTA_AV_RCB* p_rcb = NULL;
  int i;

  for (i = 0; i < BTA_AV_NUM_RCB; i++) {
    if (bta_av_cb.rcb[i].shdl == shdl &&
        bta_av_cb.rcb[i].handle != BTA_AV_RC_HANDLE_NONE) {
      p_rcb = &bta_av_cb.rcb[i];
      break;
    }
  }
  return p_rcb;
}
#define BTA_AV_STS_NO_RSP 0xFF /* a number not used by tAVRC_STS */

/*******************************************************************************
 *
 * Function         bta_av_del_rc
 *
 * Description      delete the given AVRC handle.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_del_rc(tBTA_AV_RCB* p_rcb) {
  tBTA_AV_SCB* p_scb;
  uint8_t rc_handle; /* connected AVRCP handle */

  p_scb = NULL;
  if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
    if (p_rcb->shdl) {
      /* Validate array index*/
      if ((p_rcb->shdl - 1) < BTA_AV_NUM_STRS) {
        p_scb = bta_av_cb.p_scb[p_rcb->shdl - 1];
      }
      if (p_scb) {
        APPL_TRACE_DEBUG("%s: shdl:%d, srch:%d rc_handle:%d", __func__,
                         p_rcb->shdl, p_scb->rc_handle, p_rcb->handle);
        if (p_scb->rc_handle == p_rcb->handle)
          p_scb->rc_handle = BTA_AV_RC_HANDLE_NONE;
        /* just in case the RC timer is active
        if (bta_av_cb.features & BTA_AV_FEAT_RCCT && p_scb->chnl ==
        BTA_AV_CHNL_AUDIO) */
        alarm_cancel(p_scb->avrc_ct_timer);
      }
    }

    APPL_TRACE_EVENT("%s: handle: %d status=0x%x, rc_acp_handle:%d, idx:%d",
                     __func__, p_rcb->handle, p_rcb->status,
                     bta_av_cb.rc_acp_handle, bta_av_cb.rc_acp_idx);
    rc_handle = p_rcb->handle;
    if (!(p_rcb->status & BTA_AV_RC_CONN_MASK) ||
        ((p_rcb->status & BTA_AV_RC_ROLE_MASK) == BTA_AV_RC_ROLE_INT)) {
      p_rcb->status = 0;
      p_rcb->handle = BTA_AV_RC_HANDLE_NONE;
      p_rcb->shdl = 0;
      p_rcb->lidx = 0;
    }
    /* else ACP && connected. do not clear the handle yet */
    AVRC_Close(rc_handle);
    if (rc_handle == bta_av_cb.rc_acp_handle)
      bta_av_cb.rc_acp_handle = BTA_AV_RC_HANDLE_NONE;
    APPL_TRACE_EVENT(
        "%s: end del_rc handle: %d status=0x%x, rc_acp_handle:%d, lidx:%d",
        __func__, p_rcb->handle, p_rcb->status, bta_av_cb.rc_acp_handle,
        p_rcb->lidx);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_close_all_rc
 *
 * Description      close the all AVRC handle.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_close_all_rc(tBTA_AV_CB* p_cb) {
  int i;

  for (i = 0; i < BTA_AV_NUM_RCB; i++) {
    if ((p_cb->disabling) || (bta_av_cb.rcb[i].shdl != 0))
      bta_av_del_rc(&bta_av_cb.rcb[i]);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_del_sdp_rec
 *
 * Description      delete the given SDP record handle.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_del_sdp_rec(uint32_t* p_sdp_handle) {
  if (*p_sdp_handle != 0) {
    SDP_DeleteRecord(*p_sdp_handle);
    *p_sdp_handle = 0;
  }
}

/*******************************************************************************
 *
 * Function         bta_av_avrc_sdp_cback
 *
 * Description      AVRCP service discovery callback.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_avrc_sdp_cback(UNUSED_ATTR uint16_t status) {
  BT_HDR* p_msg = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_msg->event = BTA_AV_SDP_AVRC_DISC_EVT;

  bta_sys_sendmsg(p_msg);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_ctrl_cback
 *
 * Description      AVRCP control callback.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_rc_ctrl_cback(uint8_t handle, uint8_t event,
                                 UNUSED_ATTR uint16_t result,
                                 const RawAddress* peer_addr) {
  uint16_t msg_event = 0;

  APPL_TRACE_EVENT("%s: handle: %d event=0x%x", __func__, handle, event);
  if (event == AVRC_OPEN_IND_EVT) {
    /* save handle of opened connection
    bta_av_cb.rc_handle = handle;*/

    msg_event = BTA_AV_AVRC_OPEN_EVT;
  } else if (event == AVRC_CLOSE_IND_EVT) {
    msg_event = BTA_AV_AVRC_CLOSE_EVT;
  } else if (event == AVRC_BROWSE_OPEN_IND_EVT) {
    msg_event = BTA_AV_AVRC_BROWSE_OPEN_EVT;
  } else if (event == AVRC_BROWSE_CLOSE_IND_EVT) {
    msg_event = BTA_AV_AVRC_BROWSE_CLOSE_EVT;
  }

  if (msg_event) {
    tBTA_AV_RC_CONN_CHG* p_msg =
        (tBTA_AV_RC_CONN_CHG*)osi_malloc(sizeof(tBTA_AV_RC_CONN_CHG));
    p_msg->hdr.event = msg_event;
    p_msg->handle = handle;
    if (peer_addr) p_msg->peer_addr = *peer_addr;
    bta_sys_sendmsg(p_msg);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_msg_cback
 *
 * Description      AVRCP message callback.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_rc_msg_cback(uint8_t handle, uint8_t label, uint8_t opcode,
                                tAVRC_MSG* p_msg) {
  uint8_t* p_data_src = NULL;
  uint16_t data_len = 0;

  APPL_TRACE_DEBUG("%s: handle: %u opcode=0x%x", __func__, handle, opcode);

  /* Copy avrc packet into BTA message buffer (for sending to BTA state machine)
   */

  /* Get size of payload data  (for vendor and passthrough messages only; for
   * browsing
   * messages, use zero-copy) */
  if (opcode == AVRC_OP_VENDOR && p_msg->vendor.p_vendor_data != NULL) {
    p_data_src = p_msg->vendor.p_vendor_data;
    data_len = (uint16_t)p_msg->vendor.vendor_len;
  } else if (opcode == AVRC_OP_PASS_THRU && p_msg->pass.p_pass_data != NULL) {
    p_data_src = p_msg->pass.p_pass_data;
    data_len = (uint16_t)p_msg->pass.pass_len;
  }

  /* Create a copy of the message */
  tBTA_AV_RC_MSG* p_buf =
      (tBTA_AV_RC_MSG*)osi_malloc(sizeof(tBTA_AV_RC_MSG) + data_len);

  p_buf->hdr.event = BTA_AV_AVRC_MSG_EVT;
  p_buf->handle = handle;
  p_buf->label = label;
  p_buf->opcode = opcode;
  memcpy(&p_buf->msg, p_msg, sizeof(tAVRC_MSG));
  /* Copy the data payload, and set the pointer to it */
  if (p_data_src != NULL) {
    uint8_t* p_data_dst = (uint8_t*)(p_buf + 1);
    memcpy(p_data_dst, p_data_src, data_len);

    /* Update bta message buffer to point to payload data */
    /* (Note AVRC_OP_BROWSING uses zero-copy: p_buf->msg.browse.p_browse_data
     * already points to original avrc buffer) */
    if (opcode == AVRC_OP_VENDOR)
      p_buf->msg.vendor.p_vendor_data = p_data_dst;
    else if (opcode == AVRC_OP_PASS_THRU)
      p_buf->msg.pass.p_pass_data = p_data_dst;
  }

  if (opcode == AVRC_OP_BROWSE) {
    /* set p_pkt to NULL, so avrc would not free the buffer */
    p_msg->browse.p_browse_pkt = NULL;
  }

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_create
 *
 * Description      alloc RCB and call AVRC_Open
 *
 * Returns          the created rc handle
 *
 ******************************************************************************/
uint8_t bta_av_rc_create(tBTA_AV_CB* p_cb, uint8_t role, uint8_t shdl,
                         uint8_t lidx) {
  if (is_new_avrcp_enabled()) {
    APPL_TRACE_WARNING("%s: Skipping RC creation for the old AVRCP profile",
                       __func__);
    return BTA_AV_RC_HANDLE_NONE;
  }

  tAVRC_CONN_CB ccb;
  RawAddress bda = RawAddress::kAny;
  uint8_t status = BTA_AV_RC_ROLE_ACP;
  int i;
  uint8_t rc_handle;
  tBTA_AV_RCB* p_rcb;

  if (role == AVCT_INT) {
    // Can't grab a stream control block that doesn't have a valid handle
    if (!shdl) {
      APPL_TRACE_ERROR(
          "%s: Can't grab stream control block for shdl = %d -> index = %d",
          __func__, shdl, shdl - 1);
      return BTA_AV_RC_HANDLE_NONE;
    }
    tBTA_AV_SCB* p_scb = p_cb->p_scb[shdl - 1];
    bda = p_scb->PeerAddress();
    status = BTA_AV_RC_ROLE_INT;
  } else {
    p_rcb = bta_av_get_rcb_by_shdl(shdl);
    if (p_rcb != NULL) {
      APPL_TRACE_ERROR("%s: ACP handle exist for shdl:%d", __func__, shdl);
      p_rcb->lidx = lidx;
      return p_rcb->handle;
    }
  }

  ccb.ctrl_cback = base::Bind(bta_av_rc_ctrl_cback);
  ccb.msg_cback = base::Bind(bta_av_rc_msg_cback);
  ccb.company_id = p_bta_av_cfg->company_id;
  ccb.conn = role;
  /* note: BTA_AV_FEAT_RCTG = AVRC_CT_TARGET, BTA_AV_FEAT_RCCT = AVRC_CT_CONTROL
   */
  ccb.control = p_cb->features & (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_RCCT |
                                  BTA_AV_FEAT_METADATA | AVRC_CT_PASSIVE);

  if (AVRC_Open(&rc_handle, &ccb, bda) != AVRC_SUCCESS)
    return BTA_AV_RC_HANDLE_NONE;

  i = rc_handle;
  p_rcb = &p_cb->rcb[i];

  if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
    APPL_TRACE_ERROR("%s: found duplicated handle:%d", __func__, rc_handle);
  }

  p_rcb->handle = rc_handle;
  p_rcb->status = status;
  p_rcb->shdl = shdl;
  p_rcb->lidx = lidx;
  p_rcb->peer_features = 0;
  if (lidx == (BTA_AV_NUM_LINKS + 1)) {
    /* this LIDX is reserved for the AVRCP ACP connection */
    p_cb->rc_acp_handle = p_rcb->handle;
    p_cb->rc_acp_idx = (i + 1);
    APPL_TRACE_DEBUG("%s: rc_acp_handle:%d idx:%d", __func__,
                     p_cb->rc_acp_handle, p_cb->rc_acp_idx);
  }
  APPL_TRACE_DEBUG(
      "%s: create %d, role: %d, shdl:%d, rc_handle:%d, lidx:%d, status:0x%x",
      __func__, i, role, shdl, p_rcb->handle, lidx, p_rcb->status);

  return rc_handle;
}

/*******************************************************************************
 *
 * Function         bta_av_valid_group_navi_msg
 *
 * Description      Check if it is Group Navigation Msg for Metadata
 *
 * Returns          AVRC_RSP_ACCEPT or AVRC_RSP_NOT_IMPL
 *
 ******************************************************************************/
static tBTA_AV_CODE bta_av_group_navi_supported(uint8_t len, uint8_t* p_data,
                                                bool is_inquiry) {
  tBTA_AV_CODE ret = AVRC_RSP_NOT_IMPL;
  uint8_t* p_ptr = p_data;
  uint16_t u16;
  uint32_t u32;

  if (p_bta_av_cfg->avrc_group && len == BTA_GROUP_NAVI_MSG_OP_DATA_LEN) {
    BTA_AV_BE_STREAM_TO_CO_ID(u32, p_ptr);
    BE_STREAM_TO_UINT16(u16, p_ptr);

    if (u32 == AVRC_CO_METADATA) {
      if (is_inquiry) {
        if (u16 <= AVRC_PDU_PREV_GROUP) ret = AVRC_RSP_IMPL_STBL;
      } else {
        if (u16 <= AVRC_PDU_PREV_GROUP)
          ret = AVRC_RSP_ACCEPT;
        else
          ret = AVRC_RSP_REJ;
      }
    }
  }

  return ret;
}

/*******************************************************************************
 *
 * Function         bta_av_op_supported
 *
 * Description      Check if remote control operation is supported.
 *
 * Returns          AVRC_RSP_ACCEPT of supported, AVRC_RSP_NOT_IMPL if not.
 *
 ******************************************************************************/
static tBTA_AV_CODE bta_av_op_supported(tBTA_AV_RC rc_id, bool is_inquiry) {
  tBTA_AV_CODE ret_code = AVRC_RSP_NOT_IMPL;

  if (p_bta_av_rc_id) {
    if (is_inquiry) {
      if (p_bta_av_rc_id[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
        ret_code = AVRC_RSP_IMPL_STBL;
      }
    } else {
      if (p_bta_av_rc_id[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
        ret_code = AVRC_RSP_ACCEPT;
      } else if ((p_bta_av_cfg->rc_pass_rsp == AVRC_RSP_INTERIM) &&
                 p_bta_av_rc_id_ac) {
        if (p_bta_av_rc_id_ac[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
          ret_code = AVRC_RSP_INTERIM;
        }
      }
    }
  }
  return ret_code;
}

/*******************************************************************************
 *
 * Function         bta_av_find_lcb
 *
 * Description      Given BD_addr, find the associated LCB.
 *
 * Returns          NULL, if not found.
 *
 ******************************************************************************/
tBTA_AV_LCB* bta_av_find_lcb(const RawAddress& addr, uint8_t op) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  int xx;
  uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: address: %s op:%d", __func__, addr.ToString().c_str(),
                   op);
  for (xx = 0; xx < BTA_AV_NUM_LINKS; xx++) {
    mask = 1 << xx; /* the used mask for this lcb */
    if ((mask & p_cb->conn_lcb) && p_cb->lcb[xx].addr == addr) {
      p_lcb = &p_cb->lcb[xx];
      if (op == BTA_AV_LCB_FREE) {
        p_cb->conn_lcb &= ~mask; /* clear the connect mask */
        APPL_TRACE_DEBUG("%s: conn_lcb: 0x%x", __func__, p_cb->conn_lcb);
      }
      break;
    }
  }
  return p_lcb;
}

/*******************************************************************************
 *
 * Function         bta_av_rc_opened
 *
 * Description      Set AVRCP state to opened.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_opened(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RC_OPEN rc_open;
  tBTA_AV_SCB* p_scb;
  int i;
  uint8_t shdl = 0;
  tBTA_AV_LCB* p_lcb;
  tBTA_AV_RCB* p_rcb;
  uint8_t tmp;
  uint8_t disc = 0;

  /* find the SCB & stop the timer */
  for (i = 0; i < BTA_AV_NUM_STRS; i++) {
    p_scb = p_cb->p_scb[i];
    if (p_scb && p_scb->PeerAddress() == p_data->rc_conn_chg.peer_addr) {
      p_scb->rc_handle = p_data->rc_conn_chg.handle;
      APPL_TRACE_DEBUG("%s: shdl:%d, srch %d", __func__, i + 1,
                       p_scb->rc_handle);
      shdl = i + 1;
      LOG_INFO(LOG_TAG, "%s: allow incoming AVRCP connections:%d", __func__,
               p_scb->use_rc);
      alarm_cancel(p_scb->avrc_ct_timer);
      disc = p_scb->hndl;
      break;
    }
  }

  i = p_data->rc_conn_chg.handle;
  if (p_cb->rcb[i].handle == BTA_AV_RC_HANDLE_NONE) {
    APPL_TRACE_ERROR("%s: not a valid handle:%d any more", __func__, i);
    return;
  }

  APPL_TRACE_DEBUG("%s: local features %d peer features %d", __func__,
                   p_cb->features, p_cb->rcb[i].peer_features);

  /* listen to browsing channel when the connection is open,
   * if peer initiated AVRCP connection and local device supports browsing
   * channel */
  AVRC_OpenBrowse(p_data->rc_conn_chg.handle, AVCT_ACP);

  if (p_cb->rcb[i].lidx == (BTA_AV_NUM_LINKS + 1) && shdl != 0) {
    /* rc is opened on the RC only ACP channel, but is for a specific
     * SCB -> need to switch RCBs */
    p_rcb = bta_av_get_rcb_by_shdl(shdl);
    if (p_rcb) {
      p_rcb->shdl = p_cb->rcb[i].shdl;
      tmp = p_rcb->lidx;
      p_rcb->lidx = p_cb->rcb[i].lidx;
      p_cb->rcb[i].lidx = tmp;
      p_cb->rc_acp_handle = p_rcb->handle;
      p_cb->rc_acp_idx = (p_rcb - p_cb->rcb) + 1;
      APPL_TRACE_DEBUG("%s: switching RCB rc_acp_handle:%d idx:%d", __func__,
                       p_cb->rc_acp_handle, p_cb->rc_acp_idx);
    }
  }

  p_cb->rcb[i].shdl = shdl;
  rc_open.rc_handle = i;
  APPL_TRACE_ERROR("%s: rcb[%d] shdl:%d lidx:%d/%d", __func__, i, shdl,
                   p_cb->rcb[i].lidx, p_cb->lcb[BTA_AV_NUM_LINKS].lidx);
  p_cb->rcb[i].status |= BTA_AV_RC_CONN_MASK;

  if (!shdl && 0 == p_cb->lcb[BTA_AV_NUM_LINKS].lidx) {
    /* no associated SCB -> connected to an RC only device
     * update the index to the extra LCB */
    p_lcb = &p_cb->lcb[BTA_AV_NUM_LINKS];
    p_lcb->addr = p_data->rc_conn_chg.peer_addr;
    p_lcb->lidx = BTA_AV_NUM_LINKS + 1;
    p_cb->rcb[i].lidx = p_lcb->lidx;
    p_lcb->conn_msk = 1;
    APPL_TRACE_ERROR("%s: bd_addr: %s rcb[%d].lidx=%d, lcb.conn_msk=x%x",
                     __func__, p_lcb->addr.ToString().c_str(), i,
                     p_cb->rcb[i].lidx, p_lcb->conn_msk);
    disc = p_data->rc_conn_chg.handle | BTA_AV_CHNL_MSK;
  }

  rc_open.peer_addr = p_data->rc_conn_chg.peer_addr;
  rc_open.peer_features = p_cb->rcb[i].peer_features;
  rc_open.status = BTA_AV_SUCCESS;
  APPL_TRACE_DEBUG("%s: local features:x%x peer_features:x%x", __func__,
                   p_cb->features, rc_open.peer_features);
  if (rc_open.peer_features == 0) {
    /* we have not done SDP on peer RC capabilities.
     * peer must have initiated the RC connection */
    if (p_cb->features & BTA_AV_FEAT_RCCT)
      rc_open.peer_features |= BTA_AV_FEAT_RCTG;
    if (p_cb->features & BTA_AV_FEAT_RCTG)
      rc_open.peer_features |= BTA_AV_FEAT_RCCT;

    bta_av_rc_disc(disc);
  }
  tBTA_AV bta_av_data;
  bta_av_data.rc_open = rc_open;
  (*p_cb->p_cback)(BTA_AV_RC_OPEN_EVT, &bta_av_data);

  /* if local initiated AVRCP connection and both peer and locals device support
   * browsing channel, open the browsing channel now
   * TODO (sanketa): Some TG would not broadcast browse feature hence check
   * inter-op. */
  if ((p_cb->features & BTA_AV_FEAT_BROWSE) &&
      (rc_open.peer_features & BTA_AV_FEAT_BROWSE) &&
      ((p_cb->rcb[i].status & BTA_AV_RC_ROLE_MASK) == BTA_AV_RC_ROLE_INT)) {
    APPL_TRACE_DEBUG("%s: opening AVRC Browse channel", __func__);
    AVRC_OpenBrowse(p_data->rc_conn_chg.handle, AVCT_INT);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_remote_cmd
 *
 * Description      Send an AVRCP remote control command.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_remote_cmd(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
  if (p_cb->features & BTA_AV_FEAT_RCCT) {
    if (p_data->hdr.layer_specific < BTA_AV_NUM_RCB) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
      if (p_rcb->status & BTA_AV_RC_CONN_MASK) {
        AVRC_PassCmd(p_rcb->handle, p_data->api_remote_cmd.label,
                     &p_data->api_remote_cmd.msg);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_vendor_cmd
 *
 * Description      Send an AVRCP vendor specific command.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_vendor_cmd(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
  if ((p_cb->features & (BTA_AV_FEAT_RCCT | BTA_AV_FEAT_VENDOR)) ==
      (BTA_AV_FEAT_RCCT | BTA_AV_FEAT_VENDOR)) {
    if (p_data->hdr.layer_specific < BTA_AV_NUM_RCB) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
      AVRC_VendorCmd(p_rcb->handle, p_data->api_vendor.label,
                     &p_data->api_vendor.msg);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_vendor_rsp
 *
 * Description      Send an AVRCP vendor specific response.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_vendor_rsp(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
  if ((p_cb->features & (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_VENDOR)) ==
      (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_VENDOR)) {
    if (p_data->hdr.layer_specific < BTA_AV_NUM_RCB) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
      AVRC_VendorRsp(p_rcb->handle, p_data->api_vendor.label,
                     &p_data->api_vendor.msg);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_meta_rsp
 *
 * Description      Send an AVRCP metadata/advanced control command/response.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_meta_rsp(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
  bool do_free = true;

  if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
      (p_data->hdr.layer_specific < BTA_AV_NUM_RCB)) {
    if ((p_data->api_meta_rsp.is_rsp && (p_cb->features & BTA_AV_FEAT_RCTG)) ||
        (!p_data->api_meta_rsp.is_rsp && (p_cb->features & BTA_AV_FEAT_RCCT))) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
      if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
        AVRC_MsgReq(p_rcb->handle, p_data->api_meta_rsp.label,
                    p_data->api_meta_rsp.rsp_code, p_data->api_meta_rsp.p_pkt);
        do_free = false;
      }
    }
  }

  if (do_free) osi_free_and_reset((void**)&p_data->api_meta_rsp.p_pkt);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_free_rsp
 *
 * Description      free an AVRCP metadata command buffer.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_free_rsp(UNUSED_ATTR tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  osi_free_and_reset((void**)&p_data->api_meta_rsp.p_pkt);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_free_browse_msg
 *
 * Description      free an AVRCP browse message buffer.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_free_browse_msg(UNUSED_ATTR tBTA_AV_CB* p_cb,
                               tBTA_AV_DATA* p_data) {
  if (p_data->rc_msg.opcode == AVRC_OP_BROWSE) {
    osi_free_and_reset((void**)&p_data->rc_msg.msg.browse.p_browse_pkt);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_chk_notif_evt_id
 *
 * Description      make sure the requested player id is valid.
 *
 * Returns          BTA_AV_STS_NO_RSP, if no error
 *
 ******************************************************************************/
static tAVRC_STS bta_av_chk_notif_evt_id(tAVRC_MSG_VENDOR* p_vendor) {
  tAVRC_STS status = BTA_AV_STS_NO_RSP;
  uint8_t xx;
  uint16_t u16;
  uint8_t* p = p_vendor->p_vendor_data + 2;

  BE_STREAM_TO_UINT16(u16, p);
  /* double check the fixed length */
  if ((u16 != 5) || (p_vendor->vendor_len != 9)) {
    status = AVRC_STS_INTERNAL_ERR;
  } else {
    /* make sure the player_id is valid */
    for (xx = 0; xx < p_bta_av_cfg->num_evt_ids; xx++) {
      if (*p == p_bta_av_cfg->p_meta_evt_ids[xx]) {
        break;
      }
    }
    if (xx == p_bta_av_cfg->num_evt_ids) {
      status = AVRC_STS_BAD_PARAM;
    }
  }

  return status;
}

/*******************************************************************************
 *
 * Function         bta_av_proc_meta_cmd
 *
 * Description      Process an AVRCP metadata command from the peer.
 *
 * Returns          true to respond immediately
 *
 ******************************************************************************/
tBTA_AV_EVT bta_av_proc_meta_cmd(tAVRC_RESPONSE* p_rc_rsp,
                                 tBTA_AV_RC_MSG* p_msg, uint8_t* p_ctype) {
  tBTA_AV_EVT evt = BTA_AV_META_MSG_EVT;
  uint8_t u8, pdu, *p;
  uint16_t u16;
  tAVRC_MSG_VENDOR* p_vendor = &p_msg->msg.vendor;

  pdu = *(p_vendor->p_vendor_data);
  p_rc_rsp->pdu = pdu;
  *p_ctype = AVRC_RSP_REJ;

  /* Check to ansure a  valid minimum meta data length */
  if ((AVRC_MIN_META_CMD_LEN + p_vendor->vendor_len) > AVRC_META_CMD_BUF_SIZE) {
    /* reject it */
    p_rc_rsp->rsp.status = AVRC_STS_BAD_PARAM;
    APPL_TRACE_ERROR("%s: Invalid meta-command length: %d", __func__,
                     p_vendor->vendor_len);
    return 0;
  }

  /* Metadata messages only use PANEL sub-unit type */
  if (p_vendor->hdr.subunit_type != AVRC_SUB_PANEL) {
    APPL_TRACE_DEBUG("%s: SUBUNIT must be PANEL", __func__);
    /* reject it */
    evt = 0;
    p_vendor->hdr.ctype = AVRC_RSP_NOT_IMPL;
    p_vendor->vendor_len = 0;
    p_rc_rsp->rsp.status = AVRC_STS_BAD_PARAM;
  } else if (!AVRC_IsValidAvcType(pdu, p_vendor->hdr.ctype)) {
    APPL_TRACE_DEBUG("%s: Invalid pdu/ctype: 0x%x, %d", __func__, pdu,
                     p_vendor->hdr.ctype);
    /* reject invalid message without reporting to app */
    evt = 0;
    p_rc_rsp->rsp.status = AVRC_STS_BAD_CMD;
  } else {
    switch (pdu) {
      case AVRC_PDU_GET_CAPABILITIES:
        /* process GetCapabilities command without reporting the event to app */
        evt = 0;
        if (p_vendor->vendor_len != 5) {
          android_errorWriteLog(0x534e4554, "111893951");
          p_rc_rsp->get_caps.status = AVRC_STS_INTERNAL_ERR;
          break;
        }
        u8 = *(p_vendor->p_vendor_data + 4);
        p = p_vendor->p_vendor_data + 2;
        p_rc_rsp->get_caps.capability_id = u8;
        BE_STREAM_TO_UINT16(u16, p);
        if (u16 != 1) {
          p_rc_rsp->get_caps.status = AVRC_STS_INTERNAL_ERR;
        } else {
          p_rc_rsp->get_caps.status = AVRC_STS_NO_ERROR;
          if (u8 == AVRC_CAP_COMPANY_ID) {
            *p_ctype = AVRC_RSP_IMPL_STBL;
            p_rc_rsp->get_caps.count = p_bta_av_cfg->num_co_ids;
            memcpy(p_rc_rsp->get_caps.param.company_id,
                   p_bta_av_cfg->p_meta_co_ids,
                   (p_bta_av_cfg->num_co_ids << 2));
          } else if (u8 == AVRC_CAP_EVENTS_SUPPORTED) {
            *p_ctype = AVRC_RSP_IMPL_STBL;
            p_rc_rsp->get_caps.count = p_bta_av_cfg->num_evt_ids;
            memcpy(p_rc_rsp->get_caps.param.event_id,
                   p_bta_av_cfg->p_meta_evt_ids, p_bta_av_cfg->num_evt_ids);
          } else {
            APPL_TRACE_DEBUG("%s: Invalid capability ID: 0x%x", __func__, u8);
            /* reject - unknown capability ID */
            p_rc_rsp->get_caps.status = AVRC_STS_BAD_PARAM;
          }
        }
        break;

      case AVRC_PDU_REGISTER_NOTIFICATION:
        /* make sure the event_id is implemented */
        p_rc_rsp->rsp.status = bta_av_chk_notif_evt_id(p_vendor);
        if (p_rc_rsp->rsp.status != BTA_AV_STS_NO_RSP) evt = 0;
        break;
    }
  }

  return evt;
}

/*******************************************************************************
 *
 * Function         bta_av_rc_msg
 *
 * Description      Process an AVRCP message from the peer.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_msg(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_EVT evt = 0;
  tBTA_AV av;
  BT_HDR* p_pkt = NULL;
  tAVRC_MSG_VENDOR* p_vendor = &p_data->rc_msg.msg.vendor;
  bool is_inquiry = ((p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_SPEC_INQ) ||
                     p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_GEN_INQ);
  uint8_t ctype = 0;
  tAVRC_RESPONSE rc_rsp;

  rc_rsp.rsp.status = BTA_AV_STS_NO_RSP;

  if (NULL == p_data) {
    APPL_TRACE_ERROR("%s: Message from peer with no data", __func__);
    return;
  }

  APPL_TRACE_DEBUG("%s: opcode=%x, ctype=%x", __func__, p_data->rc_msg.opcode,
                   p_data->rc_msg.msg.hdr.ctype);

  if (p_data->rc_msg.opcode == AVRC_OP_PASS_THRU) {
    /* if this is a pass thru command */
    if ((p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_CTRL) ||
        (p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_SPEC_INQ) ||
        (p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_GEN_INQ)) {
      /* check if operation is supported */
      char avrcp_ct_support[PROPERTY_VALUE_MAX];
      osi_property_get("bluetooth.pts.avrcp_ct.support", avrcp_ct_support,
                       "false");
      if (p_data->rc_msg.msg.pass.op_id == AVRC_ID_VENDOR) {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_NOT_IMPL;
        if (p_cb->features & BTA_AV_FEAT_METADATA)
          p_data->rc_msg.msg.hdr.ctype = bta_av_group_navi_supported(
              p_data->rc_msg.msg.pass.pass_len,
              p_data->rc_msg.msg.pass.p_pass_data, is_inquiry);
      } else if (((p_data->rc_msg.msg.pass.op_id == AVRC_ID_VOL_UP) ||
                  (p_data->rc_msg.msg.pass.op_id == AVRC_ID_VOL_DOWN)) &&
                 !strcmp(avrcp_ct_support, "true")) {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_ACCEPT;
      } else {
        p_data->rc_msg.msg.hdr.ctype =
            bta_av_op_supported(p_data->rc_msg.msg.pass.op_id, is_inquiry);
      }

      APPL_TRACE_DEBUG("%s: ctype %d", __func__, p_data->rc_msg.msg.hdr.ctype)

      /* send response */
      if (p_data->rc_msg.msg.hdr.ctype != AVRC_RSP_INTERIM)
        AVRC_PassRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
                     &p_data->rc_msg.msg.pass);

      /* set up for callback if supported */
      if (p_data->rc_msg.msg.hdr.ctype == AVRC_RSP_ACCEPT ||
          p_data->rc_msg.msg.hdr.ctype == AVRC_RSP_INTERIM) {
        evt = BTA_AV_REMOTE_CMD_EVT;
        av.remote_cmd.rc_id = p_data->rc_msg.msg.pass.op_id;
        av.remote_cmd.key_state = p_data->rc_msg.msg.pass.state;
        av.remote_cmd.p_data = p_data->rc_msg.msg.pass.p_pass_data;
        av.remote_cmd.len = p_data->rc_msg.msg.pass.pass_len;
        memcpy(&av.remote_cmd.hdr, &p_data->rc_msg.msg.hdr, sizeof(tAVRC_HDR));
        av.remote_cmd.label = p_data->rc_msg.label;
      }
    }
    /* else if this is a pass thru response */
    /* id response type is not impl, we have to release label */
    else if (p_data->rc_msg.msg.hdr.ctype >= AVRC_RSP_NOT_IMPL) {
      /* set up for callback */
      evt = BTA_AV_REMOTE_RSP_EVT;
      av.remote_rsp.rc_id = p_data->rc_msg.msg.pass.op_id;
      av.remote_rsp.key_state = p_data->rc_msg.msg.pass.state;
      av.remote_rsp.rsp_code = p_data->rc_msg.msg.hdr.ctype;
      av.remote_rsp.label = p_data->rc_msg.label;

      /* If this response is for vendor unique command  */
      if ((p_data->rc_msg.msg.pass.op_id == AVRC_ID_VENDOR) &&
          (p_data->rc_msg.msg.pass.pass_len > 0)) {
        av.remote_rsp.p_data =
            (uint8_t*)osi_malloc(p_data->rc_msg.msg.pass.pass_len);
        APPL_TRACE_DEBUG("%s: Vendor Unique data len = %d", __func__,
                         p_data->rc_msg.msg.pass.pass_len);
        memcpy(av.remote_rsp.p_data, p_data->rc_msg.msg.pass.p_pass_data,
               p_data->rc_msg.msg.pass.pass_len);
      }
    }
    /* must be a bad ctype -> reject*/
    else {
      p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_REJ;
      AVRC_PassRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
                   &p_data->rc_msg.msg.pass);
    }
  }
  /* else if this is a vendor specific command or response */
  else if (p_data->rc_msg.opcode == AVRC_OP_VENDOR) {
    /* set up for callback */
    av.vendor_cmd.code = p_data->rc_msg.msg.hdr.ctype;
    av.vendor_cmd.company_id = p_vendor->company_id;
    av.vendor_cmd.label = p_data->rc_msg.label;
    av.vendor_cmd.p_data = p_vendor->p_vendor_data;
    av.vendor_cmd.len = p_vendor->vendor_len;

    /* if configured to support vendor specific and it's a command */
    if ((p_cb->features & BTA_AV_FEAT_VENDOR) &&
        p_data->rc_msg.msg.hdr.ctype <= AVRC_CMD_GEN_INQ) {
      if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
          (p_vendor->company_id == AVRC_CO_METADATA)) {
        av.meta_msg.p_msg = &p_data->rc_msg.msg;
        rc_rsp.rsp.status = BTA_AV_STS_NO_RSP;
        evt = bta_av_proc_meta_cmd(&rc_rsp, &p_data->rc_msg, &ctype);
      } else {
        evt = BTA_AV_VENDOR_CMD_EVT;
      }
    } else if ((p_cb->features & BTA_AV_FEAT_VENDOR) &&
               p_data->rc_msg.msg.hdr.ctype >= AVRC_RSP_NOT_IMPL) {
      /* else if configured to support vendor specific and it's a response */
      if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
          (p_vendor->company_id == AVRC_CO_METADATA)) {
        av.meta_msg.p_msg = &p_data->rc_msg.msg;
        evt = BTA_AV_META_MSG_EVT;
      } else {
        evt = BTA_AV_VENDOR_RSP_EVT;
      }
    } else if (!(p_cb->features & BTA_AV_FEAT_VENDOR) &&
               p_data->rc_msg.msg.hdr.ctype <= AVRC_CMD_GEN_INQ) {
      /* else if not configured to support vendor specific and it's a command */
      if (p_data->rc_msg.msg.vendor.p_vendor_data[0] == AVRC_PDU_INVALID) {
        /* reject it */
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_REJ;
        p_data->rc_msg.msg.vendor.p_vendor_data[4] = AVRC_STS_BAD_CMD;
      } else {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_NOT_IMPL;
      }
      AVRC_VendorRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
                     &p_data->rc_msg.msg.vendor);
    }
  } else if (p_data->rc_msg.opcode == AVRC_OP_BROWSE) {
    /* set up for callback */
    av.meta_msg.rc_handle = p_data->rc_msg.handle;
    av.meta_msg.company_id = p_vendor->company_id;
    av.meta_msg.code = p_data->rc_msg.msg.hdr.ctype;
    av.meta_msg.label = p_data->rc_msg.label;
    av.meta_msg.p_msg = &p_data->rc_msg.msg;
    av.meta_msg.p_data = p_data->rc_msg.msg.browse.p_browse_data;
    av.meta_msg.len = p_data->rc_msg.msg.browse.browse_len;
    evt = BTA_AV_META_MSG_EVT;
  }

  if (evt == 0 && rc_rsp.rsp.status != BTA_AV_STS_NO_RSP) {
    if (!p_pkt) {
      rc_rsp.rsp.opcode = p_data->rc_msg.opcode;
      AVRC_BldResponse(0, &rc_rsp, &p_pkt);
    }
    if (p_pkt)
      AVRC_MsgReq(p_data->rc_msg.handle, p_data->rc_msg.label, ctype, p_pkt);
  }

  /* call callback */
  if (evt != 0) {
    av.remote_cmd.rc_handle = p_data->rc_msg.handle;
    (*p_cb->p_cback)(evt, &av);
    /* If browsing message, then free the browse message buffer */
    bta_av_rc_free_browse_msg(p_cb, p_data);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_close
 *
 * Description      close the specified AVRC handle.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_close(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  uint16_t handle = p_data->hdr.layer_specific;
  tBTA_AV_SCB* p_scb;
  tBTA_AV_RCB* p_rcb;

  if (handle < BTA_AV_NUM_RCB) {
    p_rcb = &p_cb->rcb[handle];

    APPL_TRACE_DEBUG("%s: handle: %d, status=0x%x", __func__, p_rcb->handle,
                     p_rcb->status);
    if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
      if (p_rcb->shdl) {
        p_scb = bta_av_cb.p_scb[p_rcb->shdl - 1];
        if (p_scb) {
          /* just in case the RC timer is active
          if (bta_av_cb.features & BTA_AV_FEAT_RCCT &&
             p_scb->chnl == BTA_AV_CHNL_AUDIO) */
          alarm_cancel(p_scb->avrc_ct_timer);
        }
      }

      AVRC_Close(p_rcb->handle);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_browse_close
 *
 * Description      Empty placeholder.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_browse_close(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  APPL_TRACE_WARNING("%s: empty placeholder does nothing!", __func__);
}

/*******************************************************************************
 *
 * Function         bta_av_get_shdl
 *
 * Returns          The index to p_scb[]
 *
 ******************************************************************************/
static uint8_t bta_av_get_shdl(tBTA_AV_SCB* p_scb) {
  int i;
  uint8_t shdl = 0;
  /* find the SCB & stop the timer */
  for (i = 0; i < BTA_AV_NUM_STRS; i++) {
    if (p_scb == bta_av_cb.p_scb[i]) {
      shdl = i + 1;
      break;
    }
  }
  return shdl;
}

/*******************************************************************************
 *
 * Function         bta_av_stream_chg
 *
 * Description      audio streaming status changed.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_stream_chg(tBTA_AV_SCB* p_scb, bool started) {
  uint8_t started_msk = BTA_AV_HNDL_TO_MSK(p_scb->hdi);

  APPL_TRACE_DEBUG("%s: peer %s started:%s started_msk:0x%x", __func__,
                   p_scb->PeerAddress().ToString().c_str(),
                   logbool(started).c_str(), started_msk);

  if (started) {
    bta_av_cb.audio_streams |= started_msk;
    /* Let L2CAP know this channel is processed with high priority */
    L2CA_SetAclPriority(p_scb->PeerAddress(), L2CAP_PRIORITY_HIGH);
  } else {
    bta_av_cb.audio_streams &= ~started_msk;
    /* Let L2CAP know this channel is processed with low priority */
    L2CA_SetAclPriority(p_scb->PeerAddress(), L2CAP_PRIORITY_NORMAL);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_conn_chg
 *
 * Description      connetion status changed.
 *                  Open an AVRCP acceptor channel, if new conn.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_conn_chg(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb = NULL;
  tBTA_AV_SCB* p_scbi;
  uint8_t mask;
  uint8_t conn_msk;
  uint8_t old_msk;
  int i;
  int index = (p_data->hdr.layer_specific & BTA_AV_HNDL_MSK) - 1;
  tBTA_AV_LCB* p_lcb;
  tBTA_AV_LCB* p_lcb_rc;
  tBTA_AV_RCB *p_rcb, *p_rcb2;
  bool chk_restore = false;

  /* Validate array index*/
  if (index < BTA_AV_NUM_STRS) {
    p_scb = p_cb->p_scb[index];
  }
  mask = BTA_AV_HNDL_TO_MSK(index);
  p_lcb = bta_av_find_lcb(p_data->conn_chg.peer_addr, BTA_AV_LCB_FIND);
  conn_msk = 1 << (index + 1);
  if (p_data->conn_chg.is_up) {
    /* set the conned mask for this channel */
    if (p_scb) {
      if (p_lcb) {
        p_lcb->conn_msk |= conn_msk;
        for (i = 0; i < BTA_AV_NUM_RCB; i++) {
          if (bta_av_cb.rcb[i].lidx == p_lcb->lidx) {
            bta_av_cb.rcb[i].shdl = index + 1;
            APPL_TRACE_DEBUG(
                "%s: conn_chg up[%d]: %d, status=0x%x, shdl:%d, lidx:%d",
                __func__, i, bta_av_cb.rcb[i].handle, bta_av_cb.rcb[i].status,
                bta_av_cb.rcb[i].shdl, bta_av_cb.rcb[i].lidx);
            break;
          }
        }
      }
      old_msk = p_cb->conn_audio;
      p_cb->conn_audio |= mask;

      if ((old_msk & mask) == 0) {
        /* increase the audio open count, if not set yet */
        bta_av_cb.audio_open_cnt++;
      }

      APPL_TRACE_DEBUG("%s: rc_acp_handle:%d rc_acp_idx:%d", __func__,
                       p_cb->rc_acp_handle, p_cb->rc_acp_idx);
      /* check if the AVRCP ACP channel is already connected */
      if (p_lcb && p_cb->rc_acp_handle != BTA_AV_RC_HANDLE_NONE &&
          p_cb->rc_acp_idx) {
        p_lcb_rc = &p_cb->lcb[BTA_AV_NUM_LINKS];
        APPL_TRACE_DEBUG(
            "%s: rc_acp is connected && conn_chg on same addr "
            "p_lcb_rc->conn_msk:x%x",
            __func__, p_lcb_rc->conn_msk);
        /* check if the RC is connected to the scb addr */
        LOG_INFO(LOG_TAG, "%s: p_lcb_rc->addr: %s conn_chg.peer_addr: %s",
                 __func__, p_lcb_rc->addr.ToString().c_str(),
                 p_data->conn_chg.peer_addr.ToString().c_str());

        if (p_lcb_rc->conn_msk &&
            p_lcb_rc->addr == p_data->conn_chg.peer_addr) {
          /* AVRCP is already connected.
           * need to update the association betwen SCB and RCB */
          p_lcb_rc->conn_msk = 0; /* indicate RC ONLY is not connected */
          p_lcb_rc->lidx = 0;
          p_scb->rc_handle = p_cb->rc_acp_handle;
          p_rcb = &p_cb->rcb[p_cb->rc_acp_idx - 1];
          p_rcb->shdl = bta_av_get_shdl(p_scb);
          APPL_TRACE_DEBUG("%s: update rc_acp shdl:%d/%d srch:%d", __func__,
                           index + 1, p_rcb->shdl, p_scb->rc_handle);

          p_rcb2 = bta_av_get_rcb_by_shdl(p_rcb->shdl);
          if (p_rcb2) {
            /* found the RCB that was created to associated with this SCB */
            p_cb->rc_acp_handle = p_rcb2->handle;
            p_cb->rc_acp_idx = (p_rcb2 - p_cb->rcb) + 1;
            APPL_TRACE_DEBUG("%s: new rc_acp_handle:%d, idx:%d", __func__,
                             p_cb->rc_acp_handle, p_cb->rc_acp_idx);
            p_rcb2->lidx = (BTA_AV_NUM_LINKS + 1);
            APPL_TRACE_DEBUG("%s: rc2 handle:%d lidx:%d/%d", __func__,
                             p_rcb2->handle, p_rcb2->lidx,
                             p_cb->lcb[p_rcb2->lidx - 1].lidx);
          }
          p_rcb->lidx = p_lcb->lidx;
          APPL_TRACE_DEBUG("%s: rc handle:%d lidx:%d/%d", __func__,
                           p_rcb->handle, p_rcb->lidx,
                           p_cb->lcb[p_rcb->lidx - 1].lidx);
        }
      }
    }
  } else {
    if ((p_cb->conn_audio & mask) && bta_av_cb.audio_open_cnt) {
      /* this channel is still marked as open. decrease the count */
      bta_av_cb.audio_open_cnt--;
    }

    /* clear the conned mask for this channel */
    p_cb->conn_audio &= ~mask;
    if (p_scb) {
      // The stream is closed. Clear the state.
      p_scb->OnDisconnected();
      if (p_scb->chnl == BTA_AV_CHNL_AUDIO) {
        if (p_lcb) {
          p_lcb->conn_msk &= ~conn_msk;
        }
        /* audio channel is down. make sure the INT channel is down */
        /* just in case the RC timer is active
        if (p_cb->features & BTA_AV_FEAT_RCCT) */
        { alarm_cancel(p_scb->avrc_ct_timer); }
        /* one audio channel goes down. check if we need to restore high
         * priority */
        chk_restore = true;
      }
    }

    APPL_TRACE_DEBUG("%s: shdl:%d", __func__, index + 1);
    for (i = 0; i < BTA_AV_NUM_RCB; i++) {
      APPL_TRACE_DEBUG("%s: conn_chg dn[%d]: %d, status=0x%x, shdl:%d, lidx:%d",
                       __func__, i, bta_av_cb.rcb[i].handle,
                       bta_av_cb.rcb[i].status, bta_av_cb.rcb[i].shdl,
                       bta_av_cb.rcb[i].lidx);
      if (bta_av_cb.rcb[i].shdl == index + 1) {
        bta_av_del_rc(&bta_av_cb.rcb[i]);
        /* since the connection is already down and info was removed, clean
         * reference */
        bta_av_cb.rcb[i].shdl = 0;
        break;
      }
    }

    if (p_cb->conn_audio == 0) {
      /* if both channels are not connected,
       * close all RC channels */
      bta_av_close_all_rc(p_cb);
    }

    /* if the AVRCP is no longer listening, create the listening channel */
    if (bta_av_cb.rc_acp_handle == BTA_AV_RC_HANDLE_NONE &&
        bta_av_cb.features & BTA_AV_FEAT_RCTG)
      bta_av_rc_create(&bta_av_cb, AVCT_ACP, 0, BTA_AV_NUM_LINKS + 1);
  }

  APPL_TRACE_DEBUG(
      "%s: audio:%x up:%d conn_msk:0x%x chk_restore:%d "
      "audio_open_cnt:%d",
      __func__, p_cb->conn_audio, p_data->conn_chg.is_up, conn_msk, chk_restore,
      p_cb->audio_open_cnt);

  if (chk_restore) {
    if (p_cb->audio_open_cnt == 1) {
      /* one audio channel goes down and there's one audio channel remains open.
       * restore the switch role in default link policy */
      bta_sys_set_default_policy(BTA_ID_AV, HCI_ENABLE_MASTER_SLAVE_SWITCH);
      /* allow role switch, if this is the last connection */
      bta_av_restore_switch();
    }
    if (p_cb->audio_open_cnt) {
      /* adjust flush timeout settings to longer period */
      for (i = 0; i < BTA_AV_NUM_STRS; i++) {
        p_scbi = bta_av_cb.p_scb[i];
        if (p_scbi && p_scbi->chnl == BTA_AV_CHNL_AUDIO && p_scbi->co_started) {
          /* may need to update the flush timeout of this already started stream
           */
          if (p_scbi->co_started != bta_av_cb.audio_open_cnt) {
            p_scbi->co_started = bta_av_cb.audio_open_cnt;
            L2CA_SetFlushTimeout(
                p_scbi->PeerAddress(),
                p_bta_av_cfg->p_audio_flush_to[p_scbi->co_started - 1]);
          }
        }
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_disable
 *
 * Description      disable AV.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_disable(tBTA_AV_CB* p_cb, UNUSED_ATTR tBTA_AV_DATA* p_data) {
  BT_HDR hdr;
  uint16_t xx;

  p_cb->disabling = true;

  bta_av_close_all_rc(p_cb);

  osi_free_and_reset((void**)&p_cb->p_disc_db);

  /* disable audio/video - de-register all channels,
   * expect BTA_AV_DEREG_COMP_EVT when deregister is complete */
  for (xx = 0; xx < BTA_AV_NUM_STRS; xx++) {
    if (p_cb->p_scb[xx] != NULL) {
      hdr.layer_specific = xx + 1;
      bta_av_api_deregister((tBTA_AV_DATA*)&hdr);
    }
  }

  alarm_free(p_cb->link_signalling_timer);
  p_cb->link_signalling_timer = NULL;
  alarm_free(p_cb->accept_signalling_timer);
  p_cb->accept_signalling_timer = NULL;
}

/*******************************************************************************
 *
 * Function         bta_av_api_disconnect
 *
 * Description      .
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_api_disconnect(tBTA_AV_DATA* p_data) {
  AVDT_DisconnectReq(p_data->api_discnt.bd_addr, bta_av_conn_cback);
  alarm_cancel(bta_av_cb.link_signalling_timer);
}

/**
 * Find the index for the free LCB entry to use.
 *
 * The selection order is:
 * (1) Find the index if there is already SCB entry for the peer address
 * (2) If there is no SCB entry for the peer address, find the first
 * SCB entry that is not assigned.
 *
 * @param peer_address the peer address to use
 * @return the index for the free LCB entry to use or BTA_AV_NUM_LINKS
 * if no entry is found
 */
static uint8_t bta_av_find_lcb_index_by_scb_and_address(
    const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer_address: %s conn_lcb: 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_cb.conn_lcb);

  // Find the index if there is already SCB entry for the peer address
  for (uint8_t index = 0; index < BTA_AV_NUM_LINKS; index++) {
    uint8_t mask = 1 << index;
    if (mask & bta_av_cb.conn_lcb) {
      continue;
    }
    tBTA_AV_SCB* p_scb = bta_av_cb.p_scb[index];
    if (p_scb == nullptr) {
      continue;
    }
    if (p_scb->PeerAddress() == peer_address) {
      return index;
    }
  }

  // Find the first SCB entry that is not assigned.
  for (uint8_t index = 0; index < BTA_AV_NUM_LINKS; index++) {
    uint8_t mask = 1 << index;
    if (mask & bta_av_cb.conn_lcb) {
      continue;
    }
    tBTA_AV_SCB* p_scb = bta_av_cb.p_scb[index];
    if (p_scb == nullptr) {
      continue;
    }
    if (!p_scb->IsAssigned()) {
      return index;
    }
  }

  return BTA_AV_NUM_LINKS;
}

/*******************************************************************************
 *
 * Function         bta_av_sig_chg
 *
 * Description      process AVDT signal channel up/down.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_sig_chg(tBTA_AV_DATA* p_data) {
  uint16_t event = p_data->str_msg.hdr.layer_specific;
  tBTA_AV_CB* p_cb = &bta_av_cb;
  uint32_t xx;
  uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: event: %d", __func__, event);
  if (event == AVDT_CONNECT_IND_EVT) {
    APPL_TRACE_DEBUG("%s: AVDT_CONNECT_IND_EVT: peer %s", __func__,
                     p_data->str_msg.bd_addr.ToString().c_str());

    p_lcb = bta_av_find_lcb(p_data->str_msg.bd_addr, BTA_AV_LCB_FIND);
    if (!p_lcb) {
      /* if the address does not have an LCB yet, alloc one */
      xx = bta_av_find_lcb_index_by_scb_and_address(p_data->str_msg.bd_addr);

      /* check if we found something */
      if (xx >= BTA_AV_NUM_LINKS) {
        /* We do not have scb for this avdt connection.     */
        /* Silently close the connection.                   */
        APPL_TRACE_ERROR("%s: av scb not available for avdt connection for %s",
                         __func__, p_data->str_msg.bd_addr.ToString().c_str());
        AVDT_DisconnectReq(p_data->str_msg.bd_addr, NULL);
        return;
      }
      LOG_INFO(LOG_TAG,
               "%s: AVDT_CONNECT_IND_EVT: peer %s selected lcb_index %d",
               __func__, p_data->str_msg.bd_addr.ToString().c_str(), xx);

      tBTA_AV_SCB* p_scb = p_cb->p_scb[xx];
      mask = 1 << xx;
      p_lcb = &p_cb->lcb[xx];
      p_lcb->lidx = xx + 1;
      p_lcb->addr = p_data->str_msg.bd_addr;
      p_lcb->conn_msk = 0; /* clear the connect mask */
      /* start listening when the signal channel is open */
      if (p_cb->features & BTA_AV_FEAT_RCTG) {
        bta_av_rc_create(p_cb, AVCT_ACP, 0, p_lcb->lidx);
      }
      /* this entry is not used yet. */
      p_cb->conn_lcb |= mask; /* mark it as used */
      APPL_TRACE_DEBUG("%s: start sig timer %d", __func__, p_data->hdr.offset);
      if (p_data->hdr.offset == AVDT_ACP) {
        APPL_TRACE_DEBUG("%s: Incoming L2CAP acquired, set state as incoming",
                         __func__);
        p_scb->OnConnected(p_data->str_msg.bd_addr);
        p_scb->use_rc = true; /* allowing RC for incoming connection */
        bta_av_ssm_execute(p_scb, BTA_AV_ACP_CONNECT_EVT, p_data);

        /* The Pending Event should be sent as soon as the L2CAP signalling
         * channel
         * is set up, which is NOW. Earlier this was done only after
         * BTA_AV_SIGNALLING_TIMEOUT_MS.
         * The following function shall send the event and start the
         * recurring timer
         */
        bta_av_signalling_timer(NULL);

        APPL_TRACE_DEBUG("%s: Re-start timer for AVDTP service", __func__);
        bta_sys_conn_open(BTA_ID_AV, p_scb->app_id, p_scb->PeerAddress());
        /* Possible collision : need to avoid outgoing processing while the
         * timer is running */
        p_scb->coll_mask = BTA_AV_COLL_INC_TMR;
        alarm_set_on_mloop(
            p_cb->accept_signalling_timer, BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS,
            bta_av_accept_signalling_timer_cback, UINT_TO_PTR(xx));
      }
    }
  }
#if (BTA_AR_INCLUDED == TRUE)
  else if (event == BTA_AR_AVDT_CONN_EVT) {
    alarm_cancel(bta_av_cb.link_signalling_timer);
  }
#endif
  else {
    /* disconnected. */
    APPL_TRACE_DEBUG("%s: bta_av_cb.conn_lcb is %d", __func__,
                     bta_av_cb.conn_lcb);

    p_lcb = bta_av_find_lcb(p_data->str_msg.bd_addr, BTA_AV_LCB_FREE);
    if (p_lcb && (p_lcb->conn_msk || bta_av_cb.conn_lcb)) {
      APPL_TRACE_DEBUG("%s: conn_msk: 0x%x", __func__, p_lcb->conn_msk);
      /* clean up ssm  */
      for (xx = 0; xx < BTA_AV_NUM_STRS; xx++) {
        if (p_cb->p_scb[xx] &&
            p_cb->p_scb[xx]->PeerAddress() == p_data->str_msg.bd_addr) {
          APPL_TRACE_DEBUG("%s: Closing timer for AVDTP service", __func__);
          bta_sys_conn_close(BTA_ID_AV, p_cb->p_scb[xx]->app_id,
                             p_cb->p_scb[xx]->PeerAddress());
        }
        mask = 1 << (xx + 1);
        if (((mask & p_lcb->conn_msk) || bta_av_cb.conn_lcb) &&
            p_cb->p_scb[xx] &&
            p_cb->p_scb[xx]->PeerAddress() == p_data->str_msg.bd_addr) {
          APPL_TRACE_WARNING("%s: Sending AVDT_DISCONNECT_EVT peer_addr=%s",
                             __func__,
                             p_cb->p_scb[xx]->PeerAddress().ToString().c_str());
          bta_av_ssm_execute(p_cb->p_scb[xx], BTA_AV_AVDT_DISCONNECT_EVT, NULL);
        }
      }
    }
  }
  APPL_TRACE_DEBUG("%s: sig_chg conn_lcb: 0x%x", __func__, p_cb->conn_lcb);
}

/*******************************************************************************
 *
 * Function         bta_av_signalling_timer
 *
 * Description      process the signal channel timer. This timer is started
 *                  when the AVDTP signal channel is connected. If no profile
 *                  is connected, the timer goes off every
 *                  BTA_AV_SIGNALLING_TIMEOUT_MS.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_signalling_timer(UNUSED_ATTR tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  int xx;
  uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: conn_lcb=0x%x", __func__, p_cb->conn_lcb);
  for (xx = 0; xx < BTA_AV_NUM_LINKS; xx++) {
    p_lcb = &p_cb->lcb[xx];
    mask = 1 << xx;
    APPL_TRACE_DEBUG(
        "%s: index=%d conn_lcb=0x%x peer=%s conn_mask=0x%x lidx=%d", __func__,
        xx, p_cb->conn_lcb, p_lcb->addr.ToString().c_str(), p_lcb->conn_msk,
        p_lcb->lidx);
    if (mask & p_cb->conn_lcb) {
      /* this entry is used. check if it is connected */
      if (!p_lcb->conn_msk) {
        bta_sys_start_timer(p_cb->link_signalling_timer,
                            BTA_AV_SIGNALLING_TIMEOUT_MS,
                            BTA_AV_SIGNALLING_TIMER_EVT, 0);
        tBTA_AV_PEND pend;
        pend.bd_addr = p_lcb->addr;
        tBTA_AV bta_av_data;
        bta_av_data.pend = pend;
        APPL_TRACE_DEBUG(
            "%s: BTA_AV_PENDING_EVT for %s index=%d conn_mask=0x%x lidx=%d",
            __func__, pend.bd_addr.ToString().c_str(), xx, p_lcb->conn_msk,
            p_lcb->lidx);
        (*p_cb->p_cback)(BTA_AV_PENDING_EVT, &bta_av_data);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_accept_signalling_timer_cback
 *
 * Description      Process the timeout when SRC is accepting connection
 *                  and SNK did not start signalling.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_av_accept_signalling_timer_cback(void* data) {
  uint32_t inx = PTR_TO_UINT(data);
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb = NULL;
  if (inx < BTA_AV_NUM_STRS) {
    p_scb = p_cb->p_scb[inx];
  }
  if (p_scb) {
    APPL_TRACE_DEBUG("%s: coll_mask = 0x%02X", __func__, p_scb->coll_mask);

    if (p_scb->coll_mask & BTA_AV_COLL_INC_TMR) {
      p_scb->coll_mask &= ~BTA_AV_COLL_INC_TMR;

      if (bta_av_is_scb_opening(p_scb)) {
        APPL_TRACE_DEBUG("%s: stream state opening: SDP started = %d", __func__,
                         p_scb->sdp_discovery_started);
        if (p_scb->sdp_discovery_started) {
          /* We are still doing SDP. Run the timer again. */
          p_scb->coll_mask |= BTA_AV_COLL_INC_TMR;

          alarm_set_on_mloop(p_cb->accept_signalling_timer,
                             BTA_AV_ACCEPT_SIGNALLING_TIMEOUT_MS,
                             bta_av_accept_signalling_timer_cback,
                             UINT_TO_PTR(inx));
        } else {
          /* SNK did not start signalling, resume signalling process. */
          bta_av_discover_req(p_scb, NULL);
        }
      } else if (bta_av_is_scb_incoming(p_scb)) {
        /* Stay in incoming state if SNK does not start signalling */

        APPL_TRACE_DEBUG("%s: stream state incoming", __func__);
        /* API open was called right after SNK opened L2C connection. */
        if (p_scb->coll_mask & BTA_AV_COLL_API_CALLED) {
          p_scb->coll_mask &= ~BTA_AV_COLL_API_CALLED;

          /* BTA_AV_API_OPEN_EVT */
          tBTA_AV_API_OPEN* p_buf =
              (tBTA_AV_API_OPEN*)osi_malloc(sizeof(tBTA_AV_API_OPEN));
          memcpy(p_buf, &(p_scb->open_api), sizeof(tBTA_AV_API_OPEN));
          bta_sys_sendmsg(p_buf);
        }
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_check_peer_features
 *
 * Description      check supported features on the peer device from the SDP
 *                  record and return the feature mask
 *
 * Returns          tBTA_AV_FEAT peer device feature mask
 *
 ******************************************************************************/
tBTA_AV_FEAT bta_av_check_peer_features(uint16_t service_uuid) {
  tBTA_AV_FEAT peer_features = 0;
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tSDP_DISC_REC* p_rec = NULL;
  tSDP_DISC_ATTR* p_attr;
  uint16_t peer_rc_version = 0;
  uint16_t categories = 0;

  APPL_TRACE_DEBUG("%s: service_uuid:x%x", __func__, service_uuid);
  /* loop through all records we found */
  while (true) {
    /* get next record; if none found, we're done */
    p_rec = SDP_FindServiceInDb(p_cb->p_disc_db, service_uuid, p_rec);
    if (p_rec == NULL) {
      break;
    }

    if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_SERVICE_CLASS_ID_LIST)) !=
        NULL) {
      /* find peer features */
      if (SDP_FindServiceInDb(p_cb->p_disc_db, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                              NULL)) {
        peer_features |= BTA_AV_FEAT_RCCT;
      }
      if (SDP_FindServiceInDb(p_cb->p_disc_db,
                              UUID_SERVCLASS_AV_REM_CTRL_TARGET, NULL)) {
        peer_features |= BTA_AV_FEAT_RCTG;
      }
    }

    if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_BT_PROFILE_DESC_LIST)) != NULL) {
      /* get profile version (if failure, version parameter is not updated) */
      SDP_FindProfileVersionInRec(p_rec, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                                  &peer_rc_version);
      APPL_TRACE_DEBUG("%s: peer_rc_version 0x%x", __func__, peer_rc_version);

      if (peer_rc_version >= AVRC_REV_1_3)
        peer_features |= (BTA_AV_FEAT_VENDOR | BTA_AV_FEAT_METADATA);

      if (peer_rc_version >= AVRC_REV_1_4) {
        /* get supported categories */
        p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_SUPPORTED_FEATURES);
        if (p_attr != NULL) {
          categories = p_attr->attr_value.v.u16;
          if (categories & AVRC_SUPF_CT_CAT2)
            peer_features |= (BTA_AV_FEAT_ADV_CTRL);
          if (categories & AVRC_SUPF_CT_BROWSE)
            peer_features |= (BTA_AV_FEAT_BROWSE);
        }
      }
    }
  }
  APPL_TRACE_DEBUG("%s: peer_features:x%x", __func__, peer_features);
  return peer_features;
}

/*******************************************************************************
 *
 * Function         bta_avk_check_peer_features
 *
 * Description      check supported features on the peer device from the SDP
 *                  record and return the feature mask
 *
 * Returns          tBTA_AV_FEAT peer device feature mask
 *
 ******************************************************************************/
tBTA_AV_FEAT bta_avk_check_peer_features(uint16_t service_uuid) {
  tBTA_AV_FEAT peer_features = 0;
  tBTA_AV_CB* p_cb = &bta_av_cb;

  APPL_TRACE_DEBUG("%s: service_uuid:x%x", __func__, service_uuid);

  /* loop through all records we found */
  tSDP_DISC_REC* p_rec =
      SDP_FindServiceInDb(p_cb->p_disc_db, service_uuid, NULL);
  while (p_rec) {
    APPL_TRACE_DEBUG("%s: found Service record for x%x", __func__,
                     service_uuid);

    if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_SERVICE_CLASS_ID_LIST)) !=
        NULL) {
      /* find peer features */
      if (SDP_FindServiceInDb(p_cb->p_disc_db, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                              NULL)) {
        peer_features |= BTA_AV_FEAT_RCCT;
      }
      if (SDP_FindServiceInDb(p_cb->p_disc_db,
                              UUID_SERVCLASS_AV_REM_CTRL_TARGET, NULL)) {
        peer_features |= BTA_AV_FEAT_RCTG;
      }
    }

    if ((SDP_FindAttributeInRec(p_rec, ATTR_ID_BT_PROFILE_DESC_LIST)) != NULL) {
      /* get profile version (if failure, version parameter is not updated) */
      uint16_t peer_rc_version = 0;
      bool val = SDP_FindProfileVersionInRec(
          p_rec, UUID_SERVCLASS_AV_REMOTE_CONTROL, &peer_rc_version);
      APPL_TRACE_DEBUG("%s: peer_rc_version for TG 0x%x, profile_found %d",
                       __func__, peer_rc_version, val);

      if (peer_rc_version >= AVRC_REV_1_3)
        peer_features |= (BTA_AV_FEAT_VENDOR | BTA_AV_FEAT_METADATA);

      /*
       * Though Absolute Volume came after in 1.4 and above, but there are few
       * devices
       * in market which supports absolute Volume and they are still 1.3
       * TO avoid IOT issuses with those devices, we check for 1.3 as minimum
       * version
       */
      if (peer_rc_version >= AVRC_REV_1_3) {
        /* get supported features */
        tSDP_DISC_ATTR* p_attr =
            SDP_FindAttributeInRec(p_rec, ATTR_ID_SUPPORTED_FEATURES);
        if (p_attr != NULL) {
          uint16_t categories = p_attr->attr_value.v.u16;
          if (categories & AVRC_SUPF_CT_CAT2)
            peer_features |= (BTA_AV_FEAT_ADV_CTRL);
          if (categories & AVRC_SUPF_CT_APP_SETTINGS)
            peer_features |= (BTA_AV_FEAT_APP_SETTING);
          if (categories & AVRC_SUPF_CT_BROWSE)
            peer_features |= (BTA_AV_FEAT_BROWSE);
        }
      }
    }
    /* get next record; if none found, we're done */
    p_rec = SDP_FindServiceInDb(p_cb->p_disc_db, service_uuid, p_rec);
  }
  APPL_TRACE_DEBUG("%s: peer_features:x%x", __func__, peer_features);
  return peer_features;
}

/*******************************************************************************
 *
 * Function         bta_av_rc_disc_done
 *
 * Description      Handle AVRCP service discovery results.  If matching
 *                  service found, open AVRCP connection.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_disc_done(UNUSED_ATTR tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb = NULL;
  tBTA_AV_LCB* p_lcb;
  uint8_t rc_handle;
  tBTA_AV_FEAT peer_features = 0; /* peer features mask */

  APPL_TRACE_DEBUG("%s: bta_av_rc_disc_done disc:x%x", __func__, p_cb->disc);
  if (!p_cb->disc) {
    return;
  }

  if ((p_cb->disc & BTA_AV_CHNL_MSK) == BTA_AV_CHNL_MSK) {
    /* this is the rc handle/index to tBTA_AV_RCB */
    rc_handle = p_cb->disc & (~BTA_AV_CHNL_MSK);
  } else {
    /* Validate array index*/
    if (((p_cb->disc & BTA_AV_HNDL_MSK) - 1) < BTA_AV_NUM_STRS) {
      p_scb = p_cb->p_scb[(p_cb->disc & BTA_AV_HNDL_MSK) - 1];
    }
    if (p_scb) {
      rc_handle = p_scb->rc_handle;
    } else {
      p_cb->disc = 0;
      return;
    }
  }

  APPL_TRACE_DEBUG("%s: rc_handle %d", __func__, rc_handle);
#if (BTA_AV_SINK_INCLUDED == TRUE)
  if (p_cb->sdp_a2dp_snk_handle) {
    /* This is Sink + CT + TG(Abs Vol) */
    peer_features =
        bta_avk_check_peer_features(UUID_SERVCLASS_AV_REM_CTRL_TARGET);
    APPL_TRACE_DEBUG("%s: populating rem ctrl target features %d", __func__,
                     peer_features);
    if (BTA_AV_FEAT_ADV_CTRL &
        bta_avk_check_peer_features(UUID_SERVCLASS_AV_REMOTE_CONTROL))
      peer_features |= (BTA_AV_FEAT_ADV_CTRL | BTA_AV_FEAT_RCCT);
  } else
#endif
      if (p_cb->sdp_a2dp_handle) {
    /* check peer version and whether support CT and TG role */
    peer_features =
        bta_av_check_peer_features(UUID_SERVCLASS_AV_REMOTE_CONTROL);
    if ((p_cb->features & BTA_AV_FEAT_ADV_CTRL) &&
        ((peer_features & BTA_AV_FEAT_ADV_CTRL) == 0)) {
      /* if we support advance control and peer does not, check their support on
       * TG role
       * some implementation uses 1.3 on CT ans 1.4 on TG */
      peer_features |=
          bta_av_check_peer_features(UUID_SERVCLASS_AV_REM_CTRL_TARGET);
    }

    /* Change our features if the remote AVRCP version is 1.3 or less */
    tSDP_DISC_REC* p_rec = nullptr;
    p_rec = SDP_FindServiceInDb(p_cb->p_disc_db,
                                UUID_SERVCLASS_AV_REMOTE_CONTROL, p_rec);
    if (p_rec != NULL &&
        SDP_FindAttributeInRec(p_rec, ATTR_ID_BT_PROFILE_DESC_LIST) != NULL) {
      /* get profile version (if failure, version parameter is not updated) */
      uint16_t peer_rc_version = 0xFFFF;  // Don't change the AVRCP version
      SDP_FindProfileVersionInRec(p_rec, UUID_SERVCLASS_AV_REMOTE_CONTROL,
                                  &peer_rc_version);
      if (peer_rc_version <= AVRC_REV_1_3) {
        APPL_TRACE_DEBUG("%s: Using AVRCP 1.3 Capabilities with remote device",
                         __func__);
        p_bta_av_cfg = &bta_av_cfg_compatibility;
      }
    }
  }

  p_cb->disc = 0;
  osi_free_and_reset((void**)&p_cb->p_disc_db);

  APPL_TRACE_DEBUG("%s: peer_features 0x%x, features 0x%x", __func__,
                   peer_features, p_cb->features);

  /* if we have no rc connection */
  if (rc_handle == BTA_AV_RC_HANDLE_NONE) {
    if (p_scb) {
      /* if peer remote control service matches ours and USE_RC is true */
      if ((((p_cb->features & BTA_AV_FEAT_RCCT) &&
            (peer_features & BTA_AV_FEAT_RCTG)) ||
           ((p_cb->features & BTA_AV_FEAT_RCTG) &&
            (peer_features & BTA_AV_FEAT_RCCT)))) {
        p_lcb = bta_av_find_lcb(p_scb->PeerAddress(), BTA_AV_LCB_FIND);
        if (p_lcb) {
          rc_handle = bta_av_rc_create(p_cb, AVCT_INT,
                                       (uint8_t)(p_scb->hdi + 1), p_lcb->lidx);
          p_cb->rcb[rc_handle].peer_features = peer_features;
        } else {
          APPL_TRACE_ERROR("%s: can not find LCB!!", __func__);
        }
      } else if (p_scb->use_rc) {
        /* can not find AVRC on peer device. report failure */
        p_scb->use_rc = false;
        tBTA_AV_RC_OPEN rc_open;
        rc_open.peer_addr = p_scb->PeerAddress();
        rc_open.peer_features = 0;
        rc_open.status = BTA_AV_FAIL_SDP;
        tBTA_AV bta_av_data;
        bta_av_data.rc_open = rc_open;
        (*p_cb->p_cback)(BTA_AV_RC_OPEN_EVT, &bta_av_data);
      }
    }
  } else {
    tBTA_AV_RC_FEAT rc_feat;
    p_cb->rcb[rc_handle].peer_features = peer_features;
    rc_feat.rc_handle = rc_handle;
    rc_feat.peer_features = peer_features;
    if (p_scb == NULL) {
      /*
       * In case scb is not created by the time we are done with SDP
       * we still need to send RC feature event. So we need to get BD
       * from Message.  Note that lidx is 1 based not 0 based
       */
      rc_feat.peer_addr = p_cb->lcb[p_cb->rcb[rc_handle].lidx - 1].addr;
    } else {
      rc_feat.peer_addr = p_scb->PeerAddress();
    }
    tBTA_AV bta_av_data;
    bta_av_data.rc_feat = rc_feat;
    (*p_cb->p_cback)(BTA_AV_RC_FEAT_EVT, &bta_av_data);
  }
}

/*******************************************************************************
 *
 * Function         bta_av_rc_closed
 *
 * Description      Set AVRCP state to closed.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_closed(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CLOSE rc_close;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RCB* p_rcb;
  tBTA_AV_SCB* p_scb;
  int i;
  bool conn = false;
  tBTA_AV_LCB* p_lcb;

  rc_close.rc_handle = BTA_AV_RC_HANDLE_NONE;
  p_scb = NULL;
  APPL_TRACE_DEBUG("%s: rc_handle:%d", __func__, p_msg->handle);
  for (i = 0; i < BTA_AV_NUM_RCB; i++) {
    p_rcb = &p_cb->rcb[i];
    APPL_TRACE_DEBUG("%s: rcb[%d] rc_handle:%d, status=0x%x", __func__, i,
                     p_rcb->handle, p_rcb->status);
    if (p_rcb->handle == p_msg->handle) {
      rc_close.rc_handle = i;
      p_rcb->status &= ~BTA_AV_RC_CONN_MASK;
      p_rcb->peer_features = 0;
      APPL_TRACE_DEBUG("%s: shdl:%d, lidx:%d", __func__, p_rcb->shdl,
                       p_rcb->lidx);
      if (p_rcb->shdl) {
        if ((p_rcb->shdl - 1) < BTA_AV_NUM_STRS) {
          p_scb = bta_av_cb.p_scb[p_rcb->shdl - 1];
        }
        if (p_scb) {
          rc_close.peer_addr = p_scb->PeerAddress();
          if (p_scb->rc_handle == p_rcb->handle)
            p_scb->rc_handle = BTA_AV_RC_HANDLE_NONE;
          APPL_TRACE_DEBUG("%s: shdl:%d, srch:%d", __func__, p_rcb->shdl,
                           p_scb->rc_handle);
        }
        p_rcb->shdl = 0;
      } else if (p_rcb->lidx == (BTA_AV_NUM_LINKS + 1)) {
        /* if the RCB uses the extra LCB, use the addr for event and clean it */
        p_lcb = &p_cb->lcb[BTA_AV_NUM_LINKS];
        rc_close.peer_addr = p_msg->peer_addr;
        LOG_INFO(LOG_TAG, "%s: rc_only closed bd_addr: %s", __func__,
                 p_msg->peer_addr.ToString().c_str());
        p_lcb->conn_msk = 0;
        p_lcb->lidx = 0;
      }
      p_rcb->lidx = 0;

      if ((p_rcb->status & BTA_AV_RC_ROLE_MASK) == BTA_AV_RC_ROLE_INT) {
        /* AVCT CCB is deallocated */
        p_rcb->handle = BTA_AV_RC_HANDLE_NONE;
        p_rcb->status = 0;
      } else {
        /* AVCT CCB is still there. dealloc */
        bta_av_del_rc(p_rcb);
      }
    } else if ((p_rcb->handle != BTA_AV_RC_HANDLE_NONE) &&
               (p_rcb->status & BTA_AV_RC_CONN_MASK)) {
      /* at least one channel is still connected */
      conn = true;
    }
  }

  if (!conn) {
    /* no AVRC channels are connected, go back to INIT state */
    bta_av_sm_execute(p_cb, BTA_AV_AVRC_NONE_EVT, NULL);
  }

  if (rc_close.rc_handle == BTA_AV_RC_HANDLE_NONE) {
    rc_close.rc_handle = p_msg->handle;
    rc_close.peer_addr = p_msg->peer_addr;
  }
  tBTA_AV bta_av_data;
  bta_av_data.rc_close = rc_close;
  (*p_cb->p_cback)(BTA_AV_RC_CLOSE_EVT, &bta_av_data);
  if (bta_av_cb.rc_acp_handle == BTA_AV_RC_HANDLE_NONE
                  && bta_av_cb.features & BTA_AV_FEAT_RCTG)
      bta_av_rc_create(&bta_av_cb, AVCT_ACP, 0, BTA_AV_NUM_LINKS + 1);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_browse_opened
 *
 * Description      AVRC browsing channel is opened
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_browse_opened(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RC_BROWSE_OPEN rc_browse_open;

  LOG_INFO(LOG_TAG, "%s: peer_addr: %s rc_handle:%d", __func__,
           p_msg->peer_addr.ToString().c_str(), p_msg->handle);

  rc_browse_open.status = BTA_AV_SUCCESS;
  rc_browse_open.rc_handle = p_msg->handle;
  rc_browse_open.peer_addr = p_msg->peer_addr;

  tBTA_AV bta_av_data;
  bta_av_data.rc_browse_open = rc_browse_open;
  (*p_cb->p_cback)(BTA_AV_RC_BROWSE_OPEN_EVT, &bta_av_data);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_browse_closed
 *
 * Description      AVRC browsing channel is closed
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_browse_closed(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_RC_CONN_CHG* p_msg = (tBTA_AV_RC_CONN_CHG*)p_data;
  tBTA_AV_RC_BROWSE_CLOSE rc_browse_close;

  LOG_INFO(LOG_TAG, "%s: peer_addr: %s rc_handle:%d", __func__,
           p_msg->peer_addr.ToString().c_str(), p_msg->handle);

  rc_browse_close.rc_handle = p_msg->handle;
  rc_browse_close.peer_addr = p_msg->peer_addr;

  tBTA_AV bta_av_data;
  bta_av_data.rc_browse_close = rc_browse_close;
  (*p_cb->p_cback)(BTA_AV_RC_BROWSE_CLOSE_EVT, &bta_av_data);
}

/*******************************************************************************
 *
 * Function         bta_av_rc_disc
 *
 * Description      start AVRC SDP discovery.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_rc_disc(uint8_t disc) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tAVRC_SDP_DB_PARAMS db_params;
  uint16_t attr_list[] = {ATTR_ID_SERVICE_CLASS_ID_LIST,
                          ATTR_ID_BT_PROFILE_DESC_LIST,
                          ATTR_ID_SUPPORTED_FEATURES};
  uint8_t hdi;
  tBTA_AV_SCB* p_scb;
  RawAddress peer_addr = RawAddress::kEmpty;
  uint8_t rc_handle;

  APPL_TRACE_DEBUG("%s: disc: 0x%x, bta_av_cb.disc: 0x%x", __func__, disc,
                   bta_av_cb.disc);
  if ((bta_av_cb.disc != 0) || (disc == 0)) return;

  if ((disc & BTA_AV_CHNL_MSK) == BTA_AV_CHNL_MSK) {
    /* this is the rc handle/index to tBTA_AV_RCB */
    rc_handle = disc & (~BTA_AV_CHNL_MSK);
    if (p_cb->rcb[rc_handle].lidx) {
      peer_addr = p_cb->lcb[p_cb->rcb[rc_handle].lidx - 1].addr;
    }
  } else {
    hdi = (disc & BTA_AV_HNDL_MSK) - 1;
    p_scb = p_cb->p_scb[hdi];

    if (p_scb) {
      APPL_TRACE_DEBUG("%s: rc_handle %d", __func__, p_scb->rc_handle);
      peer_addr = p_scb->PeerAddress();
    }
  }

  if (!peer_addr.IsEmpty()) {
    /* allocate discovery database */
    if (p_cb->p_disc_db == NULL)
      p_cb->p_disc_db = (tSDP_DISCOVERY_DB*)osi_malloc(BTA_AV_DISC_BUF_SIZE);

    /* set up parameters */
    db_params.db_len = BTA_AV_DISC_BUF_SIZE;
    db_params.num_attr = 3;
    db_params.p_db = p_cb->p_disc_db;
    db_params.p_attrs = attr_list;

    /* searching for UUID_SERVCLASS_AV_REMOTE_CONTROL gets both TG and CT */
    if (AVRC_FindService(UUID_SERVCLASS_AV_REMOTE_CONTROL, peer_addr,
                         &db_params,
                         base::Bind(bta_av_avrc_sdp_cback)) == AVRC_SUCCESS) {
      p_cb->disc = disc;
      APPL_TRACE_DEBUG("%s: disc 0x%x", __func__, p_cb->disc);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_av_dereg_comp
 *
 * Description      deregister complete. free the stream control block.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_av_dereg_comp(tBTA_AV_DATA* p_data) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
  tBTA_AV_SCB* p_scb;
  tBTA_UTL_COD cod;
  uint8_t mask;
  BT_HDR* p_buf;

  /* find the stream control block */
  p_scb = bta_av_hndl_to_scb(p_data->hdr.layer_specific);

  if (p_scb) {
    APPL_TRACE_DEBUG("%s: deregistered %d(h%d)", __func__, p_scb->chnl,
                     p_scb->hndl);
    mask = BTA_AV_HNDL_TO_MSK(p_scb->hdi);
    p_cb->reg_audio &= ~mask;
    if ((p_cb->conn_audio & mask) && bta_av_cb.audio_open_cnt) {
      /* this channel is still marked as open. decrease the count */
      bta_av_cb.audio_open_cnt--;
    }
    p_cb->conn_audio &= ~mask;

    if (p_scb->q_tag == BTA_AV_Q_TAG_STREAM && p_scb->a2dp_list) {
      /* make sure no buffers are in a2dp_list */
      while (!list_is_empty(p_scb->a2dp_list)) {
        p_buf = (BT_HDR*)list_front(p_scb->a2dp_list);
        list_remove(p_scb->a2dp_list, p_buf);
        osi_free(p_buf);
      }
    }

    /* remove the A2DP SDP record, if no more audio stream is left */
    if (!p_cb->reg_audio) {
#if (BTA_AR_INCLUDED == TRUE)
      bta_ar_dereg_avrc(UUID_SERVCLASS_AV_REMOTE_CONTROL, BTA_ID_AV);
#endif
      if (p_cb->sdp_a2dp_handle) {
        bta_av_del_sdp_rec(&p_cb->sdp_a2dp_handle);
        p_cb->sdp_a2dp_handle = 0;
        bta_sys_remove_uuid(UUID_SERVCLASS_AUDIO_SOURCE);
      }

#if (BTA_AV_SINK_INCLUDED == TRUE)
      if (p_cb->sdp_a2dp_snk_handle) {
        bta_av_del_sdp_rec(&p_cb->sdp_a2dp_snk_handle);
        p_cb->sdp_a2dp_snk_handle = 0;
        bta_sys_remove_uuid(UUID_SERVCLASS_AUDIO_SINK);
      }
#endif
    }

    bta_av_free_scb(p_scb);
  }

  APPL_TRACE_DEBUG("%s: audio 0x%x, disable:%d", __func__, p_cb->reg_audio,
                   p_cb->disabling);
  /* if no stream control block is active */
  if (p_cb->reg_audio == 0) {
#if (BTA_AR_INCLUDED == TRUE)
    /* deregister from AVDT */
    bta_ar_dereg_avdt(BTA_ID_AV);

    /* deregister from AVCT */
    bta_ar_dereg_avrc(UUID_SERVCLASS_AV_REM_CTRL_TARGET, BTA_ID_AV);
    bta_ar_dereg_avct(BTA_ID_AV);
#endif

    if (p_cb->disabling) {
      p_cb->disabling = false;
      bta_av_cb.features = 0;
    }

    /* Clear the Capturing service class bit */
    cod.service = BTM_COD_SERVICE_CAPTURING;
    utl_set_device_class(&cod, BTA_UTL_CLR_COD_SERVICE_CLASS);
  }
}
