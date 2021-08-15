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

#include "update_engine/update_manager/real_config_provider.h"

#include "update_engine/update_manager/generic_variables.h"

namespace chromeos_update_manager {

bool RealConfigProvider::Init() {
  var_is_oobe_enabled_.reset(new ConstCopyVariable<bool>(
      "is_oobe_enabled", hardware_->IsOOBEEnabled()));

  return true;
}

}  // namespace chromeos_update_manager
