/******************************************************************************
 *
 *  Copyright 2008-2012 Broadcom Corporation
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
 *  This file contains the implementation of the SMP interface used by
 *  applications that can run over an SMP.
 *
 ******************************************************************************/
#include <base/logging.h>
#include <string.h>

#include "bt_target.h"
#include "bt_utils.h"
#include "stack_config.h"

#include "btm_int.h"
#include "hcimsgs.h"
#include "l2c_int.h"
#include "l2cdefs.h"
#include "smp_api.h"
#include "smp_int.h"

#include "btu.h"
#include "p_256_ecc_pp.h"

/*******************************************************************************
 *
 * Function         SMP_Init
 *
 * Description      This function initializes the SMP unit.
 *
 * Returns          void
 *
 ******************************************************************************/
void SMP_Init(void) {
  memset(&smp_cb, 0, sizeof(tSMP_CB));
  smp_cb.smp_rsp_timer_ent = alarm_new("smp.smp_rsp_timer_ent");
  smp_cb.delayed_auth_timer_ent = alarm_new("smp.delayed_auth_timer_ent");

#if defined(SMP_INITIAL_TRACE_LEVEL)
  smp_cb.trace_level = SMP_INITIAL_TRACE_LEVEL;
#else
  smp_cb.trace_level = BT_TRACE_LEVEL_NONE; /* No traces */
#endif
  SMP_TRACE_EVENT("%s", __func__);

  smp_l2cap_if_init();
  /* initialization of P-256 parameters */
  p_256_init_curve(KEY_LENGTH_DWORDS_P256);

  /* Initialize failure case for certification */
  smp_cb.cert_failure =
      stack_config_get_interface()->get_pts_smp_failure_case();
  if (smp_cb.cert_failure)
    SMP_TRACE_ERROR("%s PTS FAILURE MODE IN EFFECT (CASE %d)", __func__,
                    smp_cb.cert_failure);
}

/*******************************************************************************
 *
 * Function         SMP_SetTraceLevel
 *
 * Description      This function sets the trace level for SMP.  If called with
 *                  a value of 0xFF, it simply returns the current trace level.
 *
 *                  Input Parameters:
 *                      level:  The level to set the GATT tracing to:
 *                      0xff-returns the current setting.
 *                      0-turns off tracing.
 *                      >= 1-Errors.
 *                      >= 2-Warnings.
 *                      >= 3-APIs.
 *                      >= 4-Events.
 *                      >= 5-Debug.
 *
 * Returns          The new or current trace level
 *
 ******************************************************************************/
extern uint8_t SMP_SetTraceLevel(uint8_t new_level) {
  if (new_level != 0xFF) smp_cb.trace_level = new_level;

  return (smp_cb.trace_level);
}

/*******************************************************************************
 *
 * Function         SMP_Register
 *
 * Description      This function register for the SMP services callback.
 *
 * Returns          void
 *
 ******************************************************************************/
bool SMP_Register(tSMP_CALLBACK* p_cback) {
  SMP_TRACE_EVENT("SMP_Register state=%d", smp_cb.state);

  if (smp_cb.p_callback != NULL) {
    SMP_TRACE_ERROR("SMP_Register: duplicate registration, overwrite it");
  }
  smp_cb.p_callback = p_cback;

  return (true);
}

/*******************************************************************************
 *
 * Function         SMP_Pair
 *
 * Description      This function call to perform a SMP pairing with peer
 *                  device. Device support one SMP pairing at one time.
 *
 * Parameters       bd_addr - peer device bd address.
 *
 * Returns          None
 *
 ******************************************************************************/
tSMP_STATUS SMP_Pair(const RawAddress& bd_addr) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_EVENT("%s: state=%d br_state=%d flag=0x%x, bd_addr=%s", __func__,
                  p_cb->state, p_cb->br_state, p_cb->flags,
                  bd_addr.ToString().c_str());

  if (p_cb->state != SMP_STATE_IDLE ||
      p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD || p_cb->smp_over_br) {
    /* pending security on going, reject this one */
    return SMP_BUSY;
  } else {
    p_cb->flags = SMP_PAIR_FLAGS_WE_STARTED_DD;
    p_cb->pairing_bda = bd_addr;

    if (!L2CA_ConnectFixedChnl(L2CAP_SMP_CID, bd_addr)) {
      tSMP_INT_DATA smp_int_data;
      smp_int_data.status = SMP_PAIR_INTERNAL_ERR;
      SMP_TRACE_ERROR("%s: L2C connect fixed channel failed.", __func__);
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
      return SMP_PAIR_INTERNAL_ERR;
    }

    return SMP_STARTED;
  }
}

/*******************************************************************************
 *
 * Function         SMP_BR_PairWith
 *
 * Description      This function is called to start a SMP pairing over BR/EDR.
 *                  Device support one SMP pairing at one time.
 *
 * Parameters       bd_addr - peer device bd address.
 *
 * Returns          SMP_STARTED if pairing started, otherwise the reason for
 *                  failure.
 *
 ******************************************************************************/
tSMP_STATUS SMP_BR_PairWith(const RawAddress& bd_addr) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_EVENT("%s: state=%d br_state=%d flag=0x%x, bd_addr=%s", __func__,
                  p_cb->state, p_cb->br_state, p_cb->flags,
                  bd_addr.ToString().c_str());

  if (p_cb->state != SMP_STATE_IDLE || p_cb->smp_over_br ||
      p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD) {
    /* pending security on going, reject this one */
    return SMP_BUSY;
  }

  p_cb->role = HCI_ROLE_MASTER;
  p_cb->flags = SMP_PAIR_FLAGS_WE_STARTED_DD;
  p_cb->smp_over_br = true;
  p_cb->pairing_bda = bd_addr;

  if (!L2CA_ConnectFixedChnl(L2CAP_SMP_BR_CID, bd_addr)) {
    SMP_TRACE_ERROR("%s: L2C connect fixed channel failed.", __func__);
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_PAIR_INTERNAL_ERR;
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
    return SMP_PAIR_INTERNAL_ERR;
  }

  return SMP_STARTED;
}

/*******************************************************************************
 *
 * Function         SMP_PairCancel
 *
 * Description      This function call to cancel a SMP pairing with peer device.
 *
 * Parameters       bd_addr - peer device bd address.
 *
 * Returns          true - Pairining is cancelled
 *
 ******************************************************************************/
bool SMP_PairCancel(const RawAddress& bd_addr) {
  tSMP_CB* p_cb = &smp_cb;
  uint8_t err_code = SMP_PAIR_FAIL_UNKNOWN;

  // PTS SMP failure test cases
  if (p_cb->cert_failure == SMP_PASSKEY_ENTRY_FAIL ||
      p_cb->cert_failure == SMP_NUMERIC_COMPAR_FAIL)
    err_code = p_cb->cert_failure;

  BTM_TRACE_EVENT("SMP_CancelPair state=%d flag=0x%x ", p_cb->state,
                  p_cb->flags);
  if (p_cb->state != SMP_STATE_IDLE && p_cb->pairing_bda == bd_addr) {
    p_cb->is_pair_cancel = true;
    SMP_TRACE_DEBUG("Cancel Pairing: set fail reason Unknown");
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_PAIR_FAIL_UNKNOWN;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
    return true;
  }

  return false;
}
/*******************************************************************************
 *
 * Function         SMP_SecurityGrant
 *
 * Description      This function is called to grant security process.
 *
 * Parameters       bd_addr - peer device bd address.
 *                  res     - result of the operation SMP_SUCCESS if success.
 *                            Otherwise, SMP_REPEATED_ATTEMPTS if too many
 *                            attempts.
 *
 * Returns          None
 *
 ******************************************************************************/
void SMP_SecurityGrant(const RawAddress& bd_addr, uint8_t res) {
  SMP_TRACE_EVENT("SMP_SecurityGrant ");

  if (smp_cb.smp_over_br) {
    if (smp_cb.br_state != SMP_BR_STATE_WAIT_APP_RSP ||
        smp_cb.cb_evt != SMP_SEC_REQUEST_EVT || smp_cb.pairing_bda != bd_addr) {
      return;
    }

    /* clear the SMP_SEC_REQUEST_EVT event after get grant */
    /* avoid generating duplicate pair request */
    smp_cb.cb_evt = 0;
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = res;
    smp_br_state_machine_event(&smp_cb, SMP_BR_API_SEC_GRANT_EVT,
                               &smp_int_data);
    return;
  }

  if (smp_cb.state != SMP_STATE_WAIT_APP_RSP ||
      smp_cb.cb_evt != SMP_SEC_REQUEST_EVT || smp_cb.pairing_bda != bd_addr)
    return;
  /* clear the SMP_SEC_REQUEST_EVT event after get grant */
  /* avoid generate duplicate pair request */
  smp_cb.cb_evt = 0;
  tSMP_INT_DATA smp_int_data;
  smp_int_data.status = res;
  smp_sm_event(&smp_cb, SMP_API_SEC_GRANT_EVT, &smp_int_data);
}

/*******************************************************************************
 *
 * Function         SMP_PasskeyReply
 *
 * Description      This function is called after Security Manager submitted
 *                  passkey request to the application.
 *
 * Parameters:      bd_addr - Address of the device for which passkey was
 *                            requested
 *                  res     - result of the operation SMP_SUCCESS if success
 *                  passkey - numeric value in the range of
 *                            BTM_MIN_PASSKEY_VAL(0) -
 *                            BTM_MAX_PASSKEY_VAL(999999(0xF423F)).
 *
 ******************************************************************************/
void SMP_PasskeyReply(const RawAddress& bd_addr, uint8_t res,
                      uint32_t passkey) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_EVENT("SMP_PasskeyReply: Key: %d  Result:%d", passkey, res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if (p_cb->cb_evt != SMP_PASSKEY_REQ_EVT) {
    SMP_TRACE_WARNING("SMP_PasskeyReply() - Wrong State: %d", p_cb->state);
    return;
  }

  if (bd_addr != p_cb->pairing_bda) {
    SMP_TRACE_ERROR("SMP_PasskeyReply() - Wrong BD Addr");
    return;
  }

  if (btm_find_dev(bd_addr) == NULL) {
    SMP_TRACE_ERROR("SMP_PasskeyReply() - no dev CB");
    return;
  }

  if (passkey > BTM_MAX_PASSKEY_VAL || res != SMP_SUCCESS) {
    SMP_TRACE_WARNING(
        "SMP_PasskeyReply() - Wrong key len: %d or passkey entry fail",
        passkey);
    /* send pairing failure */
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_PASSKEY_ENTRY_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);

  } else if (p_cb->selected_association_model ==
             SMP_MODEL_SEC_CONN_PASSKEY_ENT) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.passkey = passkey;
    smp_sm_event(&smp_cb, SMP_SC_KEY_READY_EVT, &smp_int_data);
  } else {
    smp_convert_string_to_tk(&p_cb->tk, passkey);
  }

  return;
}

/*******************************************************************************
 *
 * Function         SMP_ConfirmReply
 *
 * Description      This function is called after Security Manager submitted
 *                  numeric comparison request to the application.
 *
 * Parameters:      bd_addr      - Address of the device with which numeric
 *                                 comparison was requested
 *                  res          - comparison result SMP_SUCCESS if success
 *
 ******************************************************************************/
void SMP_ConfirmReply(const RawAddress& bd_addr, uint8_t res) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_EVENT("%s: Result:%d", __func__, res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if (p_cb->cb_evt != SMP_NC_REQ_EVT) {
    SMP_TRACE_WARNING("%s() - Wrong State: %d", __func__, p_cb->state);
    return;
  }

  if (bd_addr != p_cb->pairing_bda) {
    SMP_TRACE_ERROR("%s() - Wrong BD Addr", __func__);
    return;
  }

  if (btm_find_dev(bd_addr) == NULL) {
    SMP_TRACE_ERROR("%s() - no dev CB", __func__);
    return;
  }

  if (res != SMP_SUCCESS) {
    SMP_TRACE_WARNING("%s() - Numeric Comparison fails", __func__);
    /* send pairing failure */
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_NUMERIC_COMPAR_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
  } else {
    smp_sm_event(p_cb, SMP_SC_NC_OK_EVT, NULL);
  }
}

/*******************************************************************************
 *
 * Function         SMP_OobDataReply
 *
 * Description      This function is called to provide the OOB data for
 *                  SMP in response to SMP_OOB_REQ_EVT
 *
 * Parameters:      bd_addr     - Address of the peer device
 *                  res         - result of the operation SMP_SUCCESS if success
 *                  p_data      - simple pairing Randomizer  C.
 *
 ******************************************************************************/
void SMP_OobDataReply(const RawAddress& bd_addr, tSMP_STATUS res, uint8_t len,
                      uint8_t* p_data) {
  tSMP_CB* p_cb = &smp_cb;
  tSMP_KEY key;

  SMP_TRACE_EVENT("%s State: %d  res:%d", __func__, smp_cb.state, res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if (p_cb->state != SMP_STATE_WAIT_APP_RSP || p_cb->cb_evt != SMP_OOB_REQ_EVT)
    return;

  if (res != SMP_SUCCESS || len == 0 || !p_data) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_OOB_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
  } else {
    if (len > OCTET16_LEN) len = OCTET16_LEN;

    memcpy(p_cb->tk.data(), p_data, len);

    key.key_type = SMP_KEY_TYPE_TK;
    key.p_data = p_cb->tk.data();

    tSMP_INT_DATA smp_int_data;
    smp_int_data.key = key;
    smp_sm_event(&smp_cb, SMP_KEY_READY_EVT, &smp_int_data);
  }
}

/*******************************************************************************
 *
 * Function         SMP_SecureConnectionOobDataReply
 *
 * Description      This function is called to provide the SC OOB data for
 *                  SMP in response to SMP_SC_OOB_REQ_EVT
 *
 * Parameters:      p_data      - pointer to the data
 *
 ******************************************************************************/
void SMP_SecureConnectionOobDataReply(uint8_t* p_data) {
  tSMP_CB* p_cb = &smp_cb;

  tSMP_SC_OOB_DATA* p_oob = (tSMP_SC_OOB_DATA*)p_data;
  if (!p_oob) {
    SMP_TRACE_ERROR("%s received no data", __func__);
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_OOB_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
    return;
  }

  SMP_TRACE_EVENT(
      "%s req_oob_type: %d, loc_oob_data.present: %d, "
      "peer_oob_data.present: %d",
      __func__, p_cb->req_oob_type, p_oob->loc_oob_data.present,
      p_oob->peer_oob_data.present);

  if (p_cb->state != SMP_STATE_WAIT_APP_RSP ||
      p_cb->cb_evt != SMP_SC_OOB_REQ_EVT)
    return;

  bool data_missing = false;
  switch (p_cb->req_oob_type) {
    case SMP_OOB_PEER:
      if (!p_oob->peer_oob_data.present) data_missing = true;
      break;
    case SMP_OOB_LOCAL:
      if (!p_oob->loc_oob_data.present) data_missing = true;
      break;
    case SMP_OOB_BOTH:
      if (!p_oob->loc_oob_data.present || !p_oob->peer_oob_data.present)
        data_missing = true;
      break;
    default:
      SMP_TRACE_EVENT("Unexpected OOB data type requested. Fail OOB");
      data_missing = true;
      break;
  }

  tSMP_INT_DATA smp_int_data;
  if (data_missing) {
    smp_int_data.status = SMP_OOB_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
    return;
  }

  p_cb->sc_oob_data = *p_oob;

  smp_int_data.p_data = p_data;
  smp_sm_event(&smp_cb, SMP_SC_OOB_DATA_EVT, &smp_int_data);
}

/*******************************************************************************
 *
 * Function         SMP_KeypressNotification
 *
 * Description      This function is called to notify Security Manager about
 *                  Keypress Notification.
 *
 * Parameters:     bd_addr      Address of the device to send keypress
 *                              notification to
 *                 value        Keypress notification parameter value
 *
 ******************************************************************************/
void SMP_KeypressNotification(const RawAddress& bd_addr, uint8_t value) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_EVENT("%s: Value: %d", __func__, value);

  if (bd_addr != p_cb->pairing_bda) {
    SMP_TRACE_ERROR("%s() - Wrong BD Addr", __func__);
    return;
  }

  if (btm_find_dev(bd_addr) == NULL) {
    SMP_TRACE_ERROR("%s() - no dev CB", __func__);
    return;
  }

  /* Keypress Notification is used by a device with KeyboardOnly IO capabilities
   * during the passkey entry protocol */
  if (p_cb->local_io_capability != SMP_IO_CAP_IN) {
    SMP_TRACE_ERROR("%s() - wrong local IO capabilities %d", __func__,
                    p_cb->local_io_capability);
    return;
  }

  if (p_cb->selected_association_model != SMP_MODEL_SEC_CONN_PASSKEY_ENT) {
    SMP_TRACE_ERROR("%s() - wrong protocol %d", __func__,
                    p_cb->selected_association_model);
    return;
  }

  tSMP_INT_DATA smp_int_data;
  smp_int_data.status = value;
  smp_sm_event(p_cb, SMP_KEYPRESS_NOTIFICATION_EVENT, &smp_int_data);
}

/*******************************************************************************
 *
 * Function         SMP_CreateLocalSecureConnectionsOobData
 *
 * Description      This function is called to start creation of local SC OOB
 *                  data set (tSMP_LOC_OOB_DATA).
 *
 * Parameters:      bd_addr - Address of the device to send OOB data block to
 *
 *  Returns         Boolean - true: creation of local SC OOB data set started.
 ******************************************************************************/
bool SMP_CreateLocalSecureConnectionsOobData(tBLE_BD_ADDR* addr_to_send_to) {
  tSMP_CB* p_cb = &smp_cb;

  if (addr_to_send_to == NULL) {
    SMP_TRACE_ERROR("%s addr_to_send_to is not provided", __func__);
    return false;
  }

  VLOG(2) << __func__ << " addr type:" << +addr_to_send_to->type
          << ", BDA:" << addr_to_send_to->bda << ", state:" << p_cb->state
          << ", br_state: " << p_cb->br_state;

  if ((p_cb->state != SMP_STATE_IDLE) || (p_cb->smp_over_br)) {
    SMP_TRACE_WARNING(
        "%s creation of local OOB data set "
        "starts only in IDLE state",
        __func__);
    return false;
  }

  p_cb->sc_oob_data.loc_oob_data.addr_sent_to = *addr_to_send_to;
  smp_sm_event(p_cb, SMP_CR_LOC_SC_OOB_DATA_EVT, NULL);

  return true;
}
