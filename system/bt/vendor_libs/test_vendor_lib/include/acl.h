/*
 * Copyright 2017 The Android Open Source Project
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
namespace acl {

// ACL data packets are specified in the Bluetooth Core Specification Version
// 4.2, Volume 2, Part E, Section 5.4.2
static constexpr uint16_t kReservedHandle = 0xF00;

enum class PacketBoundaryFlagsType : uint8_t {
  FIRST_NON_AUTOMATICALLY_FLUSHABLE = 0,
  CONTINUING = 1,
  FIRST_AUTOMATICALLY_FLUSHABLE = 2,
  COMPLETE = 3
};

enum class BroadcastFlagsType : uint8_t {
  POINT_TO_POINT = 0,
  ACTIVE_SLAVE_BROADCAST = 1,
  PARKED_SLAVE_BROADCAST = 2,
  RESERVED = 3
};
}  // namespace acl
}  // namespace test_vendor_lib
