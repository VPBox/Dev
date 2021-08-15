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

#ifndef UPDATE_ENGINE_POWER_MANAGER_CHROMEOS_H_
#define UPDATE_ENGINE_POWER_MANAGER_CHROMEOS_H_

#include <base/macros.h>
#include <power_manager/dbus-proxies.h>

#include "update_engine/power_manager_interface.h"

namespace chromeos_update_engine {

class PowerManagerChromeOS : public PowerManagerInterface {
 public:
  PowerManagerChromeOS();
  ~PowerManagerChromeOS() override = default;

  // PowerManagerInterface overrides.
  bool RequestReboot() override;

 private:
  // Real DBus proxy using the DBus connection.
  org::chromium::PowerManagerProxy power_manager_proxy_;

  DISALLOW_COPY_AND_ASSIGN(PowerManagerChromeOS);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_POWER_MANAGER_CHROMEOS_H_
