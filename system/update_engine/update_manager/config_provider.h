//
// Copyright (C) 2014 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_CONFIG_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_CONFIG_PROVIDER_H_

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// Provider for const system configurations. This provider reads the
// configuration from a file on /etc.
class ConfigProvider : public Provider {
 public:
  // Returns a variable stating whether the out of the box experience (OOBE) is
  // enabled on this device. A value of false means that the device doesn't have
  // an OOBE workflow.
  virtual Variable<bool>* var_is_oobe_enabled() = 0;

 protected:
  ConfigProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_CONFIG_PROVIDER_H_
