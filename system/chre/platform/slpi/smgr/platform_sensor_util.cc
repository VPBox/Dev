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

#include "chre/platform/slpi/smgr/platform_sensor_util.h"

#include <algorithm>

#ifdef GTEST
// This value is taken from the SMGR API definition.
#define SNS_SMGR_SAMPLING_RATE_INVERSION_POINT_V01 1000
#else
#include "sns_smgr_common_v01.h"
#endif  // GTEST

namespace chre {

uint16_t intervalToSmgrSamplingRate(Nanoseconds interval) {
  constexpr uint64_t kInversionPoint =
      SNS_SMGR_SAMPLING_RATE_INVERSION_POINT_V01;
  uint16_t smgrRate = 0;
  Milliseconds millis = Milliseconds(interval);

  if (millis.getMilliseconds() > kInversionPoint) {
    constexpr uint64_t kMaxInterval = INT16_MAX;

    smgrRate = static_cast<uint16_t>(
        std::min(millis.getMilliseconds(), kMaxInterval));
  } else if (interval != Nanoseconds(0)) {
    constexpr uint64_t kMaxRate = kInversionPoint;

    uint64_t rateHz =
        (Seconds(1).toRawNanoseconds() / interval.toRawNanoseconds());
    smgrRate = static_cast<uint16_t>(std::min(rateHz, kMaxRate));
  }

  return smgrRate;
}

uint32_t intervalToSmgrQ16ReportRate(Nanoseconds interval) {
  // Q16 is interpreted as a signed integer by SMGR, but passed through QMI as a
  // uint32_t
  constexpr uint32_t kMaxFreq = INT32_MAX;
  uint64_t freq = kMaxFreq;

  if (interval != Nanoseconds(0)) {
    freq = (Seconds(1).toRawNanoseconds() << 16) / interval.toRawNanoseconds();
  }

  return (freq > kMaxFreq) ? kMaxFreq : static_cast<uint32_t>(freq);
}

}  // namespace chre
