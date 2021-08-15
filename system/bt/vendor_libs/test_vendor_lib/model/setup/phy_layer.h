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

#include "include/phy.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"

namespace test_vendor_lib {

class PhyLayer {
 public:
  PhyLayer(Phy::Type phy_type, uint32_t id, const std::function<void(packets::LinkLayerPacketView)>& device_receive)
      : phy_type_(phy_type), id_(id), transmit_to_device_(device_receive) {}

  virtual void Send(const std::shared_ptr<packets::LinkLayerPacketBuilder> packet) = 0;

  virtual void Receive(packets::LinkLayerPacketView packet) = 0;

  virtual void TimerTick() = 0;

  Phy::Type GetType() {
    return phy_type_;
  }

  uint32_t GetId() {
    return id_;
  }

  virtual ~PhyLayer() = default;

 private:
  Phy::Type phy_type_;
  uint32_t id_;

 protected:
  const std::function<void(packets::LinkLayerPacketView)> transmit_to_device_;
};

}  // namespace test_vendor_lib
