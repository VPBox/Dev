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

#include "counted_builder.h"

using std::vector;

namespace test_vendor_lib {
namespace packets {

size_t CountedBuilder::size() const {
  size_t payload_size = sizeof(uint8_t);
  for (size_t i = 0; i < sub_builders_.size(); i++) {
    payload_size += sub_builders_[i]->size();
  }
  return payload_size;
}

void CountedBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  insert(static_cast<uint8_t>(sub_builders_.size()), it);
  for (size_t i = 0; i < sub_builders_.size(); i++) {
    sub_builders_[i]->Serialize(it);
  }
}

void CountedBuilder::Add(std::unique_ptr<BasePacketBuilder> builder) {
  sub_builders_.push_back(std::move(builder));
}

}  // namespace packets
}  // namespace test_vendor_lib
