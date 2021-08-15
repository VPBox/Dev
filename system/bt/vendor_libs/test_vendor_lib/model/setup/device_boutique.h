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
#include <memory>
#include <string>
#include <vector>

#include "model/devices/device.h"

namespace test_vendor_lib {

// Create customized devices from a centralized shop.
class DeviceBoutique {
 public:
  DeviceBoutique();
  virtual ~DeviceBoutique() = default;

  // Register a constructor for a device type.
  static bool Register(const std::string& device_type, const std::function<std::shared_ptr<Device>()> method);

  // Call the constructor that matches arg[0], then call dev->Initialize(args).
  static std::shared_ptr<Device> Create(const std::vector<std::string>& args);

  template <typename D>
  struct Registrar {
    explicit Registrar(std::string const& name) {
      DeviceBoutique::Register(name, &D::Create);
    }
    static Registrar<D> registrar_;
  };

 private:
  static std::unordered_map<std::string, std::function<std::shared_ptr<Device>()>>& GetMap();
};

}  // namespace test_vendor_lib
