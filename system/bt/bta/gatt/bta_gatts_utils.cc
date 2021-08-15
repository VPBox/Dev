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
 *  This file contains the GATT client utility function.
 *
 ******************************************************************************/

#include "bt_target.h"

#include <string.h>

#include "bt_common.h"
#include "bta_gatts_int.h"
#include "bta_sys.h"
#include "utl.h"

/*******************************************************************************
 *
 * Function         bta_gatts_alloc_srvc_cb
 *
 * Description      allocate a service control block.
 *
 * Returns          pointer to the control block, or otherwise NULL when failed.
 *
 ******************************************************************************/
uint8_t bta_gatts_alloc_srvc_cb(tBTA_GATTS_CB* p_cb, uint8_t rcb_idx) {
  uint8_t i;

  for (i = 0; i < BTA_GATTS_MAX_SRVC_NUM; i++) {
    if (!p_cb->srvc_cb[i].in_use) {
      p_cb->srvc_cb[i].in_use = true;
      p_cb->srvc_cb[i].rcb_idx = rcb_idx;
      return i;
    }
  }
  return BTA_GATTS_INVALID_APP;
}

/*******************************************************************************
 *
 * Function         bta_gatts_find_app_rcb_by_app_if
 *
 * Description      find the index of the application control block by app ID.
 *
 * Returns          pointer to the control block if success, otherwise NULL
 *
 ******************************************************************************/
tBTA_GATTS_RCB* bta_gatts_find_app_rcb_by_app_if(tGATT_IF server_if) {
  uint8_t i;
  tBTA_GATTS_RCB* p_reg;

  for (i = 0, p_reg = bta_gatts_cb.rcb; i < BTA_GATTS_MAX_APP_NUM;
       i++, p_reg++) {
    if (p_reg->in_use && p_reg->gatt_if == server_if) return p_reg;
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_gatts_find_app_rcb_idx_by_app_if
 *
 * Description      find the index of the application control block by app ID.
 *
 * Returns          index of the control block, or BTA_GATTS_INVALID_APP if
 *                  failed.
 *
 ******************************************************************************/

uint8_t bta_gatts_find_app_rcb_idx_by_app_if(tBTA_GATTS_CB* p_cb,
                                             tGATT_IF server_if) {
  uint8_t i;

  for (i = 0; i < BTA_GATTS_MAX_APP_NUM; i++) {
    if (p_cb->rcb[i].in_use && p_cb->rcb[i].gatt_if == server_if) return i;
  }
  return BTA_GATTS_INVALID_APP;
}
/*******************************************************************************
 *
 * Function         bta_gatts_find_srvc_cb_by_srvc_id
 *
 * Description      find the service control block by service ID.
 *
 * Returns          pointer to the rcb.
 *
 ******************************************************************************/
tBTA_GATTS_SRVC_CB* bta_gatts_find_srvc_cb_by_srvc_id(tBTA_GATTS_CB* p_cb,
                                                      uint16_t service_id) {
  uint8_t i;
  VLOG(1) << __func__ << ": service_id=" << +service_id;
  for (i = 0; i < BTA_GATTS_MAX_SRVC_NUM; i++) {
    if (p_cb->srvc_cb[i].in_use && p_cb->srvc_cb[i].service_id == service_id) {
      VLOG(1) << __func__ << ": found service cb index=" << +i;
      return &p_cb->srvc_cb[i];
    }
  }
  return NULL;
}
/*******************************************************************************
 *
 * Function         bta_gatts_find_srvc_cb_by_attr_id
 *
 * Description      find the service control block by attribute ID.
 *
 * Returns          pointer to the rcb.
 *
 ******************************************************************************/
tBTA_GATTS_SRVC_CB* bta_gatts_find_srvc_cb_by_attr_id(tBTA_GATTS_CB* p_cb,
                                                      uint16_t attr_id) {
  uint8_t i;

  for (i = 0; i < (BTA_GATTS_MAX_SRVC_NUM); i++) {
    if (/* middle service */
        (i < (BTA_GATTS_MAX_SRVC_NUM - 1) && p_cb->srvc_cb[i].in_use &&
         p_cb->srvc_cb[i + 1].in_use &&
         attr_id >= p_cb->srvc_cb[i].service_id &&
         attr_id < p_cb->srvc_cb[i + 1].service_id) ||
        /* last active service */
        (i < (BTA_GATTS_MAX_SRVC_NUM - 1) && p_cb->srvc_cb[i].in_use &&
         !p_cb->srvc_cb[i + 1].in_use &&
         attr_id >= p_cb->srvc_cb[i].service_id) ||
        /* last service incb */
        (i == (BTA_GATTS_MAX_SRVC_NUM - 1) &&
         attr_id >= p_cb->srvc_cb[i].service_id)) {
      return &p_cb->srvc_cb[i];
    }
  }
  return NULL;
}
