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

namespace test_vendor_lib {
namespace packets {

class PageResponseBuilder : public PacketBuilder<true> {
 public:
  virtual ~PageResponseBuilder() = default;

  static std::unique_ptr<PageResponseBuilder> Create(uint8_t try_role_switch) {
    return std::unique_ptr<PageResponseBuilder>(new PageResponseBuilder(try_role_switch));
  }

  virtual size_t size() const override {
    return sizeof(try_role_switch_);
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(try_role_switch_, it);
  }

 private:
  explicit PageResponseBuilder(uint8_t try_role_switch) : try_role_switch_(try_role_switch) {}
  uint8_t try_role_switch_;
};

}  // namespace packets
}  // namespace test_vendor_lib
