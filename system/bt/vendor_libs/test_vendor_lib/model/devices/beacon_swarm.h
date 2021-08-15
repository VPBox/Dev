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

#include "beacon.h"

namespace test_vendor_lib {

// Pretend to be a lot of beacons by changing the advertising address.
class BeaconSwarm : public Beacon {
 public:
  BeaconSwarm();
  virtual ~BeaconSwarm() = default;

  static std::shared_ptr<Device> Create() {
    return std::make_shared<BeaconSwarm>();
  }

  // Return a string representation of the type of device.
  virtual std::string GetTypeString() const override {
    return "beacon_swarm";
  }

  // Set the address and advertising interval from string args.
  virtual void Initialize(const std::vector<std::string>& args) override;

  virtual void TimerTick() override;

 private:
  static bool registered_;
};

}  // namespace test_vendor_lib
