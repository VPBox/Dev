/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_UTIL_SCOPE_TIMER_IMPL_H_
#define CHRE_UTIL_SCOPE_TIMER_IMPL_H_

#include "chre/platform/system_time.h"

namespace chre {

ScopeTimer::ScopeTimer(const char *name)
    : mName(name), mStartTime(SystemTime::getMonotonicTime()) {}

ScopeTimer::~ScopeTimer() {
  Nanoseconds stopTime = SystemTime::getMonotonicTime();
  LOGD("ScopeTimer '%s' took %" PRIu64 "ns",
       mName, (stopTime - mStartTime).toRawNanoseconds());
}

}  // namespace chre

#endif  // CHRE_UTIL_SCOPE_TIMER_IMPL_H_
