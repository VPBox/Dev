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
#ifndef BTM_INT_H
#define BTM_INT_H

#include "bt_common.h"
#include "bt_target.h"
#include "hcidefs.h"

#include "osi/include/alarm.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/list.h"
#include "rfcdefs.h"

#include "btm_api.h"
#include "device/include/esco_parameters.h"

#include "btm_ble_int.h"
#include "btm_int_types.h"
#include "l2cdefs.h"
#include "smp_api.h"

extern tBTM_CB btm_cb;

/* Internal functions provided by btm_main.cc
 *******************************************
*/
extern void btm_init(void);
extern void btm_free(void);

/* Internal functions provided by btm_inq.cc
 ******************************************
*/
extern tBTM_STATUS btm_initiate_rem_name(const RawAddress& remote_bda,
                                         uint8_t origin, uint64_t timeout_ms,
                                         tBTM_CMPL_CB* p_cb);

extern void btm_process_remote_name(const RawAddress* bda, BD_NAME name,
                                    uint16_t evt_len, uint8_t hci_status);
extern void btm_inq_rmt_name_failed(void);
extern void btm_inq_remote_name_timer_timeout(void* data);

/* Inquiry related functions */
extern void btm_clr_inq_db(const RawAddress* p_bda);
extern void btm_inq_db_init(void);
extern void btm_process_inq_results(uint8_t* p, uint8_t hci_evt_len,
                                    uint8_t inq_res_mode);
extern void btm_process_inq_complete(uint8_t status, uint8_t mode);
extern void btm_process_cancel_complete(uint8_t status, uint8_t mode);
extern void btm_event_filter_complete(uint8_t* p);
extern void btm_inq_stop_on_ssp(void);
extern void btm_inq_clear_ssp(void);
extern tINQ_DB_ENT* btm_inq_db_find(const RawAddress& p_bda);
extern bool btm_inq_find_bdaddr(const RawAddress& p_bda);

/* Internal functions provided by btm_acl.cc
 *******************************************
*/
extern void btm_acl_init(void);
extern void btm_acl_created(const RawAddress& bda, DEV_CLASS dc, BD_NAME bdn,
                            uint16_t hci_handle, uint8_t link_role,
                            tBT_TRANSPORT transport);
extern void btm_acl_removed(const RawAddress& bda, tBT_TRANSPORT transport);
extern void btm_acl_device_down(void);
extern void btm_acl_update_busy_level(tBTM_BLI_EVENT event);

extern void btm_cont_rswitch(tACL_CONN* p, tBTM_SEC_DEV_REC* p_dev_rec,
                             uint8_t hci_status);

extern uint8_t btm_handle_to_acl_index(uint16_t hci_handle);
extern void btm_read_link_policy_complete(uint8_t* p);

extern void btm_read_rssi_timeout(void* data);
extern void btm_read_rssi_complete(uint8_t* p);

extern void btm_read_failed_contact_counter_timeout(void* data);
extern void btm_read_failed_contact_counter_complete(uint8_t* p);

extern void btm_read_automatic_flush_timeout_timeout(void* data);
extern void btm_read_automatic_flush_timeout_complete(uint8_t* p);

extern void btm_read_tx_power_timeout(void* data);
extern void btm_read_tx_power_complete(uint8_t* p, bool is_ble);

extern void btm_read_link_quality_timeout(void* data);
extern void btm_read_link_quality_complete(uint8_t* p);

extern tBTM_STATUS btm_set_packet_types(tACL_CONN* p, uint16_t pkt_types);
extern void btm_process_clk_off_comp_evt(uint16_t hci_handle,
                                         uint16_t clock_offset);
extern void btm_acl_role_changed(uint8_t hci_status, const RawAddress* bd_addr,
                                 uint8_t new_role);
extern void btm_blacklist_role_change_device(const RawAddress& bd_addr,
                                             uint8_t hci_status);
extern void btm_acl_encrypt_change(uint16_t handle, uint8_t status,
                                   uint8_t encr_enable);
extern uint16_t btm_get_acl_disc_reason_code(void);
extern tBTM_STATUS btm_remove_acl(const RawAddress& bd_addr,
                                  tBT_TRANSPORT transport);
extern void btm_read_remote_features_complete(uint8_t* p);
extern void btm_read_remote_ext_features_complete(uint8_t* p, uint8_t evt_len);
extern void btm_read_remote_ext_features_failed(uint8_t status,
                                                uint16_t handle);
extern void btm_read_remote_version_complete(uint8_t* p);
extern void btm_establish_continue(tACL_CONN* p_acl_cb);

extern void btm_acl_chk_peer_pkt_type_support(tACL_CONN* p,
                                              uint16_t* p_pkt_type);
/* Read maximum data packet that can be sent over current connection */
extern uint16_t btm_get_max_packet_size(const RawAddress& addr);
extern tACL_CONN* btm_bda_to_acl(const RawAddress& bda,
                                 tBT_TRANSPORT transport);
extern bool btm_acl_notif_conn_collision(const RawAddress& bda);
extern void btm_acl_update_conn_addr(uint16_t conn_handle,
                                     const RawAddress& address);

extern void btm_pm_reset(void);
extern void btm_pm_sm_alloc(uint8_t ind);
extern void btm_pm_proc_cmd_status(uint8_t status);
extern void btm_pm_proc_mode_change(uint8_t hci_status, uint16_t hci_handle,
                                    uint8_t mode, uint16_t interval);
extern void btm_pm_proc_ssr_evt(uint8_t* p, uint16_t evt_len);
extern tBTM_STATUS btm_read_power_mode_state(const RawAddress& remote_bda,
                                             tBTM_PM_STATE* pmState);
extern void btm_sco_chk_pend_unpark(uint8_t hci_status, uint16_t hci_handle);
extern void btm_qos_setup_timeout(void* data);
extern void btm_qos_setup_complete(uint8_t status, uint16_t handle,
                                   FLOW_SPEC* p_flow);

/* Internal functions provided by btm_sco.cc
 *******************************************
*/
extern void btm_sco_init(void);
extern void btm_sco_connected(uint8_t hci_status, const RawAddress* bda,
                              uint16_t hci_handle, tBTM_ESCO_DATA* p_esco_data);
extern void btm_esco_proc_conn_chg(uint8_t status, uint16_t handle,
                                   uint8_t tx_interval, uint8_t retrans_window,
                                   uint16_t rx_pkt_len, uint16_t tx_pkt_len);
extern void btm_sco_conn_req(const RawAddress& bda, DEV_CLASS dev_class,
                             uint8_t link_type);
extern void btm_sco_removed(uint16_t hci_handle, uint8_t reason);
extern void btm_sco_acl_removed(const RawAddress* bda);
extern void btm_route_sco_data(BT_HDR* p_msg);
extern bool btm_is_sco_active(uint16_t handle);
extern void btm_remove_sco_links(const RawAddress& bda);
extern bool btm_is_sco_active_by_bdaddr(const RawAddress& remote_bda);

extern void btm_read_def_esco_mode(enh_esco_params_t* p_parms);
extern uint16_t btm_find_scb_by_handle(uint16_t handle);
extern void btm_sco_flush_sco_data(uint16_t sco_inx);

/* Internal functions provided by btm_devctl.cc
 *********************************************
*/
extern void btm_dev_init(void);
extern void btm_read_local_name_timeout(void* data);
extern void btm_read_local_name_complete(uint8_t* p, uint16_t evt_len);

extern void btm_ble_create_conn_cancel_complete(uint8_t* p);
extern bool btm_ble_addr_resolvable(const RawAddress& rpa,
                                    tBTM_SEC_DEV_REC* p_dev_rec);
extern tBTM_STATUS btm_ble_read_resolving_list_entry(
    tBTM_SEC_DEV_REC* p_dev_rec);
extern bool btm_ble_resolving_list_load_dev(tBTM_SEC_DEV_REC* p_dev_rec);
extern void btm_ble_resolving_list_remove_dev(tBTM_SEC_DEV_REC* p_dev_rec);

/* Vendor Specific Command complete evt handler */
extern void btm_vsc_complete(uint8_t* p, uint16_t cc_opcode, uint16_t evt_len,
                             tBTM_VSC_CMPL_CB* p_vsc_cplt_cback);
extern void btm_inq_db_reset(void);
extern void btm_vendor_specific_evt(uint8_t* p, uint8_t evt_len);
extern void btm_delete_stored_link_key_complete(uint8_t* p);
extern void btm_report_device_status(tBTM_DEV_STATUS status);
extern tBTM_STATUS BTM_BT_Quality_Report_VSE_Register(
    bool is_register, tBTM_BT_QUALITY_REPORT_RECEIVER* p_bqr_report_receiver);

/* Internal functions provided by btm_dev.cc
 *********************************************
*/
extern bool btm_dev_support_switch(const RawAddress& bd_addr);

extern tBTM_SEC_DEV_REC* btm_sec_allocate_dev_rec(void);
extern tBTM_SEC_DEV_REC* btm_sec_alloc_dev(const RawAddress& bd_addr);
extern void wipe_secrets_and_remove(tBTM_SEC_DEV_REC* p_dev_rec);
extern tBTM_SEC_DEV_REC* btm_find_dev(const RawAddress& bd_addr);
extern tBTM_SEC_DEV_REC* btm_find_or_alloc_dev(const RawAddress& bd_addr);
extern tBTM_SEC_DEV_REC* btm_find_dev_by_handle(uint16_t handle);
extern tBTM_BOND_TYPE btm_get_bond_type_dev(const RawAddress& bd_addr);
extern bool btm_set_bond_type_dev(const RawAddress& bd_addr,
                                  tBTM_BOND_TYPE bond_type);

/* Internal functions provided by btm_sec.cc
 *********************************************
*/
extern bool btm_dev_support_switch(const RawAddress& bd_addr);
extern tBTM_STATUS btm_sec_l2cap_access_req(const RawAddress& bd_addr,
                                            uint16_t psm, uint16_t handle,
                                            CONNECTION_TYPE conn_type,
                                            tBTM_SEC_CALLBACK* p_callback,
                                            void* p_ref_data);
extern tBTM_STATUS btm_sec_mx_access_request(const RawAddress& bd_addr,
                                             uint16_t psm, bool is_originator,
                                             uint32_t mx_proto_id,
                                             uint32_t mx_chan_id,
                                             tBTM_SEC_CALLBACK* p_callback,
                                             void* p_ref_data);
extern void btm_sec_conn_req(const RawAddress& bda, uint8_t* dc);
extern void btm_create_conn_cancel_complete(uint8_t* p);

extern void btm_read_inq_tx_power_timeout(void* data);
extern void btm_read_inq_tx_power_complete(uint8_t* p);

extern void btm_sec_init(uint8_t sec_mode);
extern void btm_sec_dev_reset(void);
extern void btm_sec_abort_access_req(const RawAddress& bd_addr);
extern void btm_sec_auth_complete(uint16_t handle, uint8_t status);
extern void btm_sec_encrypt_change(uint16_t handle, uint8_t status,
                                   uint8_t encr_enable);
extern void btm_sec_connected(const RawAddress& bda, uint16_t handle,
                              uint8_t status, uint8_t enc_mode);
extern tBTM_STATUS btm_sec_disconnect(uint16_t handle, uint8_t reason);
extern void btm_sec_disconnected(uint16_t handle, uint8_t reason);
extern void btm_sec_rmt_name_request_complete(const RawAddress* bd_addr,
                                              uint8_t* bd_name, uint8_t status);
extern void btm_sec_rmt_host_support_feat_evt(uint8_t* p);
extern void btm_io_capabilities_req(const RawAddress& p);
extern void btm_io_capabilities_rsp(uint8_t* p);
extern void btm_proc_sp_req_evt(tBTM_SP_EVT event, uint8_t* p);
extern void btm_keypress_notif_evt(uint8_t* p);
extern void btm_simple_pair_complete(uint8_t* p);
extern void btm_sec_link_key_notification(const RawAddress& p_bda,
                                          const Octet16& link_key,
                                          uint8_t key_type);
extern void btm_sec_link_key_request(const RawAddress& p_bda);
extern void btm_sec_pin_code_request(const RawAddress& p_bda);
extern void btm_sec_update_clock_offset(uint16_t handle, uint16_t clock_offset);
extern void btm_sec_dev_rec_cback_event(tBTM_SEC_DEV_REC* p_dev_rec,
                                        uint8_t res, bool is_le_trasnport);
extern void btm_sec_set_peer_sec_caps(tACL_CONN* p_acl_cb,
                                      tBTM_SEC_DEV_REC* p_dev_rec);

extern void btm_sec_clear_ble_keys(tBTM_SEC_DEV_REC* p_dev_rec);
extern bool btm_sec_is_a_bonded_dev(const RawAddress& bda);
extern void btm_consolidate_dev(tBTM_SEC_DEV_REC* p_target_rec);
extern bool btm_sec_is_le_capable_dev(const RawAddress& bda);
extern bool btm_ble_init_pseudo_addr(tBTM_SEC_DEV_REC* p_dev_rec,
                                     const RawAddress& new_pseudo_addr);
extern tBTM_SEC_SERV_REC* btm_sec_find_first_serv(CONNECTION_TYPE conn_type,
                                                  uint16_t psm);
extern tL2CAP_LE_RESULT_CODE btm_ble_start_sec_check(
    const RawAddress& bd_addr, uint16_t psm, bool is_originator,
    tBTM_SEC_CALLBACK* p_callback, void* p_ref_data);

extern tINQ_DB_ENT* btm_inq_db_new(const RawAddress& p_bda);

extern void btm_rem_oob_req(uint8_t* p);
extern void btm_read_local_oob_complete(uint8_t* p);

extern void btm_acl_resubmit_page(void);
extern void btm_acl_reset_paging(void);
extern void btm_acl_paging(BT_HDR* p, const RawAddress& dest);
extern uint8_t btm_sec_clr_service_by_psm(uint16_t psm);
extern void btm_sec_clr_temp_auth_service(const RawAddress& bda);
extern tBTM_STATUS btm_sec_execute_procedure(tBTM_SEC_DEV_REC* p_dev_rec);

#endif
