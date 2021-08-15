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

#ifndef ANDROID_AUDIO_CLOCK_NANOSLEEP_H
#define ANDROID_AUDIO_CLOCK_NANOSLEEP_H

#include <time.h>
#include <unistd.h>

#ifdef __linux__

#include <sys/syscall.h>
#ifdef __ANDROID__
// bionic for Android provides clock_nanosleep
#define audio_utils_clock_nanosleep clock_nanosleep
#else
// bionic for desktop Linux omits clock_nanosleep
static inline
int audio_utils_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *request,
        struct timespec *remain)
{
    return syscall(SYS_clock_nanosleep, clock_id, flags, request, remain);
}
#endif  // __ANDROID__

#else   // __linux__

// macOS <10.12 doesn't have clockid_t / CLOCK_MONOTONIC
#ifndef CLOCK_MONOTONIC
typedef int clockid_t;
#define CLOCK_MONOTONIC 0
#endif
// macOS doesn't have clock_nanosleep
static inline
int audio_utils_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *request,
        struct timespec *remain)
{
    (void) clock_id;
    (void) flags;
    (void) request;
    (void) remain;
    errno = ENOSYS;
    return -1;
}

#endif  // __linux__

#endif  // !ANDROID_AUDIO_CLOCK_NANOSLEEP_H
