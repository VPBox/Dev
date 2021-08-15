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

#include "chre/platform/slpi/system_time_util.h"

#include "chre/util/time.h"

namespace chre {

uint64_t getNanosecondsFromQTimerTicks(uint64_t ticks) {
  constexpr uint64_t kClockFreq = 19200000;  // 19.2MHz QTimer clock

  uint64_t nsec = 0;
  if (ticks >= kClockFreq) {
    uint64_t seconds = (ticks / kClockFreq);
    ticks %= kClockFreq;

    nsec = (seconds * kOneSecondInNanoseconds);
  }
  nsec += (ticks * kOneSecondInNanoseconds) / kClockFreq;
  return nsec;
}

}  // namespace chre
