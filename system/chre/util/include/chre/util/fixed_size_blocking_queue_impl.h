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

#ifndef CHRE_UTIL_FIXED_SIZE_BLOCKING_QUEUE_IMPL_H_
#define CHRE_UTIL_FIXED_SIZE_BLOCKING_QUEUE_IMPL_H_

#include "chre/util/fixed_size_blocking_queue.h"
#include "chre/util/lock_guard.h"

namespace chre {

template<typename ElementType, size_t kSize>
bool FixedSizeBlockingQueue<ElementType, kSize>::push(
    const ElementType& element) {
  bool success;
  {
    LockGuard<Mutex> lock(mMutex);
    success = mQueue.push(element);
  }
  if (success) {
    mConditionVariable.notify_one();
  }
  return success;
}

template<typename ElementType, size_t kSize>
bool FixedSizeBlockingQueue<ElementType, kSize>::push(ElementType&& element) {
  bool success;
  {
    LockGuard<Mutex> lock(mMutex);
    success = mQueue.push(std::move(element));
  }
  if (success) {
    mConditionVariable.notify_one();
  }
  return success;
}

template<typename ElementType, size_t kSize>
ElementType FixedSizeBlockingQueue<ElementType, kSize>::pop() {
  LockGuard<Mutex> lock(mMutex);
  while (mQueue.empty()) {
    mConditionVariable.wait(mMutex);
  }

  ElementType element(std::move(mQueue.front()));
  mQueue.pop();
  return element;
}

template<typename ElementType, size_t kSize>
bool FixedSizeBlockingQueue<ElementType, kSize>::empty() {
  LockGuard<Mutex> lock(mMutex);
  return mQueue.empty();
}

template<typename ElementType, size_t kSize>
size_t FixedSizeBlockingQueue<ElementType, kSize>::size() {
  LockGuard<Mutex> lock(mMutex);
  return mQueue.size();
}

template<typename ElementType, size_t kSize>
bool FixedSizeBlockingQueue<ElementType, kSize>::remove(size_t index) {
  LockGuard<Mutex> lock(mMutex);
  return mQueue.remove(index);
}

template<typename ElementType, size_t kCapacity>
ElementType& FixedSizeBlockingQueue<ElementType, kCapacity>::operator[](
    size_t index) {
  LockGuard<Mutex> lock(mMutex);
  return mQueue[index];
}

template<typename ElementType, size_t kCapacity>
const ElementType& FixedSizeBlockingQueue<ElementType, kCapacity>::operator[](
    size_t index) const {
  LockGuard<Mutex> lock(mMutex);
  return mQueue[index];
}

}  // namespace chre

#endif  // CHRE_UTIL_BLOCKING_QUEUE_IMPL_H_
