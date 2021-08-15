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

#include "avrcp_packet.h"

namespace bluetooth {
namespace avrcp {

class VendorPacketBuilder : public avrcp::PacketBuilder {
 public:
  virtual ~VendorPacketBuilder() = default;

  static std::unique_ptr<VendorPacketBuilder> MakeBuilder(
      CType ctype, CommandPdu pdu, PacketType packet_type,
      std::unique_ptr<::bluetooth::PacketBuilder> payload);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  CommandPdu pdu_;
  PacketType packet_type_;
  uint16_t param_length_;

  void PushHeader(const std::shared_ptr<::bluetooth::Packet>& pkt,
                  uint16_t parameter_length);

  // Helper function used a couple other AVRCP packet builders
  bool PushAttributeValue(const std::shared_ptr<::bluetooth::Packet>& pkt,
                          const AttributeEntry& entry);

  VendorPacketBuilder(CType ctype, CommandPdu pdu, PacketType packet_type)
      : PacketBuilder(ctype, 0x09, 0x00, Opcode::VENDOR),
        pdu_(pdu),
        packet_type_(packet_type){};
};

class VendorPacket : public avrcp::Packet {
 public:
  virtual ~VendorPacket() = default;

  /**
   * Avrcp Vendor Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t parameter_length;
   *   uint8_t[] payload;
   */
  static constexpr size_t kMinSize() { return Packet::kMinSize() + 7; };

  // Getter Functions
  uint32_t GetCompanyId() const;
  CommandPdu GetCommandPdu() const;
  PacketType GetPacketType() const;
  uint16_t GetParameterLength() const;

  // Overloaded functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using Packet::Packet;
};

}  // namespace avrcp
}  // namespace bluetooth