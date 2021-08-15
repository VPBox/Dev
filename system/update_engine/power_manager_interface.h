//
// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_POWER_MANAGER_INTERFACE_H_
#define UPDATE_ENGINE_POWER_MANAGER_INTERFACE_H_

#include <memory>

#include <base/macros.h>

namespace chromeos_update_engine {

class PowerManagerInterface {
 public:
  virtual ~PowerManagerInterface() = default;

  // Request the power manager to restart the device. Returns true on success.
  virtual bool RequestReboot() = 0;

 protected:
  PowerManagerInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(PowerManagerInterface);
};

namespace power_manager {
// Factory function which create a PowerManager.
std::unique_ptr<PowerManagerInterface> CreatePowerManager();
}  // namespace power_manager

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_POWER_MANAGER_INTERFACE_H_
