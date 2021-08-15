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

#ifndef CHRE_WIFI_OFFLOAD_ERROR_CODES_H_
#define CHRE_WIFI_OFFLOAD_ERROR_CODES_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

namespace wifi_offload {

enum ErrorCode : uint32_t {
  SUCCESS = 0,

  FAILED_TO_ALLOCATE_MESSAGE_BUFFER,
  FAILED_TO_SERIALIZE_MESSAGE,
  FAILED_TO_SEND_MESSAGE,

  FAILED_TO_DESERIALIZE_SCAN_CONFIG,
  INVALID_SUBSCRIBE_MESSAGE_SIZE,
  SCAN_CONFIG_NOT_INITIALIZED,
  UNSPECIFIED_HOST_ENDPOINT,

  FAILED_TO_SEND_SCAN_RESULTS,
  FAILED_TO_SEND_SCAN_STATS,

  SCAN_MONITORING_NOT_SUPPORTED,
  FAILED_TO_START_SCAN_MONITORING,
  FAILED_TO_STOP_SCAN_MONITORING,
  FAILED_TO_CONFIGURE_SCAN_MONITORING_ASYNC,

  ONDEMAND_SCAN_NOT_SUPPORTED,
  FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST,
  FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST_ASYNC,

  OUT_OF_ORDER_SCAN_RESULTS,
  INCOMPLETE_SCAN_RESULTS_BEFORE_SCAN_REQUEST,

  FAILED_TO_SET_SCAN_TIMER,
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_ERROR_CODES_H_
