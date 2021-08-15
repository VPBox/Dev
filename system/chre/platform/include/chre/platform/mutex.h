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

#ifndef CHRE_PLATFORM_MUTEX_H_
#define CHRE_PLATFORM_MUTEX_H_

#include "chre/target_platform/mutex_base.h"
#include "chre/util/non_copyable.h"

namespace chre {

/**
 * Provides an implementation of a Mutex. The public API meets the BasicLockable
 * requirements in order to be compatible with std::lock_guard. MutexBase is
 * subclassed here to allow platforms to inject their own storage for their
 * mutex implementation.
 */
class Mutex : public MutexBase,
              public NonCopyable {
 public:
  /**
   * Allows the platform to do any mutex initialization at construction time.
   */
  Mutex();

  /**
   * Allows the platform to do any mutex deinitialization at destruction time.
   */
  ~Mutex();

  /**
   * Locks the mutex, or blocks if it is held by another thread. Illegal to call
   * if the current thread already holds the lock.
   */
  void lock();

  /**
   * Attempts to lock the mutex. If it is already held by some other thread,
   * returns immediately. Illegal to call if the current thread already holds
   * the lock.
   *
   * @return true if the mutex was acquired, false otherwise
   */
  bool try_lock();

  /**
   * Unlocks the mutex. Illegal to call if the current thread does not hold the
   * lock.
   */
  void unlock();
};

}  // namespace chre

#include "chre/target_platform/mutex_base_impl.h"

#endif  // CHRE_PLATFORM_MUTEX_H_
