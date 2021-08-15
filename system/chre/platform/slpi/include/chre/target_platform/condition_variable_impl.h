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

#ifndef CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_IMPL_H_
#define CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_IMPL_H_

#include "chre/platform/condition_variable.h"

#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"

namespace chre {

inline ConditionVariable::ConditionVariable() {
  qurt_cond_init(&mConditionVariable);
}

inline ConditionVariable::~ConditionVariable() {
  qurt_cond_destroy(&mConditionVariable);
}

inline void ConditionVariable::notify_one() {
  qurt_cond_signal(&mConditionVariable);
}

inline void ConditionVariable::wait(Mutex& mutex) {
  qurt_cond_wait(&mConditionVariable, &mutex.mMutex);
}

// Note: The wait_for function is designed to work for a single thread waiting
// on the condition variable.
inline bool ConditionVariable::wait_for(Mutex& mutex, Nanoseconds timeout) {
  if (!mTimerInitialized) {
    if (!mTimeoutTimer.init()) {
      FATAL_ERROR("Failed to initialize condition variable timer");
    } else {
      mTimerInitialized = true;
    }
  }

  struct TimeoutCallbackData {
    ConditionVariable *cvPtr;
    bool timedOut;
  };
  auto callback = [](void *data) {
    auto cbData = static_cast<TimeoutCallbackData*>(data);
    cbData->timedOut = true;
    cbData->cvPtr->notify_one();
  };

  TimeoutCallbackData callbackData;
  callbackData.cvPtr = this;
  callbackData.timedOut = false;
  if (!mTimeoutTimer.set(callback, &callbackData, timeout)) {
    LOGE("Failed to set condition variable timer");
  }

  wait(mutex);
  if (mTimeoutTimer.isActive()) {
    if (!mTimeoutTimer.cancel()) {
      LOGD("Failed to cancel condition variable timer");
    }
  }
  return !callbackData.timedOut;
}

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_CONDITION_VARIABLE_IMPL_H_
