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
#include <memory>

#include "base/logging.h"

#include "packets/packet_builder.h"

namespace test_vendor_lib {
namespace packets {

class EncryptConnectionBuilder : public PacketBuilder<true> {
 public:
  virtual ~EncryptConnectionBuilder() = default;

  static std::unique_ptr<EncryptConnectionBuilder> Create(const std::vector<uint8_t>& key) {
    return std::unique_ptr<EncryptConnectionBuilder>(new EncryptConnectionBuilder(key));
  }

  virtual size_t size() const override {
    return key_.size();
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert_vector(key_, it);
  }

 private:
  explicit EncryptConnectionBuilder(const std::vector<uint8_t>& key) : key_(key.begin(), key.begin() + 16) {}
  std::vector<uint8_t> key_;
};

}  // namespace packets
}  // namespace test_vendor_lib
