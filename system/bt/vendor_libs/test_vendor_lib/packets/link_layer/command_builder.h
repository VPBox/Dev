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

#include "base/logging.h"

#include "packets/packet_builder.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

class CommandBuilder : public PacketBuilder<true> {
 public:
  virtual ~CommandBuilder() = default;

  static std::unique_ptr<CommandBuilder> Create(uint16_t opcode, PacketView<true> args) {
    return std::unique_ptr<CommandBuilder>(new CommandBuilder(opcode, args));
  }

  virtual size_t size() const override {
    return sizeof(opcode_) + args_.size();
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(opcode_, it);
    for (const auto&& byte : args_) {
      insert(byte, it);
    }
  }

 private:
  explicit CommandBuilder(uint16_t opcode, PacketView<true> args) : opcode_(opcode), args_(args) {}
  uint16_t opcode_;
  PacketView<true> args_;
};

}  // namespace packets
}  // namespace test_vendor_lib
