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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_TIME_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_TIME_PROVIDER_H_

#include <base/time/time.h>

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// Provider for time related information.
class TimeProvider : public Provider {
 public:
  ~TimeProvider() override {}

  // Returns the current date. The time of day component will be zero.
  virtual Variable<base::Time>* var_curr_date() = 0;

  // Returns the current hour (0 to 23) in local time. The type is int to keep
  // consistent with base::Time.
  virtual Variable<int>* var_curr_hour() = 0;

  // Returns the current minutes (0 to 60) in local time.
  virtual Variable<int>* var_curr_minute() = 0;

 protected:
  TimeProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TimeProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_TIME_PROVIDER_H_
