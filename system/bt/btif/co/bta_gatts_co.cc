/******************************************************************************
 *
 *  Copyright 2009-2013 Broadcom Corporation
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

#include "bta_api.h"

#include <stdlib.h>
#include <string.h>
#include "bt_common.h"
#include "bta_gatts_co.h"
#include "btif_util.h"
#include "osi/include/osi.h"

/*****************************************************************************
 *  Local type definitions
 ****************************************************************************/

#define BTIF_GATTS_MAX_SRV_CHG_CLT_SIZE 50

typedef struct {
  bool enable;
  uint8_t num_clients;
  tGATTS_SRV_CHG srv_chg[BTIF_GATTS_MAX_SRV_CHG_CLT_SIZE];
} __attribute__((packed)) btif_gatts_srv_chg_cb_t;

/*****************************************************************************
 *  Static variables
 ****************************************************************************/

static btif_gatts_srv_chg_cb_t btif_gatts_srv_chg_cb;

/*****************************************************************************
 *  Static functions
 ****************************************************************************/

static void btif_gatts_check_init(void) {
  btif_gatts_srv_chg_cb_t* p_cb = &btif_gatts_srv_chg_cb;

  if (!p_cb->enable) {
    memset(p_cb, 0, sizeof(btif_gatts_srv_chg_cb_t));
    p_cb->enable = true;
  }
}

/*****************************************************************************
 *  Externally called functions
 ****************************************************************************/

void btif_gatts_add_bonded_dev_from_nv(const RawAddress& bda) {
  btif_gatts_srv_chg_cb_t* p_cb = &btif_gatts_srv_chg_cb;
  bool found = false;
  uint8_t i;

  btif_gatts_check_init();

  for (i = 0; i != p_cb->num_clients; ++i) {
    if (p_cb->srv_chg[i].bda == bda) {
      found = true;
      break;
    }
  }

  if (!found) {
    if (p_cb->num_clients < BTIF_GATTS_MAX_SRV_CHG_CLT_SIZE) {
      p_cb->srv_chg[p_cb->num_clients].bda = bda;
      p_cb->srv_chg[p_cb->num_clients].srv_changed = false;
      p_cb->num_clients++;
    }
  }
}

/*****************************************************************************
 *  Call-out functions
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         bta_gatts_co_update_handle_range
 *
 * Description      This callout function is executed by GATTS when a GATT
 *                  server handle range ios to be added or removed.
 *
 * Parameter        is_add: true is to add a handle range; otherwise is to
 *                          delete.
 *                  p_hndl_range: handle range.
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_gatts_co_update_handle_range(
    UNUSED_ATTR bool is_add, UNUSED_ATTR tBTA_GATTS_HNDL_RANGE* p_hndl_range) {}

/*******************************************************************************
 *
 * Function         bta_gatts_co_srv_chg
 *
 * Description      This call-out is to read/write/remove service change related
 *                  informaiton. The request consists of the cmd and p_req and
 *                  the response is returned in p_rsp
 *
 * Parameter        cmd - request command
 *                  p_req - request paramters
 *                  p_rsp - response data for the request
 *
 * Returns          true - if the request is processed successfully and
 *                         the response is returned in p_rsp.
 *                  false - if the request can not be processed
 *
 ******************************************************************************/
bool bta_gatts_co_srv_chg(UNUSED_ATTR tGATTS_SRV_CHG_CMD cmd,
                          UNUSED_ATTR tGATTS_SRV_CHG_REQ* p_req,
                          UNUSED_ATTR tGATTS_SRV_CHG_RSP* p_rsp) {
  return false;
}

/*******************************************************************************
 *
 * Function         bta_gatts_co_load_handle_range
 *
 * Description      This callout function is executed by GATTS when a GATT
 *                  server handle range is requested to be loaded from NV.
 *
 * Parameter
 *
 * Returns          void.
 *
 ******************************************************************************/
bool bta_gatts_co_load_handle_range(
    UNUSED_ATTR uint8_t index,
    UNUSED_ATTR tBTA_GATTS_HNDL_RANGE* p_handle_range) {
  return false;
}
