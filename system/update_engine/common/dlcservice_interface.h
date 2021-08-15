//
// Copyright (C) 2018 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_DLCSERVICE_INTERFACE_H_
#define UPDATE_ENGINE_COMMON_DLCSERVICE_INTERFACE_H_

#include <string>
#include <vector>

#include <base/macros.h>

namespace chromeos_update_engine {

// The abstract dlcservice interface defines the interaction with the
// platform's dlcservice.
class DlcServiceInterface {
 public:
  virtual ~DlcServiceInterface() = default;

  // Returns true and a list of installed DLC module ids in |dlc_module_ids|.
  // On failure it returns false.
  virtual bool GetInstalled(std::vector<std::string>* dlc_module_ids) = 0;

 protected:
  DlcServiceInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(DlcServiceInterface);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_DLCSERVICE_INTERFACE_H_
