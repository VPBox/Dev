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

#include "include/le_advertisement.h"
#include "packets/packet_builder.h"

namespace test_vendor_lib {
namespace packets {

class LeAdvertisementBuilder : public PacketBuilder<true>, public LeAdvertisement {
 public:
  virtual ~LeAdvertisementBuilder() = default;

  static std::unique_ptr<LeAdvertisementBuilder> Create(AddressType address_type, AdvertisementType advertisement_type,
                                                        const std::vector<uint8_t>& advertisement) {
    return std::unique_ptr<LeAdvertisementBuilder>(
        new LeAdvertisementBuilder(address_type, advertisement_type, advertisement));
  }

  virtual size_t size() const override {
    return sizeof(address_type_) + sizeof(advertisement_type_) + advertisement_.size();
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(static_cast<uint8_t>(address_type_), it);
    insert(static_cast<uint8_t>(advertisement_type_), it);
    insert_vector(advertisement_, it);
  }

 private:
  LeAdvertisementBuilder() = delete;
  explicit LeAdvertisementBuilder(AddressType address_type, AdvertisementType advertisement_type,
                                  const std::vector<uint8_t>& advertisement)
      : address_type_(address_type), advertisement_type_(advertisement_type), advertisement_(advertisement) {}
  AddressType address_type_;
  AdvertisementType advertisement_type_;
  std::vector<uint8_t> advertisement_;
};

}  // namespace packets
}  // namespace test_vendor_lib
