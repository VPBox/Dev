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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_

#include <memory>

#include "update_engine/common/hardware_interface.h"
#include "update_engine/update_manager/config_provider.h"
#include "update_engine/update_manager/generic_variables.h"

namespace chromeos_update_manager {

// ConfigProvider concrete implementation.
class RealConfigProvider : public ConfigProvider {
 public:
  explicit RealConfigProvider(
      chromeos_update_engine::HardwareInterface* hardware)
      : hardware_(hardware) {}

  // Initializes the provider and returns whether it succeeded.
  bool Init();

  Variable<bool>* var_is_oobe_enabled() override {
    return var_is_oobe_enabled_.get();
  }

 private:
  std::unique_ptr<ConstCopyVariable<bool>> var_is_oobe_enabled_;

  chromeos_update_engine::HardwareInterface* hardware_;

  DISALLOW_COPY_AND_ASSIGN(RealConfigProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_
