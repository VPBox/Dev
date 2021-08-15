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
#include <vector>

#include "packets/packet_builder.h"
#include "types/address.h"

namespace test_vendor_lib {
namespace packets {

class EventPayloadBuilder : public PacketBuilder<true> {
 public:
  EventPayloadBuilder() = default;
  EventPayloadBuilder(size_t max_bytes);
  virtual ~EventPayloadBuilder() = default;

  virtual size_t size() const override;

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const;

  // Add |octets| bytes to the payload.  Return true if:
  // - the size of |bytes| is equal to |octets| and
  // - the new size of the payload is still < |max_bytes_|
  bool AddPayloadOctets(size_t octets, const std::vector<uint8_t>& bytes);

  bool AddPayloadOctets(const std::vector<uint8_t>& bytes);

  bool AddPayloadOctets1(uint8_t value);
  bool AddPayloadOctets2(uint16_t value);
  bool AddPayloadOctets3(uint32_t value);
  bool AddPayloadOctets4(uint32_t value);
  bool AddPayloadOctets6(uint64_t value);
  bool AddPayloadOctets8(uint64_t value);

 private:
  // Add |octets| bytes to the payload.  Return true if:
  // - the value of |value| fits in |octets| bytes and
  // - the new size of the payload is still < |max_bytes_|
  bool AddPayloadOctets(size_t octets, uint64_t value);

  // Add |address| to the payload.  Return true if:
  // - the new size of the payload is still < |max_bytes_|
  bool AddPayloadAddress(const Address& address);

  // Return true if |num_bytes| can be added to the payload.
  bool CanAddPayloadOctets(size_t num_bytes) const;

  size_t max_bytes_{255};

  // Underlying containers for storing the actual packet
  std::vector<uint8_t> payload_;
};

}  // namespace packets
}  // namespace test_vendor_lib
