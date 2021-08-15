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

#include "get_item_attributes.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<GetItemAttributesResponseBuilder>
GetItemAttributesResponseBuilder::MakeBuilder(Status status, size_t mtu) {
  std::unique_ptr<GetItemAttributesResponseBuilder> builder(
      new GetItemAttributesResponseBuilder(status, mtu));

  return builder;
}

bool GetItemAttributesResponseBuilder::AddAttributeEntry(AttributeEntry entry) {
  CHECK(entries_.size() < 0xFF);

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

bool GetItemAttributesResponseBuilder::AddAttributeEntry(Attribute attribute,
                                                         std::string value) {
  return AddAttributeEntry(AttributeEntry(attribute, value));
}

size_t GetItemAttributesResponseBuilder::size() const {
  size_t len = BrowsePacket::kMinSize();
  len += 1;  // Status
  if (status_ != Status::NO_ERROR) return len;

  len += 1;  // Number of attributes
  for (const auto& entry : entries_) {
    len += entry.size();
  }
  return len;
}

bool GetItemAttributesResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  BrowsePacketBuilder::PushHeader(pkt, size() - BrowsePacket::kMinSize());

  AddPayloadOctets1(pkt, (uint8_t)status_);
  if (status_ != Status::NO_ERROR) return true;

  AddPayloadOctets1(pkt, entries_.size());
  for (const auto& entry : entries_) {
    AddPayloadOctets4(pkt, base::ByteSwap((uint32_t)entry.attribute()));
    uint16_t character_set = 0x006a;  // UTF-8
    AddPayloadOctets2(pkt, base::ByteSwap(character_set));
    uint16_t value_length = entry.value().length();
    AddPayloadOctets2(pkt, base::ByteSwap(value_length));
    for (const uint8_t& byte : entry.value()) {
      AddPayloadOctets1(pkt, byte);
    }
  }

  return true;
}

Scope GetItemAttributesRequest::GetScope() const {
  auto it = begin() + BrowsePacket::kMinSize();
  return static_cast<Scope>(*it);
}

uint64_t GetItemAttributesRequest::GetUid() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(1);
  return it.extractBE<uint64_t>();
}

uint16_t GetItemAttributesRequest::GetUidCounter() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(9);
  return it.extractBE<uint16_t>();
}

uint8_t GetItemAttributesRequest::GetNumAttributes() const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(11);
  return *it;
}

std::vector<Attribute> GetItemAttributesRequest::GetAttributesRequested()
    const {
  auto it = begin() + BrowsePacket::kMinSize() + static_cast<size_t>(11);
  size_t number_of_attributes = it.extract<uint8_t>();

  std::vector<Attribute> attribute_list;
  for (size_t i = 0; i < number_of_attributes; i++) {
    attribute_list.push_back((Attribute)it.extractBE<uint32_t>());
  }

  return attribute_list;
}

bool GetItemAttributesRequest::IsValid() const {
  if (!BrowsePacket::IsValid()) return false;
  if (size() < kMinSize()) return false;

  // Casting the int returned from end - attr_start should be fine. If an
  // overflow occurs we can definitly say the packet is invalid
  return (GetNumAttributes() * sizeof(Attribute)) == (size() - kMinSize());
}

std::string GetItemAttributesRequest::ToString() const {
  std::stringstream ss;
  ss << "GetItemAttributesRequestPacket: " << std::endl;
  ss << "  └ PDU = " << GetPdu() << std::endl;
  ss << "  └ Length = " << GetLength() << std::endl;
  ss << "  └ Scope = " << GetScope() << std::endl;
  ss << "  └ UID Requested = " << loghex(GetUid()) << std::endl;
  ss << "  └ UID Counter = " << loghex(GetUidCounter()) << std::endl;
  ss << "  └ Num Attributes = " << loghex(GetNumAttributes()) << std::endl;

  auto attr_list = GetAttributesRequested();
  ss << "  └ Attribute List: Size: " << attr_list.size() << std::endl;
  for (auto it = attr_list.begin(); it != attr_list.end(); it++) {
    ss << "      └ " << loghex((uint32_t)(*it)) << std::endl;
  }
  ss << std::endl;

  return ss.str();
}

}  // namespace avrcp
}  // namespace bluetooth
