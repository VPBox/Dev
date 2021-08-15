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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_TIME_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_TIME_PROVIDER_H_

#include "update_engine/update_manager/fake_variable.h"
#include "update_engine/update_manager/time_provider.h"

namespace chromeos_update_manager {

// Fake implementation of the TimeProvider base class.
class FakeTimeProvider : public TimeProvider {
 public:
  FakeTimeProvider() {}

  FakeVariable<base::Time>* var_curr_date() override { return &var_curr_date_; }
  FakeVariable<int>* var_curr_hour() override { return &var_curr_hour_; }
  FakeVariable<int>* var_curr_minute() override { return &var_curr_minute_; }

 private:
  FakeVariable<base::Time> var_curr_date_{"curr_date", kVariableModePoll};
  FakeVariable<int> var_curr_hour_{"curr_hour", kVariableModePoll};
  FakeVariable<int> var_curr_minute_{"curr_minute", kVariableModePoll};

  DISALLOW_COPY_AND_ASSIGN(FakeTimeProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_TIME_PROVIDER_H_
