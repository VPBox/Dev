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
 *  This is the private file for the BTA GATT server.
 *
 ******************************************************************************/
#ifndef BTA_GATTS_INT_H
#define BTA_GATTS_INT_H

#include "bt_target.h"
#include "bta_gatt_api.h"
#include "bta_sys.h"
#include "gatt_api.h"

#include <base/strings/stringprintf.h>
#include "bt_common.h"

/*****************************************************************************
 *  Constants and data types
 ****************************************************************************/
enum {
  BTA_GATTS_API_REG_EVT = BTA_SYS_EVT_START(BTA_ID_GATTS),
  BTA_GATTS_INT_START_IF_EVT,
  BTA_GATTS_API_DEREG_EVT,
  BTA_GATTS_API_INDICATION_EVT,

  BTA_GATTS_API_DEL_SRVC_EVT,
  BTA_GATTS_API_STOP_SRVC_EVT,
  BTA_GATTS_API_RSP_EVT,
  BTA_GATTS_API_OPEN_EVT,
  BTA_GATTS_API_CANCEL_OPEN_EVT,
  BTA_GATTS_API_CLOSE_EVT,
  BTA_GATTS_API_DISABLE_EVT
};
typedef uint16_t tBTA_GATTS_INT_EVT;

/* max number of application allowed on device */
#define BTA_GATTS_MAX_APP_NUM GATT_MAX_SR_PROFILES

/* max number of services allowed in the device */
#define BTA_GATTS_MAX_SRVC_NUM GATT_MAX_SR_PROFILES

/* internal strucutre for GATTC register API  */
typedef struct {
  BT_HDR hdr;
  bluetooth::Uuid app_uuid;
  tBTA_GATTS_CBACK* p_cback;
} tBTA_GATTS_API_REG;

typedef struct {
  BT_HDR hdr;
  tGATT_IF server_if;
} tBTA_GATTS_INT_START_IF;

typedef tBTA_GATTS_INT_START_IF tBTA_GATTS_API_DEREG;

typedef struct {
  BT_HDR hdr;
  tGATT_IF server_if;
  btgatt_db_element_t* service;
  uint16_t count;
} tBTA_GATTS_API_ADD_SERVICE;

typedef struct {
  BT_HDR hdr;
  uint16_t attr_id;
  uint16_t len;
  bool need_confirm;
  uint8_t value[GATT_MAX_ATTR_LEN];
} tBTA_GATTS_API_INDICATION;

typedef struct {
  BT_HDR hdr;
  uint32_t trans_id;
  tGATT_STATUS status;
  tGATTS_RSP* p_rsp;
} tBTA_GATTS_API_RSP;

typedef struct {
  BT_HDR hdr;
  tGATT_TRANSPORT transport;
} tBTA_GATTS_API_START;

typedef struct {
  BT_HDR hdr;
  RawAddress remote_bda;
  tGATT_IF server_if;
  bool is_direct;
  tGATT_TRANSPORT transport;

} tBTA_GATTS_API_OPEN;

typedef tBTA_GATTS_API_OPEN tBTA_GATTS_API_CANCEL_OPEN;

typedef union {
  BT_HDR hdr;
  tBTA_GATTS_API_REG api_reg;
  tBTA_GATTS_API_DEREG api_dereg;
  tBTA_GATTS_API_ADD_SERVICE api_add_service;
  tBTA_GATTS_API_INDICATION api_indicate;
  tBTA_GATTS_API_RSP api_rsp;
  tBTA_GATTS_API_OPEN api_open;
  tBTA_GATTS_API_CANCEL_OPEN api_cancel_open;

  tBTA_GATTS_INT_START_IF int_start_if;
} tBTA_GATTS_DATA;

/* application registration control block */
typedef struct {
  bool in_use;
  bluetooth::Uuid app_uuid;
  tBTA_GATTS_CBACK* p_cback;
  tGATT_IF gatt_if;
} tBTA_GATTS_RCB;

/* service registration control block */
typedef struct {
  bluetooth::Uuid service_uuid; /* service UUID */
  uint16_t service_id;   /* service start handle */
  uint8_t rcb_idx;
  uint8_t idx; /* self index of serviec CB */
  bool in_use;
} tBTA_GATTS_SRVC_CB;

/* GATT server control block */
typedef struct {
  bool enabled;
  tBTA_GATTS_RCB rcb[BTA_GATTS_MAX_APP_NUM];
  tBTA_GATTS_SRVC_CB srvc_cb[BTA_GATTS_MAX_SRVC_NUM];
} tBTA_GATTS_CB;

/*****************************************************************************
 *  Global data
 ****************************************************************************/

/* GATTC control block */
extern tBTA_GATTS_CB bta_gatts_cb;

/*****************************************************************************
 *  Function prototypes
 ****************************************************************************/
extern bool bta_gatts_hdl_event(BT_HDR* p_msg);

extern void bta_gatts_api_disable(tBTA_GATTS_CB* p_cb);
extern void bta_gatts_api_enable(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_data);
extern void bta_gatts_register(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_start_if(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_deregister(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_delete_service(tBTA_GATTS_SRVC_CB* p_srvc_cb,
                                     tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_stop_service(tBTA_GATTS_SRVC_CB* p_srvc_cb,
                                   tBTA_GATTS_DATA* p_msg);

extern void bta_gatts_send_rsp(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_indicate_handle(tBTA_GATTS_CB* p_cb,
                                      tBTA_GATTS_DATA* p_msg);

extern void bta_gatts_open(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_cancel_open(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);
extern void bta_gatts_close(tBTA_GATTS_CB* p_cb, tBTA_GATTS_DATA* p_msg);

extern tBTA_GATTS_RCB* bta_gatts_find_app_rcb_by_app_if(tGATT_IF server_if);
extern uint8_t bta_gatts_find_app_rcb_idx_by_app_if(tBTA_GATTS_CB* p_cb,
                                                    tGATT_IF server_if);
extern uint8_t bta_gatts_alloc_srvc_cb(tBTA_GATTS_CB* p_cb, uint8_t rcb_idx);
extern tBTA_GATTS_SRVC_CB* bta_gatts_find_srvc_cb_by_srvc_id(
    tBTA_GATTS_CB* p_cb, uint16_t service_id);
extern tBTA_GATTS_SRVC_CB* bta_gatts_find_srvc_cb_by_attr_id(
    tBTA_GATTS_CB* p_cb, uint16_t attr_id);

#endif /* BTA_GATTS_INT_H */
