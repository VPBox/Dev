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

#ifndef CHRE_PLATFORM_LINUX_SYSTEM_TIMER_BASE_H_
#define CHRE_PLATFORM_LINUX_SYSTEM_TIMER_BASE_H_

#include <cinttypes>
#include <signal.h>
#include <time.h>

namespace chre {

/**
 * The Linux base class for the SystemTimer. The Linux implementation uses a
 * POSIX timer.
 */
class SystemTimerBase {
 protected:
  //! The timer id that is generated during the initialization phase.
  timer_t mTimerId;

  //! Tracks whether the timer has been initialized correctly.
  bool mInitialized = false;

  //! A static method that is invoked by the underlying POSIX timer.
  static void systemTimerNotifyCallback(union sigval cookie);

  //! A utility function to set a POSIX timer.
  bool setInternal(uint64_t delayNs);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_SYSTEM_TIMER_BASE_H_
