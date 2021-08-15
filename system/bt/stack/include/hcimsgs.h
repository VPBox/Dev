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

#ifndef HCIMSGS_H
#define HCIMSGS_H

#include "bt_target.h"
#include "bt_types.h"
#include "device/include/esco_parameters.h"
#include "hcidefs.h"

#include <base/callback_forward.h>

void bte_main_hci_send(BT_HDR* p_msg, uint16_t event);

/* Message by message.... */

extern void btsnd_hcic_inquiry(const LAP inq_lap, uint8_t duration,
                               uint8_t response_cnt);

#define HCIC_PARAM_SIZE_INQUIRY 5

#define HCIC_INQ_INQ_LAP_OFF 0
#define HCIC_INQ_DUR_OFF 3
#define HCIC_INQ_RSP_CNT_OFF 4
/* Inquiry */

/* Inquiry Cancel */
extern void btsnd_hcic_inq_cancel(void);

#define HCIC_PARAM_SIZE_INQ_CANCEL 0

/* Periodic Inquiry Mode */
extern void btsnd_hcic_per_inq_mode(uint16_t max_period, uint16_t min_period,
                                    const LAP inq_lap, uint8_t duration,
                                    uint8_t response_cnt);

#define HCIC_PARAM_SIZE_PER_INQ_MODE 9

#define HCI_PER_INQ_MAX_INTRVL_OFF 0
#define HCI_PER_INQ_MIN_INTRVL_OFF 2
#define HCI_PER_INQ_INQ_LAP_OFF 4
#define HCI_PER_INQ_DURATION_OFF 7
#define HCI_PER_INQ_RSP_CNT_OFF 8
/* Periodic Inquiry Mode */

/* Exit Periodic Inquiry Mode */
extern void btsnd_hcic_exit_per_inq(void);

#define HCIC_PARAM_SIZE_EXIT_PER_INQ 0
/* Create Connection */
extern void btsnd_hcic_create_conn(const RawAddress& dest,
                                   uint16_t packet_types,
                                   uint8_t page_scan_rep_mode,
                                   uint8_t page_scan_mode,
                                   uint16_t clock_offset, uint8_t allow_switch);

#define HCIC_PARAM_SIZE_CREATE_CONN 13

#define HCIC_CR_CONN_BD_ADDR_OFF 0
#define HCIC_CR_CONN_PKT_TYPES_OFF 6
#define HCIC_CR_CONN_REP_MODE_OFF 8
#define HCIC_CR_CONN_PAGE_SCAN_MODE_OFF 9
#define HCIC_CR_CONN_CLK_OFF_OFF 10
#define HCIC_CR_CONN_ALLOW_SWITCH_OFF 12
/* Create Connection */

/* Disconnect */
extern void btsnd_hcic_disconnect(uint16_t handle, uint8_t reason);

#define HCIC_PARAM_SIZE_DISCONNECT 3

#define HCI_DISC_HANDLE_OFF 0
#define HCI_DISC_REASON_OFF 2
/* Disconnect */

/* Add SCO Connection */
extern void btsnd_hcic_add_SCO_conn(uint16_t handle, uint16_t packet_types);

#define HCIC_PARAM_SIZE_ADD_SCO_CONN 4

#define HCI_ADD_SCO_HANDLE_OFF 0
#define HCI_ADD_SCO_PACKET_TYPES_OFF 2
/* Add SCO Connection */

/* Create Connection Cancel */
extern void btsnd_hcic_create_conn_cancel(const RawAddress& dest);

#define HCIC_PARAM_SIZE_CREATE_CONN_CANCEL 6

#define HCIC_CR_CONN_CANCEL_BD_ADDR_OFF 0
/* Create Connection Cancel */

/* Accept Connection Request */
extern void btsnd_hcic_accept_conn(const RawAddress& bd_addr, uint8_t role);

#define HCIC_PARAM_SIZE_ACCEPT_CONN 7

#define HCI_ACC_CONN_BD_ADDR_OFF 0
#define HCI_ACC_CONN_ROLE_OFF 6
/* Accept Connection Request */

/* Reject Connection Request */
extern void btsnd_hcic_reject_conn(const RawAddress& bd_addr, uint8_t reason);

#define HCIC_PARAM_SIZE_REJECT_CONN 7

#define HCI_REJ_CONN_BD_ADDR_OFF 0
#define HCI_REJ_CONN_REASON_OFF 6
/* Reject Connection Request */

/* Link Key Request Reply */
extern void btsnd_hcic_link_key_req_reply(const RawAddress& bd_addr,
                                          const LinkKey& link_key);

#define HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY 22

#define HCI_LINK_KEY_REPLY_BD_ADDR_OFF 0
#define HCI_LINK_KEY_REPLY_LINK_KEY_OFF 6
/* Link Key Request Reply  */

/* Link Key Request Neg Reply */
extern void btsnd_hcic_link_key_neg_reply(const RawAddress& bd_addr);

#define HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY 6

#define HCI_LINK_KEY_NEG_REP_BD_ADR_OFF 0
/* Link Key Request Neg Reply  */

/* PIN Code Request Reply */
extern void btsnd_hcic_pin_code_req_reply(const RawAddress& bd_addr,
                                          uint8_t pin_code_len,
                                          PIN_CODE pin_code);

#define HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY 23

#define HCI_PIN_CODE_REPLY_BD_ADDR_OFF 0
#define HCI_PIN_CODE_REPLY_PIN_LEN_OFF 6
#define HCI_PIN_CODE_REPLY_PIN_CODE_OFF 7
/* PIN Code Request Reply  */

/* Link Key Request Neg Reply */
extern void btsnd_hcic_pin_code_neg_reply(const RawAddress& bd_addr);

#define HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY 6

#define HCI_PIN_CODE_NEG_REP_BD_ADR_OFF 0
/* Link Key Request Neg Reply  */

/* Change Connection Type */
extern void btsnd_hcic_change_conn_type(uint16_t handle, uint16_t packet_types);

#define HCIC_PARAM_SIZE_CHANGE_CONN_TYPE 4

#define HCI_CHNG_PKT_TYPE_HANDLE_OFF 0
#define HCI_CHNG_PKT_TYPE_PKT_TYPE_OFF 2
/* Change Connection Type */

#define HCIC_PARAM_SIZE_CMD_HANDLE 2

#define HCI_CMD_HANDLE_HANDLE_OFF 0

extern void btsnd_hcic_auth_request(
    uint16_t handle); /* Authentication Request */

/* Set Connection Encryption */
extern void btsnd_hcic_set_conn_encrypt(uint16_t handle, bool enable);
#define HCIC_PARAM_SIZE_SET_CONN_ENCRYPT 3

#define HCI_SET_ENCRYPT_HANDLE_OFF 0
#define HCI_SET_ENCRYPT_ENABLE_OFF 2
/* Set Connection Encryption */

/* Remote Name Request */
extern void btsnd_hcic_rmt_name_req(const RawAddress& bd_addr,
                                    uint8_t page_scan_rep_mode,
                                    uint8_t page_scan_mode,
                                    uint16_t clock_offset);

#define HCIC_PARAM_SIZE_RMT_NAME_REQ 10

#define HCI_RMT_NAME_BD_ADDR_OFF 0
#define HCI_RMT_NAME_REP_MODE_OFF 6
#define HCI_RMT_NAME_PAGE_SCAN_MODE_OFF 7
#define HCI_RMT_NAME_CLK_OFF_OFF 8
/* Remote Name Request */

/* Remote Name Request Cancel */
extern void btsnd_hcic_rmt_name_req_cancel(const RawAddress& bd_addr);

#define HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL 6

#define HCI_RMT_NAME_CANCEL_BD_ADDR_OFF 0
/* Remote Name Request Cancel */

extern void btsnd_hcic_rmt_features_req(
    uint16_t handle); /* Remote Features Request */

/* Remote Extended Features */
extern void btsnd_hcic_rmt_ext_features(uint16_t handle, uint8_t page_num);

#define HCIC_PARAM_SIZE_RMT_EXT_FEATURES 3

#define HCI_RMT_EXT_FEATURES_HANDLE_OFF 0
#define HCI_RMT_EXT_FEATURES_PAGE_NUM_OFF 2
/* Remote Extended Features */

extern void btsnd_hcic_rmt_ver_req(
    uint16_t handle); /* Remote Version Info Request */
extern void btsnd_hcic_read_rmt_clk_offset(
    uint16_t handle); /* Remote Clock Offset */
extern void btsnd_hcic_read_lmp_handle(uint16_t handle); /* Remote LMP Handle */
extern void btsnd_hcic_setup_esco_conn(uint16_t handle,
                                       uint32_t transmit_bandwidth,
                                       uint32_t receive_bandwidth,
                                       uint16_t max_latency, uint16_t voice,
                                       uint8_t retrans_effort,
                                       uint16_t packet_types);
#define HCIC_PARAM_SIZE_SETUP_ESCO 17

#define HCI_SETUP_ESCO_HANDLE_OFF 0
#define HCI_SETUP_ESCO_TX_BW_OFF 2
#define HCI_SETUP_ESCO_RX_BW_OFF 6
#define HCI_SETUP_ESCO_MAX_LAT_OFF 10
#define HCI_SETUP_ESCO_VOICE_OFF 12
#define HCI_SETUP_ESCO_RETRAN_EFF_OFF 14
#define HCI_SETUP_ESCO_PKT_TYPES_OFF 15

extern void btsnd_hcic_accept_esco_conn(
    const RawAddress& bd_addr, uint32_t transmit_bandwidth,
    uint32_t receive_bandwidth, uint16_t max_latency, uint16_t content_fmt,
    uint8_t retrans_effort, uint16_t packet_types);
#define HCIC_PARAM_SIZE_ACCEPT_ESCO 21

#define HCI_ACCEPT_ESCO_BDADDR_OFF 0
#define HCI_ACCEPT_ESCO_TX_BW_OFF 6
#define HCI_ACCEPT_ESCO_RX_BW_OFF 10
#define HCI_ACCEPT_ESCO_MAX_LAT_OFF 14
#define HCI_ACCEPT_ESCO_VOICE_OFF 16
#define HCI_ACCEPT_ESCO_RETRAN_EFF_OFF 18
#define HCI_ACCEPT_ESCO_PKT_TYPES_OFF 19

extern void btsnd_hcic_reject_esco_conn(const RawAddress& bd_addr,
                                        uint8_t reason);
#define HCIC_PARAM_SIZE_REJECT_ESCO 7

#define HCI_REJECT_ESCO_BDADDR_OFF 0
#define HCI_REJECT_ESCO_REASON_OFF 6

/* Hold Mode */
extern void btsnd_hcic_hold_mode(uint16_t handle, uint16_t max_hold_period,
                                 uint16_t min_hold_period);

#define HCIC_PARAM_SIZE_HOLD_MODE 6

#define HCI_HOLD_MODE_HANDLE_OFF 0
#define HCI_HOLD_MODE_MAX_PER_OFF 2
#define HCI_HOLD_MODE_MIN_PER_OFF 4
/* Hold Mode */

/* Sniff Mode */
extern void btsnd_hcic_sniff_mode(uint16_t handle, uint16_t max_sniff_period,
                                  uint16_t min_sniff_period,
                                  uint16_t sniff_attempt,
                                  uint16_t sniff_timeout);

#define HCIC_PARAM_SIZE_SNIFF_MODE 10

#define HCI_SNIFF_MODE_HANDLE_OFF 0
#define HCI_SNIFF_MODE_MAX_PER_OFF 2
#define HCI_SNIFF_MODE_MIN_PER_OFF 4
#define HCI_SNIFF_MODE_ATTEMPT_OFF 6
#define HCI_SNIFF_MODE_TIMEOUT_OFF 8
/* Sniff Mode */

extern void btsnd_hcic_exit_sniff_mode(uint16_t handle); /* Exit Sniff Mode */

/* Park Mode */
extern void btsnd_hcic_park_mode(uint16_t handle, uint16_t beacon_max_interval,
                                 uint16_t beacon_min_interval);

#define HCIC_PARAM_SIZE_PARK_MODE 6

#define HCI_PARK_MODE_HANDLE_OFF 0
#define HCI_PARK_MODE_MAX_PER_OFF 2
#define HCI_PARK_MODE_MIN_PER_OFF 4
/* Park Mode */

extern void btsnd_hcic_exit_park_mode(uint16_t handle); /* Exit Park Mode */

/* QoS Setup */
extern void btsnd_hcic_qos_setup(uint16_t handle, uint8_t flags,
                                 uint8_t service_type, uint32_t token_rate,
                                 uint32_t peak, uint32_t latency,
                                 uint32_t delay_var);

#define HCIC_PARAM_SIZE_QOS_SETUP 20

#define HCI_QOS_HANDLE_OFF 0
#define HCI_QOS_FLAGS_OFF 2
#define HCI_QOS_SERVICE_TYPE_OFF 3
#define HCI_QOS_TOKEN_RATE_OFF 4
#define HCI_QOS_PEAK_BANDWIDTH_OFF 8
#define HCI_QOS_LATENCY_OFF 12
#define HCI_QOS_DELAY_VAR_OFF 16
/* QoS Setup */

/* Switch Role Request */
extern void btsnd_hcic_switch_role(const RawAddress& bd_addr, uint8_t role);

#define HCIC_PARAM_SIZE_SWITCH_ROLE 7

#define HCI_SWITCH_BD_ADDR_OFF 0
#define HCI_SWITCH_ROLE_OFF 6
/* Switch Role Request */

/* Write Policy Settings */
extern void btsnd_hcic_write_policy_set(uint16_t handle, uint16_t settings);

#define HCIC_PARAM_SIZE_WRITE_POLICY_SET 4

#define HCI_WRITE_POLICY_HANDLE_OFF 0
#define HCI_WRITE_POLICY_SETTINGS_OFF 2
/* Write Policy Settings */

/* Write Default Policy Settings */
extern void btsnd_hcic_write_def_policy_set(uint16_t settings);

#define HCIC_PARAM_SIZE_WRITE_DEF_POLICY_SET 2

#define HCI_WRITE_DEF_POLICY_SETTINGS_OFF 0
/* Write Default Policy Settings */

/******************************************
 *    Lisbon Features
 ******************************************/
#if (BTM_SSR_INCLUDED == TRUE)
/* Sniff Subrating */
extern void btsnd_hcic_sniff_sub_rate(uint16_t handle, uint16_t max_lat,
                                      uint16_t min_remote_lat,
                                      uint16_t min_local_lat);

#define HCIC_PARAM_SIZE_SNIFF_SUB_RATE 8

#define HCI_SNIFF_SUB_RATE_HANDLE_OFF 0
#define HCI_SNIFF_SUB_RATE_MAX_LAT_OFF 2
#define HCI_SNIFF_SUB_RATE_MIN_REM_LAT_OFF 4
#define HCI_SNIFF_SUB_RATE_MIN_LOC_LAT_OFF 6
/* Sniff Subrating */

#else /* BTM_SSR_INCLUDED == FALSE */

#define btsnd_hcic_sniff_sub_rate(handle, max_lat, min_remote_lat, \
                                  min_local_lat)                   \
  false

#endif /* BTM_SSR_INCLUDED */

/* Extended Inquiry Response */
extern void btsnd_hcic_write_ext_inquiry_response(void* buffer,
                                                  uint8_t fec_req);

#define HCIC_PARAM_SIZE_EXT_INQ_RESP 241

#define HCIC_EXT_INQ_RESP_FEC_OFF 0
#define HCIC_EXT_INQ_RESP_RESPONSE 1
/* IO Capabilities Response */
extern void btsnd_hcic_io_cap_req_reply(const RawAddress& bd_addr,
                                        uint8_t capability, uint8_t oob_present,
                                        uint8_t auth_req);

#define HCIC_PARAM_SIZE_IO_CAP_RESP 9

#define HCI_IO_CAP_BD_ADDR_OFF 0
#define HCI_IO_CAPABILITY_OFF 6
#define HCI_IO_CAP_OOB_DATA_OFF 7
#define HCI_IO_CAP_AUTH_REQ_OFF 8

/* IO Capabilities Req Neg Reply */
extern void btsnd_hcic_io_cap_req_neg_reply(const RawAddress& bd_addr,
                                            uint8_t err_code);

#define HCIC_PARAM_SIZE_IO_CAP_NEG_REPLY 7

#define HCI_IO_CAP_NR_BD_ADDR_OFF 0
#define HCI_IO_CAP_NR_ERR_CODE 6

/* Read Local OOB Data */
extern void btsnd_hcic_read_local_oob_data(void);

#define HCIC_PARAM_SIZE_R_LOCAL_OOB 0

extern void btsnd_hcic_user_conf_reply(const RawAddress& bd_addr, bool is_yes);

#define HCIC_PARAM_SIZE_UCONF_REPLY 6

#define HCI_USER_CONF_BD_ADDR_OFF 0

extern void btsnd_hcic_user_passkey_reply(const RawAddress& bd_addr,
                                          uint32_t value);

#define HCIC_PARAM_SIZE_U_PKEY_REPLY 10

#define HCI_USER_PASSKEY_BD_ADDR_OFF 0
#define HCI_USER_PASSKEY_VALUE_OFF 6

extern void btsnd_hcic_user_passkey_neg_reply(const RawAddress& bd_addr);

#define HCIC_PARAM_SIZE_U_PKEY_NEG_REPLY 6

#define HCI_USER_PASSKEY_NEG_BD_ADDR_OFF 0

/* Remote OOB Data Request Reply */
extern void btsnd_hcic_rem_oob_reply(const RawAddress& bd_addr,
                                     const Octet16& c, const Octet16& r);

#define HCIC_PARAM_SIZE_REM_OOB_REPLY 38

#define HCI_REM_OOB_DATA_BD_ADDR_OFF 0
#define HCI_REM_OOB_DATA_C_OFF 6
#define HCI_REM_OOB_DATA_R_OFF 22

/* Remote OOB Data Request Negative Reply */
extern void btsnd_hcic_rem_oob_neg_reply(const RawAddress& bd_addr);

#define HCIC_PARAM_SIZE_REM_OOB_NEG_REPLY 6

#define HCI_REM_OOB_DATA_NEG_BD_ADDR_OFF 0

/* Read Tx Power Level */
extern void btsnd_hcic_read_inq_tx_power(void);

#define HCIC_PARAM_SIZE_R_TX_POWER 0

/* Read Default Erroneous Data Reporting */
extern void btsnd_hcic_read_default_erroneous_data_rpt(void);

#define HCIC_PARAM_SIZE_R_ERR_DATA_RPT 0

#if (L2CAP_NON_FLUSHABLE_PB_INCLUDED == TRUE)
extern void btsnd_hcic_enhanced_flush(uint16_t handle, uint8_t packet_type);

#define HCIC_PARAM_SIZE_ENHANCED_FLUSH 3
#endif

extern void btsnd_hcic_send_keypress_notif(const RawAddress& bd_addr,
                                           uint8_t notif);

#define HCIC_PARAM_SIZE_SEND_KEYPRESS_NOTIF 7

#define HCI_SEND_KEYPRESS_NOTIF_BD_ADDR_OFF 0
#define HCI_SEND_KEYPRESS_NOTIF_NOTIF_OFF 6

/**** end of Simple Pairing Commands ****/

/* Store Current Settings */
#define MAX_FILT_COND (sizeof(BD_ADDR) + 1)

extern void btsnd_hcic_set_event_filter(uint8_t filt_type,
                                        uint8_t filt_cond_type,
                                        uint8_t* filt_cond,
                                        uint8_t filt_cond_len);

#define HCIC_PARAM_SIZE_SET_EVT_FILTER 9

#define HCI_FILT_COND_FILT_TYPE_OFF 0
#define HCI_FILT_COND_COND_TYPE_OFF 1
#define HCI_FILT_COND_FILT_OFF 2
/* Set Event Filter */

/* Delete Stored Key */
extern void btsnd_hcic_delete_stored_key(const RawAddress& bd_addr,
                                         bool delete_all_flag);

#define HCIC_PARAM_SIZE_DELETE_STORED_KEY 7

#define HCI_DELETE_KEY_BD_ADDR_OFF 0
#define HCI_DELETE_KEY_ALL_FLAG_OFF 6
/* Delete Stored Key */

/* Change Local Name */
extern void btsnd_hcic_change_name(BD_NAME name);

#define HCIC_PARAM_SIZE_CHANGE_NAME BD_NAME_LEN

#define HCI_CHANGE_NAME_NAME_OFF 0
/* Change Local Name */

#define HCIC_PARAM_SIZE_READ_CMD 0

#define HCIC_PARAM_SIZE_WRITE_PARAM1 1

#define HCIC_WRITE_PARAM1_PARAM_OFF 0

#define HCIC_PARAM_SIZE_WRITE_PARAM2 2

#define HCIC_WRITE_PARAM2_PARAM_OFF 0

#define HCIC_PARAM_SIZE_WRITE_PARAM3 3

#define HCIC_WRITE_PARAM3_PARAM_OFF 0

#define HCIC_PARAM_SIZE_SET_AFH_CHANNELS 10

extern void btsnd_hcic_write_pin_type(uint8_t type);    /* Write PIN Type */
extern void btsnd_hcic_write_auto_accept(uint8_t flag); /* Write Auto Accept */
extern void btsnd_hcic_read_name(void);                 /* Read Local Name */
extern void btsnd_hcic_write_page_tout(
    uint16_t timeout);                                  /* Write Page Timout */
extern void btsnd_hcic_write_scan_enable(uint8_t flag); /* Write Scan Enable */
extern void btsnd_hcic_write_pagescan_cfg(
    uint16_t interval, uint16_t window); /* Write Page Scan Activity */

#define HCIC_PARAM_SIZE_ENH_SET_ESCO_CONN 59
#define HCIC_PARAM_SIZE_ENH_ACC_ESCO_CONN 63

#define HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG 4

#define HCI_SCAN_CFG_INTERVAL_OFF 0
#define HCI_SCAN_CFG_WINDOW_OFF 2
/* Write Page Scan Activity */

/* Write Inquiry Scan Activity */
extern void btsnd_hcic_write_inqscan_cfg(uint16_t interval, uint16_t window);

#define HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG 4

#define HCI_SCAN_CFG_INTERVAL_OFF 0
#define HCI_SCAN_CFG_WINDOW_OFF 2
/* Write Inquiry Scan Activity */

extern void btsnd_hcic_write_auth_enable(
    uint8_t flag); /* Write Authentication Enable */
extern void btsnd_hcic_write_dev_class(
    DEV_CLASS dev); /* Write Class of Device */
extern void btsnd_hcic_write_voice_settings(
    uint16_t flags); /* Write Voice Settings */

/* Host Controller to Host flow control */
#define HCI_HOST_FLOW_CTRL_OFF 0
#define HCI_HOST_FLOW_CTRL_ACL_ON 1
#define HCI_HOST_FLOW_CTRL_SCO_ON 2
#define HCI_HOST_FLOW_CTRL_BOTH_ON 3

extern void btsnd_hcic_write_auto_flush_tout(
    uint16_t handle, uint16_t timeout); /* Write Retransmit Timout */

#define HCIC_PARAM_SIZE_WRITE_AUTOMATIC_FLUSH_TIMEOUT 4

#define HCI_FLUSH_TOUT_HANDLE_OFF 0
#define HCI_FLUSH_TOUT_TOUT_OFF 2

extern void btsnd_hcic_read_tx_power(uint16_t handle,
                                     uint8_t type); /* Read Tx Power */

#define HCIC_PARAM_SIZE_READ_TX_POWER 3

#define HCI_READ_TX_POWER_HANDLE_OFF 0
#define HCI_READ_TX_POWER_TYPE_OFF 2

/* Read transmit power level parameter */
#define HCI_READ_CURRENT 0x00
#define HCI_READ_MAXIMUM 0x01

extern void btsnd_hcic_host_num_xmitted_pkts(
    uint8_t num_handles, uint16_t* handle,
    uint16_t* num_pkts); /* Set Host Buffer Size */

#define HCIC_PARAM_SIZE_NUM_PKTS_DONE_SIZE sizeof(btmsg_hcic_num_pkts_done_t)

#define MAX_DATA_HANDLES 10

#define HCI_PKTS_DONE_NUM_HANDLES_OFF 0
#define HCI_PKTS_DONE_HANDLE_OFF 1
#define HCI_PKTS_DONE_NUM_PKTS_OFF 3

/* Write Link Supervision Timeout */
extern void btsnd_hcic_write_link_super_tout(uint8_t local_controller_id,
                                             uint16_t handle, uint16_t timeout);

#define HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT 4

#define HCI_LINK_SUPER_TOUT_HANDLE_OFF 0
#define HCI_LINK_SUPER_TOUT_TOUT_OFF 2
/* Write Link Supervision Timeout */

extern void btsnd_hcic_write_cur_iac_lap(
    uint8_t num_cur_iac, LAP* const iac_lap); /* Write Current IAC LAP */

#define MAX_IAC_LAPS 0x40

#define HCI_WRITE_IAC_LAP_NUM_OFF 0
#define HCI_WRITE_IAC_LAP_LAP_OFF 1
/* Write Current IAC LAP */

extern void btsnd_hcic_get_link_quality(uint16_t handle); /* Get Link Quality */
extern void btsnd_hcic_read_rssi(uint16_t handle);        /* Read RSSI */
using ReadEncKeySizeCb = base::OnceCallback<void(uint8_t, uint16_t, uint8_t)>;
extern void btsnd_hcic_read_encryption_key_size(uint16_t handle, ReadEncKeySizeCb cb);
extern void btsnd_hcic_read_failed_contact_counter(uint16_t handle);
extern void btsnd_hcic_read_automatic_flush_timeout(uint16_t handle);
extern void btsnd_hcic_enable_test_mode(
    void); /* Enable Device Under Test Mode */
extern void btsnd_hcic_write_pagescan_type(
    uint8_t type); /* Write Page Scan Type */
extern void btsnd_hcic_write_inqscan_type(
    uint8_t type); /* Write Inquiry Scan Type */
extern void btsnd_hcic_write_inquiry_mode(
    uint8_t type); /* Write Inquiry Mode */

/* Enhanced setup SCO connection (CSA2) */
extern void btsnd_hcic_enhanced_set_up_synchronous_connection(
    uint16_t conn_handle, enh_esco_params_t* p_parms);

/* Enhanced accept SCO connection request (CSA2) */
extern void btsnd_hcic_enhanced_accept_synchronous_connection(
    const RawAddress& bd_addr, enh_esco_params_t* p_parms);

#define HCI_DATA_HANDLE_MASK 0x0FFF

#define HCID_GET_HANDLE_EVENT(p)                     \
  (uint16_t)((*((uint8_t*)((p) + 1) + (p)->offset) + \
              (*((uint8_t*)((p) + 1) + (p)->offset + 1) << 8)))

#define HCID_GET_HANDLE(u16) (uint16_t)((u16)&HCI_DATA_HANDLE_MASK)

#define HCI_DATA_EVENT_MASK 3
#define HCI_DATA_EVENT_OFFSET 12
#define HCID_GET_EVENT(u16) \
  (uint8_t)(((u16) >> HCI_DATA_EVENT_OFFSET) & HCI_DATA_EVENT_MASK)

#define HCI_DATA_BCAST_MASK 3
#define HCI_DATA_BCAST_OFFSET 10
#define HCID_GET_BCAST(u16) \
  (uint8_t)(((u16) >> HCI_DATA_BCAST_OFFSET) & HCI_DATA_BCAST_MASK)

#define HCID_GET_ACL_LEN(p)                              \
  (uint16_t)((*((uint8_t*)((p) + 1) + (p)->offset + 2) + \
              (*((uint8_t*)((p) + 1) + (p)->offset + 3) << 8)))

#define HCID_HEADER_SIZE 4

#define HCID_GET_SCO_LEN(p) (*((uint8_t*)((p) + 1) + (p)->offset + 2))

extern void btsnd_hcic_vendor_spec_cmd(void* buffer, uint16_t opcode,
                                       uint8_t len, uint8_t* p_data,
                                       void* p_cmd_cplt_cback);

/*******************************************************************************
 * BLE Commands
 *      Note: "local_controller_id" is for transport, not counted in HCI
 *             message size
 ******************************************************************************/
#define HCIC_BLE_RAND_DI_SIZE 8
#define HCIC_BLE_IRK_SIZE 16

#define HCIC_PARAM_SIZE_SET_USED_FEAT_CMD 8
#define HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD 6
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS 15
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP 31
#define HCIC_PARAM_SIZE_WRITE_ADV_ENABLE 1
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_PARAM 7
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_ENABLE 2
#define HCIC_PARAM_SIZE_BLE_CREATE_LL_CONN 25
#define HCIC_PARAM_SIZE_BLE_CREATE_CONN_CANCEL 0
#define HCIC_PARAM_SIZE_CLEAR_WHITE_LIST 0
#define HCIC_PARAM_SIZE_ADD_WHITE_LIST 7
#define HCIC_PARAM_SIZE_REMOVE_WHITE_LIST 7
#define HCIC_PARAM_SIZE_BLE_UPD_LL_CONN_PARAMS 14
#define HCIC_PARAM_SIZE_SET_HOST_CHNL_CLASS 5
#define HCIC_PARAM_SIZE_READ_CHNL_MAP 2
#define HCIC_PARAM_SIZE_BLE_READ_REMOTE_FEAT 2
#define HCIC_PARAM_SIZE_BLE_ENCRYPT 32
#define HCIC_PARAM_SIZE_WRITE_LE_HOST_SUPPORTED 2

#define HCIC_BLE_RAND_DI_SIZE 8
#define HCIC_BLE_ENCRYT_KEY_SIZE 16
#define HCIC_PARAM_SIZE_BLE_START_ENC \
  (4 + HCIC_BLE_RAND_DI_SIZE + HCIC_BLE_ENCRYT_KEY_SIZE)
#define HCIC_PARAM_SIZE_LTK_REQ_REPLY (2 + HCIC_BLE_ENCRYT_KEY_SIZE)
#define HCIC_PARAM_SIZE_LTK_REQ_NEG_REPLY 2
#define HCIC_BLE_CHNL_MAP_SIZE 5
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA 31

#define HCIC_PARAM_SIZE_BLE_ADD_DEV_RESOLVING_LIST (7 + HCIC_BLE_IRK_SIZE * 2)
#define HCIC_PARAM_SIZE_BLE_RM_DEV_RESOLVING_LIST 7
#define HCIC_PARAM_SIZE_BLE_SET_PRIVACY_MODE 8
#define HCIC_PARAM_SIZE_BLE_CLEAR_RESOLVING_LIST 0
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVING_LIST_SIZE 0
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_PEER 7
#define HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_LOCAL 7
#define HCIC_PARAM_SIZE_BLE_SET_ADDR_RESOLUTION_ENABLE 1
#define HCIC_PARAM_SIZE_BLE_SET_RAND_PRIV_ADDR_TIMOUT 2

#define HCIC_PARAM_SIZE_BLE_READ_PHY 2
#define HCIC_PARAM_SIZE_BLE_SET_DEFAULT_PHY 3
#define HCIC_PARAM_SIZE_BLE_SET_PHY 7
#define HCIC_PARAM_SIZE_BLE_ENH_RX_TEST 3
#define HCIC_PARAM_SIZE_BLE_ENH_TX_TEST 4

#define HCIC_PARAM_SIZE_BLE_SET_DATA_LENGTH 6
#define HCIC_PARAM_SIZE_BLE_WRITE_EXTENDED_SCAN_PARAM 11

/* ULP HCI command */
extern void btsnd_hcic_ble_set_evt_mask(BT_EVENT_MASK event_mask);

extern void btsnd_hcic_ble_read_buffer_size(void);

extern void btsnd_hcic_ble_read_local_spt_feat(void);

extern void btsnd_hcic_ble_set_local_used_feat(uint8_t feat_set[8]);

extern void btsnd_hcic_ble_set_random_addr(const RawAddress& random_addr);

extern void btsnd_hcic_ble_write_adv_params(
    uint16_t adv_int_min, uint16_t adv_int_max, uint8_t adv_type,
    uint8_t addr_type_own, uint8_t addr_type_dir, const RawAddress& direct_bda,
    uint8_t channel_map, uint8_t adv_filter_policy);

extern void btsnd_hcic_ble_read_adv_chnl_tx_power(void);

extern void btsnd_hcic_ble_set_adv_data(uint8_t data_len, uint8_t* p_data);

extern void btsnd_hcic_ble_set_scan_rsp_data(uint8_t data_len,
                                             uint8_t* p_scan_rsp);

extern void btsnd_hcic_ble_set_adv_enable(uint8_t adv_enable);

extern void btsnd_hcic_ble_set_scan_params(uint8_t scan_type, uint16_t scan_int,
                                           uint16_t scan_win, uint8_t addr_type,
                                           uint8_t scan_filter_policy);

extern void btsnd_hcic_ble_set_scan_enable(uint8_t scan_enable,
                                           uint8_t duplicate);

extern void btsnd_hcic_ble_create_ll_conn(
    uint16_t scan_int, uint16_t scan_win, uint8_t init_filter_policy,
    uint8_t addr_type_peer, const RawAddress& bda_peer, uint8_t addr_type_own,
    uint16_t conn_int_min, uint16_t conn_int_max, uint16_t conn_latency,
    uint16_t conn_timeout, uint16_t min_ce_len, uint16_t max_ce_len);

extern void btsnd_hcic_ble_create_conn_cancel(void);

extern void btsnd_hcic_ble_read_white_list_size(void);

extern void btsnd_hcic_ble_clear_white_list(
    base::OnceCallback<void(uint8_t*, uint16_t)> cb);

extern void btsnd_hcic_ble_add_white_list(
    uint8_t addr_type, const RawAddress& bda,
    base::OnceCallback<void(uint8_t*, uint16_t)> cb);

extern void btsnd_hcic_ble_remove_from_white_list(
    uint8_t addr_type, const RawAddress& bda,
    base::OnceCallback<void(uint8_t*, uint16_t)> cb);

extern void btsnd_hcic_ble_upd_ll_conn_params(
    uint16_t handle, uint16_t conn_int_min, uint16_t conn_int_max,
    uint16_t conn_latency, uint16_t conn_timeout, uint16_t min_len,
    uint16_t max_len);

extern void btsnd_hcic_ble_set_host_chnl_class(
    uint8_t chnl_map[HCIC_BLE_CHNL_MAP_SIZE]);

extern void btsnd_hcic_ble_read_chnl_map(uint16_t handle);

extern void btsnd_hcic_ble_read_remote_feat(uint16_t handle);

extern void btsnd_hcic_ble_encrypt(uint8_t* key, uint8_t key_len,
                                   uint8_t* plain_text, uint8_t pt_len,
                                   void* p_cmd_cplt_cback);

extern void btsnd_hcic_ble_rand(base::Callback<void(BT_OCTET8)> cb);

extern void btsnd_hcic_ble_start_enc(uint16_t handle,
                                     uint8_t rand[HCIC_BLE_RAND_DI_SIZE],
                                     uint16_t ediv, const Octet16& ltk);

extern void btsnd_hcic_ble_ltk_req_reply(uint16_t handle, const Octet16& ltk);

extern void btsnd_hcic_ble_ltk_req_neg_reply(uint16_t handle);

extern void btsnd_hcic_ble_read_supported_states(void);

extern void btsnd_hcic_ble_write_host_supported(uint8_t le_host_spt,
                                                uint8_t simul_le_host_spt);

extern void btsnd_hcic_ble_read_host_supported(void);

extern void btsnd_hcic_ble_receiver_test(uint8_t rx_freq);

extern void btsnd_hcic_ble_transmitter_test(uint8_t tx_freq,
                                            uint8_t test_data_len,
                                            uint8_t payload);
extern void btsnd_hcic_ble_test_end(void);

#if (BLE_LLT_INCLUDED == TRUE)

#define HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_REPLY 14
extern void btsnd_hcic_ble_rc_param_req_reply(
    uint16_t handle, uint16_t conn_int_min, uint16_t conn_int_max,
    uint16_t conn_latency, uint16_t conn_timeout, uint16_t min_ce_len,
    uint16_t max_ce_len);

#define HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_NEG_REPLY 3
extern void btsnd_hcic_ble_rc_param_req_neg_reply(uint16_t handle,
                                                  uint8_t reason);

#endif /* BLE_LLT_INCLUDED */

extern void btsnd_hcic_ble_set_data_length(uint16_t conn_handle,
                                           uint16_t tx_octets,
                                           uint16_t tx_time);

extern void btsnd_hcic_ble_add_device_resolving_list(uint8_t addr_type_peer,
                                                     const RawAddress& bda_peer,
                                                     const Octet16& irk_peer,
                                                     const Octet16& irk_local);

struct scanning_phy_cfg {
  uint8_t scan_type;
  uint16_t scan_int;
  uint16_t scan_win;
};

extern void btsnd_hcic_ble_set_extended_scan_params(
    uint8_t own_address_type, uint8_t scanning_filter_policy,
    uint8_t scanning_phys, scanning_phy_cfg* phy_cfg);

extern void btsnd_hcic_ble_set_extended_scan_enable(uint8_t enable,
                                                    uint8_t filter_duplicates,
                                                    uint16_t duration,
                                                    uint16_t period);

struct EXT_CONN_PHY_CFG {
  uint16_t scan_int;
  uint16_t scan_win;
  uint16_t conn_int_min;
  uint16_t conn_int_max;
  uint16_t conn_latency;
  uint16_t sup_timeout;
  uint16_t min_ce_len;
  uint16_t max_ce_len;
};

extern void btsnd_hcic_ble_ext_create_conn(uint8_t init_filter_policy,
                                           uint8_t addr_type_own,
                                           uint8_t addr_type_peer,
                                           const RawAddress& bda_peer,
                                           uint8_t initiating_phys,
                                           EXT_CONN_PHY_CFG* phy_cfg);

extern void btsnd_hcic_ble_rm_device_resolving_list(uint8_t addr_type_peer,
                                                    const RawAddress& bda_peer);

extern void btsnd_hcic_ble_set_privacy_mode(uint8_t addr_type_peer,
                                            const RawAddress& bda_peer,
                                            uint8_t privacy_type);

extern void btsnd_hcic_ble_clear_resolving_list(void);

extern void btsnd_hcic_ble_read_resolvable_addr_peer(
    uint8_t addr_type_peer, const RawAddress& bda_peer);

extern void btsnd_hcic_ble_read_resolvable_addr_local(
    uint8_t addr_type_peer, const RawAddress& bda_peer);

extern void btsnd_hcic_ble_set_addr_resolution_enable(
    uint8_t addr_resolution_enable);

extern void btsnd_hcic_ble_set_rand_priv_addr_timeout(uint16_t rpa_timout);

extern void btsnd_hcic_read_authenticated_payload_tout(uint16_t handle);

extern void btsnd_hcic_write_authenticated_payload_tout(uint16_t handle,
                                                        uint16_t timeout);

#define HCIC_PARAM_SIZE_WRITE_AUTHENT_PAYLOAD_TOUT 4

#define HCI__WRITE_AUTHENT_PAYLOAD_TOUT_HANDLE_OFF 0
#define HCI__WRITE_AUTHENT_PAYLOAD_TOUT_TOUT_OFF 2

#endif
