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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_CONFIG_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_CONFIG_PROVIDER_H_

#include "update_engine/update_manager/config_provider.h"
#include "update_engine/update_manager/fake_variable.h"

namespace chromeos_update_manager {

// Fake implementation of the ConfigProvider base class.
class FakeConfigProvider : public ConfigProvider {
 public:
  FakeConfigProvider() {}

  FakeVariable<bool>* var_is_oobe_enabled() override {
    return &var_is_oobe_enabled_;
  }

 private:
  FakeVariable<bool> var_is_oobe_enabled_{"is_oobe_enabled",
                                          kVariableModeConst};

  DISALLOW_COPY_AND_ASSIGN(FakeConfigProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_CONFIG_PROVIDER_H_
