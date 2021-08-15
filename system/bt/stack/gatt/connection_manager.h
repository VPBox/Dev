/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include <set>

#include "types/raw_address.h"

/* connection_manager takes care of all the low-level details of LE connection
 * initiation. It accept requests from multiple subsystems to connect to
 * devices, and multiplex them into whitelist add/remove, and scan parameter
 * changes.
 *
 * There is no code for app_id generation. GATT clients use their GATT_IF, and
 * L2CAP layer uses CONN_MGR_ID_L2CAP as fixed app_id. In case any further
 * subsystems also use connection_manager, we should consider adding a proper
 * mechanism for app_id generation.
 */
namespace connection_manager {

using tAPP_ID = uint8_t;

/* for background connection */
extern bool background_connect_add(tAPP_ID app_id, const RawAddress& address);
extern bool background_connect_remove(tAPP_ID app_id,
                                      const RawAddress& address);
extern bool remove_unconditional(const RawAddress& address);

extern void reset(bool after_reset);

extern void on_app_deregistered(tAPP_ID app_id);
extern void on_connection_complete(const RawAddress& address);

extern std::set<tAPP_ID> get_apps_connecting_to(const RawAddress& remote_bda);

extern bool direct_connect_add(tAPP_ID app_id, const RawAddress& address);
extern bool direct_connect_remove(tAPP_ID app_id, const RawAddress& address);

extern void dump(int fd);

/* This callback will be executed when direct connect attempt fails due to
 * timeout. It must be implemented by users of connection_manager */
extern void on_connection_timed_out(uint8_t app_id, const RawAddress& address);

}  // namespace connection_manager
