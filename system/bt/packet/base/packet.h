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
#include <type_traits>
#include <utility>
#include <vector>

namespace bluetooth {

// Abstract base class that is subclassed to provide type-specifc accessors on
// data. Manages said data's memory and guarantees the data's persistence. Once
// created the underlying data is immutable.
class Packet : public std::enable_shared_from_this<Packet> {
  friend class Iterator;
  friend class PacketBuilder;

 public:
  virtual ~Packet() = default;

 protected:
  Packet()
      : packet_start_index_(0),
        packet_end_index_(0),
        data_(std::make_shared<std::vector<uint8_t>>(0)){};
  Packet(std::shared_ptr<const Packet> pkt, size_t start, size_t end)
      : packet_start_index_(start), packet_end_index_(end), data_(pkt->data_){};
  Packet(std::shared_ptr<const Packet> pkt) : data_(pkt->data_) {
    auto indices = pkt->GetPayloadIndecies();
    packet_start_index_ = indices.first;
    packet_end_index_ = indices.second;
  };

 public:
  size_t size() const;
  class Iterator begin() const;
  class Iterator end() const;

  uint8_t operator[](size_t i);

  // Check to see if the packet is structured correctly and have the correct
  // lengths. Data access on an invalid packet may cause a crash.
  virtual bool IsValid() const = 0;

  // Debug string representation of the packet
  virtual std::string ToString() const = 0;

  // Convert a packet horizontally in a layer, you may only specialize
  // into a more specific type and doing otherwise will cause a compiler error
  //
  // Example:
  // std::shared_ptr<AvrcpPacket> base;
  // std::shared_ptr<AvrcpVendorPacket> p =
  //    Packet::Specialize<AvrcpVendorPacket>(base);
  template <class T, class U>
  static std::shared_ptr<T> Specialize(const std::shared_ptr<U>& pkt) {
    static_assert(std::is_convertible<U*, Packet*>::value,
                  "Unable to specialize a non-packet object.");
    static_assert(std::is_convertible<T*, Packet*>::value,
                  "Unable to specialize to something that isn't a packet");
    static_assert(std::is_convertible<T*, U*>::value,
                  "Can not convert between the two packet types.");
    return std::shared_ptr<T>(
        new T(pkt, pkt->packet_start_index_, pkt->packet_end_index_));
  };

 protected:
  // Packet should be immutable other than when building
  size_t packet_start_index_;
  size_t packet_end_index_;
  std::shared_ptr<std::vector<uint8_t>> data_;

 private:
  // Only Available to the iterators
  virtual size_t get_length() const;
  virtual uint8_t get_at_index(size_t index) const;

  // Returns the begining and end indicies of the payload of the packet.
  // Used when constructing a packet from another packet when moving
  // between layers.
  virtual std::pair<size_t, size_t> GetPayloadIndecies() const = 0;
};

}  // namespace bluetooth