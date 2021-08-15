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
#include <memory>
#include <vector>

#include "include/acl.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

// ACL data packets are specified in the Bluetooth Core Specification Version
// 4.2, Volume 2, Part E, Section 5.4.2
class AclPacketView : public PacketView<true> {
 public:
  virtual ~AclPacketView() override = default;

  static AclPacketView Create(std::shared_ptr<std::vector<uint8_t>> packet);
  static AclPacketView Create(PacketView<true> packet_view);

  uint16_t GetHandle() const;
  acl::PacketBoundaryFlagsType GetPacketBoundaryFlags() const;
  acl::BroadcastFlagsType GetBroadcastFlags() const;
  PacketView<true> GetPayload() const;

 private:
  AclPacketView(std::shared_ptr<std::vector<uint8_t>> packet);
  AclPacketView(PacketView<true> packet_view);
  AclPacketView() = delete;
};

}  // namespace packets
}  // namespace test_vendor_lib
