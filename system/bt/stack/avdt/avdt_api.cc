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
 *  This module contains API of the audio/video distribution transport
 *  protocol.
 *
 ******************************************************************************/

#include "avdt_api.h"
#include <string.h>
#include "avdt_int.h"
#include "avdtc_api.h"
#include "bt_target.h"
#include "bt_types.h"
#include "btm_api.h"
#include "btu.h"
#include "l2c_api.h"
#include "stack/include/a2dp_codec_api.h"

/* Control block for AVDTP */
AvdtpCb avdtp_cb;

void avdt_ccb_idle_ccb_timer_timeout(void* data) {
  AvdtpCcb* p_ccb = (AvdtpCcb*)data;
  uint8_t avdt_event = AVDT_CCB_IDLE_TOUT_EVT;
  uint8_t err_code = AVDT_ERR_TIMEOUT;

  tAVDT_CCB_EVT avdt_ccb_evt;
  avdt_ccb_evt.err_code = err_code;
  avdt_ccb_event(p_ccb, avdt_event, &avdt_ccb_evt);
}

void avdt_ccb_ret_ccb_timer_timeout(void* data) {
  AvdtpCcb* p_ccb = (AvdtpCcb*)data;
  uint8_t avdt_event = AVDT_CCB_RET_TOUT_EVT;
  uint8_t err_code = AVDT_ERR_TIMEOUT;

  tAVDT_CCB_EVT avdt_ccb_evt;
  avdt_ccb_evt.err_code = err_code;
  avdt_ccb_event(p_ccb, avdt_event, &avdt_ccb_evt);
}

void avdt_ccb_rsp_ccb_timer_timeout(void* data) {
  AvdtpCcb* p_ccb = (AvdtpCcb*)data;
  uint8_t avdt_event = AVDT_CCB_RSP_TOUT_EVT;
  uint8_t err_code = AVDT_ERR_TIMEOUT;

  tAVDT_CCB_EVT avdt_ccb_evt;
  avdt_ccb_evt.err_code = err_code;
  avdt_ccb_event(p_ccb, avdt_event, &avdt_ccb_evt);
}

void avdt_scb_transport_channel_timer_timeout(void* data) {
  AvdtpScb* p_scb = (AvdtpScb*)data;
  uint8_t avdt_event = AVDT_SCB_TC_TOUT_EVT;

  avdt_scb_event(p_scb, avdt_event, NULL);
}

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
void AVDT_Register(AvdtpRcb* p_reg, tAVDT_CTRL_CBACK* p_cback) {
  /* register PSM with L2CAP */
  L2CA_Register(AVDT_PSM, (tL2CAP_APPL_INFO*)&avdt_l2c_appl,
                true /* enable_snoop */);

  /* set security level */
  BTM_SetSecurityLevel(true, "", BTM_SEC_SERVICE_AVDTP, p_reg->sec_mask,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_SIG);
  BTM_SetSecurityLevel(false, "", BTM_SEC_SERVICE_AVDTP, p_reg->sec_mask,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_SIG);

  /* do not use security on the media channel */
  BTM_SetSecurityLevel(true, "", BTM_SEC_SERVICE_AVDTP_NOSEC, BTM_SEC_NONE,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_MEDIA);
  BTM_SetSecurityLevel(false, "", BTM_SEC_SERVICE_AVDTP_NOSEC, BTM_SEC_NONE,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_MEDIA);

  /* do not use security on the reporting channel */
  BTM_SetSecurityLevel(true, "", BTM_SEC_SERVICE_AVDTP_NOSEC, BTM_SEC_NONE,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_REPORT);
  BTM_SetSecurityLevel(false, "", BTM_SEC_SERVICE_AVDTP_NOSEC, BTM_SEC_NONE,
                       AVDT_PSM, BTM_SEC_PROTO_AVDT, AVDT_CHAN_REPORT);

  /* initialize AVDTP data structures */
  avdt_scb_init();
  avdt_ccb_init();
  avdt_ad_init();

  /* copy registration struct */
  avdtp_cb.rcb = *p_reg;
  avdtp_cb.p_conn_cback = p_cback;
}

/*******************************************************************************
 *
 * Function         AVDT_Deregister
 *
 * Description      This function is called to deregister use AVDTP protocol.
 *                  It is called when AVDTP is no longer being used by any
 *                  application in the system.  Before this function can be
 *                  called, all streams must be removed with
 *                  AVDT_RemoveStream().
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void AVDT_Deregister(void) {
  /* deregister PSM with L2CAP */
  L2CA_Deregister(AVDT_PSM);
}

void AVDT_AbortReq(uint8_t handle) {
  AVDT_TRACE_WARNING("%s: handle=%d", __func__, handle);

  AvdtpScb* p_scb = avdt_scb_by_hdl(handle);
  if (p_scb != NULL) {
    avdt_scb_event(p_scb, AVDT_SCB_API_ABORT_REQ_EVT, NULL);
  } else {
    AVDT_TRACE_ERROR("%s Improper SCB, can not abort the stream", __func__);
  }
}

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
uint16_t AVDT_CreateStream(uint8_t peer_id, uint8_t* p_handle,
                           const AvdtpStreamConfig& avdtp_stream_config) {
  uint16_t result = AVDT_SUCCESS;
  AvdtpScb* p_scb;

  AVDT_TRACE_DEBUG("%s: peer_id=%d", __func__, peer_id);

  /* Verify parameters; if invalid, return failure */
  if (((avdtp_stream_config.cfg.psc_mask & (~AVDT_PSC)) != 0) ||
      (avdtp_stream_config.p_avdt_ctrl_cback == NULL)) {
    result = AVDT_BAD_PARAMS;
  }
  /* Allocate scb; if no scbs, return failure */
  else {
    p_scb = avdt_scb_alloc(peer_id, avdtp_stream_config);
    if (p_scb == NULL) {
      result = AVDT_NO_RESOURCES;
    } else {
      *p_handle = avdt_scb_to_hdl(p_scb);
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d handle=%d scb_index=%d", __func__, result,
                   *p_handle, avdtp_stream_config.scb_index);

  return result;
}

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
uint16_t AVDT_RemoveStream(uint8_t handle) {
  uint16_t result = AVDT_SUCCESS;
  AvdtpScb* p_scb;

  AVDT_TRACE_DEBUG("%s: handle=%d", __func__, handle);

  /* look up scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  } else {
    /* send remove event to scb */
    avdt_scb_event(p_scb, AVDT_SCB_API_REMOVE_EVT, NULL);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_DiscoverReq(const RawAddress& bd_addr, uint8_t channel_index,
                          tAVDT_SEP_INFO* p_sep_info, uint8_t max_seps,
                          tAVDT_CTRL_CBACK* p_cback) {
  AvdtpCcb* p_ccb;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_CCB_EVT evt;

  AVDT_TRACE_DEBUG("%s", __func__);

  /* find channel control block for this bd addr; if none, allocate one */
  p_ccb = avdt_ccb_by_bd(bd_addr);
  if (p_ccb == NULL) {
    p_ccb = avdt_ccb_alloc_by_channel_index(bd_addr, channel_index);
    if (p_ccb == NULL) {
      /* could not allocate channel control block */
      result = AVDT_NO_RESOURCES;
    }
  }

  if (result == AVDT_SUCCESS) {
    /* make sure no discovery or get capabilities req already in progress */
    if (p_ccb->proc_busy) {
      result = AVDT_BUSY;
    }
    /* send event to ccb */
    else {
      evt.discover.p_sep_info = p_sep_info;
      evt.discover.num_seps = max_seps;
      evt.discover.p_cback = p_cback;
      avdt_ccb_event(p_ccb, AVDT_CCB_API_DISCOVER_REQ_EVT, &evt);
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

/*******************************************************************************
 *
 * Function         avdt_get_cap_req
 *
 * Description      internal function to serve AVDT_GetCapReq
 *
 * Returns          AVDT_SUCCESS if successful, otherwise error.
 *
 ******************************************************************************/
static uint16_t avdt_get_cap_req(const RawAddress& bd_addr,
                                 uint8_t channel_index,
                                 tAVDT_CCB_API_GETCAP* p_evt) {
  AvdtpCcb* p_ccb = NULL;
  uint16_t result = AVDT_SUCCESS;

  AVDT_TRACE_DEBUG("%s", __func__);

  /* verify SEID */
  if ((p_evt->single.seid < AVDT_SEID_MIN) ||
      (p_evt->single.seid > AVDT_SEID_MAX)) {
    AVDT_TRACE_ERROR("seid: %d", p_evt->single.seid);
    result = AVDT_BAD_PARAMS;
  }
  /* find channel control block for this bd addr; if none, allocate one */
  else {
    p_ccb = avdt_ccb_by_bd(bd_addr);
    if (p_ccb == NULL) {
      p_ccb = avdt_ccb_alloc_by_channel_index(bd_addr, channel_index);
      if (p_ccb == NULL) {
        /* could not allocate channel control block */
        result = AVDT_NO_RESOURCES;
      }
    }
  }

  if (result == AVDT_SUCCESS) {
    /* make sure no discovery or get capabilities req already in progress */
    if (p_ccb->proc_busy) {
      result = AVDT_BUSY;
    }
    /* send event to ccb */
    else {
      avdt_ccb_event(p_ccb, AVDT_CCB_API_GETCAP_REQ_EVT, (tAVDT_CCB_EVT*)p_evt);
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_GetCapReq(const RawAddress& bd_addr, uint8_t channel_index,
                        uint8_t seid, AvdtpSepConfig* p_cfg,
                        tAVDT_CTRL_CBACK* p_cback, bool get_all_cap) {
  tAVDT_CCB_API_GETCAP getcap;
  uint16_t result = AVDT_SUCCESS;

  AVDT_TRACE_DEBUG("%s", __func__);

  getcap.single.seid = seid;
  if (get_all_cap) {
    getcap.single.sig_id = AVDT_SIG_GET_ALLCAP;
  } else {
    getcap.single.sig_id = AVDT_SIG_GETCAP;
  }
  getcap.p_cfg = p_cfg;
  getcap.p_cback = p_cback;
  result = avdt_get_cap_req(bd_addr, channel_index, &getcap);

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_DelayReport(uint8_t handle, uint8_t seid, uint16_t delay) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_SCB_EVT evt;

  AVDT_TRACE_DEBUG("%s: handle=%d ceid=%d delay=%d", __func__, handle, seid,
                   delay);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  } else
  /* send event to scb */
  {
    evt.apidelay.hdr.seid = seid;
    evt.apidelay.delay = delay;
    avdt_scb_event(p_scb, AVDT_SCB_API_DELAY_RPT_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_OpenReq(uint8_t handle, const RawAddress& bd_addr,
                      uint8_t channel_index, uint8_t seid,
                      AvdtpSepConfig* p_cfg) {
  AvdtpCcb* p_ccb = NULL;
  AvdtpScb* p_scb = NULL;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_SCB_EVT evt;

  AVDT_TRACE_DEBUG("%s: handle=%d seid=%d", __func__, handle, seid);

  /* verify SEID */
  if ((seid < AVDT_SEID_MIN) || (seid > AVDT_SEID_MAX)) {
    result = AVDT_BAD_PARAMS;
  }
  /* map handle to scb */
  else {
    p_scb = avdt_scb_by_hdl(handle);
    if (p_scb == NULL) {
      result = AVDT_BAD_HANDLE;
    }
    /* find channel control block for this bd addr; if none, allocate one */
    else {
      p_ccb = avdt_ccb_by_bd(bd_addr);
      if (p_ccb == NULL) {
        p_ccb = avdt_ccb_alloc_by_channel_index(bd_addr, channel_index);
        if (p_ccb == NULL) {
          /* could not allocate channel control block */
          result = AVDT_NO_RESOURCES;
        }
      }
    }
  }

  /* send event to scb */
  if (result == AVDT_SUCCESS) {
    AVDT_TRACE_DEBUG("%s: codec: %s", __func__,
                     A2DP_CodecInfoString(p_cfg->codec_info).c_str());

    evt.msg.config_cmd.hdr.seid = seid;
    evt.msg.config_cmd.hdr.ccb_idx = avdt_ccb_to_idx(p_ccb);
    evt.msg.config_cmd.int_seid = handle;
    evt.msg.config_cmd.p_cfg = p_cfg;
    avdt_scb_event(p_scb, AVDT_SCB_API_SETCONFIG_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_ConfigRsp(uint8_t handle, uint8_t label, uint8_t error_code,
                        uint8_t category) {
  AvdtpScb* p_scb;
  tAVDT_SCB_EVT evt;
  uint16_t result = AVDT_SUCCESS;
  uint8_t event_code;

  AVDT_TRACE_DEBUG("%s: handle=%d label=%d error_code=0x%x category=%d",
                   __func__, handle, label, error_code, category);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  }
  /* handle special case when this function is called but peer has not send
  ** a configuration cmd; ignore and return error result
  */
  else if (!p_scb->in_use) {
    result = AVDT_BAD_HANDLE;
  }
  /* send event to scb */
  else {
    evt.msg.hdr.err_code = error_code;
    evt.msg.hdr.err_param = category;
    evt.msg.hdr.label = label;
    if (error_code == 0) {
      event_code = AVDT_SCB_API_SETCONFIG_RSP_EVT;
    } else {
      event_code = AVDT_SCB_API_SETCONFIG_REJ_EVT;
    }
    avdt_scb_event(p_scb, event_code, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_StartReq(uint8_t* p_handles, uint8_t num_handles) {
  AvdtpScb* p_scb = NULL;
  tAVDT_CCB_EVT evt;
  uint16_t result = AVDT_SUCCESS;
  int i;

  AVDT_TRACE_DEBUG("%s: num_handles=%d", __func__, num_handles);

  if ((num_handles == 0) || (num_handles > AVDT_NUM_SEPS)) {
    result = AVDT_BAD_PARAMS;
  } else {
    /* verify handles */
    for (i = 0; i < num_handles; i++) {
      p_scb = avdt_scb_by_hdl(p_handles[i]);
      if (p_scb == NULL) {
        result = AVDT_BAD_HANDLE;
        break;
      }
    }
  }

  if (result == AVDT_SUCCESS) {
    if (p_scb->p_ccb == NULL) {
      result = AVDT_BAD_HANDLE;
    } else {
      /* send event to ccb */
      memcpy(evt.msg.multi.seid_list, p_handles, num_handles);
      evt.msg.multi.num_seps = num_handles;
      avdt_ccb_event(p_scb->p_ccb, AVDT_CCB_API_START_REQ_EVT, &evt);
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_SuspendReq(uint8_t* p_handles, uint8_t num_handles) {
  AvdtpScb* p_scb = NULL;
  tAVDT_CCB_EVT evt;
  uint16_t result = AVDT_SUCCESS;
  int i;

  AVDT_TRACE_DEBUG("%s: num_handles=%d", __func__, num_handles);

  if ((num_handles == 0) || (num_handles > AVDT_NUM_SEPS)) {
    result = AVDT_BAD_PARAMS;
  } else {
    /* verify handles */
    for (i = 0; i < num_handles; i++) {
      p_scb = avdt_scb_by_hdl(p_handles[i]);
      if (p_scb == NULL) {
        result = AVDT_BAD_HANDLE;
        break;
      }
    }
  }

  if (result == AVDT_SUCCESS) {
    if (p_scb->p_ccb == NULL) {
      result = AVDT_BAD_HANDLE;
    } else {
      /* send event to ccb */
      memcpy(evt.msg.multi.seid_list, p_handles, num_handles);
      evt.msg.multi.num_seps = num_handles;
      avdt_ccb_event(p_scb->p_ccb, AVDT_CCB_API_SUSPEND_REQ_EVT, &evt);
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_CloseReq(uint8_t handle) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_SUCCESS;

  AVDT_TRACE_DEBUG("%s: handle=%d", __func__, handle);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  } else
  /* send event to scb */
  {
    avdt_scb_event(p_scb, AVDT_SCB_API_CLOSE_REQ_EVT, NULL);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_ReconfigReq(uint8_t handle, AvdtpSepConfig* p_cfg) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_SCB_EVT evt;

  AVDT_TRACE_DEBUG("%s: handle=%d", __func__, handle);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  }
  /* send event to scb */
  else {
    /* force psc_mask to zero */
    p_cfg->psc_mask = 0;
    evt.msg.reconfig_cmd.p_cfg = p_cfg;
    avdt_scb_event(p_scb, AVDT_SCB_API_RECONFIG_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_ReconfigRsp(uint8_t handle, uint8_t label, uint8_t error_code,
                          uint8_t category) {
  AvdtpScb* p_scb;
  tAVDT_SCB_EVT evt;
  uint16_t result = AVDT_SUCCESS;

  AVDT_TRACE_DEBUG("%s: handle=%d label=%d error_code=0x%x category=%d",
                   __func__, handle, label, error_code, category);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  }
  /* send event to scb */
  else {
    evt.msg.hdr.err_code = error_code;
    evt.msg.hdr.err_param = category;
    evt.msg.hdr.label = label;
    avdt_scb_event(p_scb, AVDT_SCB_API_RECONFIG_RSP_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_SecurityReq(uint8_t handle, uint8_t* p_data, uint16_t len) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_SCB_EVT evt;

  AVDT_TRACE_DEBUG("%s: handle=%d len=%d", __func__, handle, len);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  }
  /* send event to scb */
  else {
    evt.msg.security_rsp.p_data = p_data;
    evt.msg.security_rsp.len = len;
    avdt_scb_event(p_scb, AVDT_SCB_API_SECURITY_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_SecurityRsp(uint8_t handle, uint8_t label, uint8_t error_code,
                          uint8_t* p_data, uint16_t len) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_SCB_EVT evt;

  AVDT_TRACE_DEBUG("%s: handle=%d label=%d error_code=0x%x len=%d", __func__,
                   handle, label, error_code, len);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  }
  /* send event to scb */
  else {
    evt.msg.security_rsp.hdr.err_code = error_code;
    evt.msg.security_rsp.hdr.label = label;
    evt.msg.security_rsp.p_data = p_data;
    evt.msg.security_rsp.len = len;
    avdt_scb_event(p_scb, AVDT_SCB_API_SECURITY_RSP_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

/*******************************************************************************
 *
 * Function         AVDT_WriteReqOpt
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
 *                  field must be equal to or greater than AVDT_MEDIA_OFFSET
 *                  (if NO_RTP is specified, L2CAP_MIN_OFFSET can be used).
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
uint16_t AVDT_WriteReqOpt(uint8_t handle, BT_HDR* p_pkt, uint32_t time_stamp,
                          uint8_t m_pt, tAVDT_DATA_OPT_MASK opt) {
  AvdtpScb* p_scb;
  tAVDT_SCB_EVT evt;
  uint16_t result = AVDT_SUCCESS;

  AVDT_TRACE_DEBUG("%s: handle=%d timestamp=%d m_pt=0x%x opt=0x%x", __func__,
                   handle, time_stamp, m_pt, opt);

  /* map handle to scb */
  p_scb = avdt_scb_by_hdl(handle);
  if (p_scb == NULL) {
    result = AVDT_BAD_HANDLE;
  } else {
    evt.apiwrite.p_buf = p_pkt;
    evt.apiwrite.time_stamp = time_stamp;
    evt.apiwrite.m_pt = m_pt;
    evt.apiwrite.opt = opt;
    avdt_scb_event(p_scb, AVDT_SCB_API_WRITE_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint16_t AVDT_WriteReq(uint8_t handle, BT_HDR* p_pkt, uint32_t time_stamp,
                       uint8_t m_pt) {
  return AVDT_WriteReqOpt(handle, p_pkt, time_stamp, m_pt, AVDT_DATA_OPT_NONE);
}

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
uint16_t AVDT_ConnectReq(const RawAddress& bd_addr, uint8_t channel_index,
                         uint8_t sec_mask, tAVDT_CTRL_CBACK* p_cback) {
  AvdtpCcb* p_ccb = NULL;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_CCB_EVT evt;

  AVDT_TRACE_WARNING("%s: address=%s channel_index=%d sec_mask=0x%x", __func__,
                     bd_addr.ToString().c_str(), channel_index, sec_mask);

  /* find channel control block for this bd addr; if none, allocate one */
  p_ccb = avdt_ccb_by_bd(bd_addr);
  if (p_ccb == NULL) {
    p_ccb = avdt_ccb_alloc_by_channel_index(bd_addr, channel_index);
    if (p_ccb == NULL) {
      /* could not allocate channel control block */
      result = AVDT_NO_RESOURCES;
    }
  } else if (!p_ccb->ll_opened) {
    AVDT_TRACE_WARNING("AVDT_ConnectReq: CCB LL is in the middle of opening");

    /* ccb was already allocated for the incoming signalling. */
    result = AVDT_BUSY;
  }

  if (result == AVDT_SUCCESS) {
    /* send event to ccb */
    evt.connect.p_cback = p_cback;
    evt.connect.sec_mask = sec_mask;
    avdt_ccb_event(p_ccb, AVDT_CCB_API_CONNECT_REQ_EVT, &evt);
  }

  AVDT_TRACE_WARNING("%s: address=%s result=%d", __func__,
                     bd_addr.ToString().c_str(), result);

  return result;
}

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
uint16_t AVDT_DisconnectReq(const RawAddress& bd_addr,
                            tAVDT_CTRL_CBACK* p_cback) {
  AvdtpCcb* p_ccb = NULL;
  uint16_t result = AVDT_SUCCESS;
  tAVDT_CCB_EVT evt;

  AVDT_TRACE_WARNING("%s: address=%s", __func__, bd_addr.ToString().c_str());

  /* find channel control block for this bd addr; if none, error */
  p_ccb = avdt_ccb_by_bd(bd_addr);
  if (p_ccb == NULL) {
    result = AVDT_BAD_PARAMS;
  }

  if (result == AVDT_SUCCESS) {
    /* send event to ccb */
    evt.disconnect.p_cback = p_cback;
    avdt_ccb_event(p_ccb, AVDT_CCB_API_DISCONNECT_REQ_EVT, &evt);
  }

  AVDT_TRACE_DEBUG("%s: address=%s result=%d", __func__,
                   bd_addr.ToString().c_str(), result);

  return result;
}

/*******************************************************************************
 *
 * Function         AVDT_GetL2CapChannel
 *
 * Description      Get the L2CAP CID used by the handle.
 *
 * Returns          CID if successful, otherwise 0.
 *
 ******************************************************************************/
uint16_t AVDT_GetL2CapChannel(uint8_t handle) {
  AvdtpScb* p_scb;
  AvdtpCcb* p_ccb;
  uint8_t tcid;
  uint16_t lcid = 0;

  /* map handle to scb */
  if (((p_scb = avdt_scb_by_hdl(handle)) != NULL) &&
      ((p_ccb = p_scb->p_ccb) != NULL)) {
    /* get tcid from type, scb */
    tcid = avdt_ad_type_to_tcid(AVDT_CHAN_MEDIA, p_scb);

    lcid = avdtp_cb.ad.rt_tbl[avdt_ccb_to_idx(p_ccb)][tcid].lcid;
  }

  return (lcid);
}

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
uint16_t AVDT_GetSignalChannel(uint8_t handle, const RawAddress& bd_addr) {
  AvdtpScb* p_scb;
  AvdtpCcb* p_ccb;
  uint8_t tcid = 0; /* tcid is always 0 for signal channel */
  uint16_t lcid = 0;

  /* map handle to scb */
  if (((p_scb = avdt_scb_by_hdl(handle)) != NULL) &&
      ((p_ccb = p_scb->p_ccb) != NULL)) {
    lcid = avdtp_cb.ad.rt_tbl[avdt_ccb_to_idx(p_ccb)][tcid].lcid;
  } else {
    p_ccb = avdt_ccb_by_bd(bd_addr);
    if (p_ccb != NULL) {
      lcid = avdtp_cb.ad.rt_tbl[avdt_ccb_to_idx(p_ccb)][tcid].lcid;
    }
  }

  return (lcid);
}

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
uint16_t AVDT_SendReport(uint8_t handle, AVDT_REPORT_TYPE type,
                         tAVDT_REPORT_DATA* p_data) {
  AvdtpScb* p_scb;
  uint16_t result = AVDT_BAD_PARAMS;
  AvdtpTransportChannel* p_tbl;
  uint8_t *p, *plen, *pm1, *p_end;
  uint32_t ssrc;
  uint16_t len;

  AVDT_TRACE_DEBUG("%s: handle=%d type=%d", __func__, handle, type);

  /* map handle to scb && verify parameters */
  if (((p_scb = avdt_scb_by_hdl(handle)) != NULL) && (p_scb->p_ccb != NULL) &&
      (((type == AVDT_RTCP_PT_SR) &&
        (p_scb->stream_config.tsep == AVDT_TSEP_SRC)) ||
       ((type == AVDT_RTCP_PT_RR) &&
        (p_scb->stream_config.tsep == AVDT_TSEP_SNK)) ||
       (type == AVDT_RTCP_PT_SDES))) {
    result = AVDT_NO_RESOURCES;

    /* build SR - assume fit in one packet */
    p_tbl = avdt_ad_tc_tbl_by_type(AVDT_CHAN_REPORT, p_scb->p_ccb, p_scb);
    if (p_tbl->state == AVDT_AD_ST_OPEN) {
      BT_HDR* p_pkt = (BT_HDR*)osi_malloc(p_tbl->peer_mtu + sizeof(BT_HDR));

      p_pkt->offset = L2CAP_MIN_OFFSET;
      p = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
      pm1 = p;
      *p++ = AVDT_MEDIA_OCTET1 | 1;
      *p++ = type;
      /* save the location for length */
      plen = p;
      p += 2;
      ssrc = avdt_scb_gen_ssrc(p_scb);
      UINT32_TO_BE_STREAM(p, ssrc);

      switch (type) {
        case AVDT_RTCP_PT_SR: /* Sender Report */
          *pm1 = AVDT_MEDIA_OCTET1;
          UINT32_TO_BE_STREAM(p, p_data->sr.ntp_sec);
          UINT32_TO_BE_STREAM(p, p_data->sr.ntp_frac);
          UINT32_TO_BE_STREAM(p, p_data->sr.rtp_time);
          UINT32_TO_BE_STREAM(p, p_data->sr.pkt_count);
          UINT32_TO_BE_STREAM(p, p_data->sr.octet_count);
          break;

        case AVDT_RTCP_PT_RR: /* Receiver Report */
          *p++ = p_data->rr.frag_lost;
          AVDT_TRACE_API("packet_lost: %d", p_data->rr.packet_lost);
          p_data->rr.packet_lost &= 0xFFFFFF;
          AVDT_TRACE_API("packet_lost: %d", p_data->rr.packet_lost);
          UINT24_TO_BE_STREAM(p, p_data->rr.packet_lost);
          UINT32_TO_BE_STREAM(p, p_data->rr.seq_num_rcvd);
          UINT32_TO_BE_STREAM(p, p_data->rr.jitter);
          UINT32_TO_BE_STREAM(p, p_data->rr.lsr);
          UINT32_TO_BE_STREAM(p, p_data->rr.dlsr);
          break;

        case AVDT_RTCP_PT_SDES: /* Source Description */
          *p++ = AVDT_RTCP_SDES_CNAME;
          len = strlen((char*)p_data->cname);
          if (len > AVDT_MAX_CNAME_SIZE) len = AVDT_MAX_CNAME_SIZE;
          *p++ = (uint8_t)len;
          strlcpy((char*)p, (char*)p_data->cname, len + 1);
          p += len;
          break;
      }
      p_end = p;
      len = p - pm1 - 1;
      UINT16_TO_BE_STREAM(plen, len);

      /* set the actual payload length */
      p_pkt->len = p_end - p;
      /* send the packet */
      if (L2CAP_DW_FAILED !=
          avdt_ad_write_req(AVDT_CHAN_REPORT, p_scb->p_ccb, p_scb, p_pkt))
        result = AVDT_SUCCESS;
    }
  }

  AVDT_TRACE_DEBUG("%s: result=%d", __func__, result);

  return result;
}

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
uint8_t AVDT_SetTraceLevel(uint8_t new_level) {
  if (new_level != 0xFF) avdtp_cb.SetTraceLevel(new_level);

  return avdtp_cb.TraceLevel();
}

void stack_debug_avdtp_api_dump(int fd) {
  if (appl_trace_level < BT_TRACE_LEVEL_DEBUG) return;

  dprintf(fd, "\nAVDTP Stack State:\n");
  dprintf(fd, "  AVDTP signalling L2CAP channel MTU: %d\n",
          avdtp_cb.rcb.ctrl_mtu);
  dprintf(fd, "  Security mask: 0x%x\n", avdtp_cb.rcb.sec_mask);

  for (size_t i = 0; i < AVDT_NUM_LINKS; i++) {
    const AvdtpCcb& ccb = avdtp_cb.ccb[i];
    if (ccb.peer_addr.IsEmpty()) {
      continue;
    }
    dprintf(fd, "\n  Channel control block: %zu peer: %s\n", i,
            ccb.peer_addr.ToString().c_str());
    dprintf(fd, "    Allocated: %s\n", ccb.allocated ? "true" : "false");
    dprintf(fd, "    State: %d\n", ccb.state);
    dprintf(fd, "    Link-layer opened: %s\n",
            ccb.ll_opened ? "true" : "false");
    dprintf(fd, "    Discover in progress: %s\n",
            ccb.proc_busy ? "true" : "false");
    dprintf(fd, "    Congested: %s\n", ccb.cong ? "true" : "false");
    dprintf(fd, "    Reinitiate connection on idle: %s\n",
            ccb.reconn ? "true" : "false");
    dprintf(fd, "    Command retransmission count: %d\n", ccb.ret_count);
    dprintf(fd, "    BTA AV SCB index: %d\n", ccb.BtaAvScbIndex());

    for (size_t i = 0; i < AVDT_NUM_SEPS; i++) {
      const AvdtpScb& scb = ccb.scb[i];
      if (!scb.in_use) {
        continue;
      }
      dprintf(fd, "\n    Stream control block: %zu\n", i);
      dprintf(fd, "      SEP codec: %s\n",
              A2DP_CodecName(scb.stream_config.cfg.codec_info));
      dprintf(fd, "      SEP protocol service capabilities: 0x%x\n",
              scb.stream_config.cfg.psc_mask);
      dprintf(fd, "      SEP type: 0x%x\n", scb.stream_config.tsep);
      dprintf(fd, "      Media type: 0x%x\n", scb.stream_config.media_type);
      dprintf(fd, "      MTU: %d\n", scb.stream_config.mtu);
      dprintf(fd, "      SCB handle: %d\n", scb.ScbHandle());
      dprintf(fd, "      SCB index: %d\n", scb.stream_config.scb_index);
      dprintf(fd, "      Configured codec: %s\n",
              A2DP_CodecName(scb.curr_cfg.codec_info));
      dprintf(fd, "      Requested codec: %s\n",
              A2DP_CodecName(scb.req_cfg.codec_info));
      dprintf(fd, "      Transport channel connect timer: %s\n",
              alarm_is_scheduled(scb.transport_channel_timer)
                  ? "Scheduled"
                  : "Not scheduled");
      dprintf(fd, "      Channel control block peer: %s\n",
              (scb.p_ccb != nullptr) ? scb.p_ccb->peer_addr.ToString().c_str()
                                     : "null");
      dprintf(fd, "      Allocated: %s\n", scb.allocated ? "true" : "false");
      dprintf(fd, "      In use: %s\n", scb.in_use ? "true" : "false");
      dprintf(fd, "      Role: 0x%x\n", scb.role);
      dprintf(fd, "      Remove: %s\n", scb.remove ? "true" : "false");
      dprintf(fd, "      State: %d\n", scb.state);
      dprintf(fd, "      Peer SEID: %d\n", scb.peer_seid);
      dprintf(fd, "      Current event: %d\n", scb.curr_evt);
      dprintf(fd, "      Congested: %s\n", scb.cong ? "true" : "false");
      dprintf(fd, "      Close response code: %d\n", scb.close_code);
    }
  }
}
