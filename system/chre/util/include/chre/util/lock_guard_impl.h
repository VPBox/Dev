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

#ifndef CHRE_UTIL_LOCK_GUARD_IMPL_H_
#define CHRE_UTIL_LOCK_GUARD_IMPL_H_

#include "chre/util/lock_guard.h"

namespace chre {

template<typename MutexType>
LockGuard<MutexType>::LockGuard(MutexType& mutex)
    : mMutex(mutex) {
  mMutex.lock();
}

template<typename MutexType>
LockGuard<MutexType>::~LockGuard() {
  mMutex.unlock();
}

}  // namespace chre

#endif  // CHRE_UTIL_LOCK_GUARD_IMPL_H_
