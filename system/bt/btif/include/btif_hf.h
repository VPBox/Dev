/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
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

#include <hardware/bluetooth_headset_interface.h>

namespace bluetooth {
namespace headset {

/**
 * Get an instance of the headset interface from the loaded shared library
 *
 * @return an instance of the headset interface
 */
Interface* GetInterface();

/**
 * Check whether there is a Hands-Free call in progress.
 *
 * @return true if no call is in progress.
 */
bool IsCallIdle();

/**
 * Start up or shutdown the service
 *
 * @param b_enable true to enable, false to disable
 * @return BT_STATUS_SUCCESS on success, BT_STATUS_FAIL otherwise
 */
bt_status_t ExecuteService(bool b_enable);

}  // namespace headset
}  // namespace bluetooth
