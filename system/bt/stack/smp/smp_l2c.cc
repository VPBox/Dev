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
 *  This file contains functions for the SMP L2Cap interface
 *
 ******************************************************************************/

#include <cutils/log.h>
#include "bt_target.h"

#include <string.h>
#include "btm_ble_api.h"
#include "common/metrics.h"
#include "l2c_api.h"

#include "smp_int.h"

static void smp_tx_complete_callback(uint16_t cid, uint16_t num_pkt);

static void smp_connect_callback(uint16_t channel, const RawAddress& bd_addr,
                                 bool connected, uint16_t reason,
                                 tBT_TRANSPORT transport);
static void smp_data_received(uint16_t channel, const RawAddress& bd_addr,
                              BT_HDR* p_buf);

static void smp_br_connect_callback(uint16_t channel, const RawAddress& bd_addr,
                                    bool connected, uint16_t reason,
                                    tBT_TRANSPORT transport);
static void smp_br_data_received(uint16_t channel, const RawAddress& bd_addr,
                                 BT_HDR* p_buf);

/*******************************************************************************
 *
 * Function         smp_l2cap_if_init
 *
 * Description      This function is called during the SMP task startup
 *                  to register interface functions with L2CAP.
 *
 ******************************************************************************/
void smp_l2cap_if_init(void) {
  tL2CAP_FIXED_CHNL_REG fixed_reg;
  SMP_TRACE_EVENT("SMDBG l2c %s", __func__);
  fixed_reg.fixed_chnl_opts.mode = L2CAP_FCR_BASIC_MODE;
  fixed_reg.fixed_chnl_opts.max_transmit = 0;
  fixed_reg.fixed_chnl_opts.rtrans_tout = 0;
  fixed_reg.fixed_chnl_opts.mon_tout = 0;
  fixed_reg.fixed_chnl_opts.mps = 0;
  fixed_reg.fixed_chnl_opts.tx_win_sz = 0;

  fixed_reg.pL2CA_FixedConn_Cb = smp_connect_callback;
  fixed_reg.pL2CA_FixedData_Cb = smp_data_received;
  fixed_reg.pL2CA_FixedTxComplete_Cb = smp_tx_complete_callback;

  fixed_reg.pL2CA_FixedCong_Cb =
      NULL; /* do not handle congestion on this channel */
  fixed_reg.default_idle_tout =
      60; /* set 60 seconds timeout, 0xffff default idle timeout */

  L2CA_RegisterFixedChannel(L2CAP_SMP_CID, &fixed_reg);

  fixed_reg.pL2CA_FixedConn_Cb = smp_br_connect_callback;
  fixed_reg.pL2CA_FixedData_Cb = smp_br_data_received;

  L2CA_RegisterFixedChannel(L2CAP_SMP_BR_CID, &fixed_reg);
}

/*******************************************************************************
 *
 * Function         smp_connect_callback
 *
 * Description      This callback function is called by L2CAP to indicate that
 *                  SMP channel is
 *                      connected (conn = true)/disconnected (conn = false).
 *
 ******************************************************************************/
static void smp_connect_callback(uint16_t channel, const RawAddress& bd_addr,
                                 bool connected, uint16_t reason,
                                 tBT_TRANSPORT transport) {
  tSMP_CB* p_cb = &smp_cb;
  tSMP_INT_DATA int_data;

  SMP_TRACE_EVENT("%s: SMDBG l2c: bd_addr=%s, p_cb->pairing_bda=%s", __func__,
                  bd_addr.ToString().c_str(),
                  p_cb->pairing_bda.ToString().c_str());

  if (transport == BT_TRANSPORT_BR_EDR || bd_addr.IsEmpty()) return;

  if (bd_addr == p_cb->pairing_bda) {
    VLOG(2) << __func__ << " for pairing BDA: " << bd_addr
            << " Event: " << ((connected) ? "connected" : "disconnected");

    if (connected) {
      if (!p_cb->connect_initialized) {
        p_cb->connect_initialized = true;
        /* initiating connection established */
        p_cb->role = L2CA_GetBleConnRole(bd_addr);

        /* initialize local i/r key to be default keys */
        p_cb->local_r_key = p_cb->local_i_key = SMP_SEC_DEFAULT_KEY;
        p_cb->loc_auth_req = p_cb->peer_auth_req = SMP_DEFAULT_AUTH_REQ;
        p_cb->cb_evt = SMP_IO_CAP_REQ_EVT;
        smp_sm_event(p_cb, SMP_L2CAP_CONN_EVT, NULL);
      }
    } else {
      int_data.reason = reason;
      /* Disconnected while doing security */
      smp_sm_event(p_cb, SMP_L2CAP_DISCONN_EVT, &int_data);
    }
  }
}

/*******************************************************************************
 *
 * Function         smp_data_received
 *
 * Description      This function is called when data is received from L2CAP on
 *                  SMP channel.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void smp_data_received(uint16_t channel, const RawAddress& bd_addr,
                              BT_HDR* p_buf) {
  tSMP_CB* p_cb = &smp_cb;
  uint8_t* p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  uint8_t cmd;

  if (p_buf->len < 1) {
    android_errorWriteLog(0x534e4554, "111215315");
    SMP_TRACE_WARNING("%s: smp packet length %d too short: must be at least 1",
                      __func__, p_buf->len);
    osi_free(p_buf);
    return;
  }

  STREAM_TO_UINT8(cmd, p);

  SMP_TRACE_EVENT("%s: SMDBG l2c, cmd=0x%x", __func__, cmd);

  /* sanity check */
  if ((SMP_OPCODE_MAX < cmd) || (SMP_OPCODE_MIN > cmd)) {
    SMP_TRACE_WARNING("Ignore received command with RESERVED code 0x%02x", cmd);
    osi_free(p_buf);
    return;
  }

  /* reject the pairing request if there is an on-going SMP pairing */
  if (SMP_OPCODE_PAIRING_REQ == cmd || SMP_OPCODE_SEC_REQ == cmd) {
    if ((p_cb->state == SMP_STATE_IDLE) &&
        (p_cb->br_state == SMP_BR_STATE_IDLE) &&
        !(p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD)) {
      p_cb->role = L2CA_GetBleConnRole(bd_addr);
      p_cb->pairing_bda = bd_addr;
    } else if (bd_addr != p_cb->pairing_bda) {
      osi_free(p_buf);
      smp_reject_unexpected_pairing_command(bd_addr);
      return;
    }
    /* else, out of state pairing request/security request received, passed into
     * SM */
  }

  if (bd_addr == p_cb->pairing_bda) {
    alarm_set_on_mloop(p_cb->smp_rsp_timer_ent, SMP_WAIT_FOR_RSP_TIMEOUT_MS,
                       smp_rsp_timeout, NULL);

    smp_log_metrics(p_cb->pairing_bda, false /* incoming */,
                    p_buf->data + p_buf->offset, p_buf->len);

    if (cmd == SMP_OPCODE_CONFIRM) {
      SMP_TRACE_DEBUG(
          "in %s cmd = 0x%02x, peer_auth_req = 0x%02x,"
          "loc_auth_req = 0x%02x",
          __func__, cmd, p_cb->peer_auth_req, p_cb->loc_auth_req);

      if ((p_cb->peer_auth_req & SMP_SC_SUPPORT_BIT) &&
          (p_cb->loc_auth_req & SMP_SC_SUPPORT_BIT)) {
        cmd = SMP_OPCODE_PAIR_COMMITM;
      }
    }

    p_cb->rcvd_cmd_code = cmd;
    p_cb->rcvd_cmd_len = (uint8_t)p_buf->len;
    tSMP_INT_DATA smp_int_data;
    smp_int_data.p_data = p;
    smp_sm_event(p_cb, cmd, &smp_int_data);
  }

  osi_free(p_buf);
}

/*******************************************************************************
 *
 * Function         smp_tx_complete_callback
 *
 * Description      SMP channel tx complete callback
 *
 ******************************************************************************/
static void smp_tx_complete_callback(uint16_t cid, uint16_t num_pkt) {
  tSMP_CB* p_cb = &smp_cb;

  if (p_cb->total_tx_unacked >= num_pkt)
    p_cb->total_tx_unacked -= num_pkt;
  else
    SMP_TRACE_ERROR("Unexpected %s: num_pkt = %d", __func__, num_pkt);

  if (p_cb->total_tx_unacked == 0 && p_cb->wait_for_authorization_complete) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_SUCCESS;
    if (cid == L2CAP_SMP_CID) {
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
    } else {
      smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
    }
  }
}

/*******************************************************************************
 *
 * Function         smp_br_connect_callback
 *
 * Description      This callback function is called by L2CAP to indicate that
 *                  SMP BR channel is
 *                      connected (conn = true)/disconnected (conn = false).
 *
 ******************************************************************************/
static void smp_br_connect_callback(uint16_t channel, const RawAddress& bd_addr,
                                    bool connected, uint16_t reason,
                                    tBT_TRANSPORT transport) {
  tSMP_CB* p_cb = &smp_cb;
  tSMP_INT_DATA int_data;

  SMP_TRACE_EVENT("%s", __func__);

  if (transport != BT_TRANSPORT_BR_EDR) {
    SMP_TRACE_WARNING("%s is called on unexpected transport %d", __func__,
                      transport);
    return;
  }

  VLOG(1) << __func__ << " for pairing BDA: " << bd_addr
          << ", pairing_bda:" << p_cb->pairing_bda
          << " Event: " << ((connected) ? "connected" : "disconnected");

  if (bd_addr != p_cb->pairing_bda) return;

  if (connected) {
    if (!p_cb->connect_initialized) {
      p_cb->connect_initialized = true;
      /* initialize local i/r key to be default keys */
      p_cb->local_r_key = p_cb->local_i_key = SMP_BR_SEC_DEFAULT_KEY;
      p_cb->loc_auth_req = p_cb->peer_auth_req = 0;
      p_cb->cb_evt = SMP_BR_KEYS_REQ_EVT;
      smp_br_state_machine_event(p_cb, SMP_BR_L2CAP_CONN_EVT, NULL);
    }
  } else {
    int_data.reason = reason;
    /* Disconnected while doing security */
    smp_br_state_machine_event(p_cb, SMP_BR_L2CAP_DISCONN_EVT, &int_data);
  }
}

/*******************************************************************************
 *
 * Function         smp_br_data_received
 *
 * Description      This function is called when data is received from L2CAP on
 *                  SMP BR channel.
 *
 * Returns          void
 *
 ******************************************************************************/
static void smp_br_data_received(uint16_t channel, const RawAddress& bd_addr,
                                 BT_HDR* p_buf) {
  tSMP_CB* p_cb = &smp_cb;
  uint8_t* p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  uint8_t cmd;
  SMP_TRACE_EVENT("SMDBG l2c %s", __func__);

  if (p_buf->len < 1) {
    android_errorWriteLog(0x534e4554, "111215315");
    SMP_TRACE_WARNING("%s: smp packet length %d too short: must be at least 1",
                      __func__, p_buf->len);
    osi_free(p_buf);
    return;
  }

  STREAM_TO_UINT8(cmd, p);

  /* sanity check */
  if ((SMP_OPCODE_MAX < cmd) || (SMP_OPCODE_MIN > cmd)) {
    SMP_TRACE_WARNING("Ignore received command with RESERVED code 0x%02x", cmd);
    osi_free(p_buf);
    return;
  }

  /* reject the pairing request if there is an on-going SMP pairing */
  if (SMP_OPCODE_PAIRING_REQ == cmd) {
    if ((p_cb->state == SMP_STATE_IDLE) &&
        (p_cb->br_state == SMP_BR_STATE_IDLE)) {
      p_cb->role = HCI_ROLE_SLAVE;
      p_cb->smp_over_br = true;
      p_cb->pairing_bda = bd_addr;
    } else if (bd_addr != p_cb->pairing_bda) {
      osi_free(p_buf);
      smp_reject_unexpected_pairing_command(bd_addr);
      return;
    }
    /* else, out of state pairing request received, passed into State Machine */
  }

  if (bd_addr == p_cb->pairing_bda) {
    alarm_set_on_mloop(p_cb->smp_rsp_timer_ent, SMP_WAIT_FOR_RSP_TIMEOUT_MS,
                       smp_rsp_timeout, NULL);

    smp_log_metrics(p_cb->pairing_bda, false /* incoming */,
                    p_buf->data + p_buf->offset, p_buf->len);

    p_cb->rcvd_cmd_code = cmd;
    p_cb->rcvd_cmd_len = (uint8_t)p_buf->len;
    tSMP_INT_DATA smp_int_data;
    smp_int_data.p_data = p;
    smp_br_state_machine_event(p_cb, cmd, &smp_int_data);
  }

  osi_free(p_buf);
}
