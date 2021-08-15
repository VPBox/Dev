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

#pragma once

#include <cstdint>
#include <forward_list>
#include <iterator>
#include <memory>
#include <vector>

#include "common/address.h"
#include "common/class_of_device.h"
#include "os/log.h"
#include "packet/base_packet_builder.h"
#include "packet/bit_inserter.h"

namespace bluetooth {
namespace packet {

// Abstract base class that is subclassed to build specifc packets.
// The template parameter little_endian controls the generation of insert().
template <bool little_endian>
class PacketBuilder : public BasePacketBuilder {
 public:
  PacketBuilder() = default;
  virtual ~PacketBuilder() = default;

  // Classes which need fragmentation should define a function like this:
  // std::forward_list<DerivedBuilder>& Fragment(size_t max_size);

 protected:
  // Write sizeof(FixedWidthIntegerType) bytes using the iterator
  template <typename FixedWidthIntegerType,
            typename std::enable_if<std::is_integral<FixedWidthIntegerType>::value, int>::type = 0>
  void insert(FixedWidthIntegerType value, BitInserter& it) const {
    for (size_t i = 0; i < sizeof(FixedWidthIntegerType); i++) {
      if (little_endian == true) {
        it.insert_byte(static_cast<uint8_t>(value >> (i * 8)));
      } else {
        it.insert_byte(static_cast<uint8_t>(value >> ((sizeof(FixedWidthIntegerType) - i - 1) * 8)));
      }
    }
  }

  // Write num_bits bits using the iterator
  template <typename FixedWidthIntegerType,
            typename std::enable_if<std::is_integral<FixedWidthIntegerType>::value, int>::type = 0>
  void insert(FixedWidthIntegerType value, BitInserter& it, size_t num_bits) const {
    ASSERT(num_bits <= (sizeof(FixedWidthIntegerType) * 8));

    for (size_t i = 0; i < num_bits / 8; i++) {
      if (little_endian == true) {
        it.insert_byte(static_cast<uint8_t>(value >> (i * 8)));
      } else {
        it.insert_byte(static_cast<uint8_t>(value >> (((num_bits / 8) - i - 1) * 8)));
      }
    }
    if (num_bits % 8) {
      it.insert_bits(static_cast<uint8_t>(value >> ((num_bits / 8) * 8)), num_bits % 8);
    }
  }

  // Specialized insert that allows inserting enums without casting
  template <typename Enum, typename std::enable_if<std::is_enum_v<Enum>, int>::type = 0>
  inline void insert(Enum value, BitInserter& it) const {
    using enum_type = typename std::underlying_type_t<Enum>;
    static_assert(std::is_unsigned_v<enum_type>, "Enum type is signed. Did you forget to specify the enum size?");
    insert<enum_type>(static_cast<enum_type>(value), it);
  }

  // Write a vector of FixedWidthIntegerType using the iterator
  template <typename FixedWidthIntegerType>
  void insert_vector(const std::vector<FixedWidthIntegerType>& vec, BitInserter& it) const {
    static_assert(std::is_integral<FixedWidthIntegerType>::value,
                  "PacketBuilder::insert requires an integral type vector.");
    for (const auto& element : vec) {
      insert(element, it);
    }
  }

  void insert_address(const common::Address& addr, BitInserter& it) const {
    for (const auto& element : addr.address) {
      insert(element, it);
    }
  }

  void insert_class_of_device(const common::ClassOfDevice& cod, BitInserter& it) const {
    for (const auto& element : cod.cod) {
      insert(element, it);
    }
  }
};

}  // namespace packet
}  // namespace bluetooth
