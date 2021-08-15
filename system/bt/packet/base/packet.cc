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

#include "packet.h"

#include <base/logging.h>
#include <algorithm>

#include "iterator.h"

namespace bluetooth {

size_t Packet::size() const { return packet_end_index_ - packet_start_index_; }

Iterator Packet::begin() const {
  return Iterator(shared_from_this(), packet_start_index_);
}

Iterator Packet::end() const {
  return Iterator(shared_from_this(), packet_end_index_);
}

// For the Array operator, treat index 0 as the relative start of the packet
uint8_t Packet::operator[](size_t i) {
  return get_at_index(i + packet_start_index_);
}

size_t Packet::get_length() const { return data_->size(); }

// Iterators use the absolute index to access data.
uint8_t Packet::get_at_index(size_t index) const {
  CHECK_GE(index, packet_start_index_);
  CHECK_LT(index, packet_end_index_);
  return data_->at(index);
}

}  // namespace bluetooth