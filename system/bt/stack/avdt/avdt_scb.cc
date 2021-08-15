/******************************************************************************
 *
 *  Copyright 2002-2012 Broadcom Corporation
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
 *  This module contains the stream control block and functions which
 *  operate on the stream control block.
 *
 ******************************************************************************/

#include <string.h>
#include "avdt_api.h"
#include "avdt_int.h"
#include "avdtc_api.h"
#include "bt_common.h"
#include "bt_target.h"
#include "bt_types.h"
#include "bt_utils.h"
#include "btu.h"
#include "osi/include/osi.h"

/*****************************************************************************
 * state machine constants and types
 ****************************************************************************/
#if (AVDT_DEBUG == TRUE)

/* verbose state strings for trace */
const char* const avdt_scb_st_str[] = {"SCB_IDLE_ST",    "SCB_CONF_ST",
                                       "SCB_OPENING_ST", "SCB_OPEN_ST",
                                       "SCB_STREAM_ST",  "SCB_CLOSING_ST"};

/* verbose event strings for trace */
const char* const avdt_scb_evt_str[] = {
    "API_REMOVE_EVT",        "API_WRITE_REQ_EVT",     "API_GETCONFIG_REQ_EVT",
    "API_DELAY_RPT_REQ",     "API_SETCONFIG_REQ_EVT", "API_OPEN_REQ_EVT",
    "API_CLOSE_REQ_EVT",     "API_RECONFIG_REQ_EVT",  "API_SECURITY_REQ_EVT",
    "API_ABORT_REQ_EVT",     "API_GETCONFIG_RSP_EVT", "API_SETCONFIG_RSP_EVT",
    "API_SETCONFIG_REJ_EVT", "API_OPEN_RSP_EVT",      "API_CLOSE_RSP_EVT",
    "API_RECONFIG_RSP_EVT",  "API_SECURITY_RSP_EVT",  "API_ABORT_RSP_EVT",
    "MSG_SETCONFIG_CMD_EVT", "MSG_GETCONFIG_CMD_EVT", "MSG_OPEN_CMD_EVT",
    "MSG_START_CMD_EVT",     "MSG_SUSPEND_CMD_EVT",   "MSG_CLOSE_CMD_EVT",
    "MSG_ABORT_CMD_EVT",     "MSG_RECONFIG_CMD_EVT",  "MSG_SECURITY_CMD_EVT",
    "MSG_DELAY_RPT_CMD_EVT", "MSG_DELAY_RPT_RSP_EVT", "MSG_SETCONFIG_RSP_EVT",
    "MSG_GETCONFIG_RSP_EVT", "MSG_OPEN_RSP_EVT",      "MSG_START_RSP_EVT",
    "MSG_SUSPEND_RSP_EVT",   "MSG_CLOSE_RSP_EVT",     "MSG_ABORT_RSP_EVT",
    "MSG_RECONFIG_RSP_EVT",  "MSG_SECURITY_RSP_EVT",  "MSG_SETCONFIG_REJ_EVT",
    "MSG_OPEN_REJ_EVT",      "MSG_START_REJ_EVT",     "MSG_SUSPEND_REJ_EVT",
    "TC_TOUT_EVT",           "TC_OPEN_EVT",           "TC_CLOSE_EVT",
    "TC_CONG_EVT",           "TC_DATA_EVT",           "CC_CLOSE_EVT"};

#endif

/* action function list */
const tAVDT_SCB_ACTION avdt_scb_action[] = {avdt_scb_hdl_abort_cmd,
                                            avdt_scb_hdl_abort_rsp,
                                            avdt_scb_hdl_close_cmd,
                                            avdt_scb_hdl_close_rsp,
                                            avdt_scb_hdl_getconfig_cmd,
                                            avdt_scb_hdl_getconfig_rsp,
                                            avdt_scb_hdl_open_cmd,
                                            avdt_scb_hdl_open_rej,
                                            avdt_scb_hdl_open_rsp,
                                            avdt_scb_hdl_pkt,
                                            avdt_scb_drop_pkt,
                                            avdt_scb_hdl_reconfig_cmd,
                                            avdt_scb_hdl_reconfig_rsp,
                                            avdt_scb_hdl_security_cmd,
                                            avdt_scb_hdl_security_rsp,
                                            avdt_scb_hdl_setconfig_cmd,
                                            avdt_scb_hdl_setconfig_rej,
                                            avdt_scb_hdl_setconfig_rsp,
                                            avdt_scb_hdl_start_cmd,
                                            avdt_scb_hdl_start_rsp,
                                            avdt_scb_hdl_suspend_cmd,
                                            avdt_scb_hdl_suspend_rsp,
                                            avdt_scb_hdl_tc_close,
                                            avdt_scb_hdl_tc_close_sto,
                                            avdt_scb_hdl_tc_open,
                                            avdt_scb_hdl_tc_open_sto,
                                            avdt_scb_snd_delay_rpt_req,
                                            avdt_scb_hdl_delay_rpt_cmd,
                                            avdt_scb_hdl_delay_rpt_rsp,
                                            avdt_scb_hdl_write_req,
                                            avdt_scb_snd_abort_req,
                                            avdt_scb_snd_abort_rsp,
                                            avdt_scb_snd_close_req,
                                            avdt_scb_snd_stream_close,
                                            avdt_scb_snd_close_rsp,
                                            avdt_scb_snd_getconfig_req,
                                            avdt_scb_snd_getconfig_rsp,
                                            avdt_scb_snd_open_req,
                                            avdt_scb_snd_open_rsp,
                                            avdt_scb_snd_reconfig_req,
                                            avdt_scb_snd_reconfig_rsp,
                                            avdt_scb_snd_security_req,
                                            avdt_scb_snd_security_rsp,
                                            avdt_scb_snd_setconfig_req,
                                            avdt_scb_snd_setconfig_rej,
                                            avdt_scb_snd_setconfig_rsp,
                                            avdt_scb_snd_tc_close,
                                            avdt_scb_cb_err,
                                            avdt_scb_cong_state,
                                            avdt_scb_rej_state,
                                            avdt_scb_rej_in_use,
                                            avdt_scb_rej_not_in_use,
                                            avdt_scb_set_remove,
                                            avdt_scb_free_pkt,
                                            avdt_scb_clr_pkt,
                                            avdt_scb_chk_snd_pkt,
                                            avdt_scb_transport_channel_timer,
                                            avdt_scb_clr_vars,
                                            avdt_scb_dealloc};

/* state table information */
#define AVDT_SCB_ACTIONS 2    /* number of actions */
#define AVDT_SCB_NEXT_STATE 2 /* position of next state */
#define AVDT_SCB_NUM_COLS 3   /* number of columns in state tables */

/* state table for idle state */
const uint8_t avdt_scb_st_idle[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_DEALLOC, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_FREE_PKT, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_SND_SETCONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_SND_SETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_SND_SETCONFIG_REJ, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_HDL_SETCONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_REJ_NOT_IN_USE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_HDL_SETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_HDL_SETCONFIG_REJ, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_DROP_PKT, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_CLR_VARS, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST}};

/* state table for configured state */
const uint8_t avdt_scb_st_conf[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_SET_REMOVE, AVDT_SCB_CONF_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_FREE_PKT, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_SND_GETCONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_SND_DELAY_RPT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_SND_OPEN_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_SND_SECURITY_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_SND_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_SND_OPEN_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_SND_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IDLE_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_IN_USE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_HDL_GETCONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_HDL_OPEN_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_HDL_SECURITY_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_HDL_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_HDL_OPEN_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_HDL_ABORT_RSP, AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IDLE_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_HDL_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_HDL_OPEN_REJ, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_DROP_PKT, AVDT_SCB_IGNORE, AVDT_SCB_CONF_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST}};

/* state table for opening state */
const uint8_t avdt_scb_st_opening[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_SND_CLOSE_REQ, AVDT_SCB_SET_REMOVE, AVDT_SCB_CLOSING_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_FREE_PKT, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_SND_GETCONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_SND_DELAY_RPT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_SND_CLOSE_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_SND_SECURITY_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_SND_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_SND_CLOSE_RSP, AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_SND_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_HDL_GETCONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_HDL_CLOSE_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_HDL_SECURITY_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_HDL_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_HDL_ABORT_RSP, AVDT_SCB_CLOSING_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_HDL_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_HDL_TC_OPEN, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_CONG_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_DROP_PKT, AVDT_SCB_IGNORE, AVDT_SCB_OPENING_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST}};

/* state table for open state */
const uint8_t avdt_scb_st_open[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_SND_CLOSE_REQ, AVDT_SCB_SET_REMOVE, AVDT_SCB_CLOSING_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_FREE_PKT, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_SND_GETCONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_SND_DELAY_RPT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_SND_CLOSE_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_SND_RECONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_SND_SECURITY_REQ, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_SND_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_SND_CLOSE_RSP, AVDT_SCB_TC_TIMER, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_SND_RECONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_SND_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_TC_TIMER, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_HDL_GETCONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_HDL_START_CMD, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_HDL_CLOSE_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_HDL_RECONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_HDL_SECURITY_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_HDL_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_HDL_RECONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_HDL_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_HDL_TC_OPEN_STO, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_HDL_TC_CLOSE_STO, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_CONG_STATE, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_DROP_PKT, AVDT_SCB_IGNORE, AVDT_SCB_OPEN_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST}};

/* state table for streaming state */
const uint8_t avdt_scb_st_stream[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_SND_STREAM_CLOSE, AVDT_SCB_SET_REMOVE, AVDT_SCB_CLOSING_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_HDL_WRITE_REQ, AVDT_SCB_CHK_SND_PKT, AVDT_SCB_STREAM_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_SND_GETCONFIG_REQ, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_SND_DELAY_RPT_REQ, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_SND_STREAM_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_SND_SECURITY_REQ, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_SND_ABORT_REQ, AVDT_SCB_CLR_PKT, AVDT_SCB_CLOSING_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_SND_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_SND_CLOSE_RSP, AVDT_SCB_TC_TIMER, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_SND_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_TC_TIMER, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_HDL_GETCONFIG_CMD, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_HDL_SUSPEND_CMD, AVDT_SCB_CLR_PKT, AVDT_SCB_OPEN_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_HDL_CLOSE_CMD, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_CLR_PKT, AVDT_SCB_STREAM_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_HDL_SECURITY_CMD, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_HDL_GETCONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_CLR_PKT, AVDT_SCB_OPEN_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_HDL_RECONFIG_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_HDL_SECURITY_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_CONG_STATE, AVDT_SCB_CHK_SND_PKT, AVDT_SCB_STREAM_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_HDL_PKT, AVDT_SCB_IGNORE, AVDT_SCB_STREAM_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST}};

/* state table for closing state */
const uint8_t avdt_scb_st_closing[][AVDT_SCB_NUM_COLS] = {
    /* Event */
    /* Action 1                       Action 2                    Next state */
    /* API_REMOVE_EVT */
    {AVDT_SCB_SET_REMOVE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_WRITE_REQ_EVT */
    {AVDT_SCB_FREE_PKT, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_GETCONFIG_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_DELAY_RPT_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_SETCONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_OPEN_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_CLOSE_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_SECURITY_REQ_EVT */
    {AVDT_SCB_CB_ERR, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_ABORT_REQ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_GETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_CLOSE_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_SECURITY_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* API_ABORT_RSP_EVT */
    {AVDT_SCB_SND_ABORT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_GETCONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_OPEN_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_START_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SUSPEND_CMD_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_CLOSE_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_ABORT_CMD_EVT */
    {AVDT_SCB_HDL_ABORT_CMD, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_RECONFIG_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SECURITY_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_DELAY_RPT_CMD_EVT */
    {AVDT_SCB_REJ_STATE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_DELAY_RPT_RSP_EVT */
    {AVDT_SCB_HDL_DELAY_RPT_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_GETCONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_OPEN_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_START_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SUSPEND_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_CLOSE_RSP_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_HDL_CLOSE_RSP, AVDT_SCB_CLOSING_ST},
    /* MSG_ABORT_RSP_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_HDL_ABORT_RSP, AVDT_SCB_CLOSING_ST},
    /* MSG_RECONFIG_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SECURITY_RSP_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SETCONFIG_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_OPEN_REJ_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_START_REJ_EVT */
    {AVDT_SCB_HDL_START_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* MSG_SUSPEND_REJ_EVT */
    {AVDT_SCB_HDL_SUSPEND_RSP, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* TC_TOUT_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* TC_OPEN_EVT */
    {AVDT_SCB_SND_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* TC_CLOSE_EVT */
    {AVDT_SCB_HDL_TC_CLOSE, AVDT_SCB_IGNORE, AVDT_SCB_IDLE_ST},
    /* TC_CONG_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* TC_DATA_EVT */
    {AVDT_SCB_DROP_PKT, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST},
    /* CC_CLOSE_EVT */
    {AVDT_SCB_IGNORE, AVDT_SCB_IGNORE, AVDT_SCB_CLOSING_ST}};

/* type for state table */
typedef const uint8_t (*tAVDT_SCB_ST_TBL)[AVDT_SCB_NUM_COLS];

/* state table */
const tAVDT_SCB_ST_TBL avdt_scb_st_tbl[] = {
    avdt_scb_st_idle, avdt_scb_st_conf,   avdt_scb_st_opening,
    avdt_scb_st_open, avdt_scb_st_stream, avdt_scb_st_closing};

/*******************************************************************************
 *
 * Function         avdt_scb_event
 *
 * Description      State machine event handling function for scb
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_event(AvdtpScb* p_scb, uint8_t event, tAVDT_SCB_EVT* p_data) {
  tAVDT_SCB_ST_TBL state_table;
  uint8_t action;
  int i;

#if (AVDT_DEBUG == TRUE)
  AVDT_TRACE_EVENT(
      "%s: SCB hdl=%d event=%d/%s state=%s p_avdt_scb=%p scb_index=%d",
      __func__, avdt_scb_to_hdl(p_scb), event, avdt_scb_evt_str[event],
      avdt_scb_st_str[p_scb->state], p_scb, p_scb->stream_config.scb_index);
#endif
  /* set current event */
  p_scb->curr_evt = event;

  /* look up the state table for the current state */
  state_table = avdt_scb_st_tbl[p_scb->state];

  /* set next state */
  if (p_scb->state != state_table[event][AVDT_SCB_NEXT_STATE]) {
    p_scb->state = state_table[event][AVDT_SCB_NEXT_STATE];
  }

  /* execute action functions */
  for (i = 0; i < AVDT_SCB_ACTIONS; i++) {
    action = state_table[event][i];
    if (action != AVDT_SCB_IGNORE) {
      (*avdtp_cb.p_scb_act[action])(p_scb, p_data);
    } else {
      break;
    }
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_init
 *
 * Description      Initialize stream control block module.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_init(void) {
  for (size_t i = 0; i < AVDT_NUM_LINKS; i++) {
    for (size_t j = 0; j < AVDT_NUM_SEPS; j++) {
      avdtp_cb.ccb[i].scb[j].Reset(0);
    }
  }

  avdtp_cb.p_scb_act = avdt_scb_action;
}

/*******************************************************************************
 *
 * Function         avdt_scb_alloc
 *
 * Description      Allocate a stream control block.
 *
 *
 * Returns          pointer to the scb, or NULL if none could be allocated.
 *
 ******************************************************************************/
AvdtpScb* avdt_scb_alloc(uint8_t peer_id,
                         const AvdtpStreamConfig& avdtp_stream_config) {
  CHECK(peer_id < AVDT_NUM_LINKS);

  // Find available entry
  AvdtpScb* p_scb = &avdtp_cb.ccb[peer_id].scb[0];
  for (int i = 0; i < AVDT_NUM_SEPS; i++, p_scb++) {
    if (!p_scb->allocated) {
      p_scb->Allocate(&avdtp_cb.ccb[peer_id], avdtp_stream_config);
      AVDT_TRACE_DEBUG("%s: allocated (handle=%d, psc_mask:0x%x)", __func__,
                       p_scb->ScbHandle(), avdtp_stream_config.cfg.psc_mask);
      return p_scb;
    }
  }

  AVDT_TRACE_WARNING("%s: out of AvdtScb entries for peer_id %d", __func__,
                     peer_id);
  return nullptr;
}

void AvdtpScb::Allocate(AvdtpCcb* p_avdtp_ccb,
                        const AvdtpStreamConfig& avdtp_stream_config) {
  uint8_t scb_handle = avdtp_cb.ComputeScbHandle(this);
  Reset(scb_handle);
  p_ccb = p_avdtp_ccb;
  stream_config = avdtp_stream_config;
  transport_channel_timer = alarm_new("avdtp_scb.transport_channel_timer");
  allocated = true;
}

/*******************************************************************************
 *
 * Function         avdt_scb_dealloc
 *
 * Description      Deallocate a stream control block.
 *
 *
 * Returns          void.
 *
 ******************************************************************************/
void avdt_scb_dealloc(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  AVDT_TRACE_DEBUG("%s: hdl=%d", __func__, avdt_scb_to_hdl(p_scb));
  p_scb->Recycle();
}

/*******************************************************************************
 *
 * Function         avdt_scb_to_hdl
 *
 * Description      Given a pointer to an scb, return its handle (or seid).
 *
 *
 * Returns          Index of scb.
 *
 ******************************************************************************/
uint8_t avdt_scb_to_hdl(AvdtpScb* p_scb) { return p_scb->ScbHandle(); }

/*******************************************************************************
 *
 * Function         avdt_scb_by_hdl
 *
 * Description      Given an scb handle (or seid), return a pointer to the scb.
 *
 *
 * Returns          Pointer to scb or NULL if index is out of range or scb
 *                  is not allocated.
 *
 ******************************************************************************/
AvdtpScb* avdt_scb_by_hdl(uint8_t hdl) {
  // Verify the index
  if ((hdl < 1) || (hdl > AVDT_NUM_LINKS * AVDT_NUM_SEPS)) {
    AVDT_TRACE_WARNING("%s: SCB handle %d out of range", __func__, hdl);
    return nullptr;
  }

  uint8_t index = hdl - 1;
  size_t i = index / AVDT_NUM_SEPS;
  size_t j = index % AVDT_NUM_SEPS;

  AvdtpScb* p_scb = &avdtp_cb.ccb[i].scb[j];
  // Verify the whether the scb is allocated
  if (!p_scb->allocated) {
    AVDT_TRACE_WARNING("%s: SCB handle %d not allocated", __func__, hdl);
    return nullptr;
  }

  AVDT_TRACE_DEBUG("%s: SCB for handle %d found: p_scb=%p scb_index=%d",
                   __func__, hdl, p_scb, p_scb->stream_config.scb_index);
  return p_scb;
}

/*******************************************************************************
 *
 * Function         avdt_scb_verify
 *
 * Description      Verify the condition of a list of scbs.
 *
 *
 * Returns          SEID that failed, or 0 if success.
 *
 ******************************************************************************/
uint8_t avdt_scb_verify(AvdtpCcb* p_ccb, uint8_t state, uint8_t* p_seid,
                        uint16_t num_seid, uint8_t* p_err_code) {
  int i;
  AvdtpScb* p_scb;
  uint8_t nsc_mask;
  uint8_t ret = 0;

  AVDT_TRACE_DEBUG("avdt_scb_verify state %d", state);
  /* set nonsupported command mask */
  /* translate public state into private state */
  nsc_mask = 0;
  if (state == AVDT_VERIFY_SUSPEND) {
    nsc_mask = AvdtpStreamConfig::AVDT_NSC_SUSPEND;
  }

  /* verify every scb */
  for (i = 0, *p_err_code = 0;
       (i < num_seid) && (*p_err_code == 0) && (i < AVDT_NUM_SEPS); i++) {
    p_scb = avdt_scb_by_hdl(p_seid[i]);
    if (p_scb == NULL)
      *p_err_code = AVDT_ERR_BAD_STATE;
    else if (p_scb->p_ccb != p_ccb)
      *p_err_code = AVDT_ERR_BAD_STATE;
    else if (p_scb->stream_config.nsc_mask & nsc_mask)
      *p_err_code = AVDT_ERR_NSC;

    switch (state) {
      case AVDT_VERIFY_OPEN:
      case AVDT_VERIFY_START:
        if (p_scb->state != AVDT_SCB_OPEN_ST &&
            p_scb->state != AVDT_SCB_STREAM_ST)
          *p_err_code = AVDT_ERR_BAD_STATE;
        break;

      case AVDT_VERIFY_SUSPEND:
      case AVDT_VERIFY_STREAMING:
        if (p_scb->state != AVDT_SCB_STREAM_ST)
          *p_err_code = AVDT_ERR_BAD_STATE;
        break;
    }
  }

  if ((i != num_seid) && (i < AVDT_NUM_SEPS)) {
    ret = p_seid[i];
  }

  return ret;
}

/*******************************************************************************
 *
 * Function         avdt_scb_peer_seid_list
 *
 * Description      Given a list of SCB handles, return a list of peer SEIDs
 *                  for the handles, copied in place into the struct passed in.
 *
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_peer_seid_list(tAVDT_MULTI* p_multi) {
  int i;
  AvdtpScb* p_scb;

  for (i = 0; i < p_multi->num_seps; i++) {
    p_scb = avdt_scb_by_hdl(p_multi->seid_list[i]);
    if (p_scb != NULL) {
      p_multi->seid_list[i] = p_scb->peer_seid;
    }
  }
}
