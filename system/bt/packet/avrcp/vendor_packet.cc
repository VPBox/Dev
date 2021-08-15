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

#include "vendor_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<VendorPacketBuilder> VendorPacketBuilder::MakeBuilder(
    CType ctype, CommandPdu pdu, PacketType packet_type,
    std::unique_ptr<::bluetooth::PacketBuilder> payload) {
  // If the payload size is greater than max uint16_t
  // the packet should be fragmented
  CHECK_LE(payload->size(), size_t(0xFFFF))
      << __func__ << ": payload size bigger than uint16_t";

  std::unique_ptr<VendorPacketBuilder> builder(
      new VendorPacketBuilder(ctype, pdu, packet_type));
  builder->payload_ = std::move(payload);

  return builder;
}

size_t VendorPacketBuilder::size() const {
  return VendorPacket::kMinSize() + payload_->size();
}

bool VendorPacketBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the standard avrcp headers
  PacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers
  CHECK_LT(payload_->size(), size_t(0xFFFF))
      << __func__ << ": payload size bigger than uint16_t";
  PushHeader(pkt, payload_->size());

  // Push the payload for the packet
  return payload_->Serialize(pkt);
}

void VendorPacketBuilder::PushHeader(
    const std::shared_ptr<::bluetooth::Packet>& pkt,
    uint16_t parameter_length) {
  PushCompanyId(pkt, BLUETOOTH_COMPANY_ID);
  AddPayloadOctets1(pkt, static_cast<uint8_t>(pdu_));
  AddPayloadOctets1(pkt, static_cast<uint8_t>(packet_type_));
  AddPayloadOctets2(pkt, base::ByteSwap(parameter_length));
}

bool VendorPacketBuilder::PushAttributeValue(
    const std::shared_ptr<::bluetooth::Packet>& pkt,
    const AttributeEntry& entry) {
  AddPayloadOctets4(pkt,
                    base::ByteSwap(static_cast<uint32_t>(entry.attribute())));
  uint16_t character_set = 0x006a;  // UTF-8
  AddPayloadOctets2(pkt, base::ByteSwap(character_set));
  uint16_t value_length = entry.value().length();
  AddPayloadOctets2(pkt, base::ByteSwap(value_length));
  for (int i = 0; i < value_length; i++) {
    AddPayloadOctets1(pkt, entry.value()[i]);
  }

  return true;
}

uint32_t VendorPacket::GetCompanyId() const {
  return PullCompanyId(begin() + Packet::kMinSize());
}

CommandPdu VendorPacket::GetCommandPdu() const {
  auto value = *(begin() + Packet::kMinSize() + static_cast<size_t>(3));
  return static_cast<CommandPdu>(value);
}

PacketType VendorPacket::GetPacketType() const {
  auto value = *(begin() + Packet::kMinSize() + static_cast<size_t>(4));
  return static_cast<PacketType>(value);
}

uint16_t VendorPacket::GetParameterLength() const {
  auto it = begin() + Packet::kMinSize() + static_cast<size_t>(5);
  // Swap to little endian
  return it.extractBE<uint16_t>();
}

bool VendorPacket::IsValid() const {
  if (size() < VendorPacket::kMinSize()) return false;

  auto start = begin() + VendorPacket::kMinSize();
  // Even if end is less than start and a sign extension occurs, thats fine as
  // its pretty definitive proof that the packet is poorly formated
  return GetParameterLength() == (end() - start);
}

std::string VendorPacket::ToString() const {
  std::stringstream ss;
  ss << "VendorPacket: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << GetCommandPdu() << std::endl;
  ss << "  └ PacketType = " << GetPacketType() << std::endl;
  ss << "  └ Parameter Length = " << loghex(GetParameterLength()) << std::endl;
  ss << "  └ Payload =";
  for (auto it = begin(); it != end(); it++) {
    ss << " " << loghex(*it);
  }
  ss << std::endl;
  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
