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

#include "chre/platform/system_time.h"

#include <cerrno>
#include <cstring>
#include <ctime>

#include "chre/platform/assert.h"
#include "chre/platform/log.h"

namespace chre {

Nanoseconds SystemTime::getMonotonicTime() {
  struct timespec timeNow;
  if (clock_gettime(CLOCK_MONOTONIC, &timeNow)) {
    CHRE_ASSERT_LOG(false, "Failed to obtain time with error: %s",
                    strerror(errno));
    return Nanoseconds(UINT64_MAX);
  }

  // The C++11 spec guarantees that tv_sec and tv_nsec only have values >= 0 and
  // [0, 999999999]. It is safe to static cast these to their unsigned
  // counterpart.
  return Seconds(static_cast<uint64_t>(timeNow.tv_sec))
      + Nanoseconds(static_cast<uint64_t>(timeNow.tv_nsec));
}

int64_t SystemTime::getEstimatedHostTimeOffset() {
  return 0;
}

}  // namespace chre
