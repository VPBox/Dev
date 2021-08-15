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

#define LOG_TAG "remote_loopback_device"

#include "remote_loopback_device.h"

#include "model/setup/device_boutique.h"

#include "osi/include/log.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"

using std::vector;

namespace test_vendor_lib {

using packets::LinkLayerPacketBuilder;
using packets::LinkLayerPacketView;
using packets::PageResponseBuilder;

bool RemoteLoopbackDevice::registered_ = DeviceBoutique::Register(LOG_TAG, &RemoteLoopbackDevice::Create);

RemoteLoopbackDevice::RemoteLoopbackDevice() {}

std::string RemoteLoopbackDevice::ToString() const {
  return GetTypeString() + " (no address)";
}

void RemoteLoopbackDevice::Initialize(const std::vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetAddress(addr);
}

void RemoteLoopbackDevice::IncomingPacket(LinkLayerPacketView packet) {
  // TODO: Check sender?
  // TODO: Handle other packet types
  Phy::Type phy_type = Phy::Type::BR_EDR;

  Link::PacketType type = packet.GetType();
  switch (type) {
    case Link::PacketType::PAGE:
      SendLinkLayerPacket(LinkLayerPacketBuilder::WrapPageResponse(
                              PageResponseBuilder::Create(true), packet.GetSourceAddress(), packet.GetSourceAddress()),
                          Phy::Type::BR_EDR);
      break;
    default: {
      ALOGW("Resend = %d", static_cast<int>(packet.size()));
      std::shared_ptr<std::vector<uint8_t>> extracted_packet = std::make_shared<std::vector<uint8_t>>();
      extracted_packet->reserve(packet.size());
      for (const auto byte : packet) {
        extracted_packet->push_back(byte);
      }

      SendLinkLayerPacket(LinkLayerPacketBuilder::ReWrap(extracted_packet), phy_type);
    }
  }
}

}  // namespace test_vendor_lib
