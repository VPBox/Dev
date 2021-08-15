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

#define LOG_TAG "link_layer_socket_device"

#include "link_layer_socket_device.h"

#include <unistd.h>

#include "osi/include/log.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "packets/packet_view.h"
#include "packets/view.h"

using std::vector;

namespace test_vendor_lib {

LinkLayerSocketDevice::LinkLayerSocketDevice(int socket_fd, Phy::Type phy_type)
    : socket_(socket_fd), phy_type_(phy_type) {}

void LinkLayerSocketDevice::TimerTick() {
  if (bytes_left_ == 0) {
    received_ = std::make_shared<std::vector<uint8_t>>(Link::kSizeBytes);
    size_t bytes_received = socket_.TryReceive(Link::kSizeBytes, received_->data());
    if (bytes_received == 0) {
      return;
    }
    CHECK(bytes_received == Link::kSizeBytes) << "bytes_received == " << bytes_received;
    packets::PacketView<true> size({packets::View(received_, 0, Link::kSizeBytes)});
    bytes_left_ = size.begin().extract<uint32_t>();
    received_->resize(Link::kSizeBytes + bytes_left_);
    offset_ = Link::kSizeBytes;
  }
  size_t bytes_received = socket_.TryReceive(bytes_left_, received_->data() + offset_);
  if (bytes_received == 0) {
    return;
  }
  bytes_left_ -= bytes_received;
  offset_ += bytes_received;
  if (bytes_left_ == 0) {
    SendLinkLayerPacket(packets::LinkLayerPacketBuilder::ReWrap(received_), phy_type_);
    offset_ = 0;
    received_.reset();
  }
}

void LinkLayerSocketDevice::IncomingPacket(packets::LinkLayerPacketView packet) {
  socket_.TrySend(packet);
}

}  // namespace test_vendor_lib
