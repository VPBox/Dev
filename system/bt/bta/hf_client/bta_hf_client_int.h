/******************************************************************************
 *
 *  Copyright (c) 2014 The Android Open Source Project
 *  Copyright 2003-2012 Broadcom Corporation
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

#include "bta_api.h"
#include "bta_hf_client_api.h"
#include "bta_hf_client_at.h"
#include "bta_sys.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/
#define HFP_VERSION_1_1 0x0101
#define HFP_VERSION_1_5 0x0105
#define HFP_VERSION_1_6 0x0106

/* RFCOMM MTU SIZE */
#define BTA_HF_CLIENT_MTU 256

/* profile role for connection */
#define BTA_HF_CLIENT_ACP 0 /* accepted connection */
#define BTA_HF_CLIENT_INT 1 /* initiating connection */

/* Time (in milliseconds) to wait for retry in case of collision */
#ifndef BTA_HF_CLIENT_COLLISION_TIMER_MS
#define BTA_HF_CLIENT_COLLISION_TIMER_MS 2411
#endif

/* Maximum number of HF devices supported simultaneously */
#define HF_CLIENT_MAX_DEVICES 10

enum {
  /* these events are handled by the state machine */
  BTA_HF_CLIENT_API_OPEN_EVT = BTA_SYS_EVT_START(BTA_ID_HS),
  BTA_HF_CLIENT_API_CLOSE_EVT,
  BTA_HF_CLIENT_API_AUDIO_OPEN_EVT,
  BTA_HF_CLIENT_API_AUDIO_CLOSE_EVT,
  BTA_HF_CLIENT_RFC_OPEN_EVT,
  BTA_HF_CLIENT_RFC_CLOSE_EVT,
  BTA_HF_CLIENT_RFC_SRV_CLOSE_EVT,
  BTA_HF_CLIENT_RFC_DATA_EVT,
  BTA_HF_CLIENT_DISC_ACP_RES_EVT,
  BTA_HF_CLIENT_DISC_INT_RES_EVT,
  BTA_HF_CLIENT_DISC_OK_EVT,
  BTA_HF_CLIENT_DISC_FAIL_EVT,
  BTA_HF_CLIENT_SCO_OPEN_EVT,
  BTA_HF_CLIENT_SCO_CLOSE_EVT,
  BTA_HF_CLIENT_SEND_AT_CMD_EVT,
  BTA_HF_CLIENT_MAX_EVT,

  /* these events are handled outside of the state machine */
  BTA_HF_CLIENT_API_ENABLE_EVT,
  BTA_HF_CLIENT_API_DISABLE_EVT
};

/* AT Command enum */
enum {
  BTA_HF_CLIENT_AT_NONE,
  BTA_HF_CLIENT_AT_BRSF,
  BTA_HF_CLIENT_AT_BAC,
  BTA_HF_CLIENT_AT_CIND,
  BTA_HF_CLIENT_AT_CIND_STATUS,
  BTA_HF_CLIENT_AT_CMER,
  BTA_HF_CLIENT_AT_CHLD,
  BTA_HF_CLIENT_AT_CMEE,
  BTA_HF_CLIENT_AT_BIA,
  BTA_HF_CLIENT_AT_CLIP,
  BTA_HF_CLIENT_AT_CCWA,
  BTA_HF_CLIENT_AT_COPS,
  BTA_HF_CLIENT_AT_CLCC,
  BTA_HF_CLIENT_AT_BVRA,
  BTA_HF_CLIENT_AT_VGS,
  BTA_HF_CLIENT_AT_VGM,
  BTA_HF_CLIENT_AT_ATD,
  BTA_HF_CLIENT_AT_BLDN,
  BTA_HF_CLIENT_AT_ATA,
  BTA_HF_CLIENT_AT_CHUP,
  BTA_HF_CLIENT_AT_BTRH,
  BTA_HF_CLIENT_AT_VTS,
  BTA_HF_CLIENT_AT_BCC,
  BTA_HF_CLIENT_AT_BCS,
  BTA_HF_CLIENT_AT_CNUM,
  BTA_HF_CLIENT_AT_NREC,
  BTA_HF_CLIENT_AT_BINP,
};

/*****************************************************************************
 *  Data types
 ****************************************************************************/
/* data type for BTA_HF_CLIENT_API_OPEN_EVT */
typedef struct {
  BT_HDR hdr;
  RawAddress bd_addr;
  uint16_t* handle;
  tBTA_SEC sec_mask;
} tBTA_HF_CLIENT_API_OPEN;

/* data type for BTA_HF_CLIENT_DISC_RESULT_EVT */
typedef struct {
  BT_HDR hdr;
  uint16_t status;
} tBTA_HF_CLIENT_DISC_RESULT;

/* data type for RFCOMM events */
typedef struct {
  BT_HDR hdr;
  uint16_t port_handle;
} tBTA_HF_CLIENT_RFC;

/* generic purpose data type for other events */
typedef struct {
  BT_HDR hdr;
  bool bool_val;
  uint8_t uint8_val;
  uint32_t uint32_val1;
  uint32_t uint32_val2;
  char str[BTA_HF_CLIENT_NUMBER_LEN + 1];
} tBTA_HF_CLIENT_DATA_VAL;

/* union of all event datatypes */
typedef union {
  BT_HDR hdr;
  tBTA_HF_CLIENT_API_OPEN api_open;
  tBTA_HF_CLIENT_DISC_RESULT disc_result;
  tBTA_HF_CLIENT_RFC rfc;
  tBTA_HF_CLIENT_DATA_VAL val;

} tBTA_HF_CLIENT_DATA;

/* First handle for the control block */
#define BTA_HF_CLIENT_CB_FIRST_HANDLE 1

/* sco states */
enum {
  BTA_HF_CLIENT_SCO_SHUTDOWN_ST, /* no listening, no connection */
  BTA_HF_CLIENT_SCO_LISTEN_ST,   /* listening */
  BTA_HF_CLIENT_SCO_OPENING_ST,  /* connection opening */
  BTA_HF_CLIENT_SCO_OPEN_CL_ST,  /* opening connection being closed */
  BTA_HF_CLIENT_SCO_OPEN_ST,     /* open */
  BTA_HF_CLIENT_SCO_CLOSING_ST,  /* closing */
  BTA_HF_CLIENT_SCO_CLOSE_OP_ST, /* closing sco being opened */
  BTA_HF_CLIENT_SCO_SHUTTING_ST  /* sco shutting down */
};

/* type for HF control block */
typedef struct {
  // Fields useful for particular control block.
  uint8_t handle;               /* Handle of the control block to be
                                   used by upper layer */
  RawAddress peer_addr;         /* peer bd address */
  tSDP_DISCOVERY_DB* p_disc_db; /* pointer to discovery database */
  uint16_t conn_handle;         /* RFCOMM handle of connected service */
  tBTA_SEC cli_sec_mask;        /* client security mask */
  tBTA_HF_CLIENT_PEER_FEAT peer_features; /* peer device features */
  tBTA_HF_CLIENT_CHLD_FEAT chld_features; /* call handling features */
  uint16_t peer_version;                  /* profile version of peer device */
  uint8_t peer_scn;                       /* peer scn */
  uint8_t role;                           /* initiator/acceptor role */
  uint16_t sco_idx;                       /* SCO handle */
  uint8_t sco_state;                      /* SCO state variable */
  bool sco_close_rfc; /* true if also close RFCOMM after SCO */
  tBTM_SCO_CODEC_TYPE negotiated_codec; /* negotiated codec */
  bool svc_conn;      /* set to true when service level connection is up */
  bool send_at_reply; /* set to true to notify framework about AT results */
  tBTA_HF_CLIENT_AT_CB at_cb;           /* AT Parser control block */
  uint8_t state;                        /* state machine state */
  bool is_allocated; /* if the control block is already allocated */
  alarm_t* collision_timer;             /* Collision timer */
} tBTA_HF_CLIENT_CB;

typedef struct {
  // Common fields, should be taken out.
  uint32_t sdp_handle;
  uint8_t scn;
  tBTA_HF_CLIENT_CBACK* p_cback; /* application callback */
  tBTA_HF_CLIENT_FEAT features;  /* features registered by application */
  tBTA_SEC serv_sec_mask;        /* server security mask */
  uint16_t serv_handle;          /* RFCOMM server handle */
  bool deregister;               /* true if service shutting down */

  // Maximum number of control blocks supported by the BTA layer.
  tBTA_HF_CLIENT_CB cb[HF_CLIENT_MAX_DEVICES];
} tBTA_HF_CLIENT_CB_ARR;

extern tBTA_HF_CLIENT_CB_ARR bta_hf_client_cb_arr;

/*****************************************************************************
 *  Function prototypes
 ****************************************************************************/

/* main functions */
extern tBTA_HF_CLIENT_CB* bta_hf_client_find_cb_by_handle(uint16_t handle);
extern tBTA_HF_CLIENT_CB* bta_hf_client_find_cb_by_bda(
    const RawAddress& bd_addr);
extern tBTA_HF_CLIENT_CB* bta_hf_client_find_cb_by_rfc_handle(uint16_t handle);
extern tBTA_HF_CLIENT_CB* bta_hf_client_find_cb_by_sco_handle(uint16_t handle);
extern bool bta_hf_client_hdl_event(BT_HDR* p_msg);
extern void bta_hf_client_sm_execute(uint16_t event,
                                     tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_slc_seq(tBTA_HF_CLIENT_CB* client_cb, bool error);
extern bool bta_hf_client_allocate_handle(const RawAddress& bd_addr,
                                          uint16_t* p_handle);
extern void bta_hf_client_app_callback(uint16_t event, tBTA_HF_CLIENT* data);
extern void bta_hf_client_collision_cback(tBTA_SYS_CONN_STATUS status,
                                          uint8_t id, uint8_t app_id,
                                          const RawAddress& peer_addr);
extern void bta_hf_client_resume_open(tBTA_HF_CLIENT_CB* client_cb);
extern tBTA_STATUS bta_hf_client_api_enable(tBTA_HF_CLIENT_CBACK* p_cback,
                                            tBTA_SEC sec_mask,
                                            tBTA_HF_CLIENT_FEAT features,
                                            const char* p_service_name);

extern void bta_hf_client_api_disable(void);
extern void bta_hf_client_dump_statistics(int fd);
extern void bta_hf_client_cb_arr_init(void);

/* SDP functions */
extern bool bta_hf_client_add_record(char* p_service_name, uint8_t scn,
                                     tBTA_HF_CLIENT_FEAT features,
                                     uint32_t sdp_handle);
extern void bta_hf_client_create_record(tBTA_HF_CLIENT_CB_ARR* client_cb,
                                        const char* p_data);
extern void bta_hf_client_del_record(tBTA_HF_CLIENT_CB_ARR* client_cb);
extern bool bta_hf_client_sdp_find_attr(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_do_disc(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_free_db(tBTA_HF_CLIENT_DATA* p_data);

/* RFCOMM functions */
extern void bta_hf_client_setup_port(uint16_t handle);
extern void bta_hf_client_start_server();
extern void bta_hf_client_close_server();
extern void bta_hf_client_rfc_do_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_do_close(tBTA_HF_CLIENT_DATA* p_data);

/* SCO functions */
extern void bta_hf_client_sco_listen(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_sco_conn_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_sco_conn_close(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_sco_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_sco_close(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_sco_shutdown(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_cback_sco(tBTA_HF_CLIENT_CB* client_cb,
                                    uint8_t event);

/* AT command functions */
extern void bta_hf_client_at_parse(tBTA_HF_CLIENT_CB* client_cb, char* buf,
                                   unsigned int len);
extern void bta_hf_client_send_at_brsf(tBTA_HF_CLIENT_CB* client_cb,
                                       tBTA_HF_CLIENT_FEAT features);
extern void bta_hf_client_send_at_bac(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_cind(tBTA_HF_CLIENT_CB* client_cb,
                                       bool status);
extern void bta_hf_client_send_at_cmer(tBTA_HF_CLIENT_CB* client_cb,
                                       bool activate);
extern void bta_hf_client_send_at_chld(tBTA_HF_CLIENT_CB* client_cb, char cmd,
                                       uint32_t idx);
extern void bta_hf_client_send_at_clip(tBTA_HF_CLIENT_CB* client_cb,
                                       bool activate);
extern void bta_hf_client_send_at_ccwa(tBTA_HF_CLIENT_CB* client_cb,
                                       bool activate);
extern void bta_hf_client_send_at_cmee(tBTA_HF_CLIENT_CB* client_cb,
                                       bool activate);
extern void bta_hf_client_send_at_cops(tBTA_HF_CLIENT_CB* client_cb,
                                       bool query);
extern void bta_hf_client_send_at_clcc(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_bvra(tBTA_HF_CLIENT_CB* client_cb,
                                       bool enable);
extern void bta_hf_client_send_at_vgs(tBTA_HF_CLIENT_CB* client_cb,
                                      uint32_t volume);
extern void bta_hf_client_send_at_vgm(tBTA_HF_CLIENT_CB* client_cb,
                                      uint32_t volume);
extern void bta_hf_client_send_at_atd(tBTA_HF_CLIENT_CB* client_cb,
                                      char* number, uint32_t memory);
extern void bta_hf_client_send_at_bldn(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_ata(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_chup(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_btrh(tBTA_HF_CLIENT_CB* client_cb, bool query,
                                       uint32_t val);
extern void bta_hf_client_send_at_vts(tBTA_HF_CLIENT_CB* client_cb, char code);
extern void bta_hf_client_send_at_bcc(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_bcs(tBTA_HF_CLIENT_CB* client_cb,
                                      uint32_t codec);
extern void bta_hf_client_send_at_cnum(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_nrec(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_send_at_binp(tBTA_HF_CLIENT_CB* client_cb,
                                       uint32_t action);
extern void bta_hf_client_send_at_bia(tBTA_HF_CLIENT_CB* client_cb);

/* AT API Functions */
void bta_hf_client_at_init(tBTA_HF_CLIENT_CB* client_cb);
void bta_hf_client_at_reset(tBTA_HF_CLIENT_CB* client_cb);
extern void bta_hf_client_ind(tBTA_HF_CLIENT_CB* client_cb,
                              tBTA_HF_CLIENT_IND_TYPE type, uint16_t value);
extern void bta_hf_client_evt_val(tBTA_HF_CLIENT_CB* client_cb,
                                  tBTA_HF_CLIENT_EVT type, uint16_t value);
extern void bta_hf_client_operator_name(tBTA_HF_CLIENT_CB* client_name,
                                        char* name);
extern void bta_hf_client_clip(tBTA_HF_CLIENT_CB* client_cb, char* number);
extern void bta_hf_client_ccwa(tBTA_HF_CLIENT_CB* client_cb, char* number);
extern void bta_hf_client_at_result(tBTA_HF_CLIENT_CB* client_cb,
                                    tBTA_HF_CLIENT_AT_RESULT_TYPE type,
                                    uint16_t cme);
extern void bta_hf_client_clcc(tBTA_HF_CLIENT_CB* client_cb, uint32_t idx,
                               bool incoming, uint8_t status, bool mpty,
                               char* number);
extern void bta_hf_client_cnum(tBTA_HF_CLIENT_CB* client_cb, char* number,
                               uint16_t service);
extern void bta_hf_client_binp(tBTA_HF_CLIENT_CB* client_cb, char* number);

/* Action functions */
extern void bta_hf_client_start_close(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_start_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_acp_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_open(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_fail(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_disc_fail(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_open_fail(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_close(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_disc_acp_res(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_rfc_data(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_disc_int_res(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_svc_conn_open(tBTA_HF_CLIENT_DATA* p_data);

/* Commands handling functions */
extern void bta_hf_client_dial(tBTA_HF_CLIENT_DATA* p_data);
extern void bta_hf_client_send_at_cmd(tBTA_HF_CLIENT_DATA* p_data);
