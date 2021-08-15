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
#include <memory>
#include <vector>

#include "packets/packet_builder.h"

namespace test_vendor_lib {
namespace packets {

class ResponseBuilder : public PacketBuilder<true> {
 public:
  virtual ~ResponseBuilder() = default;

  static std::unique_ptr<ResponseBuilder> Create(uint16_t opcode, const std::vector<uint64_t>& data) {
    return std::unique_ptr<ResponseBuilder>(new ResponseBuilder(opcode, data));
  }

  virtual size_t size() const override {
    return sizeof(opcode_) + data_.size() * sizeof(uint64_t);
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(opcode_, it);
    insert_vector(data_, it);
  }

 private:
  explicit ResponseBuilder(uint16_t opcode, const std::vector<uint64_t> data) : opcode_(opcode), data_(data) {}
  uint16_t opcode_;
  std::vector<uint64_t> data_;
};

}  // namespace packets
}  // namespace test_vendor_lib
