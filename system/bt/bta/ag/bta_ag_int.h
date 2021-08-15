/******************************************************************************
 *
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

/******************************************************************************
 *
 *  This is the private interface file for the BTA audio gateway.
 *
 ******************************************************************************/
#ifndef BTA_AG_INT_H
#define BTA_AG_INT_H

#include "bta_ag_api.h"
#include "bta_ag_at.h"
#include "bta_api.h"
#include "bta_sys.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/
/* Time to wait for retry in case of collision */
#ifndef BTA_AG_COLLISION_TIMEOUT_MS
#define BTA_AG_COLLISION_TIMEOUT_MS (2 * 1000) /* 2 seconds */
#endif

/* RFCOMM MTU SIZE */
#define BTA_AG_MTU 256

/* Max number of peer and local HF indicators */
#define BTA_AG_MAX_NUM_PEER_HF_IND 20
#define BTA_AG_MAX_NUM_LOCAL_HF_IND 4

/* Internal profile indexes */
#define BTA_AG_HSP 0     /* index for HSP */
#define BTA_AG_HFP 1     /* index for HFP */
#define BTA_AG_NUM_IDX 2 /* number of profile indexes */

/* profile role for connection */
#define BTA_AG_ACP 0 /* accepted connection */
#define BTA_AG_INT 1 /* initiating connection */

#define BTA_AG_SDP_FEAT_SPEC                                \
  (BTA_AG_FEAT_3WAY | BTA_AG_FEAT_ECNR | BTA_AG_FEAT_VREC | \
   BTA_AG_FEAT_INBAND | BTA_AG_FEAT_VTAG)

enum {
  /* these events are handled by the state machine */
  BTA_AG_API_REGISTER_EVT = BTA_SYS_EVT_START(BTA_ID_AG),
  BTA_AG_API_DEREGISTER_EVT,
  BTA_AG_API_OPEN_EVT,
  BTA_AG_API_CLOSE_EVT,
  BTA_AG_API_AUDIO_OPEN_EVT,
  BTA_AG_API_AUDIO_CLOSE_EVT,
  BTA_AG_API_RESULT_EVT,
  BTA_AG_API_SETCODEC_EVT,
  BTA_AG_RFC_OPEN_EVT,
  BTA_AG_RFC_CLOSE_EVT,
  BTA_AG_RFC_SRV_CLOSE_EVT,
  BTA_AG_RFC_DATA_EVT,
  BTA_AG_SCO_OPEN_EVT,
  BTA_AG_SCO_CLOSE_EVT,
  BTA_AG_DISC_ACP_RES_EVT,
  BTA_AG_DISC_INT_RES_EVT,
  BTA_AG_DISC_OK_EVT,
  BTA_AG_DISC_FAIL_EVT,
  BTA_AG_RING_TIMEOUT_EVT,
  BTA_AG_SVC_TIMEOUT_EVT,
  BTA_AG_COLLISION_EVT,
  BTA_AG_MAX_EVT,
};

/* Actions to perform after a SCO event */
enum {
  BTA_AG_POST_SCO_NONE,      /* no action */
  BTA_AG_POST_SCO_CLOSE_RFC, /* close RFCOMM channel after SCO closes */
  BTA_AG_POST_SCO_RING,      /* send RING result code after SCO opens */
  BTA_AG_POST_SCO_CALL_CONN, /* send call indicators after SCO opens/closes */
  BTA_AG_POST_SCO_CALL_ORIG, /* send call indicators after SCO closes */
  BTA_AG_POST_SCO_CALL_END,  /* send call indicators after SCO closes */
  BTA_AG_POST_SCO_CALL_END_INCALL /* send call indicators for end call &
                                     incoming call after SCO closes */
};

/* sco states */
enum {
  BTA_AG_SCO_SHUTDOWN_ST,   /* no sco listening, all sco connections closed */
  BTA_AG_SCO_LISTEN_ST,     /* sco listening */
  BTA_AG_SCO_CODEC_ST,      /* sco codec negotiation */
  BTA_AG_SCO_OPENING_ST,    /* sco connection opening */
  BTA_AG_SCO_OPEN_CL_ST,    /* opening sco connection being closed */
  BTA_AG_SCO_OPEN_XFER_ST,  /* opening sco connection being transferred */
  BTA_AG_SCO_OPEN_ST,       /* sco open */
  BTA_AG_SCO_CLOSING_ST,    /* sco closing */
  BTA_AG_SCO_CLOSE_OP_ST,   /* closing sco being opened */
  BTA_AG_SCO_CLOSE_XFER_ST, /* closing sco being transferred */
  BTA_AG_SCO_SHUTTING_ST    /* sco shutting down */
};

/*****************************************************************************
 *  Data types
 ****************************************************************************/

/* data type for BTA_AG_API_REGISTER_EVT */
typedef struct {
  char p_name[2][BTA_SERVICE_NAME_LEN + 1];
  tBTA_SERVICE_MASK services;
  tBTA_SEC sec_mask;
  tBTA_AG_FEAT features;
  uint8_t app_id;
} tBTA_AG_API_REGISTER;

/* data type for BTA_AG_API_OPEN_EVT */
typedef struct {
  RawAddress bd_addr;
  tBTA_SEC sec_mask;
} tBTA_AG_API_OPEN;

/* data type for BTA_AG_API_RESULT_EVT */
typedef struct {
  tBTA_AG_RES result;
  tBTA_AG_RES_DATA data;
} tBTA_AG_API_RESULT;

/* data type for BTA_AG_API_SETCODEC_EVT */
typedef struct {
  tBTA_AG_PEER_CODEC codec;
} tBTA_AG_API_SETCODEC;

/* data type for BTA_AG_DISC_RESULT_EVT */
typedef struct {
  uint16_t status;
} tBTA_AG_DISC_RESULT;

/* data type for RFCOMM events */
typedef struct {
  uint16_t port_handle;
} tBTA_AG_RFC;

/* union of all event datatypes */
union tBTA_AG_DATA {
  tBTA_AG_API_REGISTER api_register;
  tBTA_AG_API_OPEN api_open;
  tBTA_AG_API_RESULT api_result;
  tBTA_AG_API_SETCODEC api_setcodec;
  tBTA_AG_DISC_RESULT disc_result;
  tBTA_AG_RFC rfc;
  static const tBTA_AG_DATA kEmpty;

  /**
   * Check if two tBTA_AG_DATA are equal in memory
   *
   * @param rhs other tBTA_AG_DATA
   * @return true if both unions are equal in memory
   */
  bool operator==(const tBTA_AG_DATA& rhs) const {
    return (std::memcmp(this, &rhs, sizeof(tBTA_AG_DATA)) == 0);
  }

  /**
   * Check if this union is empty by comparing it to the kEmpty constant
   *
   * @return true if this union is empty
   */
  bool IsEmpty() const { return *this == kEmpty; }
};

/* type for each profile */
typedef struct {
  uint32_t sdp_handle;
  uint8_t scn;
} tBTA_AG_PROFILE;

typedef enum {
  BTA_AG_SCO_MSBC_SETTINGS_T2 = 0, /* preferred/default when codec is mSBC */
  BTA_AG_SCO_MSBC_SETTINGS_T1,
} tBTA_AG_SCO_MSBC_SETTINGS;

/* type for each service control block */
struct tBTA_AG_SCB {
  char clip[BTA_AG_AT_MAX_LEN + 1];     /* number string used for CLIP */
  uint16_t serv_handle[BTA_AG_NUM_IDX]; /* RFCOMM server handles */
  tBTA_AG_AT_CB at_cb;                  /* AT command interpreter */
  RawAddress peer_addr;                 /* peer bd address */
  tSDP_DISCOVERY_DB* p_disc_db;         /* pointer to discovery database */
  tBTA_SERVICE_MASK reg_services;       /* services specified in register API */
  tBTA_SERVICE_MASK open_services;      /* services specified in open API */
  uint16_t conn_handle;                 /* RFCOMM handle of connected service */
  tBTA_SEC serv_sec_mask;               /* server security mask */
  tBTA_SEC cli_sec_mask;                /* client security mask */
  tBTA_AG_FEAT features;                /* features registered by application */
  tBTA_AG_PEER_FEAT peer_features;      /* peer device features */
  uint16_t peer_sdp_features;           /* peer device SDP features */
  uint16_t peer_version;                /* profile version of peer device */
  uint16_t hsp_version;                 /* HSP profile version before SDP */
  uint16_t sco_idx;                     /* SCO handle */
  bool in_use;                          /* scb in use */
  bool dealloc;                         /* true if service shutting down */
  bool clip_enabled;        /* set to true if HF enables CLIP reporting */
  bool ccwa_enabled;        /* set to true if HF enables CCWA reporting */
  bool cmer_enabled;        /* set to true if HF enables CMER reporting */
  bool cmee_enabled;        /* set to true if HF enables CME ERROR reporting */
  bool inband_enabled;      /* set to true if inband ring enabled */
  bool svc_conn;            /* set to true when service level connection up */
  uint8_t state;            /* state machine state */
  uint8_t conn_service;     /* connected service */
  uint8_t peer_scn;         /* peer scn */
  uint8_t app_id;           /* application id */
  uint8_t role;             /* initiator/acceptor role */
  uint8_t post_sco;         /* action to perform after sco event */
  uint8_t call_ind;         /* CIEV call indicator value */
  uint8_t callsetup_ind;    /* CIEV callsetup indicator value */
  uint8_t service_ind;      /* CIEV service indicator value */
  uint8_t signal_ind;       /* CIEV signal indicator value */
  uint8_t roam_ind;         /* CIEV roam indicator value */
  uint8_t battchg_ind;      /* CIEV battery charge indicator value */
  uint8_t callheld_ind;     /* CIEV call held indicator value */
  uint32_t bia_masked_out;  /* indicators HF does not want us to send */
  alarm_t* collision_timer;
  alarm_t* ring_timer;
  alarm_t* codec_negotiation_timer;
  bool received_at_bac; /* indicate AT+BAC is received at least once */
  tBTA_AG_PEER_CODEC peer_codecs; /* codecs for eSCO supported by the peer */
  tBTA_AG_PEER_CODEC sco_codec;   /* codec to be used for eSCO connection */
  tBTA_AG_PEER_CODEC
      inuse_codec;     /* codec being used for the current SCO connection */
  bool codec_updated;  /* set to true whenever the app updates codec type */
  bool codec_fallback; /* If sco nego fails for mSBC, fallback to CVSD */
  tBTA_AG_SCO_MSBC_SETTINGS
      codec_msbc_settings; /* settings to be used for the impending eSCO */

  tBTA_AG_HF_IND
      peer_hf_indicators[BTA_AG_MAX_NUM_PEER_HF_IND]; /* Peer supported
                                                  HF indicators */
  tBTA_AG_HF_IND
      local_hf_indicators[BTA_AG_MAX_NUM_LOCAL_HF_IND]; /* Local supported
                                                    HF indicators */
};

/* type for sco data */
typedef struct {
  tBTM_ESCO_CONN_REQ_EVT_DATA conn_data; /* SCO data for pending conn request */
  tBTA_AG_SCB* p_curr_scb;  /* SCB associated with SCO connection */
  tBTA_AG_SCB* p_xfer_scb;  /* SCB associated with SCO transfer */
  uint16_t cur_idx;         /* SCO handle */
  uint8_t state;            /* SCO state variable */
  bool is_local;            /* SCO connection initiated locally or remotely */
} tBTA_AG_SCO_CB;

/* type for AG control block */
typedef struct {
  tBTA_AG_SCB scb[BTA_AG_MAX_NUM_CLIENTS]; /* service control blocks */
  tBTA_AG_PROFILE profile[BTA_AG_NUM_IDX]; /* profile-specific data */
  tBTA_AG_SCO_CB sco;                      /* SCO data */
  tBTA_AG_CBACK* p_cback;                  /* application callback */
} tBTA_AG_CB;

/*****************************************************************************
 *  Global data
 ****************************************************************************/

/* constant lookup tables */
extern const uint16_t bta_ag_uuid[BTA_AG_NUM_IDX];
extern const uint8_t bta_ag_sec_id[BTA_AG_NUM_IDX];
extern const tBTA_AG_AT_CMD* bta_ag_at_tbl[BTA_AG_NUM_IDX];

/* control block declaration */
extern tBTA_AG_CB bta_ag_cb;

/* config struct */
extern const tBTA_AG_CFG* p_bta_ag_cfg;
extern const tBTA_AG_HF_IND bta_ag_local_hf_ind_cfg[];

/*****************************************************************************
 *  Function prototypes
 ****************************************************************************/
bool bta_ag_hdl_event(BT_HDR* p_msg);

/* API functions */
extern void bta_ag_api_enable(tBTA_AG_CBACK* p_cback);
extern void bta_ag_api_disable();
extern void bta_ag_api_set_active_device(const RawAddress& new_active_device);
extern void bta_ag_api_register(tBTA_SERVICE_MASK services, tBTA_SEC sec_mask,
                                tBTA_AG_FEAT features,
                                const std::vector<std::string>& service_names,
                                uint8_t app_id);
extern void bta_ag_api_result(uint16_t handle, tBTA_AG_RES result,
                              const tBTA_AG_RES_DATA& result_data);

/* main functions */
extern void bta_ag_scb_dealloc(tBTA_AG_SCB* p_scb);
extern uint16_t bta_ag_scb_to_idx(tBTA_AG_SCB* p_scb);
extern tBTA_AG_SCB* bta_ag_scb_by_idx(uint16_t idx);
extern uint8_t bta_ag_service_to_idx(tBTA_SERVICE_MASK services);
extern uint16_t bta_ag_idx_by_bdaddr(const RawAddress* peer_addr);
extern bool bta_ag_other_scb_open(tBTA_AG_SCB* p_curr_scb);
extern bool bta_ag_scb_open(tBTA_AG_SCB* p_curr_scb);
extern void bta_ag_sm_execute(tBTA_AG_SCB* p_scb, uint16_t event,
                              const tBTA_AG_DATA& data);
extern void bta_ag_sm_execute_by_handle(uint16_t handle, uint16_t event,
                                        const tBTA_AG_DATA& data);
extern void bta_ag_collision_cback(tBTA_SYS_CONN_STATUS status, uint8_t id,
                                   uint8_t app_id, const RawAddress& peer_addr);
extern void bta_ag_resume_open(tBTA_AG_SCB* p_scb);

/* SDP functions */
extern bool bta_ag_add_record(uint16_t service_uuid, const char* p_service_name,
                              uint8_t scn, tBTA_AG_FEAT features,
                              uint32_t sdp_handle);
extern void bta_ag_create_records(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_del_records(tBTA_AG_SCB* p_scb);
extern bool bta_ag_sdp_find_attr(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK service);
extern void bta_ag_do_disc(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK service);
extern void bta_ag_free_db(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);

/* RFCOMM functions */
extern void bta_ag_start_servers(tBTA_AG_SCB* p_scb,
                                 tBTA_SERVICE_MASK services);
extern void bta_ag_close_servers(tBTA_AG_SCB* p_scb,
                                 tBTA_SERVICE_MASK services);
extern bool bta_ag_is_server_closed(tBTA_AG_SCB* p_scb);
extern void bta_ag_rfc_do_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_do_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);

/* SCO functions */
extern bool bta_ag_sco_is_active_device(const RawAddress& bd_addr);
extern bool bta_ag_sco_is_open(tBTA_AG_SCB* p_scb);
extern bool bta_ag_sco_is_opening(tBTA_AG_SCB* p_scb);
extern void bta_ag_sco_conn_rsp(tBTA_AG_SCB* p_scb,
                                tBTM_ESCO_CONN_REQ_EVT_DATA* data);

/* AT command functions */
extern void bta_ag_at_hsp_cback(tBTA_AG_SCB* p_scb, uint16_t cmd,
                                uint8_t arg_type, char* p_arg, char* p_end,
                                int16_t int_arg);
extern void bta_ag_at_hfp_cback(tBTA_AG_SCB* p_scb, uint16_t cmd,
                                uint8_t arg_type, char* p_arg, char* p_end,
                                int16_t int_arg);
extern void bta_ag_at_err_cback(tBTA_AG_SCB* p_scb, bool unknown,
                                const char* p_arg);
extern bool bta_ag_inband_enabled(tBTA_AG_SCB* p_scb);
extern void bta_ag_send_call_inds(tBTA_AG_SCB* p_scb, tBTA_AG_RES result);

/* Action functions */
extern void bta_ag_register(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_deregister(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_start_dereg(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_start_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_start_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_disc_int_res(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_disc_acp_res(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_disc_fail(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_open_fail(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_fail(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_acp_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_rfc_data(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_listen(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_shutdown(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_conn_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_sco_conn_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_post_sco_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_post_sco_close(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_svc_conn_open(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_result(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_setcodec(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_send_ring(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);
extern void bta_ag_handle_collision(tBTA_AG_SCB* p_scb,
                                    const tBTA_AG_DATA& data);

/* Internal utility functions */
extern void bta_ag_sco_codec_nego(tBTA_AG_SCB* p_scb, bool result);
extern void bta_ag_codec_negotiate(tBTA_AG_SCB* p_scb);
extern void bta_ag_send_bcs(tBTA_AG_SCB* p_scb);
extern void bta_ag_set_sco_allowed(bool value);
extern const RawAddress& bta_ag_get_active_device();
extern void bta_clear_active_device();

#endif /* BTA_AG_INT_H */
