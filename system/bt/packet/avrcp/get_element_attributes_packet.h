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

#pragma once

#include <set>
#include "vendor_packet.h"

namespace bluetooth {
namespace avrcp {

class GetElementAttributesRequest : public VendorPacket {
 public:
  virtual ~GetElementAttributesRequest() = default;

  /**
   *  Register Notificaiton Request Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t param_length;
   *   RegisterNotificationRequestPacket:
   *     uint64_t identifier;
   *     uint8_t number_of_attributes;
   *     uint32_t attributes_requested[];
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 9; }

  // Getter Functions
  uint64_t GetIdentifier() const;
  uint8_t GetNumAttributes() const;
  std::vector<Attribute> GetAttributesRequested() const;

  // Overloaded Functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

class GetElementAttributesResponseBuilder : public VendorPacketBuilder {
 public:
  virtual ~GetElementAttributesResponseBuilder() = default;

  static std::unique_ptr<GetElementAttributesResponseBuilder> MakeBuilder(
      size_t mtu);

  bool AddAttributeEntry(AttributeEntry entry);
  bool AddAttributeEntry(Attribute attribute, std::string value);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 private:
  std::set<AttributeEntry> entries_;
  size_t mtu_;

  GetElementAttributesResponseBuilder(size_t mtu)
      : VendorPacketBuilder(CType::STABLE, CommandPdu::GET_ELEMENT_ATTRIBUTES,
                            PacketType::SINGLE),
        mtu_(mtu){};
};

}  // namespace avrcp
}  // namespace bluetooth