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

#include <base/logging.h>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "avrcp_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<PacketBuilder> PacketBuilder::MakeBuilder(
    CType type, uint8_t subunit_type, uint8_t subunit_id, Opcode opcode,
    std::unique_ptr<::bluetooth::PacketBuilder> payload) {
  std::unique_ptr<PacketBuilder> builder = std::unique_ptr<PacketBuilder>(
      new PacketBuilder(type, subunit_type, subunit_id, opcode));

  builder->payload_ = std::move(payload);

  return builder;
}

size_t PacketBuilder::size() const {
  // The size of the header for an Packet is 3
  return payload_->size() + Packet::kMinSize();
}

bool PacketBuilder::Serialize(const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the header for the packet
  PushHeader(pkt);

  // Push the payload for the packet
  return payload_->Serialize(pkt);
}

void PacketBuilder::PushHeader(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  AddPayloadOctets1(pkt, static_cast<uint8_t>(c_type_));
  AddPayloadOctets1(pkt, (subunit_type_ << 3) | subunit_id_);
  AddPayloadOctets1(pkt, static_cast<uint8_t>(opcode_));
}

bool PacketBuilder::PushCompanyId(
    const std::shared_ptr<::bluetooth::Packet>& pkt, uint32_t company_id) {
  company_id = base::ByteSwap(company_id);
  for (int i = 0; i < 3; i++) {
    company_id >>= 8;
    AddPayloadOctets1(pkt, company_id & 0xFF);
  }

  return true;
}

std::shared_ptr<Packet> Packet::Parse(
    std::shared_ptr<::bluetooth::Packet> pkt) {
  return std::shared_ptr<Packet>(new Packet(pkt));
}

CType Packet::GetCType() const {
  auto value = *begin() & 0x0F;
  return static_cast<CType>(value);
}

uint8_t Packet::GetSubunitType() const {
  return *(begin() + static_cast<size_t>(1)) >> 3;
}

uint8_t Packet::GetSubunitId() const {
  return *(begin() + static_cast<size_t>(1)) & 0b00000111;
}

Opcode Packet::GetOpcode() const {
  auto value = *(begin() + static_cast<size_t>(2));
  return static_cast<Opcode>(value);
}

bool Packet::IsValid() const { return size() >= kMinSize(); }

std::string Packet::ToString() const {
  std::stringstream ss;
  ss << "avrcp::Packet: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Payload =";
  for (auto it = begin() + static_cast<size_t>(3); it != end(); it++) {
    ss << " " << loghex(*it);
  }
  ss << std::endl;

  return ss.str();
}

std::pair<size_t, size_t> Packet::GetPayloadIndecies() const {
  return std::pair<size_t, size_t>(packet_start_index_ + 3, packet_end_index_);
}

}  // namespace avrcp
}  // namespace bluetooth