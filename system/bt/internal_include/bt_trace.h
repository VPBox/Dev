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

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static const char BTE_LOGMSG_MODULE[] = "bte_logmsg_module";

/* BTE tracing IDs for debug purposes */
/* LayerIDs for stack */
#define BTTRC_ID_STK_GKI 1
#define BTTRC_ID_STK_BTU 2
#define BTTRC_ID_STK_HCI 3
#define BTTRC_ID_STK_L2CAP 4
#define BTTRC_ID_STK_RFCM_MX 5
#define BTTRC_ID_STK_RFCM_PRT 6
#define BTTRC_ID_STK_OBEX_C 7
#define BTTRC_ID_STK_OBEX_S 8
#define BTTRC_ID_STK_AVCT 9
#define BTTRC_ID_STK_AVDT 10
#define BTTRC_ID_STK_AVRC 11
#define BTTRC_ID_STK_BIC 12
#define BTTRC_ID_STK_BIS 13
#define BTTRC_ID_STK_BNEP 14
#define BTTRC_ID_STK_BPP 15
#define BTTRC_ID_STK_BTM_ACL 16
#define BTTRC_ID_STK_BTM_PM 17
#define BTTRC_ID_STK_BTM_DEV_CTRL 18
#define BTTRC_ID_STK_BTM_SVC_DSC 19
#define BTTRC_ID_STK_BTM_INQ 20
#define BTTRC_ID_STK_BTM_SCO 21
#define BTTRC_ID_STK_BTM_SEC 22
#define BTTRC_ID_STK_HID 24
#define BTTRC_ID_STK_HSP2 25
#define BTTRC_ID_STK_CTP 26
#define BTTRC_ID_STK_FTC 27
#define BTTRC_ID_STK_FTS 28
#define BTTRC_ID_STK_HCRP 31
#define BTTRC_ID_STK_ICP 32
#define BTTRC_ID_STK_OPC 33
#define BTTRC_ID_STK_OPS 34
#define BTTRC_ID_STK_PAN 35
#define BTTRC_ID_STK_SAP 36
#define BTTRC_ID_STK_SDP 37
#define BTTRC_ID_STK_SLIP 38
#define BTTRC_ID_STK_SPP 39
#define BTTRC_ID_STK_TCS 40
#define BTTRC_ID_STK_VDP 41
#define BTTRC_ID_STK_MCAP 42 /* OBSOLETE */
#define BTTRC_ID_STK_GATT 43
#define BTTRC_ID_STK_SMP 44
#define BTTRC_ID_STK_NFC 45
#define BTTRC_ID_STK_NCI 46
#define BTTRC_ID_STK_IDEP 47
#define BTTRC_ID_STK_NDEP 48
#define BTTRC_ID_STK_LLCP 49
#define BTTRC_ID_STK_RW 50
#define BTTRC_ID_STK_CE 51
#define BTTRC_ID_STK_SNEP 52
#define BTTRC_ID_STK_NDEF 53
#define BTTRC_ID_STK_HIDD 54

/* LayerIDs for BTA */
#define BTTRC_ID_BTA_ACC 55 /* Advanced Camera Client */
#define BTTRC_ID_BTA_AG 56  /* audio gateway */
#define BTTRC_ID_BTA_AV 57  /* Advanced audio */
#define BTTRC_ID_BTA_BIC 58 /* Basic Imaging Client */
#define BTTRC_ID_BTA_BIS 59 /* Basic Imaging Server */
#define BTTRC_ID_BTA_BP 60  /* Basic Printing Client */
#define BTTRC_ID_BTA_CG 61
#define BTTRC_ID_BTA_CT 62      /* cordless telephony terminal */
#define BTTRC_ID_BTA_DG 63      /* data gateway */
#define BTTRC_ID_BTA_DM 64      /* device manager */
#define BTTRC_ID_BTA_DM_SRCH 65 /* device manager search */
#define BTTRC_ID_BTA_DM_SEC 66  /* device manager security */
#define BTTRC_ID_BTA_FM 67
#define BTTRC_ID_BTA_FTC 68 /* file transfer client */
#define BTTRC_ID_BTA_FTS 69 /* file transfer server */
#define BTTRC_ID_BTA_HIDH 70
#define BTTRC_ID_BTA_HIDD 71
#define BTTRC_ID_BTA_JV 72
#define BTTRC_ID_BTA_OPC 73  /* object push client */
#define BTTRC_ID_BTA_OPS 74  /* object push server */
#define BTTRC_ID_BTA_PAN 75  /* Personal Area Networking */
#define BTTRC_ID_BTA_PR 76   /* Printer client */
#define BTTRC_ID_BTA_SC 77   /* SIM Card Access server */
#define BTTRC_ID_BTA_SS 78   /* synchronization server */
#define BTTRC_ID_BTA_SYS 79  /* system manager */
#define BTTRC_ID_AVDT_SCB 80 /* avdt scb */
#define BTTRC_ID_AVDT_CCB 81 /* avdt ccb */

/* LayerIDs added for BTL-A. Probably should modify bte_logmsg.cc in future. */
#define BTTRC_ID_STK_RFCOMM 82
#define BTTRC_ID_STK_RFCOMM_DATA 83
#define BTTRC_ID_STK_OBEX 84
#define BTTRC_ID_STK_A2DP 85
#define BTTRC_ID_STK_BIP 86

/* LayerIDs for BT APP */
#define BTTRC_ID_BTAPP 87
/* this is a temporary solution to allow dynamic enable/disable of
 * BT_PROTOCOL_TRACE */
#define BTTRC_ID_BT_PROTOCOL 88
#define BTTRC_ID_MAX_ID BTTRC_ID_BT_PROTOCOL
#define BTTRC_ID_ALL_LAYERS 0xFF /* all trace layers */

/******************************************************************************
 *
 * Trace Levels
 *
 * The following values may be used for different levels:
 *      BT_TRACE_LEVEL_NONE    0        * No trace messages to be generated
 *      BT_TRACE_LEVEL_ERROR   1        * Error condition trace messages
 *      BT_TRACE_LEVEL_WARNING 2        * Warning condition trace messages
 *      BT_TRACE_LEVEL_API     3        * API traces
 *      BT_TRACE_LEVEL_EVENT   4        * Debug messages for events
 *      BT_TRACE_LEVEL_DEBUG   5        * Debug messages (general)
 *****************************************************************************/

/* Core Stack default trace levels */
#ifndef HCI_INITIAL_TRACE_LEVEL
#define HCI_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef BTM_INITIAL_TRACE_LEVEL
#define BTM_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef L2CAP_INITIAL_TRACE_LEVEL
#define L2CAP_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef RFCOMM_INITIAL_TRACE_LEVEL
#define RFCOMM_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef SDP_INITIAL_TRACE_LEVEL
#define SDP_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef BNEP_INITIAL_TRACE_LEVEL
#define BNEP_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef PAN_INITIAL_TRACE_LEVEL
#define PAN_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef A2DP_INITIAL_TRACE_LEVEL
#define A2DP_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef AVDT_INITIAL_TRACE_LEVEL
#define AVDT_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef AVCT_INITIAL_TRACE_LEVEL
#define AVCT_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef AVRC_INITIAL_TRACE_LEVEL
#define AVRC_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef HID_INITIAL_TRACE_LEVEL
#define HID_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef APPL_INITIAL_TRACE_LEVEL
#define APPL_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef GATT_INITIAL_TRACE_LEVEL
#define GATT_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#ifndef SMP_INITIAL_TRACE_LEVEL
#define SMP_INITIAL_TRACE_LEVEL BT_TRACE_LEVEL_WARNING
#endif

#define BT_TRACE(l, t, ...) \
  LogMsg((TRACE_CTRL_GENERAL | (l) | TRACE_ORG_STACK | (t)), ##__VA_ARGS__)

/* Define tracing for the HCI unit */
#define HCI_TRACE_ERROR(...)                                      \
  {                                                               \
    if (btu_trace_level >= BT_TRACE_LEVEL_ERROR)                  \
      BT_TRACE(TRACE_LAYER_HCI, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define HCI_TRACE_WARNING(...)                                      \
  {                                                                 \
    if (btu_trace_level >= BT_TRACE_LEVEL_WARNING)                  \
      BT_TRACE(TRACE_LAYER_HCI, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define HCI_TRACE_EVENT(...)                                      \
  {                                                               \
    if (btu_trace_level >= BT_TRACE_LEVEL_EVENT)                  \
      BT_TRACE(TRACE_LAYER_HCI, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define HCI_TRACE_DEBUG(...)                                      \
  {                                                               \
    if (btu_trace_level >= BT_TRACE_LEVEL_DEBUG)                  \
      BT_TRACE(TRACE_LAYER_HCI, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* Define tracing for BTM */
#define BTM_TRACE_ERROR(...)                                      \
  {                                                               \
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_BTM, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define BTM_TRACE_WARNING(...)                                      \
  {                                                                 \
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_BTM, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define BTM_TRACE_API(...)                                      \
  {                                                             \
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_BTM, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define BTM_TRACE_EVENT(...)                                      \
  {                                                               \
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_BTM, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define BTM_TRACE_DEBUG(...)                                      \
  {                                                               \
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_BTM, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* Define tracing for the L2CAP unit */
#define L2CAP_TRACE_ERROR(...)                                      \
  {                                                                 \
    if (l2cb.l2cap_trace_level >= BT_TRACE_LEVEL_ERROR)             \
      BT_TRACE(TRACE_LAYER_L2CAP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define L2CAP_TRACE_WARNING(...)                                      \
  {                                                                   \
    if (l2cb.l2cap_trace_level >= BT_TRACE_LEVEL_WARNING)             \
      BT_TRACE(TRACE_LAYER_L2CAP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define L2CAP_TRACE_API(...)                                      \
  {                                                               \
    if (l2cb.l2cap_trace_level >= BT_TRACE_LEVEL_API)             \
      BT_TRACE(TRACE_LAYER_L2CAP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define L2CAP_TRACE_EVENT(...)                                      \
  {                                                                 \
    if (l2cb.l2cap_trace_level >= BT_TRACE_LEVEL_EVENT)             \
      BT_TRACE(TRACE_LAYER_L2CAP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define L2CAP_TRACE_DEBUG(...)                                      \
  {                                                                 \
    if (l2cb.l2cap_trace_level >= BT_TRACE_LEVEL_DEBUG)             \
      BT_TRACE(TRACE_LAYER_L2CAP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* Define tracing for the SDP unit */
#define SDP_TRACE_ERROR(...)                                      \
  {                                                               \
    if (sdp_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_SDP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define SDP_TRACE_WARNING(...)                                      \
  {                                                                 \
    if (sdp_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_SDP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define SDP_TRACE_API(...)                                      \
  {                                                             \
    if (sdp_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_SDP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define SDP_TRACE_EVENT(...)                                      \
  {                                                               \
    if (sdp_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_SDP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define SDP_TRACE_DEBUG(...)                                      \
  {                                                               \
    if (sdp_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_SDP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* Define tracing for the RFCOMM unit */
#define RFCOMM_TRACE_ERROR(...)                                      \
  {                                                                  \
    if (rfc_cb.trace_level >= BT_TRACE_LEVEL_ERROR)                  \
      BT_TRACE(TRACE_LAYER_RFCOMM, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define RFCOMM_TRACE_WARNING(...)                                      \
  {                                                                    \
    if (rfc_cb.trace_level >= BT_TRACE_LEVEL_WARNING)                  \
      BT_TRACE(TRACE_LAYER_RFCOMM, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define RFCOMM_TRACE_API(...)                                      \
  {                                                                \
    if (rfc_cb.trace_level >= BT_TRACE_LEVEL_API)                  \
      BT_TRACE(TRACE_LAYER_RFCOMM, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define RFCOMM_TRACE_EVENT(...)                                      \
  {                                                                  \
    if (rfc_cb.trace_level >= BT_TRACE_LEVEL_EVENT)                  \
      BT_TRACE(TRACE_LAYER_RFCOMM, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define RFCOMM_TRACE_DEBUG(...)                                      \
  {                                                                  \
    if (rfc_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)                  \
      BT_TRACE(TRACE_LAYER_RFCOMM, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* define traces for HID Host */
#define HIDH_TRACE_ERROR(...)                                     \
  {                                                               \
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_ERROR)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define HIDH_TRACE_WARNING(...)                                     \
  {                                                                 \
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define HIDH_TRACE_API(...)                                     \
  {                                                             \
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_API)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define HIDH_TRACE_EVENT(...)                                     \
  {                                                               \
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define HIDH_TRACE_DEBUG(...)                                     \
  {                                                               \
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* define traces for HID Device */
#define HIDD_TRACE_ERROR(...)                                     \
  {                                                               \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_ERROR)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define HIDD_TRACE_WARNING(...)                                     \
  {                                                                 \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_WARNING)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define HIDD_TRACE_API(...)                                     \
  {                                                             \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_API)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define HIDD_TRACE_EVENT(...)                                     \
  {                                                               \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_EVENT)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define HIDD_TRACE_DEBUG(...)                                     \
  {                                                               \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)                \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }
#define HIDD_TRACE_VERBOSE(...)                                   \
  {                                                               \
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_VERBOSE)              \
      BT_TRACE(TRACE_LAYER_HID, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* define traces for BNEP */
#define BNEP_TRACE_ERROR(...)                                      \
  {                                                                \
    if (bnep_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_BNEP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define BNEP_TRACE_WARNING(...)                                      \
  {                                                                  \
    if (bnep_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_BNEP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define BNEP_TRACE_API(...)                                      \
  {                                                              \
    if (bnep_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_BNEP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define BNEP_TRACE_EVENT(...)                                      \
  {                                                                \
    if (bnep_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_BNEP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define BNEP_TRACE_DEBUG(...)                                      \
  {                                                                \
    if (bnep_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_BNEP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* define traces for PAN */
#define PAN_TRACE_ERROR(...)                                      \
  {                                                               \
    if (pan_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_PAN, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define PAN_TRACE_WARNING(...)                                      \
  {                                                                 \
    if (pan_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_PAN, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define PAN_TRACE_API(...)                                      \
  {                                                             \
    if (pan_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_PAN, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define PAN_TRACE_EVENT(...)                                      \
  {                                                               \
    if (pan_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_PAN, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define PAN_TRACE_DEBUG(...)                                      \
  {                                                               \
    if (pan_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_PAN, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

/* Define tracing for the A2DP profile */
#define A2DP_TRACE_ERROR(...)                                      \
  {                                                                \
    if (a2dp_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_A2DP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define A2DP_TRACE_WARNING(...)                                      \
  {                                                                  \
    if (a2dp_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_A2DP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define A2DP_TRACE_EVENT(...)                                      \
  {                                                                \
    if (a2dp_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_A2DP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define A2DP_TRACE_DEBUG(...)                                      \
  {                                                                \
    if (a2dp_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_A2DP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }
#define A2DP_TRACE_API(...)                                      \
  {                                                              \
    if (a2dp_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_A2DP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }

/* AVDTP */
#define AVDT_TRACE_ERROR(...)                                     \
  {                                                               \
    if (avdtp_cb.TraceLevel() >= BT_TRACE_LEVEL_ERROR)            \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define AVDT_TRACE_WARNING(...)                                     \
  {                                                                 \
    if (avdtp_cb.TraceLevel() >= BT_TRACE_LEVEL_WARNING)            \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define AVDT_TRACE_EVENT(...)                                     \
  {                                                               \
    if (avdtp_cb.TraceLevel() >= BT_TRACE_LEVEL_EVENT)            \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define AVDT_TRACE_DEBUG(...)                                     \
  {                                                               \
    if (avdtp_cb.TraceLevel() >= BT_TRACE_LEVEL_DEBUG)            \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }
#define AVDT_TRACE_API(...)                                     \
  {                                                             \
    if (avdtp_cb.TraceLevel() >= BT_TRACE_LEVEL_API)            \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }

/* Define tracing for the AVCTP protocol */
#define AVCT_TRACE_ERROR(...)                                     \
  {                                                               \
    if (avct_cb.trace_level >= BT_TRACE_LEVEL_ERROR)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define AVCT_TRACE_WARNING(...)                                     \
  {                                                                 \
    if (avct_cb.trace_level >= BT_TRACE_LEVEL_WARNING)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define AVCT_TRACE_EVENT(...)                                     \
  {                                                               \
    if (avct_cb.trace_level >= BT_TRACE_LEVEL_EVENT)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define AVCT_TRACE_DEBUG(...)                                     \
  {                                                               \
    if (avct_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }
#define AVCT_TRACE_API(...)                                     \
  {                                                             \
    if (avct_cb.trace_level >= BT_TRACE_LEVEL_API)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }

/* Define tracing for the AVRCP profile */
#define AVRC_TRACE_ERROR(...)                                     \
  {                                                               \
    if (avrc_cb.trace_level >= BT_TRACE_LEVEL_ERROR)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define AVRC_TRACE_WARNING(...)                                     \
  {                                                                 \
    if (avrc_cb.trace_level >= BT_TRACE_LEVEL_WARNING)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define AVRC_TRACE_EVENT(...)                                     \
  {                                                               \
    if (avrc_cb.trace_level >= BT_TRACE_LEVEL_EVENT)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define AVRC_TRACE_DEBUG(...)                                     \
  {                                                               \
    if (avrc_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }
#define AVRC_TRACE_API(...)                                     \
  {                                                             \
    if (avrc_cb.trace_level >= BT_TRACE_LEVEL_API)              \
      BT_TRACE(TRACE_LAYER_AVP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }

/* Define tracing for the SMP unit */
#define SMP_TRACE_ERROR(...)                                      \
  {                                                               \
    if (smp_cb.trace_level >= BT_TRACE_LEVEL_ERROR)               \
      BT_TRACE(TRACE_LAYER_SMP, TRACE_TYPE_ERROR, ##__VA_ARGS__); \
  }
#define SMP_TRACE_WARNING(...)                                      \
  {                                                                 \
    if (smp_cb.trace_level >= BT_TRACE_LEVEL_WARNING)               \
      BT_TRACE(TRACE_LAYER_SMP, TRACE_TYPE_WARNING, ##__VA_ARGS__); \
  }
#define SMP_TRACE_API(...)                                      \
  {                                                             \
    if (smp_cb.trace_level >= BT_TRACE_LEVEL_API)               \
      BT_TRACE(TRACE_LAYER_SMP, TRACE_TYPE_API, ##__VA_ARGS__); \
  }
#define SMP_TRACE_EVENT(...)                                      \
  {                                                               \
    if (smp_cb.trace_level >= BT_TRACE_LEVEL_EVENT)               \
      BT_TRACE(TRACE_LAYER_SMP, TRACE_TYPE_EVENT, ##__VA_ARGS__); \
  }
#define SMP_TRACE_DEBUG(...)                                      \
  {                                                               \
    if (smp_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)               \
      BT_TRACE(TRACE_LAYER_SMP, TRACE_TYPE_DEBUG, ##__VA_ARGS__); \
  }

extern uint8_t btif_trace_level;

/* define traces for application */
#define BTIF_TRACE_ERROR(...)                                         \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_ERROR)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_ERROR,                                    \
             ##__VA_ARGS__);                                          \
  }
#define BTIF_TRACE_WARNING(...)                                       \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_WARNING)                   \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_WARNING,                                  \
             ##__VA_ARGS__);                                          \
  }
#define BTIF_TRACE_API(...)                                           \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_API)                       \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_API,                                      \
             ##__VA_ARGS__);                                          \
  }
#define BTIF_TRACE_EVENT(...)                                         \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_EVENT)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_EVENT,                                    \
             ##__VA_ARGS__);                                          \
  }
#define BTIF_TRACE_DEBUG(...)                                         \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_DEBUG)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_DEBUG,                                    \
             ##__VA_ARGS__);                                          \
  }
#define BTIF_TRACE_VERBOSE(...)                                       \
  {                                                                   \
    if (btif_trace_level >= BT_TRACE_LEVEL_VERBOSE)                   \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_DEBUG,                                    \
             ##__VA_ARGS__);                                          \
  }

/* define traces for application */
#define APPL_TRACE_ERROR(...)                                         \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_ERROR)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_ERROR,                                    \
             ##__VA_ARGS__);                                          \
  }
#define APPL_TRACE_WARNING(...)                                       \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_WARNING)                   \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_WARNING,                                  \
             ##__VA_ARGS__);                                          \
  }
#define APPL_TRACE_API(...)                                           \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_API)                       \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_API,                                      \
             ##__VA_ARGS__);                                          \
  }
#define APPL_TRACE_EVENT(...)                                         \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_EVENT)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_EVENT,                                    \
             ##__VA_ARGS__);                                          \
  }
#define APPL_TRACE_DEBUG(...)                                         \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_DEBUG)                     \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_DEBUG,                                    \
             ##__VA_ARGS__);                                          \
  }
#define APPL_TRACE_VERBOSE(...)                                       \
  {                                                                   \
    if (appl_trace_level >= BT_TRACE_LEVEL_VERBOSE)                   \
      LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | \
                 TRACE_TYPE_DEBUG,                                    \
             ##__VA_ARGS__);                                          \
  }

typedef uint8_t tBTTRC_LAYER_ID;
typedef uint8_t(tBTTRC_SET_TRACE_LEVEL)(uint8_t);

typedef struct {
  const tBTTRC_LAYER_ID layer_id_start;
  const tBTTRC_LAYER_ID layer_id_end;
  tBTTRC_SET_TRACE_LEVEL* p_f;
  const char* trc_name;
  uint8_t trace_level;
} tBTTRC_FUNC_MAP;

/* External declaration for appl_trace_level here to avoid to add the
 * declaration in all the files using APPL_TRACExxx macros */
extern uint8_t appl_trace_level;

void LogMsg(uint32_t trace_set_mask, const char* fmt_str, ...);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <iomanip>
#include <sstream>
#include <type_traits>

#include <base/logging.h>

/* Prints integral parameter x as hex string, with '0' fill */
template <typename T>
std::string loghex(T x) {
  static_assert(std::is_integral<T>::value,
                "loghex parameter must be integral.");
  std::stringstream tmp;
  tmp << std::showbase << std::internal << std::hex << std::setfill('0')
      << std::setw((sizeof(T) * 2) + 2) << +x;
  return tmp.str();
}

/* Prints integral array as hex string, with '0' fill */
template <typename T, size_t N>
std::string loghex(std::array<T, N> array) {
  static_assert(std::is_integral<T>::value,
                "type stored in array must be integral.");
  std::stringstream tmp;
  for (const auto& x : array) {
    tmp << std::internal << std::hex << std::setfill('0')
        << std::setw((sizeof(uint8_t) * 2) + 2) << +x;
  }
  return tmp.str();
}

/**
 * Obtains the string representation of a boolean value.
 *
 * @param value the boolean value to use
 * @return the string representation of the boolean value: "true" or "false"
 */
inline std::string logbool(bool value) {
  std::stringstream tmp;
  tmp << std::boolalpha << value;
  return tmp.str();
}

/**
 * Append a field name to a string.
 *
 * The field names are added to the string with "|" in between.
 *
 * @param p_result a pointer to the result string to add the field name to
 * @param append if true the field name will be added
 * @param name the field name to add
 * @return the result string
 */
inline std::string& AppendField(std::string* p_result, bool append,
                                const std::string& name) {
  CHECK(p_result != nullptr);
  if (!append) return *p_result;
  if (!p_result->empty()) *p_result += "|";
  *p_result += name;
  return *p_result;
}

// This object puts the stream in a state where every time that a new line
// occurs, the next line is indented a certain number of spaces. The stream is
// reset to its previous state when the object is destroyed.
class ScopedIndent {
 public:
  ScopedIndent(std::ostream& stream, int indent_size = DEFAULT_TAB)
      : indented_buf_(stream, indent_size) {
    old_stream_ = &stream;
    old_stream_buf_ = stream.rdbuf();
    stream.rdbuf(&indented_buf_);
  }

  ~ScopedIndent() { old_stream_->rdbuf(old_stream_buf_); }

  static const size_t DEFAULT_TAB = 2;

 private:
  class IndentedStreamBuf : public std::streambuf {
   public:
    IndentedStreamBuf(std::ostream& stream, int indent_size)
        : wrapped_buf_(stream.rdbuf()),
          indent_size_(indent_size),
          indent_next_line_(true){};

   protected:
    virtual int overflow(int character) override {
      if (indent_next_line_ && character != '\n') {
        for (int i = 0; i < indent_size_; i++) wrapped_buf_->sputc(' ');
      }

      indent_next_line_ = false;
      if (character == '\n') {
        indent_next_line_ = true;
      }

      return wrapped_buf_->sputc(character);
    }

   private:
    std::streambuf* wrapped_buf_;
    int indent_size_;
    bool indent_next_line_;
  };

  std::ostream* old_stream_;
  std::streambuf* old_stream_buf_;
  IndentedStreamBuf indented_buf_;
};

#endif
