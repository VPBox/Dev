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

#ifndef CHRE_UTIL_PRIORITY_QUEUE_H_
#define CHRE_UTIL_PRIORITY_QUEUE_H_

#include <cstddef>
#include <functional>

#include "chre/util/dynamic_vector.h"
#include "chre/util/non_copyable.h"

namespace chre {

/**
 * An implementation of a priority queue. This allows for efficient lookup of
 * the highest priority element as defined by the CompareFunction.
 */
template<typename ElementType, typename CompareFunction = std::less<ElementType>>
class PriorityQueue : public NonCopyable {
 public:
  /**
   * Constructs the object.
   */
  PriorityQueue();

  /**
   * Constructs the object with a compare type that provides a strict weak
   * ordering.
   *
   * @param compare The comparator that returns true if left < right.
   */
  PriorityQueue(const CompareFunction& compare);

  /**
   * Returns the current number of elements in the queue.
   *
   * @return The number of elements in the queue.
   */
  size_t size() const;

  /**
   * Returns the maximum number of elements that can be stored in this queue
   * without a resize operation.
   *
   * @return The capacity of the queue.
   */
  size_t capacity() const;

  /**
   * Determines whether the queue is empty or not.
   *
   * @return true if the queue is empty.
   */
  bool empty() const;

  /**
   * Pushes an element onto the queue. If the queue requires a resize and that
   * allocation fails, this function will return false. All iterators and
   * references are invalidated.
   *
   * @param element The element to push onto the queue.
   * @return true if the element was pushed successfully.
   */
  bool push(const ElementType& element);

  /**
   * Constructs an element onto the the queue. All iterators and references are
   * invalidated.
   *
   * @param args The arguments to the constructor of ElementType
   */
  template<typename... Args>
  bool emplace(Args&&... args);

  /*
   * Obtains a const element of the queue given an index. It is illegal to
   * index this queue out of bounds and the user of the API must check the
   * size() function prior to indexing this queue to ensure that they will not
   * read out of bounds. It returns the top element with index equal to 0 when
   * the queue is not empty, and there is no guarantee for index > 0.
   *
   * @param index The index of the element.
   * @return The element.
   */
  ElementType& operator[](size_t index);

  /*
   * Obtains a const element of the queue given an index. It is illegal to
   * index this queue out of bounds and the user of the API must check the
   * size() function prior to indexing this queye to ensure that they will not
   * read out of bounds. It returns the top element with index equal to 0 when
   * the queue is not empty, and there is no guarantee for index > 0.
   *
   * @param index The index of the element.
   * @return The element.
   */
  const ElementType& operator[](size_t index) const;

  /**
   * Obtains the top element of the queue. It is illegal to do this when the
   * queue is empty. The user of the API must check the size() or empty()
   * function prior to calling this to ensure that they will not
   * read out of bounds.
   *
   * @return The element.
   */
  ElementType& top();

  /**
   * Obtains the top element of the queue. It is illegal to do this when the
   * queue is empty. The user of the API must check the size() or empty()
   * function prior to calling this to ensure that they will not
   * read out of bounds.
   *
   * @return The element.
   */
  const ElementType& top() const;

  /**
   * Removes the top element from the queue if the queue is not empty. All
   * iterators and references are invalidated.
   */
  void pop();

  /**
   * Removes an element from the queue given an index. The index passed in must
   * be less than the size() of the queue. If the index is greater than or
   * equal to the size no operation is performed. All iterators and references
   * are invalidated.
   *
   * @param index The index to remove an element at.
   */
  void remove(size_t index);

  /**
   * Random-access iterator that points to some element in the container.
   */
  typedef ElementType* iterator;
  typedef const ElementType* const_iterator;

  /**
   * @return A random-access iterator to the beginning.
   */
  typename PriorityQueue<ElementType, CompareFunction>::iterator begin();
  typename PriorityQueue<ElementType, CompareFunction>::const_iterator begin() const;
  typename PriorityQueue<ElementType, CompareFunction>::const_iterator cbegin() const;

  /**
   * @return A random-access iterator to the end.
   */
  typename PriorityQueue<ElementType, CompareFunction>::iterator end();
  typename PriorityQueue<ElementType, CompareFunction>::const_iterator end() const;
  typename PriorityQueue<ElementType, CompareFunction>::const_iterator cend() const;


 private:
  //! The dynamic vector that serves as the underlying container.
  DynamicVector<ElementType> mData;

  //! The comparator that is used to order the queue.
  CompareFunction mCompare;
};

}  // namespace chre

#include "chre/util/priority_queue_impl.h"

#endif  // CHRE_UTIL_PRIORITY_QUEUE_H_
