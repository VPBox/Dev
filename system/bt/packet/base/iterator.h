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

#include <iterator>
#include <memory>

namespace bluetooth {

// Forward declare Packet class
class Packet;

// Iterator is a custom iterator class for Packets.
class Iterator
    : public std::iterator<std::random_access_iterator_tag, uint8_t> {
 public:
  Iterator(std::shared_ptr<const Packet> packet, size_t i);
  Iterator(const Iterator& itr);

  // All addition and subtraction operators are bounded from 0 to the length of
  // the packet.
  Iterator operator+(size_t offset);
  Iterator& operator+=(size_t offset);
  Iterator operator++(int);
  Iterator& operator++();

  Iterator operator-(size_t offset);
  int operator-(const Iterator& itr);
  Iterator& operator-=(size_t offset);
  Iterator operator--(int);
  Iterator& operator--();

  Iterator& operator=(const Iterator& itr);

  bool operator!=(const Iterator& itr) const;
  bool operator==(const Iterator& itr) const;

  bool operator<(const Iterator& itr) const;
  bool operator>(const Iterator& itr) const;

  bool operator<=(const Iterator& itr) const;
  bool operator>=(const Iterator& itr) const;

  uint8_t operator*() const;

  template <typename FixedWidthIntegerType>
  FixedWidthIntegerType extract() {
    static_assert(std::is_integral<FixedWidthIntegerType>::value,
                  "Iterator::extract requires an integral type.");

    FixedWidthIntegerType extracted_value = 0;
    for (size_t i = 0; i < sizeof(FixedWidthIntegerType); i++) {
      extracted_value |= static_cast<FixedWidthIntegerType>(**this) << i * 8;
      (*this)++;
    }

    return extracted_value;
  }

  // Extract in Little Endian Format
  template <typename FixedWidthIntegerType>
  FixedWidthIntegerType extractBE() {
    static_assert(std::is_integral<FixedWidthIntegerType>::value,
                  "Iterator::extract requires an integral type.");

    FixedWidthIntegerType extracted_value = 0;
    for (size_t i = 0; i < sizeof(FixedWidthIntegerType); i++) {
      extracted_value |= static_cast<FixedWidthIntegerType>(**this)
                         << (sizeof(FixedWidthIntegerType) - 1 - i) * 8;
      (*this)++;
    }

    return extracted_value;
  }

  uint8_t extract8() { return extract<uint8_t>(); }
  uint16_t extract16() { return extract<uint16_t>(); }
  uint32_t extract32() { return extract<uint32_t>(); }
  uint64_t extract64() { return extract<uint64_t>(); }

 private:
  std::shared_ptr<const Packet> packet_;
  size_t index_;
};  // Iterator

}  // namespace bluetooth
