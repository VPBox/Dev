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

#define LOG_TAG "bt_task"

#include <base/logging.h>
#include <pthread.h>
#include <string.h>

#include "bt_target.h"
#include "btm_int.h"
#include "btu.h"
#include "common/message_loop_thread.h"
#include "device/include/controller.h"
#include "gatt_api.h"
#include "gatt_int.h"
#include "l2c_int.h"
#include "osi/include/alarm.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/log.h"
#include "sdpint.h"
#include "smp_int.h"

using bluetooth::common::MessageLoopThread;

MessageLoopThread bt_startup_thread("bt_startup_thread");

void btu_task_start_up(void* context);
void btu_task_shut_down(void* context);

/*****************************************************************************
 *
 * Function         btu_init_core
 *
 * Description      Initialize control block memory for each core component.
 *
 *
 * Returns          void
 *
 *****************************************************************************/
void btu_init_core() {
  /* Initialize the mandatory core stack components */
  btm_init();

  l2c_init();

  sdp_init();

  gatt_init();

  SMP_Init();

  btm_ble_init();
}

/*****************************************************************************
 *
 * Function         btu_free_core
 *
 * Description      Releases control block memory for each core component.
 *
 *
 * Returns          void
 *
 *****************************************************************************/
void btu_free_core() {
  /* Free the mandatory core stack components */
  gatt_free();

  l2c_free();

  sdp_free();

  btm_free();
}

/*****************************************************************************
 *
 * Function         BTU_StartUp
 *
 * Description      Initializes the BTU control block.
 *
 *                  NOTE: Must be called before creating any tasks
 *                      (RPC, BTU, HCIT, APPL, etc.)
 *
 * Returns          void
 *
 *****************************************************************************/
void BTU_StartUp() {
  btu_trace_level = HCI_INITIAL_TRACE_LEVEL;
  bt_startup_thread.StartUp();
  if (!bt_startup_thread.EnableRealTimeScheduling()) {
    LOG(ERROR) << __func__ << ": Unable to set real time scheduling policy for "
               << bt_startup_thread;
    BTU_ShutDown();
    return;
  }
  if (!bt_startup_thread.DoInThread(FROM_HERE,
                                    base::Bind(btu_task_start_up, nullptr))) {
    LOG(ERROR) << __func__ << ": Unable to continue start-up on "
               << bt_startup_thread;
    BTU_ShutDown();
    return;
  }
}

void BTU_ShutDown() {
  btu_task_shut_down(nullptr);
  bt_startup_thread.ShutDown();
}
