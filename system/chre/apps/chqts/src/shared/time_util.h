/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef _GTS_NANOAPPS_SHARED_TIME_UTIL_H_
#define _GTS_NANOAPPS_SHARED_TIME_UTIL_H_

#include <cstdint>

namespace nanoapp_testing {

//! The number of milliseconds in one min.
constexpr uint64_t kOneMinuteInMilliseconds(60000);

//! The number of milliseconds in one second.
constexpr uint64_t kOneSecondInMilliseconds(1000);

//! The number of nanoseconds in one second.
constexpr uint64_t kOneSecondInNanoseconds(1000000000);

//! The number of nanoseconds in one millisecond.
constexpr uint64_t kOneMillisecondInNanoseconds(1000000);

//! The number of nanoseconds in one microsecond.
constexpr uint64_t kOneMicrosecondInNanoseconds(1000);

//! The number of microseconds in one millisecond.
constexpr uint64_t kOneMillisecondInMicroseconds(1000);

}  // namespace nanoapp_testing

#endif  // _GTS_NANOAPPS_SHARED_TIME_UTIL_H_
