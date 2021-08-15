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

#pragma once

#include <chrono>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "model/devices/device_properties.h"
#include "model/setup/phy_layer.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"
#include "types/address.h"

#include "stack/include/btm_ble_api.h"

namespace test_vendor_lib {

// Represent a Bluetooth Device
//  - Provide Get*() and Set*() functions for device attributes.
class Device {
 public:
  Device(const std::string properties_filename = "")
      : time_stamp_(std::chrono::steady_clock::now()), properties_(properties_filename) {}
  virtual ~Device() = default;

  // Initialize the device based on the values of |args|.
  virtual void Initialize(const std::vector<std::string>& args) = 0;

  // Return a string representation of the type of device.
  virtual std::string GetTypeString() const = 0;

  // Return the string representation of the device.
  virtual std::string ToString() const;

  // Decide whether to accept a connection request
  // May need to be extended to check peer address & type, and other
  // connection parameters.
  // Return true if the device accepts the connection request.
  virtual bool LeConnect() {
    return false;
  }

  // Set the advertisement interval in milliseconds.
  void SetAdvertisementInterval(std::chrono::milliseconds ms) {
    advertising_interval_ms_ = ms;
  }

  // Returns true if the host could see an advertisement in the next
  // |scan_time| milliseconds.
  virtual bool IsAdvertisementAvailable(std::chrono::milliseconds scan_time) const;

  // Let the device know that time has passed.
  virtual void TimerTick() {}

  void RegisterPhyLayer(std::shared_ptr<PhyLayer> phy);

  void UnregisterPhyLayer(std::shared_ptr<PhyLayer> phy);

  virtual void IncomingPacket(packets::LinkLayerPacketView){};

  virtual void SendLinkLayerPacket(std::shared_ptr<packets::LinkLayerPacketBuilder> packet, Phy::Type phy_type);

 protected:
  std::map<Phy::Type, std::vector<std::shared_ptr<PhyLayer>>> phy_layers_;

  std::chrono::steady_clock::time_point time_stamp_;

  // The time between page scans.
  std::chrono::milliseconds page_scan_delay_ms_;

  // The spec defines the advertising interval as a 16-bit value, but since it
  // is never sent in packets, we use std::chrono::milliseconds.
  std::chrono::milliseconds advertising_interval_ms_;

  DeviceProperties properties_;
};

}  // namespace test_vendor_lib
