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

#ifndef CHRE_UTIL_CONDITIONAL_LOCK_GUARD_H_
#define CHRE_UTIL_CONDITIONAL_LOCK_GUARD_H_

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * A version of LockGuard that only acquires the lock if a boolean constructor
 * argument evaluates to true.
 */
template<typename MutexType>
class ConditionalLockGuard : public NonCopyable {
 public:
  /**
   * Acquires a lock on the mutex if shouldLock is true, otherwise does nothing
   */
  ConditionalLockGuard(MutexType& mutex, bool shouldLock);

  /**
   * Releases the lock if it was acquired in the constructor
   */
  ~ConditionalLockGuard();

 private:
  MutexType& mMutex;
  bool mShouldLock;
};

}  // namespace chre

#include "chre/util/conditional_lock_guard_impl.h"

#endif  // CHRE_UTIL_CONDITIONAL_LOCK_GUARD_H_
