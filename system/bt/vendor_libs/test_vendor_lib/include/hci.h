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

#include "include/hci/event_code.h"
#include "include/hci/le_sub_event_code.h"
#include "include/hci/op_code.h"
#include "include/hci/status.h"

namespace test_vendor_lib {
namespace hci {

enum class PacketType : uint8_t {
  UNKNOWN = 0,
  COMMAND = 1,
  ACL = 2,
  SCO = 3,
  EVENT = 4,
};

enum class LinkType : uint8_t {
  SCO = 0x00,
  ACL = 0x01,
  ESCO = 0x02,
};

enum class LoopbackMode : uint8_t {
  NO = 0x00,
  LOCAL = 0x01,
  REMOTE = 0x02,
};

/* HCI, PAL, and LMP Version numbers are the same */
enum class Version : uint8_t {
  V1_0 = 0,
  V1_1 = 1,
  V1_2 = 2,
  V2_0 = 3,
  V2_1 = 4,
  V3_0 = 5,
  V4_0 = 6,
  V4_1 = 7,
  V4_2 = 8,
  V5_0 = 9,
};

}  // namespace hci
}  // namespace test_vendor_lib
