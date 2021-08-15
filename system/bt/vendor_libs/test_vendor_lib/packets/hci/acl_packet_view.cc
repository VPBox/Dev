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

#include "packets/hci/acl_packet_view.h"

#include <base/logging.h>

using std::vector;
using test_vendor_lib::acl::BroadcastFlagsType;
using test_vendor_lib::acl::PacketBoundaryFlagsType;

namespace test_vendor_lib {
namespace packets {

AclPacketView::AclPacketView(std::shared_ptr<std::vector<uint8_t>> packet) : PacketView<true>(packet) {}

AclPacketView::AclPacketView(PacketView<true> packet_view) : PacketView<true>(packet_view) {}

AclPacketView AclPacketView::Create(std::shared_ptr<std::vector<uint8_t>> packet) {
  return AclPacketView(packet);
}

AclPacketView AclPacketView::Create(PacketView<true> packet_view) {
  return AclPacketView(packet_view);
}

uint16_t AclPacketView::GetHandle() const {
  return begin().extract<uint16_t>() & 0xfff;
}

PacketBoundaryFlagsType AclPacketView::GetPacketBoundaryFlags() const {
  return static_cast<PacketBoundaryFlagsType>(((begin() + 1).extract<uint8_t>() & 0x30) >> 4);
}

BroadcastFlagsType AclPacketView::GetBroadcastFlags() const {
  return static_cast<BroadcastFlagsType>(((begin() + 1).extract<uint8_t>() & 0xc0) >> 6);
}

PacketView<true> AclPacketView::GetPayload() const {
  uint16_t payload_size = (begin() + sizeof(uint16_t)).extract<uint16_t>();
  CHECK(static_cast<uint16_t>(size() - 2 * sizeof(uint16_t)) == payload_size)
      << "Malformed ACL packet payload_size " << payload_size << " + 4 != " << size();
  return SubViewLittleEndian(2 * sizeof(uint16_t), size());
}

}  // namespace packets
}  // namespace test_vendor_lib
