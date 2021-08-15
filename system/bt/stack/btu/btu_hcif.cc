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
 *  This file contains functions that interface with the HCI transport. On
 *  the receive side, it routes events to the appropriate handler, e.g.
 *  L2CAP, ScoMgr. On the transmit side, it manages the command
 *  transmission.
 *
 ******************************************************************************/

#define LOG_TAG "bt_btu_hcif"

#include <base/bind.h>
#include <base/callback.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/threading/thread.h>
#include <frameworks/base/core/proto/android/bluetooth/enums.pb.h>
#include <frameworks/base/core/proto/android/bluetooth/hci/enums.pb.h>
#include <log/log.h>
#include <statslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bt_common.h"
#include "bt_types.h"
#include "bt_utils.h"
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "common/metrics.h"
#include "device/include/controller.h"
#include "hci_layer.h"
#include "hcimsgs.h"
#include "l2c_int.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

using base::Location;

extern void btm_process_cancel_complete(uint8_t status, uint8_t mode);
extern void btm_ble_test_command_complete(uint8_t* p);
extern void smp_cancel_start_encryption_attempt();

/******************************************************************************/
/*            L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/******************************************************************************/
static void btu_hcif_inquiry_comp_evt(uint8_t* p);
static void btu_hcif_inquiry_result_evt(uint8_t* p, uint8_t hci_evt_len);
static void btu_hcif_inquiry_rssi_result_evt(uint8_t* p, uint8_t hci_evt_len);
static void btu_hcif_extended_inquiry_result_evt(uint8_t* p,
                                                 uint8_t hci_evt_len);

static void btu_hcif_connection_comp_evt(uint8_t* p, uint8_t evt_len);
static void btu_hcif_connection_request_evt(uint8_t* p);
static void btu_hcif_disconnection_comp_evt(uint8_t* p);
static void btu_hcif_authentication_comp_evt(uint8_t* p);
static void btu_hcif_rmt_name_request_comp_evt(uint8_t* p, uint16_t evt_len);
static void btu_hcif_encryption_change_evt(uint8_t* p);
static void btu_hcif_read_rmt_features_comp_evt(uint8_t* p);
static void btu_hcif_read_rmt_ext_features_comp_evt(uint8_t* p,
                                                    uint8_t evt_len);
static void btu_hcif_read_rmt_version_comp_evt(uint8_t* p);
static void btu_hcif_qos_setup_comp_evt(uint8_t* p);
static void btu_hcif_command_complete_evt(BT_HDR* response, void* context);
static void btu_hcif_command_status_evt(uint8_t status, BT_HDR* command,
                                        void* context);
static void btu_hcif_hardware_error_evt(uint8_t* p);
static void btu_hcif_flush_occured_evt(void);
static void btu_hcif_role_change_evt(uint8_t* p);
static void btu_hcif_num_compl_data_pkts_evt(uint8_t* p, uint8_t evt_len);
static void btu_hcif_mode_change_evt(uint8_t* p);
static void btu_hcif_pin_code_request_evt(uint8_t* p);
static void btu_hcif_link_key_request_evt(uint8_t* p);
static void btu_hcif_link_key_notification_evt(uint8_t* p);
static void btu_hcif_loopback_command_evt(void);
static void btu_hcif_data_buf_overflow_evt(void);
static void btu_hcif_max_slots_changed_evt(void);
static void btu_hcif_read_clock_off_comp_evt(uint8_t* p);
static void btu_hcif_conn_pkt_type_change_evt(void);
static void btu_hcif_qos_violation_evt(uint8_t* p);
static void btu_hcif_page_scan_mode_change_evt(void);
static void btu_hcif_page_scan_rep_mode_chng_evt(void);
static void btu_hcif_esco_connection_comp_evt(uint8_t* p);
static void btu_hcif_esco_connection_chg_evt(uint8_t* p);

/* Simple Pairing Events */
static void btu_hcif_host_support_evt(uint8_t* p);
static void btu_hcif_io_cap_request_evt(uint8_t* p);
static void btu_hcif_io_cap_response_evt(uint8_t* p);
static void btu_hcif_user_conf_request_evt(uint8_t* p);
static void btu_hcif_user_passkey_request_evt(uint8_t* p);
static void btu_hcif_user_passkey_notif_evt(uint8_t* p);
static void btu_hcif_keypress_notif_evt(uint8_t* p);
static void btu_hcif_rem_oob_request_evt(uint8_t* p);

static void btu_hcif_simple_pair_complete_evt(uint8_t* p);
#if L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE
static void btu_hcif_enhanced_flush_complete_evt(void);
#endif

#if (BTM_SSR_INCLUDED == TRUE)
static void btu_hcif_ssr_evt(uint8_t* p, uint16_t evt_len);
#endif /* BTM_SSR_INCLUDED == TRUE */

static void btu_ble_ll_conn_complete_evt(uint8_t* p, uint16_t evt_len);
static void btu_ble_read_remote_feat_evt(uint8_t* p);
static void btu_ble_ll_conn_param_upd_evt(uint8_t* p, uint16_t evt_len);
static void btu_ble_proc_ltk_req(uint8_t* p);
static void btu_hcif_encryption_key_refresh_cmpl_evt(uint8_t* p);
static void btu_ble_data_length_change_evt(uint8_t* p, uint16_t evt_len);
#if (BLE_LLT_INCLUDED == TRUE)
static void btu_ble_rc_param_req_evt(uint8_t* p);
#endif
#if (BLE_PRIVACY_SPT == TRUE)
static void btu_ble_proc_enhanced_conn_cmpl(uint8_t* p, uint16_t evt_len);
#endif

/**
 * Log HCI event metrics that are not handled in special functions
 * @param evt_code event code
 * @param p_event pointer to event parameter, skipping paremter length
 */
void btu_hcif_log_event_metrics(uint8_t evt_code, uint8_t* p_event) {
  uint32_t cmd = android::bluetooth::hci::CMD_UNKNOWN;
  uint16_t status = android::bluetooth::hci::STATUS_UNKNOWN;
  uint16_t reason = android::bluetooth::hci::STATUS_UNKNOWN;
  uint16_t handle = bluetooth::common::kUnknownConnectionHandle;
  int64_t value = 0;

  RawAddress bda = RawAddress::kEmpty;
  switch (evt_code) {
    case HCI_IO_CAPABILITY_REQUEST_EVT:
    case HCI_IO_CAPABILITY_RESPONSE_EVT:
    case HCI_LINK_KEY_REQUEST_EVT:
    case HCI_LINK_KEY_NOTIFICATION_EVT:
    case HCI_USER_PASSKEY_REQUEST_EVT:
    case HCI_USER_PASSKEY_NOTIFY_EVT:
    case HCI_USER_CONFIRMATION_REQUEST_EVT:
    case HCI_KEYPRESS_NOTIFY_EVT:
    case HCI_REMOTE_OOB_DATA_REQUEST_EVT:
      STREAM_TO_BDADDR(bda, p_event);
      bluetooth::common::LogClassicPairingEvent(bda, handle, cmd, evt_code, status, reason, value);
      break;
    case HCI_SIMPLE_PAIRING_COMPLETE_EVT:
    case HCI_RMT_NAME_REQUEST_COMP_EVT:
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_BDADDR(bda, p_event);
      bluetooth::common::LogClassicPairingEvent(bda, handle, cmd, evt_code, status, reason, value);
      break;
    case HCI_AUTHENTICATION_COMP_EVT:
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      handle = HCID_GET_HANDLE(handle);
      bluetooth::common::LogClassicPairingEvent(bda, handle, cmd, evt_code, status, reason, value);
      break;
    case HCI_ENCRYPTION_CHANGE_EVT: {
      uint8_t encryption_enabled;
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      STREAM_TO_UINT8(encryption_enabled, p_event);
      bluetooth::common::LogClassicPairingEvent(bda, handle, cmd, evt_code, status, reason, encryption_enabled);
      break;
    }
    case HCI_CONNECTION_COMP_EVT: {
      uint8_t link_type;
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      STREAM_TO_BDADDR(bda, p_event);
      STREAM_TO_UINT8(link_type, p_event);
      handle = HCID_GET_HANDLE(handle);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bda, handle, android::bluetooth::DIRECTION_UNKNOWN, link_type, cmd,
          evt_code, android::bluetooth::hci::BLE_EVT_UNKNOWN, status, reason);
      break;
    }
    case HCI_CONNECTION_REQUEST_EVT: {
      DEV_CLASS dc;
      uint8_t link_type;
      STREAM_TO_BDADDR(bda, p_event);
      STREAM_TO_DEVCLASS(dc, p_event);
      STREAM_TO_UINT8(link_type, p_event);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bda, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING, link_type, cmd, evt_code,
          android::bluetooth::hci::BLE_EVT_UNKNOWN, status, reason);
      break;
    }
    case HCI_DISCONNECTION_COMP_EVT: {
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      STREAM_TO_UINT8(reason, p_event);
      handle = HCID_GET_HANDLE(handle);
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, handle, android::bluetooth::DIRECTION_UNKNOWN,
          android::bluetooth::LINK_TYPE_UNKNOWN, cmd, evt_code,
          android::bluetooth::hci::BLE_EVT_UNKNOWN, status, reason);
      break;
    }
    case HCI_ESCO_CONNECTION_COMP_EVT: {
      uint8_t link_type;
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      STREAM_TO_BDADDR(bda, p_event);
      STREAM_TO_UINT8(link_type, p_event);
      handle = HCID_GET_HANDLE(handle);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bda, handle, android::bluetooth::DIRECTION_UNKNOWN, link_type, cmd,
          evt_code, android::bluetooth::hci::BLE_EVT_UNKNOWN, status, reason);
      break;
    }
    case HCI_ESCO_CONNECTION_CHANGED_EVT: {
      STREAM_TO_UINT8(status, p_event);
      STREAM_TO_UINT16(handle, p_event);
      handle = HCID_GET_HANDLE(handle);
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, handle, android::bluetooth::DIRECTION_UNKNOWN,
          android::bluetooth::LINK_TYPE_UNKNOWN, cmd, evt_code,
          android::bluetooth::hci::BLE_EVT_UNKNOWN, status, reason);
      break;
    }
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_process_event
 *
 * Description      This function is called when an event is received from
 *                  the Host Controller.
 *
 * Returns          void
 *
 ******************************************************************************/
void btu_hcif_process_event(UNUSED_ATTR uint8_t controller_id, BT_HDR* p_msg) {
  uint8_t* p = (uint8_t*)(p_msg + 1) + p_msg->offset;
  uint8_t hci_evt_code, hci_evt_len;
  uint8_t ble_sub_code;
  STREAM_TO_UINT8(hci_evt_code, p);
  STREAM_TO_UINT8(hci_evt_len, p);

  btu_hcif_log_event_metrics(hci_evt_code, p);

  switch (hci_evt_code) {
    case HCI_INQUIRY_COMP_EVT:
      btu_hcif_inquiry_comp_evt(p);
      break;
    case HCI_INQUIRY_RESULT_EVT:
      btu_hcif_inquiry_result_evt(p, hci_evt_len);
      break;
    case HCI_INQUIRY_RSSI_RESULT_EVT:
      btu_hcif_inquiry_rssi_result_evt(p, hci_evt_len);
      break;
    case HCI_EXTENDED_INQUIRY_RESULT_EVT:
      btu_hcif_extended_inquiry_result_evt(p, hci_evt_len);
      break;
    case HCI_CONNECTION_COMP_EVT:
      btu_hcif_connection_comp_evt(p, hci_evt_len);
      break;
    case HCI_CONNECTION_REQUEST_EVT:
      btu_hcif_connection_request_evt(p);
      break;
    case HCI_DISCONNECTION_COMP_EVT:
      btu_hcif_disconnection_comp_evt(p);
      break;
    case HCI_AUTHENTICATION_COMP_EVT:
      btu_hcif_authentication_comp_evt(p);
      break;
    case HCI_RMT_NAME_REQUEST_COMP_EVT:
      btu_hcif_rmt_name_request_comp_evt(p, hci_evt_len);
      break;
    case HCI_ENCRYPTION_CHANGE_EVT:
      btu_hcif_encryption_change_evt(p);
      break;
    case HCI_ENCRYPTION_KEY_REFRESH_COMP_EVT:
      btu_hcif_encryption_key_refresh_cmpl_evt(p);
      break;
    case HCI_READ_RMT_FEATURES_COMP_EVT:
      btu_hcif_read_rmt_features_comp_evt(p);
      break;
    case HCI_READ_RMT_EXT_FEATURES_COMP_EVT:
      btu_hcif_read_rmt_ext_features_comp_evt(p, hci_evt_len);
      break;
    case HCI_READ_RMT_VERSION_COMP_EVT:
      btu_hcif_read_rmt_version_comp_evt(p);
      break;
    case HCI_QOS_SETUP_COMP_EVT:
      btu_hcif_qos_setup_comp_evt(p);
      break;
    case HCI_COMMAND_COMPLETE_EVT:
      LOG_ERROR(LOG_TAG,
                "%s should not have received a command complete event. "
                "Someone didn't go through the hci transmit_command function.",
                __func__);
      break;
    case HCI_COMMAND_STATUS_EVT:
      LOG_ERROR(LOG_TAG,
                "%s should not have received a command status event. "
                "Someone didn't go through the hci transmit_command function.",
                __func__);
      break;
    case HCI_HARDWARE_ERROR_EVT:
      btu_hcif_hardware_error_evt(p);
      break;
    case HCI_FLUSH_OCCURED_EVT:
      btu_hcif_flush_occured_evt();
      break;
    case HCI_ROLE_CHANGE_EVT:
      btu_hcif_role_change_evt(p);
      break;
    case HCI_NUM_COMPL_DATA_PKTS_EVT:
      btu_hcif_num_compl_data_pkts_evt(p, hci_evt_len);
      break;
    case HCI_MODE_CHANGE_EVT:
      btu_hcif_mode_change_evt(p);
      break;
    case HCI_PIN_CODE_REQUEST_EVT:
      btu_hcif_pin_code_request_evt(p);
      break;
    case HCI_LINK_KEY_REQUEST_EVT:
      btu_hcif_link_key_request_evt(p);
      break;
    case HCI_LINK_KEY_NOTIFICATION_EVT:
      btu_hcif_link_key_notification_evt(p);
      break;
    case HCI_LOOPBACK_COMMAND_EVT:
      btu_hcif_loopback_command_evt();
      break;
    case HCI_DATA_BUF_OVERFLOW_EVT:
      btu_hcif_data_buf_overflow_evt();
      break;
    case HCI_MAX_SLOTS_CHANGED_EVT:
      btu_hcif_max_slots_changed_evt();
      break;
    case HCI_READ_CLOCK_OFF_COMP_EVT:
      btu_hcif_read_clock_off_comp_evt(p);
      break;
    case HCI_CONN_PKT_TYPE_CHANGE_EVT:
      btu_hcif_conn_pkt_type_change_evt();
      break;
    case HCI_QOS_VIOLATION_EVT:
      btu_hcif_qos_violation_evt(p);
      break;
    case HCI_PAGE_SCAN_MODE_CHANGE_EVT:
      btu_hcif_page_scan_mode_change_evt();
      break;
    case HCI_PAGE_SCAN_REP_MODE_CHNG_EVT:
      btu_hcif_page_scan_rep_mode_chng_evt();
      break;
    case HCI_ESCO_CONNECTION_COMP_EVT:
      btu_hcif_esco_connection_comp_evt(p);
      break;
    case HCI_ESCO_CONNECTION_CHANGED_EVT:
      btu_hcif_esco_connection_chg_evt(p);
      break;
#if (BTM_SSR_INCLUDED == TRUE)
    case HCI_SNIFF_SUB_RATE_EVT:
      btu_hcif_ssr_evt(p, hci_evt_len);
      break;
#endif /* BTM_SSR_INCLUDED == TRUE */
    case HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT:
      btu_hcif_host_support_evt(p);
      break;
    case HCI_IO_CAPABILITY_REQUEST_EVT:
      btu_hcif_io_cap_request_evt(p);
      break;
    case HCI_IO_CAPABILITY_RESPONSE_EVT:
      btu_hcif_io_cap_response_evt(p);
      break;
    case HCI_USER_CONFIRMATION_REQUEST_EVT:
      btu_hcif_user_conf_request_evt(p);
      break;
    case HCI_USER_PASSKEY_REQUEST_EVT:
      btu_hcif_user_passkey_request_evt(p);
      break;
    case HCI_REMOTE_OOB_DATA_REQUEST_EVT:
      btu_hcif_rem_oob_request_evt(p);
      break;
    case HCI_SIMPLE_PAIRING_COMPLETE_EVT:
      btu_hcif_simple_pair_complete_evt(p);
      break;
    case HCI_USER_PASSKEY_NOTIFY_EVT:
      btu_hcif_user_passkey_notif_evt(p);
      break;
    case HCI_KEYPRESS_NOTIFY_EVT:
      btu_hcif_keypress_notif_evt(p);
      break;
#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
    case HCI_ENHANCED_FLUSH_COMPLETE_EVT:
      btu_hcif_enhanced_flush_complete_evt();
      break;
#endif

    case HCI_BLE_EVENT: {
      STREAM_TO_UINT8(ble_sub_code, p);

      HCI_TRACE_EVENT("BLE HCI(id=%d) event = 0x%02x)", hci_evt_code,
                      ble_sub_code);

      uint8_t ble_evt_len = hci_evt_len - 1;
      switch (ble_sub_code) {
        case HCI_BLE_ADV_PKT_RPT_EVT: /* result of inquiry */
          HCI_TRACE_EVENT("HCI_BLE_ADV_PKT_RPT_EVT");
          btm_ble_process_adv_pkt(ble_evt_len, p);
          break;
        case HCI_BLE_CONN_COMPLETE_EVT:
          btu_ble_ll_conn_complete_evt(p, hci_evt_len);
          break;
        case HCI_BLE_LL_CONN_PARAM_UPD_EVT:
          btu_ble_ll_conn_param_upd_evt(p, hci_evt_len);
          break;
        case HCI_BLE_READ_REMOTE_FEAT_CMPL_EVT:
          btu_ble_read_remote_feat_evt(p);
          break;
        case HCI_BLE_LTK_REQ_EVT: /* received only at slave device */
          btu_ble_proc_ltk_req(p);
          break;
#if (BLE_PRIVACY_SPT == TRUE)
        case HCI_BLE_ENHANCED_CONN_COMPLETE_EVT:
          btu_ble_proc_enhanced_conn_cmpl(p, hci_evt_len);
          break;
#endif
#if (BLE_LLT_INCLUDED == TRUE)
        case HCI_BLE_RC_PARAM_REQ_EVT:
          btu_ble_rc_param_req_evt(p);
          break;
#endif
        case HCI_BLE_DATA_LENGTH_CHANGE_EVT:
          btu_ble_data_length_change_evt(p, hci_evt_len);
          break;

        case HCI_BLE_PHY_UPDATE_COMPLETE_EVT:
          btm_ble_process_phy_update_pkt(ble_evt_len, p);
          break;

        case HCI_LE_EXTENDED_ADVERTISING_REPORT_EVT:
          btm_ble_process_ext_adv_pkt(hci_evt_len, p);
          break;

        case HCI_LE_ADVERTISING_SET_TERMINATED_EVT:
          btm_le_on_advertising_set_terminated(p, hci_evt_len);
          break;
      }
      break;
    }

    case HCI_VENDOR_SPECIFIC_EVT:
      btm_vendor_specific_evt(p, hci_evt_len);
      break;
  }
}

static void btu_hcif_log_command_metrics(uint16_t opcode, uint8_t* p_cmd,
                                         uint16_t cmd_status,
                                         bool is_cmd_status) {
  static uint16_t kUnknownBleEvt = android::bluetooth::hci::BLE_EVT_UNKNOWN;

  uint16_t hci_event = android::bluetooth::hci::EVT_COMMAND_STATUS;
  if (!is_cmd_status) {
    hci_event = android::bluetooth::hci::EVT_UNKNOWN;
    cmd_status = android::bluetooth::hci::STATUS_UNKNOWN;
  }

  RawAddress bd_addr;
  uint16_t handle;
  uint8_t reason;

  switch (opcode) {
    case HCI_CREATE_CONNECTION:
    case HCI_CREATE_CONNECTION_CANCEL:
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bd_addr, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_OUTGOING,
          android::bluetooth::LINK_TYPE_ACL, opcode, hci_event, kUnknownBleEvt,
          cmd_status, android::bluetooth::hci::STATUS_UNKNOWN);
      break;
    case HCI_DISCONNECT:
      STREAM_TO_UINT16(handle, p_cmd);
      STREAM_TO_UINT8(reason, p_cmd);
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, handle, android::bluetooth::DIRECTION_UNKNOWN,
          android::bluetooth::LINK_TYPE_UNKNOWN, opcode, hci_event,
          kUnknownBleEvt, cmd_status, reason);
      break;
    case HCI_SETUP_ESCO_CONNECTION:
    case HCI_ENH_SETUP_ESCO_CONNECTION:
      STREAM_TO_UINT16(handle, p_cmd);
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, handle, android::bluetooth::DIRECTION_OUTGOING,
          android::bluetooth::LINK_TYPE_UNKNOWN, opcode, hci_event,
          kUnknownBleEvt, cmd_status, android::bluetooth::hci::STATUS_UNKNOWN);
      break;
    case HCI_ACCEPT_CONNECTION_REQUEST:
    case HCI_ACCEPT_ESCO_CONNECTION:
    case HCI_ENH_ACCEPT_ESCO_CONNECTION:
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bd_addr, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING,
          android::bluetooth::LINK_TYPE_UNKNOWN, opcode, hci_event,
          kUnknownBleEvt, cmd_status, android::bluetooth::hci::STATUS_UNKNOWN);
      break;
    case HCI_REJECT_CONNECTION_REQUEST:
    case HCI_REJECT_ESCO_CONNECTION:
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      STREAM_TO_UINT8(reason, p_cmd);
      bluetooth::common::LogLinkLayerConnectionEvent(
          &bd_addr, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING,
          android::bluetooth::LINK_TYPE_UNKNOWN, opcode, hci_event,
          kUnknownBleEvt, cmd_status, reason);
      break;

      // BLE Commands
    case HCI_BLE_CREATE_LL_CONN: {
      p_cmd += 2;  // Skip LE_Scan_Interval
      p_cmd += 2;  // Skip LE_Scan_Window;
      uint8_t initiator_filter_policy;
      STREAM_TO_UINT8(initiator_filter_policy, p_cmd);
      uint8_t peer_address_type;
      STREAM_TO_UINT8(peer_address_type, p_cmd);
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      // Peer address should not be used if initiator filter policy is not 0x00
      const RawAddress* bd_addr_p = nullptr;
      if (initiator_filter_policy == 0x00) {
        bd_addr_p = &bd_addr;
#if (BLE_PRIVACY_SPT == TRUE)
        if (peer_address_type == BLE_ADDR_PUBLIC_ID ||
            peer_address_type == BLE_ADDR_RANDOM_ID) {
          // if identity address is not matched, this address is invalid
          if (!btm_identity_addr_to_random_pseudo(&bd_addr, &peer_address_type,
                                                  false)) {
            bd_addr_p = nullptr;
          }
        }
#endif
      }
      if (initiator_filter_policy == 0x00 ||
          (cmd_status != HCI_SUCCESS && !is_cmd_status)) {
        // Selectively log to avoid log spam due to whitelist connections:
        // - When doing non-whitelist connection
        // - When there is an error in command status
        bluetooth::common::LogLinkLayerConnectionEvent(
            bd_addr_p, bluetooth::common::kUnknownConnectionHandle,
            android::bluetooth::DIRECTION_OUTGOING,
            android::bluetooth::LINK_TYPE_ACL, opcode, hci_event,
            kUnknownBleEvt, cmd_status,
            android::bluetooth::hci::STATUS_UNKNOWN);
      }
      break;
    }
    case HCI_LE_EXTENDED_CREATE_CONNECTION: {
      uint8_t initiator_filter_policy;
      STREAM_TO_UINT8(initiator_filter_policy, p_cmd);
      p_cmd += 1;  // Skip Own_Address_Type
      uint8_t peer_addr_type;
      STREAM_TO_UINT8(peer_addr_type, p_cmd);
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      // Peer address should not be used if initiator filter policy is not 0x00
      const RawAddress* bd_addr_p = nullptr;
      if (initiator_filter_policy == 0x00) {
        bd_addr_p = &bd_addr;
#if (BLE_PRIVACY_SPT == TRUE)
        // if identity address is not matched, this should be a static address
        btm_identity_addr_to_random_pseudo(&bd_addr, &peer_addr_type, false);
#endif
      }
      if (initiator_filter_policy == 0x00 ||
          (cmd_status != HCI_SUCCESS && !is_cmd_status)) {
        // Selectively log to avoid log spam due to whitelist connections:
        // - When doing non-whitelist connection
        // - When there is an error in command status
        bluetooth::common::LogLinkLayerConnectionEvent(
            bd_addr_p, bluetooth::common::kUnknownConnectionHandle,
            android::bluetooth::DIRECTION_OUTGOING,
            android::bluetooth::LINK_TYPE_ACL, opcode, hci_event,
            kUnknownBleEvt, cmd_status,
            android::bluetooth::hci::STATUS_UNKNOWN);
      }
      break;
    }
    case HCI_BLE_CREATE_CONN_CANCEL:
      if (cmd_status != HCI_SUCCESS && !is_cmd_status) {
        // Only log errors to prevent log spam due to whitelist connections
        bluetooth::common::LogLinkLayerConnectionEvent(
            nullptr, bluetooth::common::kUnknownConnectionHandle,
            android::bluetooth::DIRECTION_OUTGOING,
            android::bluetooth::LINK_TYPE_ACL, opcode, hci_event,
            kUnknownBleEvt, cmd_status,
            android::bluetooth::hci::STATUS_UNKNOWN);
      }
      break;
    case HCI_BLE_CLEAR_WHITE_LIST:
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING,
          android::bluetooth::LINK_TYPE_ACL, opcode, hci_event, kUnknownBleEvt,
          cmd_status, android::bluetooth::hci::STATUS_UNKNOWN);
      break;
    case HCI_BLE_ADD_WHITE_LIST:
    case HCI_BLE_REMOVE_WHITE_LIST: {
      uint8_t peer_addr_type;
      STREAM_TO_UINT8(peer_addr_type, p_cmd);
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      const RawAddress* bd_addr_p = nullptr;
      // When peer_addr_type is 0xFF, bd_addr should be ignored per BT spec
      if (peer_addr_type != BLE_ADDR_ANONYMOUS) {
        bd_addr_p = &bd_addr;
#if (BLE_PRIVACY_SPT == TRUE)
        bool addr_is_rpa = peer_addr_type == BLE_ADDR_RANDOM &&
                           BTM_BLE_IS_RESOLVE_BDA(bd_addr);
        // Only try to match identity address for pseudo if address is not RPA
        if (!addr_is_rpa) {
          // if identity address is not matched, this should be a static address
          btm_identity_addr_to_random_pseudo(&bd_addr, &peer_addr_type, false);
        }
#endif
      }
      bluetooth::common::LogLinkLayerConnectionEvent(
          bd_addr_p, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING,
          android::bluetooth::LINK_TYPE_ACL, opcode, hci_event, kUnknownBleEvt,
          cmd_status, android::bluetooth::hci::STATUS_UNKNOWN);
      break;
    }
    case HCI_READ_LOCAL_OOB_DATA:
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, bluetooth::common::kUnknownConnectionHandle, opcode,
                                                hci_event, cmd_status, android::bluetooth::hci::STATUS_UNKNOWN, 0);
      break;
    case HCI_WRITE_SIMPLE_PAIRING_MODE: {
      uint8_t simple_pairing_mode;
      STREAM_TO_UINT8(simple_pairing_mode, p_cmd);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, bluetooth::common::kUnknownConnectionHandle, opcode,
                                                hci_event, cmd_status, android::bluetooth::hci::STATUS_UNKNOWN,
                                                simple_pairing_mode);
      break;
    }
    case HCI_WRITE_SECURE_CONNS_SUPPORT: {
      uint8_t secure_conn_host_support;
      STREAM_TO_UINT8(secure_conn_host_support, p_cmd);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, bluetooth::common::kUnknownConnectionHandle, opcode,
                                                hci_event, cmd_status, android::bluetooth::hci::STATUS_UNKNOWN,
                                                secure_conn_host_support);
      break;
    }
    case HCI_AUTHENTICATION_REQUESTED:
      STREAM_TO_UINT16(handle, p_cmd);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, handle, opcode, hci_event, cmd_status,
                                                android::bluetooth::hci::STATUS_UNKNOWN, 0);
      break;
    case HCI_SET_CONN_ENCRYPTION: {
      STREAM_TO_UINT16(handle, p_cmd);
      uint8_t encryption_enable;
      STREAM_TO_UINT8(encryption_enable, p_cmd);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, handle, opcode, hci_event, cmd_status,
                                                android::bluetooth::hci::STATUS_UNKNOWN, encryption_enable);
      break;
    }
    case HCI_DELETE_STORED_LINK_KEY: {
      uint8_t delete_all_flag;
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      STREAM_TO_UINT8(delete_all_flag, p_cmd);
      bluetooth::common::LogClassicPairingEvent(bd_addr, bluetooth::common::kUnknownConnectionHandle, opcode, hci_event,
                                                cmd_status, android::bluetooth::hci::STATUS_UNKNOWN, delete_all_flag);
      break;
    }
    case HCI_RMT_NAME_REQUEST:
    case HCI_RMT_NAME_REQUEST_CANCEL:
    case HCI_LINK_KEY_REQUEST_REPLY:
    case HCI_LINK_KEY_REQUEST_NEG_REPLY:
    case HCI_IO_CAPABILITY_REQUEST_REPLY:
    case HCI_USER_CONF_REQUEST_REPLY:
    case HCI_USER_CONF_VALUE_NEG_REPLY:
    case HCI_USER_PASSKEY_REQ_REPLY:
    case HCI_USER_PASSKEY_REQ_NEG_REPLY:
    case HCI_REM_OOB_DATA_REQ_REPLY:
    case HCI_REM_OOB_DATA_REQ_NEG_REPLY:
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      bluetooth::common::LogClassicPairingEvent(bd_addr, bluetooth::common::kUnknownConnectionHandle, opcode, hci_event,
                                                cmd_status, android::bluetooth::hci::STATUS_UNKNOWN, 0);
      break;
    case HCI_IO_CAP_REQ_NEG_REPLY:
      STREAM_TO_BDADDR(bd_addr, p_cmd);
      STREAM_TO_UINT8(reason, p_cmd);
      bluetooth::common::LogClassicPairingEvent(bd_addr, bluetooth::common::kUnknownConnectionHandle, opcode, hci_event,
                                                cmd_status, reason, 0);
      break;
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_send_cmd
 *
 * Description      This function is called to send commands to the Host
 *                  Controller.
 *
 * Returns          void
 *
 ******************************************************************************/
void btu_hcif_send_cmd(UNUSED_ATTR uint8_t controller_id, BT_HDR* p_buf) {
  if (!p_buf) return;

  uint16_t opcode;
  uint8_t* stream = p_buf->data + p_buf->offset;
  void* vsc_callback = NULL;

  STREAM_TO_UINT16(opcode, stream);

  // Eww...horrible hackery here
  /* If command was a VSC, then extract command_complete callback */
  if ((opcode & HCI_GRP_VENDOR_SPECIFIC) == HCI_GRP_VENDOR_SPECIFIC ||
      (opcode == HCI_BLE_RAND) || (opcode == HCI_BLE_ENCRYPT)) {
    vsc_callback = *((void**)(p_buf + 1));
  }

  // Skip parameter length before logging
  stream++;
  btu_hcif_log_command_metrics(opcode, stream,
                               android::bluetooth::hci::STATUS_UNKNOWN, false);

  hci_layer_get_interface()->transmit_command(
      p_buf, btu_hcif_command_complete_evt, btu_hcif_command_status_evt,
      vsc_callback);
}

using hci_cmd_cb = base::OnceCallback<void(
    uint8_t* /* return_parameters */, uint16_t /* return_parameters_length*/)>;

struct cmd_with_cb_data {
  hci_cmd_cb cb;
  Location posted_from;
};

void cmd_with_cb_data_init(cmd_with_cb_data* cb_wrapper) {
  new (&cb_wrapper->cb) hci_cmd_cb;
  new (&cb_wrapper->posted_from) Location;
}

void cmd_with_cb_data_cleanup(cmd_with_cb_data* cb_wrapper) {
  cb_wrapper->cb.~hci_cmd_cb();
  cb_wrapper->posted_from.~Location();
}

/**
 * Log command complete events that is not handled individually in this file
 * @param opcode opcode of the command
 * @param p_return_params pointer to returned parameter after parameter length
 *                        field
 */
static void btu_hcif_log_command_complete_metrics(uint16_t opcode,
                                                  uint8_t* p_return_params) {
  uint16_t status = android::bluetooth::hci::STATUS_UNKNOWN;
  uint16_t reason = android::bluetooth::hci::STATUS_UNKNOWN;
  uint16_t hci_event = android::bluetooth::hci::EVT_COMMAND_COMPLETE;
  uint16_t hci_ble_event = android::bluetooth::hci::BLE_EVT_UNKNOWN;
  RawAddress bd_addr = RawAddress::kEmpty;
  switch (opcode) {
    case HCI_BLE_CLEAR_WHITE_LIST:
    case HCI_BLE_ADD_WHITE_LIST:
    case HCI_BLE_REMOVE_WHITE_LIST: {
      STREAM_TO_UINT8(status, p_return_params);
      bluetooth::common::LogLinkLayerConnectionEvent(
          nullptr, bluetooth::common::kUnknownConnectionHandle,
          android::bluetooth::DIRECTION_INCOMING,
          android::bluetooth::LINK_TYPE_ACL, opcode, hci_event, hci_ble_event,
          status, reason);
      break;
    }
    case HCI_DELETE_STORED_LINK_KEY:
    case HCI_READ_LOCAL_OOB_DATA:
    case HCI_WRITE_SIMPLE_PAIRING_MODE:
    case HCI_WRITE_SECURE_CONNS_SUPPORT:
      STREAM_TO_UINT8(status, p_return_params);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, bluetooth::common::kUnknownConnectionHandle, opcode,
                                                hci_event, status, reason, 0);
      break;
    case HCI_READ_ENCR_KEY_SIZE: {
      uint16_t handle;
      uint8_t key_size;
      STREAM_TO_UINT8(status, p_return_params);
      STREAM_TO_UINT16(handle, p_return_params);
      STREAM_TO_UINT8(key_size, p_return_params);
      bluetooth::common::LogClassicPairingEvent(RawAddress::kEmpty, handle, opcode, hci_event, status, reason,
                                                key_size);
      break;
    }
    case HCI_LINK_KEY_REQUEST_REPLY:
    case HCI_LINK_KEY_REQUEST_NEG_REPLY:
    case HCI_IO_CAPABILITY_REQUEST_REPLY:
    case HCI_IO_CAP_REQ_NEG_REPLY:
    case HCI_USER_CONF_REQUEST_REPLY:
    case HCI_USER_CONF_VALUE_NEG_REPLY:
    case HCI_USER_PASSKEY_REQ_REPLY:
    case HCI_USER_PASSKEY_REQ_NEG_REPLY:
    case HCI_REM_OOB_DATA_REQ_REPLY:
    case HCI_REM_OOB_DATA_REQ_NEG_REPLY:
      STREAM_TO_UINT8(status, p_return_params);
      STREAM_TO_BDADDR(bd_addr, p_return_params);
      bluetooth::common::LogClassicPairingEvent(bd_addr, bluetooth::common::kUnknownConnectionHandle, opcode, hci_event,
                                                status, reason, 0);
      break;
  }
}

static void btu_hcif_command_complete_evt_with_cb_on_task(BT_HDR* event,
                                                          void* context) {
  command_opcode_t opcode;
  // 2 for event header: event code (1) + parameter length (1)
  // 1 for num_hci_pkt command credit
  uint8_t* stream = event->data + event->offset + 3;
  STREAM_TO_UINT16(opcode, stream);

  btu_hcif_log_command_complete_metrics(opcode, stream);

  cmd_with_cb_data* cb_wrapper = (cmd_with_cb_data*)context;
  HCI_TRACE_DEBUG("command complete for: %s",
                  cb_wrapper->posted_from.ToString().c_str());
  // 2 for event header: event code (1) + parameter length (1)
  // 3 for command complete header: num_hci_pkt (1) + opcode (2)
  uint16_t param_len = static_cast<uint16_t>(event->len - 5);
  std::move(cb_wrapper->cb).Run(stream, param_len);
  cmd_with_cb_data_cleanup(cb_wrapper);
  osi_free(cb_wrapper);

  osi_free(event);
}

static void btu_hcif_command_complete_evt_with_cb(BT_HDR* response,
                                                  void* context) {
  do_in_main_thread(FROM_HERE,
                    base::Bind(btu_hcif_command_complete_evt_with_cb_on_task,
                               response, context));
}

static void btu_hcif_command_status_evt_with_cb_on_task(uint8_t status,
                                                        BT_HDR* event,
                                                        void* context) {
  command_opcode_t opcode;
  uint8_t* stream = event->data + event->offset;
  STREAM_TO_UINT16(opcode, stream);

  CHECK(status != 0);

  // stream + 1 to skip parameter length field
  // No need to check length since stream is written by us
  btu_hcif_log_command_metrics(opcode, stream + 1, status, true);

  // report command status error
  cmd_with_cb_data* cb_wrapper = (cmd_with_cb_data*)context;
  HCI_TRACE_DEBUG("command status for: %s",
                  cb_wrapper->posted_from.ToString().c_str());
  std::move(cb_wrapper->cb).Run(&status, sizeof(uint16_t));
  cmd_with_cb_data_cleanup(cb_wrapper);
  osi_free(cb_wrapper);

  osi_free(event);
}

static void btu_hcif_command_status_evt_with_cb(uint8_t status, BT_HDR* command,
                                                void* context) {
  // Command is pending, we  report only error.
  if (!status) {
    osi_free(command);
    return;
  }

  do_in_main_thread(
      FROM_HERE, base::Bind(btu_hcif_command_status_evt_with_cb_on_task, status,
                            command, context));
}

/* This function is called to send commands to the Host Controller. |cb| is
 * called when command status event is called with error code, or when the
 * command complete event is received. */
void btu_hcif_send_cmd_with_cb(const Location& posted_from, uint16_t opcode,
                               uint8_t* params, uint8_t params_len,
                               hci_cmd_cb cb) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + params_len;
  p->offset = 0;

  UINT16_TO_STREAM(pp, opcode);
  UINT8_TO_STREAM(pp, params_len);
  if (params) {
    memcpy(pp, params, params_len);
  }

  btu_hcif_log_command_metrics(opcode, pp,
                               android::bluetooth::hci::STATUS_UNKNOWN, false);

  cmd_with_cb_data* cb_wrapper =
      (cmd_with_cb_data*)osi_malloc(sizeof(cmd_with_cb_data));

  cmd_with_cb_data_init(cb_wrapper);
  cb_wrapper->cb = std::move(cb);
  cb_wrapper->posted_from = posted_from;

  hci_layer_get_interface()->transmit_command(
      p, btu_hcif_command_complete_evt_with_cb,
      btu_hcif_command_status_evt_with_cb, (void*)cb_wrapper);
}

/*******************************************************************************
 *
 * Function         btu_hcif_inquiry_comp_evt
 *
 * Description      Process event HCI_INQUIRY_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_inquiry_comp_evt(uint8_t* p) {
  uint8_t status;

  STREAM_TO_UINT8(status, p);

  /* Tell inquiry processing that we are done */
  btm_process_inq_complete(status, BTM_BR_INQUIRY_MASK);
}

/*******************************************************************************
 *
 * Function         btu_hcif_inquiry_result_evt
 *
 * Description      Process event HCI_INQUIRY_RESULT_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_inquiry_result_evt(uint8_t* p, uint8_t hci_evt_len) {
  /* Store results in the cache */
  btm_process_inq_results(p, hci_evt_len, BTM_INQ_RESULT_STANDARD);
}

/*******************************************************************************
 *
 * Function         btu_hcif_inquiry_rssi_result_evt
 *
 * Description      Process event HCI_INQUIRY_RSSI_RESULT_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_inquiry_rssi_result_evt(uint8_t* p, uint8_t hci_evt_len) {
  /* Store results in the cache */
  btm_process_inq_results(p, hci_evt_len, BTM_INQ_RESULT_WITH_RSSI);
}

/*******************************************************************************
 *
 * Function         btu_hcif_extended_inquiry_result_evt
 *
 * Description      Process event HCI_EXTENDED_INQUIRY_RESULT_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_extended_inquiry_result_evt(uint8_t* p,
                                                 uint8_t hci_evt_len) {
  /* Store results in the cache */
  btm_process_inq_results(p, hci_evt_len, BTM_INQ_RESULT_EXTENDED);
}

/*******************************************************************************
 *
 * Function         btu_hcif_connection_comp_evt
 *
 * Description      Process event HCI_CONNECTION_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_connection_comp_evt(uint8_t* p, uint8_t evt_len) {
  uint8_t status;
  uint16_t handle;
  RawAddress bda;
  uint8_t link_type;
  uint8_t enc_mode;
  tBTM_ESCO_DATA esco_data;

  if (evt_len < 11) {
    android_errorWriteLog(0x534e4554, "141619686");
    HCI_TRACE_WARNING("%s: malformed event of size %hhd", __func__, evt_len);
    return;
  }

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_BDADDR(bda, p);
  STREAM_TO_UINT8(link_type, p);
  STREAM_TO_UINT8(enc_mode, p);

  handle = HCID_GET_HANDLE(handle);

  if (status != HCI_SUCCESS) {
    HCI_TRACE_DEBUG(
        "%s: Connection failed: status=%d, handle=%d, link_type=%d, "
        "enc_mode=%d",
        __func__, status, handle, link_type, enc_mode);
  }

  if (link_type == HCI_LINK_TYPE_ACL) {
    btm_sec_connected(bda, handle, status, enc_mode);

    l2c_link_hci_conn_comp(status, handle, bda);
  } else {
    memset(&esco_data, 0, sizeof(tBTM_ESCO_DATA));
    /* esco_data.link_type = HCI_LINK_TYPE_SCO; already zero */
    esco_data.bd_addr = bda;
    btm_sco_connected(status, &bda, handle, &esco_data);
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_connection_request_evt
 *
 * Description      Process event HCI_CONNECTION_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_connection_request_evt(uint8_t* p) {
  RawAddress bda;
  DEV_CLASS dc;
  uint8_t link_type;

  STREAM_TO_BDADDR(bda, p);
  STREAM_TO_DEVCLASS(dc, p);
  STREAM_TO_UINT8(link_type, p);

  /* Pass request to security manager to check connect filters before */
  /* passing request to l2cap */
  if (link_type == HCI_LINK_TYPE_ACL) {
    btm_sec_conn_req(bda, dc);
  } else {
    btm_sco_conn_req(bda, dc, link_type);
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_disconnection_comp_evt
 *
 * Description      Process event HCI_DISCONNECTION_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_disconnection_comp_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;
  uint8_t reason;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT8(reason, p);

  handle = HCID_GET_HANDLE(handle);

  if ((reason != HCI_ERR_CONN_CAUSE_LOCAL_HOST) &&
      (reason != HCI_ERR_PEER_USER)) {
    /* Uncommon disconnection reasons */
    HCI_TRACE_DEBUG("%s: Got Disconn Complete Event: reason=%d, handle=%d",
                    __func__, reason, handle);
  }

  /* If L2CAP doesn't know about it, send it to SCO */
  if (!l2c_link_hci_disc_comp(handle, reason)) btm_sco_removed(handle, reason);

  /* Notify security manager */
  btm_sec_disconnected(handle, reason);
}

/*******************************************************************************
 *
 * Function         btu_hcif_authentication_comp_evt
 *
 * Description      Process event HCI_AUTHENTICATION_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_authentication_comp_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);

  btm_sec_auth_complete(handle, status);
}

/*******************************************************************************
 *
 * Function         btu_hcif_rmt_name_request_comp_evt
 *
 * Description      Process event HCI_RMT_NAME_REQUEST_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_rmt_name_request_comp_evt(uint8_t* p, uint16_t evt_len) {
  uint8_t status;
  RawAddress bd_addr;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_BDADDR(bd_addr, p);

  evt_len -= (1 + BD_ADDR_LEN);

  btm_process_remote_name(&bd_addr, p, evt_len, status);

  btm_sec_rmt_name_request_complete(&bd_addr, p, status);
}

constexpr uint8_t MIN_KEY_SIZE = 7;

static void read_encryption_key_size_complete_after_encryption_change(uint8_t status, uint16_t handle,
                                                                      uint8_t key_size) {
  if (status == HCI_ERR_INSUFFCIENT_SECURITY) {
    /* If remote device stop the encryption before we call "Read Encryption Key
     * Size", we might receive Insufficient Security, which means that link is
     * no longer encrypted. */
    LOG(INFO) << __func__ << ": encryption stopped on link: " << loghex(handle);
    return;
  }

  if (status != HCI_SUCCESS) {
    LOG(INFO) << __func__ << ": disconnecting, status: " << loghex(status);
    btsnd_hcic_disconnect(handle, HCI_ERR_PEER_USER);
    return;
  }

  if (key_size < MIN_KEY_SIZE) {
    android_errorWriteLog(0x534e4554, "124301137");
    LOG(ERROR) << __func__ << " encryption key too short, disconnecting. handle: " << loghex(handle)
               << " key_size: " << +key_size;

    btsnd_hcic_disconnect(handle, HCI_ERR_HOST_REJECT_SECURITY);
    return;
  }

  // good key size - succeed
  btm_acl_encrypt_change(handle, status, 1 /* enable */);
  btm_sec_encrypt_change(handle, status, 1 /* enable */);
}
/*******************************************************************************
 *
 * Function         btu_hcif_encryption_change_evt
 *
 * Description      Process event HCI_ENCRYPTION_CHANGE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_encryption_change_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;
  uint8_t encr_enable;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT8(encr_enable, p);

  if (status != HCI_SUCCESS || encr_enable == 0 || BTM_IsBleConnection(handle)) {
    if (status == HCI_ERR_CONNECTION_TOUT) {
      smp_cancel_start_encryption_attempt();
      return;
    }

    btm_acl_encrypt_change(handle, status, encr_enable);
    btm_sec_encrypt_change(handle, status, encr_enable);
  } else {
    btsnd_hcic_read_encryption_key_size(handle, base::Bind(&read_encryption_key_size_complete_after_encryption_change));
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_read_rmt_features_comp_evt
 *
 * Description      Process event HCI_READ_RMT_FEATURES_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_read_rmt_features_comp_evt(uint8_t* p) {
  btm_read_remote_features_complete(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_read_rmt_ext_features_comp_evt
 *
 * Description      Process event HCI_READ_RMT_EXT_FEATURES_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_read_rmt_ext_features_comp_evt(uint8_t* p,
                                                    uint8_t evt_len) {
  uint8_t* p_cur = p;
  uint8_t status;
  uint16_t handle;

  STREAM_TO_UINT8(status, p_cur);

  if (status == HCI_SUCCESS)
    btm_read_remote_ext_features_complete(p, evt_len);
  else {
    STREAM_TO_UINT16(handle, p_cur);
    btm_read_remote_ext_features_failed(status, handle);
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_read_rmt_version_comp_evt
 *
 * Description      Process event HCI_READ_RMT_VERSION_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_read_rmt_version_comp_evt(uint8_t* p) {
  btm_read_remote_version_complete(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_qos_setup_comp_evt
 *
 * Description      Process event HCI_QOS_SETUP_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_qos_setup_comp_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;
  FLOW_SPEC flow;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT8(flow.qos_flags, p);
  STREAM_TO_UINT8(flow.service_type, p);
  STREAM_TO_UINT32(flow.token_rate, p);
  STREAM_TO_UINT32(flow.peak_bandwidth, p);
  STREAM_TO_UINT32(flow.latency, p);
  STREAM_TO_UINT32(flow.delay_variation, p);

  btm_qos_setup_complete(status, handle, &flow);
}

/*******************************************************************************
 *
 * Function         btu_hcif_esco_connection_comp_evt
 *
 * Description      Process event HCI_ESCO_CONNECTION_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_esco_connection_comp_evt(uint8_t* p) {
  tBTM_ESCO_DATA data;
  uint16_t handle;
  RawAddress bda;
  uint8_t status;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_BDADDR(bda, p);

  STREAM_TO_UINT8(data.link_type, p);
  STREAM_TO_UINT8(data.tx_interval, p);
  STREAM_TO_UINT8(data.retrans_window, p);
  STREAM_TO_UINT16(data.rx_pkt_len, p);
  STREAM_TO_UINT16(data.tx_pkt_len, p);
  STREAM_TO_UINT8(data.air_mode, p);

  handle = HCID_GET_HANDLE(handle);

  data.bd_addr = bda;
  btm_sco_connected(status, &bda, handle, &data);
}

/*******************************************************************************
 *
 * Function         btu_hcif_esco_connection_chg_evt
 *
 * Description      Process event HCI_ESCO_CONNECTION_CHANGED_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_esco_connection_chg_evt(uint8_t* p) {
  uint16_t handle;
  uint16_t tx_pkt_len;
  uint16_t rx_pkt_len;
  uint8_t status;
  uint8_t tx_interval;
  uint8_t retrans_window;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);

  STREAM_TO_UINT8(tx_interval, p);
  STREAM_TO_UINT8(retrans_window, p);
  STREAM_TO_UINT16(rx_pkt_len, p);
  STREAM_TO_UINT16(tx_pkt_len, p);

  handle = HCID_GET_HANDLE(handle);

  btm_esco_proc_conn_chg(status, handle, tx_interval, retrans_window,
                         rx_pkt_len, tx_pkt_len);
}

/*******************************************************************************
 *
 * Function         btu_hcif_hdl_command_complete
 *
 * Description      Handle command complete event
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_hdl_command_complete(uint16_t opcode, uint8_t* p,
                                          uint16_t evt_len,
                                          void* p_cplt_cback) {
  switch (opcode) {
    case HCI_INQUIRY_CANCEL:
      /* Tell inquiry processing that we are done */
      btm_process_cancel_complete(HCI_SUCCESS, BTM_BR_INQUIRY_MASK);
      break;
    case HCI_SET_EVENT_FILTER:
      btm_event_filter_complete(p);
      break;

    case HCI_DELETE_STORED_LINK_KEY:
      btm_delete_stored_link_key_complete(p);
      break;

    case HCI_READ_LOCAL_NAME:
      btm_read_local_name_complete(p, evt_len);
      break;

    case HCI_GET_LINK_QUALITY:
      btm_read_link_quality_complete(p);
      break;

    case HCI_READ_RSSI:
      btm_read_rssi_complete(p);
      break;

    case HCI_READ_FAILED_CONTACT_COUNTER:
      btm_read_failed_contact_counter_complete(p);
      break;

    case HCI_READ_AUTOMATIC_FLUSH_TIMEOUT:
      btm_read_automatic_flush_timeout_complete(p);
      break;

    case HCI_READ_TRANSMIT_POWER_LEVEL:
      btm_read_tx_power_complete(p, false);
      break;

    case HCI_CREATE_CONNECTION_CANCEL:
      btm_create_conn_cancel_complete(p);
      break;

    case HCI_READ_LOCAL_OOB_DATA:
      btm_read_local_oob_complete(p);
      break;

    case HCI_READ_INQ_TX_POWER_LEVEL:
      btm_read_inq_tx_power_complete(p);
      break;

    /* BLE Commands sComplete*/
    case HCI_BLE_RAND:
    case HCI_BLE_ENCRYPT:
      btm_ble_rand_enc_complete(p, opcode, (tBTM_RAND_ENC_CB*)p_cplt_cback);
      break;

    case HCI_BLE_READ_ADV_CHNL_TX_POWER:
      btm_read_tx_power_complete(p, true);
      break;

    case HCI_BLE_WRITE_ADV_ENABLE:
      btm_ble_write_adv_enable_complete(p);
      break;

    case HCI_BLE_CREATE_LL_CONN:
    case HCI_LE_EXTENDED_CREATE_CONNECTION:
      // No command complete event for those commands according to spec
      LOG(ERROR) << "No command complete expected, but received!";
      break;

    case HCI_BLE_CREATE_CONN_CANCEL:
      btm_ble_create_conn_cancel_complete(p);
      break;

    case HCI_BLE_TRANSMITTER_TEST:
    case HCI_BLE_RECEIVER_TEST:
    case HCI_BLE_TEST_END:
      btm_ble_test_command_complete(p);
      break;

#if (BLE_PRIVACY_SPT == TRUE)
    case HCI_BLE_ADD_DEV_RESOLVING_LIST:
      btm_ble_add_resolving_list_entry_complete(p, evt_len);
      break;

    case HCI_BLE_RM_DEV_RESOLVING_LIST:
      btm_ble_remove_resolving_list_entry_complete(p, evt_len);
      break;

    case HCI_BLE_CLEAR_RESOLVING_LIST:
      btm_ble_clear_resolving_list_complete(p, evt_len);
      break;

    case HCI_BLE_READ_RESOLVABLE_ADDR_PEER:
      btm_ble_read_resolving_list_entry_complete(p, evt_len);
      break;

    case HCI_BLE_READ_RESOLVABLE_ADDR_LOCAL:
    case HCI_BLE_SET_ADDR_RESOLUTION_ENABLE:
    case HCI_BLE_SET_RAND_PRIV_ADDR_TIMOUT:
      break;
#endif
    default:
      if ((opcode & HCI_GRP_VENDOR_SPECIFIC) == HCI_GRP_VENDOR_SPECIFIC)
        btm_vsc_complete(p, opcode, evt_len, (tBTM_VSC_CMPL_CB*)p_cplt_cback);
      break;
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_command_complete_evt
 *
 * Description      Process event HCI_COMMAND_COMPLETE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_command_complete_evt_on_task(BT_HDR* event,
                                                  void* context) {
  command_opcode_t opcode;
  // 2 for event header: event code (1) + parameter length (1)
  // 1 for num_hci_pkt command credit
  uint8_t* stream = event->data + event->offset + 3;
  STREAM_TO_UINT16(opcode, stream);

  btu_hcif_log_command_complete_metrics(opcode, stream);
  // 2 for event header: event code (1) + parameter length (1)
  // 3 for command complete header: num_hci_pkt (1) + opcode (2)
  uint16_t param_len = static_cast<uint16_t>(event->len - 5);
  btu_hcif_hdl_command_complete(opcode, stream, param_len, context);

  osi_free(event);
}

static void btu_hcif_command_complete_evt(BT_HDR* response, void* context) {
  do_in_main_thread(FROM_HERE, base::Bind(btu_hcif_command_complete_evt_on_task,
                                          response, context));
}

/*******************************************************************************
 *
 * Function         btu_hcif_hdl_command_status
 *
 * Description      Handle a command status event
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_hdl_command_status(uint16_t opcode, uint8_t status,
                                        uint8_t* p_cmd,
                                        void* p_vsc_status_cback) {
  CHECK_NE(p_cmd, nullptr) << "Null command for opcode 0x" << loghex(opcode);
  p_cmd++;  // Skip parameter total length

  RawAddress bd_addr;
  uint16_t handle;

  switch (opcode) {
    // Link Control Commands
    case HCI_INQUIRY:
      if (status != HCI_SUCCESS) {
        // Tell inquiry processing that we are done
        btm_process_inq_complete(status, BTM_BR_INQUIRY_MASK);
      }
      break;
    case HCI_QOS_SETUP:
      if (status != HCI_SUCCESS) {
        // Tell qos setup that we are done
        btm_qos_setup_complete(status, 0, nullptr);
      }
      break;
    case HCI_SWITCH_ROLE:
      if (status != HCI_SUCCESS) {
        // Tell BTM that the command failed
        STREAM_TO_BDADDR(bd_addr, p_cmd);
        btm_acl_role_changed(status, &bd_addr, BTM_ROLE_UNDEFINED);
        l2c_link_role_changed(nullptr, BTM_ROLE_UNDEFINED,
                              HCI_ERR_COMMAND_DISALLOWED);
      }
      break;
    case HCI_CREATE_CONNECTION:
      if (status != HCI_SUCCESS) {
        STREAM_TO_BDADDR(bd_addr, p_cmd);
        btm_sec_connected(bd_addr, HCI_INVALID_HANDLE, status, 0);
        l2c_link_hci_conn_comp(status, HCI_INVALID_HANDLE, bd_addr);
      }
      break;
    case HCI_AUTHENTICATION_REQUESTED:
      if (status != HCI_SUCCESS) {
        // Device refused to start authentication
        // This is treated as an authentication failure
        btm_sec_auth_complete(BTM_INVALID_HCI_HANDLE, status);
      }
      break;
    case HCI_SET_CONN_ENCRYPTION:
      if (status != HCI_SUCCESS) {
        // Device refused to start encryption
        // This is treated as an encryption failure
        btm_sec_encrypt_change(BTM_INVALID_HCI_HANDLE, status, false);
      }
      break;
    case HCI_RMT_NAME_REQUEST:
      if (status != HCI_SUCCESS) {
        // Tell inquiry processing that we are done
        btm_process_remote_name(nullptr, nullptr, 0, status);
        btm_sec_rmt_name_request_complete(nullptr, nullptr, status);
      }
      break;
    case HCI_READ_RMT_EXT_FEATURES:
      if (status != HCI_SUCCESS) {
        STREAM_TO_UINT16(handle, p_cmd);
        btm_read_remote_ext_features_failed(status, handle);
      }
      break;
    case HCI_SETUP_ESCO_CONNECTION:
    case HCI_ENH_SETUP_ESCO_CONNECTION:
      if (status != HCI_SUCCESS) {
        STREAM_TO_UINT16(handle, p_cmd);
        // Determine if initial connection failed or is a change of setup
        if (btm_is_sco_active(handle)) {
          btm_esco_proc_conn_chg(status, handle, 0, 0, 0, 0);
        } else {
          btm_sco_connected(status, nullptr, handle, nullptr);
        }
      }
      break;

    // BLE Commands
    case HCI_BLE_CREATE_LL_CONN:
    case HCI_LE_EXTENDED_CREATE_CONNECTION:
      if (status != HCI_SUCCESS) {
        btm_ble_create_ll_conn_complete(status);
      }
      break;
    case HCI_BLE_START_ENC:
      // Race condition: disconnection happened right before we send
      // "LE Encrypt", controller responds with no connection, we should
      // cancel the encryption attempt, rather than unpair the device.
      if (status == HCI_ERR_NO_CONNECTION) {
        smp_cancel_start_encryption_attempt();
      }
      break;

    // Link Policy Commands
    case HCI_EXIT_SNIFF_MODE:
    case HCI_EXIT_PARK_MODE:
      if (status != HCI_SUCCESS) {
        // Allow SCO initiation to continue if waiting for change mode event
        STREAM_TO_UINT16(handle, p_cmd);
        btm_sco_chk_pend_unpark(status, handle);
      }
      FALLTHROUGH_INTENDED; /* FALLTHROUGH */
    case HCI_HOLD_MODE:
    case HCI_SNIFF_MODE:
    case HCI_PARK_MODE:
      btm_pm_proc_cmd_status(status);
      break;

    default:
      if ((opcode & HCI_GRP_VENDOR_SPECIFIC) == HCI_GRP_VENDOR_SPECIFIC) {
        btm_vsc_complete(&status, opcode, 1,
                         (tBTM_VSC_CMPL_CB*)p_vsc_status_cback);
      }
  }
}

/*******************************************************************************
 *
 * Function         btu_hcif_command_status_evt
 *
 * Description      Process event HCI_COMMAND_STATUS_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_command_status_evt_on_task(uint8_t status, BT_HDR* event,
                                                void* context) {
  command_opcode_t opcode;
  uint8_t* stream = event->data + event->offset;
  STREAM_TO_UINT16(opcode, stream);

  // stream + 1 to skip parameter length field
  // No need to check length since stream is written by us
  btu_hcif_log_command_metrics(opcode, stream + 1, status, true);

  btu_hcif_hdl_command_status(opcode, status, stream, context);
  osi_free(event);
}

static void btu_hcif_command_status_evt(uint8_t status, BT_HDR* command,
                                        void* context) {
  do_in_main_thread(FROM_HERE, base::Bind(btu_hcif_command_status_evt_on_task,
                                          status, command, context));
}

/*******************************************************************************
 *
 * Function         btu_hcif_hardware_error_evt
 *
 * Description      Process event HCI_HARDWARE_ERROR_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_hardware_error_evt(uint8_t* p) {
  HCI_TRACE_ERROR("Ctlr H/w error event - code:0x%x", *p);

  /* If anyone wants device status notifications, give him one. */
  btm_report_device_status(BTM_DEV_STATUS_DOWN);

  /* Reset the controller */
  if (BTM_IsDeviceUp()) BTM_DeviceReset(NULL);
}

/*******************************************************************************
 *
 * Function         btu_hcif_flush_occured_evt
 *
 * Description      Process event HCI_FLUSH_OCCURED_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_flush_occured_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_role_change_evt
 *
 * Description      Process event HCI_ROLE_CHANGE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_role_change_evt(uint8_t* p) {
  uint8_t status;
  RawAddress bda;
  uint8_t role;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_BDADDR(bda, p);
  STREAM_TO_UINT8(role, p);

  btm_blacklist_role_change_device(bda, status);
  l2c_link_role_changed(&bda, role, status);
  btm_acl_role_changed(status, &bda, role);
}

/*******************************************************************************
 *
 * Function         btu_hcif_num_compl_data_pkts_evt
 *
 * Description      Process event HCI_NUM_COMPL_DATA_PKTS_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_num_compl_data_pkts_evt(uint8_t* p, uint8_t evt_len) {
  /* Process for L2CAP and SCO */
  l2c_link_process_num_completed_pkts(p, evt_len);

  /* Send on to SCO */
  /*?? No SCO for now */
}

/*******************************************************************************
 *
 * Function         btu_hcif_mode_change_evt
 *
 * Description      Process event HCI_MODE_CHANGE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_mode_change_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;
  uint8_t current_mode;
  uint16_t interval;

  STREAM_TO_UINT8(status, p);

  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT8(current_mode, p);
  STREAM_TO_UINT16(interval, p);
  btm_sco_chk_pend_unpark(status, handle);
  btm_pm_proc_mode_change(status, handle, current_mode, interval);

#if (HID_DEV_INCLUDED == TRUE && HID_DEV_PM_INCLUDED == TRUE)
  hidd_pm_proc_mode_change(status, current_mode, interval);
#endif
}

/*******************************************************************************
 *
 * Function         btu_hcif_ssr_evt
 *
 * Description      Process event HCI_SNIFF_SUB_RATE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
#if (BTM_SSR_INCLUDED == TRUE)
static void btu_hcif_ssr_evt(uint8_t* p, uint16_t evt_len) {
  btm_pm_proc_ssr_evt(p, evt_len);
}
#endif

/*******************************************************************************
 *
 * Function         btu_hcif_pin_code_request_evt
 *
 * Description      Process event HCI_PIN_CODE_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_pin_code_request_evt(uint8_t* p) {
  RawAddress bda;

  STREAM_TO_BDADDR(bda, p);

  /* Tell L2CAP that there was a PIN code request,  */
  /* it may need to stretch timeouts                */
  l2c_pin_code_request(bda);

  btm_sec_pin_code_request(bda);
}

/*******************************************************************************
 *
 * Function         btu_hcif_link_key_request_evt
 *
 * Description      Process event HCI_LINK_KEY_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_link_key_request_evt(uint8_t* p) {
  RawAddress bda;

  STREAM_TO_BDADDR(bda, p);
  btm_sec_link_key_request(bda);
}

/*******************************************************************************
 *
 * Function         btu_hcif_link_key_notification_evt
 *
 * Description      Process event HCI_LINK_KEY_NOTIFICATION_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_link_key_notification_evt(uint8_t* p) {
  RawAddress bda;
  Octet16 key;
  uint8_t key_type;

  STREAM_TO_BDADDR(bda, p);
  STREAM_TO_ARRAY16(key.data(), p);
  STREAM_TO_UINT8(key_type, p);

  btm_sec_link_key_notification(bda, key, key_type);
}

/*******************************************************************************
 *
 * Function         btu_hcif_loopback_command_evt
 *
 * Description      Process event HCI_LOOPBACK_COMMAND_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_loopback_command_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_data_buf_overflow_evt
 *
 * Description      Process event HCI_DATA_BUF_OVERFLOW_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_data_buf_overflow_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_max_slots_changed_evt
 *
 * Description      Process event HCI_MAX_SLOTS_CHANGED_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_max_slots_changed_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_read_clock_off_comp_evt
 *
 * Description      Process event HCI_READ_CLOCK_OFF_COMP_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_read_clock_off_comp_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;
  uint16_t clock_offset;

  STREAM_TO_UINT8(status, p);

  /* If failed to get clock offset just drop the result */
  if (status != HCI_SUCCESS) return;

  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT16(clock_offset, p);

  handle = HCID_GET_HANDLE(handle);

  btm_process_clk_off_comp_evt(handle, clock_offset);
  btm_sec_update_clock_offset(handle, clock_offset);
}

/*******************************************************************************
 *
 * Function         btu_hcif_conn_pkt_type_change_evt
 *
 * Description      Process event HCI_CONN_PKT_TYPE_CHANGE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_conn_pkt_type_change_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_qos_violation_evt
 *
 * Description      Process event HCI_QOS_VIOLATION_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_qos_violation_evt(uint8_t* p) {
  uint16_t handle;

  STREAM_TO_UINT16(handle, p);

  handle = HCID_GET_HANDLE(handle);

  l2c_link_hci_qos_violation(handle);
}

/*******************************************************************************
 *
 * Function         btu_hcif_page_scan_mode_change_evt
 *
 * Description      Process event HCI_PAGE_SCAN_MODE_CHANGE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_page_scan_mode_change_evt(void) {}

/*******************************************************************************
 *
 * Function         btu_hcif_page_scan_rep_mode_chng_evt
 *
 * Description      Process event HCI_PAGE_SCAN_REP_MODE_CHNG_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_page_scan_rep_mode_chng_evt(void) {}

/**********************************************
 * Simple Pairing Events
 **********************************************/

/*******************************************************************************
 *
 * Function         btu_hcif_host_support_evt
 *
 * Description      Process event HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_host_support_evt(uint8_t* p) {
  btm_sec_rmt_host_support_feat_evt(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_io_cap_request_evt
 *
 * Description      Process event HCI_IO_CAPABILITY_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_io_cap_request_evt(uint8_t* p) {
  RawAddress bda;
  STREAM_TO_BDADDR(bda, p);
  btm_io_capabilities_req(bda);
}

/*******************************************************************************
 *
 * Function         btu_hcif_io_cap_response_evt
 *
 * Description      Process event HCI_IO_CAPABILITY_RESPONSE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_io_cap_response_evt(uint8_t* p) {
  btm_io_capabilities_rsp(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_user_conf_request_evt
 *
 * Description      Process event HCI_USER_CONFIRMATION_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_user_conf_request_evt(uint8_t* p) {
  btm_proc_sp_req_evt(BTM_SP_CFM_REQ_EVT, p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_user_passkey_request_evt
 *
 * Description      Process event HCI_USER_PASSKEY_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_user_passkey_request_evt(uint8_t* p) {
  btm_proc_sp_req_evt(BTM_SP_KEY_REQ_EVT, p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_user_passkey_notif_evt
 *
 * Description      Process event HCI_USER_PASSKEY_NOTIFY_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_user_passkey_notif_evt(uint8_t* p) {
  btm_proc_sp_req_evt(BTM_SP_KEY_NOTIF_EVT, p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_keypress_notif_evt
 *
 * Description      Process event HCI_KEYPRESS_NOTIFY_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_keypress_notif_evt(uint8_t* p) {
  btm_keypress_notif_evt(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_rem_oob_request_evt
 *
 * Description      Process event HCI_REMOTE_OOB_DATA_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_rem_oob_request_evt(uint8_t* p) { btm_rem_oob_req(p); }

/*******************************************************************************
 *
 * Function         btu_hcif_simple_pair_complete_evt
 *
 * Description      Process event HCI_SIMPLE_PAIRING_COMPLETE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
static void btu_hcif_simple_pair_complete_evt(uint8_t* p) {
  btm_simple_pair_complete(p);
}

/*******************************************************************************
 *
 * Function         btu_hcif_enhanced_flush_complete_evt
 *
 * Description      Process event HCI_ENHANCED_FLUSH_COMPLETE_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
static void btu_hcif_enhanced_flush_complete_evt(void) {
  /* This is empty until an upper layer cares about returning event */
}
#endif
/**********************************************
 * End of Simple Pairing Events
 **********************************************/

static void read_encryption_key_size_complete_after_key_refresh(uint8_t status, uint16_t handle, uint8_t key_size) {
  if (status == HCI_ERR_INSUFFCIENT_SECURITY) {
    /* If remote device stop the encryption before we call "Read Encryption Key
     * Size", we might receive Insufficient Security, which means that link is
     * no longer encrypted. */
    LOG(INFO) << __func__ << ": encryption stopped on link: " << loghex(handle);
    return;
  }

  if (status != HCI_SUCCESS) {
    LOG(INFO) << __func__ << ": disconnecting, status: " << loghex(status);
    btsnd_hcic_disconnect(handle, HCI_ERR_PEER_USER);
    return;
  }

  if (key_size < MIN_KEY_SIZE) {
    android_errorWriteLog(0x534e4554, "124301137");
    LOG(ERROR) << __func__ << " encryption key too short, disconnecting. handle: " << loghex(handle)
               << " key_size: " << +key_size;

    btsnd_hcic_disconnect(handle, HCI_ERR_HOST_REJECT_SECURITY);
    return;
  }

  btm_sec_encrypt_change(handle, status, 1 /* enc_enable */);
}

static void btu_hcif_encryption_key_refresh_cmpl_evt(uint8_t* p) {
  uint8_t status;
  uint16_t handle;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);

  if (status != HCI_SUCCESS || BTM_IsBleConnection(handle)) {
    btm_sec_encrypt_change(handle, status, (status == HCI_SUCCESS) ? 1 : 0);
  } else {
    btsnd_hcic_read_encryption_key_size(handle, base::Bind(&read_encryption_key_size_complete_after_key_refresh));
  }
}

/**********************************************
 * BLE Events
 **********************************************/

static void btu_ble_ll_conn_complete_evt(uint8_t* p, uint16_t evt_len) {
  btm_ble_conn_complete(p, evt_len, false);
}
#if (BLE_PRIVACY_SPT == TRUE)
static void btu_ble_proc_enhanced_conn_cmpl(uint8_t* p, uint16_t evt_len) {
  btm_ble_conn_complete(p, evt_len, true);
}
#endif

extern void gatt_notify_conn_update(uint16_t handle, uint16_t interval,
                                    uint16_t latency, uint16_t timeout,
                                    uint8_t status);

static void btu_ble_ll_conn_param_upd_evt(uint8_t* p, uint16_t evt_len) {
  /* LE connection update has completed successfully as a master. */
  /* We can enable the update request if the result is a success. */
  /* extract the HCI handle first */
  uint8_t status;
  uint16_t handle;
  uint16_t interval;
  uint16_t latency;
  uint16_t timeout;

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT16(interval, p);
  STREAM_TO_UINT16(latency, p);
  STREAM_TO_UINT16(timeout, p);

  l2cble_process_conn_update_evt(handle, status, interval, latency, timeout);

  gatt_notify_conn_update(handle & 0x0FFF, interval, latency, timeout, status);
}

static void btu_ble_read_remote_feat_evt(uint8_t* p) {
  btm_ble_read_remote_features_complete(p);
}

static void btu_ble_proc_ltk_req(uint8_t* p) {
  uint16_t ediv, handle;
  uint8_t* pp;

  STREAM_TO_UINT16(handle, p);
  pp = p + 8;
  STREAM_TO_UINT16(ediv, pp);
  btm_ble_ltk_request(handle, p, ediv);
  /* This is empty until an upper layer cares about returning event */
}

static void btu_ble_data_length_change_evt(uint8_t* p, uint16_t evt_len) {
  uint16_t handle;
  uint16_t tx_data_len;
  uint16_t rx_data_len;

  if (!controller_get_interface()->supports_ble_packet_extension()) {
    HCI_TRACE_WARNING("%s, request not supported", __func__);
    return;
  }

  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT16(tx_data_len, p);
  p += 2; /* Skip the TxTimer */
  STREAM_TO_UINT16(rx_data_len, p);

  l2cble_process_data_length_change_event(handle, tx_data_len, rx_data_len);
}

/**********************************************
 * End of BLE Events Handler
 **********************************************/
#if (BLE_LLT_INCLUDED == TRUE)
static void btu_ble_rc_param_req_evt(uint8_t* p) {
  uint16_t handle;
  uint16_t int_min, int_max, latency, timeout;

  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT16(int_min, p);
  STREAM_TO_UINT16(int_max, p);
  STREAM_TO_UINT16(latency, p);
  STREAM_TO_UINT16(timeout, p);

  l2cble_process_rc_param_request_evt(handle, int_min, int_max, latency,
                                      timeout);
}
#endif /* BLE_LLT_INCLUDED */
