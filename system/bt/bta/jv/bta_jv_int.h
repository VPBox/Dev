/******************************************************************************
 *
 *  Copyright 2006-2012 Broadcom Corporation
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
 *  This is the private interface file for the BTA Java I/F
 *
 ******************************************************************************/
#ifndef BTA_JV_INT_H
#define BTA_JV_INT_H

#include "bta_api.h"
#include "bta_jv_api.h"
#include "bta_sys.h"
#include "port_api.h"
#include "rfcdefs.h"

#include <memory>

/*****************************************************************************
 *  Constants
 ****************************************************************************/

#ifndef BTA_JV_RFC_EV_MASK
#define BTA_JV_RFC_EV_MASK \
  (PORT_EV_RXCHAR | PORT_EV_TXEMPTY | PORT_EV_FC | PORT_EV_FCS)
#endif

enum {
  BTA_JV_PM_FREE_ST = 0, /* empty PM slot */
  BTA_JV_PM_IDLE_ST,
  BTA_JV_PM_BUSY_ST
};

/* BTA JV PM control block */
typedef struct {
  uint32_t handle;      /* The connection handle */
  uint8_t state;        /* state: see above enum */
  tBTA_JV_PM_ID app_id; /* JV app specific id indicating power table to use */
  RawAddress peer_bd_addr; /* Peer BD address */
} tBTA_JV_PM_CB;

enum {
  BTA_JV_ST_NONE = 0,
  BTA_JV_ST_CL_OPENING,
  BTA_JV_ST_CL_OPEN,
  BTA_JV_ST_CL_CLOSING,
  BTA_JV_ST_SR_LISTEN,
  BTA_JV_ST_SR_OPEN,
  BTA_JV_ST_SR_CLOSING
};
typedef uint8_t tBTA_JV_STATE;
#define BTA_JV_ST_CL_MAX BTA_JV_ST_CL_CLOSING
/* JV L2CAP control block */
typedef struct {
  tBTA_JV_L2CAP_CBACK* p_cback; /* the callback function */
  uint16_t psm;                 /* the psm used for this server connection */
  tBTA_JV_STATE state;          /* the state of this control block */
  tBTA_SERVICE_ID sec_id;       /* service id */
  uint32_t handle; /* the handle reported to java app (same as gap handle) */
  bool cong;       /* true, if congested */
  tBTA_JV_PM_CB* p_pm_cb; /* ptr to pm control block, NULL: unused */
  uint32_t l2cap_socket_id;
} tBTA_JV_L2C_CB;

#define BTA_JV_RFC_HDL_MASK 0xFF
#define BTA_JV_RFCOMM_MASK 0x80
#define BTA_JV_ALL_APP_ID 0xFF
#define BTA_JV_RFC_HDL_TO_SIDX(r) (((r)&0xFF00) >> 8)
#define BTA_JV_RFC_H_S_TO_HDL(h, s) ((h) | ((s) << 8))

/* port control block */
typedef struct {
  uint32_t handle;      /* the rfcomm session handle at jv */
  uint16_t port_handle; /* port handle */
  tBTA_JV_STATE state;  /* the state of this control block */
  uint8_t max_sess;     /* max sessions */
  uint32_t rfcomm_slot_id;
  bool cong;              /* true, if congested */
  tBTA_JV_PM_CB* p_pm_cb; /* ptr to pm control block, NULL: unused */
} tBTA_JV_PCB;

/* JV RFCOMM control block */
typedef struct {
  tBTA_JV_RFCOMM_CBACK* p_cback; /* the callback function */
  uint16_t rfc_hdl[BTA_JV_MAX_RFC_SR_SESSION];
  tBTA_SERVICE_ID sec_id; /* service id */
  uint8_t handle;         /* index: the handle reported to java app */
  uint8_t scn;            /* the scn of the server */
  uint8_t max_sess;       /* max sessions */
  int curr_sess;          /* current sessions count*/
} tBTA_JV_RFC_CB;

/* JV control block */
typedef struct {
  /* the SDP handle reported to JV user is the (index + 1) to sdp_handle[].
   * if sdp_handle[i]==0, it's not used.
   * otherwise sdp_handle[i] is the stack SDP handle. */
  uint32_t sdp_handle[BTA_JV_MAX_SDP_REC]; /* SDP records created */
  uint8_t* p_sel_raw_data; /* the raw data of last service select */
  tBTA_JV_DM_CBACK* p_dm_cback;
  tBTA_JV_L2C_CB l2c_cb[BTA_JV_MAX_L2C_CONN]; /* index is GAP handle (index) */
  tBTA_JV_RFC_CB rfc_cb[BTA_JV_MAX_RFC_CONN];
  tBTA_JV_PCB port_cb[MAX_RFC_PORTS];          /* index of this array is
                                                  the port_handle, */
  uint8_t sec_id[BTA_JV_NUM_SERVICE_ID];       /* service ID */
  bool scn[BTA_JV_MAX_SCN];                    /* SCN allocated by java */
  uint16_t free_psm_list[BTA_JV_MAX_L2C_CONN]; /* PSMs freed by java
                                                (can be reused) */
  uint8_t sdp_active;                          /* see BTA_JV_SDP_ACT_* */
  bluetooth::Uuid uuid;                   /* current uuid of sdp discovery*/
  tBTA_JV_PM_CB pm_cb[BTA_JV_PM_MAX_NUM]; /* PM on a per JV handle bases */
} tBTA_JV_CB;

enum {
  BTA_JV_SDP_ACT_NONE = 0,
  BTA_JV_SDP_ACT_YES,   /* waiting for SDP result */
  BTA_JV_SDP_ACT_CANCEL /* waiting for cancel complete */
};

/* JV control block */
extern tBTA_JV_CB bta_jv_cb;

/* config struct */
extern tBTA_JV_CFG* p_bta_jv_cfg;

extern void bta_jv_enable(tBTA_JV_DM_CBACK* p_cback);
extern void bta_jv_disable();
extern void bta_jv_get_channel_id(int32_t type, int32_t channel,
                                  uint32_t l2cap_socket_id,
                                  uint32_t rfcomm_slot_id);
extern void bta_jv_free_scn(int32_t type, uint16_t scn);
extern void bta_jv_start_discovery(const RawAddress& bd_addr, uint16_t num_uuid,
                                   bluetooth::Uuid* uuid_list,
                                   uint32_t rfcomm_slot_id);
extern void bta_jv_create_record(uint32_t rfcomm_slot_id);
extern void bta_jv_delete_record(uint32_t handle);
extern void bta_jv_l2cap_connect(int32_t type, tBTA_SEC sec_mask,
                                 tBTA_JV_ROLE role, uint16_t remote_psm,
                                 uint16_t rx_mtu,
                                 const RawAddress& peer_bd_addr,
                                 std::unique_ptr<tL2CAP_CFG_INFO> cfg,
                                 std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info,
                                 tBTA_JV_L2CAP_CBACK* p_cback,
                                 uint32_t l2cap_socket_id);
extern void bta_jv_l2cap_close(uint32_t handle, tBTA_JV_L2C_CB* p_cb);
extern void bta_jv_l2cap_start_server(
    int32_t type, tBTA_SEC sec_mask, tBTA_JV_ROLE role, uint16_t local_psm,
    uint16_t rx_mtu, std::unique_ptr<tL2CAP_CFG_INFO> cfg_param,
    std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info, tBTA_JV_L2CAP_CBACK* p_cback,
    uint32_t l2cap_socket_id);
extern void bta_jv_l2cap_stop_server(uint16_t local_psm,
                                     uint32_t l2cap_socket_id);
extern void bta_jv_l2cap_write(uint32_t handle, uint32_t req_id, BT_HDR* msg,
                               uint32_t user_id, tBTA_JV_L2C_CB* p_cb);
extern void bta_jv_rfcomm_connect(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                                  uint8_t remote_scn,
                                  const RawAddress& peer_bd_addr,
                                  tBTA_JV_RFCOMM_CBACK* p_cback,
                                  uint32_t rfcomm_slot_id);
extern void bta_jv_rfcomm_close(uint32_t handle, uint32_t rfcomm_slot_id);
extern void bta_jv_rfcomm_start_server(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                                       uint8_t local_scn, uint8_t max_session,
                                       tBTA_JV_RFCOMM_CBACK* p_cback,
                                       uint32_t rfcomm_slot_id);
extern void bta_jv_rfcomm_stop_server(uint32_t handle, uint32_t rfcomm_slot_id);
extern void bta_jv_rfcomm_write(uint32_t handle, uint32_t req_id,
                                tBTA_JV_RFC_CB* p_cb, tBTA_JV_PCB* p_pcb);
extern void bta_jv_set_pm_profile(uint32_t handle, tBTA_JV_PM_ID app_id,
                                  tBTA_JV_CONN_STATE init_st);
extern void bta_jv_l2cap_connect_le(uint16_t remote_chan,
                                    const RawAddress& peer_bd_addr,
                                    tBTA_JV_L2CAP_CBACK* p_cback,
                                    uint32_t l2cap_socket_id);
extern void bta_jv_l2cap_start_server_le(uint16_t local_chan,
                                         tBTA_JV_L2CAP_CBACK* p_cback,
                                         uint32_t l2cap_socket_id);
extern void bta_jv_l2cap_stop_server_le(uint16_t local_chan);
extern void bta_jv_l2cap_write_fixed(uint16_t channel, const RawAddress& addr,
                                     uint32_t req_id, BT_HDR* msg,
                                     uint32_t user_id,
                                     tBTA_JV_L2CAP_CBACK* p_cback);
extern void bta_jv_l2cap_close_fixed(uint32_t handle);

#endif /* BTA_JV_INT_H */
