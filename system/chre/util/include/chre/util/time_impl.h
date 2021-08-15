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

#ifndef CHRE_UTIL_TIME_IMPL_H_
#define CHRE_UTIL_TIME_IMPL_H_

#include "chre/util/time.h"

namespace chre {

constexpr Seconds::Seconds(uint64_t seconds)
    : mSeconds(seconds) {}

constexpr uint64_t Seconds::toRawNanoseconds() const {
  // Perform the simple unit conversion. Warning: overflow is caught and
  // handled by returning UINT64_MAX. A ternary expression is used because
  // constexpr requires it.
  return (mSeconds > (UINT64_MAX / kOneSecondInNanoseconds)) ? UINT64_MAX
      : mSeconds * kOneSecondInNanoseconds;
}

constexpr uint64_t Seconds::getMilliseconds() const {
  // Perform the simple unit conversion. Warning: overflow is caught and
  // handled by returning UINT64_MAX. A ternary expression is used because
  // constexpr requires it.
  return (mSeconds > (UINT64_MAX / kOneSecondInMilliseconds)) ? UINT64_MAX
      : mSeconds * kOneSecondInMilliseconds;
}

constexpr Milliseconds::Milliseconds()
    : mMilliseconds(0) {}

constexpr Milliseconds::Milliseconds(uint64_t milliseconds)
    : mMilliseconds(milliseconds) {}

constexpr Milliseconds::Milliseconds(Nanoseconds nanoseconds)
    : mMilliseconds(
        nanoseconds.toRawNanoseconds() / kOneMillisecondInNanoseconds) {}

constexpr uint64_t Milliseconds::toRawNanoseconds() const {
  // Perform the simple unit conversion. Warning: overflow is caught and
  // handled by returning UINT64_MAX. A ternary expression is used because
  // constexpr requires it.
  return (mMilliseconds > (UINT64_MAX / kOneMillisecondInNanoseconds))
      ? UINT64_MAX : mMilliseconds * kOneMillisecondInNanoseconds;
}

constexpr uint64_t Milliseconds::getMicroseconds() const {
  // Perform the simple unit conversion. Warning: overflow is caught and
  // handled by returning UINT64_MAX. A ternary expression is used because
  // constexpr requires it.
  return (mMilliseconds > (UINT64_MAX / kOneMillisecondInMicroseconds))
      ? UINT64_MAX : mMilliseconds * kOneMillisecondInMicroseconds ;
}

constexpr uint64_t Milliseconds::getMilliseconds() const {
  return mMilliseconds;
}

constexpr bool Milliseconds::operator==(const Milliseconds& millis) const {
  return (mMilliseconds == millis.mMilliseconds);
}

constexpr Microseconds::Microseconds(uint64_t microseconds)
    : mMicroseconds(microseconds) {}

constexpr Microseconds::Microseconds(Nanoseconds nanoseconds)
    : mMicroseconds(
        nanoseconds.toRawNanoseconds() / kOneMicrosecondInNanoseconds) {}

constexpr uint64_t Microseconds::toRawNanoseconds() const {
  // Perform the simple unit conversion. Warning: overflow is caught and
  // handled by returning UINT64_MAX. A ternary expression is used because
  // constexpr requires it.
  return (mMicroseconds > (UINT64_MAX / kOneMicrosecondInNanoseconds))
      ? UINT64_MAX : mMicroseconds * kOneMicrosecondInNanoseconds;
}

constexpr uint64_t Microseconds::getMicroseconds() const {
  return mMicroseconds;
}

constexpr uint64_t Microseconds::getMilliseconds() const {
  return (mMicroseconds / kOneMillisecondInMicroseconds);
}

constexpr Nanoseconds::Nanoseconds()
    : mNanoseconds(0) {}

constexpr Nanoseconds::Nanoseconds(uint64_t nanoseconds)
    : mNanoseconds(nanoseconds) {}

constexpr Nanoseconds::Nanoseconds(Seconds seconds)
    : mNanoseconds(seconds.toRawNanoseconds()) {}

constexpr Nanoseconds::Nanoseconds(Milliseconds milliseconds)
    : mNanoseconds(milliseconds.toRawNanoseconds()) {}

constexpr Nanoseconds::Nanoseconds(Microseconds microseconds)
    : mNanoseconds(microseconds.toRawNanoseconds()) {}

constexpr uint64_t Nanoseconds::toRawNanoseconds() const {
  return mNanoseconds;
}

constexpr bool Nanoseconds::operator==(const Nanoseconds& nanos) const {
  return (mNanoseconds == nanos.mNanoseconds);
}

constexpr bool Nanoseconds::operator!=(const Nanoseconds& nanos) const {
  return !(mNanoseconds == nanos.mNanoseconds);
}

constexpr Nanoseconds operator+(const Seconds& secs, const Nanoseconds& nanos) {
  return Nanoseconds(secs.toRawNanoseconds() + nanos.toRawNanoseconds());
}

constexpr Nanoseconds operator+(const Nanoseconds& nanos_a,
                                const Nanoseconds& nanos_b) {
  return Nanoseconds(nanos_a.toRawNanoseconds() + nanos_b.toRawNanoseconds());
}

constexpr Nanoseconds operator-(const Nanoseconds& nanos_a,
                                const Nanoseconds& nanos_b) {
  return Nanoseconds(nanos_a.toRawNanoseconds() - nanos_b.toRawNanoseconds());
}

constexpr bool operator>=(const Nanoseconds& nanos_a,
                          const Nanoseconds& nanos_b) {
  return nanos_a.toRawNanoseconds() >= nanos_b.toRawNanoseconds();
}

constexpr bool operator<=(const Nanoseconds& nanos_a,
                          const Nanoseconds& nanos_b) {
  return nanos_a.toRawNanoseconds() <= nanos_b.toRawNanoseconds();
}

constexpr bool operator<(const Nanoseconds& nanos_a,
                         const Nanoseconds& nanos_b) {
  return nanos_a.toRawNanoseconds() < nanos_b.toRawNanoseconds();
}

constexpr bool operator>(const Nanoseconds& nanos_a,
                         const Nanoseconds& nanos_b) {
  return nanos_a.toRawNanoseconds() > nanos_b.toRawNanoseconds();
}

}  // namespace chre

#endif  // CHRE_UTIL_TIME_IMPL_H_
