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

#include "packets/hci/command_packet_view.h"

#include <base/logging.h>

using std::vector;

namespace test_vendor_lib {
namespace packets {

CommandPacketView::CommandPacketView(std::shared_ptr<std::vector<uint8_t>> packet) : PacketView<true>(packet) {}

CommandPacketView CommandPacketView::Create(std::shared_ptr<std::vector<uint8_t>> packet) {
  return CommandPacketView(packet);
}

uint16_t CommandPacketView::GetOpcode() const {
  return begin().extract<uint16_t>();
}

PacketView<true> CommandPacketView::GetPayload() const {
  uint8_t payload_size = (begin() + sizeof(uint16_t)).extract<uint8_t>();
  CHECK(static_cast<uint8_t>(size() - sizeof(uint16_t) - sizeof(uint8_t)) == payload_size)
      << "Malformed Command packet payload_size " << payload_size << " + 2 != " << size();
  return SubViewLittleEndian(sizeof(uint16_t) + sizeof(uint8_t), size());
}

}  // namespace packets
}  // namespace test_vendor_lib
