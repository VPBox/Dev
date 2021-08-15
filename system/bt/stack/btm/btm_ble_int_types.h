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

#ifndef BTM_BLE_INT_TYPES_H
#define BTM_BLE_INT_TYPES_H

#include "osi/include/alarm.h"

/* scanning enable status */
#define BTM_BLE_SCAN_ENABLE 0x01
#define BTM_BLE_SCAN_DISABLE 0x00

/* advertising enable status */
#define BTM_BLE_ADV_ENABLE 0x01
#define BTM_BLE_ADV_DISABLE 0x00

/* use the high 4 bits unused by inquiry mode */
#define BTM_BLE_SELECT_SCAN 0x20
#define BTM_BLE_NAME_REQUEST 0x40
#define BTM_BLE_OBSERVE 0x80

#define BTM_BLE_MAX_WL_ENTRY 1
#define BTM_BLE_AD_DATA_LEN 31

#define BTM_BLE_ENC_MASK 0x03

#define BTM_BLE_DUPLICATE_ENABLE 1
#define BTM_BLE_DUPLICATE_DISABLE 0

/* Interval(scan_int) = 11.25 ms= 0x0010 * 0.625 ms */
#define BTM_BLE_GAP_DISC_SCAN_INT 18
/* scan_window = 11.25 ms= 0x0010 * 0.625 ms */
#define BTM_BLE_GAP_DISC_SCAN_WIN 18
/* Tgap(gen_disc) = 1.28 s= 512 * 0.625 ms */
#define BTM_BLE_GAP_ADV_INT 512
/* Tgap(lim_timeout) = 180s max */
#define BTM_BLE_GAP_LIM_TIMEOUT_MS (180 * 1000)
/* Interval(scan_int) = 5s= 8000 * 0.625 ms */
#define BTM_BLE_LOW_LATENCY_SCAN_INT 8000
/* scan_window = 5s= 8000 * 0.625 ms */
#define BTM_BLE_LOW_LATENCY_SCAN_WIN 8000

/* TGAP(adv_fast_interval1) = 30(used) ~ 60 ms  = 48 *0.625 */
#define BTM_BLE_GAP_ADV_FAST_INT_1 48
/* TGAP(adv_fast_interval2) = 100(used) ~ 150 ms = 160 * 0.625 ms */
#define BTM_BLE_GAP_ADV_FAST_INT_2 160
/* Tgap(adv_slow_interval) = 1.28 s= 512 * 0.625 ms */
#define BTM_BLE_GAP_ADV_SLOW_INT 2048
/* Tgap(dir_conn_adv_int_max) = 500 ms = 800 * 0.625 ms */
#define BTM_BLE_GAP_ADV_DIR_MAX_INT 800
/* Tgap(dir_conn_adv_int_min) = 250 ms = 400 * 0.625 ms */
#define BTM_BLE_GAP_ADV_DIR_MIN_INT 400

#define BTM_BLE_GAP_FAST_ADV_TIMEOUT_MS (30 * 1000)

#define BTM_BLE_SEC_REQ_ACT_NONE 0
/* encrypt the link using current key or key refresh */
#define BTM_BLE_SEC_REQ_ACT_ENCRYPT 1
#define BTM_BLE_SEC_REQ_ACT_PAIR 2
/* discard the sec request while encryption is started but not completed */
#define BTM_BLE_SEC_REQ_ACT_DISCARD 3
typedef uint8_t tBTM_BLE_SEC_REQ_ACT;

#define BLE_STATIC_PRIVATE_MSB_MASK 0x3f
/*  most significant bit, bit7, bit6 is 01 to be resolvable random */
#define BLE_RESOLVE_ADDR_MSB 0x40
/* bit 6, and bit7 */
#define BLE_RESOLVE_ADDR_MASK 0xc0
inline bool BTM_BLE_IS_RESOLVE_BDA(const RawAddress& x) {
  return ((x.address)[0] & BLE_RESOLVE_ADDR_MASK) == BLE_RESOLVE_ADDR_MSB;
}

/* LE scan activity bit mask, continue with LE inquiry bits */
/* observe is in progress */
#define BTM_LE_OBSERVE_ACTIVE 0x80

/* BLE scan activity mask checking */
#define BTM_BLE_IS_SCAN_ACTIVE(x) ((x)&BTM_BLE_SCAN_ACTIVE_MASK)
#define BTM_BLE_IS_INQ_ACTIVE(x) ((x)&BTM_BLE_INQUIRY_MASK)
#define BTM_BLE_IS_OBS_ACTIVE(x) ((x)&BTM_LE_OBSERVE_ACTIVE)

/* BLE ADDR type ID bit */
#define BLE_ADDR_TYPE_ID_BIT 0x02

#define BTM_VSC_CHIP_CAPABILITY_L_VERSION 55
#define BTM_VSC_CHIP_CAPABILITY_M_VERSION 95

typedef struct {
  uint16_t data_mask;
  uint8_t* p_flags;
  uint8_t ad_data[BTM_BLE_AD_DATA_LEN];
  uint8_t* p_pad;
} tBTM_BLE_LOCAL_ADV_DATA;

typedef struct {
  /* Used for determining if a response has already been received for the
   * current inquiry operation. (We do not want to flood the caller with
   * multiple responses from the same device. */
  uint32_t inq_count;
  bool scan_rsp;
  tBLE_BD_ADDR le_bda;
} tINQ_LE_BDADDR;

#define BTM_BLE_ISVALID_PARAM(x, min, max) \
  (((x) >= (min) && (x) <= (max)) || ((x) == BTM_BLE_CONN_PARAM_UNDEF))

/* 15 minutes minimum for random address refreshing */
#define BTM_BLE_PRIVATE_ADDR_INT_MS (15 * 60 * 1000)

typedef struct {
  uint16_t discoverable_mode;
  uint16_t connectable_mode;
  uint32_t scan_window;
  uint32_t scan_interval;
  uint8_t scan_type;             /* current scan type: active or passive */
  uint8_t scan_duplicate_filter; /* duplicate filter enabled for scan */
  uint16_t adv_interval_min;
  uint16_t adv_interval_max;
  tBTM_BLE_AFP afp; /* advertising filter policy */
  tBTM_BLE_SFP sfp; /* scanning filter policy */

  tBLE_ADDR_TYPE adv_addr_type;
  uint8_t evt_type;
  uint8_t adv_mode;
  tBLE_BD_ADDR direct_bda;
  tBTM_BLE_EVT directed_conn;
  bool fast_adv_on;
  alarm_t* fast_adv_timer;

  /* inquiry BD addr database */
  uint8_t num_bd_entries;
  uint8_t max_bd_entries;
  tBTM_BLE_LOCAL_ADV_DATA adv_data;
  tBTM_BLE_ADV_CHNL_MAP adv_chnl_map;

  alarm_t* inquiry_timer;
  bool scan_rsp;
  uint8_t state; /* Current state that the inquiry process is in */
  int8_t tx_power;
} tBTM_BLE_INQ_CB;

/* random address resolving complete callback */
typedef void(tBTM_BLE_RESOLVE_CBACK)(void* match_rec, void* p);

typedef void(tBTM_BLE_ADDR_CBACK)(const RawAddress& static_random, void* p);

/* random address management control block */
typedef struct {
  tBLE_ADDR_TYPE own_addr_type; /* local device LE address type */
  RawAddress private_addr;
  RawAddress random_bda;
  tBTM_BLE_ADDR_CBACK* p_generate_cback;
  void* p;
  alarm_t* refresh_raddr_timer;
} tBTM_LE_RANDOM_CB;

typedef struct {
  uint16_t min_conn_int;
  uint16_t max_conn_int;
  uint16_t slave_latency;
  uint16_t supervision_tout;

} tBTM_LE_CONN_PRAMS;

typedef struct {
  RawAddress bd_addr;
  uint8_t attr;
  bool is_connected;
  bool in_use;
} tBTM_LE_BG_CONN_DEV;

/* white list using state as a bit mask */
constexpr uint8_t BTM_BLE_WL_IDLE = 0;
constexpr uint8_t BTM_BLE_WL_INIT = 1;

/* resolving list using state as a bit mask */
#define BTM_BLE_RL_IDLE 0
#define BTM_BLE_RL_INIT 1
#define BTM_BLE_RL_SCAN 2
#define BTM_BLE_RL_ADV 4
typedef uint8_t tBTM_BLE_RL_STATE;

/* BLE connection state */
#define BLE_CONN_IDLE 0
#define BLE_CONNECTING 2
#define BLE_CONN_CANCEL 3
typedef uint8_t tBTM_BLE_CONN_ST;

typedef struct { void* p_param; } tBTM_BLE_CONN_REQ;

/* LE state request */
#define BTM_BLE_STATE_INVALID 0
#define BTM_BLE_STATE_CONN_ADV 1
#define BTM_BLE_STATE_INIT 2
#define BTM_BLE_STATE_MASTER 3
#define BTM_BLE_STATE_SLAVE 4
#define BTM_BLE_STATE_LO_DUTY_DIR_ADV 5
#define BTM_BLE_STATE_HI_DUTY_DIR_ADV 6
#define BTM_BLE_STATE_NON_CONN_ADV 7
#define BTM_BLE_STATE_PASSIVE_SCAN 8
#define BTM_BLE_STATE_ACTIVE_SCAN 9
#define BTM_BLE_STATE_SCAN_ADV 10
#define BTM_BLE_STATE_MAX 11
typedef uint8_t tBTM_BLE_STATE;

#define BTM_BLE_STATE_CONN_ADV_BIT 0x0001
#define BTM_BLE_STATE_INIT_BIT 0x0002
#define BTM_BLE_STATE_MASTER_BIT 0x0004
#define BTM_BLE_STATE_SLAVE_BIT 0x0008
#define BTM_BLE_STATE_LO_DUTY_DIR_ADV_BIT 0x0010
#define BTM_BLE_STATE_HI_DUTY_DIR_ADV_BIT 0x0020
#define BTM_BLE_STATE_NON_CONN_ADV_BIT 0x0040
#define BTM_BLE_STATE_PASSIVE_SCAN_BIT 0x0080
#define BTM_BLE_STATE_ACTIVE_SCAN_BIT 0x0100
#define BTM_BLE_STATE_SCAN_ADV_BIT 0x0200
typedef uint16_t tBTM_BLE_STATE_MASK;

#define BTM_BLE_STATE_ALL_MASK 0x03ff
#define BTM_BLE_STATE_ALL_ADV_MASK                                  \
  (BTM_BLE_STATE_CONN_ADV_BIT | BTM_BLE_STATE_LO_DUTY_DIR_ADV_BIT | \
   BTM_BLE_STATE_HI_DUTY_DIR_ADV_BIT | BTM_BLE_STATE_SCAN_ADV_BIT)
#define BTM_BLE_STATE_ALL_SCAN_MASK \
  (BTM_BLE_STATE_PASSIVE_SCAN_BIT | BTM_BLE_STATE_ACTIVE_SCAN_BIT)
#define BTM_BLE_STATE_ALL_CONN_MASK \
  (BTM_BLE_STATE_MASTER_BIT | BTM_BLE_STATE_SLAVE_BIT)

#ifndef BTM_LE_RESOLVING_LIST_MAX
#define BTM_LE_RESOLVING_LIST_MAX 0x20
#endif

typedef struct {
  RawAddress* resolve_q_random_pseudo;
  uint8_t* resolve_q_action;
  uint8_t q_next;
  uint8_t q_pending;
} tBTM_BLE_RESOLVE_Q;

typedef struct {
  bool in_use;
  bool to_add;
  RawAddress bd_addr;
  uint8_t attr;
} tBTM_BLE_WL_OP;

/* BLE privacy mode */
#define BTM_PRIVACY_NONE 0 /* BLE no privacy */
#define BTM_PRIVACY_1_1 1  /* BLE privacy 1.1, do not support privacy 1.0 */
#define BTM_PRIVACY_1_2 2  /* BLE privacy 1.2 */
#define BTM_PRIVACY_MIXED \
  3 /* BLE privacy mixed mode, broadcom propietary mode */
typedef uint8_t tBTM_PRIVACY_MODE;

/* data length change event callback */
typedef void(tBTM_DATA_LENGTH_CHANGE_CBACK)(uint16_t max_tx_length,
                                            uint16_t max_rx_length);

/* Define BLE Device Management control structure
*/
typedef struct {
  uint8_t scan_activity; /* LE scan activity mask */

  /*****************************************************
  **      BLE Inquiry
  *****************************************************/
  tBTM_BLE_INQ_CB inq_var;

  /* observer callback and timer */
  tBTM_INQ_RESULTS_CB* p_obs_results_cb;
  tBTM_CMPL_CB* p_obs_cmpl_cb;
  alarm_t* observer_timer;

  /* background connection procedure cb value */
  uint16_t scan_int;
  uint16_t scan_win;

  /* white list information */
  uint8_t wl_state;

  tBTM_BLE_CONN_ST conn_state;

  /* random address management control block */
  tBTM_LE_RANDOM_CB addr_mgnt_cb;

  bool enabled;

#if (BLE_PRIVACY_SPT == TRUE)
  bool mixed_mode;                   /* privacy 1.2 mixed mode is on or not */
  tBTM_PRIVACY_MODE privacy_mode;    /* privacy mode */
  uint8_t resolving_list_avail_size; /* resolving list available size */
  tBTM_BLE_RESOLVE_Q resolving_list_pend_q; /* Resolving list queue */
  tBTM_BLE_RL_STATE suspended_rl_state;     /* Suspended resolving list state */
  uint8_t* irk_list_mask; /* IRK list availability mask, up to max entry bits */
  tBTM_BLE_RL_STATE rl_state; /* Resolving list state */
#endif

  /* current BLE link state */
  tBTM_BLE_STATE_MASK cur_states; /* bit mask of tBTM_BLE_STATE */
  uint8_t link_count[2];          /* total link count master and slave*/
} tBTM_BLE_CB;

#endif  // BTM_BLE_INT_TYPES_H
