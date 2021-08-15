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

#ifndef CHRE_UTIL_HEAP_IMPL_H_
#define CHRE_UTIL_HEAP_IMPL_H_

#include <utility>

#include "chre/platform/assert.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/fixed_size_vector.h"

namespace chre {

namespace {

template<typename ContainerType, typename CompareFunction>
void siftUp(ContainerType& container, size_t index,
            const CompareFunction& compare) {
  CHRE_ASSERT(index < container.size());
  size_t current = index;
  while (current > 0) {
    size_t parent = (current - 1) / 2;
    if (compare(container[parent], container[current])) {
      container.swap(parent, current);
      current = parent;
    } else {
      break;
    }
  }
}

template<typename ContainerType, typename CompareFunction>
void siftDown(ContainerType& container, size_t index,
              const CompareFunction& compare) {
  CHRE_ASSERT(index < container.size());
  size_t current = index;

  // The last element is to be removed. If that's the element being indexed,
  // it's a no-op.
  while (index < container.size() - 1) {
    size_t child = 2 * current + 1;  // left child

    // If there are two children, pick the dominant one.
    if (child + 1 < container.size() - 1 &&
        compare(container[child], container[child + 1])) {
      child++;
    }

    // If the current element is not childless and the dominant child dominates
    // over it, swap and continue.
    if (child < container.size() - 1 &&
        compare(container[current], container[child])) {
      container.swap(current, child);
      current = child;
    } else {
      break;
    }
  }
}

}  // namespace

template<typename ContainerType, typename CompareFunction>
void push_heap(ContainerType& container, const CompareFunction& compare) {
  CHRE_ASSERT(container.size() > 0);
  if (container.size() > 0) {
    siftUp(container, container.size() - 1, compare);
  }
}

template<typename ContainerType, typename CompareFunction>
void pop_heap(ContainerType& container, const CompareFunction& compare) {
  CHRE_ASSERT(container.size() > 0);
  if (container.size() > 0) {
    container.swap(0, container.size() - 1);
    siftDown(container, 0, compare);
  }
}

template<typename ContainerType, typename CompareFunction>
void remove_heap(ContainerType& container, size_t index,
                 const CompareFunction& compare) {
  CHRE_ASSERT(index < container.size());
  container.swap(index, container.size() - 1);

  size_t parent = (index - 1) / 2;
  // When index = 0, it has no parent and can only sift down.
  if (index > 0 && compare(container[parent], container[index])) {
    siftUp(container, index, compare);
  } else {
    siftDown(container, index, compare);
  }
}

}  // namespace chre

#endif  // CHRE_UTIL_PRIORITY_QUEUE_IMPL_H_
