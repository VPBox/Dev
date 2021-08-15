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

/*******************************************************************************
 *
 *  Filename:      btif_hh.c
 *
 *  Description:   HID Host Profile Bluetooth Interface
 *
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_hh"

#include "btif_hh.h"

#include <base/logging.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bt_common.h"
#include "bta_api.h"
#include "btif_common.h"
#include "btif_storage.h"
#include "btif_util.h"
#include "l2c_api.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

#define BTIF_HH_APP_ID_MI 0x01
#define BTIF_HH_APP_ID_KB 0x02

#define COD_HID_KEYBOARD 0x0540
#define COD_HID_POINTING 0x0580
#define COD_HID_COMBO 0x05C0

#define KEYSTATE_FILEPATH \
  "/data/misc/bluedroid/bt_hh_ks"  // keep this in sync with HID host jni

#define HID_REPORT_CAPSLOCK 0x39
#define HID_REPORT_NUMLOCK 0x53
#define HID_REPORT_SCROLLLOCK 0x47

// For Apple Magic Mouse
#define MAGICMOUSE_VENDOR_ID 0x05ac
#define MAGICMOUSE_PRODUCT_ID 0x030d

#define LOGITECH_KB_MX5500_VENDOR_ID 0x046D
#define LOGITECH_KB_MX5500_PRODUCT_ID 0xB30B

extern const int BT_UID;
extern const int BT_GID;
static int btif_hh_keylockstates = 0;  // The current key state of each key

#define BTIF_HH_ID_1 0
#define BTIF_HH_DEV_DISCONNECTED 3

#define BTIF_TIMEOUT_VUP_MS (3 * 1000)

#ifndef BTUI_HH_SECURITY
#define BTUI_HH_SECURITY (BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT)
#endif

#ifndef BTUI_HH_MOUSE_SECURITY
#define BTUI_HH_MOUSE_SECURITY (BTA_SEC_NONE)
#endif

/* HH request events */
typedef enum {
  BTIF_HH_CONNECT_REQ_EVT = 0,
  BTIF_HH_DISCONNECT_REQ_EVT,
  BTIF_HH_VUP_REQ_EVT
} btif_hh_req_evt_t;

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/
#define BTIF_HH_SERVICES (BTA_HID_SERVICE_MASK)

/*******************************************************************************
 *  Local type definitions
 ******************************************************************************/

typedef struct hid_kb_list {
  uint16_t product_id;
  uint16_t version_id;
  const char* kb_name;
} tHID_KB_LIST;

/*******************************************************************************
 *  Static variables
 ******************************************************************************/
btif_hh_cb_t btif_hh_cb;

static bthh_callbacks_t* bt_hh_callbacks = NULL;

/* List of HID keyboards for which the NUMLOCK state needs to be
 * turned ON by default. Add devices to this list to apply the
 * NUMLOCK state toggle on fpr first connect.*/
static tHID_KB_LIST hid_kb_numlock_on_list[] = {{LOGITECH_KB_MX5500_PRODUCT_ID,
                                                 LOGITECH_KB_MX5500_VENDOR_ID,
                                                 "Logitech MX5500 Keyboard"}};

#define CHECK_BTHH_INIT()                                             \
  do {                                                                \
    if (bt_hh_callbacks == NULL) {                                    \
      BTIF_TRACE_WARNING("BTHH: %s: BTHH not initialized", __func__); \
      return BT_STATUS_NOT_READY;                                     \
    }                                                                 \
  } while (0)

/*******************************************************************************
 *  Static functions
 ******************************************************************************/

/*******************************************************************************
 *  Externs
 ******************************************************************************/
extern void bta_hh_co_destroy(int fd);
extern void bta_hh_co_write(int fd, uint8_t* rpt, uint16_t len);
extern bt_status_t btif_dm_remove_bond(const RawAddress* bd_addr);
extern void bta_hh_co_send_hid_info(btif_hh_device_t* p_dev,
                                    const char* dev_name, uint16_t vendor_id,
                                    uint16_t product_id, uint16_t version,
                                    uint8_t ctry_code, int dscp_len,
                                    uint8_t* p_dscp);
extern bool check_cod(const RawAddress* remote_bdaddr, uint32_t cod);
extern void btif_dm_cb_remove_bond(const RawAddress* bd_addr);
extern bool check_cod_hid(const RawAddress* remote_bdaddr);
extern int scru_ascii_2_hex(char* p_ascii, int len, uint8_t* p_hex);
extern void btif_dm_hh_open_failed(RawAddress* bdaddr);
extern void btif_hd_service_registration();

/*****************************************************************************
 *  Local Function prototypes
 ****************************************************************************/
static void set_keylockstate(int keymask, bool isSet);
static void toggle_os_keylockstates(int fd, int changedkeystates);
static void sync_lockstate_on_connect(btif_hh_device_t* p_dev);
// static void hh_update_keyboard_lockstates(btif_hh_device_t *p_dev);
void btif_hh_timer_timeout(void* data);
void bte_hh_evt(tBTA_HH_EVT event, tBTA_HH* p_data);

/*******************************************************************************
 *  Functions
 ******************************************************************************/

static int get_keylockstates() { return btif_hh_keylockstates; }

static void set_keylockstate(int keymask, bool isSet) {
  if (isSet) btif_hh_keylockstates |= keymask;
}

/*******************************************************************************
 *
 * Function         toggle_os_keylockstates
 *
 * Description      Function to toggle the keyboard lock states managed by the
 linux.
 *                  This function is used in by two call paths
 *                  (1) if the lock state change occurred from an onscreen
 keyboard,
 *                  this function is called to update the lock state maintained
                    for the HID keyboard(s)
 *                  (2) if a HID keyboard is disconnected and reconnected,
 *                  this function is called to update the lock state maintained
                    for the HID keyboard(s)
 * Returns          void
 ******************************************************************************/

static void toggle_os_keylockstates(int fd, int changedlockstates) {
  BTIF_TRACE_EVENT("%s: fd = %d, changedlockstates = 0x%x", __func__, fd,
                   changedlockstates);
  uint8_t hidreport[9];
  int reportIndex;
  memset(hidreport, 0, 9);
  hidreport[0] = 1;
  reportIndex = 4;

  if (changedlockstates & BTIF_HH_KEYSTATE_MASK_CAPSLOCK) {
    BTIF_TRACE_DEBUG("%s Setting CAPSLOCK", __func__);
    hidreport[reportIndex++] = (uint8_t)HID_REPORT_CAPSLOCK;
  }

  if (changedlockstates & BTIF_HH_KEYSTATE_MASK_NUMLOCK) {
    BTIF_TRACE_DEBUG("%s Setting NUMLOCK", __func__);
    hidreport[reportIndex++] = (uint8_t)HID_REPORT_NUMLOCK;
  }

  if (changedlockstates & BTIF_HH_KEYSTATE_MASK_SCROLLLOCK) {
    BTIF_TRACE_DEBUG("%s Setting SCROLLLOCK", __func__);
    hidreport[reportIndex++] = (uint8_t)HID_REPORT_SCROLLLOCK;
  }

  BTIF_TRACE_DEBUG(
      "Writing hidreport #1 to os: "
      "%s:  %x %x %x",
      __func__, hidreport[0], hidreport[1], hidreport[2]);
  BTIF_TRACE_DEBUG("%s:  %x %x %x", __func__, hidreport[3], hidreport[4],
                   hidreport[5]);
  BTIF_TRACE_DEBUG("%s:  %x %x %x", __func__, hidreport[6], hidreport[7],
                   hidreport[8]);
  bta_hh_co_write(fd, hidreport, sizeof(hidreport));
  usleep(200000);
  memset(hidreport, 0, 9);
  hidreport[0] = 1;
  BTIF_TRACE_DEBUG(
      "Writing hidreport #2 to os: "
      "%s:  %x %x %x",
      __func__, hidreport[0], hidreport[1], hidreport[2]);
  BTIF_TRACE_DEBUG("%s:  %x %x %x", __func__, hidreport[3], hidreport[4],
                   hidreport[5]);
  BTIF_TRACE_DEBUG("%s:  %x %x %x ", __func__, hidreport[6], hidreport[7],
                   hidreport[8]);
  bta_hh_co_write(fd, hidreport, sizeof(hidreport));
}

/*******************************************************************************
 *
 * Function         create_pbuf
 *
 * Description      Helper function to create p_buf for send_data or set_report
 *
 ******************************************************************************/
static BT_HDR* create_pbuf(uint16_t len, uint8_t* data) {
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(len + BTA_HH_MIN_OFFSET + sizeof(BT_HDR));
  uint8_t* pbuf_data;

  p_buf->len = len;
  p_buf->offset = BTA_HH_MIN_OFFSET;

  pbuf_data = (uint8_t*)(p_buf + 1) + p_buf->offset;
  memcpy(pbuf_data, data, len);

  return p_buf;
}

/*******************************************************************************
 *
 * Function         update_keyboard_lockstates
 *
 * Description      Sends a report to the keyboard to set the lock states of
 *                  keys.
 *
 ******************************************************************************/
static void update_keyboard_lockstates(btif_hh_device_t* p_dev) {
  uint8_t len = 2; /* reportid + 1 byte report*/
  BT_HDR* p_buf;
  uint8_t data[] = {0x01, /* report id */
                    static_cast<uint8_t>(btif_hh_keylockstates)}; /* keystate */

  /* Set report for other keyboards */
  BTIF_TRACE_EVENT("%s: setting report on dev_handle %d to 0x%x", __func__,
                   p_dev->dev_handle, btif_hh_keylockstates);

  /* Get SetReport buffer */
  p_buf = create_pbuf(len, data);
  if (p_buf != NULL) {
    p_buf->layer_specific = BTA_HH_RPTT_OUTPUT;
    BTA_HhSendData(p_dev->dev_handle, p_dev->bd_addr, p_buf);
  }
}

/*******************************************************************************
 *
 * Function         sync_lockstate_on_connect
 *
 * Description      Function to update the keyboard lock states managed by the
 *                  OS when a HID keyboard is connected or disconnected and
 *                  reconnected
 *
 * Returns          void
 ******************************************************************************/
static void sync_lockstate_on_connect(btif_hh_device_t* p_dev) {
  int keylockstates;

  BTIF_TRACE_EVENT(
      "%s: Syncing keyboard lock states after "
      "reconnect...",
      __func__);
  /*If the device is connected, update keyboard state */
  update_keyboard_lockstates(p_dev);

  /*Check if the lockstate of caps,scroll,num is set.
   If so, send a report to the kernel
  so the lockstate is in sync */
  keylockstates = get_keylockstates();
  if (keylockstates) {
    BTIF_TRACE_DEBUG(
        "%s: Sending hid report to kernel "
        "indicating lock key state 0x%x",
        __func__, keylockstates);
    usleep(200000);
    toggle_os_keylockstates(p_dev->fd, keylockstates);
  } else {
    BTIF_TRACE_DEBUG(
        "%s: NOT sending hid report to kernel "
        "indicating lock key state 0x%x",
        __func__, keylockstates);
  }
}

/*******************************************************************************
 *
 * Function         btif_hh_find_connected_dev_by_handle
 *
 * Description      Return the connected device pointer of the specified device
 *                  handle
 *
 * Returns          Device entry pointer in the device table
 ******************************************************************************/
btif_hh_device_t* btif_hh_find_connected_dev_by_handle(uint8_t handle) {
  uint32_t i;
  for (i = 0; i < BTIF_HH_MAX_HID; i++) {
    if (btif_hh_cb.devices[i].dev_status == BTHH_CONN_STATE_CONNECTED &&
        btif_hh_cb.devices[i].dev_handle == handle) {
      return &btif_hh_cb.devices[i];
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         btif_hh_find_dev_by_bda
 *
 * Description      Return the device pointer of the specified RawAddress.
 *
 * Returns          Device entry pointer in the device table
 ******************************************************************************/
static btif_hh_device_t* btif_hh_find_dev_by_bda(const RawAddress& bd_addr) {
  uint32_t i;
  for (i = 0; i < BTIF_HH_MAX_HID; i++) {
    if (btif_hh_cb.devices[i].dev_status != BTHH_CONN_STATE_UNKNOWN &&
        btif_hh_cb.devices[i].bd_addr == bd_addr) {
      return &btif_hh_cb.devices[i];
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         btif_hh_find_connected_dev_by_bda
 *
 * Description      Return the connected device pointer of the specified
 *                  RawAddress.
 *
 * Returns          Device entry pointer in the device table
 ******************************************************************************/
static btif_hh_device_t* btif_hh_find_connected_dev_by_bda(
    const RawAddress& bd_addr) {
  uint32_t i;
  for (i = 0; i < BTIF_HH_MAX_HID; i++) {
    if (btif_hh_cb.devices[i].dev_status == BTHH_CONN_STATE_CONNECTED &&
        btif_hh_cb.devices[i].bd_addr == bd_addr) {
      return &btif_hh_cb.devices[i];
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function      btif_hh_stop_vup_timer
 *
 * Description  stop vitual unplug timer
 *
 * Returns      void
 ******************************************************************************/
void btif_hh_stop_vup_timer(RawAddress* bd_addr) {
  btif_hh_device_t* p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);

  if (p_dev != NULL) {
    BTIF_TRACE_DEBUG("stop VUP timer");
    alarm_free(p_dev->vup_timer);
    p_dev->vup_timer = NULL;
  }
}
/*******************************************************************************
 *
 * Function      btif_hh_start_vup_timer
 *
 * Description  start virtual unplug timer
 *
 * Returns      void
 ******************************************************************************/
void btif_hh_start_vup_timer(const RawAddress* bd_addr) {
  BTIF_TRACE_DEBUG("%s", __func__);

  btif_hh_device_t* p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  CHECK(p_dev != NULL);

  alarm_free(p_dev->vup_timer);
  p_dev->vup_timer = alarm_new("btif_hh.vup_timer");
  alarm_set_on_mloop(p_dev->vup_timer, BTIF_TIMEOUT_VUP_MS,
                     btif_hh_timer_timeout, p_dev);
}

/*******************************************************************************
 *
 * Function         btif_hh_add_added_dev
 *
 * Description      Add a new device to the added device list.
 *
 * Returns          true if add successfully, otherwise false.
 ******************************************************************************/
bool btif_hh_add_added_dev(const RawAddress& bda, tBTA_HH_ATTR_MASK attr_mask) {
  int i;
  for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
    if (btif_hh_cb.added_devices[i].bd_addr == bda) {
      LOG(WARNING) << " Device " << bda << " already added";
      return false;
    }
  }
  for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
    if (btif_hh_cb.added_devices[i].bd_addr.IsEmpty()) {
      LOG(WARNING) << " Added device " << bda;
      btif_hh_cb.added_devices[i].bd_addr = bda;
      btif_hh_cb.added_devices[i].dev_handle = BTA_HH_INVALID_HANDLE;
      btif_hh_cb.added_devices[i].attr_mask = attr_mask;
      return true;
    }
  }

  BTIF_TRACE_WARNING("%s: Error, out of space to add device", __func__);
  return false;
}

/*******************************************************************************
 **
 ** Function         btif_hh_remove_device
 **
 ** Description      Remove an added device from the stack.
 **
 ** Returns          void
 ******************************************************************************/
void btif_hh_remove_device(RawAddress bd_addr) {
  int i;
  btif_hh_device_t* p_dev;
  btif_hh_added_device_t* p_added_dev;

  LOG(INFO) << __func__ << ": bda = " << bd_addr;

  for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
    p_added_dev = &btif_hh_cb.added_devices[i];
    if (p_added_dev->bd_addr == bd_addr) {
      BTA_HhRemoveDev(p_added_dev->dev_handle);
      btif_storage_remove_hid_info(&(p_added_dev->bd_addr));
      memset(&(p_added_dev->bd_addr), 0, 6);
      p_added_dev->dev_handle = BTA_HH_INVALID_HANDLE;
      break;
    }
  }

  p_dev = btif_hh_find_dev_by_bda(bd_addr);
  if (p_dev == NULL) {
    LOG(WARNING) << " Oops, can't find device " << bd_addr;
    return;
  }

  /* need to notify up-layer device is disconnected to avoid state out of sync
   * with up-layer */
  HAL_CBACK(bt_hh_callbacks, connection_state_cb, &(p_dev->bd_addr),
            BTHH_CONN_STATE_DISCONNECTED);

  p_dev->dev_status = BTHH_CONN_STATE_UNKNOWN;
  p_dev->dev_handle = BTA_HH_INVALID_HANDLE;
  p_dev->ready_for_data = false;

  if (btif_hh_cb.device_num > 0) {
    btif_hh_cb.device_num--;
  } else {
    BTIF_TRACE_WARNING("%s: device_num = 0", __func__);
  }

  p_dev->hh_keep_polling = 0;
  p_dev->hh_poll_thread_id = -1;
  BTIF_TRACE_DEBUG("%s: uhid fd = %d", __func__, p_dev->fd);
  if (p_dev->fd >= 0) {
    bta_hh_co_destroy(p_dev->fd);
    p_dev->fd = -1;
  }
}

bool btif_hh_copy_hid_info(tBTA_HH_DEV_DSCP_INFO* dest,
                           tBTA_HH_DEV_DSCP_INFO* src) {
  memset(dest, 0, sizeof(tBTA_HH_DEV_DSCP_INFO));
  dest->descriptor.dl_len = 0;
  if (src->descriptor.dl_len > 0) {
    dest->descriptor.dsc_list = (uint8_t*)osi_malloc(src->descriptor.dl_len);
  }
  memcpy(dest->descriptor.dsc_list, src->descriptor.dsc_list,
         src->descriptor.dl_len);
  dest->descriptor.dl_len = src->descriptor.dl_len;
  dest->vendor_id = src->vendor_id;
  dest->product_id = src->product_id;
  dest->version = src->version;
  dest->ctry_code = src->ctry_code;
  dest->ssr_max_latency = src->ssr_max_latency;
  dest->ssr_min_tout = src->ssr_min_tout;
  return true;
}

/*******************************************************************************
 *
 * Function         btif_hh_virtual_unplug
 *
 * Description      Virtual unplug initiated from the BTIF thread context
 *                  Special handling for HID mouse-
 *
 * Returns          void
 *
 ******************************************************************************/

bt_status_t btif_hh_virtual_unplug(const RawAddress* bd_addr) {
  BTIF_TRACE_DEBUG("%s", __func__);
  btif_hh_device_t* p_dev;
  p_dev = btif_hh_find_dev_by_bda(*bd_addr);
  if ((p_dev != NULL) && (p_dev->dev_status == BTHH_CONN_STATE_CONNECTED) &&
      (p_dev->attr_mask & HID_VIRTUAL_CABLE)) {
    BTIF_TRACE_DEBUG("%s Sending BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG", __func__);
    /* start the timer */
    btif_hh_start_vup_timer(bd_addr);
    p_dev->local_vup = true;
    BTA_HhSendCtrl(p_dev->dev_handle, BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG);
    return BT_STATUS_SUCCESS;
  } else if ((p_dev != NULL) &&
             (p_dev->dev_status == BTHH_CONN_STATE_CONNECTED)) {
    BTIF_TRACE_ERROR("%s: Virtual unplug not suported, disconnecting device");
    /* start the timer */
    btif_hh_start_vup_timer(bd_addr);
    p_dev->local_vup = true;
    BTA_HhClose(p_dev->dev_handle);
    return BT_STATUS_SUCCESS;
  } else {
    BTIF_TRACE_ERROR("%s: Error, device %s not opened, status = %d", __func__,
                     bd_addr->ToString().c_str(), btif_hh_cb.status);
    if ((btif_hh_cb.pending_conn_address == *bd_addr) &&
       (btif_hh_cb.status == BTIF_HH_DEV_CONNECTING)) {
          btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
          btif_hh_cb.pending_conn_address = RawAddress::kEmpty;
    }
    return BT_STATUS_FAIL;
  }
}

/*******************************************************************************
 *
 * Function         btif_hh_connect
 *
 * Description      connection initiated from the BTIF thread context
 *
 * Returns          int status
 *
 ******************************************************************************/

bt_status_t btif_hh_connect(const RawAddress* bd_addr) {
  btif_hh_added_device_t* added_dev = NULL;
  CHECK_BTHH_INIT();
  BTIF_TRACE_EVENT("BTHH: %s", __func__);
  btif_hh_device_t* dev = btif_hh_find_dev_by_bda(*bd_addr);
  if (!dev && btif_hh_cb.device_num >= BTIF_HH_MAX_HID) {
    // No space for more HID device now.
    BTIF_TRACE_WARNING(
        "%s: Error, exceeded the maximum supported HID device number %d",
        __func__, BTIF_HH_MAX_HID);
    return BT_STATUS_FAIL;
  }

  for (int i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
    if (btif_hh_cb.added_devices[i].bd_addr == *bd_addr) {
      added_dev = &btif_hh_cb.added_devices[i];
      LOG(WARNING) << __func__ << ": Device " << *bd_addr
                   << " already added, attr_mask = 0x" << std::hex
                   << added_dev->attr_mask;
    }
  }

  if (added_dev != NULL) {
    if (added_dev->dev_handle == BTA_HH_INVALID_HANDLE) {
      // No space for more HID device now.
      LOG(ERROR) << __func__ << ": Error, device " << *bd_addr
                 << " added but addition failed";
      added_dev->bd_addr = RawAddress::kEmpty;
      added_dev->dev_handle = BTA_HH_INVALID_HANDLE;
      return BT_STATUS_FAIL;
    }
  }

  /* Not checking the NORMALLY_Connectible flags from sdp record, and anyways
   sending this
   request from host, for subsequent user initiated connection. If the remote is
   not in
   pagescan mode, we will do 2 retries to connect before giving up */
  tBTA_SEC sec_mask = BTUI_HH_SECURITY;
  btif_hh_cb.status = BTIF_HH_DEV_CONNECTING;
  btif_hh_cb.pending_conn_address = *bd_addr;
  BTA_HhOpen(*bd_addr, BTA_HH_PROTO_RPT_MODE, sec_mask);

  // TODO(jpawlowski); make cback accept const and remove tmp!
  auto tmp = *bd_addr;
  HAL_CBACK(bt_hh_callbacks, connection_state_cb, &tmp,
            BTHH_CONN_STATE_CONNECTING);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_hh_disconnect
 *
 * Description      disconnection initiated from the BTIF thread context
 *
 * Returns          void
 *
 ******************************************************************************/

void btif_hh_disconnect(RawAddress* bd_addr) {
  btif_hh_device_t* p_dev;
  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev != NULL) {
    BTA_HhClose(p_dev->dev_handle);
  } else
    BTIF_TRACE_DEBUG("%s-- Error: device not connected:", __func__);
}

/*******************************************************************************
 *
 * Function         btif_btif_hh_setreport
 *
 * Description      setreport initiated from the BTIF thread context
 *
 * Returns          void
 *
 ******************************************************************************/
void btif_hh_setreport(btif_hh_device_t* p_dev, bthh_report_type_t r_type,
                       uint16_t size, uint8_t* report) {
  BT_HDR* p_buf = create_pbuf(size, report);
  if (p_buf == NULL) {
    APPL_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, size = %d",
                     __func__, size);
    return;
  }
  BTA_HhSetReport(p_dev->dev_handle, r_type, p_buf);
}

/*******************************************************************************
 *
 * Function         btif_hh_service_registration
 *
 * Description      Registers or derigisters the hid host service
 *
 * Returns          none
 *
 ******************************************************************************/
void btif_hh_service_registration(bool enable) {
  BTIF_TRACE_API("%s", __func__);

  BTIF_TRACE_API("enable = %d", enable);
  if (bt_hh_callbacks == NULL) {
    // The HID Host service was never initialized (it is either disabled or not
    // available in this build). We should proceed directly to changing the HID
    // Device service state (if needed).
    if (!enable) {
      btif_hd_service_registration();
    }
  } else if (enable) {
    BTA_HhEnable(BTA_SEC_ENCRYPT, bte_hh_evt);
  } else {
    btif_hh_cb.service_dereg_active = TRUE;
    BTA_HhDisable();
  }
}

/*******************************************************************************
 *
 *
 * Function         btif_hh_getreport
 *
 * Description      getreport initiated from the BTIF thread context
 *
 * Returns          void
 *
 ******************************************************************************/
void btif_hh_getreport(btif_hh_device_t* p_dev, bthh_report_type_t r_type,
                       uint8_t reportId, uint16_t bufferSize) {
  BTA_HhGetReport(p_dev->dev_handle, r_type, reportId, bufferSize);
}

/*****************************************************************************
 *   Section name (Group of functions)
 ****************************************************************************/

/*****************************************************************************
 *
 *   btif hh api functions (no context switch)
 *
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         btif_hh_upstreams_evt
 *
 * Description      Executes HH UPSTREAMS events in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_hh_upstreams_evt(uint16_t event, char* p_param) {
  tBTA_HH* p_data = (tBTA_HH*)p_param;
  btif_hh_device_t* p_dev = NULL;
  int i;
  int len, tmplen;

  BTIF_TRACE_DEBUG("%s: event=%s dereg = %d", __func__, dump_hh_event(event),
                   btif_hh_cb.service_dereg_active);

  switch (event) {
    case BTA_HH_ENABLE_EVT:
      BTIF_TRACE_DEBUG("%s: BTA_HH_ENABLE_EVT: status =%d", __func__,
                       p_data->status);
      if (p_data->status == BTA_HH_OK) {
        btif_hh_cb.status = BTIF_HH_ENABLED;
        BTIF_TRACE_DEBUG("%s--Loading added devices", __func__);
        /* Add hid descriptors for already bonded hid devices*/
        btif_storage_load_bonded_hid_info();
      } else {
        btif_hh_cb.status = BTIF_HH_DISABLED;
        BTIF_TRACE_WARNING(
            "BTA_HH_ENABLE_EVT: Error, HH enabling failed, status = %d",
            p_data->status);
      }
      break;

    case BTA_HH_DISABLE_EVT:
      btif_hh_cb.status = BTIF_HH_DISABLED;
      if (btif_hh_cb.service_dereg_active) {
        BTIF_TRACE_DEBUG("BTA_HH_DISABLE_EVT: enabling HID Device service");
        btif_hd_service_registration();
        btif_hh_cb.service_dereg_active = FALSE;
      }
      if (p_data->status == BTA_HH_OK) {
        int i;
        // Clear the control block
        for (i = 0; i < BTIF_HH_MAX_HID; i++) {
          alarm_free(btif_hh_cb.devices[i].vup_timer);
        }
        memset(&btif_hh_cb, 0, sizeof(btif_hh_cb));
        for (i = 0; i < BTIF_HH_MAX_HID; i++) {
          btif_hh_cb.devices[i].dev_status = BTHH_CONN_STATE_UNKNOWN;
        }
      } else
        BTIF_TRACE_WARNING(
            "BTA_HH_DISABLE_EVT: Error, HH disabling failed, status = %d",
            p_data->status);
      break;

    case BTA_HH_OPEN_EVT:
      BTIF_TRACE_WARNING("%s: BTA_HH_OPN_EVT: handle=%d, status =%d", __func__,
                         p_data->conn.handle, p_data->conn.status);
      btif_hh_cb.pending_conn_address = RawAddress::kEmpty;
      if (p_data->conn.status == BTA_HH_OK) {
        p_dev = btif_hh_find_connected_dev_by_handle(p_data->conn.handle);
        if (p_dev == NULL) {
          BTIF_TRACE_WARNING(
              "BTA_HH_OPEN_EVT: Error, cannot find device with handle %d",
              p_data->conn.handle);
          btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
          // The connect request must come from device side and exceeded the
          // connected
          // HID device number.
          BTA_HhClose(p_data->conn.handle);
          HAL_CBACK(bt_hh_callbacks, connection_state_cb,
                    (RawAddress*)&p_data->conn.bda,
                    BTHH_CONN_STATE_DISCONNECTED);
        } else if (p_dev->fd < 0) {
          BTIF_TRACE_WARNING(
              "BTA_HH_OPEN_EVT: Error, failed to find the uhid driver...");
          p_dev->bd_addr = p_data->conn.bda;
          // remove the connection  and then try again to reconnect from the
          // mouse side to recover
          btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
          BTA_HhClose(p_data->conn.handle);
        } else {
          BTIF_TRACE_WARNING(
              "BTA_HH_OPEN_EVT: Found device...Getting dscp info for handle "
              "... %d",
              p_data->conn.handle);
          p_dev->bd_addr = p_data->conn.bda;
          btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_CONNECTED;
          // Send set_idle if the peer_device is a keyboard
          if (check_cod(&p_data->conn.bda, COD_HID_KEYBOARD) ||
              check_cod(&p_data->conn.bda, COD_HID_COMBO))
            BTA_HhSetIdle(p_data->conn.handle, 0);
          btif_hh_cb.p_curr_dev =
              btif_hh_find_connected_dev_by_handle(p_data->conn.handle);
          BTA_HhGetDscpInfo(p_data->conn.handle);
          p_dev->dev_status = BTHH_CONN_STATE_CONNECTED;
          HAL_CBACK(bt_hh_callbacks, connection_state_cb, &(p_dev->bd_addr),
                    p_dev->dev_status);
        }
      } else {
        RawAddress* bdaddr = &p_data->conn.bda;
        btif_dm_hh_open_failed(bdaddr);
        p_dev = btif_hh_find_dev_by_bda(*bdaddr);
        if (p_dev != NULL) {
          btif_hh_stop_vup_timer(&(p_dev->bd_addr));
          if (p_dev->fd >= 0) {
            bta_hh_co_destroy(p_dev->fd);
            p_dev->fd = -1;
          }
          p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;
        }
        HAL_CBACK(bt_hh_callbacks, connection_state_cb,
                  (RawAddress*)&p_data->conn.bda, BTHH_CONN_STATE_DISCONNECTED);
        btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
      }
      break;

    case BTA_HH_CLOSE_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_CLOSE_EVT: status = %d, handle = %d",
                       p_data->dev_status.status, p_data->dev_status.handle);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
      if (p_dev != NULL) {
        BTIF_TRACE_DEBUG("%s: uhid fd=%d local_vup=%d", __func__, p_dev->fd,
                         p_dev->local_vup);
        btif_hh_stop_vup_timer(&(p_dev->bd_addr));
        /* If this is a locally initiated VUP, remove the bond as ACL got
         *  disconnected while VUP being processed.
         */
        if (p_dev->local_vup) {
          p_dev->local_vup = false;
          BTA_DmRemoveDevice(p_dev->bd_addr);
        }

        btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
        p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;

        if (p_dev->fd >= 0) {
          bta_hh_co_destroy(p_dev->fd);
          p_dev->fd = -1;
        }
        HAL_CBACK(bt_hh_callbacks, connection_state_cb, &(p_dev->bd_addr),
                  p_dev->dev_status);
      } else {
        BTIF_TRACE_WARNING("Error: cannot find device with handle %d",
                           p_data->dev_status.handle);
      }
      break;

    case BTA_HH_GET_RPT_EVT: {
      BT_HDR* hdr = p_data->hs_data.rsp_data.p_rpt_data;
      uint8_t* data = NULL;
      uint16_t len = 0;

      BTIF_TRACE_DEBUG("BTA_HH_GET_RPT_EVT: status = %d, handle = %d",
                       p_data->hs_data.status, p_data->hs_data.handle);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->hs_data.handle);
      if (p_dev) {
        /* p_rpt_data is NULL in HANDSHAKE response case */
        if (hdr) {
          data = (uint8_t*)(hdr + 1) + hdr->offset;
          len = hdr->len;
          HAL_CBACK(bt_hh_callbacks, get_report_cb,
                    (RawAddress*)&(p_dev->bd_addr),
                    (bthh_status_t)p_data->hs_data.status, data, len);
        } else {
          HAL_CBACK(bt_hh_callbacks, handshake_cb,
                    (RawAddress*)&(p_dev->bd_addr),
                    (bthh_status_t)p_data->hs_data.status);
        }
      } else {
        BTIF_TRACE_WARNING("Error: cannot find device with handle %d",
                           p_data->hs_data.handle);
      }
      break;
    }

    case BTA_HH_SET_RPT_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_SET_RPT_EVT: status = %d, handle = %d",
                       p_data->dev_status.status, p_data->dev_status.handle);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
      if (p_dev != NULL) {
        HAL_CBACK(bt_hh_callbacks, handshake_cb, (RawAddress*)&(p_dev->bd_addr),
                  (bthh_status_t)p_data->hs_data.status);
      }
      break;

    case BTA_HH_GET_PROTO_EVT:
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->hs_data.handle);
      BTIF_TRACE_WARNING(
          "BTA_HH_GET_PROTO_EVT: status = %d, handle = %d, proto = [%d], %s",
          p_data->hs_data.status, p_data->hs_data.handle,
          p_data->hs_data.rsp_data.proto_mode,
          (p_data->hs_data.rsp_data.proto_mode == BTA_HH_PROTO_RPT_MODE)
              ? "Report Mode"
              : (p_data->hs_data.rsp_data.proto_mode == BTA_HH_PROTO_BOOT_MODE)
                    ? "Boot Mode"
                    : "Unsupported");
      if (p_data->hs_data.rsp_data.proto_mode != BTA_HH_PROTO_UNKNOWN) {
        HAL_CBACK(bt_hh_callbacks, protocol_mode_cb,
                  (RawAddress*)&(p_dev->bd_addr),
                  (bthh_status_t)p_data->hs_data.status,
                  (bthh_protocol_mode_t)p_data->hs_data.rsp_data.proto_mode);
      } else {
        HAL_CBACK(bt_hh_callbacks, handshake_cb, (RawAddress*)&(p_dev->bd_addr),
                  (bthh_status_t)p_data->hs_data.status);
      }
      break;

    case BTA_HH_SET_PROTO_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_SET_PROTO_EVT: status = %d, handle = %d",
                       p_data->dev_status.status, p_data->dev_status.handle);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
      if (p_dev) {
        HAL_CBACK(bt_hh_callbacks, handshake_cb, (RawAddress*)&(p_dev->bd_addr),
                  (bthh_status_t)p_data->hs_data.status);
      }
      break;

    case BTA_HH_GET_IDLE_EVT:
      BTIF_TRACE_DEBUG(
          "BTA_HH_GET_IDLE_EVT: handle = %d, status = %d, rate = %d",
          p_data->hs_data.handle, p_data->hs_data.status,
          p_data->hs_data.rsp_data.idle_rate);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->hs_data.handle);
      HAL_CBACK(bt_hh_callbacks, idle_time_cb, (RawAddress*)&(p_dev->bd_addr),
                (bthh_status_t)p_data->hs_data.status,
                p_data->hs_data.rsp_data.idle_rate);
      break;

    case BTA_HH_SET_IDLE_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_SET_IDLE_EVT: status = %d, handle = %d",
                       p_data->dev_status.status, p_data->dev_status.handle);
      break;

    case BTA_HH_GET_DSCP_EVT:
      len = p_data->dscp_info.descriptor.dl_len;
      BTIF_TRACE_DEBUG("BTA_HH_GET_DSCP_EVT: len = %d", len);
      p_dev = btif_hh_cb.p_curr_dev;
      if (p_dev == NULL) {
        BTIF_TRACE_ERROR(
            "BTA_HH_GET_DSCP_EVT: No HID device is currently connected");
        return;
      }
      if (p_dev->fd < 0) {
        LOG_ERROR(
            LOG_TAG,
            "BTA_HH_GET_DSCP_EVT: Error, failed to find the uhid driver...");
        return;
      }
      {
        const char* cached_name = NULL;
        bt_bdname_t bdname;
        bt_property_t prop_name;
        BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_BDNAME,
                                   sizeof(bt_bdname_t), &bdname);
        if (btif_storage_get_remote_device_property(
                &p_dev->bd_addr, &prop_name) == BT_STATUS_SUCCESS) {
          cached_name = (char*)bdname.name;
        } else {
          cached_name = "Bluetooth HID";
        }

        BTIF_TRACE_WARNING("%s: name = %s", __func__, cached_name);
        bta_hh_co_send_hid_info(p_dev, cached_name, p_data->dscp_info.vendor_id,
                                p_data->dscp_info.product_id,
                                p_data->dscp_info.version,
                                p_data->dscp_info.ctry_code, len,
                                p_data->dscp_info.descriptor.dsc_list);
        if (btif_hh_add_added_dev(p_dev->bd_addr, p_dev->attr_mask)) {
          tBTA_HH_DEV_DSCP_INFO dscp_info;
          bt_status_t ret;
          btif_hh_copy_hid_info(&dscp_info, &p_data->dscp_info);
          VLOG(1) << "BTA_HH_GET_DSCP_EVT:bda = " << p_dev->bd_addr;
          BTA_HhAddDev(p_dev->bd_addr, p_dev->attr_mask, p_dev->sub_class,
                       p_dev->app_id, dscp_info);
          // write hid info to nvram
          ret = btif_storage_add_hid_device_info(
              &(p_dev->bd_addr), p_dev->attr_mask, p_dev->sub_class,
              p_dev->app_id, p_data->dscp_info.vendor_id,
              p_data->dscp_info.product_id, p_data->dscp_info.version,
              p_data->dscp_info.ctry_code, p_data->dscp_info.ssr_max_latency,
              p_data->dscp_info.ssr_min_tout, len,
              p_data->dscp_info.descriptor.dsc_list);

          ASSERTC(ret == BT_STATUS_SUCCESS, "storing hid info failed", ret);
          BTIF_TRACE_WARNING("BTA_HH_GET_DSCP_EVT: Called add device");

          // Free buffer created for dscp_info;
          if (dscp_info.descriptor.dl_len > 0 &&
              dscp_info.descriptor.dsc_list != NULL) {
            osi_free_and_reset((void**)&dscp_info.descriptor.dsc_list);
            dscp_info.descriptor.dl_len = 0;
          }
        } else {
          // Device already added.
          BTIF_TRACE_WARNING("%s: Device already added ", __func__);
        }
        /*Sync HID Keyboard lockstates */
        tmplen = sizeof(hid_kb_numlock_on_list) / sizeof(tHID_KB_LIST);
        for (i = 0; i < tmplen; i++) {
          if (p_data->dscp_info.vendor_id ==
                  hid_kb_numlock_on_list[i].version_id &&
              p_data->dscp_info.product_id ==
                  hid_kb_numlock_on_list[i].product_id) {
            BTIF_TRACE_DEBUG(
                "%s() idx[%d] Enabling "
                "NUMLOCK for device :: %s",
                __func__, i, hid_kb_numlock_on_list[i].kb_name);
            /* Enable NUMLOCK by default so that numeric
                keys work from first keyboard connect */
            set_keylockstate(BTIF_HH_KEYSTATE_MASK_NUMLOCK, true);
            sync_lockstate_on_connect(p_dev);
            /* End Sync HID Keyboard lockstates */
            break;
          }
        }
      }
      break;

    case BTA_HH_ADD_DEV_EVT:
      BTIF_TRACE_WARNING("BTA_HH_ADD_DEV_EVT: status = %d, handle = %d",
                         p_data->dev_info.status, p_data->dev_info.handle);
      int i;
      for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
        if (btif_hh_cb.added_devices[i].bd_addr == p_data->dev_info.bda) {
          if (p_data->dev_info.status == BTA_HH_OK) {
            btif_hh_cb.added_devices[i].dev_handle = p_data->dev_info.handle;
          } else {
            btif_hh_cb.added_devices[i].bd_addr = RawAddress::kEmpty;
            btif_hh_cb.added_devices[i].dev_handle = BTA_HH_INVALID_HANDLE;
          }
          break;
        }
      }
      break;
    case BTA_HH_RMV_DEV_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_RMV_DEV_EVT: status = %d, handle = %d",
                       p_data->dev_info.status, p_data->dev_info.handle);
      VLOG(1) << "BTA_HH_RMV_DEV_EVT:bda = " << p_data->dev_info.bda;
      break;

    case BTA_HH_VC_UNPLUG_EVT:
      BTIF_TRACE_DEBUG("BTA_HH_VC_UNPLUG_EVT: status = %d, handle = %d",
                       p_data->dev_status.status, p_data->dev_status.handle);
      p_dev = btif_hh_find_connected_dev_by_handle(p_data->dev_status.handle);
      btif_hh_cb.status = (BTIF_HH_STATUS)BTIF_HH_DEV_DISCONNECTED;
      if (p_dev != NULL) {
        VLOG(1) << "BTA_HH_VC_UNPLUG_EVT:bda = " << p_dev->bd_addr;

        /* Stop the VUP timer */
        btif_hh_stop_vup_timer(&(p_dev->bd_addr));
        p_dev->dev_status = BTHH_CONN_STATE_DISCONNECTED;
        BTIF_TRACE_DEBUG("%s---Sending connection state change", __func__);
        HAL_CBACK(bt_hh_callbacks, connection_state_cb, &(p_dev->bd_addr),
                  p_dev->dev_status);
        BTIF_TRACE_DEBUG("%s---Removing HID bond", __func__);
        /* If it is locally initiated VUP or remote device has its major COD as
        Peripheral removed the bond.*/
        if (p_dev->local_vup || check_cod_hid(&(p_dev->bd_addr))) {
          p_dev->local_vup = false;
          BTA_DmRemoveDevice(p_dev->bd_addr);
        } else
          btif_hh_remove_device(p_dev->bd_addr);
        HAL_CBACK(bt_hh_callbacks, virtual_unplug_cb, &(p_dev->bd_addr),
                  (bthh_status_t)p_data->dev_status.status);
      }
      break;

    case BTA_HH_API_ERR_EVT:
      LOG_INFO(LOG_TAG, "BTA_HH API_ERR");
      break;

    default:
      BTIF_TRACE_WARNING("%s: Unhandled event: %d", __func__, event);
      break;
  }
}

/*******************************************************************************
 *
 * Function         bte_hh_evt
 *
 * Description      Switches context from BTE to BTIF for all HH events
 *
 * Returns          void
 *
 ******************************************************************************/

void bte_hh_evt(tBTA_HH_EVT event, tBTA_HH* p_data) {
  bt_status_t status;
  int param_len = 0;

  if (BTA_HH_ENABLE_EVT == event)
    param_len = sizeof(tBTA_HH_STATUS);
  else if (BTA_HH_OPEN_EVT == event)
    param_len = sizeof(tBTA_HH_CONN);
  else if (BTA_HH_DISABLE_EVT == event)
    param_len = sizeof(tBTA_HH_STATUS);
  else if (BTA_HH_CLOSE_EVT == event)
    param_len = sizeof(tBTA_HH_CBDATA);
  else if (BTA_HH_GET_DSCP_EVT == event)
    param_len = sizeof(tBTA_HH_DEV_DSCP_INFO);
  else if ((BTA_HH_GET_PROTO_EVT == event) || (BTA_HH_GET_RPT_EVT == event) ||
           (BTA_HH_GET_IDLE_EVT == event))
    param_len = sizeof(tBTA_HH_HSDATA);
  else if ((BTA_HH_SET_PROTO_EVT == event) || (BTA_HH_SET_RPT_EVT == event) ||
           (BTA_HH_VC_UNPLUG_EVT == event) || (BTA_HH_SET_IDLE_EVT == event))
    param_len = sizeof(tBTA_HH_CBDATA);
  else if ((BTA_HH_ADD_DEV_EVT == event) || (BTA_HH_RMV_DEV_EVT == event))
    param_len = sizeof(tBTA_HH_DEV_INFO);
  else if (BTA_HH_API_ERR_EVT == event)
    param_len = 0;
  /* switch context to btif task context (copy full union size for convenience)
   */
  status = btif_transfer_context(btif_hh_upstreams_evt, (uint16_t)event,
                                 (char*)p_data, param_len, NULL);

  /* catch any failed context transfers */
  ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}

/*******************************************************************************
 *
 * Function         btif_hh_handle_evt
 *
 * Description      Switches context for immediate callback
 *
 * Returns          void
 *
 ******************************************************************************/

static void btif_hh_handle_evt(uint16_t event, char* p_param) {
  RawAddress* bd_addr = (RawAddress*)p_param;
  BTIF_TRACE_EVENT("%s: event=%d", __func__, event);
  int ret;
  switch (event) {
    case BTIF_HH_CONNECT_REQ_EVT: {
      ret = btif_hh_connect(bd_addr);
      if (ret == BT_STATUS_SUCCESS) {
        HAL_CBACK(bt_hh_callbacks, connection_state_cb, bd_addr,
                  BTHH_CONN_STATE_CONNECTING);
      } else
        HAL_CBACK(bt_hh_callbacks, connection_state_cb, bd_addr,
                  BTHH_CONN_STATE_DISCONNECTED);
    } break;

    case BTIF_HH_DISCONNECT_REQ_EVT: {
      BTIF_TRACE_EVENT("%s: event=%d", __func__, event);
      btif_hh_disconnect(bd_addr);
      HAL_CBACK(bt_hh_callbacks, connection_state_cb, bd_addr,
                BTHH_CONN_STATE_DISCONNECTING);
    } break;

    case BTIF_HH_VUP_REQ_EVT: {
      BTIF_TRACE_EVENT("%s: event=%d", __func__, event);
      ret = btif_hh_virtual_unplug(bd_addr);
    } break;

    default: {
      BTIF_TRACE_WARNING("%s : Unknown event 0x%x", __func__, event);
    } break;
  }
}

/*******************************************************************************
 *
 * Function      btif_hh_timer_timeout
 *
 * Description   Process timer timeout
 *
 * Returns      void
 ******************************************************************************/
void btif_hh_timer_timeout(void* data) {
  btif_hh_device_t* p_dev = (btif_hh_device_t*)data;
  tBTA_HH_EVT event = BTA_HH_VC_UNPLUG_EVT;
  tBTA_HH p_data;
  int param_len = sizeof(tBTA_HH_CBDATA);

  BTIF_TRACE_DEBUG("%s", __func__);
  if (p_dev->dev_status != BTHH_CONN_STATE_CONNECTED) return;

  memset(&p_data, 0, sizeof(tBTA_HH));
  p_data.dev_status.status = BTHH_ERR;
  p_data.dev_status.handle = p_dev->dev_handle;

  /* switch context to btif task context */
  btif_transfer_context(btif_hh_upstreams_evt, (uint16_t)event, (char*)&p_data,
                        param_len, NULL);
}

/*******************************************************************************
 *
 * Function         btif_hh_init
 *
 * Description     initializes the hh interface
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t init(bthh_callbacks_t* callbacks) {
  uint32_t i;
  BTIF_TRACE_EVENT("%s", __func__);

  bt_hh_callbacks = callbacks;
  memset(&btif_hh_cb, 0, sizeof(btif_hh_cb));
  for (i = 0; i < BTIF_HH_MAX_HID; i++) {
    btif_hh_cb.devices[i].dev_status = BTHH_CONN_STATE_UNKNOWN;
  }
  /* Invoke the enable service API to the core to set the appropriate service_id
   */
  btif_enable_service(BTA_HID_SERVICE_ID);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function        connect
 *
 * Description     connect to hid device
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t connect(RawAddress* bd_addr) {
  if (btif_hh_cb.status != BTIF_HH_DEV_CONNECTING) {
    btif_transfer_context(btif_hh_handle_evt, BTIF_HH_CONNECT_REQ_EVT,
                          (char*)bd_addr, sizeof(RawAddress), NULL);
    return BT_STATUS_SUCCESS;
  } else
    return BT_STATUS_BUSY;
}

/*******************************************************************************
 *
 * Function         disconnect
 *
 * Description      disconnect from hid device
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t disconnect(RawAddress* bd_addr) {
  CHECK_BTHH_INIT();
  BTIF_TRACE_EVENT("BTHH: %s", __func__);
  btif_hh_device_t* p_dev;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_WARNING("%s: Error, HH status = %d", __func__,
                       btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }
  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev != NULL) {
    return btif_transfer_context(btif_hh_handle_evt, BTIF_HH_DISCONNECT_REQ_EVT,
                                 (char*)bd_addr, sizeof(RawAddress), NULL);
  } else {
    BTIF_TRACE_WARNING("%s: Error, device  not opened.", __func__);
    return BT_STATUS_FAIL;
  }
}

/*******************************************************************************
 *
 * Function         virtual_unplug
 *
 * Description      Virtual UnPlug (VUP) the specified HID device.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t virtual_unplug(RawAddress* bd_addr) {
  CHECK_BTHH_INIT();
  BTIF_TRACE_EVENT("BTHH: %s", __func__);
  btif_hh_device_t* p_dev;
  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }
  p_dev = btif_hh_find_dev_by_bda(*bd_addr);
  if (!p_dev) {
    BTIF_TRACE_ERROR("%s: Error, device %s not opened.", __func__,
                     bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }
  btif_transfer_context(btif_hh_handle_evt, BTIF_HH_VUP_REQ_EVT, (char*)bd_addr,
                        sizeof(RawAddress), NULL);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
**
** Function         get_idle_time
**
** Description      Get the HID idle time
**
** Returns         bt_status_t
**
*******************************************************************************/
static bt_status_t get_idle_time(RawAddress* bd_addr) {
  CHECK_BTHH_INIT();

  BTIF_TRACE_DEBUG("%s: addr = %s", __func__, bd_addr->ToString().c_str());

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  btif_hh_device_t* p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) return BT_STATUS_FAIL;

  BTA_HhGetIdle(p_dev->dev_handle);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
**
** Function         set_idle_time
**
** Description      Set the HID idle time
**
** Returns         bt_status_t
**
*******************************************************************************/
static bt_status_t set_idle_time(RawAddress* bd_addr, uint8_t idle_time) {
  CHECK_BTHH_INIT();

  BTIF_TRACE_DEBUG("%s: addr = %s, idle time = %d", __func__,
                   bd_addr->ToString().c_str(), idle_time);

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  btif_hh_device_t* p_dev = p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) {
    BTIF_TRACE_WARNING("%s: addr = %s not opened", __func__,
                       bd_addr->ToString().c_str());
    return BT_STATUS_FAIL;
  }

  BTA_HhSetIdle(p_dev->dev_handle, idle_time);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         set_info
 *
 * Description      Set the HID device descriptor for the specified HID device.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t set_info(RawAddress* bd_addr, bthh_hid_info_t hid_info) {
  CHECK_BTHH_INIT();
  tBTA_HH_DEV_DSCP_INFO dscp_info;

  VLOG(1) << __func__ << " BTHH: addr = " << *bd_addr;
  BTIF_TRACE_DEBUG(
      "BTHH: %s: sub_class = 0x%02x, app_id = %d, vendor_id = 0x%04x, "
      "product_id = 0x%04x, version= 0x%04x",
      __func__, hid_info.sub_class, hid_info.app_id, hid_info.vendor_id,
      hid_info.product_id, hid_info.version);

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  memset(&dscp_info, 0, sizeof(dscp_info));
  dscp_info.vendor_id = hid_info.vendor_id;
  dscp_info.product_id = hid_info.product_id;
  dscp_info.version = hid_info.version;
  dscp_info.ctry_code = hid_info.ctry_code;

  dscp_info.descriptor.dl_len = hid_info.dl_len;
  dscp_info.descriptor.dsc_list =
      (uint8_t*)osi_malloc(dscp_info.descriptor.dl_len);
  memcpy(dscp_info.descriptor.dsc_list, &(hid_info.dsc_list), hid_info.dl_len);

  if (btif_hh_add_added_dev(*bd_addr, hid_info.attr_mask)) {
    BTA_HhAddDev(*bd_addr, hid_info.attr_mask, hid_info.sub_class,
                 hid_info.app_id, dscp_info);
  }

  osi_free_and_reset((void**)&dscp_info.descriptor.dsc_list);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         get_protocol
 *
 * Description      Get the HID proto mode.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t get_protocol(RawAddress* bd_addr,
                                UNUSED_ATTR bthh_protocol_mode_t protocolMode) {
  CHECK_BTHH_INIT();

  VLOG(1) << __func__ << " BTHH: addr = " << *bd_addr;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  btif_hh_device_t* p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (!p_dev) return BT_STATUS_FAIL;

  BTA_HhGetProtoMode(p_dev->dev_handle);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         set_protocol
 *
 * Description      Set the HID proto mode.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t set_protocol(RawAddress* bd_addr,
                                bthh_protocol_mode_t protocolMode) {
  CHECK_BTHH_INIT();
  btif_hh_device_t* p_dev;
  uint8_t proto_mode = protocolMode;

  VLOG(1) << __func__ << " BTHH: proto_mod=" << protocolMode
          << " addr = " << *bd_addr;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) {
    LOG(WARNING) << " Error, device" << *bd_addr << " not opened";
    return BT_STATUS_FAIL;
  } else if (protocolMode != BTA_HH_PROTO_RPT_MODE &&
             protocolMode != BTA_HH_PROTO_BOOT_MODE) {
    BTIF_TRACE_WARNING("%s: Error, device proto_mode = %d.", __func__,
                       proto_mode);
    return BT_STATUS_FAIL;
  } else {
    BTA_HhSetProtoMode(p_dev->dev_handle, protocolMode);
  }

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         get_report
 *
 * Description      Send a GET_REPORT to HID device.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t get_report(RawAddress* bd_addr,
                              bthh_report_type_t reportType, uint8_t reportId,
                              int bufferSize) {
  CHECK_BTHH_INIT();
  btif_hh_device_t* p_dev;

  VLOG(1) << __func__ << " BTHH: r_type = " << reportType
          << ", rpt_id = " << reportId << ", buf_size = " << bufferSize
          << " addr = " << *bd_addr;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) {
    LOG(ERROR) << " Error, device" << *bd_addr << " not opened";
    return BT_STATUS_FAIL;
  } else if (((int)reportType) <= BTA_HH_RPTT_RESRV ||
             ((int)reportType) > BTA_HH_RPTT_FEATURE) {
    LOG(ERROR) << " Error, report type=" << +reportType << " not supported";
    return BT_STATUS_FAIL;
  } else {
    BTA_HhGetReport(p_dev->dev_handle, reportType, reportId, bufferSize);
  }

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         set_report
 *
 * Description      Send a SET_REPORT to HID device.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t set_report(RawAddress* bd_addr,
                              bthh_report_type_t reportType, char* report) {
  CHECK_BTHH_INIT();
  btif_hh_device_t* p_dev;

  VLOG(1) << __func__ << " BTHH: reportType=" << reportType
          << " addr=" << *bd_addr;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) {
    LOG(ERROR) << " Error, device" << *bd_addr << " not opened";
    return BT_STATUS_FAIL;
  } else if (((int)reportType) <= BTA_HH_RPTT_RESRV ||
             ((int)reportType) > BTA_HH_RPTT_FEATURE) {
    LOG(ERROR) << " Error, report type=" << +reportType << " not supported";
    return BT_STATUS_FAIL;
  } else {
    int hex_bytes_filled;
    size_t len = (strlen(report) + 1) / 2;
    uint8_t* hexbuf = (uint8_t*)osi_calloc(len);

    /* Build a SetReport data buffer */
    // TODO
    hex_bytes_filled = ascii_2_hex(report, len, hexbuf);
    LOG_INFO(LOG_TAG, "Hex bytes filled, hex value: %d", hex_bytes_filled);
    if (hex_bytes_filled) {
      BT_HDR* p_buf = create_pbuf(hex_bytes_filled, hexbuf);
      if (p_buf == NULL) {
        BTIF_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, len = %d",
                         __func__, hex_bytes_filled);
        osi_free(hexbuf);
        return BT_STATUS_FAIL;
      }
      BTA_HhSetReport(p_dev->dev_handle, reportType, p_buf);
      osi_free(hexbuf);
      return BT_STATUS_SUCCESS;
    }
    osi_free(hexbuf);
    return BT_STATUS_FAIL;
  }
}

/*******************************************************************************
 *
 * Function         send_data
 *
 * Description      Send a SEND_DATA to HID device.
 *
 * Returns         bt_status_t
 *
 ******************************************************************************/
static bt_status_t send_data(RawAddress* bd_addr, char* data) {
  CHECK_BTHH_INIT();
  btif_hh_device_t* p_dev;

  VLOG(1) << __func__ << " addr=" << *bd_addr;

  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_ERROR("%s: Error, HH status = %d", __func__, btif_hh_cb.status);
    return BT_STATUS_FAIL;
  }

  p_dev = btif_hh_find_connected_dev_by_bda(*bd_addr);
  if (p_dev == NULL) {
    LOG(ERROR) << " Error, device" << *bd_addr << " not opened";
    return BT_STATUS_FAIL;
  }

  else {
    int hex_bytes_filled;
    size_t len = (strlen(data) + 1) / 2;
    uint8_t* hexbuf = (uint8_t*)osi_calloc(len);

    /* Build a SendData data buffer */
    hex_bytes_filled = ascii_2_hex(data, len, hexbuf);
    BTIF_TRACE_ERROR("Hex bytes filled, hex value: %d, %d", hex_bytes_filled,
                     len);

    if (hex_bytes_filled) {
      BT_HDR* p_buf = create_pbuf(hex_bytes_filled, hexbuf);
      if (p_buf == NULL) {
        BTIF_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, len = %d",
                         __func__, hex_bytes_filled);
        osi_free(hexbuf);
        return BT_STATUS_FAIL;
      }
      p_buf->layer_specific = BTA_HH_RPTT_OUTPUT;
      BTA_HhSendData(p_dev->dev_handle, *bd_addr, p_buf);
      osi_free(hexbuf);
      return BT_STATUS_SUCCESS;
    }
    osi_free(hexbuf);
    return BT_STATUS_FAIL;
  }
}

/*******************************************************************************
 *
 * Function         cleanup
 *
 * Description      Closes the HH interface
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
static void cleanup(void) {
  BTIF_TRACE_EVENT("%s", __func__);
  btif_hh_device_t* p_dev;
  int i;
  if (btif_hh_cb.status == BTIF_HH_DISABLED) {
    BTIF_TRACE_WARNING("%s: HH disabling or disabled already, status = %d",
                       __func__, btif_hh_cb.status);
    return;
  }
  if (bt_hh_callbacks) {
    btif_hh_cb.status = BTIF_HH_DISABLING;
    /* update flag, not to enable hid device service now as BT is switching off
     */
    btif_hh_cb.service_dereg_active = FALSE;
    btif_disable_service(BTA_HID_SERVICE_ID);
    bt_hh_callbacks = NULL;
  }
  for (i = 0; i < BTIF_HH_MAX_HID; i++) {
    p_dev = &btif_hh_cb.devices[i];
    if (p_dev->dev_status != BTHH_CONN_STATE_UNKNOWN && p_dev->fd >= 0) {
      BTIF_TRACE_DEBUG("%s: Closing uhid fd = %d", __func__, p_dev->fd);
      if (p_dev->fd >= 0) {
        bta_hh_co_destroy(p_dev->fd);
        p_dev->fd = -1;
      }
      p_dev->hh_keep_polling = 0;
      p_dev->hh_poll_thread_id = -1;
    }
  }

}

static const bthh_interface_t bthhInterface = {
    sizeof(bthhInterface),
    init,
    connect,
    disconnect,
    virtual_unplug,
    set_info,
    get_protocol,
    set_protocol,
    get_idle_time,
    set_idle_time,
    get_report,
    set_report,
    send_data,
    cleanup,
};

/*******************************************************************************
 *
 * Function         btif_hh_execute_service
 *
 * Description      Initializes/Shuts down the service
 *
 * Returns          BT_STATUS_SUCCESS on success, BT_STATUS_FAIL otherwise
 *
 ******************************************************************************/
bt_status_t btif_hh_execute_service(bool b_enable) {
  if (b_enable) {
    /* Enable and register with BTA-HH */
    BTA_HhEnable(BTUI_HH_SECURITY, bte_hh_evt);
  } else {
    /* Disable HH */
    BTA_HhDisable();
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_hh_get_interface
 *
 * Description      Get the hh callback interface
 *
 * Returns          bthh_interface_t
 *
 ******************************************************************************/
const bthh_interface_t* btif_hh_get_interface() {
  BTIF_TRACE_EVENT("%s", __func__);
  return &bthhInterface;
}
