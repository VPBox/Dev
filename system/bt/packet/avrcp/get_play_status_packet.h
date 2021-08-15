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

// TODO (apanicke): This packet doesn't really need to exist as it provides
// zero extra information other than it is a strong type and provides a
// validator
class GetPlayStatusRequest : public VendorPacket {
 public:
  virtual ~GetPlayStatusRequest() = default;

  /**
   *  Get Capabilities Response Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t param_length = 0;
   */
  static constexpr size_t kMinSize() { return Packet::kMinSize() + 7; }

  // Overloaded Functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

class GetPlayStatusResponseBuilder : public VendorPacketBuilder {
 public:
  virtual ~GetPlayStatusResponseBuilder() = default;

  static std::unique_ptr<GetPlayStatusResponseBuilder> MakeBuilder(
      uint32_t song_length, uint32_t song_position, uint8_t play_status);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  uint32_t song_length_;
  uint32_t song_position_;
  uint8_t play_status_;

  GetPlayStatusResponseBuilder(uint32_t song_length, uint32_t song_position,
                               uint8_t play_status)
      : VendorPacketBuilder(CType::STABLE, CommandPdu::GET_PLAY_STATUS,
                            PacketType::SINGLE),
        song_length_(song_length),
        song_position_(song_position),
        play_status_(play_status){};
};

}  // namespace avrcp
}  // namespace bluetooth