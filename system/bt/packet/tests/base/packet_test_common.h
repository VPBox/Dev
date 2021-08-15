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

#include "packet.h"
#include "packet_test_helper.h"

// We have our own definition of loghex to avoid dependencies
namespace {
template <typename T>
std::string loghex(T x) {
  std::stringstream tmp;
  tmp << "0x" << std::internal << std::hex << std::setfill('0')
      << std::setw(sizeof(T) * 2) << (unsigned int)x;
  return tmp.str();
}
}  // namespace

namespace bluetooth {

class PacketImpl : public Packet {
 public:
  using Packet::Packet;  // Inherit constructors

  virtual bool IsValid() const { return true; }

  virtual std::string ToString() const {
    std::stringstream ss;
    ss << "TestPacket: Start = " << packet_start_index_
       << " : End = " << packet_end_index_ << std::endl;
    ss << "  └ Payload =";
    for (auto it = begin(); it != end(); it++) {
      ss << " " << loghex(*it);
    }
    ss << std::endl;

    return ss.str();
  };

  virtual std::pair<size_t, size_t> GetPayloadIndecies() const {
    return std::pair<size_t, size_t>(packet_start_index_, packet_end_index_);
  }
};

using TestPacket = TestPacketType<PacketImpl>;

// A helper class that has public accessors to protected methods
class TestPacketBuilder : public PacketBuilder {
 public:
  static std::unique_ptr<TestPacketBuilder> MakeBuilder(
      std::vector<uint8_t> data) {
    std::unique_ptr<TestPacketBuilder> builder(new TestPacketBuilder(data));
    return builder;
  };

  // Make all the utility functions public
  using PacketBuilder::ReserveSpace;
  using PacketBuilder::AddPayloadOctets1;
  using PacketBuilder::AddPayloadOctets2;
  using PacketBuilder::AddPayloadOctets3;
  using PacketBuilder::AddPayloadOctets4;
  using PacketBuilder::AddPayloadOctets6;
  using PacketBuilder::AddPayloadOctets8;

  size_t size() const override { return data_.size(); };

  bool Serialize(const std::shared_ptr<Packet>& pkt) override {
    ReserveSpace(pkt, size());

    for (uint8_t byte : data_) {
      AddPayloadOctets1(pkt, byte);
    }

    return true;
  }

  TestPacketBuilder(std::vector<uint8_t> data) : data_(data){};

  std::vector<uint8_t> data_;
};

}  // namespace bluetooth