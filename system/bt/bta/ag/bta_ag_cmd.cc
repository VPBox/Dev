/******************************************************************************
 *
 *  Copyright 2004-2012 Broadcom Corporation
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

#define LOG_TAG "bta_ag_cmd"

#include <cctype>
#include <cstdio>
#include <cstring>

#include "bt_common.h"
#include "bt_target.h"
#include "bt_types.h"
#include "bta_ag_api.h"
#include "bta_ag_at.h"
#include "bta_ag_int.h"
#include "bta_api.h"
#include "bta_sys.h"
#include "log/log.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "port_api.h"
#include "utl.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/

/* Ring timeout */
#define BTA_AG_RING_TIMEOUT_MS (5 * 1000) /* 5 seconds */

#define BTA_AG_CMD_MAX_VAL 32767 /* Maximum value is signed 16-bit value */

/* Invalid Chld command */
#define BTA_AG_INVALID_CHLD 255

#define COLON_IDX_4_VGSVGM 4

/* Local events which will not trigger a higher layer callback */
enum {
  BTA_AG_LOCAL_EVT_FIRST = 0x100,
  BTA_AG_LOCAL_EVT_CCWA,
  BTA_AG_LOCAL_EVT_CLIP,
  BTA_AG_LOCAL_EVT_CMER,
  BTA_AG_LOCAL_EVT_BRSF,
  BTA_AG_LOCAL_EVT_CMEE,
  BTA_AG_LOCAL_EVT_BCC,
};

/* AT command interpreter table for HSP */
const tBTA_AG_AT_CMD bta_ag_hsp_cmd[] = {
    {"+CKPD", BTA_AG_AT_CKPD_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 200, 200},
    {"+VGS", BTA_AG_SPK_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 15},
    {"+VGM", BTA_AG_MIC_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 15},
    /* End-of-table marker used to stop lookup iteration */
    {"", 0, 0, 0, 0, 0}};

/* AT command interpreter table for HFP */
const tBTA_AG_AT_CMD bta_ag_hfp_cmd[] = {
    {"A", BTA_AG_AT_A_EVT, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"D", BTA_AG_AT_D_EVT, BTA_AG_AT_NONE | BTA_AG_AT_FREE, BTA_AG_AT_STR, 0,
     0},
    {"+VGS", BTA_AG_SPK_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 15},
    {"+VGM", BTA_AG_MIC_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 15},
    {"+CCWA", BTA_AG_LOCAL_EVT_CCWA, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 1},
    /* Consider CHLD as str to take care of indexes for ECC */
    {"+CHLD", BTA_AG_AT_CHLD_EVT, BTA_AG_AT_SET | BTA_AG_AT_TEST, BTA_AG_AT_STR,
     0, 4},
    {"+CHUP", BTA_AG_AT_CHUP_EVT, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"+CIND", BTA_AG_AT_CIND_EVT, BTA_AG_AT_READ | BTA_AG_AT_TEST,
     BTA_AG_AT_STR, 0, 0},
    {"+CLIP", BTA_AG_LOCAL_EVT_CLIP, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 1},
    {"+CMER", BTA_AG_LOCAL_EVT_CMER, BTA_AG_AT_SET, BTA_AG_AT_STR, 0, 0},
    {"+VTS", BTA_AG_AT_VTS_EVT, BTA_AG_AT_SET, BTA_AG_AT_STR, 0, 0},
    {"+BINP", BTA_AG_AT_BINP_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 1, 1},
    {"+BLDN", BTA_AG_AT_BLDN_EVT, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"+BVRA", BTA_AG_AT_BVRA_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 1},
    {"+BRSF", BTA_AG_LOCAL_EVT_BRSF, BTA_AG_AT_SET, BTA_AG_AT_INT, 0,
     BTA_AG_CMD_MAX_VAL},
    {"+NREC", BTA_AG_AT_NREC_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 0},
    {"+CNUM", BTA_AG_AT_CNUM_EVT, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"+BTRH", BTA_AG_AT_BTRH_EVT, BTA_AG_AT_READ | BTA_AG_AT_SET, BTA_AG_AT_INT,
     0, 2},
    {"+CLCC", BTA_AG_AT_CLCC_EVT, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"+COPS", BTA_AG_AT_COPS_EVT, BTA_AG_AT_READ | BTA_AG_AT_SET, BTA_AG_AT_STR,
     0, 0},
    {"+CMEE", BTA_AG_LOCAL_EVT_CMEE, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 1},
    {"+BIA", BTA_AG_AT_BIA_EVT, BTA_AG_AT_SET, BTA_AG_AT_STR, 0, 20},
    {"+CBC", BTA_AG_AT_CBC_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0, 100},
    {"+BCC", BTA_AG_LOCAL_EVT_BCC, BTA_AG_AT_NONE, BTA_AG_AT_STR, 0, 0},
    {"+BCS", BTA_AG_AT_BCS_EVT, BTA_AG_AT_SET, BTA_AG_AT_INT, 0,
     BTA_AG_CMD_MAX_VAL},
    {"+BIND", BTA_AG_AT_BIND_EVT,
     BTA_AG_AT_SET | BTA_AG_AT_READ | BTA_AG_AT_TEST, BTA_AG_AT_STR, 0, 0},
    {"+BIEV", BTA_AG_AT_BIEV_EVT, BTA_AG_AT_SET, BTA_AG_AT_STR, 0, 0},
    {"+BAC", BTA_AG_AT_BAC_EVT, BTA_AG_AT_SET, BTA_AG_AT_STR, 0, 0},
    /* End-of-table marker used to stop lookup iteration */
    {"", 0, 0, 0, 0, 0}};

/* AT result code table element */
typedef struct {
  const char* result_string; /* AT result string */
  size_t result_id;          /* Local or BTA result id */
  uint8_t arg_type;          /* whether argument is int or string */
} tBTA_AG_RESULT;

/* AT result code argument types */
enum {
  BTA_AG_RES_FMT_NONE, /* no argument */
  BTA_AG_RES_FMT_INT,  /* integer argument */
  BTA_AG_RES_FMT_STR   /* string argument */
};

/* Local AT command result codes not defined in bta_ag_api.h */
enum {
  BTA_AG_LOCAL_RES_FIRST = 0x0100,
  BTA_AG_LOCAL_RES_OK,
  BTA_AG_LOCAL_RES_ERROR,
  BTA_AG_LOCAL_RES_RING,
  BTA_AG_LOCAL_RES_CLIP,
  BTA_AG_LOCAL_RES_BRSF,
  BTA_AG_LOCAL_RES_CMEE,
  BTA_AG_LOCAL_RES_BCS
};

/* AT result code constant table */
const tBTA_AG_RESULT bta_ag_result_tbl[] = {
    {"OK", BTA_AG_LOCAL_RES_OK, BTA_AG_RES_FMT_NONE},
    {"ERROR", BTA_AG_LOCAL_RES_ERROR, BTA_AG_RES_FMT_NONE},
    {"RING", BTA_AG_LOCAL_RES_RING, BTA_AG_RES_FMT_NONE},
    {"+VGS: ", BTA_AG_SPK_RES, BTA_AG_RES_FMT_INT},
    {"+VGM: ", BTA_AG_MIC_RES, BTA_AG_RES_FMT_INT},
    {"+CCWA: ", BTA_AG_CALL_WAIT_RES, BTA_AG_RES_FMT_STR},
    {"+CHLD: ", BTA_AG_IN_CALL_HELD_RES, BTA_AG_RES_FMT_STR},
    {"+CIND: ", BTA_AG_CIND_RES, BTA_AG_RES_FMT_STR},
    {"+CLIP: ", BTA_AG_LOCAL_RES_CLIP, BTA_AG_RES_FMT_STR},
    {"+CIEV: ", BTA_AG_IND_RES, BTA_AG_RES_FMT_STR},
    {"+BINP: ", BTA_AG_BINP_RES, BTA_AG_RES_FMT_STR},
    {"+BVRA: ", BTA_AG_BVRA_RES, BTA_AG_RES_FMT_INT},
    {"+BRSF: ", BTA_AG_LOCAL_RES_BRSF, BTA_AG_RES_FMT_INT},
    {"+BSIR: ", BTA_AG_INBAND_RING_RES, BTA_AG_RES_FMT_INT},
    {"+CNUM: ", BTA_AG_CNUM_RES, BTA_AG_RES_FMT_STR},
    {"+BTRH: ", BTA_AG_BTRH_RES, BTA_AG_RES_FMT_INT},
    {"+CLCC: ", BTA_AG_CLCC_RES, BTA_AG_RES_FMT_STR},
    {"+COPS: ", BTA_AG_COPS_RES, BTA_AG_RES_FMT_STR},
    {"+CME ERROR: ", BTA_AG_LOCAL_RES_CMEE, BTA_AG_RES_FMT_INT},
    {"+BCS: ", BTA_AG_LOCAL_RES_BCS, BTA_AG_RES_FMT_INT},
    {"+BIND: ", BTA_AG_BIND_RES, BTA_AG_RES_FMT_STR},
    {"", BTA_AG_UNAT_RES, BTA_AG_RES_FMT_STR}};

static const tBTA_AG_RESULT* bta_ag_result_by_code(size_t code) {
  for (size_t i = 0;
       i != sizeof(bta_ag_result_tbl) / sizeof(bta_ag_result_tbl[0]); ++i) {
    if (code == bta_ag_result_tbl[i].result_id) return &bta_ag_result_tbl[i];
  }
  return nullptr;
}

const tBTA_AG_AT_CMD* bta_ag_at_tbl[BTA_AG_NUM_IDX] = {bta_ag_hsp_cmd,
                                                       bta_ag_hfp_cmd};

typedef struct {
  size_t result_code;
  size_t indicator;
} tBTA_AG_INDICATOR_MAP;

/* callsetup indicator value lookup table */
const tBTA_AG_INDICATOR_MAP callsetup_indicator_map[] = {
    {BTA_AG_IN_CALL_RES, BTA_AG_CALLSETUP_INCOMING},
    {BTA_AG_CALL_WAIT_RES, BTA_AG_CALLSETUP_INCOMING},
    {BTA_AG_OUT_CALL_ORIG_RES, BTA_AG_CALLSETUP_OUTGOING},
    {BTA_AG_OUT_CALL_ALERT_RES, BTA_AG_CALLSETUP_ALERTING}};

static size_t bta_ag_indicator_by_result_code(size_t code) {
  for (size_t i = 0;
       i !=
       sizeof(callsetup_indicator_map) / sizeof(callsetup_indicator_map[0]);
       ++i) {
    if (code == callsetup_indicator_map[i].result_code)
      return callsetup_indicator_map[i].indicator;
  }
  return BTA_AG_CALLSETUP_NONE;
}

/*******************************************************************************
 *
 * Function         bta_ag_send_result
 *
 * Description      Send an AT result code.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_send_result(tBTA_AG_SCB* p_scb, size_t code,
                               const char* p_arg, int16_t int_arg) {
  const tBTA_AG_RESULT* result = bta_ag_result_by_code(code);
  if (result == nullptr) {
    LOG_ERROR(LOG_TAG, "%s Unable to lookup result for code %zu", __func__,
              code);
    return;
  }

  char buf[BTA_AG_AT_MAX_LEN + 16] = "";
  char* p = buf;

  /* init with \r\n */
  *p++ = '\r';
  *p++ = '\n';

  /* copy result code string */
  strlcpy(p, result->result_string, sizeof(buf) - 2);

  if (p_scb->conn_service == BTA_AG_HSP) {
    /* If HSP then ":"symbol should be changed as "=" for HSP compatibility */
    switch (code) {
      case BTA_AG_SPK_RES:
      case BTA_AG_MIC_RES:
        if (*(p + COLON_IDX_4_VGSVGM) == ':') {
          *(p + COLON_IDX_4_VGSVGM) = '=';
        }
        break;
    }
  }

  p += strlen(result->result_string);

  /* copy argument if any */
  if (result->arg_type == BTA_AG_RES_FMT_INT) {
    p += utl_itoa((uint16_t)int_arg, p);
  } else if (result->arg_type == BTA_AG_RES_FMT_STR) {
    strcpy(p, p_arg);
    p += strlen(p_arg);
  }

  /* finish with \r\n */
  *p++ = '\r';
  *p++ = '\n';

  /* send to RFCOMM */
  uint16_t len = 0;
  PORT_WriteData(p_scb->conn_handle, buf, (uint16_t)(p - buf), &len);
}

/*******************************************************************************
 *
 * Function         bta_ag_send_ok
 *
 * Description      Send an OK result code.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_send_ok(tBTA_AG_SCB* p_scb) {
  bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_OK, nullptr, 0);
}

/*******************************************************************************
 *
 * Function         bta_ag_send_error
 *
 * Description      Send an ERROR result code.
 *                      errcode - used to send verbose errocode
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_send_error(tBTA_AG_SCB* p_scb, int16_t errcode) {
  /* If HFP and extended audio gateway error codes are enabled */
  if (p_scb->conn_service == BTA_AG_HFP && p_scb->cmee_enabled)
    bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_CMEE, nullptr, errcode);
  else
    bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_ERROR, nullptr, 0);
}

/*******************************************************************************
 *
 * Function         bta_ag_send_ind
 *
 * Description      Send an indicator CIEV result code.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_send_ind(tBTA_AG_SCB* p_scb, uint16_t id, uint16_t value,
                            bool on_demand) {
  char str[12];
  char* p = str;

  /* If the indicator is masked out, just return */
  /* Mandatory indicators can not be masked out. */
  if ((p_scb->bia_masked_out & ((uint32_t)1 << id)) &&
      ((id != BTA_AG_IND_CALL) && (id != BTA_AG_IND_CALLSETUP) &&
       (id != BTA_AG_IND_CALLHELD)))
    return;

  /* Ensure we do not send duplicate indicators if not requested by app */
  /* If it was requested by app, transmit CIEV even if it is duplicate. */
  if (id == BTA_AG_IND_CALL) {
    if ((value == p_scb->call_ind) && (!on_demand)) return;

    p_scb->call_ind = (uint8_t)value;
  }

  if ((id == BTA_AG_IND_CALLSETUP) && (!on_demand)) {
    if (value == p_scb->callsetup_ind) return;

    p_scb->callsetup_ind = (uint8_t)value;
  }

  if ((id == BTA_AG_IND_SERVICE) && (!on_demand)) {
    if (value == p_scb->service_ind) return;

    p_scb->service_ind = (uint8_t)value;
  }
  if ((id == BTA_AG_IND_SIGNAL) && (!on_demand)) {
    if (value == p_scb->signal_ind) return;

    p_scb->signal_ind = (uint8_t)value;
  }
  if ((id == BTA_AG_IND_ROAM) && (!on_demand)) {
    if (value == p_scb->roam_ind) return;

    p_scb->roam_ind = (uint8_t)value;
  }
  if ((id == BTA_AG_IND_BATTCHG) && (!on_demand)) {
    if (value == p_scb->battchg_ind) return;

    p_scb->battchg_ind = (uint8_t)value;
  }

  if ((id == BTA_AG_IND_CALLHELD) && (!on_demand)) {
    /* call swap could result in sending callheld=1 multiple times */
    if ((value != 1) && (value == p_scb->callheld_ind)) return;

    p_scb->callheld_ind = (uint8_t)value;
  }

  if (p_scb->cmer_enabled) {
    p += utl_itoa(id, p);
    *p++ = ',';
    utl_itoa(value, p);
    bta_ag_send_result(p_scb, BTA_AG_IND_RES, str, 0);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_parse_cmer
 *
 * Description      Parse AT+CMER parameter string.
 *
 *
 * Returns          true if parsed ok, false otherwise.
 *
 ******************************************************************************/
static bool bta_ag_parse_cmer(char* p_s, char* p_end, bool* p_enabled) {
  int16_t n[4] = {-1, -1, -1, -1};
  int i;
  char* p;

  for (i = 0; i < 4; i++, p_s = p + 1) {
    /* skip to comma delimiter */
    for (p = p_s; p < p_end && *p != ',' && *p != 0; p++)
      ;

    /* get integer value */
    if (p > p_end) {
      android_errorWriteLog(0x534e4554, "112860487");
      return false;
    }
    *p = 0;
    n[i] = utl_str2int(p_s);
  }

  /* process values */
  if (n[0] < 0 || n[3] < 0) {
    return false;
  }

  if ((n[0] == 3) && ((n[3] == 1) || (n[3] == 0))) {
    *p_enabled = (bool)n[3];
  }

  return true;
}

/*******************************************************************************
 *
 * Function         bta_ag_parse_chld
 *
 * Description      Parse AT+CHLD parameter string.
 *
 *
 * Returns          Returns idx (1-7), 0 if ECC not enabled or
 BTA_AG_INVALID_CHLD
                    if idx doesn't exist/1st character of argument is not a
 digit
 *
 ******************************************************************************/
static uint8_t bta_ag_parse_chld(UNUSED_ATTR tBTA_AG_SCB* p_scb, char* p_s) {
  uint8_t retval = 0;

  if (!isdigit(p_s[0])) {
    return BTA_AG_INVALID_CHLD;
  }

  if (p_s[1] != 0) {
    /* p_idxstr++;  point to beginning of call number */
    int16_t idx = utl_str2int(&p_s[1]);
    if (idx != -1 && idx < 255) {
      retval = (uint8_t)idx;
    } else {
      retval = BTA_AG_INVALID_CHLD;
    }
  }

  return (retval);
}

/*******************************************************************************
 *
 * Function         bta_ag_parse_bac
 *
 * Description      Parse AT+BAC parameter string.
 *
 * Returns          Returns bitmap of supported codecs.
 *
 ******************************************************************************/
static tBTA_AG_PEER_CODEC bta_ag_parse_bac(tBTA_AG_SCB* p_scb, char* p_s,
                                           char* p_end) {
  tBTA_AG_PEER_CODEC retval = BTA_AG_CODEC_NONE;
  uint16_t uuid_codec;
  char* p;

  while (p_s) {
    /* skip to comma delimiter */
    for (p = p_s; p < p_end && *p != ',' && *p != 0; p++)
      ;

    /* get integer value */
    if (p > p_end) {
      android_errorWriteLog(0x534e4554, "112860487");
      break;
    }
    bool cont = false;  // Continue processing
    if (*p != 0) {
      *p = 0;
      cont = true;
    }
    uuid_codec = utl_str2int(p_s);
    switch (uuid_codec) {
      case UUID_CODEC_CVSD:
        retval |= BTA_AG_CODEC_CVSD;
        break;
      case UUID_CODEC_MSBC:
        retval |= BTA_AG_CODEC_MSBC;
        break;
      default:
        APPL_TRACE_ERROR("Unknown Codec UUID(%d) received", uuid_codec);
        break;
    }

    if (cont)
      p_s = p + 1;
    else
      break;
  }

  return (retval);
}

/*******************************************************************************
 *
 * Function         bta_ag_process_unat_res
 *
 * Description      Process the unat response data and remove extra carriage
 *                  return and line feed
 *
 *
 * Returns          void
 *
 ******************************************************************************/

static void bta_ag_process_unat_res(char* unat_result) {
  uint8_t j = 0;
  uint8_t pairs_of_nl_cr;
  char trim_data[BTA_AG_AT_MAX_LEN];

  uint8_t str_leng = strlen(unat_result);

  /* If no extra CR and LF, just return */
  if (str_leng < 4) return;

  /* Remove the carriage return and left feed */
  while (unat_result[0] == '\r' && unat_result[1] == '\n' &&
         unat_result[str_leng - 2] == '\r' &&
         unat_result[str_leng - 1] == '\n') {
    pairs_of_nl_cr = 1;
    for (int i = 0; i < (str_leng - 4 * pairs_of_nl_cr); i++) {
      trim_data[j++] = unat_result[i + pairs_of_nl_cr * 2];
    }
    /* Add EOF */
    trim_data[j] = '\0';
    str_leng = str_leng - 4;
    strlcpy(unat_result, trim_data, str_leng + 1);
    j = 0;

    if (str_leng < 4) return;
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_inband_enabled
 *
 * Description      Determine whether in-band ring can be used.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
bool bta_ag_inband_enabled(tBTA_AG_SCB* p_scb) {
  /* if feature is enabled and no other scbs connected */
  return p_scb->inband_enabled && !bta_ag_other_scb_open(p_scb);
}

/*******************************************************************************
 *
 * Function         bta_ag_send_call_inds
 *
 * Description      Send call and callsetup indicators.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_send_call_inds(tBTA_AG_SCB* p_scb, tBTA_AG_RES result) {
  uint8_t call;

  /* set new call and callsetup values based on BTA_AgResult */
  size_t callsetup = bta_ag_indicator_by_result_code(result);

  if (result == BTA_AG_END_CALL_RES) {
    call = BTA_AG_CALL_INACTIVE;
  } else if (result == BTA_AG_IN_CALL_CONN_RES ||
             result == BTA_AG_OUT_CALL_CONN_RES ||
             result == BTA_AG_IN_CALL_HELD_RES) {
    call = BTA_AG_CALL_ACTIVE;
  } else {
    call = p_scb->call_ind;
  }

  /* Send indicator function tracks if the values have actually changed */
  bta_ag_send_ind(p_scb, BTA_AG_IND_CALL, call, false);
  bta_ag_send_ind(p_scb, BTA_AG_IND_CALLSETUP, callsetup, false);
}

/*******************************************************************************
 *
 * Function         bta_ag_at_hsp_cback
 *
 * Description      AT command processing callback for HSP.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_at_hsp_cback(tBTA_AG_SCB* p_scb, uint16_t command_id,
                         uint8_t arg_type, char* p_arg, char* p_end,
                         int16_t int_arg) {
  APPL_TRACE_DEBUG("AT cmd:%d arg_type:%d arg:%d arg:%s", command_id, arg_type,
                   int_arg, p_arg);

  bta_ag_send_ok(p_scb);

  tBTA_AG_VAL val = {};
  val.hdr.handle = bta_ag_scb_to_idx(p_scb);
  val.hdr.app_id = p_scb->app_id;
  val.num = (uint16_t)int_arg;

  if ((p_end - p_arg + 1) >= (long)sizeof(val.str)) {
    APPL_TRACE_ERROR("%s: p_arg is too long, send error and return", __func__);
    bta_ag_send_error(p_scb, BTA_AG_ERR_TEXT_TOO_LONG);
    android_errorWriteLog(0x534e4554, "112860487");
    return;
  }
  strlcpy(val.str, p_arg, sizeof(val.str));

  /* call callback with event */
  (*bta_ag_cb.p_cback)(command_id, (tBTA_AG*)&val);
}

static void remove_spaces(char* str) {
  char* dest_str = str;

  while (*str) {
    if (*str == ' ') {
      str++;
    } else {
      *dest_str++ = *str++;
    }
  }
  *dest_str = '\0';
}

/*******************************************************************************
 *
 * Function         bta_ag_find_empty_hf_ind)
 *
 * Description      This function returns the index of an empty HF indicator
 *                  structure.
 *
 * Returns          int : index of the empty HF indicator structure or
 *                            -1 if no empty indicator
 *                            is available.
 *
 ******************************************************************************/
static int bta_ag_find_empty_hf_ind(tBTA_AG_SCB* p_scb) {
  for (int index = 0; index < BTA_AG_MAX_NUM_PEER_HF_IND; index++) {
    if (p_scb->peer_hf_indicators[index].ind_id == 0) return index;
  }

  return -1;
}

/*******************************************************************************
 *
 * Function         bta_ag_find_hf_ind_by_id
 *
 * Description      This function returns the index of the HF indicator
 *                  structure by the indicator id
 *
 * Returns          int : index of the HF indicator structure
 *                            -1 if the indicator
 *                            was not found.
 *
 ******************************************************************************/
static int bta_ag_find_hf_ind_by_id(tBTA_AG_HF_IND* p_hf_ind, int size,
                                    uint32_t ind_id) {
  for (int index = 0; index < size; index++) {
    if (p_hf_ind[index].ind_id == ind_id) return index;
  }

  return -1;
}

/*******************************************************************************
 *
 * Function         bta_ag_parse_bind_set
 *
 * Description      Parse AT+BIND set command and save the indicators
 *
 * Returns          true if successful
 *
 ******************************************************************************/
static bool bta_ag_parse_bind_set(tBTA_AG_SCB* p_scb, tBTA_AG_VAL val) {
  char* p_token = strtok(val.str, ",");
  if (p_token == nullptr) return false;

  while (p_token != nullptr) {
    uint16_t rcv_ind_id = atoi(p_token);
    int index = bta_ag_find_empty_hf_ind(p_scb);
    if (index == -1) {
      APPL_TRACE_WARNING("%s Can't save more indicators", __func__);
      return false;
    }

    p_scb->peer_hf_indicators[index].ind_id = rcv_ind_id;
    APPL_TRACE_DEBUG("%s peer_hf_ind[%d] = %d", __func__, index, rcv_ind_id);

    p_token = strtok(nullptr, ",");
  }

  return true;
}

/*******************************************************************************
 *
 * Function         bta_ag_bind_response
 *
 * Description      Send response for the AT+BIND command (HFP 1.7) received
 *                  from the headset based on the argument types.
 *
 * Returns          Void
 *
 ******************************************************************************/
static void bta_ag_bind_response(tBTA_AG_SCB* p_scb, uint8_t arg_type) {
  char buffer[BTA_AG_AT_MAX_LEN] = "";

  if (arg_type == BTA_AG_AT_TEST) {
    int index = 0;
    buffer[index++] = '(';

    for (uint32_t i = 0; i < bta_ag_local_hf_ind_cfg[0].ind_id; i++) {
      if (bta_ag_local_hf_ind_cfg[i + 1].is_supported) {
        /* Add ',' from second indicator */
        if (index > 1) buffer[index++] = ',';
        snprintf(&buffer[index++], 2, "%d",
                 bta_ag_local_hf_ind_cfg[i + 1].ind_id);
      }
    }

    buffer[index++] = ')';

    bta_ag_send_result(p_scb, BTA_AG_BIND_RES, buffer, 0);
    bta_ag_send_ok(p_scb);
  } else if (arg_type == BTA_AG_AT_READ) {
    char* p = buffer;

    /* bta_ag_local_hf_ind_cfg[0].ind_id is used as BTA_AG_NUM_LOCAL_HF_IND */
    for (uint32_t i = 0; i < bta_ag_local_hf_ind_cfg[0].ind_id; i++) {
      if (i == BTA_AG_MAX_NUM_LOCAL_HF_IND) {
        APPL_TRACE_WARNING("%s No space for more HF indicators", __func__);
        break;
      }

      p_scb->local_hf_indicators[i].ind_id =
          bta_ag_local_hf_ind_cfg[i + 1].ind_id;
      p_scb->local_hf_indicators[i].is_supported =
          bta_ag_local_hf_ind_cfg[i + 1].is_supported;
      p_scb->local_hf_indicators[i].is_enable =
          bta_ag_local_hf_ind_cfg[i + 1].is_enable;

      int peer_index = bta_ag_find_hf_ind_by_id(
          p_scb->peer_hf_indicators, BTA_AG_MAX_NUM_PEER_HF_IND,
          p_scb->local_hf_indicators[i].ind_id);

      /* Check whether local and peer sides support this indicator */
      if (p_scb->local_hf_indicators[i].is_supported && peer_index != -1) {
        /* In the format of ind, state */
        p += utl_itoa((uint16_t)p_scb->local_hf_indicators[i].ind_id, p);
        *p++ = ',';
        p += utl_itoa((uint16_t)p_scb->local_hf_indicators[i].is_enable, p);

        bta_ag_send_result(p_scb, BTA_AG_BIND_RES, buffer, 0);
        // have to use memset here because assigning to "" will not zero
        // initialize the rest of the buffer
        memset(buffer, 0, sizeof(buffer));
        p = buffer;
      } else {
        /* If indicator is not supported, also set it to disable */
        p_scb->local_hf_indicators[i].is_enable = false;
      }
    }

    bta_ag_send_ok(p_scb);

    /* If the service level connection wan't already open, now it's open */
    if (!p_scb->svc_conn) {
      bta_ag_svc_conn_open(p_scb, tBTA_AG_DATA::kEmpty);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_parse_biev_response
 *
 * Description      Send response for AT+BIEV command (HFP 1.7) received from
 *                  the headset based on the argument types.
 *
 * Returns          true if the response was parsed successfully
 *
 ******************************************************************************/
static bool bta_ag_parse_biev_response(tBTA_AG_SCB* p_scb, tBTA_AG_VAL* val) {
  char* p_token = strtok(val->str, ",");
  if (p_token == nullptr) return false;
  uint16_t rcv_ind_id = atoi(p_token);

  p_token = strtok(nullptr, ",");
  if (p_token == nullptr) return false;
  uint16_t rcv_ind_val = atoi(p_token);

  APPL_TRACE_DEBUG("%s BIEV indicator id %d, value %d", __func__, rcv_ind_id,
                   rcv_ind_val);

  /* Check whether indicator ID is valid or not */
  if (rcv_ind_id > BTA_AG_NUM_LOCAL_HF_IND) {
    APPL_TRACE_WARNING("%s received invalid indicator id %d", __func__,
                       rcv_ind_id);
    return false;
  }

  /* Check this indicator is support or not and enabled or not */
  int local_index = bta_ag_find_hf_ind_by_id(
      p_scb->local_hf_indicators, BTA_AG_MAX_NUM_LOCAL_HF_IND, rcv_ind_id);
  if (local_index == -1 ||
      !p_scb->local_hf_indicators[local_index].is_supported ||
      !p_scb->local_hf_indicators[local_index].is_enable) {
    APPL_TRACE_WARNING("%s indicator id %d not supported or disabled", __func__,
                       rcv_ind_id);
    return false;
  }

  /* For each indicator ID, check whether the indicator value is in range */
  if (rcv_ind_val < bta_ag_local_hf_ind_cfg[rcv_ind_id].ind_min_val ||
      rcv_ind_val > bta_ag_local_hf_ind_cfg[rcv_ind_id].ind_max_val) {
    APPL_TRACE_WARNING("%s invalid ind_val %d", __func__, rcv_ind_val);
    return false;
  }

  val->lidx = rcv_ind_id;
  val->num = rcv_ind_val;

  return true;
}

/*******************************************************************************
 *
 * Function         bta_ag_at_hfp_cback
 *
 * Description      AT command processing callback for HFP.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_at_hfp_cback(tBTA_AG_SCB* p_scb, uint16_t cmd, uint8_t arg_type,
                         char* p_arg, char* p_end, int16_t int_arg) {
  tBTA_AG_VAL val = {};
  tBTA_AG_SCB* ag_scb;
  uint32_t i, ind_id;
  uint32_t bia_masked_out;
  if (p_arg == nullptr) {
    APPL_TRACE_ERROR("%s: p_arg is null, send error and return", __func__);
    bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_TSTR);
    return;
  }

  APPL_TRACE_DEBUG("%s: AT command %d, arg_type %d, int_arg %d, arg %s",
                   __func__, cmd, arg_type, int_arg, p_arg);

  val.hdr.handle = bta_ag_scb_to_idx(p_scb);
  val.hdr.app_id = p_scb->app_id;
  val.hdr.status = BTA_AG_SUCCESS;
  val.num = static_cast<uint32_t>(int_arg);
  val.bd_addr = p_scb->peer_addr;

  if ((p_end - p_arg + 1) >= (long)sizeof(val.str)) {
    APPL_TRACE_ERROR("%s: p_arg is too long, send error and return", __func__);
    bta_ag_send_error(p_scb, BTA_AG_ERR_TEXT_TOO_LONG);
    android_errorWriteLog(0x534e4554, "112860487");
    return;
  }
  strlcpy(val.str, p_arg, sizeof(val.str));

  /**
   * Unless this this is a local event, by default we'll forward
   * the event code to the application.
   * If |event| is 0 at the end of this function, the application
   * callback is NOT invoked.
   */
  tBTA_AG_EVT event = 0;
  if (cmd < BTA_AG_LOCAL_EVT_FIRST) {
    event = static_cast<tBTA_AG_EVT>(cmd);
  }

  switch (cmd) {
    case BTA_AG_AT_A_EVT:
    case BTA_AG_SPK_EVT:
    case BTA_AG_MIC_EVT:
    case BTA_AG_AT_CBC_EVT:
      /* send OK */
      bta_ag_send_ok(p_scb);
      break;

    case BTA_AG_AT_CHUP_EVT:
      if (!bta_ag_sco_is_active_device(p_scb->peer_addr)) {
        LOG(WARNING) << __func__ << ": AT+CHUP rejected as " << p_scb->peer_addr
                << " is not the active device";
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_ALLOWED);
      } else {
        bta_ag_send_ok(p_scb);
      }
      break;

    case BTA_AG_AT_BLDN_EVT:
      /* Do not send OK, App will send error or OK depending on
      ** last dial number enabled or not */
      break;

    case BTA_AG_AT_D_EVT:
      /* Do not send OK for Dial cmds
      ** Let application decide whether to send OK or ERROR*/

      /* if mem dial cmd, make sure string contains only digits */
      if (val.str[0] == '>') {
        /* Some car kits may add some unwanted space characters in the
        ** input string. This workaround will trim the unwanted chars. */
        remove_spaces(val.str + 1);

        if (!utl_isintstr(val.str + 1)) {
          event = 0;
          bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_DSTR);
        }
      } else if (val.str[0] == 'V') /* ATDV : Dial VoIP Call */
      {
        /* We do not check string. Code will be added later if needed. */
        if (!((p_scb->peer_features & BTA_AG_PEER_FEAT_VOIP) &&
              (p_scb->features & BTA_AG_FEAT_VOIP))) {
          event = 0;
          bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
        }
      }
      /* If dial cmd, make sure string contains only dial digits
      ** Dial digits are 0-9, A-C, *, #, + */
      else {
        /* Some car kits may add some unwanted space characters in the
        ** input string. This workaround will trim the unwanted chars. */
        remove_spaces(val.str);

        if (!utl_isdialstr(val.str)) {
          event = 0;
          bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_DSTR);
        }
      }
      break;

    case BTA_AG_LOCAL_EVT_CCWA:
      /* store setting */
      p_scb->ccwa_enabled = (bool)int_arg;

      /* send OK */
      bta_ag_send_ok(p_scb);
      break;

    case BTA_AG_AT_CHLD_EVT:
      if (arg_type == BTA_AG_AT_TEST) {
        /* don't call callback */
        event = 0;

        /* send CHLD string */
        /* Form string based on supported 1.5 feature */
        if ((p_scb->peer_version >= HFP_VERSION_1_5) &&
            (p_scb->features & BTA_AG_FEAT_ECC) &&
            (p_scb->peer_features & BTA_AG_PEER_FEAT_ECC))
          bta_ag_send_result(p_scb, BTA_AG_IN_CALL_HELD_RES,
                             p_bta_ag_cfg->chld_val_ecc, 0);
        else
          bta_ag_send_result(p_scb, BTA_AG_IN_CALL_HELD_RES,
                             p_bta_ag_cfg->chld_val, 0);

        /* send OK */
        bta_ag_send_ok(p_scb);

        /* if service level conn. not already open, now it's open */
        bta_ag_svc_conn_open(p_scb, tBTA_AG_DATA::kEmpty);
      } else {
        val.idx = bta_ag_parse_chld(p_scb, val.str);

        if (val.idx == BTA_AG_INVALID_CHLD) {
          event = 0;
          bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
          break;
        }
        if (val.idx &&
            !((p_scb->features & BTA_AG_FEAT_ECC) &&
              (p_scb->peer_features & BTA_AG_PEER_FEAT_ECC))) {
          /* we do not support ECC, but HF is sending us a CHLD with call
           * index*/
          event = 0;
          bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);

        } else {
          /* If it is swap between calls, set call held indicator to 3(out of
          *valid 0-2)
          ** Application will set it back to 1
          ** callheld indicator will be sent across to the peer. */
          if (val.str[0] == '2') {
            for (i = 0, ag_scb = &bta_ag_cb.scb[0]; i < BTA_AG_MAX_NUM_CLIENTS;
                 i++, ag_scb++) {
              if (ag_scb->in_use) {
                if ((ag_scb->call_ind == BTA_AG_CALL_ACTIVE) &&
                    (ag_scb->callsetup_ind == BTA_AG_CALLSETUP_NONE))
                  ag_scb->callheld_ind = BTA_AG_CALLHELD_NOACTIVE + 1;
              }
            }
          }
        }

        /* Do not send OK. Let app decide after parsing the val str */
        /* bta_ag_send_ok(p_scb); */
      }
      break;

    case BTA_AG_AT_BIND_EVT:
      APPL_TRACE_DEBUG("%s BTA_AG_AT_BIND_EVT arg_type: %d", __func__,
                       arg_type);
      if (arg_type == BTA_AG_AT_SET) {
        if (bta_ag_parse_bind_set(p_scb, val)) {
          bta_ag_send_ok(p_scb);
        } else {
          event = 0; /* don't call callback */
          bta_ag_send_error(p_scb, BTA_AG_ERR_INVALID_INDEX);
        }
      } else {
        bta_ag_bind_response(p_scb, arg_type);

        /* Need not pass this command beyond BTIF.*/
        /* Stack handles it internally */
        event = 0; /* don't call callback */
      }
      break;

    case BTA_AG_AT_BIEV_EVT:
      if (bta_ag_parse_biev_response(p_scb, &val)) {
        bta_ag_send_ok(p_scb);
      } else {
        bta_ag_send_error(p_scb, BTA_AG_ERR_INVALID_INDEX);
        /* don't call callback receiving invalid indicator */
        event = 0;
      }
      break;

    case BTA_AG_AT_CIND_EVT:
      if (arg_type == BTA_AG_AT_TEST) {
        /* don't call callback */
        event = 0;

        /* send CIND string, send OK */
        bta_ag_send_result(p_scb, BTA_AG_CIND_RES, p_bta_ag_cfg->cind_info, 0);
        bta_ag_send_ok(p_scb);
      }
      break;

    case BTA_AG_LOCAL_EVT_CLIP:
      /* store setting, send OK */
      p_scb->clip_enabled = (bool)int_arg;
      bta_ag_send_ok(p_scb);
      break;

    case BTA_AG_LOCAL_EVT_CMER:
      /* if parsed ok store setting, send OK */
      if (bta_ag_parse_cmer(p_arg, p_end, &p_scb->cmer_enabled)) {
        bta_ag_send_ok(p_scb);

        /* if service level conn. not already open and our features and
        ** peer features do not have 3-way, service level conn. now open
        */
        if (!p_scb->svc_conn &&
            !((p_scb->features & BTA_AG_FEAT_3WAY) &&
              (p_scb->peer_features & BTA_AG_PEER_FEAT_3WAY))) {
          bta_ag_svc_conn_open(p_scb, tBTA_AG_DATA::kEmpty);
        }
      } else {
        bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_TSTR);
      }
      break;

    case BTA_AG_AT_VTS_EVT:
      /* check argument */
      if (strlen(p_arg) == 1) {
        bta_ag_send_ok(p_scb);
      } else {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_TSTR);
      }
      break;

    case BTA_AG_AT_BINP_EVT:
      /* if feature not set don't call callback, send ERROR */
      if (!(p_scb->features & BTA_AG_FEAT_VTAG)) {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      break;

    case BTA_AG_AT_BVRA_EVT:
      /* if feature not supported don't call callback, send ERROR. App will send
       * OK */
      if (!(p_scb->features & BTA_AG_FEAT_VREC)) {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      break;

    case BTA_AG_LOCAL_EVT_BRSF: {
      /* store peer features */
      p_scb->peer_features = (uint16_t)int_arg;

      tBTA_AG_FEAT features = p_scb->features;
      if (p_scb->peer_version < HFP_VERSION_1_7) {
        features &= HFP_1_6_FEAT_MASK;
      }

      APPL_TRACE_DEBUG("%s BRSF HF: 0x%x, phone: 0x%x", __func__,
                       p_scb->peer_features, features);

      /* send BRSF, send OK */
      bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_BRSF, nullptr,
                         (int16_t)features);
      bta_ag_send_ok(p_scb);
      break;
    }

    case BTA_AG_AT_NREC_EVT:
      /* if feature send OK, else don't call callback, send ERROR */
      if (p_scb->features & BTA_AG_FEAT_ECNR) {
        bta_ag_send_ok(p_scb);
      } else {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      break;

    case BTA_AG_AT_BTRH_EVT:
      /* if feature send BTRH, send OK:, else don't call callback, send ERROR */
      if (p_scb->features & BTA_AG_FEAT_BTRH) {
        /* If set command; send response and notify app */
        if (arg_type == BTA_AG_AT_SET) {
          for (i = 0, ag_scb = &bta_ag_cb.scb[0]; i < BTA_AG_MAX_NUM_CLIENTS;
               i++, ag_scb++) {
            if (ag_scb->in_use) {
              bta_ag_send_result(ag_scb, BTA_AG_BTRH_RES, nullptr, int_arg);
            }
          }
          bta_ag_send_ok(p_scb);
        } else /* Read Command */
        {
          val.num = BTA_AG_BTRH_READ;
        }
      } else {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      break;

    case BTA_AG_AT_COPS_EVT:
      if (arg_type == BTA_AG_AT_SET) {
        /* don't call callback */
        event = 0;

        /* send OK */
        bta_ag_send_ok(p_scb);
      }
      break;

    case BTA_AG_LOCAL_EVT_CMEE:
      if (p_scb->features & BTA_AG_FEAT_EXTERR) {
        /* store setting */
        p_scb->cmee_enabled = (bool)int_arg;

        /* send OK */
        bta_ag_send_ok(p_scb);
      } else {
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      /* don't call callback */
      event = 0;
      break;

    case BTA_AG_AT_BIA_EVT:
      bia_masked_out = p_scb->bia_masked_out;

      /* Parse the indicator mask */
      for (i = 0, ind_id = 1; (val.str[i] != 0) && (ind_id <= 20);
           i++, ind_id++) {
        if (val.str[i] == ',') {
          continue;
        }

        if (val.str[i] == '0') {
          bia_masked_out |= ((uint32_t)1 << ind_id);
        } else if (val.str[i] == '1') {
          bia_masked_out &= ~((uint32_t)1 << ind_id);
        } else {
          break;
        }

        i++;
        if (val.str[i] != ',') {
          break;
        }
      }
      if (val.str[i] == 0) {
        p_scb->bia_masked_out = bia_masked_out;
        val.num = bia_masked_out;
        bta_ag_send_ok(p_scb);
      } else {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_INVALID_INDEX);
      }
      break;

    case BTA_AG_AT_CNUM_EVT:
      break;

    case BTA_AG_AT_CLCC_EVT:
      if (!(p_scb->features & BTA_AG_FEAT_ECS)) {
        event = 0;
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      }
      break;

    case BTA_AG_AT_BAC_EVT:
      bta_ag_send_ok(p_scb);
      p_scb->received_at_bac = true;

      /* store available codecs from the peer */
      if ((p_scb->peer_features & BTA_AG_PEER_FEAT_CODEC) &&
          (p_scb->features & BTA_AG_FEAT_CODEC)) {
        p_scb->peer_codecs = bta_ag_parse_bac(p_scb, p_arg, p_end);
        p_scb->codec_updated = true;

        if (p_scb->peer_codecs & BTA_AG_CODEC_MSBC) {
          p_scb->sco_codec = UUID_CODEC_MSBC;
          APPL_TRACE_DEBUG("Received AT+BAC, updating sco codec to MSBC");
        } else {
          p_scb->sco_codec = UUID_CODEC_CVSD;
          APPL_TRACE_DEBUG("Received AT+BAC, updating sco codec to CVSD");
        }
        /* The above logic sets the stack preferred codec based on local and
        peer codec
        capabilities. This can be overridden by the application depending on its
        preference
        using the bta_ag_setcodec API. We send the peer_codecs to the
        application. */
        val.num = p_scb->peer_codecs;
        /* Received BAC while in codec negotiation. */
        if ((bta_ag_cb.sco.state == BTA_AG_SCO_CODEC_ST) &&
            (bta_ag_cb.sco.p_curr_scb == p_scb)) {
          bta_ag_codec_negotiate(p_scb);
        }
      } else {
        p_scb->peer_codecs = BTA_AG_CODEC_CVSD;
        APPL_TRACE_ERROR(
            "Unexpected CMD:AT+BAC, Codec Negotiation is not supported");
      }
      break;

    case BTA_AG_AT_BCS_EVT: {
      tBTA_AG_PEER_CODEC codec_type, codec_sent;
      bta_ag_send_ok(p_scb);
      alarm_cancel(p_scb->codec_negotiation_timer);

      switch (int_arg) {
        case UUID_CODEC_CVSD:
          codec_type = BTA_AG_CODEC_CVSD;
          break;
        case UUID_CODEC_MSBC:
          codec_type = BTA_AG_CODEC_MSBC;
          break;
        default:
          APPL_TRACE_ERROR("Unknown codec_uuid %d", int_arg);
          codec_type = 0xFFFF;
          break;
      }

      if (p_scb->codec_fallback)
        codec_sent = BTA_AG_CODEC_CVSD;
      else
        codec_sent = p_scb->sco_codec;

      bta_ag_sco_codec_nego(p_scb, codec_type == codec_sent);

      /* send final codec info to callback */
      val.num = codec_sent;
      break;
    }
    case BTA_AG_LOCAL_EVT_BCC: {
      if (!bta_ag_sco_is_active_device(p_scb->peer_addr)) {
        LOG(WARNING) << __func__ << ": AT+BCC rejected as " << p_scb->peer_addr
                     << " is not the active device";
        bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_ALLOWED);
        break;
      }
      bta_ag_send_ok(p_scb);
      bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
      break;
    }
    default:
      bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
      break;
  }

  /* call callback */
  if (event != 0) {
    (*bta_ag_cb.p_cback)(event, (tBTA_AG*)&val);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_at_err_cback
 *
 * Description      AT command parser error callback.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_at_err_cback(tBTA_AG_SCB* p_scb, bool unknown, const char* p_arg) {
  if (unknown && (!strlen(p_arg))) {
    APPL_TRACE_DEBUG("Empty AT cmd string received");
    bta_ag_send_ok(p_scb);
    return;
  }

  tBTA_AG_VAL val = {};
  /* if unknown AT command and configured to pass these to app */
  if (unknown && (p_scb->features & BTA_AG_FEAT_UNAT)) {
    val.hdr.handle = bta_ag_scb_to_idx(p_scb);
    val.hdr.app_id = p_scb->app_id;
    val.hdr.status = BTA_AG_SUCCESS;
    val.num = 0;
    strlcpy(val.str, p_arg, sizeof(val.str));
    (*bta_ag_cb.p_cback)(BTA_AG_AT_UNAT_EVT, (tBTA_AG*)&val);
  } else {
    bta_ag_send_error(p_scb, BTA_AG_ERR_OP_NOT_SUPPORTED);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_hsp_result
 *
 * Description      Handle API result for HSP connections.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_hsp_result(tBTA_AG_SCB* p_scb, const tBTA_AG_API_RESULT& result) {
  APPL_TRACE_DEBUG("bta_ag_hsp_result : res = %d", result.result);

  switch (result.result) {
    case BTA_AG_SPK_RES:
    case BTA_AG_MIC_RES:
      bta_ag_send_result(p_scb, result.result, nullptr, result.data.num);
      break;

    case BTA_AG_IN_CALL_RES:
      /* tell sys to stop av if any */
      bta_sys_sco_use(BTA_ID_AG, p_scb->app_id, p_scb->peer_addr);

      /* if sco already opened or no inband ring send ring now */
      if (bta_ag_sco_is_open(p_scb) || !bta_ag_inband_enabled(p_scb) ||
          (p_scb->features & BTA_AG_FEAT_NOSCO)) {
        bta_ag_send_ring(p_scb, tBTA_AG_DATA::kEmpty);
      } else {
        /* else open sco, send ring after sco opened */
        /* HSPv1.2: AG shall not send RING if using in-band ring tone. */
        if (p_scb->peer_version >= HSP_VERSION_1_2) {
          p_scb->post_sco = BTA_AG_POST_SCO_NONE;
        } else {
          p_scb->post_sco = BTA_AG_POST_SCO_RING;
        }
        bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
      }
      break;

    case BTA_AG_IN_CALL_CONN_RES:
    case BTA_AG_OUT_CALL_ORIG_RES:
      /* if incoming call connected stop ring timer */
      if (result.result == BTA_AG_IN_CALL_CONN_RES) {
        alarm_cancel(p_scb->ring_timer);
      }

      if (!(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        /* if audio connected to this scb AND sco is not opened, open sco */
        if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb) &&
            !bta_ag_sco_is_open(p_scb)) {
          bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
        } else if (result.data.audio_handle == BTA_AG_HANDLE_NONE &&
                   bta_ag_sco_is_open(p_scb)) {
          /* else if no audio at call close sco */
          bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
        }
      }
      break;

    case BTA_AG_END_CALL_RES:
      alarm_cancel(p_scb->ring_timer);

      /* close sco */
      if ((bta_ag_sco_is_open(p_scb) || bta_ag_sco_is_opening(p_scb)) &&
          !(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
      } else {
        /* if av got suspended by this call, let it resume. */
        bta_sys_sco_unuse(BTA_ID_AG, p_scb->app_id, p_scb->peer_addr);
      }
      break;

    case BTA_AG_INBAND_RING_RES:
      p_scb->inband_enabled = result.data.state;
      APPL_TRACE_DEBUG("inband_enabled set to %d", p_scb->inband_enabled);
      break;

    case BTA_AG_UNAT_RES:
      if (result.data.ok_flag != BTA_AG_OK_ERROR) {
        if (result.data.str[0] != 0) {
          bta_ag_send_result(p_scb, result.result, result.data.str, 0);
        }

        if (result.data.ok_flag == BTA_AG_OK_DONE) bta_ag_send_ok(p_scb);
      } else {
        bta_ag_send_error(p_scb, BTA_AG_ERR_INV_CHAR_IN_TSTR);
      }
      break;

    default:
      /* ignore all others */
      break;
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_hfp_result
 *
 * Description      Handle API result for HFP connections.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_hfp_result(tBTA_AG_SCB* p_scb, const tBTA_AG_API_RESULT& result) {
  APPL_TRACE_DEBUG("bta_ag_hfp_result : res = %d", result.result);

  switch (result.result) {
    case BTA_AG_SPK_RES:
    case BTA_AG_MIC_RES:
      bta_ag_send_result(p_scb, result.result, nullptr, result.data.num);
      break;

    case BTA_AG_IN_CALL_RES: {
      /* tell sys to stop av if any */
      bta_sys_sco_use(BTA_ID_AG, p_scb->app_id, p_scb->peer_addr);

      p_scb->clip[0] = 0;
      if (result.data.str[0] != 0) {
        snprintf(p_scb->clip, sizeof(p_scb->clip), "%s", result.data.str);
      }
      /* send callsetup indicator */
      if (p_scb->post_sco == BTA_AG_POST_SCO_CALL_END) {
        /* Need to sent 2 callsetup IND's(Call End and Incoming call) after SCO
         * close. */
        p_scb->post_sco = BTA_AG_POST_SCO_CALL_END_INCALL;
      } else {
        bta_ag_send_call_inds(p_scb, result.result);

        /* if sco already opened or no inband ring send ring now */
        if (bta_ag_sco_is_open(p_scb) || !bta_ag_inband_enabled(p_scb) ||
            (p_scb->features & BTA_AG_FEAT_NOSCO) ||
            (result.data.audio_handle != bta_ag_scb_to_idx(p_scb))) {
          bta_ag_send_ring(p_scb, tBTA_AG_DATA::kEmpty);
        } else {
          /* else open sco, send ring after sco opened */
          p_scb->post_sco = BTA_AG_POST_SCO_RING;
          bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
        }
      }
      break;
    }
    case BTA_AG_IN_CALL_CONN_RES:
      alarm_cancel(p_scb->ring_timer);

      /* if sco not opened and we need to open it, send indicators first
      ** then  open sco.
      */
      bta_ag_send_call_inds(p_scb, result.result);

      if (!(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb) &&
            !bta_ag_sco_is_open(p_scb)) {
          bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
        } else if ((result.data.audio_handle == BTA_AG_HANDLE_NONE) &&
                   bta_ag_sco_is_open(p_scb)) {
          bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
        }
      }
      break;

    case BTA_AG_IN_CALL_HELD_RES:
      alarm_cancel(p_scb->ring_timer);

      bta_ag_send_call_inds(p_scb, result.result);

      break;

    case BTA_AG_OUT_CALL_ORIG_RES:
      bta_ag_send_call_inds(p_scb, result.result);
      if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb) &&
          !(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
      }
      break;

    case BTA_AG_OUT_CALL_ALERT_RES:
      /* send indicators */
      bta_ag_send_call_inds(p_scb, result.result);
      if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb) &&
          !(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
      }
      break;

    case BTA_AG_MULTI_CALL_RES:
      /* open SCO at SLC for this three way call */
      APPL_TRACE_DEBUG("Headset Connected in three way call");
      if (!(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb)) {
          bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
        } else if (result.data.audio_handle == BTA_AG_HANDLE_NONE) {
          bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
        }
      }
      break;

    case BTA_AG_OUT_CALL_CONN_RES:
      /* send indicators */
      bta_ag_send_call_inds(p_scb, result.result);

      /* open or close sco */
      if (!(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        if (result.data.audio_handle == bta_ag_scb_to_idx(p_scb)) {
          bta_ag_sco_open(p_scb, tBTA_AG_DATA::kEmpty);
        } else if (result.data.audio_handle == BTA_AG_HANDLE_NONE) {
          bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
        }
      }
      break;

    case BTA_AG_CALL_CANCEL_RES:
      /* send indicators */
      bta_ag_send_call_inds(p_scb, result.result);
      break;

    case BTA_AG_END_CALL_RES:
      alarm_cancel(p_scb->ring_timer);

      /* if sco open, close sco then send indicator values */
      if ((bta_ag_sco_is_open(p_scb) || bta_ag_sco_is_opening(p_scb)) &&
          !(p_scb->features & BTA_AG_FEAT_NOSCO)) {
        p_scb->post_sco = BTA_AG_POST_SCO_CALL_END;
        bta_ag_sco_close(p_scb, tBTA_AG_DATA::kEmpty);
      } else if (p_scb->post_sco == BTA_AG_POST_SCO_CALL_END_INCALL) {
        /* sco closing for outgoing call because of incoming call */
        /* Send only callsetup end indicator after sco close */
        p_scb->post_sco = BTA_AG_POST_SCO_CALL_END;
      } else {
        bta_ag_send_call_inds(p_scb, result.result);

        /* if av got suspended by this call, let it resume. */
        bta_sys_sco_unuse(BTA_ID_AG, p_scb->app_id, p_scb->peer_addr);
      }
      break;

    case BTA_AG_INBAND_RING_RES:
      p_scb->inband_enabled = result.data.state;
      APPL_TRACE_DEBUG("inband_enabled set to %d", p_scb->inband_enabled);
      bta_ag_send_result(p_scb, result.result, nullptr, result.data.state);
      break;

    case BTA_AG_CIND_RES:
      /* store local values */
      p_scb->call_ind = result.data.str[0] - '0';
      p_scb->callsetup_ind = result.data.str[2] - '0';
      p_scb->service_ind = result.data.str[4] - '0';
      p_scb->signal_ind = result.data.str[6] - '0';
      p_scb->roam_ind = result.data.str[8] - '0';
      p_scb->battchg_ind = result.data.str[10] - '0';
      p_scb->callheld_ind = result.data.str[12] - '0';
      APPL_TRACE_DEBUG("cind call:%d callsetup:%d", p_scb->call_ind,
                       p_scb->callsetup_ind);

      bta_ag_send_result(p_scb, result.result, result.data.str, 0);
      bta_ag_send_ok(p_scb);
      break;

    case BTA_AG_BINP_RES:
    case BTA_AG_CNUM_RES:
    case BTA_AG_CLCC_RES:
    case BTA_AG_COPS_RES:
      if (result.data.ok_flag != BTA_AG_OK_ERROR) {
        if (result.data.str[0] != 0) {
          bta_ag_send_result(p_scb, result.result, result.data.str, 0);
        }

        if (result.data.ok_flag == BTA_AG_OK_DONE) bta_ag_send_ok(p_scb);
      } else {
        bta_ag_send_error(p_scb, result.data.errcode);
      }
      break;

    case BTA_AG_UNAT_RES: {
      if (result.data.ok_flag != BTA_AG_OK_ERROR) {
        if (result.data.str[0] != 0) {
          tBTA_AG_API_RESULT result_copy(result);
          bta_ag_process_unat_res(result_copy.data.str);
          APPL_TRACE_DEBUG("BTA_AG_RES :%s", result_copy.data.str);
          bta_ag_send_result(p_scb, result_copy.result, result_copy.data.str,
                             0);
        }
        if (result.data.ok_flag == BTA_AG_OK_DONE) {
          bta_ag_send_ok(p_scb);
        }
      } else {
        bta_ag_send_error(p_scb, result.data.errcode);
      }
      break;
    }

    case BTA_AG_CALL_WAIT_RES:
      if (p_scb->ccwa_enabled) {
        bta_ag_send_result(p_scb, result.result, result.data.str, 0);
      }
      bta_ag_send_call_inds(p_scb, result.result);
      break;

    case BTA_AG_IND_RES:
      bta_ag_send_ind(p_scb, result.data.ind.id, result.data.ind.value, false);
      break;

    case BTA_AG_IND_RES_ON_DEMAND:
      bta_ag_send_ind(p_scb, result.data.ind.id, result.data.ind.value, true);
      break;

    case BTA_AG_BVRA_RES:
      bta_ag_send_result(p_scb, result.result, nullptr, result.data.state);
      break;

    case BTA_AG_BTRH_RES:
      if (result.data.ok_flag != BTA_AG_OK_ERROR) {
        /* Don't respond to read if not in response & hold state */
        if (result.data.num != BTA_AG_BTRH_NO_RESP) {
          bta_ag_send_result(p_scb, result.result, nullptr, result.data.num);
        }

        /* In case of a response to a read request we need to send OK */
        if (result.data.ok_flag == BTA_AG_OK_DONE) {
          bta_ag_send_ok(p_scb);
        }
      } else {
        bta_ag_send_error(p_scb, result.data.errcode);
      }
      break;

    case BTA_AG_BIND_RES: {
      /* Find whether ind_id is supported by local device or not */
      int local_index = bta_ag_find_hf_ind_by_id(p_scb->local_hf_indicators,
                                                 BTA_AG_MAX_NUM_LOCAL_HF_IND,
                                                 result.data.ind.id);
      if (local_index == -1) {
        APPL_TRACE_WARNING("%s Invalid HF Indicator ID %d", __func__,
                           result.data.ind.id);
        return;
      }

      /* Find whether ind_id is supported by peer device or not */
      int peer_index = bta_ag_find_hf_ind_by_id(p_scb->peer_hf_indicators,
                                                BTA_AG_MAX_NUM_PEER_HF_IND,
                                                result.data.ind.id);
      if (peer_index == -1) {
        APPL_TRACE_WARNING("%s Invalid HF Indicator ID %d", __func__,
                           result.data.ind.id);
        return;
      } else {
        /* If the current state is different from the one upper layer request
           change current state and send out the result */
        if (p_scb->local_hf_indicators[local_index].is_enable !=
            result.data.ind.on_demand) {
          char buffer[BTA_AG_AT_MAX_LEN] = {0};
          char* p = buffer;

          p_scb->local_hf_indicators[local_index].is_enable =
              result.data.ind.on_demand;
          p += utl_itoa(result.data.ind.id, p);
          *p++ = ',';
          p += utl_itoa(p_scb->local_hf_indicators[local_index].is_enable, p);

          bta_ag_send_result(p_scb, result.result, buffer, 0);
        } else {
          APPL_TRACE_DEBUG(
              "%s HF Indicator %d already %s", result.data.ind.id,
              (result.data.ind.on_demand) ? "Enabled" : "Disabled");
        }
      }
      break;
    }
    default:
      break;
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_result
 *
 * Description      Handle API result.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_result(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data) {
  if (p_scb->conn_service == BTA_AG_HSP) {
    bta_ag_hsp_result(p_scb, data.api_result);
  } else {
    bta_ag_hfp_result(p_scb, data.api_result);
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_send_bcs
 *
 * Description      Send +BCS AT command to peer.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_send_bcs(tBTA_AG_SCB* p_scb) {
  uint16_t codec_uuid;

  if (p_scb->codec_fallback) {
    codec_uuid = UUID_CODEC_CVSD;
  } else {
    switch (p_scb->sco_codec) {
      case BTA_AG_CODEC_NONE:
        codec_uuid = UUID_CODEC_CVSD;
        break;
      case BTA_AG_CODEC_CVSD:
        codec_uuid = UUID_CODEC_CVSD;
        break;
      case BTA_AG_CODEC_MSBC:
        codec_uuid = UUID_CODEC_MSBC;
        break;
      default:
        APPL_TRACE_ERROR("bta_ag_send_bcs: unknown codec %d, use CVSD",
                         p_scb->sco_codec);
        codec_uuid = UUID_CODEC_CVSD;
        break;
    }
  }

  /* send +BCS */
  APPL_TRACE_DEBUG("send +BCS codec is %d", codec_uuid);
  bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_BCS, nullptr, codec_uuid);
}

/*******************************************************************************
 *
 * Function         bta_ag_send_ring
 *
 * Description      Send RING result code to peer.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_send_ring(tBTA_AG_SCB* p_scb,
                      UNUSED_ATTR const tBTA_AG_DATA& data) {
  if ((p_scb->conn_service == BTA_AG_HFP) &&
      p_scb->callsetup_ind != BTA_AG_CALLSETUP_INCOMING) {
    LOG(WARNING) << __func__ << ": don't send RING, conn_service="
                 << std::to_string(p_scb->conn_service)
                 << ", callsetup_ind=" << std::to_string(p_scb->callsetup_ind);
    return;
  }
  /* send RING */
  bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_RING, nullptr, 0);

  /* if HFP and clip enabled and clip data send CLIP */
  if (p_scb->conn_service == BTA_AG_HFP && p_scb->clip_enabled &&
      p_scb->clip[0] != 0) {
    bta_ag_send_result(p_scb, BTA_AG_LOCAL_RES_CLIP, p_scb->clip, 0);
  }

  bta_sys_start_timer(p_scb->ring_timer, BTA_AG_RING_TIMEOUT_MS,
                      BTA_AG_RING_TIMEOUT_EVT, bta_ag_scb_to_idx(p_scb));
}
