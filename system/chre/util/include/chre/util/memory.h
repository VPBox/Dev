/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_UTIL_MEMORY_H_
#define CHRE_UTIL_MEMORY_H_

#include <cstddef>

namespace chre {

/**
 * Destroys count objects starting at first. This function is similar to
 * std::destroy_n.
 *
 * @param first Starting address of count objects to destroy
 * @param count The number of objects to destroy
 */
template<typename ElementType>
void destroy(ElementType *first, size_t count);

/**
 * Performs move assignment (dest = std::move(source)) if supported by
 * ElementType, otherwise copy assignment (dest = source).
 */
template<typename ElementType>
void moveOrCopyAssign(ElementType& dest, ElementType& source);

/**
 * Initializes a new block of memory by transferring objects from another block,
 * using memcpy if valid for the underlying type, or the move constructor if
 * available, or the copy constructor. This function is similar to
 * std::uninitialized_move_n.
 *
 * @param source The beginning of the data to transfer
 * @param count The number of elements to transfer
 * @param dest An uninitialized buffer to be populated with count elements
 */
template<typename ElementType>
void uninitializedMoveOrCopy(ElementType *source, size_t count,
                             ElementType *dest);

/**
 * Allocates memory for an object of size T and constructs the object in the
 * newly allocated object by forwarding the provided parameters.
 */
template<typename T, typename... Args>
T *memoryAlloc(Args&&... args);

}  // namespace chre

#include "chre/util/memory_impl.h"

#endif  // CHRE_UTIL_MEMORY_H
