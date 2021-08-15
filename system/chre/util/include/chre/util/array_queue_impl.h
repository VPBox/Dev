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

#ifndef CHRE_UTIL_ARRAY_QUEUE_IMPL_H_
#define CHRE_UTIL_ARRAY_QUEUE_IMPL_H_

#include <new>
#include <utility>

#include "chre/util/array_queue.h"
#include "chre/util/container_support.h"

namespace chre {

template<typename ElementType, size_t kCapacity>
ArrayQueue<ElementType, kCapacity>::~ArrayQueue() {
  clear();
}

template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::empty() const {
  return (mSize == 0);
}

template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::full() const {
  return (mSize == kCapacity);
}

template<typename ElementType, size_t kCapacity>
size_t ArrayQueue<ElementType, kCapacity>::size() const {
  return mSize;
}

template<typename ElementType, size_t kCapacity>
ElementType& ArrayQueue<ElementType, kCapacity>::front() {
  CHRE_ASSERT(mSize > 0);
  return data()[mHead];
}

template<typename ElementType, size_t kCapacity>
const ElementType& ArrayQueue<ElementType, kCapacity>::front() const {
  CHRE_ASSERT(mSize > 0);
  return data()[mHead];
}

template<typename ElementType, size_t kCapacity>
ElementType& ArrayQueue<ElementType, kCapacity>::back() {
  CHRE_ASSERT(mSize > 0);
  return data()[mTail];
}

template<typename ElementType, size_t kCapacity>
const ElementType& ArrayQueue<ElementType, kCapacity>::back() const {
  CHRE_ASSERT(mSize > 0);
  return data()[mTail];
}

template<typename ElementType, size_t kCapacity>
ElementType& ArrayQueue<ElementType, kCapacity>::operator[](size_t index) {
  CHRE_ASSERT(index < mSize);
  return data()[relativeIndexToAbsolute(index)];
}

template<typename ElementType, size_t kCapacity>
const ElementType& ArrayQueue<ElementType, kCapacity>::operator[](
    size_t index) const {
  CHRE_ASSERT(index < mSize);
  return data()[relativeIndexToAbsolute(index)];
}

template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::push(const ElementType& element) {
  bool success = pushTail();
  if (success) {
    new (&data()[mTail]) ElementType(element);
  }
  return success;
}

template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::push(ElementType&& element) {
  bool success = pushTail();
  if (success) {
    new (&data()[mTail]) ElementType(std::move(element));
  }
  return success;
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::kick_push(const ElementType& element) {
  if (full()) {
    pop();
  }
  push(element);
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::kick_push(ElementType&& element) {
  if (full()) {
    pop();
  }
  push(element);
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::pop() {
  if (mSize > 0) {
    data()[mHead].~ElementType();
    pullHead();
  }
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::pop_back() {
  if (mSize > 0) {
    size_t absoluteIndex = relativeIndexToAbsolute(mSize - 1);
    data()[absoluteIndex].~ElementType();
    pullTail();
  }
}

// Assuming popping from the middle of the queue is rare, part of the
// array is copied over.
template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::remove(size_t index) {
  // If we used memmove to shift the array down when removing an element in the
  // middle of the queue, then we'd need to add this somewhere:
  // static_assert(std::is_trivially_copyable<ElementType>::value,
  //               "Elements within ArrayQueue must be trivially copyable");

  bool success;
  if (index >= mSize) {
    success = false;
  } else {
    // Number of elements before the one to be popped
    size_t headLength = index;

    size_t absoluteIndex = relativeIndexToAbsolute(index);
    data()[absoluteIndex].~ElementType();

    // Move all the elements before the one just popped to the next storage
    // space.
    // TODO: optimize by comparing headLength to mSize/2.
    // If headLength < mSize/2, pull heads towards tail.
    // Otherwise, pull tails towards head.
    for (size_t i = 0; i < headLength; ++i) {
      size_t prev = (absoluteIndex == 0) ? (kCapacity - 1) : (absoluteIndex - 1);
      data()[absoluteIndex] = data()[prev];
      absoluteIndex = prev;
    }

    pullHead();
    success = true;
  }
  return success;
}

template<typename ElementType, size_t kCapacity>
template<typename... Args>
bool ArrayQueue<ElementType, kCapacity>::emplace(Args&&... args) {
  bool success = pushTail();
  if (success) {
    new (&data()[mTail]) ElementType(std::forward<Args>(args)...);
  }
  return success;
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::clear() {
  if (!std::is_trivially_destructible<ElementType>::value) {
    while (!empty()) {
      pop();
    }
  } else {
    mSize = 0;
    mHead = 0;
    mTail = kCapacity - 1;
  }
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::iterator
ArrayQueue<ElementType, kCapacity>::begin() {
  // Align begin() and end() outside of the memory block when empty.
  return empty() ? end() : iterator(data() + mHead, data(), mTail);
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::iterator
    ArrayQueue<ElementType, kCapacity>::end() {
  return iterator(data() + kCapacity, data(), mTail);
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::const_iterator
ArrayQueue<ElementType, kCapacity>::begin() const {
  return cbegin();
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::const_iterator
    ArrayQueue<ElementType, kCapacity>::end() const {
  return cend();
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::const_iterator
ArrayQueue<ElementType, kCapacity>::cbegin() const {
  // Align begin() and end() outside of the memory block when empty.
  return empty() ? cend() : const_iterator(data() + mHead, data(), mTail);
}

template<typename ElementType, size_t kCapacity>
typename ArrayQueue<ElementType, kCapacity>::const_iterator
    ArrayQueue<ElementType, kCapacity>::cend() const {
  return const_iterator(data() + kCapacity, data(), mTail);
}

template<typename ElementType, size_t kCapacity>
ElementType *ArrayQueue<ElementType, kCapacity>::data() {
  return reinterpret_cast<ElementType *>(mData);
}

template<typename ElementType, size_t kCapacity>
const ElementType *ArrayQueue<ElementType, kCapacity>::data() const {
  return reinterpret_cast<const ElementType *>(mData);
}

template<typename ElementType, size_t kCapacity>
size_t ArrayQueue<ElementType, kCapacity>::relativeIndexToAbsolute(
    size_t index) const {
  size_t absoluteIndex = mHead + index;
  if (absoluteIndex >= kCapacity) {
    absoluteIndex -= kCapacity;
  }
  return absoluteIndex;
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::pullHead() {
  CHRE_ASSERT(mSize > 0);
  if (++mHead == kCapacity) {
      mHead = 0;
  }
  mSize--;
}

template<typename ElementType, size_t kCapacity>
void ArrayQueue<ElementType, kCapacity>::pullTail() {
  CHRE_ASSERT(mSize > 0);
  if (mTail == 0) {
    mTail = kCapacity - 1;
  } else {
    mTail--;
  }
  mSize--;
}

template<typename ElementType, size_t kCapacity>
bool ArrayQueue<ElementType, kCapacity>::pushTail() {
  bool success;
  if (mSize >= kCapacity) {
    success = false;
  } else {
    if (++mTail == kCapacity) {
      mTail = 0;
    }
    mSize++;
    success = true;
  }
  return success;
}

}  // namespace chre

#endif  // CHRE_UTIL_ARRAY_QUEUE_IMPL_H_
