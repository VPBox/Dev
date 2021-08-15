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

#include <cstdint>
#include <forward_list>

#include "types/address.h"
#include "view.h"

namespace test_vendor_lib {
namespace packets {

// Templated Iterator for endianness
template <bool little_endian>
class Iterator : public std::iterator<std::random_access_iterator_tag, uint8_t> {
 public:
  Iterator(std::forward_list<View> data, size_t offset);
  Iterator(const Iterator& itr) = default;
  virtual ~Iterator() = default;

  // All addition and subtraction operators are unbounded.
  Iterator operator+(int offset);
  Iterator& operator+=(int offset);
  Iterator operator++(int);
  Iterator& operator++();

  Iterator operator-(int offset);
  int operator-(Iterator& itr);
  Iterator& operator-=(int offset);
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
  uint8_t operator->() const;

  size_t NumBytesRemaining() const;

  // Get the next sizeof(FixedWidthPODType) bytes and return the filled type
  template <typename FixedWidthPODType>
  FixedWidthPODType extract() {
    static_assert(std::is_pod<FixedWidthPODType>::value, "Iterator::extract requires an fixed type.");
    FixedWidthPODType extracted_value;
    uint8_t* value_ptr = (uint8_t*)&extracted_value;

    for (size_t i = 0; i < sizeof(FixedWidthPODType); i++) {
      size_t index = (little_endian ? i : sizeof(FixedWidthPODType) - i - 1);
      value_ptr[index] = *((*this)++);
    }
    return extracted_value;
  }

 private:
  std::forward_list<View> data_;
  size_t index_;
  size_t length_;
};

}  // namespace packets
}  // namespace test_vendor_lib
