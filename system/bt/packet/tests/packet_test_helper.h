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

#include <memory>

#include "packet.h"

namespace bluetooth {

// A helper templated class to access the protected members of Packet to make
// testing easier
template <class PacketType>
class TestPacketType : public PacketType {
 public:
  using PacketType::PacketType;

  static std::shared_ptr<TestPacketType<PacketType>> Make() {
    return std::shared_ptr<TestPacketType<PacketType>>(
        new TestPacketType<PacketType>());
  }

  static std::shared_ptr<TestPacketType<PacketType>> Make(
      std::shared_ptr<Packet> packet) {
    return std::shared_ptr<TestPacketType<PacketType>>(
        new TestPacketType<PacketType>(packet));
  }

  static std::shared_ptr<TestPacketType<PacketType>> Make(
      std::vector<uint8_t> payload) {
    size_t end = payload.size();
    return Make(std::move(payload), 0, end);
  }

  static std::shared_ptr<TestPacketType<PacketType>> Make(
      std::vector<uint8_t> payload, size_t start, size_t end) {
    auto pkt = std::shared_ptr<TestPacketType<PacketType>>(
        new TestPacketType<PacketType>());
    pkt->packet_start_index_ = start;
    pkt->packet_end_index_ = end;
    pkt->data_ = std::make_shared<std::vector<uint8_t>>(std::move(payload));
    return pkt;
  }

  const std::vector<uint8_t>& GetData() { return *PacketType::data_; }

  std::shared_ptr<std::vector<uint8_t>> GetDataPointer() {
    return PacketType::data_;
  }
};

}  // namespace bluetooth