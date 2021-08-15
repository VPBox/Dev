/******************************************************************************
 *
 *  Copyright 2009-2013 Broadcom Corporation
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

#ifndef BTIF_GATT_UTIL_H
#define BTIF_GATT_UTIL_H

#include <hardware/bluetooth.h>
#include <hardware/bt_gatt.h>

#include "bta/include/bta_gatt_api.h"

void btif_to_bta_response(tGATTS_RSP* p_dest, btgatt_response_t* p_src);

void btif_gatt_check_encrypted_link(RawAddress bd_addr,
                                    tGATT_TRANSPORT transport);
extern void btif_gatt_move_track_adv_data(btgatt_track_adv_info_t* p_dest,
                                          btgatt_track_adv_info_t* p_src);

#endif
