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
 *  this file contains the main Bluetooth Manager (BTM) internal
 *  definitions.
 *
 ******************************************************************************/

#ifndef BTM_BLE_INT_H
#define BTM_BLE_INT_H

#include "bt_common.h"
#include "bt_target.h"
#include "btm_ble_api.h"
#include "btm_ble_int_types.h"
#include "btm_int.h"
#include "btm_int_types.h"
#include "hcidefs.h"
#include "smp_api.h"

extern bool ble_evt_type_is_connectable(uint16_t evt_type);
extern void btm_ble_refresh_raddr_timer_timeout(void* data);
extern void btm_ble_process_adv_pkt(uint8_t len, uint8_t* p);
extern void btm_ble_process_phy_update_pkt(uint8_t len, uint8_t* p);
extern void btm_ble_process_ext_adv_pkt(uint8_t len, uint8_t* p);
extern void btm_ble_proc_scan_rsp_rpt(uint8_t* p);
extern tBTM_STATUS btm_ble_read_remote_name(const RawAddress& remote_bda,
                                            tBTM_CMPL_CB* p_cb);
extern bool btm_ble_cancel_remote_name(const RawAddress& remote_bda);

extern tBTM_STATUS btm_ble_set_discoverability(uint16_t combined_mode);
extern tBTM_STATUS btm_ble_set_connectability(uint16_t combined_mode);
extern void btm_send_hci_scan_enable(uint8_t enable, uint8_t filter_duplicates);
extern void btm_send_hci_set_scan_params(uint8_t scan_type, uint16_t scan_int,
                                         uint16_t scan_win,
                                         uint8_t addr_type_own,
                                         uint8_t scan_filter_policy);
extern tBTM_STATUS btm_ble_start_inquiry(uint8_t mode, uint8_t duration);
extern void btm_ble_stop_scan(void);
extern void btm_clear_all_pending_le_entry(void);

extern void btm_ble_stop_scan();
extern void btm_ble_stop_inquiry(void);
extern void btm_ble_init(void);
extern void btm_ble_connected(const RawAddress& bda, uint16_t handle,
                              uint8_t enc_mode, uint8_t role,
                              tBLE_ADDR_TYPE addr_type, bool addr_matched);
extern void btm_ble_read_remote_features_complete(uint8_t* p);
extern void btm_ble_write_adv_enable_complete(uint8_t* p);
extern void btm_ble_conn_complete(uint8_t* p, uint16_t evt_len, bool enhanced);
extern void btm_read_ble_local_supported_states_complete(uint8_t* p,
                                                         uint16_t evt_len);
extern tBTM_BLE_CONN_ST btm_ble_get_conn_st(void);
extern tBTM_STATUS btm_ble_start_adv(void);
extern tBTM_STATUS btm_ble_stop_adv(void);
extern void btm_le_on_advertising_set_terminated(uint8_t* p, uint16_t length);
extern tBTM_STATUS btm_ble_start_scan(void);
extern void btm_ble_create_ll_conn_complete(uint8_t status);

/* LE security function from btm_sec.cc */
extern void btm_ble_link_sec_check(const RawAddress& bd_addr,
                                   tBTM_LE_AUTH_REQ auth_req,
                                   tBTM_BLE_SEC_REQ_ACT* p_sec_req_act);
extern void btm_ble_ltk_request_reply(const RawAddress& bda, bool use_stk,
                                      const Octet16& stk);
extern uint8_t btm_proc_smp_cback(tSMP_EVT event, const RawAddress& bd_addr,
                                  tSMP_EVT_DATA* p_data);
extern tBTM_STATUS btm_ble_set_encryption(const RawAddress& bd_addr,
                                          tBTM_BLE_SEC_ACT sec_act,
                                          uint8_t link_role);
extern void btm_ble_ltk_request(uint16_t handle, uint8_t rand[8],
                                uint16_t ediv);
extern tBTM_STATUS btm_ble_start_encrypt(const RawAddress& bda, bool use_stk,
                                         Octet16* p_stk);
extern void btm_ble_link_encrypted(const RawAddress& bd_addr,
                                   uint8_t encr_enable);

/* LE device management functions */
extern void btm_ble_reset_id(void);

/* security related functions */
extern void btm_ble_increment_sign_ctr(const RawAddress& bd_addr,
                                       bool is_local);
extern bool btm_get_local_div(const RawAddress& bd_addr, uint16_t* p_div);
extern bool btm_ble_get_enc_key_type(const RawAddress& bd_addr,
                                     uint8_t* p_key_types);

extern void btm_ble_test_command_complete(uint8_t* p);
extern void btm_ble_rand_enc_complete(uint8_t* p, uint16_t op_code,
                                      tBTM_RAND_ENC_CB* p_enc_cplt_cback);

extern void btm_sec_save_le_key(const RawAddress& bd_addr,
                                tBTM_LE_KEY_TYPE key_type,
                                tBTM_LE_KEY_VALUE* p_keys,
                                bool pass_to_application);
extern void btm_ble_update_sec_key_size(const RawAddress& bd_addr,
                                        uint8_t enc_key_size);
extern uint8_t btm_ble_read_sec_key_size(const RawAddress& bd_addr);

/* white list function */
extern void btm_update_scanner_filter_policy(tBTM_BLE_SFP scan_policy);
extern void btm_update_adv_filter_policy(tBTM_BLE_AFP adv_policy);
extern void btm_ble_white_list_init(uint8_t white_list_size);

/* background connection function */
extern bool btm_ble_suspend_bg_conn(void);
extern bool btm_ble_resume_bg_conn(void);
extern bool btm_ble_start_auto_conn();
extern bool btm_ble_stop_auto_conn();
extern void btm_ble_update_mode_operation(uint8_t link_role,
                                          const RawAddress* bda,
                                          uint8_t status);
extern bool btm_execute_wl_dev_operation(void);
extern void btm_ble_update_link_topology_mask(uint8_t role, bool increase);
extern void btm_ble_bgconn_cancel_if_disconnected(const RawAddress& bd_addr);

/* BLE address management */
extern void btm_gen_resolvable_private_addr(
    base::Callback<void(const RawAddress& rpa)> cb);
extern void btm_gen_non_resolvable_private_addr(tBTM_BLE_ADDR_CBACK* p_cback,
                                                void* p);
extern tBTM_SEC_DEV_REC* btm_ble_resolve_random_addr(
    const RawAddress& random_bda);
extern void btm_gen_resolve_paddr_low(const RawAddress& address);

/*  privacy function */
#if (BLE_PRIVACY_SPT == TRUE)
/* BLE address mapping with CS feature */
extern bool btm_identity_addr_to_random_pseudo(RawAddress* bd_addr,
                                               uint8_t* p_addr_type,
                                               bool refresh);
extern bool btm_random_pseudo_to_identity_addr(RawAddress* random_pseudo,
                                               uint8_t* p_identity_addr_type);
extern void btm_ble_refresh_peer_resolvable_private_addr(
    const RawAddress& pseudo_bda, const RawAddress& rra, uint8_t rra_type);
extern void btm_ble_refresh_local_resolvable_private_addr(
    const RawAddress& pseudo_addr, const RawAddress& local_rpa);
extern void btm_ble_read_resolving_list_entry_complete(uint8_t* p,
                                                       uint16_t evt_len);
extern void btm_ble_remove_resolving_list_entry_complete(uint8_t* p,
                                                         uint16_t evt_len);
extern void btm_ble_add_resolving_list_entry_complete(uint8_t* p,
                                                      uint16_t evt_len);
extern void btm_ble_clear_resolving_list_complete(uint8_t* p, uint16_t evt_len);
extern void btm_read_ble_resolving_list_size_complete(uint8_t* p,
                                                      uint16_t evt_len);
extern void btm_ble_enable_resolving_list(uint8_t);
extern bool btm_ble_disable_resolving_list(uint8_t rl_mask, bool to_resume);
extern void btm_ble_enable_resolving_list_for_platform(uint8_t rl_mask);
extern void btm_ble_resolving_list_init(uint8_t max_irk_list_sz);
extern void btm_ble_resolving_list_cleanup(void);
#endif

extern void btm_ble_adv_init(void);
extern void* btm_ble_multi_adv_get_ref(uint8_t inst_id);
extern void btm_ble_multi_adv_cleanup(void);
extern void btm_ble_batchscan_init(void);
extern void btm_ble_batchscan_cleanup(void);
extern void btm_ble_adv_filter_init(void);
extern void btm_ble_adv_filter_cleanup(void);
extern bool btm_ble_topology_check(tBTM_BLE_STATE_MASK request);
extern bool btm_ble_clear_topology_mask(tBTM_BLE_STATE_MASK request_state);
extern bool btm_ble_set_topology_mask(tBTM_BLE_STATE_MASK request_state);
extern void btm_ble_set_random_address(const RawAddress& random_bda);

#if (BTM_BLE_CONFORMANCE_TESTING == TRUE)
extern void btm_ble_set_no_disc_if_pair_fail(bool disble_disc);
extern void btm_ble_set_test_mac_value(bool enable, uint8_t* p_test_mac_val);
extern void btm_ble_set_test_local_sign_cntr_value(
    bool enable, uint32_t test_local_sign_cntr);
extern void btm_ble_set_keep_rfu_in_auth_req(bool keep_rfu);
#endif

#endif
