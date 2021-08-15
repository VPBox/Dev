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
#include <vector>

#include "device.h"
#include "include/link.h"
#include "include/phy.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "polled_socket.h"

namespace test_vendor_lib {

class LinkLayerSocketDevice : public Device {
 public:
  LinkLayerSocketDevice(int socket_fd, Phy::Type phy_type);
  LinkLayerSocketDevice(LinkLayerSocketDevice&& s) = default;
  virtual ~LinkLayerSocketDevice() = default;

  static std::shared_ptr<Device> Create(int socket_fd, Phy::Type phy_type) {
    return std::make_shared<LinkLayerSocketDevice>(socket_fd, phy_type);
  }

  virtual std::string GetTypeString() const override {
    return "link_layer_socket_device";
  }

  virtual void Initialize(const std::vector<std::string>&) override {}

  virtual void IncomingPacket(packets::LinkLayerPacketView packet) override;

  virtual void TimerTick() override;

 private:
  net::PolledSocket socket_;
  Phy::Type phy_type_;
  size_t bytes_left_{0};
  size_t offset_;
  std::shared_ptr<std::vector<uint8_t>> received_;
  std::vector<packets::LinkLayerPacketView> packet_queue_;
};

}  // namespace test_vendor_lib
