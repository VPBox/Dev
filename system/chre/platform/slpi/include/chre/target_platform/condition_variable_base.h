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

#ifndef CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_BASE_H_
#define CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_BASE_H_

extern "C" {

#include "qurt_cond.h"

}  // extern "C"

#include "chre/platform/system_timer.h"

namespace chre {

class ConditionVariableBase {
 protected:
  //! The underlying QURT condition variable.
  qurt_cond_t mConditionVariable;

  //! The timer used for timed condition variable wait.
  SystemTimer mTimeoutTimer;

  //! Set to true when the timeout timer is initialized.
  bool mTimerInitialized = false;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_BASE_H_
