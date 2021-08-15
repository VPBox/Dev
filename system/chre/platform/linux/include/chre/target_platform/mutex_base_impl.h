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

#ifndef CHRE_PLATFORM_LINUX_MUTEX_BASE_IMPL_H_
#define CHRE_PLATFORM_LINUX_MUTEX_BASE_IMPL_H_

#include "chre/platform/mutex.h"

namespace chre {

inline Mutex::Mutex() {}

inline Mutex::~Mutex() {}

inline void Mutex::lock() {
  mMutex.lock();
}

inline bool Mutex::try_lock() {
  return mMutex.try_lock();
}

inline void Mutex::unlock() {
  mMutex.unlock();
}

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_MUTEX_BASE_IMPL_H_
