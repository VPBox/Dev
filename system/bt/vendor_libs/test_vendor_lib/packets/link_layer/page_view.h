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

class PageView : public PacketView<true> {
 public:
  PageView(const PageView&) = default;
  virtual ~PageView() = default;

  static PageView GetPage(const LinkLayerPacketView& view) {
    CHECK(view.GetType() == Link::PacketType::PAGE);
    return PageView(view.GetPayload());
  }

  ClassOfDevice GetClassOfDevice() {
    return begin().extract<ClassOfDevice>();
  }

  uint8_t GetAllowRoleSwitch() {
    return at(3);
  }

 private:
  PageView() = delete;
  PageView(const PacketView<true>& view) : PacketView(view) {}
};

}  // namespace packets
}  // namespace test_vendor_lib
