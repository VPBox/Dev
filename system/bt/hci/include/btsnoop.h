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

#include <stdbool.h>

#include "bt_types.h"

static const char BTSNOOP_MODULE[] = "btsnoop_module";

typedef struct btsnoop_t {
  // Capture |packet| and dump it to the btsnoop logs. If |is_received| is
  // true, the packet is marked as incoming. Otherwise, the packet is marked
  // as outgoing.
  void (*capture)(const BT_HDR* packet, bool is_received);

  // Set a L2CAP channel as whitelisted, allowing packets with that L2CAP CID
  // to show up in the snoop logs.
  void (*whitelist_l2c_channel)(uint16_t conn_handle, uint16_t local_cid,
                                uint16_t remote_cid);

  // Set a RFCOMM dlci as whitelisted, allowing packets with that RFCOMM CID
  // to show up in the snoop logs. The local_cid is used to associate it with
  // its corrisponding ACL connection. The dlci is the channel with direction
  // so there is no chance of a collision if two services are using the same
  // channel but in different directions.
  void (*whitelist_rfc_dlci)(uint16_t local_cid, uint8_t dlci);

  // Indicate that the provided L2CAP channel is being used for RFCOMM.
  // If packets with the provided L2CAP CID are encountered, they will be
  // filtered on RFCOMM based on channels provided to |filter_rfc_channel|.
  void (*add_rfc_l2c_channel)(uint16_t conn_handle, uint16_t local_cid,
                              uint16_t remote_cid);

  // Clear an L2CAP channel from being filtered.
  void (*clear_l2cap_whitelist)(uint16_t conn_handle, uint16_t local_cid,
                                uint16_t remote_cid);
} btsnoop_t;

const btsnoop_t* btsnoop_get_interface(void);
