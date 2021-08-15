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

#include "packets/link_layer/link_layer_packet_view.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

class DisconnectView : public PacketView<true> {
 public:
  DisconnectView(const DisconnectView&) = default;
  virtual ~DisconnectView() = default;

  static DisconnectView GetDisconnect(const LinkLayerPacketView& view) {
    CHECK(view.GetType() == Link::PacketType::DISCONNECT);
    return DisconnectView(view.GetPayload());
  }

  uint8_t GetReason() {
    return at(0);
  }

 private:
  DisconnectView() = delete;
  DisconnectView(const PacketView<true>& view) : PacketView(view) {}
};

}  // namespace packets
}  // namespace test_vendor_lib
