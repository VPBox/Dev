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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_STATE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_STATE_H_

#include "update_engine/update_manager/config_provider.h"
#include "update_engine/update_manager/device_policy_provider.h"
#include "update_engine/update_manager/random_provider.h"
#include "update_engine/update_manager/shill_provider.h"
#include "update_engine/update_manager/system_provider.h"
#include "update_engine/update_manager/time_provider.h"
#include "update_engine/update_manager/updater_provider.h"

namespace chromeos_update_manager {

// The State class is an interface to the ensemble of providers. This class
// gives visibility of the state providers to policy implementations.
class State {
 public:
  virtual ~State() {}

  // These methods return the given provider.
  virtual ConfigProvider* config_provider() = 0;
  virtual DevicePolicyProvider* device_policy_provider() = 0;
  virtual RandomProvider* random_provider() = 0;
  virtual ShillProvider* shill_provider() = 0;
  virtual SystemProvider* system_provider() = 0;
  virtual TimeProvider* time_provider() = 0;
  virtual UpdaterProvider* updater_provider() = 0;

 protected:
  State() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(State);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_STATE_H_
