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

#ifndef CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_IMPL_H_
#define CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_IMPL_H_

#include "chre/util/lock_guard.h"
#include "chre/util/synchronized_memory_pool.h"

namespace chre {

template<typename ElementType, size_t kSize>
template<typename... Args>
ElementType *SynchronizedMemoryPool<ElementType, kSize>::allocate(
    Args&&... args) {
  LockGuard<Mutex> lock(mMutex);
  return mMemoryPool.allocate(args...);
}

template<typename ElementType, size_t kSize>
void SynchronizedMemoryPool<ElementType, kSize>::deallocate(
    ElementType *element) {
  LockGuard<Mutex> lock(mMutex);
  mMemoryPool.deallocate(element);
}

template<typename ElementType, size_t kSize>
size_t SynchronizedMemoryPool<ElementType, kSize>::getFreeBlockCount() {
  LockGuard<Mutex> lock(mMutex);
  return mMemoryPool.getFreeBlockCount();
}

}  // namespace chre

#endif  // CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_IMPL_H_
