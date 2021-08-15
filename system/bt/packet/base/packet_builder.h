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

#include <memory>

namespace bluetooth {

class Packet;

class PacketBuilder {
 public:
  virtual size_t size() const = 0;
  virtual bool Serialize(const std::shared_ptr<Packet>& pkt) = 0;

  virtual ~PacketBuilder() = default;

 protected:
  // Only Builders should be able to call these
  void ReserveSpace(const std::shared_ptr<Packet>& pkt, size_t size);
  bool AddPayloadOctets1(const std::shared_ptr<Packet>& pkt, uint8_t value) {
    return AddPayloadOctets(pkt, 1, value);
  }
  bool AddPayloadOctets2(const std::shared_ptr<Packet>& pkt, uint16_t value) {
    return AddPayloadOctets(pkt, 2, value);
  }
  bool AddPayloadOctets3(const std::shared_ptr<Packet>& pkt, uint32_t value) {
    return AddPayloadOctets(pkt, 3, value);
  }
  bool AddPayloadOctets4(const std::shared_ptr<Packet>& pkt, uint32_t value) {
    return AddPayloadOctets(pkt, 4, value);
  }
  bool AddPayloadOctets6(const std::shared_ptr<Packet>& pkt, uint64_t value) {
    return AddPayloadOctets(pkt, 6, value);
  }
  bool AddPayloadOctets8(const std::shared_ptr<Packet>& pkt, uint64_t value) {
    return AddPayloadOctets(pkt, 8, value);
  }

 private:
  // Add |octets| bytes to the payload.  Return true if:
  // - the value of |value| fits in |octets| bytes and
  // - the new size of the payload is still < |kMaxPayloadOctets|
  bool AddPayloadOctets(const std::shared_ptr<Packet>& pkt, size_t octets,
                        uint64_t value);
};

}  // namespace bluetooth