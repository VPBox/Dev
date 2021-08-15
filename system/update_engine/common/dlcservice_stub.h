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

#ifndef UPDATE_ENGINE_COMMON_DLCSERVICE_STUB_H_
#define UPDATE_ENGINE_COMMON_DLCSERVICE_STUB_H_

#include <string>
#include <vector>

#include "update_engine/common/dlcservice_interface.h"

namespace chromeos_update_engine {

// An implementation of the DlcServiceInterface that does nothing.
class DlcServiceStub : public DlcServiceInterface {
 public:
  DlcServiceStub() = default;
  ~DlcServiceStub() = default;

  // BootControlInterface overrides.
  bool GetInstalled(std::vector<std::string>* dlc_module_ids) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(DlcServiceStub);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_DLCSERVICE_STUB_H_
