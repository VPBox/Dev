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
#include <string>
#include <vector>

#include "include/link.h"
#include "packets/packet_view.h"
#include "types/address.h"

namespace test_vendor_lib {
namespace packets {

// Link-layer packets are an abstraction of LMP PDUs.
class LinkLayerPacketView : public PacketView<true> {
 public:
  LinkLayerPacketView(const LinkLayerPacketView&) = default;
  virtual ~LinkLayerPacketView() = default;

  static LinkLayerPacketView Create(std::shared_ptr<std::vector<uint8_t>> raw);

  Link::PacketType GetType() const;
  Address GetSourceAddress() const;
  Address GetDestinationAddress() const;
  PacketView<true> GetPayload() const;

 private:
  LinkLayerPacketView() = delete;
  LinkLayerPacketView(std::shared_ptr<std::vector<uint8_t>> raw);
};

}  // namespace packets
}  // namespace test_vendor_lib
