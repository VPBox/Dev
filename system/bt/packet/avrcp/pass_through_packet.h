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

class PassThroughPacketBuilder : public PacketBuilder {
 public:
  virtual ~PassThroughPacketBuilder() = default;

  static std::unique_ptr<PassThroughPacketBuilder> MakeBuilder(
      bool response, bool pushed, uint8_t operation_id);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 private:
  bool pushed_;
  uint8_t opperation_id_;

  PassThroughPacketBuilder(bool response, bool pushed, uint8_t opperation_id)
      : PacketBuilder(response ? CType::ACCEPTED : CType::CONTROL, 0x09, 0x00,
                      Opcode::PASS_THROUGH),
        pushed_(pushed),
        opperation_id_(opperation_id){};
};

class PassThroughPacket : public Packet {
 public:
  virtual ~PassThroughPacket() = default;

  /**
   * Pass Through Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   PassThroughPacket:
   *     uint8_t state : 1;
   *     uint8_t opperation_id : 7;
   *     uint8_t data_length;
   */
  static constexpr size_t kMinSize() { return Packet::kMinSize() + 2; }

  // Getter Functions
  KeyState GetKeyState() const;
  uint8_t GetOperationId() const;

  // Overloaded Functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using Packet::Packet;
};

}  // namespace avrcp
}  // namespace bluetooth