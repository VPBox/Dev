/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_UTIL_DYNAMIC_VECTOR_BASE_H_
#define CHRE_UTIL_DYNAMIC_VECTOR_BASE_H_

#include <cstddef>

#include "chre/util/non_copyable.h"

namespace chre {

class DynamicVectorBase : public NonCopyable {
 protected:
  DynamicVectorBase() = default;

  /**
   * Move-constructs a dynamic vector from another. The other dynamic vector is
   * left in an empty state.
   *
   * @param other The other vector to move from.
   */
  DynamicVectorBase(DynamicVectorBase&& other);

  /**
   * Performs a reserve operation for DynamicVector when the underlying type is
   * trivial. See {@link DynamicVector::reserve} for further details.
   *
   * @param elementSize The size of the element used to determine the effective
   *        size of the underlying data.
   */
  bool doReserve(size_t newCapacity, size_t elementSize);

  /**
   * Performs a prepare for push operation for DynamicVector when the underlying
   * type is trivial. See {@link DynamicVector::prepareForPush} for further
   * details.
   *
   * @param elementSize The size of the element used to determine the effective
   *        size of the underlying data.
   */
  bool doPrepareForPush(size_t elementSize);

  /**
   * @return the next size of allocation to perform when growing the size of
   *         this vector. If no growth is required (mSize is less than
   *         mCapacity), the current capacity is returned.
   */
  size_t getNextGrowthCapacity() const;

  /*
   * Performs an erase operation for DynamicVector when the underlying type is
   * trivial. See {@link DynamicVector::erase} for further details.
   *
   * @param elementSize The size of the element used to determine the effective
   *        size of the underlying data.
   */
  void doErase(size_t index, size_t elementSize);

  /**
   * Performs a push back operation for DynamicVector when the underlying type
   * is trivial. See {@link DynamicVector::push_back} for further details.
   *
   * @param elementSize The size of the element used to determine the effective
   *        size of the underlying data.
   */
  bool doPushBack(const void *element, size_t elementSize);

  //! A pointer to the underlying data buffer.
  void *mData = nullptr;

  //! The current size of the vector, as in the number of elements stored.
  size_t mSize = 0;

  //! The current capacity of the vector, as in the maximum number of elements
  //! that can be stored.
  size_t mCapacity = 0;
};

}  // namespace chre

#endif  // CHRE_UTIL_DYNAMIC_VECTOR_BASE_H_
