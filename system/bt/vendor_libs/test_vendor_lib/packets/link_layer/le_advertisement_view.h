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

#include <log/log.h>

#include "include/link.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

class LeAdvertisementView : public PacketView<true>, public LeAdvertisement {
 public:
  LeAdvertisementView(const LeAdvertisementView&) = default;
  virtual ~LeAdvertisementView() = default;

  static LeAdvertisementView GetLeAdvertisementView(const LinkLayerPacketView& view) {
    CHECK(view.GetType() == Link::PacketType::LE_ADVERTISEMENT || view.GetType() == Link::PacketType::LE_SCAN_RESPONSE);
    return LeAdvertisementView(view.GetPayload());
  }

  AddressType GetAddressType() {
    return static_cast<AddressType>(at(0));
  }
  AdvertisementType GetAdvertisementType() {
    return static_cast<AdvertisementType>(at(1));
  }
  Iterator<true> GetData() {
    return begin() + 2 * sizeof(uint8_t);
  }

 private:
  LeAdvertisementView() = delete;
  LeAdvertisementView(const PacketView<true>& view) : PacketView(view) {}
};

}  // namespace packets
}  // namespace test_vendor_lib
