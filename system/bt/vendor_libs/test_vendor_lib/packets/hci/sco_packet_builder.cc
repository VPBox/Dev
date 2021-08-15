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

#include "packets/hci/sco_packet_builder.h"

#include <base/logging.h>

using std::vector;
using test_vendor_lib::sco::PacketStatusFlagsType;

namespace test_vendor_lib {
namespace packets {

ScoPacketBuilder::ScoPacketBuilder(uint16_t handle, PacketStatusFlagsType packet_status_flags,
                                   std::unique_ptr<BasePacketBuilder> payload)
    : handle_(handle), packet_status_flags_(packet_status_flags), payload_(std::move(payload)) {}

size_t ScoPacketBuilder::size() const {
  return 2 * sizeof(uint16_t) + payload_->size();
}

void ScoPacketBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint16_t>((handle_ & 0xfff) | (static_cast<uint16_t>(packet_status_flags_) << 12)), it);
  uint8_t payload_size = payload_->size();

  CHECK(static_cast<size_t>(payload_size) == payload_->size())
      << "Payload too large for a SCO packet: " << payload_->size();
  insert(payload_size, it);
  payload_->Serialize(it);
}

}  // namespace packets
}  // namespace test_vendor_lib
