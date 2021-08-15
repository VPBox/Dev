/******************************************************************************
 *
 *  Copyright 2000-2012 Broadcom Corporation
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

/*****************************************************************************
 *
 *  Name:          btm_acl.cc
 *
 *  Description:   This file contains functions that handle ACL connections.
 *                 This includes operations such as hold and sniff modes,
 *                 supported packet types.
 *
 *                 This module contains both internal and external (API)
 *                 functions. External (API) functions are distinguishable
 *                 by their names beginning with uppercase BTM.
 *
 *
 *****************************************************************************/

#define LOG_TAG "btm_acl"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bt_common.h"
#include "bt_target.h"
#include "bt_types.h"
#include "bt_utils.h"
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "common/metrics.h"
#include "device/include/controller.h"
#include "device/include/interop.h"
#include "hcidefs.h"
#include "hcimsgs.h"
#include "l2c_int.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

static void btm_read_remote_features(uint16_t handle);
static void btm_read_remote_ext_features(uint16_t handle, uint8_t page_number);
static void btm_process_remote_ext_features(tACL_CONN* p_acl_cb,
                                            uint8_t num_read_pages);

/* 3 seconds timeout waiting for responses */
#define BTM_DEV_REPLY_TIMEOUT_MS (3 * 1000)

/*******************************************************************************
 *
 * Function         btm_acl_init
 *
 * Description      This function is called at BTM startup to initialize
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_init(void) {
  BTM_TRACE_DEBUG("btm_acl_init");
  /* Initialize nonzero defaults */
  btm_cb.btm_def_link_super_tout = HCI_DEFAULT_INACT_TOUT;
  btm_cb.acl_disc_reason = 0xff;
}

/*******************************************************************************
 *
 * Function        btm_bda_to_acl
 *
 * Description     This function returns the FIRST acl_db entry for the passed
 *                 BDA.
 *
 * Parameters      bda : BD address of the remote device
 *                 transport : Physical transport used for ACL connection
 *                 (BR/EDR or LE)
 *
 * Returns         Returns pointer to the ACL DB for the requested BDA if found.
 *                 NULL if not found.
 *
 ******************************************************************************/
tACL_CONN* btm_bda_to_acl(const RawAddress& bda, tBT_TRANSPORT transport) {
  tACL_CONN* p = &btm_cb.acl_db[0];
  uint16_t xx;
  for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p++) {
    if ((p->in_use) && p->remote_addr == bda && p->transport == transport) {
      BTM_TRACE_DEBUG("btm_bda_to_acl found");
      return (p);
    }
  }

  /* If here, no BD Addr found */
  return ((tACL_CONN*)NULL);
}

/*******************************************************************************
 *
 * Function         btm_handle_to_acl_index
 *
 * Description      This function returns the FIRST acl_db entry for the passed
 *                  hci_handle.
 *
 * Returns          index to the acl_db or MAX_L2CAP_LINKS.
 *
 ******************************************************************************/
uint8_t btm_handle_to_acl_index(uint16_t hci_handle) {
  tACL_CONN* p = &btm_cb.acl_db[0];
  uint8_t xx;
  BTM_TRACE_DEBUG("btm_handle_to_acl_index");
  for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p++) {
    if ((p->in_use) && (p->hci_handle == hci_handle)) {
      break;
    }
  }

  /* If here, no BD Addr found */
  return (xx);
}

#if (BLE_PRIVACY_SPT == TRUE)
/*******************************************************************************
 *
 * Function         btm_ble_get_acl_remote_addr
 *
 * Description      This function reads the active remote address used for the
 *                  connection.
 *
 * Returns          success return true, otherwise false.
 *
 ******************************************************************************/
bool btm_ble_get_acl_remote_addr(tBTM_SEC_DEV_REC* p_dev_rec,
                                 RawAddress& conn_addr,
                                 tBLE_ADDR_TYPE* p_addr_type) {
  bool st = true;

  if (p_dev_rec == NULL) {
    BTM_TRACE_ERROR("%s can not find device with matching address", __func__);
    return false;
  }

  switch (p_dev_rec->ble.active_addr_type) {
    case BTM_BLE_ADDR_PSEUDO:
      conn_addr = p_dev_rec->bd_addr;
      *p_addr_type = p_dev_rec->ble.ble_addr_type;
      break;

    case BTM_BLE_ADDR_RRA:
      conn_addr = p_dev_rec->ble.cur_rand_addr;
      *p_addr_type = BLE_ADDR_RANDOM;
      break;

    case BTM_BLE_ADDR_STATIC:
      conn_addr = p_dev_rec->ble.identity_addr;
      *p_addr_type = p_dev_rec->ble.identity_addr_type;
      break;

    default:
      BTM_TRACE_ERROR("Unknown active address: %d",
                      p_dev_rec->ble.active_addr_type);
      st = false;
      break;
  }

  return st;
}
#endif
/*******************************************************************************
 *
 * Function         btm_acl_created
 *
 * Description      This function is called by L2CAP when an ACL connection
 *                  is created.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_created(const RawAddress& bda, DEV_CLASS dc, BD_NAME bdn,
                     uint16_t hci_handle, uint8_t link_role,
                     tBT_TRANSPORT transport) {
  tBTM_SEC_DEV_REC* p_dev_rec = NULL;
  tACL_CONN* p;
  uint8_t xx;

  BTM_TRACE_DEBUG("%s: peer %s hci_handle=%d link_role=%d  transport=%d",
                  __func__, bda.ToString().c_str(), hci_handle, link_role,
                  transport);
  /* Ensure we don't have duplicates */
  p = btm_bda_to_acl(bda, transport);
  if (p != (tACL_CONN*)NULL) {
    p->hci_handle = hci_handle;
    p->link_role = link_role;
    p->transport = transport;
    VLOG(1) << "Duplicate btm_acl_created: RemBdAddr: " << bda;
    BTM_SetLinkPolicy(p->remote_addr, &btm_cb.btm_def_link_policy);
    return;
  }

  /* Allocate acl_db entry */
  for (xx = 0, p = &btm_cb.acl_db[0]; xx < MAX_L2CAP_LINKS; xx++, p++) {
    if (!p->in_use) {
      p->in_use = true;
      p->hci_handle = hci_handle;
      p->link_role = link_role;
      p->link_up_issued = false;
      p->remote_addr = bda;

      p->transport = transport;
#if (BLE_PRIVACY_SPT == TRUE)
      if (transport == BT_TRANSPORT_LE)
        btm_ble_refresh_local_resolvable_private_addr(
            bda, btm_cb.ble_ctr_cb.addr_mgnt_cb.private_addr);
#else
      p->conn_addr_type = BLE_ADDR_PUBLIC;
      p->conn_addr = *controller_get_interface()->get_address();

#endif
      p->switch_role_failed_attempts = 0;
      p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;

      btm_pm_sm_alloc(xx);

      if (dc) memcpy(p->remote_dc, dc, DEV_CLASS_LEN);

      if (bdn) memcpy(p->remote_name, bdn, BTM_MAX_REM_BD_NAME_LEN);

      /* if BR/EDR do something more */
      if (transport == BT_TRANSPORT_BR_EDR) {
        btsnd_hcic_read_rmt_clk_offset(p->hci_handle);
        btsnd_hcic_rmt_ver_req(p->hci_handle);
      }
      p_dev_rec = btm_find_dev_by_handle(hci_handle);

      if (p_dev_rec) {
        BTM_TRACE_DEBUG("%s: peer %s device_type=0x%x", __func__,
                        bda.ToString().c_str(), p_dev_rec->device_type);
      }

      if (p_dev_rec && !(transport == BT_TRANSPORT_LE)) {
        /* If remote features already known, copy them and continue connection
         * setup */
        if ((p_dev_rec->num_read_pages) &&
            (p_dev_rec->num_read_pages <= (HCI_EXT_FEATURES_PAGE_MAX + 1))) {
          memcpy(p->peer_lmp_feature_pages, p_dev_rec->feature_pages,
                 (HCI_FEATURE_BYTES_PER_PAGE * p_dev_rec->num_read_pages));
          p->num_read_pages = p_dev_rec->num_read_pages;

          const uint8_t req_pend = (p_dev_rec->sm4 & BTM_SM4_REQ_PEND);

          /* Store the Peer Security Capabilites (in SM4 and rmt_sec_caps) */
          btm_sec_set_peer_sec_caps(p, p_dev_rec);

          BTM_TRACE_API("%s: pend:%d", __func__, req_pend);
          if (req_pend) {
            /* Request for remaining Security Features (if any) */
            l2cu_resubmit_pending_sec_req(&p_dev_rec->bd_addr);
          }
          btm_establish_continue(p);
          return;
        }
      }

      /* If here, features are not known yet */
      if (p_dev_rec && transport == BT_TRANSPORT_LE) {
#if (BLE_PRIVACY_SPT == TRUE)
        btm_ble_get_acl_remote_addr(p_dev_rec, p->active_remote_addr,
                                    &p->active_remote_addr_type);
#endif

        if (HCI_LE_SLAVE_INIT_FEAT_EXC_SUPPORTED(
                controller_get_interface()->get_features_ble()->as_array) ||
            link_role == HCI_ROLE_MASTER) {
          btsnd_hcic_ble_read_remote_feat(p->hci_handle);
        } else {
          btm_establish_continue(p);
        }
      }

      /* read page 1 - on rmt feature event for buffer reasons */
      return;
    }
  }
}

void btm_acl_update_conn_addr(uint16_t conn_handle, const RawAddress& address) {
  uint8_t idx = btm_handle_to_acl_index(conn_handle);
  if (idx != MAX_L2CAP_LINKS) {
    btm_cb.acl_db[idx].conn_addr = address;
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_report_role_change
 *
 * Description      This function is called when the local device is deemed
 *                  to be down. It notifies L2CAP of the failure.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_report_role_change(uint8_t hci_status, const RawAddress* bda) {
  tBTM_ROLE_SWITCH_CMPL ref_data;
  BTM_TRACE_DEBUG("btm_acl_report_role_change");
  if (btm_cb.devcb.p_switch_role_cb &&
      (bda && btm_cb.devcb.switch_role_ref_data.remote_bd_addr == *bda)) {
    memcpy(&ref_data, &btm_cb.devcb.switch_role_ref_data,
           sizeof(tBTM_ROLE_SWITCH_CMPL));
    ref_data.hci_status = hci_status;
    (*btm_cb.devcb.p_switch_role_cb)(&ref_data);
    memset(&btm_cb.devcb.switch_role_ref_data, 0,
           sizeof(tBTM_ROLE_SWITCH_CMPL));
    btm_cb.devcb.p_switch_role_cb = NULL;
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_removed
 *
 * Description      This function is called by L2CAP when an ACL connection
 *                  is removed. Since only L2CAP creates ACL links, we use
 *                  the L2CAP link index as our index into the control blocks.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_removed(const RawAddress& bda, tBT_TRANSPORT transport) {
  tACL_CONN* p;
  tBTM_SEC_DEV_REC* p_dev_rec = NULL;
  BTM_TRACE_DEBUG("btm_acl_removed");
  p = btm_bda_to_acl(bda, transport);
  if (p != (tACL_CONN*)NULL) {
    p->in_use = false;

    /* if the disconnected channel has a pending role switch, clear it now */
    btm_acl_report_role_change(HCI_ERR_NO_CONNECTION, &bda);

    /* Only notify if link up has had a chance to be issued */
    if (p->link_up_issued) {
      p->link_up_issued = false;

      /* If anyone cares, indicate the database changed */
      if (btm_cb.p_bl_changed_cb) {
        tBTM_BL_EVENT_DATA evt_data;
        evt_data.event = BTM_BL_DISCN_EVT;
        evt_data.discn.p_bda = &bda;
        evt_data.discn.handle = p->hci_handle;
        evt_data.discn.transport = p->transport;
        (*btm_cb.p_bl_changed_cb)(&evt_data);
      }

      btm_acl_update_busy_level(BTM_BLI_ACL_DOWN_EVT);
    }

    BTM_TRACE_DEBUG(
        "acl hci_handle=%d transport=%d connectable_mode=0x%0x link_role=%d",
        p->hci_handle, p->transport, btm_cb.ble_ctr_cb.inq_var.connectable_mode,
        p->link_role);

    p_dev_rec = btm_find_dev(bda);
    if (p_dev_rec) {
      BTM_TRACE_DEBUG("before update p_dev_rec->sec_flags=0x%x",
                      p_dev_rec->sec_flags);
      if (p->transport == BT_TRANSPORT_LE) {
        BTM_TRACE_DEBUG("LE link down");
        p_dev_rec->sec_flags &= ~(BTM_SEC_LE_ENCRYPTED | BTM_SEC_ROLE_SWITCHED);
        if ((p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_KNOWN) == 0) {
          BTM_TRACE_DEBUG("Not Bonded");
          p_dev_rec->sec_flags &=
              ~(BTM_SEC_LE_LINK_KEY_AUTHED | BTM_SEC_LE_AUTHENTICATED);
        } else {
          BTM_TRACE_DEBUG("Bonded");
        }
      } else {
        BTM_TRACE_DEBUG("Bletooth link down");
        p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED |
                                  BTM_SEC_ENCRYPTED | BTM_SEC_ROLE_SWITCHED);
      }
      BTM_TRACE_DEBUG("after update p_dev_rec->sec_flags=0x%x",
                      p_dev_rec->sec_flags);
    } else {
      BTM_TRACE_ERROR("Device not found");
    }

    /* Clear the ACL connection data */
    memset(p, 0, sizeof(tACL_CONN));
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_device_down
 *
 * Description      This function is called when the local device is deemed
 *                  to be down. It notifies L2CAP of the failure.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_device_down(void) {
  tACL_CONN* p = &btm_cb.acl_db[0];
  uint16_t xx;
  BTM_TRACE_DEBUG("btm_acl_device_down");
  for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p++) {
    if (p->in_use) {
      BTM_TRACE_DEBUG("hci_handle=%d HCI_ERR_HW_FAILURE ", p->hci_handle);
      l2c_link_hci_disc_comp(p->hci_handle, HCI_ERR_HW_FAILURE);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_update_busy_level
 *
 * Description      This function is called to update the busy level of the
 *                  system.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_update_busy_level(tBTM_BLI_EVENT event) {
  bool old_inquiry_state = btm_cb.is_inquiry;
  tBTM_BL_UPDATE_DATA evt;
  evt.busy_level_flags = 0;
  switch (event) {
    case BTM_BLI_ACL_UP_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_ACL_UP_EVT");
      break;
    case BTM_BLI_ACL_DOWN_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_ACL_DOWN_EVT");
      break;
    case BTM_BLI_PAGE_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_PAGE_EVT");
      btm_cb.is_paging = true;
      evt.busy_level_flags = BTM_BL_PAGING_STARTED;
      break;
    case BTM_BLI_PAGE_DONE_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_PAGE_DONE_EVT");
      btm_cb.is_paging = false;
      evt.busy_level_flags = BTM_BL_PAGING_COMPLETE;
      break;
    case BTM_BLI_INQ_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_INQ_EVT");
      btm_cb.is_inquiry = true;
      evt.busy_level_flags = BTM_BL_INQUIRY_STARTED;
      break;
    case BTM_BLI_INQ_CANCEL_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_INQ_CANCEL_EVT");
      btm_cb.is_inquiry = false;
      evt.busy_level_flags = BTM_BL_INQUIRY_CANCELLED;
      break;
    case BTM_BLI_INQ_DONE_EVT:
      BTM_TRACE_DEBUG("BTM_BLI_INQ_DONE_EVT");
      btm_cb.is_inquiry = false;
      evt.busy_level_flags = BTM_BL_INQUIRY_COMPLETE;
      break;
  }

  uint8_t busy_level;
  if (btm_cb.is_paging || btm_cb.is_inquiry)
    busy_level = 10;
  else
    busy_level = BTM_GetNumAclLinks();

  if ((busy_level != btm_cb.busy_level) ||
      (old_inquiry_state != btm_cb.is_inquiry)) {
    evt.event = BTM_BL_UPDATE_EVT;
    evt.busy_level = busy_level;
    btm_cb.busy_level = busy_level;
    if (btm_cb.p_bl_changed_cb && (btm_cb.bl_evt_mask & BTM_BL_UPDATE_MASK)) {
      tBTM_BL_EVENT_DATA btm_bl_event_data;
      btm_bl_event_data.update = evt;
      (*btm_cb.p_bl_changed_cb)(&btm_bl_event_data);
    }
  }
}

/*******************************************************************************
 *
 * Function         BTM_GetRole
 *
 * Description      This function is called to get the role of the local device
 *                  for the ACL connection with the specified remote device
 *
 * Returns          BTM_SUCCESS if connection exists.
 *                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
 *
 ******************************************************************************/
tBTM_STATUS BTM_GetRole(const RawAddress& remote_bd_addr, uint8_t* p_role) {
  tACL_CONN* p;
  BTM_TRACE_DEBUG("BTM_GetRole");
  p = btm_bda_to_acl(remote_bd_addr, BT_TRANSPORT_BR_EDR);
  if (p == NULL) {
    *p_role = BTM_ROLE_UNDEFINED;
    return (BTM_UNKNOWN_ADDR);
  }

  /* Get the current role */
  *p_role = p->link_role;
  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_SwitchRole
 *
 * Description      This function is called to switch role between master and
 *                  slave.  If role is already set it will do nothing.  If the
 *                  command was initiated, the callback function is called upon
 *                  completion.
 *
 * Returns          BTM_SUCCESS if already in specified role.
 *                  BTM_CMD_STARTED if command issued to controller.
 *                  BTM_NO_RESOURCES if couldn't allocate memory to issue
 *                                   command
 *                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
 *                  BTM_MODE_UNSUPPORTED if local device does not support role
 *                                       switching
 *                  BTM_BUSY if the previous command is not completed
 *
 ******************************************************************************/
tBTM_STATUS BTM_SwitchRole(const RawAddress& remote_bd_addr, uint8_t new_role,
                           tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p;
  tBTM_SEC_DEV_REC* p_dev_rec = NULL;
  bool is_sco_active;
  tBTM_STATUS status;
  tBTM_PM_MODE pwr_mode;
  tBTM_PM_PWR_MD settings;

  LOG_INFO(LOG_TAG, "%s: peer %s new_role=0x%x p_cb=%p p_switch_role_cb=%p",
           __func__, remote_bd_addr.ToString().c_str(), new_role, p_cb,
           btm_cb.devcb.p_switch_role_cb);

  /* Make sure the local device supports switching */
  if (!controller_get_interface()->supports_master_slave_role_switch())
    return (BTM_MODE_UNSUPPORTED);

  if (btm_cb.devcb.p_switch_role_cb && p_cb) {
    VLOG(2) << "Role switch on other device is in progress "
            << btm_cb.devcb.switch_role_ref_data.remote_bd_addr;
    return (BTM_BUSY);
  }

  p = btm_bda_to_acl(remote_bd_addr, BT_TRANSPORT_BR_EDR);
  if (p == NULL) return (BTM_UNKNOWN_ADDR);

  /* Finished if already in desired role */
  if (p->link_role == new_role) return (BTM_SUCCESS);

  if (interop_match_addr(INTEROP_DISABLE_ROLE_SWITCH, &remote_bd_addr))
    return BTM_DEV_BLACKLISTED;

  /* Check if there is any SCO Active on this BD Address */
  is_sco_active = btm_is_sco_active_by_bdaddr(remote_bd_addr);

  if (is_sco_active) return (BTM_NO_RESOURCES);

  /* Ignore role switch request if the previous request was not completed */
  if (p->switch_role_state != BTM_ACL_SWKEY_STATE_IDLE) {
    BTM_TRACE_DEBUG("BTM_SwitchRole busy: %d", p->switch_role_state);
    return (BTM_BUSY);
  }

  if (interop_match_addr(INTEROP_DYNAMIC_ROLE_SWITCH, &remote_bd_addr)) {
    BTM_TRACE_DEBUG("%s, Device blacklisted under INTEROP_DYNAMIC_ROLE_SWITCH.",
                    __func__);
    return BTM_DEV_BLACKLISTED;
  }

  status = BTM_ReadPowerMode(p->remote_addr, &pwr_mode);
  if (status != BTM_SUCCESS) return (status);

  /* Wake up the link if in sniff or park before attempting switch */
  if (pwr_mode == BTM_PM_MD_PARK || pwr_mode == BTM_PM_MD_SNIFF) {
    memset((void*)&settings, 0, sizeof(settings));
    settings.mode = BTM_PM_MD_ACTIVE;
    status = BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, p->remote_addr, &settings);
    if (status != BTM_CMD_STARTED) return (BTM_WRONG_MODE);

    p->switch_role_state = BTM_ACL_SWKEY_STATE_MODE_CHANGE;
  }
  /* some devices do not support switch while encryption is on */
  else {
    p_dev_rec = btm_find_dev(remote_bd_addr);
    if ((p_dev_rec != NULL) &&
        ((p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) != 0) &&
        !BTM_EPR_AVAILABLE(p)) {
      /* bypass turning off encryption if change link key is already doing it */
      if (p->encrypt_state != BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF) {
        btsnd_hcic_set_conn_encrypt(p->hci_handle, false);
        p->encrypt_state = BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF;
      }

      p->switch_role_state = BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF;
    } else {
      btsnd_hcic_switch_role(remote_bd_addr, new_role);
      p->switch_role_state = BTM_ACL_SWKEY_STATE_IN_PROGRESS;

#if (BTM_DISC_DURING_RS == TRUE)
      if (p_dev_rec) p_dev_rec->rs_disc_pending = BTM_SEC_RS_PENDING;
#endif
    }
  }

  /* Initialize return structure in case request fails */
  if (p_cb) {
    btm_cb.devcb.switch_role_ref_data.remote_bd_addr = remote_bd_addr;
    btm_cb.devcb.switch_role_ref_data.role = new_role;
    /* initialized to an error code */
    btm_cb.devcb.switch_role_ref_data.hci_status = HCI_ERR_UNSUPPORTED_VALUE;
    btm_cb.devcb.p_switch_role_cb = p_cb;
  }
  return (BTM_CMD_STARTED);
}

/*******************************************************************************
 *
 * Function         btm_acl_encrypt_change
 *
 * Description      This function is when encryption of the connection is
 *                  completed by the LM.  Checks to see if a role switch or
 *                  change of link key was active and initiates or continues
 *                  process if needed.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_encrypt_change(uint16_t handle, uint8_t status,
                            uint8_t encr_enable) {
  tACL_CONN* p;
  uint8_t xx;
  tBTM_SEC_DEV_REC* p_dev_rec;

  BTM_TRACE_DEBUG("btm_acl_encrypt_change handle=%d status=%d encr_enabl=%d",
                  handle, status, encr_enable);
  xx = btm_handle_to_acl_index(handle);
  /* don't assume that we can never get a bad hci_handle */
  if (xx < MAX_L2CAP_LINKS)
    p = &btm_cb.acl_db[xx];
  else
    return;

  /* Process Role Switch if active */
  if (p->switch_role_state == BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF) {
    /* if encryption turn off failed we still will try to switch role */
    if (encr_enable) {
      p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;
      p->encrypt_state = BTM_ACL_ENCRYPT_STATE_IDLE;
    } else {
      p->switch_role_state = BTM_ACL_SWKEY_STATE_SWITCHING;
      p->encrypt_state = BTM_ACL_ENCRYPT_STATE_TEMP_FUNC;
    }

    btsnd_hcic_switch_role(p->remote_addr, (uint8_t)!p->link_role);
#if (BTM_DISC_DURING_RS == TRUE)
    p_dev_rec = btm_find_dev(p->remote_addr);
    if (p_dev_rec != NULL) p_dev_rec->rs_disc_pending = BTM_SEC_RS_PENDING;
#endif

  }
  /* Finished enabling Encryption after role switch */
  else if (p->switch_role_state == BTM_ACL_SWKEY_STATE_ENCRYPTION_ON) {
    p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;
    p->encrypt_state = BTM_ACL_ENCRYPT_STATE_IDLE;
    btm_acl_report_role_change(btm_cb.devcb.switch_role_ref_data.hci_status,
                               &p->remote_addr);

    /* if role change event is registered, report it now */
    if (btm_cb.p_bl_changed_cb && (btm_cb.bl_evt_mask & BTM_BL_ROLE_CHG_MASK)) {
      tBTM_BL_ROLE_CHG_DATA evt;
      evt.event = BTM_BL_ROLE_CHG_EVT;
      evt.new_role = btm_cb.devcb.switch_role_ref_data.role;
      evt.p_bda = &btm_cb.devcb.switch_role_ref_data.remote_bd_addr;
      evt.hci_status = btm_cb.devcb.switch_role_ref_data.hci_status;
      tBTM_BL_EVENT_DATA btm_bl_event_data;
      btm_bl_event_data.role_chg = evt;
      (*btm_cb.p_bl_changed_cb)(&btm_bl_event_data);

      BTM_TRACE_DEBUG(
          "%s: Role Switch Event: new_role 0x%02x, HCI Status 0x%02x, rs_st:%d",
          __func__, evt.new_role, evt.hci_status, p->switch_role_state);
    }

#if (BTM_DISC_DURING_RS == TRUE)
    /* If a disconnect is pending, issue it now that role switch has completed
     */
    p_dev_rec = btm_find_dev(p->remote_addr);
    if (p_dev_rec != NULL) {
      if (p_dev_rec->rs_disc_pending == BTM_SEC_DISC_PENDING) {
        BTM_TRACE_WARNING(
            "btm_acl_encrypt_change -> Issuing delayed HCI_Disconnect!!!");
        btsnd_hcic_disconnect(p_dev_rec->hci_handle, HCI_ERR_PEER_USER);
      }
      BTM_TRACE_ERROR(
          "btm_acl_encrypt_change: tBTM_SEC_DEV:0x%x rs_disc_pending=%d",
          PTR_TO_UINT(p_dev_rec), p_dev_rec->rs_disc_pending);
      p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */
    }
#endif
  }
}
/*******************************************************************************
 *
 * Function         BTM_SetLinkPolicy
 *
 * Description      Create and send HCI "Write Policy Set" command
 *
 * Returns          status of the operation
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetLinkPolicy(const RawAddress& remote_bda,
                              uint16_t* settings) {
  tACL_CONN* p;
  uint8_t* localFeatures = BTM_ReadLocalFeatures();
  BTM_TRACE_DEBUG("%s", __func__);
  /*  BTM_TRACE_API ("%s: requested settings: 0x%04x", __func__, *settings ); */

  /* First, check if hold mode is supported */
  if (*settings != HCI_DISABLE_ALL_LM_MODES) {
    if ((*settings & HCI_ENABLE_MASTER_SLAVE_SWITCH) &&
        (!HCI_SWITCH_SUPPORTED(localFeatures))) {
      *settings &= (~HCI_ENABLE_MASTER_SLAVE_SWITCH);
      BTM_TRACE_API("BTM_SetLinkPolicy switch not supported (settings: 0x%04x)",
                    *settings);
    }
    if ((*settings & HCI_ENABLE_HOLD_MODE) &&
        (!HCI_HOLD_MODE_SUPPORTED(localFeatures))) {
      *settings &= (~HCI_ENABLE_HOLD_MODE);
      BTM_TRACE_API("BTM_SetLinkPolicy hold not supported (settings: 0x%04x)",
                    *settings);
    }
    if ((*settings & HCI_ENABLE_SNIFF_MODE) &&
        (!HCI_SNIFF_MODE_SUPPORTED(localFeatures))) {
      *settings &= (~HCI_ENABLE_SNIFF_MODE);
      BTM_TRACE_API("BTM_SetLinkPolicy sniff not supported (settings: 0x%04x)",
                    *settings);
    }
    if ((*settings & HCI_ENABLE_PARK_MODE) &&
        (!HCI_PARK_MODE_SUPPORTED(localFeatures))) {
      *settings &= (~HCI_ENABLE_PARK_MODE);
      BTM_TRACE_API("BTM_SetLinkPolicy park not supported (settings: 0x%04x)",
                    *settings);
    }
  }

  p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_BR_EDR);
  if (p != NULL) {
    btsnd_hcic_write_policy_set(p->hci_handle, *settings);
    return BTM_CMD_STARTED;
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_SetDefaultLinkPolicy
 *
 * Description      Set the default value for HCI "Write Policy Set" command
 *                  to use when an ACL link is created.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetDefaultLinkPolicy(uint16_t settings) {
  uint8_t* localFeatures = BTM_ReadLocalFeatures();

  BTM_TRACE_DEBUG("BTM_SetDefaultLinkPolicy setting:0x%04x", settings);

  if ((settings & HCI_ENABLE_MASTER_SLAVE_SWITCH) &&
      (!HCI_SWITCH_SUPPORTED(localFeatures))) {
    settings &= ~HCI_ENABLE_MASTER_SLAVE_SWITCH;
    BTM_TRACE_DEBUG(
        "BTM_SetDefaultLinkPolicy switch not supported (settings: 0x%04x)",
        settings);
  }
  if ((settings & HCI_ENABLE_HOLD_MODE) &&
      (!HCI_HOLD_MODE_SUPPORTED(localFeatures))) {
    settings &= ~HCI_ENABLE_HOLD_MODE;
    BTM_TRACE_DEBUG(
        "BTM_SetDefaultLinkPolicy hold not supported (settings: 0x%04x)",
        settings);
  }
  if ((settings & HCI_ENABLE_SNIFF_MODE) &&
      (!HCI_SNIFF_MODE_SUPPORTED(localFeatures))) {
    settings &= ~HCI_ENABLE_SNIFF_MODE;
    BTM_TRACE_DEBUG(
        "BTM_SetDefaultLinkPolicy sniff not supported (settings: 0x%04x)",
        settings);
  }
  if ((settings & HCI_ENABLE_PARK_MODE) &&
      (!HCI_PARK_MODE_SUPPORTED(localFeatures))) {
    settings &= ~HCI_ENABLE_PARK_MODE;
    BTM_TRACE_DEBUG(
        "BTM_SetDefaultLinkPolicy park not supported (settings: 0x%04x)",
        settings);
  }
  BTM_TRACE_DEBUG("Set DefaultLinkPolicy:0x%04x", settings);

  btm_cb.btm_def_link_policy = settings;

  /* Set the default Link Policy of the controller */
  btsnd_hcic_write_def_policy_set(settings);
}

void btm_use_preferred_conn_params(const RawAddress& bda) {
  tL2C_LCB* p_lcb = l2cu_find_lcb_by_bd_addr(bda, BT_TRANSPORT_LE);
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_or_alloc_dev(bda);

  /* If there are any preferred connection parameters, set them now */
  if ((p_lcb != NULL) && (p_dev_rec != NULL) &&
      (p_dev_rec->conn_params.min_conn_int >= BTM_BLE_CONN_INT_MIN) &&
      (p_dev_rec->conn_params.min_conn_int <= BTM_BLE_CONN_INT_MAX) &&
      (p_dev_rec->conn_params.max_conn_int >= BTM_BLE_CONN_INT_MIN) &&
      (p_dev_rec->conn_params.max_conn_int <= BTM_BLE_CONN_INT_MAX) &&
      (p_dev_rec->conn_params.slave_latency <= BTM_BLE_CONN_LATENCY_MAX) &&
      (p_dev_rec->conn_params.supervision_tout >= BTM_BLE_CONN_SUP_TOUT_MIN) &&
      (p_dev_rec->conn_params.supervision_tout <= BTM_BLE_CONN_SUP_TOUT_MAX) &&
      ((p_lcb->min_interval < p_dev_rec->conn_params.min_conn_int &&
        p_dev_rec->conn_params.min_conn_int != BTM_BLE_CONN_PARAM_UNDEF) ||
       (p_lcb->min_interval > p_dev_rec->conn_params.max_conn_int) ||
       (p_lcb->latency > p_dev_rec->conn_params.slave_latency) ||
       (p_lcb->timeout > p_dev_rec->conn_params.supervision_tout))) {
    BTM_TRACE_DEBUG(
        "%s: HANDLE=%d min_conn_int=%d max_conn_int=%d slave_latency=%d "
        "supervision_tout=%d",
        __func__, p_lcb->handle, p_dev_rec->conn_params.min_conn_int,
        p_dev_rec->conn_params.max_conn_int,
        p_dev_rec->conn_params.slave_latency,
        p_dev_rec->conn_params.supervision_tout);

    p_lcb->min_interval = p_dev_rec->conn_params.min_conn_int;
    p_lcb->max_interval = p_dev_rec->conn_params.max_conn_int;
    p_lcb->timeout = p_dev_rec->conn_params.supervision_tout;
    p_lcb->latency = p_dev_rec->conn_params.slave_latency;

    btsnd_hcic_ble_upd_ll_conn_params(
        p_lcb->handle, p_dev_rec->conn_params.min_conn_int,
        p_dev_rec->conn_params.max_conn_int,
        p_dev_rec->conn_params.slave_latency,
        p_dev_rec->conn_params.supervision_tout, 0, 0);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_remote_version_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the remote version info.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_version_complete(uint8_t* p) {
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];
  uint8_t status;
  uint16_t handle;
  int xx;
  BTM_TRACE_DEBUG("btm_read_remote_version_complete");

  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);

  /* Look up the connection by handle and copy features */
  for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p_acl_cb++) {
    if ((p_acl_cb->in_use) && (p_acl_cb->hci_handle == handle)) {
      if (status == HCI_SUCCESS) {
        STREAM_TO_UINT8(p_acl_cb->lmp_version, p);
        STREAM_TO_UINT16(p_acl_cb->manufacturer, p);
        STREAM_TO_UINT16(p_acl_cb->lmp_subversion, p);

        if (p_acl_cb->transport == BT_TRANSPORT_BR_EDR) {
          btm_read_remote_features(p_acl_cb->hci_handle);
        }
        bluetooth::common::LogRemoteVersionInfo(
            handle, status, p_acl_cb->lmp_version, p_acl_cb->manufacturer,
            p_acl_cb->lmp_subversion);
      } else {
        bluetooth::common::LogRemoteVersionInfo(handle, status, 0, 0, 0);
      }

      if (p_acl_cb->transport == BT_TRANSPORT_LE) {
        l2cble_notify_le_connection(p_acl_cb->remote_addr);
        btm_use_preferred_conn_params(p_acl_cb->remote_addr);
      }
      break;
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_process_remote_ext_features
 *
 * Description      Local function called to process all extended features pages
 *                  read from a remote device.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_remote_ext_features(tACL_CONN* p_acl_cb,
                                     uint8_t num_read_pages) {
  uint16_t handle = p_acl_cb->hci_handle;
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
  uint8_t page_idx;

  BTM_TRACE_DEBUG("btm_process_remote_ext_features");

  /* Make sure we have the record to save remote features information */
  if (p_dev_rec == NULL) {
    /* Get a new device; might be doing dedicated bonding */
    p_dev_rec = btm_find_or_alloc_dev(p_acl_cb->remote_addr);
  }

  p_acl_cb->num_read_pages = num_read_pages;
  p_dev_rec->num_read_pages = num_read_pages;

  /* Move the pages to placeholder */
  for (page_idx = 0; page_idx < num_read_pages; page_idx++) {
    if (page_idx > HCI_EXT_FEATURES_PAGE_MAX) {
      BTM_TRACE_ERROR("%s: page=%d unexpected", __func__, page_idx);
      break;
    }
    memcpy(p_dev_rec->feature_pages[page_idx],
           p_acl_cb->peer_lmp_feature_pages[page_idx],
           HCI_FEATURE_BYTES_PER_PAGE);
  }

  if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) ||
      p_dev_rec->is_originator) {
    BTM_TRACE_DEBUG("%s: Calling Next Security Procedure", __func__);
    uint8_t status = btm_sec_execute_procedure(p_dev_rec);
    if (status != BTM_CMD_STARTED) {
      BTM_TRACE_ERROR("%s: Security procedure not started! status %d", __func__,
                      status);
      btm_sec_dev_rec_cback_event(p_dev_rec, status, false);
    }
  }
  const uint8_t req_pend = (p_dev_rec->sm4 & BTM_SM4_REQ_PEND);

  /* Store the Peer Security Capabilites (in SM4 and rmt_sec_caps) */
  btm_sec_set_peer_sec_caps(p_acl_cb, p_dev_rec);

  BTM_TRACE_API("%s: pend:%d", __func__, req_pend);
  if (req_pend) {
    /* Request for remaining Security Features (if any) */
    l2cu_resubmit_pending_sec_req(&p_dev_rec->bd_addr);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_remote_features
 *
 * Description      Local function called to send a read remote supported
 *                  features/remote extended features page[0].
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_features(uint16_t handle) {
  uint8_t acl_idx;
  tACL_CONN* p_acl_cb;

  BTM_TRACE_DEBUG("btm_read_remote_features() handle: %d", handle);

  acl_idx = btm_handle_to_acl_index(handle);
  if (acl_idx >= MAX_L2CAP_LINKS) {
    BTM_TRACE_ERROR("btm_read_remote_features handle=%d invalid", handle);
    return;
  }

  p_acl_cb = &btm_cb.acl_db[acl_idx];
  p_acl_cb->num_read_pages = 0;
  memset(p_acl_cb->peer_lmp_feature_pages, 0,
         sizeof(p_acl_cb->peer_lmp_feature_pages));

  /* first send read remote supported features HCI command */
  /* because we don't know whether the remote support extended feature command
   */
  btsnd_hcic_rmt_features_req(handle);
}

/*******************************************************************************
 *
 * Function         btm_read_remote_ext_features
 *
 * Description      Local function called to send a read remote extended
 *                  features
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_ext_features(uint16_t handle, uint8_t page_number) {
  BTM_TRACE_DEBUG("btm_read_remote_ext_features() handle: %d page: %d", handle,
                  page_number);

  btsnd_hcic_rmt_ext_features(handle, page_number);
}

/*******************************************************************************
 *
 * Function         btm_read_remote_features_complete
 *
 * Description      This function is called when the remote supported features
 *                  complete event is received from the HCI.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_features_complete(uint8_t* p) {
  tACL_CONN* p_acl_cb;
  uint8_t status;
  uint16_t handle;
  uint8_t acl_idx;

  BTM_TRACE_DEBUG("btm_read_remote_features_complete");
  STREAM_TO_UINT8(status, p);

  if (status != HCI_SUCCESS) {
    BTM_TRACE_ERROR("btm_read_remote_features_complete failed (status 0x%02x)",
                    status);
    return;
  }

  STREAM_TO_UINT16(handle, p);

  acl_idx = btm_handle_to_acl_index(handle);
  if (acl_idx >= MAX_L2CAP_LINKS) {
    BTM_TRACE_ERROR("btm_read_remote_features_complete handle=%d invalid",
                    handle);
    return;
  }

  p_acl_cb = &btm_cb.acl_db[acl_idx];

  /* Copy the received features page */
  STREAM_TO_ARRAY(p_acl_cb->peer_lmp_feature_pages[0], p,
                  HCI_FEATURE_BYTES_PER_PAGE);

  if ((HCI_LMP_EXTENDED_SUPPORTED(p_acl_cb->peer_lmp_feature_pages[0])) &&
      (controller_get_interface()
           ->supports_reading_remote_extended_features())) {
    /* if the remote controller has extended features and local controller
       supports HCI_Read_Remote_Extended_Features command then start reading
       these feature starting with extended features page 1 */
    BTM_TRACE_DEBUG("Start reading remote extended features");
    btm_read_remote_ext_features(handle, 1);
    return;
  }

  /* Remote controller has no extended features. Process remote controller
     supported features (features page 0). */
  btm_process_remote_ext_features(p_acl_cb, 1);

  /* Continue with HCI connection establishment */
  btm_establish_continue(p_acl_cb);
}

/*******************************************************************************
 *
 * Function         btm_read_remote_ext_features_complete
 *
 * Description      This function is called when the remote extended features
 *                  complete event is received from the HCI.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_ext_features_complete(uint8_t* p, uint8_t evt_len) {
  tACL_CONN* p_acl_cb;
  uint8_t page_num, max_page;
  uint16_t handle;
  uint8_t acl_idx;

  BTM_TRACE_DEBUG("btm_read_remote_ext_features_complete");

  if (evt_len < HCI_EXT_FEATURES_SUCCESS_EVT_LEN) {
    android_errorWriteLog(0x534e4554, "141552859");
    BTM_TRACE_ERROR(
        "btm_read_remote_ext_features_complete evt length too short. length=%d",
        evt_len);
    return;
  }

  ++p;
  STREAM_TO_UINT16(handle, p);
  STREAM_TO_UINT8(page_num, p);
  STREAM_TO_UINT8(max_page, p);

  /* Validate parameters */
  acl_idx = btm_handle_to_acl_index(handle);
  if (acl_idx >= MAX_L2CAP_LINKS) {
    BTM_TRACE_ERROR("btm_read_remote_ext_features_complete handle=%d invalid",
                    handle);
    return;
  }

  if (max_page > HCI_EXT_FEATURES_PAGE_MAX) {
    BTM_TRACE_ERROR("btm_read_remote_ext_features_complete page=%d unknown",
                    max_page);
    return;
  }

  if (page_num > HCI_EXT_FEATURES_PAGE_MAX) {
    android_errorWriteLog(0x534e4554, "141552859");
    BTM_TRACE_ERROR("btm_read_remote_ext_features_complete num_page=%d invalid",
                    page_num);
    return;
  }

  if (page_num > max_page) {
    BTM_TRACE_WARNING(
        "btm_read_remote_ext_features_complete num_page=%d, max_page=%d "
        "invalid", page_num, max_page);
  }

  p_acl_cb = &btm_cb.acl_db[acl_idx];

  /* Copy the received features page */
  STREAM_TO_ARRAY(p_acl_cb->peer_lmp_feature_pages[page_num], p,
                  HCI_FEATURE_BYTES_PER_PAGE);

  /* If there is the next remote features page and
   * we have space to keep this page data - read this page */
  if ((page_num < max_page) && (page_num < HCI_EXT_FEATURES_PAGE_MAX)) {
    page_num++;
    BTM_TRACE_DEBUG("BTM reads next remote extended features page (%d)",
                    page_num);
    btm_read_remote_ext_features(handle, page_num);
    return;
  }

  /* Reading of remote feature pages is complete */
  BTM_TRACE_DEBUG("BTM reached last remote extended features page (%d)",
                  page_num);

  /* Process the pages */
  btm_process_remote_ext_features(p_acl_cb, (uint8_t)(page_num + 1));

  /* Continue with HCI connection establishment */
  btm_establish_continue(p_acl_cb);
}

/*******************************************************************************
 *
 * Function         btm_read_remote_ext_features_failed
 *
 * Description      This function is called when the remote extended features
 *                  complete event returns a failed status.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_remote_ext_features_failed(uint8_t status, uint16_t handle) {
  tACL_CONN* p_acl_cb;
  uint8_t acl_idx;

  BTM_TRACE_WARNING(
      "btm_read_remote_ext_features_failed (status 0x%02x) for handle %d",
      status, handle);

  acl_idx = btm_handle_to_acl_index(handle);
  if (acl_idx >= MAX_L2CAP_LINKS) {
    BTM_TRACE_ERROR("btm_read_remote_ext_features_failed handle=%d invalid",
                    handle);
    return;
  }

  p_acl_cb = &btm_cb.acl_db[acl_idx];

  /* Process supported features only */
  btm_process_remote_ext_features(p_acl_cb, 1);

  /* Continue HCI connection establishment */
  btm_establish_continue(p_acl_cb);
}

/*******************************************************************************
 *
 * Function         btm_establish_continue
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read local link policy
 *                  request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_establish_continue(tACL_CONN* p_acl_cb) {
  tBTM_BL_EVENT_DATA evt_data;
  BTM_TRACE_DEBUG("btm_establish_continue");
#if (BTM_BYPASS_EXTRA_ACL_SETUP == FALSE)
  if (p_acl_cb->transport == BT_TRANSPORT_BR_EDR) {
    /* For now there are a some devices that do not like sending */
    /* commands events and data at the same time. */
    /* Set the packet types to the default allowed by the device */
    btm_set_packet_types(p_acl_cb, btm_cb.btm_acl_pkt_types_supported);

    if (btm_cb.btm_def_link_policy)
      BTM_SetLinkPolicy(p_acl_cb->remote_addr, &btm_cb.btm_def_link_policy);
  }
#endif
  if (p_acl_cb->link_up_issued) {
    BTM_TRACE_ERROR("%s: Already link is up ", __func__);
    return;
  }
  p_acl_cb->link_up_issued = true;

  /* If anyone cares, tell him database changed */
  if (btm_cb.p_bl_changed_cb) {
    evt_data.event = BTM_BL_CONN_EVT;
    evt_data.conn.p_bda = &p_acl_cb->remote_addr;
    evt_data.conn.p_bdn = p_acl_cb->remote_name;
    evt_data.conn.p_dc = p_acl_cb->remote_dc;
    evt_data.conn.p_features = p_acl_cb->peer_lmp_feature_pages[0];
    evt_data.conn.handle = p_acl_cb->hci_handle;
    evt_data.conn.transport = p_acl_cb->transport;

    (*btm_cb.p_bl_changed_cb)(&evt_data);
  }
  btm_acl_update_busy_level(BTM_BLI_ACL_UP_EVT);
}

/*******************************************************************************
 *
 * Function         BTM_SetDefaultLinkSuperTout
 *
 * Description      Set the default value for HCI "Write Link Supervision
 *                                                 Timeout"
 *                  command to use when an ACL link is created.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetDefaultLinkSuperTout(uint16_t timeout) {
  BTM_TRACE_DEBUG("BTM_SetDefaultLinkSuperTout");
  btm_cb.btm_def_link_super_tout = timeout;
}

/*******************************************************************************
 *
 * Function         BTM_GetLinkSuperTout
 *
 * Description      Read the link supervision timeout value of the connection
 *
 * Returns          status of the operation
 *
 ******************************************************************************/
tBTM_STATUS BTM_GetLinkSuperTout(const RawAddress& remote_bda,
                                 uint16_t* p_timeout) {
  tACL_CONN* p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_BR_EDR);

  BTM_TRACE_DEBUG("BTM_GetLinkSuperTout");
  if (p != (tACL_CONN*)NULL) {
    *p_timeout = p->link_super_tout;
    return (BTM_SUCCESS);
  }
  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_SetLinkSuperTout
 *
 * Description      Create and send HCI "Write Link Supervision Timeout" command
 *
 * Returns          status of the operation
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetLinkSuperTout(const RawAddress& remote_bda,
                                 uint16_t timeout) {
  tACL_CONN* p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_BR_EDR);

  BTM_TRACE_DEBUG("BTM_SetLinkSuperTout");
  if (p != (tACL_CONN*)NULL) {
    p->link_super_tout = timeout;

    /* Only send if current role is Master; 2.0 spec requires this */
    if (p->link_role == BTM_ROLE_MASTER) {
      btsnd_hcic_write_link_super_tout(LOCAL_BR_EDR_CONTROLLER_ID,
                                       p->hci_handle, timeout);
      return (BTM_CMD_STARTED);
    } else {
      return (BTM_SUCCESS);
    }
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_IsAclConnectionUp
 *
 * Description      This function is called to check if an ACL connection exists
 *                  to a specific remote BD Address.
 *
 * Returns          true if connection is up, else false.
 *
 ******************************************************************************/
bool BTM_IsAclConnectionUp(const RawAddress& remote_bda,
                           tBT_TRANSPORT transport) {
  tACL_CONN* p;

  VLOG(2) << __func__ << " RemBdAddr: " << remote_bda;

  p = btm_bda_to_acl(remote_bda, transport);
  if (p != (tACL_CONN*)NULL) {
    return (true);
  }

  /* If here, no BD Addr found */
  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_GetNumAclLinks
 *
 * Description      This function is called to count the number of
 *                  ACL links that are active.
 *
 * Returns          uint16_t Number of active ACL links
 *
 ******************************************************************************/
uint16_t BTM_GetNumAclLinks(void) {
  uint16_t num_acl = 0;

  for (uint16_t i = 0; i < MAX_L2CAP_LINKS; ++i) {
    if (btm_cb.acl_db[i].in_use) ++num_acl;
  }

  return num_acl;
}

/*******************************************************************************
 *
 * Function         btm_get_acl_disc_reason_code
 *
 * Description      This function is called to get the disconnection reason code
 *                  returned by the HCI at disconnection complete event.
 *
 * Returns          true if connection is up, else false.
 *
 ******************************************************************************/
uint16_t btm_get_acl_disc_reason_code(void) {
  uint8_t res = btm_cb.acl_disc_reason;
  BTM_TRACE_DEBUG("btm_get_acl_disc_reason_code");
  return (res);
}

/*******************************************************************************
 *
 * Function         BTM_GetHCIConnHandle
 *
 * Description      This function is called to get the handle for an ACL
 *                  connection to a specific remote BD Address.
 *
 * Returns          the handle of the connection, or 0xFFFF if none.
 *
 ******************************************************************************/
uint16_t BTM_GetHCIConnHandle(const RawAddress& remote_bda,
                              tBT_TRANSPORT transport) {
  tACL_CONN* p;
  BTM_TRACE_DEBUG("BTM_GetHCIConnHandle");
  p = btm_bda_to_acl(remote_bda, transport);
  if (p != (tACL_CONN*)NULL) {
    return (p->hci_handle);
  }

  /* If here, no BD Addr found */
  return (0xFFFF);
}

/*******************************************************************************
 *
 * Function         btm_process_clk_off_comp_evt
 *
 * Description      This function is called when clock offset command completes.
 *
 * Input Parms      hci_handle - connection handle associated with the change
 *                  clock offset
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_clk_off_comp_evt(uint16_t hci_handle, uint16_t clock_offset) {
  uint8_t xx;
  BTM_TRACE_DEBUG("btm_process_clk_off_comp_evt");
  /* Look up the connection by handle and set the current mode */
  xx = btm_handle_to_acl_index(hci_handle);
  if (xx < MAX_L2CAP_LINKS) btm_cb.acl_db[xx].clock_offset = clock_offset;
}

/*******************************************************************************
*
* Function         btm_blacklist_role_change_device
*
* Description      This function is used to blacklist the device if the role
*                  switch fails for maximum number of times. It also removes
*                  the device from the black list if the role switch succeeds.
*
* Input Parms      bd_addr - remote BD addr
*                  hci_status - role switch status
*
* Returns          void
*
*******************************************************************************/
void btm_blacklist_role_change_device(const RawAddress& bd_addr,
                                      uint8_t hci_status) {
  tACL_CONN* p = btm_bda_to_acl(bd_addr, BT_TRANSPORT_BR_EDR);
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

  if (!p || !p_dev_rec) {
    return;
  }
  if (hci_status == HCI_SUCCESS) {
    p->switch_role_failed_attempts = 0;
    return;
  }

  /* check for carkits */
  const uint32_t cod_audio_device =
      (BTM_COD_SERVICE_AUDIO | BTM_COD_MAJOR_AUDIO) << 8;
  const uint32_t cod =
      ((p_dev_rec->dev_class[0] << 16) | (p_dev_rec->dev_class[1] << 8) |
       p_dev_rec->dev_class[2]) &
      0xffffff;
  if ((hci_status != HCI_SUCCESS) &&
      ((p->switch_role_state == BTM_ACL_SWKEY_STATE_SWITCHING) ||
       (p->switch_role_state == BTM_ACL_SWKEY_STATE_IN_PROGRESS)) &&
      ((cod & cod_audio_device) == cod_audio_device) &&
      (!interop_match_addr(INTEROP_DYNAMIC_ROLE_SWITCH, &bd_addr))) {
    p->switch_role_failed_attempts++;
    if (p->switch_role_failed_attempts == BTM_MAX_SW_ROLE_FAILED_ATTEMPTS) {
      BTM_TRACE_WARNING(
          "%s: Device %s blacklisted for role switching - "
          "multiple role switch failed attempts: %u",
          __func__, bd_addr.ToString().c_str(), p->switch_role_failed_attempts);
      interop_database_add(INTEROP_DYNAMIC_ROLE_SWITCH, &bd_addr, 3);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_role_changed
 *
 * Description      This function is called whan a link's master/slave role
 *                  change event or command status event (with error) is
 *                  received. It updates the link control block, and calls the
 *                  registered callback with status and role (if registered).
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_acl_role_changed(uint8_t hci_status, const RawAddress* bd_addr,
                          uint8_t new_role) {
  const RawAddress* p_bda =
      (bd_addr) ? bd_addr : &btm_cb.devcb.switch_role_ref_data.remote_bd_addr;
  tACL_CONN* p = btm_bda_to_acl(*p_bda, BT_TRANSPORT_BR_EDR);
  tBTM_ROLE_SWITCH_CMPL* p_data = &btm_cb.devcb.switch_role_ref_data;
  tBTM_SEC_DEV_REC* p_dev_rec;

  BTM_TRACE_DEBUG("%s: peer %s hci_status:0x%x new_role:%d", __func__,
                  (p_bda != nullptr) ? bd_addr->ToString().c_str() : "nullptr",
                  hci_status, new_role);
  /* Ignore any stray events */
  if (p == NULL) {
    /* it could be a failure */
    if (hci_status != HCI_SUCCESS)
      btm_acl_report_role_change(hci_status, bd_addr);
    return;
  }

  p_data->hci_status = hci_status;

  if (hci_status == HCI_SUCCESS) {
    p_data->role = new_role;
    p_data->remote_bd_addr = *p_bda;

    /* Update cached value */
    p->link_role = new_role;

    /* Reload LSTO: link supervision timeout is reset in the LM after a role
     * switch */
    if (new_role == BTM_ROLE_MASTER) {
      BTM_SetLinkSuperTout(p->remote_addr, p->link_super_tout);
    }
  } else {
    /* so the BTM_BL_ROLE_CHG_EVT uses the old role */
    new_role = p->link_role;
  }

  /* Check if any SCO req is pending for role change */
  btm_sco_chk_pend_rolechange(p->hci_handle);

  /* if switching state is switching we need to turn encryption on */
  /* if idle, we did not change encryption */
  if (p->switch_role_state == BTM_ACL_SWKEY_STATE_SWITCHING) {
    btsnd_hcic_set_conn_encrypt(p->hci_handle, true);
    p->encrypt_state = BTM_ACL_ENCRYPT_STATE_ENCRYPT_ON;
    p->switch_role_state = BTM_ACL_SWKEY_STATE_ENCRYPTION_ON;
    return;
  }

  /* Set the switch_role_state to IDLE since the reply received from HCI */
  /* regardless of its result either success or failed. */
  if (p->switch_role_state == BTM_ACL_SWKEY_STATE_IN_PROGRESS) {
    p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;
    p->encrypt_state = BTM_ACL_ENCRYPT_STATE_IDLE;
  }

  /* if role switch complete is needed, report it now */
  btm_acl_report_role_change(hci_status, bd_addr);

  /* if role change event is registered, report it now */
  if (btm_cb.p_bl_changed_cb && (btm_cb.bl_evt_mask & BTM_BL_ROLE_CHG_MASK)) {
    tBTM_BL_ROLE_CHG_DATA evt;
    evt.event = BTM_BL_ROLE_CHG_EVT;
    evt.new_role = new_role;
    evt.p_bda = p_bda;
    evt.hci_status = hci_status;
    tBTM_BL_EVENT_DATA btm_bl_event_data;
    btm_bl_event_data.role_chg = evt;
    (*btm_cb.p_bl_changed_cb)(&btm_bl_event_data);
  }

  BTM_TRACE_DEBUG(
      "%s: peer %s Role Switch Event: new_role 0x%02x, HCI Status 0x%02x, "
      "rs_st:%d",
      __func__, (p_bda != nullptr) ? p_bda->ToString().c_str() : "nullptr",
      p_data->role, p_data->hci_status, p->switch_role_state);

#if (BTM_DISC_DURING_RS == TRUE)
  /* If a disconnect is pending, issue it now that role switch has completed */
  p_dev_rec = btm_find_dev(*p_bda);
  if (p_dev_rec != NULL) {
    if (p_dev_rec->rs_disc_pending == BTM_SEC_DISC_PENDING) {
      BTM_TRACE_WARNING(
          "%s peer %s Issuing delayed HCI_Disconnect!!!", __func__,
          (p_bda != nullptr) ? p_bda->ToString().c_str() : "nullptr");
      btsnd_hcic_disconnect(p_dev_rec->hci_handle, HCI_ERR_PEER_USER);
    }
    BTM_TRACE_ERROR("%s: peer %s tBTM_SEC_DEV:0x%x rs_disc_pending=%d",
                    __func__,
                    (p_bda != nullptr) ? p_bda->ToString().c_str() : "nullptr",
                    PTR_TO_UINT(p_dev_rec), p_dev_rec->rs_disc_pending);
    p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */
  }

#endif
}

/*******************************************************************************
 *
 * Function         BTM_AllocateSCN
 *
 * Description      Look through the Server Channel Numbers for a free one.
 *
 * Returns          Allocated SCN number or 0 if none.
 *
 ******************************************************************************/

uint8_t BTM_AllocateSCN(void) {
  uint8_t x;
  BTM_TRACE_DEBUG("BTM_AllocateSCN");

  // stack reserves scn 1 for HFP, HSP we still do the correct way
  for (x = 1; x < BTM_MAX_SCN; x++) {
    if (!btm_cb.btm_scn[x]) {
      btm_cb.btm_scn[x] = true;
      return (x + 1);
    }
  }

  return (0); /* No free ports */
}

/*******************************************************************************
 *
 * Function         BTM_TryAllocateSCN
 *
 * Description      Try to allocate a fixed server channel
 *
 * Returns          Returns true if server channel was available
 *
 ******************************************************************************/

bool BTM_TryAllocateSCN(uint8_t scn) {
  /* Make sure we don't exceed max port range.
   * Stack reserves scn 1 for HFP, HSP we still do the correct way.
   */
  if ((scn >= BTM_MAX_SCN) || (scn == 1)) return false;

  /* check if this port is available */
  if (!btm_cb.btm_scn[scn - 1]) {
    btm_cb.btm_scn[scn - 1] = true;
    return true;
  }

  return (false); /* Port was busy */
}

/*******************************************************************************
 *
 * Function         BTM_FreeSCN
 *
 * Description      Free the specified SCN.
 *
 * Returns          true or false
 *
 ******************************************************************************/
bool BTM_FreeSCN(uint8_t scn) {
  BTM_TRACE_DEBUG("BTM_FreeSCN ");
  if (scn <= BTM_MAX_SCN) {
    btm_cb.btm_scn[scn - 1] = false;
    return (true);
  } else {
    return (false); /* Illegal SCN passed in */
  }
}

/*******************************************************************************
 *
 * Function         btm_set_packet_types
 *
 * Description      This function sets the packet types used for a specific
 *                  ACL connection. It is called internally by btm_acl_created
 *                  or by an application/profile by BTM_SetPacketTypes.
 *
 * Returns          status of the operation
 *
 ******************************************************************************/
tBTM_STATUS btm_set_packet_types(tACL_CONN* p, uint16_t pkt_types) {
  uint16_t temp_pkt_types;
  BTM_TRACE_DEBUG("btm_set_packet_types");
  /* Save in the ACL control blocks, types that we support */
  temp_pkt_types = (pkt_types & BTM_ACL_SUPPORTED_PKTS_MASK &
                    btm_cb.btm_acl_pkt_types_supported);

  /* OR in any exception packet types if at least 2.0 version of spec */
  temp_pkt_types |=
      ((pkt_types & BTM_ACL_EXCEPTION_PKTS_MASK) |
       (btm_cb.btm_acl_pkt_types_supported & BTM_ACL_EXCEPTION_PKTS_MASK));

  /* Exclude packet types not supported by the peer */
  btm_acl_chk_peer_pkt_type_support(p, &temp_pkt_types);

  BTM_TRACE_DEBUG("SetPacketType Mask -> 0x%04x", temp_pkt_types);

  btsnd_hcic_change_conn_type(p->hci_handle, temp_pkt_types);
  p->pkt_types_mask = temp_pkt_types;

  return (BTM_CMD_STARTED);
}

/*******************************************************************************
 *
 * Function         btm_get_max_packet_size
 *
 * Returns          Returns maximum packet size that can be used for current
 *                  connection, 0 if connection is not established
 *
 ******************************************************************************/
uint16_t btm_get_max_packet_size(const RawAddress& addr) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  uint16_t pkt_types = 0;
  uint16_t pkt_size = 0;
  BTM_TRACE_DEBUG("btm_get_max_packet_size");
  if (p != NULL) {
    pkt_types = p->pkt_types_mask;
  } else {
    /* Special case for when info for the local device is requested */
    if (addr == *controller_get_interface()->get_address()) {
      pkt_types = btm_cb.btm_acl_pkt_types_supported;
    }
  }

  if (pkt_types) {
    if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_3_DH5))
      pkt_size = HCI_EDR3_DH5_PACKET_SIZE;
    else if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_2_DH5))
      pkt_size = HCI_EDR2_DH5_PACKET_SIZE;
    else if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_3_DH3))
      pkt_size = HCI_EDR3_DH3_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DH5)
      pkt_size = HCI_DH5_PACKET_SIZE;
    else if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_2_DH3))
      pkt_size = HCI_EDR2_DH3_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DM5)
      pkt_size = HCI_DM5_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DH3)
      pkt_size = HCI_DH3_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DM3)
      pkt_size = HCI_DM3_PACKET_SIZE;
    else if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_3_DH1))
      pkt_size = HCI_EDR3_DH1_PACKET_SIZE;
    else if (!(pkt_types & BTM_ACL_PKT_TYPES_MASK_NO_2_DH1))
      pkt_size = HCI_EDR2_DH1_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DH1)
      pkt_size = HCI_DH1_PACKET_SIZE;
    else if (pkt_types & BTM_ACL_PKT_TYPES_MASK_DM1)
      pkt_size = HCI_DM1_PACKET_SIZE;
  }

  return (pkt_size);
}

/*******************************************************************************
 *
 * Function         BTM_ReadRemoteVersion
 *
 * Returns          If connected report peer device info
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadRemoteVersion(const RawAddress& addr, uint8_t* lmp_version,
                                  uint16_t* manufacturer,
                                  uint16_t* lmp_sub_version) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  BTM_TRACE_DEBUG("BTM_ReadRemoteVersion");
  if (p == NULL) return (BTM_UNKNOWN_ADDR);

  if (lmp_version) *lmp_version = p->lmp_version;

  if (manufacturer) *manufacturer = p->manufacturer;

  if (lmp_sub_version) *lmp_sub_version = p->lmp_subversion;

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_ReadRemoteFeatures
 *
 * Returns          pointer to the remote supported features mask (8 bytes)
 *
 ******************************************************************************/
uint8_t* BTM_ReadRemoteFeatures(const RawAddress& addr) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  BTM_TRACE_DEBUG("BTM_ReadRemoteFeatures");
  if (p == NULL) {
    return (NULL);
  }

  return (p->peer_lmp_feature_pages[0]);
}

/*******************************************************************************
 *
 * Function         BTM_ReadRemoteExtendedFeatures
 *
 * Returns          pointer to the remote extended features mask (8 bytes)
 *                  or NULL if bad page
 *
 ******************************************************************************/
uint8_t* BTM_ReadRemoteExtendedFeatures(const RawAddress& addr,
                                        uint8_t page_number) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  BTM_TRACE_DEBUG("BTM_ReadRemoteExtendedFeatures");
  if (p == NULL) {
    return (NULL);
  }

  if (page_number > HCI_EXT_FEATURES_PAGE_MAX) {
    BTM_TRACE_ERROR("Warning: BTM_ReadRemoteExtendedFeatures page %d unknown",
                    page_number);
    return NULL;
  }

  return (p->peer_lmp_feature_pages[page_number]);
}

/*******************************************************************************
 *
 * Function         BTM_ReadNumberRemoteFeaturesPages
 *
 * Returns          number of features pages read from the remote device.
 *
 ******************************************************************************/
uint8_t BTM_ReadNumberRemoteFeaturesPages(const RawAddress& addr) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  BTM_TRACE_DEBUG("BTM_ReadNumberRemoteFeaturesPages");
  if (p == NULL) {
    return (0);
  }

  return (p->num_read_pages);
}

/*******************************************************************************
 *
 * Function         BTM_ReadAllRemoteFeatures
 *
 * Returns          pointer to all features of the remote (24 bytes).
 *
 ******************************************************************************/
uint8_t* BTM_ReadAllRemoteFeatures(const RawAddress& addr) {
  tACL_CONN* p = btm_bda_to_acl(addr, BT_TRANSPORT_BR_EDR);
  BTM_TRACE_DEBUG("BTM_ReadAllRemoteFeatures");
  if (p == NULL) {
    return (NULL);
  }

  return (p->peer_lmp_feature_pages[0]);
}

/*******************************************************************************
 *
 * Function         BTM_RegBusyLevelNotif
 *
 * Description      This function is called to register a callback to receive
 *                  busy level change events.
 *
 * Returns          BTM_SUCCESS if successfully registered, otherwise error
 *
 ******************************************************************************/
tBTM_STATUS BTM_RegBusyLevelNotif(tBTM_BL_CHANGE_CB* p_cb, uint8_t* p_level,
                                  tBTM_BL_EVENT_MASK evt_mask) {
  BTM_TRACE_DEBUG("BTM_RegBusyLevelNotif");
  if (p_level) *p_level = btm_cb.busy_level;

  btm_cb.bl_evt_mask = evt_mask;

  if (!p_cb)
    btm_cb.p_bl_changed_cb = NULL;
  else if (btm_cb.p_bl_changed_cb)
    return (BTM_BUSY);
  else
    btm_cb.p_bl_changed_cb = p_cb;

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_SetQoS
 *
 * Description      This function is called to setup QoS
 *
 * Returns          status of the operation
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetQoS(const RawAddress& bd, FLOW_SPEC* p_flow,
                       tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p = &btm_cb.acl_db[0];

  VLOG(2) << __func__ << " BdAddr: " << bd;

  /* If someone already waiting on the version, do not allow another */
  if (btm_cb.devcb.p_qos_setup_cmpl_cb) return (BTM_BUSY);

  p = btm_bda_to_acl(bd, BT_TRANSPORT_BR_EDR);
  if (p != NULL) {
    btm_cb.devcb.p_qos_setup_cmpl_cb = p_cb;
    alarm_set_on_mloop(btm_cb.devcb.qos_setup_timer, BTM_DEV_REPLY_TIMEOUT_MS,
                       btm_qos_setup_timeout, NULL);

    btsnd_hcic_qos_setup(p->hci_handle, p_flow->qos_flags, p_flow->service_type,
                         p_flow->token_rate, p_flow->peak_bandwidth,
                         p_flow->latency, p_flow->delay_variation);
    return (BTM_CMD_STARTED);
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         btm_qos_setup_timeout
 *
 * Description      Callback when QoS setup times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_qos_setup_timeout(UNUSED_ATTR void* data) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_qos_setup_cmpl_cb;
  btm_cb.devcb.p_qos_setup_cmpl_cb = NULL;
  if (p_cb) (*p_cb)((void*)NULL);
}

/*******************************************************************************
 *
 * Function         btm_qos_setup_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the qos setup request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_qos_setup_complete(uint8_t status, uint16_t handle,
                            FLOW_SPEC* p_flow) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_qos_setup_cmpl_cb;
  tBTM_QOS_SETUP_CMPL qossu;

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.qos_setup_timer);
  btm_cb.devcb.p_qos_setup_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    memset(&qossu, 0, sizeof(tBTM_QOS_SETUP_CMPL));
    qossu.status = status;
    qossu.handle = handle;
    if (p_flow != NULL) {
      qossu.flow.qos_flags = p_flow->qos_flags;
      qossu.flow.service_type = p_flow->service_type;
      qossu.flow.token_rate = p_flow->token_rate;
      qossu.flow.peak_bandwidth = p_flow->peak_bandwidth;
      qossu.flow.latency = p_flow->latency;
      qossu.flow.delay_variation = p_flow->delay_variation;
    }
    BTM_TRACE_DEBUG("BTM: p_flow->delay_variation: 0x%02x",
                    qossu.flow.delay_variation);
    (*p_cb)(&qossu);
  }
}

/*******************************************************************************
 *
 * Function         BTM_ReadRSSI
 *
 * Description      This function is called to read the link policy settings.
 *                  The address of link policy results are returned in the
 *                  callback.
 *                  (tBTM_RSSI_RESULT)
 *
 * Returns          BTM_CMD_STARTED if successfully initiated or error code
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadRSSI(const RawAddress& remote_bda, tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p = NULL;
  tBT_DEVICE_TYPE dev_type;
  tBLE_ADDR_TYPE addr_type;

  /* If someone already waiting on the version, do not allow another */
  if (btm_cb.devcb.p_rssi_cmpl_cb) return (BTM_BUSY);

  BTM_ReadDevInfo(remote_bda, &dev_type, &addr_type);

  if (dev_type & BT_DEVICE_TYPE_BLE) {
    p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_LE);
  }

  if (p == NULL && dev_type & BT_DEVICE_TYPE_BREDR) {
    p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_BR_EDR);
  }

  if (p) {
    btm_cb.devcb.p_rssi_cmpl_cb = p_cb;
    alarm_set_on_mloop(btm_cb.devcb.read_rssi_timer, BTM_DEV_REPLY_TIMEOUT_MS,
                       btm_read_rssi_timeout, NULL);

    btsnd_hcic_read_rssi(p->hci_handle);
    return (BTM_CMD_STARTED);
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_ReadFailedContactCounter
 *
 * Description      This function is called to read the failed contact counter.
 *                  The result is returned in the callback.
 *                  (tBTM_FAILED_CONTACT_COUNTER_RESULT)
 *
 * Returns          BTM_CMD_STARTED if successfully initiated or error code
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadFailedContactCounter(const RawAddress& remote_bda,
                                         tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p;
  tBT_TRANSPORT transport = BT_TRANSPORT_BR_EDR;
  tBT_DEVICE_TYPE dev_type;
  tBLE_ADDR_TYPE addr_type;

  /* If someone already waiting on the result, do not allow another */
  if (btm_cb.devcb.p_failed_contact_counter_cmpl_cb) return (BTM_BUSY);

  BTM_ReadDevInfo(remote_bda, &dev_type, &addr_type);
  if (dev_type == BT_DEVICE_TYPE_BLE) transport = BT_TRANSPORT_LE;

  p = btm_bda_to_acl(remote_bda, transport);
  if (p != (tACL_CONN*)NULL) {
    btm_cb.devcb.p_failed_contact_counter_cmpl_cb = p_cb;
    alarm_set_on_mloop(btm_cb.devcb.read_failed_contact_counter_timer,
                       BTM_DEV_REPLY_TIMEOUT_MS,
                       btm_read_failed_contact_counter_timeout, NULL);

    btsnd_hcic_read_failed_contact_counter(p->hci_handle);
    return (BTM_CMD_STARTED);
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_ReadAutomaticFlushTimeout
 *
 * Description      This function is called to read the automatic flush timeout.
 *                  The result is returned in the callback.
 *                  (tBTM_AUTOMATIC_FLUSH_TIMEOUT_RESULT)
 *
 * Returns          BTM_CMD_STARTED if successfully initiated or error code
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadAutomaticFlushTimeout(const RawAddress& remote_bda,
                                          tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p;
  tBT_TRANSPORT transport = BT_TRANSPORT_BR_EDR;
  tBT_DEVICE_TYPE dev_type;
  tBLE_ADDR_TYPE addr_type;

  /* If someone already waiting on the result, do not allow another */
  if (btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb) return (BTM_BUSY);

  BTM_ReadDevInfo(remote_bda, &dev_type, &addr_type);
  if (dev_type == BT_DEVICE_TYPE_BLE) transport = BT_TRANSPORT_LE;

  p = btm_bda_to_acl(remote_bda, transport);
  if (!p) return BTM_UNKNOWN_ADDR;

  btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb = p_cb;
  alarm_set_on_mloop(btm_cb.devcb.read_automatic_flush_timeout_timer,
                     BTM_DEV_REPLY_TIMEOUT_MS,
                     btm_read_automatic_flush_timeout_timeout, nullptr);

  btsnd_hcic_read_automatic_flush_timeout(p->hci_handle);
  return BTM_CMD_STARTED;
}

/*******************************************************************************
 *
 * Function         BTM_ReadLinkQuality
 *
 * Description      This function is called to read the link qulaity.
 *                  The value of the link quality is returned in the callback.
 *                  (tBTM_LINK_QUALITY_RESULT)
 *
 * Returns          BTM_CMD_STARTED if successfully initiated or error code
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadLinkQuality(const RawAddress& remote_bda,
                                tBTM_CMPL_CB* p_cb) {
  VLOG(2) << __func__ << ": RemBdAddr: " << remote_bda;

  /* If someone already waiting on the version, do not allow another */
  if (btm_cb.devcb.p_link_qual_cmpl_cb) return (BTM_BUSY);

  tACL_CONN* p = btm_bda_to_acl(remote_bda, BT_TRANSPORT_BR_EDR);
  if (p != (tACL_CONN*)NULL) {
    btm_cb.devcb.p_link_qual_cmpl_cb = p_cb;
    alarm_set_on_mloop(btm_cb.devcb.read_link_quality_timer,
                       BTM_DEV_REPLY_TIMEOUT_MS, btm_read_link_quality_timeout,
                       NULL);

    btsnd_hcic_get_link_quality(p->hci_handle);
    return (BTM_CMD_STARTED);
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_ReadTxPower
 *
 * Description      This function is called to read the current
 *                  TX power of the connection. The tx power level results
 *                  are returned in the callback.
 *                  (tBTM_RSSI_RESULT)
 *
 * Returns          BTM_CMD_STARTED if successfully initiated or error code
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadTxPower(const RawAddress& remote_bda,
                            tBT_TRANSPORT transport, tBTM_CMPL_CB* p_cb) {
  tACL_CONN* p;
#define BTM_READ_RSSI_TYPE_CUR 0x00
#define BTM_READ_RSSI_TYPE_MAX 0X01

  VLOG(2) << __func__ << ": RemBdAddr: " << remote_bda;

  /* If someone already waiting on the version, do not allow another */
  if (btm_cb.devcb.p_tx_power_cmpl_cb) return (BTM_BUSY);

  p = btm_bda_to_acl(remote_bda, transport);
  if (p != (tACL_CONN*)NULL) {
    btm_cb.devcb.p_tx_power_cmpl_cb = p_cb;
    alarm_set_on_mloop(btm_cb.devcb.read_tx_power_timer,
                       BTM_DEV_REPLY_TIMEOUT_MS, btm_read_tx_power_timeout,
                       NULL);

    if (p->transport == BT_TRANSPORT_LE) {
      btm_cb.devcb.read_tx_pwr_addr = remote_bda;
      btsnd_hcic_ble_read_adv_chnl_tx_power();
    } else {
      btsnd_hcic_read_tx_power(p->hci_handle, BTM_READ_RSSI_TYPE_CUR);
    }

    return (BTM_CMD_STARTED);
  }

  /* If here, no BD Addr found */
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         btm_read_tx_power_timeout
 *
 * Description      Callback when reading the tx power times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_tx_power_timeout(UNUSED_ATTR void* data) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_tx_power_cmpl_cb;
  btm_cb.devcb.p_tx_power_cmpl_cb = NULL;
  if (p_cb) (*p_cb)((void*)NULL);
}

/*******************************************************************************
 *
 * Function         btm_read_tx_power_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read tx power request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_tx_power_complete(uint8_t* p, bool is_ble) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_tx_power_cmpl_cb;
  tBTM_TX_POWER_RESULT result;
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_tx_power_timer);
  btm_cb.devcb.p_tx_power_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      result.status = BTM_SUCCESS;

      if (!is_ble) {
        uint16_t handle;
        STREAM_TO_UINT16(handle, p);
        STREAM_TO_UINT8(result.tx_power, p);

        /* Search through the list of active channels for the correct BD Addr */
        for (uint16_t index = 0; index < MAX_L2CAP_LINKS; index++, p_acl_cb++) {
          if ((p_acl_cb->in_use) && (handle == p_acl_cb->hci_handle)) {
            result.rem_bda = p_acl_cb->remote_addr;
            break;
          }
        }
      } else {
        STREAM_TO_UINT8(result.tx_power, p);
        result.rem_bda = btm_cb.devcb.read_tx_pwr_addr;
      }
      BTM_TRACE_DEBUG("BTM TX power Complete: tx_power %d, hci status 0x%02x",
                      result.tx_power, result.hci_status);
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_rssi_timeout
 *
 * Description      Callback when reading the RSSI times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_rssi_timeout(UNUSED_ATTR void* data) {
  tBTM_RSSI_RESULT result;
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_rssi_cmpl_cb;
  btm_cb.devcb.p_rssi_cmpl_cb = NULL;
  result.status = BTM_DEVICE_TIMEOUT;
  if (p_cb) (*p_cb)(&result);
}

/*******************************************************************************
 *
 * Function         btm_read_rssi_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read rssi request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_rssi_complete(uint8_t* p) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_rssi_cmpl_cb;
  tBTM_RSSI_RESULT result;
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_rssi_timer);
  btm_cb.devcb.p_rssi_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      uint16_t handle;
      result.status = BTM_SUCCESS;

      STREAM_TO_UINT16(handle, p);

      STREAM_TO_UINT8(result.rssi, p);
      BTM_TRACE_DEBUG("BTM RSSI Complete: rssi %d, hci status 0x%02x",
                      result.rssi, result.hci_status);

      /* Search through the list of active channels for the correct BD Addr */
      for (uint16_t index = 0; index < MAX_L2CAP_LINKS; index++, p_acl_cb++) {
        if ((p_acl_cb->in_use) && (handle == p_acl_cb->hci_handle)) {
          result.rem_bda = p_acl_cb->remote_addr;
          break;
        }
      }
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_failed_contact_counter_timeout
 *
 * Description      Callback when reading the failed contact counter times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_failed_contact_counter_timeout(UNUSED_ATTR void* data) {
  tBTM_FAILED_CONTACT_COUNTER_RESULT result;
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_failed_contact_counter_cmpl_cb;
  btm_cb.devcb.p_failed_contact_counter_cmpl_cb = NULL;
  result.status = BTM_DEVICE_TIMEOUT;
  if (p_cb) (*p_cb)(&result);
}

/*******************************************************************************
 *
 * Function         btm_read_failed_contact_counter_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read failed contact
 *                  counter request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_failed_contact_counter_complete(uint8_t* p) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_failed_contact_counter_cmpl_cb;
  tBTM_FAILED_CONTACT_COUNTER_RESULT result;
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_failed_contact_counter_timer);
  btm_cb.devcb.p_failed_contact_counter_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    uint16_t handle;
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      result.status = BTM_SUCCESS;

      STREAM_TO_UINT16(handle, p);

      STREAM_TO_UINT16(result.failed_contact_counter, p);
      BTM_TRACE_DEBUG(
          "BTM Failed Contact Counter Complete: counter %u, hci status 0x%02x",
          result.failed_contact_counter, result.hci_status);

      /* Search through the list of active channels for the correct BD Addr */
      for (uint16_t index = 0; index < MAX_L2CAP_LINKS; index++, p_acl_cb++) {
        if ((p_acl_cb->in_use) && (handle == p_acl_cb->hci_handle)) {
          result.rem_bda = p_acl_cb->remote_addr;
          break;
        }
      }
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_automatic_flush_timeout_timeout
 *
 * Description      Callback when reading the automatic flush timeout times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_automatic_flush_timeout_timeout(UNUSED_ATTR void* data) {
  tBTM_AUTOMATIC_FLUSH_TIMEOUT_RESULT result;
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb;
  btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb = nullptr;
  result.status = BTM_DEVICE_TIMEOUT;
  if (p_cb) (*p_cb)(&result);
}

/*******************************************************************************
 *
 * Function         btm_read_automatic_flush_timeout_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read automatic flush
 *                  timeout request.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_automatic_flush_timeout_complete(uint8_t* p) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb;
  tBTM_AUTOMATIC_FLUSH_TIMEOUT_RESULT result;
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_automatic_flush_timeout_timer);
  btm_cb.devcb.p_automatic_flush_timeout_cmpl_cb = nullptr;

  /* If there was a registered callback, call it */
  if (p_cb) {
    uint16_t handle;
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      result.status = BTM_SUCCESS;

      STREAM_TO_UINT16(handle, p);

      STREAM_TO_UINT16(result.automatic_flush_timeout, p);
      BTM_TRACE_DEBUG(
          "BTM Automatic Flush Timeout Complete: timeout %u, hci status 0x%02x",
          result.automatic_flush_timeout, result.hci_status);

      /* Search through the list of active channels for the correct BD Addr */
      for (uint16_t index = 0; index < MAX_L2CAP_LINKS; index++, p_acl_cb++) {
        if ((p_acl_cb->in_use) && (handle == p_acl_cb->hci_handle)) {
          result.rem_bda = p_acl_cb->remote_addr;
          break;
        }
      }
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}

/*******************************************************************************
 *
 * Function         btm_read_link_quality_timeout
 *
 * Description      Callback when reading the link quality times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_link_quality_timeout(UNUSED_ATTR void* data) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_link_qual_cmpl_cb;
  btm_cb.devcb.p_link_qual_cmpl_cb = NULL;
  if (p_cb) (*p_cb)((void*)NULL);
}

/*******************************************************************************
 *
 * Function         btm_read_link_quality_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read link quality.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_link_quality_complete(uint8_t* p) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_link_qual_cmpl_cb;
  tBTM_LINK_QUALITY_RESULT result;
  tACL_CONN* p_acl_cb = &btm_cb.acl_db[0];

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_link_quality_timer);
  btm_cb.devcb.p_link_qual_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      uint16_t handle;
      result.status = BTM_SUCCESS;

      STREAM_TO_UINT16(handle, p);

      STREAM_TO_UINT8(result.link_quality, p);
      BTM_TRACE_DEBUG(
          "BTM Link Quality Complete: Link Quality %d, hci status 0x%02x",
          result.link_quality, result.hci_status);

      /* Search through the list of active channels for the correct BD Addr */
      for (uint16_t index = 0; index < MAX_L2CAP_LINKS; index++, p_acl_cb++) {
        if ((p_acl_cb->in_use) && (handle == p_acl_cb->hci_handle)) {
          result.rem_bda = p_acl_cb->remote_addr;
          break;
        }
      }
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}

/*******************************************************************************
 *
 * Function         btm_remove_acl
 *
 * Description      This function is called to disconnect an ACL connection
 *
 * Returns          BTM_SUCCESS if successfully initiated, otherwise
 *                  BTM_NO_RESOURCES.
 *
 ******************************************************************************/
tBTM_STATUS btm_remove_acl(const RawAddress& bd_addr, tBT_TRANSPORT transport) {
  uint16_t hci_handle = BTM_GetHCIConnHandle(bd_addr, transport);
  tBTM_STATUS status = BTM_SUCCESS;

  BTM_TRACE_DEBUG("btm_remove_acl");
#if (BTM_DISC_DURING_RS == TRUE)
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

  /* Role Switch is pending, postpone until completed */
  if (p_dev_rec && (p_dev_rec->rs_disc_pending == BTM_SEC_RS_PENDING)) {
    p_dev_rec->rs_disc_pending = BTM_SEC_DISC_PENDING;
  } else /* otherwise can disconnect right away */
#endif
  {
    if (hci_handle != 0xFFFF && p_dev_rec &&
        p_dev_rec->sec_state != BTM_SEC_STATE_DISCONNECTING) {
      btsnd_hcic_disconnect(hci_handle, HCI_ERR_PEER_USER);
    } else {
      status = BTM_UNKNOWN_ADDR;
    }
  }

  return status;
}

/*******************************************************************************
 *
 * Function         BTM_SetTraceLevel
 *
 * Description      This function sets the trace level for BTM.  If called with
 *                  a value of 0xFF, it simply returns the current trace level.
 *
 * Returns          The new or current trace level
 *
 ******************************************************************************/
uint8_t BTM_SetTraceLevel(uint8_t new_level) {
  BTM_TRACE_DEBUG("BTM_SetTraceLevel");
  if (new_level != 0xFF) btm_cb.trace_level = new_level;

  return (btm_cb.trace_level);
}

/*******************************************************************************
 *
 * Function         btm_cont_rswitch
 *
 * Description      This function is called to continue processing an active
 *                  role switch. It first disables encryption if enabled and
 *                  EPR is not supported
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_cont_rswitch(tACL_CONN* p, tBTM_SEC_DEV_REC* p_dev_rec,
                      uint8_t hci_status) {
  BTM_TRACE_DEBUG("btm_cont_rswitch");
  /* Check to see if encryption needs to be turned off if pending
     change of link key or role switch */
  if (p->switch_role_state == BTM_ACL_SWKEY_STATE_MODE_CHANGE) {
    /* Must turn off Encryption first if necessary */
    /* Some devices do not support switch or change of link key while encryption
     * is on */
    if (p_dev_rec != NULL &&
        ((p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) != 0) &&
        !BTM_EPR_AVAILABLE(p)) {
      btsnd_hcic_set_conn_encrypt(p->hci_handle, false);
      p->encrypt_state = BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF;
      if (p->switch_role_state == BTM_ACL_SWKEY_STATE_MODE_CHANGE)
        p->switch_role_state = BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF;
    } else /* Encryption not used or EPR supported, continue with switch
              and/or change of link key */
    {
      if (p->switch_role_state == BTM_ACL_SWKEY_STATE_MODE_CHANGE) {
        p->switch_role_state = BTM_ACL_SWKEY_STATE_IN_PROGRESS;
#if (BTM_DISC_DURING_RS == TRUE)
        if (p_dev_rec) p_dev_rec->rs_disc_pending = BTM_SEC_RS_PENDING;
#endif
        btsnd_hcic_switch_role(p->remote_addr, (uint8_t)!p->link_role);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_resubmit_page
 *
 * Description      send pending page request
 *
 ******************************************************************************/
void btm_acl_resubmit_page(void) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  BT_HDR* p_buf;
  uint8_t* pp;
  BTM_TRACE_DEBUG("btm_acl_resubmit_page");
  /* If there were other page request schedule can start the next one */
  p_buf = (BT_HDR*)fixed_queue_try_dequeue(btm_cb.page_queue);
  if (p_buf != NULL) {
    /* skip 3 (2 bytes opcode and 1 byte len) to get to the bd_addr
     * for both create_conn and rmt_name */
    pp = (uint8_t*)(p_buf + 1) + p_buf->offset + 3;

    RawAddress bda;
    STREAM_TO_BDADDR(bda, pp);

    p_dev_rec = btm_find_or_alloc_dev(bda);

    btm_cb.connecting_bda = p_dev_rec->bd_addr;
    memcpy(btm_cb.connecting_dc, p_dev_rec->dev_class, DEV_CLASS_LEN);

    btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p_buf);
  } else {
    btm_cb.paging = false;
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_reset_paging
 *
 * Description      set paging to false and free the page queue - called at
 *                  hci_reset
 *
 ******************************************************************************/
void btm_acl_reset_paging(void) {
  BT_HDR* p;
  BTM_TRACE_DEBUG("btm_acl_reset_paging");
  /* If we sent reset we are definitely not paging any more */
  while ((p = (BT_HDR*)fixed_queue_try_dequeue(btm_cb.page_queue)) != NULL)
    osi_free(p);

  btm_cb.paging = false;
}

/*******************************************************************************
 *
 * Function         btm_acl_paging
 *
 * Description      send a paging command or queue it in btm_cb
 *
 ******************************************************************************/
void btm_acl_paging(BT_HDR* p, const RawAddress& bda) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  VLOG(2) << __func__ << ":" << btm_cb.discing << " , paging:" << btm_cb.paging
          << " BDA: " << bda;

  if (btm_cb.discing) {
    btm_cb.paging = true;
    fixed_queue_enqueue(btm_cb.page_queue, p);
  } else {
    if (!BTM_ACL_IS_CONNECTED(bda)) {
      VLOG(1) << "connecting_bda: " << btm_cb.connecting_bda;
      if (btm_cb.paging && bda != btm_cb.connecting_bda) {
        fixed_queue_enqueue(btm_cb.page_queue, p);
      } else {
        p_dev_rec = btm_find_or_alloc_dev(bda);
        btm_cb.connecting_bda = p_dev_rec->bd_addr;
        memcpy(btm_cb.connecting_dc, p_dev_rec->dev_class, DEV_CLASS_LEN);

        btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
      }

      btm_cb.paging = true;
    } else /* ACL is already up */
    {
      btu_hcif_send_cmd(LOCAL_BR_EDR_CONTROLLER_ID, p);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_notif_conn_collision
 *
 * Description      Send connection collision event to upper layer if registered
 *
 * Returns          true if sent out to upper layer,
 *                  false if no one needs the notification.
 *
 ******************************************************************************/
bool btm_acl_notif_conn_collision(const RawAddress& bda) {
  /* Report possible collision to the upper layer. */
  if (btm_cb.p_bl_changed_cb) {
    VLOG(1) << __func__ << " RemBdAddr: " << bda;

    tBTM_BL_EVENT_DATA evt_data;
    evt_data.event = BTM_BL_COLLISION_EVT;
    evt_data.conn.p_bda = &bda;
    evt_data.conn.transport = BT_TRANSPORT_BR_EDR;
    evt_data.conn.handle = BTM_INVALID_HCI_HANDLE;
    (*btm_cb.p_bl_changed_cb)(&evt_data);
    return true;
  } else {
    return false;
  }
}

/*******************************************************************************
 *
 * Function         btm_acl_chk_peer_pkt_type_support
 *
 * Description      Check if peer supports requested packets
 *
 ******************************************************************************/
void btm_acl_chk_peer_pkt_type_support(tACL_CONN* p, uint16_t* p_pkt_type) {
  /* 3 and 5 slot packets? */
  if (!HCI_3_SLOT_PACKETS_SUPPORTED(p->peer_lmp_feature_pages[0]))
    *p_pkt_type &= ~(BTM_ACL_PKT_TYPES_MASK_DH3 + BTM_ACL_PKT_TYPES_MASK_DM3);

  if (!HCI_5_SLOT_PACKETS_SUPPORTED(p->peer_lmp_feature_pages[0]))
    *p_pkt_type &= ~(BTM_ACL_PKT_TYPES_MASK_DH5 + BTM_ACL_PKT_TYPES_MASK_DM5);

  /* 2 and 3 MPS support? */
  if (!HCI_EDR_ACL_2MPS_SUPPORTED(p->peer_lmp_feature_pages[0]))
    /* Not supported. Add 'not_supported' mask for all 2MPS packet types */
    *p_pkt_type |=
        (BTM_ACL_PKT_TYPES_MASK_NO_2_DH1 + BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 +
         BTM_ACL_PKT_TYPES_MASK_NO_2_DH5);

  if (!HCI_EDR_ACL_3MPS_SUPPORTED(p->peer_lmp_feature_pages[0]))
    /* Not supported. Add 'not_supported' mask for all 3MPS packet types */
    *p_pkt_type |=
        (BTM_ACL_PKT_TYPES_MASK_NO_3_DH1 + BTM_ACL_PKT_TYPES_MASK_NO_3_DH3 +
         BTM_ACL_PKT_TYPES_MASK_NO_3_DH5);

  /* EDR 3 and 5 slot support? */
  if (HCI_EDR_ACL_2MPS_SUPPORTED(p->peer_lmp_feature_pages[0]) ||
      HCI_EDR_ACL_3MPS_SUPPORTED(p->peer_lmp_feature_pages[0])) {
    if (!HCI_3_SLOT_EDR_ACL_SUPPORTED(p->peer_lmp_feature_pages[0]))
      /* Not supported. Add 'not_supported' mask for all 3-slot EDR packet types
       */
      *p_pkt_type |=
          (BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 + BTM_ACL_PKT_TYPES_MASK_NO_3_DH3);

    if (!HCI_5_SLOT_EDR_ACL_SUPPORTED(p->peer_lmp_feature_pages[0]))
      /* Not supported. Add 'not_supported' mask for all 5-slot EDR packet types
       */
      *p_pkt_type |=
          (BTM_ACL_PKT_TYPES_MASK_NO_2_DH5 + BTM_ACL_PKT_TYPES_MASK_NO_3_DH5);
  }
}
