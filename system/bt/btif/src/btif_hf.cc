/******************************************************************************
 *
 *  Copyright 2009-2012 Broadcom Corporation
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

/*******************************************************************************
 *
 *  Filename:      btif_hf.c
 *
 *  Description:   Handsfree Profile Bluetooth Interface
 *
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_hf"

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <bta/include/bta_ag_api.h>
#include <hardware/bluetooth.h>
#include <hardware/bluetooth_headset_callbacks.h>
#include <hardware/bluetooth_headset_interface.h>
#include <hardware/bt_hf.h>
#include <log/log.h>

#include "bta/include/utl.h"
#include "bta_ag_api.h"
#include "btif_common.h"
#include "btif_hf.h"
#include "btif_profile_queue.h"
#include "btif_util.h"
#include "common/metrics.h"

namespace bluetooth {
namespace headset {

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/
#ifndef BTIF_HSAG_SERVICE_NAME
#define BTIF_HSAG_SERVICE_NAME ("Headset Gateway")
#endif

#ifndef BTIF_HFAG_SERVICE_NAME
#define BTIF_HFAG_SERVICE_NAME ("Handsfree Gateway")
#endif

#ifndef BTIF_HF_SERVICES
#define BTIF_HF_SERVICES (BTA_HSP_SERVICE_MASK | BTA_HFP_SERVICE_MASK)
#endif

#ifndef BTIF_HF_SERVICE_NAMES
#define BTIF_HF_SERVICE_NAMES \
  { BTIF_HSAG_SERVICE_NAME, BTIF_HFAG_SERVICE_NAME }
#endif

#ifndef BTIF_HF_SECURITY
#define BTIF_HF_SECURITY (BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT)
#endif

#ifndef BTIF_HF_FEATURES
#define BTIF_HF_FEATURES                                       \
  (BTA_AG_FEAT_3WAY | BTA_AG_FEAT_ECNR | BTA_AG_FEAT_REJECT |  \
   BTA_AG_FEAT_ECS | BTA_AG_FEAT_EXTERR | BTA_AG_FEAT_VREC |   \
   BTA_AG_FEAT_CODEC | BTA_AG_FEAT_HF_IND | BTA_AG_FEAT_ESCO | \
   BTA_AG_FEAT_UNAT)
#endif

/* HF features supported at runtime */
static uint32_t btif_hf_features = BTIF_HF_FEATURES;

#define BTIF_HF_INVALID_IDX (-1)

/* Max HF clients supported from App */
static int btif_max_hf_clients = 1;
static RawAddress active_bda = {};

/*******************************************************************************
 *  Static variables
 ******************************************************************************/
static Callbacks* bt_hf_callbacks = nullptr;

#define CHECK_BTHF_INIT()                                             \
  do {                                                                \
    if (!bt_hf_callbacks) {                                           \
      BTIF_TRACE_WARNING("BTHF: %s: BTHF not initialized", __func__); \
      return BT_STATUS_NOT_READY;                                     \
    } else {                                                          \
      BTIF_TRACE_EVENT("BTHF: %s", __func__);                         \
    }                                                                 \
  } while (false)

/* BTIF-HF control block to map bdaddr to BTA handle */
struct btif_hf_cb_t {
  uint16_t handle;
  bool is_initiator;
  RawAddress connected_bda;
  bthf_connection_state_t state;
  tBTA_AG_PEER_FEAT peer_feat;
  int num_active;
  int num_held;
  bthf_call_state_t call_setup_state;
};

static btif_hf_cb_t btif_hf_cb[BTA_AG_MAX_NUM_CLIENTS];

static const char* dump_hf_call_state(bthf_call_state_t call_state) {
  switch (call_state) {
    CASE_RETURN_STR(BTHF_CALL_STATE_IDLE)
    CASE_RETURN_STR(BTHF_CALL_STATE_HELD)
    CASE_RETURN_STR(BTHF_CALL_STATE_DIALING)
    CASE_RETURN_STR(BTHF_CALL_STATE_ALERTING)
    CASE_RETURN_STR(BTHF_CALL_STATE_INCOMING)
    CASE_RETURN_STR(BTHF_CALL_STATE_WAITING)
    CASE_RETURN_STR(BTHF_CALL_STATE_ACTIVE)
    CASE_RETURN_STR(BTHF_CALL_STATE_DISCONNECTED)
    default:
      return "UNKNOWN CALL STATE";
  }
}

/**
 * Check if bd_addr is the current active device.
 *
 * @param bd_addr target device address
 * @return True if bd_addr is the current active device, False otherwise or if
 * no active device is set (i.e. active_device_addr is empty)
 */
static bool is_active_device(const RawAddress& bd_addr) {
  return !active_bda.IsEmpty() && active_bda == bd_addr;
}

/*******************************************************************************
 *
 * Function         is_connected
 *
 * Description      Internal function to check if HF is connected
 *                  is_connected(nullptr) returns TRUE if one of the control
 *                  blocks is connected
 *
 * Returns          true if connected
 *
 ******************************************************************************/
static bool is_connected(RawAddress* bd_addr) {
  for (int i = 0; i < btif_max_hf_clients; ++i) {
    if (((btif_hf_cb[i].state == BTHF_CONNECTION_STATE_CONNECTED) ||
         (btif_hf_cb[i].state == BTHF_CONNECTION_STATE_SLC_CONNECTED)) &&
        (!bd_addr || *bd_addr == btif_hf_cb[i].connected_bda))
      return true;
  }
  return false;
}

/*******************************************************************************
 *
 * Function         btif_hf_idx_by_bdaddr
 *
 * Description      Internal function to get idx by bdaddr
 *
 * Returns          idx
 *
 ******************************************************************************/
static int btif_hf_idx_by_bdaddr(RawAddress* bd_addr) {
  for (int i = 0; i < btif_max_hf_clients; ++i) {
    if (*bd_addr == btif_hf_cb[i].connected_bda) return i;
  }
  return BTIF_HF_INVALID_IDX;
}

/*******************************************************************************
 *
 * Function         callstate_to_callsetup
 *
 * Description      Converts HAL call state to BTA call setup indicator value
 *
 * Returns          BTA call indicator value
 *
 ******************************************************************************/
static uint8_t callstate_to_callsetup(bthf_call_state_t call_state) {
  switch (call_state) {
    case BTHF_CALL_STATE_INCOMING:
      return 1;
    case BTHF_CALL_STATE_DIALING:
      return 2;
    case BTHF_CALL_STATE_ALERTING:
      return 3;
    default:
      return 0;
  }
}

/*******************************************************************************
 *
 * Function         send_at_result
 *
 * Description      Send AT result code (OK/ERROR)
 *
 * Returns          void
 *
 ******************************************************************************/
static void send_at_result(uint8_t ok_flag, uint16_t errcode, int idx) {
  tBTA_AG_RES_DATA ag_res = {};
  ag_res.ok_flag = ok_flag;
  if (ok_flag == BTA_AG_OK_ERROR) {
    ag_res.errcode = errcode;
  }
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_UNAT_RES, ag_res);
}

/*******************************************************************************
 *
 * Function         send_indicator_update
 *
 * Description      Send indicator update (CIEV)
 *
 * Returns          void
 *
 ******************************************************************************/
static void send_indicator_update(const btif_hf_cb_t& control_block,
                                  uint16_t indicator, uint16_t value) {
  tBTA_AG_RES_DATA ag_res = {};
  ag_res.ind.id = indicator;
  ag_res.ind.value = value;
  BTA_AgResult(control_block.handle, BTA_AG_IND_RES, ag_res);
}

static bool is_nth_bit_enabled(uint32_t value, int n) {
  return (value & (static_cast<uint32_t>(1) << n)) != 0;
}

void clear_phone_state_multihf(btif_hf_cb_t* hf_cb) {
  hf_cb->call_setup_state = BTHF_CALL_STATE_IDLE;
  hf_cb->num_active = 0;
  hf_cb->num_held = 0;
}

static void reset_control_block(btif_hf_cb_t* hf_cb) {
  hf_cb->state = BTHF_CONNECTION_STATE_DISCONNECTED;
  hf_cb->is_initiator = false;
  hf_cb->connected_bda = RawAddress::kEmpty;
  hf_cb->peer_feat = 0;
  clear_phone_state_multihf(hf_cb);
}

/**
 * Check if Service Level Connection (SLC) is established for bd_addr
 *
 * @param bd_addr remote device address
 * @return true if SLC is established for bd_addr
 */
static bool IsSlcConnected(RawAddress* bd_addr) {
  if (!bd_addr) {
    LOG(WARNING) << __func__ << ": bd_addr is null";
    return false;
  }
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if (idx < 0 || idx > BTA_AG_MAX_NUM_CLIENTS) {
    LOG(WARNING) << __func__ << ": invalid index " << idx << " for "
                 << *bd_addr;
    return false;
  }
  return btif_hf_cb[idx].state == BTHF_CONNECTION_STATE_SLC_CONNECTED;
}

/*******************************************************************************
 *
 * Function         btif_hf_upstreams_evt
 *
 * Description      Executes HF UPSTREAMS events in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_hf_upstreams_evt(uint16_t event, char* p_param) {
  if (event == BTA_AG_ENABLE_EVT || event == BTA_AG_DISABLE_EVT) {
    LOG(INFO) << __func__ << ": AG enable/disable event " << event;
    return;
  }
  if (p_param == nullptr) {
    LOG(ERROR) << __func__ << ": parameter is null";
    return;
  }
  tBTA_AG* p_data = (tBTA_AG*)p_param;
  int idx = p_data->hdr.handle - 1;

  BTIF_TRACE_DEBUG("%s: event=%s", __func__, dump_hf_event(event));

  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return;
  }
  if (!bt_hf_callbacks) {
    BTIF_TRACE_ERROR("%s: Headset callback is NULL", __func__);
    return;
  }

  switch (event) {
    case BTA_AG_REGISTER_EVT:
      btif_hf_cb[idx].handle = p_data->reg.hdr.handle;
      BTIF_TRACE_DEBUG("%s: BTA_AG_REGISTER_EVT, btif_hf_cb.handle = %d",
                       __func__, btif_hf_cb[idx].handle);
      break;
    // RFCOMM connected or failed to connect
    case BTA_AG_OPEN_EVT:
      // Check if an outoging connection is pending
      if (btif_hf_cb[idx].is_initiator) {
        CHECK_EQ(btif_hf_cb[idx].state, BTHF_CONNECTION_STATE_CONNECTING)
            << "Control block must be in connecting state when initiating";
        CHECK(!btif_hf_cb[idx].connected_bda.IsEmpty())
            << "Remote device address must not be empty when initiating";
        CHECK_EQ(btif_hf_cb[idx].connected_bda, p_data->open.bd_addr)
            << "Incoming message's address must match expected one";
      }
      if (p_data->open.status == BTA_AG_SUCCESS) {
        // In case this is an incoming connection
        btif_hf_cb[idx].connected_bda = p_data->open.bd_addr;
        btif_hf_cb[idx].state = BTHF_CONNECTION_STATE_CONNECTED;
        btif_hf_cb[idx].peer_feat = 0;
        clear_phone_state_multihf(&btif_hf_cb[idx]);
        bluetooth::common::BluetoothMetricsLogger::GetInstance()
            ->LogHeadsetProfileRfcConnection(p_data->open.service_id);
        bt_hf_callbacks->ConnectionStateCallback(
            btif_hf_cb[idx].state, &btif_hf_cb[idx].connected_bda);
      } else {
        if (!btif_hf_cb[idx].is_initiator) {
          // Ignore remote initiated open failures
          LOG(WARNING) << __func__ << ": Unexpected AG open failure "
                       << std::to_string(p_data->open.status) << " for "
                       << p_data->open.bd_addr << " is ignored";
          break;
        }
        LOG(ERROR) << __func__ << ": self initiated AG open failed for "
                   << btif_hf_cb[idx].connected_bda << ", status "
                   << std::to_string(p_data->open.status);
        RawAddress connected_bda = btif_hf_cb[idx].connected_bda;
        reset_control_block(&btif_hf_cb[idx]);
        bt_hf_callbacks->ConnectionStateCallback(btif_hf_cb[idx].state,
                                                 &connected_bda);
        btif_queue_advance();
      }
      break;
    // SLC and RFCOMM both disconnected
    case BTA_AG_CLOSE_EVT: {
      BTIF_TRACE_DEBUG("%s: BTA_AG_CLOSE_EVT, idx = %d, btif_hf_cb.handle = %d",
                       __func__, idx, btif_hf_cb[idx].handle);
      // If AG_OPEN was received but SLC was not connected in time, then
      // AG_CLOSE may be received. We need to advance the queue here.
      bool failed_to_setup_slc =
          (btif_hf_cb[idx].state != BTHF_CONNECTION_STATE_SLC_CONNECTED) &&
          btif_hf_cb[idx].is_initiator;
      RawAddress connected_bda = btif_hf_cb[idx].connected_bda;
      reset_control_block(&btif_hf_cb[idx]);
      bt_hf_callbacks->ConnectionStateCallback(btif_hf_cb[idx].state,
                                               &connected_bda);
      if (failed_to_setup_slc) {
        LOG(ERROR) << __func__ << ": failed to setup SLC for " << connected_bda;
        btif_queue_advance();
      }
      break;
    }
    // SLC connected
    case BTA_AG_CONN_EVT:
      BTIF_TRACE_DEBUG("%s: BTA_AG_CONN_EVT, idx = %d ", __func__, idx);
      btif_hf_cb[idx].peer_feat = p_data->conn.peer_feat;
      btif_hf_cb[idx].state = BTHF_CONNECTION_STATE_SLC_CONNECTED;
      bt_hf_callbacks->ConnectionStateCallback(btif_hf_cb[idx].state,
                                               &btif_hf_cb[idx].connected_bda);
      if (btif_hf_cb[idx].is_initiator) {
        btif_queue_advance();
      }
      break;

    case BTA_AG_AUDIO_OPEN_EVT:
      bt_hf_callbacks->AudioStateCallback(BTHF_AUDIO_STATE_CONNECTED,
                                          &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AUDIO_CLOSE_EVT:
      bt_hf_callbacks->AudioStateCallback(BTHF_AUDIO_STATE_DISCONNECTED,
                                          &btif_hf_cb[idx].connected_bda);
      break;

    /* BTA auto-responds, silently discard */
    case BTA_AG_SPK_EVT:
    case BTA_AG_MIC_EVT:
      bt_hf_callbacks->VolumeControlCallback(
          (event == BTA_AG_SPK_EVT) ? BTHF_VOLUME_TYPE_SPK
                                    : BTHF_VOLUME_TYPE_MIC,
          p_data->val.num, &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_A_EVT:
      bt_hf_callbacks->AnswerCallCallback(&btif_hf_cb[idx].connected_bda);
      break;

    /* Java needs to send OK/ERROR for these commands */
    case BTA_AG_AT_BLDN_EVT:
    case BTA_AG_AT_D_EVT:
      bt_hf_callbacks->DialCallCallback(
          (event == BTA_AG_AT_D_EVT) ? p_data->val.str : (char*)"",
          &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_CHUP_EVT:
      bt_hf_callbacks->HangupCallCallback(&btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_CIND_EVT:
      bt_hf_callbacks->AtCindCallback(&btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_VTS_EVT:
      bt_hf_callbacks->DtmfCmdCallback(p_data->val.str[0],
                                       &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_BVRA_EVT:
      bt_hf_callbacks->VoiceRecognitionCallback((p_data->val.num == 1)
                                                    ? BTHF_VR_STATE_STARTED
                                                    : BTHF_VR_STATE_STOPPED,
                                                &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_NREC_EVT:
      bt_hf_callbacks->NoiseReductionCallback(
          (p_data->val.num == 1) ? BTHF_NREC_START : BTHF_NREC_STOP,
          &btif_hf_cb[idx].connected_bda);
      break;

    /* TODO: Add a callback for CBC */
    case BTA_AG_AT_CBC_EVT:
      break;

    case BTA_AG_AT_CKPD_EVT:
      bt_hf_callbacks->KeyPressedCallback(&btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_WBS_EVT:
      BTIF_TRACE_DEBUG(
          "BTA_AG_WBS_EVT Set codec status %d codec %d 1=CVSD 2=MSBC",
          p_data->val.hdr.status, p_data->val.num);
      if (p_data->val.num == BTA_AG_CODEC_CVSD) {
        bt_hf_callbacks->WbsCallback(BTHF_WBS_NO,
                                     &btif_hf_cb[idx].connected_bda);
      } else if (p_data->val.num == BTA_AG_CODEC_MSBC) {
        bt_hf_callbacks->WbsCallback(BTHF_WBS_YES,
                                     &btif_hf_cb[idx].connected_bda);
      } else {
        bt_hf_callbacks->WbsCallback(BTHF_WBS_NONE,
                                     &btif_hf_cb[idx].connected_bda);
      }
      break;

    /* Java needs to send OK/ERROR for these commands */
    case BTA_AG_AT_CHLD_EVT:
      bt_hf_callbacks->AtChldCallback((bthf_chld_type_t)atoi(p_data->val.str),
                                      &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_CLCC_EVT:
      bt_hf_callbacks->AtClccCallback(&btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_COPS_EVT:
      bt_hf_callbacks->AtCopsCallback(&btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_UNAT_EVT:
      bt_hf_callbacks->UnknownAtCallback(p_data->val.str,
                                         &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_CNUM_EVT:
      bt_hf_callbacks->AtCnumCallback(&btif_hf_cb[idx].connected_bda);
      break;

    /* TODO: Some of these commands may need to be sent to app. For now respond
     * with error */
    case BTA_AG_AT_BINP_EVT:
    case BTA_AG_AT_BTRH_EVT:
      send_at_result(BTA_AG_OK_ERROR, BTA_AG_ERR_OP_NOT_SUPPORTED, idx);
      break;
    case BTA_AG_AT_BAC_EVT:
      BTIF_TRACE_DEBUG("AG Bitmap of peer-codecs %d", p_data->val.num);
      /* If the peer supports mSBC and the BTIF preferred codec is also mSBC,
      then
      we should set the BTA AG Codec to mSBC. This would trigger a +BCS to mSBC
      at the time
      of SCO connection establishment */
      if (p_data->val.num & BTA_AG_CODEC_MSBC) {
        BTIF_TRACE_EVENT("%s: btif_hf override-Preferred Codec to MSBC",
                         __func__);
        BTA_AgSetCodec(btif_hf_cb[idx].handle, BTA_AG_CODEC_MSBC);
      } else {
        BTIF_TRACE_EVENT("%s btif_hf override-Preferred Codec to CVSD",
                         __func__);
        BTA_AgSetCodec(btif_hf_cb[idx].handle, BTA_AG_CODEC_CVSD);
      }
      break;
    case BTA_AG_AT_BCS_EVT:
      BTIF_TRACE_DEBUG("%s: AG final selected codec is 0x%02x 1=CVSD 2=MSBC",
                       __func__, p_data->val.num);
      /* No BTHF_WBS_NONE case, because HF1.6 supported device can send BCS */
      /* Only CVSD is considered narrow band speech */
      bt_hf_callbacks->WbsCallback(
          (p_data->val.num == BTA_AG_CODEC_CVSD) ? BTHF_WBS_NO : BTHF_WBS_YES,
          &btif_hf_cb[idx].connected_bda);
      break;

    case BTA_AG_AT_BIND_EVT:
      if (p_data->val.hdr.status == BTA_AG_SUCCESS) {
        bt_hf_callbacks->AtBindCallback(p_data->val.str,
                                        &btif_hf_cb[idx].connected_bda);
      }
      break;

    case BTA_AG_AT_BIEV_EVT:
      if (p_data->val.hdr.status == BTA_AG_SUCCESS) {
        bt_hf_callbacks->AtBievCallback((bthf_hf_ind_type_t)p_data->val.lidx,
                                        (int)p_data->val.num,
                                        &btif_hf_cb[idx].connected_bda);
      }
      break;
    case BTA_AG_AT_BIA_EVT:
      if (p_data->val.hdr.status == BTA_AG_SUCCESS) {
        uint32_t bia_mask_out = p_data->val.num;
        bool service = !is_nth_bit_enabled(bia_mask_out, BTA_AG_IND_SERVICE);
        bool roam = !is_nth_bit_enabled(bia_mask_out, BTA_AG_IND_ROAM);
        bool signal = !is_nth_bit_enabled(bia_mask_out, BTA_AG_IND_SIGNAL);
        bool battery = !is_nth_bit_enabled(bia_mask_out, BTA_AG_IND_BATTCHG);
        bt_hf_callbacks->AtBiaCallback(service, roam, signal, battery,
                                       &btif_hf_cb[idx].connected_bda);
      }
      break;
    default:
      LOG(WARNING) << __func__ << ": unhandled event " << event;
      break;
  }
}

/*******************************************************************************
 *
 * Function         bte_hf_evt
 *
 * Description      Switches context from BTE to BTIF for all HF events
 *
 * Returns          void
 *
 ******************************************************************************/

static void bte_hf_evt(tBTA_AG_EVT event, tBTA_AG* p_data) {
  bt_status_t status;
  int param_len = 0;

  /* TODO: BTA sends the union members and not tBTA_AG. If using
   * param_len=sizeof(tBTA_AG), we get a crash on memcpy */
  if (BTA_AG_REGISTER_EVT == event)
    param_len = sizeof(tBTA_AG_REGISTER);
  else if (BTA_AG_OPEN_EVT == event)
    param_len = sizeof(tBTA_AG_OPEN);
  else if (BTA_AG_CONN_EVT == event)
    param_len = sizeof(tBTA_AG_CONN);
  else if ((BTA_AG_CLOSE_EVT == event) || (BTA_AG_AUDIO_OPEN_EVT == event) ||
           (BTA_AG_AUDIO_CLOSE_EVT == event))
    param_len = sizeof(tBTA_AG_HDR);
  else if (p_data)
    param_len = sizeof(tBTA_AG_VAL);

  /* switch context to btif task context (copy full union size for convenience)
   */
  status = btif_transfer_context(btif_hf_upstreams_evt, (uint16_t)event,
                                 (char*)p_data, param_len, nullptr);

  /* catch any failed context transfers */
  ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}

/*******************************************************************************
 *
 * Function         connect
 *
 * Description     connect to headset
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t connect_int(RawAddress* bd_addr, uint16_t uuid) {
  CHECK_BTHF_INIT();
  if (is_connected(bd_addr)) {
    BTIF_TRACE_WARNING("%s: device %s is already connected", __func__,
                       bd_addr->ToString().c_str());
    return BT_STATUS_BUSY;
  }
  btif_hf_cb_t* hf_cb = nullptr;
  for (int i = 0; i < btif_max_hf_clients; i++) {
    if (btif_hf_cb[i].state == BTHF_CONNECTION_STATE_DISCONNECTED) {
      hf_cb = &btif_hf_cb[i];
      break;
    }
    // Due to btif queue implementation, when connect_int is called, no btif
    // control block should be in connecting state
    // Crash here to prevent future code changes from breaking this mechanism
    if (btif_hf_cb[i].state == BTHF_CONNECTION_STATE_CONNECTING) {
      LOG(FATAL) << __func__ << ": " << btif_hf_cb[i].connected_bda
                 << ", handle " << btif_hf_cb[i].handle
                 << ", is still in connecting state " << btif_hf_cb[i].state;
    }
  }
  if (hf_cb == nullptr) {
    BTIF_TRACE_WARNING(
        "%s: Cannot connect %s: maximum %d clients already connected", __func__,
        bd_addr->ToString().c_str(), btif_max_hf_clients);
    return BT_STATUS_BUSY;
  }
  hf_cb->state = BTHF_CONNECTION_STATE_CONNECTING;
  hf_cb->connected_bda = *bd_addr;
  hf_cb->is_initiator = true;
  hf_cb->peer_feat = 0;
  BTA_AgOpen(hf_cb->handle, hf_cb->connected_bda, BTIF_HF_SECURITY);
  return BT_STATUS_SUCCESS;
}

static void UpdateCallStates(btif_hf_cb_t* control_block, int num_active,
                             int num_held, bthf_call_state_t call_setup_state) {
  control_block->num_active = num_active;
  control_block->num_held = num_held;
  control_block->call_setup_state = call_setup_state;
}

/*******************************************************************************
 *
 * Function         btif_hf_is_call_idle
 *
 * Description      returns true if no call is in progress
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bool IsCallIdle() {
  if (!bt_hf_callbacks) return true;

  for (int i = 0; i < btif_max_hf_clients; ++i) {
    if ((btif_hf_cb[i].call_setup_state != BTHF_CALL_STATE_IDLE) ||
        ((btif_hf_cb[i].num_held + btif_hf_cb[i].num_active) > 0))
      return false;
  }

  return true;
}

class HeadsetInterface : Interface {
 public:
  static Interface* GetInstance() {
    static Interface* instance = new HeadsetInterface();
    return instance;
  }
  bt_status_t Init(Callbacks* callbacks, int max_hf_clients,
                   bool inband_ringing_enabled) override;
  bt_status_t Connect(RawAddress* bd_addr) override;
  bt_status_t Disconnect(RawAddress* bd_addr) override;
  bt_status_t ConnectAudio(RawAddress* bd_addr) override;
  bt_status_t DisconnectAudio(RawAddress* bd_addr) override;
  bt_status_t StartVoiceRecognition(RawAddress* bd_addr) override;
  bt_status_t StopVoiceRecognition(RawAddress* bd_addr) override;
  bt_status_t VolumeControl(bthf_volume_type_t type, int volume,
                            RawAddress* bd_addr) override;
  bt_status_t DeviceStatusNotification(bthf_network_state_t ntk_state,
                                       bthf_service_type_t svc_type, int signal,
                                       int batt_chg,
                                       RawAddress* bd_addr) override;
  bt_status_t CopsResponse(const char* cops, RawAddress* bd_addr) override;
  bt_status_t CindResponse(int svc, int num_active, int num_held,
                           bthf_call_state_t call_setup_state, int signal,
                           int roam, int batt_chg,
                           RawAddress* bd_addr) override;
  bt_status_t FormattedAtResponse(const char* rsp,
                                  RawAddress* bd_addr) override;
  bt_status_t AtResponse(bthf_at_response_t response_code, int error_code,
                         RawAddress* bd_addr) override;
  bt_status_t ClccResponse(int index, bthf_call_direction_t dir,
                           bthf_call_state_t state, bthf_call_mode_t mode,
                           bthf_call_mpty_type_t mpty, const char* number,
                           bthf_call_addrtype_t type,
                           RawAddress* bd_addr) override;
  bt_status_t PhoneStateChange(int num_active, int num_held,
                               bthf_call_state_t call_setup_state,
                               const char* number, bthf_call_addrtype_t type,
                               const char* name, RawAddress* bd_addr) override;

  void Cleanup() override;
  bt_status_t SetScoAllowed(bool value) override;
  bt_status_t SendBsir(bool value, RawAddress* bd_addr) override;
  bt_status_t SetActiveDevice(RawAddress* active_device_addr) override;
};

bt_status_t HeadsetInterface::Init(Callbacks* callbacks, int max_hf_clients,
                                   bool inband_ringing_enabled) {
  if (inband_ringing_enabled) {
    btif_hf_features |= BTA_AG_FEAT_INBAND;
  } else {
    btif_hf_features &= ~BTA_AG_FEAT_INBAND;
  }
  CHECK_LE(max_hf_clients, BTA_AG_MAX_NUM_CLIENTS)
      << __func__
      << "Too many HF clients,"
         " maximum is "
      << BTA_AG_MAX_NUM_CLIENTS << " was given " << max_hf_clients;
  btif_max_hf_clients = max_hf_clients;
  BTIF_TRACE_DEBUG(
      "%s: btif_hf_features=%zu, max_hf_clients=%d, inband_ringing_enabled=%d",
      __func__, btif_hf_features, btif_max_hf_clients, inband_ringing_enabled);
  bt_hf_callbacks = callbacks;
  for (btif_hf_cb_t& hf_cb : btif_hf_cb) {
    reset_control_block(&hf_cb);
  }

// Invoke the enable service API to the core to set the appropriate service_id
// Internally, the HSP_SERVICE_ID shall also be enabled if HFP is enabled
// (phone) otherwise only HSP is enabled (tablet)
#if (defined(BTIF_HF_SERVICES) && (BTIF_HF_SERVICES & BTA_HFP_SERVICE_MASK))
  btif_enable_service(BTA_HFP_SERVICE_ID);
#else
  btif_enable_service(BTA_HSP_SERVICE_ID);
#endif

  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::Connect(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  return btif_queue_connect(UUID_SERVCLASS_AG_HANDSFREE, bd_addr, connect_int);
}

bt_status_t HeadsetInterface::Disconnect(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  BTA_AgClose(btif_hf_cb[idx].handle);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::ConnectAudio(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  /* Check if SLC is connected */
  if (!IsSlcConnected(bd_addr)) {
    LOG(ERROR) << ": SLC not connected for " << *bd_addr;
    return BT_STATUS_NOT_READY;
  }
  do_in_jni_thread(base::Bind(&Callbacks::AudioStateCallback,
                              // Manual pointer management for now
                              base::Unretained(bt_hf_callbacks),
                              BTHF_AUDIO_STATE_CONNECTING,
                              &btif_hf_cb[idx].connected_bda));
  BTA_AgAudioOpen(btif_hf_cb[idx].handle);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::DisconnectAudio(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  BTA_AgAudioClose(btif_hf_cb[idx].handle);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::StartVoiceRecognition(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_NOT_READY;
  }
  if (!(btif_hf_cb[idx].peer_feat & BTA_AG_PEER_FEAT_VREC)) {
    BTIF_TRACE_ERROR("%s: voice recognition not supported, features=0x%x",
                     __func__, btif_hf_cb[idx].peer_feat);
    return BT_STATUS_UNSUPPORTED;
  }
  tBTA_AG_RES_DATA ag_res = {};
  ag_res.state = true;
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_BVRA_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::StopVoiceRecognition(RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);

  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_NOT_READY;
  }
  if (!(btif_hf_cb[idx].peer_feat & BTA_AG_PEER_FEAT_VREC)) {
    BTIF_TRACE_ERROR("%s: voice recognition not supported, features=0x%x",
                     __func__, btif_hf_cb[idx].peer_feat);
    return BT_STATUS_UNSUPPORTED;
  }
  tBTA_AG_RES_DATA ag_res = {};
  ag_res.state = false;
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_BVRA_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::VolumeControl(bthf_volume_type_t type, int volume,
                                            RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  tBTA_AG_RES_DATA ag_res = {};
  ag_res.num = static_cast<uint16_t>(volume);
  BTA_AgResult(btif_hf_cb[idx].handle,
               (type == BTHF_VOLUME_TYPE_SPK) ? BTA_AG_SPK_RES : BTA_AG_MIC_RES,
               ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::DeviceStatusNotification(
    bthf_network_state_t ntk_state, bthf_service_type_t svc_type, int signal,
    int batt_chg, RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  if (!bd_addr) {
    BTIF_TRACE_WARNING("%s: bd_addr is null", __func__);
    return BT_STATUS_FAIL;
  }
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if (idx < 0 || idx > BTA_AG_MAX_NUM_CLIENTS) {
    BTIF_TRACE_WARNING("%s: invalid index %d for %s", __func__, idx,
                       bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  const btif_hf_cb_t& control_block = btif_hf_cb[idx];
  // ok if no device is connected
  if (is_connected(nullptr)) {
    // send all indicators to BTA.
    // BTA will make sure no duplicates are sent out
    send_indicator_update(control_block, BTA_AG_IND_SERVICE,
                          (ntk_state == BTHF_NETWORK_STATE_AVAILABLE) ? 1 : 0);
    send_indicator_update(control_block, BTA_AG_IND_ROAM,
                          (svc_type == BTHF_SERVICE_TYPE_HOME) ? 0 : 1);
    send_indicator_update(control_block, BTA_AG_IND_SIGNAL, signal);
    send_indicator_update(control_block, BTA_AG_IND_BATTCHG, batt_chg);
  }
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::CopsResponse(const char* cops,
                                           RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  tBTA_AG_RES_DATA ag_res = {};
  /* Format the response */
  snprintf(ag_res.str, sizeof(ag_res.str), "0,0,\"%.16s\"", cops);
  ag_res.ok_flag = BTA_AG_OK_DONE;
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_COPS_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::CindResponse(int svc, int num_active,
                                           int num_held,
                                           bthf_call_state_t call_setup_state,
                                           int signal, int roam, int batt_chg,
                                           RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  tBTA_AG_RES_DATA ag_res = {};
  // per the errata 2043, call=1 implies atleast one call is in progress
  // (active/held), see:
  // https://www.bluetooth.org/errata/errata_view.cfm?errata_id=2043
  snprintf(ag_res.str, sizeof(ag_res.str), "%d,%d,%d,%d,%d,%d,%d",
           (num_active + num_held) ? 1 : 0,          /* Call state */
           callstate_to_callsetup(call_setup_state), /* Callsetup state */
           svc,                                      /* network service */
           signal,                                   /* Signal strength */
           roam,                                     /* Roaming indicator */
           batt_chg,                                 /* Battery level */
           ((num_held == 0) ? 0 : ((num_active == 0) ? 2 : 1))); /* Call held */
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_CIND_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::FormattedAtResponse(const char* rsp,
                                                  RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  tBTA_AG_RES_DATA ag_res = {};
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  /* Format the response and send */
  strncpy(ag_res.str, rsp, BTA_AG_AT_MAX_LEN);
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_UNAT_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::AtResponse(bthf_at_response_t response_code,
                                         int error_code, RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  send_at_result(
      (response_code == BTHF_AT_RESPONSE_OK) ? BTA_AG_OK_DONE : BTA_AG_OK_ERROR,
      static_cast<uint16_t>(error_code), idx);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::ClccResponse(
    int index, bthf_call_direction_t dir, bthf_call_state_t state,
    bthf_call_mode_t mode, bthf_call_mpty_type_t mpty, const char* number,
    bthf_call_addrtype_t type, RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d", __func__, idx);
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s is not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  tBTA_AG_RES_DATA ag_res = {};
  /* Format the response */
  if (index == 0) {
    ag_res.ok_flag = BTA_AG_OK_DONE;
  } else {
    BTIF_TRACE_EVENT(
        "clcc_response: [%d] dir %d state %d mode %d number = %s type = %d",
        index, dir, state, mode, number, type);
    int res_strlen = snprintf(ag_res.str, sizeof(ag_res.str), "%d,%d,%d,%d,%d",
                              index, dir, state, mode, mpty);
    if (number) {
      size_t rem_bytes = sizeof(ag_res.str) - res_strlen;
      char dialnum[sizeof(ag_res.str)];
      size_t newidx = 0;
      if (type == BTHF_CALL_ADDRTYPE_INTERNATIONAL && *number != '+') {
        dialnum[newidx++] = '+';
      }
      for (size_t i = 0; number[i] != 0; i++) {
        if (newidx >= (sizeof(dialnum) - res_strlen - 1)) {
          android_errorWriteLog(0x534e4554, "79266386");
          break;
        }
        if (utl_isdialchar(number[i])) {
          dialnum[newidx++] = number[i];
        }
      }
      dialnum[newidx] = 0;
      // Reserve 5 bytes for ["][,][3_digit_type]
      snprintf(&ag_res.str[res_strlen], rem_bytes - 5, ",\"%s", dialnum);
      std::stringstream remaining_string;
      remaining_string << "\"," << type;
      strncat(&ag_res.str[res_strlen], remaining_string.str().c_str(), 5);
    }
  }
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_CLCC_RES, ag_res);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::PhoneStateChange(
    int num_active, int num_held, bthf_call_state_t call_setup_state,
    const char* number, bthf_call_addrtype_t type, const char* name,
    RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  if (!bd_addr) {
    BTIF_TRACE_WARNING("%s: bd_addr is null", __func__);
    return BT_STATUS_FAIL;
  }
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if (idx < 0 || idx > BTA_AG_MAX_NUM_CLIENTS) {
    BTIF_TRACE_WARNING("%s: invalid index %d for %s", __func__, idx,
                       bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  const btif_hf_cb_t& control_block = btif_hf_cb[idx];
  if (!IsSlcConnected(bd_addr)) {
    LOG(WARNING) << ": SLC not connected for " << *bd_addr;
    return BT_STATUS_NOT_READY;
  }
  if (call_setup_state == BTHF_CALL_STATE_DISCONNECTED) {
    // HFP spec does not handle cases when a call is being disconnected.
    // Since DISCONNECTED state must lead to IDLE state, ignoring it here.s
    LOG(INFO) << __func__
              << ": Ignore call state change to DISCONNECTED, idx=" << idx
              << ", addr=" << *bd_addr << ", num_active=" << num_active
              << ", num_held=" << num_held;
    return BT_STATUS_SUCCESS;
  }
  LOG(INFO) << __func__ << ": idx=" << idx << ", addr=" << *bd_addr
            << ", active_bda=" << active_bda << ", num_active=" << num_active
            << ", prev_num_active" << control_block.num_active
            << ", num_held=" << num_held
            << ", prev_num_held=" << control_block.num_held
            << ", call_state=" << dump_hf_call_state(call_setup_state)
            << ", prev_call_state="
            << dump_hf_call_state(control_block.call_setup_state);
  tBTA_AG_RES res = 0xFF;
  bt_status_t status = BT_STATUS_SUCCESS;
  bool active_call_updated = false;

  /* if all indicators are 0, send end call and return */
  if (num_active == 0 && num_held == 0 &&
      call_setup_state == BTHF_CALL_STATE_IDLE) {
    VLOG(1) << __func__ << ": call ended";
    BTA_AgResult(control_block.handle, BTA_AG_END_CALL_RES,
                 tBTA_AG_RES_DATA::kEmpty);
    /* if held call was present, reset that as well */
    if (control_block.num_held) {
      send_indicator_update(control_block, BTA_AG_IND_CALLHELD, 0);
    }
    UpdateCallStates(&btif_hf_cb[idx], num_active, num_held, call_setup_state);
    return status;
  }

  /* active state can change when:
  ** 1. an outgoing/incoming call was answered
  ** 2. an held was resumed
  ** 3. without callsetup notifications, call became active
  ** (3) can happen if call is active and a headset connects to us
  **
  ** In the case of (3), we will have to notify the stack of an active
  ** call, instead of sending an indicator update. This will also
  ** force the SCO to be setup. Handle this special case here prior to
  ** call setup handling
  */
  if (((num_active + num_held) > 0) && (control_block.num_active == 0) &&
      (control_block.num_held == 0) &&
      (control_block.call_setup_state == BTHF_CALL_STATE_IDLE)) {
    tBTA_AG_RES_DATA ag_res = {};
    BTIF_TRACE_DEBUG(
        "%s: Active/Held call notification received without call setup "
        "update",
        __func__);

    ag_res.audio_handle = BTA_AG_HANDLE_SCO_NO_CHANGE;
    // Addition call setup with the Active call
    // CIND response should have been updated.
    // just open SCO connection.
    if (call_setup_state != BTHF_CALL_STATE_IDLE) {
      res = BTA_AG_MULTI_CALL_RES;
    } else {
      res = BTA_AG_OUT_CALL_CONN_RES;
    }
    BTA_AgResult(control_block.handle, res, ag_res);
    active_call_updated = true;
  }

  /* Ringing call changed? */
  if (call_setup_state != control_block.call_setup_state) {
    tBTA_AG_RES_DATA ag_res = {};
    ag_res.audio_handle = BTA_AG_HANDLE_SCO_NO_CHANGE;
    BTIF_TRACE_DEBUG("%s: Call setup states changed. old: %s new: %s", __func__,
                     dump_hf_call_state(control_block.call_setup_state),
                     dump_hf_call_state(call_setup_state));
    switch (call_setup_state) {
      case BTHF_CALL_STATE_IDLE: {
        switch (control_block.call_setup_state) {
          case BTHF_CALL_STATE_INCOMING:
            if (num_active > control_block.num_active) {
              res = BTA_AG_IN_CALL_CONN_RES;
              if (is_active_device(*bd_addr)) {
                ag_res.audio_handle = control_block.handle;
              }
            } else if (num_held > control_block.num_held)
              res = BTA_AG_IN_CALL_HELD_RES;
            else
              res = BTA_AG_CALL_CANCEL_RES;
            break;
          case BTHF_CALL_STATE_DIALING:
          case BTHF_CALL_STATE_ALERTING:
            if (num_active > control_block.num_active) {
              res = BTA_AG_OUT_CALL_CONN_RES;
            } else
              res = BTA_AG_CALL_CANCEL_RES;
            break;
          default:
            BTIF_TRACE_ERROR("%s: Incorrect call state prev=%d, now=%d",
                             __func__, control_block.call_setup_state,
                             call_setup_state);
            status = BT_STATUS_PARM_INVALID;
            break;
        }
      } break;

      case BTHF_CALL_STATE_INCOMING:
        if (num_active || num_held) {
          res = BTA_AG_CALL_WAIT_RES;
        } else {
          res = BTA_AG_IN_CALL_RES;
          if (is_active_device(*bd_addr)) {
            ag_res.audio_handle = control_block.handle;
          }
        }
        if (number) {
          std::ostringstream call_number_stream;
          if ((type == BTHF_CALL_ADDRTYPE_INTERNATIONAL) && (*number != '+')) {
            call_number_stream << "\"+";
          } else {
            call_number_stream << "\"";
          }

          std::string name_str;
          if (name) {
            name_str.append(name);
          }
          std::string number_str(number);
          // 13 = ["][+]["][,][3_digit_type][,,,]["]["][null_terminator]
          int overflow_size =
              13 + static_cast<int>(number_str.length() + name_str.length()) -
              static_cast<int>(sizeof(ag_res.str));
          if (overflow_size > 0) {
            android_errorWriteLog(0x534e4554, "79431031");
            int extra_overflow_size =
                overflow_size - static_cast<int>(name_str.length());
            if (extra_overflow_size > 0) {
              number_str.resize(number_str.length() - extra_overflow_size);
              name_str.clear();
            } else {
              name_str.resize(name_str.length() - overflow_size);
            }
          }
          call_number_stream << number_str << "\"";

          // Store caller id string and append type info.
          // Make sure type info is valid, otherwise add 129 as default type
          ag_res.num = static_cast<uint16_t>(type);
          if ((ag_res.num < BTA_AG_CLIP_TYPE_MIN) ||
              (ag_res.num > BTA_AG_CLIP_TYPE_MAX)) {
            if (ag_res.num != BTA_AG_CLIP_TYPE_VOIP) {
              ag_res.num = BTA_AG_CLIP_TYPE_DEFAULT;
            }
          }

          if (res == BTA_AG_CALL_WAIT_RES || name_str.empty()) {
            call_number_stream << "," << std::to_string(ag_res.num);
          } else {
            call_number_stream << "," << std::to_string(ag_res.num) << ",,,\""
                               << name_str << "\"";
          }
          snprintf(ag_res.str, sizeof(ag_res.str), "%s",
                   call_number_stream.str().c_str());
        }
        break;
      case BTHF_CALL_STATE_DIALING:
        if (!(num_active + num_held) && is_active_device(*bd_addr)) {
          ag_res.audio_handle = control_block.handle;
        }
        res = BTA_AG_OUT_CALL_ORIG_RES;
        break;
      case BTHF_CALL_STATE_ALERTING:
        /* if we went from idle->alert, force SCO setup here. dialing usually
         * triggers it */
        if ((control_block.call_setup_state == BTHF_CALL_STATE_IDLE) &&
            !(num_active + num_held) && is_active_device(*bd_addr)) {
          ag_res.audio_handle = control_block.handle;
        }
        res = BTA_AG_OUT_CALL_ALERT_RES;
        break;
      default:
        BTIF_TRACE_ERROR("%s: Incorrect call state prev=%d, now=%d", __func__,
                         control_block.call_setup_state, call_setup_state);
        status = BT_STATUS_PARM_INVALID;
        break;
    }
    BTIF_TRACE_DEBUG("%s: Call setup state changed. res=%d, audio_handle=%d",
                     __func__, res, ag_res.audio_handle);

    if (res != 0xFF) {
      BTA_AgResult(control_block.handle, res, ag_res);
    }

    /* if call setup is idle, we have already updated call indicator, jump out
     */
    if (call_setup_state == BTHF_CALL_STATE_IDLE) {
      /* check & update callheld */
      if ((num_held > 0) && (num_active > 0)) {
        send_indicator_update(control_block, BTA_AG_IND_CALLHELD, 1);
      }
      UpdateCallStates(&btif_hf_cb[idx], num_active, num_held,
                       call_setup_state);
      return status;
    }
  }

  /**
   * Handle call indicator change
   *
   * Per the errata 2043, call=1 implies at least one call is in progress
   * (active or held)
   * See: https://www.bluetooth.org/errata/errata_view.cfm?errata_id=2043
   *
   **/
  if (!active_call_updated &&
      ((num_active + num_held) !=
       (control_block.num_active + control_block.num_held))) {
    VLOG(1) << __func__ << ": in progress call states changed, active=["
            << control_block.num_active << "->" << num_active << "], held=["
            << control_block.num_held << "->" << num_held;
    send_indicator_update(control_block, BTA_AG_IND_CALL,
                          ((num_active + num_held) > 0) ? BTA_AG_CALL_ACTIVE
                                                        : BTA_AG_CALL_INACTIVE);
  }

  /* Held Changed? */
  if (num_held != control_block.num_held ||
      ((num_active == 0) && ((num_held + control_block.num_held) > 1))) {
    BTIF_TRACE_DEBUG("%s: Held call states changed. old: %d new: %d", __func__,
                     control_block.num_held, num_held);
    send_indicator_update(control_block, BTA_AG_IND_CALLHELD,
                          ((num_held == 0) ? 0 : ((num_active == 0) ? 2 : 1)));
  }

  /* Calls Swapped? */
  if ((call_setup_state == control_block.call_setup_state) &&
      (num_active && num_held) && (num_active == control_block.num_active) &&
      (num_held == control_block.num_held)) {
    BTIF_TRACE_DEBUG("%s: Calls swapped", __func__);
    send_indicator_update(control_block, BTA_AG_IND_CALLHELD, 1);
  }

  /* When call is hung up and still there is another call is in active,
   * some of the HF cannot acquire the call states by its own. If HF try
   * to terminate a call, it may not send the command AT+CHUP because the
   * call states are not updated properly. HF should get informed the call
   * status forcibly.
   */
  if ((control_block.num_active == num_active && num_active != 0) &&
      (control_block.num_held != num_held && num_held == 0)) {
    tBTA_AG_RES_DATA ag_res = {};
    ag_res.ind.id = BTA_AG_IND_CALL;
    ag_res.ind.value = num_active;
    BTA_AgResult(control_block.handle, BTA_AG_IND_RES_ON_DEMAND, ag_res);
  }

  UpdateCallStates(&btif_hf_cb[idx], num_active, num_held, call_setup_state);
  return status;
}

void HeadsetInterface::Cleanup() {
  BTIF_TRACE_EVENT("%s", __func__);

  btif_queue_cleanup(UUID_SERVCLASS_AG_HANDSFREE);
  if (bt_hf_callbacks) {
#if (defined(BTIF_HF_SERVICES) && (BTIF_HF_SERVICES & BTA_HFP_SERVICE_MASK))
    btif_disable_service(BTA_HFP_SERVICE_ID);
#else
    btif_disable_service(BTA_HSP_SERVICE_ID);
#endif
    bt_hf_callbacks = nullptr;
  }
}

bt_status_t HeadsetInterface::SetScoAllowed(bool value) {
  CHECK_BTHF_INIT();
  BTA_AgSetScoAllowed(value);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::SendBsir(bool value, RawAddress* bd_addr) {
  CHECK_BTHF_INIT();
  int idx = btif_hf_idx_by_bdaddr(bd_addr);
  if ((idx < 0) || (idx >= BTA_AG_MAX_NUM_CLIENTS)) {
    BTIF_TRACE_ERROR("%s: Invalid index %d for %s", __func__, idx,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  if (!is_connected(bd_addr)) {
    BTIF_TRACE_ERROR("%s: %s not connected", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  tBTA_AG_RES_DATA ag_result = {};
  ag_result.state = value;
  BTA_AgResult(btif_hf_cb[idx].handle, BTA_AG_INBAND_RING_RES, ag_result);
  return BT_STATUS_SUCCESS;
}

bt_status_t HeadsetInterface::SetActiveDevice(RawAddress* active_device_addr) {
  CHECK_BTHF_INIT();
  active_bda = *active_device_addr;
  BTA_AgSetActiveDevice(*active_device_addr);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_hf_execute_service
 *
 * Description      Initializes/Shuts down the service
 *
 * Returns          BT_STATUS_SUCCESS on success, BT_STATUS_FAIL otherwise
 *
 ******************************************************************************/
bt_status_t ExecuteService(bool b_enable) {
  const char* service_names_raw[] = BTIF_HF_SERVICE_NAMES;
  std::vector<std::string> service_names;
  for (const char* service_name_raw : service_names_raw) {
    if (service_name_raw) {
      service_names.emplace_back(service_name_raw);
    }
  }
  if (b_enable) {
    /* Enable and register with BTA-AG */
    BTA_AgEnable(bte_hf_evt);
    for (uint8_t app_id = 0; app_id < btif_max_hf_clients; app_id++) {
      BTA_AgRegister(BTIF_HF_SERVICES, BTIF_HF_SECURITY, btif_hf_features,
                     service_names, app_id);
    }
  } else {
    /* De-register AG */
    for (int i = 0; i < btif_max_hf_clients; i++) {
      BTA_AgDeregister(btif_hf_cb[i].handle);
    }
    /* Disable AG */
    BTA_AgDisable();
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_hf_get_interface
 *
 * Description      Get the hf callback interface
 *
 * Returns          bthf_interface_t
 *
 ******************************************************************************/
Interface* GetInterface() {
  VLOG(0) << __func__;
  return HeadsetInterface::GetInstance();
}

}  // namespace headset
}  // namespace bluetooth
