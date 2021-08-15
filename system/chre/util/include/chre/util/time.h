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

#ifndef CHRE_UTIL_TIME_H_
#define CHRE_UTIL_TIME_H_

#include <cstdint>

namespace chre {

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

// Forward declare classes for unit-conversion constructors.
class Milliseconds;
class Microseconds;
class Nanoseconds;


class Seconds {
 public:
  /**
   * Construct a Seconds time duration given a value.
   */
  constexpr explicit Seconds(uint64_t seconds);

  /**
   * Converts the underlying seconds to a raw uint64_t representation of
   * nanoseconds. Handles overflyw by returning UINT64_MAX.
   *
   * @return the value of seconds converted to nanoseconds
   */
  constexpr uint64_t toRawNanoseconds() const;

  /**
   * Obtains the number of Milliseconds stored by this time duration.
   *
   * @return the value of milliseconds.
   */
  constexpr uint64_t getMilliseconds() const;

 private:
  uint64_t mSeconds;
};

/**
 * Represents a duration of time in milliseconds.
 */
class Milliseconds {
 public:
  /**
   * Default constructs a milliseconds time duration to zero.
   */
  constexpr Milliseconds();

  /**
   * Construct a Milliseconds time duration given a value.
   */
  constexpr explicit Milliseconds(uint64_t milliseconds);

  /**
   * Constructs a Microseconds time duration given nanoseconds.
   */
  constexpr Milliseconds(Nanoseconds nanoseconds);

  /**
   * Converts the underlying milliseconds to a raw uint64_t representation of
   * nanoseconds. Handles overflow by returning UINT64_MAX.
   *
   * @return the value of milliseconds converted to nanoseconds
   */
  constexpr uint64_t toRawNanoseconds() const;

  /**
   * Obtains the number of Microseconds stored by this time duration.
   *
   * @return the value of microseconds.
   */
  constexpr uint64_t getMicroseconds() const;

  /**
   * Obtains the number of Milliseconds stored by this time duration.
   *
   * @return the value of milliseconds.
   */
  constexpr uint64_t getMilliseconds() const;

  /**
   * Performs an equality comparison to another Milliseconds value.
   *
   * @return Returns true if this milliseconds object is equal to another.
   */
  constexpr bool operator==(const Milliseconds& millis) const;

 private:
  //! Store the time duration.
  uint64_t mMilliseconds;
};

/**
 * Represents a duration of time in microseconds.
 */
class Microseconds {
 public:
  /**
   * Construct a Microseconds time duration given a value.
   */
  constexpr explicit Microseconds(uint64_t microseconds);

  /**
   * Constructs a Microseconds time duration given nanoseconds.
   */
  constexpr Microseconds(Nanoseconds nanoseconds);

  /**
   * Converts the underlying microseconds to a raw uint64_t representation of
   * nanoseconds. Handles overflow by returning UINT64_MAX.
   *
   * @return the value of microseconds converted to nanoseconds.
   */
  constexpr uint64_t toRawNanoseconds() const;

  /**
   * Obtains the number of Microseconds stored by this time duration.
   *
   * @return the value of microseconds.
   */
  constexpr uint64_t getMicroseconds() const;

  /**
   * Obtains the rounded-down number of Milliseconds stored by this time
   * duration.
   *
   * @return the value of milliseconds.
   */
  constexpr uint64_t getMilliseconds() const;

 private:
  //! Store the time duration.
  uint64_t mMicroseconds;
};

/**
 * Represents a duration of time in nanoseconds.
 */
class Nanoseconds {
 public:
  /**
   * Default constructs a Nanoseconds time duration to zero.
   */
  constexpr Nanoseconds();

  /**
   * Constructs a Nanoseconds time duration given a value.
   */
  constexpr explicit Nanoseconds(uint64_t nanoseconds);

  /**
   * Converts a seconds value to nanoseconds.
   */
  constexpr Nanoseconds(Seconds seconds);

  /**
   * Converts a milliseconds value to nanoseconds.
   */
  constexpr Nanoseconds(Milliseconds milliseconds);

  /**
   * Constructs a Nanoseconds time duration given microseconds.
   */
  constexpr Nanoseconds(Microseconds microseconds);

  /**
   * Converts the underlying nanoseconds to a raw uint64_t representation of
   * nanoseconds.
   *
   * @return the value of nanoseconds
   */
  constexpr uint64_t toRawNanoseconds() const;

  /**
   * Performs an equality comparison to another Nanoseconds value.
   *
   * @return Returns true if this nanoseconds object is equal to another.
   */
  constexpr bool operator==(const Nanoseconds& nanos) const;

  /**
   * Performs an inequality comparison to another Nanoseconds value.
   *
   * @return Returns true if this nanoseconds object is not equal to another.
   */
  constexpr bool operator!=(const Nanoseconds& nanos) const;

 private:
  uint64_t mNanoseconds;
};

/**
 * Add seconds to nanoseconds.
 *
 * @param seconds the seconds duration
 * @param nanoseconds the nanoseconds duration
 * @return the added time quantity expressed in nanoseconds
 */
constexpr Nanoseconds operator+(const Seconds& secs, const Nanoseconds& nanos);

/**
 * Add nanoseconds to nanoseconds.
 *
 * @param nanos_a The first nanoseconds duration
 * @param nanos_b The second nanoseconds duration
 * @return The added time quantity expressed in nanoseconds
 */
constexpr Nanoseconds operator+(const Nanoseconds& nanos_a,
                                const Nanoseconds& nanos_b);

/**
 * Subtract two nanosecond durations.
 *
 * @param nanos_a the first nanoseconds duration
 * @param nanos_b the second nanoseconds duration
 * @return the difference between the two durations
 */
constexpr Nanoseconds operator-(const Nanoseconds& nanos_a,
                                const Nanoseconds& nanos_b);

/**
 * Performs a greater than or equal to comparison on two nanoseconds values.
 *
 * @param nanos_a the first nanoseconds duration
 * @param nanos_b the second nanoseconds duration
 * @return Whether nanos_a is greater than or equal to nanos_b.
 */
constexpr bool operator>=(const Nanoseconds& nanos_a,
                          const Nanoseconds& nanos_b);

/**
 * Performs a less than or equal to comparison on two nanoseconds values.
 *
 * @param nanos_a the first nanoseconds duration
 * @param nanos_b the second nanoseconds duration
 * @return Whether nanos_a is less than or equal to nanos_b.
 */
constexpr bool operator<=(const Nanoseconds& nanos_a,
                          const Nanoseconds& nanos_b);

/**
 * Performs a less than comparison on two nanoseconds values.
 *
 * @param nanos_a the first nanoseconds duration
 * @param nanos_b the second nanoseconds duration
 * @return Whether nanos_a is less than nanos_b.
 */
constexpr bool operator<(const Nanoseconds& nanos_a,
                         const Nanoseconds& nanos_b);

/**
 * Performs a greater than comparison on two nanoseconds values.
 *
 * @param nanos_a the first nanoseconds duration
 * @param nanos_b the second nanoseconds duration
 * @return Whether nanos_a is less than nanos_b.
 */
constexpr bool operator>(const Nanoseconds& nanos_a,
                         const Nanoseconds& nanos_b);

}  // namespace chre

#include "chre/util/time_impl.h"

#endif // CHRE_UTIL_TIME_H_
