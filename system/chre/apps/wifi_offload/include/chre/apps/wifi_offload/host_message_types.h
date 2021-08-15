/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHRE_WIFI_OFFLOAD_HOST_MESSAGE_TYPES_H_
#define CHRE_WIFI_OFFLOAD_HOST_MESSAGE_TYPES_H_

namespace wifi_offload {

enum HostMessageType : uint32_t {
  HOST_CMD_BASE = 0x00001000,

  HOST_CMD_CONFIG_SCANS,
  HOST_CMD_SUBSCRIBE_SCAN_RESULTS,
  HOST_CMD_UNSUBSCRIBE_SCAN_RESULTS,
  HOST_CMD_GET_SCAN_STATS,
  HOST_CMD_RESET,

  HOST_MSG_BASE = 0x0005000,

  HOST_MSG_SCAN_RESULTS,
  HOST_MSG_SCAN_STATS,
  HOST_MSG_ERROR,
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_HOST_MESSAGE_TYPES_H_
