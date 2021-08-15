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

#ifndef ANDROID_AUDIO_FUTEX_H
#define ANDROID_AUDIO_FUTEX_H

// FIXME futex portion is not supported on macOS, should use the macOS alternative

#ifdef __linux__
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#else
#include <errno.h>
#define FUTEX_WAIT 0
#define FUTEX_WAIT_PRIVATE 0
#define FUTEX_WAKE 0
#define FUTEX_WAKE_PRIVATE 0
#endif

static inline
int sys_futex(void *addr1, int op, int val1, const struct timespec *timeout, void *addr2,
        int val3)
{
#ifdef __linux__
    return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
#else   // __linux__
    // macOS doesn't have futex
    (void) addr1;
    (void) op;
    (void) val1;
    (void) timeout;
    (void) addr2;
    (void) val3;
    errno = ENOSYS;
    return -1;
#endif  // __linux__
}

#endif  // !ANDROID_AUDIO_FUTEX_H
