/******************************************************************************
 *
 *  Copyright 2001-2012 Broadcom Corporation
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
 *  This file contains BNEP utility functions
 *
 ******************************************************************************/

#include <log/log.h>
#include <stdio.h>
#include <string.h>

#include "bnep_int.h"
#include "bt_common.h"
#include "bt_types.h"
#include "bt_utils.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "osi/include/osi.h"

using bluetooth::Uuid;

/******************************************************************************/
/*            L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/******************************************************************************/
static uint8_t* bnepu_init_hdr(BT_HDR* p_buf, uint16_t hdr_len,
                               uint8_t pkt_type);

void bnepu_process_peer_multicast_filter_set(tBNEP_CONN* p_bcb,
                                             uint8_t* p_filters, uint16_t len);
void bnepu_send_peer_multicast_filter_rsp(tBNEP_CONN* p_bcb,
                                          uint16_t response_code);

/*******************************************************************************
 *
 * Function         bnepu_find_bcb_by_cid
 *
 * Description      This function searches the bcb table for an entry with the
 *                  passed CID.
 *
 * Returns          the BCB address, or NULL if not found.
 *
 ******************************************************************************/
tBNEP_CONN* bnepu_find_bcb_by_cid(uint16_t cid) {
  uint16_t xx;
  tBNEP_CONN* p_bcb;

  /* Look through each connection control block */
  for (xx = 0, p_bcb = bnep_cb.bcb; xx < BNEP_MAX_CONNECTIONS; xx++, p_bcb++) {
    if ((p_bcb->con_state != BNEP_STATE_IDLE) && (p_bcb->l2cap_cid == cid))
      return (p_bcb);
  }

  /* If here, not found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         bnepu_find_bcb_by_bd_addr
 *
 * Description      This function searches the BCB table for an entry with the
 *                  passed Bluetooth Address.
 *
 * Returns          the BCB address, or NULL if not found.
 *
 ******************************************************************************/
tBNEP_CONN* bnepu_find_bcb_by_bd_addr(const RawAddress& p_bda) {
  uint16_t xx;
  tBNEP_CONN* p_bcb;

  /* Look through each connection control block */
  for (xx = 0, p_bcb = bnep_cb.bcb; xx < BNEP_MAX_CONNECTIONS; xx++, p_bcb++) {
    if (p_bcb->con_state != BNEP_STATE_IDLE) {
      if (p_bcb->rem_bda == p_bda) return (p_bcb);
    }
  }

  /* If here, not found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         bnepu_allocate_bcb
 *
 * Description      This function allocates a new BCB.
 *
 * Returns          BCB address, or NULL if none available.
 *
 ******************************************************************************/
tBNEP_CONN* bnepu_allocate_bcb(const RawAddress& p_rem_bda) {
  uint16_t xx;
  tBNEP_CONN* p_bcb;

  /* Look through each connection control block for a free one */
  for (xx = 0, p_bcb = bnep_cb.bcb; xx < BNEP_MAX_CONNECTIONS; xx++, p_bcb++) {
    if (p_bcb->con_state == BNEP_STATE_IDLE) {
      alarm_free(p_bcb->conn_timer);
      memset((uint8_t*)p_bcb, 0, sizeof(tBNEP_CONN));
      p_bcb->conn_timer = alarm_new("bnep.conn_timer");

      p_bcb->rem_bda = p_rem_bda;
      p_bcb->handle = xx + 1;
      p_bcb->xmit_q = fixed_queue_new(SIZE_MAX);

      return (p_bcb);
    }
  }

  /* If here, no free BCB found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         bnepu_release_bcb
 *
 * Description      This function releases a BCB.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_release_bcb(tBNEP_CONN* p_bcb) {
  /* Ensure timer is stopped */
  alarm_free(p_bcb->conn_timer);
  p_bcb->conn_timer = NULL;

  /* Drop any response pointer we may be holding */
  p_bcb->con_state = BNEP_STATE_IDLE;
  osi_free_and_reset((void**)&p_bcb->p_pending_data);

  /* Free transmit queue */
  while (!fixed_queue_is_empty(p_bcb->xmit_q)) {
    osi_free(fixed_queue_try_dequeue(p_bcb->xmit_q));
  }
  fixed_queue_free(p_bcb->xmit_q, NULL);
  p_bcb->xmit_q = NULL;
}

/*******************************************************************************
 *
 * Function         bnep_send_conn_req
 *
 * Description      This function sends a BNEP connection request to peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_send_conn_req(tBNEP_CONN* p_bcb) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t *p, *p_start;

  BNEP_TRACE_DEBUG("%s: sending setup req with dst uuid %s", __func__,
                   p_bcb->dst_uuid.ToString().c_str());

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = p_start = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_SETUP_CONNECTION_REQUEST_MSG);

  int len = std::max(p_bcb->dst_uuid.GetShortestRepresentationSize(),
                     p_bcb->src_uuid.GetShortestRepresentationSize());

  UINT8_TO_BE_STREAM(p, len);

  if (len == Uuid::kNumBytes16) {
    UINT16_TO_BE_STREAM(p, p_bcb->dst_uuid.As16Bit());
    UINT16_TO_BE_STREAM(p, p_bcb->src_uuid.As16Bit());
  } else if (len == Uuid::kNumBytes32) {
    UINT32_TO_BE_STREAM(p, p_bcb->dst_uuid.As32Bit());
    UINT32_TO_BE_STREAM(p, p_bcb->src_uuid.As32Bit());
  } else if (len == Uuid::kNumBytes128) {
    memcpy(p, p_bcb->dst_uuid.To128BitBE().data(), Uuid::kNumBytes128);
    p += Uuid::kNumBytes128;
    memcpy(p, p_bcb->src_uuid.To128BitBE().data(), Uuid::kNumBytes128);
    p += Uuid::kNumBytes128;
  } else {
    BNEP_TRACE_ERROR("%s: uuid: %s, invalid length: %zu", __func__,
                     p_bcb->dst_uuid.ToString().c_str(),
                     p_bcb->dst_uuid.GetShortestRepresentationSize());
  }

  p_buf->len = (uint16_t)(p - p_start);

  bnepu_check_send_packet(p_bcb, p_buf);
}

/*******************************************************************************
 *
 * Function         bnep_send_conn_responce
 *
 * Description      This function sends a BNEP setup response to peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_send_conn_responce(tBNEP_CONN* p_bcb, uint16_t resp_code) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;

  BNEP_TRACE_EVENT("BNEP - bnep_send_conn_responce for CID: 0x%x",
                   p_bcb->l2cap_cid);

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_SETUP_CONNECTION_RESPONSE_MSG);

  UINT16_TO_BE_STREAM(p, resp_code);

  p_buf->len = 4;

  bnepu_check_send_packet(p_bcb, p_buf);
}

/*******************************************************************************
 *
 * Function         bnepu_send_peer_our_filters
 *
 * Description      This function sends our filters to a peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_send_peer_our_filters(tBNEP_CONN* p_bcb) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;
  uint16_t xx;

  BNEP_TRACE_DEBUG("BNEP sending peer our filters");

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_FILTER_NET_TYPE_SET_MSG);

  UINT16_TO_BE_STREAM(p, (4 * p_bcb->sent_num_filters));
  for (xx = 0; xx < p_bcb->sent_num_filters; xx++) {
    UINT16_TO_BE_STREAM(p, p_bcb->sent_prot_filter_start[xx]);
    UINT16_TO_BE_STREAM(p, p_bcb->sent_prot_filter_end[xx]);
  }

  p_buf->len = 4 + (4 * p_bcb->sent_num_filters);

  bnepu_check_send_packet(p_bcb, p_buf);

  p_bcb->con_flags |= BNEP_FLAGS_FILTER_RESP_PEND;

  /* Start timer waiting for setup response */
  alarm_set_on_mloop(p_bcb->conn_timer, BNEP_FILTER_SET_TIMEOUT_MS,
                     bnep_conn_timer_timeout, p_bcb);
}

/*******************************************************************************
 *
 * Function         bnepu_send_peer_our_multi_filters
 *
 * Description      This function sends our multicast filters to a peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_send_peer_our_multi_filters(tBNEP_CONN* p_bcb) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;
  uint16_t xx;

  BNEP_TRACE_DEBUG("BNEP sending peer our multicast filters");

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_FILTER_MULTI_ADDR_SET_MSG);

  UINT16_TO_BE_STREAM(p, (2 * BD_ADDR_LEN * p_bcb->sent_mcast_filters));
  for (xx = 0; xx < p_bcb->sent_mcast_filters; xx++) {
    memcpy(p, p_bcb->sent_mcast_filter_start[xx].address, BD_ADDR_LEN);
    p += BD_ADDR_LEN;
    memcpy(p, p_bcb->sent_mcast_filter_end[xx].address, BD_ADDR_LEN);
    p += BD_ADDR_LEN;
  }

  p_buf->len = 4 + (2 * BD_ADDR_LEN * p_bcb->sent_mcast_filters);

  bnepu_check_send_packet(p_bcb, p_buf);

  p_bcb->con_flags |= BNEP_FLAGS_MULTI_RESP_PEND;

  /* Start timer waiting for setup response */
  alarm_set_on_mloop(p_bcb->conn_timer, BNEP_FILTER_SET_TIMEOUT_MS,
                     bnep_conn_timer_timeout, p_bcb);
}

/*******************************************************************************
 *
 * Function         bnepu_send_peer_filter_rsp
 *
 * Description      This function sends a filter response to a peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_send_peer_filter_rsp(tBNEP_CONN* p_bcb, uint16_t response_code) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;

  BNEP_TRACE_DEBUG("BNEP sending filter response");

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_FILTER_NET_TYPE_RESPONSE_MSG);

  UINT16_TO_BE_STREAM(p, response_code);

  p_buf->len = 4;

  bnepu_check_send_packet(p_bcb, p_buf);
}

/*******************************************************************************
 *
 * Function         bnep_send_command_not_understood
 *
 * Description      This function sends a BNEP command not understood message
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_send_command_not_understood(tBNEP_CONN* p_bcb, uint8_t cmd_code) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;

  BNEP_TRACE_EVENT(
      "BNEP - bnep_send_command_not_understood for CID: 0x%x, cmd 0x%x",
      p_bcb->l2cap_cid, cmd_code);

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD);

  UINT8_TO_BE_STREAM(p, cmd_code);

  p_buf->len = 3;

  bnepu_check_send_packet(p_bcb, p_buf);
}

/*******************************************************************************
 *
 * Function         bnepu_check_send_packet
 *
 * Description      This function tries to send a packet to L2CAP.
 *                  If L2CAP is flow controlled, it enqueues the
 *                  packet to the transmit queue
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_check_send_packet(tBNEP_CONN* p_bcb, BT_HDR* p_buf) {
  BNEP_TRACE_EVENT("BNEP - bnepu_check_send_packet for CID: 0x%x",
                   p_bcb->l2cap_cid);
  if (p_bcb->con_flags & BNEP_FLAGS_L2CAP_CONGESTED) {
    if (fixed_queue_length(p_bcb->xmit_q) >= BNEP_MAX_XMITQ_DEPTH) {
      BNEP_TRACE_EVENT("BNEP - congested, dropping buf, CID: 0x%x",
                       p_bcb->l2cap_cid);

      osi_free(p_buf);
    } else {
      fixed_queue_enqueue(p_bcb->xmit_q, p_buf);
    }
  } else {
    L2CA_DataWrite(p_bcb->l2cap_cid, p_buf);
  }
}

/*******************************************************************************
 *
 * Function         bnepu_build_bnep_hdr
 *
 * Description      This function builds the BNEP header for a packet
 *                  Extension headers are not sent yet, so there is no
 *                  check for that.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_build_bnep_hdr(tBNEP_CONN* p_bcb, BT_HDR* p_buf, uint16_t protocol,
                          const RawAddress* p_src_addr,
                          const RawAddress* p_dest_addr, bool fw_ext_present) {
  const controller_t* controller = controller_get_interface();
  uint8_t ext_bit, *p = (uint8_t*)NULL;
  uint8_t type = BNEP_FRAME_COMPRESSED_ETHERNET;

  ext_bit = fw_ext_present ? 0x80 : 0x00;

  if (p_src_addr && *p_src_addr != *controller->get_address())
    type = BNEP_FRAME_COMPRESSED_ETHERNET_SRC_ONLY;

  if (*p_dest_addr != p_bcb->rem_bda)
    type = (type == BNEP_FRAME_COMPRESSED_ETHERNET)
               ? BNEP_FRAME_COMPRESSED_ETHERNET_DEST_ONLY
               : BNEP_FRAME_GENERAL_ETHERNET;

  if (!p_src_addr) p_src_addr = controller->get_address();

  switch (type) {
    case BNEP_FRAME_GENERAL_ETHERNET:
      p = bnepu_init_hdr(p_buf, 15,
                         (uint8_t)(ext_bit | BNEP_FRAME_GENERAL_ETHERNET));

      memcpy(p, p_dest_addr->address, BD_ADDR_LEN);
      p += BD_ADDR_LEN;

      memcpy(p, p_src_addr->address, BD_ADDR_LEN);
      p += BD_ADDR_LEN;
      break;

    case BNEP_FRAME_COMPRESSED_ETHERNET:
      p = bnepu_init_hdr(p_buf, 3,
                         (uint8_t)(ext_bit | BNEP_FRAME_COMPRESSED_ETHERNET));
      break;

    case BNEP_FRAME_COMPRESSED_ETHERNET_SRC_ONLY:
      p = bnepu_init_hdr(
          p_buf, 9,
          (uint8_t)(ext_bit | BNEP_FRAME_COMPRESSED_ETHERNET_SRC_ONLY));

      memcpy(p, p_src_addr->address, BD_ADDR_LEN);
      p += BD_ADDR_LEN;
      break;

    case BNEP_FRAME_COMPRESSED_ETHERNET_DEST_ONLY:
      p = bnepu_init_hdr(
          p_buf, 9,
          (uint8_t)(ext_bit | BNEP_FRAME_COMPRESSED_ETHERNET_DEST_ONLY));

      memcpy(p, p_dest_addr->address, BD_ADDR_LEN);
      p += BD_ADDR_LEN;
      break;
  }

  UINT16_TO_BE_STREAM(p, protocol);
}

/*******************************************************************************
 *
 * Function         bnepu_init_hdr
 *
 * Description      This function initializes the BNEP header
 *
 * Returns          pointer to header in buffer
 *
 ******************************************************************************/
static uint8_t* bnepu_init_hdr(BT_HDR* p_buf, uint16_t hdr_len,
                               uint8_t pkt_type) {
  uint8_t* p = (uint8_t*)(p_buf + 1) + p_buf->offset;

  /* See if we need to make space in the buffer */
  if (p_buf->offset < (hdr_len + L2CAP_MIN_OFFSET)) {
    uint16_t xx, diff = BNEP_MINIMUM_OFFSET - p_buf->offset;
    p = p + p_buf->len - 1;
    for (xx = 0; xx < p_buf->len; xx++, p--) p[diff] = *p;

    p_buf->offset = BNEP_MINIMUM_OFFSET;
    p = (uint8_t*)(p_buf + 1) + p_buf->offset;
  }

  p_buf->len += hdr_len;
  p_buf->offset -= hdr_len;
  p -= hdr_len;

  *p++ = pkt_type;

  return (p);
}

/*******************************************************************************
 *
 * Function         bnep_process_setup_conn_req
 *
 * Description      This function processes a peer's setup connection request
 *                  message. The destination UUID is verified and response sent
 *                  Connection open indication will be given to PAN profile
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_process_setup_conn_req(tBNEP_CONN* p_bcb, uint8_t* p_setup,
                                 uint8_t len) {
  BNEP_TRACE_EVENT("BNEP - %s for CID: 0x%x", __func__, p_bcb->l2cap_cid);

  if (p_bcb->con_state != BNEP_STATE_CONN_SETUP &&
      p_bcb->con_state != BNEP_STATE_SEC_CHECKING &&
      p_bcb->con_state != BNEP_STATE_CONNECTED) {
    BNEP_TRACE_ERROR("BNEP - setup request in bad state %d", p_bcb->con_state);
    bnep_send_conn_responce(p_bcb, BNEP_SETUP_CONN_NOT_ALLOWED);
    return;
  }

  /* Check if we already initiated security check or if waiting for user
   * responce */
  if (p_bcb->con_flags & BNEP_FLAGS_SETUP_RCVD) {
    BNEP_TRACE_EVENT(
        "BNEP - Duplicate Setup message received while doing security check");
    return;
  }

  /* Check if peer is the originator */
  if (p_bcb->con_state != BNEP_STATE_CONNECTED &&
      (!(p_bcb->con_flags & BNEP_FLAGS_SETUP_RCVD)) &&
      (p_bcb->con_flags & BNEP_FLAGS_IS_ORIG)) {
    BNEP_TRACE_ERROR("BNEP - setup request when we are originator",
                     p_bcb->con_state);
    bnep_send_conn_responce(p_bcb, BNEP_SETUP_CONN_NOT_ALLOWED);
    return;
  }

  if (p_bcb->con_state == BNEP_STATE_CONNECTED) {
    p_bcb->prv_src_uuid = p_bcb->src_uuid;
    p_bcb->prv_dst_uuid = p_bcb->dst_uuid;
  }

  if (len == Uuid::kNumBytes16) {
    /* because peer initiated connection keep src uuid as dst uuid */
    uint16_t tmp;

    BE_STREAM_TO_UINT16(tmp, p_setup);
    p_bcb->src_uuid = Uuid::From16Bit(tmp);

    BE_STREAM_TO_UINT16(tmp, p_setup);
    p_bcb->dst_uuid = Uuid::From16Bit(tmp);

    /* If nothing has changed don't bother the profile */
    if (p_bcb->con_state == BNEP_STATE_CONNECTED &&
        p_bcb->src_uuid == p_bcb->prv_src_uuid &&
        p_bcb->dst_uuid == p_bcb->prv_dst_uuid) {
      bnep_send_conn_responce(p_bcb, BNEP_SETUP_CONN_OK);
      return;
    }
  } else if (len == Uuid::kNumBytes32) {
    uint32_t tmp;

    BE_STREAM_TO_UINT32(tmp, p_setup);
    p_bcb->src_uuid = Uuid::From32Bit(tmp);

    BE_STREAM_TO_UINT32(tmp, p_setup);
    p_bcb->dst_uuid = Uuid::From32Bit(tmp);
  } else if (len == Uuid::kNumBytes128) {
    p_bcb->src_uuid = Uuid::From128BitBE(p_setup);
    p_setup += len;

    p_bcb->dst_uuid = Uuid::From128BitBE(p_setup);
    p_setup += len;
  } else {
    BNEP_TRACE_ERROR("BNEP - Bad UID len %d in ConnReq", len);
    bnep_send_conn_responce(p_bcb, BNEP_SETUP_INVALID_UUID_SIZE);
    return;
  }

  p_bcb->con_state = BNEP_STATE_SEC_CHECKING;
  p_bcb->con_flags |= BNEP_FLAGS_SETUP_RCVD;

  BNEP_TRACE_EVENT(
      "BNEP initiating security check for incoming call for uuid %s",
      p_bcb->src_uuid.ToString().c_str());
#if (BNEP_DO_AUTH_FOR_ROLE_SWITCH == FALSE)
  if (p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED)
    bnep_sec_check_complete(p_bcb->rem_bda, p_bcb, BTM_SUCCESS);
  else
#endif
    btm_sec_mx_access_request(p_bcb->rem_bda, BT_PSM_BNEP, false,
                              BTM_SEC_PROTO_BNEP, p_bcb->src_uuid.As32Bit(),
                              &bnep_sec_check_complete, p_bcb);

  return;
}

/*******************************************************************************
 *
 * Function         bnep_process_setup_conn_responce
 *
 * Description      This function processes a peer's setup connection response
 *                  message. The response code is verified and
 *                  Connection open indication will be given to PAN profile
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_process_setup_conn_responce(tBNEP_CONN* p_bcb, uint8_t* p_setup) {
  tBNEP_RESULT resp;
  uint16_t resp_code;

  BNEP_TRACE_DEBUG("BNEP received setup responce");
  /* The state should be either SETUP or CONNECTED */
  if (p_bcb->con_state != BNEP_STATE_CONN_SETUP) {
    /* Should we disconnect ? */
    BNEP_TRACE_ERROR("BNEP - setup response in bad state %d", p_bcb->con_state);
    return;
  }

  /* Check if we are the originator */
  if (!(p_bcb->con_flags & BNEP_FLAGS_IS_ORIG)) {
    BNEP_TRACE_ERROR("BNEP - setup response when we are not originator",
                     p_bcb->con_state);
    return;
  }

  BE_STREAM_TO_UINT16(resp_code, p_setup);

  switch (resp_code) {
    case BNEP_SETUP_INVALID_SRC_UUID:
      resp = BNEP_CONN_FAILED_SRC_UUID;
      break;

    case BNEP_SETUP_INVALID_DEST_UUID:
      resp = BNEP_CONN_FAILED_DST_UUID;
      break;

    case BNEP_SETUP_INVALID_UUID_SIZE:
      resp = BNEP_CONN_FAILED_UUID_SIZE;
      break;

    case BNEP_SETUP_CONN_NOT_ALLOWED:
    default:
      resp = BNEP_CONN_FAILED;
      break;
  }

  /* Check the responce code */
  if (resp_code != BNEP_SETUP_CONN_OK) {
    if (p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED) {
      BNEP_TRACE_EVENT("BNEP - role change response is %d", resp_code);

      /* Restore the earlier BNEP status */
      p_bcb->con_state = BNEP_STATE_CONNECTED;
      p_bcb->con_flags &= (~BNEP_FLAGS_SETUP_RCVD);
      p_bcb->src_uuid = p_bcb->prv_src_uuid;
      p_bcb->dst_uuid = p_bcb->prv_dst_uuid;

      /* Ensure timer is stopped */
      alarm_cancel(p_bcb->conn_timer);
      p_bcb->re_transmits = 0;

      /* Tell the user if he has a callback */
      if (bnep_cb.p_conn_state_cb)
        (*bnep_cb.p_conn_state_cb)(p_bcb->handle, p_bcb->rem_bda, resp, true);

      return;
    } else {
      BNEP_TRACE_ERROR("BNEP - setup response %d is not OK", resp_code);

      L2CA_DisconnectReq(p_bcb->l2cap_cid);

      /* Tell the user if he has a callback */
      if ((p_bcb->con_flags & BNEP_FLAGS_IS_ORIG) && (bnep_cb.p_conn_state_cb))
        (*bnep_cb.p_conn_state_cb)(p_bcb->handle, p_bcb->rem_bda, resp, false);

      bnepu_release_bcb(p_bcb);
      return;
    }
  }

  /* Received successful responce */
  bnep_connected(p_bcb);
}

/*******************************************************************************
 *
 * Function         bnep_process_control_packet
 *
 * Description      This function processes a peer's setup connection request
 *                  message. The destination UUID is verified and response sent
 *                  Connection open indication will be given to PAN profile
 *
 * Returns          void
 *
 ******************************************************************************/
uint8_t* bnep_process_control_packet(tBNEP_CONN* p_bcb, uint8_t* p,
                                     uint16_t* rem_len, bool is_ext) {
  uint8_t control_type;
  uint16_t len, ext_len = 0;

  if (p == NULL || rem_len == NULL) {
    if (rem_len != NULL) *rem_len = 0;
    BNEP_TRACE_DEBUG("%s: invalid packet: p = %p rem_len = %p", __func__, p,
                     rem_len);
    return NULL;
  }
  uint16_t rem_len_orig = *rem_len;

  if (is_ext) {
    if (*rem_len < 1) goto bad_packet_length;
    ext_len = *p++;
    *rem_len = *rem_len - 1;
  }

  if (*rem_len < 1) goto bad_packet_length;
  control_type = *p++;
  *rem_len = *rem_len - 1;

  BNEP_TRACE_EVENT(
      "%s: BNEP processing control packet rem_len %d, is_ext %d, ctrl_type %d",
      __func__, *rem_len, is_ext, control_type);

  switch (control_type) {
    case BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD:
      if (*rem_len < 1) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD with bad length",
            __func__);
        goto bad_packet_length;
      }
      BNEP_TRACE_ERROR(
          "%s: Received BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD for pkt type: %d",
          __func__, *p);
      p++;
      *rem_len = *rem_len - 1;
      break;

    case BNEP_SETUP_CONNECTION_REQUEST_MSG:
      if (*rem_len < 1) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_SETUP_CONNECTION_REQUEST_MSG with bad length",
            __func__);
        android_errorWriteLog(0x534e4554, "69177292");
        goto bad_packet_length;
      }
      len = *p++;
      if (*rem_len < ((2 * len) + 1)) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_SETUP_CONNECTION_REQUEST_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      if (!is_ext) bnep_process_setup_conn_req(p_bcb, p, (uint8_t)len);
      p += (2 * len);
      *rem_len = *rem_len - (2 * len) - 1;
      break;

    case BNEP_SETUP_CONNECTION_RESPONSE_MSG:
      if (*rem_len < 2) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_SETUP_CONNECTION_RESPONSE_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      if (!is_ext) bnep_process_setup_conn_responce(p_bcb, p);
      p += 2;
      *rem_len = *rem_len - 2;
      break;

    case BNEP_FILTER_NET_TYPE_SET_MSG:
      if (*rem_len < 2) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_NET_TYPE_SET_MSG with bad length",
            __func__);
        android_errorWriteLog(0x534e4554, "69177292");
        goto bad_packet_length;
      }
      BE_STREAM_TO_UINT16(len, p);
      if (*rem_len < (len + 2)) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_NET_TYPE_SET_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      bnepu_process_peer_filter_set(p_bcb, p, len);
      p += len;
      *rem_len = *rem_len - len - 2;
      break;

    case BNEP_FILTER_NET_TYPE_RESPONSE_MSG:
      if (*rem_len < 2) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_NET_TYPE_RESPONSE_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      bnepu_process_peer_filter_rsp(p_bcb, p);
      p += 2;
      *rem_len = *rem_len - 2;
      break;

    case BNEP_FILTER_MULTI_ADDR_SET_MSG:
      if (*rem_len < 2) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_MULTI_ADDR_SET_MSG with bad length",
            __func__);
        android_errorWriteLog(0x534e4554, "69177292");
        goto bad_packet_length;
      }
      BE_STREAM_TO_UINT16(len, p);
      if (*rem_len < (len + 2)) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_MULTI_ADDR_SET_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      bnepu_process_peer_multicast_filter_set(p_bcb, p, len);
      p += len;
      *rem_len = *rem_len - len - 2;
      break;

    case BNEP_FILTER_MULTI_ADDR_RESPONSE_MSG:
      if (*rem_len < 2) {
        BNEP_TRACE_ERROR(
            "%s: Received BNEP_FILTER_MULTI_ADDR_RESPONSE_MSG with bad length",
            __func__);
        goto bad_packet_length;
      }
      bnepu_process_multicast_filter_rsp(p_bcb, p);
      p += 2;
      *rem_len = *rem_len - 2;
      break;

    default:
      BNEP_TRACE_ERROR("%s: BNEP - bad ctl pkt type: %d", __func__,
                       control_type);
      bnep_send_command_not_understood(p_bcb, control_type);
      if (is_ext && (ext_len > 0)) {
        if (*rem_len < (ext_len - 1)) {
          goto bad_packet_length;
        }
        p += (ext_len - 1);
        *rem_len -= (ext_len - 1);
      }
      break;
  }
  return p;

bad_packet_length:
  BNEP_TRACE_ERROR("%s: bad control packet length: original=%d remaining=%d",
                   __func__, rem_len_orig, *rem_len);
  *rem_len = 0;
  return NULL;
}

/*******************************************************************************
 *
 * Function         bnepu_process_peer_filter_set
 *
 * Description      This function processes a peer's filter control
 *                  'set' message. The filters are stored in the BCB,
 *                  and an appropriate filter response message sent.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_process_peer_filter_set(tBNEP_CONN* p_bcb, uint8_t* p_filters,
                                   uint16_t len) {
  uint16_t num_filters = 0;
  uint16_t xx, resp_code = BNEP_FILTER_CRL_OK;
  uint16_t start, end;
  uint8_t* p_temp_filters;

  if ((p_bcb->con_state != BNEP_STATE_CONNECTED) &&
      (!(p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED))) {
    BNEP_TRACE_DEBUG(
        "BNEP received filter set from peer when there is no connection");
    return;
  }

  BNEP_TRACE_DEBUG("BNEP received filter set from peer");
  /* Check for length not a multiple of 4 */
  if (len & 3) {
    BNEP_TRACE_EVENT("BNEP - bad filter len: %d", len);
    bnepu_send_peer_filter_rsp(p_bcb, BNEP_FILTER_CRL_BAD_RANGE);
    return;
  }

  if (len) num_filters = (uint16_t)(len >> 2);

  /* Validate filter values */
  if (num_filters <= BNEP_MAX_PROT_FILTERS) {
    p_temp_filters = p_filters;
    for (xx = 0; xx < num_filters; xx++) {
      BE_STREAM_TO_UINT16(start, p_temp_filters);
      BE_STREAM_TO_UINT16(end, p_temp_filters);

      if (start > end) {
        resp_code = BNEP_FILTER_CRL_BAD_RANGE;
        break;
      }
    }
  } else
    resp_code = BNEP_FILTER_CRL_MAX_REACHED;

  if (resp_code != BNEP_FILTER_CRL_OK) {
    bnepu_send_peer_filter_rsp(p_bcb, resp_code);
    return;
  }

  if (bnep_cb.p_filter_ind_cb)
    (*bnep_cb.p_filter_ind_cb)(p_bcb->handle, true, 0, len, p_filters);

  p_bcb->rcvd_num_filters = num_filters;
  for (xx = 0; xx < num_filters; xx++) {
    BE_STREAM_TO_UINT16(start, p_filters);
    BE_STREAM_TO_UINT16(end, p_filters);

    p_bcb->rcvd_prot_filter_start[xx] = start;
    p_bcb->rcvd_prot_filter_end[xx] = end;
  }

  bnepu_send_peer_filter_rsp(p_bcb, resp_code);
}

/*******************************************************************************
 *
 * Function         bnepu_process_peer_filter_rsp
 *
 * Description      This function processes a peer's filter control
 *                  'response' message.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_process_peer_filter_rsp(tBNEP_CONN* p_bcb, uint8_t* p_data) {
  uint16_t resp_code;
  tBNEP_RESULT result;

  BNEP_TRACE_DEBUG("BNEP received filter responce");
  /* The state should be  CONNECTED */
  if ((p_bcb->con_state != BNEP_STATE_CONNECTED) &&
      (!(p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED))) {
    BNEP_TRACE_ERROR("BNEP - filter response in bad state %d",
                     p_bcb->con_state);
    return;
  }

  /* Check if we are the originator */
  if (!(p_bcb->con_flags & BNEP_FLAGS_FILTER_RESP_PEND)) {
    BNEP_TRACE_ERROR("BNEP - filter response when not expecting");
    return;
  }

  /* Ensure timer is stopped */
  alarm_cancel(p_bcb->conn_timer);
  p_bcb->con_flags &= ~BNEP_FLAGS_FILTER_RESP_PEND;
  p_bcb->re_transmits = 0;

  BE_STREAM_TO_UINT16(resp_code, p_data);

  result = BNEP_SUCCESS;
  if (resp_code != BNEP_FILTER_CRL_OK) result = BNEP_SET_FILTER_FAIL;

  if (bnep_cb.p_filter_ind_cb)
    (*bnep_cb.p_filter_ind_cb)(p_bcb->handle, false, result, 0, NULL);
}

/*******************************************************************************
 *
 * Function         bnepu_process_multicast_filter_rsp
 *
 * Description      This function processes multicast filter control
 *                  'response' message.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_process_multicast_filter_rsp(tBNEP_CONN* p_bcb, uint8_t* p_data) {
  uint16_t resp_code;
  tBNEP_RESULT result;

  BNEP_TRACE_DEBUG("BNEP received multicast filter responce");
  /* The state should be  CONNECTED */
  if ((p_bcb->con_state != BNEP_STATE_CONNECTED) &&
      (!(p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED))) {
    BNEP_TRACE_ERROR("BNEP - multicast filter response in bad state %d",
                     p_bcb->con_state);
    return;
  }

  /* Check if we are the originator */
  if (!(p_bcb->con_flags & BNEP_FLAGS_MULTI_RESP_PEND)) {
    BNEP_TRACE_ERROR("BNEP - multicast filter response when not expecting");
    return;
  }

  /* Ensure timer is stopped */
  alarm_cancel(p_bcb->conn_timer);
  p_bcb->con_flags &= ~BNEP_FLAGS_MULTI_RESP_PEND;
  p_bcb->re_transmits = 0;

  BE_STREAM_TO_UINT16(resp_code, p_data);

  result = BNEP_SUCCESS;
  if (resp_code != BNEP_FILTER_CRL_OK) result = BNEP_SET_FILTER_FAIL;

  if (bnep_cb.p_mfilter_ind_cb)
    (*bnep_cb.p_mfilter_ind_cb)(p_bcb->handle, false, result, 0, NULL);
}

/*******************************************************************************
 *
 * Function         bnepu_process_peer_multicast_filter_set
 *
 * Description      This function processes a peer's filter control
 *                  'set' message. The filters are stored in the BCB,
 *                  and an appropriate filter response message sent.
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_process_peer_multicast_filter_set(tBNEP_CONN* p_bcb,
                                             uint8_t* p_filters, uint16_t len) {
  uint16_t resp_code = BNEP_FILTER_CRL_OK;
  uint16_t num_filters, xx;
  uint8_t *p_temp_filters, null_bda[BD_ADDR_LEN] = {0, 0, 0, 0, 0, 0};

  if ((p_bcb->con_state != BNEP_STATE_CONNECTED) &&
      (!(p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED))) {
    BNEP_TRACE_DEBUG(
        "BNEP received multicast filter set from peer when there is no "
        "connection");
    return;
  }

  if (len % 12) {
    BNEP_TRACE_EVENT("BNEP - bad filter len: %d", len);
    bnepu_send_peer_multicast_filter_rsp(p_bcb, BNEP_FILTER_CRL_BAD_RANGE);
    return;
  }

  if (len > (BNEP_MAX_MULTI_FILTERS * 2 * BD_ADDR_LEN)) {
    BNEP_TRACE_EVENT("BNEP - Too many filters");
    bnepu_send_peer_multicast_filter_rsp(p_bcb, BNEP_FILTER_CRL_MAX_REACHED);
    return;
  }

  num_filters = 0;
  if (len) num_filters = (uint16_t)(len / 12);

  /* Validate filter values */
  if (num_filters <= BNEP_MAX_MULTI_FILTERS) {
    p_temp_filters = p_filters;
    for (xx = 0; xx < num_filters; xx++) {
      if (memcmp(p_temp_filters, p_temp_filters + BD_ADDR_LEN, BD_ADDR_LEN) >
          0) {
        bnepu_send_peer_multicast_filter_rsp(p_bcb, BNEP_FILTER_CRL_BAD_RANGE);
        return;
      }

      p_temp_filters += (BD_ADDR_LEN * 2);
    }
  }

  p_bcb->rcvd_mcast_filters = num_filters;
  for (xx = 0; xx < num_filters; xx++) {
    memcpy(p_bcb->rcvd_mcast_filter_start[xx].address, p_filters, BD_ADDR_LEN);
    memcpy(p_bcb->rcvd_mcast_filter_end[xx].address, p_filters + BD_ADDR_LEN,
           BD_ADDR_LEN);
    p_filters += (BD_ADDR_LEN * 2);

    /* Check if any of the ranges have all zeros as both starting and ending
     * addresses */
    if ((memcmp(null_bda, p_bcb->rcvd_mcast_filter_start[xx].address,
                BD_ADDR_LEN) == 0) &&
        (memcmp(null_bda, p_bcb->rcvd_mcast_filter_end[xx].address,
                BD_ADDR_LEN) == 0)) {
      p_bcb->rcvd_mcast_filters = 0xFFFF;
      break;
    }
  }

  BNEP_TRACE_EVENT("BNEP multicast filters %d", p_bcb->rcvd_mcast_filters);
  bnepu_send_peer_multicast_filter_rsp(p_bcb, resp_code);

  if (bnep_cb.p_mfilter_ind_cb)
    (*bnep_cb.p_mfilter_ind_cb)(p_bcb->handle, true, 0, len, p_filters);
}

/*******************************************************************************
 *
 * Function         bnepu_send_peer_multicast_filter_rsp
 *
 * Description      This function sends a filter response to a peer
 *
 * Returns          void
 *
 ******************************************************************************/
void bnepu_send_peer_multicast_filter_rsp(tBNEP_CONN* p_bcb,
                                          uint16_t response_code) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(BNEP_BUF_SIZE);
  uint8_t* p;

  BNEP_TRACE_DEBUG("BNEP sending multicast filter response %d", response_code);

  p_buf->offset = L2CAP_MIN_OFFSET;
  p = (uint8_t*)(p_buf + 1) + L2CAP_MIN_OFFSET;

  /* Put in BNEP frame type - filter control */
  UINT8_TO_BE_STREAM(p, BNEP_FRAME_CONTROL);

  /* Put in filter message type - set filters */
  UINT8_TO_BE_STREAM(p, BNEP_FILTER_MULTI_ADDR_RESPONSE_MSG);

  UINT16_TO_BE_STREAM(p, response_code);

  p_buf->len = 4;

  bnepu_check_send_packet(p_bcb, p_buf);
}

/*******************************************************************************
 *
 * Function         bnep_sec_check_complete
 *
 * Description      This function is registered with BTM and will be called
 *                  after completing the security procedures
 *
 * Returns          void
 *
 ******************************************************************************/
void bnep_sec_check_complete(UNUSED_ATTR const RawAddress* bd_addr,
                             UNUSED_ATTR tBT_TRANSPORT trasnport,
                             void* p_ref_data, uint8_t result) {
  tBNEP_CONN* p_bcb = (tBNEP_CONN*)p_ref_data;
  uint16_t resp_code = BNEP_SETUP_CONN_OK;
  bool is_role_change;

  BNEP_TRACE_EVENT("BNEP security callback returned result %d", result);
  if (p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED)
    is_role_change = true;
  else
    is_role_change = false;

  /* check if the port is still waiting for security to complete */
  if (p_bcb->con_state != BNEP_STATE_SEC_CHECKING) {
    BNEP_TRACE_ERROR(
        "BNEP Connection in wrong state %d when security is completed",
        p_bcb->con_state);
    return;
  }

  /* if it is outgoing call and result is FAILURE return security fail error */
  if (!(p_bcb->con_flags & BNEP_FLAGS_SETUP_RCVD)) {
    if (result != BTM_SUCCESS) {
      if (p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED) {
        /* Tell the user that role change is failed because of security */
        if (bnep_cb.p_conn_state_cb)
          (*bnep_cb.p_conn_state_cb)(p_bcb->handle, p_bcb->rem_bda,
                                     BNEP_SECURITY_FAIL, is_role_change);

        p_bcb->con_state = BNEP_STATE_CONNECTED;
        p_bcb->src_uuid = p_bcb->prv_src_uuid;
        p_bcb->dst_uuid = p_bcb->prv_dst_uuid;
        return;
      }

      L2CA_DisconnectReq(p_bcb->l2cap_cid);

      /* Tell the user if he has a callback */
      if (bnep_cb.p_conn_state_cb)
        (*bnep_cb.p_conn_state_cb)(p_bcb->handle, p_bcb->rem_bda,
                                   BNEP_SECURITY_FAIL, is_role_change);

      bnepu_release_bcb(p_bcb);
      return;
    }

    /* Transition to the next appropriate state, waiting for connection confirm.
     */
    p_bcb->con_state = BNEP_STATE_CONN_SETUP;

    bnep_send_conn_req(p_bcb);
    alarm_set_on_mloop(p_bcb->conn_timer, BNEP_CONN_TIMEOUT_MS,
                       bnep_conn_timer_timeout, p_bcb);
    return;
  }

  /* it is an incoming call respond appropriately */
  if (result != BTM_SUCCESS) {
    bnep_send_conn_responce(p_bcb, BNEP_SETUP_CONN_NOT_ALLOWED);
    if (p_bcb->con_flags & BNEP_FLAGS_CONN_COMPLETED) {
      /* Role change is failed because of security. Revert back to connected
       * state */
      p_bcb->con_state = BNEP_STATE_CONNECTED;
      p_bcb->con_flags &= (~BNEP_FLAGS_SETUP_RCVD);
      p_bcb->src_uuid = p_bcb->prv_src_uuid;
      p_bcb->dst_uuid = p_bcb->prv_dst_uuid;
      return;
    }

    L2CA_DisconnectReq(p_bcb->l2cap_cid);

    bnepu_release_bcb(p_bcb);
    return;
  }

  if (bnep_cb.p_conn_ind_cb) {
    p_bcb->con_state = BNEP_STATE_CONN_SETUP;
    (*bnep_cb.p_conn_ind_cb)(p_bcb->handle, p_bcb->rem_bda, p_bcb->dst_uuid,
                             p_bcb->src_uuid, is_role_change);
  } else {
    /* Profile didn't register connection indication call back */
    bnep_send_conn_responce(p_bcb, resp_code);
    bnep_connected(p_bcb);
  }

  return;
}

/*******************************************************************************
 *
 * Function         bnep_is_packet_allowed
 *
 * Description      This function verifies whether the protocol passes through
 *                  the protocol filters set by the peer
 *
 * Returns          BNEP_SUCCESS          - if the protocol is allowed
 *                  BNEP_IGNORE_CMD       - if the protocol is filtered out
 *
 ******************************************************************************/
tBNEP_RESULT bnep_is_packet_allowed(tBNEP_CONN* p_bcb,
                                    const RawAddress& p_dest_addr,
                                    uint16_t protocol, bool fw_ext_present,
                                    uint8_t* p_data, uint16_t org_len) {
  if (p_bcb->rcvd_num_filters) {
    uint16_t i, proto;

    /* Findout the actual protocol to check for the filtering */
    proto = protocol;
    if (proto == BNEP_802_1_P_PROTOCOL) {
      uint16_t new_len = 0;
      if (fw_ext_present) {
        uint8_t len, ext;
        /* parse the extension headers and findout actual protocol */
        do {
          if ((new_len + 2) > org_len) {
            return BNEP_IGNORE_CMD;
          }

          ext = *p_data++;
          len = *p_data++;
          p_data += len;

          new_len += (len + 2);

        } while (ext & 0x80);
      }
      if ((new_len + 4) > org_len) {
        return BNEP_IGNORE_CMD;
      }
      p_data += 2;
      BE_STREAM_TO_UINT16(proto, p_data);
    }

    for (i = 0; i < p_bcb->rcvd_num_filters; i++) {
      if ((p_bcb->rcvd_prot_filter_start[i] <= proto) &&
          (proto <= p_bcb->rcvd_prot_filter_end[i]))
        break;
    }

    if (i == p_bcb->rcvd_num_filters) {
      BNEP_TRACE_DEBUG("Ignoring protocol 0x%x in BNEP data write", proto);
      return BNEP_IGNORE_CMD;
    }
  }

  /* Ckeck for multicast address filtering */
  if ((p_dest_addr.address[0] & 0x01) && p_bcb->rcvd_mcast_filters) {
    uint16_t i;

    /* Check if every multicast should be filtered */
    if (p_bcb->rcvd_mcast_filters != 0xFFFF) {
      /* Check if the address is mentioned in the filter range */
      for (i = 0; i < p_bcb->rcvd_mcast_filters; i++) {
        if ((memcmp(p_bcb->rcvd_mcast_filter_start[i].address,
                    p_dest_addr.address, BD_ADDR_LEN) <= 0) &&
            (memcmp(p_bcb->rcvd_mcast_filter_end[i].address,
                    p_dest_addr.address, BD_ADDR_LEN) >= 0))
          break;
      }
    }

    /*
    ** If every multicast should be filtered or the address is not in the filter
    *range
    ** drop the packet
    */
    if ((p_bcb->rcvd_mcast_filters == 0xFFFF) ||
        (i == p_bcb->rcvd_mcast_filters)) {
      VLOG(1) << "Ignoring multicast address " << p_dest_addr
              << " in BNEP data write";
      return BNEP_IGNORE_CMD;
    }
  }

  return BNEP_SUCCESS;
}
