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

#ifndef CHRE_UTIL_SCOPE_TIMER_H_
#define CHRE_UTIL_SCOPE_TIMER_H_

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * Measures the lifespan of this object and logs the result. This is useful for
 * benchmarking code manually and avoids creating time variables.
 */
class ScopeTimer : public NonCopyable {
 public:
  /**
   * Constructs a ScopeTimer with a name. The provided name would typically be a
   * string literal. The pointer must be valid for the lifespan of the
   * ScopeTimer object.
   */
  ScopeTimer(const char *name);

  /**
   * Destructs the ScopeTimer by logging the lifespan of this object along with
   * the name.
   */
  ~ScopeTimer();

 private:
  //! The name of this scope timer for logging use.
  const char *mName;

  //! The construction time of this scope timer.
  Nanoseconds mStartTime;
};

}  // namespace chre

#include "chre/util/scope_timer_impl.h"

#endif  // CHRE_UTIL_SCOPE_TIMER_H_
