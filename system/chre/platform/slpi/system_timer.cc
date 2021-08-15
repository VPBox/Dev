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

#include "chre/platform/system_timer.h"

#include "chre/platform/log.h"

namespace chre {

SystemTimer::SystemTimer() {}

SystemTimer::~SystemTimer() {
  if (mInitialized) {
    slpiTimerUndef(&mTimerHandle);
  }
}

bool SystemTimer::init() {
  if (mInitialized) {
    LOGW("Tried re-initializing timer");
  } else {
#ifdef CHRE_SLPI_UIMG_ENABLED
    SlpiTimerErrorType status = utimer_def_osal(
        &mTimerHandle, UTIMER_FUNC1_CB_TYPE,
        reinterpret_cast<utimer_osal_notify_obj_ptr>(systemTimerNotifyCallback),
        reinterpret_cast<utimer_osal_notify_data>(this));
#else
    SlpiTimerErrorType status = timer_def_osal(
        &mTimerHandle, &timer_non_defer_group, TIMER_FUNC1_CB_TYPE,
        reinterpret_cast<time_osal_notify_obj_ptr>(systemTimerNotifyCallback),
        reinterpret_cast<time_osal_notify_data>(this));
#endif  // CHRE_SLPI_UIMG_ENABLED

    if (status != SLPI_TIMER_SUCCESS) {
      LOGE("Error initializing timer %d", status);
    } else {
      mInitialized = true;
    }
  }

  return mInitialized;
}

bool SystemTimer::set(SystemTimerCallback *callback, void *data,
    Nanoseconds delay) {
  bool wasSet = false;
  if (mInitialized) {
    mCallback = callback;
    mData = data;
    SlpiTimerErrorType status = slpiTimerSet64(&mTimerHandle,
        Microseconds(delay).getMicroseconds(), 0, SlpiTimerMicroUnit);
    if (status != SLPI_TIMER_SUCCESS) {
      LOGE("Error setting timer %d", status);
    } else {
      wasSet = true;
    }
  }

  return wasSet;
}

bool SystemTimer::cancel() {
  bool wasCancelled = false;
  if (mInitialized) {
    SlpiTimerTickType ticksRemaining = slpiTimerClr64(&mTimerHandle,
                                                      SlpiTimerTickUnit);
    wasCancelled = (ticksRemaining > 0);
  }

  return wasCancelled;
}

bool SystemTimer::isActive() {
  SlpiTimerTickType ticksRemaining = slpiTimerGet64(&mTimerHandle,
                                                    SlpiTimerTickUnit);
  return (mInitialized && ticksRemaining > 0);
}

void SystemTimerBase::systemTimerNotifyCallback(SlpiTimerCallbackDataType data) {
  SystemTimer *systemTimer = reinterpret_cast<SystemTimer *>(data);
  systemTimer->mCallback(systemTimer->mData);
}

}  // namespace chre
