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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SHILL_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SHILL_PROVIDER_H_

#include "update_engine/update_manager/fake_variable.h"
#include "update_engine/update_manager/shill_provider.h"

namespace chromeos_update_manager {

// Fake implementation of the ShillProvider base class.
class FakeShillProvider : public ShillProvider {
 public:
  FakeShillProvider() {}

  FakeVariable<bool>* var_is_connected() override { return &var_is_connected_; }

  FakeVariable<chromeos_update_engine::ConnectionType>* var_conn_type()
      override {
    return &var_conn_type_;
  }

  FakeVariable<chromeos_update_engine::ConnectionTethering>*
  var_conn_tethering() override {
    return &var_conn_tethering_;
  }

  FakeVariable<base::Time>* var_conn_last_changed() override {
    return &var_conn_last_changed_;
  }

 private:
  FakeVariable<bool> var_is_connected_{"is_connected", kVariableModePoll};
  FakeVariable<chromeos_update_engine::ConnectionType> var_conn_type_{
      "conn_type", kVariableModePoll};
  FakeVariable<chromeos_update_engine::ConnectionTethering> var_conn_tethering_{
      "conn_tethering", kVariableModePoll};
  FakeVariable<base::Time> var_conn_last_changed_{"conn_last_changed",
                                                  kVariableModePoll};

  DISALLOW_COPY_AND_ASSIGN(FakeShillProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_SHILL_PROVIDER_H_
