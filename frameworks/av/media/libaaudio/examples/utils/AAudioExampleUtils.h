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

#ifndef AAUDIO_EXAMPLE_UTILS_H
#define AAUDIO_EXAMPLE_UTILS_H

#include <atomic>
#include <errno.h>
#include <linux/futex.h>
#include <sched.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <aaudio/AAudio.h>
#include <utils/Errors.h>

#define NANOS_PER_MICROSECOND ((int64_t)1000)
#define NANOS_PER_MILLISECOND (NANOS_PER_MICROSECOND * 1000)
#define NANOS_PER_SECOND      (NANOS_PER_MILLISECOND * 1000)

template <class T = aaudio_sharing_mode_t>
const char *getSharingModeText(aaudio_sharing_mode_t mode) {
    const char *text = "unknown";
    switch (mode) {
        case AAUDIO_SHARING_MODE_EXCLUSIVE:
            text = "EXCLUSIVE";
            break;
        case AAUDIO_SHARING_MODE_SHARED:
            text = "SHARED";
            break;
        default:
            break;
    }
    return text;
}

const char *getPerformanceModeText(aaudio_performance_mode_t mode) {
    const char *text = "unknown";
    switch (mode) {
        case AAUDIO_PERFORMANCE_MODE_NONE:
            text = "NONE";
            break;
        case AAUDIO_PERFORMANCE_MODE_LOW_LATENCY:
            text = "LOW_LATENCY";
            break;
        case AAUDIO_PERFORMANCE_MODE_POWER_SAVING:
            text = "POWER_SAVING";
            break;
        default:
            break;
    }
    return text;
}

const char *getDirectionText(aaudio_direction_t direction) {
    const char *text = "unknown";
    switch (direction) {
        case AAUDIO_DIRECTION_INPUT:
            text = "INPUT";
            break;
        case AAUDIO_DIRECTION_OUTPUT:
            text = "OUTPUT";
            break;
        default:
            break;
    }
    return text;
}

template <class T = int64_t>
void convertNanosecondsToTimespec(int64_t nanoseconds, struct timespec *time) {
    time->tv_sec = nanoseconds / NANOS_PER_SECOND;
    // Calculate the fractional nanoseconds. Avoids expensive % operation.
    time->tv_nsec = nanoseconds - (time->tv_sec * NANOS_PER_SECOND);
}

template <class T = clockid_t>
int64_t getNanoseconds(clockid_t clockId = CLOCK_MONOTONIC) {
    struct timespec time;
    int result = clock_gettime(clockId, &time);
    if (result < 0) {
        return -errno;
    }
    return (time.tv_sec * NANOS_PER_SECOND) + time.tv_nsec;
}

template <class T = float>
void displayPeakLevel(float peakLevel) {
    printf("%5.3f ", peakLevel);
    const int maxStars = 50; // arbitrary, fits on one line
    int numStars = (int) (peakLevel * maxStars);
    for (int i = 0; i < numStars; i++) {
        printf("*");
    }
    printf("\n");
}

/**
 * @param position1 position of hardware frame
 * @param nanoseconds1
 * @param position2 position of client read/write
 * @param nanoseconds2
 * @param sampleRate
 * @return latency in milliseconds
 */
template <class T = int64_t>
double calculateLatencyMillis(int64_t position1, int64_t nanoseconds1,
                              int64_t position2, int64_t nanoseconds2,
                              int64_t sampleRate) {
    int64_t deltaFrames = position2 - position1;
    int64_t deltaTime =
            (NANOS_PER_SECOND * deltaFrames / sampleRate);
    int64_t timeCurrentFramePlayed = nanoseconds1 + deltaTime;
    int64_t latencyNanos = timeCurrentFramePlayed - nanoseconds2;
    double latencyMillis = latencyNanos / 1000000.0;
    return latencyMillis;
}

// ================================================================================
// These Futex calls are common online examples.
template <class T = int>
android::status_t sys_futex(void *addr1, int op, int val1,
                      struct timespec *timeout, void *addr2, int val3) {
    android::status_t result = (android::status_t) syscall(SYS_futex, addr1,
                                                           op, val1, timeout,
                                                           addr2, val3);
    return (result == 0) ? 0 : -errno;
}

template <class T = int>
android::status_t futex_wake(void *addr, int numWake) {
    // Use _PRIVATE because we are just using the futex in one process.
    return sys_futex(addr, FUTEX_WAKE_PRIVATE, numWake, NULL, NULL, 0);
}

template <class T = int>
android::status_t futex_wait(void *addr, int current, struct timespec *time) {
    // Use _PRIVATE because we are just using the futex in one process.
    return sys_futex(addr, FUTEX_WAIT_PRIVATE, current, time, NULL, 0);
}

// TODO better name?
/**
 * The WakeUp class is used to send a wakeup signal to one or more sleeping threads.
 */
class WakeUp {
public:
    WakeUp() : mValue(0) {}
    explicit WakeUp(int32_t value) : mValue(value) {}

    /**
     * Wait until the internal value no longer matches the given value.
     * Note that this code uses a futex, which is subject to spurious wake-ups.
     * So check to make sure that the desired condition has been met.
     *
     * @return zero if the value changes or various negative errors including
     *    -ETIMEDOUT if a timeout occurs,
     *    or -EINTR if interrupted by a signal,
     *    or -EAGAIN or -EWOULDBLOCK if the internal value does not match the specified value
     */
    android::status_t wait(int32_t value, int64_t timeoutNanoseconds) {
        struct timespec time;
        convertNanosecondsToTimespec(timeoutNanoseconds, &time);
        return futex_wait(&mValue, value, &time);
    }

    /**
     * Increment value and wake up any threads that need to be woken.
     *
     * @return number of waiters woken up
     */
    android::status_t wake() {
        ++mValue;
        return futex_wake(&mValue, INT_MAX);
    }

    /**
     * Set value and wake up any threads that need to be woken.
     *
     * @return number of waiters woken up
     */
    android::status_t wake(int32_t value) {
        mValue.store(value);
        return futex_wake(&mValue, INT_MAX);
    }

    int32_t get() {
        return mValue.load();
    }

private:
    std::atomic<int32_t>   mValue;
};

#endif // AAUDIO_EXAMPLE_UTILS_H
