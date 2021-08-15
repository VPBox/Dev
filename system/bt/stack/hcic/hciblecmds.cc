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
 *  This file contains function of the HCIC unit to format and send HCI
 *  commands.
 *
 ******************************************************************************/

#include "bt_common.h"
#include "bt_target.h"
#include "btu.h"
#include "hcidefs.h"
#include "hcimsgs.h"

#include <base/bind.h>
#include <stddef.h>
#include <string.h>

void btsnd_hcic_ble_set_local_used_feat(uint8_t feat_set[8]) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_USED_FEAT_CMD;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_LOCAL_SPT_FEAT);
  ARRAY_TO_STREAM(pp, feat_set, HCIC_PARAM_SIZE_SET_USED_FEAT_CMD);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_random_addr(const RawAddress& random_bda) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_RANDOM_ADDR);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD);

  BDADDR_TO_STREAM(pp, random_bda);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_write_adv_params(uint16_t adv_int_min, uint16_t adv_int_max,
                                     uint8_t adv_type, uint8_t addr_type_own,
                                     uint8_t addr_type_dir,
                                     const RawAddress& direct_bda,
                                     uint8_t channel_map,
                                     uint8_t adv_filter_policy) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_ADV_PARAMS);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS);

  UINT16_TO_STREAM(pp, adv_int_min);
  UINT16_TO_STREAM(pp, adv_int_max);
  UINT8_TO_STREAM(pp, adv_type);
  UINT8_TO_STREAM(pp, addr_type_own);
  UINT8_TO_STREAM(pp, addr_type_dir);
  BDADDR_TO_STREAM(pp, direct_bda);
  UINT8_TO_STREAM(pp, channel_map);
  UINT8_TO_STREAM(pp, adv_filter_policy);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}
void btsnd_hcic_ble_read_adv_chnl_tx_power(void) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_READ_ADV_CHNL_TX_POWER);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_adv_data(uint8_t data_len, uint8_t* p_data) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_ADV_DATA);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1);

  memset(pp, 0, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA);

  if (p_data != NULL && data_len > 0) {
    if (data_len > HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA)
      data_len = HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA;

    UINT8_TO_STREAM(pp, data_len);

    ARRAY_TO_STREAM(pp, p_data, data_len);
  }
  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}
void btsnd_hcic_ble_set_scan_rsp_data(uint8_t data_len, uint8_t* p_scan_rsp) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP + 1;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_SCAN_RSP_DATA);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP + 1);

  memset(pp, 0, HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP);

  if (p_scan_rsp != NULL && data_len > 0) {
    if (data_len > HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP)
      data_len = HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP;

    UINT8_TO_STREAM(pp, data_len);

    ARRAY_TO_STREAM(pp, p_scan_rsp, data_len);
  }

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_adv_enable(uint8_t adv_enable) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_ADV_ENABLE;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_ADV_ENABLE);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_ADV_ENABLE);

  UINT8_TO_STREAM(pp, adv_enable);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}
void btsnd_hcic_ble_set_scan_params(uint8_t scan_type, uint16_t scan_int,
                                    uint16_t scan_win, uint8_t addr_type_own,
                                    uint8_t scan_filter_policy) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_WRITE_SCAN_PARAM;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_SCAN_PARAMS);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_WRITE_SCAN_PARAM);

  UINT8_TO_STREAM(pp, scan_type);
  UINT16_TO_STREAM(pp, scan_int);
  UINT16_TO_STREAM(pp, scan_win);
  UINT8_TO_STREAM(pp, addr_type_own);
  UINT8_TO_STREAM(pp, scan_filter_policy);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_scan_enable(uint8_t scan_enable, uint8_t duplicate) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_WRITE_SCAN_ENABLE;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_WRITE_SCAN_ENABLE);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_WRITE_SCAN_ENABLE);

  UINT8_TO_STREAM(pp, scan_enable);
  UINT8_TO_STREAM(pp, duplicate);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

/* link layer connection management commands */
void btsnd_hcic_ble_create_ll_conn(
    uint16_t scan_int, uint16_t scan_win, uint8_t init_filter_policy,
    uint8_t addr_type_peer, const RawAddress& bda_peer, uint8_t addr_type_own,
    uint16_t conn_int_min, uint16_t conn_int_max, uint16_t conn_latency,
    uint16_t conn_timeout, uint16_t min_ce_len, uint16_t max_ce_len) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_CREATE_LL_CONN;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_CREATE_LL_CONN);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_CREATE_LL_CONN);

  UINT16_TO_STREAM(pp, scan_int);
  UINT16_TO_STREAM(pp, scan_win);
  UINT8_TO_STREAM(pp, init_filter_policy);

  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);
  UINT8_TO_STREAM(pp, addr_type_own);

  UINT16_TO_STREAM(pp, conn_int_min);
  UINT16_TO_STREAM(pp, conn_int_max);
  UINT16_TO_STREAM(pp, conn_latency);
  UINT16_TO_STREAM(pp, conn_timeout);

  UINT16_TO_STREAM(pp, min_ce_len);
  UINT16_TO_STREAM(pp, max_ce_len);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_create_conn_cancel(void) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_CREATE_CONN_CANCEL;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_CREATE_CONN_CANCEL);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_CREATE_CONN_CANCEL);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_clear_white_list(
    base::OnceCallback<void(uint8_t*, uint16_t)> cb) {
  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_CLEAR_WHITE_LIST, nullptr, 0,
                            std::move(cb));
}

void btsnd_hcic_ble_add_white_list(
    uint8_t addr_type, const RawAddress& bda,
    base::OnceCallback<void(uint8_t*, uint16_t)> cb) {
  uint8_t param[HCIC_PARAM_SIZE_ADD_WHITE_LIST];
  uint8_t* pp = param;

  UINT8_TO_STREAM(pp, addr_type);
  BDADDR_TO_STREAM(pp, bda);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_ADD_WHITE_LIST, param,
                            HCIC_PARAM_SIZE_ADD_WHITE_LIST, std::move(cb));
}

void btsnd_hcic_ble_remove_from_white_list(
    uint8_t addr_type, const RawAddress& bda,
    base::OnceCallback<void(uint8_t*, uint16_t)> cb) {
  uint8_t param[HCIC_PARAM_SIZE_REMOVE_WHITE_LIST];
  uint8_t* pp = param;

  UINT8_TO_STREAM(pp, addr_type);
  BDADDR_TO_STREAM(pp, bda);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_REMOVE_WHITE_LIST, param,
                            HCIC_PARAM_SIZE_REMOVE_WHITE_LIST, std::move(cb));
}

void btsnd_hcic_ble_upd_ll_conn_params(uint16_t handle, uint16_t conn_int_min,
                                       uint16_t conn_int_max,
                                       uint16_t conn_latency,
                                       uint16_t conn_timeout,
                                       uint16_t min_ce_len,
                                       uint16_t max_ce_len) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_UPD_LL_CONN_PARAMS;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_UPD_LL_CONN_PARAMS);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_UPD_LL_CONN_PARAMS);

  UINT16_TO_STREAM(pp, handle);

  UINT16_TO_STREAM(pp, conn_int_min);
  UINT16_TO_STREAM(pp, conn_int_max);
  UINT16_TO_STREAM(pp, conn_latency);
  UINT16_TO_STREAM(pp, conn_timeout);
  UINT16_TO_STREAM(pp, min_ce_len);
  UINT16_TO_STREAM(pp, max_ce_len);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_host_chnl_class(
    uint8_t chnl_map[HCIC_BLE_CHNL_MAP_SIZE]) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_HOST_CHNL_CLASS;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_SET_HOST_CHNL_CLASS);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_HOST_CHNL_CLASS);

  ARRAY_TO_STREAM(pp, chnl_map, HCIC_BLE_CHNL_MAP_SIZE);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_read_chnl_map(uint16_t handle) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CHNL_MAP;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_READ_CHNL_MAP);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CHNL_MAP);

  UINT16_TO_STREAM(pp, handle);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_read_remote_feat(uint16_t handle) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_READ_REMOTE_FEAT;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_READ_REMOTE_FEAT);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_READ_REMOTE_FEAT);

  UINT16_TO_STREAM(pp, handle);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

/* security management commands */
void btsnd_hcic_ble_encrypt(uint8_t* key, uint8_t key_len, uint8_t* plain_text,
                            uint8_t pt_len, void* p_cmd_cplt_cback) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_ENCRYPT;
  p->offset = sizeof(void*);

  *((void**)pp) =
      p_cmd_cplt_cback; /* Store command complete callback in buffer */
  pp += sizeof(void*);  /* Skip over callback pointer */

  UINT16_TO_STREAM(pp, HCI_BLE_ENCRYPT);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_ENCRYPT);

  memset(pp, 0, HCIC_PARAM_SIZE_BLE_ENCRYPT);

  if (key_len > HCIC_BLE_ENCRYT_KEY_SIZE) key_len = HCIC_BLE_ENCRYT_KEY_SIZE;
  if (pt_len > HCIC_BLE_ENCRYT_KEY_SIZE) pt_len = HCIC_BLE_ENCRYT_KEY_SIZE;

  ARRAY_TO_STREAM(pp, key, key_len);
  pp += (HCIC_BLE_ENCRYT_KEY_SIZE - key_len);
  ARRAY_TO_STREAM(pp, plain_text, pt_len);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_rand(base::Callback<void(BT_OCTET8)> cb) {
  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_RAND, nullptr, 0,
                            base::Bind(
                                [](base::Callback<void(BT_OCTET8)> cb,
                                   uint8_t* param, uint16_t param_len) {
                                  CHECK(param[0] == 0)
                                      << "LE Rand return status must be zero";
                                  cb.Run(param + 1 /* skip status */);
                                },
                                std::move(cb)));
}

void btsnd_hcic_ble_start_enc(uint16_t handle,
                              uint8_t rand[HCIC_BLE_RAND_DI_SIZE],
                              uint16_t ediv, const Octet16& ltk) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_START_ENC;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_START_ENC);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_START_ENC);

  UINT16_TO_STREAM(pp, handle);
  ARRAY_TO_STREAM(pp, rand, HCIC_BLE_RAND_DI_SIZE);
  UINT16_TO_STREAM(pp, ediv);
  ARRAY_TO_STREAM(pp, ltk.data(), HCIC_BLE_ENCRYT_KEY_SIZE);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_ltk_req_reply(uint16_t handle, const Octet16& ltk) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_LTK_REQ_REPLY;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_LTK_REQ_REPLY);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_LTK_REQ_REPLY);

  UINT16_TO_STREAM(pp, handle);
  ARRAY_TO_STREAM(pp, ltk.data(), HCIC_BLE_ENCRYT_KEY_SIZE);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_ltk_req_neg_reply(uint16_t handle) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_LTK_REQ_NEG_REPLY;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_LTK_REQ_NEG_REPLY);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_LTK_REQ_NEG_REPLY);

  UINT16_TO_STREAM(pp, handle);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_receiver_test(uint8_t rx_freq) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_RECEIVER_TEST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

  UINT8_TO_STREAM(pp, rx_freq);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_transmitter_test(uint8_t tx_freq, uint8_t test_data_len,
                                     uint8_t payload) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM3;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_TRANSMITTER_TEST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM3);

  UINT8_TO_STREAM(pp, tx_freq);
  UINT8_TO_STREAM(pp, test_data_len);
  UINT8_TO_STREAM(pp, payload);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_test_end(void) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_TEST_END);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_read_host_supported(void) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_READ_LE_HOST_SUPPORT);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

#if (BLE_LLT_INCLUDED == TRUE)

void btsnd_hcic_ble_rc_param_req_reply(uint16_t handle, uint16_t conn_int_min,
                                       uint16_t conn_int_max,
                                       uint16_t conn_latency,
                                       uint16_t conn_timeout,
                                       uint16_t min_ce_len,
                                       uint16_t max_ce_len) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_REPLY;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_RC_PARAM_REQ_REPLY);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_REPLY);

  UINT16_TO_STREAM(pp, handle);
  UINT16_TO_STREAM(pp, conn_int_min);
  UINT16_TO_STREAM(pp, conn_int_max);
  UINT16_TO_STREAM(pp, conn_latency);
  UINT16_TO_STREAM(pp, conn_timeout);
  UINT16_TO_STREAM(pp, min_ce_len);
  UINT16_TO_STREAM(pp, max_ce_len);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_rc_param_req_neg_reply(uint16_t handle, uint8_t reason) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_NEG_REPLY;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_RC_PARAM_REQ_NEG_REPLY);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_RC_PARAM_REQ_NEG_REPLY);

  UINT16_TO_STREAM(pp, handle);
  UINT8_TO_STREAM(pp, reason);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}
#endif

void btsnd_hcic_ble_add_device_resolving_list(uint8_t addr_type_peer,
                                              const RawAddress& bda_peer,
                                              const Octet16& irk_peer,
                                              const Octet16& irk_local) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_ADD_DEV_RESOLVING_LIST;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_ADD_DEV_RESOLVING_LIST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_ADD_DEV_RESOLVING_LIST);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);
  ARRAY_TO_STREAM(pp, irk_peer.data(), HCIC_BLE_ENCRYT_KEY_SIZE);
  ARRAY_TO_STREAM(pp, irk_local.data(), HCIC_BLE_ENCRYT_KEY_SIZE);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_rm_device_resolving_list(uint8_t addr_type_peer,
                                             const RawAddress& bda_peer) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_RM_DEV_RESOLVING_LIST;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_RM_DEV_RESOLVING_LIST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_RM_DEV_RESOLVING_LIST);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_privacy_mode(uint8_t addr_type_peer,
                                     const RawAddress& bda_peer,
                                     uint8_t privacy_type) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_SET_PRIVACY_MODE;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_SET_PRIVACY_MODE);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_SET_PRIVACY_MODE);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);
  UINT8_TO_STREAM(pp, privacy_type);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_clear_resolving_list(void) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_CLEAR_RESOLVING_LIST;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_CLEAR_RESOLVING_LIST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_CLEAR_RESOLVING_LIST);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_read_resolvable_addr_peer(uint8_t addr_type_peer,
                                              const RawAddress& bda_peer) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_PEER;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_READ_RESOLVABLE_ADDR_PEER);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_PEER);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_read_resolvable_addr_local(uint8_t addr_type_peer,
                                               const RawAddress& bda_peer) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_LOCAL;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_READ_RESOLVABLE_ADDR_LOCAL);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_READ_RESOLVABLE_ADDR_LOCAL);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_addr_resolution_enable(uint8_t addr_resolution_enable) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_SET_ADDR_RESOLUTION_ENABLE;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_SET_ADDR_RESOLUTION_ENABLE);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_SET_ADDR_RESOLUTION_ENABLE);
  UINT8_TO_STREAM(pp, addr_resolution_enable);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_rand_priv_addr_timeout(uint16_t rpa_timout) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_SET_RAND_PRIV_ADDR_TIMOUT;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_SET_RAND_PRIV_ADDR_TIMOUT);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_SET_RAND_PRIV_ADDR_TIMOUT);
  UINT16_TO_STREAM(pp, rpa_timout);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_data_length(uint16_t conn_handle, uint16_t tx_octets,
                                    uint16_t tx_time) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_SET_DATA_LENGTH;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_SET_DATA_LENGTH);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_SET_DATA_LENGTH);

  UINT16_TO_STREAM(pp, conn_handle);
  UINT16_TO_STREAM(pp, tx_octets);
  UINT16_TO_STREAM(pp, tx_time);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_enh_rx_test(uint8_t rx_chan, uint8_t phy,
                                uint8_t mod_index) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_ENH_RX_TEST;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_ENH_RECEIVER_TEST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_ENH_RX_TEST);

  UINT8_TO_STREAM(pp, rx_chan);
  UINT8_TO_STREAM(pp, phy);
  UINT8_TO_STREAM(pp, mod_index);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_enh_tx_test(uint8_t tx_chan, uint8_t data_len,
                                uint8_t payload, uint8_t phy) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_BLE_ENH_TX_TEST;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_BLE_ENH_TRANSMITTER_TEST);
  UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_BLE_ENH_TX_TEST);
  UINT8_TO_STREAM(pp, tx_chan);
  UINT8_TO_STREAM(pp, data_len);
  UINT8_TO_STREAM(pp, payload);
  UINT8_TO_STREAM(pp, phy);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_extended_scan_params(uint8_t own_address_type,
                                             uint8_t scanning_filter_policy,
                                             uint8_t scanning_phys,
                                             scanning_phy_cfg* phy_cfg) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  int phy_cnt =
      std::bitset<std::numeric_limits<uint8_t>::digits>(scanning_phys).count();

  uint16_t param_len = 3 + (5 * phy_cnt);
  p->len = HCIC_PREAMBLE_SIZE + param_len;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_LE_SET_EXTENDED_SCAN_PARAMETERS);
  UINT8_TO_STREAM(pp, param_len);

  UINT8_TO_STREAM(pp, own_address_type);
  UINT8_TO_STREAM(pp, scanning_filter_policy);
  UINT8_TO_STREAM(pp, scanning_phys);

  for (int i = 0; i < phy_cnt; i++) {
    UINT8_TO_STREAM(pp, phy_cfg[i].scan_type);
    UINT16_TO_STREAM(pp, phy_cfg[i].scan_int);
    UINT16_TO_STREAM(pp, phy_cfg[i].scan_win);
  }

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_set_extended_scan_enable(uint8_t enable,
                                             uint8_t filter_duplicates,
                                             uint16_t duration,
                                             uint16_t period) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  const int param_len = 6;
  p->len = HCIC_PREAMBLE_SIZE + param_len;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_LE_SET_EXTENDED_SCAN_ENABLE);
  UINT8_TO_STREAM(pp, param_len);

  UINT8_TO_STREAM(pp, enable);
  UINT8_TO_STREAM(pp, filter_duplicates);
  UINT16_TO_STREAM(pp, duration);
  UINT16_TO_STREAM(pp, period);

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}

void btsnd_hcic_ble_ext_create_conn(uint8_t init_filter_policy,
                                    uint8_t addr_type_own,
                                    uint8_t addr_type_peer,
                                    const RawAddress& bda_peer,
                                    uint8_t initiating_phys,
                                    EXT_CONN_PHY_CFG* phy_cfg) {
  BT_HDR* p = (BT_HDR*)osi_malloc(HCI_CMD_BUF_SIZE);
  uint8_t* pp = (uint8_t*)(p + 1);

  int phy_cnt =
      std::bitset<std::numeric_limits<uint8_t>::digits>(initiating_phys)
          .count();

  /* param_len = initial_params + size_per_channel * num_of_channels */
  uint8_t param_len = 10 + (16 * phy_cnt);

  p->len = HCIC_PREAMBLE_SIZE + param_len;
  p->offset = 0;

  UINT16_TO_STREAM(pp, HCI_LE_EXTENDED_CREATE_CONNECTION);
  UINT8_TO_STREAM(pp, param_len);

  UINT8_TO_STREAM(pp, init_filter_policy);
  UINT8_TO_STREAM(pp, addr_type_own);
  UINT8_TO_STREAM(pp, addr_type_peer);
  BDADDR_TO_STREAM(pp, bda_peer);

  UINT8_TO_STREAM(pp, initiating_phys);

  for (int i = 0; i < phy_cnt; i++) {
    UINT16_TO_STREAM(pp, phy_cfg[i].scan_int);
    UINT16_TO_STREAM(pp, phy_cfg[i].scan_win);
    UINT16_TO_STREAM(pp, phy_cfg[i].conn_int_min);
    UINT16_TO_STREAM(pp, phy_cfg[i].conn_int_max);
    UINT16_TO_STREAM(pp, phy_cfg[i].conn_latency);
    UINT16_TO_STREAM(pp, phy_cfg[i].sup_timeout);
    UINT16_TO_STREAM(pp, phy_cfg[i].min_ce_len);
    UINT16_TO_STREAM(pp, phy_cfg[i].max_ce_len);
  }

  btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
}
