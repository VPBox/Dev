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

#include "pass_through_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<PassThroughPacketBuilder> PassThroughPacketBuilder::MakeBuilder(
    bool response, bool pushed, uint8_t opperation_id) {
  auto builder = std::unique_ptr<PassThroughPacketBuilder>(
      new PassThroughPacketBuilder(response, pushed, opperation_id));

  return builder;
}

size_t PassThroughPacketBuilder::size() const {
  return PassThroughPacket::kMinSize();
}

bool PassThroughPacketBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  PacketBuilder::PushHeader(pkt);

  uint8_t byte = opperation_id_ & 0b01111111;
  if (!pushed_) byte |= 0b10000000;
  AddPayloadOctets1(pkt, byte);
  // Data length, for this packet it's always 0;
  AddPayloadOctets1(pkt, 0x00);

  return true;
}

KeyState PassThroughPacket::GetKeyState() const {
  auto it = begin() + Packet::kMinSize();
  return static_cast<KeyState>(((*it) & 0b10000000) >> 7);
}

uint8_t PassThroughPacket::GetOperationId() const {
  return *(begin() + Packet::kMinSize()) & 0b01111111;
}

bool PassThroughPacket::IsValid() const { return size() == kMinSize(); }

std::string PassThroughPacket::ToString() const {
  std::stringstream ss;
  ss << "Avrcp::AvrcpPacket: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Pushed = " << GetKeyState() << std::endl;
  ss << "  └ Opperation ID = " << loghex(GetOperationId()) << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth