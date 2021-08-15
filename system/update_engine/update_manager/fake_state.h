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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_STATE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_STATE_H_

#include "update_engine/update_manager/fake_config_provider.h"
#include "update_engine/update_manager/fake_device_policy_provider.h"
#include "update_engine/update_manager/fake_random_provider.h"
#include "update_engine/update_manager/fake_shill_provider.h"
#include "update_engine/update_manager/fake_system_provider.h"
#include "update_engine/update_manager/fake_time_provider.h"
#include "update_engine/update_manager/fake_updater_provider.h"
#include "update_engine/update_manager/state.h"

namespace chromeos_update_manager {

// A fake State class that creates fake providers for all the providers.
// This fake can be used in unit testing of Policy subclasses. To fake out the
// value a variable is exposing, just call FakeVariable<T>::SetValue() on the
// variable you fake out. For example:
//
//   FakeState fake_state_;
//   fake_state_.random_provider_->var_seed()->SetValue(new uint64_t(12345));
//
// You can call SetValue more than once and the FakeVariable will take care of
// the memory, but only the last value will remain.
class FakeState : public State {
 public:
  // Creates and initializes the FakeState using fake providers.
  FakeState() {}

  ~FakeState() override {}

  // Downcasted getters to access the fake instances during testing.
  FakeConfigProvider* config_provider() override { return &config_provider_; }

  FakeDevicePolicyProvider* device_policy_provider() override {
    return &device_policy_provider_;
  }

  FakeRandomProvider* random_provider() override { return &random_provider_; }

  FakeShillProvider* shill_provider() override { return &shill_provider_; }

  FakeSystemProvider* system_provider() override { return &system_provider_; }

  FakeTimeProvider* time_provider() override { return &time_provider_; }

  FakeUpdaterProvider* updater_provider() override {
    return &updater_provider_;
  }

 private:
  FakeConfigProvider config_provider_;
  FakeDevicePolicyProvider device_policy_provider_;
  FakeRandomProvider random_provider_;
  FakeShillProvider shill_provider_;
  FakeSystemProvider system_provider_;
  FakeTimeProvider time_provider_;
  FakeUpdaterProvider updater_provider_;

  DISALLOW_COPY_AND_ASSIGN(FakeState);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_STATE_H_
