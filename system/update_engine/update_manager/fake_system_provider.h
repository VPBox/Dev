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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SYSTEM_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SYSTEM_PROVIDER_H_

#include "update_engine/update_manager/system_provider.h"

#include <string>

#include "update_engine/update_manager/fake_variable.h"

namespace chromeos_update_manager {

// Fake implementation of the SystemProvider base class.
class FakeSystemProvider : public SystemProvider {
 public:
  FakeSystemProvider() {}

  FakeVariable<bool>* var_is_normal_boot_mode() override {
    return &var_is_normal_boot_mode_;
  }

  FakeVariable<bool>* var_is_official_build() override {
    return &var_is_official_build_;
  }

  FakeVariable<bool>* var_is_oobe_complete() override {
    return &var_is_oobe_complete_;
  }

  FakeVariable<unsigned int>* var_num_slots() override {
    return &var_num_slots_;
  }

  FakeVariable<std::string>* var_kiosk_required_platform_version() override {
    return &var_kiosk_required_platform_version_;
  }

 private:
  FakeVariable<bool> var_is_normal_boot_mode_{"is_normal_boot_mode",
                                              kVariableModeConst};
  FakeVariable<bool> var_is_official_build_{"is_official_build",
                                            kVariableModeConst};
  FakeVariable<bool> var_is_oobe_complete_{"is_oobe_complete",
                                           kVariableModePoll};
  FakeVariable<unsigned int> var_num_slots_{"num_slots", kVariableModePoll};
  FakeVariable<std::string> var_kiosk_required_platform_version_{
      "kiosk_required_platform_version", kVariableModePoll};

  DISALLOW_COPY_AND_ASSIGN(FakeSystemProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SYSTEM_PROVIDER_H_
