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

/******************************************************************************
 *
 *  Filename:      bte_main.cc
 *
 *  Description:   Contains BTE core stack initialization and shutdown code
 *
 ******************************************************************************/

#define LOG_TAG "bt_main"

#include <base/logging.h>
#include <base/threading/thread.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include <hardware/bluetooth.h>

#include "bt_common.h"
#include "bt_hci_bdroid.h"
#include "bt_utils.h"
#include "bta_api.h"
#include "btcore/include/module.h"
#include "bte.h"
#include "btif_common.h"
#include "btsnoop.h"
#include "btu.h"
#include "device/include/interop.h"
#include "hci_layer.h"
#include "hcimsgs.h"
#include "osi/include/alarm.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/future.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "stack_config.h"

/*******************************************************************************
 *  Constants & Macros
 ******************************************************************************/

/* Run-time configuration file for BLE*/
#ifndef BTE_BLE_STACK_CONF_FILE
// TODO(armansito): Find a better way than searching by a hardcoded path.
#if defined(OS_GENERIC)
#define BTE_BLE_STACK_CONF_FILE "ble_stack.conf"
#else  // !defined(OS_GENERIC)
#define BTE_BLE_STACK_CONF_FILE "/etc/bluetooth/ble_stack.conf"
#endif  // defined(OS_GENERIC)
#endif  // BT_BLE_STACK_CONF_FILE

/******************************************************************************
 *  Variables
 *****************************************************************************/

/*******************************************************************************
 *  Static variables
 ******************************************************************************/
static const hci_t* hci;

/*******************************************************************************
 *  Externs
 ******************************************************************************/
extern void btu_hci_msg_process(BT_HDR* p_msg);

/*******************************************************************************
 *  Static functions
 ******************************************************************************/

/******************************************************************************
 *
 * Function         post_to_hci_message_loop
 *
 * Description      Post an HCI event to the main thread
 *
 * Returns          None
 *
 *****************************************************************************/
void post_to_main_message_loop(const base::Location& from_here, BT_HDR* p_msg) {
  if (do_in_main_thread(from_here, base::Bind(&btu_hci_msg_process, p_msg)) !=
      BT_STATUS_SUCCESS) {
    LOG(ERROR) << __func__ << ": do_in_main_thread failed from "
               << from_here.ToString();
  }
}

/******************************************************************************
 *
 * Function         bte_main_boot_entry
 *
 * Description      BTE MAIN API - Entry point for BTE chip/stack initialization
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_boot_entry(void) {
  module_init(get_module(INTEROP_MODULE));

  hci = hci_layer_get_interface();
  if (!hci) {
    LOG_ERROR(LOG_TAG, "%s could not get hci layer interface.", __func__);
    return;
  }

  hci->set_data_cb(base::Bind(&post_to_main_message_loop));

  module_init(get_module(STACK_CONFIG_MODULE));
}

/******************************************************************************
 *
 * Function         bte_main_cleanup
 *
 * Description      BTE MAIN API - Cleanup code for BTE chip/stack
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_cleanup() {
  module_clean_up(get_module(STACK_CONFIG_MODULE));

  module_clean_up(get_module(INTEROP_MODULE));
}

/******************************************************************************
 *
 * Function         bte_main_enable
 *
 * Description      BTE MAIN API - Creates all the BTE tasks. Should be called
 *                  part of the Bluetooth stack enable sequence
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_enable() {
  APPL_TRACE_DEBUG("%s", __func__);

  module_start_up(get_module(BTSNOOP_MODULE));
  module_start_up(get_module(HCI_MODULE));

  BTU_StartUp();
}

/******************************************************************************
 *
 * Function         bte_main_disable
 *
 * Description      BTE MAIN API - Destroys all the BTE tasks. Should be called
 *                  part of the Bluetooth stack disable sequence
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_disable(void) {
  APPL_TRACE_DEBUG("%s", __func__);

  module_shut_down(get_module(HCI_MODULE));
  module_shut_down(get_module(BTSNOOP_MODULE));

  BTU_ShutDown();
}

/******************************************************************************
 *
 * Function         bte_main_postload_cfg
 *
 * Description      BTE MAIN API - Stack postload configuration
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_postload_cfg(void) {
  // TODO(eisenbach): [HIDL] DEPRECATE?
}

/******************************************************************************
 *
 * Function         bte_main_hci_send
 *
 * Description      BTE MAIN API - This function is called by the upper stack to
 *                  send an HCI message. The function displays a protocol trace
 *                  message (if enabled), and then calls the 'transmit' function
 *                  associated with the currently selected HCI transport
 *
 * Returns          None
 *
 *****************************************************************************/
void bte_main_hci_send(BT_HDR* p_msg, uint16_t event) {
  uint16_t sub_event = event & BT_SUB_EVT_MASK; /* local controller ID */

  p_msg->event = event;

  if ((sub_event == LOCAL_BR_EDR_CONTROLLER_ID) ||
      (sub_event == LOCAL_BLE_CONTROLLER_ID)) {
    hci->transmit_downward(event, p_msg);
  } else {
    APPL_TRACE_ERROR("Invalid Controller ID. Discarding message.");
    osi_free(p_msg);
  }
}
