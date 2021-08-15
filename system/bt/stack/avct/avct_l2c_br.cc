/******************************************************************************
 *
 *  Copyright 2008-2016 Broadcom Corporation
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
 *  Name:           avct_l2c_br.cc
 *
 *  Description:    This AVCTP module interfaces to L2CAP
 *
 *****************************************************************************/

#include <string.h>
#include "avct_api.h"
#include "avct_int.h"
#include "bt_target.h"
#include "bt_utils.h"
#include "l2c_api.h"
#include "l2cdefs.h"
#include "osi/include/osi.h"

/* Configuration flags. */
#define AVCT_L2C_CFG_IND_DONE (1 << 0)
#define AVCT_L2C_CFG_CFM_DONE (1 << 1)

/* AVCTP Browsing channel FCR Option:
 * Size of the transmission window when using enhanced retransmission mode. Not
 * used in basic and streaming modes. Range: 1 - 63
 */
#define AVCT_BR_FCR_OPT_TX_WINDOW_SIZE 10

/* AVCTP Browsing channel FCR Option:
 * Number of transmission attempts for a single I-Frame before taking
 * Down the connection. Used In ERTM mode only. Value is Ignored in basic and
 * Streaming modes.
 * Range: 0, 1-0xFF
 *     0 - infinite retransmissions
 *     1 - single transmission
 */
#define AVCT_BR_FCR_OPT_MAX_TX_B4_DISCNT 20

/* AVCTP Browsing channel FCR Option: Retransmission Timeout
 * The AVRCP specification set a value in the range of 300 - 2000 ms
 * Timeout (in msecs) to detect Lost I-Frames. Only used in Enhanced
 * retransmission mode.
 * Range: Minimum 2000 (2 secs) when supporting PBF.
 */
#define AVCT_BR_FCR_OPT_RETX_TOUT 2000

/* AVCTP Browsing channel FCR Option: Monitor Timeout
 * The AVRCP specification set a value in the range of 300 - 2000 ms
 * Timeout (in msecs) to detect Lost S-Frames. Only used in Enhanced
 * retransmission mode.
 * Range: Minimum 12000 (12 secs) when supporting PBF.
 */
#define AVCT_BR_FCR_OPT_MONITOR_TOUT 12000

/* callback function declarations */
void avct_l2c_br_connect_ind_cback(const RawAddress& bd_addr, uint16_t lcid,
                                   uint16_t psm, uint8_t id);
void avct_l2c_br_connect_cfm_cback(uint16_t lcid, uint16_t result);
void avct_l2c_br_config_cfm_cback(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg);
void avct_l2c_br_config_ind_cback(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg);
void avct_l2c_br_disconnect_ind_cback(uint16_t lcid, bool ack_needed);
void avct_l2c_br_disconnect_cfm_cback(uint16_t lcid, uint16_t result);
void avct_l2c_br_congestion_ind_cback(uint16_t lcid, bool is_congested);
void avct_l2c_br_data_ind_cback(uint16_t lcid, BT_HDR* p_buf);

/* L2CAP callback function structure */
const tL2CAP_APPL_INFO avct_l2c_br_appl = {
    avct_l2c_br_connect_ind_cback,
    avct_l2c_br_connect_cfm_cback,
    NULL,
    avct_l2c_br_config_ind_cback,
    avct_l2c_br_config_cfm_cback,
    avct_l2c_br_disconnect_ind_cback,
    avct_l2c_br_disconnect_cfm_cback,
    NULL,
    avct_l2c_br_data_ind_cback,
    avct_l2c_br_congestion_ind_cback,
    NULL, /* tL2CA_TX_COMPLETE_CB */
    NULL /* tL2CA_CREDITS_RECEIVED_CB */};

/* Browsing channel eL2CAP default options */
const tL2CAP_FCR_OPTS avct_l2c_br_fcr_opts_def = {
    L2CAP_FCR_ERTM_MODE,              /* Mandatory for Browsing channel */
    AVCT_BR_FCR_OPT_TX_WINDOW_SIZE,   /* Tx window size */
    AVCT_BR_FCR_OPT_MAX_TX_B4_DISCNT, /* Maximum transmissions before
                                         disconnecting */
    AVCT_BR_FCR_OPT_RETX_TOUT,        /* Retransmission timeout (2 secs) */
    AVCT_BR_FCR_OPT_MONITOR_TOUT,     /* Monitor timeout (12 secs) */
    L2CAP_DEFAULT_ERM_MPS             /* MPS segment size */
};

/*******************************************************************************
 *
 * Function         avct_l2c_br_connect_ind_cback
 *
 * Description      This is the L2CAP connect indication callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_connect_ind_cback(const RawAddress& bd_addr, uint16_t lcid,
                                   UNUSED_ATTR uint16_t psm, uint8_t id) {
  tAVCT_LCB* p_lcb;
  uint16_t result = L2CAP_CONN_NO_RESOURCES;
  tL2CAP_CFG_INFO cfg;
  tAVCT_BCB* p_bcb;
  tL2CAP_ERTM_INFO ertm_info;

  memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));
  cfg.mtu_present = true;

  p_lcb = avct_lcb_by_bd(bd_addr);
  if (p_lcb != NULL) {
    /* control channel exists */
    p_bcb = avct_bcb_by_lcb(p_lcb);
    p_bcb->peer_addr = bd_addr;

    if (p_bcb->allocated == 0) {
      /* browsing channel does not exist yet and the browsing channel is
       * registered
       * - accept connection */
      p_bcb->allocated = p_lcb->allocated; /* copy the index from lcb */

      result = L2CAP_CONN_OK;
      cfg.mtu = avct_cb.mtu_br;

      cfg.fcr_present = true;
      cfg.fcr = avct_l2c_br_fcr_opts_def;
    }
  }
  /* else no control channel yet, reject */

  /* Set the FCR options: Browsing channel mandates ERTM */
  ertm_info.preferred_mode = cfg.fcr.mode;
  ertm_info.allowed_modes = L2CAP_FCR_CHAN_OPT_ERTM;
  ertm_info.user_rx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.user_tx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.fcr_rx_buf_size = BT_DEFAULT_BUFFER_SIZE;
  ertm_info.fcr_tx_buf_size = BT_DEFAULT_BUFFER_SIZE;

  /* Send L2CAP connect rsp */
  L2CA_ErtmConnectRsp(bd_addr, id, lcid, result, 0, &ertm_info);

  /* if result ok, proceed with connection */
  if (result == L2CAP_CONN_OK) {
    /* store LCID */
    p_bcb->ch_lcid = lcid;

    /* transition to configuration state */
    p_bcb->ch_state = AVCT_CH_CFG;

    /* Send L2CAP config req */
    L2CA_ConfigReq(lcid, &cfg);
  }
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_connect_cfm_cback
 *
 * Description      This is the L2CAP connect confirm callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_connect_cfm_cback(uint16_t lcid, uint16_t result) {
  tAVCT_BCB* p_lcb;
  tL2CAP_CFG_INFO cfg;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if ((p_lcb == NULL) || (p_lcb->ch_state != AVCT_CH_CONN)) return;

  if (result != L2CAP_CONN_OK) {
    /* failure */
    tAVCT_LCB_EVT avct_lcb_evt;
    avct_lcb_evt.result = result;
    avct_bcb_event(p_lcb, AVCT_LCB_LL_CLOSE_EVT, &avct_lcb_evt);
    return;
  }

  /* result is successful */
  /* set channel state */
  p_lcb->ch_state = AVCT_CH_CFG;

  /* Send L2CAP config req */
  memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));

  cfg.mtu_present = true;
  cfg.mtu = avct_cb.mtu_br;

  cfg.fcr_present = true;
  cfg.fcr = avct_l2c_br_fcr_opts_def;

  L2CA_ConfigReq(lcid, &cfg);
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_config_cfm_cback
 *
 * Description      This is the L2CAP config confirm callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_config_cfm_cback(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg) {
  tAVCT_BCB* p_lcb;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if ((p_lcb == NULL) || (p_lcb->ch_state != AVCT_CH_CFG)) return;

  /* if result successful */
  if (p_cfg->result == L2CAP_CFG_OK) {
    /* update flags */
    p_lcb->ch_flags |= AVCT_L2C_CFG_CFM_DONE;

    /* if configuration complete */
    if (p_lcb->ch_flags & AVCT_L2C_CFG_IND_DONE) {
      p_lcb->ch_state = AVCT_CH_OPEN;
      avct_bcb_event(p_lcb, AVCT_LCB_LL_OPEN_EVT, NULL);
    }
  }
  /* else failure */
  else {
    /* store result value */
    p_lcb->ch_result = p_cfg->result;

    /* Send L2CAP disconnect req */
    L2CA_DisconnectReq(lcid);
  }
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_config_ind_cback
 *
 * Description      This is the L2CAP config indication callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_config_ind_cback(uint16_t lcid, tL2CAP_CFG_INFO* p_cfg) {
  tAVCT_BCB* p_lcb;
  uint16_t max_mtu = BT_DEFAULT_BUFFER_SIZE - L2CAP_MIN_OFFSET - BT_HDR_SIZE;

  /* Don't include QoS nor flush timeout in the response since we
     currently always accept these values.  Note: fcr_present is left
     untouched since l2cap negotiates this internally
  */
  p_cfg->flush_to_present = false;
  p_cfg->qos_present = false;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if (p_lcb == NULL) return;

  /* store the mtu in tbl */
  p_lcb->peer_mtu = L2CAP_DEFAULT_MTU;
  if (p_cfg->mtu_present) {
    p_lcb->peer_mtu = p_cfg->mtu;
  }

  if (p_lcb->peer_mtu > max_mtu) {
    p_lcb->peer_mtu = p_cfg->mtu = max_mtu;

    /* Must tell the peer what the adjusted value is */
    p_cfg->mtu_present = true;
  } else /* Don't include in the response */
    p_cfg->mtu_present = false;

  AVCT_TRACE_DEBUG("%s peer_mtu:%d use:%d", __func__, p_lcb->peer_mtu, max_mtu);

  if (p_lcb->peer_mtu >= AVCT_MIN_BROWSE_MTU)
    p_cfg->result = L2CAP_CFG_OK;
  else {
    p_cfg->result = L2CAP_CFG_UNACCEPTABLE_PARAMS;
    p_cfg->mtu_present = true;
    p_cfg->mtu = AVCT_MIN_BROWSE_MTU;
  }

  /* send L2CAP configure response */
  L2CA_ConfigRsp(lcid, p_cfg);

  if (p_cfg->result != L2CAP_CFG_OK) {
    return;
  }

  /* if first config ind */
  if ((p_lcb->ch_flags & AVCT_L2C_CFG_IND_DONE) == 0) {
    /* update flags */
    p_lcb->ch_flags |= AVCT_L2C_CFG_IND_DONE;

    /* if configuration complete */
    if (p_lcb->ch_flags & AVCT_L2C_CFG_CFM_DONE) {
      p_lcb->ch_state = AVCT_CH_OPEN;
      avct_bcb_event(p_lcb, AVCT_LCB_LL_OPEN_EVT, NULL);
    }
  }
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_disconnect_ind_cback
 *
 * Description      This is the L2CAP disconnect indication callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_disconnect_ind_cback(uint16_t lcid, bool ack_needed) {
  tAVCT_BCB* p_lcb;
  uint16_t result = AVCT_RESULT_FAIL;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if (p_lcb == NULL) return;

  if (ack_needed) {
    /* send L2CAP disconnect response */
    L2CA_DisconnectRsp(lcid);
  }

  tAVCT_LCB_EVT avct_lcb_evt;
  avct_lcb_evt.result = result;
  avct_bcb_event(p_lcb, AVCT_LCB_LL_CLOSE_EVT, &avct_lcb_evt);
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_disconnect_cfm_cback
 *
 * Description      This is the L2CAP disconnect confirm callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_disconnect_cfm_cback(uint16_t lcid, uint16_t result) {
  tAVCT_BCB* p_lcb;
  uint16_t res;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if (p_lcb == NULL) return;

  /* result value may be previously stored */
  res = (p_lcb->ch_result != 0) ? p_lcb->ch_result : result;
  p_lcb->ch_result = 0;

  tAVCT_LCB_EVT avct_lcb_evt;
  avct_lcb_evt.result = res;
  avct_bcb_event(p_lcb, AVCT_LCB_LL_CLOSE_EVT, &avct_lcb_evt);
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_congestion_ind_cback
 *
 * Description      This is the L2CAP congestion indication callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_congestion_ind_cback(uint16_t lcid, bool is_congested) {
  tAVCT_BCB* p_lcb;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if (p_lcb == NULL) return;

  tAVCT_LCB_EVT avct_lcb_evt;
  avct_lcb_evt.cong = is_congested;
  avct_bcb_event(p_lcb, AVCT_LCB_LL_CONG_EVT, &avct_lcb_evt);
}

/*******************************************************************************
 *
 * Function         avct_l2c_br_data_ind_cback
 *
 * Description      This is the L2CAP data indication callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void avct_l2c_br_data_ind_cback(uint16_t lcid, BT_HDR* p_buf) {
  tAVCT_BCB* p_lcb;
  tAVCT_LCB_EVT evt_data;

  /* look up lcb for this channel */
  p_lcb = avct_bcb_by_lcid(lcid);
  if (p_lcb == NULL) {
    /* prevent buffer leak */
    osi_free(p_buf);
    return;
  }

  evt_data.p_buf = p_buf;
  avct_bcb_event(p_lcb, AVCT_LCB_LL_MSG_EVT, &evt_data);
}
