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

#ifndef CHRE_UTIL_DYNAMIC_VECTOR_IMPL_H_
#define CHRE_UTIL_DYNAMIC_VECTOR_IMPL_H_

#include "chre/util/dynamic_vector.h"

#include <memory>
#include <new>
#include <utility>

#include "chre/util/container_support.h"
#include "chre/util/memory.h"

namespace chre {

template<typename ElementType>
DynamicVector<ElementType>::DynamicVector() {}

template<typename ElementType>
DynamicVector<ElementType>::DynamicVector(DynamicVector<ElementType>&& other)
    : DynamicVectorBase(std::move(other)) {}

template<typename ElementType>
DynamicVector<ElementType>::~DynamicVector() {
  clear();
  memoryFree(data());
}

template<typename ElementType>
DynamicVector<ElementType>& DynamicVector<ElementType>::operator=(
    DynamicVector<ElementType>&& other) {
  if (this != &other) {
    this->~DynamicVector();
    mData = other.mData;
    mSize = other.mSize;
    mCapacity = other.mCapacity;

    other.mData = nullptr;
    other.mSize = 0;
    other.mCapacity = 0;
  }

  return *this;
}

template <typename ElementType>
void DynamicVector<ElementType>::clear() {
  destroy(data(), mSize);
  mSize = 0;
}

template<typename ElementType>
ElementType *DynamicVector<ElementType>::data() {
  return static_cast<ElementType *>(mData);
}

template<typename ElementType>
const ElementType *DynamicVector<ElementType>::data() const {
  return static_cast<const ElementType *>(mData);
}

template<typename ElementType>
typename DynamicVector<ElementType>::size_type
    DynamicVector<ElementType>::size() const {
  return mSize;
}

template<typename ElementType>
typename DynamicVector<ElementType>::size_type
    DynamicVector<ElementType>::capacity() const {
  return mCapacity;
}

template<typename ElementType>
bool DynamicVector<ElementType>::empty() const {
  return (mSize == 0);
}

template<typename ElementType>
void DynamicVector<ElementType>::pop_back() {
  CHRE_ASSERT(!empty());
  erase(mSize - 1);
}

template<typename ElementType>
bool DynamicVector<ElementType>::push_back(const ElementType& element) {
  return doPushBack(element, typename std::is_trivial<ElementType>::type());
}

template<typename ElementType>
bool DynamicVector<ElementType>::doPushBack(const ElementType& element,
                                            std::true_type) {
  return DynamicVectorBase::doPushBack(static_cast<const void *>(&element),
                                       sizeof(ElementType));
}

template<typename ElementType>
bool DynamicVector<ElementType>::doPushBack(const ElementType& element,
                                            std::false_type) {
  bool spaceAvailable = prepareForPush();
  if (spaceAvailable) {
    new (&data()[mSize++]) ElementType(element);
  }

  return spaceAvailable;
}

template<typename ElementType>
bool DynamicVector<ElementType>::push_back(ElementType&& element) {
  bool spaceAvailable = prepareForPush();
  if (spaceAvailable) {
    new (&data()[mSize++]) ElementType(std::move(element));
  }

  return spaceAvailable;
}

template<typename ElementType>
template<typename... Args>
bool DynamicVector<ElementType>::emplace_back(Args&&... args) {
  bool spaceAvailable = prepareForPush();
  if (spaceAvailable) {
    new (&data()[mSize++]) ElementType(std::forward<Args>(args)...);
  }

  return spaceAvailable;
}

template<typename ElementType>
ElementType& DynamicVector<ElementType>::operator[](size_type index) {
  CHRE_ASSERT(index < mSize);
  return data()[index];
}

template<typename ElementType>
const ElementType& DynamicVector<ElementType>::operator[](size_type index)
    const {
  CHRE_ASSERT(index < mSize);
  return data()[index];
}

template<typename ElementType>
bool DynamicVector<ElementType>::operator==(const DynamicVector<ElementType>& other)
    const {
  bool vectorsAreEqual = (mSize == other.mSize);
  if (vectorsAreEqual) {
    for (size_type i = 0; i < mSize; i++) {
      if (!(data()[i] == other.data()[i])) {
        vectorsAreEqual = false;
        break;
      }
    }
  }

  return vectorsAreEqual;
}

template<typename ElementType>
bool DynamicVector<ElementType>::reserve(size_type newCapacity) {
  return doReserve(newCapacity, typename std::is_trivial<ElementType>::type());
}

template<typename ElementType>
bool DynamicVector<ElementType>::doReserve(size_type newCapacity,
                                           std::true_type) {
  return DynamicVectorBase::doReserve(newCapacity, sizeof(ElementType));
}

template<typename ElementType>
bool DynamicVector<ElementType>::doReserve(size_type newCapacity,
                                           std::false_type) {
  bool success = (newCapacity <= mCapacity);
  if (!success) {
    ElementType *newData = static_cast<ElementType *>(
        memoryAlloc(newCapacity * sizeof(ElementType)));
    if (newData != nullptr) {
      uninitializedMoveOrCopy(data(), mSize, newData);
      destroy(data(), mSize);
      memoryFree(data());
      mData = newData;
      mCapacity = newCapacity;
      success = true;
    }
  }

  return success;
}

template<typename ElementType>
bool DynamicVector<ElementType>::resize(size_type newSize) {
  // Remove elements from the back to minimize move operations.
  while (mSize > newSize) {
    pop_back();
  }

  bool success = true;
  while (success && mSize < newSize) {
    success = emplace_back();
  }

  return success;
}

template<typename ElementType>
bool DynamicVector<ElementType>::insert(size_type index,
                                        const ElementType& element) {
  bool inserted = prepareInsert(index);
  if (inserted) {
    new (&data()[index]) ElementType(element);
  }
  return inserted;
}

template<typename ElementType>
bool DynamicVector<ElementType>::insert(size_type index, ElementType&& element) {
  bool inserted = prepareInsert(index);
  if (inserted) {
    new (&data()[index]) ElementType(std::move(element));
  }
  return inserted;
}

template<typename ElementType>
bool DynamicVector<ElementType>::prepareInsert(size_type index) {
  // Insertions are not allowed to create a sparse array.
  CHRE_ASSERT(index <= mSize);

  // TODO: this can be optimized in the case where we need to grow the vector to
  // do the shift when transferring the values from the old array to the new.
  bool readyForInsert = (index <= mSize && prepareForPush());
  if (readyForInsert) {
    // If we aren't simply appending the new object, create an opening where
    // we'll insert it
    if (index < mSize) {
      // Make a duplicate of the last item in the slot where we're growing
      uninitializedMoveOrCopy(&data()[mSize - 1], 1, &data()[mSize]);
      // Shift all elements starting at index towards the end
      for (size_type i = mSize - 1; i > index; i--) {
        moveOrCopyAssign(data()[i], data()[i - 1]);
      }

      data()[index].~ElementType();
    }

    mSize++;
  }

  return readyForInsert;
}

template<typename ElementType>
void DynamicVector<ElementType>::erase(size_type index) {
  CHRE_ASSERT(index < mSize);
  doErase(index, typename std::is_trivial<ElementType>::type());
}

template<typename ElementType>
void DynamicVector<ElementType>::doErase(size_type index, std::true_type) {
  DynamicVectorBase::doErase(index, sizeof(ElementType));
}

template<typename ElementType>
void DynamicVector<ElementType>::doErase(size_type index, std::false_type) {
  mSize--;
  for (size_type i = index; i < mSize; i++) {
    moveOrCopyAssign(data()[i], data()[i + 1]);
  }

  data()[mSize].~ElementType();
}

template<typename ElementType>
typename DynamicVector<ElementType>::size_type
    DynamicVector<ElementType>::find(const ElementType& element) const {
  // TODO: Consider adding iterator support and making this a free function.
  size_type i;
  for (i = 0; i < size(); i++) {
    if (data()[i] == element) {
      break;
    }
  }

  return i;
}

template<typename ElementType>
void DynamicVector<ElementType>::swap(size_type index0, size_type index1) {
  CHRE_ASSERT(index0 < mSize && index1 < mSize);
  if (index0 != index1) {
    typename std::aligned_storage<sizeof(ElementType),
        alignof(ElementType)>::type tempStorage;
    ElementType& temp = *reinterpret_cast<ElementType *>(&tempStorage);
    uninitializedMoveOrCopy(&data()[index0], 1, &temp);
    moveOrCopyAssign(data()[index0], data()[index1]);
    moveOrCopyAssign(data()[index1], temp);
  }
}

template<typename ElementType>
ElementType& DynamicVector<ElementType>::front() {
  CHRE_ASSERT(mSize > 0);
  return data()[0];
}

template<typename ElementType>
const ElementType& DynamicVector<ElementType>::front() const {
  CHRE_ASSERT(mSize > 0);
  return data()[0];
}

template<typename ElementType>
ElementType& DynamicVector<ElementType>::back() {
  CHRE_ASSERT(mSize > 0);
  return data()[mSize - 1];
}

template<typename ElementType>
const ElementType& DynamicVector<ElementType>::back() const {
  CHRE_ASSERT(mSize > 0);
  return data()[mSize - 1];
}

template<typename ElementType>
bool DynamicVector<ElementType>::prepareForPush() {
  return doPrepareForPush(typename std::is_trivial<ElementType>::type());
}

template<typename ElementType>
bool DynamicVector<ElementType>::doPrepareForPush(std::true_type) {
  return DynamicVectorBase::doPrepareForPush(sizeof(ElementType));
}

template<typename ElementType>
bool DynamicVector<ElementType>::doPrepareForPush(std::false_type) {
  return reserve(getNextGrowthCapacity());
}

template<typename ElementType>
typename DynamicVector<ElementType>::iterator
    DynamicVector<ElementType>::begin() {
  return data();
}

template<typename ElementType>
typename DynamicVector<ElementType>::iterator
    DynamicVector<ElementType>::end() {
  return (data() + mSize);
}

template<typename ElementType>
typename DynamicVector<ElementType>::const_iterator
    DynamicVector<ElementType>::begin() const {
  return cbegin();
}

template<typename ElementType>
typename DynamicVector<ElementType>::const_iterator
    DynamicVector<ElementType>::end() const {
  return cend();
}

template<typename ElementType>
typename DynamicVector<ElementType>::const_iterator
    DynamicVector<ElementType>::cbegin() const {
  return data();
}

template<typename ElementType>
typename DynamicVector<ElementType>::const_iterator
    DynamicVector<ElementType>::cend() const {
  return (data() + mSize);
}

}  // namespace chre

#endif  // CHRE_UTIL_DYNAMIC_VECTOR_IMPL_H_
