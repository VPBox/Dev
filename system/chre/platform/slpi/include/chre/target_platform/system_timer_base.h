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

#ifndef CHRE_PLATFORM_SLPI_SYSTEM_TIMER_BASE_H_
#define CHRE_PLATFORM_SLPI_SYSTEM_TIMER_BASE_H_

extern "C" {

#ifdef CHRE_SLPI_UIMG_ENABLED
#include "utimer.h"

typedef utimer_type SlpiTimerHandle;
typedef utimer_cb_data_type SlpiTimerCallbackDataType;
typedef utimer_timetick_type SlpiTimerTickType;
typedef utimer_error_type SlpiTimerErrorType;

#define SlpiTimerTickUnit UT_TICK
#define SlpiTimerMicroUnit UT_USEC
#define SLPI_TIMER_SUCCESS UTE_SUCCESS
#define slpiTimerClr64 utimer_clr_64
#define slpiTimerGet64 utimer_get_64
#define slpiTimerSet64 utimer_set_64
#define slpiTimerUndef utimer_undef
#else
#include "timer.h"

typedef timer_type SlpiTimerHandle;
typedef timer_cb_data_type SlpiTimerCallbackDataType;
typedef time_timetick_type SlpiTimerTickType;
typedef timer_error_type SlpiTimerErrorType;

#define SlpiTimerTickUnit T_TICK
#define SlpiTimerMicroUnit T_USEC
#define SLPI_TIMER_SUCCESS TE_SUCCESS
#define slpiTimerClr64 timer_clr_64
#define slpiTimerGet64 timer_get_64
#define slpiTimerSet64 timer_set_64
#define slpiTimerUndef timer_undef
#endif  // CHRE_SLPI_UIMG_ENABLED

}  // extern "C"

namespace chre {

class SystemTimerBase {
 public:
  //! The underlying QURT timer.
  SlpiTimerHandle mTimerHandle;

  //! Tracks whether the timer has been initialized correctly.
  bool mInitialized = false;

  //! A static method that is invoked by the underlying QURT timer.
  static void systemTimerNotifyCallback(SlpiTimerCallbackDataType data);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SYSTEM_TIMER_BASE_H_
