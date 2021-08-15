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

#include "update_engine/common/clock.h"

#include <time.h>

namespace chromeos_update_engine {

base::Time Clock::GetWallclockTime() {
  return base::Time::Now();
}

base::Time Clock::GetMonotonicTime() {
  struct timespec now_ts;
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &now_ts) != 0) {
    // Avoid logging this as an error as call-sites may call this very
    // often and we don't want to fill up the disk. Note that this
    // only fails if running on ancient kernels (CLOCK_MONOTONIC_RAW
    // was added in Linux 2.6.28) so it never fails on a ChromeOS
    // device.
    return base::Time();
  }
  struct timeval now_tv;
  now_tv.tv_sec = now_ts.tv_sec;
  now_tv.tv_usec = now_ts.tv_nsec / base::Time::kNanosecondsPerMicrosecond;
  return base::Time::FromTimeVal(now_tv);
}

base::Time Clock::GetBootTime() {
  struct timespec now_ts;
  if (clock_gettime(CLOCK_BOOTTIME, &now_ts) != 0) {
    // Avoid logging this as an error as call-sites may call this very
    // often and we don't want to fill up the disk. Note that this
    // only fails if running on ancient kernels (CLOCK_BOOTTIME was
    // added in Linux 2.6.39) so it never fails on a ChromeOS device.
    return base::Time();
  }
  struct timeval now_tv;
  now_tv.tv_sec = now_ts.tv_sec;
  now_tv.tv_usec = now_ts.tv_nsec / base::Time::kNanosecondsPerMicrosecond;
  return base::Time::FromTimeVal(now_tv);
}

}  // namespace chromeos_update_engine
