/******************************************************************************
 *
 *  Copyright (c) 2014 The Android Open Source Project
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
 *  This is the implementation of the API for the handsfree (HF role)
 *  subsystem of BTA
 *
 ******************************************************************************/

#include <string.h>

#include "bta_hf_client_api.h"
#include "bta_hf_client_int.h"
#include "osi/include/compat.h"

/*****************************************************************************
 *  External Function Declarations
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         BTA_HfClientEnable
 *
 * Description      Enable the HF CLient service. It does the following:
 *                  1. Sets the state to initialized (control blocks)
 *                  2. Starts the SDP for the client role (HF)
 *                  3. Starts the RFCOMM server to accept incoming connections
 *                  The function is synchronous and returns with an error code
 *                  if anything went wrong. This should be the first call to the
 *                  API before doing an BTA_HfClientOpen
 *
 * Returns          BTA_SUCCESS if OK, BTA_FAILURE otherwise.
 *
 ******************************************************************************/
tBTA_STATUS BTA_HfClientEnable(tBTA_HF_CLIENT_CBACK* p_cback, tBTA_SEC sec_mask,
                               tBTA_HF_CLIENT_FEAT features,
                               const char* p_service_name) {
  return bta_hf_client_api_enable(p_cback, sec_mask, features, p_service_name);
}

/*******************************************************************************
 *
 * Function         BTA_HfClientDisable
 *
 * Description      Disable the HF Client service
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientDisable(void) { bta_hf_client_api_disable(); }

/*******************************************************************************
 *
 * Function         BTA_HfClientOpen
 *
 * Description      Opens up a RF connection to the remote device and
 *                  subsequently set it up for a HF SLC
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientOpen(const RawAddress& bd_addr, tBTA_SEC sec_mask,
                      uint16_t* p_handle) {
  APPL_TRACE_DEBUG("%s", __func__);
  tBTA_HF_CLIENT_API_OPEN* p_buf =
      (tBTA_HF_CLIENT_API_OPEN*)osi_malloc(sizeof(tBTA_HF_CLIENT_API_OPEN));

  if (!bta_hf_client_allocate_handle(bd_addr, p_handle)) {
    APPL_TRACE_ERROR("%s: could not allocate handle", __func__);
    return;
  }

  p_buf->hdr.event = BTA_HF_CLIENT_API_OPEN_EVT;
  p_buf->hdr.layer_specific = *p_handle;
  p_buf->bd_addr = bd_addr;
  p_buf->sec_mask = sec_mask;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_HfClientClose
 *
 * Description      Close the current connection to an audio gateway.
 *                  Any current audio connection will also be closed
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientClose(uint16_t handle) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_HF_CLIENT_API_CLOSE_EVT;
  p_buf->layer_specific = handle;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_HfCllientAudioOpen
 *
 * Description      Opens an audio connection to the currently connected
 *                 audio gateway
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientAudioOpen(uint16_t handle) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_HF_CLIENT_API_AUDIO_OPEN_EVT;
  p_buf->layer_specific = handle;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_HfClientAudioClose
 *
 * Description      Close the currently active audio connection to an audio
 *                  gateway. The data connection remains open
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientAudioClose(uint16_t handle) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR));

  p_buf->event = BTA_HF_CLIENT_API_AUDIO_CLOSE_EVT;
  p_buf->layer_specific = handle;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_HfClientSendAT
 *
 * Description      send AT command
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_HfClientSendAT(uint16_t handle, tBTA_HF_CLIENT_AT_CMD_TYPE at,
                        uint32_t val1, uint32_t val2, const char* str) {
  tBTA_HF_CLIENT_DATA_VAL* p_buf =
      (tBTA_HF_CLIENT_DATA_VAL*)osi_malloc(sizeof(tBTA_HF_CLIENT_DATA_VAL));

  p_buf->hdr.event = BTA_HF_CLIENT_SEND_AT_CMD_EVT;
  p_buf->uint8_val = at;
  p_buf->uint32_val1 = val1;
  p_buf->uint32_val2 = val2;

  if (str) {
    strlcpy(p_buf->str, str, BTA_HF_CLIENT_NUMBER_LEN + 1);
    p_buf->str[BTA_HF_CLIENT_NUMBER_LEN] = '\0';
  } else {
    p_buf->str[0] = '\0';
  }

  p_buf->hdr.layer_specific = handle;

  bta_sys_sendmsg(p_buf);
}

/*******************************************************************************
 *
 * Function         BTA_HfClientDumpStatistics
 *
 * Description      Dump statistics about the various control blocks
 *                  and other relevant connection statistics
 *
 * Returns          Void
 *
 ******************************************************************************/
void BTA_HfClientDumpStatistics(int fd) { bta_hf_client_dump_statistics(fd); }
