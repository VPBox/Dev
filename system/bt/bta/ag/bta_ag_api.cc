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
 *  This is the implementation of the API for the audio gateway (AG)
 *  subsystem of BTA, Broadcom's Bluetooth application layer for mobile
 *  phones.
 *
 ******************************************************************************/

#include "bta_ag_api.h"
#include <base/bind.h>
#include <cstring>
#include "bt_common.h"
#include "bta_ag_int.h"
#include "bta_api.h"
#include "bta_sys.h"
#include "stack/include/btu.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/

static const tBTA_SYS_REG bta_ag_reg = {bta_ag_hdl_event, BTA_AgDisable};
const tBTA_AG_RES_DATA tBTA_AG_RES_DATA::kEmpty = {};

/*******************************************************************************
 *
 * Function         BTA_AgEnable
 *
 * Description      Enable the audio gateway service. When the enable
 *                  operation is complete the callback function will be
 *                  called with a BTA_AG_ENABLE_EVT. This function must
 *                  be called before other function in the AG API are
 *                  called.
 *
 * Returns          BTA_SUCCESS if OK, BTA_FAILURE otherwise.
 *
 ******************************************************************************/
tBTA_STATUS BTA_AgEnable(tBTA_AG_CBACK* p_cback) {
  /* Error if AG is already enabled, or AG is in the middle of disabling. */
  for (const tBTA_AG_SCB& scb : bta_ag_cb.scb) {
    if (scb.in_use) {
      APPL_TRACE_ERROR("BTA_AgEnable: FAILED, AG already enabled.");
      return BTA_FAILURE;
    }
  }
  bta_sys_register(BTA_ID_AG, &bta_ag_reg);
  do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_api_enable, p_cback));
  return BTA_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_AgDisable
 *
 * Description      Disable the audio gateway service
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgDisable() {
  do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_api_disable));
}

/*******************************************************************************
 *
 * Function         BTA_AgRegister
 *
 * Description      Register an Audio Gateway service.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgRegister(tBTA_SERVICE_MASK services, tBTA_SEC sec_mask,
                    tBTA_AG_FEAT features,
                    const std::vector<std::string>& service_names,
                    uint8_t app_id) {
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_api_register, services, sec_mask, features,
                            service_names, app_id));
}

/*******************************************************************************
 *
 * Function         BTA_AgDeregister
 *
 * Description      Deregister an audio gateway service.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgDeregister(uint16_t handle) {
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle,
                            BTA_AG_API_DEREGISTER_EVT, tBTA_AG_DATA::kEmpty));
}

/*******************************************************************************
 *
 * Function         BTA_AgOpen
 *
 * Description      Opens a connection to a headset or hands-free device.
 *                  When connection is open callback function is called
 *                  with a BTA_AG_OPEN_EVT. Only the data connection is
 *                  opened. The audio connection is not opened.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgOpen(uint16_t handle, const RawAddress& bd_addr, tBTA_SEC sec_mask) {
  tBTA_AG_DATA data = {};
  data.api_open.bd_addr = bd_addr;
  data.api_open.sec_mask = sec_mask;
  do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle,
                                          BTA_AG_API_OPEN_EVT, data));
}

/*******************************************************************************
 *
 * Function         BTA_AgClose
 *
 * Description      Close the current connection to a headset or a handsfree
 *                  Any current audio connection will also be closed.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgClose(uint16_t handle) {
  do_in_main_thread(FROM_HERE,
                    base::Bind(&bta_ag_sm_execute_by_handle, handle,
                               BTA_AG_API_CLOSE_EVT, tBTA_AG_DATA::kEmpty));
}

/*******************************************************************************
 *
 * Function         BTA_AgAudioOpen
 *
 * Description      Opens an audio connection to the currently connected
 *                  headset or hnadsfree.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgAudioOpen(uint16_t handle) {
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle,
                            BTA_AG_API_AUDIO_OPEN_EVT, tBTA_AG_DATA::kEmpty));
}

/*******************************************************************************
 *
 * Function         BTA_AgAudioClose
 *
 * Description      Close the currently active audio connection to a headset
 *                  or hnadsfree. The data connection remains open
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgAudioClose(uint16_t handle) {
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle,
                            BTA_AG_API_AUDIO_CLOSE_EVT, tBTA_AG_DATA::kEmpty));
}

/*******************************************************************************
 *
 * Function         BTA_AgResult
 *
 * Description      Send an AT result code to a headset or hands-free device.
 *                  This function is only used when the AG parse mode is set
 *                  to BTA_AG_PARSE.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgResult(uint16_t handle, tBTA_AG_RES result,
                  const tBTA_AG_RES_DATA& data) {
  do_in_main_thread(FROM_HERE,
                    base::Bind(&bta_ag_api_result, handle, result, data));
}

/*******************************************************************************
 *
 * Function         BTA_AgSetCodec
 *
 * Description      Specify the codec type to be used for the subsequent
 *                  audio connection.
 *
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_AgSetCodec(uint16_t handle, tBTA_AG_PEER_CODEC codec) {
  tBTA_AG_DATA data = {};
  data.api_setcodec.codec = codec;
  do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, handle,
                                          BTA_AG_API_SETCODEC_EVT, data));
}

void BTA_AgSetScoAllowed(bool value) {
  do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_set_sco_allowed, value));
}

void BTA_AgSetActiveDevice(const RawAddress& active_device_addr) {
  do_in_main_thread(
      FROM_HERE, base::Bind(&bta_ag_api_set_active_device, active_device_addr));
}
