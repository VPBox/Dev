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

#include <memory>
#include <vector>

#include "include/phy.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "phy_layer.h"

namespace test_vendor_lib {

class PhyLayerFactory {
  friend class PhyLayerImpl;

 public:
  PhyLayerFactory(Phy::Type phy_type);

  virtual ~PhyLayerFactory() = default;

  Phy::Type GetType();

  std::shared_ptr<PhyLayer> GetPhyLayer(const std::function<void(packets::LinkLayerPacketView)>& device_receive);

  void UnregisterPhyLayer(uint32_t id);

  virtual void TimerTick();

  virtual std::string ToString() const;

 protected:
  virtual void Send(const std::shared_ptr<packets::LinkLayerPacketBuilder> packet, uint32_t id);

 private:
  Phy::Type phy_type_;
  std::vector<std::shared_ptr<PhyLayer>> phy_layers_;
  uint32_t next_id_{1};
};

class PhyLayerImpl : public PhyLayer {
 public:
  PhyLayerImpl(Phy::Type phy_type, uint32_t id, const std::function<void(packets::LinkLayerPacketView)>& device_receive,
               const std::shared_ptr<PhyLayerFactory>& factory);
  virtual ~PhyLayerImpl() override;

  virtual void Send(const std::shared_ptr<packets::LinkLayerPacketBuilder> packet) override;
  virtual void Receive(packets::LinkLayerPacketView packet) override;
  virtual void TimerTick() override;

 private:
  std::shared_ptr<PhyLayerFactory> factory_;
};
}  // namespace test_vendor_lib
