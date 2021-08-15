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

class BrowsePacketBuilder : public ::bluetooth::PacketBuilder {
 public:
  virtual ~BrowsePacketBuilder() = default;

  static std::unique_ptr<BrowsePacketBuilder> MakeBuilder(
      BrowsePdu pdu, std::unique_ptr<::bluetooth::PacketBuilder> payload);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  BrowsePdu pdu_;
  std::unique_ptr<::bluetooth::PacketBuilder> payload_;

  void PushHeader(const std::shared_ptr<::bluetooth::Packet>& pkt,
                  uint16_t length);

  BrowsePacketBuilder(BrowsePdu pdu) : pdu_(pdu){};
};

class BrowsePacket : public ::bluetooth::Packet {
 public:
  virtual ~BrowsePacket() = default;

  static std::shared_ptr<BrowsePacket> Parse(
      std::shared_ptr<::bluetooth::Packet> pkt);

  /**
   * Avrcp Browse Packet Layout
   *   uint8_t pdu_;
   *   uint16_t length_;
   *   uint8_t[] payload_;
   */
  static constexpr size_t kMinSize() { return 3; }

  BrowsePdu GetPdu() const;
  uint16_t GetLength() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using ::bluetooth::Packet::Packet;

 private:
  virtual std::pair<size_t, size_t> GetPayloadIndecies() const override;
  DISALLOW_COPY_AND_ASSIGN(BrowsePacket);
};

}  // namespace avrcp
}  // namespace bluetooth