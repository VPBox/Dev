//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_CPU_LIMITER_H_
#define UPDATE_ENGINE_COMMON_CPU_LIMITER_H_

#include <brillo/message_loops/message_loop.h>

namespace chromeos_update_engine {

// Cgroups cpu shares constants. 1024 is the default shares a standard process
// gets and 2 is the minimum value. We set High as a value that gives the
// update-engine 2x the cpu share of a standard process.
enum class CpuShares : int {
  kHigh = 2048,
  kNormal = 1024,
  kLow = 2,
};

// Sets the current process shares to |shares|. Returns true on
// success, false otherwise.
bool SetCpuShares(CpuShares shares);

class CPULimiter {
 public:
  CPULimiter() = default;
  ~CPULimiter();

  // Sets the cpu shares to low and sets up timeout events to stop the limiter.
  void StartLimiter();

  // Resets the cpu shares to normal and destroys any scheduled timeout sources.
  void StopLimiter();

  // Sets the cpu shares to |shares|. This method can be user at any time, but
  // if the limiter is not running, the shares won't be reset to normal.
  bool SetCpuShares(CpuShares shares);

 private:
  // The cpu shares timeout source callback sets the current cpu shares to
  // normal.
  void StopLimiterCallback();

  // Current cpu shares.
  CpuShares shares_ = CpuShares::kNormal;

  // The cpu shares management timeout task id.
  brillo::MessageLoop::TaskId manage_shares_id_{
      brillo::MessageLoop::kTaskIdNull};
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_CPU_LIMITER_H_
