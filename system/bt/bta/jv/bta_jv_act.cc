/******************************************************************************
 *
 *  Copyright 2006-2012 Broadcom Corporation
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
 *  This file contains action functions for BTA JV APIs.
 *
 ******************************************************************************/
#include <arpa/inet.h>
#include <bluetooth/uuid.h>
#include <hardware/bluetooth.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "avct_api.h"
#include "avdt_api.h"
#include "bt_common.h"
#include "bt_types.h"
#include "bta_api.h"
#include "bta_jv_api.h"
#include "bta_jv_co.h"
#include "bta_jv_int.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "btm_int.h"
#include "device/include/controller.h"
#include "gap_api.h"
#include "l2c_api.h"
#include "osi/include/allocator.h"
#include "port_api.h"
#include "rfcdefs.h"
#include "sdp_api.h"
#include "stack/l2cap/l2c_int.h"
#include "utl.h"

#include "osi/include/osi.h"

using bluetooth::Uuid;

tBTA_JV_CB bta_jv_cb;

/* one of these exists for each client */
struct fc_client {
  struct fc_client* next_all_list;
  struct fc_client* next_chan_list;
  RawAddress remote_addr;
  uint32_t id;
  tBTA_JV_L2CAP_CBACK* p_cback;
  uint32_t l2cap_socket_id;
  uint16_t handle;
  uint16_t chan;
  uint8_t sec_id;
  unsigned server : 1;
  unsigned init_called : 1;
};

/* one of these exists for each channel we're dealing with */
struct fc_channel {
  struct fc_channel* next;
  struct fc_client* clients;
  uint8_t has_server : 1;
  uint16_t chan;
};

static struct fc_client* fc_clients;
static struct fc_channel* fc_channels;
static uint32_t fc_next_id;

static void fcchan_conn_chng_cbk(uint16_t chan, const RawAddress& bd_addr,
                                 bool connected, uint16_t reason,
                                 tBT_TRANSPORT);
static void fcchan_data_cbk(uint16_t chan, const RawAddress& bd_addr,
                            BT_HDR* p_buf);

static tBTA_JV_PCB* bta_jv_add_rfc_port(tBTA_JV_RFC_CB* p_cb,
                                        tBTA_JV_PCB* p_pcb_open);
static tBTA_JV_STATUS bta_jv_free_set_pm_profile_cb(uint32_t jv_handle);
static void bta_jv_pm_conn_busy(tBTA_JV_PM_CB* p_cb);
static void bta_jv_pm_conn_idle(tBTA_JV_PM_CB* p_cb);
static void bta_jv_pm_state_change(tBTA_JV_PM_CB* p_cb,
                                   const tBTA_JV_CONN_STATE state);

/*******************************************************************************
 *
 * Function     bta_jv_alloc_sec_id
 *
 * Description  allocate a security id
 *
 * Returns
 *
 ******************************************************************************/
uint8_t bta_jv_alloc_sec_id(void) {
  uint8_t ret = 0;
  int i;
  for (i = 0; i < BTA_JV_NUM_SERVICE_ID; i++) {
    if (0 == bta_jv_cb.sec_id[i]) {
      bta_jv_cb.sec_id[i] = BTA_JV_FIRST_SERVICE_ID + i;
      ret = bta_jv_cb.sec_id[i];
      break;
    }
  }
  return ret;
}
static int get_sec_id_used(void) {
  int i;
  int used = 0;
  for (i = 0; i < BTA_JV_NUM_SERVICE_ID; i++) {
    if (bta_jv_cb.sec_id[i]) used++;
  }
  if (used == BTA_JV_NUM_SERVICE_ID)
    LOG(ERROR) << __func__
               << ": sec id exceeds the limit=" << BTA_JV_NUM_SERVICE_ID;
  return used;
}
static int get_rfc_cb_used(void) {
  int i;
  int used = 0;
  for (i = 0; i < BTA_JV_MAX_RFC_CONN; i++) {
    if (bta_jv_cb.rfc_cb[i].handle) used++;
  }
  if (used == BTA_JV_MAX_RFC_CONN)
    LOG(ERROR) << __func__
               << ": rfc ctrl block exceeds the limit=" << BTA_JV_MAX_RFC_CONN;
  return used;
}

/*******************************************************************************
 *
 * Function     bta_jv_free_sec_id
 *
 * Description  free the given security id
 *
 * Returns
 *
 ******************************************************************************/
static void bta_jv_free_sec_id(uint8_t* p_sec_id) {
  uint8_t sec_id = *p_sec_id;
  *p_sec_id = 0;
  if (sec_id >= BTA_JV_FIRST_SERVICE_ID && sec_id <= BTA_JV_LAST_SERVICE_ID) {
    BTM_SecClrService(sec_id);
    bta_jv_cb.sec_id[sec_id - BTA_JV_FIRST_SERVICE_ID] = 0;
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_alloc_rfc_cb
 *
 * Description  allocate a control block for the given port handle
 *
 * Returns
 *
 ******************************************************************************/
tBTA_JV_RFC_CB* bta_jv_alloc_rfc_cb(uint16_t port_handle,
                                    tBTA_JV_PCB** pp_pcb) {
  tBTA_JV_RFC_CB* p_cb = NULL;
  tBTA_JV_PCB* p_pcb;
  int i, j;
  for (i = 0; i < BTA_JV_MAX_RFC_CONN; i++) {
    if (0 == bta_jv_cb.rfc_cb[i].handle) {
      p_cb = &bta_jv_cb.rfc_cb[i];
      /* mask handle to distinguish it with L2CAP handle */
      p_cb->handle = (i + 1) | BTA_JV_RFCOMM_MASK;

      p_cb->max_sess = 1;
      p_cb->curr_sess = 1;
      for (j = 0; j < BTA_JV_MAX_RFC_SR_SESSION; j++) p_cb->rfc_hdl[j] = 0;
      p_cb->rfc_hdl[0] = port_handle;
      VLOG(2) << __func__ << "port_handle=" << +port_handle
              << ", handle=" << loghex(p_cb->handle);

      p_pcb = &bta_jv_cb.port_cb[port_handle - 1];
      p_pcb->handle = p_cb->handle;
      p_pcb->port_handle = port_handle;
      p_pcb->p_pm_cb = NULL;
      *pp_pcb = p_pcb;
      break;
    }
  }
  if (p_cb == NULL) {
    LOG(ERROR) << __func__ << "port_handle=" << port_handle << " ctrl block exceeds limit:" << BTA_JV_MAX_RFC_CONN;
  }
  return p_cb;
}

/*******************************************************************************
 *
 * Function     bta_jv_rfc_port_to_pcb
 *
 * Description  find the port control block associated with the given port
 *              handle
 *
 * Returns
 *
 ******************************************************************************/
tBTA_JV_PCB* bta_jv_rfc_port_to_pcb(uint16_t port_handle) {
  tBTA_JV_PCB* p_pcb = NULL;

  if ((port_handle > 0) && (port_handle <= MAX_RFC_PORTS) &&
      bta_jv_cb.port_cb[port_handle - 1].handle) {
    p_pcb = &bta_jv_cb.port_cb[port_handle - 1];
  }

  return p_pcb;
}

/*******************************************************************************
 *
 * Function     bta_jv_rfc_port_to_cb
 *
 * Description  find the RFCOMM control block associated with the given port
 *              handle
 *
 * Returns
 *
 ******************************************************************************/
tBTA_JV_RFC_CB* bta_jv_rfc_port_to_cb(uint16_t port_handle) {
  tBTA_JV_RFC_CB* p_cb = NULL;
  uint32_t handle;

  if ((port_handle > 0) && (port_handle <= MAX_RFC_PORTS) &&
      bta_jv_cb.port_cb[port_handle - 1].handle) {
    handle = bta_jv_cb.port_cb[port_handle - 1].handle;
    handle &= BTA_JV_RFC_HDL_MASK;
    handle &= ~BTA_JV_RFCOMM_MASK;
    if (handle) p_cb = &bta_jv_cb.rfc_cb[handle - 1];
  } else {
    LOG(WARNING) << __func__
                 << ": jv handle not found port_handle:" << port_handle;
  }
  return p_cb;
}

static tBTA_JV_STATUS bta_jv_free_rfc_cb(tBTA_JV_RFC_CB* p_cb,
                                         tBTA_JV_PCB* p_pcb) {
  tBTA_JV_STATUS status = BTA_JV_SUCCESS;
  bool remove_server = false;
  int close_pending = 0;

  if (!p_cb || !p_pcb) {
    LOG(ERROR) << __func__ << " p_cb or p_pcb cannot be null";
    return BTA_JV_FAILURE;
  }
  VLOG(2) << __func__ << ": max_sess=" << p_cb->max_sess
          << ", curr_sess=" << p_cb->curr_sess << ", p_pcb=" << p_pcb
          << ", user=" << p_pcb->rfcomm_slot_id << ", state=" << p_pcb->state
          << ", jv handle=" << loghex(p_pcb->handle);

  if (p_cb->curr_sess <= 0) return BTA_JV_SUCCESS;

  switch (p_pcb->state) {
    case BTA_JV_ST_CL_CLOSING:
    case BTA_JV_ST_SR_CLOSING:
      LOG(WARNING) << __func__
                   << ": return on closing, port state=" << p_pcb->state
                   << ", scn=" << p_cb->scn << ", p_pcb=" << p_pcb
                   << ", user_data=" << p_pcb->rfcomm_slot_id;
      status = BTA_JV_FAILURE;
      return status;
    case BTA_JV_ST_CL_OPEN:
    case BTA_JV_ST_CL_OPENING:
      VLOG(2) << __func__ << ": state=" << p_pcb->state << ", scn=" << p_cb->scn
              << ", user_data=" << p_pcb->rfcomm_slot_id;
      p_pcb->state = BTA_JV_ST_CL_CLOSING;
      break;
    case BTA_JV_ST_SR_LISTEN:
      p_pcb->state = BTA_JV_ST_SR_CLOSING;
      remove_server = true;
      VLOG(2) << __func__ << ": state: BTA_JV_ST_SR_LISTEN, scn=" << p_cb->scn
              << ", user_data=" << p_pcb->rfcomm_slot_id;
      break;
    case BTA_JV_ST_SR_OPEN:
      p_pcb->state = BTA_JV_ST_SR_CLOSING;
      VLOG(2) << ": state: BTA_JV_ST_SR_OPEN, scn=" << p_cb->scn
              << " user_data=" << p_pcb->rfcomm_slot_id;
      break;
    default:
      LOG(WARNING) << __func__ << ":failed, ignore port state= " << p_pcb->state
                   << ", scn=" << p_cb->scn << ", p_pcb= " << p_pcb
                   << ", jv handle=" << loghex(p_pcb->handle)
                   << ", port_handle=" << p_pcb->port_handle
                   << ", user_data=" << p_pcb->rfcomm_slot_id;
      status = BTA_JV_FAILURE;
      break;
  }
  if (BTA_JV_SUCCESS == status) {
    int port_status;

    if (!remove_server)
      port_status = RFCOMM_RemoveConnection(p_pcb->port_handle);
    else
      port_status = RFCOMM_RemoveServer(p_pcb->port_handle);
    if (port_status != PORT_SUCCESS) {
      status = BTA_JV_FAILURE;
      LOG(WARNING) << __func__ << ": Remove jv handle=" << loghex(p_pcb->handle)
                   << ", state=" << p_pcb->state
                   << ", port_status=" << port_status
                   << ", port_handle=" << p_pcb->port_handle
                   << ", close_pending=" << close_pending;
    }
  }
  if (!close_pending) {
    p_pcb->port_handle = 0;
    p_pcb->state = BTA_JV_ST_NONE;
    bta_jv_free_set_pm_profile_cb(p_pcb->handle);

    // Initialize congestion flags
    p_pcb->cong = false;
    p_pcb->rfcomm_slot_id = 0;
    int si = BTA_JV_RFC_HDL_TO_SIDX(p_pcb->handle);
    if (0 <= si && si < BTA_JV_MAX_RFC_SR_SESSION) p_cb->rfc_hdl[si] = 0;
    p_pcb->handle = 0;
    p_cb->curr_sess--;
    if (p_cb->curr_sess == 0) {
      p_cb->scn = 0;
      bta_jv_free_sec_id(&p_cb->sec_id);
      p_cb->p_cback = NULL;
      p_cb->handle = 0;
      p_cb->curr_sess = -1;
    }
    if (remove_server) {
      bta_jv_free_sec_id(&p_cb->sec_id);
    }
  }
  return status;
}

/*******************************************************************************
 *
 * Function     bta_jv_free_l2c_cb
 *
 * Description  free the given L2CAP control block
 *
 * Returns
 *
 ******************************************************************************/
tBTA_JV_STATUS bta_jv_free_l2c_cb(tBTA_JV_L2C_CB* p_cb) {
  tBTA_JV_STATUS status = BTA_JV_SUCCESS;

  if (BTA_JV_ST_NONE != p_cb->state) {
    bta_jv_free_set_pm_profile_cb((uint32_t)p_cb->handle);
    if (GAP_ConnClose(p_cb->handle) != BT_PASS) status = BTA_JV_FAILURE;
  }
  p_cb->psm = 0;
  p_cb->state = BTA_JV_ST_NONE;
  p_cb->cong = false;
  bta_jv_free_sec_id(&p_cb->sec_id);
  p_cb->p_cback = NULL;
  p_cb->handle = 0;
  p_cb->l2cap_socket_id = 0;
  return status;
}

/*******************************************************************************
 *
 *
 * Function    bta_jv_clear_pm_cb
 *
 * Description clears jv pm control block and optionally calls
 *             bta_sys_conn_close()
 *             In general close_conn should be set to true to remove registering
 *             with dm pm!
 *
 * WARNING:    Make sure to clear pointer form port or l2c to this control block
 *             too!
 *
 ******************************************************************************/
static void bta_jv_clear_pm_cb(tBTA_JV_PM_CB* p_pm_cb, bool close_conn) {
  /* needs to be called if registered with bta pm, otherwise we may run out of
   * dm pm slots! */
  if (close_conn)
    bta_sys_conn_close(BTA_ID_JV, p_pm_cb->app_id, p_pm_cb->peer_bd_addr);
  p_pm_cb->state = BTA_JV_PM_FREE_ST;
  p_pm_cb->app_id = BTA_JV_PM_ALL;
  p_pm_cb->handle = BTA_JV_PM_HANDLE_CLEAR;
  p_pm_cb->peer_bd_addr = RawAddress::kEmpty;
}

/*******************************************************************************
 *
 * Function     bta_jv_free_set_pm_profile_cb
 *
 * Description  free pm profile control block
 *
 * Returns     BTA_JV_SUCCESS if cb has been freed correctly,
 *             BTA_JV_FAILURE in case of no profile has been registered or
 *             already freed
 *
 ******************************************************************************/
static tBTA_JV_STATUS bta_jv_free_set_pm_profile_cb(uint32_t jv_handle) {
  tBTA_JV_STATUS status = BTA_JV_FAILURE;
  tBTA_JV_PM_CB** p_cb;
  int i, j, bd_counter = 0, appid_counter = 0;

  for (i = 0; i < BTA_JV_PM_MAX_NUM; i++) {
    p_cb = NULL;
    if ((bta_jv_cb.pm_cb[i].state != BTA_JV_PM_FREE_ST) &&
        (jv_handle == bta_jv_cb.pm_cb[i].handle)) {
      for (j = 0; j < BTA_JV_PM_MAX_NUM; j++) {
        if (bta_jv_cb.pm_cb[j].peer_bd_addr == bta_jv_cb.pm_cb[i].peer_bd_addr)
          bd_counter++;
        if (bta_jv_cb.pm_cb[j].app_id == bta_jv_cb.pm_cb[i].app_id)
          appid_counter++;
      }

      VLOG(2) << __func__ << ": jv_handle=" << loghex(jv_handle)
              << ", idx=" << i << "app_id=" << bta_jv_cb.pm_cb[i].app_id
              << ", bd_counter=" << bd_counter
              << ", appid_counter=" << appid_counter;
      if (bd_counter > 1) {
        bta_jv_pm_conn_idle(&bta_jv_cb.pm_cb[i]);
      }

      if (bd_counter <= 1 || (appid_counter <= 1)) {
        bta_jv_clear_pm_cb(&bta_jv_cb.pm_cb[i], true);
      } else {
        bta_jv_clear_pm_cb(&bta_jv_cb.pm_cb[i], false);
      }

      if (BTA_JV_RFCOMM_MASK & jv_handle) {
        uint32_t hi =
            ((jv_handle & BTA_JV_RFC_HDL_MASK) & ~BTA_JV_RFCOMM_MASK) - 1;
        uint32_t si = BTA_JV_RFC_HDL_TO_SIDX(jv_handle);
        if (hi < BTA_JV_MAX_RFC_CONN && bta_jv_cb.rfc_cb[hi].p_cback &&
            si < BTA_JV_MAX_RFC_SR_SESSION &&
            bta_jv_cb.rfc_cb[hi].rfc_hdl[si]) {
          tBTA_JV_PCB* p_pcb =
              bta_jv_rfc_port_to_pcb(bta_jv_cb.rfc_cb[hi].rfc_hdl[si]);
          if (p_pcb) {
            if (NULL == p_pcb->p_pm_cb)
              LOG(WARNING) << __func__ << ": jv_handle=" << loghex(jv_handle)
                           << ", port_handle=" << p_pcb->port_handle
                           << ", i=" << i << ", no link to pm_cb?";
            p_cb = &p_pcb->p_pm_cb;
          }
        }
      } else {
        if (jv_handle < BTA_JV_MAX_L2C_CONN) {
          tBTA_JV_L2C_CB* p_l2c_cb = &bta_jv_cb.l2c_cb[jv_handle];
          if (NULL == p_l2c_cb->p_pm_cb)
            LOG(WARNING) << __func__ << ": jv_handle=" << loghex(jv_handle)
                         << ", i=" << i << " no link to pm_cb?";
          p_cb = &p_l2c_cb->p_pm_cb;
        }
      }
      if (p_cb) {
        *p_cb = NULL;
        status = BTA_JV_SUCCESS;
      }
    }
  }
  return status;
}

/*******************************************************************************
 *
 * Function    bta_jv_alloc_set_pm_profile_cb
 *
 * Description set PM profile control block
 *
 * Returns     pointer to allocated cb or NULL in case of failure
 *
 ******************************************************************************/
static tBTA_JV_PM_CB* bta_jv_alloc_set_pm_profile_cb(uint32_t jv_handle,
                                                     tBTA_JV_PM_ID app_id) {
  bool bRfcHandle = (jv_handle & BTA_JV_RFCOMM_MASK) != 0;
  RawAddress peer_bd_addr = RawAddress::kEmpty;
  int i, j;
  tBTA_JV_PM_CB** pp_cb;

  for (i = 0; i < BTA_JV_PM_MAX_NUM; i++) {
    pp_cb = NULL;
    if (bta_jv_cb.pm_cb[i].state == BTA_JV_PM_FREE_ST) {
      /* rfc handle bd addr retrieval requires core stack handle */
      if (bRfcHandle) {
        for (j = 0; j < BTA_JV_MAX_RFC_CONN; j++) {
          if (jv_handle == bta_jv_cb.port_cb[j].handle) {
            pp_cb = &bta_jv_cb.port_cb[j].p_pm_cb;
            if (PORT_SUCCESS !=
                PORT_CheckConnection(bta_jv_cb.port_cb[j].port_handle,
                                     &peer_bd_addr, NULL)) {
              i = BTA_JV_PM_MAX_NUM;
            }
            break;
          }
        }
      } else {
        /* use jv handle for l2cap bd address retrieval */
        for (j = 0; j < BTA_JV_MAX_L2C_CONN; j++) {
          if (jv_handle == bta_jv_cb.l2c_cb[j].handle) {
            pp_cb = &bta_jv_cb.l2c_cb[j].p_pm_cb;
            const RawAddress* p_bd_addr =
                GAP_ConnGetRemoteAddr((uint16_t)jv_handle);
            if (p_bd_addr)
              peer_bd_addr = *p_bd_addr;
            else
              i = BTA_JV_PM_MAX_NUM;
            break;
          }
        }
      }
      VLOG(2) << __func__ << ": handle=" << loghex(jv_handle)
              << ", app_id=" << app_id << ", idx=" << i
              << ", BTA_JV_PM_MAX_NUM=" << BTA_JV_PM_MAX_NUM
              << ", pp_cb=" << pp_cb;
      break;
    }
  }

  if ((i != BTA_JV_PM_MAX_NUM) && (NULL != pp_cb)) {
    *pp_cb = &bta_jv_cb.pm_cb[i];
    bta_jv_cb.pm_cb[i].handle = jv_handle;
    bta_jv_cb.pm_cb[i].app_id = app_id;
    bta_jv_cb.pm_cb[i].peer_bd_addr = peer_bd_addr;
    bta_jv_cb.pm_cb[i].state = BTA_JV_PM_IDLE_ST;
    return &bta_jv_cb.pm_cb[i];
  }
  LOG(WARNING) << __func__ << ": handle=" << loghex(jv_handle)
               << ", app_id=" << app_id << ", return NULL";
  return NULL;
}

/*******************************************************************************
 *
 * Function     bta_jv_check_psm
 *
 * Description  for now use only the legal PSM per JSR82 spec
 *
 * Returns      true, if allowed
 *
 ******************************************************************************/
bool bta_jv_check_psm(uint16_t psm) {
  bool ret = false;

  if (L2C_IS_VALID_PSM(psm)) {
    if (psm < 0x1001) {
      /* see if this is defined by spec */
      switch (psm) {
        case SDP_PSM:       /* 1 */
        case BT_PSM_RFCOMM: /* 3 */
          /* do not allow java app to use these 2 PSMs */
          break;

        case TCS_PSM_INTERCOM: /* 5 */
        case TCS_PSM_CORDLESS: /* 7 */
          if (!bta_sys_is_register(BTA_ID_CT) &&
              !bta_sys_is_register(BTA_ID_CG))
            ret = true;
          break;

        case BT_PSM_BNEP: /* F */
          if (!bta_sys_is_register(BTA_ID_PAN)) ret = true;
          break;

        case HID_PSM_CONTROL:   /* 0x11 */
        case HID_PSM_INTERRUPT: /* 0x13 */
          // FIX: allow HID Device and HID Host to coexist
          if (!bta_sys_is_register(BTA_ID_HD) ||
              !bta_sys_is_register(BTA_ID_HH))
            ret = true;
          break;

        case AVCT_PSM: /* 0x17 */
        case AVDT_PSM: /* 0x19 */
          if ((!bta_sys_is_register(BTA_ID_AV)) &&
              (!bta_sys_is_register(BTA_ID_AVK)))
            ret = true;
          break;

        default:
          ret = true;
          break;
      }
    } else {
      ret = true;
    }
  }
  return ret;
}

/* Initialises the JAVA I/F */
void bta_jv_enable(tBTA_JV_DM_CBACK* p_cback) {
  tBTA_JV_STATUS status = BTA_JV_SUCCESS;
  bta_jv_cb.p_dm_cback = p_cback;
  tBTA_JV bta_jv;
  bta_jv.status = status;
  bta_jv_cb.p_dm_cback(BTA_JV_ENABLE_EVT, &bta_jv, 0);
  memset(bta_jv_cb.free_psm_list, 0, sizeof(bta_jv_cb.free_psm_list));
}

/** Disables the BT device manager free the resources used by java */
void bta_jv_disable() { LOG(INFO) << __func__; }

/**
 * We keep a list of PSM's that have been freed from JAVA, for reuse.
 * This function will return a free PSM, and delete it from the free
 * list.
 * If no free PSMs exist, 0 will be returned.
 */
static uint16_t bta_jv_get_free_psm() {
  const int cnt =
      sizeof(bta_jv_cb.free_psm_list) / sizeof(bta_jv_cb.free_psm_list[0]);
  for (int i = 0; i < cnt; i++) {
    uint16_t psm = bta_jv_cb.free_psm_list[i];
    if (psm != 0) {
      VLOG(2) << __func__ << ": Reusing PSM=" << loghex(psm);
      bta_jv_cb.free_psm_list[i] = 0;
      return psm;
    }
  }
  return 0;
}

static void bta_jv_set_free_psm(uint16_t psm) {
  int free_index = -1;
  const int cnt =
      sizeof(bta_jv_cb.free_psm_list) / sizeof(bta_jv_cb.free_psm_list[0]);
  for (int i = 0; i < cnt; i++) {
    if (bta_jv_cb.free_psm_list[i] == 0) {
      free_index = i;
    } else if (psm == bta_jv_cb.free_psm_list[i]) {
      return;  // PSM already freed?
    }
  }
  if (free_index != -1) {
    bta_jv_cb.free_psm_list[free_index] = psm;
    VLOG(2) << __func__ << ": Recycling PSM=" << loghex(psm);
  } else {
    LOG(ERROR) << __func__ << ": unable to free psm=" << loghex(psm)
               << " no more free slots";
  }
}

/** Obtain a free SCN (Server Channel Number) (RFCOMM channel or L2CAP PSM) */
void bta_jv_get_channel_id(
    int32_t type /* One of BTA_JV_CONN_TYPE_ */,
    int32_t channel /* optionally request a specific channel */,
    uint32_t l2cap_socket_id, uint32_t rfcomm_slot_id) {
  uint16_t psm = 0;

  switch (type) {
    case BTA_JV_CONN_TYPE_RFCOMM: {
      uint8_t scn = 0;
      if (channel > 0) {
        if (!BTM_TryAllocateSCN(channel)) {
          LOG(ERROR) << "rfc channel=" << channel
                     << " already in use or invalid";
          channel = 0;
        }
      } else {
        channel = BTM_AllocateSCN();
        if (channel == 0) {
          LOG(ERROR) << "run out of rfc channels";
          channel = 0;
        }
      }
      if (channel != 0) {
        bta_jv_cb.scn[channel - 1] = true;
        scn = (uint8_t)channel;
      }
      if (bta_jv_cb.p_dm_cback) {
        tBTA_JV bta_jv;
        bta_jv.scn = scn;
        bta_jv_cb.p_dm_cback(BTA_JV_GET_SCN_EVT, &bta_jv, rfcomm_slot_id);
      }
      return;
    }
    case BTA_JV_CONN_TYPE_L2CAP:
      psm = bta_jv_get_free_psm();
      if (psm == 0) {
        psm = L2CA_AllocatePSM();
        VLOG(2) << __func__ << ": returned PSM=" << loghex(psm);
      }
      break;
    case BTA_JV_CONN_TYPE_L2CAP_LE:
      psm = L2CA_AllocateLePSM();
      if (psm == 0) {
        LOG(ERROR) << __func__ << ": Error: No free LE PSM available";
      }
      break;
    default:
      break;
  }

  if (bta_jv_cb.p_dm_cback) {
    tBTA_JV bta_jv;
    bta_jv.psm = psm;
    bta_jv_cb.p_dm_cback(BTA_JV_GET_PSM_EVT, &bta_jv, l2cap_socket_id);
  }
}

/** free a SCN */
void bta_jv_free_scn(int32_t type /* One of BTA_JV_CONN_TYPE_ */,
                     uint16_t scn) {
  switch (type) {
    case BTA_JV_CONN_TYPE_RFCOMM: {
      if (scn > 0 && scn <= BTA_JV_MAX_SCN && bta_jv_cb.scn[scn - 1]) {
        /* this scn is used by JV */
        bta_jv_cb.scn[scn - 1] = false;
        BTM_FreeSCN(scn);
      }
      break;
    }
    case BTA_JV_CONN_TYPE_L2CAP:
      bta_jv_set_free_psm(scn);
      break;
    case BTA_JV_CONN_TYPE_L2CAP_LE:
      VLOG(2) << __func__ << ": type=BTA_JV_CONN_TYPE_L2CAP_LE. psm=" << scn;
      L2CA_FreeLePSM(scn);
      break;
    default:
      break;
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_start_discovery_cback
 *
 * Description  Callback for Start Discovery
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_start_discovery_cback(uint16_t result, void* user_data) {
  tBTA_JV_STATUS status;
  uint32_t* p_rfcomm_slot_id = static_cast<uint32_t*>(user_data);

  VLOG(2) << __func__ << ": res=" << loghex(result);

  bta_jv_cb.sdp_active = BTA_JV_SDP_ACT_NONE;
  if (bta_jv_cb.p_dm_cback) {
    tBTA_JV_DISCOVERY_COMP dcomp;
    dcomp.scn = 0;
    status = BTA_JV_FAILURE;
    if (result == SDP_SUCCESS || result == SDP_DB_FULL) {
      tSDP_DISC_REC* p_sdp_rec = NULL;
      tSDP_PROTOCOL_ELEM pe;
      VLOG(2) << __func__ << ": bta_jv_cb.uuid=" << bta_jv_cb.uuid;
      p_sdp_rec = SDP_FindServiceUUIDInDb(p_bta_jv_cfg->p_sdp_db,
                                          bta_jv_cb.uuid, p_sdp_rec);
      VLOG(2) << __func__ << ": p_sdp_rec=" << p_sdp_rec;
      if (p_sdp_rec &&
          SDP_FindProtocolListElemInRec(p_sdp_rec, UUID_PROTOCOL_RFCOMM, &pe)) {
        dcomp.scn = (uint8_t)pe.params[0];
        status = BTA_JV_SUCCESS;
      }
    }

    dcomp.status = status;
    tBTA_JV bta_jv;
    bta_jv.disc_comp = dcomp;
    bta_jv_cb.p_dm_cback(BTA_JV_DISCOVERY_COMP_EVT, &bta_jv, *p_rfcomm_slot_id);
    osi_free(p_rfcomm_slot_id);
  }
}

/* Discovers services on a remote device */
void bta_jv_start_discovery(const RawAddress& bd_addr, uint16_t num_uuid,
                            bluetooth::Uuid* uuid_list,
                            uint32_t rfcomm_slot_id) {
  tBTA_JV_STATUS status = BTA_JV_FAILURE;
  VLOG(2) << __func__ << ": in, sdp_active=" << bta_jv_cb.sdp_active;
  if (bta_jv_cb.sdp_active != BTA_JV_SDP_ACT_NONE) {
    /* SDP is still in progress */
    status = BTA_JV_BUSY;
    if (bta_jv_cb.p_dm_cback) {
      tBTA_JV bta_jv;
      bta_jv.status = status;
      bta_jv_cb.p_dm_cback(BTA_JV_DISCOVERY_COMP_EVT, &bta_jv, rfcomm_slot_id);
    }
    return;
  }

  /* init the database/set up the filter */
  VLOG(2) << __func__ << ": call SDP_InitDiscoveryDb, num_uuid=" << num_uuid;
  SDP_InitDiscoveryDb(p_bta_jv_cfg->p_sdp_db, p_bta_jv_cfg->sdp_db_size,
                      num_uuid, uuid_list, 0, NULL);

  /* tell SDP to keep the raw data */
  p_bta_jv_cfg->p_sdp_db->raw_data = p_bta_jv_cfg->p_sdp_raw_data;
  p_bta_jv_cfg->p_sdp_db->raw_size = p_bta_jv_cfg->sdp_raw_size;

  bta_jv_cb.p_sel_raw_data = 0;
  bta_jv_cb.uuid = uuid_list[0];

  bta_jv_cb.sdp_active = BTA_JV_SDP_ACT_YES;

  uint32_t* rfcomm_slot_id_copy = (uint32_t*)osi_malloc(sizeof(uint32_t));
  *rfcomm_slot_id_copy = rfcomm_slot_id;

  if (!SDP_ServiceSearchAttributeRequest2(bd_addr, p_bta_jv_cfg->p_sdp_db,
                                          bta_jv_start_discovery_cback,
                                          (void*)rfcomm_slot_id_copy)) {
    bta_jv_cb.sdp_active = BTA_JV_SDP_ACT_NONE;
    /* failed to start SDP. report the failure right away */
    if (bta_jv_cb.p_dm_cback) {
      tBTA_JV bta_jv;
      bta_jv.status = status;
      bta_jv_cb.p_dm_cback(BTA_JV_DISCOVERY_COMP_EVT, &bta_jv, rfcomm_slot_id);
    }
  }
  /*
  else report the result when the cback is called
  */
}

/* Create an SDP record with the given attributes */
void bta_jv_create_record(uint32_t rfcomm_slot_id) {
  tBTA_JV_CREATE_RECORD evt_data;
  evt_data.status = BTA_JV_SUCCESS;
  if (bta_jv_cb.p_dm_cback) {
    // callback immediately to create the sdp record in stack thread context
    tBTA_JV bta_jv;
    bta_jv.create_rec = evt_data;
    bta_jv_cb.p_dm_cback(BTA_JV_CREATE_RECORD_EVT, &bta_jv, rfcomm_slot_id);
  }
}

/* Delete an SDP record */
void bta_jv_delete_record(uint32_t handle) {
  if (handle) {
    /* this is a record created by btif layer*/
    SDP_DeleteRecord(handle);
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_l2cap_client_cback
 *
 * Description  handles the l2cap client events
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_l2cap_client_cback(uint16_t gap_handle, uint16_t event,
                                      tGAP_CB_DATA* data) {
  tBTA_JV_L2C_CB* p_cb = &bta_jv_cb.l2c_cb[gap_handle];
  tBTA_JV evt_data;

  if (gap_handle >= BTA_JV_MAX_L2C_CONN && !p_cb->p_cback) return;

  VLOG(2) << __func__ << ": gap_handle=" << gap_handle
          << ", evt=" << loghex(event);
  evt_data.l2c_open.status = BTA_JV_SUCCESS;
  evt_data.l2c_open.handle = gap_handle;

  switch (event) {
    case GAP_EVT_CONN_OPENED:
      evt_data.l2c_open.rem_bda = *GAP_ConnGetRemoteAddr(gap_handle);
      evt_data.l2c_open.tx_mtu = GAP_ConnGetRemMtuSize(gap_handle);
      p_cb->state = BTA_JV_ST_CL_OPEN;
      p_cb->p_cback(BTA_JV_L2CAP_OPEN_EVT, &evt_data, p_cb->l2cap_socket_id);
      break;

    case GAP_EVT_CONN_CLOSED:
      p_cb->state = BTA_JV_ST_NONE;
      bta_jv_free_sec_id(&p_cb->sec_id);
      evt_data.l2c_close.async = true;
      p_cb->p_cback(BTA_JV_L2CAP_CLOSE_EVT, &evt_data, p_cb->l2cap_socket_id);
      p_cb->p_cback = NULL;
      break;

    case GAP_EVT_CONN_DATA_AVAIL:
      evt_data.data_ind.handle = gap_handle;
      /* Reset idle timer to avoid requesting sniff mode while receiving data */
      bta_jv_pm_conn_busy(p_cb->p_pm_cb);
      p_cb->p_cback(BTA_JV_L2CAP_DATA_IND_EVT, &evt_data,
                    p_cb->l2cap_socket_id);
      bta_jv_pm_conn_idle(p_cb->p_pm_cb);
      break;

    case GAP_EVT_TX_EMPTY:
      bta_jv_pm_conn_idle(p_cb->p_pm_cb);
      break;

    case GAP_EVT_CONN_CONGESTED:
    case GAP_EVT_CONN_UNCONGESTED:
      p_cb->cong = (event == GAP_EVT_CONN_CONGESTED) ? true : false;
      evt_data.l2c_cong.cong = p_cb->cong;
      p_cb->p_cback(BTA_JV_L2CAP_CONG_EVT, &evt_data, p_cb->l2cap_socket_id);
      break;

    default:
      break;
  }
}

/* makes an l2cap client connection */
void bta_jv_l2cap_connect(int32_t type, tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                          uint16_t remote_psm, uint16_t rx_mtu,
                          const RawAddress& peer_bd_addr,
                          std::unique_ptr<tL2CAP_CFG_INFO> cfg_param,
                          std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info,
                          tBTA_JV_L2CAP_CBACK* p_cback,
                          uint32_t l2cap_socket_id) {
  uint16_t handle = GAP_INVALID_HANDLE;
  uint8_t chan_mode_mask = GAP_FCR_CHAN_OPT_BASIC;

  tL2CAP_CFG_INFO cfg;
  memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));
  if (cfg_param) {
    cfg = *cfg_param;
    if (cfg.fcr_present && cfg.fcr.mode == L2CAP_FCR_ERTM_MODE) {
      chan_mode_mask = GAP_FCR_CHAN_OPT_ERTM;
    }
  }

  /* We need to use this value for MTU to be able to handle cases where cfg is
   * not set in req. */
  cfg.mtu_present = true;
  cfg.mtu = rx_mtu;

  /* TODO: DM role manager
  L2CA_SetDesireRole(role);
  */

  uint8_t sec_id = bta_jv_alloc_sec_id();
  tBTA_JV_L2CAP_CL_INIT evt_data;
  evt_data.sec_id = sec_id;
  evt_data.status = BTA_JV_FAILURE;

  if (sec_id) {
    /* PSM checking is not required for LE COC */
    if ((type != BTA_JV_CONN_TYPE_L2CAP) ||
        (bta_jv_check_psm(remote_psm))) /* allowed */
    {
      uint16_t max_mps = 0xffff;  // Let GAP_ConnOpen set the max_mps.
      handle = GAP_ConnOpen("", sec_id, 0, &peer_bd_addr, remote_psm, max_mps,
                            &cfg, ertm_info.get(), sec_mask, chan_mode_mask,
                            bta_jv_l2cap_client_cback, type);
      if (handle != GAP_INVALID_HANDLE) {
        evt_data.status = BTA_JV_SUCCESS;
      }
    }
  }

  if (evt_data.status == BTA_JV_SUCCESS) {
    tBTA_JV_L2C_CB* p_cb;
    p_cb = &bta_jv_cb.l2c_cb[handle];
    p_cb->handle = handle;
    p_cb->p_cback = p_cback;
    p_cb->l2cap_socket_id = l2cap_socket_id;
    p_cb->psm = 0; /* not a server */
    p_cb->sec_id = sec_id;
    p_cb->state = BTA_JV_ST_CL_OPENING;
  } else {
    bta_jv_free_sec_id(&sec_id);
  }

  evt_data.handle = handle;
  if (p_cback) {
    tBTA_JV bta_jv;
    bta_jv.l2c_cl_init = evt_data;
    p_cback(BTA_JV_L2CAP_CL_INIT_EVT, &bta_jv, l2cap_socket_id);
  }
}

/** Close an L2CAP client connection */
void bta_jv_l2cap_close(uint32_t handle, tBTA_JV_L2C_CB* p_cb) {
  tBTA_JV_L2CAP_CLOSE evt_data;
  tBTA_JV_L2CAP_CBACK* p_cback = p_cb->p_cback;
  uint32_t l2cap_socket_id = p_cb->l2cap_socket_id;

  evt_data.handle = handle;
  evt_data.status = bta_jv_free_l2c_cb(p_cb);
  evt_data.async = false;

  if (p_cback) {
    tBTA_JV bta_jv;
    bta_jv.l2c_close = evt_data;
    p_cback(BTA_JV_L2CAP_CLOSE_EVT, &bta_jv, l2cap_socket_id);
  }
}

/*******************************************************************************
 *
 * Function         bta_jv_l2cap_server_cback
 *
 * Description      handles the l2cap server callback
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_jv_l2cap_server_cback(uint16_t gap_handle, uint16_t event,
                                      tGAP_CB_DATA* data) {
  tBTA_JV_L2C_CB* p_cb = &bta_jv_cb.l2c_cb[gap_handle];
  tBTA_JV evt_data;
  tBTA_JV_L2CAP_CBACK* p_cback;
  uint32_t socket_id;

  if (gap_handle >= BTA_JV_MAX_L2C_CONN && !p_cb->p_cback) return;

  VLOG(2) << __func__ << ": gap_handle=" << gap_handle
          << ", evt=" << loghex(event);
  evt_data.l2c_open.status = BTA_JV_SUCCESS;
  evt_data.l2c_open.handle = gap_handle;

  switch (event) {
    case GAP_EVT_CONN_OPENED:
      evt_data.l2c_open.rem_bda = *GAP_ConnGetRemoteAddr(gap_handle);
      evt_data.l2c_open.tx_mtu = GAP_ConnGetRemMtuSize(gap_handle);
      p_cb->state = BTA_JV_ST_SR_OPEN;
      p_cb->p_cback(BTA_JV_L2CAP_OPEN_EVT, &evt_data, p_cb->l2cap_socket_id);
      break;

    case GAP_EVT_CONN_CLOSED:
      evt_data.l2c_close.async = true;
      evt_data.l2c_close.handle = p_cb->handle;
      p_cback = p_cb->p_cback;
      socket_id = p_cb->l2cap_socket_id;
      evt_data.l2c_close.status = bta_jv_free_l2c_cb(p_cb);
      p_cback(BTA_JV_L2CAP_CLOSE_EVT, &evt_data, socket_id);
      break;

    case GAP_EVT_CONN_DATA_AVAIL:
      evt_data.data_ind.handle = gap_handle;
      /* Reset idle timer to avoid requesting sniff mode while receiving data */
      bta_jv_pm_conn_busy(p_cb->p_pm_cb);
      p_cb->p_cback(BTA_JV_L2CAP_DATA_IND_EVT, &evt_data,
                    p_cb->l2cap_socket_id);
      bta_jv_pm_conn_idle(p_cb->p_pm_cb);
      break;

    case GAP_EVT_TX_EMPTY:
      bta_jv_pm_conn_idle(p_cb->p_pm_cb);
      break;

    case GAP_EVT_CONN_CONGESTED:
    case GAP_EVT_CONN_UNCONGESTED:
      p_cb->cong = (event == GAP_EVT_CONN_CONGESTED) ? true : false;
      evt_data.l2c_cong.cong = p_cb->cong;
      p_cb->p_cback(BTA_JV_L2CAP_CONG_EVT, &evt_data, p_cb->l2cap_socket_id);
      break;

    default:
      break;
  }
}

/** starts an L2CAP server */
void bta_jv_l2cap_start_server(int32_t type, tBTA_SEC sec_mask,
                               tBTA_JV_ROLE role, uint16_t local_psm,
                               uint16_t rx_mtu,
                               std::unique_ptr<tL2CAP_CFG_INFO> cfg_param,
                               std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info,
                               tBTA_JV_L2CAP_CBACK* p_cback,
                               uint32_t l2cap_socket_id) {
  uint16_t handle;
  tBTA_JV_L2CAP_START evt_data;
  uint8_t chan_mode_mask = GAP_FCR_CHAN_OPT_BASIC;

  tL2CAP_CFG_INFO cfg;
  memset(&cfg, 0, sizeof(tL2CAP_CFG_INFO));
  if (cfg_param) {
    cfg = *cfg_param;
    if (cfg.fcr_present && cfg.fcr.mode == L2CAP_FCR_ERTM_MODE) {
      chan_mode_mask = GAP_FCR_CHAN_OPT_ERTM;
    }
  }

  // FIX: MTU=0 means not present
  if (rx_mtu > 0) {
    cfg.mtu_present = true;
    cfg.mtu = rx_mtu;
  } else {
    cfg.mtu_present = false;
    cfg.mtu = 0;
  }

  /* TODO DM role manager
  L2CA_SetDesireRole(role);
  */

  uint8_t sec_id = bta_jv_alloc_sec_id();
  uint16_t max_mps = 0xffff;  // Let GAP_ConnOpen set the max_mps.
  /* PSM checking is not required for LE COC */
  if (0 == sec_id ||
      ((type == BTA_JV_CONN_TYPE_L2CAP) && (!bta_jv_check_psm(local_psm))) ||
      (handle = GAP_ConnOpen("JV L2CAP", sec_id, 1, nullptr, local_psm, max_mps,
                             &cfg, ertm_info.get(), sec_mask, chan_mode_mask,
                             bta_jv_l2cap_server_cback, type)) ==
          GAP_INVALID_HANDLE) {
    bta_jv_free_sec_id(&sec_id);
    evt_data.status = BTA_JV_FAILURE;
  } else {
    tBTA_JV_L2C_CB* p_cb = &bta_jv_cb.l2c_cb[handle];
    evt_data.status = BTA_JV_SUCCESS;
    evt_data.handle = handle;
    evt_data.sec_id = sec_id;
    p_cb->p_cback = p_cback;
    p_cb->l2cap_socket_id = l2cap_socket_id;
    p_cb->handle = handle;
    p_cb->sec_id = sec_id;
    p_cb->state = BTA_JV_ST_SR_LISTEN;
    p_cb->psm = local_psm;
  }

  if (p_cback) {
    tBTA_JV bta_jv;
    bta_jv.l2c_start = evt_data;
    p_cback(BTA_JV_L2CAP_START_EVT, &bta_jv, l2cap_socket_id);
  }
}

/* stops an L2CAP server */
void bta_jv_l2cap_stop_server(uint16_t local_psm, uint32_t l2cap_socket_id) {
  for (int i = 0; i < BTA_JV_MAX_L2C_CONN; i++) {
    if (bta_jv_cb.l2c_cb[i].l2cap_socket_id == l2cap_socket_id) {
      tBTA_JV_L2C_CB* p_cb = &bta_jv_cb.l2c_cb[i];
      tBTA_JV_L2CAP_CBACK* p_cback = p_cb->p_cback;
      tBTA_JV_L2CAP_CLOSE evt_data;
      evt_data.handle = p_cb->handle;
      evt_data.status = bta_jv_free_l2c_cb(p_cb);
      evt_data.async = false;
      if (p_cback) {
        tBTA_JV bta_jv;
        bta_jv.l2c_close = evt_data;
        p_cback(BTA_JV_L2CAP_CLOSE_EVT, &bta_jv, l2cap_socket_id);
      }
      break;
    }
  }
}

/* Write data to an L2CAP connection */
void bta_jv_l2cap_write(uint32_t handle, uint32_t req_id, BT_HDR* msg,
                        uint32_t user_id, tBTA_JV_L2C_CB* p_cb) {
  /* As we check this callback exists before the tBTA_JV_API_L2CAP_WRITE can be
   * send through the API this check should not be needed. But the API is not
   * designed to be used (safely at least) in a multi-threaded scheduler, hence
   * if the peer device disconnects the l2cap link after the API is called, but
   * before this message is handled, the ->p_cback will be cleared at this
   * point. At first glanch this seems highly unlikely, but for all
   * obex-profiles with two channels connected - e.g. MAP, this happens around 1
   * of 4 disconnects, as a disconnect on the server channel causes a disconnect
   * to be send on the client (notification) channel, but at the peer typically
   * disconnects both the OBEX disconnect request crosses the incoming l2cap
   * disconnect. If p_cback is cleared, we simply discard the data. RISK: The
   * caller must handle any cleanup based on another signal than
   * BTA_JV_L2CAP_WRITE_EVT, which is typically not possible, as the pointer to
   * the allocated buffer is stored in this message, and can therefore not be
   * freed, hence we have a mem-leak-by-design.*/
  if (!p_cb->p_cback) {
    /* As this pointer is checked in the API function, this occurs only when the
     * channel is disconnected after the API function is called, but before the
     * message is handled. */
    LOG(ERROR) << __func__ << ": p_cb->p_cback == NULL";
    osi_free(msg);
    return;
  }

  tBTA_JV_L2CAP_WRITE evt_data;
  evt_data.status = BTA_JV_FAILURE;
  evt_data.handle = handle;
  evt_data.req_id = req_id;
  evt_data.cong = p_cb->cong;
  evt_data.len = msg->len;

  bta_jv_pm_conn_busy(p_cb->p_pm_cb);

  // TODO: this was set only for non-fixed channel packets. Is that needed ?
  msg->event = BT_EVT_TO_BTU_SP_DATA;

  if (evt_data.cong) {
    osi_free(msg);
  } else {
    if (GAP_ConnWriteData(handle, msg) == BT_PASS)
      evt_data.status = BTA_JV_SUCCESS;
  }

  tBTA_JV bta_jv;
  bta_jv.l2c_write = evt_data;
  p_cb->p_cback(BTA_JV_L2CAP_WRITE_EVT, &bta_jv, user_id);
}

/* Write data to an L2CAP connection using Fixed channels */
void bta_jv_l2cap_write_fixed(uint16_t channel, const RawAddress& addr,
                              uint32_t req_id, BT_HDR* msg, uint32_t user_id,
                              tBTA_JV_L2CAP_CBACK* p_cback) {
  tBTA_JV_L2CAP_WRITE_FIXED evt_data;
  evt_data.status = BTA_JV_FAILURE;
  evt_data.channel = channel;
  evt_data.addr = addr;
  evt_data.req_id = req_id;
  evt_data.len = 0;

  L2CA_SendFixedChnlData(channel, addr, msg);

  tBTA_JV bta_jv;
  bta_jv.l2c_write_fixed = evt_data;
  p_cback(BTA_JV_L2CAP_WRITE_FIXED_EVT, &bta_jv, user_id);
}

/*******************************************************************************
 *
 * Function     bta_jv_port_data_co_cback
 *
 * Description  port data callback function of rfcomm
 *              connections
 *
 * Returns      void
 *
 ******************************************************************************/
static int bta_jv_port_data_co_cback(uint16_t port_handle, uint8_t* buf,
                                     uint16_t len, int type) {
  tBTA_JV_RFC_CB* p_cb = bta_jv_rfc_port_to_cb(port_handle);
  tBTA_JV_PCB* p_pcb = bta_jv_rfc_port_to_pcb(port_handle);
  VLOG(2) << __func__ << ": p_cb=" << p_cb << ", p_pcb=" << p_pcb
          << ", len=" << len << ", type=" << type;
  if (p_pcb != NULL) {
    switch (type) {
      case DATA_CO_CALLBACK_TYPE_INCOMING:
        return bta_co_rfc_data_incoming(p_pcb->rfcomm_slot_id, (BT_HDR*)buf);
      case DATA_CO_CALLBACK_TYPE_OUTGOING_SIZE:
        return bta_co_rfc_data_outgoing_size(p_pcb->rfcomm_slot_id, (int*)buf);
      case DATA_CO_CALLBACK_TYPE_OUTGOING:
        return bta_co_rfc_data_outgoing(p_pcb->rfcomm_slot_id, buf, len);
      default:
        LOG(ERROR) << __func__ << ": unknown callout type=" << type;
        break;
    }
  }
  return 0;
}

/*******************************************************************************
 *
 * Function     bta_jv_port_mgmt_cl_cback
 *
 * Description  callback for port mamangement function of rfcomm
 *              client connections
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_port_mgmt_cl_cback(uint32_t code, uint16_t port_handle) {
  tBTA_JV_RFC_CB* p_cb = bta_jv_rfc_port_to_cb(port_handle);
  tBTA_JV_PCB* p_pcb = bta_jv_rfc_port_to_pcb(port_handle);
  tBTA_JV evt_data;
  RawAddress rem_bda = RawAddress::kEmpty;
  uint16_t lcid;
  tBTA_JV_RFCOMM_CBACK* p_cback; /* the callback function */

  VLOG(2) << __func__ << ": code=" << code << ", port_handle=" << port_handle;
  if (NULL == p_cb || NULL == p_cb->p_cback) return;

  VLOG(2) << __func__ << ": code=" << code << ", port_handle=" << port_handle
          << ", handle=" << p_cb->handle;

  PORT_CheckConnection(port_handle, &rem_bda, &lcid);

  if (code == PORT_SUCCESS) {
    evt_data.rfc_open.handle = p_cb->handle;
    evt_data.rfc_open.status = BTA_JV_SUCCESS;
    evt_data.rfc_open.rem_bda = rem_bda;
    p_pcb->state = BTA_JV_ST_CL_OPEN;
    p_cb->p_cback(BTA_JV_RFCOMM_OPEN_EVT, &evt_data, p_pcb->rfcomm_slot_id);
  } else {
    evt_data.rfc_close.handle = p_cb->handle;
    evt_data.rfc_close.status = BTA_JV_FAILURE;
    evt_data.rfc_close.port_status = code;
    evt_data.rfc_close.async = true;
    if (p_pcb->state == BTA_JV_ST_CL_CLOSING) {
      evt_data.rfc_close.async = false;
    }
    // p_pcb->state = BTA_JV_ST_NONE;
    // p_pcb->cong = false;
    p_cback = p_cb->p_cback;
    p_cback(BTA_JV_RFCOMM_CLOSE_EVT, &evt_data, p_pcb->rfcomm_slot_id);
    // bta_jv_free_rfc_cb(p_cb, p_pcb);
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_port_event_cl_cback
 *
 * Description  Callback for RFCOMM client port events
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_port_event_cl_cback(uint32_t code, uint16_t port_handle) {
  tBTA_JV_RFC_CB* p_cb = bta_jv_rfc_port_to_cb(port_handle);
  tBTA_JV_PCB* p_pcb = bta_jv_rfc_port_to_pcb(port_handle);
  tBTA_JV evt_data;

  VLOG(2) << __func__ << ": port_handle=" << port_handle;
  if (NULL == p_cb || NULL == p_cb->p_cback) return;

  VLOG(2) << __func__ << ": code=" << loghex(code)
          << ", port_handle=" << port_handle << ", handle=" << p_cb->handle;
  if (code & PORT_EV_RXCHAR) {
    evt_data.data_ind.handle = p_cb->handle;
    p_cb->p_cback(BTA_JV_RFCOMM_DATA_IND_EVT, &evt_data, p_pcb->rfcomm_slot_id);
  }

  if (code & PORT_EV_FC) {
    p_pcb->cong = (code & PORT_EV_FCS) ? false : true;
    evt_data.rfc_cong.cong = p_pcb->cong;
    evt_data.rfc_cong.handle = p_cb->handle;
    evt_data.rfc_cong.status = BTA_JV_SUCCESS;
    p_cb->p_cback(BTA_JV_RFCOMM_CONG_EVT, &evt_data, p_pcb->rfcomm_slot_id);
  }

  if (code & PORT_EV_TXEMPTY) {
    bta_jv_pm_conn_idle(p_pcb->p_pm_cb);
  }
}

/* Client initiates an RFCOMM connection */
void bta_jv_rfcomm_connect(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                           uint8_t remote_scn, const RawAddress& peer_bd_addr,
                           tBTA_JV_RFCOMM_CBACK* p_cback,
                           uint32_t rfcomm_slot_id) {
  uint16_t handle = 0;
  uint32_t event_mask = BTA_JV_RFC_EV_MASK;
  tPORT_STATE port_state;

  /* TODO DM role manager
  L2CA_SetDesireRole(role);
  */

  uint8_t sec_id = bta_jv_alloc_sec_id();

  tBTA_JV_RFCOMM_CL_INIT evt_data;
  memset(&evt_data, 0, sizeof(evt_data));
  evt_data.sec_id = sec_id;
  evt_data.status = BTA_JV_SUCCESS;
  if (0 == sec_id ||
      !BTM_SetSecurityLevel(true, "", sec_id, sec_mask, BT_PSM_RFCOMM,
                            BTM_SEC_PROTO_RFCOMM, remote_scn)) {
    evt_data.status = BTA_JV_FAILURE;
    LOG(ERROR) << __func__ << ": sec_id=" << +sec_id
               << " is zero or BTM_SetSecurityLevel failed, remote_scn:"
               << +remote_scn;
  }

  if (evt_data.status == BTA_JV_SUCCESS &&
      RFCOMM_CreateConnection(UUID_SERVCLASS_SERIAL_PORT, remote_scn, false,
                              BTA_JV_DEF_RFC_MTU, peer_bd_addr, &handle,
                              bta_jv_port_mgmt_cl_cback) != PORT_SUCCESS) {
    LOG(ERROR) << __func__ << ": RFCOMM_CreateConnection failed";
    evt_data.status = BTA_JV_FAILURE;
  }
  if (evt_data.status == BTA_JV_SUCCESS) {
    tBTA_JV_PCB* p_pcb;
    tBTA_JV_RFC_CB* p_cb = bta_jv_alloc_rfc_cb(handle, &p_pcb);
    if (p_cb) {
      p_cb->p_cback = p_cback;
      p_cb->sec_id = sec_id;
      p_cb->scn = 0;
      p_pcb->state = BTA_JV_ST_CL_OPENING;
      p_pcb->rfcomm_slot_id = rfcomm_slot_id;
      evt_data.use_co = true;

      PORT_SetEventCallback(handle, bta_jv_port_event_cl_cback);
      PORT_SetEventMask(handle, event_mask);
      PORT_SetDataCOCallback(handle, bta_jv_port_data_co_cback);

      PORT_GetState(handle, &port_state);

      port_state.fc_type = (PORT_FC_CTS_ON_INPUT | PORT_FC_CTS_ON_OUTPUT);

      PORT_SetState(handle, &port_state);

      evt_data.handle = p_cb->handle;
    } else {
      evt_data.status = BTA_JV_FAILURE;
      LOG(ERROR) << __func__ << ": run out of rfc control block";
    }
  }
  tBTA_JV bta_jv;
  bta_jv.rfc_cl_init = evt_data;
  p_cback(BTA_JV_RFCOMM_CL_INIT_EVT, &bta_jv, rfcomm_slot_id);
  if (bta_jv.rfc_cl_init.status == BTA_JV_FAILURE) {
    if (sec_id) bta_jv_free_sec_id(&sec_id);
    if (handle) RFCOMM_RemoveConnection(handle);
  }
}

static int find_rfc_pcb(uint32_t rfcomm_slot_id, tBTA_JV_RFC_CB** cb,
                        tBTA_JV_PCB** pcb) {
  *cb = NULL;
  *pcb = NULL;
  int i;
  for (i = 0; i < MAX_RFC_PORTS; i++) {
    uint32_t rfc_handle = bta_jv_cb.port_cb[i].handle & BTA_JV_RFC_HDL_MASK;
    rfc_handle &= ~BTA_JV_RFCOMM_MASK;
    if (rfc_handle && bta_jv_cb.port_cb[i].rfcomm_slot_id == rfcomm_slot_id) {
      *pcb = &bta_jv_cb.port_cb[i];
      *cb = &bta_jv_cb.rfc_cb[rfc_handle - 1];
      VLOG(2) << __func__ << ": FOUND rfc_cb_handle=" << loghex(rfc_handle)
              << ", port.jv_handle=" << loghex((*pcb)->handle)
              << ", state=" << (*pcb)->state
              << ", rfc_cb->handle=" << loghex((*cb)->handle);
      return 1;
    }
  }
  VLOG(2) << __func__
          << ": cannot find rfc_cb from user data:" << rfcomm_slot_id;
  return 0;
}

/* Close an RFCOMM connection */
void bta_jv_rfcomm_close(uint32_t handle, uint32_t rfcomm_slot_id) {
  if (!handle) {
    LOG(ERROR) << __func__ << ": rfc handle is null";
    return;
  }

  VLOG(2) << __func__ << ": rfc handle=" << handle;

  tBTA_JV_RFC_CB* p_cb = NULL;
  tBTA_JV_PCB* p_pcb = NULL;

  if (!find_rfc_pcb(rfcomm_slot_id, &p_cb, &p_pcb)) return;
  bta_jv_free_rfc_cb(p_cb, p_pcb);
  VLOG(2) << __func__ << ": sec id in use=" << get_sec_id_used()
          << ", rfc_cb in use=" << get_rfc_cb_used();
}

/*******************************************************************************
 *
 * Function     bta_jv_port_mgmt_sr_cback
 *
 * Description  callback for port mamangement function of rfcomm
 *              server connections
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_port_mgmt_sr_cback(uint32_t code, uint16_t port_handle) {
  tBTA_JV_PCB* p_pcb = bta_jv_rfc_port_to_pcb(port_handle);
  tBTA_JV_RFC_CB* p_cb = bta_jv_rfc_port_to_cb(port_handle);
  tBTA_JV evt_data;
  RawAddress rem_bda = RawAddress::kEmpty;
  uint16_t lcid;
  VLOG(2) << __func__ << ": code=" << code << ", port_handle=" << port_handle;
  if (NULL == p_cb || NULL == p_cb->p_cback) {
    LOG(ERROR) << __func__ << ": p_cb=" << p_cb
               << ", p_cb->p_cback=" << (p_cb ? p_cb->p_cback : 0);
    return;
  }
  uint32_t rfcomm_slot_id = p_pcb->rfcomm_slot_id;
  VLOG(2) << __func__ << ": code=" << code
          << ", port_handle=" << loghex(port_handle)
          << ", handle=" << loghex(p_cb->handle) << ", p_pcb" << p_pcb
          << ", user=" << p_pcb->rfcomm_slot_id;

  int status = PORT_CheckConnection(port_handle, &rem_bda, &lcid);
  int failed = true;
  if (code == PORT_SUCCESS) {
    if (status != PORT_SUCCESS) {
      LOG(ERROR) << __func__ << ": PORT_CheckConnection returned " << status
                 << ", although port is supposed to be connected";
    }
    evt_data.rfc_srv_open.handle = p_pcb->handle;
    evt_data.rfc_srv_open.status = BTA_JV_SUCCESS;
    evt_data.rfc_srv_open.rem_bda = rem_bda;
    tBTA_JV_PCB* p_pcb_new_listen = bta_jv_add_rfc_port(p_cb, p_pcb);
    if (p_pcb_new_listen) {
      evt_data.rfc_srv_open.new_listen_handle = p_pcb_new_listen->handle;
      p_pcb_new_listen->rfcomm_slot_id =
          p_cb->p_cback(BTA_JV_RFCOMM_SRV_OPEN_EVT, &evt_data, rfcomm_slot_id);
      VLOG(2) << __func__ << ": curr_sess=" << p_cb->curr_sess
              << ", max_sess=" << p_cb->max_sess;
      failed = false;
    } else
      LOG(ERROR) << __func__ << ": failed to create new listen port";
  }
  if (failed) {
    evt_data.rfc_close.handle = p_cb->handle;
    evt_data.rfc_close.status = BTA_JV_FAILURE;
    evt_data.rfc_close.async = true;
    evt_data.rfc_close.port_status = code;
    p_pcb->cong = false;

    tBTA_JV_RFCOMM_CBACK* p_cback = p_cb->p_cback;
    VLOG(2) << __func__
            << ": PORT_CLOSED before BTA_JV_RFCOMM_CLOSE_EVT: curr_sess="
            << p_cb->curr_sess << ", max_sess=" << p_cb->max_sess;
    if (BTA_JV_ST_SR_CLOSING == p_pcb->state) {
      evt_data.rfc_close.async = false;
      evt_data.rfc_close.status = BTA_JV_SUCCESS;
    }
    // p_pcb->state = BTA_JV_ST_NONE;
    p_cback(BTA_JV_RFCOMM_CLOSE_EVT, &evt_data, rfcomm_slot_id);
    // bta_jv_free_rfc_cb(p_cb, p_pcb);

    VLOG(2) << __func__
            << ": PORT_CLOSED after BTA_JV_RFCOMM_CLOSE_EVT: curr_sess="
            << p_cb->curr_sess << ", max_sess=" << p_cb->max_sess;
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_port_event_sr_cback
 *
 * Description  Callback for RFCOMM server port events
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_port_event_sr_cback(uint32_t code, uint16_t port_handle) {
  tBTA_JV_PCB* p_pcb = bta_jv_rfc_port_to_pcb(port_handle);
  tBTA_JV_RFC_CB* p_cb = bta_jv_rfc_port_to_cb(port_handle);
  tBTA_JV evt_data;

  if (NULL == p_cb || NULL == p_cb->p_cback) {
    LOG(ERROR) << __func__ << ": p_cb=" << p_cb
               << ", p_cb->p_cback=" << (p_cb ? p_cb->p_cback : 0);
    return;
  }

  VLOG(2) << __func__ << ": code=" << loghex(code)
          << ", port_handle=" << port_handle << ", handle=" << p_cb->handle;

  uint32_t user_data = p_pcb->rfcomm_slot_id;
  if (code & PORT_EV_RXCHAR) {
    evt_data.data_ind.handle = p_cb->handle;
    p_cb->p_cback(BTA_JV_RFCOMM_DATA_IND_EVT, &evt_data, user_data);
  }

  if (code & PORT_EV_FC) {
    p_pcb->cong = (code & PORT_EV_FCS) ? false : true;
    evt_data.rfc_cong.cong = p_pcb->cong;
    evt_data.rfc_cong.handle = p_cb->handle;
    evt_data.rfc_cong.status = BTA_JV_SUCCESS;
    p_cb->p_cback(BTA_JV_RFCOMM_CONG_EVT, &evt_data, user_data);
  }

  if (code & PORT_EV_TXEMPTY) {
    bta_jv_pm_conn_idle(p_pcb->p_pm_cb);
  }
}

/*******************************************************************************
 *
 * Function     bta_jv_add_rfc_port
 *
 * Description  add a port for server when the existing posts is open
 *
 * Returns   return a pointer to tBTA_JV_PCB just added
 *
 ******************************************************************************/
static tBTA_JV_PCB* bta_jv_add_rfc_port(tBTA_JV_RFC_CB* p_cb,
                                        tBTA_JV_PCB* p_pcb_open) {
  uint8_t used = 0, i, listen = 0;
  uint32_t si = 0;
  tPORT_STATE port_state;
  uint32_t event_mask = BTA_JV_RFC_EV_MASK;
  tBTA_JV_PCB* p_pcb = NULL;
  if (p_cb->max_sess > 1) {
    for (i = 0; i < p_cb->max_sess; i++) {
      if (p_cb->rfc_hdl[i] != 0) {
        p_pcb = &bta_jv_cb.port_cb[p_cb->rfc_hdl[i] - 1];
        if (p_pcb->state == BTA_JV_ST_SR_LISTEN) {
          listen++;
          if (p_pcb_open == p_pcb) {
            VLOG(2) << __func__ << ": port_handle=" << p_pcb->port_handle
                    << ", change the listen port to open state";
            p_pcb->state = BTA_JV_ST_SR_OPEN;

          } else {
            LOG(ERROR) << __func__
                       << ": open pcb not matching listen one, count=" << listen
                       << ", listen pcb handle=" << p_pcb->port_handle
                       << ", open pcb=" << p_pcb_open->handle;
            return NULL;
          }
        }
        used++;
      } else if (si == 0) {
        si = i + 1;
      }
    }

    VLOG(2) << __func__ << ": max_sess=" << p_cb->max_sess << ", used=" << used
            << ", curr_sess=" << p_cb->curr_sess << ", listen=" << listen
            << ", si=" << si;
    if (used < p_cb->max_sess && listen == 1 && si) {
      si--;
      if (RFCOMM_CreateConnection(p_cb->sec_id, p_cb->scn, true,
                                  BTA_JV_DEF_RFC_MTU, RawAddress::kAny,
                                  &(p_cb->rfc_hdl[si]),
                                  bta_jv_port_mgmt_sr_cback) == PORT_SUCCESS) {
        p_cb->curr_sess++;
        p_pcb = &bta_jv_cb.port_cb[p_cb->rfc_hdl[si] - 1];
        p_pcb->state = BTA_JV_ST_SR_LISTEN;
        p_pcb->port_handle = p_cb->rfc_hdl[si];
        p_pcb->rfcomm_slot_id = p_pcb_open->rfcomm_slot_id;

        PORT_ClearKeepHandleFlag(p_pcb->port_handle);
        PORT_SetEventCallback(p_pcb->port_handle, bta_jv_port_event_sr_cback);
        PORT_SetDataCOCallback(p_pcb->port_handle, bta_jv_port_data_co_cback);
        PORT_SetEventMask(p_pcb->port_handle, event_mask);
        PORT_GetState(p_pcb->port_handle, &port_state);

        port_state.fc_type = (PORT_FC_CTS_ON_INPUT | PORT_FC_CTS_ON_OUTPUT);

        PORT_SetState(p_pcb->port_handle, &port_state);
        p_pcb->handle = BTA_JV_RFC_H_S_TO_HDL(p_cb->handle, si);
        VLOG(2) << __func__ << ": p_pcb->handle=" << loghex(p_pcb->handle)
                << ", curr_sess=" << p_cb->curr_sess;
      }
    } else {
      LOG(ERROR) << __func__ << ": cannot create new rfc listen port";
      return NULL;
    }
  }
  VLOG(2) << __func__ << ": sec id in use=" << get_sec_id_used()
          << ", rfc_cb in use=" << get_rfc_cb_used();
  return p_pcb;
}

/* waits for an RFCOMM client to connect */
void bta_jv_rfcomm_start_server(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                                uint8_t local_scn, uint8_t max_session,
                                tBTA_JV_RFCOMM_CBACK* p_cback,
                                uint32_t rfcomm_slot_id) {
  uint16_t handle = 0;
  uint32_t event_mask = BTA_JV_RFC_EV_MASK;
  tPORT_STATE port_state;
  uint8_t sec_id = 0;
  tBTA_JV_RFC_CB* p_cb = NULL;
  tBTA_JV_PCB* p_pcb;
  tBTA_JV_RFCOMM_START evt_data;

  /* TODO DM role manager
  L2CA_SetDesireRole(role);
  */
  memset(&evt_data, 0, sizeof(evt_data));
  evt_data.status = BTA_JV_FAILURE;
  VLOG(2) << __func__ << ": sec id in use=" << get_sec_id_used()
          << ", rfc_cb in use=" << get_rfc_cb_used();

  do {
    sec_id = bta_jv_alloc_sec_id();

    if (0 == sec_id ||
        !BTM_SetSecurityLevel(false, "JV PORT", sec_id, sec_mask, BT_PSM_RFCOMM,
                              BTM_SEC_PROTO_RFCOMM, local_scn)) {
      LOG(ERROR) << __func__ << ": run out of sec_id";
      break;
    }

    if (RFCOMM_CreateConnection(sec_id, local_scn, true, BTA_JV_DEF_RFC_MTU,
                                RawAddress::kAny, &handle,
                                bta_jv_port_mgmt_sr_cback) != PORT_SUCCESS) {
      LOG(ERROR) << __func__ << ": RFCOMM_CreateConnection failed";
      break;
    }

    p_cb = bta_jv_alloc_rfc_cb(handle, &p_pcb);
    if (!p_cb) {
      LOG(ERROR) << __func__ << ": run out of rfc control block";
      break;
    }

    p_cb->max_sess = max_session;
    p_cb->p_cback = p_cback;
    p_cb->sec_id = sec_id;
    p_cb->scn = local_scn;
    p_pcb->state = BTA_JV_ST_SR_LISTEN;
    p_pcb->rfcomm_slot_id = rfcomm_slot_id;
    evt_data.status = BTA_JV_SUCCESS;
    evt_data.handle = p_cb->handle;
    evt_data.sec_id = sec_id;
    evt_data.use_co = true;

    PORT_ClearKeepHandleFlag(handle);
    PORT_SetEventCallback(handle, bta_jv_port_event_sr_cback);
    PORT_SetEventMask(handle, event_mask);
    PORT_GetState(handle, &port_state);

    port_state.fc_type = (PORT_FC_CTS_ON_INPUT | PORT_FC_CTS_ON_OUTPUT);

    PORT_SetState(handle, &port_state);
  } while (0);

  tBTA_JV bta_jv;
  bta_jv.rfc_start = evt_data;
  p_cback(BTA_JV_RFCOMM_START_EVT, &bta_jv, rfcomm_slot_id);
  if (bta_jv.rfc_start.status == BTA_JV_SUCCESS) {
    PORT_SetDataCOCallback(handle, bta_jv_port_data_co_cback);
  } else {
    if (sec_id) bta_jv_free_sec_id(&sec_id);
    if (handle) RFCOMM_RemoveConnection(handle);
  }
}

/* stops an RFCOMM server */
void bta_jv_rfcomm_stop_server(uint32_t handle, uint32_t rfcomm_slot_id) {
  if (!handle) {
    LOG(ERROR) << __func__ << ": jv handle is null";
    return;
  }

  VLOG(2) << __func__;
  tBTA_JV_RFC_CB* p_cb = NULL;
  tBTA_JV_PCB* p_pcb = NULL;

  if (!find_rfc_pcb(rfcomm_slot_id, &p_cb, &p_pcb)) return;
  VLOG(2) << __func__ << ": p_pcb=" << p_pcb
          << ", p_pcb->port_handle=" << p_pcb->port_handle;
  bta_jv_free_rfc_cb(p_cb, p_pcb);
  VLOG(2) << __func__ << ": sec id in use=" << get_sec_id_used()
          << ", rfc_cb in use=" << get_rfc_cb_used();
}

/* write data to an RFCOMM connection */
void bta_jv_rfcomm_write(uint32_t handle, uint32_t req_id, tBTA_JV_RFC_CB* p_cb,
                         tBTA_JV_PCB* p_pcb) {
  if (p_pcb->state == BTA_JV_ST_NONE) {
    LOG(ERROR) << __func__ << ": in state BTA_JV_ST_NONE - cannot write";
    return;
  }

  tBTA_JV_RFCOMM_WRITE evt_data;
  evt_data.status = BTA_JV_FAILURE;
  evt_data.handle = handle;
  evt_data.req_id = req_id;
  evt_data.cong = p_pcb->cong;
  evt_data.len = 0;

  bta_jv_pm_conn_busy(p_pcb->p_pm_cb);

  if (!evt_data.cong &&
      PORT_WriteDataCO(p_pcb->port_handle, &evt_data.len) == PORT_SUCCESS) {
    evt_data.status = BTA_JV_SUCCESS;
  }

  // Update congestion flag
  evt_data.cong = p_pcb->cong;

  if (!p_cb->p_cback) {
    LOG(ERROR) << __func__ << ": No JV callback set";
    return;
  }

  tBTA_JV bta_jv;
  bta_jv.rfc_write = evt_data;
  p_cb->p_cback(BTA_JV_RFCOMM_WRITE_EVT, &bta_jv, p_pcb->rfcomm_slot_id);
}

/* Set or free power mode profile for a JV application */
void bta_jv_set_pm_profile(uint32_t handle, tBTA_JV_PM_ID app_id,
                           tBTA_JV_CONN_STATE init_st) {
  tBTA_JV_STATUS status;
  tBTA_JV_PM_CB* p_cb;

  VLOG(2) << __func__ << " handle=" << loghex(handle) << ", app_id=" << app_id
          << ", init_st=" << +init_st;

  /* clear PM control block */
  if (app_id == BTA_JV_PM_ID_CLEAR) {
    status = bta_jv_free_set_pm_profile_cb(handle);

    if (status != BTA_JV_SUCCESS) {
      LOG(WARNING) << __func__ << ": free pm cb failed: reason=" << +status;
    }
  } else /* set PM control block */
  {
    p_cb = bta_jv_alloc_set_pm_profile_cb(handle, app_id);

    if (NULL != p_cb)
      bta_jv_pm_state_change(p_cb, init_st);
    else
      LOG(WARNING) << __func__ << ": failed";
  }
}

/*******************************************************************************
 *
 * Function    bta_jv_pm_conn_busy
 *
 * Description set pm connection busy state (input param safe)
 *
 * Params      p_cb: pm control block of jv connection
 *
 * Returns     void
 *
 ******************************************************************************/
static void bta_jv_pm_conn_busy(tBTA_JV_PM_CB* p_cb) {
  if ((NULL != p_cb) && (BTA_JV_PM_IDLE_ST == p_cb->state))
    bta_jv_pm_state_change(p_cb, BTA_JV_CONN_BUSY);
}

/*******************************************************************************
 *
 * Function    bta_jv_pm_conn_busy
 *
 * Description set pm connection busy state (input param safe)
 *
 * Params      p_cb: pm control block of jv connection
 *
 * Returns     void
 *
 ******************************************************************************/
static void bta_jv_pm_conn_idle(tBTA_JV_PM_CB* p_cb) {
  if ((NULL != p_cb) && (BTA_JV_PM_IDLE_ST != p_cb->state))
    bta_jv_pm_state_change(p_cb, BTA_JV_CONN_IDLE);
}

/*******************************************************************************
 *
 * Function     bta_jv_pm_state_change
 *
 * Description  Notify power manager there is state change
 *
 * Params      p_cb: must be NONE NULL
 *
 * Returns      void
 *
 ******************************************************************************/
static void bta_jv_pm_state_change(tBTA_JV_PM_CB* p_cb,
                                   const tBTA_JV_CONN_STATE state) {
  VLOG(2) << __func__ << ": p_cb=" << p_cb
          << ", handle=" << loghex(p_cb->handle)
          << ", busy/idle_state=" << p_cb->state << ", app_id=" << p_cb->app_id
          << ", conn_state=" << state;

  switch (state) {
    case BTA_JV_CONN_OPEN:
      bta_sys_conn_open(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_CONN_CLOSE:
      bta_sys_conn_close(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_APP_OPEN:
      bta_sys_app_open(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_APP_CLOSE:
      bta_sys_app_close(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_SCO_OPEN:
      bta_sys_sco_open(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_SCO_CLOSE:
      bta_sys_sco_close(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_CONN_IDLE:
      p_cb->state = BTA_JV_PM_IDLE_ST;
      bta_sys_idle(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    case BTA_JV_CONN_BUSY:
      p_cb->state = BTA_JV_PM_BUSY_ST;
      bta_sys_busy(BTA_ID_JV, p_cb->app_id, p_cb->peer_bd_addr);
      break;

    default:
      LOG(WARNING) << __func__ << ": Invalid state=" << +state;
      break;
  }
}
/******************************************************************************/

static struct fc_channel* fcchan_get(uint16_t chan, char create) {
  struct fc_channel* t = fc_channels;
  static tL2CAP_FIXED_CHNL_REG fcr = {
      .pL2CA_FixedConn_Cb = fcchan_conn_chng_cbk,
      .pL2CA_FixedData_Cb = fcchan_data_cbk,
      .default_idle_tout = 0xffff,
      .fixed_chnl_opts =
          {
              .mode = L2CAP_FCR_BASIC_MODE,
              .max_transmit = 0xFF,
              .rtrans_tout = 2000,
              .mon_tout = 12000,
              .mps = 670,
              .tx_win_sz = 1,
          },
  };

  while (t && t->chan != chan) t = t->next;

  if (t)
    return t;
  else if (!create)
    return NULL; /* we cannot alloc a struct if not asked to */

  t = static_cast<struct fc_channel*>(osi_calloc(sizeof(*t)));
  t->chan = chan;

  if (!L2CA_RegisterFixedChannel(chan, &fcr)) {
    osi_free(t);
    return NULL;
  }

  // link it in
  t->next = fc_channels;
  fc_channels = t;

  return t;
}

/* pass NULL to find servers */
static struct fc_client* fcclient_find_by_addr(struct fc_client* start,
                                               const RawAddress* addr) {
  struct fc_client* t = start;

  while (t) {
    /* match client if have addr */
    if (addr && addr == &t->remote_addr) break;

    /* match server if do not have addr */
    if (!addr && t->server) break;

    t = t->next_all_list;
  }

  return t;
}

static struct fc_client* fcclient_find_by_id(uint32_t id) {
  struct fc_client* t = fc_clients;

  while (t && t->id != id) t = t->next_all_list;

  return t;
}

static struct fc_client* fcclient_alloc(uint16_t chan, char server,
                                        const uint8_t* sec_id_to_use) {
  struct fc_channel* fc = fcchan_get(chan, true);
  struct fc_client* t;
  uint8_t sec_id;

  if (!fc) return NULL;

  if (fc->has_server && server)
    return NULL; /* no way to have multiple servers on same channel */

  if (sec_id_to_use)
    sec_id = *sec_id_to_use;
  else
    sec_id = bta_jv_alloc_sec_id();

  t = static_cast<fc_client*>(osi_calloc(sizeof(*t)));
  // Allocate it a unique ID
  do {
    t->id = ++fc_next_id;
  } while (!t->id || fcclient_find_by_id(t->id));

  // Populate some params
  t->chan = chan;
  t->server = server;

  // Get a security id
  t->sec_id = sec_id;

  // Link it in to global list
  t->next_all_list = fc_clients;
  fc_clients = t;

  // Link it in to channel list
  t->next_chan_list = fc->clients;
  fc->clients = t;

  // Update channel if needed
  if (server) fc->has_server = true;

  return t;
}

static void fcclient_free(struct fc_client* fc) {
  struct fc_client* t = fc_clients;
  struct fc_channel* tc = fcchan_get(fc->chan, false);

  // remove from global list
  while (t && t->next_all_list != fc) t = t->next_all_list;

  if (!t && fc != fc_clients) return; /* prevent double-free */

  if (t)
    t->next_all_list = fc->next_all_list;
  else
    fc_clients = fc->next_all_list;

  // remove from channel list
  if (tc) {
    t = tc->clients;

    while (t && t->next_chan_list != fc) t = t->next_chan_list;

    if (t)
      t->next_chan_list = fc->next_chan_list;
    else
      tc->clients = fc->next_chan_list;

    // if was server then channel no longer has a server
    if (fc->server) tc->has_server = false;
  }

  // free security id
  bta_jv_free_sec_id(&fc->sec_id);

  osi_free(fc);
}

static void fcchan_conn_chng_cbk(uint16_t chan, const RawAddress& bd_addr,
                                 bool connected, uint16_t reason,
                                 tBT_TRANSPORT transport) {
  tBTA_JV init_evt;
  tBTA_JV open_evt;
  struct fc_channel* tc;
  struct fc_client *t = NULL, *new_conn;
  tBTA_JV_L2CAP_CBACK* p_cback = NULL;
  char call_init = false;
  uint32_t l2cap_socket_id;

  tc = fcchan_get(chan, false);
  if (tc) {
    t = fcclient_find_by_addr(
        tc->clients,
        &bd_addr);  // try to find an open socked for that addr
    if (t) {
      p_cback = t->p_cback;
      l2cap_socket_id = t->l2cap_socket_id;
    } else {
      t = fcclient_find_by_addr(
          tc->clients,
          NULL);  // try to find a listening socked for that channel
      if (t) {
        // found: create a normal connection socket and assign the connection to
        // it
        new_conn = fcclient_alloc(chan, false, &t->sec_id);
        if (new_conn) {
          new_conn->remote_addr = bd_addr;
          new_conn->p_cback = NULL;     // for now
          new_conn->init_called = true; /*nop need to do it again */

          p_cback = t->p_cback;
          l2cap_socket_id = t->l2cap_socket_id;

          t = new_conn;
        }
      } else {
        // drop it
        return;
      }
    }
  }

  if (t) {
    if (!t->init_called) {
      call_init = true;
      t->init_called = true;

      init_evt.l2c_cl_init.handle = t->id;
      init_evt.l2c_cl_init.status = BTA_JV_SUCCESS;
      init_evt.l2c_cl_init.sec_id = t->sec_id;
    }

    open_evt.l2c_open.handle = t->id;
    open_evt.l2c_open.tx_mtu = 23; /* 23, why not ?*/
    memcpy(&open_evt.l2c_le_open.rem_bda, &t->remote_addr,
           sizeof(open_evt.l2c_le_open.rem_bda));
    // TODO: (apanicke) Change the way these functions work so that casting
    // isn't needed
    open_evt.l2c_le_open.p_p_cback = (void**)&t->p_cback;
    open_evt.l2c_le_open.p_user_data = (void**)&t->l2cap_socket_id;
    open_evt.l2c_le_open.status = BTA_JV_SUCCESS;

    if (connected) {
      open_evt.l2c_open.status = BTA_JV_SUCCESS;
    } else {
      fcclient_free(t);
      open_evt.l2c_open.status = BTA_JV_FAILURE;
    }
  }

  if (call_init) p_cback(BTA_JV_L2CAP_CL_INIT_EVT, &init_evt, l2cap_socket_id);

  // call this with lock taken so socket does not disappear from under us */
  if (p_cback) {
    p_cback(BTA_JV_L2CAP_OPEN_EVT, &open_evt, l2cap_socket_id);
    if (!t->p_cback) /* no callback set, means they do not want this one... */
      fcclient_free(t);
  }
}

static void fcchan_data_cbk(uint16_t chan, const RawAddress& bd_addr,
                            BT_HDR* p_buf) {
  tBTA_JV evt_data;
  struct fc_channel* tc;
  struct fc_client* t = NULL;
  tBTA_JV_L2CAP_CBACK* sock_cback = NULL;
  uint32_t sock_id;

  tc = fcchan_get(chan, false);
  if (tc) {
    // try to find an open socked for that addr and channel
    t = fcclient_find_by_addr(tc->clients, &bd_addr);
  }
  if (!t) {
    // no socket -> drop it
    return;
  }


  sock_cback = t->p_cback;
  sock_id = t->l2cap_socket_id;
  evt_data.le_data_ind.handle = t->id;
  evt_data.le_data_ind.p_buf = p_buf;

  if (sock_cback) sock_cback(BTA_JV_L2CAP_DATA_IND_EVT, &evt_data, sock_id);
}

/** makes an le l2cap client connection */
void bta_jv_l2cap_connect_le(uint16_t remote_chan,
                             const RawAddress& peer_bd_addr,
                             tBTA_JV_L2CAP_CBACK* p_cback,
                             uint32_t l2cap_socket_id) {
  tBTA_JV evt;
  uint32_t id;
  char call_init_f = true;
  struct fc_client* t;

  evt.l2c_cl_init.handle = GAP_INVALID_HANDLE;
  evt.l2c_cl_init.status = BTA_JV_FAILURE;

  t = fcclient_alloc(remote_chan, false, NULL);
  if (!t) {
    p_cback(BTA_JV_L2CAP_CL_INIT_EVT, &evt, l2cap_socket_id);
    return;
  }

  t->p_cback = p_cback;
  t->l2cap_socket_id = l2cap_socket_id;
  t->remote_addr = peer_bd_addr;
  id = t->id;
  t->init_called = false;

  if (L2CA_ConnectFixedChnl(t->chan, t->remote_addr)) {
    evt.l2c_cl_init.status = BTA_JV_SUCCESS;
    evt.l2c_cl_init.handle = id;
  }

  // it could have been deleted/moved from under us, so re-find it */
  t = fcclient_find_by_id(id);
  if (t) {
    if (evt.l2c_cl_init.status == BTA_JV_SUCCESS) {
      call_init_f = !t->init_called;
    } else {
      fcclient_free(t);
      t = NULL;
    }
  }
  if (call_init_f) p_cback(BTA_JV_L2CAP_CL_INIT_EVT, &evt, l2cap_socket_id);
  if (t) {
    t->init_called = true;
  }
}

/* stops an LE L2CAP server */
void bta_jv_l2cap_stop_server_le(uint16_t local_chan) {
  struct fc_client* fcclient;

  tBTA_JV evt;
  evt.l2c_close.status = BTA_JV_FAILURE;
  evt.l2c_close.async = false;
  evt.l2c_close.handle = GAP_INVALID_HANDLE;

  struct fc_channel* fcchan = fcchan_get(local_chan, false);
  if (fcchan) {
    while ((fcclient = fcchan->clients)) {
      tBTA_JV_L2CAP_CBACK* p_cback = fcclient->p_cback;
      uint32_t l2cap_socket_id = fcclient->l2cap_socket_id;

      evt.l2c_close.handle = fcclient->id;
      evt.l2c_close.status = BTA_JV_SUCCESS;
      evt.l2c_close.async = false;

      fcclient_free(fcclient);

      if (p_cback) p_cback(BTA_JV_L2CAP_CLOSE_EVT, &evt, l2cap_socket_id);
    }
  }
}

/** starts an LE L2CAP server */
void bta_jv_l2cap_start_server_le(uint16_t local_chan,
                                  tBTA_JV_L2CAP_CBACK* p_cback,
                                  uint32_t l2cap_socket_id) {
  tBTA_JV_L2CAP_START evt_data;
  evt_data.handle = GAP_INVALID_HANDLE;
  evt_data.status = BTA_JV_FAILURE;

  struct fc_client* t = fcclient_alloc(local_chan, true, NULL);
  if (!t) goto out;

  t->p_cback = p_cback;
  t->l2cap_socket_id = l2cap_socket_id;

  // if we got here, we're registered...
  evt_data.status = BTA_JV_SUCCESS;
  evt_data.handle = t->id;
  evt_data.sec_id = t->sec_id;

out:
  tBTA_JV bta_jv;
  bta_jv.l2c_start = evt_data;
  p_cback(BTA_JV_L2CAP_START_EVT, &bta_jv, l2cap_socket_id);
}

/* close a fixed channel connection. calls no callbacks. idempotent */
extern void bta_jv_l2cap_close_fixed(uint32_t handle) {
  struct fc_client* t = fcclient_find_by_id(handle);
  if (t) fcclient_free(t);
}
