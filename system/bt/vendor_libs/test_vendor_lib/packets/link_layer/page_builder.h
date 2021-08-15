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

#include <base/logging.h>

#include "packets/packet_builder.h"
#include "types/class_of_device.h"

namespace test_vendor_lib {
namespace packets {

class PageBuilder : public PacketBuilder<true> {
 public:
  virtual ~PageBuilder() = default;

  static std::unique_ptr<PageBuilder> Create(const ClassOfDevice& class_of_device, uint8_t allow_role_switch) {
    return std::unique_ptr<PageBuilder>(new PageBuilder(class_of_device, allow_role_switch));
  }

  virtual size_t size() const override {
    return sizeof(class_of_device_) + sizeof(allow_role_switch_);
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert_class_of_device(class_of_device_, it);
    insert(allow_role_switch_, it);
  }

 private:
  explicit PageBuilder(const ClassOfDevice& class_of_device, uint8_t allow_role_switch)
      : class_of_device_(class_of_device), allow_role_switch_(allow_role_switch) {}
  ClassOfDevice class_of_device_;
  uint8_t allow_role_switch_;
};

}  // namespace packets
}  // namespace test_vendor_lib
