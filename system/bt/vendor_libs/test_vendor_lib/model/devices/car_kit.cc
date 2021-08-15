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

#define LOG_TAG "car_kit"

#include "car_kit.h"

#include "osi/include/log.h"

#include "model/setup/device_boutique.h"

using std::vector;

namespace test_vendor_lib {

bool CarKit::registered_ = DeviceBoutique::Register(LOG_TAG, &CarKit::Create);

CarKit::CarKit() : Device(kCarKitPropertiesFile) {
  advertising_interval_ms_ = std::chrono::milliseconds(0);

  page_scan_delay_ms_ = std::chrono::milliseconds(600);

  // Stub in packet handling for now
  link_layer_controller_.RegisterAclChannel([](std::shared_ptr<std::vector<uint8_t>>) {});
  link_layer_controller_.RegisterEventChannel([](std::shared_ptr<std::vector<uint8_t>>) {});
  link_layer_controller_.RegisterScoChannel([](std::shared_ptr<std::vector<uint8_t>>) {});
  link_layer_controller_.RegisterRemoteChannel(
      [this](std::shared_ptr<packets::LinkLayerPacketBuilder> packet, Phy::Type phy_type) {
        CarKit::SendLinkLayerPacket(packet, phy_type);
      });

  properties_.SetPageScanRepetitionMode(0);
  properties_.SetClassOfDevice(0x600420);
  properties_.SetSupportedFeatures(0x8779ff9bfe8defff);
  properties_.SetExtendedInquiryData({
      16,  // length
      9,   // Type: Device Name
      'g',  'D', 'e', 'v', 'i', 'c', 'e', '-', 'c', 'a', 'r', '_', 'k', 'i', 't',
      7,     // length
      3,     // Type: 16-bit UUIDs
      0x0e,  // AVRC
      0x11,
      0x0B,  // Audio Sink
      0x11,
      0x00,  // PnP Information
      0x12,
  });
  properties_.SetName({
      'g',
      'D',
      'e',
      'v',
      'i',
      'c',
      'e',
      '-',
      'C',
      'a',
      'r',
      '_',
      'K',
      'i',
      't',
  });
}

void CarKit::Initialize(const vector<std::string>& args) {
  if (args.size() < 2) return;

  Address addr;
  if (Address::FromString(args[1], addr)) properties_.SetAddress(addr);
  LOG_INFO(LOG_TAG, "%s SetAddress %s", ToString().c_str(), addr.ToString().c_str());

  if (args.size() < 3) return;

  properties_.SetClockOffset(std::stoi(args[2]));
}

void CarKit::TimerTick() {
  link_layer_controller_.TimerTick();
}

void CarKit::IncomingPacket(packets::LinkLayerPacketView packet) {
  LOG_WARN(LOG_TAG, "Incoming Packet");
  link_layer_controller_.IncomingPacket(packet);
}

}  // namespace test_vendor_lib
