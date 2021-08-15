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

#include "link_layer_packet_view.h"
#include "base/logging.h"

namespace test_vendor_lib {
constexpr size_t Link::kSizeBytes;
constexpr size_t Link::kTypeBytes;

namespace packets {
LinkLayerPacketView::LinkLayerPacketView(std::shared_ptr<std::vector<uint8_t>> raw) : PacketView<true>(raw) {}

LinkLayerPacketView LinkLayerPacketView::Create(std::shared_ptr<std::vector<uint8_t>> raw) {
  CHECK(raw->size() >= Link::kSizeBytes + Link::kTypeBytes + 2 * Address::kLength);
  return LinkLayerPacketView(raw);
}

Link::PacketType LinkLayerPacketView::GetType() const {
  return static_cast<Link::PacketType>(at(Link::kSizeBytes));
}

Address LinkLayerPacketView::GetSourceAddress() const {
  size_t offset = Link::kSizeBytes + Link::kTypeBytes;
  return (begin() + offset).extract<Address>();
}

Address LinkLayerPacketView::GetDestinationAddress() const {
  size_t offset = Link::kSizeBytes + Link::kTypeBytes + Address::kLength;
  return (begin() + offset).extract<Address>();
}

PacketView<true> LinkLayerPacketView::GetPayload() const {
  return SubViewLittleEndian(Link::kSizeBytes + Link::kTypeBytes + 2 * Address::kLength, size());
}

}  // namespace packets
}  // namespace test_vendor_lib
