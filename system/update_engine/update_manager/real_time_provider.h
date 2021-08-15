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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_REAL_TIME_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_REAL_TIME_PROVIDER_H_

#include <memory>

#include <base/time/time.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/update_manager/time_provider.h"

namespace chromeos_update_manager {

// TimeProvider concrete implementation.
class RealTimeProvider : public TimeProvider {
 public:
  explicit RealTimeProvider(chromeos_update_engine::ClockInterface* clock)
      : clock_(clock) {}

  // Initializes the provider and returns whether it succeeded.
  bool Init();

  Variable<base::Time>* var_curr_date() override {
    return var_curr_date_.get();
  }

  Variable<int>* var_curr_hour() override { return var_curr_hour_.get(); }

  Variable<int>* var_curr_minute() override { return var_curr_minute_.get(); }

 private:
  // A clock abstraction (fakeable).
  chromeos_update_engine::ClockInterface* const clock_;

  std::unique_ptr<Variable<base::Time>> var_curr_date_;
  std::unique_ptr<Variable<int>> var_curr_hour_;
  std::unique_ptr<Variable<int>> var_curr_minute_;

  DISALLOW_COPY_AND_ASSIGN(RealTimeProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_REAL_TIME_PROVIDER_H_
