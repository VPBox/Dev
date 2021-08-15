/******************************************************************************
 *
 *  Copyright 2009-2012 Broadcom Corporation
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
#include <stdio.h>
#include <stdlib.h>

#include "bt_utils.h"
#include "bta_api.h"
#include "bta_dm_ci.h"
#include "bta_dm_co.h"
#include "bta_sys.h"
#include "bte_appl.h"
#include "btif_dm.h"
#include "btif_storage.h"
#include "osi/include/osi.h"

// tBTE_APPL_CFG.ble_io_cap is set to BTM_IO_CAP_UNKNOWN at structure
// initialization since btif_storage isn't ready yet for data to be fetched.
// This value is initialized properly during first use by fetching properly
// from btif_storage.
tBTE_APPL_CFG bte_appl_cfg = {
    BTA_LE_AUTH_REQ_SC_MITM_BOND,  // Authentication requirements
    BTM_IO_CAP_UNKNOWN, BTM_BLE_INITIATOR_KEY_SIZE, BTM_BLE_RESPONDER_KEY_SIZE,
    BTM_BLE_MAX_KEY_SIZE};

/*******************************************************************************
 *
 * Function         bta_dm_co_get_compress_memory
 *
 * Description      This callout function is executed by DM to get memory for
 compression

 * Parameters       id  -  BTA SYS ID
 *                  memory_p - memory return by callout
 *                  memory_size - memory size
 *
 * Returns          true for success, false for fail.
 *
 ******************************************************************************/
bool bta_dm_co_get_compress_memory(UNUSED_ATTR tBTA_SYS_ID id,
                                   UNUSED_ATTR uint8_t** memory_p,
                                   UNUSED_ATTR uint32_t* memory_size) {
  return true;
}

/*******************************************************************************
 *
 * Function         bta_dm_co_io_req
 *
 * Description      This callout function is executed by DM to get IO
 *                  capabilities of the local device for the Simple Pairing
 *                  process.
 *
 * Parameters       bd_addr  - The peer device
 *                  *p_io_cap - The local Input/Output capabilities
 *                  *p_oob_data - true, if OOB data is available for the peer
 *                                device.
 *                  *p_auth_req - true, if MITM protection is required.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_io_req(UNUSED_ATTR const RawAddress& bd_addr,
                      tBTA_IO_CAP* p_io_cap, tBTA_OOB_DATA* p_oob_data,
                      tBTA_AUTH_REQ* p_auth_req, bool is_orig) {
  btif_dm_set_oob_for_io_req(p_oob_data);
  btif_dm_proc_io_req(bd_addr, p_io_cap, p_oob_data, p_auth_req, is_orig);
  BTIF_TRACE_DEBUG("bta_dm_co_io_req *p_oob_data = %d", *p_oob_data);
  BTIF_TRACE_DEBUG("bta_dm_co_io_req *p_io_cap = %d", *p_io_cap);
  BTIF_TRACE_DEBUG("bta_dm_co_io_req *p_auth_req = %d", *p_auth_req);
  BTIF_TRACE_DEBUG("bta_dm_co_io_req is_orig = %d", is_orig);
}

/*******************************************************************************
 *
 * Function         bta_dm_co_io_rsp
 *
 * Description      This callout function is executed by DM to report IO
 *                  capabilities of the peer device for the Simple Pairing
 *                  process.
 *
 * Parameters       bd_addr  - The peer device
 *                  io_cap - The remote Input/Output capabilities
 *                  oob_data - true, if OOB data is available for the peer
 *                             device.
 *                  auth_req - true, if MITM protection is required.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_io_rsp(const RawAddress& bd_addr, tBTA_IO_CAP io_cap,
                      tBTA_OOB_DATA oob_data, tBTA_AUTH_REQ auth_req) {
  btif_dm_proc_io_rsp(bd_addr, io_cap, oob_data, auth_req);
}

/*******************************************************************************
 *
 * Function         bta_dm_co_lk_upgrade
 *
 * Description      This callout function is executed by DM to check if the
 *                  platform wants allow link key upgrade
 *
 * Parameters       bd_addr  - The peer device
 *                  *p_upgrade - true, if link key upgrade is desired.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_lk_upgrade(UNUSED_ATTR const RawAddress& bd_addr,
                          UNUSED_ATTR bool* p_upgrade) {}

/*******************************************************************************
 *
 * Function         bta_dm_co_loc_oob
 *
 * Description      This callout function is executed by DM to report the OOB
 *                  data of the local device for the Simple Pairing process
 *
 * Parameters       valid - true, if the local OOB data is retrieved from LM
 *                  c     - Simple Pairing Hash C
 *                  r     - Simple Pairing Randomnizer R
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_loc_oob(bool valid, const Octet16& c, const Octet16& r) {
  BTIF_TRACE_DEBUG("bta_dm_co_loc_oob, valid = %d", valid);
#ifdef BTIF_DM_OOB_TEST
  btif_dm_proc_loc_oob(valid, c, r);
#endif
}

/*******************************************************************************
 *
 * Function         bta_dm_co_rmt_oob
 *
 * Description      This callout function is executed by DM to request the OOB
 *                  data for the remote device for the Simple Pairing process
 *                  Need to call bta_dm_ci_rmt_oob() in response
 *
 * Parameters       bd_addr  - The peer device
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_rmt_oob(const RawAddress& bd_addr) {
  Octet16 c;
  Octet16 r;
  bool result = false;

#ifdef BTIF_DM_OOB_TEST
  result = btif_dm_proc_rmt_oob(bd_addr, &c, &r);
#endif

  BTIF_TRACE_DEBUG("bta_dm_co_rmt_oob: result=%d", result);
  bta_dm_ci_rmt_oob(result, bd_addr, c, r);
}

/*******************************************************************************
 *
 * Function         bta_dm_co_le_io_key_req
 *
 * Description      This callout function is executed by DM to get BLE key
 *                  information
 *                  before SMP pairing gets going.
 *
 * Parameters       bd_addr  - The peer device
 *                  *p_max_key_size - max key size local device supported.
 *                  *p_init_key - initiator keys.
 *                  *p_resp_key - responder keys.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_le_io_key_req(UNUSED_ATTR const RawAddress& bd_addr,
                             uint8_t* p_max_key_size,
                             tBTA_LE_KEY_TYPE* p_init_key,
                             tBTA_LE_KEY_TYPE* p_resp_key) {
  BTIF_TRACE_ERROR("##################################");
  BTIF_TRACE_ERROR("bta_dm_co_le_io_key_req: only setting max size to 16");
  BTIF_TRACE_ERROR("##################################");
  *p_max_key_size = 16;
  *p_init_key = *p_resp_key =
      (BTA_LE_KEY_PENC | BTA_LE_KEY_PID | BTA_LE_KEY_PCSRK | BTA_LE_KEY_LENC |
       BTA_LE_KEY_LID | BTA_LE_KEY_LCSRK);
}

/*******************************************************************************
 *
 * Function         bta_dm_co_ble_local_key_reload
 *
 * Description      This callout function is to load the local BLE keys if
 *                  available on the device.
 *
 * Parameters       none
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_ble_load_local_keys(tBTA_DM_BLE_LOCAL_KEY_MASK* p_key_mask,
                                   Octet16* p_er,
                                   tBTA_BLE_LOCAL_ID_KEYS* p_id_keys) {
  BTIF_TRACE_DEBUG("##################################");
  BTIF_TRACE_DEBUG(
      "bta_dm_co_ble_load_local_keys:  Load local keys if any are persisted");
  BTIF_TRACE_DEBUG("##################################");
  btif_dm_get_ble_local_keys(p_key_mask, p_er, p_id_keys);
}

/*******************************************************************************
 *
 * Function         bta_dm_co_ble_io_req
 *
 * Description      This callout function is executed by DM to get BLE IO
 *                  capabilities before SMP pairing gets going.
 *
 * Parameters       bd_addr  - The peer device
 *                  *p_io_cap - The local Input/Output capabilities
 *                  *p_oob_data - true, if OOB data is available for the peer
 *                                device.
 *                  *p_auth_req -  Auth request setting (Bonding and MITM
 *                                 required or not)
 *                  *p_max_key_size - max key size local device supported.
 *                  *p_init_key - initiator keys.
 *                  *p_resp_key - responder keys.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_dm_co_ble_io_req(const RawAddress& bd_addr, tBTA_IO_CAP* p_io_cap,
                          tBTA_OOB_DATA* p_oob_data,
                          tBTA_LE_AUTH_REQ* p_auth_req, uint8_t* p_max_key_size,
                          tBTA_LE_KEY_TYPE* p_init_key,
                          tBTA_LE_KEY_TYPE* p_resp_key) {
  bte_appl_cfg.ble_io_cap = btif_storage_get_local_io_caps_ble();

  /* Retrieve the properties from file system if possible */
  tBTE_APPL_CFG nv_config;
  if (btif_dm_get_smp_config(&nv_config)) bte_appl_cfg = nv_config;

  /* *p_auth_req by default is false for devices with NoInputNoOutput; true for
   * other devices. */

  if (bte_appl_cfg.ble_auth_req)
    *p_auth_req = bte_appl_cfg.ble_auth_req |
                  (bte_appl_cfg.ble_auth_req & 0x04) | ((*p_auth_req) & 0x04);

  /* if OOB is not supported, this call-out function does not need to do
   * anything
   * otherwise, look for the OOB data associated with the address and set
   * *p_oob_data accordingly.
   * If the answer can not be obtained right away,
   * set *p_oob_data to BTA_OOB_UNKNOWN and call bta_dm_ci_io_req() when the
   * answer is available.
   */

  btif_dm_set_oob_for_le_io_req(bd_addr, p_oob_data, p_auth_req);

  if (bte_appl_cfg.ble_io_cap <= 4) *p_io_cap = bte_appl_cfg.ble_io_cap;

  if (bte_appl_cfg.ble_init_key <= BTM_BLE_INITIATOR_KEY_SIZE)
    *p_init_key = bte_appl_cfg.ble_init_key;

  if (bte_appl_cfg.ble_resp_key <= BTM_BLE_RESPONDER_KEY_SIZE)
    *p_resp_key = bte_appl_cfg.ble_resp_key;

  if (bte_appl_cfg.ble_max_key_size > 7 && bte_appl_cfg.ble_max_key_size <= 16)
    *p_max_key_size = bte_appl_cfg.ble_max_key_size;
}
