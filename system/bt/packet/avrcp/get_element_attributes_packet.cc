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

#include <algorithm>

#include "get_element_attributes_packet.h"

namespace bluetooth {
namespace avrcp {

uint64_t GetElementAttributesRequest::GetIdentifier() const {
  auto it = begin() + VendorPacket::kMinSize();
  return it.extract<uint64_t>();
}

uint8_t GetElementAttributesRequest::GetNumAttributes() const {
  auto it = begin() + VendorPacket::kMinSize() + static_cast<size_t>(8);
  return it.extract<uint8_t>();
}

std::vector<Attribute> GetElementAttributesRequest::GetAttributesRequested()
    const {
  auto it = begin() + VendorPacket::kMinSize() + static_cast<size_t>(8);

  size_t number_of_attributes = it.extract<uint8_t>();

  std::vector<Attribute> attribute_list;

  for (size_t i = 0; i < number_of_attributes; i++) {
    attribute_list.push_back((Attribute)it.extractBE<uint32_t>());
  }

  return attribute_list;
}

bool GetElementAttributesRequest::IsValid() const {
  if (!VendorPacket::IsValid()) return false;
  if (size() < kMinSize()) return false;

  size_t num_attributes = GetNumAttributes();
  auto attr_start = begin() + VendorPacket::kMinSize() + static_cast<size_t>(9);

  // Casting the int returned from end - attr_start should be fine. If an
  // overflow occurs we can definitly say the packet is invalid
  return (num_attributes * sizeof(Attribute)) == (size_t)(end() - attr_start);
}

std::string GetElementAttributesRequest::ToString() const {
  std::stringstream ss;
  ss << "RegisterNotificationPacket: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << GetCommandPdu() << std::endl;
  ss << "  └ PacketType = " << GetPacketType() << std::endl;
  ss << "  └ Parameter Length = " << GetParameterLength() << std::endl;
  ss << "  └ Identifier = " << loghex(GetIdentifier()) << std::endl;

  auto attr_list = GetAttributesRequested();

  ss << "  └ Attribute List: Size: " << attr_list.size() << std::endl;
  for (auto it = attr_list.begin(); it != attr_list.end(); it++) {
    ss << "      └ " << loghex((uint32_t)(*it)) << std::endl;
  }
  ss << std::endl;

  return ss.str();
}

std::unique_ptr<GetElementAttributesResponseBuilder>
GetElementAttributesResponseBuilder::MakeBuilder(size_t mtu) {
  std::unique_ptr<GetElementAttributesResponseBuilder> builder(
      new GetElementAttributesResponseBuilder(mtu));

  return builder;
}

bool GetElementAttributesResponseBuilder::AddAttributeEntry(
    AttributeEntry entry) {
  CHECK_LT(entries_.size(), size_t(0xFF))
      << __func__ << ": attribute entry overflow";

  size_t remaining_space = mtu_ - size();
  if (entry.size() > remaining_space) {
    entry.resize(remaining_space);
  }

  if (entry.empty()) {
    return false;
  }

  entries_.insert(entry);
  return true;
}

bool GetElementAttributesResponseBuilder::AddAttributeEntry(Attribute attribute,
                                                            std::string value) {
  return AddAttributeEntry(AttributeEntry(attribute, value));
}

size_t GetElementAttributesResponseBuilder::size() const {
  size_t attr_list_size = 0;

  for (auto& attribute_entry : entries_) {
    attr_list_size += attribute_entry.size();
  }

  return VendorPacket::kMinSize() + 1 + attr_list_size;
}

bool GetElementAttributesResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  PacketBuilder::PushHeader(pkt);

  VendorPacketBuilder::PushHeader(pkt, size() - VendorPacket::kMinSize());

  AddPayloadOctets1(pkt, entries_.size());
  for (const auto& attribute_entry : entries_) {
    PushAttributeValue(pkt, attribute_entry);
  }

  return true;
}

}  // namespace avrcp
}  // namespace bluetooth
