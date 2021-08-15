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

#define LOG_TAG "phy_layer_factory"

#include "phy_layer_factory.h"

#include "base/logging.h"

#include "osi/include/log.h"

namespace test_vendor_lib {

PhyLayerFactory::PhyLayerFactory(Phy::Type phy_type) : phy_type_(phy_type) {}

Phy::Type PhyLayerFactory::GetType() {
  return phy_type_;
}

std::shared_ptr<PhyLayer> PhyLayerFactory::GetPhyLayer(
    const std::function<void(packets::LinkLayerPacketView)>& device_receive) {
  std::shared_ptr<PhyLayer> new_phy =
      std::make_shared<PhyLayerImpl>(phy_type_, next_id_++, device_receive, std::shared_ptr<PhyLayerFactory>(this));
  phy_layers_.push_back(new_phy);
  return new_phy;
}

void PhyLayerFactory::UnregisterPhyLayer(uint32_t id) {
  for (auto it = phy_layers_.begin(); it != phy_layers_.end();) {
    if ((*it)->GetId() == id) {
      it = phy_layers_.erase(it);
    } else {
      it++;
    }
  }
}

void PhyLayerFactory::Send(const std::shared_ptr<packets::LinkLayerPacketBuilder> packet, uint32_t id) {
  // Convert from a Builder to a View
  std::shared_ptr<std::vector<uint8_t>> serialized_packet =
      std::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>());
  std::back_insert_iterator<std::vector<uint8_t>> itr(*serialized_packet);
  serialized_packet->reserve(packet->size());
  packet->Serialize(itr);
  packets::LinkLayerPacketView packet_view = packets::LinkLayerPacketView::Create(serialized_packet);

  for (const auto phy : phy_layers_) {
    if (id != phy->GetId()) {
      phy->Receive(packet_view);
    }
  }
}

void PhyLayerFactory::TimerTick() {
  for (auto phy : phy_layers_) {
    phy->TimerTick();
  }
}

std::string PhyLayerFactory::ToString() const {
  switch (phy_type_) {
    case Phy::Type::LOW_ENERGY:
      return "LOW_ENERGY";
      break;
    case Phy::Type::BR_EDR:
      return "BR_EDR";
      break;
    default:
      return "Unknown";
  }
}

PhyLayerImpl::PhyLayerImpl(Phy::Type phy_type, uint32_t id,
                           const std::function<void(packets::LinkLayerPacketView)>& device_receive,
                           const std::shared_ptr<PhyLayerFactory>& factory)
    : PhyLayer(phy_type, id, device_receive), factory_(factory) {}

PhyLayerImpl::~PhyLayerImpl() {
  factory_->UnregisterPhyLayer(GetId());
  PhyLayer::~PhyLayer();
}

void PhyLayerImpl::Send(const std::shared_ptr<packets::LinkLayerPacketBuilder> packet) {
  factory_->Send(packet, GetId());
}

void PhyLayerImpl::Receive(packets::LinkLayerPacketView packet) {
  transmit_to_device_(packet);
}

void PhyLayerImpl::TimerTick() {}

}  // namespace test_vendor_lib
