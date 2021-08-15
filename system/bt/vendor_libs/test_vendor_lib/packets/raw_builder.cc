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

#include "raw_builder.h"

#include <base/logging.h>
#include <algorithm>

using std::vector;

namespace test_vendor_lib {
namespace packets {

RawBuilder::RawBuilder(size_t max_bytes) : max_bytes_(max_bytes) {}

bool RawBuilder::AddOctets(size_t octets, const vector<uint8_t>& bytes) {
  if (payload_.size() + octets > max_bytes_) return false;

  if (octets != bytes.size()) return false;

  payload_.insert(payload_.end(), bytes.begin(), bytes.end());

  return true;
}

bool RawBuilder::AddOctets(const vector<uint8_t>& bytes) {
  return AddOctets(bytes.size(), bytes);
}

bool RawBuilder::AddOctets(size_t octets, uint64_t value) {
  vector<uint8_t> val_vector;

  uint64_t v = value;

  if (octets > sizeof(uint64_t)) return false;

  for (size_t i = 0; i < octets; i++) {
    val_vector.push_back(v & 0xff);
    v = v >> 8;
  }

  if (v != 0) return false;

  return AddOctets(octets, val_vector);
}

bool RawBuilder::AddAddress(const Address& address) {
  if (payload_.size() + Address::kLength > max_bytes_) return false;

  for (size_t i = 0; i < Address::kLength; i++) {
    payload_.push_back(address.address[i]);
  }
  return true;
}

bool RawBuilder::AddOctets1(uint8_t value) {
  return AddOctets(1, value);
}

bool RawBuilder::AddOctets2(uint16_t value) {
  return AddOctets(2, value);
}

bool RawBuilder::AddOctets3(uint32_t value) {
  return AddOctets(3, value);
}

bool RawBuilder::AddOctets4(uint32_t value) {
  return AddOctets(4, value);
}

bool RawBuilder::AddOctets6(uint64_t value) {
  return AddOctets(6, value);
}

bool RawBuilder::AddOctets8(uint64_t value) {
  return AddOctets(8, value);
}

bool RawBuilder::CanAddOctets(size_t num_bytes) const {
  return payload_.size() + num_bytes <= max_bytes_;
}

void RawBuilder::Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const {
  for (const auto& val : payload_) {
    insert(val, it);
  }
}

size_t RawBuilder::size() const {
  return payload_.size();
}
}  // namespace packets
}  // namespace test_vendor_lib
