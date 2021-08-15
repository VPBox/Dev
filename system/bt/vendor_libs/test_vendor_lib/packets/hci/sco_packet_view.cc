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

#include "packets/hci/sco_packet_view.h"

#include <base/logging.h>

using test_vendor_lib::sco::PacketStatusFlagsType;

namespace test_vendor_lib {
namespace packets {

ScoPacketView::ScoPacketView(std::shared_ptr<std::vector<uint8_t>> packet) : PacketView<true>(packet) {}

ScoPacketView ScoPacketView::Create(std::shared_ptr<std::vector<uint8_t>> packet) {
  return ScoPacketView(packet);
}

uint16_t ScoPacketView::GetHandle() const {
  return begin().extract<uint16_t>() & 0xfff;
}

PacketStatusFlagsType ScoPacketView::GetPacketStatusFlags() const {
  return static_cast<PacketStatusFlagsType>(((begin() + 1).extract<uint8_t>() & 0x30) >> 4);
}

PacketView<true> ScoPacketView::GetPayload() const {
  uint8_t payload_size = (begin() + sizeof(uint16_t)).extract<uint8_t>();
  CHECK(static_cast<uint8_t>(size() - sizeof(uint16_t) - sizeof(uint8_t)) == payload_size)
      << "Malformed SCO packet payload_size " << payload_size << " + 4 != " << size();
  return SubViewLittleEndian(sizeof(uint16_t) + sizeof(uint8_t), size());
}

}  // namespace packets
}  // namespace test_vendor_lib
