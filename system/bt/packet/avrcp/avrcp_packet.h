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

#include <base/logging.h>
#include <base/macros.h>
#include <iostream>

#include "hardware/avrcp/avrcp_common.h"
#include "hardware/avrcp/avrcp_logging_helper.h"
#include "packet/base/iterator.h"
#include "packet/base/packet.h"
#include "packet/base/packet_builder.h"

namespace bluetooth {
namespace avrcp {

class PacketBuilder : public ::bluetooth::PacketBuilder {
 public:
  virtual ~PacketBuilder() = default;

  static std::unique_ptr<PacketBuilder> MakeBuilder(
      CType cType, uint8_t subunit_type, uint8_t subunit_id, Opcode opcode,
      std::unique_ptr<::bluetooth::PacketBuilder> packet);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  CType c_type_;
  uint8_t subunit_type_ : 5;
  uint8_t subunit_id_ : 3;
  Opcode opcode_;
  std::unique_ptr<::bluetooth::PacketBuilder> payload_;

  void PushHeader(const std::shared_ptr<::bluetooth::Packet>& pkt);
  bool PushCompanyId(const std::shared_ptr<::bluetooth::Packet>& pkt,
                     uint32_t company_id);

  PacketBuilder(CType type, uint8_t subunit_type, uint8_t subunit_id,
                Opcode opcode)
      : c_type_(type),
        subunit_type_(subunit_type),
        subunit_id_(subunit_id),
        opcode_(opcode){};
};

class Packet : public ::bluetooth::Packet {
 public:
  virtual ~Packet() = default;

  // TODO (apanicke): Right now we can use this to build an AvrcpPacket from
  // another packet type. In the future, we can remove this in favor of
  // getting an AVRCP Packet directly from an AVCTP Packet
  static std::shared_ptr<Packet> Parse(
      std::shared_ptr<::bluetooth::Packet> pkt);

  /**
   * Avrcp Packet Layout
   *   CType c_type_;
   *   uint8_t subunit_type_ : 5;
   *   uint8_t subunit_id_ : 3;
   *   Opcode opcode_;
   *   uint8_t[] payload_;
   */
  static constexpr size_t kMinSize() { return 3; };

  // Getter Functions
  CType GetCType() const;
  uint8_t GetSubunitType() const;
  uint8_t GetSubunitId() const;
  Opcode GetOpcode() const;

  // Overloaded Functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using ::bluetooth::Packet::Packet;

  static inline uint32_t PullCompanyId(Iterator it) {
    uint32_t value = 0;
    for (int i = 0; i < 3; i++) {
      value <<= 8;
      value |= *it++;
    }
    return value;
  }

 private:
  virtual std::pair<size_t, size_t> GetPayloadIndecies() const override;
  DISALLOW_COPY_AND_ASSIGN(Packet);
};

}  // namespace avrcp
}  // namespace bluetooth
