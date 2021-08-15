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
 *  This module contains the action functions associated with the stream
 *  control block state machine.
 *
 ******************************************************************************/

#include <cutils/log.h>
#include <string.h>
#include "a2dp_codec_api.h"
#include "avdt_api.h"
#include "avdt_int.h"
#include "avdtc_api.h"
#include "bt_common.h"
#include "bt_target.h"
#include "bt_types.h"
#include "bt_utils.h"
#include "btu.h"
#include "osi/include/osi.h"

/* This table is used to lookup the callback event that matches a particular
 * state machine API request event.  Note that state machine API request
 * events are at the beginning of the event list starting at zero, thus
 * allowing for this table.
 */
const uint8_t avdt_scb_cback_evt[] = {
    0,                     /* API_REMOVE_EVT (no event) */
    AVDT_WRITE_CFM_EVT,    /* API_WRITE_REQ_EVT */
    0,                     /* API_GETCONFIG_REQ_EVT (no event) */
    0,                     /* API_DELAY_RPT_REQ_EVT (no event) */
    AVDT_OPEN_CFM_EVT,     /* API_SETCONFIG_REQ_EVT */
    AVDT_OPEN_CFM_EVT,     /* API_OPEN_REQ_EVT */
    AVDT_CLOSE_CFM_EVT,    /* API_CLOSE_REQ_EVT */
    AVDT_RECONFIG_CFM_EVT, /* API_RECONFIG_REQ_EVT */
    AVDT_SECURITY_CFM_EVT, /* API_SECURITY_REQ_EVT */
    0                      /* API_ABORT_REQ_EVT (no event) */
};

/*******************************************************************************
 *
 * Function         avdt_scb_gen_ssrc
 *
 * Description      This function generates a SSRC number unique to the stream.
 *
 * Returns          SSRC value.
 *
 ******************************************************************************/
uint32_t avdt_scb_gen_ssrc(AvdtpScb* p_scb) {
  /* combine the value of the media type and codec type of the SCB */
  return ((uint32_t)(p_scb->stream_config.cfg.codec_info[1] |
                     p_scb->stream_config.cfg.codec_info[2]));
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_abort_cmd
 *
 * Description      This function sends the SCB an AVDT_SCB_API_ABORT_RSP_EVT
 *                  to initiate sending of an abort response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_abort_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_scb->role = AVDT_CLOSE_ACP;
  avdt_scb_event(p_scb, AVDT_SCB_API_ABORT_RSP_EVT, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_abort_rsp
 *
 * Description      This function is an empty function; it serves as a
 *                  placeholder for a conformance API action function.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_abort_rsp(UNUSED_ATTR AvdtpScb* p_scb,
                            UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  return;
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_close_cmd
 *
 * Description      This function sends the SCB an AVDT_SCB_API_CLOSE_RSP_EVT
 *                  to initiate sending of a close response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_close_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_scb->role = AVDT_CLOSE_ACP;
  avdt_scb_event(p_scb, AVDT_SCB_API_CLOSE_RSP_EVT, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_close_rsp
 *
 * Description      This function sets the close_code variable to the error
 *                  code returned in the close response.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_close_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_scb->close_code = p_data->msg.hdr.err_code;
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_getconfig_cmd
 *
 * Description      This function retrieves the configuration parameters of
 *                  the SCB and sends the SCB an AVDT_SCB_API_GETCONFIG_RSP_EVT
 *                  to initiate sending of a get configuration response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_getconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_data->msg.svccap.p_cfg = &p_scb->curr_cfg;

  avdt_scb_event(p_scb, AVDT_SCB_API_GETCONFIG_RSP_EVT, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_getconfig_rsp
 *
 * Description      This function is an empty function; it serves as a
 *                  placeholder for a conformance API action function.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_getconfig_rsp(UNUSED_ATTR AvdtpScb* p_scb,
                                UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  return;
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_open_cmd
 *
 * Description      This function sends the SCB an AVDT_SCB_API_OPEN_RSP_EVT
 *                  to initiate sending of an open response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_open_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  avdt_scb_event(p_scb, AVDT_SCB_API_OPEN_RSP_EVT, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_open_rej
 *
 * Description      This function calls the application callback function
 *                  indicating the open request has failed.  It initializes
 *                  certain SCB variables and sends a AVDT_CCB_UL_CLOSE_EVT
 *                  to the CCB.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_open_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* do exactly same as setconfig reject */
  avdt_scb_hdl_setconfig_rej(p_scb, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_open_rsp
 *
 * Description      This function calls avdt_ad_open_req() to initiate
 *                  connection of the transport channel for this stream.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_open_rsp(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  /* initiate opening of trans channels for this SEID */
  p_scb->role = AVDT_OPEN_INT;
  avdt_ad_open_req(AVDT_CHAN_MEDIA, p_scb->p_ccb, p_scb, AVDT_INT);

  /* start tc connect timer */
  alarm_set_on_mloop(p_scb->transport_channel_timer,
                     AVDT_SCB_TC_CONN_TIMEOUT_MS,
                     avdt_scb_transport_channel_timer_timeout, p_scb);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_pkt_no_frag
 *
 * Description
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_pkt_no_frag(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  uint8_t *p, *p_start;
  uint8_t o_v, o_p, o_x, o_cc;
  uint8_t m_pt;
  uint8_t marker;
  uint16_t seq;
  uint32_t time_stamp;
  uint16_t offset;
  uint16_t ex_len;
  uint8_t pad_len = 0;
  uint16_t len = p_data->p_pkt->len;

  p = p_start = (uint8_t*)(p_data->p_pkt + 1) + p_data->p_pkt->offset;

  /* parse media packet header */
  offset = 12;
  // AVDT_MSG_PRS_OCTET1(1) + AVDT_MSG_PRS_M_PT(1) + UINT16(2) + UINT32(4) + 4
  if (offset > len) goto length_error;
  AVDT_MSG_PRS_OCTET1(p, o_v, o_p, o_x, o_cc);
  AVDT_MSG_PRS_M_PT(p, m_pt, marker);
  BE_STREAM_TO_UINT16(seq, p);
  BE_STREAM_TO_UINT32(time_stamp, p);
  p += 4;

  /* skip over any csrc's in packet */
  offset += o_cc * 4;
  p += o_cc * 4;

  /* check for and skip over extension header */
  if (o_x) {
    offset += 4;
    if (offset > len) goto length_error;
    p += 2;
    BE_STREAM_TO_UINT16(ex_len, p);
    offset += ex_len * 4;
    p += ex_len * 4;
  }

  /* adjust length for any padding at end of packet */
  if (o_p) {
    /* padding length in last byte of packet */
    pad_len = *(p_start + p_data->p_pkt->len);
  }

  /* do sanity check */
  if ((offset > p_data->p_pkt->len) ||
      ((pad_len + offset) > p_data->p_pkt->len)) {
    AVDT_TRACE_WARNING("Got bad media packet");
    osi_free_and_reset((void**)&p_data->p_pkt);
  }
  /* adjust offset and length and send it up */
  else {
    p_data->p_pkt->len -= (offset + pad_len);
    p_data->p_pkt->offset += offset;

    if (p_scb->stream_config.p_sink_data_cback != NULL) {
      /* report sequence number */
      p_data->p_pkt->layer_specific = seq;
      (*p_scb->stream_config.p_sink_data_cback)(
          avdt_scb_to_hdl(p_scb), p_data->p_pkt, time_stamp,
          (uint8_t)(m_pt | (marker << 7)));
    } else {
      osi_free_and_reset((void**)&p_data->p_pkt);
    }
  }
  return;
length_error:
  android_errorWriteLog(0x534e4554, "111450156");
  AVDT_TRACE_WARNING("%s: hdl packet length %d too short: must be at least %d",
                     __func__, len, offset);
  osi_free_and_reset((void**)&p_data->p_pkt);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_report
 *
 * Description
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
uint8_t* avdt_scb_hdl_report(AvdtpScb* p_scb, uint8_t* p, uint16_t len) {
  uint16_t result = AVDT_SUCCESS;
  uint8_t* p_start = p;
  uint32_t ssrc;
  uint8_t o_v, o_p, o_cc;
  uint16_t min_len = 0;
  AVDT_REPORT_TYPE pt;
  tAVDT_REPORT_DATA report;

  AVDT_TRACE_DEBUG("%s", __func__);
  if (p_scb->stream_config.p_report_cback) {
    /* parse report packet header */
    min_len += 8;
    if (min_len > len) {
      android_errorWriteLog(0x534e4554, "111450156");
      AVDT_TRACE_WARNING(
          "%s: hdl packet length %d too short: must be at least %d", __func__,
          len, min_len);
      goto avdt_scb_hdl_report_exit;
    }
    AVDT_MSG_PRS_RPT_OCTET1(p, o_v, o_p, o_cc);
    pt = *p++;
    p += 2;
    BE_STREAM_TO_UINT32(ssrc, p);

    switch (pt) {
      case AVDT_RTCP_PT_SR: /* the packet type - SR (Sender Report) */
        min_len += 20;
        if (min_len > len) {
          android_errorWriteLog(0x534e4554, "111450156");
          AVDT_TRACE_WARNING(
              "%s: hdl packet length %d too short: must be at least %d",
              __func__, len, min_len);
          goto avdt_scb_hdl_report_exit;
        }
        BE_STREAM_TO_UINT32(report.sr.ntp_sec, p);
        BE_STREAM_TO_UINT32(report.sr.ntp_frac, p);
        BE_STREAM_TO_UINT32(report.sr.rtp_time, p);
        BE_STREAM_TO_UINT32(report.sr.pkt_count, p);
        BE_STREAM_TO_UINT32(report.sr.octet_count, p);
        break;

      case AVDT_RTCP_PT_RR: /* the packet type - RR (Receiver Report) */
        min_len += 20;
        if (min_len > len) {
          android_errorWriteLog(0x534e4554, "111450156");
          AVDT_TRACE_WARNING(
              "%s: hdl packet length %d too short: must be at least %d",
              __func__, len, min_len);
          goto avdt_scb_hdl_report_exit;
        }
        report.rr.frag_lost = *p;
        BE_STREAM_TO_UINT32(report.rr.packet_lost, p);
        report.rr.packet_lost &= 0xFFFFFF;
        BE_STREAM_TO_UINT32(report.rr.seq_num_rcvd, p);
        BE_STREAM_TO_UINT32(report.rr.jitter, p);
        BE_STREAM_TO_UINT32(report.rr.lsr, p);
        BE_STREAM_TO_UINT32(report.rr.dlsr, p);
        break;

      case AVDT_RTCP_PT_SDES: /* the packet type - SDES (Source Description) */
        uint8_t sdes_type;
        min_len += 1;
        if (min_len > len) {
          android_errorWriteLog(0x534e4554, "111450156");
          AVDT_TRACE_WARNING(
              "%s: hdl packet length %d too short: must be at least %d",
              __func__, len, min_len);
          goto avdt_scb_hdl_report_exit;
        }
        BE_STREAM_TO_UINT8(sdes_type, p);
        if (sdes_type == AVDT_RTCP_SDES_CNAME) {
          uint8_t name_length;
          min_len += 1;
          if (min_len > len) {
            android_errorWriteLog(0x534e4554, "111450156");
            AVDT_TRACE_WARNING(
                "%s: hdl packet length %d too short: must be at least %d",
                __func__, len, min_len);
            goto avdt_scb_hdl_report_exit;
          }
          BE_STREAM_TO_UINT8(name_length, p);
          if (name_length > len - 2 || name_length > AVDT_MAX_CNAME_SIZE) {
            result = AVDT_BAD_PARAMS;
          } else {
            BE_STREAM_TO_ARRAY(p, &(report.cname[0]), name_length);
          }
        } else {
          if (min_len + 1 > len) {
            android_errorWriteLog(0x534e4554, "111450156");
            AVDT_TRACE_WARNING(
                "%s: hdl packet length %d too short: must be at least %d",
                __func__, len, min_len);
            goto avdt_scb_hdl_report_exit;
          }
          AVDT_TRACE_WARNING(" - SDES SSRC=0x%08x sc=%d %d len=%d %s", ssrc,
                             o_cc, *p, *(p + 1), p + 2);
          result = AVDT_BUSY;
        }
        break;

      default:
        AVDT_TRACE_ERROR("Bad Report pkt - packet type: %d", pt);
        result = AVDT_BAD_PARAMS;
    }

    if (result == AVDT_SUCCESS)
      (*p_scb->stream_config.p_report_cback)(avdt_scb_to_hdl(p_scb), pt,
                                             &report);
  }
avdt_scb_hdl_report_exit:
  p_start += len;
  return p_start;
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_pkt
 *
 * Description
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_data->p_pkt->layer_specific == AVDT_CHAN_REPORT) {
    uint8_t* p = (uint8_t*)(p_data->p_pkt + 1) + p_data->p_pkt->offset;
    avdt_scb_hdl_report(p_scb, p, p_data->p_pkt->len);
    osi_free_and_reset((void**)&p_data->p_pkt);
  } else {
    avdt_scb_hdl_pkt_no_frag(p_scb, p_data);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_drop_pkt
 *
 * Description      Drop an incoming media packet.  This function is called if
 *                  a media packet is received in any state besides streaming.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_drop_pkt(UNUSED_ATTR AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  AVDT_TRACE_ERROR("%s dropped incoming media packet", __func__);
  osi_free_and_reset((void**)&p_data->p_pkt);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_reconfig_cmd
 *
 * Description      This function calls the application callback function
 *                  with a reconfiguration indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_reconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* if command not supported */
  if (p_scb->stream_config.nsc_mask & AvdtpStreamConfig::AVDT_NSC_RECONFIG) {
    /* send reject */
    p_data->msg.hdr.err_code = AVDT_ERR_NSC;
    p_data->msg.hdr.err_param = 0;
    avdt_scb_event(p_scb, AVDT_SCB_API_RECONFIG_RSP_EVT, p_data);
  } else {
    /* store requested configuration */
    p_scb->req_cfg = *p_data->msg.reconfig_cmd.p_cfg;

    /* call application callback */
    (*p_scb->stream_config.p_avdt_ctrl_cback)(
        avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_RECONFIG_IND_EVT,
        (tAVDT_CTRL*)&p_data->msg.reconfig_cmd, p_scb->stream_config.scb_index);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_reconfig_rsp
 *
 * Description      This function calls the application callback function
 *                  with a reconfiguration confirm.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_reconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_data->msg.hdr.err_code == 0) {
    /* store new configuration */
    if (p_scb->req_cfg.num_codec > 0) {
      p_scb->curr_cfg.num_codec = p_scb->req_cfg.num_codec;
      memcpy(p_scb->curr_cfg.codec_info, p_scb->req_cfg.codec_info,
             AVDT_CODEC_SIZE);
    }
    if (p_scb->req_cfg.num_protect > 0) {
      p_scb->curr_cfg.num_protect = p_scb->req_cfg.num_protect;
      memcpy(p_scb->curr_cfg.protect_info, p_scb->req_cfg.protect_info,
             AVDT_PROTECT_SIZE);
    }
  }

  p_data->msg.svccap.p_cfg = &p_scb->curr_cfg;

  /* call application callback */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_RECONFIG_CFM_EVT,
      (tAVDT_CTRL*)&p_data->msg.svccap, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_security_cmd
 *
 * Description      This function calls the application callback with a
 *                  security indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_security_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* if command not supported */
  if (p_scb->stream_config.nsc_mask & AvdtpStreamConfig::AVDT_NSC_SECURITY) {
    /* send reject */
    p_data->msg.hdr.err_code = AVDT_ERR_NSC;
    avdt_scb_event(p_scb, AVDT_SCB_API_SECURITY_RSP_EVT, p_data);
  } else {
    /* call application callback */
    (*p_scb->stream_config.p_avdt_ctrl_cback)(
        avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_SECURITY_IND_EVT,
        (tAVDT_CTRL*)&p_data->msg.security_cmd, p_scb->stream_config.scb_index);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_security_rsp
 *
 * Description      This function calls the application callback with a
 *                  security confirm.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_security_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* call application callback */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_SECURITY_CFM_EVT,
      (tAVDT_CTRL*)&p_data->msg.security_cmd, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_setconfig_cmd
 *
 * Description      This function marks the SCB as in use and copies the
 *                  configuration and peer SEID to the SCB.  It then calls
 *                  the application callback with a configuration indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_setconfig_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  AVDT_TRACE_DEBUG("%s: p_scb->in_use=%d p_avdt_scb=%p scb_index=%d", __func__,
                   p_scb->in_use, p_scb, p_scb->stream_config.scb_index);

  if (!p_scb->in_use) {
    AVDT_TRACE_DEBUG(
        "%s: codec: %s", __func__,
        A2DP_CodecInfoString(p_scb->stream_config.cfg.codec_info).c_str());
    AVDT_TRACE_DEBUG(
        "%s: codec: %s", __func__,
        A2DP_CodecInfoString(p_data->msg.config_cmd.p_cfg->codec_info).c_str());
    AvdtpSepConfig* p_cfg = p_data->msg.config_cmd.p_cfg;
    if (A2DP_GetCodecType(p_scb->stream_config.cfg.codec_info) ==
        A2DP_GetCodecType(p_cfg->codec_info)) {
      /* copy info to scb */
      AvdtpCcb* p_ccb = avdt_ccb_by_idx(p_data->msg.config_cmd.hdr.ccb_idx);
      if (p_scb->p_ccb != p_ccb) {
        AVDT_TRACE_ERROR(
            "%s: mismatch in AVDTP SCB/CCB state: (p_scb->p_ccb=%p != "
            "p_ccb=%p): "
            "p_scb=%p scb_handle=%d ccb_idx=%d",
            __func__, p_scb->p_ccb, p_ccb, p_scb, p_scb->ScbHandle(),
            p_data->msg.config_cmd.hdr.ccb_idx);
        avdt_scb_rej_not_in_use(p_scb, p_data);
        return;
      }
      /* set sep as in use */
      p_scb->in_use = true;

      p_scb->peer_seid = p_data->msg.config_cmd.int_seid;
      p_scb->req_cfg = *p_cfg;
      /* call app callback */
      /* handle of scb- which is same as sep handle of bta_av_cb.p_scb*/
      (*p_scb->stream_config.p_avdt_ctrl_cback)(
          avdt_scb_to_hdl(p_scb),
          p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
          AVDT_CONFIG_IND_EVT, (tAVDT_CTRL*)&p_data->msg.config_cmd,
          p_scb->stream_config.scb_index);
    } else {
      p_data->msg.hdr.err_code = AVDT_ERR_UNSUP_CFG;
      p_data->msg.hdr.err_param = 0;
      avdt_msg_send_rej(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx),
                        p_data->msg.hdr.sig_id, &p_data->msg);
    }
  } else {
    AVDT_TRACE_DEBUG("%s: calling avdt_scb_rej_in_use()", __func__);
    avdt_scb_rej_in_use(p_scb, p_data);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_setconfig_rej
 *
 * Description      This function marks the SCB as not in use and calls the
 *                  application callback with an open confirm indicating
 *                  failure.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_setconfig_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* clear scb variables */
  avdt_scb_clr_vars(p_scb, p_data);

  /* tell ccb we're done with signaling channel */
  avdt_ccb_event(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx),
                 AVDT_CCB_UL_CLOSE_EVT, NULL);

  /* call application callback */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_OPEN_CFM_EVT,
      (tAVDT_CTRL*)&p_data->msg.hdr, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_setconfig_rsp
 *
 * Description      This function sends the SCB an AVDT_SCB_API_OPEN_REQ_EVT
 *                  to initiate sending of an open command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_setconfig_rsp(AvdtpScb* p_scb,
                                UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_EVT_HDR single;

  if (p_scb->p_ccb != NULL) {
    /* save configuration */
    p_scb->curr_cfg = p_scb->req_cfg;

    /* initiate open */
    single.seid = p_scb->peer_seid;
    tAVDT_SCB_EVT avdt_scb_evt;
    avdt_scb_evt.msg.single = single;
    avdt_scb_event(p_scb, AVDT_SCB_API_OPEN_REQ_EVT, &avdt_scb_evt);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_start_cmd
 *
 * Description      This function calls the application callback with a
 *                  start indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_start_cmd(AvdtpScb* p_scb,
                            UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_START_IND_EVT, NULL, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_start_rsp
 *
 * Description      This function calls the application callback with a
 *                  start confirm.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_start_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_START_CFM_EVT, (tAVDT_CTRL*)&p_data->msg.hdr,
      p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_suspend_cmd
 *
 * Description      This function calls the application callback with a suspend
 *                  indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_suspend_cmd(AvdtpScb* p_scb,
                              UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_SUSPEND_IND_EVT, NULL, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_suspend_rsp
 *
 * Description      This function calls the application callback with a suspend
 *                  confirm.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_suspend_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_SUSPEND_CFM_EVT, (tAVDT_CTRL*)&p_data->msg.hdr,
      p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_tc_close
 *
 * Description      This function is called when the transport channel is
 *                  closed.  It marks the SCB as not in use and
 *                  initializes certain SCB parameters.  It then sends
 *                  an AVDT_CCB_UL_CLOSE_EVT to the CCB if the SCB
 *                  initiated the close.  It then checks to see if the SCB
 *                  is to be removed.  If it is it deallocates the SCB.
 *                  Finally, it calls the application callback with a close
 *                  indication.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_tc_close(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  uint8_t hdl = avdt_scb_to_hdl(p_scb);
  tAVDT_CTRL_CBACK* p_avdt_ctrl_cback = p_scb->stream_config.p_avdt_ctrl_cback;
  tAVDT_CTRL avdt_ctrl;
  uint8_t event;
  AvdtpCcb* p_ccb = p_scb->p_ccb;
  RawAddress remote_addr = p_ccb->peer_addr;
  uint8_t scb_index = p_scb->stream_config.scb_index;

  /* set up hdr */
  avdt_ctrl.hdr.err_code = p_scb->close_code;

  /* clear sep variables */
  avdt_scb_clr_vars(p_scb, p_data);
  p_scb->media_seq = 0;
  p_scb->cong = false;

  /* free pkt we're holding, if any */
  osi_free_and_reset((void**)&p_scb->p_pkt);

  alarm_cancel(p_scb->transport_channel_timer);

  if ((p_scb->role == AVDT_CLOSE_INT) || (p_scb->role == AVDT_OPEN_INT)) {
    /* tell ccb we're done with signaling channel */
    avdt_ccb_event(p_ccb, AVDT_CCB_UL_CLOSE_EVT, NULL);
  }
  event =
      (p_scb->role == AVDT_CLOSE_INT) ? AVDT_CLOSE_CFM_EVT : AVDT_CLOSE_IND_EVT;
  p_scb->role = AVDT_CLOSE_ACP;

  if (p_scb->remove) {
    avdt_scb_dealloc(p_scb, NULL);
  }

  /* call app callback */
  (*p_avdt_ctrl_cback)(hdl, remote_addr, event, &avdt_ctrl, scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_delay_rpt_req
 *
 * Description      This function calls the application callback with a delay
 *                  report.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_delay_rpt_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_DELAY_RPT,
                    (tAVDT_MSG*)&p_data->apidelay);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_delay_rpt_cmd
 *
 * Description      This function calls the application callback with a delay
 *                  report.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_delay_rpt_cmd(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_DELAY_REPORT_EVT, (tAVDT_CTRL*)&p_data->msg.hdr,
      p_scb->stream_config.scb_index);

  if (p_scb->p_ccb)
    avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_DELAY_RPT, &p_data->msg);
  else
    avdt_scb_rej_not_in_use(p_scb, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_delay_rpt_rsp
 *
 * Description      This function calls the application callback with a delay
 *                  report.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_delay_rpt_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
      AVDT_DELAY_REPORT_CFM_EVT, (tAVDT_CTRL*)&p_data->msg.hdr,
      p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_tc_close_sto
 *
 * Description      This function is called when a channel is closed in OPEN
 *                  state.  Check the channel type and process accordingly.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_tc_close_sto(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;
  /* AVDT_CHAN_SIG does not visit this action */
  if (p_data && p_data->close.type != AVDT_CHAN_MEDIA) {
    /* it's reporting or recovery channel,
     * the channel close in open state means the peer does not support it */
    if (p_data->close.old_tc_state == AVDT_AD_ST_OPEN) {
      avdt_ctrl.hdr.err_code = 0;
      avdt_ctrl.hdr.err_param = 0;
      /* call app callback */
      (*p_scb->stream_config.p_avdt_ctrl_cback)(
          avdt_scb_to_hdl(p_scb),
          p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
          AVDT_REPORT_DISCONN_EVT, &avdt_ctrl, p_scb->stream_config.scb_index);
    }
  } else {
    /* must be in OPEN state. need to go back to idle */
    avdt_scb_event(p_scb, AVDT_SCB_MSG_ABORT_RSP_EVT, NULL);
    avdt_scb_hdl_tc_close(p_scb, p_data);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_tc_open
 *
 * Description      This function is called when the transport channel is
 *                  opened while in the opening state.  It calls the
 *                  application callback with an open indication or open
 *                  confirm depending on who initiated the open procedure.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_tc_open(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  uint8_t event;
  uint8_t role;

  alarm_cancel(p_scb->transport_channel_timer);

  event =
      (p_scb->role == AVDT_OPEN_INT) ? AVDT_OPEN_CFM_EVT : AVDT_OPEN_IND_EVT;
  p_data->open.hdr.err_code = 0;

  AVDT_TRACE_DEBUG("%s: psc_mask: cfg: 0x%x, req:0x%x, cur: 0x%x", __func__,
                   p_scb->stream_config.cfg.psc_mask, p_scb->req_cfg.psc_mask,
                   p_scb->curr_cfg.psc_mask);
  if (p_scb->curr_cfg.psc_mask & AVDT_PSC_REPORT) {
    /* open the reporting channel, if both devices support it */
    role = (p_scb->role == AVDT_OPEN_INT) ? AVDT_INT : AVDT_ACP;
    avdt_ad_open_req(AVDT_CHAN_REPORT, p_scb->p_ccb, p_scb, role);
  }

  /* call app callback */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb),
      p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty, event,
      (tAVDT_CTRL*)&p_data->open, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_tc_open_sto
 *
 * Description      This function is called when the transport channel is
 *                  opened while in the opening state.  It calls the
 *                  application callback with an open indication or open
 *                  confirm depending on who initiated the open procedure.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_tc_open_sto(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;
  /* open reporting channel here, when it is implemented */

  /* call app callback */
  if (p_data->open.hdr.err_code == AVDT_CHAN_REPORT) {
    avdt_ctrl.hdr.err_code = 0;
    avdt_ctrl.hdr.err_param = 1;
    (*p_scb->stream_config.p_avdt_ctrl_cback)(
        avdt_scb_to_hdl(p_scb),
        p_scb->p_ccb ? p_scb->p_ccb->peer_addr : RawAddress::kEmpty,
        AVDT_REPORT_CONN_EVT, &avdt_ctrl, p_scb->stream_config.scb_index);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_hdl_write_req
 *
 * Description      This function frees the media packet currently stored in
 *                  the SCB, if any.  Then it builds a new media packet from
 *                  with the passed in buffer and stores it in the SCB.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_hdl_write_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  uint8_t* p;
  uint32_t ssrc;
  bool add_rtp_header = !(p_data->apiwrite.opt & AVDT_DATA_OPT_NO_RTP);

  /* free packet we're holding, if any; to be replaced with new */
  if (p_scb->p_pkt != NULL) {
    /* this shouldn't be happening */
    AVDT_TRACE_WARNING("Dropped media packet; congested");
  }
  osi_free_and_reset((void**)&p_scb->p_pkt);

  /* Recompute only if the RTP header wasn't disabled by the API */
  if (add_rtp_header) {
    bool is_content_protection = (p_scb->curr_cfg.num_protect > 0);
    add_rtp_header =
        A2DP_UsesRtpHeader(is_content_protection, p_scb->curr_cfg.codec_info);
  }

  /* Build a media packet, and add an RTP header if required. */
  if (add_rtp_header) {
    ssrc = avdt_scb_gen_ssrc(p_scb);

    p_data->apiwrite.p_buf->len += AVDT_MEDIA_HDR_SIZE;
    p_data->apiwrite.p_buf->offset -= AVDT_MEDIA_HDR_SIZE;
    p_scb->media_seq++;
    p = (uint8_t*)(p_data->apiwrite.p_buf + 1) + p_data->apiwrite.p_buf->offset;

    UINT8_TO_BE_STREAM(p, AVDT_MEDIA_OCTET1);
    UINT8_TO_BE_STREAM(p, p_data->apiwrite.m_pt);
    UINT16_TO_BE_STREAM(p, p_scb->media_seq);
    UINT32_TO_BE_STREAM(p, p_data->apiwrite.time_stamp);
    UINT32_TO_BE_STREAM(p, ssrc);
  }

  /* store it */
  p_scb->p_pkt = p_data->apiwrite.p_buf;
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_abort_req
 *
 * Description      This function sends an abort command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_abort_req(AvdtpScb* p_scb,
                            UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_EVT_HDR hdr;

  AVDT_TRACE_DEBUG("%s: p_scb->p_ccb=%p", __func__, p_scb->p_ccb);

  if (p_scb->p_ccb != NULL) {
    p_scb->role = AVDT_CLOSE_INT;

    hdr.seid = p_scb->peer_seid;

    tAVDT_MSG avdt_msg;
    avdt_msg.hdr = hdr;
    avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_ABORT, &avdt_msg);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_abort_rsp
 *
 * Description      This function sends an abort response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_abort_rsp(UNUSED_ATTR AvdtpScb* p_scb,
                            tAVDT_SCB_EVT* p_data) {
  avdt_msg_send_rsp(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx), AVDT_SIG_ABORT,
                    &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_close_req
 *
 * Description      This function sends a close command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_close_req(AvdtpScb* p_scb,
                            UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_EVT_HDR hdr;

  p_scb->role = AVDT_CLOSE_INT;

  hdr.seid = p_scb->peer_seid;

  tAVDT_MSG avdt_msg;
  avdt_msg.hdr = hdr;
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_CLOSE, &avdt_msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_stream_close
 *
 * Description      This function sends a close command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_stream_close(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  osi_free_and_reset((void**)&p_scb->p_pkt);
  avdt_scb_snd_close_req(p_scb, p_data);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_close_rsp
 *
 * Description      This function sends a close response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_close_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_CLOSE, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_getconfig_req
 *
 * Description      This function sends a get configuration command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_getconfig_req(AvdtpScb* p_scb,
                                UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_EVT_HDR hdr;

  hdr.seid = p_scb->peer_seid;

  tAVDT_MSG avdt_msg;
  avdt_msg.hdr = hdr;
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_GETCONFIG, &avdt_msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_getconfig_rsp
 *
 * Description      This function sends a get configuration response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_getconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_GETCONFIG, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_open_req
 *
 * Description      This function sends an open command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_open_req(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_EVT_HDR hdr;

  hdr.seid = p_scb->peer_seid;

  tAVDT_MSG avdt_msg;
  avdt_msg.hdr = hdr;
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_OPEN, &avdt_msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_open_rsp
 *
 * Description      This function sends an open response message.  It also
 *                  calls avdt_ad_open_req() to accept a transport channel
 *                  connection.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_open_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  /* notify adaption that we're waiting for transport channel open */
  p_scb->role = AVDT_OPEN_ACP;
  avdt_ad_open_req(AVDT_CHAN_MEDIA, p_scb->p_ccb, p_scb, AVDT_ACP);

  /* send response */
  avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_OPEN, &p_data->msg);

  alarm_set_on_mloop(p_scb->transport_channel_timer,
                     AVDT_SCB_TC_CONN_TIMEOUT_MS,
                     avdt_scb_transport_channel_timer_timeout, p_scb);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_reconfig_req
 *
 * Description      This function stores the configuration parameters in the
 *                  SCB and sends a reconfiguration command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_reconfig_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  AVDT_TRACE_DEBUG("%s: p_scb->peer_seid=%d p_data->msg.hdr.seid=%d", __func__,
                   p_scb->peer_seid, p_data->msg.hdr.seid);
  AVDT_TRACE_DEBUG(
      "%s: codec: %s", __func__,
      A2DP_CodecInfoString(p_data->msg.config_cmd.p_cfg->codec_info).c_str());

  p_scb->req_cfg = *p_data->msg.config_cmd.p_cfg;
  p_data->msg.hdr.seid = p_scb->peer_seid;
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_RECONFIG, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_reconfig_rsp
 *
 * Description      This function stores the configuration parameters in the
 *                  SCB and sends a reconfiguration response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_reconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_data->msg.hdr.err_code == 0) {
    /* store new configuration */
    if (p_scb->req_cfg.num_codec > 0) {
      p_scb->curr_cfg.num_codec = p_scb->req_cfg.num_codec;
      memcpy(p_scb->curr_cfg.codec_info, p_scb->req_cfg.codec_info,
             AVDT_CODEC_SIZE);
    }
    if (p_scb->req_cfg.num_protect > 0) {
      p_scb->curr_cfg.num_protect = p_scb->req_cfg.num_protect;
      memcpy(p_scb->curr_cfg.protect_info, p_scb->req_cfg.protect_info,
             AVDT_PROTECT_SIZE);
    }

    /* send response */
    avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_RECONFIG, &p_data->msg);
  } else {
    /* send reject */
    avdt_msg_send_rej(p_scb->p_ccb, AVDT_SIG_RECONFIG, &p_data->msg);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_security_req
 *
 * Description      This function sends a security command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_security_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_data->msg.hdr.seid = p_scb->peer_seid;
  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_SECURITY, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_security_rsp
 *
 * Description      This function sends a security response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_security_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_data->msg.hdr.err_code == 0) {
    avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_SECURITY, &p_data->msg);
  } else {
    avdt_msg_send_rej(p_scb->p_ccb, AVDT_SIG_SECURITY, &p_data->msg);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_setconfig_rej
 *
 * Description      This function marks the SCB as not in use and sends a
 *                  set configuration reject message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_setconfig_rej(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_scb->p_ccb != NULL) {
    avdt_msg_send_rej(p_scb->p_ccb, AVDT_SIG_SETCONFIG, &p_data->msg);

    /* clear scb variables */
    avdt_scb_clr_vars(p_scb, p_data);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_setconfig_req
 *
 * Description      This function marks the SCB as in use and copies the
 *                  configuration parameters to the SCB.  Then the function
 *                  sends a set configuration command message and initiates
 *                  opening of the signaling channel.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_setconfig_req(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  AvdtpSepConfig *p_req, *p_cfg;

  AVDT_TRACE_DEBUG(
      "%s: codec: %s", __func__,
      A2DP_CodecInfoString(p_data->msg.config_cmd.p_cfg->codec_info).c_str());

  /* copy API parameters to scb, set scb as in use */

  AvdtpCcb* p_ccb = avdt_ccb_by_idx(p_data->msg.config_cmd.hdr.ccb_idx);
  if (p_scb->p_ccb != p_ccb) {
    AVDT_TRACE_ERROR(
        "%s: mismatch in AVDTP SCB/CCB state: (p_scb->p_ccb=%p != p_ccb=%p): "
        "p_scb=%p scb_handle=%d ccb_idx=%d",
        __func__, p_scb->p_ccb, p_ccb, p_scb, p_scb->ScbHandle(),
        p_data->msg.config_cmd.hdr.ccb_idx);
    avdt_scb_rej_not_in_use(p_scb, p_data);
    return;
  }
  p_scb->in_use = true;
  p_scb->peer_seid = p_data->msg.config_cmd.hdr.seid;
  p_req = p_data->msg.config_cmd.p_cfg;
  p_cfg = &p_scb->stream_config.cfg;
  p_scb->req_cfg = *p_data->msg.config_cmd.p_cfg;

  avdt_msg_send_cmd(p_scb->p_ccb, p_scb, AVDT_SIG_SETCONFIG, &p_data->msg);

  /* tell ccb to open channel */
  avdt_ccb_event(p_scb->p_ccb, AVDT_CCB_UL_OPEN_EVT, NULL);
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_setconfig_rsp
 *
 * Description      This function copies the requested configuration into the
 *                  current configuration and sends a set configuration
 *                  response message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_setconfig_rsp(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  if (p_scb->p_ccb != NULL) {
    p_scb->curr_cfg = p_scb->req_cfg;

    avdt_msg_send_rsp(p_scb->p_ccb, AVDT_SIG_SETCONFIG, &p_data->msg);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_snd_tc_close
 *
 * Description      This function calls avdt_ad_close_req() to close the
 *                  transport channel for this SCB.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_snd_tc_close(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  if (p_scb->curr_cfg.psc_mask & AVDT_PSC_REPORT)
    avdt_ad_close_req(AVDT_CHAN_REPORT, p_scb->p_ccb, p_scb);
  avdt_ad_close_req(AVDT_CHAN_MEDIA, p_scb->p_ccb, p_scb);
}

/*******************************************************************************
 *
 * Function         avdt_scb_cb_err
 *
 * Description      This function calls the application callback function
 *                  indicating an error.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_cb_err(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;

  /* set error code and parameter */
  avdt_ctrl.hdr.err_code = AVDT_ERR_BAD_STATE;
  avdt_ctrl.hdr.err_param = 0;

  /* call callback, using lookup table to get callback event */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb), RawAddress::kEmpty,
      avdt_scb_cback_evt[p_scb->curr_evt], &avdt_ctrl,
      p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_cong_state
 *
 * Description      This function sets the congestion state of the SCB media
 *                  transport channel.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_cong_state(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_scb->cong = p_data->llcong;
}

/*******************************************************************************
 *
 * Function         avdt_scb_rej_state
 *
 * Description      This function sends a reject message to the peer indicating
 *                  incorrect state for the received command message.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_rej_state(UNUSED_ATTR AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_data->msg.hdr.err_code = AVDT_ERR_BAD_STATE;
  p_data->msg.hdr.err_param = 0;
  avdt_msg_send_rej(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx),
                    p_data->msg.hdr.sig_id, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_rej_in_use
 *
 * Description      This function sends a reject message to the peer indicating
 *                  the stream is in use.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_rej_in_use(UNUSED_ATTR AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  p_data->msg.hdr.err_code = AVDT_ERR_IN_USE;
  p_data->msg.hdr.err_param = 0;
  avdt_msg_send_rej(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx),
                    p_data->msg.hdr.sig_id, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_rej_not_in_use
 *
 * Description      This function sends a reject message to the peer indicating
 *                  the stream is in use.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_rej_not_in_use(UNUSED_ATTR AvdtpScb* p_scb,
                             tAVDT_SCB_EVT* p_data) {
  p_data->msg.hdr.err_code = AVDT_ERR_NOT_IN_USE;
  p_data->msg.hdr.err_param = 0;
  avdt_msg_send_rej(avdt_ccb_by_idx(p_data->msg.hdr.ccb_idx),
                    p_data->msg.hdr.sig_id, &p_data->msg);
}

/*******************************************************************************
 *
 * Function         avdt_scb_set_remove
 *
 * Description      This function marks an SCB to be removed.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_set_remove(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  p_scb->remove = true;
}

/*******************************************************************************
 *
 * Function         avdt_scb_free_pkt
 *
 * Description      This function frees the media packet passed in.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_free_pkt(AvdtpScb* p_scb, tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;

  /* set error code and parameter */
  avdt_ctrl.hdr.err_code = AVDT_ERR_BAD_STATE;
  avdt_ctrl.hdr.err_param = 0;

  osi_free_and_reset((void**)&p_data->apiwrite.p_buf);

  AVDT_TRACE_WARNING("Dropped media packet");

  /* we need to call callback to keep data flow going */
  (*p_scb->stream_config.p_avdt_ctrl_cback)(
      avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_WRITE_CFM_EVT,
      &avdt_ctrl, p_scb->stream_config.scb_index);
}

/*******************************************************************************
 *
 * Function         avdt_scb_clr_pkt
 *
 * Description      This function frees the media packet stored in the SCB.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_clr_pkt(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;
  AvdtpCcb* p_ccb;
  uint8_t tcid;
  uint16_t lcid;

  /* set error code and parameter */
  avdt_ctrl.hdr.err_code = AVDT_ERR_BAD_STATE;
  avdt_ctrl.hdr.err_param = 0;
  /* flush the media data queued at L2CAP */
  p_ccb = p_scb->p_ccb;
  if (p_ccb != NULL) {
    /* get tcid from type, scb */
    tcid = avdt_ad_type_to_tcid(AVDT_CHAN_MEDIA, p_scb);

    lcid = avdtp_cb.ad.rt_tbl[avdt_ccb_to_idx(p_ccb)][tcid].lcid;
    L2CA_FlushChannel(lcid, L2CAP_FLUSH_CHANS_ALL);
  }

  if (p_scb->p_pkt != NULL) {
    osi_free_and_reset((void**)&p_scb->p_pkt);

    AVDT_TRACE_DEBUG("Dropped stored media packet");

    /* we need to call callback to keep data flow going */
    (*p_scb->stream_config.p_avdt_ctrl_cback)(
        avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_WRITE_CFM_EVT,
        &avdt_ctrl, p_scb->stream_config.scb_index);
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_chk_snd_pkt
 *
 * Description      This function checks if the SCB is congested, and if not
 *                  congested it sends a stored media packet, if any.  After it
 *                  sends the packet it calls the application callback function
 *                  with a write confirm.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_chk_snd_pkt(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  tAVDT_CTRL avdt_ctrl;
  BT_HDR* p_pkt;

  avdt_ctrl.hdr.err_code = 0;

  if (!p_scb->cong) {
    if (p_scb->p_pkt != NULL) {
      p_pkt = p_scb->p_pkt;
      p_scb->p_pkt = NULL;
      avdt_ad_write_req(AVDT_CHAN_MEDIA, p_scb->p_ccb, p_scb, p_pkt);

      (*p_scb->stream_config.p_avdt_ctrl_cback)(
          avdt_scb_to_hdl(p_scb), RawAddress::kEmpty, AVDT_WRITE_CFM_EVT,
          &avdt_ctrl, p_scb->stream_config.scb_index);
    }
  }
}

/*******************************************************************************
 *
 * Function         avdt_scb_transport_channel_timer
 *
 * Description      This function is called to start a timer when the peer
 *                  initiates closing of the stream.  The timer verifies that
 *                  the peer disconnects the transport channel.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_transport_channel_timer(AvdtpScb* p_scb,
                                      UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  alarm_set_on_mloop(p_scb->transport_channel_timer,
                     AVDT_SCB_TC_DISC_TIMEOUT_MS,
                     avdt_scb_transport_channel_timer_timeout, p_scb);
}

/*******************************************************************************
 *
 * Function         avdt_scb_clr_vars
 *
 * Description      This function initializes certain SCB variables.
 *
 * Returns          Nothing.
 *
 ******************************************************************************/
void avdt_scb_clr_vars(AvdtpScb* p_scb, UNUSED_ATTR tAVDT_SCB_EVT* p_data) {
  p_scb->in_use = false;
  p_scb->peer_seid = 0;
}
