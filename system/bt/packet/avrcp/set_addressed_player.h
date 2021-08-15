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

#include "vendor_packet.h"

namespace bluetooth {
namespace avrcp {

class SetAddressedPlayerResponseBuilder : public VendorPacketBuilder {
 public:
  virtual ~SetAddressedPlayerResponseBuilder() = default;

  static std::unique_ptr<SetAddressedPlayerResponseBuilder> MakeBuilder(
      Status status);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Status status_;

  SetAddressedPlayerResponseBuilder(Status status)
      : VendorPacketBuilder(CType::ACCEPTED, CommandPdu::SET_ADDRESSED_PLAYER,
                            PacketType::SINGLE),
        status_(status){};
};

class SetAddressedPlayerRequest : public VendorPacket {
 public:
  virtual ~SetAddressedPlayerRequest() = default;

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
   *   SetAddressedPlayerRequest:
   *     uint16_t player_id;
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 2; }

  uint16_t GetPlayerId() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

}  // namespace avrcp
}  // namespace bluetooth