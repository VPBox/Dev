//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_BOOT_CONTROL_H_
#define UPDATE_ENGINE_COMMON_BOOT_CONTROL_H_

#include <memory>

#include "update_engine/common/boot_control_interface.h"

namespace chromeos_update_engine {
namespace boot_control {

// The real BootControlInterface is platform-specific. This factory function
// creates a new BootControlInterface instance for the current platform. If
// this fails nullptr is returned.
std::unique_ptr<BootControlInterface> CreateBootControl();

}  // namespace boot_control
}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_BOOT_CONTROL_H_
