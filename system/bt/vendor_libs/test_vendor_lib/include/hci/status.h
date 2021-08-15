/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once
#include <cstdint>

namespace test_vendor_lib {
namespace hci {

enum class Status : uint8_t {
  SUCCESS = 0,
  UNKNOWN_COMMAND = 1,
  UNKNOWN_CONNECTION = 2,
  HARDWARE_FAILURE = 3,
  PAGE_TIMEOUT = 4,
  AUTHENTICATION_FAILURE = 5,
  PIN_OR_KEY_MISSING = 6,
  MEMORY_CAPACITY_EXCEEDED = 7,
  CONNECTION_TIMEOUT = 8,
  COMMAND_DISALLOWED = 0x0c,
  CONNECTION_REJECTED_LIMITED_RESOURCES = 0x0d,
  CONNECTION_REJECTED_SECURITY = 0x0e,
  CONNECTION_REJECTED_UNACCEPTABLE_BD_ADDR = 0x0f,
  INVALID_HCI_COMMAND_PARAMETERS = 0x12,
  REMOTE_USER_TERMINATED_CONNECTION = 0x13,
  CONNECTION_TERMINATED_BY_LOCAL_HOST = 0x16,
  UNSPECIFIED_ERROR = 0x1f,
  ENCRYPTION_MODE_NOT_ACCEPTABLE = 0x25,
  HOST_BUSY_PAIRING = 0x38,
  CONTROLLER_BUSY = 0x3a,
};
}
}  // namespace test_vendor_lib
