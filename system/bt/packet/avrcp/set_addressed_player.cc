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

#include "set_addressed_player.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<SetAddressedPlayerResponseBuilder>
SetAddressedPlayerResponseBuilder::MakeBuilder(Status status) {
  std::unique_ptr<SetAddressedPlayerResponseBuilder> builder(
      new SetAddressedPlayerResponseBuilder(status));

  return builder;
}

size_t SetAddressedPlayerResponseBuilder::size() const {
  size_t len = VendorPacket::kMinSize();
  len += 1;  // Status
  return len;
}

bool SetAddressedPlayerResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  PacketBuilder::PushHeader(pkt);

  VendorPacketBuilder::PushHeader(pkt, size() - VendorPacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);

  return true;
}

uint16_t SetAddressedPlayerRequest::GetPlayerId() const {
  auto it = begin() + VendorPacket::kMinSize();
  return it.extractBE<uint16_t>();
}

bool SetAddressedPlayerRequest::IsValid() const {
  if (!VendorPacket::IsValid()) return false;
  return size() == kMinSize();
}

std::string SetAddressedPlayerRequest::ToString() const {
  std::stringstream ss;
  ss << "SetAddressedPlayerRequest: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << GetCommandPdu() << std::endl;
  ss << "  └ PacketType = " << GetPacketType() << std::endl;
  ss << "  └ Parameter Length = " << loghex(GetParameterLength()) << std::endl;
  ss << "  └ Player ID = " << loghex(GetPlayerId()) << std::endl;
  ss << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
