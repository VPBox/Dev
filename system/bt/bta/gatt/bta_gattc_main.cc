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
 *  This file contains the GATT client main functions and state machine.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <string.h>

#include "bt_common.h"
#include "bta_gattc_int.h"

using base::StringPrintf;

/*****************************************************************************
 * Constants and types
 ****************************************************************************/

/* state machine action enumeration list */
enum {
  BTA_GATTC_OPEN,
  BTA_GATTC_OPEN_FAIL,
  BTA_GATTC_OPEN_ERROR,
  BTA_GATTC_CANCEL_OPEN,
  BTA_GATTC_CANCEL_OPEN_OK,
  BTA_GATTC_CANCEL_OPEN_ERROR,
  BTA_GATTC_CONN,
  BTA_GATTC_START_DISCOVER,
  BTA_GATTC_DISC_CMPL,
  BTA_GATTC_Q_CMD,
  BTA_GATTC_CLOSE,
  BTA_GATTC_CLOSE_FAIL,
  BTA_GATTC_READ,
  BTA_GATTC_WRITE,
  BTA_GATTC_OP_CMPL,
  BTA_GATTC_SEARCH,
  BTA_GATTC_FAIL,
  BTA_GATTC_CONFIRM,
  BTA_GATTC_EXEC,
  BTA_GATTC_READ_MULTI,
  BTA_GATTC_IGNORE_OP_CMPL,
  BTA_GATTC_DISC_CLOSE,
  BTA_GATTC_RESTART_DISCOVER,
  BTA_GATTC_CFG_MTU,

  BTA_GATTC_IGNORE
};
/* type for action functions */
typedef void (*tBTA_GATTC_ACTION)(tBTA_GATTC_CLCB* p_clcb,
                                  tBTA_GATTC_DATA* p_data);

/* action function list */
const tBTA_GATTC_ACTION bta_gattc_action[] = {
    bta_gattc_open,              /* BTA_GATTC_OPEN */
    bta_gattc_open_fail,         /* BTA_GATTC_OPEN_FAIL */
    bta_gattc_open_error,        /* BTA_GATTC_OPEN_ERROR */
    bta_gattc_cancel_open,       /* BTA_GATTC_CANCEL_OPEN */
    bta_gattc_cancel_open_ok,    /* BTA_GATTC_CANCEL_OPEN_OK */
    bta_gattc_cancel_open_error, /* BTA_GATTC_CANCEL_OPEN_ERROR */
    bta_gattc_conn,              /* BTA_GATTC_CONN */
    bta_gattc_start_discover,    /* BTA_GATTC_START_DISCOVER */
    bta_gattc_disc_cmpl,         /* BTA_GATTC_DISC_CMPL */
    bta_gattc_q_cmd,             /* BTA_GATTC_Q_CMD */
    bta_gattc_close,             /* BTA_GATTC_CLOSE */
    bta_gattc_close_fail,        /* BTA_GATTC_CLOSE_FAIL */
    bta_gattc_read,              /* BTA_GATTC_READ */
    bta_gattc_write,             /* BTA_GATTC_WRITE */
    bta_gattc_op_cmpl,           /* BTA_GATTC_OP_CMPL */
    bta_gattc_search,            /* BTA_GATTC_SEARCH */
    bta_gattc_fail,              /* BTA_GATTC_FAIL */
    bta_gattc_confirm,           /* BTA_GATTC_CONFIRM */
    bta_gattc_execute,           /* BTA_GATTC_EXEC */
    bta_gattc_read_multi,        /* BTA_GATTC_READ_MULTI */
    bta_gattc_ignore_op_cmpl,    /* BTA_GATTC_IGNORE_OP_CMPL */
    bta_gattc_disc_close,        /* BTA_GATTC_DISC_CLOSE */
    bta_gattc_restart_discover,  /* BTA_GATTC_RESTART_DISCOVER */
    bta_gattc_cfg_mtu            /* BTA_GATTC_CFG_MTU */
};

/* state table information */
#define BTA_GATTC_ACTIONS 1    /* number of actions */
#define BTA_GATTC_NEXT_STATE 1 /* position of next state */
#define BTA_GATTC_NUM_COLS 2   /* number of columns in state tables */

/* state table for idle state */
static const uint8_t bta_gattc_st_idle[][BTA_GATTC_NUM_COLS] = {
    /* Event                            Action 1                  Next state */
    /* BTA_GATTC_API_OPEN_EVT           */ {BTA_GATTC_OPEN,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_INT_OPEN_FAIL_EVT      */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_CANCEL_OPEN_EVT    */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_INT_CANCEL_OPEN_OK_EVT */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_API_READ_EVT           */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_WRITE_EVT          */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_EXEC_EVT           */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_CFG_MTU_EVT        */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_API_CLOSE_EVT          */ {BTA_GATTC_CLOSE_FAIL,
                                            BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_API_SEARCH_EVT         */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_CONFIRM_EVT        */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_READ_MULTI_EVT     */ {BTA_GATTC_FAIL, BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_INT_CONN_EVT           */ {BTA_GATTC_CONN, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_INT_DISCOVER_EVT       */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_DISCOVER_CMPL_EVT      */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_OP_CMPL_EVT            */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_INT_DISCONN_EVT       */ {BTA_GATTC_IGNORE, BTA_GATTC_IDLE_ST},

};

/* state table for wait for open state */
static const uint8_t bta_gattc_st_w4_conn[][BTA_GATTC_NUM_COLS] = {
    /* Event                            Action 1 Next state */
    /* BTA_GATTC_API_OPEN_EVT           */ {BTA_GATTC_OPEN,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_INT_OPEN_FAIL_EVT      */ {BTA_GATTC_OPEN_FAIL,
                                            BTA_GATTC_IDLE_ST},
    /* BTA_GATTC_API_CANCEL_OPEN_EVT    */ {BTA_GATTC_CANCEL_OPEN,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_INT_CANCEL_OPEN_OK_EVT */ {BTA_GATTC_CANCEL_OPEN_OK,
                                            BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_API_READ_EVT           */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_API_WRITE_EVT          */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_API_EXEC_EVT           */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_API_CFG_MTU_EVT        */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_W4_CONN_ST},

    /* BTA_GATTC_API_CLOSE_EVT          */ {BTA_GATTC_CANCEL_OPEN,
                                            BTA_GATTC_W4_CONN_ST},

    /* BTA_GATTC_API_SEARCH_EVT         */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_API_CONFIRM_EVT        */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_API_READ_MULTI_EVT     */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_W4_CONN_ST},

    /* BTA_GATTC_INT_CONN_EVT           */ {BTA_GATTC_CONN, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_INT_DISCOVER_EVT       */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_DISCOVER_CMPL_EVT       */ {BTA_GATTC_IGNORE,
                                             BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_OP_CMPL_EVT            */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_W4_CONN_ST},
    /* BTA_GATTC_INT_DISCONN_EVT      */ {BTA_GATTC_OPEN_FAIL,
                                          BTA_GATTC_IDLE_ST},

};

/* state table for open state */
static const uint8_t bta_gattc_st_connected[][BTA_GATTC_NUM_COLS] = {
    /* Event                            Action 1 Next state */
    /* BTA_GATTC_API_OPEN_EVT           */ {BTA_GATTC_OPEN, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_INT_OPEN_FAIL_EVT      */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_CANCEL_OPEN_EVT    */ {BTA_GATTC_CANCEL_OPEN_ERROR,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_INT_CANCEL_OPEN_OK_EVT */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_CONN_ST},

    /* BTA_GATTC_API_READ_EVT           */ {BTA_GATTC_READ, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_WRITE_EVT          */ {BTA_GATTC_WRITE, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_EXEC_EVT           */ {BTA_GATTC_EXEC, BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_CFG_MTU_EVT        */ {BTA_GATTC_CFG_MTU,
                                            BTA_GATTC_CONN_ST},

    /* BTA_GATTC_API_CLOSE_EVT          */ {BTA_GATTC_CLOSE, BTA_GATTC_IDLE_ST},

    /* BTA_GATTC_API_SEARCH_EVT         */ {BTA_GATTC_SEARCH,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_CONFIRM_EVT        */ {BTA_GATTC_CONFIRM,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_API_READ_MULTI_EVT     */ {BTA_GATTC_READ_MULTI,
                                            BTA_GATTC_CONN_ST},

    /* BTA_GATTC_INT_CONN_EVT           */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_INT_DISCOVER_EVT       */ {BTA_GATTC_START_DISCOVER,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_DISCOVER_CMPL_EVT       */ {BTA_GATTC_IGNORE,
                                             BTA_GATTC_CONN_ST},
    /* BTA_GATTC_OP_CMPL_EVT            */ {BTA_GATTC_OP_CMPL,
                                            BTA_GATTC_CONN_ST},

    /* BTA_GATTC_INT_DISCONN_EVT        */ {BTA_GATTC_CLOSE, BTA_GATTC_IDLE_ST},

};

/* state table for discover state */
static const uint8_t bta_gattc_st_discover[][BTA_GATTC_NUM_COLS] = {
    /* Event                            Action 1 Next state */
    /* BTA_GATTC_API_OPEN_EVT           */ {BTA_GATTC_OPEN,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_INT_OPEN_FAIL_EVT      */ {BTA_GATTC_IGNORE,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_CANCEL_OPEN_EVT    */ {BTA_GATTC_CANCEL_OPEN_ERROR,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_INT_CANCEL_OPEN_OK_EVT */ {BTA_GATTC_FAIL,
                                            BTA_GATTC_DISCOVER_ST},

    /* BTA_GATTC_API_READ_EVT           */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_WRITE_EVT          */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_EXEC_EVT           */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_CFG_MTU_EVT        */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},

    /* BTA_GATTC_API_CLOSE_EVT          */ {BTA_GATTC_DISC_CLOSE,
                                            BTA_GATTC_DISCOVER_ST},

    /* BTA_GATTC_API_SEARCH_EVT         */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_CONFIRM_EVT        */ {BTA_GATTC_CONFIRM,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_API_READ_MULTI_EVT     */ {BTA_GATTC_Q_CMD,
                                            BTA_GATTC_DISCOVER_ST},

    /* BTA_GATTC_INT_CONN_EVT           */ {BTA_GATTC_CONN,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_INT_DISCOVER_EVT       */ {BTA_GATTC_RESTART_DISCOVER,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_DISCOVER_CMPL_EVT      */ {BTA_GATTC_DISC_CMPL,
                                            BTA_GATTC_CONN_ST},
    /* BTA_GATTC_OP_CMPL_EVT            */ {BTA_GATTC_IGNORE_OP_CMPL,
                                            BTA_GATTC_DISCOVER_ST},
    /* BTA_GATTC_INT_DISCONN_EVT        */ {BTA_GATTC_CLOSE, BTA_GATTC_IDLE_ST},

};

/* type for state table */
typedef const uint8_t (*tBTA_GATTC_ST_TBL)[BTA_GATTC_NUM_COLS];

/* state table */
const tBTA_GATTC_ST_TBL bta_gattc_st_tbl[] = {
    bta_gattc_st_idle,      /* BTA_GATTC_IDLE_ST */
    bta_gattc_st_w4_conn,   /* BTA_GATTC_W4_CONN_ST */
    bta_gattc_st_connected, /* BTA_GATTC_CONN_ST */
    bta_gattc_st_discover   /* BTA_GATTC_DISCOVER_ST */
};

/*****************************************************************************
 * Global data
 ****************************************************************************/

/* GATTC control block */
tBTA_GATTC_CB bta_gattc_cb;

#if (BTA_GATT_DEBUG == TRUE)
static const char* gattc_evt_code(tBTA_GATTC_INT_EVT evt_code);
static const char* gattc_state_code(tBTA_GATTC_STATE state_code);
#endif

/*******************************************************************************
 *
 * Function         bta_gattc_sm_execute
 *
 * Description      State machine event handling function for GATTC
 *
 *
 * Returns          bool  : true if queued client request buffer can be
 *                          immediately released, else false
 *
 ******************************************************************************/
bool bta_gattc_sm_execute(tBTA_GATTC_CLCB* p_clcb, uint16_t event,
                          tBTA_GATTC_DATA* p_data) {
  tBTA_GATTC_ST_TBL state_table;
  uint8_t action;
  int i;
  bool rt = true;
  tBTA_GATTC_STATE in_state = p_clcb->state;
  uint16_t in_event = event;
#if (BTA_GATT_DEBUG == TRUE)
  VLOG(1) << StringPrintf("%s: State 0x%02x [%s], Event 0x%x[%s]", __func__,
                          in_state, gattc_state_code(in_state), in_event,
                          gattc_evt_code(in_event));
#else
  VLOG(1) << StringPrintf("%s: State 0x%02x, Event 0x%x", __func__, in_state,
                          in_event);
#endif

  /* look up the state table for the current state */
  state_table = bta_gattc_st_tbl[p_clcb->state];

  event &= 0x00FF;

  /* set next state */
  p_clcb->state = state_table[event][BTA_GATTC_NEXT_STATE];

  /* execute action functions */
  for (i = 0; i < BTA_GATTC_ACTIONS; i++) {
    action = state_table[event][i];
    if (action != BTA_GATTC_IGNORE) {
      (*bta_gattc_action[action])(p_clcb, p_data);
      if (p_clcb->p_q_cmd == p_data) {
        /* buffer is queued, don't free in the bta dispatcher.
         * we free it ourselves when a completion event is received.
         */
        rt = false;
      }
    } else {
      break;
    }
  }

#if (BTA_GATT_DEBUG == TRUE)
  if (in_state != p_clcb->state) {
    VLOG(1) << StringPrintf("GATTC State Change: [%s] -> [%s] after Event [%s]",
                            gattc_state_code(in_state),
                            gattc_state_code(p_clcb->state),
                            gattc_evt_code(in_event));
  }
#else
  VLOG(1) << StringPrintf(
      "%s: GATTC State Change: 0x%02x -> 0x%02x after Event 0x%x", __func__,
      in_state, p_clcb->state, in_event);
#endif
  return rt;
}

/*******************************************************************************
 *
 * Function         bta_gattc_hdl_event
 *
 * Description      GATT client main event handling function.
 *
 *
 * Returns          bool
 *
 ******************************************************************************/
bool bta_gattc_hdl_event(BT_HDR* p_msg) {
  tBTA_GATTC_CLCB* p_clcb = NULL;
  bool rt = true;
#if (BTA_GATT_DEBUG == TRUE)
  VLOG(1) << __func__ << ": Event:" << gattc_evt_code(p_msg->event);
#endif
  switch (p_msg->event) {

    case BTA_GATTC_API_OPEN_EVT:
      bta_gattc_process_api_open((tBTA_GATTC_DATA*)p_msg);
      break;

    case BTA_GATTC_API_CANCEL_OPEN_EVT:
      bta_gattc_process_api_open_cancel((tBTA_GATTC_DATA*)p_msg);
      break;

    default:
      if (p_msg->event == BTA_GATTC_INT_CONN_EVT)
        p_clcb = bta_gattc_find_int_conn_clcb((tBTA_GATTC_DATA*)p_msg);
      else if (p_msg->event == BTA_GATTC_INT_DISCONN_EVT)
        p_clcb = bta_gattc_find_int_disconn_clcb((tBTA_GATTC_DATA*)p_msg);
      else
        p_clcb = bta_gattc_find_clcb_by_conn_id(p_msg->layer_specific);

      if (p_clcb != NULL) {
        rt =
            bta_gattc_sm_execute(p_clcb, p_msg->event, (tBTA_GATTC_DATA*)p_msg);
      } else {
        VLOG(1) << "Ignore unknown conn ID: " << +p_msg->layer_specific;
      }

      break;
  }

  return rt;
}

/*****************************************************************************
 *  Debug Functions
 ****************************************************************************/
#if (BTA_GATT_DEBUG == TRUE)

/*******************************************************************************
 *
 * Function         gattc_evt_code
 *
 * Description
 *
 * Returns          void
 *
 ******************************************************************************/
static const char* gattc_evt_code(tBTA_GATTC_INT_EVT evt_code) {
  switch (evt_code) {
    case BTA_GATTC_API_OPEN_EVT:
      return "BTA_GATTC_API_OPEN_EVT";
    case BTA_GATTC_INT_OPEN_FAIL_EVT:
      return "BTA_GATTC_INT_OPEN_FAIL_EVT";
    case BTA_GATTC_API_CANCEL_OPEN_EVT:
      return "BTA_GATTC_API_CANCEL_OPEN_EVT";
    case BTA_GATTC_INT_CANCEL_OPEN_OK_EVT:
      return "BTA_GATTC_INT_CANCEL_OPEN_OK_EVT";
    case BTA_GATTC_API_READ_EVT:
      return "BTA_GATTC_API_READ_EVT";
    case BTA_GATTC_API_WRITE_EVT:
      return "BTA_GATTC_API_WRITE_EVT";
    case BTA_GATTC_API_EXEC_EVT:
      return "BTA_GATTC_API_EXEC_EVT";
    case BTA_GATTC_API_CLOSE_EVT:
      return "BTA_GATTC_API_CLOSE_EVT";
    case BTA_GATTC_API_SEARCH_EVT:
      return "BTA_GATTC_API_SEARCH_EVT";
    case BTA_GATTC_API_CONFIRM_EVT:
      return "BTA_GATTC_API_CONFIRM_EVT";
    case BTA_GATTC_API_READ_MULTI_EVT:
      return "BTA_GATTC_API_READ_MULTI_EVT";
    case BTA_GATTC_INT_CONN_EVT:
      return "BTA_GATTC_INT_CONN_EVT";
    case BTA_GATTC_INT_DISCOVER_EVT:
      return "BTA_GATTC_INT_DISCOVER_EVT";
    case BTA_GATTC_DISCOVER_CMPL_EVT:
      return "BTA_GATTC_DISCOVER_CMPL_EVT";
    case BTA_GATTC_OP_CMPL_EVT:
      return "BTA_GATTC_OP_CMPL_EVT";
    case BTA_GATTC_INT_DISCONN_EVT:
      return "BTA_GATTC_INT_DISCONN_EVT";
    case BTA_GATTC_API_CFG_MTU_EVT:
      return "BTA_GATTC_API_CFG_MTU_EVT";
    default:
      return "unknown GATTC event code";
  }
}

/*******************************************************************************
 *
 * Function         gattc_state_code
 *
 * Description
 *
 * Returns          void
 *
 ******************************************************************************/
static const char* gattc_state_code(tBTA_GATTC_STATE state_code) {
  switch (state_code) {
    case BTA_GATTC_IDLE_ST:
      return "GATTC_IDLE_ST";
    case BTA_GATTC_W4_CONN_ST:
      return "GATTC_W4_CONN_ST";
    case BTA_GATTC_CONN_ST:
      return "GATTC_CONN_ST";
    case BTA_GATTC_DISCOVER_ST:
      return "GATTC_DISCOVER_ST";
    default:
      return "unknown GATTC state code";
  }
}

#endif /* Debug Functions */
