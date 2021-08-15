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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_STATE_FACTORY_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_STATE_FACTORY_H_

#include "update_engine/system_state.h"
#include "update_engine/update_manager/state.h"

namespace org {
namespace chromium {
class KioskAppServiceInterfaceProxyInterface;
}  // namespace chromium
}  // namespace org

namespace chromeos_update_manager {

// Creates and initializes a new UpdateManager State instance containing real
// providers instantiated using the passed interfaces. The State doesn't take
// ownership of the passed interfaces, which need to remain available during the
// life of this instance.  Returns null if one of the underlying providers fails
// to initialize.
State* DefaultStateFactory(
    policy::PolicyProvider* policy_provider,
    org::chromium::KioskAppServiceInterfaceProxyInterface* kiosk_app_proxy,
    chromeos_update_engine::SystemState* system_state);

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_STATE_FACTORY_H_
