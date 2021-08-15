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
#include <vector>

#include "common/address.h"
#include "packet/bit_inserter.h"
#include "packet/packet_builder.h"

namespace bluetooth {
namespace packet {

class RawBuilder : public PacketBuilder<true> {
 public:
  RawBuilder() = default;
  RawBuilder(size_t max_bytes);
  virtual ~RawBuilder() = default;

  virtual size_t size() const override;

  virtual void Serialize(BitInserter& it) const;

  // Add |address| to the payload.  Return true if:
  // - the new size of the payload is still <= |max_bytes_|
  bool AddAddress(const common::Address& address);

  // Return true if |num_bytes| can be added to the payload.
  bool CanAddOctets(size_t num_bytes) const;

  // Add |octets| bytes to the payload.  Return true if:
  // - the size of |bytes| is equal to |octets| and
  // - the new size of the payload is still <= |max_bytes_|
  bool AddOctets(size_t octets, const std::vector<uint8_t>& bytes);

  bool AddOctets(const std::vector<uint8_t>& bytes);

  bool AddOctets1(uint8_t value);
  bool AddOctets2(uint16_t value);
  bool AddOctets3(uint32_t value);
  bool AddOctets4(uint32_t value);
  bool AddOctets6(uint64_t value);
  bool AddOctets8(uint64_t value);

 private:
  // Add |octets| bytes to the payload.  Return true if:
  // - the value of |value| fits in |octets| bytes and
  // - the new size of the payload is still <= |max_bytes_|
  bool AddOctets(size_t octets, uint64_t value);

  size_t max_bytes_{255};

  // Underlying containers for storing the actual packet
  std::vector<uint8_t> payload_;
};

}  // namespace packet
}  // namespace bluetooth
