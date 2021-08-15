//
// Copyright (C) 2013 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_FAKE_CLOCK_H_
#define UPDATE_ENGINE_COMMON_FAKE_CLOCK_H_

#include "update_engine/common/clock_interface.h"

namespace chromeos_update_engine {

// Implements a clock that can be made to tell any time you want.
class FakeClock : public ClockInterface {
 public:
  FakeClock() {}

  base::Time GetWallclockTime() override { return wallclock_time_; }

  base::Time GetMonotonicTime() override { return monotonic_time_; }

  base::Time GetBootTime() override { return boot_time_; }

  void SetWallclockTime(const base::Time& time) { wallclock_time_ = time; }

  void SetMonotonicTime(const base::Time& time) { monotonic_time_ = time; }

  void SetBootTime(const base::Time& time) { boot_time_ = time; }

 private:
  base::Time wallclock_time_;
  base::Time monotonic_time_;
  base::Time boot_time_;

  DISALLOW_COPY_AND_ASSIGN(FakeClock);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_FAKE_CLOCK_H_
