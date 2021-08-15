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

class IoCapabilityBuilder : public PacketBuilder<true> {
 public:
  virtual ~IoCapabilityBuilder() = default;

  static std::unique_ptr<IoCapabilityBuilder> Create(uint8_t io_capability, uint8_t oob_data_present,
                                                     uint8_t authentication_requirements) {
    return std::unique_ptr<IoCapabilityBuilder>(
        new IoCapabilityBuilder(io_capability, oob_data_present, authentication_requirements));
  }

  virtual size_t size() const override {
    return sizeof(io_capability_) + sizeof(oob_data_present_) + sizeof(authentication_requirements_);
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(io_capability_, it);
    insert(oob_data_present_, it);
    insert(authentication_requirements_, it);
  }

 private:
  explicit IoCapabilityBuilder(uint8_t io_capability, uint8_t oob_data_present, uint8_t authentication_requirements)
      : io_capability_(io_capability), oob_data_present_(oob_data_present),
        authentication_requirements_(authentication_requirements) {}
  uint8_t io_capability_;
  uint8_t oob_data_present_;
  uint8_t authentication_requirements_;
};

}  // namespace packets
}  // namespace test_vendor_lib
