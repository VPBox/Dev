/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
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

#pragma once

#include <base/callback.h>
#include <base/location.h>
#include <stdbool.h>

#include "bt_types.h"
#include "osi/include/allocator.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/future.h"
#include "osi/include/osi.h"

static const char HCI_MODULE[] = "hci_module";

///// LEGACY DEFINITIONS /////

/* Message event mask across Host/Controller lib and stack */
#define MSG_EVT_MASK 0xFF00     /* eq. BT_EVT_MASK */
#define MSG_SUB_EVT_MASK 0x00FF /* eq. BT_SUB_EVT_MASK */

/* Message event ID passed from Host/Controller lib to stack */
#define MSG_HC_TO_STACK_HCI_ERR 0x1300      /* eq. BT_EVT_TO_BTU_HCIT_ERR */
#define MSG_HC_TO_STACK_HCI_ACL 0x1100      /* eq. BT_EVT_TO_BTU_HCI_ACL */
#define MSG_HC_TO_STACK_HCI_SCO 0x1200      /* eq. BT_EVT_TO_BTU_HCI_SCO */
#define MSG_HC_TO_STACK_HCI_EVT 0x1000      /* eq. BT_EVT_TO_BTU_HCI_EVT */
#define MSG_HC_TO_STACK_L2C_SEG_XMIT 0x1900 /* BT_EVT_TO_BTU_L2C_SEG_XMIT */

/* Message event ID passed from stack to vendor lib */
#define MSG_STACK_TO_HC_HCI_ACL 0x2100 /* eq. BT_EVT_TO_LM_HCI_ACL */
#define MSG_STACK_TO_HC_HCI_SCO 0x2200 /* eq. BT_EVT_TO_LM_HCI_SCO */
#define MSG_STACK_TO_HC_HCI_CMD 0x2000 /* eq. BT_EVT_TO_LM_HCI_CMD */

/* Local Bluetooth Controller ID for BR/EDR */
#define LOCAL_BR_EDR_CONTROLLER_ID 0

///// END LEGACY DEFINITIONS /////

typedef struct hci_hal_t hci_hal_t;
typedef struct btsnoop_t btsnoop_t;
typedef struct controller_t controller_t;
typedef struct hci_inject_t hci_inject_t;
typedef struct packet_fragmenter_t packet_fragmenter_t;
typedef struct vendor_t vendor_t;

typedef unsigned char* bdaddr_t;
typedef uint16_t command_opcode_t;

typedef void (*command_complete_cb)(BT_HDR* response, void* context);
typedef void (*command_status_cb)(uint8_t status, BT_HDR* command,
                                  void* context);

typedef struct hci_t {
  // Set the callback that the HCI layer uses to send data upwards
  void (*set_data_cb)(
      base::Callback<void(const base::Location&, BT_HDR*)> send_data_cb);

  // Send a command through the HCI layer
  void (*transmit_command)(BT_HDR* command,
                           command_complete_cb complete_callback,
                           command_status_cb status_cb, void* context);

  future_t* (*transmit_command_futured)(BT_HDR* command);

  // Send some data downward through the HCI layer
  void (*transmit_downward)(uint16_t type, void* data);
} hci_t;

const hci_t* hci_layer_get_interface();

const hci_t* hci_layer_get_test_interface(
    const allocator_t* buffer_allocator_interface,
    const btsnoop_t* btsnoop_interface,
    const packet_fragmenter_t* packet_fragmenter_interface);

void post_to_main_message_loop(const base::Location& from_here, BT_HDR* p_msg);

void hci_layer_cleanup_interface();
