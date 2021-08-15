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

#ifndef CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_H_
#define CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_H_

#include "chre/platform/mutex.h"
#include "chre/util/memory_pool.h"

namespace chre {

/**
 * This is a thread-safe version of the MemoryPool.
 */
template<typename ElementType, size_t kSize>
class SynchronizedMemoryPool : public NonCopyable {
 public:
  /**
   * Allocates space for an object, constructs it and returns the pointer to
   * that object. This method is thread-safe and a lock will be acquired
   * upon entry to this method.
   *
   * @param  The arguments to be forwarded to the constructor of the object.
   * @return A pointer to a constructed object or nullptr if the allocation
   *         fails.
   */
  template<typename... Args>
  ElementType *allocate(Args&&... args);

  /**
   * Releases the memory of a previously allocated element. The pointer provided
   * here must be one that was produced by a previous call to the allocate()
   * function. The destructor is invoked on the object. This method is
   * thread-safe and a lock will be acquired upon entry to this method.
   *
   * @param A pointer to an element that was previously allocated by the
   *        allocate() function.
   */
  void deallocate(ElementType *element);

  /**
   * @return the number of unused blocks in this memory pool.
   */
  size_t getFreeBlockCount();

 private:
  //! The mutex used to guard access to this memory pool.
  Mutex mMutex;

  //! The non-synchronized MemoryPool that is used to implement this thread-safe
  //! version.
  MemoryPool<ElementType, kSize> mMemoryPool;
};

}  // namespace chre

#include "chre/util/synchronized_memory_pool_impl.h"

#endif  // CHRE_UTIL_SYNCHRONIZED_MEMORY_POOL_H_
