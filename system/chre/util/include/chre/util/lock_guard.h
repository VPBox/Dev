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

#ifndef CHRE_UTIL_LOCK_GUARD_H_
#define CHRE_UTIL_LOCK_GUARD_H_

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * This is an RAII-style wrapper for a mutex that allows acquiring and releasing
 * locks that follow the scope of the guard. The concept is the same as
 * std::lock_guard.
 */
template<typename MutexType>
class LockGuard : public NonCopyable {
 public:
  /**
   * Constructs a LockGuard and acquires the lock.
   */
  LockGuard(MutexType& mutex);

  /**
   * Deconstructs a LockGuard and releases the lock.
   */
  ~LockGuard();

 private:
  //! The mutex to lock and unlock on destruction.
  MutexType& mMutex;
};

}  // namespace chre

#include "chre/util/lock_guard_impl.h"

#endif  // CHRE_UTIL_LOCK_GUARD_H_
