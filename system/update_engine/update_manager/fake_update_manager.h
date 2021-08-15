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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_UPDATE_MANAGER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_UPDATE_MANAGER_H_

#include "update_engine/update_manager/update_manager.h"

#include "update_engine/update_manager/default_policy.h"
#include "update_engine/update_manager/fake_state.h"

namespace chromeos_update_manager {

class FakeUpdateManager : public UpdateManager {
 public:
  explicit FakeUpdateManager(chromeos_update_engine::ClockInterface* clock)
      : UpdateManager(clock,
                      base::TimeDelta::FromSeconds(5),
                      base::TimeDelta::FromHours(1),
                      new FakeState()) {
    // The FakeUpdateManager uses a DefaultPolicy.
    set_policy(new DefaultPolicy(clock));
  }

  // UpdateManager overrides.
  using UpdateManager::set_policy;

  FakeState* state() {
    return reinterpret_cast<FakeState*>(UpdateManager::state());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeUpdateManager);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_UPDATE_MANAGER_H_
