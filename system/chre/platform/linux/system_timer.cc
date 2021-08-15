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

#include "chre/platform/system_timer.h"

#include "chre/platform/log.h"
#include "chre/util/time.h"

#include <errno.h>
#include <signal.h>
#include <string.h>

namespace chre {

namespace {

constexpr uint64_t kOneSecondInNanoseconds = 1000000000;

void NanosecondsToTimespec(uint64_t ns, struct timespec *ts) {
  ts->tv_sec = ns / kOneSecondInNanoseconds;
  ts->tv_nsec = ns % kOneSecondInNanoseconds;
}

}  // anonymous namespace

void SystemTimerBase::systemTimerNotifyCallback(union sigval cookie) {
  SystemTimer *sysTimer = static_cast<SystemTimer*>(cookie.sival_ptr);
  sysTimer->mCallback(sysTimer->mData);
}

SystemTimer::SystemTimer() {}

SystemTimer::~SystemTimer() {
  if (mInitialized) {
    int ret = timer_delete(mTimerId);
    if (ret != 0) {
      LOGE("Couldn't delete timer: %s", strerror(errno));
    }
    mInitialized = false;
  }
}

bool SystemTimer::init() {
  if (mInitialized) {
    LOGW("Tried re-initializing timer");
  } else {
    struct sigevent sigevt = {};
    sigevt.sigev_notify = SIGEV_THREAD;
    sigevt.sigev_value.sival_ptr = this;
    sigevt.sigev_notify_function = systemTimerNotifyCallback;
    sigevt.sigev_notify_attributes = nullptr;

    int ret = timer_create(CLOCK_MONOTONIC, &sigevt, &mTimerId);
    if (ret != 0) {
      LOGE("Couldn't create timer: %s", strerror(errno));
    } else {
      mInitialized = true;
    }
  }

  return mInitialized;
}

bool SystemTimer::set(SystemTimerCallback *callback, void *data,
    Nanoseconds delay) {
  // 0 has a special meaning in POSIX, i.e. cancel the timer. In our API, a
  // value of 0 just means fire right away.
  if (delay.toRawNanoseconds() == 0) {
    delay = Nanoseconds(1);
  }

  if (mInitialized) {
    mCallback = callback;
    mData = data;
    return setInternal(delay.toRawNanoseconds());
  } else {
    return false;
  }
}

bool SystemTimer::cancel() {
  if (mInitialized) {
    // Setting delay to 0 disarms the timer.
    return setInternal(0);
  } else {
    return false;
  }
}

bool SystemTimer::isActive() {
  bool isActive = false;
  if (mInitialized) {
    struct itimerspec spec = {};
    int ret = timer_gettime(mTimerId, &spec);
    if (ret != 0) {
      LOGE("Couldn't obtain current timer configuration: %s", strerror(errno));
    }

    isActive = (spec.it_value.tv_sec > 0 || spec.it_value.tv_nsec > 0);
  }

  return isActive;
}

bool SystemTimerBase::setInternal(uint64_t delayNs) {
  constexpr int kFlags = 0;
  struct itimerspec spec = {};
  bool success = false;

  NanosecondsToTimespec(delayNs, &spec.it_value);
  NanosecondsToTimespec(0, &spec.it_interval);

  int ret = timer_settime(mTimerId, kFlags, &spec, nullptr);
  if (ret != 0) {
    LOGE("Couldn't set timer: %s", strerror(errno));
  } else {
    success = true;
  }

  return success;
}

}  // namespace chre
