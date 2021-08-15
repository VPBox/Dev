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

#include "capabilities_packet.h"

namespace bluetooth {
namespace avrcp {

std::unique_ptr<GetCapabilitiesRequestBuilder>
GetCapabilitiesRequestBuilder::MakeBuilder(Capability capability) {
  std::unique_ptr<GetCapabilitiesRequestBuilder> builder(
      new GetCapabilitiesRequestBuilder(capability));

  return builder;
}

size_t GetCapabilitiesRequestBuilder::size() const {
  return GetCapabilitiesRequest::kMinSize();
}

bool GetCapabilitiesRequestBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the standard avrcp headers
  PacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers, the parameter length is always 1
  VendorPacketBuilder::PushHeader(pkt, 1);

  // Push the capability
  return AddPayloadOctets1(pkt, static_cast<uint8_t>(capability_));
}

Capability GetCapabilitiesRequest::GetCapabilityRequested() const {
  auto value = *(begin() + VendorPacket::kMinSize());
  return static_cast<Capability>(value);
}

bool GetCapabilitiesRequest::IsValid() const {
  if (!VendorPacket::IsValid()) return false;
  return (size() == VendorPacket::kMinSize() + 1);
}

std::string GetCapabilitiesRequest::ToString() const {
  std::stringstream ss;
  ss << "AvrcpCapabilityReqPacket: " << std::endl;
  ss << "  └ cType = " << GetCType() << std::endl;
  ss << "  └ Subunit Type = " << loghex(GetSubunitType()) << std::endl;
  ss << "  └ Subunit ID = " << loghex(GetSubunitId()) << std::endl;
  ss << "  └ OpCode = " << GetOpcode() << std::endl;
  ss << "  └ Company ID = " << loghex(GetCompanyId()) << std::endl;
  ss << "  └ Command PDU = " << GetCommandPdu() << std::endl;
  ss << "  └ PacketType = " << GetPacketType() << std::endl;
  ss << "  └ Parameter Length = " << loghex(GetParameterLength()) << std::endl;
  ss << "  └ Capabilities Request = " << GetCapabilityRequested() << std::endl;
  return ss.str();
}

std::unique_ptr<GetCapabilitiesResponseBuilder>
GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(
    uint32_t company_id_element) {
  std::unique_ptr<GetCapabilitiesResponseBuilder> builder(
      new GetCapabilitiesResponseBuilder(Capability::COMPANY_ID));

  company_id_element &= 0x00FFFFFF;
  builder->elements_.insert(company_id_element);

  return builder;
}

std::unique_ptr<GetCapabilitiesResponseBuilder>
GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(Event event) {
  std::unique_ptr<GetCapabilitiesResponseBuilder> builder(
      new GetCapabilitiesResponseBuilder(Capability::EVENTS_SUPPORTED));

  builder->elements_.insert(static_cast<uint8_t>(event));

  return builder;
}

GetCapabilitiesResponseBuilder* GetCapabilitiesResponseBuilder::AddCompanyId(
    uint32_t company_id) {
  CHECK_EQ(capability_, Capability::COMPANY_ID);
  CHECK_LT(elements_.size(), size_t(0xFF))
      << __func__ << ": maximum capability count reached";

  company_id &= 0x00FFFFFF;
  elements_.insert(company_id);

  return this;
}

GetCapabilitiesResponseBuilder* GetCapabilitiesResponseBuilder::AddEvent(
    Event event) {
  CHECK_EQ(capability_, Capability::EVENTS_SUPPORTED);
  CHECK_LT(elements_.size(), size_t(0xFF))
      << __func__ << ": maximum capability count reached";

  elements_.insert(static_cast<uint8_t>(event));

  return this;
}

size_t GetCapabilitiesResponseBuilder::size() const {
  // Company ID's have a size of 3 while Events have a size of 1
  size_t capability_count = elements_.size();
  size_t capability_size = capability_ == Capability::COMPANY_ID ? 3 : 1;

  return GetCapabilitiesResponse::kMinSize() +
         (capability_count * capability_size);
}

bool GetCapabilitiesResponseBuilder::Serialize(
    const std::shared_ptr<::bluetooth::Packet>& pkt) {
  ReserveSpace(pkt, size());

  // Push the standard avrcp headers
  PacketBuilder::PushHeader(pkt);

  // Push the avrcp vendor command headers
  uint16_t parameter_count = size() - VendorPacket::kMinSize();
  VendorPacketBuilder::PushHeader(pkt, parameter_count);

  // Push the capability, capability count, and elements
  AddPayloadOctets1(pkt, static_cast<uint8_t>(capability_));
  AddPayloadOctets1(pkt, elements_.size());
  for (auto it = elements_.begin(); it != elements_.end(); it++) {
    if (capability_ == Capability::COMPANY_ID)
      PushCompanyId(pkt, *it);
    else
      AddPayloadOctets1(pkt, *it);
  }

  return true;
}

}  // namespace avrcp
}  // namespace bluetooth