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
 *  This is the main implementation file for the BTA audio gateway.
 *
 ******************************************************************************/

#include <stack/include/bt_types.h>
#include <string.h>

#include "bta_ag_int.h"
#include "bta_api.h"
#include "bta_sys.h"
#include "osi/include/osi.h"
#include "utl.h"

/*****************************************************************************
 * Constants and types
 ****************************************************************************/
/* state machine states */
enum { BTA_AG_INIT_ST, BTA_AG_OPENING_ST, BTA_AG_OPEN_ST, BTA_AG_CLOSING_ST };

/* state machine action enumeration list */
enum {
  BTA_AG_REGISTER,
  BTA_AG_DEREGISTER,
  BTA_AG_START_OPEN,
  BTA_AG_RFC_DO_OPEN,
  BTA_AG_RFC_DO_CLOSE,
  BTA_AG_START_DEREG,
  BTA_AG_START_CLOSE,
  BTA_AG_RFC_OPEN,
  BTA_AG_OPEN_FAIL,
  BTA_AG_RFC_ACP_OPEN,
  BTA_AG_RFC_CLOSE,
  BTA_AG_RFC_FAIL,
  BTA_AG_RFC_DATA,
  BTA_AG_DISC_INT_RES,
  BTA_AG_DISC_FAIL,
  BTA_AG_DISC_ACP_RES,
  BTA_AG_FREE_DB,
  BTA_AG_SCO_CONN_OPEN,
  BTA_AG_SCO_CONN_CLOSE,
  BTA_AG_SCO_LISTEN,
  BTA_AG_SCO_OPEN,
  BTA_AG_SCO_CLOSE,
  BTA_AG_SCO_SHUTDOWN,
  BTA_AG_POST_SCO_OPEN,
  BTA_AG_POST_SCO_CLOSE,
  BTA_AG_SVC_CONN_OPEN,
  BTA_AG_RESULT,
  BTA_AG_SETCODEC,
  BTA_AG_SEND_RING,
  BTA_AG_HANDLE_COLLISION,
  BTA_AG_NUM_ACTIONS
};

#define BTA_AG_IGNORE BTA_AG_NUM_ACTIONS

/* type for action functions */
typedef void (*tBTA_AG_ACTION)(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data);

#define CASE_RETURN_STR(const) \
  case const:                  \
    return #const;

static const char* bta_ag_res_str(tBTA_AG_RES result) {
  switch (result) {
    CASE_RETURN_STR(BTA_AG_SPK_RES)
    CASE_RETURN_STR(BTA_AG_MIC_RES)
    CASE_RETURN_STR(BTA_AG_INBAND_RING_RES)
    CASE_RETURN_STR(BTA_AG_CIND_RES)
    CASE_RETURN_STR(BTA_AG_BINP_RES)
    CASE_RETURN_STR(BTA_AG_IND_RES)
    CASE_RETURN_STR(BTA_AG_BVRA_RES)
    CASE_RETURN_STR(BTA_AG_CNUM_RES)
    CASE_RETURN_STR(BTA_AG_BTRH_RES)
    CASE_RETURN_STR(BTA_AG_CLCC_RES)
    CASE_RETURN_STR(BTA_AG_COPS_RES)
    CASE_RETURN_STR(BTA_AG_IN_CALL_RES)
    CASE_RETURN_STR(BTA_AG_IN_CALL_CONN_RES)
    CASE_RETURN_STR(BTA_AG_CALL_WAIT_RES)
    CASE_RETURN_STR(BTA_AG_OUT_CALL_ORIG_RES)
    CASE_RETURN_STR(BTA_AG_OUT_CALL_ALERT_RES)
    CASE_RETURN_STR(BTA_AG_OUT_CALL_CONN_RES)
    CASE_RETURN_STR(BTA_AG_CALL_CANCEL_RES)
    CASE_RETURN_STR(BTA_AG_END_CALL_RES)
    CASE_RETURN_STR(BTA_AG_IN_CALL_HELD_RES)
    CASE_RETURN_STR(BTA_AG_UNAT_RES)
    CASE_RETURN_STR(BTA_AG_MULTI_CALL_RES)
    CASE_RETURN_STR(BTA_AG_BIND_RES)
    CASE_RETURN_STR(BTA_AG_IND_RES_ON_DEMAND)
    default:
      return "Unknown AG Result";
  }
}

static const char* bta_ag_evt_str(uint16_t event) {
  switch (event) {
    CASE_RETURN_STR(BTA_AG_API_REGISTER_EVT)
    CASE_RETURN_STR(BTA_AG_API_DEREGISTER_EVT)
    CASE_RETURN_STR(BTA_AG_API_OPEN_EVT)
    CASE_RETURN_STR(BTA_AG_API_CLOSE_EVT)
    CASE_RETURN_STR(BTA_AG_API_AUDIO_OPEN_EVT)
    CASE_RETURN_STR(BTA_AG_API_AUDIO_CLOSE_EVT)
    CASE_RETURN_STR(BTA_AG_API_RESULT_EVT)
    CASE_RETURN_STR(BTA_AG_API_SETCODEC_EVT)
    CASE_RETURN_STR(BTA_AG_RFC_OPEN_EVT)
    CASE_RETURN_STR(BTA_AG_RFC_CLOSE_EVT)
    CASE_RETURN_STR(BTA_AG_RFC_SRV_CLOSE_EVT)
    CASE_RETURN_STR(BTA_AG_RFC_DATA_EVT)
    CASE_RETURN_STR(BTA_AG_SCO_OPEN_EVT)
    CASE_RETURN_STR(BTA_AG_SCO_CLOSE_EVT)
    CASE_RETURN_STR(BTA_AG_DISC_ACP_RES_EVT)
    CASE_RETURN_STR(BTA_AG_DISC_INT_RES_EVT)
    CASE_RETURN_STR(BTA_AG_DISC_OK_EVT)
    CASE_RETURN_STR(BTA_AG_DISC_FAIL_EVT)
    CASE_RETURN_STR(BTA_AG_RING_TIMEOUT_EVT)
    CASE_RETURN_STR(BTA_AG_SVC_TIMEOUT_EVT)
    CASE_RETURN_STR(BTA_AG_COLLISION_EVT)
    default:
      return "Unknown AG Event";
  }
}

static const char* bta_ag_state_str(uint8_t state) {
  switch (state) {
    CASE_RETURN_STR(BTA_AG_INIT_ST)
    CASE_RETURN_STR(BTA_AG_OPENING_ST)
    CASE_RETURN_STR(BTA_AG_OPEN_ST)
    CASE_RETURN_STR(BTA_AG_CLOSING_ST)
    default:
      return "Unknown AG State";
  }
}

/* action functions */
const tBTA_AG_ACTION bta_ag_action[] = {
    bta_ag_register,       bta_ag_deregister,    bta_ag_start_open,
    bta_ag_rfc_do_open,    bta_ag_rfc_do_close,  bta_ag_start_dereg,
    bta_ag_start_close,    bta_ag_rfc_open,      bta_ag_open_fail,
    bta_ag_rfc_acp_open,   bta_ag_rfc_close,     bta_ag_rfc_fail,
    bta_ag_rfc_data,       bta_ag_disc_int_res,  bta_ag_disc_fail,
    bta_ag_disc_acp_res,   bta_ag_free_db,       bta_ag_sco_conn_open,
    bta_ag_sco_conn_close, bta_ag_sco_listen,    bta_ag_sco_open,
    bta_ag_sco_close,      bta_ag_sco_shutdown,  bta_ag_post_sco_open,
    bta_ag_post_sco_close, bta_ag_svc_conn_open, bta_ag_result,
    bta_ag_setcodec,       bta_ag_send_ring,     bta_ag_handle_collision};

static_assert(sizeof(bta_ag_action) / sizeof(tBTA_AG_ACTION) ==
                  BTA_AG_NUM_ACTIONS,
              "bta_ag_action must handle all actions");

/* state table information */
#define BTA_AG_ACTIONS 2    /* number of actions */
#define BTA_AG_NEXT_STATE 2 /* position of next state */
#define BTA_AG_NUM_COLS 3   /* number of columns in state tables */

/* state table for init state */
const uint8_t bta_ag_st_init[][BTA_AG_NUM_COLS] = {
    /* Event                    Action 1                Action 2 Next state */
    /* API_REGISTER_EVT */ {BTA_AG_REGISTER, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_DEREGISTER_EVT */ {BTA_AG_DEREGISTER, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_OPEN_EVT */ {BTA_AG_START_OPEN, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_AUDIO_OPEN_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_AUDIO_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_RESULT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* API_SETCODEC_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_OPEN_EVT */ {BTA_AG_RFC_ACP_OPEN, BTA_AG_SCO_LISTEN, BTA_AG_OPEN_ST},
    /* RFC_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_SRV_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_DATA_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* SCO_OPEN_EVT */ {BTA_AG_SCO_CONN_OPEN, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* SCO_CLOSE_EVT */ {BTA_AG_SCO_CONN_CLOSE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* DISC_ACP_RES_EVT */ {BTA_AG_FREE_DB, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* DISC_INT_RES_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* DISC_OK_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* DISC_FAIL_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RING_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* SVC_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* COLLISION_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_INIT_ST}};

/* state table for opening state */
const uint8_t bta_ag_st_opening[][BTA_AG_NUM_COLS] = {
    /* Event                    Action 1                Action 2 Next state */
    /* API_REGISTER_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_DEREGISTER_EVT */
    {BTA_AG_RFC_DO_CLOSE, BTA_AG_START_DEREG, BTA_AG_CLOSING_ST},
    /* API_OPEN_EVT */ {BTA_AG_OPEN_FAIL, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_CLOSE_EVT */ {BTA_AG_RFC_DO_CLOSE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_AUDIO_OPEN_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_AUDIO_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_RESULT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* API_SETCODEC_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* RFC_OPEN_EVT */ {BTA_AG_RFC_OPEN, BTA_AG_SCO_LISTEN, BTA_AG_OPEN_ST},
    /* RFC_CLOSE_EVT */ {BTA_AG_RFC_FAIL, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_SRV_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* RFC_DATA_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* SCO_OPEN_EVT */ {BTA_AG_SCO_CONN_OPEN, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* SCO_CLOSE_EVT */
    {BTA_AG_SCO_CONN_CLOSE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* DISC_ACP_RES_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* DISC_INT_RES_EVT */
    {BTA_AG_DISC_INT_RES, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* DISC_OK_EVT */ {BTA_AG_RFC_DO_OPEN, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* DISC_FAIL_EVT */ {BTA_AG_DISC_FAIL, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RING_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* SVC_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPENING_ST},
    /* COLLISION_EVT */
    {BTA_AG_HANDLE_COLLISION, BTA_AG_IGNORE, BTA_AG_INIT_ST}};

/* state table for open state */
const uint8_t bta_ag_st_open[][BTA_AG_NUM_COLS] = {
    /* Event                    Action 1                Action 2 Next state */
    /* API_REGISTER_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* API_DEREGISTER_EVT */
    {BTA_AG_START_CLOSE, BTA_AG_START_DEREG, BTA_AG_CLOSING_ST},
    /* API_OPEN_EVT */ {BTA_AG_OPEN_FAIL, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* API_CLOSE_EVT */ {BTA_AG_START_CLOSE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_AUDIO_OPEN_EVT */ {BTA_AG_SCO_OPEN, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* API_AUDIO_CLOSE_EVT */ {BTA_AG_SCO_CLOSE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* API_RESULT_EVT */ {BTA_AG_RESULT, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* API_SETCODEC_EVT */ {BTA_AG_SETCODEC, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* RFC_OPEN_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* RFC_CLOSE_EVT */ {BTA_AG_RFC_CLOSE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_SRV_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* RFC_DATA_EVT */ {BTA_AG_RFC_DATA, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* SCO_OPEN_EVT */
    {BTA_AG_SCO_CONN_OPEN, BTA_AG_POST_SCO_OPEN, BTA_AG_OPEN_ST},
    /* SCO_CLOSE_EVT */
    {BTA_AG_SCO_CONN_CLOSE, BTA_AG_POST_SCO_CLOSE, BTA_AG_OPEN_ST},
    /* DISC_ACP_RES_EVT */ {BTA_AG_DISC_ACP_RES, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* DISC_INT_RES_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* DISC_OK_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* DISC_FAIL_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* RING_TOUT_EVT */ {BTA_AG_SEND_RING, BTA_AG_IGNORE, BTA_AG_OPEN_ST},
    /* SVC_TOUT_EVT */ {BTA_AG_START_CLOSE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* COLLISION_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_OPEN_ST}};

/* state table for closing state */
const uint8_t bta_ag_st_closing[][BTA_AG_NUM_COLS] = {
    /* Event                    Action 1                Action 2 Next state */
    /* API_REGISTER_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_DEREGISTER_EVT */
    {BTA_AG_START_DEREG, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_OPEN_EVT */ {BTA_AG_OPEN_FAIL, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_AUDIO_OPEN_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_AUDIO_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_RESULT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* API_SETCODEC_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* RFC_OPEN_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* RFC_CLOSE_EVT */ {BTA_AG_RFC_CLOSE, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* RFC_SRV_CLOSE_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* RFC_DATA_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* SCO_OPEN_EVT */ {BTA_AG_SCO_CONN_OPEN, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* SCO_CLOSE_EVT */
    {BTA_AG_SCO_CONN_CLOSE, BTA_AG_POST_SCO_CLOSE, BTA_AG_CLOSING_ST},
    /* DISC_ACP_RES_EVT */ {BTA_AG_FREE_DB, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* DISC_INT_RES_EVT */ {BTA_AG_FREE_DB, BTA_AG_IGNORE, BTA_AG_INIT_ST},
    /* DISC_OK_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* DISC_FAIL_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* RING_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* SVC_TOUT_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST},
    /* COLLISION_EVT */ {BTA_AG_IGNORE, BTA_AG_IGNORE, BTA_AG_CLOSING_ST}};

constexpr size_t BTA_AG_NUM_EVENTS =
    BTA_AG_MAX_EVT - BTA_SYS_EVT_START(BTA_ID_AG);
static_assert(sizeof(bta_ag_st_init) / BTA_AG_NUM_COLS == BTA_AG_NUM_EVENTS,
              "bta_ag_st_init must handle all AG events");
static_assert(sizeof(bta_ag_st_opening) / BTA_AG_NUM_COLS == BTA_AG_NUM_EVENTS,
              "bta_ag_st_opening must handle all AG events");
static_assert(sizeof(bta_ag_st_open) / BTA_AG_NUM_COLS == BTA_AG_NUM_EVENTS,
              "bta_ag_st_open must handle all AG events");
static_assert(sizeof(bta_ag_st_closing) / BTA_AG_NUM_COLS == BTA_AG_NUM_EVENTS,
              "bta_ag_st_closing must handle all AG events");

/* type for state table */
typedef const uint8_t (*tBTA_AG_ST_TBL)[BTA_AG_NUM_COLS];

/* state table */
const tBTA_AG_ST_TBL bta_ag_st_tbl[] = {bta_ag_st_init, bta_ag_st_opening,
                                        bta_ag_st_open, bta_ag_st_closing};

/*****************************************************************************
 * Global data
 ****************************************************************************/

/* AG control block */
tBTA_AG_CB bta_ag_cb;
const tBTA_AG_DATA tBTA_AG_DATA::kEmpty = {};

/*******************************************************************************
 *
 * Function         bta_ag_scb_alloc
 *
 * Description      Allocate an AG service control block.
 *
 *
 * Returns          pointer to the scb, or NULL if none could be allocated.
 *
 ******************************************************************************/
static tBTA_AG_SCB* bta_ag_scb_alloc(void) {
  tBTA_AG_SCB* p_scb = &bta_ag_cb.scb[0];
  int i;

  for (i = 0; i < BTA_AG_MAX_NUM_CLIENTS; i++, p_scb++) {
    if (!p_scb->in_use) {
      /* initialize variables */
      p_scb->in_use = true;
      p_scb->sco_idx = BTM_INVALID_SCO_INDEX;
      p_scb->received_at_bac = false;
      p_scb->codec_updated = false;
      p_scb->codec_fallback = false;
      p_scb->peer_codecs = BTA_AG_CODEC_CVSD;
      p_scb->sco_codec = BTA_AG_CODEC_CVSD;
      p_scb->peer_version = HFP_HSP_VERSION_UNKNOWN;
      p_scb->hsp_version = HSP_VERSION_1_2;
      p_scb->peer_sdp_features = 0;
      /* set up timers */
      p_scb->ring_timer = alarm_new("bta_ag.scb_ring_timer");
      p_scb->collision_timer = alarm_new("bta_ag.scb_collision_timer");
      p_scb->codec_negotiation_timer =
          alarm_new("bta_ag.scb_codec_negotiation_timer");
      /* set eSCO mSBC setting to T2 as the preferred */
      p_scb->codec_msbc_settings = BTA_AG_SCO_MSBC_SETTINGS_T2;
      APPL_TRACE_DEBUG("bta_ag_scb_alloc %d", bta_ag_scb_to_idx(p_scb));
      break;
    }
  }

  if (i == BTA_AG_MAX_NUM_CLIENTS) {
    /* out of scbs */
    p_scb = nullptr;
    APPL_TRACE_WARNING("%s: Out of scbs", __func__);
  }
  return p_scb;
}

/*******************************************************************************
 *
 * Function         bta_ag_scb_dealloc
 *
 * Description      Deallocate a service control block.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_scb_dealloc(tBTA_AG_SCB* p_scb) {
  uint8_t idx;
  bool allocated = false;

  APPL_TRACE_DEBUG("bta_ag_scb_dealloc %d", bta_ag_scb_to_idx(p_scb));

  /* stop and free timers */
  alarm_free(p_scb->ring_timer);
  alarm_free(p_scb->codec_negotiation_timer);
  alarm_free(p_scb->collision_timer);

  /* initialize control block */
  *p_scb = {};
  p_scb->sco_idx = BTM_INVALID_SCO_INDEX;

  /* If all scbs are deallocated, callback with disable event */
  if (!bta_sys_is_register(BTA_ID_AG)) {
    for (idx = 0; idx < BTA_AG_MAX_NUM_CLIENTS; idx++) {
      if (bta_ag_cb.scb[idx].in_use) {
        allocated = true;
        break;
      }
    }

    if (!allocated) {
      (*bta_ag_cb.p_cback)(BTA_AG_DISABLE_EVT, nullptr);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_scb_to_idx
 *
 * Description      Given a pointer to an scb, return its index.
 *
 *
 * Returns          Index of scb starting from 1
 *
 ******************************************************************************/
uint16_t bta_ag_scb_to_idx(tBTA_AG_SCB* p_scb) {
  /* use array arithmetic to determine index */
  return static_cast<uint16_t>(p_scb - bta_ag_cb.scb + 1);
}

/*******************************************************************************
 *
 * Function         bta_ag_scb_by_idx
 *
 * Description      Given an scb index return pointer to scb.
 *
 *
 * Returns          Pointer to scb or NULL if not allocated.
 *
 ******************************************************************************/
tBTA_AG_SCB* bta_ag_scb_by_idx(uint16_t idx) {
  tBTA_AG_SCB* p_scb;

  /* verify index */
  if (idx > 0 && idx <= BTA_AG_MAX_NUM_CLIENTS) {
    p_scb = &bta_ag_cb.scb[idx - 1];
    if (!p_scb->in_use) {
      p_scb = nullptr;
      APPL_TRACE_WARNING("ag scb idx %d not allocated", idx);
    }
  } else {
    p_scb = nullptr;
    APPL_TRACE_DEBUG("ag scb idx %d out of range", idx);
  }
  return p_scb;
}

/*******************************************************************************
 *
 * Function         bta_ag_service_to_idx
 *
 * Description      Given a BTA service mask convert to profile index.
 *
 *
 * Returns          Profile ndex of scb.
 *
 ******************************************************************************/
uint8_t bta_ag_service_to_idx(tBTA_SERVICE_MASK services) {
  if (services & BTA_HFP_SERVICE_MASK) {
    return BTA_AG_HFP;
  } else {
    return BTA_AG_HSP;
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_idx_by_bdaddr
 *
 * Description      Find SCB associated with peer BD address.
 *
 *
 * Returns          Index of SCB or zero if none found.
 *
 ******************************************************************************/
uint16_t bta_ag_idx_by_bdaddr(const RawAddress* peer_addr) {
  tBTA_AG_SCB* p_scb = &bta_ag_cb.scb[0];
  if (peer_addr != nullptr) {
    for (uint16_t i = 0; i < BTA_AG_MAX_NUM_CLIENTS; i++, p_scb++) {
      if (p_scb->in_use && *peer_addr == p_scb->peer_addr) {
        return (i + 1);
      }
    }
  }

  /* no scb found */
  APPL_TRACE_WARNING("No ag scb for peer addr");
  return 0;
}

/*******************************************************************************
 *
 * Function         bta_ag_other_scb_open
 *
 * Description      Check whether any other scb is in open state.
 *
 *
 * Returns          true if another scb is in open state, false otherwise.
 *
 ******************************************************************************/
bool bta_ag_other_scb_open(tBTA_AG_SCB* p_curr_scb) {
  tBTA_AG_SCB* p_scb = &bta_ag_cb.scb[0];
  for (int i = 0; i < BTA_AG_MAX_NUM_CLIENTS; i++, p_scb++) {
    if (p_scb->in_use && p_scb != p_curr_scb &&
        p_scb->state == BTA_AG_OPEN_ST) {
      return true;
    }
  }
  /* no other scb found */
  APPL_TRACE_DEBUG("No other ag scb open");
  return false;
}

/*******************************************************************************
 *
 * Function         bta_ag_scb_open
 *
 * Description      Check whether given scb is in open state.
 *
 *
 * Returns          true if scb is in open state, false otherwise.
 *
 ******************************************************************************/
bool bta_ag_scb_open(tBTA_AG_SCB* p_curr_scb) {
  return p_curr_scb && p_curr_scb->in_use &&
         p_curr_scb->state == BTA_AG_OPEN_ST;
}

/*******************************************************************************
 *
 * Function         bta_ag_collision_cback
 *
 * Description      Get notified about collision.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_collision_cback(UNUSED_ATTR tBTA_SYS_CONN_STATUS status, uint8_t id,
                            UNUSED_ATTR uint8_t app_id,
                            const RawAddress& peer_addr) {
  /* Check if we have opening scb for the peer device. */
  uint16_t handle = bta_ag_idx_by_bdaddr(&peer_addr);
  tBTA_AG_SCB* p_scb = bta_ag_scb_by_idx(handle);

  if (p_scb && (p_scb->state == BTA_AG_OPENING_ST)) {
    if (id == BTA_ID_SYS) {
      LOG(WARNING) << __func__ << ": AG found collision (ACL) for handle "
                   << unsigned(handle) << " device " << peer_addr;
    } else if (id == BTA_ID_AG) {
      LOG(WARNING) << __func__ << ": AG found collision (RFCOMM) for handle "
                   << unsigned(handle) << " device " << peer_addr;
    } else {
      LOG(WARNING) << __func__ << ": AG found collision (UNKNOWN) for handle "
                   << unsigned(handle) << " device " << peer_addr;
    }
    bta_ag_sm_execute(p_scb, BTA_AG_COLLISION_EVT, tBTA_AG_DATA::kEmpty);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_resume_open
 *
 * Description      Resume opening process.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_resume_open(tBTA_AG_SCB* p_scb) {
  if (p_scb->state == BTA_AG_INIT_ST) {
    LOG(INFO) << __func__ << ": Resume connection to " << p_scb->peer_addr
              << ", handle" << bta_ag_scb_to_idx(p_scb);
    tBTA_AG_DATA open_data = {.api_open.bd_addr = p_scb->peer_addr,
                              .api_open.sec_mask = p_scb->cli_sec_mask};
    bta_ag_sm_execute(p_scb, BTA_AG_API_OPEN_EVT, open_data);
  } else {
    VLOG(1) << __func__ << ": device " << p_scb->peer_addr
            << " is already in state " << std::to_string(p_scb->state);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_api_enable
 *
 * Description      Handle an API enable event.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_api_enable(tBTA_AG_CBACK* p_cback) {
  /* initialize control block */
  for (tBTA_AG_SCB& scb : bta_ag_cb.scb) {
    alarm_free(scb.ring_timer);
    alarm_free(scb.codec_negotiation_timer);
    alarm_free(scb.collision_timer);
    scb = {};
  }

  /* store callback function */
  bta_ag_cb.p_cback = p_cback;

  /* call init call-out */
  BTM_WriteVoiceSettings(AG_VOICE_SETTINGS);

  bta_sys_collision_register(BTA_ID_AG, bta_ag_collision_cback);

  /* call callback with enable event */
  (*bta_ag_cb.p_cback)(BTA_AG_ENABLE_EVT, nullptr);
}

/*******************************************************************************
 *
 * Function         bta_ag_api_disable
 *
 * Description      Handle an API disable event.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_api_disable() {
  /* deregister all scbs in use */
  tBTA_AG_SCB* p_scb = &bta_ag_cb.scb[0];
  bool do_dereg = false;
  int i;

  if (!bta_sys_is_register(BTA_ID_AG)) {
    APPL_TRACE_ERROR("BTA AG is already disabled, ignoring ...");
    return;
  }

  /* De-register with BTA system manager */
  bta_sys_deregister(BTA_ID_AG);

  for (i = 0; i < BTA_AG_MAX_NUM_CLIENTS; i++, p_scb++) {
    if (p_scb->in_use) {
      bta_ag_sm_execute(p_scb, BTA_AG_API_DEREGISTER_EVT, tBTA_AG_DATA::kEmpty);
      do_dereg = true;
    }
  }

  if (!do_dereg) {
    /* Done, send callback evt to app */
    (*bta_ag_cb.p_cback)(BTA_AG_DISABLE_EVT, nullptr);
  }

  bta_sys_collision_register(BTA_ID_AG, nullptr);
}

/*******************************************************************************
 *
 * Function         bta_ag_api_register
 *
 * Description      Handle an API event registers a new service.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_api_register(tBTA_SERVICE_MASK services, tBTA_SEC sec_mask,
                         tBTA_AG_FEAT features,
                         const std::vector<std::string>& service_names,
                         uint8_t app_id) {
  tBTA_AG_SCB* p_scb = bta_ag_scb_alloc();
  if (p_scb) {
    APPL_TRACE_DEBUG("bta_ag_api_register: p_scb 0x%08x ", p_scb);
    tBTA_AG_DATA data = {};
    data.api_register.features = features;
    data.api_register.sec_mask = sec_mask;
    data.api_register.services = services;
    data.api_register.app_id = app_id;
    for (int i = 0; i < BTA_AG_NUM_IDX; i++) {
      if (!service_names[i].empty()) {
        strlcpy(data.api_register.p_name[i], service_names[i].c_str(),
                BTA_SERVICE_NAME_LEN);
      } else {
        data.api_register.p_name[i][0] = 0;
      }
    }
    bta_ag_sm_execute(p_scb, BTA_AG_API_REGISTER_EVT, data);
  } else {
    tBTA_AG bta_ag = {};
    bta_ag.reg.status = BTA_AG_FAIL_RESOURCES;
    (*bta_ag_cb.p_cback)(BTA_AG_REGISTER_EVT, &bta_ag);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_api_result
 *
 * Description      Handle an API result event.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_api_result(uint16_t handle, tBTA_AG_RES result,
                       const tBTA_AG_RES_DATA& result_data) {
  tBTA_AG_DATA event_data = {};
  event_data.api_result.result = result;
  event_data.api_result.data = result_data;
  tBTA_AG_SCB* p_scb;
  if (handle != BTA_AG_HANDLE_ALL) {
    p_scb = bta_ag_scb_by_idx(handle);
    if (p_scb) {
      APPL_TRACE_DEBUG("bta_ag_api_result: p_scb 0x%08x ", p_scb);
      bta_ag_sm_execute(p_scb, static_cast<uint16_t>(BTA_AG_API_RESULT_EVT),
                        event_data);
    }
  } else {
    int i;
    for (i = 0, p_scb = &bta_ag_cb.scb[0]; i < BTA_AG_MAX_NUM_CLIENTS;
         i++, p_scb++) {
      if (p_scb->in_use && p_scb->svc_conn) {
        APPL_TRACE_DEBUG("bta_ag_api_result p_scb 0x%08x ", p_scb);
        bta_ag_sm_execute(p_scb, static_cast<uint16_t>(BTA_AG_API_RESULT_EVT),
                          event_data);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_sm_execute
 *
 * Description      State machine event handling function for AG
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_sm_execute(tBTA_AG_SCB* p_scb, uint16_t event,
                       const tBTA_AG_DATA& data) {
  tBTA_AG_ST_TBL state_table;
  uint8_t action;
  int i;
  uint16_t previous_event = event;
  uint8_t previous_state = p_scb->state;

  APPL_TRACE_EVENT(
      "%s: handle=0x%04x, bd_addr=%s, state=%s(0x%02x), "
      "event=%s(0x%04x), result=%s(0x%02x)",
      __func__, bta_ag_scb_to_idx(p_scb), p_scb->peer_addr.ToString().c_str(),
      bta_ag_state_str(p_scb->state), p_scb->state, bta_ag_evt_str(event),
      event, bta_ag_res_str(data.api_result.result), data.api_result.result);

  event &= 0x00FF;
  if (event >= (BTA_AG_MAX_EVT & 0x00FF)) {
    APPL_TRACE_ERROR("%s: event out of range, ignored", __func__);
    return;
  }

  /* look up the state table for the current state */
  state_table = bta_ag_st_tbl[p_scb->state];

  /* set next state */
  p_scb->state = state_table[event][BTA_AG_NEXT_STATE];

  /* execute action functions */
  for (i = 0; i < BTA_AG_ACTIONS; i++) {
    action = state_table[event][i];
    if (action != BTA_AG_IGNORE) {
      (*bta_ag_action[action])(p_scb, data);
    } else {
      break;
    }
  }
  if (p_scb->state != previous_state) {
    APPL_TRACE_EVENT(
        "%s: handle=0x%04x, bd_addr=%s, state_change[%s(0x%02x)]->[%s(0x%02x)],"
        " event[%s(0x%04x)], result[%s(0x%02x)]",
        __func__, bta_ag_scb_to_idx(p_scb), p_scb->peer_addr.ToString().c_str(),
        bta_ag_state_str(previous_state), previous_state,
        bta_ag_state_str(p_scb->state), p_scb->state,
        bta_ag_evt_str(previous_event), previous_event,
        bta_ag_res_str(data.api_result.result), data.api_result.result);
  }
}

void bta_ag_sm_execute_by_handle(uint16_t handle, uint16_t event,
                                 const tBTA_AG_DATA& data) {
  tBTA_AG_SCB* p_scb = bta_ag_scb_by_idx(handle);
  if (p_scb) {
    APPL_TRACE_DEBUG("%s: p_scb 0x%08x ", __func__, p_scb);
    bta_ag_sm_execute(p_scb, event, data);
  }
}

/**
 * Handles event from bta_sys_sendmsg(). It is here to support legacy alarm
 * implementation that is mainly for timeouts.
 *
 * @param p_msg event message
 * @return True to free p_msg, or False if p_msg is freed within this function
 */
bool bta_ag_hdl_event(BT_HDR* p_msg) {
  switch (p_msg->event) {
    case BTA_AG_RING_TIMEOUT_EVT:
    case BTA_AG_SVC_TIMEOUT_EVT:
      bta_ag_sm_execute_by_handle(p_msg->layer_specific, p_msg->event,
                                  tBTA_AG_DATA::kEmpty);
      break;
    default:
      LOG(FATAL) << __func__ << ": bad event " << p_msg->event
                 << " layer_specific=" << p_msg->layer_specific;
      break;
  }
  return true;
}
