/******************************************************************************
 *
 *  Copyright 2014 The Android Open Source Project
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
 *  Filename:      btif_core.c
 *
 *  Description:   Contains core functionality related to interfacing between
 *                 Bluetooth HAL and BTE core stack.
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_core"

#include <base/at_exit.h>
#include <base/bind.h>
#include <base/run_loop.h>
#include <base/threading/platform_thread.h>
#include <base/threading/thread.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <hardware/bluetooth.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "bt_common.h"
#include "bt_utils.h"
#include "bta_api.h"
#include "bte.h"
#include "btif_api.h"
#include "btif_av.h"
#include "btif_config.h"
#include "btif_pan.h"
#include "btif_profile_queue.h"
#include "btif_sock.h"
#include "btif_storage.h"
#include "btif_uid.h"
#include "btif_util.h"
#include "btu.h"
#include "common/message_loop_thread.h"
#include "device/include/controller.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/future.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"
#include "stack_manager.h"

using base::PlatformThread;
using bluetooth::Uuid;
using bluetooth::common::MessageLoopThread;

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/

#ifndef BTE_DID_CONF_FILE
// TODO(armansito): Find a better way than searching by a hardcoded path.
#if defined(OS_GENERIC)
#define BTE_DID_CONF_FILE "bt_did.conf"
#else  // !defined(OS_GENERIC)
#define BTE_DID_CONF_FILE "/etc/bluetooth/bt_did.conf"
#endif  // defined(OS_GENERIC)
#endif  // BTE_DID_CONF_FILE

/*******************************************************************************
 *  Local type definitions
 ******************************************************************************/

/* These type definitions are used when passing data from the HAL to BTIF
 * context in the downstream path for the adapter and remote_device property
 * APIs
 */

typedef struct {
  RawAddress bd_addr;
  bt_property_type_t type;
} btif_storage_read_t;

typedef struct {
  RawAddress bd_addr;
  bt_property_t prop;
} btif_storage_write_t;

typedef union {
  btif_storage_read_t read_req;
  btif_storage_write_t write_req;
} btif_storage_req_t;

typedef enum {
  BTIF_CORE_STATE_DISABLED = 0,
  BTIF_CORE_STATE_ENABLING,
  BTIF_CORE_STATE_ENABLED,
  BTIF_CORE_STATE_DISABLING
} btif_core_state_t;

/*******************************************************************************
 *  Static variables
 ******************************************************************************/

static tBTA_SERVICE_MASK btif_enabled_services = 0;

/*
 * This variable should be set to 1, if the Bluedroid+BTIF libraries are to
 * function in DUT mode.
 *
 * To set this, the btif_init_bluetooth needs to be called with argument as 1
 */
static uint8_t btif_dut_mode = 0;

static MessageLoopThread jni_thread("bt_jni_thread");
static base::AtExitManager* exit_manager;
static uid_set_t* uid_set;

/*******************************************************************************
 *  Static functions
 ******************************************************************************/
static void btif_jni_associate();
static void btif_jni_disassociate();

/* sends message to btif task */
static void btif_sendmsg(void* p_msg);

/*******************************************************************************
 *  Externs
 ******************************************************************************/
extern fixed_queue_t* btu_hci_msg_queue;

void btif_dm_execute_service_request(uint16_t event, char* p_param);
#ifdef BTIF_DM_OOB_TEST
void btif_dm_load_local_oob(void);
#endif

/*******************************************************************************
 *
 * Function         btif_context_switched
 *
 * Description      Callback used to execute transferred context callback
 *
 *                  p_msg : message to be executed in btif context
 *
 * Returns          void
 *
 ******************************************************************************/

static void btif_context_switched(void* p_msg) {
  BTIF_TRACE_VERBOSE("btif_context_switched");

  tBTIF_CONTEXT_SWITCH_CBACK* p = (tBTIF_CONTEXT_SWITCH_CBACK*)p_msg;

  /* each callback knows how to parse the data */
  if (p->p_cb) p->p_cb(p->event, p->p_param);
}

/*******************************************************************************
 *
 * Function         btif_transfer_context
 *
 * Description      This function switches context to btif task
 *
 *                  p_cback   : callback used to process message in btif context
 *                  event     : event id of message
 *                  p_params  : parameter area passed to callback (copied)
 *                  param_len : length of parameter area
 *                  p_copy_cback : If set this function will be invoked for deep
 *                                 copy
 *
 * Returns          void
 *
 ******************************************************************************/

bt_status_t btif_transfer_context(tBTIF_CBACK* p_cback, uint16_t event,
                                  char* p_params, int param_len,
                                  tBTIF_COPY_CBACK* p_copy_cback) {
  tBTIF_CONTEXT_SWITCH_CBACK* p_msg = (tBTIF_CONTEXT_SWITCH_CBACK*)osi_malloc(
      sizeof(tBTIF_CONTEXT_SWITCH_CBACK) + param_len);

  BTIF_TRACE_VERBOSE("btif_transfer_context event %d, len %d", event,
                     param_len);

  /* allocate and send message that will be executed in btif context */
  p_msg->hdr.event = BT_EVT_CONTEXT_SWITCH_EVT; /* internal event */
  p_msg->p_cb = p_cback;

  p_msg->event = event; /* callback event */

  /* check if caller has provided a copy callback to do the deep copy */
  if (p_copy_cback) {
    p_copy_cback(event, p_msg->p_param, p_params);
  } else if (p_params) {
    memcpy(p_msg->p_param, p_params, param_len); /* callback parameter data */
  }

  btif_sendmsg(p_msg);

  return BT_STATUS_SUCCESS;
}

/**
 * This function posts a task into the btif message loop, that executes it in
 * the JNI message loop.
 **/
bt_status_t do_in_jni_thread(const base::Location& from_here,
                             base::OnceClosure task) {
  if (!jni_thread.DoInThread(from_here, std::move(task))) {
    LOG(ERROR) << __func__ << ": Post task to task runner failed!";
    return BT_STATUS_FAIL;
  }
  return BT_STATUS_SUCCESS;
}

bt_status_t do_in_jni_thread(base::OnceClosure task) {
  return do_in_jni_thread(FROM_HERE, std::move(task));
}

bool is_on_jni_thread() {
  return jni_thread.GetThreadId() == PlatformThread::CurrentId();
}

base::MessageLoop* get_jni_message_loop() { return jni_thread.message_loop(); }

/*******************************************************************************
 *
 * Function         btif_is_dut_mode
 *
 * Description      checks if BTIF is currently in DUT mode
 *
 * Returns          true if test mode, otherwise false
 *
 ******************************************************************************/

bool btif_is_dut_mode() { return btif_dut_mode == 1; }

/*******************************************************************************
 *
 * Function         btif_is_enabled
 *
 * Description      checks if main adapter is fully enabled
 *
 * Returns          1 if fully enabled, otherwize 0
 *
 ******************************************************************************/

int btif_is_enabled(void) {
  return ((!btif_is_dut_mode()) &&
          (stack_manager_get_interface()->get_stack_is_running()));
}

void btif_init_ok(UNUSED_ATTR uint16_t event, UNUSED_ATTR char* p_param) {
  BTIF_TRACE_DEBUG("btif_task: received trigger stack init event");
  btif_dm_load_ble_local_keys();
  BTA_EnableBluetooth(bte_dm_evt);
}

/*******************************************************************************
 *
 * Function         btif_task
 *
 * Description      BTIF task handler managing all messages being passed
 *                  Bluetooth HAL and BTA.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bt_jni_msg_ready(void* context) {
  BT_HDR* p_msg = (BT_HDR*)context;

  BTIF_TRACE_VERBOSE("btif task fetched event %x", p_msg->event);

  switch (p_msg->event) {
    case BT_EVT_CONTEXT_SWITCH_EVT:
      btif_context_switched(p_msg);
      break;
    default:
      BTIF_TRACE_ERROR("unhandled btif event (%d)", p_msg->event & BT_EVT_MASK);
      break;
  }
  osi_free(p_msg);
}

/*******************************************************************************
 *
 * Function         btif_sendmsg
 *
 * Description      Sends msg to BTIF task
 *
 * Returns          void
 *
 ******************************************************************************/

void btif_sendmsg(void* p_msg) {
  do_in_jni_thread(base::Bind(&bt_jni_msg_ready, p_msg));
}

/*******************************************************************************
 *
 * Function         btif_init_bluetooth
 *
 * Description      Creates BTIF task and prepares BT scheduler for startup
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_init_bluetooth() {
  LOG_INFO(LOG_TAG, "%s entered", __func__);
  exit_manager = new base::AtExitManager();
  bte_main_boot_entry();
  jni_thread.StartUp();
  jni_thread.DoInThread(FROM_HERE, base::Bind(btif_jni_associate));
  LOG_INFO(LOG_TAG, "%s finished", __func__);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_enable_bluetooth_evt
 *
 * Description      Event indicating bluetooth enable is completed
 *                  Notifies HAL user with updated adapter state
 *
 * Returns          void
 *
 ******************************************************************************/

void btif_enable_bluetooth_evt(tBTA_STATUS status) {
  LOG_INFO(LOG_TAG, "%s entered: status %d", __func__, status);

  /* Fetch the local BD ADDR */
  RawAddress local_bd_addr = *controller_get_interface()->get_address();

  std::string bdstr = local_bd_addr.ToString();

  char val[PROPERTY_VALUE_MAX] = "";
  int val_size = 0;
  if ((btif_config_get_str("Adapter", "Address", val, &val_size) == 0) ||
      strcmp(bdstr.c_str(), val) == 0) {
    // This address is not present in the config file, save it there.
    BTIF_TRACE_WARNING("%s: Saving the Adapter Address", __func__);
    btif_config_set_str("Adapter", "Address", bdstr.c_str());
    btif_config_save();

    // fire HAL callback for property change
    bt_property_t prop;
    prop.type = BT_PROPERTY_BDADDR;
    prop.val = (void*)&local_bd_addr;
    prop.len = sizeof(RawAddress);
    HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, BT_STATUS_SUCCESS, 1,
              &prop);
  }

  bte_main_postload_cfg();

  /* callback to HAL */
  if (status == BTA_SUCCESS) {
    uid_set = uid_set_create();

    btif_dm_init(uid_set);

    /* init rfcomm & l2cap api */
    btif_sock_init(uid_set);

    /* init pan */
    btif_pan_init();

    /* load did configuration */
    bte_load_did_conf(BTE_DID_CONF_FILE);

#ifdef BTIF_DM_OOB_TEST
    btif_dm_load_local_oob();
#endif

    future_ready(stack_manager_get_hack_future(), FUTURE_SUCCESS);
  } else {
    /* cleanup rfcomm & l2cap api */
    btif_sock_cleanup();

    btif_pan_cleanup();

    future_ready(stack_manager_get_hack_future(), FUTURE_FAIL);
  }

  LOG_INFO(LOG_TAG, "%s finished", __func__);
}

/*******************************************************************************
 *
 * Function         btif_disable_bluetooth
 *
 * Description      Inititates shutdown of Bluetooth system.
 *                  Any active links will be dropped and device entering
 *                  non connectable/discoverable mode
 *
 * Returns          void
 *
 ******************************************************************************/
bt_status_t btif_disable_bluetooth() {
  LOG_INFO(LOG_TAG, "%s entered", __func__);

  do_in_main_thread(FROM_HERE, base::Bind(&btm_ble_multi_adv_cleanup));
  // TODO(jpawlowski): this should do whole BTA_VendorCleanup(), but it would
  // kill the stack now.

  btif_dm_on_disable();
  /* cleanup rfcomm & l2cap api */
  btif_sock_cleanup();
  btif_pan_cleanup();
  BTA_DisableBluetooth();

  LOG_INFO(LOG_TAG, "%s finished", __func__);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_disable_bluetooth_evt
 *
 * Description      Event notifying BT disable is now complete.
 *                  Terminates main stack tasks and notifies HAL
 *                  user with updated BT state.
 *
 * Returns          void
 *
 ******************************************************************************/

void btif_disable_bluetooth_evt() {
  LOG_INFO(LOG_TAG, "%s entered", __func__);

  bte_main_disable();

  /* callback to HAL */
  future_ready(stack_manager_get_hack_future(), FUTURE_SUCCESS);

  LOG_INFO(LOG_TAG, "%s finished", __func__);
}

/*******************************************************************************
 *
 * Function         btif_cleanup_bluetooth
 *
 * Description      Cleanup BTIF state.
 *
 * Returns          void
 *
 ******************************************************************************/

bt_status_t btif_cleanup_bluetooth() {
  LOG_INFO(LOG_TAG, "%s entered", __func__);
  do_in_main_thread(FROM_HERE, base::Bind(&BTA_VendorCleanup));
  btif_dm_cleanup();
  jni_thread.DoInThread(FROM_HERE, base::BindOnce(btif_jni_disassociate));
  btif_queue_release();
  jni_thread.ShutDown();
  bte_main_cleanup();
  delete exit_manager;
  exit_manager = nullptr;
  btif_dut_mode = 0;
  LOG_INFO(LOG_TAG, "%s finished", __func__);
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dut_mode_cback
 *
 * Description     Callback invoked on completion of vendor specific test mode
 *                 command
 *
 * Returns          None
 *
 ******************************************************************************/
static void btif_dut_mode_cback(UNUSED_ATTR tBTM_VSC_CMPL* p) {
  /* For now nothing to be done. */
}

/*******************************************************************************
 *
 * Function         btif_dut_mode_configure
 *
 * Description      Configure Test Mode - 'enable' to 1 puts the device in test
 *                       mode and 0 exits test mode
 *
 * Returns          BT_STATUS_SUCCESS on success
 *
 ******************************************************************************/
bt_status_t btif_dut_mode_configure(uint8_t enable) {
  BTIF_TRACE_DEBUG("%s", __func__);

  if (!stack_manager_get_interface()->get_stack_is_running()) {
    BTIF_TRACE_ERROR("btif_dut_mode_configure : Bluetooth not enabled");
    return BT_STATUS_NOT_READY;
  }

  btif_dut_mode = enable;
  if (enable == 1) {
    BTA_EnableTestMode();
  } else {
    BTA_DisableTestMode();
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dut_mode_send
 *
 * Description     Sends a HCI Vendor specific command to the controller
 *
 * Returns          BT_STATUS_SUCCESS on success
 *
 ******************************************************************************/
bt_status_t btif_dut_mode_send(uint16_t opcode, uint8_t* buf, uint8_t len) {
  /* TODO: Check that opcode is a vendor command group */
  BTIF_TRACE_DEBUG("%s", __func__);
  if (!btif_is_dut_mode()) {
    BTIF_TRACE_ERROR("Bluedroid HAL needs to be init with test_mode set to 1.");
    return BT_STATUS_FAIL;
  }
  BTM_VendorSpecificCommand(opcode, len, buf, btif_dut_mode_cback);
  return BT_STATUS_SUCCESS;
}

/*****************************************************************************
 *
 *   btif api adapter property functions
 *
 ****************************************************************************/

static bt_status_t btif_in_get_adapter_properties(void) {
  const static uint32_t NUM_ADAPTER_PROPERTIES = 8;
  bt_property_t properties[NUM_ADAPTER_PROPERTIES];
  uint32_t num_props = 0;

  RawAddress addr;
  bt_bdname_t name;
  bt_scan_mode_t mode;
  uint32_t disc_timeout;
  RawAddress bonded_devices[BTM_SEC_MAX_DEVICE_RECORDS];
  Uuid local_uuids[BT_MAX_NUM_UUIDS];
  bt_status_t status;
  bt_io_cap_t local_bt_io_cap;
  bt_io_cap_t local_bt_io_cap_ble;

  /* RawAddress */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props], BT_PROPERTY_BDADDR,
                             sizeof(addr), &addr);
  status = btif_storage_get_adapter_property(&properties[num_props]);
  // Add BT_PROPERTY_BDADDR property into list only when successful.
  // Otherwise, skip this property entry.
  if (status == BT_STATUS_SUCCESS) {
    num_props++;
  }

  /* BD_NAME */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props], BT_PROPERTY_BDNAME,
                             sizeof(name), &name);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  /* SCAN_MODE */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props],
                             BT_PROPERTY_ADAPTER_SCAN_MODE, sizeof(mode),
                             &mode);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  /* DISC_TIMEOUT */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props],
                             BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT,
                             sizeof(disc_timeout), &disc_timeout);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  /* BONDED_DEVICES */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props],
                             BT_PROPERTY_ADAPTER_BONDED_DEVICES,
                             sizeof(bonded_devices), bonded_devices);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  /* LOCAL UUIDs */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props], BT_PROPERTY_UUIDS,
                             sizeof(local_uuids), local_uuids);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  /* LOCAL IO Capabilities */
  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props], BT_PROPERTY_LOCAL_IO_CAPS,
                             sizeof(bt_io_cap_t), &local_bt_io_cap);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  BTIF_STORAGE_FILL_PROPERTY(&properties[num_props],
                             BT_PROPERTY_LOCAL_IO_CAPS_BLE, sizeof(bt_io_cap_t),
                             &local_bt_io_cap_ble);
  btif_storage_get_adapter_property(&properties[num_props]);
  num_props++;

  HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, BT_STATUS_SUCCESS, num_props,
            properties);

  return BT_STATUS_SUCCESS;
}

static bt_status_t btif_in_get_remote_device_properties(RawAddress* bd_addr) {
  bt_property_t remote_properties[8];
  uint32_t num_props = 0;

  bt_bdname_t name, alias;
  uint32_t cod, devtype;
  Uuid remote_uuids[BT_MAX_NUM_UUIDS];

  memset(remote_properties, 0, sizeof(remote_properties));
  BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_BDNAME,
                             sizeof(name), &name);
  btif_storage_get_remote_device_property(bd_addr,
                                          &remote_properties[num_props]);
  num_props++;

  BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props],
                             BT_PROPERTY_REMOTE_FRIENDLY_NAME, sizeof(alias),
                             &alias);
  btif_storage_get_remote_device_property(bd_addr,
                                          &remote_properties[num_props]);
  num_props++;

  BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props],
                             BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
  btif_storage_get_remote_device_property(bd_addr,
                                          &remote_properties[num_props]);
  num_props++;

  BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props],
                             BT_PROPERTY_TYPE_OF_DEVICE, sizeof(devtype),
                             &devtype);
  btif_storage_get_remote_device_property(bd_addr,
                                          &remote_properties[num_props]);
  num_props++;

  BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_UUIDS,
                             sizeof(remote_uuids), remote_uuids);
  btif_storage_get_remote_device_property(bd_addr,
                                          &remote_properties[num_props]);
  num_props++;

  HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, BT_STATUS_SUCCESS,
            bd_addr, num_props, remote_properties);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         execute_storage_request
 *
 * Description      Executes adapter storage request in BTIF context
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

static void execute_storage_request(uint16_t event, char* p_param) {
  bt_status_t status = BT_STATUS_SUCCESS;

  BTIF_TRACE_EVENT("execute storage request event : %d", event);

  switch (event) {
    case BTIF_CORE_STORAGE_ADAPTER_WRITE: {
      btif_storage_req_t* p_req = (btif_storage_req_t*)p_param;
      bt_property_t* p_prop = &(p_req->write_req.prop);
      BTIF_TRACE_EVENT("type: %d, len %d, 0x%x", p_prop->type, p_prop->len,
                       p_prop->val);

      status = btif_storage_set_adapter_property(p_prop);
      HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 1, p_prop);
    } break;

    case BTIF_CORE_STORAGE_ADAPTER_READ: {
      btif_storage_req_t* p_req = (btif_storage_req_t*)p_param;
      char buf[512];
      bt_property_t prop;
      prop.type = p_req->read_req.type;
      prop.val = (void*)buf;
      prop.len = sizeof(buf);
      if (prop.type == BT_PROPERTY_LOCAL_LE_FEATURES) {
        tBTM_BLE_VSC_CB cmn_vsc_cb;
        bt_local_le_features_t local_le_features;

        /* LE features are not stored in storage. Should be retrived from stack
         */
        BTM_BleGetVendorCapabilities(&cmn_vsc_cb);
        local_le_features.local_privacy_enabled = BTM_BleLocalPrivacyEnabled();

        prop.len = sizeof(bt_local_le_features_t);
        if (cmn_vsc_cb.filter_support == 1)
          local_le_features.max_adv_filter_supported = cmn_vsc_cb.max_filter;
        else
          local_le_features.max_adv_filter_supported = 0;
        local_le_features.max_adv_instance = cmn_vsc_cb.adv_inst_max;
        local_le_features.max_irk_list_size = cmn_vsc_cb.max_irk_list_sz;
        local_le_features.rpa_offload_supported = cmn_vsc_cb.rpa_offloading;
        local_le_features.scan_result_storage_size =
            cmn_vsc_cb.tot_scan_results_strg;
        local_le_features.activity_energy_info_supported =
            cmn_vsc_cb.energy_support;
        local_le_features.version_supported = cmn_vsc_cb.version_supported;
        local_le_features.total_trackable_advertisers =
            cmn_vsc_cb.total_trackable_advertisers;

        local_le_features.extended_scan_support =
            cmn_vsc_cb.extended_scan_support > 0;
        local_le_features.debug_logging_supported =
            cmn_vsc_cb.debug_logging_supported > 0;
        memcpy(prop.val, &local_le_features, prop.len);
      } else {
        status = btif_storage_get_adapter_property(&prop);
      }
      HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 1, &prop);
    } break;

    case BTIF_CORE_STORAGE_ADAPTER_READ_ALL: {
      status = btif_in_get_adapter_properties();
    } break;

    case BTIF_CORE_STORAGE_NOTIFY_STATUS: {
      HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 0, NULL);
    } break;

    default:
      BTIF_TRACE_ERROR("%s invalid event id (%d)", __func__, event);
      break;
  }
}

static void execute_storage_remote_request(uint16_t event, char* p_param) {
  bt_status_t status = BT_STATUS_FAIL;
  bt_property_t prop;

  BTIF_TRACE_EVENT("execute storage remote request event : %d", event);

  switch (event) {
    case BTIF_CORE_STORAGE_REMOTE_READ: {
      char buf[1024];
      btif_storage_req_t* p_req = (btif_storage_req_t*)p_param;
      prop.type = p_req->read_req.type;
      prop.val = (void*)buf;
      prop.len = sizeof(buf);

      status = btif_storage_get_remote_device_property(
          &(p_req->read_req.bd_addr), &prop);
      HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, status,
                &(p_req->read_req.bd_addr), 1, &prop);
    } break;
    case BTIF_CORE_STORAGE_REMOTE_WRITE: {
      btif_storage_req_t* p_req = (btif_storage_req_t*)p_param;
      status = btif_storage_set_remote_device_property(
          &(p_req->write_req.bd_addr), &(p_req->write_req.prop));
    } break;
    case BTIF_CORE_STORAGE_REMOTE_READ_ALL: {
      btif_storage_req_t* p_req = (btif_storage_req_t*)p_param;
      btif_in_get_remote_device_properties(&p_req->read_req.bd_addr);
    } break;
  }
}

void btif_adapter_properties_evt(bt_status_t status, uint32_t num_props,
                                 bt_property_t* p_props) {
  HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, num_props, p_props);
}
void btif_remote_properties_evt(bt_status_t status, RawAddress* remote_addr,
                                uint32_t num_props, bt_property_t* p_props) {
  HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, status, remote_addr,
            num_props, p_props);
}

/*******************************************************************************
 *
 * Function         btif_in_storage_request_copy_cb
 *
 * Description     Switch context callback function to perform the deep copy for
 *                 both the adapter and remote_device property API
 *
 * Returns          None
 *
 ******************************************************************************/
static void btif_in_storage_request_copy_cb(uint16_t event, char* p_new_buf,
                                            char* p_old_buf) {
  btif_storage_req_t* new_req = (btif_storage_req_t*)p_new_buf;
  btif_storage_req_t* old_req = (btif_storage_req_t*)p_old_buf;

  BTIF_TRACE_EVENT("%s", __func__);
  switch (event) {
    case BTIF_CORE_STORAGE_REMOTE_WRITE:
    case BTIF_CORE_STORAGE_ADAPTER_WRITE: {
      new_req->write_req.bd_addr = old_req->write_req.bd_addr;
      /* Copy the member variables one at a time */
      new_req->write_req.prop.type = old_req->write_req.prop.type;
      new_req->write_req.prop.len = old_req->write_req.prop.len;

      new_req->write_req.prop.val =
          (uint8_t*)(p_new_buf + sizeof(btif_storage_req_t));
      memcpy(new_req->write_req.prop.val, old_req->write_req.prop.val,
             old_req->write_req.prop.len);
    } break;
  }
}

/*******************************************************************************
 *
 * Function         btif_get_adapter_properties
 *
 * Description      Fetch all available properties (local & remote)
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_get_adapter_properties(void) {
  BTIF_TRACE_EVENT("%s", __func__);

  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  return btif_transfer_context(execute_storage_request,
                               BTIF_CORE_STORAGE_ADAPTER_READ_ALL, NULL, 0,
                               NULL);
}

/*******************************************************************************
 *
 * Function         btif_get_adapter_property
 *
 * Description      Fetches property value from local cache
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_get_adapter_property(bt_property_type_t type) {
  btif_storage_req_t req;

  BTIF_TRACE_EVENT("%s %d", __func__, type);

  /* Allow get_adapter_property only for BDADDR and BDNAME if BT is disabled */
  if (!btif_is_enabled() && (type != BT_PROPERTY_BDADDR) &&
      (type != BT_PROPERTY_BDNAME) && (type != BT_PROPERTY_CLASS_OF_DEVICE))
    return BT_STATUS_NOT_READY;

  req.read_req.bd_addr = RawAddress::kEmpty;
  req.read_req.type = type;

  return btif_transfer_context(execute_storage_request,
                               BTIF_CORE_STORAGE_ADAPTER_READ, (char*)&req,
                               sizeof(btif_storage_req_t), NULL);
}

/*******************************************************************************
 *
 * Function         btif_set_adapter_property
 *
 * Description      Updates core stack with property value and stores it in
 *                  local cache
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_set_adapter_property(const bt_property_t* property) {
  btif_storage_req_t req;
  bt_status_t status = BT_STATUS_SUCCESS;
  int storage_req_id = BTIF_CORE_STORAGE_NOTIFY_STATUS; /* default */
  char bd_name[BTM_MAX_LOC_BD_NAME_LEN + 1];
  uint16_t name_len = 0;

  BTIF_TRACE_EVENT("btif_set_adapter_property type: %d, len %d, 0x%x",
                   property->type, property->len, property->val);

  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  switch (property->type) {
    case BT_PROPERTY_BDNAME: {
      name_len = property->len > BTM_MAX_LOC_BD_NAME_LEN
                     ? BTM_MAX_LOC_BD_NAME_LEN
                     : property->len;
      memcpy(bd_name, property->val, name_len);
      bd_name[name_len] = '\0';

      BTIF_TRACE_EVENT("set property name : %s", (char*)bd_name);

      BTA_DmSetDeviceName((char*)bd_name);

      storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
    } break;

    case BT_PROPERTY_ADAPTER_SCAN_MODE: {
      bt_scan_mode_t mode = *(bt_scan_mode_t*)property->val;
      tBTA_DM_DISC disc_mode;
      tBTA_DM_CONN conn_mode;

      switch (mode) {
        case BT_SCAN_MODE_NONE:
          disc_mode = BTA_DM_NON_DISC;
          conn_mode = BTA_DM_NON_CONN;
          break;

        case BT_SCAN_MODE_CONNECTABLE:
          disc_mode = BTA_DM_NON_DISC;
          conn_mode = BTA_DM_CONN;
          break;

        case BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE:
          disc_mode = BTA_DM_GENERAL_DISC;
          conn_mode = BTA_DM_CONN;
          break;

        default:
          BTIF_TRACE_ERROR("invalid scan mode (0x%x)", mode);
          return BT_STATUS_PARM_INVALID;
      }

      BTIF_TRACE_EVENT("set property scan mode : %x", mode);

      BTA_DmSetVisibility(disc_mode, conn_mode, BTA_DM_IGNORE, BTA_DM_IGNORE);

      storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
    } break;
    case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT: {
      /* Nothing to do beside store the value in NV.  Java
         will change the SCAN_MODE property after setting timeout,
         if required */
      storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
    } break;
    case BT_PROPERTY_CLASS_OF_DEVICE: {
      DEV_CLASS dev_class;
      memcpy(dev_class, property->val, DEV_CLASS_LEN);

      BTIF_TRACE_EVENT("set property dev_class : 0x%02x%02x%02x", dev_class[0],
                       dev_class[1], dev_class[2]);

      BTM_SetDeviceClass(dev_class);
    } break;
    case BT_PROPERTY_BDADDR:
    case BT_PROPERTY_UUIDS:
    case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
    case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
      /* no write support through HAL, these properties are only populated from
       * BTA events */
      status = BT_STATUS_FAIL;
      break;
    case BT_PROPERTY_LOCAL_IO_CAPS:
    case BT_PROPERTY_LOCAL_IO_CAPS_BLE: {
      // Changing IO Capability of stack at run-time is not currently supported.
      // This call changes the stored value which will affect the stack next
      // time it starts up.
      storage_req_id = BTIF_CORE_STORAGE_ADAPTER_WRITE;
    } break;
    default:
      BTIF_TRACE_ERROR("btif_get_adapter_property : invalid type %d",
                       property->type);
      status = BT_STATUS_FAIL;
      break;
  }

  if (storage_req_id != BTIF_CORE_STORAGE_NO_ACTION) {
    /* pass on to storage for updating local database */

    req.write_req.bd_addr = RawAddress::kEmpty;
    memcpy(&(req.write_req.prop), property, sizeof(bt_property_t));

    return btif_transfer_context(execute_storage_request, storage_req_id,
                                 (char*)&req,
                                 sizeof(btif_storage_req_t) + property->len,
                                 btif_in_storage_request_copy_cb);
  }

  return status;
}

/*******************************************************************************
 *
 * Function         btif_get_remote_device_property
 *
 * Description      Fetches the remote device property from the NVRAM
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_get_remote_device_property(RawAddress* remote_addr,
                                            bt_property_type_t type) {
  btif_storage_req_t req;

  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  req.read_req.bd_addr = *remote_addr;
  req.read_req.type = type;
  return btif_transfer_context(execute_storage_remote_request,
                               BTIF_CORE_STORAGE_REMOTE_READ, (char*)&req,
                               sizeof(btif_storage_req_t), NULL);
}

/*******************************************************************************
 *
 * Function         btif_get_remote_device_properties
 *
 * Description      Fetches all the remote device properties from NVRAM
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_get_remote_device_properties(RawAddress* remote_addr) {
  btif_storage_req_t req;

  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  req.read_req.bd_addr = *remote_addr;
  return btif_transfer_context(execute_storage_remote_request,
                               BTIF_CORE_STORAGE_REMOTE_READ_ALL, (char*)&req,
                               sizeof(btif_storage_req_t), NULL);
}

/*******************************************************************************
 *
 * Function         btif_set_remote_device_property
 *
 * Description      Writes the remote device property to NVRAM.
 *                  Currently, BT_PROPERTY_REMOTE_FRIENDLY_NAME is the only
 *                  remote device property that can be set
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_set_remote_device_property(RawAddress* remote_addr,
                                            const bt_property_t* property) {
  btif_storage_req_t req;

  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  req.write_req.bd_addr = *remote_addr;
  memcpy(&(req.write_req.prop), property, sizeof(bt_property_t));

  return btif_transfer_context(execute_storage_remote_request,
                               BTIF_CORE_STORAGE_REMOTE_WRITE, (char*)&req,
                               sizeof(btif_storage_req_t) + property->len,
                               btif_in_storage_request_copy_cb);
}

/*******************************************************************************
 *
 * Function         btif_get_remote_service_record
 *
 * Description      Looks up the service matching uuid on the remote device
 *                  and fetches the SCN and service_name if the UUID is found
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_get_remote_service_record(const RawAddress& remote_addr,
                                           const Uuid& uuid) {
  if (!btif_is_enabled()) return BT_STATUS_NOT_READY;

  return btif_dm_get_remote_service_record(remote_addr, uuid);
}

/*******************************************************************************
 *
 * Function         btif_get_enabled_services_mask
 *
 * Description      Fetches currently enabled services
 *
 * Returns          tBTA_SERVICE_MASK
 *
 ******************************************************************************/

tBTA_SERVICE_MASK btif_get_enabled_services_mask(void) {
  return btif_enabled_services;
}

/*******************************************************************************
 *
 * Function         btif_enable_service
 *
 * Description      Enables the service 'service_ID' to the service_mask.
 *                  Upon BT enable, BTIF core shall invoke the BTA APIs to
 *                  enable the profiles
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_enable_service(tBTA_SERVICE_ID service_id) {
  tBTA_SERVICE_ID* p_id = &service_id;

  /* If BT is enabled, we need to switch to BTIF context and trigger the
   * enable for that profile
   *
   * Otherwise, we just set the flag. On BT_Enable, the DM will trigger
   * enable for the profiles that have been enabled */

  btif_enabled_services |= (1 << service_id);

  BTIF_TRACE_DEBUG("%s: current services:0x%x", __func__,
                   btif_enabled_services);

  if (btif_is_enabled()) {
    btif_transfer_context(btif_dm_execute_service_request,
                          BTIF_DM_ENABLE_SERVICE, (char*)p_id,
                          sizeof(tBTA_SERVICE_ID), NULL);
  }

  return BT_STATUS_SUCCESS;
}
/*******************************************************************************
 *
 * Function         btif_disable_service
 *
 * Description      Disables the service 'service_ID' to the service_mask.
 *                  Upon BT disable, BTIF core shall invoke the BTA APIs to
 *                  disable the profiles
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_disable_service(tBTA_SERVICE_ID service_id) {
  tBTA_SERVICE_ID* p_id = &service_id;

  /* If BT is enabled, we need to switch to BTIF context and trigger the
   * disable for that profile so that the appropriate uuid_property_changed will
   * be triggerred. Otherwise, we just need to clear the service_id in the mask
   */

  btif_enabled_services &= (tBTA_SERVICE_MASK)(~(1 << service_id));

  BTIF_TRACE_DEBUG("%s: Current Services:0x%x", __func__,
                   btif_enabled_services);

  if (btif_is_enabled()) {
    btif_transfer_context(btif_dm_execute_service_request,
                          BTIF_DM_DISABLE_SERVICE, (char*)p_id,
                          sizeof(tBTA_SERVICE_ID), NULL);
  }

  return BT_STATUS_SUCCESS;
}

static void btif_jni_associate() {
  BTIF_TRACE_DEBUG("%s Associating thread to JVM", __func__);
  HAL_CBACK(bt_hal_cbacks, thread_evt_cb, ASSOCIATE_JVM);
}

static void btif_jni_disassociate() {
  BTIF_TRACE_DEBUG("%s Disassociating thread from JVM", __func__);
  HAL_CBACK(bt_hal_cbacks, thread_evt_cb, DISASSOCIATE_JVM);
  bt_hal_cbacks = NULL;
}
