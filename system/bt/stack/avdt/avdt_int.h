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
 *  This file contains interfaces which are internal to AVDTP.
 *
 ******************************************************************************/
#ifndef AVDT_INT_H
#define AVDT_INT_H

#include "avdt_api.h"
#include "avdt_defs.h"
#include "avdtc_api.h"
#include "bt_common.h"
#include "btm_api.h"
#include "l2c_api.h"
#include "osi/include/alarm.h"
#include "osi/include/fixed_queue.h"

#ifndef AVDT_DEBUG
#define AVDT_DEBUG FALSE
#endif

/*****************************************************************************
 * constants
 ****************************************************************************/

/* channel types */
enum {
  AVDT_CHAN_SIG,   /* signaling channel */
  AVDT_CHAN_MEDIA, /* media channel */
  AVDT_CHAN_REPORT, /* reporting channel */
  AVDT_CHAN_NUM_TYPES
};

/* protocol service capabilities of this AVDTP implementation */
#define AVDT_PSC (AVDT_PSC_TRANS | AVDT_PSC_REPORT | AVDT_PSC_DELAY_RPT)
#define AVDT_LEG_PSC (AVDT_PSC_TRANS | AVDT_PSC_REPORT)

/* initiator/acceptor signaling roles */
#define AVDT_CLOSE_ACP 0
#define AVDT_CLOSE_INT 1
#define AVDT_OPEN_ACP 2
#define AVDT_OPEN_INT 3

/* states for avdt_scb_verify */
#define AVDT_VERIFY_OPEN 0
#define AVDT_VERIFY_STREAMING 1
#define AVDT_VERIFY_SUSPEND 2
#define AVDT_VERIFY_START 3

/* to distinguish CCB events from SCB events */
#define AVDT_CCB_MKR 0x80

/* offset where AVDTP signaling message content starts;
 * use the size of a start header since it's the largest possible
 * layout of signaling message in a buffer is:
 *
 * |  BT_HDR  | SCB handles | L2CAP + HCI header | AVDTP header | data ... |
 *
 * Note that we "hide" the scb handles at the top of the message buffer.
*/
#define AVDT_MSG_OFFSET (L2CAP_MIN_OFFSET + AVDT_NUM_SEPS + AVDT_LEN_TYPE_START)

/* scb transport channel connect timeout value (in milliseconds) */
#define AVDT_SCB_TC_CONN_TIMEOUT_MS (10 * 1000)

/* scb transport channel disconnect timeout value (in milliseconds) */
#define AVDT_SCB_TC_DISC_TIMEOUT_MS (10 * 1000)

/* maximum number of command retransmissions */
#ifndef AVDT_RET_MAX
#define AVDT_RET_MAX 1
#endif

/* ccb state machine states */
enum {
  AVDT_CCB_IDLE_ST,
  AVDT_CCB_OPENING_ST,
  AVDT_CCB_OPEN_ST,
  AVDT_CCB_CLOSING_ST
};

/* state machine action enumeration list */
enum {
  AVDT_CCB_CHAN_OPEN,
  AVDT_CCB_CHAN_CLOSE,
  AVDT_CCB_CHK_CLOSE,
  AVDT_CCB_HDL_DISCOVER_CMD,
  AVDT_CCB_HDL_DISCOVER_RSP,
  AVDT_CCB_HDL_GETCAP_CMD,
  AVDT_CCB_HDL_GETCAP_RSP,
  AVDT_CCB_HDL_START_CMD,
  AVDT_CCB_HDL_START_RSP,
  AVDT_CCB_HDL_SUSPEND_CMD,
  AVDT_CCB_HDL_SUSPEND_RSP,
  AVDT_CCB_SND_DISCOVER_CMD,
  AVDT_CCB_SND_DISCOVER_RSP,
  AVDT_CCB_SND_GETCAP_CMD,
  AVDT_CCB_SND_GETCAP_RSP,
  AVDT_CCB_SND_START_CMD,
  AVDT_CCB_SND_START_RSP,
  AVDT_CCB_SND_SUSPEND_CMD,
  AVDT_CCB_SND_SUSPEND_RSP,
  AVDT_CCB_CLEAR_CMDS,
  AVDT_CCB_CMD_FAIL,
  AVDT_CCB_FREE_CMD,
  AVDT_CCB_CONG_STATE,
  AVDT_CCB_RET_CMD,
  AVDT_CCB_SND_CMD,
  AVDT_CCB_SND_MSG,
  AVDT_CCB_SET_RECONN,
  AVDT_CCB_CLR_RECONN,
  AVDT_CCB_CHK_RECONN,
  AVDT_CCB_CHK_TIMER,
  AVDT_CCB_SET_CONN,
  AVDT_CCB_SET_DISCONN,
  AVDT_CCB_DO_DISCONN,
  AVDT_CCB_LL_CLOSED,
  AVDT_CCB_LL_OPENED,
  AVDT_CCB_DEALLOC,
  AVDT_CCB_NUM_ACTIONS
};

#define AVDT_CCB_IGNORE AVDT_CCB_NUM_ACTIONS

/* ccb state machine events */
enum {
  AVDT_CCB_API_DISCOVER_REQ_EVT,
  AVDT_CCB_API_GETCAP_REQ_EVT,
  AVDT_CCB_API_START_REQ_EVT,
  AVDT_CCB_API_SUSPEND_REQ_EVT,
  AVDT_CCB_API_DISCOVER_RSP_EVT,
  AVDT_CCB_API_GETCAP_RSP_EVT,
  AVDT_CCB_API_START_RSP_EVT,
  AVDT_CCB_API_SUSPEND_RSP_EVT,
  AVDT_CCB_API_CONNECT_REQ_EVT,
  AVDT_CCB_API_DISCONNECT_REQ_EVT,
  AVDT_CCB_MSG_DISCOVER_CMD_EVT,
  AVDT_CCB_MSG_GETCAP_CMD_EVT,
  AVDT_CCB_MSG_START_CMD_EVT,
  AVDT_CCB_MSG_SUSPEND_CMD_EVT,
  AVDT_CCB_MSG_DISCOVER_RSP_EVT,
  AVDT_CCB_MSG_GETCAP_RSP_EVT,
  AVDT_CCB_MSG_START_RSP_EVT,
  AVDT_CCB_MSG_SUSPEND_RSP_EVT,
  AVDT_CCB_RCVRSP_EVT,
  AVDT_CCB_SENDMSG_EVT,
  AVDT_CCB_RET_TOUT_EVT,
  AVDT_CCB_RSP_TOUT_EVT,
  AVDT_CCB_IDLE_TOUT_EVT,
  AVDT_CCB_UL_OPEN_EVT,
  AVDT_CCB_UL_CLOSE_EVT,
  AVDT_CCB_LL_OPEN_EVT,
  AVDT_CCB_LL_CLOSE_EVT,
  AVDT_CCB_LL_CONG_EVT
};

/* scb state machine states; these state values are private to this module so
 * the scb state cannot be read or set by actions functions
*/
enum {
  AVDT_SCB_IDLE_ST,
  AVDT_SCB_CONF_ST,
  AVDT_SCB_OPENING_ST,
  AVDT_SCB_OPEN_ST,
  AVDT_SCB_STREAM_ST,
  AVDT_SCB_CLOSING_ST
};

/* state machine action enumeration list */
enum {
  AVDT_SCB_HDL_ABORT_CMD,
  AVDT_SCB_HDL_ABORT_RSP,
  AVDT_SCB_HDL_CLOSE_CMD,
  AVDT_SCB_HDL_CLOSE_RSP,
  AVDT_SCB_HDL_GETCONFIG_CMD,
  AVDT_SCB_HDL_GETCONFIG_RSP,
  AVDT_SCB_HDL_OPEN_CMD,
  AVDT_SCB_HDL_OPEN_REJ,
  AVDT_SCB_HDL_OPEN_RSP,
  AVDT_SCB_HDL_PKT,
  AVDT_SCB_DROP_PKT,
  AVDT_SCB_HDL_RECONFIG_CMD,
  AVDT_SCB_HDL_RECONFIG_RSP,
  AVDT_SCB_HDL_SECURITY_CMD,
  AVDT_SCB_HDL_SECURITY_RSP,
  AVDT_SCB_HDL_SETCONFIG_CMD,
  AVDT_SCB_HDL_SETCONFIG_REJ,
  AVDT_SCB_HDL_SETCONFIG_RSP,
  AVDT_SCB_HDL_START_CMD,
  AVDT_SCB_HDL_START_RSP,
  AVDT_SCB_HDL_SUSPEND_CMD,
  AVDT_SCB_HDL_SUSPEND_RSP,
  AVDT_SCB_HDL_TC_CLOSE,
  AVDT_SCB_HDL_TC_CLOSE_STO,
  AVDT_SCB_HDL_TC_OPEN,
  AVDT_SCB_HDL_TC_OPEN_STO,
  AVDT_SCB_SND_DELAY_RPT_REQ,
  AVDT_SCB_HDL_DELAY_RPT_CMD,
  AVDT_SCB_HDL_DELAY_RPT_RSP,
  AVDT_SCB_HDL_WRITE_REQ,
  AVDT_SCB_SND_ABORT_REQ,
  AVDT_SCB_SND_ABORT_RSP,
  AVDT_SCB_SND_CLOSE_REQ,
  AVDT_SCB_SND_STREAM_CLOSE,
  AVDT_SCB_SND_CLOSE_RSP,
  AVDT_SCB_SND_GETCONFIG_REQ,
  AVDT_SCB_SND_GETCONFIG_RSP,
  AVDT_SCB_SND_OPEN_REQ,
  AVDT_SCB_SND_OPEN_RSP,
  AVDT_SCB_SND_RECONFIG_REQ,
  AVDT_SCB_SND_RECONFIG_RSP,
  AVDT_SCB_SND_SECURITY_REQ,
  AVDT_SCB_SND_SECURITY_RSP,
  AVDT_SCB_SND_SETCONFIG_REQ,
  AVDT_SCB_SND_SETCONFIG_REJ,
  AVDT_SCB_SND_SETCONFIG_RSP,
  AVDT_SCB_SND_TC_CLOSE,
  AVDT_SCB_CB_ERR,
  AVDT_SCB_CONG_STATE,
  AVDT_SCB_REJ_STATE,
  AVDT_SCB_REJ_IN_USE,
  AVDT_SCB_REJ_NOT_IN_USE,
  AVDT_SCB_SET_REMOVE,
  AVDT_SCB_FREE_PKT,
  AVDT_SCB_CLR_PKT,
  AVDT_SCB_CHK_SND_PKT,
  AVDT_SCB_TC_TIMER,
  AVDT_SCB_CLR_VARS,
  AVDT_SCB_DEALLOC,
  AVDT_SCB_NUM_ACTIONS
};

#define AVDT_SCB_IGNORE AVDT_SCB_NUM_ACTIONS

/* scb state machine events */
enum {
  AVDT_SCB_API_REMOVE_EVT,
  AVDT_SCB_API_WRITE_REQ_EVT,
  AVDT_SCB_API_GETCONFIG_REQ_EVT,
  AVDT_SCB_API_DELAY_RPT_REQ_EVT,
  AVDT_SCB_API_SETCONFIG_REQ_EVT,
  AVDT_SCB_API_OPEN_REQ_EVT,
  AVDT_SCB_API_CLOSE_REQ_EVT,
  AVDT_SCB_API_RECONFIG_REQ_EVT,
  AVDT_SCB_API_SECURITY_REQ_EVT,
  AVDT_SCB_API_ABORT_REQ_EVT,
  AVDT_SCB_API_GETCONFIG_RSP_EVT,
  AVDT_SCB_API_SETCONFIG_RSP_EVT,
  AVDT_SCB_API_SETCONFIG_REJ_EVT,
  AVDT_SCB_API_OPEN_RSP_EVT,
  AVDT_SCB_API_CLOSE_RSP_EVT,
  AVDT_SCB_API_RECONFIG_RSP_EVT,
  AVDT_SCB_API_SECURITY_RSP_EVT,
  AVDT_SCB_API_ABORT_RSP_EVT,
  AVDT_SCB_MSG_SETCONFIG_CMD_EVT,
  AVDT_SCB_MSG_GETCONFIG_CMD_EVT,
  AVDT_SCB_MSG_OPEN_CMD_EVT,
  AVDT_SCB_MSG_START_CMD_EVT,
  AVDT_SCB_MSG_SUSPEND_CMD_EVT,
  AVDT_SCB_MSG_CLOSE_CMD_EVT,
  AVDT_SCB_MSG_ABORT_CMD_EVT,
  AVDT_SCB_MSG_RECONFIG_CMD_EVT,
  AVDT_SCB_MSG_SECURITY_CMD_EVT,
  AVDT_SCB_MSG_DELAY_RPT_CMD_EVT,
  AVDT_SCB_MSG_DELAY_RPT_RSP_EVT,
  AVDT_SCB_MSG_SETCONFIG_RSP_EVT,
  AVDT_SCB_MSG_GETCONFIG_RSP_EVT,
  AVDT_SCB_MSG_OPEN_RSP_EVT,
  AVDT_SCB_MSG_START_RSP_EVT,
  AVDT_SCB_MSG_SUSPEND_RSP_EVT,
  AVDT_SCB_MSG_CLOSE_RSP_EVT,
  AVDT_SCB_MSG_ABORT_RSP_EVT,
  AVDT_SCB_MSG_RECONFIG_RSP_EVT,
  AVDT_SCB_MSG_SECURITY_RSP_EVT,
  AVDT_SCB_MSG_SETCONFIG_REJ_EVT,
  AVDT_SCB_MSG_OPEN_REJ_EVT,
  AVDT_SCB_MSG_START_REJ_EVT,
  AVDT_SCB_MSG_SUSPEND_REJ_EVT,
  AVDT_SCB_TC_TOUT_EVT,
  AVDT_SCB_TC_OPEN_EVT,
  AVDT_SCB_TC_CLOSE_EVT,
  AVDT_SCB_TC_CONG_EVT,
  AVDT_SCB_TC_DATA_EVT,
  AVDT_SCB_CC_CLOSE_EVT
};

/* adaption layer number of stream routing table entries */
/* 2 channels(1 media, 1 report) for each SEP and one for signalling */
#define AVDT_NUM_RT_TBL (AVDT_NUM_SEPS * AVDT_CHAN_NUM_TYPES + 1)

/* adaption layer number of transport channel table entries - moved to target.h
#define AVDT_NUM_TC_TBL     (AVDT_NUM_SEPS + AVDT_NUM_LINKS) */

/* "states" used in transport channel table */
#define AVDT_AD_ST_UNUSED 0  /* Unused - unallocated */
#define AVDT_AD_ST_IDLE 1    /* No connection */
#define AVDT_AD_ST_ACP 2     /* Waiting to accept a connection */
#define AVDT_AD_ST_INT 3     /* Initiating a connection */
#define AVDT_AD_ST_CONN 4    /* Waiting for connection confirm */
#define AVDT_AD_ST_CFG 5     /* Waiting for configuration complete */
#define AVDT_AD_ST_OPEN 6    /* Channel opened */
#define AVDT_AD_ST_SEC_INT 7 /* Security process as INT */
#define AVDT_AD_ST_SEC_ACP 8 /* Security process as ACP */

/* Configuration flags. AvdtpTransportChannel.cfg_flags */
#define AVDT_L2C_CFG_IND_DONE (1 << 0)
#define AVDT_L2C_CFG_CFM_DONE (1 << 1)
#define AVDT_L2C_CFG_CONN_INT (1 << 2)
#define AVDT_L2C_CFG_CONN_ACP (1 << 3)

/* result code for avdt_ad_write_req() (L2CA_DataWrite()) */
#define AVDT_AD_FAILED L2CAP_DW_FAILED       /* FALSE */
#define AVDT_AD_SUCCESS L2CAP_DW_SUCCESS     /* TRUE */
#define AVDT_AD_CONGESTED L2CAP_DW_CONGESTED /* 2 */

/*****************************************************************************
 * data types
 ****************************************************************************/

/* msg union of all message parameter types */
typedef union {
  tAVDT_EVT_HDR hdr;
  tAVDT_EVT_HDR single;
  tAVDT_SETCONFIG config_cmd;
  tAVDT_CONFIG reconfig_cmd;
  tAVDT_MULTI multi;
  tAVDT_SECURITY security_cmd;
  tAVDT_DISCOVER discover_rsp;
  tAVDT_CONFIG svccap;
  tAVDT_SECURITY security_rsp;
  tAVDT_DELAY_RPT delay_rpt_cmd;
} tAVDT_MSG;

/* data type for AVDT_CCB_API_DISCOVER_REQ_EVT */
typedef struct {
  tAVDT_CTRL_CBACK* p_cback;
  tAVDT_SEP_INFO* p_sep_info;
  uint8_t num_seps;
} tAVDT_CCB_API_DISCOVER;

/* data type for AVDT_CCB_API_GETCAP_REQ_EVT */
typedef struct {
  tAVDT_EVT_HDR single;
  tAVDT_CTRL_CBACK* p_cback;
  AvdtpSepConfig* p_cfg;
} tAVDT_CCB_API_GETCAP;

/* data type for AVDT_CCB_API_CONNECT_REQ_EVT */
typedef struct {
  tAVDT_CTRL_CBACK* p_cback;
  uint8_t sec_mask;
} tAVDT_CCB_API_CONNECT;

/* data type for AVDT_CCB_API_DISCONNECT_REQ_EVT */
typedef struct { tAVDT_CTRL_CBACK* p_cback; } tAVDT_CCB_API_DISCONNECT;

/* union associated with ccb state machine events */
typedef union {
  tAVDT_CCB_API_DISCOVER discover;
  tAVDT_CCB_API_GETCAP getcap;
  tAVDT_CCB_API_CONNECT connect;
  tAVDT_CCB_API_DISCONNECT disconnect;
  tAVDT_MSG msg;
  bool llcong;
  uint8_t err_code;
} tAVDT_CCB_EVT;

/* type for AVDT_SCB_API_WRITE_REQ_EVT */
typedef struct {
  BT_HDR* p_buf;
  uint32_t time_stamp;
  uint8_t m_pt;
  tAVDT_DATA_OPT_MASK opt;
} tAVDT_SCB_APIWRITE;

/* type for AVDT_SCB_TC_CLOSE_EVT */
typedef struct {
  uint8_t old_tc_state; /* channel state before closed */
  uint8_t tcid;         /* TCID  */
  uint8_t type;         /* channel type */
} tAVDT_SCB_TC_CLOSE;

/* type for scb event data */
typedef union {
  tAVDT_MSG msg;
  tAVDT_SCB_APIWRITE apiwrite;
  tAVDT_DELAY_RPT apidelay;
  tAVDT_OPEN open;
  tAVDT_SCB_TC_CLOSE close;
  bool llcong;
  BT_HDR* p_pkt;
} tAVDT_SCB_EVT;

class AvdtpCcb;

/**
 * AVDTP Stream Control Block.
 */
class AvdtpScb {
 public:
  AvdtpScb()
      : transport_channel_timer(nullptr),
        p_pkt(nullptr),
        p_ccb(nullptr),
        media_seq(0),
        allocated(false),
        in_use(false),
        role(0),
        remove(false),
        state(0),
        peer_seid(0),
        curr_evt(0),
        cong(false),
        close_code(0),
        scb_handle_(0) {}

  /**
   * Allocate the entry for usage.
   * Previous state will be reset and initialized.
   *
   * @param p_avdtp_ccb the AvdtCcb entry to use
   * @param avdtp_stream_config the stream config to use
   */
  void Allocate(AvdtpCcb* p_avdtp_ccb,
                const AvdtpStreamConfig& avdtp_stream_config);

  /**
   * Recycle the entry by resetting it, mark it as allocated and keeping
   * the following state:
   *  - stream_config
   *  - p_ccb
   */
  void Recycle() {
    AvdtpStreamConfig stream_config_saved = stream_config;
    AvdtpCcb* p_ccb_saved = p_ccb;
    Allocate(p_ccb_saved, stream_config_saved);
  }

  /**
   * Reset all the state.
   *
   * @param scb_handle the AVDTP SCB handle to use
   */
  void Reset(uint8_t scb_handle) {
    stream_config.Reset();
    curr_cfg.Reset();
    req_cfg.Reset();

    alarm_free(transport_channel_timer);
    transport_channel_timer = nullptr;

    p_pkt = nullptr;
    p_ccb = nullptr;
    media_seq = 0;
    allocated = false;
    in_use = false;
    role = 0;
    remove = false;
    state = 0;
    peer_seid = 0;
    curr_evt = 0;
    cong = false;
    close_code = 0;
    scb_handle_ = scb_handle;
  }

  /**
   * Get the AVDTP SCB handle for this entry.
   */
  uint8_t ScbHandle() const { return scb_handle_; }

  AvdtpStreamConfig stream_config;   // Stream configuration
  AvdtpSepConfig curr_cfg;           // Current configuration
  AvdtpSepConfig req_cfg;            // Requested configuration
  alarm_t* transport_channel_timer;  // Transport channel connect timer
  BT_HDR* p_pkt;                     // Packet waiting to be sent
  AvdtpCcb* p_ccb;                   // CCB associated with this SCB
  uint16_t media_seq;                // Media packet sequence number
  bool allocated;                    // True if the SCB is allocated
  bool in_use;                       // True if used by peer
  uint8_t role;        // Initiator/acceptor role in current procedure
  bool remove;         // True if the SCB is marked for removal
  uint8_t state;       // State machine state
  uint8_t peer_seid;   // SEID of peer stream
  uint8_t curr_evt;    // current event; set only by the state machine
  bool cong;           // True if the media transport channel is congested
  uint8_t close_code;  // Error code received in close response

 private:
  uint8_t scb_handle_;  // Unique handle for this AvdtpScb entry
};

/**
 * AVDTP Channel Control Block.
 */
class AvdtpCcb {
 public:
  AvdtpCcb()
      : peer_addr(RawAddress::kEmpty),
        scb{},
        idle_ccb_timer(nullptr),
        ret_ccb_timer(nullptr),
        rsp_ccb_timer(nullptr),
        cmd_q(nullptr),
        rsp_q(nullptr),
        proc_cback(nullptr),
        p_conn_cback(nullptr),
        p_proc_data(nullptr),
        p_curr_cmd(nullptr),
        p_curr_msg(nullptr),
        p_rx_msg(nullptr),
        allocated(false),
        state(0),
        ll_opened(false),
        proc_busy(false),
        proc_param(0),
        cong(false),
        label(0),
        reconn(false),
        ret_count(0),
        bta_av_scb_index_(0) {}

  /**
   * Allocate the entry for usage.
   *
   * NOTE: The corresponding AvdtpScb entries are allocated independently.
   * @param peer_address the peer address
   */
  void Allocate(const RawAddress& peer_address);

  /**
   * Reset all the state.
   *
   * @param bta_av_scb_index the BTA AV SCB index to use
   */
  void Reset(uint8_t bta_av_scb_index) {
    bta_av_scb_index_ = bta_av_scb_index;
    ResetCcb();
    for (size_t i = 0; i < AVDT_NUM_SEPS; i++) {
      scb[i].Reset(0);
    }
  }

  /**
   * Reset only the Channel Control Block state without the Stream
   * Control Block entries. The bta_av_scb_index_ is also preserved.
   */
  void ResetCcb() {
    peer_addr = RawAddress::kEmpty;

    alarm_free(idle_ccb_timer);
    idle_ccb_timer = nullptr;

    alarm_free(ret_ccb_timer);
    ret_ccb_timer = nullptr;

    alarm_free(rsp_ccb_timer);
    rsp_ccb_timer = nullptr;

    fixed_queue_free(cmd_q, nullptr);
    cmd_q = nullptr;

    fixed_queue_free(rsp_q, nullptr);
    rsp_q = nullptr;

    proc_cback = nullptr;
    p_conn_cback = nullptr;
    p_proc_data = nullptr;
    p_curr_cmd = nullptr;
    p_curr_msg = nullptr;
    p_rx_msg = nullptr;
    allocated = false;
    state = 0;
    ll_opened = false;
    proc_busy = false;
    proc_param = 0;
    cong = false;
    label = 0;
    reconn = false;
    ret_count = 0;
  }

  /**
   * Get the corresponding BTA AV stream control block index for this entry.
   */
  uint8_t BtaAvScbIndex() const { return bta_av_scb_index_; }

  RawAddress peer_addr;         // Bluetooth address of peer
  AvdtpScb scb[AVDT_NUM_SEPS];  // The AVDTP stream control blocks

  /*
   * NOTE: idle_ccb_timer, ret_ccb_timer and rsp_ccb_timer are mutually
   * exclusive - no more than one timer should be running at the same time.
   */
  alarm_t* idle_ccb_timer;  // Idle CCB timer entry
  alarm_t* ret_ccb_timer;   // Ret CCB timer entry
  alarm_t* rsp_ccb_timer;   // Rsp CCB timer entry
  fixed_queue_t* cmd_q;     // Queue for outgoing command messages
  fixed_queue_t* rsp_q;     // Queue for outgoing response and reject messages
  tAVDT_CTRL_CBACK* proc_cback;    // Procedure callback function
  tAVDT_CTRL_CBACK* p_conn_cback;  // Connection/disconnection callback function
  void* p_proc_data;               // Pointer to data storage for procedure
  BT_HDR* p_curr_cmd;  // Current command being sent awaiting response
  BT_HDR* p_curr_msg;  // Current message being sent
  BT_HDR* p_rx_msg;    // Current message being received
  bool allocated;      // Whether ccb is allocated
  uint8_t state;       // The CCB state machine state
  bool ll_opened;      // True if LL is opened
  bool proc_busy;      // True when a discover or get capabilities procedure in
                       // progress
  uint8_t proc_param;  // Procedure parameter; either SEID for get capabilities
                       // or number of SEPS for discover
  bool cong;           // True if the signaling channel is congested
  uint8_t label;       // Message header "label" (sequence number)
  bool reconn;        // If true, reinitiate connection after transitioning from
                      // CLOSING to IDLE state
  uint8_t ret_count;  // Command retransmission count

 private:
  // The corresponding BTA AV stream control block index for this entry
  uint8_t bta_av_scb_index_;
};

/**
 * AVDTP transport channel entry.
 * Used in the transport channel table in the adaptation layer.
 */
class AvdtpTransportChannel {
 public:
  AvdtpTransportChannel()
      : peer_mtu(0),
        my_mtu(0),
        my_flush_to(0),
        lcid(0),
        tcid(0),
        ccb_idx(0),
        state(0),
        cfg_flags(0),
        id(0) {}

  void Reset() {
    peer_mtu = 0;
    my_mtu = 0;
    my_flush_to = 0;
    lcid = 0;
    tcid = 0;
    ccb_idx = 0;
    state = 0;
    cfg_flags = 0;
    id = 0;
  }

  uint16_t peer_mtu;     // L2CAP MTU of the peer device
  uint16_t my_mtu;       // Our MTU for this channel
  uint16_t my_flush_to;  // Our flush timeout for this channel
  uint16_t lcid;
  uint8_t tcid;       // Transport channel ID
  uint8_t ccb_idx;    // Channel control block for with this transport channel
  uint8_t state;      // Transport channel state
  uint8_t cfg_flags;  // L2CAP configuration flags
  uint8_t id;
};

/**
 * AVDTP stream routing entry.
 * Used in the routing table in the adaption layer.
 */
class AvdtpRoutingEntry {
 public:
  AvdtpRoutingEntry() : lcid(0), scb_hdl(0) {}

  void Reset() {
    lcid = 0;
    scb_hdl = 0;
  }

  uint16_t lcid;    // L2CAP LCID of the associated transport channel
  uint8_t scb_hdl;  // Stream control block for this transport channel
};

/**
 * AVDTP adaption layer control block.
 */
class AvdtpAdaptationLayer {
 public:
  AvdtpAdaptationLayer() : lcid_tbl{} {}

  void Reset() {
    for (size_t i = 0; i < AVDT_NUM_LINKS; i++) {
      for (size_t j = 0; j < AVDT_NUM_RT_TBL; j++) {
        rt_tbl[i][j].Reset();
      }
    }
    for (size_t i = 0; i < AVDT_NUM_TC_TBL; i++) {
      tc_tbl[i].Reset();
    }
    memset(lcid_tbl, 0, sizeof(lcid_tbl));
  }

  /**
   * Lookup AvdtpScb entry for a transport channel.
   *
   * @param tc the transport channel
   * @return the corresponding AvdtpScb entry or null of the transport
   * channel is invalid.
   */
  AvdtpScb* LookupAvdtpScb(const AvdtpTransportChannel& tc);

  AvdtpRoutingEntry rt_tbl[AVDT_NUM_LINKS][AVDT_NUM_RT_TBL];
  AvdtpTransportChannel tc_tbl[AVDT_NUM_TC_TBL];
  uint8_t lcid_tbl[MAX_L2CAP_CHANNELS];  // Map LCID to tc_tbl index
};

/**
 * Types for action functions.
 */
typedef void (*tAVDT_CCB_ACTION)(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
typedef void (*tAVDT_SCB_ACTION)(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);

/**
 * Control block for AVDTP.
 */
class AvdtpCb {
 public:
  AvdtpCb()
      : p_conf_cback(nullptr),
        p_ccb_act(nullptr),
        p_scb_act(nullptr),
        p_conn_cback(nullptr),
        trace_level_(0) {}

  void Reset() {
    rcb.Reset();
    for (size_t i = 0; i < AVDT_NUM_LINKS; i++) {
      ccb[i].Reset(i);
    }
    ad.Reset();
    p_conf_cback = nullptr;
    p_ccb_act = nullptr;
    p_scb_act = nullptr;
    p_conn_cback = nullptr;
    trace_level_ = 0;
  }

  AvdtpRcb rcb;                       // Registration control block
  AvdtpCcb ccb[AVDT_NUM_LINKS];       // Channel control blocks
  AvdtpAdaptationLayer ad;            // Adaption layer control block
  tAVDTC_CTRL_CBACK* p_conf_cback;    // Conformance callback function
  const tAVDT_CCB_ACTION* p_ccb_act;  // Pointer to CCB action functions
  const tAVDT_SCB_ACTION* p_scb_act;  // Pointer to SCB action functions
  tAVDT_CTRL_CBACK* p_conn_cback;     // Connection callback function

  /**
   * Compute the SCB handle for a given AvdtpScb entry.
   *
   * @param p_scb the entry to use
   * @return the computed SCB handle or 0 if the entry is invalid.
   */
  uint8_t ComputeScbHandle(const AvdtpScb* p_scb) const {
    uint8_t scb_handle = 0;

    // Find the entry and in the process compute the unique index
    // TODO: This mechanism is sub-efficient and should be refactored.
    for (size_t i = 0; i < AVDT_NUM_LINKS; i++) {
      for (size_t j = 0; j < AVDT_NUM_SEPS; j++) {
        scb_handle++;
        if (&ccb[i].scb[j] == p_scb) {
          return scb_handle;
        }
      }
    }
    return 0;  // Not found
  }

  /**
   * Get the current trace level used for logging.
   *
   * @return the current trace level
   */
  uint8_t TraceLevel() const { return trace_level_; }

  /**
   * Set the current trace level used for logging.
   *
   * @param trace_level the trace level to set. Should be in the range [1, 6].
   */
  void SetTraceLevel(uint8_t trace_level) { trace_level_ = trace_level; }

 private:
  uint8_t trace_level_; /* trace level */
};

/*****************************************************************************
 * function declarations
 ****************************************************************************/

/* CCB function declarations */
extern void avdt_ccb_init(void);
extern void avdt_ccb_event(AvdtpCcb* p_ccb, uint8_t event,
                           tAVDT_CCB_EVT* p_data);
extern AvdtpCcb* avdt_ccb_by_bd(const RawAddress& bd_addr);
extern AvdtpCcb* avdt_ccb_alloc(const RawAddress& bd_addr);
extern AvdtpCcb* avdt_ccb_alloc_by_channel_index(const RawAddress& bd_addr,
                                                 uint8_t channel_index);
extern void avdt_ccb_dealloc(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern uint8_t avdt_ccb_to_idx(AvdtpCcb* p_ccb);
extern AvdtpCcb* avdt_ccb_by_idx(uint8_t idx);

/* CCB action functions */
extern void avdt_ccb_chan_open(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_chan_close(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_chk_close(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_discover_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_discover_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_getcap_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_getcap_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_start_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_start_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_suspend_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_hdl_suspend_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_discover_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_discover_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_getcap_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_getcap_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_start_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_start_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_suspend_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_suspend_rsp(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_clear_cmds(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_cmd_fail(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_free_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_cong_state(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_ret_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_cmd(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_snd_msg(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_set_reconn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_clr_reconn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_chk_reconn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_chk_timer(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_set_conn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_set_disconn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_do_disconn(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_ll_closed(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);
extern void avdt_ccb_ll_opened(AvdtpCcb* p_ccb, tAVDT_CCB_EVT* p_data);

/* SCB function prototypes */
extern void avdt_scb_event(AvdtpScb* p_scb, uint8_t event,
                           tAVDT_SCB_EVT* p_data);
extern void avdt_scb_init(void);
extern AvdtpScb* avdt_scb_alloc(uint8_t peer_id,
                                const AvdtpStreamConfig& avdtp_stream_config);
extern void avdt_scb_dealloc(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern uint8_t avdt_scb_to_hdl(AvdtpScb* p_scb);
extern AvdtpScb* avdt_scb_by_hdl(uint8_t hdl);
extern uint8_t avdt_scb_verify(AvdtpCcb* p_ccb, uint8_t state, uint8_t* p_seid,
                               uint16_t num_seid, uint8_t* p_err_code);
extern void avdt_scb_peer_seid_list(tAVDT_MULTI* p_multi);
extern uint32_t avdt_scb_gen_ssrc(AvdtpScb* p_scb);

/* SCB action functions */
extern void avdt_scb_hdl_abort_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_abort_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_close_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_close_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_getconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_getconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_open_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_open_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_open_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_drop_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_reconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_reconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_security_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_security_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_setconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_setconfig_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_setconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_start_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_start_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_suspend_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_suspend_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_delay_rpt_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_delay_rpt_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_delay_rpt_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_tc_close(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_tc_open(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_tc_close_sto(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_tc_open_sto(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_hdl_write_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_abort_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_abort_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_close_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_stream_close(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_close_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_getconfig_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_getconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_open_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_open_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_reconfig_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_reconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_security_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_security_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_setconfig_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_setconfig_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_setconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_snd_tc_close(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_cb_err(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_cong_state(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_rej_state(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_rej_in_use(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_rej_not_in_use(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_set_remove(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_free_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_chk_snd_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_clr_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);
extern void avdt_scb_transport_channel_timer(AvdtpScb* p_scb,
                                             tAVDT_SCB_EVT* p_data);
extern void avdt_scb_clr_vars(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data);

/* msg function declarations */
extern bool avdt_msg_send(AvdtpCcb* p_ccb, BT_HDR* p_msg);
extern void avdt_msg_send_cmd(AvdtpCcb* p_ccb, void* p_scb, uint8_t sig_id,
                              tAVDT_MSG* p_params);
extern void avdt_msg_send_rsp(AvdtpCcb* p_ccb, uint8_t sig_id,
                              tAVDT_MSG* p_params);
extern void avdt_msg_send_rej(AvdtpCcb* p_ccb, uint8_t sig_id,
                              tAVDT_MSG* p_params);
extern void avdt_msg_send_grej(AvdtpCcb* p_ccb, uint8_t sig_id,
                               tAVDT_MSG* p_params);
extern void avdt_msg_ind(AvdtpCcb* p_ccb, BT_HDR* p_buf);

/* adaption layer function declarations */
extern void avdt_ad_init(void);
extern uint8_t avdt_ad_type_to_tcid(uint8_t type, AvdtpScb* p_scb);
extern AvdtpTransportChannel* avdt_ad_tc_tbl_by_st(uint8_t type,
                                                   AvdtpCcb* p_ccb,
                                                   uint8_t state);
extern AvdtpTransportChannel* avdt_ad_tc_tbl_by_lcid(uint16_t lcid);
extern AvdtpTransportChannel* avdt_ad_tc_tbl_alloc(AvdtpCcb* p_ccb);
extern uint8_t avdt_ad_tc_tbl_to_idx(AvdtpTransportChannel* p_tbl);
extern void avdt_ad_tc_close_ind(AvdtpTransportChannel* p_tbl, uint16_t reason);
extern void avdt_ad_tc_open_ind(AvdtpTransportChannel* p_tbl);
extern void avdt_ad_tc_cong_ind(AvdtpTransportChannel* p_tbl,
                                bool is_congested);
extern void avdt_ad_tc_data_ind(AvdtpTransportChannel* p_tbl, BT_HDR* p_buf);
extern AvdtpTransportChannel* avdt_ad_tc_tbl_by_type(uint8_t type,
                                                     AvdtpCcb* p_ccb,
                                                     AvdtpScb* p_scb);
extern uint8_t avdt_ad_write_req(uint8_t type, AvdtpCcb* p_ccb, AvdtpScb* p_scb,
                                 BT_HDR* p_buf);
extern void avdt_ad_open_req(uint8_t type, AvdtpCcb* p_ccb, AvdtpScb* p_scb,
                             uint8_t role);
extern void avdt_ad_close_req(uint8_t type, AvdtpCcb* p_ccb, AvdtpScb* p_scb);

extern void avdt_ccb_idle_ccb_timer_timeout(void* data);
extern void avdt_ccb_ret_ccb_timer_timeout(void* data);
extern void avdt_ccb_rsp_ccb_timer_timeout(void* data);
extern void avdt_scb_transport_channel_timer_timeout(void* data);

/*****************************************************************************
 * macros
 ****************************************************************************/

/* we store the scb and the label in the layer_specific field of the
 * current cmd
*/
#define AVDT_BLD_LAYERSPEC(ls, msg, label) ls = (((label) << 4) | (msg))

#define AVDT_LAYERSPEC_LABEL(ls) ((uint8_t)((ls) >> 4))

#define AVDT_LAYERSPEC_MSG(ls) ((uint8_t)((ls)&0x000F))

/*****************************************************************************
 * global data
 ****************************************************************************/

/******************************************************************************
 * Main Control Block
 ******************************************************************************/
extern AvdtpCb avdtp_cb;

/* L2CAP callback registration structure */
extern const tL2CAP_APPL_INFO avdt_l2c_appl;

/* reject message event lookup table */
extern const uint8_t avdt_msg_rej_2_evt[];

#endif /* AVDT_INT_H */
