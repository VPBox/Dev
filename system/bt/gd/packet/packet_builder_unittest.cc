/*
 * Copyright 2019 The Android Open Source Project
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

#include "packet/packet_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

using bluetooth::packet::BasePacketBuilder;
using bluetooth::packet::BitInserter;
using bluetooth::packet::PacketBuilder;
using std::vector;

namespace {
vector<uint8_t> count_all = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

vector<uint8_t> count_1 = {
    0x00,
    0x01,
    0x02,
};

vector<uint8_t> count_2 = {
    0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
};

vector<uint8_t> count_3 = {
    0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};
}  // namespace

namespace bluetooth {
namespace packet {

template <bool little_endian>
class EndianBuilder : public PacketBuilder<little_endian> {
 public:
  EndianBuilder(uint8_t byte, uint16_t two_bytes, uint32_t four_bytes, uint64_t eight_bytes)
      : byte_(byte), two_bytes_(two_bytes), four_bytes_(four_bytes), eight_bytes_(eight_bytes) {}
  ~EndianBuilder() = default;

  virtual size_t size() const override {
    return sizeof(signature_) + sizeof(byte_) + sizeof(two_bytes_) + sizeof(four_bytes_) + sizeof(eight_bytes_);
  }

  virtual const std::unique_ptr<std::vector<uint8_t>> FinalPacket() {
    std::unique_ptr<std::vector<uint8_t>> packet = std::make_unique<std::vector<uint8_t>>();
    packet->reserve(size());
    BitInserter it(*packet);
    Serialize(it);
    return packet;
  }

  virtual void Serialize(BitInserter& it) const override {
    PacketBuilder<little_endian>::insert(signature_, it);
    PacketBuilder<little_endian>::insert(byte_, it);
    PacketBuilder<little_endian>::insert(two_bytes_, it);
    PacketBuilder<little_endian>::insert(four_bytes_, it);
    PacketBuilder<little_endian>::insert(eight_bytes_, it);
  }

 private:
  uint32_t signature_{(little_endian ? 0x03020100 : 0x00010203)};
  uint8_t byte_;
  uint16_t two_bytes_;
  uint32_t four_bytes_;
  uint64_t eight_bytes_;
};

class PacketBuilderEndianTest : public ::testing::Test {
 public:
  PacketBuilderEndianTest() = default;
  ~PacketBuilderEndianTest() = default;
};

TEST(PacketBuilderEndianTest, insertTest) {
  EndianBuilder<true> little(0x04, 0x0605, 0x0a090807, 0x1211100f0e0d0c0b);
  EndianBuilder<false> big(0x04, 0x0506, 0x0708090a, 0x0b0c0d0e0f101112);
  ASSERT_EQ(*big.FinalPacket(), *little.FinalPacket());
}

template <typename T>
class VectorBuilder : public PacketBuilder<true> {
 public:
  VectorBuilder(std::vector<uint64_t> vect) {
    for (uint64_t element : vect) {
      vect.push_back(static_cast<T>(element));
    }
  }
  ~VectorBuilder() = default;

  virtual size_t size() const override {
    return vect_.size() * sizeof(T);
  }

  virtual const std::unique_ptr<std::vector<uint8_t>> FinalPacket() {
    std::unique_ptr<std::vector<uint8_t>> packet = std::make_unique<std::vector<uint8_t>>();
    packet->reserve(size());
    BitInserter it(*packet);
    Serialize(it);
    return packet;
  }

  virtual void Serialize(BitInserter& it) const override {
    PacketBuilder<true>::insert_vector(vect_, it);
  }

 private:
  std::vector<T> vect_;
};

template <typename T>
class InsertElementsBuilder : public PacketBuilder<true> {
 public:
  InsertElementsBuilder(std::vector<uint64_t> vect) {
    for (uint64_t element : vect) {
      vect.push_back(static_cast<T>(element));
    }
  }
  virtual ~InsertElementsBuilder() = default;

  virtual size_t size() const override {
    return vect_.size() * sizeof(T);
  }

  virtual const std::unique_ptr<std::vector<uint8_t>> FinalPacket() {
    std::unique_ptr<std::vector<uint8_t>> packet = std::make_unique<std::vector<uint8_t>>();
    packet->reserve(size());
    BitInserter it(*packet);
    Serialize(it);
    return packet;
  }

  virtual void Serialize(BitInserter& it) const override {
    for (T elem : vect_) {
      PacketBuilder<true>::insert(elem, it);
    }
  }

 private:
  std::vector<T> vect_;
};

std::vector<uint64_t> vector_data{
    0x7060504030201000, 0x7161514131211101, 0x7262524232221202, 0x7363534333231303, 0x7464544434241404,
    0x7565554535251505, 0x7666564636261606, 0x7767574737271707, 0x7868584838281808,
};

template <typename T>
class VectorBuilderTest : public ::testing::Test {
 public:
  VectorBuilderTest() = default;
  ~VectorBuilderTest() = default;

  void SetUp() {
    packet_1_ = std::shared_ptr<VectorBuilder<T>>(new VectorBuilder<T>(vector_data));
    packet_2_ = std::shared_ptr<InsertElementsBuilder<T>>(new InsertElementsBuilder<T>(vector_data));
  }

  void TearDown() {
    packet_1_.reset();
    packet_2_.reset();
  }

  std::shared_ptr<VectorBuilder<T>> packet_1_;
  std::shared_ptr<InsertElementsBuilder<T>> packet_2_;
};

using VectorBaseTypes = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t>;
TYPED_TEST_CASE(VectorBuilderTest, VectorBaseTypes);

TYPED_TEST(VectorBuilderTest, insertVectorTest) {
  ASSERT_EQ(*(this->packet_1_->FinalPacket()), *(this->packet_2_->FinalPacket()));
}

class NestedBuilder : public PacketBuilder<true> {
 public:
  ~NestedBuilder() = default;

  virtual size_t size() const override {
    size_t payload_size = (payload_ ? payload_->size() : 0);
    return 1 + payload_size;
  }

  static std::unique_ptr<NestedBuilder> Create(uint8_t level) {
    return std::unique_ptr<NestedBuilder>(new NestedBuilder(level));
  }

  static std::unique_ptr<NestedBuilder> CreateNested(std::unique_ptr<BasePacketBuilder> payload, uint8_t level) {
    return std::unique_ptr<NestedBuilder>(new NestedBuilder(std::move(payload), level));
  }

  virtual const std::unique_ptr<std::vector<uint8_t>> FinalPacket() {
    std::unique_ptr<std::vector<uint8_t>> packet = std::make_unique<std::vector<uint8_t>>();
    packet->reserve(size());
    BitInserter it(*packet);
    Serialize(it);
    return packet;
  }

  virtual void Serialize(BitInserter& it) const override {
    PacketBuilder<true>::insert(level_, it);
    if (payload_) {
      payload_->Serialize(it);
    }
  }

 private:
  std::unique_ptr<BasePacketBuilder> payload_;
  uint8_t level_;

  NestedBuilder(std::unique_ptr<BasePacketBuilder> inner, uint8_t level) : payload_(std::move(inner)), level_(level) {}
  NestedBuilder(uint8_t level) : level_(level) {}
};

class BuilderBuilderTest : public ::testing::Test {};

TEST(BuilderBuilderTest, nestingTest) {
  std::unique_ptr<BasePacketBuilder> innermost = NestedBuilder::Create(0);
  std::unique_ptr<BasePacketBuilder> number_1 = NestedBuilder::CreateNested(std::move(innermost), 1);
  std::unique_ptr<BasePacketBuilder> number_2 = NestedBuilder::CreateNested(std::move(number_1), 2);
  std::unique_ptr<BasePacketBuilder> number_3 = NestedBuilder::CreateNested(std::move(number_2), 3);
  std::unique_ptr<BasePacketBuilder> number_4 = NestedBuilder::CreateNested(std::move(number_3), 4);
  std::unique_ptr<NestedBuilder> number_5 = NestedBuilder::CreateNested(std::move(number_4), 5);

  std::vector<uint8_t> count_down{5, 4, 3, 2, 1, 0};
  ASSERT_EQ(*number_5->FinalPacket(), count_down);
}
}  // namespace packet
}  // namespace bluetooth
