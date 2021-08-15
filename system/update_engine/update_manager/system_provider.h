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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_SYSTEM_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_SYSTEM_PROVIDER_H_

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// Provider for system information, mostly constant, such as the information
// reported by crossystem, the kernel boot command line and the partition table.
class SystemProvider : public Provider {
 public:
  ~SystemProvider() override {}

  // Returns true if the boot mode is normal or if it's unable to
  // determine the boot mode. Returns false if the boot mode is
  // developer.
  virtual Variable<bool>* var_is_normal_boot_mode() = 0;

  // Returns whether this is an official Chrome OS build.
  virtual Variable<bool>* var_is_official_build() = 0;

  // Returns a variable that tells whether OOBE was completed.
  virtual Variable<bool>* var_is_oobe_complete() = 0;

  // Returns a variable that tells the number of slots in the system.
  virtual Variable<unsigned int>* var_num_slots() = 0;

  // Returns the required platform version of the configured auto launch
  // with zero delay kiosk app if any.
  virtual Variable<std::string>* var_kiosk_required_platform_version() = 0;

 protected:
  SystemProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(SystemProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_SYSTEM_PROVIDER_H_
