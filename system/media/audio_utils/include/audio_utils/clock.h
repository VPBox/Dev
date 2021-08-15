/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_CLOCK_H
#define ANDROID_AUDIO_CLOCK_H

// This file can be included for either C or C++ source.

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

// These are declared as macros for compatbility with existing uses.
// TODO Spell out the words in full.
#define MICROS_PER_SECOND      1000000LL
#define MILLIS_PER_SECOND         1000LL
#define NANOS_PER_MICROSECOND     1000LL
#define NANOS_PER_MILLISECOND  1000000LL
#define NANOS_PER_SECOND    1000000000LL

#define SECONDS_PER_MINUTE  60LL
#define MINUTES_PER_HOUR    60LL

#define MICROS_PER_MINUTE   (MICROS_PER_SECOND * SECONDS_PER_MINUTE)
#define MILLIS_PER_MINUTE   (MILLIS_PER_SECOND * SECONDS_PER_MINUTE)
#define NANOS_PER_MINUTE    (NANOS_PER_SECOND  * SECONDS_PER_MINUTE)

#define MICROS_PER_HOUR     (MICROS_PER_MINUTE * MINUTES_PER_HOUR)
#define MILLIS_PER_HOUR     (MILLIS_PER_MINUTE * MINUTES_PER_HOUR)
#define NANOS_PER_HOUR      (NANOS_PER_MINUTE  * MINUTES_PER_HOUR)

/**
 * \brief Converts time in ns to a time string, with format similar to logcat.
 * \param ns          input time in nanoseconds to convert.
 * \param buffer      caller allocated string buffer, buffer_length must be >= 19 chars
 *                    in order to fully fit in time.  The string is always returned
 *                    null terminated if buffer_size is greater than zero.
 * \param buffer_size size of buffer.
 */
static inline void audio_utils_ns_to_string(int64_t ns, char *buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;

    const int one_second = 1000000000;
    const time_t sec = ns / one_second;
    struct tm tm;

    // Supported on bionic, glibc, and macOS, but not mingw.
    if (localtime_r(&sec, &tm) == NULL) {
        buffer[0] = '\0';
        return;
    }

    if (snprintf(buffer, buffer_size, "%02d-%02d %02d:%02d:%02d.%03d",
        tm.tm_mon + 1, // localtime_r uses months in 0 - 11 range
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
        (int)(ns % one_second / 1000000)) < 0) {
        buffer[0] = '\0'; // null terminate on format error, which should not happen
    }
}

/**
 * An object that contains the formatted time string.
 *
 * The time string is 19 characters (including null termination).
 * Example: "03-27 16:47:06.187"
 *           MM DD HH MM SS MS
 */
typedef struct audio_utils_time_string {
    char time[19]; /* minimum size buffer */
} audio_utils_time_string_t;

/**
 * \brief Converts time in ns to a time string object, with format similar to logcat.
 * \param ns          input time in nanoseconds to convert.
 */
static inline audio_utils_time_string_t audio_utils_time_string_from_ns(int64_t ns)
{
    audio_utils_time_string_t ts;

    audio_utils_ns_to_string(ns, ts.time, sizeof(ts.time));
    return ts;
}

/**
 * \brief Converts a timespec to nanoseconds.
 * \param ts   input timespec to convert.
 * \return     timespec converted to nanoseconds.
 */
static inline int64_t audio_utils_ns_from_timespec(const struct timespec *ts)
{
    return ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

/**
 * \brief Gets the real time clock in nanoseconds.
 * \return the real time clock in nanoseconds, or 0 on error.
 */
static inline int64_t audio_utils_get_real_time_ns() {

#if defined(__linux__)

    struct timespec now_ts;
    if (clock_gettime(CLOCK_REALTIME, &now_ts) == 0) {
        return audio_utils_ns_from_timespec(&now_ts);
    }
    return 0; // should not happen.

#else

    // Mac OS X compatible
    struct timeval now_tv;
    if (gettimeofday(&now_tv, NULL /* struct timezone * */) == 0) {
        return now_tv.tv_sec * 1000000000LL + now_tv.tv_usec * 1000LL;
    }
    return 0; // should not happen.

#endif

}

#endif  // !ANDROID_AUDIO_CLOCK_H
