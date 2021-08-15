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

#include "packets/hci/command_packet_builder.h"

#include <base/logging.h>

using std::vector;
using test_vendor_lib::hci::OpCode;

namespace test_vendor_lib {
namespace packets {

CommandPacketBuilder::CommandPacketBuilder(OpCode opcode, std::unique_ptr<BasePacketBuilder> payload)
    : opcode_(opcode), payload_(std::move(payload)) {}

size_t CommandPacketBuilder::size() const {
  return sizeof(uint16_t) + sizeof(uint8_t) + payload_->size();
}

void CommandPacketBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint16_t>(opcode_), it);
  uint8_t payload_size = static_cast<uint8_t>(payload_->size());

  CHECK(static_cast<size_t>(payload_size) == payload_->size())
      << "Payload too large for a command packet: " << payload_->size();
  insert(payload_size, it);
  payload_->Serialize(it);
}

}  // namespace packets
}  // namespace test_vendor_lib
