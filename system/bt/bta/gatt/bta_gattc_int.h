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
 *  This is the private file for the file transfer client (FTC).
 *
 ******************************************************************************/
#ifndef BTA_GATTC_INT_H
#define BTA_GATTC_INT_H

#include "bt_target.h"

#include "bta_gatt_api.h"
#include "bta_sys.h"
#include "database_builder.h"
#include "osi/include/fixed_queue.h"

#include "bt_common.h"

#include <base/logging.h>
#include <base/strings/stringprintf.h>

/*****************************************************************************
 *  Constants and data types
 ****************************************************************************/
enum {
  BTA_GATTC_API_OPEN_EVT = BTA_SYS_EVT_START(BTA_ID_GATTC),
  BTA_GATTC_INT_OPEN_FAIL_EVT,
  BTA_GATTC_API_CANCEL_OPEN_EVT,
  BTA_GATTC_INT_CANCEL_OPEN_OK_EVT,

  BTA_GATTC_API_READ_EVT,
  BTA_GATTC_API_WRITE_EVT,
  BTA_GATTC_API_EXEC_EVT,
  BTA_GATTC_API_CFG_MTU_EVT,

  BTA_GATTC_API_CLOSE_EVT,

  BTA_GATTC_API_SEARCH_EVT,
  BTA_GATTC_API_CONFIRM_EVT,
  BTA_GATTC_API_READ_MULTI_EVT,

  BTA_GATTC_INT_CONN_EVT,
  BTA_GATTC_INT_DISCOVER_EVT,
  BTA_GATTC_DISCOVER_CMPL_EVT,
  BTA_GATTC_OP_CMPL_EVT,
  BTA_GATTC_INT_DISCONN_EVT
};
typedef uint16_t tBTA_GATTC_INT_EVT;

#define BTA_GATTC_SERVICE_CHANGED_LEN 4

/* max client application GATTC can support */
#ifndef BTA_GATTC_CL_MAX
#define BTA_GATTC_CL_MAX 32
#endif

/* max known devices GATTC can support */
#ifndef BTA_GATTC_KNOWN_SR_MAX
#define BTA_GATTC_KNOWN_SR_MAX 10
#endif

#define BTA_GATTC_CONN_MAX GATT_MAX_PHY_CHANNEL

#ifndef BTA_GATTC_CLCB_MAX
#define BTA_GATTC_CLCB_MAX GATT_CL_MAX_LCB
#endif

#define BTA_GATTC_WRITE_PREPARE GATT_WRITE_PREPARE

/* internal strucutre for GATTC register API  */
typedef struct {
  BT_HDR hdr;
  RawAddress remote_bda;
  tGATT_IF client_if;
  bool is_direct;
  tBTA_TRANSPORT transport;
  uint8_t initiating_phys;
  bool opportunistic;
} tBTA_GATTC_API_OPEN;

typedef tBTA_GATTC_API_OPEN tBTA_GATTC_API_CANCEL_OPEN;

typedef struct {
  BT_HDR hdr;
  tGATT_AUTH_REQ auth_req;

  // read by handle data
  uint16_t handle;

  // read by UUID data
  bluetooth::Uuid uuid;
  uint16_t s_handle;
  uint16_t e_handle;

  tBTA_GATTC_EVT cmpl_evt;
  GATT_READ_OP_CB read_cb;
  void* read_cb_data;
} tBTA_GATTC_API_READ;

typedef struct {
  BT_HDR hdr;
  tGATT_AUTH_REQ auth_req;
  uint16_t handle;
  tGATT_WRITE_TYPE write_type;
  uint16_t offset;
  uint16_t len;
  uint8_t* p_value;
  GATT_WRITE_OP_CB write_cb;
  void* write_cb_data;
} tBTA_GATTC_API_WRITE;

typedef struct {
  BT_HDR hdr;
  bool is_execute;
} tBTA_GATTC_API_EXEC;

typedef struct {
  BT_HDR hdr;
  uint16_t handle;
} tBTA_GATTC_API_CONFIRM;

typedef tGATT_CL_COMPLETE tBTA_GATTC_CMPL;

typedef struct {
  BT_HDR hdr;
  uint8_t op_code;
  tGATT_STATUS status;
  tBTA_GATTC_CMPL* p_cmpl;
} tBTA_GATTC_OP_CMPL;

typedef struct {
  BT_HDR hdr;
  bluetooth::Uuid* p_srvc_uuid;
} tBTA_GATTC_API_SEARCH;

typedef struct {
  BT_HDR hdr;
  tGATT_AUTH_REQ auth_req;
  uint8_t num_attr;
  uint16_t handles[GATT_MAX_READ_MULTI_HANDLES];
} tBTA_GATTC_API_READ_MULTI;

typedef struct {
  BT_HDR hdr;
  uint16_t mtu;
} tBTA_GATTC_API_CFG_MTU;

typedef struct {
  BT_HDR hdr;
  RawAddress remote_bda;
  tGATT_IF client_if;
  uint8_t role;
  tBT_TRANSPORT transport;
  tGATT_DISCONN_REASON reason;
} tBTA_GATTC_INT_CONN;

typedef union {
  BT_HDR hdr;
  tBTA_GATTC_API_OPEN api_conn;
  tBTA_GATTC_API_CANCEL_OPEN api_cancel_conn;
  tBTA_GATTC_API_READ api_read;
  tBTA_GATTC_API_SEARCH api_search;
  tBTA_GATTC_API_WRITE api_write;
  tBTA_GATTC_API_CONFIRM api_confirm;
  tBTA_GATTC_API_EXEC api_exec;
  tBTA_GATTC_API_READ_MULTI api_read_multi;
  tBTA_GATTC_API_CFG_MTU api_mtu;
  tBTA_GATTC_OP_CMPL op_cmpl;
  tBTA_GATTC_INT_CONN int_conn;
} tBTA_GATTC_DATA;

enum {
  BTA_GATTC_IDLE_ST = 0, /* Idle  */
  BTA_GATTC_W4_CONN_ST,  /* Wait for connection -  (optional) */
  BTA_GATTC_CONN_ST,     /* connected state */
  BTA_GATTC_DISCOVER_ST  /* discover is in progress */
};
typedef uint8_t tBTA_GATTC_STATE;

typedef struct {
  bool in_use;
  RawAddress server_bda;
  bool connected;

#define BTA_GATTC_SERV_IDLE 0
#define BTA_GATTC_SERV_LOAD 1
#define BTA_GATTC_SERV_SAVE 2
#define BTA_GATTC_SERV_DISC 3
#define BTA_GATTC_SERV_DISC_ACT 4

  uint8_t state;

  gatt::Database gatt_database;
  uint8_t update_count; /* indication received */
  uint8_t num_clcb;     /* number of associated CLCB */

  gatt::DatabaseBuilder pending_discovery;

  uint8_t srvc_hdl_chg; /* service handle change indication pending */
  uint16_t attr_index;  /* cahce NV saving/loading attribute index */

  uint16_t mtu;
} tBTA_GATTC_SERV;

#ifndef BTA_GATTC_NOTIF_REG_MAX
#define BTA_GATTC_NOTIF_REG_MAX 15
#endif

typedef struct {
  bool in_use;
  RawAddress remote_bda;
  uint16_t handle;
} tBTA_GATTC_NOTIF_REG;

typedef struct {
  tBTA_GATTC_CBACK* p_cback;
  bool in_use;
  tGATT_IF client_if; /* client interface with BTE stack for this application */
  uint8_t num_clcb; /* number of associated CLCB */
  bool dereg_pending;
  bluetooth::Uuid app_uuid;
  tBTA_GATTC_NOTIF_REG notif_reg[BTA_GATTC_NOTIF_REG_MAX];
} tBTA_GATTC_RCB;

/* client channel is a mapping between a BTA client(cl_id) and a remote BD
 * address */
typedef struct {
  uint16_t bta_conn_id; /* client channel ID, unique for clcb */
  RawAddress bda;
  tBTA_TRANSPORT transport; /* channel transport */
  tBTA_GATTC_RCB* p_rcb;    /* pointer to the registration CB */
  tBTA_GATTC_SERV* p_srcb;  /* server cache CB */
  tBTA_GATTC_DATA* p_q_cmd; /* command in queue waiting for execution */

#define BTA_GATTC_NO_SCHEDULE 0
#define BTA_GATTC_DISC_WAITING 0x01
#define BTA_GATTC_REQ_WAITING 0x10

  uint8_t auto_update; /* auto update is waiting */
  bool disc_active;
  bool in_use;
  tBTA_GATTC_STATE state;
  tGATT_STATUS status;
  uint16_t reason;
} tBTA_GATTC_CLCB;

/* back ground connection tracking information */
#if GATT_MAX_APPS <= 8
typedef uint8_t tBTA_GATTC_CIF_MASK;
#elif GATT_MAX_APPS <= 16
typedef uint16_t tBTA_GATTC_CIF_MASK;
#elif GATT_MAX_APPS <= 32
typedef uint32_t tBTA_GATTC_CIF_MASK;
#endif

typedef struct {
  bool in_use;
  RawAddress remote_bda;
  tBTA_GATTC_CIF_MASK cif_mask;

} tBTA_GATTC_BG_TCK;

typedef struct {
  bool in_use;
  RawAddress remote_bda;
} tBTA_GATTC_CONN;

enum {
  BTA_GATTC_STATE_DISABLED,
  BTA_GATTC_STATE_ENABLING,
  BTA_GATTC_STATE_ENABLED,
  BTA_GATTC_STATE_DISABLING
};

typedef struct {
  uint8_t state;

  tBTA_GATTC_CONN conn_track[BTA_GATTC_CONN_MAX];
  tBTA_GATTC_BG_TCK bg_track[BTA_GATTC_KNOWN_SR_MAX];
  tBTA_GATTC_RCB cl_rcb[BTA_GATTC_CL_MAX];

  tBTA_GATTC_CLCB clcb[BTA_GATTC_CLCB_MAX];
  tBTA_GATTC_SERV known_server[BTA_GATTC_KNOWN_SR_MAX];
} tBTA_GATTC_CB;

/*****************************************************************************
 *  Global data
 ****************************************************************************/

/* GATTC control block */
extern tBTA_GATTC_CB bta_gattc_cb;

/*****************************************************************************
 *  Function prototypes
 ****************************************************************************/
extern bool bta_gattc_hdl_event(BT_HDR* p_msg);
extern bool bta_gattc_sm_execute(tBTA_GATTC_CLCB* p_clcb, uint16_t event,
                                 tBTA_GATTC_DATA* p_data);

/* function processed outside SM */
extern void bta_gattc_disable();
extern void bta_gattc_register(const bluetooth::Uuid& app_uuid,
                               tBTA_GATTC_CBACK* p_data,
                               BtaAppRegisterCallback cb);
extern void bta_gattc_process_api_open(tBTA_GATTC_DATA* p_msg);
extern void bta_gattc_process_api_open_cancel(tBTA_GATTC_DATA* p_msg);
extern void bta_gattc_deregister(tBTA_GATTC_RCB* p_clreg);

/* function within state machine */
extern void bta_gattc_open(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_open_fail(tBTA_GATTC_CLCB* p_clcb,
                                tBTA_GATTC_DATA* p_data);
extern void bta_gattc_open_error(tBTA_GATTC_CLCB* p_clcb,
                                 tBTA_GATTC_DATA* p_data);

extern void bta_gattc_cancel_open(tBTA_GATTC_CLCB* p_clcb,
                                  tBTA_GATTC_DATA* p_data);
extern void bta_gattc_cancel_open_ok(tBTA_GATTC_CLCB* p_clcb,
                                     tBTA_GATTC_DATA* p_data);
extern void bta_gattc_cancel_open_error(tBTA_GATTC_CLCB* p_clcb,
                                        tBTA_GATTC_DATA* p_data);

extern void bta_gattc_conn(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);

extern void bta_gattc_close(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_close_fail(tBTA_GATTC_CLCB* p_clcb,
                                 tBTA_GATTC_DATA* p_data);
extern void bta_gattc_disc_close(tBTA_GATTC_CLCB* p_clcb,
                                 tBTA_GATTC_DATA* p_data);

extern void bta_gattc_start_discover(tBTA_GATTC_CLCB* p_clcb,
                                     tBTA_GATTC_DATA* p_data);
extern void bta_gattc_disc_cmpl(tBTA_GATTC_CLCB* p_clcb,
                                tBTA_GATTC_DATA* p_data);
extern void bta_gattc_read(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_write(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_op_cmpl(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_q_cmd(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_search(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_fail(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_confirm(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_execute(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_read_multi(tBTA_GATTC_CLCB* p_clcb,
                                 tBTA_GATTC_DATA* p_data);
extern void bta_gattc_ci_open(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_ci_close(tBTA_GATTC_CLCB* p_clcb,
                               tBTA_GATTC_DATA* p_data);
extern void bta_gattc_ignore_op_cmpl(tBTA_GATTC_CLCB* p_clcb,
                                     tBTA_GATTC_DATA* p_data);
extern void bta_gattc_restart_discover(tBTA_GATTC_CLCB* p_clcb,
                                       tBTA_GATTC_DATA* p_msg);
extern void bta_gattc_init_bk_conn(tBTA_GATTC_API_OPEN* p_data,
                                   tBTA_GATTC_RCB* p_clreg);
extern void bta_gattc_cancel_bk_conn(tBTA_GATTC_API_CANCEL_OPEN* p_data);
extern void bta_gattc_send_open_cback(tBTA_GATTC_RCB* p_clreg,
                                      tGATT_STATUS status,
                                      const RawAddress& remote_bda,
                                      uint16_t conn_id,
                                      tBTA_TRANSPORT transport, uint16_t mtu);
extern void bta_gattc_process_api_refresh(const RawAddress& remote_bda);
extern void bta_gattc_cfg_mtu(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);
extern void bta_gattc_listen(tBTA_GATTC_DATA* p_msg);
extern void bta_gattc_broadcast(tBTA_GATTC_DATA* p_msg);

/* utility functions */
extern tBTA_GATTC_CLCB* bta_gattc_find_clcb_by_cif(uint8_t client_if,
                                                   const RawAddress& remote_bda,
                                                   tBTA_TRANSPORT transport);
extern tBTA_GATTC_CLCB* bta_gattc_find_clcb_by_conn_id(uint16_t conn_id);
extern tBTA_GATTC_CLCB* bta_gattc_clcb_alloc(tGATT_IF client_if,
                                             const RawAddress& remote_bda,
                                             tBTA_TRANSPORT transport);
extern void bta_gattc_clcb_dealloc(tBTA_GATTC_CLCB* p_clcb);
extern tBTA_GATTC_CLCB* bta_gattc_find_alloc_clcb(tGATT_IF client_if,
                                                  const RawAddress& remote_bda,
                                                  tBTA_TRANSPORT transport);
extern tBTA_GATTC_RCB* bta_gattc_cl_get_regcb(uint8_t client_if);
extern tBTA_GATTC_SERV* bta_gattc_find_srcb(const RawAddress& bda);
extern tBTA_GATTC_SERV* bta_gattc_srcb_alloc(const RawAddress& bda);
extern tBTA_GATTC_SERV* bta_gattc_find_scb_by_cid(uint16_t conn_id);
extern tBTA_GATTC_CLCB* bta_gattc_find_int_conn_clcb(tBTA_GATTC_DATA* p_msg);
extern tBTA_GATTC_CLCB* bta_gattc_find_int_disconn_clcb(tBTA_GATTC_DATA* p_msg);

extern bool bta_gattc_enqueue(tBTA_GATTC_CLCB* p_clcb, tBTA_GATTC_DATA* p_data);

extern bool bta_gattc_check_notif_registry(tBTA_GATTC_RCB* p_clreg,
                                           tBTA_GATTC_SERV* p_srcb,
                                           tBTA_GATTC_NOTIFY* p_notify);
extern bool bta_gattc_mark_bg_conn(tGATT_IF client_if,
                                   const RawAddress& remote_bda, bool add);
extern bool bta_gattc_check_bg_conn(tGATT_IF client_if,
                                    const RawAddress& remote_bda, uint8_t role);
extern uint8_t bta_gattc_num_reg_app(void);
extern void bta_gattc_clear_notif_registration(tBTA_GATTC_SERV* p_srcb,
                                               uint16_t conn_id,
                                               uint16_t start_handle,
                                               uint16_t end_handle);
extern tBTA_GATTC_SERV* bta_gattc_find_srvr_cache(const RawAddress& bda);

/* discovery functions */
extern void bta_gattc_disc_res_cback(uint16_t conn_id,
                                     tGATT_DISC_TYPE disc_type,
                                     tGATT_DISC_RES* p_data);
extern void bta_gattc_disc_cmpl_cback(uint16_t conn_id,
                                      tGATT_DISC_TYPE disc_type,
                                      tGATT_STATUS status);
extern tGATT_STATUS bta_gattc_discover_pri_service(uint16_t conn_id,
                                                   tBTA_GATTC_SERV* p_server_cb,
                                                   uint8_t disc_type);
extern void bta_gattc_search_service(tBTA_GATTC_CLCB* p_clcb,
                                     bluetooth::Uuid* p_uuid);
extern const std::list<gatt::Service>* bta_gattc_get_services(uint16_t conn_id);
extern const gatt::Service* bta_gattc_get_service_for_handle(uint16_t conn_id,
                                                             uint16_t handle);
const gatt::Characteristic* bta_gattc_get_characteristic_srcb(
    tBTA_GATTC_SERV* p_srcb, uint16_t handle);
extern const gatt::Service* bta_gattc_get_service_for_handle_srcb(
    tBTA_GATTC_SERV* p_srcb, uint16_t handle);
extern const gatt::Characteristic* bta_gattc_get_characteristic(
    uint16_t conn_id, uint16_t handle);
extern const gatt::Descriptor* bta_gattc_get_descriptor(uint16_t conn_id,
                                                        uint16_t handle);
extern const gatt::Characteristic* bta_gattc_get_owning_characteristic(
    uint16_t conn_id, uint16_t handle);
extern void bta_gattc_get_gatt_db(uint16_t conn_id, uint16_t start_handle,
                                  uint16_t end_handle, btgatt_db_element_t** db,
                                  int* count);
extern void bta_gattc_init_cache(tBTA_GATTC_SERV* p_srvc_cb);
extern void bta_gattc_reset_discover_st(tBTA_GATTC_SERV* p_srcb,
                                        tGATT_STATUS status);

extern tBTA_GATTC_CONN* bta_gattc_conn_alloc(const RawAddress& remote_bda);
extern tBTA_GATTC_CONN* bta_gattc_conn_find(const RawAddress& remote_bda);
extern tBTA_GATTC_CONN* bta_gattc_conn_find_alloc(const RawAddress& remote_bda);
extern bool bta_gattc_conn_dealloc(const RawAddress& remote_bda);

extern bool bta_gattc_cache_load(tBTA_GATTC_SERV* p_srcb);
extern void bta_gattc_cache_reset(const RawAddress& server_bda);

#endif /* BTA_GATTC_INT_H */
