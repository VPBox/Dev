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
 *  this file contains the main Bluetooth Upper Layer definitions. The Broadcom
 *  implementations of L2CAP RFCOMM, SDP and the BTIf run as one GKI task. The
 *  btu_task switches between them.
 *
 ******************************************************************************/

#ifndef BTU_H
#define BTU_H

#include "bt_common.h"
#include "bt_target.h"
#include "common/message_loop_thread.h"
#include "osi/include/alarm.h"

#include <base/callback.h>
#include <base/location.h>
#include <base/threading/thread.h>

/* Global BTU data */
extern uint8_t btu_trace_level;

/* Functions provided by btu_hcif.cc
 ***********************************
*/
void btu_hcif_process_event(uint8_t controller_id, BT_HDR* p_buf);
void btu_hcif_send_cmd(uint8_t controller_id, BT_HDR* p_msg);
void btu_hcif_send_cmd_with_cb(const base::Location& posted_from,
                               uint16_t opcode, uint8_t* params,
                               uint8_t params_len,
                               base::OnceCallback<void(uint8_t*, uint16_t)> cb);

/* Functions provided by btu_init.cc
 ***********************************
*/
void btu_init_core(void);
void btu_free_core(void);

/* Functions provided by btu_task.cc
 ***********************************
*/
bluetooth::common::MessageLoopThread* get_main_thread();
base::MessageLoop* get_main_message_loop();
bt_status_t do_in_main_thread(const base::Location& from_here,
                              base::OnceClosure task);

void BTU_StartUp(void);
void BTU_ShutDown(void);

#endif
