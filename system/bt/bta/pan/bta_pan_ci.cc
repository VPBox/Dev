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

/******************************************************************************
 *
 *  This is the implementation file for data gateway call-in functions.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <string.h>

#include "bt_common.h"
#include "bt_utils.h"
#include "bta_api.h"
#include "bta_pan_api.h"
#include "bta_pan_ci.h"
#include "bta_pan_int.h"
#include "osi/include/osi.h"
#include "pan_api.h"

#if (BTA_PAN_INCLUDED == TRUE)

/*******************************************************************************
 *
 * Function         bta_pan_ci_tx_ready
 *
 * Description      This function sends an event to PAN indicating the phone is
 *                  ready for more data and PAN should call
 *                  bta_pan_co_tx_path().
 *                  This function is used when the TX data path is configured
 *                  to use a pull interface.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_tx_ready(uint16_t handle) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->layer_specific = handle;
  p_buf->event = BTA_PAN_CI_TX_READY_EVT;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_rx_ready
 *
 * Description      This function sends an event to PAN indicating the phone
 *                  has data available to send to PAN and PAN should call
 *                  bta_pan_co_rx_path().  This function is used when the RX
 *                  data path is configured to use a pull interface.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_rx_ready(uint16_t handle) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->layer_specific = handle;
  p_buf->event = BTA_PAN_CI_RX_READY_EVT;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_tx_flow
 *
 * Description      This function is called to enable or disable data flow on
 *                  the TX path.  The phone should call this function to
 *                  disable data flow when it is congested and cannot handle
 *                  any more data sent by bta_pan_co_tx_write() or
 *                  bta_pan_co_tx_writebuf().  This function is used when the
 *                  TX data path is configured to use a push interface.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_tx_flow(uint16_t handle, bool enable) {
  tBTA_PAN_CI_TX_FLOW* p_buf =
      (tBTA_PAN_CI_TX_FLOW*)osi_malloc(sizeof(tBTA_PAN_CI_TX_FLOW));

  p_buf->hdr.layer_specific = handle;
  p_buf->hdr.event = BTA_PAN_CI_TX_FLOW_EVT;
  p_buf->enable = enable;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_rx_write
 *
 * Description      This function is called to send data to PAN when the RX path
 *                  is configured to use a push interface.  The function copies
 *                  data to an event buffer and sends it to PAN.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_rx_write(uint16_t handle, const RawAddress& dst,
                         const RawAddress& src, uint16_t protocol,
                         uint8_t* p_data, uint16_t len, bool ext) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(PAN_BUF_SIZE);

  p_buf->offset = PAN_MINIMUM_OFFSET;

  /* copy all other params before the data */
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->src = src;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->dst = dst;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->protocol = protocol;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->ext = ext;
  p_buf->len = len;

  /* copy data */
  memcpy((uint8_t*)(p_buf + 1) + p_buf->offset, p_data, len);

  p_buf->layer_specific = handle;
  p_buf->event = BTA_PAN_CI_RX_WRITEBUF_EVT;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_rx_writebuf
 *
 * Description      This function is called to send data to the phone when
 *                  the RX path is configured to use a push interface with
 *                  zero copy.  The function sends an event to PAN containing
 *                  the data buffer. The buffer will be freed by BTA; the
 *                  phone must not free the buffer.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_rx_writebuf(uint16_t handle, const RawAddress& dst,
                            const RawAddress& src, uint16_t protocol,
                            BT_HDR* p_buf, bool ext) {
  /* copy all other params before the data */
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->src = src;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->dst = dst;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->protocol = protocol;
  ((tBTA_PAN_DATA_PARAMS*)p_buf)->ext = ext;

  p_buf->layer_specific = handle;
  p_buf->event = BTA_PAN_CI_RX_WRITEBUF_EVT;
  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_readbuf
 *
 * Description
 *
 *
 * Returns          void
 *
 ******************************************************************************/
BT_HDR* bta_pan_ci_readbuf(uint16_t handle, RawAddress& src, RawAddress& dst,
                           uint16_t* p_protocol, bool* p_ext, bool* p_forward) {
  tBTA_PAN_SCB* p_scb = bta_pan_scb_by_handle(handle);
  BT_HDR* p_buf;

  if (p_scb == NULL) return NULL;

  p_buf = (BT_HDR*)fixed_queue_try_dequeue(p_scb->data_queue);
  if (p_buf != NULL) {
    src = ((tBTA_PAN_DATA_PARAMS*)p_buf)->src;
    dst = ((tBTA_PAN_DATA_PARAMS*)p_buf)->dst;
    *p_protocol = ((tBTA_PAN_DATA_PARAMS*)p_buf)->protocol;
    *p_ext = ((tBTA_PAN_DATA_PARAMS*)p_buf)->ext;
    *p_forward = ((tBTA_PAN_DATA_PARAMS*)p_buf)->forward;
  }

  return p_buf;
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_set_mfilters
 *
 * Description      This function is called to set multicast filters
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_set_mfilters(uint16_t handle, uint16_t num_mcast_filters,
                             uint8_t* p_start_array, uint8_t* p_end_array) {
  PAN_SetMulticastFilters(handle, num_mcast_filters, p_start_array,
                          p_end_array);
}

/*******************************************************************************
 *
 * Function         bta_pan_ci_set_mfilters
 *
 * Description      This function is called to set protocol filters
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_pan_ci_set_pfilters(uint16_t handle, uint16_t num_filters,
                             uint16_t* p_start_array, uint16_t* p_end_array) {
  PAN_SetProtocolFilters(handle, num_filters, p_start_array, p_end_array);
}
#else

void bta_pan_ci_tx_ready(UNUSED_ATTR uint16_t handle) {}

void bta_pan_ci_rx_ready(UNUSED_ATTR uint16_t handle) {}

void bta_pan_ci_tx_flow(UNUSED_ATTR uint16_t handle, UNUSED_ATTR bool enable) {}

void bta_pan_ci_rx_writebuf(UNUSED_ATTR uint16_t handle,
                            UNUSED_ATTR const RawAddress& src,
                            UNUSED_ATTR const RawAddress& dst,
                            UNUSED_ATTR uint16_t protocol,
                            UNUSED_ATTR BT_HDR* p_buf, UNUSED_ATTR bool ext) {}

BT_HDR* bta_pan_ci_readbuf(UNUSED_ATTR uint16_t handle,
                           UNUSED_ATTR RawAddress& src,
                           UNUSED_ATTR RawAddress& dst,
                           UNUSED_ATTR uint16_t* p_protocol,
                           UNUSED_ATTR bool* p_ext,
                           UNUSED_ATTR bool* p_forward) {
  return NULL;
}

void bta_pan_ci_set_pfilters(UNUSED_ATTR uint16_t handle,
                             UNUSED_ATTR uint16_t num_filters,
                             UNUSED_ATTR uint16_t* p_start_array,
                             UNUSED_ATTR uint16_t* p_end_array) {}

void bta_pan_ci_set_mfilters(UNUSED_ATTR uint16_t handle,
                             UNUSED_ATTR uint16_t num_mcast_filters,
                             UNUSED_ATTR uint8_t* p_start_array,
                             UNUSED_ATTR uint8_t* p_end_array) {}

#endif /* BTA_PAN_API */
