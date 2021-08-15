/******************************************************************************
 *
 *  Copyright 1999-2012 Broadcom Corporation
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
 *  This file contains functions for BLE controller based privacy.
 *
 ******************************************************************************/
#include <string.h>
#include "bt_target.h"

#if (BLE_PRIVACY_SPT == TRUE)
#include "ble_advertiser.h"
#include "bt_types.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "hcimsgs.h"
#include "vendor_hcidefs.h"

/* RPA offload VSC specifics */
#define BTM_BLE_META_IRK_ENABLE 0x01
#define BTM_BLE_META_ADD_IRK_ENTRY 0x02
#define BTM_BLE_META_REMOVE_IRK_ENTRY 0x03
#define BTM_BLE_META_CLEAR_IRK_LIST 0x04
#define BTM_BLE_META_READ_IRK_ENTRY 0x05
#define BTM_BLE_META_CS_RESOLVE_ADDR 0x00000001
#define BTM_BLE_IRK_ENABLE_LEN 2

#define BTM_BLE_META_ADD_IRK_LEN 24
#define BTM_BLE_META_REMOVE_IRK_LEN 8
#define BTM_BLE_META_CLEAR_IRK_LEN 1
#define BTM_BLE_META_READ_IRK_LEN 2
#define BTM_BLE_META_ADD_WL_ATTR_LEN 9

/*******************************************************************************
 *         Functions implemented controller based privacy using Resolving List
 ******************************************************************************/
/*******************************************************************************
 *
 * Function         btm_ble_enq_resolving_list_pending
 *
 * Description      add target address into resolving pending operation queue
 *
 * Parameters       target_bda: target device address
 *                  add_entry: true for add entry, false for remove entry
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_enq_resolving_list_pending(const RawAddress& pseudo_bda,
                                        uint8_t op_code) {
  tBTM_BLE_RESOLVE_Q* p_q = &btm_cb.ble_ctr_cb.resolving_list_pend_q;

  p_q->resolve_q_random_pseudo[p_q->q_next] = pseudo_bda;
  p_q->resolve_q_action[p_q->q_next] = op_code;
  p_q->q_next++;
  p_q->q_next %= controller_get_interface()->get_ble_resolving_list_max_size();
}

/*******************************************************************************
 *
 * Function         btm_ble_brcm_find_resolving_pending_entry
 *
 * Description      check to see if the action is in pending list
 *
 * Parameters       true: action pending;
 *                  false: new action
 *
 * Returns          void
 *
 ******************************************************************************/
bool btm_ble_brcm_find_resolving_pending_entry(const RawAddress& pseudo_addr,
                                               uint8_t action) {
  tBTM_BLE_RESOLVE_Q* p_q = &btm_cb.ble_ctr_cb.resolving_list_pend_q;

  for (uint8_t i = p_q->q_pending; i != p_q->q_next;) {
    if (p_q->resolve_q_random_pseudo[i] == pseudo_addr &&
        action == p_q->resolve_q_action[i])
      return true;

    i++;
    i %= controller_get_interface()->get_ble_resolving_list_max_size();
  }
  return false;
}

/*******************************************************************************
 *
 * Function         btm_ble_deq_resolving_pending
 *
 * Description      dequeue target address from resolving pending operation
 *                  queue
 *
 * Parameters       pseudo_addr: pseudo_addr device address
 *
 * Returns          void
 *
 ******************************************************************************/
bool btm_ble_deq_resolving_pending(RawAddress& pseudo_addr) {
  tBTM_BLE_RESOLVE_Q* p_q = &btm_cb.ble_ctr_cb.resolving_list_pend_q;

  if (p_q->q_next != p_q->q_pending) {
    pseudo_addr = p_q->resolve_q_random_pseudo[p_q->q_pending];
    p_q->resolve_q_random_pseudo[p_q->q_pending] = RawAddress::kEmpty;
    p_q->q_pending++;
    p_q->q_pending %=
        controller_get_interface()->get_ble_resolving_list_max_size();
    return true;
  }

  return false;
}

/*******************************************************************************
 *
 * Function         btm_ble_clear_irk_index
 *
 * Description      clear IRK list index mask for availability
 *
 * Returns          none
 *
 ******************************************************************************/
void btm_ble_clear_irk_index(uint8_t index) {
  uint8_t byte;
  uint8_t bit;

  if (index < controller_get_interface()->get_ble_resolving_list_max_size()) {
    byte = index / 8;
    bit = index % 8;
    btm_cb.ble_ctr_cb.irk_list_mask[byte] &= (~(1 << bit));
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_find_irk_index
 *
 * Description      find the first available IRK list index
 *
 * Returns          index from 0 ~ max (127 default)
 *
 ******************************************************************************/
uint8_t btm_ble_find_irk_index(void) {
  uint8_t i = 0;
  uint8_t byte;
  uint8_t bit;

  while (i < controller_get_interface()->get_ble_resolving_list_max_size()) {
    byte = i / 8;
    bit = i % 8;

    if ((btm_cb.ble_ctr_cb.irk_list_mask[byte] & (1 << bit)) == 0) {
      btm_cb.ble_ctr_cb.irk_list_mask[byte] |= (1 << bit);
      return i;
    }
    i++;
  }

  BTM_TRACE_ERROR("%s failed, list full", __func__);
  return i;
}

/*******************************************************************************
 *
 * Function         btm_ble_update_resolving_list
 *
 * Description      update resolving list entry in host maintained record
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_update_resolving_list(const RawAddress& pseudo_bda, bool add) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(pseudo_bda);
  if (p_dev_rec == NULL) return;

  if (add) {
    p_dev_rec->ble.in_controller_list |= BTM_RESOLVING_LIST_BIT;
    if (!controller_get_interface()->supports_ble_privacy())
      p_dev_rec->ble.resolving_list_index = btm_ble_find_irk_index();
  } else {
    p_dev_rec->ble.in_controller_list &= ~BTM_RESOLVING_LIST_BIT;
    if (!controller_get_interface()->supports_ble_privacy()) {
      /* clear IRK list index mask */
      btm_ble_clear_irk_index(p_dev_rec->ble.resolving_list_index);
      p_dev_rec->ble.resolving_list_index = 0;
    }
  }
}

bool clear_resolving_list_bit(void* data, void* context) {
  tBTM_SEC_DEV_REC* p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(data);
  p_dev_rec->ble.in_controller_list &= ~BTM_RESOLVING_LIST_BIT;
  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_clear_resolving_list_complete
 *
 * Description      This function is called when command complete for
 *                  clear resolving list
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_clear_resolving_list_complete(uint8_t* p, uint16_t evt_len) {
  uint8_t status = 0;
  STREAM_TO_UINT8(status, p);

  BTM_TRACE_DEBUG("%s status=%d", __func__, status);

  if (status == HCI_SUCCESS) {
    if (evt_len >= 3) {
      /* VSC complete has one extra byte for op code and list size, skip it here
       */
      p++;

      /* updated the available list size, and current list size */
      uint8_t irk_list_sz_max = 0;
      STREAM_TO_UINT8(irk_list_sz_max, p);

      if (controller_get_interface()->get_ble_resolving_list_max_size() == 0)
        btm_ble_resolving_list_init(irk_list_sz_max);

      uint8_t irk_mask_size = (irk_list_sz_max % 8) ? (irk_list_sz_max / 8 + 1)
                                                    : (irk_list_sz_max / 8);
      memset(btm_cb.ble_ctr_cb.irk_list_mask, 0, irk_mask_size);
    }

    btm_cb.ble_ctr_cb.resolving_list_avail_size =
        controller_get_interface()->get_ble_resolving_list_max_size();

    BTM_TRACE_DEBUG("%s resolving_list_avail_size=%d", __func__,
                    btm_cb.ble_ctr_cb.resolving_list_avail_size);

    list_foreach(btm_cb.sec_dev_rec, clear_resolving_list_bit, NULL);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_add_resolving_list_entry_complete
 *
 * Description      This function is called when command complete for
 *                  add resolving list entry
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_add_resolving_list_entry_complete(uint8_t* p, uint16_t evt_len) {
  uint8_t status;
  STREAM_TO_UINT8(status, p);

  BTM_TRACE_DEBUG("%s status = %d", __func__, status);

  RawAddress pseudo_bda;
  if (!btm_ble_deq_resolving_pending(pseudo_bda)) {
    BTM_TRACE_DEBUG("no pending resolving list operation");
    return;
  }

  if (status == HCI_SUCCESS) {
    /* privacy 1.2 command complete does not have these extra byte */
    if (evt_len > 2) {
      /* VSC complete has one extra byte for op code, skip it here */
      p++;
      STREAM_TO_UINT8(btm_cb.ble_ctr_cb.resolving_list_avail_size, p);
    } else
      btm_cb.ble_ctr_cb.resolving_list_avail_size--;
  } else if (status ==
             HCI_ERR_MEMORY_FULL) /* BT_ERROR_CODE_MEMORY_CAPACITY_EXCEEDED  */
  {
    btm_cb.ble_ctr_cb.resolving_list_avail_size = 0;
    BTM_TRACE_DEBUG("%s Resolving list Full ", __func__);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_remove_resolving_list_entry_complete
 *
 * Description      This function is called when command complete for
 *                  remove resolving list entry
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_remove_resolving_list_entry_complete(uint8_t* p,
                                                  uint16_t evt_len) {
  RawAddress pseudo_bda;
  uint8_t status;

  STREAM_TO_UINT8(status, p);

  BTM_TRACE_DEBUG("%s status = %d", __func__, status);

  if (!btm_ble_deq_resolving_pending(pseudo_bda)) {
    BTM_TRACE_ERROR("%s no pending resolving list operation", __func__);
    return;
  }

  if (status == HCI_SUCCESS) {
    /* proprietary: spec does not have these extra bytes */
    if (evt_len > 2) {
      p++; /* skip opcode */
      STREAM_TO_UINT8(btm_cb.ble_ctr_cb.resolving_list_avail_size, p);
    } else
      btm_cb.ble_ctr_cb.resolving_list_avail_size++;
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_read_resolving_list_entry_complete
 *
 * Description      This function is called when command complete for
 *                  remove resolving list entry
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_read_resolving_list_entry_complete(uint8_t* p, uint16_t evt_len) {
  uint8_t status, rra_type = BTM_BLE_ADDR_PSEUDO;
  RawAddress rra, pseudo_bda;

  STREAM_TO_UINT8(status, p);

  BTM_TRACE_DEBUG("%s status = %d", __func__, status);

  if (!btm_ble_deq_resolving_pending(pseudo_bda)) {
    BTM_TRACE_ERROR("no pending resolving list operation");
    return;
  }

  if (status == HCI_SUCCESS) {
    /* proprietary spec has extra bytes */
    if (evt_len > 8) {
      /* skip subcode, index, IRK value, address type, identity addr type */
      p += (2 + 16 + 1 + 6);
      STREAM_TO_BDADDR(rra, p);

      VLOG(2) << __func__ << " peer_addr: " << rra;
    } else {
      STREAM_TO_BDADDR(rra, p);
    }
    btm_ble_refresh_peer_resolvable_private_addr(pseudo_bda, rra, rra_type);
  }
}
/*******************************************************************************
                VSC that implement controller based privacy
 ******************************************************************************/
/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_vsc_op_cmpl
 *
 * Description      IRK operation VSC complete handler
 *
 * Parameters
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_resolving_list_vsc_op_cmpl(tBTM_VSC_CMPL* p_params) {
  uint8_t *p = p_params->p_param_buf, op_subcode;
  uint16_t evt_len = p_params->param_len;

  op_subcode = *(p + 1);

  BTM_TRACE_DEBUG("%s op_subcode = %d", __func__, op_subcode);

  if (op_subcode == BTM_BLE_META_CLEAR_IRK_LIST) {
    btm_ble_clear_resolving_list_complete(p, evt_len);
  } else if (op_subcode == BTM_BLE_META_ADD_IRK_ENTRY) {
    btm_ble_add_resolving_list_entry_complete(p, evt_len);
  } else if (op_subcode == BTM_BLE_META_REMOVE_IRK_ENTRY) {
    btm_ble_remove_resolving_list_entry_complete(p, evt_len);
  } else if (op_subcode == BTM_BLE_META_READ_IRK_ENTRY) {
    btm_ble_read_resolving_list_entry_complete(p, evt_len);
  } else if (op_subcode == BTM_BLE_META_IRK_ENABLE) {
    /* RPA offloading enable/disabled */
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_remove_resolving_list_entry
 *
 * Description      This function to remove an IRK entry from the list
 *
 * Parameters       ble_addr_type: address type
 *                  ble_addr: LE adddress
 *
 * Returns          status
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_remove_resolving_list_entry(tBTM_SEC_DEV_REC* p_dev_rec) {
  /* if controller does not support RPA offloading or privacy 1.2, skip */
  if (controller_get_interface()->get_ble_resolving_list_max_size() == 0)
    return BTM_WRONG_MODE;

  if (controller_get_interface()->supports_ble_privacy()) {
    btsnd_hcic_ble_rm_device_resolving_list(p_dev_rec->ble.identity_addr_type,
                                            p_dev_rec->ble.identity_addr);
  } else {
    uint8_t param[20] = {0};
    uint8_t* p = param;

    UINT8_TO_STREAM(p, BTM_BLE_META_REMOVE_IRK_ENTRY);
    UINT8_TO_STREAM(p, p_dev_rec->ble.identity_addr_type);
    BDADDR_TO_STREAM(p, p_dev_rec->ble.identity_addr);

    BTM_VendorSpecificCommand(HCI_VENDOR_BLE_RPA_VSC,
                              BTM_BLE_META_REMOVE_IRK_LEN, param,
                              btm_ble_resolving_list_vsc_op_cmpl);
  }

  btm_ble_enq_resolving_list_pending(p_dev_rec->bd_addr,
                                     BTM_BLE_META_REMOVE_IRK_ENTRY);
  return BTM_CMD_STARTED;
}

/*******************************************************************************
 *
 * Function         btm_ble_clear_resolving_list
 *
 * Description      This function clears the resolving  list
 *
 * Parameters       None.
 *
 ******************************************************************************/
void btm_ble_clear_resolving_list(void) {
  if (controller_get_interface()->supports_ble_privacy()) {
    btsnd_hcic_ble_clear_resolving_list();
  } else {
    uint8_t param[20] = {0};
    uint8_t* p = param;

    UINT8_TO_STREAM(p, BTM_BLE_META_CLEAR_IRK_LIST);
    BTM_VendorSpecificCommand(HCI_VENDOR_BLE_RPA_VSC,
                              BTM_BLE_META_CLEAR_IRK_LEN, param,
                              btm_ble_resolving_list_vsc_op_cmpl);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_read_resolving_list_entry
 *
 * Description      This function read an IRK entry by index
 *
 * Parameters       entry index.
 *
 * Returns          status
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_read_resolving_list_entry(tBTM_SEC_DEV_REC* p_dev_rec) {
  if (!(p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT))
    return BTM_WRONG_MODE;

  if (controller_get_interface()->supports_ble_privacy()) {
    btsnd_hcic_ble_read_resolvable_addr_peer(p_dev_rec->ble.identity_addr_type,
                                             p_dev_rec->ble.identity_addr);
  } else {
    uint8_t param[20] = {0};
    uint8_t* p = param;

    UINT8_TO_STREAM(p, BTM_BLE_META_READ_IRK_ENTRY);
    UINT8_TO_STREAM(p, p_dev_rec->ble.resolving_list_index);

    BTM_VendorSpecificCommand(HCI_VENDOR_BLE_RPA_VSC, BTM_BLE_META_READ_IRK_LEN,
                              param, btm_ble_resolving_list_vsc_op_cmpl);
  }

  btm_ble_enq_resolving_list_pending(p_dev_rec->bd_addr,
                                     BTM_BLE_META_READ_IRK_ENTRY);

  return BTM_CMD_STARTED;
}

/*******************************************************************************
 *
 * Function         btm_ble_suspend_resolving_list_activity
 *
 * Description      This function suspends all resolving list activity,
 *                  including scanning, initiating, and advertising, if
 *                  resolving list is being enabled.
 *
 * Parameters
 *
 * Returns          true if suspended; false otherwise
 *
 ******************************************************************************/
bool btm_ble_suspend_resolving_list_activity(void) {
  tBTM_BLE_CB* p_ble_cb = &btm_cb.ble_ctr_cb;

  /* if resolving list is not enabled, do not need to terminate any activity */
  /* if asking for stop all activity */
  /* if already suspended */
  if (p_ble_cb->suspended_rl_state != BTM_BLE_RL_IDLE) return true;

  p_ble_cb->suspended_rl_state = BTM_BLE_RL_IDLE;

  if (p_ble_cb->inq_var.adv_mode == BTM_BLE_ADV_ENABLE) {
    btm_ble_stop_adv();
    p_ble_cb->suspended_rl_state |= BTM_BLE_RL_ADV;
  }

  // If it's non-VSC implementation, suspend
  if (BleAdvertisingManager::IsInitialized() &&
      (controller_get_interface()->supports_ble_extended_advertising() ||
       BTM_BleMaxMultiAdvInstanceCount() == 0)) {
    BleAdvertisingManager::Get()->Suspend();
  }

  if (BTM_BLE_IS_SCAN_ACTIVE(p_ble_cb->scan_activity)) {
    btm_ble_stop_scan();
    p_ble_cb->suspended_rl_state |= BTM_BLE_RL_SCAN;
  }

  if (btm_ble_suspend_bg_conn())
    p_ble_cb->suspended_rl_state |= BTM_BLE_RL_INIT;

  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_resume_resolving_list_activity
 *
 * Description      This function resumes the resolving list activity, including
 *                  scanning, initiating, and advertising, if any of these
 *                  activities has been suspended earlier.
 *
 * Returns          none
 *
 ******************************************************************************/
void btm_ble_resume_resolving_list_activity(void) {
  tBTM_BLE_CB* p_ble_cb = &btm_cb.ble_ctr_cb;

  if (p_ble_cb->suspended_rl_state & BTM_BLE_RL_ADV) btm_ble_start_adv();

  // If it's non-VSC implementation, resume
  if (BleAdvertisingManager::IsInitialized() &&
      (controller_get_interface()->supports_ble_extended_advertising() ||
       BTM_BleMaxMultiAdvInstanceCount() == 0)) {
    BleAdvertisingManager::Get()->Resume();
  }

  if (p_ble_cb->suspended_rl_state & BTM_BLE_RL_SCAN) btm_ble_start_scan();

  if (p_ble_cb->suspended_rl_state & BTM_BLE_RL_INIT) btm_ble_resume_bg_conn();

  p_ble_cb->suspended_rl_state = BTM_BLE_RL_IDLE;
}

/*******************************************************************************
 *
 * Function         btm_ble_vendor_enable_irk_feature
 *
 * Description      This function is called to enable or disable the RRA
 *                  offloading feature.
 *
 * Parameters       enable: enable or disable the RRA offloading feature
 *
 ******************************************************************************/
void btm_ble_vendor_enable_irk_feature(bool enable) {
  uint8_t param[20], *p;

  p = param;
  memset(param, 0, 20);

  /* select feature based on control block settings */
  UINT8_TO_STREAM(p, BTM_BLE_META_IRK_ENABLE);
  UINT8_TO_STREAM(p, enable ? 0x01 : 0x00);

  BTM_VendorSpecificCommand(HCI_VENDOR_BLE_RPA_VSC, BTM_BLE_IRK_ENABLE_LEN,
                            param, btm_ble_resolving_list_vsc_op_cmpl);
}

/*******************************************************************************
 *
 * Function         btm_ble_exe_disable_resolving_list
 *
 * Description      execute resolving list disable
 *
 * Returns          none
 *
 ******************************************************************************/
bool btm_ble_exe_disable_resolving_list(void) {
  if (!btm_ble_suspend_resolving_list_activity()) return false;

  if (!controller_get_interface()->supports_ble_privacy())
    btm_ble_vendor_enable_irk_feature(false);
  else
    btsnd_hcic_ble_set_addr_resolution_enable(false);

  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_exe_enable_resolving_list
 *
 * Description      enable LE resolve address list
 *
 * Returns          none
 *
 ******************************************************************************/
void btm_ble_exe_enable_resolving_list(void) {
  if (!btm_ble_suspend_resolving_list_activity()) return;

  if (!controller_get_interface()->supports_ble_privacy())
    btm_ble_vendor_enable_irk_feature(true);
  else
    btsnd_hcic_ble_set_addr_resolution_enable(true);
}

/*******************************************************************************
 *
 * Function         btm_ble_disable_resolving_list
 *
 * Description      Disable LE Address resolution
 *
 * Returns          none
 *
 ******************************************************************************/
bool btm_ble_disable_resolving_list(uint8_t rl_mask, bool to_resume) {
  uint8_t rl_state = btm_cb.ble_ctr_cb.rl_state;

  /* if controller does not support RPA offloading or privacy 1.2, skip */
  if (controller_get_interface()->get_ble_resolving_list_max_size() == 0)
    return false;

  btm_cb.ble_ctr_cb.rl_state &= ~rl_mask;

  if (rl_state != BTM_BLE_RL_IDLE &&
      btm_cb.ble_ctr_cb.rl_state == BTM_BLE_RL_IDLE) {
    if (btm_ble_exe_disable_resolving_list()) {
      if (to_resume) btm_ble_resume_resolving_list_activity();

      return true;
    } else
      return false;
  }

  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_load_dev
 *
 * Description      This function adds a device which is using RPA into the
 *                  white list.
 *
 * Parameters       pointer to device security record
 *
 * Returns          true if device added, otherwise falase.
 *
 ******************************************************************************/
bool btm_ble_resolving_list_load_dev(tBTM_SEC_DEV_REC* p_dev_rec) {
  const uint8_t rl_state = btm_cb.ble_ctr_cb.rl_state;

  if (controller_get_interface()->get_ble_resolving_list_max_size() == 0) {
    BTM_TRACE_DEBUG(
        "%s: Controller does not support RPA offloading or privacy 1.2",
        __func__);
    return false;
  }

  BTM_TRACE_DEBUG("%s: btm_cb.ble_ctr_cb.privacy_mode = %d", __func__,
                  btm_cb.ble_ctr_cb.privacy_mode);

  if (!p_dev_rec) {
    BTM_TRACE_DEBUG("%s: No device security record", __func__);
    return false;
  }

  /* only add RPA enabled device into resolving list */
  if (controller_get_interface()->supports_ble_privacy()) {
    if ((p_dev_rec->ble.key_type & (BTM_LE_KEY_PID | BTM_LE_KEY_LID)) == 0) {
      BTM_TRACE_DEBUG("%s: privacy 1.2: Device not a RPA enabled device",
                      __func__);
      return false;
    }
  } else if ((p_dev_rec->ble.key_type & BTM_LE_KEY_PID) == 0) {
    BTM_TRACE_DEBUG("%s: RPA offloading: Device not a RPA enabled device",
                    __func__);
    return false;
  }

  if ((p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) ||
      btm_ble_brcm_find_resolving_pending_entry(p_dev_rec->bd_addr,
                                                BTM_BLE_META_ADD_IRK_ENTRY)) {
    BTM_TRACE_ERROR("%s: Device already in Resolving list", __func__);
    return true;
  }

  if (btm_cb.ble_ctr_cb.resolving_list_avail_size == 0) {
    return false;
  }

  if (rl_state && !btm_ble_disable_resolving_list(rl_state, false)) {
    return false;
  }

  btm_ble_update_resolving_list(p_dev_rec->bd_addr, true);
  if (controller_get_interface()->supports_ble_privacy()) {
    const Octet16& peer_irk = p_dev_rec->ble.keys.irk;
    const Octet16& local_irk = btm_cb.devcb.id_keys.irk;

    if (p_dev_rec->ble.identity_addr.IsEmpty()) {
      p_dev_rec->ble.identity_addr = p_dev_rec->bd_addr;
      p_dev_rec->ble.identity_addr_type = p_dev_rec->ble.ble_addr_type;
    }

    BTM_TRACE_DEBUG("%s: adding device %s to controller resolving list",
                    __func__, p_dev_rec->ble.identity_addr.ToString().c_str());

    // use identical IRK for now
    btsnd_hcic_ble_add_device_resolving_list(p_dev_rec->ble.identity_addr_type,
                                             p_dev_rec->ble.identity_addr,
                                             peer_irk, local_irk);

    if (controller_get_interface()->supports_ble_set_privacy_mode()) {
      BTM_TRACE_DEBUG("%s: adding device privacy mode", __func__);
      btsnd_hcic_ble_set_privacy_mode(p_dev_rec->ble.identity_addr_type,
                                      p_dev_rec->ble.identity_addr, 0x01);
    }
  } else {
    uint8_t param[40] = {0};
    uint8_t* p = param;

    UINT8_TO_STREAM(p, BTM_BLE_META_ADD_IRK_ENTRY);
    ARRAY_TO_STREAM(p, p_dev_rec->ble.keys.irk, OCTET16_LEN);
    UINT8_TO_STREAM(p, p_dev_rec->ble.identity_addr_type);
    BDADDR_TO_STREAM(p, p_dev_rec->ble.identity_addr);

    BTM_VendorSpecificCommand(HCI_VENDOR_BLE_RPA_VSC, BTM_BLE_META_ADD_IRK_LEN,
                              param, btm_ble_resolving_list_vsc_op_cmpl);
  }

  btm_ble_enq_resolving_list_pending(p_dev_rec->bd_addr,
                                     BTM_BLE_META_ADD_IRK_ENTRY);

  /* if resolving list has been turned on, re-enable it */
  if (rl_state)
    btm_ble_enable_resolving_list(rl_state);
  else
    btm_ble_enable_resolving_list(BTM_BLE_RL_INIT);

  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_remove_dev
 *
 * Description      This function removes the device from resolving list
 *
 * Parameters
 *
 * Returns          status
 *
 ******************************************************************************/
void btm_ble_resolving_list_remove_dev(tBTM_SEC_DEV_REC* p_dev_rec) {
  uint8_t rl_mask = btm_cb.ble_ctr_cb.rl_state;

  BTM_TRACE_EVENT("%s", __func__);
  if (rl_mask) {
    if (!btm_ble_disable_resolving_list(rl_mask, false)) return;
  }

  if ((p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) &&
      !btm_ble_brcm_find_resolving_pending_entry(
          p_dev_rec->bd_addr, BTM_BLE_META_REMOVE_IRK_ENTRY)) {
    btm_ble_update_resolving_list(p_dev_rec->bd_addr, false);
    btm_ble_remove_resolving_list_entry(p_dev_rec);
  } else {
    BTM_TRACE_DEBUG("Device not in resolving list");
  }

  /* if resolving list has been turned on, re-enable it */
  if (rl_mask) btm_ble_enable_resolving_list(rl_mask);
}

/*******************************************************************************
 *
 * Function         btm_ble_enable_resolving_list
 *
 * Description      enable LE resolve address list
 *
 * Returns          none
 *
 ******************************************************************************/
void btm_ble_enable_resolving_list(uint8_t rl_mask) {
  uint8_t rl_state = btm_cb.ble_ctr_cb.rl_state;

  btm_cb.ble_ctr_cb.rl_state |= rl_mask;
  if (rl_state == BTM_BLE_RL_IDLE &&
      btm_cb.ble_ctr_cb.rl_state != BTM_BLE_RL_IDLE &&
      controller_get_interface()->get_ble_resolving_list_max_size() != 0) {
    btm_ble_exe_enable_resolving_list();
    btm_ble_resume_resolving_list_activity();
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_empty
 *
 * Description      check to see if resoving list is empty or not
 *
 * Returns          true: empty; false non-empty
 *
 ******************************************************************************/
bool btm_ble_resolving_list_empty(void) {
  return (controller_get_interface()->get_ble_resolving_list_max_size() ==
          btm_cb.ble_ctr_cb.resolving_list_avail_size);
}

bool is_on_resolving_list(void* data, void* context) {
  tBTM_SEC_DEV_REC* p_dev = static_cast<tBTM_SEC_DEV_REC*>(data);
  if ((p_dev->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) &&
      (p_dev->ble.in_controller_list & BTM_WHITE_LIST_BIT))
    return false;

  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_enable_resolving_list_for_platform
 *
 * Description      enable/disable resolving list feature depending on if any
 *                  resolving list is empty and whitelist is involoved in the
 *                  operation.
 *
 * Returns          none
 *
 ******************************************************************************/
void btm_ble_enable_resolving_list_for_platform(uint8_t rl_mask) {
  /* if controller does not support, skip */
  if (controller_get_interface()->get_ble_resolving_list_max_size() == 0)
    return;

  if (btm_cb.ble_ctr_cb.wl_state == BTM_BLE_WL_IDLE) {
    if (controller_get_interface()->get_ble_resolving_list_max_size() >
        btm_cb.ble_ctr_cb.resolving_list_avail_size)
      btm_ble_enable_resolving_list(rl_mask);
    else
      btm_ble_disable_resolving_list(rl_mask, true);
    return;
  }

  list_node_t* n = list_foreach(btm_cb.sec_dev_rec, is_on_resolving_list, NULL);
  if (n)
    btm_ble_enable_resolving_list(rl_mask);
  else
    btm_ble_disable_resolving_list(rl_mask, true);
}

/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_init
 *
 * Description      Initialize resolving list in host stack
 *
 * Parameters       Max resolving list size
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_resolving_list_init(uint8_t max_irk_list_sz) {
  tBTM_BLE_RESOLVE_Q* p_q = &btm_cb.ble_ctr_cb.resolving_list_pend_q;
  uint8_t irk_mask_size =
      (max_irk_list_sz % 8) ? (max_irk_list_sz / 8 + 1) : (max_irk_list_sz / 8);

  if (max_irk_list_sz > 0) {
    p_q->resolve_q_random_pseudo =
        (RawAddress*)osi_malloc(sizeof(RawAddress) * max_irk_list_sz);
    p_q->resolve_q_action = (uint8_t*)osi_malloc(max_irk_list_sz);

    /* RPA offloading feature */
    if (btm_cb.ble_ctr_cb.irk_list_mask == NULL)
      btm_cb.ble_ctr_cb.irk_list_mask = (uint8_t*)osi_malloc(irk_mask_size);

    BTM_TRACE_DEBUG("%s max_irk_list_sz = %d", __func__, max_irk_list_sz);
  }

  controller_get_interface()->set_ble_resolving_list_max_size(max_irk_list_sz);
  btm_ble_clear_resolving_list();
  btm_cb.ble_ctr_cb.resolving_list_avail_size = max_irk_list_sz;
}

/*******************************************************************************
 *
 * Function         btm_ble_resolving_list_cleanup
 *
 * Description      Cleanup resolving list dynamic memory
 *
 * Parameters
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_resolving_list_cleanup(void) {
  tBTM_BLE_RESOLVE_Q* p_q = &btm_cb.ble_ctr_cb.resolving_list_pend_q;

  osi_free_and_reset((void**)&p_q->resolve_q_random_pseudo);
  osi_free_and_reset((void**)&p_q->resolve_q_action);

  controller_get_interface()->set_ble_resolving_list_max_size(0);

  osi_free_and_reset((void**)&btm_cb.ble_ctr_cb.irk_list_mask);
}
#endif
