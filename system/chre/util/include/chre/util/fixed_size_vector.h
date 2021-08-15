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

#ifndef CHRE_UTIL_FIXED_SIZE_VECTOR_H_
#define CHRE_UTIL_FIXED_SIZE_VECTOR_H_

#include <cstddef>
#include <type_traits>

#include "chre/util/non_copyable.h"

namespace chre {

template<typename ElementType, size_t kCapacity>
class FixedSizeVector : public NonCopyable {
 public:
  /**
   * Destructs the objects
   */
  ~FixedSizeVector();

  /**
   * @return A reference to the last element in the vector
   */
  ElementType& back();
  const ElementType& back() const;

  /**
   * @return A reference to the first element in the vector
   */
  ElementType& front();
  const ElementType& front() const;

  /**
   * Obtains a pointer to the underlying storage for the vector.
   *
   * @return A pointer to the storage used for elements in this vector.
   */
  ElementType *data();

  /**
   * Obtains a const pointer to the underlying storage for the vector.
   *
   * @return A const pointer to the storage used for elements in this vector.
   */
  const ElementType *data() const;

  /**
   * Obtains the number of elements currently stored in the static vector.
   *
   * @return The number of elements currently stored in the vector.
   */
  size_t size() const;

  /**
   * Obtains the maximum number of elements that can be stored in the static
   * vector.
   *
   * @return The maximum capacity of the vector as defined by the template
   * argument.
   */
  size_t capacity() const;

  /**
   * Determines whether the vector is empty or not.
   *
   * @return true if the vector is empty.
   */
  bool empty() const;

  /**
   * Determines whether the vector is full or not.
   *
   * @return true if the vector is full.
   */
  bool full() const;

  /**
   * Pushes an element onto the back of the vector. It is illegal to push an
   * item onto a full vector. The user of the API must check the return of the
   * full() function prior to pushing another element. All iterators and
   * references are unaffected.
   *
   * @param The element to push onto the vector.
   */
  void push_back(const ElementType& element);

  /**
   * Constructs an element onto the back of the vector. It is illegal to
   * construct an item onto a full vector. The user of the API must check the
   * return of the full() function prior to constructing another element. All
   * iterators and references are unaffected.
   *
   * @param The arguments to the constructor
   */
  template<typename... Args>
  void emplace_back(Args&&... args);

  /**
   * Erases the last element in the vector. Invalid to call on an empty vector.
   *
   * Invalidates any references to back() and end()/cend().
   */
  void pop_back();

  /**
   * Obtains an element of the vector given an index. It is illegal to index
   * this vector out of bounds and the user of the API must check the size()
   * function prior to indexing this vector to ensure that they will not read
   * out of bounds.
   *
   * @param The index of the element.
   * @return The element.
   */
  ElementType& operator[](size_t index);

  /**
   * Obtains a const element of the vector given an index. It is illegal to
   * index this vector out of bounds and the user of the API must check the
   * size() function prior to indexing this vector to ensure that they will not
   * read out of bounds.
   *
   * @param The index of the element.
   * @return The element.
   */
  const ElementType& operator[](size_t index) const;

  /**
   * Removes an element from the vector given an index. All elements after the
   * indexed one are moved forward one position. The destructor is invoked on
   * on the invalid item left at the end of the vector. The index passed in
   * must be less than the size() of the vector. If the index is greater than or
   * equal to the size no operation is performed. All iterators and references
   * to elements before the indexed one are unaffected.
   *
   * @param index The index to remove an element at.
   */
  void erase(size_t index);

  /**
   * Swaps the location of two elements stored in the vector. The indices
   * passed in must be less than the size() of the vector. If the index is
   * greater than or equal to the size, no operation is performed. All iterators
   * and references to these two indexed elements are invalidated.
   *
   * @param index0 The index of the first element
   * @param index1 The index of the second element
   */
  void swap(size_t index0, size_t index1);

  /**
   * Resizes the fixed size vector by default-constructing from the current
   * size() to the newly requested size. If the new size is smaller than the
   * current size(), the elements from the new size to the current size() are
   * destructed and the vector is shrunk. A resize operation cannot be performed
   * that is greater than kCapacity. This will result in an assertion failure
   * and a resize to kCapacity if assertions are disabled. All iterators and
   * references to elements before newSize are unaffected.
   *
   * @param newSize The new size of the vector.
   */
  void resize(size_t newSize);

  /**
   * Random-access iterator that points to some element in the container.
   */
  typedef ElementType* iterator;
  typedef const ElementType* const_iterator;

  /**
   * @return A random-access iterator to the beginning.
   */
  typename FixedSizeVector<ElementType, kCapacity>::iterator begin();
  typename FixedSizeVector<ElementType, kCapacity>::const_iterator begin() const;
  typename FixedSizeVector<ElementType, kCapacity>::const_iterator cbegin() const;

  /**
   * @return A random-access iterator to the end.
   */
  typename FixedSizeVector<ElementType, kCapacity>::iterator end();
  typename FixedSizeVector<ElementType, kCapacity>::const_iterator end() const;
  typename FixedSizeVector<ElementType, kCapacity>::const_iterator cend() const;

 private:
  //! Storage for vector elements. To avoid static initialization of members,
  //! std::aligned_storage is used.
  typename std::aligned_storage<sizeof(ElementType),
      alignof(ElementType)>::type mData[kCapacity];

  //! The number of elements in the vector. This will never be more than
  //! kCapacity.
  size_t mSize = 0;
};

}  // namespace chre

#include "chre/util/fixed_size_vector_impl.h"

#endif  // CHRE_UTIL_FIXED_SIZE_VECTOR_H_
