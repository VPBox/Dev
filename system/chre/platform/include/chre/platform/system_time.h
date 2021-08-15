/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHRE_PLATFORM_TIME_H_
#define CHRE_PLATFORM_TIME_H_

#include <cstdint>

#include "chre/util/time.h"

namespace chre {

class SystemTime {
 public:
  /**
   * Initializes the system clock. This must be called as part of the
   * initialization of the runtime.
   */
  static void init();

  /**
   * Obtains a monotonic time reference relative to an arbitrary starting point.
   * There is no mandate that getMonotonicTime and getUptime be different.
   *
   * @return The time of the system.
   */
  static Nanoseconds getMonotonicTime();

  /**
   * Obtains the uptime of CHRE relative to CHRE initialization. There is no
   * mandate that getUptime and getMonotonicTime be different.
   *
   * @return The uptime of CHRE.
   */
  static Nanoseconds getUptime();

  /**
   * Returns the estimated offset between the host and CHRE time. The offset is
   * defined as the host time minus the CHRE time.
   *
   * @return The current estimated offset in nanoseconds.
   */
  static int64_t getEstimatedHostTimeOffset();
};

}  // namespace chre

#endif  // CHRE_PLATFORM_TIME_H_
