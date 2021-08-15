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

// TODO (apanicke): Set Absolute Volume request vs response have the same
// packet structure, the only difference between the two is the CType.
// Adding a passed flag as a parameter would be possible but I feel that
// this would break the design pattern of request vs response. Look into
// this for the future.

namespace bluetooth {
namespace avrcp {

class SetAbsoluteVolumeRequestBuilder : public VendorPacketBuilder {
 public:
  virtual ~SetAbsoluteVolumeRequestBuilder() = default;

  static std::unique_ptr<SetAbsoluteVolumeRequestBuilder> MakeBuilder(
      uint8_t volume);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  uint8_t volume_;

  SetAbsoluteVolumeRequestBuilder(uint8_t volume)
      : VendorPacketBuilder(CType::CONTROL, CommandPdu::SET_ABSOLUTE_VOLUME,
                            PacketType::SINGLE),
        volume_(volume){};
};

class SetAbsoluteVolumeResponse : public VendorPacket {
 public:
  virtual ~SetAbsoluteVolumeResponse() = default;

  /**
   * AVRCP Play Item Request Packet Layout
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
   *   SetAbsoluteVolumeResponse:
   *     uint8_t volume;
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 1; }

  uint8_t GetVolume() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

}  // namespace avrcp
}  // namespace bluetooth