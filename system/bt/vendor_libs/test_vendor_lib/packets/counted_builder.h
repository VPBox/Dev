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
#include <memory>
#include <vector>

#include "packets/base_packet_builder.h"
#include "packets/packet_builder.h"
#include "packets/raw_builder.h"

namespace test_vendor_lib {
namespace packets {

class CountedBuilder : public RawBuilder {
 public:
  CountedBuilder() = default;
  virtual ~CountedBuilder() = default;

  virtual size_t size() const override;

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override;

  void Add(std::unique_ptr<BasePacketBuilder> builder);

 private:
  std::vector<std::unique_ptr<BasePacketBuilder>> sub_builders_;
};

}  // namespace packets
}  // namespace test_vendor_lib
