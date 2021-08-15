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

#include "event_payload_builder.h"

#include <base/logging.h>
#include <algorithm>

using std::vector;

namespace test_vendor_lib {
namespace packets {

EventPayloadBuilder::EventPayloadBuilder(size_t max_bytes) : max_bytes_(max_bytes) {}

bool EventPayloadBuilder::AddPayloadOctets(size_t octets, const vector<uint8_t>& bytes) {
  if (payload_.size() + octets > max_bytes_) return false;

  if (octets != bytes.size()) return false;

  payload_.insert(payload_.end(), bytes.begin(), bytes.end());

  return true;
}

bool EventPayloadBuilder::AddPayloadOctets(const vector<uint8_t>& bytes) {
  return AddPayloadOctets(bytes.size(), bytes);
}

bool EventPayloadBuilder::AddPayloadOctets(size_t octets, uint64_t value) {
  vector<uint8_t> val_vector;

  uint64_t v = value;

  if (octets > sizeof(uint64_t)) return false;

  for (size_t i = 0; i < octets; i++) {
    val_vector.push_back(v & 0xff);
    v = v >> 8;
  }

  if (v != 0) return false;

  return AddPayloadOctets(octets, val_vector);
}

bool EventPayloadBuilder::AddPayloadAddress(const Address& address) {
  if (payload_.size() + Address::kLength > max_bytes_) return false;

  for (size_t i = 0; i < Address::kLength; i++) {
    payload_.push_back(address.address[i]);
  }
  return true;
}

bool EventPayloadBuilder::AddPayloadOctets1(uint8_t value) {
  return AddPayloadOctets(1, value);
}

bool EventPayloadBuilder::AddPayloadOctets2(uint16_t value) {
  return AddPayloadOctets(2, value);
}

bool EventPayloadBuilder::AddPayloadOctets3(uint32_t value) {
  return AddPayloadOctets(3, value);
}

bool EventPayloadBuilder::AddPayloadOctets4(uint32_t value) {
  return AddPayloadOctets(4, value);
}

bool EventPayloadBuilder::AddPayloadOctets6(uint64_t value) {
  return AddPayloadOctets(6, value);
}

bool EventPayloadBuilder::AddPayloadOctets8(uint64_t value) {
  return AddPayloadOctets(8, value);
}

bool EventPayloadBuilder::CanAddPayloadOctets(size_t num_bytes) const {
  return payload_.size() + num_bytes <= max_bytes_;
}
}  // namespace packets
}  // namespace test_vendor_lib
