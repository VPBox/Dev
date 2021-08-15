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
 *  This file contains internally used SMP definitions
 *
 ******************************************************************************/
#ifndef SMP_INT_H
#define SMP_INT_H

#include "btm_api.h"
#include "btm_ble_api.h"
#include "btu.h"
#include "smp_api.h"
#include "stack/crypto_toolbox/crypto_toolbox.h"

/* Legacy mode */
#define SMP_MODEL_ENCRYPTION_ONLY 0 /* Just Works model */
#define SMP_MODEL_PASSKEY 1         /* Passkey Entry model, input the key */
#define SMP_MODEL_OOB 2             /* OOB model */
#define SMP_MODEL_KEY_NOTIF 3       /* Passkey Entry model, display the key */
/* Secure connections mode */
#define SMP_MODEL_SEC_CONN_JUSTWORKS 4    /* Just Works model */
#define SMP_MODEL_SEC_CONN_NUM_COMP 5     /* Numeric Comparison model */
#define SMP_MODEL_SEC_CONN_PASSKEY_ENT 6  /* Passkey Entry model, */
                                          /* this side inputs the key */
#define SMP_MODEL_SEC_CONN_PASSKEY_DISP 7 /* Passkey Entry model, */
                                          /* this side displays the key */
#define SMP_MODEL_SEC_CONN_OOB 8 /* Secure Connections mode, OOB model */
#define SMP_MODEL_OUT_OF_RANGE 9
typedef uint8_t tSMP_ASSO_MODEL;

#ifndef SMP_MAX_CONN
#define SMP_MAX_CONN 2
#endif

#define SMP_WAIT_FOR_RSP_TIMEOUT_MS (30 * 1000)
#define SMP_DELAYED_AUTH_TIMEOUT_MS 500

#define SMP_OPCODE_INIT 0x04

/* SMP events */
#define SMP_PAIRING_REQ_EVT SMP_OPCODE_PAIRING_REQ
#define SMP_PAIRING_RSP_EVT SMP_OPCODE_PAIRING_RSP
#define SMP_CONFIRM_EVT SMP_OPCODE_CONFIRM
#define SMP_RAND_EVT SMP_OPCODE_RAND
#define SMP_PAIRING_FAILED_EVT SMP_OPCODE_PAIRING_FAILED
#define SMP_ENCRPTION_INFO_EVT SMP_OPCODE_ENCRYPT_INFO
#define SMP_MASTER_ID_EVT SMP_OPCODE_MASTER_ID
#define SMP_ID_INFO_EVT SMP_OPCODE_IDENTITY_INFO
#define SMP_ID_ADDR_EVT SMP_OPCODE_ID_ADDR
#define SMP_SIGN_INFO_EVT SMP_OPCODE_SIGN_INFO
#define SMP_SECURITY_REQ_EVT SMP_OPCODE_SEC_REQ

#define SMP_PAIR_PUBLIC_KEY_EVT SMP_OPCODE_PAIR_PUBLIC_KEY
#define SMP_PAIR_KEYPRESS_NOTIFICATION_EVT SMP_OPCODE_PAIR_KEYPR_NOTIF

#define SMP_PAIR_COMMITM_EVT SMP_OPCODE_PAIR_COMMITM

#define SMP_SELF_DEF_EVT (SMP_PAIR_COMMITM_EVT + 1)
#define SMP_KEY_READY_EVT (SMP_SELF_DEF_EVT)
#define SMP_ENCRYPTED_EVT (SMP_SELF_DEF_EVT + 1)
#define SMP_L2CAP_CONN_EVT (SMP_SELF_DEF_EVT + 2)
#define SMP_L2CAP_DISCONN_EVT (SMP_SELF_DEF_EVT + 3)
#define SMP_IO_RSP_EVT (SMP_SELF_DEF_EVT + 4)
#define SMP_API_SEC_GRANT_EVT (SMP_SELF_DEF_EVT + 5)
#define SMP_TK_REQ_EVT (SMP_SELF_DEF_EVT + 6)
#define SMP_AUTH_CMPL_EVT (SMP_SELF_DEF_EVT + 7)
#define SMP_ENC_REQ_EVT (SMP_SELF_DEF_EVT + 8)
#define SMP_BOND_REQ_EVT (SMP_SELF_DEF_EVT + 9)
#define SMP_DISCARD_SEC_REQ_EVT (SMP_SELF_DEF_EVT + 10)

#define SMP_PAIR_DHKEY_CHCK_EVT SMP_OPCODE_PAIR_DHKEY_CHECK

/* request to start public  key exchange */
#define SMP_PUBL_KEY_EXCH_REQ_EVT (SMP_SELF_DEF_EVT + 11)

/* local public key created */
#define SMP_LOC_PUBL_KEY_CRTD_EVT (SMP_SELF_DEF_EVT + 12)

/* both local and peer public keys are saved in cb */
#define SMP_BOTH_PUBL_KEYS_RCVD_EVT (SMP_SELF_DEF_EVT + 13)

/* DHKey computation is completed, time to start SC phase1 */
#define SMP_SC_DHKEY_CMPLT_EVT (SMP_SELF_DEF_EVT + 14)

/* new local nonce is generated and saved in p_cb->rand */
#define SMP_HAVE_LOC_NONCE_EVT (SMP_SELF_DEF_EVT + 15)

/* time to start SC phase2 */
#define SMP_SC_PHASE1_CMPLT_EVT (SMP_SELF_DEF_EVT + 16)

/* request to calculate number for user check. Used only in the numeric compare
 * protocol */
#define SMP_SC_CALC_NC_EVT (SMP_SELF_DEF_EVT + 17)

/* Request to display the number for user check to the user.*/
/* Used only in the numeric compare protocol */
#define SMP_SC_DSPL_NC_EVT (SMP_SELF_DEF_EVT + 18)

/* user confirms 'OK' numeric comparison request */
#define SMP_SC_NC_OK_EVT (SMP_SELF_DEF_EVT + 19)

/* both local and peer DHKey Checks are already present - it is used on slave to
 * prevent a race condition */
#define SMP_SC_2_DHCK_CHKS_PRES_EVT (SMP_SELF_DEF_EVT + 20)

/* same meaning as SMP_KEY_READY_EVT to separate between SC and legacy actions
 */
#define SMP_SC_KEY_READY_EVT (SMP_SELF_DEF_EVT + 21)
#define SMP_KEYPRESS_NOTIFICATION_EVENT (SMP_SELF_DEF_EVT + 22)

/* SC OOB data from some repository is provided */
#define SMP_SC_OOB_DATA_EVT (SMP_SELF_DEF_EVT + 23)

#define SMP_CR_LOC_SC_OOB_DATA_EVT (SMP_SELF_DEF_EVT + 24)
#define SMP_MAX_EVT SMP_CR_LOC_SC_OOB_DATA_EVT

typedef uint8_t tSMP_EVENT;

/* Assumption it's only using the low 8 bits, if bigger than that, need to
 * expand it to 16 bits */
#define SMP_SEC_KEY_MASK 0x00ff

/* SMP pairing state */
enum {
  SMP_STATE_IDLE,
  SMP_STATE_WAIT_APP_RSP,
  SMP_STATE_SEC_REQ_PENDING,
  SMP_STATE_PAIR_REQ_RSP,
  SMP_STATE_WAIT_CONFIRM,
  SMP_STATE_CONFIRM,
  SMP_STATE_RAND,
  SMP_STATE_PUBLIC_KEY_EXCH,
  SMP_STATE_SEC_CONN_PHS1_START,
  SMP_STATE_WAIT_COMMITMENT,
  SMP_STATE_WAIT_NONCE,
  SMP_STATE_SEC_CONN_PHS2_START,
  SMP_STATE_WAIT_DHK_CHECK,
  SMP_STATE_DHK_CHECK,
  SMP_STATE_ENCRYPTION_PENDING,
  SMP_STATE_BOND_PENDING,
  SMP_STATE_CREATE_LOCAL_SEC_CONN_OOB_DATA,
  SMP_STATE_MAX
};
typedef uint8_t tSMP_STATE;

/* SMP over BR/EDR events */
#define SMP_BR_PAIRING_REQ_EVT SMP_OPCODE_PAIRING_REQ
#define SMP_BR_PAIRING_RSP_EVT SMP_OPCODE_PAIRING_RSP
#define SMP_BR_CONFIRM_EVT SMP_OPCODE_CONFIRM /* not over BR/EDR */
#define SMP_BR_RAND_EVT SMP_OPCODE_RAND       /* not over BR/EDR */
#define SMP_BR_PAIRING_FAILED_EVT SMP_OPCODE_PAIRING_FAILED
#define SMP_BR_ENCRPTION_INFO_EVT                                    \
  SMP_OPCODE_ENCRYPT_INFO                         /* not over BR/EDR \
                                                     */
#define SMP_BR_MASTER_ID_EVT SMP_OPCODE_MASTER_ID /* not over BR/EDR */
#define SMP_BR_ID_INFO_EVT SMP_OPCODE_IDENTITY_INFO
#define SMP_BR_ID_ADDR_EVT SMP_OPCODE_ID_ADDR
#define SMP_BR_SIGN_INFO_EVT SMP_OPCODE_SIGN_INFO
#define SMP_BR_SECURITY_REQ_EVT SMP_OPCODE_SEC_REQ /* not over BR/EDR */
#define SMP_BR_PAIR_PUBLIC_KEY_EVT \
  SMP_OPCODE_PAIR_PUBLIC_KEY /* not over BR/EDR */
#define SMP_BR_PAIR_DHKEY_CHCK_EVT \
  SMP_OPCODE_PAIR_DHKEY_CHECK /* not over BR/EDR */
#define SMP_BR_PAIR_KEYPR_NOTIF_EVT \
  SMP_OPCODE_PAIR_KEYPR_NOTIF /* not over BR/EDR */
#define SMP_BR_SELF_DEF_EVT SMP_BR_PAIR_KEYPR_NOTIF_EVT
#define SMP_BR_KEY_READY_EVT (SMP_BR_SELF_DEF_EVT + 1)
#define SMP_BR_ENCRYPTED_EVT (SMP_BR_SELF_DEF_EVT + 2)
#define SMP_BR_L2CAP_CONN_EVT (SMP_BR_SELF_DEF_EVT + 3)
#define SMP_BR_L2CAP_DISCONN_EVT (SMP_BR_SELF_DEF_EVT + 4)
#define SMP_BR_KEYS_RSP_EVT (SMP_BR_SELF_DEF_EVT + 5)
#define SMP_BR_API_SEC_GRANT_EVT (SMP_BR_SELF_DEF_EVT + 6)
#define SMP_BR_TK_REQ_EVT (SMP_BR_SELF_DEF_EVT + 7)
#define SMP_BR_AUTH_CMPL_EVT (SMP_BR_SELF_DEF_EVT + 8)
#define SMP_BR_ENC_REQ_EVT (SMP_BR_SELF_DEF_EVT + 9)
#define SMP_BR_BOND_REQ_EVT (SMP_BR_SELF_DEF_EVT + 10)
#define SMP_BR_DISCARD_SEC_REQ_EVT (SMP_BR_SELF_DEF_EVT + 11)
#define SMP_BR_MAX_EVT (SMP_BR_SELF_DEF_EVT + 12)
typedef uint8_t tSMP_BR_EVENT;

/* SMP over BR/EDR pairing states */
enum {
  SMP_BR_STATE_IDLE = SMP_STATE_IDLE,
  SMP_BR_STATE_WAIT_APP_RSP,
  SMP_BR_STATE_PAIR_REQ_RSP,
  SMP_BR_STATE_BOND_PENDING,
  SMP_BR_STATE_MAX
};
typedef uint8_t tSMP_BR_STATE;

enum {
  SMP_KEY_TYPE_TK,
  SMP_KEY_TYPE_CFM,
  SMP_KEY_TYPE_CMP,
  SMP_KEY_TYPE_PEER_DHK_CHCK,
  SMP_KEY_TYPE_STK,
  SMP_KEY_TYPE_LTK
};
typedef struct {
  uint8_t key_type;
  uint8_t* p_data;
} tSMP_KEY;

typedef union {
  uint8_t* p_data; /* uint8_t type data pointer */
  tSMP_KEY key;
  uint8_t status;
  uint16_t reason;
  uint32_t passkey;
  tSMP_OOB_DATA_TYPE req_oob_type;
} tSMP_INT_DATA;

/* internal status mask */
#define SMP_PAIR_FLAGS_WE_STARTED_DD (1)
#define SMP_PAIR_FLAGS_PEER_STARTED_DD (1 << 1)
#define SMP_PAIR_FLAGS_CMD_CONFIRM (1 << SMP_OPCODE_CONFIRM) /* 1 << 3 */
#define SMP_PAIR_FLAG_ENC_AFTER_PAIR (1 << 4)
#define SMP_PAIR_FLAG_HAVE_PEER_DHK_CHK \
  (1 << 5) /* used on slave to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_PEER_PUBL_KEY \
  (1 << 6) /* used on slave to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_PEER_COMM \
  (1 << 7) /* used to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_LOCAL_PUBL_KEY \
  (1 << 8) /* used on slave to resolve race condition */

/* check if authentication requirement need MITM protection */
#define SMP_NO_MITM_REQUIRED(x) (((x)&SMP_AUTH_YN_BIT) == 0)

typedef struct {
  RawAddress bd_addr;
  BT_HDR* p_copy;
} tSMP_REQ_Q_ENTRY;

/* SMP control block */
typedef struct {
  tSMP_CALLBACK* p_callback;
  alarm_t* smp_rsp_timer_ent;
  uint8_t trace_level;
  RawAddress pairing_bda;
  tSMP_STATE state;
  bool derive_lk;
  bool id_addr_rcvd;
  tBLE_ADDR_TYPE id_addr_type;
  RawAddress id_addr;
  bool smp_over_br;
  tSMP_BR_STATE br_state; /* if SMP over BR/ERD has priority over SMP */
  uint8_t failure;
  uint8_t status;
  uint8_t role;
  uint16_t flags;
  uint8_t cb_evt;
  tSMP_SEC_LEVEL sec_level;
  bool connect_initialized;
  Octet16 confirm;
  Octet16 rconfirm;
  Octet16 rrand; /* for SC this is peer nonce */
  Octet16 rand;  /* for SC this is local nonce */
  BT_OCTET32 private_key;
  BT_OCTET32 dhkey;
  Octet16 commitment;
  Octet16 remote_commitment;
  Octet16 local_random; /* local randomizer - passkey or OOB randomizer */
  Octet16 peer_random;  /* peer randomizer - passkey or OOB randomizer */
  Octet16 dhkey_check;
  Octet16 remote_dhkey_check;
  tSMP_PUBLIC_KEY loc_publ_key;
  tSMP_PUBLIC_KEY peer_publ_key;
  tSMP_OOB_DATA_TYPE req_oob_type;
  tSMP_SC_OOB_DATA sc_oob_data;
  tSMP_IO_CAP peer_io_caps;
  tSMP_IO_CAP local_io_capability;
  tSMP_OOB_FLAG peer_oob_flag;
  tSMP_OOB_FLAG loc_oob_flag;
  tSMP_AUTH_REQ peer_auth_req;
  tSMP_AUTH_REQ loc_auth_req;
  bool secure_connections_only_mode_required; /* true if locally SM is required
                                                 to operate */
  /* either in Secure Connections mode or not at all */
  tSMP_ASSO_MODEL selected_association_model;
  bool le_secure_connections_mode_is_used;
  bool key_derivation_h7_used;
  bool le_sc_kp_notif_is_used;
  tSMP_SC_KEY_TYPE local_keypress_notification;
  tSMP_SC_KEY_TYPE peer_keypress_notification;
  uint8_t
      round; /* authentication stage 1 round for passkey association model */
  uint32_t number_to_display;
  Octet16 mac_key;
  uint8_t peer_enc_size;
  uint8_t loc_enc_size;
  uint8_t peer_i_key;
  uint8_t peer_r_key;
  uint8_t local_i_key;
  uint8_t local_r_key;

  Octet16 tk;
  Octet16 ltk;
  uint16_t div;
  Octet16 csrk; /* storage for local CSRK */
  uint16_t ediv;
  BT_OCTET8 enc_rand;
  uint8_t addr_type;
  RawAddress local_bda;
  bool is_pair_cancel;
  bool discard_sec_req;
  uint8_t rcvd_cmd_code;
  uint8_t rcvd_cmd_len;
  uint16_t total_tx_unacked;
  bool wait_for_authorization_complete;
  uint8_t cert_failure; /*failure case for certification */
  alarm_t* delayed_auth_timer_ent;
} tSMP_CB;

/* Server Action functions are of this type */
typedef void (*tSMP_ACT)(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);

extern tSMP_CB smp_cb;

/* Functions provided by att_main.cc */
extern void smp_init(void);

/* smp main */
extern void smp_sm_event(tSMP_CB* p_cb, tSMP_EVENT event,
                         tSMP_INT_DATA* p_data);

extern tSMP_STATE smp_get_state(void);
extern void smp_set_state(tSMP_STATE state);

/* smp_br_main */
extern void smp_br_state_machine_event(tSMP_CB* p_cb, tSMP_BR_EVENT event,
                                       tSMP_INT_DATA* p_data);
extern tSMP_BR_STATE smp_get_br_state(void);
extern void smp_set_br_state(tSMP_BR_STATE state);

/* smp_act.cc */
extern void smp_send_pair_req(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_confirm(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_pair_fail(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_rand(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_pair_public_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_commitment(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_dhkey_check(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_keypress_notification(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_proc_pair_fail(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_confirm(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_rand(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_process_pairing_public_key(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_proc_enc_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_master_id(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_id_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_id_addr(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_sec_grant(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_sec_req(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_sl_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_start_enc(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_enc_cmpl(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_discard(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_pairing_cmpl(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_decide_association_model(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_app_cback(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_compare(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_check_auth_req(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_process_io_response(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_id_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_enc_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_csrk_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_ltk_reply(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_pair_cmd(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_pair_terminate(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_idle_terminate(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_send_pair_rsp(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_key_distribution(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_proc_srk_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_generate_csrk(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_key_pick_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_both_have_public_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_start_secure_connection_phase1(tSMP_CB* p_cb,
                                               tSMP_INT_DATA* p_data);
extern void smp_process_local_nonce(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_process_pairing_commitment(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_process_peer_nonce(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_process_dhkey_check(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_match_dhkey_checks(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_process_keypress_notification(tSMP_CB* p_cb,
                                              tSMP_INT_DATA* p_data);
extern void smp_move_to_secure_connections_phase2(tSMP_CB* p_cb,
                                                  tSMP_INT_DATA* p_data);
extern void smp_phase_2_dhkey_checks_are_present(tSMP_CB* p_cb,
                                                 tSMP_INT_DATA* p_data);
extern void smp_wait_for_both_public_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_start_passkey_verification(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_process_secure_connection_oob_data(tSMP_CB* p_cb,
                                                   tSMP_INT_DATA* p_data);
extern void smp_process_secure_connection_long_term_key(void);
extern void smp_set_local_oob_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_set_local_oob_random_commitment(tSMP_CB* p_cb,
                                                tSMP_INT_DATA* p_data);
extern void smp_set_derive_link_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_derive_link_key_from_long_term_key(tSMP_CB* p_cb,
                                                   tSMP_INT_DATA* p_data);
extern void smp_br_process_pairing_command(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_br_process_security_grant(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_br_process_slave_keys_response(tSMP_CB* p_cb,
                                               tSMP_INT_DATA* p_data);
extern void smp_br_send_pair_response(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_br_check_authorization_request(tSMP_CB* p_cb,
                                               tSMP_INT_DATA* p_data);
extern void smp_br_select_next_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_br_process_link_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_key_distribution_by_transport(tSMP_CB* p_cb,
                                              tSMP_INT_DATA* p_data);
extern void smp_br_pairing_complete(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);

/* smp_l2c */
extern void smp_l2cap_if_init(void);
extern void smp_data_ind(const RawAddress& bd_addr, BT_HDR* p_buf);

/* smp_util.cc */
extern void smp_log_metrics(const RawAddress& bd_addr, bool is_outgoing,
                            const uint8_t* p_buf, size_t buf_len);
extern bool smp_send_cmd(uint8_t cmd_code, tSMP_CB* p_cb);
extern void smp_cb_cleanup(tSMP_CB* p_cb);
extern void smp_reset_control_value(tSMP_CB* p_cb);
extern void smp_proc_pairing_cmpl(tSMP_CB* p_cb);
extern void smp_convert_string_to_tk(Octet16* tk, uint32_t passkey);
extern void smp_mask_enc_key(uint8_t loc_enc_size, Octet16* p_data);
extern void smp_rsp_timeout(void* data);
extern void smp_delayed_auth_complete_timeout(void* data);
extern bool smp_command_has_invalid_length(tSMP_CB* p_cb);
extern bool smp_command_has_invalid_parameters(tSMP_CB* p_cb);
extern void smp_reject_unexpected_pairing_command(const RawAddress& bd_addr);
extern tSMP_ASSO_MODEL smp_select_association_model(tSMP_CB* p_cb);
extern void smp_reverse_array(uint8_t* arr, uint8_t len);
extern uint8_t smp_calculate_random_input(uint8_t* random, uint8_t round);
extern void smp_collect_local_io_capabilities(uint8_t* iocap, tSMP_CB* p_cb);
extern void smp_collect_peer_io_capabilities(uint8_t* iocap, tSMP_CB* p_cb);
extern void smp_collect_local_ble_address(uint8_t* le_addr, tSMP_CB* p_cb);
extern void smp_collect_peer_ble_address(uint8_t* le_addr, tSMP_CB* p_cb);
extern bool smp_check_commitment(tSMP_CB* p_cb);
extern void smp_save_secure_connections_long_term_key(tSMP_CB* p_cb);
extern void smp_calculate_f5_mackey_and_long_term_key(tSMP_CB* p_cb);
extern void smp_remove_fixed_channel(tSMP_CB* p_cb);
extern bool smp_request_oob_data(tSMP_CB* p_cb);

/* smp_keys.cc */
extern void smp_generate_srand_mrand_confirm(tSMP_CB* p_cb,
                                             tSMP_INT_DATA* p_data);
extern void smp_generate_compare(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_generate_stk(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_generate_ltk(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_generate_passkey(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_generate_rand_cont(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_create_private_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_use_oob_private_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_compute_dhkey(tSMP_CB* p_cb);
extern void smp_calculate_local_commitment(tSMP_CB* p_cb);
extern Octet16 smp_calculate_peer_commitment(tSMP_CB* p_cb);
extern void smp_calculate_numeric_comparison_display_number(
    tSMP_CB* p_cb, tSMP_INT_DATA* p_data);
extern void smp_calculate_local_dhkey_check(tSMP_CB* p_cb,
                                            tSMP_INT_DATA* p_data);
extern void smp_calculate_peer_dhkey_check(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data);
extern void smp_start_nonce_generation(tSMP_CB* p_cb);
extern bool smp_calculate_link_key_from_long_term_key(tSMP_CB* p_cb);
extern bool smp_calculate_long_term_key_from_link_key(tSMP_CB* p_cb);

#if (SMP_DEBUG == TRUE)
extern void smp_debug_print_nbyte_little_endian(uint8_t* p,
                                                const char* key_name,
                                                uint8_t len);

inline void smp_debug_print_nbyte_little_endian(const Octet16& p,
                                                const char* key_name,
                                                uint8_t len) {
  smp_debug_print_nbyte_little_endian(const_cast<uint8_t*>(p.data()), key_name,
                                      len);
}
#endif

extern void print128(const Octet16& x, const uint8_t* key_name);
extern void smp_xor_128(Octet16* a, const Octet16& b);

#endif /* SMP_INT_H */
