/******************************************************************************
 *
 *  Copyright 2014  Broadcom Corporation
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

#define LOG_TAG "bt_btm_ble"

#include "bt_target.h"

#include "bt_types.h"
#include "bt_utils.h"
#include "btm_ble_api.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "hcidefs.h"
#include "hcimsgs.h"

#include <string.h>
#include <algorithm>
#include <vector>

#include <base/bind.h>
#include <base/bind_helpers.h>

using base::Bind;
using bluetooth::Uuid;

#define BTM_BLE_ADV_FILT_META_HDR_LENGTH 3
#define BTM_BLE_ADV_FILT_FEAT_SELN_LEN 13
#define BTM_BLE_ADV_FILT_TRACK_NUM 2

#define BTM_BLE_PF_SELECT_NONE 0

/* BLE meta vsc header: 1 bytes of sub_code, 1 byte of PCF action */
#define BTM_BLE_META_HDR_LENGTH 3
#define BTM_BLE_PF_FEAT_SEL_LEN 18
#define BTM_BLE_PCF_ENABLE_LEN 2

#define BTM_BLE_META_ADDR_LEN 7
#define BTM_BLE_META_UUID_LEN 40

#define BTM_BLE_PF_BIT_TO_MASK(x) (uint16_t)(1 << (x))

tBTM_BLE_ADV_FILTER_CB btm_ble_adv_filt_cb;
tBTM_BLE_VSC_CB cmn_ble_vsc_cb;

static uint8_t btm_ble_cs_update_pf_counter(tBTM_BLE_SCAN_COND_OP action,
                                            uint8_t cond_type,
                                            tBLE_BD_ADDR* p_bd_addr,
                                            uint8_t num_available);

#define BTM_BLE_SET_SCAN_PF_OPCODE(x, y) (((x) << 4) | (y))
#define BTM_BLE_GET_SCAN_PF_SUBCODE(x) ((x) >> 4)
#define BTM_BLE_GET_SCAN_PF_ACTION(x) ((x)&0x0f)
#define BTM_BLE_INVALID_COUNTER 0xff

/* length of each multi adv sub command */
#define BTM_BLE_ADV_FILTER_ENB_LEN 3

/* length of each batch scan command */
#define BTM_BLE_ADV_FILTER_CLEAR_LEN 3
#define BTM_BLE_ADV_FILTER_LEN 2

#define BTM_BLE_ADV_FILT_CB_EVT_MASK 0xF0
#define BTM_BLE_ADV_FILT_SUBCODE_MASK 0x0F

bool is_filtering_supported() {
  return cmn_ble_vsc_cb.filter_support != 0 && cmn_ble_vsc_cb.max_filter != 0;
}

/*******************************************************************************
 *
 * Function         btm_ble_condtype_to_ocf
 *
 * Description      Convert cond_type to OCF
 *
 * Returns          Returns ocf value
 *
 ******************************************************************************/
uint8_t btm_ble_condtype_to_ocf(uint8_t cond_type) {
  uint8_t ocf = 0;

  switch (cond_type) {
    case BTM_BLE_PF_ADDR_FILTER:
      ocf = BTM_BLE_META_PF_ADDR;
      break;
    case BTM_BLE_PF_SRVC_UUID:
      ocf = BTM_BLE_META_PF_UUID;
      break;
    case BTM_BLE_PF_SRVC_SOL_UUID:
      ocf = BTM_BLE_META_PF_SOL_UUID;
      break;
    case BTM_BLE_PF_LOCAL_NAME:
      ocf = BTM_BLE_META_PF_LOCAL_NAME;
      break;
    case BTM_BLE_PF_MANU_DATA:
      ocf = BTM_BLE_META_PF_MANU_DATA;
      break;
    case BTM_BLE_PF_SRVC_DATA_PATTERN:
      ocf = BTM_BLE_META_PF_SRVC_DATA;
      break;
    case BTM_BLE_PF_TYPE_ALL:
      ocf = BTM_BLE_META_PF_ALL;
      break;
    default:
      ocf = BTM_BLE_PF_TYPE_MAX;
      break;
  }
  return ocf;
}

/*******************************************************************************
 *
 * Function         btm_ble_ocf_to_condtype
 *
 * Description      Convert OCF to cond type
 *
 * Returns          Returns condtype value
 *
 ******************************************************************************/
uint8_t btm_ble_ocf_to_condtype(uint8_t ocf) {
  uint8_t cond_type = 0;

  switch (ocf) {
    case BTM_BLE_META_PF_FEAT_SEL:
      cond_type = BTM_BLE_META_PF_FEAT_SEL;
      break;
    case BTM_BLE_META_PF_ADDR:
      cond_type = BTM_BLE_PF_ADDR_FILTER;
      break;
    case BTM_BLE_META_PF_UUID:
      cond_type = BTM_BLE_PF_SRVC_UUID;
      break;
    case BTM_BLE_META_PF_SOL_UUID:
      cond_type = BTM_BLE_PF_SRVC_SOL_UUID;
      break;
    case BTM_BLE_META_PF_LOCAL_NAME:
      cond_type = BTM_BLE_PF_LOCAL_NAME;
      break;
    case BTM_BLE_META_PF_MANU_DATA:
      cond_type = BTM_BLE_PF_MANU_DATA;
      break;
    case BTM_BLE_META_PF_SRVC_DATA:
      cond_type = BTM_BLE_PF_SRVC_DATA_PATTERN;
      break;
    case BTM_BLE_META_PF_ALL:
      cond_type = BTM_BLE_PF_TYPE_ALL;
      break;
    default:
      cond_type = BTM_BLE_PF_TYPE_MAX;
      break;
  }
  return cond_type;
}

void btm_flt_update_cb(uint8_t expected_ocf, tBTM_BLE_PF_CFG_CBACK cb,
                       uint8_t* p, uint16_t evt_len) {
  if (evt_len != 4) {
    BTM_TRACE_ERROR("%s: bad length: %d", __func__, evt_len);
    return;
  }

  uint8_t status, op_subcode, action, num_avail;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(op_subcode, p);
  STREAM_TO_UINT8(action, p);
  STREAM_TO_UINT8(num_avail, p);

  if (expected_ocf != op_subcode) {
    BTM_TRACE_ERROR("%s: Incorrect opcode: 0x%02x, expected: 0x%02x", __func__,
                    expected_ocf, op_subcode);
    return;
  }

  if (op_subcode == BTM_BLE_META_PF_FEAT_SEL) {
    cb.Run(num_avail, action, status);
    return;
  }

  uint8_t cond_type = btm_ble_ocf_to_condtype(expected_ocf);
  BTM_TRACE_DEBUG("%s: Recd: %d, %d, %d, %d, %d", __func__, op_subcode,
                  expected_ocf, action, status, num_avail);
  if (HCI_SUCCESS == status) {
    if (btm_ble_adv_filt_cb.cur_filter_target.bda.IsEmpty())
      btm_ble_cs_update_pf_counter(action, cond_type, NULL, num_avail);
    else
      btm_ble_cs_update_pf_counter(
          action, cond_type, &btm_ble_adv_filt_cb.cur_filter_target, num_avail);
  }

  /* send ADV PF operation complete */
  btm_ble_adv_filt_cb.op_type = 0;

  cb.Run(num_avail, action, status);
}

/*******************************************************************************
 *
 * Function         btm_ble_find_addr_filter_counter
 *
 * Description      find the per bd address ADV payload filter counter by
 *                  BD_ADDR.
 *
 * Returns          pointer to the counter if found; NULL otherwise.
 *
 ******************************************************************************/
tBTM_BLE_PF_COUNT* btm_ble_find_addr_filter_counter(tBLE_BD_ADDR* p_le_bda) {
  uint8_t i;
  tBTM_BLE_PF_COUNT* p_addr_filter =
      &btm_ble_adv_filt_cb.p_addr_filter_count[1];

  if (p_le_bda == NULL) return &btm_ble_adv_filt_cb.p_addr_filter_count[0];

  for (i = 0; i < cmn_ble_vsc_cb.max_filter; i++, p_addr_filter++) {
    if (p_addr_filter->in_use && p_le_bda->bda == p_addr_filter->bd_addr) {
      return p_addr_filter;
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         btm_ble_alloc_addr_filter_counter
 *
 * Description      allocate the per device adv payload filter counter.
 *
 * Returns          pointer to the counter if allocation succeed; NULL
 *                  otherwise.
 *
 ******************************************************************************/
tBTM_BLE_PF_COUNT* btm_ble_alloc_addr_filter_counter(
    const RawAddress& bd_addr) {
  uint8_t i;
  tBTM_BLE_PF_COUNT* p_addr_filter =
      &btm_ble_adv_filt_cb.p_addr_filter_count[1];

  for (i = 0; i < cmn_ble_vsc_cb.max_filter; i++, p_addr_filter++) {
    if (p_addr_filter->bd_addr.IsEmpty()) {
      p_addr_filter->bd_addr = bd_addr;
      p_addr_filter->in_use = true;
      return p_addr_filter;
    }
  }
  return NULL;
}
/*******************************************************************************
 *
 * Function         btm_ble_dealloc_addr_filter_counter
 *
 * Description      de-allocate the per device adv payload filter counter.
 *
 * Returns          true if deallocation succeed; false otherwise.
 *
 ******************************************************************************/
bool btm_ble_dealloc_addr_filter_counter(tBLE_BD_ADDR* p_bd_addr,
                                         uint8_t filter_type) {
  uint8_t i;
  tBTM_BLE_PF_COUNT* p_addr_filter =
      &btm_ble_adv_filt_cb.p_addr_filter_count[1];
  bool found = false;

  if (BTM_BLE_PF_TYPE_ALL == filter_type && NULL == p_bd_addr)
    memset(&btm_ble_adv_filt_cb.p_addr_filter_count[0], 0,
           sizeof(tBTM_BLE_PF_COUNT));

  for (i = 0; i < cmn_ble_vsc_cb.max_filter; i++, p_addr_filter++) {
    if (p_addr_filter->in_use &&
        (!p_bd_addr || p_bd_addr->bda == p_addr_filter->bd_addr)) {
      found = true;
      memset(p_addr_filter, 0, sizeof(tBTM_BLE_PF_COUNT));

      if (p_bd_addr) break;
    }
  }
  return found;
}

/**
 * This function update(add,delete or clear) the adv local name filtering
 * condition.
 */
void BTM_LE_PF_local_name(tBTM_BLE_SCAN_COND_OP action,
                          tBTM_BLE_PF_FILT_INDEX filt_index,
                          std::vector<uint8_t> name, tBTM_BLE_PF_CFG_CBACK cb) {
  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH;

  uint8_t len_max = len + BTM_BLE_PF_STR_LEN_MAX;
  uint8_t param[len_max];
  memset(param, 0, len_max);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_LOCAL_NAME);
  UINT8_TO_STREAM(p, action);
  UINT8_TO_STREAM(p, filt_index);

  if (action != BTM_BLE_SCAN_COND_CLEAR) {
    int size = std::min(name.size(), (size_t)BTM_BLE_PF_STR_LEN_MAX);
    ARRAY_TO_STREAM(p, name.data(), size);
    len += size;
  }

  /* send local name filter */
  btu_hcif_send_cmd_with_cb(
      FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
      base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_LOCAL_NAME, cb));

  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

/**
 * this function update(add/remove) service data change filter.
 */
void BTM_LE_PF_srvc_data(tBTM_BLE_SCAN_COND_OP action,
                         tBTM_BLE_PF_FILT_INDEX filt_index) {
  uint8_t num_avail = (action == BTM_BLE_SCAN_COND_ADD) ? 0 : 1;

  btm_ble_cs_update_pf_counter(action, BTM_BLE_PF_SRVC_DATA, nullptr,
                               num_avail);
}

/**
 * This function update(add,delete or clear) the adv manufacturer data filtering
 * condition.
 */
void BTM_LE_PF_manu_data(tBTM_BLE_SCAN_COND_OP action,
                         tBTM_BLE_PF_FILT_INDEX filt_index, uint16_t company_id,
                         uint16_t company_id_mask, std::vector<uint8_t> data,
                         std::vector<uint8_t> data_mask,
                         tBTM_BLE_PF_CFG_CBACK cb) {
  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH;
  int len_max = len + BTM_BLE_PF_STR_LEN_MAX + BTM_BLE_PF_STR_LEN_MAX;

  uint8_t param[len_max];
  memset(param, 0, len_max);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_MANU_DATA);
  UINT8_TO_STREAM(p, action);
  UINT8_TO_STREAM(p, filt_index);

  if (action != BTM_BLE_SCAN_COND_CLEAR) {
    uint8_t size = std::min(data.size(), (size_t)(BTM_BLE_PF_STR_LEN_MAX - 2));

    UINT16_TO_STREAM(p, company_id);
    if (size > 0 && data_mask.size() != 0) {
      ARRAY_TO_STREAM(p, data.data(), size);
      len += size + 2;
    } else
      len += 2;

    if (company_id_mask != 0) {
      UINT16_TO_STREAM(p, company_id_mask);
    } else {
      UINT16_TO_STREAM(p, (uint16_t)0xFFFF);
    }
    len += 2;

    if (size > 0 && data_mask.size() != 0) {
      ARRAY_TO_STREAM(p, data_mask.data(), size);
      len += (size);
    }

    BTM_TRACE_DEBUG("Manuf data length: %d", len);
  }

  btu_hcif_send_cmd_with_cb(
      FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
      base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_MANU_DATA, cb));

  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

/**
 * This function update(add,delete or clear) the service data filtering
 * condition.
 **/
void BTM_LE_PF_srvc_data_pattern(tBTM_BLE_SCAN_COND_OP action,
                                 tBTM_BLE_PF_FILT_INDEX filt_index,
                                 std::vector<uint8_t> data,
                                 std::vector<uint8_t> data_mask,
                                 tBTM_BLE_PF_CFG_CBACK cb) {
  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH;
  int len_max = len + BTM_BLE_PF_STR_LEN_MAX + BTM_BLE_PF_STR_LEN_MAX;

  uint8_t param[len_max];
  memset(param, 0, len_max);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_SRVC_DATA);
  UINT8_TO_STREAM(p, action);
  UINT8_TO_STREAM(p, filt_index);

  if (action != BTM_BLE_SCAN_COND_CLEAR) {
    uint8_t size = std::min(data.size(), (size_t)(BTM_BLE_PF_STR_LEN_MAX - 2));

    if (size > 0) {
      ARRAY_TO_STREAM(p, data.data(), size);
      len += size;
      ARRAY_TO_STREAM(p, data_mask.data(), size);
      len += size;
    }
  }

  btu_hcif_send_cmd_with_cb(
      FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
      base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_SRVC_DATA, cb));

  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

/*******************************************************************************
 *
 * Function         btm_ble_cs_update_pf_counter
 *
 * Description      this function is to update the adv data payload filter
 *                  counter
 *
 * Returns          current number of the counter; BTM_BLE_INVALID_COUNTER if
 *                  counter update failed.
 *
 ******************************************************************************/
uint8_t btm_ble_cs_update_pf_counter(tBTM_BLE_SCAN_COND_OP action,
                                     uint8_t cond_type, tBLE_BD_ADDR* p_bd_addr,
                                     uint8_t num_available) {
  tBTM_BLE_PF_COUNT* p_addr_filter = NULL;
  uint8_t* p_counter = NULL;

  if (cond_type > BTM_BLE_PF_TYPE_ALL) {
    BTM_TRACE_ERROR("unknown PF filter condition type %d", cond_type);
    return BTM_BLE_INVALID_COUNTER;
  }

  /* for these three types of filter, always generic */
  if (BTM_BLE_PF_ADDR_FILTER == cond_type ||
      BTM_BLE_PF_MANU_DATA == cond_type || BTM_BLE_PF_LOCAL_NAME == cond_type ||
      BTM_BLE_PF_SRVC_DATA_PATTERN == cond_type)
    p_bd_addr = NULL;

  if ((p_addr_filter = btm_ble_find_addr_filter_counter(p_bd_addr)) == NULL &&
      BTM_BLE_SCAN_COND_ADD == action) {
    p_addr_filter = btm_ble_alloc_addr_filter_counter(p_bd_addr->bda);
  }

  if (NULL != p_addr_filter) {
    /* all filter just cleared */
    if ((BTM_BLE_PF_TYPE_ALL == cond_type &&
         BTM_BLE_SCAN_COND_CLEAR == action) ||
        /* or bd address filter been deleted */
        (BTM_BLE_PF_ADDR_FILTER == cond_type &&
         (BTM_BLE_SCAN_COND_DELETE == action ||
          BTM_BLE_SCAN_COND_CLEAR == action))) {
      btm_ble_dealloc_addr_filter_counter(p_bd_addr, cond_type);
    }
    /* if not feature selection, update new addition/reduction of the filter
       counter */
    else if (cond_type != BTM_BLE_PF_TYPE_ALL) {
      p_counter = p_addr_filter->pf_counter;
      if (num_available > 0) p_counter[cond_type] += 1;

      BTM_TRACE_DEBUG("counter = %d, maxfilt = %d, num_avbl=%d",
                      p_counter[cond_type], cmn_ble_vsc_cb.max_filter,
                      num_available);
      return p_counter[cond_type];
    }
  } else {
    BTM_TRACE_ERROR("no matching filter counter found");
  }
  /* no matching filter located and updated */
  return BTM_BLE_INVALID_COUNTER;
}

/**
 * This function updates the address filter of adv.
 */
void BTM_LE_PF_addr_filter(tBTM_BLE_SCAN_COND_OP action,
                           tBTM_BLE_PF_FILT_INDEX filt_index, tBLE_BD_ADDR addr,
                           tBTM_BLE_PF_CFG_CBACK cb) {
  const uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH + BTM_BLE_META_ADDR_LEN;

  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_ADDR);
  UINT8_TO_STREAM(p, action);
  UINT8_TO_STREAM(p, filt_index);

  if (action != BTM_BLE_SCAN_COND_CLEAR) {
#if (BLE_PRIVACY_SPT == TRUE)
    if (addr.type == BLE_ADDR_PUBLIC_ID) {
      LOG(INFO) << __func__ << " Filter address " << addr.bda
                << " has type PUBLIC_ID, try to get identity address";
      /* If no matching identity address is found for the input address,
       * this call will have no effect. */
      btm_random_pseudo_to_identity_addr(&addr.bda, &addr.type);
    }
#endif

    LOG(INFO) << __func__
              << " Adding scan filter with peer address: " << addr.bda;

    BDADDR_TO_STREAM(p, addr.bda);
    UINT8_TO_STREAM(p, addr.type);
  }

  /* send address filter */
  btu_hcif_send_cmd_with_cb(
      FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
      base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_ADDR, cb));

  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

/**
 * This function updates(adds, deletes or clears) the service UUID filter.
 */
void BTM_LE_PF_uuid_filter(tBTM_BLE_SCAN_COND_OP action,
                           tBTM_BLE_PF_FILT_INDEX filt_index,
                           tBTM_BLE_PF_COND_TYPE filter_type,
                           const bluetooth::Uuid& uuid,
                           tBTM_BLE_PF_LOGIC_TYPE cond_logic,
                           const bluetooth::Uuid& uuid_mask,
                           tBTM_BLE_PF_CFG_CBACK cb) {
  uint8_t evt_type;

  if (BTM_BLE_PF_SRVC_UUID == filter_type) {
    evt_type = BTM_BLE_META_PF_UUID;
  } else {
    evt_type = BTM_BLE_META_PF_SOL_UUID;
  }

  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH;
  uint8_t max_len = len + BTM_BLE_META_UUID_LEN;
  uint8_t param[max_len];
  memset(param, 0, max_len);
  uint8_t* p = param;

  UINT8_TO_STREAM(p, evt_type);
  UINT8_TO_STREAM(p, action);
  UINT8_TO_STREAM(p, filt_index);

  uint8_t uuid_len = uuid.GetShortestRepresentationSize();
  if (action != BTM_BLE_SCAN_COND_CLEAR) {
    if (uuid_len == Uuid::kNumBytes16) {
      UINT16_TO_STREAM(p, uuid.As16Bit());
      len += Uuid::kNumBytes16;
    } else if (uuid_len == Uuid::kNumBytes32) {
      UINT32_TO_STREAM(p, uuid.As32Bit());
      len += Uuid::kNumBytes32;
    } else if (uuid_len == Uuid::kNumBytes128) {
      const auto& tmp = uuid.To128BitLE();
      ARRAY_TO_STREAM(p, tmp.data(), (int)Uuid::kNumBytes128);
      len += Uuid::kNumBytes128;
    } else {
      BTM_TRACE_ERROR("illegal UUID length: %d", uuid_len);
      cb.Run(0, BTM_BLE_PF_CONFIG, 1 /*BTA_FAILURE*/);
      return;
    }

    if (!uuid_mask.IsEmpty()) {
      if (uuid_len == Uuid::kNumBytes16) {
        UINT16_TO_STREAM(p, uuid_mask.As16Bit());
        len += Uuid::kNumBytes16;
      } else if (uuid_len == Uuid::kNumBytes32) {
        UINT32_TO_STREAM(p, uuid_mask.As32Bit());
        len += Uuid::kNumBytes32;
      } else if (uuid_len == Uuid::kNumBytes128) {
        const auto& tmp = uuid_mask.To128BitLE();
        ARRAY_TO_STREAM(p, tmp.data(), (int)Uuid::kNumBytes128);
        len += Uuid::kNumBytes128;
      }
    } else {
      memset(p, 0xff, uuid_len);
      len += uuid_len;
    }
  }

  /* send UUID filter update */
  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
                            base::Bind(&btm_flt_update_cb, evt_type, cb));
  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

void BTM_LE_PF_set(tBTM_BLE_PF_FILT_INDEX filt_index,
                   std::vector<ApcfCommand> commands,
                   tBTM_BLE_PF_CFG_CBACK cb) {
  if (!is_filtering_supported()) {
    cb.Run(0, BTM_BLE_PF_ENABLE, 1 /* BTA_FAILURE */);
    return;
  }

  int action = BTM_BLE_SCAN_COND_ADD;
  for (const ApcfCommand& cmd : commands) {
    /* If data is passed, both mask and data have to be the same length */
    if (cmd.data.size() != cmd.data_mask.size() && cmd.data.size() != 0 &&
        cmd.data_mask.size() != 0) {
      LOG(ERROR) << __func__ << " data(" << cmd.data.size() << ") and mask("
                 << cmd.data_mask.size() << ") are of different size";
      continue;
    }

    switch (cmd.type) {
      case BTM_BLE_PF_ADDR_FILTER: {
        tBLE_BD_ADDR target_addr;
        target_addr.bda = cmd.address;
        target_addr.type = cmd.addr_type;

        BTM_LE_PF_addr_filter(action, filt_index, target_addr,
                              base::DoNothing());
        break;
      }

      case BTM_BLE_PF_SRVC_DATA:
        BTM_LE_PF_srvc_data(action, filt_index);
        break;

      case BTM_BLE_PF_SRVC_UUID:
      case BTM_BLE_PF_SRVC_SOL_UUID: {
        BTM_LE_PF_uuid_filter(action, filt_index, cmd.type, cmd.uuid,
                              BTM_BLE_PF_LOGIC_AND, cmd.uuid_mask,
                              base::DoNothing());
        break;
      }

      case BTM_BLE_PF_LOCAL_NAME: {
        BTM_LE_PF_local_name(action, filt_index, cmd.name, base::DoNothing());
        break;
      }

      case BTM_BLE_PF_MANU_DATA: {
        BTM_LE_PF_manu_data(action, filt_index, cmd.company, cmd.company_mask,
                            cmd.data, cmd.data_mask, base::DoNothing());
        break;
      }

      case BTM_BLE_PF_SRVC_DATA_PATTERN: {
        BTM_LE_PF_srvc_data_pattern(action, filt_index, cmd.data, cmd.data_mask,
                                    base::DoNothing());
        break;
      }

      default:
        LOG(ERROR) << __func__ << ": Unknown filter type: " << +cmd.type;
        break;
    }
  }
  cb.Run(0, 0, 0);
}

/**
 * all adv payload filter by de-selecting all the adv pf feature bits
 */
void BTM_LE_PF_clear(tBTM_BLE_PF_FILT_INDEX filt_index,
                     tBTM_BLE_PF_CFG_CBACK cb) {
  if (!is_filtering_supported()) {
    cb.Run(0, BTM_BLE_PF_ENABLE, 1 /* BTA_FAILURE */);
    return;
  }

  /* clear the general filter entry */
  {
    tBTM_BLE_PF_CFG_CBACK fDoNothing;

    /* clear manufactuer data filter */
    BTM_LE_PF_manu_data(BTM_BLE_SCAN_COND_CLEAR, filt_index, 0, 0, {}, {},
                        fDoNothing);

    /* clear local name filter */
    BTM_LE_PF_local_name(BTM_BLE_SCAN_COND_CLEAR, filt_index, {}, fDoNothing);

    /* update the counter for service data */
    BTM_LE_PF_srvc_data(BTM_BLE_SCAN_COND_CLEAR, filt_index);

    /* clear UUID filter */
    BTM_LE_PF_uuid_filter(BTM_BLE_SCAN_COND_CLEAR, filt_index,
                          BTM_BLE_PF_SRVC_UUID, {}, 0, Uuid::kEmpty,
                          fDoNothing);

    BTM_LE_PF_uuid_filter(BTM_BLE_SCAN_COND_CLEAR, filt_index,
                          BTM_BLE_PF_SRVC_SOL_UUID, {}, 0, Uuid::kEmpty,
                          fDoNothing);

    /* clear service data filter */
    BTM_LE_PF_srvc_data_pattern(BTM_BLE_SCAN_COND_CLEAR, filt_index, {}, {},
                                fDoNothing);
  }

  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH + BTM_BLE_PF_FEAT_SEL_LEN;
  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* p = param;

  /* select feature based on control block settings */
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_FEAT_SEL);
  UINT8_TO_STREAM(p, BTM_BLE_SCAN_COND_CLEAR);
  UINT8_TO_STREAM(p, filt_index);
  /* set PCF selection */
  UINT32_TO_STREAM(p, BTM_BLE_PF_SELECT_NONE);
  /* set logic condition as OR as default */
  UINT8_TO_STREAM(p, BTM_BLE_PF_LOGIC_OR);

  btu_hcif_send_cmd_with_cb(
      FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
      base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_FEAT_SEL, cb));

  memset(&btm_ble_adv_filt_cb.cur_filter_target, 0, sizeof(tBLE_BD_ADDR));
}

/*******************************************************************************
 *
 * Function         BTM_BleAdvFilterParamSetup
 *
 * Description      This function is called to setup the adv data payload filter
 *                  condition.
 *
 * Parameters       action - Type of action to be performed
 *                       filt_index - Filter index
 *                       p_filt_params - Filter parameters
 *                       cb - Callback
 *
 ******************************************************************************/
void BTM_BleAdvFilterParamSetup(
    int action, tBTM_BLE_PF_FILT_INDEX filt_index,
    std::unique_ptr<btgatt_filt_param_setup_t> p_filt_params,
    tBTM_BLE_PF_PARAM_CB cb) {
  tBTM_BLE_PF_COUNT* p_bda_filter = NULL;
  uint8_t len = BTM_BLE_ADV_FILT_META_HDR_LENGTH +
                BTM_BLE_ADV_FILT_FEAT_SELN_LEN + BTM_BLE_ADV_FILT_TRACK_NUM;
  uint8_t param[len], *p;

  if (!is_filtering_supported()) {
    cb.Run(0, BTM_BLE_PF_ENABLE, 1 /* BTA_FAILURE */);
    return;
  }

  p = param;
  memset(param, 0, len);
  BTM_TRACE_EVENT("%s", __func__);

  if (BTM_BLE_SCAN_COND_ADD == action) {
    p_bda_filter = btm_ble_find_addr_filter_counter(nullptr);
    if (NULL == p_bda_filter) {
      BTM_TRACE_ERROR("BD Address not found!");
      cb.Run(0, BTM_BLE_PF_ENABLE, 1 /* BTA_FAILURE */);
      return;
    }

    BTM_TRACE_DEBUG("%s : Feat mask:%d", __func__, p_filt_params->feat_seln);
    /* select feature based on control block settings */
    UINT8_TO_STREAM(p, BTM_BLE_META_PF_FEAT_SEL);
    UINT8_TO_STREAM(p, BTM_BLE_SCAN_COND_ADD);

    /* Filter index */
    UINT8_TO_STREAM(p, filt_index);

    /* set PCF selection */
    UINT16_TO_STREAM(p, p_filt_params->feat_seln);
    /* set logic type */
    UINT16_TO_STREAM(p, p_filt_params->list_logic_type);
    /* set logic condition */
    UINT8_TO_STREAM(p, p_filt_params->filt_logic_type);
    /* set RSSI high threshold */
    UINT8_TO_STREAM(p, p_filt_params->rssi_high_thres);
    /* set delivery mode */
    UINT8_TO_STREAM(p, p_filt_params->dely_mode);

    if (0x01 == p_filt_params->dely_mode) {
      /* set onfound timeout */
      UINT16_TO_STREAM(p, p_filt_params->found_timeout);
      /* set onfound timeout count*/
      UINT8_TO_STREAM(p, p_filt_params->found_timeout_cnt);
      /* set RSSI low threshold */
      UINT8_TO_STREAM(p, p_filt_params->rssi_low_thres);
      /* set onlost timeout */
      UINT16_TO_STREAM(p, p_filt_params->lost_timeout);
      /* set num_of_track_entries for firmware greater than L-release version */
      if (cmn_ble_vsc_cb.version_supported > BTM_VSC_CHIP_CAPABILITY_L_VERSION)
        UINT16_TO_STREAM(p, p_filt_params->num_of_tracking_entries);
    }

    if (cmn_ble_vsc_cb.version_supported == BTM_VSC_CHIP_CAPABILITY_L_VERSION)
      len = BTM_BLE_ADV_FILT_META_HDR_LENGTH + BTM_BLE_ADV_FILT_FEAT_SELN_LEN;
    else
      len = BTM_BLE_ADV_FILT_META_HDR_LENGTH + BTM_BLE_ADV_FILT_FEAT_SELN_LEN +
            BTM_BLE_ADV_FILT_TRACK_NUM;

    btu_hcif_send_cmd_with_cb(
        FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param, len,
        base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_FEAT_SEL, cb));
  } else if (BTM_BLE_SCAN_COND_DELETE == action) {
    /* select feature based on control block settings */
    UINT8_TO_STREAM(p, BTM_BLE_META_PF_FEAT_SEL);
    UINT8_TO_STREAM(p, BTM_BLE_SCAN_COND_DELETE);
    /* Filter index */
    UINT8_TO_STREAM(p, filt_index);

    btu_hcif_send_cmd_with_cb(
        FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param,
        (uint8_t)(BTM_BLE_ADV_FILT_META_HDR_LENGTH),
        base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_FEAT_SEL, cb));
  } else if (BTM_BLE_SCAN_COND_CLEAR == action) {
    /* Deallocate all filters here */
    btm_ble_dealloc_addr_filter_counter(NULL, BTM_BLE_PF_TYPE_ALL);

    /* select feature based on control block settings */
    UINT8_TO_STREAM(p, BTM_BLE_META_PF_FEAT_SEL);
    UINT8_TO_STREAM(p, BTM_BLE_SCAN_COND_CLEAR);

    btu_hcif_send_cmd_with_cb(
        FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param,
        (uint8_t)(BTM_BLE_ADV_FILT_META_HDR_LENGTH - 1),
        base::Bind(&btm_flt_update_cb, BTM_BLE_META_PF_FEAT_SEL, cb));
  }
}

void enable_cmpl_cback(tBTM_BLE_PF_STATUS_CBACK p_stat_cback, uint8_t* p,
                       uint16_t evt_len) {
  uint8_t status, op_subcode, action;

  if (evt_len != 3) {
    BTM_TRACE_ERROR("%s: APCF callback length = %d", __func__, evt_len);
    return;
  }

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(op_subcode, p);
  STREAM_TO_UINT8(action, p);

  if (op_subcode != BTM_BLE_META_PF_ENABLE) {
    BTM_TRACE_ERROR("%s :bad subcode: 0x%02x", __func__, op_subcode);
    return;
  }

  p_stat_cback.Run(action, status);
}

/*******************************************************************************
 *
 * Function         BTM_BleEnableDisableFilterFeature
 *
 * Description      This function is called to enable / disable the APCF feature
 *
 * Parameters       enable: enable or disable the filter condition
 *                  p_stat_cback - Status callback pointer
 *
 ******************************************************************************/
void BTM_BleEnableDisableFilterFeature(uint8_t enable,
                                       tBTM_BLE_PF_STATUS_CBACK p_stat_cback) {
  if (!is_filtering_supported()) {
    if (p_stat_cback) p_stat_cback.Run(BTM_BLE_PF_ENABLE, 1 /* BTA_FAILURE */);
    return;
  }

  uint8_t param[20];
  memset(param, 0, 20);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_META_PF_ENABLE);
  UINT8_TO_STREAM(p, enable);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_ADV_FILTER_OCF, param,
                            BTM_BLE_PCF_ENABLE_LEN,
                            base::Bind(&enable_cmpl_cback, p_stat_cback));
}

/*******************************************************************************
 *
 * Function         btm_ble_adv_filter_init
 *
 * Description      This function initializes the adv filter control block
 *
 * Parameters
 *
 * Returns          status
 *
 ******************************************************************************/
void btm_ble_adv_filter_init(void) {
  memset(&btm_ble_adv_filt_cb, 0, sizeof(tBTM_BLE_ADV_FILTER_CB));

  BTM_BleGetVendorCapabilities(&cmn_ble_vsc_cb);

  if (!is_filtering_supported()) return;

  if (cmn_ble_vsc_cb.max_filter > 0) {
    btm_ble_adv_filt_cb.p_addr_filter_count = (tBTM_BLE_PF_COUNT*)osi_malloc(
        sizeof(tBTM_BLE_PF_COUNT) * cmn_ble_vsc_cb.max_filter);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_adv_filter_cleanup
 *
 * Description      This function de-initializes the adv filter control block
 *
 * Parameters
 *
 * Returns          status
 *
 ******************************************************************************/
void btm_ble_adv_filter_cleanup(void) {
  osi_free_and_reset((void**)&btm_ble_adv_filt_cb.p_addr_filter_count);
}
