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

#define LOG_TAG "device_boutique"

#include "device_boutique.h"

#include "base/logging.h"
#include "osi/include/log.h"

using std::vector;

namespace test_vendor_lib {

std::unordered_map<std::string, std::function<std::shared_ptr<Device>()>>& DeviceBoutique::GetMap() {
  static std::unordered_map<std::string, std::function<std::shared_ptr<Device>()>> impl;
  return impl;
}

// Register a constructor for a device type.
bool DeviceBoutique::Register(const std::string& device_type,
                              const std::function<std::shared_ptr<Device>()> device_constructor) {
  LOG_INFO(LOG_TAG, "Registering %s", device_type.c_str());
  GetMap()[device_type] = device_constructor;
  return true;
}

std::shared_ptr<Device> DeviceBoutique::Create(const vector<std::string>& args) {
  CHECK(!args.empty());

  if (GetMap().find(args[0]) == GetMap().end()) {
    LOG_WARN(LOG_TAG, "No constructor registered for %s", args[0].c_str());
    return std::shared_ptr<Device>(nullptr);
  }

  std::shared_ptr<Device> new_device = GetMap()[args[0]]();
  if (new_device != nullptr) new_device->Initialize(args);

  return new_device;
}

}  // namespace test_vendor_lib
