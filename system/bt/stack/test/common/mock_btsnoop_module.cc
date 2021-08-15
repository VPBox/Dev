/******************************************************************************
 *
 *  Copyright 2019 The Android Open Source Project
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

#include "hci/include/btsnoop.h"

static void capture(const BT_HDR*, bool) { /* do nothing */
}

static void whitelist_l2c_channel(uint16_t, uint16_t,
                                  uint16_t) { /* do nothing */
}

static void whitelist_rfc_dlci(uint16_t, uint8_t) { /* do nothing */
}

static void add_rfc_l2c_channel(uint16_t, uint16_t, uint16_t) { /* do nothing */
}

static void clear_l2cap_whitelist(uint16_t, uint16_t,
                                  uint16_t) { /* do nothing */
}

static const btsnoop_t fake_snoop = {capture, whitelist_l2c_channel,
                                     whitelist_rfc_dlci, add_rfc_l2c_channel,
                                     clear_l2cap_whitelist};

const btsnoop_t* btsnoop_get_interface() { return &fake_snoop; }
