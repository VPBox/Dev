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
#include <iterator>
#include <memory>
#include <vector>

#include "os/log.h"

namespace bluetooth {
namespace packet {

class BitInserter : public std::back_insert_iterator<std::vector<uint8_t>> {
 public:
  BitInserter(std::vector<uint8_t>& vector) : std::back_insert_iterator<std::vector<uint8_t>>(vector) {}
  virtual ~BitInserter() {
    ASSERT(num_saved_bits_ == 0);
  }

  void insert_bits(uint8_t byte, size_t num_bits) {
    size_t total_bits = num_bits + num_saved_bits_;
    uint16_t new_value = saved_bits_ | (static_cast<uint16_t>(byte) << num_saved_bits_);
    if (total_bits >= 8) {
      uint8_t new_byte = static_cast<uint8_t>(new_value);
      std::back_insert_iterator<std::vector<uint8_t>>::operator=(new_byte);
      total_bits -= 8;
      new_value = new_value >> 8;
    }
    num_saved_bits_ = total_bits;
    uint8_t mask = 0xff >> (8 - num_saved_bits_);
    saved_bits_ = static_cast<uint8_t>(new_value) & mask;
  }

  void insert_byte(uint8_t byte) {
    insert_bits(byte, 8);
  }

  bool IsByteAligned() {
    return num_saved_bits_ == 0;
  }

 private:
  size_t num_saved_bits_{0};
  uint8_t saved_bits_{0};
};

}  // namespace packet
}  // namespace bluetooth
