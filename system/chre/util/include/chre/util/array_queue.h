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

#ifndef CHRE_UTIL_ARRAY_QUEUE_H_
#define CHRE_UTIL_ARRAY_QUEUE_H_

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * A fixed-size FIFO queue for storing elements. When the FIFO is full, new
 * element will not be able to be pushed in.
 */
template<typename ElementType, size_t kCapacity>
class ArrayQueue : public NonCopyable {
 public:
  /**
   * Calls the destructor of all the elements in the array queue.
   */
  ~ArrayQueue();

  /**
   * Determines whether the array queue is empty or not.
   *
   * @return true if the array queue is empty.
   */
  bool empty() const;

  /**
   * @return true if the array queue is full.
   */
  bool full() const;

  /**
   * Obtains the number of elements currently stored in the array queue.
   *
   * @return The number of elements currently stored in the array queue.
   */
  size_t size() const;

  /**
   * Obtains the front element of the array queue. It is illegal to access the
   * front element when the array queue is empty. The user of the API must check
   * the size() or empty() function prior to accessing the front element to
   * ensure that they will not read out of bounds.
   *
   * @return The front element.
   */
  ElementType& front();
  const ElementType& front() const;

  /**
   * Obtains the last element in the queue. Illegal to call when empty() is
   * true.
   *
   * @return The last element in the queue.
   */
  ElementType& back();
  const ElementType& back() const;

  /**
   * Obtains an element of the array queue given an index. It is illegal to
   * index this array queue out of bounds and the user of the API must check the
   * size() function prior to indexing this array queue to ensure that they will
   * not read out of bounds.
   *
   * @param index Requested index in range [0,size()-1]
   * @return The element.
   */
  ElementType& operator[](size_t index);

  /**
   * Obtains an element of the array queue given an index. It is illegal to
   * index this array queue out of bounds and the user of the API must check the
   * size() function prior to indexing this array queue to ensure that they will
   * not read out of bounds.
   *
   * @param index Requested index in range [0,size()-1]
   * @return The element.
   */
  const ElementType& operator[](size_t index) const;

  /**
   * Pushes an element onto the back of the array queue via copy or move
   * construction. It returns false if the array queue is full already and there
   * is no room for the elements. All iterators and references are unaffected.
   *
   * @param element The element to push onto the array queue.
   * @return true if the element is pushed successfully.
   */
  bool push(const ElementType& element);
  bool push(ElementType&& element);

  /**
   * Pushes an element onto the back of the array queue via copy or move
   * construction. If the array queue is full the front element is removed
   * to make room for the new element.
   *
   * @param element The element to push onto the array queue.
   */
  void kick_push(const ElementType& element);
  void kick_push(ElementType&& element);

  /**
   * Removes the front element from the array queue if the array queue is not
   * empty. Only iterators and references to the front of the queue are
   * invalidated.
   */
  void pop();

  /**
   * Removes the back element from the array queue if the array queue is not
   * empty. Only iterators and references to the back of the queue are
   * invalidated.
   */
  void pop_back();

  /**
   * Removes an element from the array queue given an index. It returns false if
   * the array queue contains fewer items than the index. All iterators and
   * references to elements before the removed one are unaffected. Iterators
   * and references to the removed element or any elements after it are
   * invalidated.
   *
   * @param index Requested index in range [0,size()-1]
   * @return true if the indexed element has been removed successfully.
   */
  bool remove(size_t index);

  /**
   * Constructs an element onto the back of the array queue. All iterators and
   * references are unaffected.
   *
   * @param The arguments to the constructor
   * @return true if the element is constructed successfully.
   */
  template<typename... Args>
  bool emplace(Args&&... args);

  /**
   * Removes all the elements of the queue.
   */
  void clear();

  /**
   * A template class that implements a forward iterator for the array queue.
   */
  template<typename ValueType>
  class ArrayQueueIterator {
   public:
    typedef ValueType value_type;
    typedef ValueType& reference;
    typedef ValueType* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    ArrayQueueIterator() = default;
    ArrayQueueIterator(
        ValueType *pointer, ValueType *base, size_t tail)
        : mPointer(pointer), mBase(base), mTail(tail) {}

    bool operator==(const ArrayQueueIterator& right) const {
      return (mPointer == right.mPointer);
    }

    bool operator!=(const ArrayQueueIterator& right) const {
      return (mPointer != right.mPointer);
    }

    ValueType& operator*() {
      return *mPointer;
    }

    ValueType *operator->() {
      return mPointer;
    }

    ArrayQueueIterator& operator++() {
      if (mPointer == (mBase + mTail)) {
        // Jump to end() if at tail
        mPointer = mBase + kCapacity;
      } else if (mPointer == (mBase + kCapacity - 1)) {
        // Wrap around in the memory
        mPointer = mBase;
      } else {
        mPointer++;
      }
      return *this;
    }

    ArrayQueueIterator operator++(int) {
      ArrayQueueIterator it(*this);
      operator++();
      return it;
    }

   private:
    //! Pointer of the iterator.
    ValueType *mPointer;

    //! The memory base address of this container.
    ValueType *mBase;

    //! The tail offset relative to the memory base address.
    size_t mTail;
  };

  /**
   * Forward iterator that points to some element in the container.
   */
  typedef ArrayQueueIterator<ElementType> iterator;
  typedef ArrayQueueIterator<const ElementType> const_iterator;

  /**
   * @return A forward iterator to the beginning.
   */
  typename ArrayQueue<ElementType, kCapacity>::iterator begin();
  typename ArrayQueue<ElementType, kCapacity>::const_iterator begin() const;
  typename ArrayQueue<ElementType, kCapacity>::const_iterator cbegin() const;

  /**
   * @return A forward iterator to the end.
   */
  typename ArrayQueue<ElementType, kCapacity>::iterator end();
  typename ArrayQueue<ElementType, kCapacity>::const_iterator end() const;
  typename ArrayQueue<ElementType, kCapacity>::const_iterator cend() const;

 private:
  /**
   * Storage for array queue elements. To avoid static initialization of
   * members, std::aligned_storage is used.
   */
  typename std::aligned_storage<sizeof(ElementType),
                                alignof(ElementType)>::type mData[kCapacity];

  /*
   * Initialize mTail to be (kCapacity-1). When an element is pushed in,
   * mHead and mTail will align. Also, this is consistent with
   * mSize = (mTail - mHead)%kCapacity + 1 for mSize > 0.
   */
  //! Index of the front element
  size_t mHead = 0;

  //! Index of the back element
  size_t mTail = kCapacity - 1;

  //! Number of elements in the array queue
  size_t mSize = 0;

  /**
   * Obtains a pointer to the underlying storage for the vector.
   *
   * @return A pointer to the storage used for elements in this vector.
   */
  ElementType *data();

  /**
   * Obtains a pointer to the underlying storage for the vector.
   *
   * @return A pointer to the storage used for elements in this vector.
   */
  const ElementType *data() const;

  /**
   * Converts relative index with respect to mHead to absolute index in the
   * storage array.
   *
   * @param index Relative index in range [0,size()-1]
   * @return The index of the storage array in range [0,kCapacity-1]
   */
  size_t relativeIndexToAbsolute(size_t index) const;

  /*
   * Pulls mHead to the next element in the array queue and decrements mSize
   * accordingly. It is illegal to call this function on an empty array queue.
   */
  void pullHead();

  /*
   * Pulls mTail to the previous element in the array queue and decrements mSize
   * accordingly. It is illegal to call this function on an empty array queue.
   */
  void pullTail();

  /*
   * Pushes mTail to the next available storage space and increments mSize
   * accordingly.
   *
   * @return true if the array queue is not full.
   */
  bool pushTail();
};

}  // namespace chre

#include "chre/util/array_queue_impl.h"

#endif  // CHRE_UTIL_ARRAY_QUEUE_H_
