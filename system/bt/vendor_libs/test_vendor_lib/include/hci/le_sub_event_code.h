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

enum class LeSubEventCode : uint8_t {
  CONNECTION_COMPLETE = 0x01,
  ADVERTISING_REPORT = 0x02,
  CONNECTION_UPDATE_COMPLETE = 0x03,
  READ_REMOTE_FEATURES_COMPLETE = 0x04,
  LONG_TERM_KEY_REQUEST = 0x05,
  REMOTE_CONNECTION_PARAMETER_REQUEST = 0x06,
  DATA_LENGTH_CHANGE = 0x07,
  ENHANCED_CONNECTION_COMPLETE = 0x0a,
  DIRECTED_ADVERTISING_REPORT = 0x0b,
  PHY_UPDATE_COMPLETE = 0x0c,
  EXTENDED_ADVERTISING_REPORT = 0x0D,
  PERIODIC_ADVERTISING_SYNC_ESTABLISHED = 0x0E,
  PERIODIC_ADVERTISING_REPORT = 0x0F,
  PERIODIC_ADVERTISING_SYNC_LOST = 0x10,
  SCAN_TIMEOUT = 0x11,
  ADVERTISING_SET_TERMINATED = 0x12,
  SCAN_REQUEST_RECEIVED = 0x13,
};
}
}  // namespace test_vendor_lib
