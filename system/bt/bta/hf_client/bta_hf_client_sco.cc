/******************************************************************************
 *
 *  Copyright (c) 2014 The Android Open Source Project
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
#include <string.h>

#include "bt_trace.h"
#include "bt_utils.h"
#include "bta_ag_api.h"
#include "bta_hf_client_int.h"
#include "device/include/esco_parameters.h"
#include "osi/include/osi.h"

#define BTA_HF_CLIENT_NO_EDR_ESCO                                \
  (ESCO_PKT_TYPES_MASK_NO_2_EV3 | ESCO_PKT_TYPES_MASK_NO_3_EV3 | \
   ESCO_PKT_TYPES_MASK_NO_2_EV5 | ESCO_PKT_TYPES_MASK_NO_3_EV5)

enum {
  BTA_HF_CLIENT_SCO_LISTEN_E,
  BTA_HF_CLIENT_SCO_OPEN_E,       /* open request */
  BTA_HF_CLIENT_SCO_CLOSE_E,      /* close request */
  BTA_HF_CLIENT_SCO_SHUTDOWN_E,   /* shutdown request */
  BTA_HF_CLIENT_SCO_CONN_OPEN_E,  /* SCO opened */
  BTA_HF_CLIENT_SCO_CONN_CLOSE_E, /* SCO closed */
};

/*******************************************************************************
 *
 * Function         bta_hf_client_remove_sco
 *
 * Description      Removes the specified SCO from the system.
 *
 * Returns          bool   - true if SCO removal was started
 *
 ******************************************************************************/
static bool bta_hf_client_sco_remove(tBTA_HF_CLIENT_CB* client_cb) {
  bool removed_started = false;
  tBTM_STATUS status;

  APPL_TRACE_DEBUG("%s", __func__);

  if (client_cb->sco_idx != BTM_INVALID_SCO_INDEX) {
    status = BTM_RemoveSco(client_cb->sco_idx);

    APPL_TRACE_DEBUG("%s: idx 0x%04x, status:0x%x", __func__,
                     client_cb->sco_idx, status);

    if (status == BTM_CMD_STARTED) {
      removed_started = true;
    }
    /* If no connection reset the SCO handle */
    else if ((status == BTM_SUCCESS) || (status == BTM_UNKNOWN_ADDR)) {
      client_cb->sco_idx = BTM_INVALID_SCO_INDEX;
    }
  }
  return removed_started;
}

/*******************************************************************************
 *
 * Function         bta_hf_client_cback_sco
 *
 * Description      Call application callback function with SCO event.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_cback_sco(tBTA_HF_CLIENT_CB* client_cb, uint8_t event) {
  tBTA_HF_CLIENT evt;

  memset(&evt, 0, sizeof(evt));
  evt.bd_addr = client_cb->peer_addr;

  /* call app cback */
  bta_hf_client_app_callback(event, &evt);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_conn_rsp
 *
 * Description      Process the SCO connection request
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_sco_conn_rsp(tBTA_HF_CLIENT_CB* client_cb,
                                       tBTM_ESCO_CONN_REQ_EVT_DATA* p_data) {
  enh_esco_params_t resp;
  uint8_t hci_status = HCI_SUCCESS;

  APPL_TRACE_DEBUG("%s", __func__);

  if (client_cb->sco_state == BTA_HF_CLIENT_SCO_LISTEN_ST) {
    if (p_data->link_type == BTM_LINK_TYPE_SCO) {
      resp = esco_parameters_for_codec(ESCO_CODEC_CVSD);
    } else {
      if (client_cb->negotiated_codec == BTA_AG_CODEC_MSBC) {
        resp = esco_parameters_for_codec(ESCO_CODEC_MSBC_T2);
      } else {
        // default codec
        resp = esco_parameters_for_codec(ESCO_CODEC_CVSD);
      }
    }

    /* tell sys to stop av if any */
    bta_sys_sco_use(BTA_ID_HS, 1, client_cb->peer_addr);
  } else {
    hci_status = HCI_ERR_HOST_REJECT_DEVICE;
  }

  BTM_EScoConnRsp(p_data->sco_inx, hci_status, &resp);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_connreq_cback
 *
 * Description      BTM eSCO connection requests and eSCO change requests
 *                  Only the connection requests are processed by BTA.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_esco_connreq_cback(tBTM_ESCO_EVT event,
                                             tBTM_ESCO_EVT_DATA* p_data) {
  APPL_TRACE_DEBUG("%s: %d", __func__, event);

  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_sco_handle(p_data->conn_evt.sco_inx);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong SCO handle to control block %d", __func__,
                     p_data->conn_evt.sco_inx);
    return;
  }

  if (event != BTM_ESCO_CONN_REQ_EVT) {
    return;
  }

  bta_hf_client_sco_conn_rsp(client_cb, &p_data->conn_evt);

  client_cb->sco_state = BTA_HF_CLIENT_SCO_OPENING_ST;
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_conn_cback
 *
 * Description      BTM SCO connection callback.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_sco_conn_cback(uint16_t sco_idx) {
  APPL_TRACE_DEBUG("%s: %d", __func__, sco_idx);

  tBTA_HF_CLIENT_CB* client_cb = bta_hf_client_find_cb_by_sco_handle(sco_idx);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong SCO handle to control block %d", __func__,
                     sco_idx);
    return;
  }

  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));
  p_buf->event = BTA_HF_CLIENT_SCO_OPEN_EVT;
  p_buf->layer_specific = client_cb->handle;
  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_disc_cback
 *
 * Description      BTM SCO disconnection callback.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_sco_disc_cback(uint16_t sco_idx) {
  APPL_TRACE_DEBUG("%s: sco_idx %d", __func__, sco_idx);

  tBTA_HF_CLIENT_CB* client_cb = bta_hf_client_find_cb_by_sco_handle(sco_idx);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__, sco_idx);
    return;
  }

  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));
  p_buf->event = BTA_HF_CLIENT_SCO_CLOSE_EVT;
  p_buf->layer_specific = client_cb->handle;
  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_create_sco
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_sco_create(tBTA_HF_CLIENT_CB* client_cb,
                                     bool is_orig) {
  tBTM_STATUS status;

  APPL_TRACE_DEBUG("%s: %d", __func__, is_orig);

  /* Make sure this SCO handle is not already in use */
  if (client_cb->sco_idx != BTM_INVALID_SCO_INDEX) {
    APPL_TRACE_WARNING("%s: Index 0x%04x already in use", __func__,
                       client_cb->sco_idx);
    return;
  }

  enh_esco_params_t params = esco_parameters_for_codec(ESCO_CODEC_CVSD);

  /* if initiating set current scb and peer bd addr */
  if (is_orig) {
    BTM_SetEScoMode(&params);
    /* tell sys to stop av if any */
    bta_sys_sco_use(BTA_ID_HS, 1, client_cb->peer_addr);
  }

  status = BTM_CreateSco(&client_cb->peer_addr, is_orig, params.packet_types,
                         &client_cb->sco_idx, bta_hf_client_sco_conn_cback,
                         bta_hf_client_sco_disc_cback);
  if (status == BTM_CMD_STARTED && !is_orig) {
    if (!BTM_RegForEScoEvts(client_cb->sco_idx,
                            bta_hf_client_esco_connreq_cback))
      APPL_TRACE_DEBUG("%s: SCO registration success", __func__);
  }

  APPL_TRACE_API("%s: orig %d, inx 0x%04x, status 0x%x, pkt types 0x%04x",
                 __func__, is_orig, client_cb->sco_idx, status,
                 params.packet_types);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_event
 *
 * Description      Handle SCO events
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hf_client_sco_event(tBTA_HF_CLIENT_CB* client_cb,
                                    uint8_t event) {
  APPL_TRACE_DEBUG("%s: before state: %d event: %d", __func__,
                   client_cb->sco_state, event);

  switch (client_cb->sco_state) {
    case BTA_HF_CLIENT_SCO_SHUTDOWN_ST:
      switch (event) {
        // For WBS we only listen to SCO requests. Even for outgoing SCO
        // requests we first do a AT+BCC and wait for remote to initiate SCO
        case BTA_HF_CLIENT_SCO_LISTEN_E:
          /* create SCO listen connection */
          bta_hf_client_sco_create(client_cb, false);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        // For non WBS cases and enabling outgoing SCO requests we need to force
        // open a SCO channel
        case BTA_HF_CLIENT_SCO_OPEN_E:
          /* remove listening connection */
          bta_hf_client_sco_remove(client_cb);

          /* create SCO connection to peer */
          bta_hf_client_sco_create(client_cb, true);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPENING_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_SHUTDOWN_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_LISTEN_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_LISTEN_E:
          /* create SCO listen connection */
          bta_hf_client_sco_create(client_cb, false);

        case BTA_HF_CLIENT_SCO_OPEN_E:
          /* remove listening connection */
          bta_hf_client_sco_remove(client_cb);

          /* create SCO connection to peer */
          bta_hf_client_sco_create(client_cb, true);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPENING_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
        case BTA_HF_CLIENT_SCO_CLOSE_E:
          /* remove listening connection */
          bta_hf_client_sco_remove(client_cb);

          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTDOWN_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* SCO failed; create SCO listen connection */
          bta_hf_client_sco_create(client_cb, false);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        default:
          APPL_TRACE_WARNING(
              "%s: BTA_HF_CLIENT_SCO_LISTEN_ST: Ignoring event %d", __func__,
              event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_OPENING_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_CLOSE_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPEN_CL_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTTING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_OPEN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPEN_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* SCO failed; create SCO listen connection */
          bta_hf_client_sco_create(client_cb, false);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_OPENING_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_OPEN_CL_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_OPEN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPENING_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTTING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_OPEN_E:
          /* close SCO connection */
          bta_hf_client_sco_remove(client_cb);

          client_cb->sco_state = BTA_HF_CLIENT_SCO_CLOSING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* SCO failed; create SCO listen connection */

          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_OPEN_CL_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_OPEN_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_CLOSE_E:
          if (bta_hf_client_sco_remove(client_cb)) {
            client_cb->sco_state = BTA_HF_CLIENT_SCO_CLOSING_ST;
          }
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          /* remove listening connection */
          bta_hf_client_sco_remove(client_cb);

          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTTING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* peer closed SCO */
          bta_hf_client_sco_create(client_cb, false);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_OPEN_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_CLOSING_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_OPEN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_CLOSE_OP_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTTING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* peer closed sco; create SCO listen connection */
          bta_hf_client_sco_create(client_cb, false);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_LISTEN_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_CLOSING_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_CLOSE_OP_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_CLOSE_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_CLOSING_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTTING_ST;
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          /* open SCO connection */
          bta_hf_client_sco_create(client_cb, true);
          client_cb->sco_state = BTA_HF_CLIENT_SCO_OPENING_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_CLOSE_OP_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    case BTA_HF_CLIENT_SCO_SHUTTING_ST:
      switch (event) {
        case BTA_HF_CLIENT_SCO_CONN_OPEN_E:
          /* close SCO connection; wait for conn close event */
          bta_hf_client_sco_remove(client_cb);
          break;

        case BTA_HF_CLIENT_SCO_CONN_CLOSE_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTDOWN_ST;
          break;

        case BTA_HF_CLIENT_SCO_SHUTDOWN_E:
          client_cb->sco_state = BTA_HF_CLIENT_SCO_SHUTDOWN_ST;
          break;

        default:
          APPL_TRACE_WARNING("BTA_HF_CLIENT_SCO_SHUTTING_ST: Ignoring event %d",
                             event);
          break;
      }
      break;

    default:
      break;
  }

  APPL_TRACE_DEBUG("%s: after state: %d", __func__, client_cb->sco_state);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_listen
 *
 * Description      Initialize SCO listener
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_listen(tBTA_HF_CLIENT_DATA* p_data) {
  APPL_TRACE_DEBUG("%s", __func__);

  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_handle(p_data->hdr.layer_specific);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__,
                     p_data->hdr.layer_specific);
    return;
  }

  bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_LISTEN_E);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_shutdown
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_shutdown(tBTA_HF_CLIENT_CB* client_cb) {
  APPL_TRACE_DEBUG("%s", __func__);

  bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_SHUTDOWN_E);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_conn_open
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_conn_open(tBTA_HF_CLIENT_DATA* p_data) {
  APPL_TRACE_DEBUG("%s", __func__);

  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_handle(p_data->hdr.layer_specific);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__,
                     p_data->hdr.layer_specific);
    return;
  }

  bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_CONN_OPEN_E);

  bta_sys_sco_open(BTA_ID_HS, 1, client_cb->peer_addr);

  if (client_cb->negotiated_codec == BTM_SCO_CODEC_MSBC) {
    bta_hf_client_cback_sco(client_cb, BTA_HF_CLIENT_AUDIO_MSBC_OPEN_EVT);
  } else {
    bta_hf_client_cback_sco(client_cb, BTA_HF_CLIENT_AUDIO_OPEN_EVT);
  }
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_conn_close
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_conn_close(tBTA_HF_CLIENT_DATA* p_data) {
  APPL_TRACE_DEBUG("%s", __func__);

  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_handle(p_data->hdr.layer_specific);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__,
                     p_data->hdr.layer_specific);
    return;
  }

  /* clear current scb */
  client_cb->sco_idx = BTM_INVALID_SCO_INDEX;

  bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_CONN_CLOSE_E);

  bta_sys_sco_close(BTA_ID_HS, 1, client_cb->peer_addr);

  bta_sys_sco_unuse(BTA_ID_HS, 1, client_cb->peer_addr);

  /* call app callback */
  bta_hf_client_cback_sco(client_cb, BTA_HF_CLIENT_AUDIO_CLOSE_EVT);

  if (client_cb->sco_close_rfc) {
    client_cb->sco_close_rfc = false;
    bta_hf_client_rfc_do_close(p_data);
  }
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_open
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_open(tBTA_HF_CLIENT_DATA* p_data) {
  APPL_TRACE_DEBUG("%s", __func__);

  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_handle(p_data->hdr.layer_specific);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__,
                     p_data->hdr.layer_specific);
    return;
  }

  bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_OPEN_E);
}

/*******************************************************************************
 *
 * Function         bta_hf_client_sco_close
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hf_client_sco_close(tBTA_HF_CLIENT_DATA* p_data) {
  tBTA_HF_CLIENT_CB* client_cb =
      bta_hf_client_find_cb_by_handle(p_data->hdr.layer_specific);
  if (client_cb == NULL) {
    APPL_TRACE_ERROR("%s: wrong handle to control block %d", __func__,
                     p_data->hdr.layer_specific);
    return;
  }

  APPL_TRACE_DEBUG("%s: sco_idx 0x%x", __func__, client_cb->sco_idx);

  if (client_cb->sco_idx != BTM_INVALID_SCO_INDEX) {
    bta_hf_client_sco_event(client_cb, BTA_HF_CLIENT_SCO_CLOSE_E);
  }
}
