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

class PlayItemResponseBuilder : public VendorPacketBuilder {
 public:
  virtual ~PlayItemResponseBuilder() = default;

  static std::unique_ptr<PlayItemResponseBuilder> MakeBuilder(Status status);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Status status_;

  PlayItemResponseBuilder(Status status)
      : VendorPacketBuilder(CType::ACCEPTED, CommandPdu::PLAY_ITEM,
                            PacketType::SINGLE),
        status_(status){};
};

class PlayItemRequest : public VendorPacket {
 public:
  virtual ~PlayItemRequest() = default;

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
   *   PlayItemRequest:
   *     uint8_t scope_;
   *     uint64_t uid_;
   *     uint16_t uid_counter_;
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 11; }

  Scope GetScope() const;
  uint64_t GetUid() const;
  uint16_t GetUidCounter() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

}  // namespace avrcp
}  // namespace bluetooth