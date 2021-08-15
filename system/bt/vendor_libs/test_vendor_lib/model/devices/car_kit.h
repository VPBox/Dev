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
#include "model/controller/link_layer_controller.h"

namespace {
const std::string kCarKitPropertiesFile = "/etc/bluetooth/car_kit_controller_properties.json";
}  // namespace

namespace test_vendor_lib {

class CarKit : public Device {
 public:
  CarKit();
  ~CarKit() = default;

  static std::shared_ptr<CarKit> Create() {
    return std::make_shared<CarKit>();
  }

  // Initialize the device based on the values of |args|.
  virtual void Initialize(const std::vector<std::string>& args) override;

  // Return a string representation of the type of device.
  virtual std::string GetTypeString() const override {
    return "car_kit";
  }

  virtual void IncomingPacket(packets::LinkLayerPacketView packet) override;

  virtual void TimerTick() override;

 private:
  LinkLayerController link_layer_controller_{properties_};
  static bool registered_;
};

}  // namespace test_vendor_lib
