/*
 * Copyright 2016 The Android Open Source Project
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

#define LOG_TAG "loopback"

#include "loopback.h"

#include "le_advertisement.h"
#include "model/setup/device_boutique.h"
#include "osi/include/log.h"

using std::vector;

namespace test_vendor_lib {

bool Loopback::registered_ = DeviceBoutique::Register(LOG_TAG, &Loopback::Create);

Loopback::Loopback() {
  advertising_interval_ms_ = std::chrono::milliseconds(1280);
  properties_.SetLeAdvertisementType(BTM_BLE_NON_CONNECT_EVT);
  properties_.SetLeAdvertisement({0x11,  // Length
                                  BTM_BLE_AD_TYPE_NAME_CMPL,
                                  'g',
                                  'D',
                                  'e',
                                  'v',
                                  'i',
                                  'c',
                                  'e',
                                  '-',
                                  'l',
                                  'o',
                                  'o',
                                  'p',
                                  'b',
                                  'a',
                                  'c',
                                  'k',
                                  0x02,  // Length
                                  BTM_BLE_AD_TYPE_FLAG,
                                  BTM_BLE_BREDR_NOT_SPT | BTM_BLE_GEN_DISC_FLAG});

  properties_.SetLeScanResponse({0x05,  // Length
                                 BTM_BLE_AD_TYPE_NAME_SHORT, 'l', 'o', 'o', 'p'});
}

std::string Loopback::GetTypeString() const {
  return "loopback";
}

std::string Loopback::ToString() const {
  std::string dev = GetTypeString() + "@" + properties_.GetLeAddress().ToString();

  return dev;
}

void Loopback::Initialize(const vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetLeAddress(addr);

  if (args.size() < 3) return;

  SetAdvertisementInterval(std::chrono::milliseconds(std::stoi(args[2])));
}

void Loopback::TimerTick() {}

void Loopback::IncomingPacket(packets::LinkLayerPacketView packet) {
  LOG_INFO(LOG_TAG, "Got a packet of type %d", static_cast<int>(packet.GetType()));
  if (packet.GetDestinationAddress() == properties_.GetLeAddress() && packet.GetType() == Link::PacketType::LE_SCAN) {
    LOG_INFO(LOG_TAG, "Got a scan");
    std::unique_ptr<packets::LeAdvertisementBuilder> scan_response = packets::LeAdvertisementBuilder::Create(
        LeAdvertisement::AddressType::PUBLIC, LeAdvertisement::AdvertisementType::SCAN_RESPONSE,
        properties_.GetLeScanResponse());
    std::shared_ptr<packets::LinkLayerPacketBuilder> to_send = packets::LinkLayerPacketBuilder::WrapLeScanResponse(
        std::move(scan_response), properties_.GetLeAddress(), packet.GetSourceAddress());
    std::vector<std::shared_ptr<PhyLayer>> le_phys = phy_layers_[Phy::Type::LOW_ENERGY];
    for (auto phy : le_phys) {
      LOG_INFO(LOG_TAG, "Sending a Scan Response on a Phy");
      phy->Send(to_send);
    }
  }
}

}  // namespace test_vendor_lib
