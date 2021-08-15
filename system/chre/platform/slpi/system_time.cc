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
#include "chre/platform/slpi/system_time.h"

#include "chre/platform/slpi/system_time_util.h"

extern "C" {

#include "uTimetick.h"

}  // extern "C"

namespace {

int64_t gEstimatedHostTimeOffset = 0;

} // anonymous namespace

namespace chre {

Nanoseconds SystemTime::getMonotonicTime() {
  return Nanoseconds(getNanosecondsFromQTimerTicks(uTimetick_Get()));
}

int64_t SystemTime::getEstimatedHostTimeOffset() {
  return gEstimatedHostTimeOffset;
}

void setEstimatedHostTimeOffset(int64_t offset) {
  gEstimatedHostTimeOffset = offset;
}

}  // namespace chre
