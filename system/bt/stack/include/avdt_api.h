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
 *  This interface file contains the interface to the Audio Video
 *  Distribution Transport Protocol (AVDTP).
 *
 ******************************************************************************/
#ifndef AVDT_API_H
#define AVDT_API_H

#include "bt_target.h"
#include "bt_types.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/
#define AVDT_VERSION_1_3 0x0103

#define AVDTP_VERSION_CONFIG_KEY "AvdtpVersion"

/* Maximum size in bytes of the codec capabilities information element. */
#define AVDT_CODEC_SIZE 20

/* API function return value result codes. */
#define AVDT_SUCCESS 0      /* Function successful */
#define AVDT_BAD_PARAMS 1   /* Invalid parameters */
#define AVDT_NO_RESOURCES 2 /* Not enough resources */
#define AVDT_BAD_HANDLE 3   /* Bad handle */
#define AVDT_BUSY 4         /* A procedure is already in progress */
#define AVDT_WRITE_FAIL 5   /* Write failed */

/* The index to access the codec type in codec_info[]. */
#define AVDT_CODEC_TYPE_INDEX 2

/* The size in bytes of a Adaptation Layer header. */
#define AVDT_AL_HDR_SIZE 3

/* The size in bytes of a media packet header. */
#define AVDT_MEDIA_HDR_SIZE 12

/* The handle is used when reporting MULTI_AV specific events */
#define AVDT_MULTI_AV_HANDLE 0xFF

/* The number of bytes needed by the protocol stack for the protocol headers
 * of a media packet.  This is the size of the media packet header, the
 * L2CAP packet header and HCI header.
*/
#define AVDT_MEDIA_OFFSET 23

/* The marker bit is used by the application to mark significant events such
 * as frame boundaries in the data stream.  This constant is used to check or
 * set the marker bit in the m_pt parameter of an AVDT_WriteReq()
 * or AVDT_DATA_IND_EVT.
*/
#define AVDT_MARKER_SET 0x80

#define MAX_2MBPS_AVDTP_MTU 663   // 2DH5 MTU=679, -12 for AVDTP, -4 for L2CAP
#define MAX_3MBPS_AVDTP_MTU 1005  // 3DH5 MTU=1021, -12 for AVDTP, -4 for L2CAP

/* SEP Type.  This indicates the stream endpoint type. */
#define AVDT_TSEP_SRC 0     /* Source SEP */
#define AVDT_TSEP_SNK 1     /* Sink SEP */
#define AVDT_TSEP_INVALID 3 /* Invalid SEP */

/* initiator/acceptor role for adaption */
#define AVDT_INT 0 /* initiator */
#define AVDT_ACP 1 /* acceptor */

/* Media Type of the stream endpoint */
/* The value does not include the reserved 4-bit LSBs field */
#define AVDT_MEDIA_TYPE_AUDIO 0 /* Audio SEP */
#define AVDT_MEDIA_TYPE_VIDEO 1 /* Video SEP */
#define AVDT_MEDIA_TYPE_MULTI 2 /* Multimedia SEP */

/* for reporting packets (packet types) */
#define AVDT_RTCP_PT_SR 200   /* SR (Sender Report) */
#define AVDT_RTCP_PT_RR 201   /* RR (Receiver Report) */
#define AVDT_RTCP_PT_SDES 202 /* SDES (Source Description) */
typedef uint8_t AVDT_REPORT_TYPE;

#define AVDT_RTCP_SDES_CNAME 1 /* SDES item CNAME */
#ifndef AVDT_MAX_CNAME_SIZE
#define AVDT_MAX_CNAME_SIZE 28
#endif

/* Protocol service capabilities.  This indicates the protocol service
 * capabilities of a stream endpoint.  This value is a mask.
 * Multiple values can be combined with a bitwise OR.
*/
#define AVDT_PSC_TRANS (1 << 1)     /* Media transport */
#define AVDT_PSC_REPORT (1 << 2)    /* Reporting */
#define AVDT_PSC_RECOV (1 << 3)     /* Recovery */
#define AVDT_PSC_HDRCMP (1 << 5)    /* Header compression */
#define AVDT_PSC_MUX (1 << 6)       /* Multiplexing */
#define AVDT_PSC_DELAY_RPT (1 << 8) /* Delay Report */

/* Recovery type.  This indicates the recovery type. */
#define AVDT_RECOV_RFC2733 1 /* RFC2733 recovery */

/* Header compression capabilities.  This indicates the header compression
 * capabilities.  This value is a mask.  Multiple values can be combined
 * with a bitwise OR.
*/
#define AVDT_HDRCMP_MEDIA (1 << 5)  /* Available for media packets */
#define AVDT_HDRCMP_RECOV (1 << 6)  /* Available for recovery packets */
#define AVDT_HDRCMP_BACKCH (1 << 7) /* Back channel supported */

/* Multiplexing capabilities mask. */
#define AVDT_MUX_FRAG (1 << 7) /* Allow Adaptation Layer Fragmentation */

/* Application service category. This indicates the application
 * service category.
*/
#define AVDT_ASC_PROTECT 4 /* Content protection */
#define AVDT_ASC_CODEC 7   /* Codec */

/* the content protection IDs assigned by BT SIG */
#define AVDT_CP_SCMS_T_ID 0x0002
#define AVDT_CP_DTCP_ID 0x0001

#define AVDT_CP_LOSC 2
#define AVDT_CP_INFO_LEN 3

#define AVDT_CP_SCMS_COPY_MASK 3
#define AVDT_CP_SCMS_COPY_FREE 2
#define AVDT_CP_SCMS_COPY_ONCE 1
#define AVDT_CP_SCMS_COPY_NEVER 0

/* Error codes.  The following are error codes defined in the AVDTP and GAVDP
 * specifications.  These error codes communicate protocol errors between
 * AVDTP and the application.  More detailed descriptions of the error codes
 * and their appropriate use can be found in the AVDTP and GAVDP specifications.
 * These error codes are unrelated to the result values returned by the
 * AVDTP API functions.
*/
/* Bad packet header format */
#define AVDT_ERR_HEADER 0x01
/* Bad packet length */
#define AVDT_ERR_LENGTH 0x11
/* Invalid SEID */
#define AVDT_ERR_SEID 0x12
/* The SEP is in use */
#define AVDT_ERR_IN_USE 0x13
/* The SEP is not in use */
#define AVDT_ERR_NOT_IN_USE 0x14
/* Bad service category */
#define AVDT_ERR_CATEGORY 0x17
/* Bad payload format */
#define AVDT_ERR_PAYLOAD 0x18
/* Requested command not supported */
#define AVDT_ERR_NSC 0x19
/* Reconfigure attempted invalid capabilities */
#define AVDT_ERR_INVALID_CAP 0x1A
/* Requested recovery type not defined */
#define AVDT_ERR_RECOV_TYPE 0x22
/* Media transport capability not correct */
#define AVDT_ERR_MEDIA_TRANS 0x23
/* Recovery service capability not correct */
#define AVDT_ERR_RECOV_FMT 0x25
/* Header compression service capability not correct */
#define AVDT_ERR_ROHC_FMT 0x26
/* Content protection service capability not correct */
#define AVDT_ERR_CP_FMT 0x27
/* Multiplexing service capability not correct */
#define AVDT_ERR_MUX_FMT 0x28
/* Configuration not supported */
#define AVDT_ERR_UNSUP_CFG 0x29
/* Message cannot be processed in this state */
#define AVDT_ERR_BAD_STATE 0x31
/* Report service capability not correct */
#define AVDT_ERR_REPORT_FMT 0x65
/* Invalid service category */
#define AVDT_ERR_SERVICE 0x80
/* Insufficient resources */
#define AVDT_ERR_RESOURCE 0x81
/* Invalid Media Codec Type */
#define AVDT_ERR_INVALID_MCT 0xC1
/* Unsupported Media Codec Type */
#define AVDT_ERR_UNSUP_MCT 0xC2
/* Invalid Level */
#define AVDT_ERR_INVALID_LEVEL 0xC3
/* Unsupported Level */
#define AVDT_ERR_UNSUP_LEVEL 0xC4
/* Invalid Content Protection Type */
#define AVDT_ERR_INVALID_CP 0xE0
/* Invalid Content Protection format */
#define AVDT_ERR_INVALID_FORMAT 0xE1

/* Additional error codes.  This indicates error codes used by AVDTP
 * in addition to the ones defined in the specifications.
*/
#define AVDT_ERR_CONNECT 0x07 /* Connection failed. */
#define AVDT_ERR_TIMEOUT 0x08 /* Response timeout. */

/* Control callback events. */
#define AVDT_DISCOVER_CFM_EVT 0      /* Discover confirm */
#define AVDT_GETCAP_CFM_EVT 1        /* Get capabilities confirm */
#define AVDT_OPEN_CFM_EVT 2          /* Open confirm */
#define AVDT_OPEN_IND_EVT 3          /* Open indication */
#define AVDT_CONFIG_IND_EVT 4        /* Configuration indication */
#define AVDT_START_CFM_EVT 5         /* Start confirm */
#define AVDT_START_IND_EVT 6         /* Start indication */
#define AVDT_SUSPEND_CFM_EVT 7       /* Suspend confirm */
#define AVDT_SUSPEND_IND_EVT 8       /* Suspend indication */
#define AVDT_CLOSE_CFM_EVT 9         /* Close confirm */
#define AVDT_CLOSE_IND_EVT 10        /* Close indication */
#define AVDT_RECONFIG_CFM_EVT 11     /* Reconfiguration confirm */
#define AVDT_RECONFIG_IND_EVT 12     /* Reconfiguration indication */
#define AVDT_SECURITY_CFM_EVT 13     /* Security confirm */
#define AVDT_SECURITY_IND_EVT 14     /* Security indication */
#define AVDT_WRITE_CFM_EVT 15        /* Write confirm */
#define AVDT_CONNECT_IND_EVT 16      /* Signaling channel connected */
#define AVDT_DISCONNECT_IND_EVT 17   /* Signaling channel disconnected */
#define AVDT_REPORT_CONN_EVT 18      /* Reporting channel connected */
#define AVDT_REPORT_DISCONN_EVT 19   /* Reporting channel disconnected */
#define AVDT_DELAY_REPORT_EVT 20     /* Delay report received */
#define AVDT_DELAY_REPORT_CFM_EVT 21 /* Delay report response received */

#define AVDT_MAX_EVT (AVDT_DELAY_REPORT_CFM_EVT)

/* PSM for AVDT */
#define AVDT_PSM 0x0019

/*****************************************************************************
 *  Type Definitions
 ****************************************************************************/

typedef struct {
  uint32_t ntp_sec; /* NTP time: seconds relative to 0h UTC on 1 January 1900 */
  uint32_t ntp_frac;    /* NTP time: the fractional part */
  uint32_t rtp_time;    /* timestamp in RTP header */
  uint32_t pkt_count;   /* sender's packet count: since starting transmission
                        * up until the time this SR packet was generated. */
  uint32_t octet_count; /* sender's octet count: same comment */
} tAVDT_SENDER_INFO;

typedef struct {
  uint8_t frag_lost; /* fraction lost since last RR */
  uint32_t
      packet_lost; /* cumulative number of packets lost since the beginning */
  uint32_t seq_num_rcvd; /* extended highest sequence number received */
  uint32_t jitter;       /* interarrival jitter */
  uint32_t lsr;          /* last SR timestamp */
  uint32_t dlsr;         /* delay since last SR */
} tAVDT_REPORT_BLK;

typedef union {
  tAVDT_SENDER_INFO sr;
  tAVDT_REPORT_BLK rr;
  uint8_t cname[AVDT_MAX_CNAME_SIZE + 1];
} tAVDT_REPORT_DATA;

/**
 * AVDTP Registration Control Block.
 */
class AvdtpRcb {
 public:
  AvdtpRcb()
      : ctrl_mtu(0),
        ret_tout(0),
        sig_tout(0),
        idle_tout(0),
        sec_mask(0),
        scb_index(0) {}
  AvdtpRcb& operator=(const AvdtpRcb&) = default;

  void Reset() {
    ctrl_mtu = 0;
    ret_tout = 0;
    sig_tout = 0;
    idle_tout = 0;
    sec_mask = 0;
    scb_index = 0;
  }

  uint16_t ctrl_mtu; /* L2CAP MTU of the AVDTP signaling channel */
  uint8_t ret_tout;  /* AVDTP signaling retransmission timeout */
  uint8_t sig_tout;  /* AVDTP signaling message timeout */
  uint8_t idle_tout; /* AVDTP idle signaling channel timeout */
  uint8_t sec_mask;  /* Security mask for BTM_SetSecurityLevel() */
  uint8_t scb_index; /* The Stream Control Block index */
};

/* This structure contains the SEP information.  This information is
 * transferred during the discovery procedure.
*/
typedef struct {
  bool in_use;        /* true if stream is currently in use */
  uint8_t seid;       /* Stream endpoint identifier */
  uint8_t media_type; /* Media type: AVDT_MEDIA_TYPE_* */
  uint8_t tsep;       /* SEP type */
} tAVDT_SEP_INFO;

/**
 * AVDTP SEP Configuration.
 */
class AvdtpSepConfig {
 public:
  AvdtpSepConfig()
      : codec_info{},
        protect_info{},
        num_codec(0),
        num_protect(0),
        psc_mask(0),
        recov_type(0),
        recov_mrws(0),
        recov_mnmp(0),
        hdrcmp_mask(0) {}
  AvdtpSepConfig& operator=(const AvdtpSepConfig&) = default;

  void Reset() {
    memset(codec_info, 0, sizeof(codec_info));
    memset(protect_info, 0, sizeof(protect_info));
    num_codec = 0;
    num_protect = 0;
    psc_mask = 0;
    recov_type = 0;
    recov_mrws = 0;
    recov_mnmp = 0;
    hdrcmp_mask = 0;
  }

  uint8_t codec_info[AVDT_CODEC_SIZE];     /* Codec capabilities array */
  uint8_t protect_info[AVDT_PROTECT_SIZE]; /* Content protection capabilities */
  uint8_t num_codec;   /* Number of media codec information elements */
  uint8_t num_protect; /* Number of content protection information elements */
  uint16_t psc_mask;   /* Protocol service capabilities mask */
  uint8_t recov_type;  /* Recovery type */
  uint8_t recov_mrws;  /* Maximum recovery window size */
  uint8_t recov_mnmp;  /* Recovery maximum number of media packets */
  uint8_t hdrcmp_mask; /* Header compression capabilities */
};

/* Header structure for callback event parameters. */
typedef struct {
  uint8_t
      err_code; /* Zero if operation succeeded; nonzero if operation failed */
  uint8_t err_param; /* Error parameter included for some events */
  uint8_t label;     /* Transaction label */
  uint8_t seid;      /* For internal use only */
  uint8_t sig_id;    /* For internal use only */
  uint8_t ccb_idx;   /* For internal use only */
} tAVDT_EVT_HDR;

/* This data structure is associated with the AVDT_GETCAP_CFM_EVT,
 * AVDT_RECONFIG_IND_EVT, and AVDT_RECONFIG_CFM_EVT.
*/
typedef struct {
  tAVDT_EVT_HDR hdr; /* Event header */
  AvdtpSepConfig* p_cfg; /* Pointer to configuration for this SEP */
} tAVDT_CONFIG;

/* This data structure is associated with the AVDT_CONFIG_IND_EVT. */
typedef struct {
  tAVDT_EVT_HDR hdr; /* Event header */
  AvdtpSepConfig* p_cfg; /* Pointer to configuration for this SEP */
  uint8_t int_seid;  /* Stream endpoint ID of stream initiating the operation */
} tAVDT_SETCONFIG;

/* This data structure is associated with the AVDT_OPEN_IND_EVT and
 * AVDT_OPEN_CFM_EVT. */
typedef struct {
  tAVDT_EVT_HDR hdr; /* Event header */
  uint16_t peer_mtu; /* Transport channel L2CAP MTU of the peer */
  uint16_t lcid;     /* L2CAP LCID for media channel */
} tAVDT_OPEN;

/* This data structure is associated with the AVDT_SECURITY_IND_EVT
 * and AVDT_SECURITY_CFM_EVT.
*/
typedef struct {
  tAVDT_EVT_HDR hdr; /* Event header */
  uint8_t* p_data;   /* Pointer to security data */
  uint16_t len;      /* Length in bytes of the security data */
} tAVDT_SECURITY;

/* This data structure is associated with the AVDT_DISCOVER_CFM_EVT. */
typedef struct {
  tAVDT_EVT_HDR hdr;          /* Event header */
  tAVDT_SEP_INFO* p_sep_info; /* Pointer to SEP information */
  uint8_t num_seps;           /* Number of stream endpoints */
} tAVDT_DISCOVER;

/* This data structure is associated with the AVDT_DELAY_REPORT_EVT. */
typedef struct {
  tAVDT_EVT_HDR hdr; /* Event header */
  uint16_t delay;    /* Delay value */
} tAVDT_DELAY_RPT;

/* Union of all control callback event data structures */
typedef union {
  tAVDT_EVT_HDR hdr;
  tAVDT_DISCOVER discover_cfm;
  tAVDT_CONFIG getcap_cfm;
  tAVDT_OPEN open_cfm;
  tAVDT_OPEN open_ind;
  tAVDT_SETCONFIG config_ind;
  tAVDT_EVT_HDR start_cfm;
  tAVDT_EVT_HDR suspend_cfm;
  tAVDT_EVT_HDR close_cfm;
  tAVDT_CONFIG reconfig_cfm;
  tAVDT_CONFIG reconfig_ind;
  tAVDT_SECURITY security_cfm;
  tAVDT_SECURITY security_ind;
  tAVDT_EVT_HDR connect_ind;
  tAVDT_EVT_HDR disconnect_ind;
  tAVDT_EVT_HDR report_conn;
  tAVDT_DELAY_RPT delay_rpt_cmd;
} tAVDT_CTRL;

/* This is the control callback function.  This function passes control events
 * to the application.  This function is required for all registered stream
 * endpoints and for the AVDT_DiscoverReq() and AVDT_GetCapReq() functions.
 *
*/
typedef void(tAVDT_CTRL_CBACK)(uint8_t handle, const RawAddress& bd_addr,
                               uint8_t event, tAVDT_CTRL* p_data,
                               uint8_t scb_index);

/* This is the data callback function.  It is executed when AVDTP has a media
 * packet ready for the application.  This function is required for SNK
 * endpoints and not applicable for SRC endpoints.
*/
typedef void(tAVDT_SINK_DATA_CBACK)(uint8_t handle, BT_HDR* p_pkt,
                                    uint32_t time_stamp, uint8_t m_pt);

/* This is the report callback function.  It is executed when AVDTP has a
 * reporting packet ready for the application.  This function is required for
 * streams created with AVDT_PSC_REPORT.
*/
typedef void(tAVDT_REPORT_CBACK)(uint8_t handle, AVDT_REPORT_TYPE type,
                                 tAVDT_REPORT_DATA* p_data);

/**
 * AVDTP Stream Configuration.
 * The information is used when a stream is created.
 */
class AvdtpStreamConfig {
 public:
  //
  // Non-supported protocol command messages
  //
  // Suspend command not supported
  static constexpr int AVDT_NSC_SUSPEND = 0x01;
  // Reconfigure command not supported
  static constexpr int AVDT_NSC_RECONFIG = 0x02;
  // Security command not supported
  static constexpr int AVDT_NSC_SECURITY = 0x04;

  AvdtpStreamConfig()
      : p_avdt_ctrl_cback(nullptr),
        scb_index(0),
        p_sink_data_cback(nullptr),
        p_report_cback(nullptr),
        mtu(0),
        flush_to(0),
        tsep(0),
        media_type(0),
        nsc_mask(0) {}

  void Reset() {
    cfg.Reset();
    p_avdt_ctrl_cback = nullptr;
    scb_index = 0;
    p_sink_data_cback = nullptr;
    p_report_cback = nullptr;
    mtu = 0;
    flush_to = 0;
    tsep = 0;
    media_type = 0;
    nsc_mask = 0;
  }

  AvdtpSepConfig cfg;                   // SEP configuration
  tAVDT_CTRL_CBACK* p_avdt_ctrl_cback;  // Control callback function
  uint8_t scb_index;  // The index to the bta_av_cb.p_scb[] entry
  tAVDT_SINK_DATA_CBACK* p_sink_data_cback;  // Sink data callback function
  tAVDT_REPORT_CBACK* p_report_cback;        // Report callback function
  uint16_t mtu;        // The L2CAP MTU of the transport channel
  uint16_t flush_to;   // The L2CAP flush timeout of the transport channel
  uint8_t tsep;        // SEP type
  uint8_t media_type;  // Media type: AVDT_MEDIA_TYPE_*
  uint16_t nsc_mask;   // Nonsupported protocol command messages
};

/* AVDT data option mask is used in the write request */
#define AVDT_DATA_OPT_NONE 0x00          /* No option still add RTP header */
#define AVDT_DATA_OPT_NO_RTP (0x01 << 0) /* Skip adding RTP header */

typedef uint8_t tAVDT_DATA_OPT_MASK;

/*****************************************************************************
 *  External Function Declarations
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         AVDT_Register
 *
 * Description      This is the system level registration function for the
 *                  AVDTP protocol.  This function initializes AVDTP and
 *                  prepares the protocol stack for its use.  This function
 *                  must be called once by the system or platform using AVDTP
 *                  before the other functions of the API an be used.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void AVDT_Register(AvdtpRcb* p_reg, tAVDT_CTRL_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         AVDT_Deregister
 *
 * Description      This function is called to deregister use AVDTP protocol.
 *                  It is called when AVDTP is no longer being used by any
 *                  application in the system.  Before this function can be
 *                  called, all streams must be removed with AVDT_RemoveStream.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void AVDT_Deregister(void);

/*******************************************************************************
 *
 * Function         AVDT_AbortReq
 *
 * Description      Trigger Abort request to pass AVDTP Abort related mandatory
 *                  PTS Test case.
 *
 * Returns          void.
 *
 ******************************************************************************/
extern void AVDT_AbortReq(uint8_t handle);

/*******************************************************************************
 *
 * Function         AVDT_CreateStream
 *
 * Description      Create a stream endpoint.  After a stream endpoint is
 *                  created an application can initiate a connection between
 *                  this endpoint and an endpoint on a peer device.  In
 *                  addition, a peer device can discover, get the capabilities,
 *                  and connect to this endpoint.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_CreateStream(uint8_t peer_id, uint8_t* p_handle,
                                  const AvdtpStreamConfig& avdtp_stream_config);

/*******************************************************************************
 *
 * Function         AVDT_RemoveStream
 *
 * Description      Remove a stream endpoint.  This function is called when
 *                  the application is no longer using a stream endpoint.
 *                  If this function is called when the endpoint is connected
 *                  the connection is closed and then the stream endpoint
 *                  is removed.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_RemoveStream(uint8_t handle);

/*******************************************************************************
 *
 * Function         AVDT_DiscoverReq
 *
 * Description      This function initiates a connection to the AVDTP service
 *                  on the peer device, if not already present, and discovers
 *                  the stream endpoints on the peer device.  (Please note
 *                  that AVDTP discovery is unrelated to SDP discovery).
 *                  This function can be called at any time regardless of
 *                  whether there is an AVDTP connection to the peer device.
 *
 *                  When discovery is complete, an AVDT_DISCOVER_CFM_EVT
 *                  is sent to the application via its callback function.
 *                  The application must not call AVDT_GetCapReq() or
 *                  AVDT_DiscoverReq() again to the same device until
 *                  discovery is complete.
 *
 *                  The memory addressed by sep_info is allocated by the
 *                  application.  This memory is written to by AVDTP as part
 *                  of the discovery procedure.  This memory must remain
 *                  accessible until the application receives the
 *                  AVDT_DISCOVER_CFM_EVT.
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_DiscoverReq(const RawAddress& bd_addr,
                                 uint8_t channel_index,
                                 tAVDT_SEP_INFO* p_sep_info, uint8_t max_seps,
                                 tAVDT_CTRL_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         AVDT_GetCapReq
 *
 * Description      This function initiates a connection to the AVDTP service
 *                  on the peer device, if not already present, and gets the
 *                  capabilities of a stream endpoint on the peer device.
 *                  This function can be called at any time regardless of
 *                  whether there is an AVDTP connection to the peer device.
 *
 *                  When the procedure is complete, an AVDT_GETCAP_CFM_EVT is
 *                  sent to the application via its callback function.  The
 *                  application must not call AVDT_GetCapReq() or
 *                  AVDT_DiscoverReq() again until the procedure is complete.
 *
 *                  The memory pointed to by p_cfg is allocated by the
 *                  application.  This memory is written to by AVDTP as part
 *                  of the get capabilities procedure.  This memory must
 *                  remain accessible until the application receives
 *                  the AVDT_GETCAP_CFM_EVT.
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_GetCapReq(const RawAddress& bd_addr, uint8_t channel_index,
                               uint8_t seid, AvdtpSepConfig* p_cfg,
                               tAVDT_CTRL_CBACK* p_cback, bool get_all_cap);

/*******************************************************************************
 *
 * Function         AVDT_DelayReport
 *
 * Description      This functions sends a Delay Report to the peer device
 *                  that is associated with a particular SEID.
 *                  This function is called by SNK device.
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_DelayReport(uint8_t handle, uint8_t seid, uint16_t delay);

/*******************************************************************************
 *
 * Function         AVDT_OpenReq
 *
 * Description      This function initiates a connection to the AVDTP service
 *                  on the peer device, if not already present, and connects
 *                  to a stream endpoint on a peer device.  When the connection
 *                  is completed, an AVDT_OPEN_CFM_EVT is sent to the
 *                  application via the control callback function for this
 *                  handle.
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_OpenReq(uint8_t handle, const RawAddress& bd_addr,
                             uint8_t channel_index, uint8_t seid,
                             AvdtpSepConfig* p_cfg);

/*******************************************************************************
 *
 * Function         AVDT_ConfigRsp
 *
 * Description      Respond to a configure request from the peer device.  This
 *                  function must be called if the application receives an
 *                  AVDT_CONFIG_IND_EVT through its control callback.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_ConfigRsp(uint8_t handle, uint8_t label,
                               uint8_t error_code, uint8_t category);

/*******************************************************************************
 *
 * Function         AVDT_StartReq
 *
 * Description      Start one or more stream endpoints.  This initiates the
 *                  transfer of media packets for the streams.  All stream
 *                  endpoints must previously be opened.  When the streams
 *                  are started, an AVDT_START_CFM_EVT is sent to the
 *                  application via the control callback function for each
 *                  stream.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_StartReq(uint8_t* p_handles, uint8_t num_handles);

/*******************************************************************************
 *
 * Function         AVDT_SuspendReq
 *
 * Description      Suspend one or more stream endpoints. This suspends the
 *                  transfer of media packets for the streams.  All stream
 *                  endpoints must previously be open and started.  When the
 *                  streams are suspended, an AVDT_SUSPEND_CFM_EVT is sent to
 *                  the application via the control callback function for
 *                  each stream.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_SuspendReq(uint8_t* p_handles, uint8_t num_handles);

/*******************************************************************************
 *
 * Function         AVDT_CloseReq
 *
 * Description      Close a stream endpoint.  This stops the transfer of media
 *                  packets and closes the transport channel associated with
 *                  this stream endpoint.  When the stream is closed, an
 *                  AVDT_CLOSE_CFM_EVT is sent to the application via the
 *                  control callback function for this handle.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_CloseReq(uint8_t handle);

/*******************************************************************************
 *
 * Function         AVDT_ReconfigReq
 *
 * Description      Reconfigure a stream endpoint.  This allows the application
 *                  to change the codec or content protection capabilities of
 *                  a stream endpoint after it has been opened.  This function
 *                  can only be called if the stream is opened but not started
 *                  or if the stream has been suspended.  When the procedure
 *                  is completed, an AVDT_RECONFIG_CFM_EVT is sent to the
 *                  application via the control callback function for this
 *                  handle.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_ReconfigReq(uint8_t handle, AvdtpSepConfig* p_cfg);

/*******************************************************************************
 *
 * Function         AVDT_ReconfigRsp
 *
 * Description      Respond to a reconfigure request from the peer device.
 *                  This function must be called if the application receives
 *                  an AVDT_RECONFIG_IND_EVT through its control callback.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_ReconfigRsp(uint8_t handle, uint8_t label,
                                 uint8_t error_code, uint8_t category);

/*******************************************************************************
 *
 * Function         AVDT_SecurityReq
 *
 * Description      Send a security request to the peer device.  When the
 *                  security procedure is completed, an AVDT_SECURITY_CFM_EVT
 *                  is sent to the application via the control callback function
 *                  for this handle.  (Please note that AVDTP security
 *                  procedures are unrelated to Bluetooth link level security.)
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_SecurityReq(uint8_t handle, uint8_t* p_data, uint16_t len);

/*******************************************************************************
 *
 * Function         AVDT_SecurityRsp
 *
 * Description      Respond to a security request from the peer device.
 *                  This function must be called if the application receives
 *                  an AVDT_SECURITY_IND_EVT through its control callback.
 *                  (Please note that AVDTP security procedures are unrelated
 *                  to Bluetooth link level security.)
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_SecurityRsp(uint8_t handle, uint8_t label,
                                 uint8_t error_code, uint8_t* p_data,
                                 uint16_t len);

/*******************************************************************************
 *
 * Function         AVDT_WriteReq
 *
 * Description      Send a media packet to the peer device.  The stream must
 *                  be started before this function is called.  Also, this
 *                  function can only be called if the stream is a SRC.
 *
 *                  When AVDTP has sent the media packet and is ready for the
 *                  next packet, an AVDT_WRITE_CFM_EVT is sent to the
 *                  application via the control callback.  The application must
 *                  wait for the AVDT_WRITE_CFM_EVT before it makes the next
 *                  call to AVDT_WriteReq().  If the applications calls
 *                  AVDT_WriteReq() before it receives the event the packet
 *                  will not be sent.  The application may make its first call
 *                  to AVDT_WriteReq() after it receives an AVDT_START_CFM_EVT
 *                  or AVDT_START_IND_EVT.
 *
 *                  The application passes the packet using the BT_HDR
 *                  structure.
 *                  This structure is described in section 2.1.  The offset
 *                  field must be equal to or greater than AVDT_MEDIA_OFFSET.
 *                  This allows enough space in the buffer for the L2CAP and
 *                  AVDTP headers.
 *
 *                  The memory pointed to by p_pkt must be a GKI buffer
 *                  allocated by the application.  This buffer will be freed
 *                  by the protocol stack; the application must not free
 *                  this buffer.
 *
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_WriteReq(uint8_t handle, BT_HDR* p_pkt,
                              uint32_t time_stamp, uint8_t m_pt);
/*******************************************************************************
 *
 * Function         AVDT_WriteReqOpt
 *
 * Description      Send a media packet to the peer device.  The stream must
 *                  be started before this function is called.  Also, this
 *                  function can only be called if the stream is a SRC
 *
 *                  When AVDTP has sent the media packet and is ready for the
 *                  next packet, an AVDT_WRITE_CFM_EVT is sent to the
 *                  application via the control callback.  The application must
 *                  wait for the AVDT_WRITE_CFM_EVT before it makes the next
 *                  call to AVDT_WriteReq().  If the applications calls
 *                  AVDT_WriteReq() before it receives the event the packet
 *                  will not be sent.  The application may make its first call
 *                  to AVDT_WriteReq() after it receives an AVDT_START_CFM_EVT
 *                  or AVDT_START_IND_EVT.
 *
 *                  The application passes the packet using the BT_HDR structure
 *                  This structure is described in section 2.1.  The offset
 *                  field must be equal to or greater than AVDT_MEDIA_OFFSET
 *                  (if NO_RTP is specified, L2CAP_MIN_OFFSET can be used)
 *                  This allows enough space in the buffer for the L2CAP and
 *                  AVDTP headers.
 *
 *                  The memory pointed to by p_pkt must be a GKI buffer
 *                  allocated by the application.  This buffer will be freed
 *                  by the protocol stack; the application must not free
 *                  this buffer.
 *
 *                  The opt parameter allows passing specific options like:
 *                  - NO_RTP : do not add the RTP header to buffer
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_WriteReqOpt(uint8_t handle, BT_HDR* p_pkt,
                                 uint32_t time_stamp, uint8_t m_pt,
                                 tAVDT_DATA_OPT_MASK opt);

/*******************************************************************************
 *
 * Function         AVDT_ConnectReq
 *
 * Description      This function initiates an AVDTP signaling connection
 *                  to the peer device.  When the connection is completed, an
 *                  AVDT_CONNECT_IND_EVT is sent to the application via its
 *                  control callback function.  If the connection attempt fails
 *                  an AVDT_DISCONNECT_IND_EVT is sent.  The security mask
 *                  parameter overrides the outgoing security mask set in
 *                  AVDT_Register().
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_ConnectReq(const RawAddress& bd_addr,
                                uint8_t channel_index, uint8_t sec_mask,
                                tAVDT_CTRL_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         AVDT_DisconnectReq
 *
 * Description      This function disconnect an AVDTP signaling connection
 *                  to the peer device.  When disconnected an
 *                  AVDT_DISCONNECT_IND_EVT is sent to the application via its
 *                  control callback function.
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
extern uint16_t AVDT_DisconnectReq(const RawAddress& bd_addr,
                                   tAVDT_CTRL_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         AVDT_GetL2CapChannel
 *
 * Description      Get the L2CAP CID used by the handle.
 *
 * Returns          CID if successful, otherwise 0.
 *
 ******************************************************************************/
extern uint16_t AVDT_GetL2CapChannel(uint8_t handle);

/*******************************************************************************
 *
 * Function         AVDT_GetSignalChannel
 *
 * Description      Get the L2CAP CID used by the signal channel of the given
 *                  handle.
 *
 * Returns          CID if successful, otherwise 0.
 *
 ******************************************************************************/
extern uint16_t AVDT_GetSignalChannel(uint8_t handle,
                                      const RawAddress& bd_addr);

/*******************************************************************************
 *
 * Function         AVDT_SendReport
 *
 * Description
 *
 *
 *
 * Returns
 *
 ******************************************************************************/
extern uint16_t AVDT_SendReport(uint8_t handle, AVDT_REPORT_TYPE type,
                                tAVDT_REPORT_DATA* p_data);

/******************************************************************************
 *
 * Function         AVDT_SetTraceLevel
 *
 * Description      Sets the trace level for AVDT. If 0xff is passed, the
 *                  current trace level is returned.
 *
 *                  Input Parameters:
 *                      new_level:  The level to set the AVDT tracing to:
 *                      0xff-returns the current setting.
 *                      0-turns off tracing.
 *                      >= 1-Errors.
 *                      >= 2-Warnings.
 *                      >= 3-APIs.
 *                      >= 4-Events.
 *                      >= 5-Debug.
 *
 * Returns          The new trace level or current trace level if
 *                  the input parameter is 0xff.
 *
 *****************************************************************************/
extern uint8_t AVDT_SetTraceLevel(uint8_t new_level);

/**
 * Dump debug-related information for the Stack AVDTP module.
 *
 * @param fd the file descriptor to use for writing the ASCII formatted
 * information
 */
void stack_debug_avdtp_api_dump(int fd);

#endif /* AVDT_API_H */
