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

#include "play_item.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<PlayItemResponseBuilder> PlayItemResponseBuilder::MakeBuilder(
    Status status) {
  std::unique_ptr<PlayItemResponseBuilder> builder(
      new PlayItemResponseBuilder(status));

  return builder;
}

size_t PlayItemResponseBuilder::size() const {
  size_t len = VendorPacket::kMinSize();
  len += 1;  // Status
  return len;
}

bool PlayItemResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  PacketBuilder::PushHeader(pkt);

  VendorPacketBuilder::PushHeader(pkt, size() - VendorPacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);

  return true;
}

Scope PlayItemRequest::GetScope() const {
  auto it = begin() + VendorPacket::kMinSize();
  return static_cast<Scope>(*it);
}

uint64_t PlayItemRequest::GetUid() const {
  auto it = begin() + VendorPacket::kMinSize() + static_cast<size_t>(1);
  return it.extractBE<uint64_t>();
}

uint16_t PlayItemRequest::GetUidCounter() const {
  auto it = begin() + VendorPacket::kMinSize() + static_cast<size_t>(9);
  return it.extractBE<uint16_t>();
}

bool PlayItemRequest::IsValid() const {
  if (!VendorPacket::IsValid()) return false;
  return size() == kMinSize();
}

std::string PlayItemRequest::ToString() const {
  std::stringstream ss;
  ss << "PlayItemRequest: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << GetCommandPdu() << std::endl;
  ss << "  └ PacketType = " << GetPacketType() << std::endl;
  ss << "  └ Parameter Length = " << loghex(GetParameterLength()) << std::endl;
  ss << "  └ Scope = " << GetScope() << std::endl;
  ss << "  └ UID = " << loghex(GetUid()) << std::endl;
  ss << "  └ UID Counter = " << loghex(GetUidCounter()) << std::endl;
  ss << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
