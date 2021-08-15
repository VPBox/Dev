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

namespace test_vendor_lib {

class BrokenAdv : public Device {
 public:
  BrokenAdv();
  ~BrokenAdv() = default;

  static std::shared_ptr<Device> Create() {
    return std::make_shared<BrokenAdv>();
  }

  // Initialize the device based on the values of |args|.
  virtual void Initialize(const std::vector<std::string>& args) override;

  // Return a string representation of the type of device.
  virtual std::string GetTypeString() const override {
    return "broken_adv";
  }

  // Return the string representation of the device.
  virtual std::string ToString() const override;

  // Use the timer tick to update advertisements.
  void TimerTick() override;

  // Change which advertisements are broken and the address of the device.
  void UpdateAdvertisement();

  // Change which data is broken and the address of the device.
  void UpdatePageScan();

 private:
  std::vector<uint8_t> constant_adv_data_;
  std::vector<uint8_t> constant_scan_data_;
  static bool registered_;
};

}  // namespace test_vendor_lib
