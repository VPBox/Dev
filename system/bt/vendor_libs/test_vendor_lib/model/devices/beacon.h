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

#include <cstdint>
#include <vector>

#include "device.h"
#include "stack/include/btm_ble_api.h"

namespace test_vendor_lib {

// A simple device that advertises periodically and is not connectable.
class Beacon : public Device {
 public:
  Beacon();
  virtual ~Beacon() = default;

  static std::shared_ptr<Device> Create() {
    return std::make_shared<Beacon>();
  }

  // Return a string representation of the type of device.
  virtual std::string GetTypeString() const override;

  // Return a string representation of the device.
  virtual std::string ToString() const override;

  // Set the address and advertising interval from string args.
  virtual void Initialize(const std::vector<std::string>& args) override;

  virtual void IncomingPacket(packets::LinkLayerPacketView packet) override;

  virtual void TimerTick() override;

 private:
  static bool registered_;
};

}  // namespace test_vendor_lib
