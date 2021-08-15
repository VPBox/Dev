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

#ifndef CHRE_UTIL_PRIORITY_QUEUE_IMPL_H_
#define CHRE_UTIL_PRIORITY_QUEUE_IMPL_H_

#include "chre/util/priority_queue.h"

#include <utility>

#include "chre/platform/assert.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/heap.h"

namespace chre {

template<typename ElementType, typename CompareFunction>
PriorityQueue<ElementType, CompareFunction>::PriorityQueue() {}

template<typename ElementType, typename CompareFunction>
PriorityQueue<ElementType, CompareFunction>::PriorityQueue(
    const CompareFunction& compare)
    : mCompare(compare) {}

template<typename ElementType, typename CompareFunction>
size_t PriorityQueue<ElementType, CompareFunction>::size() const {
  return mData.size();
}

template<typename ElementType, typename CompareFunction>
size_t PriorityQueue<ElementType, CompareFunction>::capacity() const {
  return mData.capacity();
}

template<typename ElementType, typename CompareFunction>
bool PriorityQueue<ElementType, CompareFunction>::empty() const {
  return mData.empty();
}

template<typename ElementType, typename CompareFunction>
bool PriorityQueue<ElementType, CompareFunction>::push(
    const ElementType& element) {
  bool success = mData.push_back(element);
  if (success) {
    push_heap(mData, mCompare);
  }
  return success;
}

template<typename ElementType, typename CompareFunction>
template<typename... Args>
bool PriorityQueue<ElementType, CompareFunction>::emplace(Args&&... args) {
  bool success = mData.emplace_back(args...);
  if (success) {
    push_heap(mData, mCompare);
  }
  return success;
}

template<typename ElementType, typename CompareFunction>
ElementType& PriorityQueue<ElementType, CompareFunction>::operator[](
    size_t index) {
  return mData[index];
}

template<typename ElementType, typename CompareFunction>
const ElementType& PriorityQueue<ElementType, CompareFunction>::operator[](
    size_t index) const {
  return mData[index];
}

template<typename ElementType, typename CompareFunction>
ElementType& PriorityQueue<ElementType, CompareFunction>::top() {
  return mData[0];
}

template<typename ElementType, typename CompareFunction>
const ElementType& PriorityQueue<ElementType, CompareFunction>::top() const {
  return mData[0];
}

template<typename ElementType, typename CompareFunction>
void PriorityQueue<ElementType, CompareFunction>::pop() {
  if (mData.size() > 0) {
    pop_heap(mData, mCompare);
    mData.erase(mData.size() - 1);
  }
}

template<typename ElementType, typename CompareFunction>
void PriorityQueue<ElementType, CompareFunction>::remove(size_t index) {
  CHRE_ASSERT(index < mData.size());
  if (index < mData.size()) {
    remove_heap(mData, index, mCompare);
    mData.erase(mData.size() - 1);
  }

  // TODO: consider resizing the dynamic array to mData.capacity()/2
  // when mData.size() <= mData.capacity()/4.
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::iterator
    PriorityQueue<ElementType, CompareFunction>::begin() {
  return mData.begin();
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::iterator
    PriorityQueue<ElementType, CompareFunction>::end() {
  return mData.end();
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::const_iterator
    PriorityQueue<ElementType, CompareFunction>::begin() const {
  return cbegin();
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::const_iterator
    PriorityQueue<ElementType, CompareFunction>::end() const {
  return cend();
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::const_iterator
    PriorityQueue<ElementType, CompareFunction>::cbegin() const {
  return mData.cbegin();
}

template<typename ElementType, typename CompareFunction>
typename PriorityQueue<ElementType, CompareFunction>::const_iterator
    PriorityQueue<ElementType, CompareFunction>::cend() const {
  return mData.cend();
}

}  // namespace chre

#endif  // CHRE_UTIL_PRIORITY_QUEUE_IMPL_H_
