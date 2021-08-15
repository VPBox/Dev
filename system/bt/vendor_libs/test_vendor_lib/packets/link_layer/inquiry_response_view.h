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

#include "include/inquiry.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

class InquiryResponseView : public PacketView<true> {
 public:
  InquiryResponseView(const InquiryResponseView&) = default;
  virtual ~InquiryResponseView() = default;

  static InquiryResponseView GetInquiryResponse(const LinkLayerPacketView& view) {
    CHECK(view.GetType() == Link::PacketType::INQUIRY_RESPONSE);
    return InquiryResponseView(view.GetPayload());
  }

  Inquiry::InquiryType GetType() {
    return static_cast<Inquiry::InquiryType>(at(0));
  }

  uint8_t GetPageScanRepetitionMode() {
    return at(1);
  }

  ClassOfDevice GetClassOfDevice() {
    size_t offset = 2 * sizeof(uint8_t);
    return (begin() + offset).extract<ClassOfDevice>();
  }

  uint16_t GetClockOffset() {
    size_t offset = 2 * sizeof(uint8_t) + 3;
    return (begin() + offset).extract<uint16_t>();
  }

  uint8_t GetRssi() {
    size_t offset = 2 * sizeof(uint8_t) + 3 + sizeof(uint16_t);
    return at(offset);
  }

  Iterator<true> GetExtendedData() {
    size_t offset = 2 * sizeof(uint8_t) + 3 + sizeof(uint16_t) + sizeof(uint8_t);
    return begin() + offset;
  }

 private:
  InquiryResponseView() = delete;
  InquiryResponseView(const PacketView<true>& view) : PacketView(view) {}
};

}  // namespace packets
}  // namespace test_vendor_lib
