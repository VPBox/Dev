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

#ifndef CHRE_UTIL_HEAP_H_
#define CHRE_UTIL_HEAP_H_

#include <cstddef>

namespace chre {

/**
 * An implementation of the heap data structure. To be used as an input
 * container, it must provide methods size(), swap() and operator[].
 * It is recommended to use FixedSizeVector or DynamicVector as the input
 * container.
 */

/**
 * Adds an element to the heap. The element must first be placed at the end
 * of the container.
 *
 * @param container The container that is used to store the elements.
 * @param compare The object that provides the custom ordering of the elements.
 */
template<typename ContainerType, typename CompareFunction>
void push_heap(ContainerType& container, const CompareFunction& compare);

/**
 * Removes the first element from the heap and adjusts the heap accordingly.
 * The element removed is temporarily placed at the end of the container. The
 * user must call the proper method to remove it.
 *
 * @param container The container that is used to store the elements.
 * @param compare The object that provides the custom ordering of the elements.
 */
template<typename ContainerType, typename CompareFunction>
void pop_heap(ContainerType& container, const CompareFunction& compare);

/**
 * Removes an element from the heap and adjusts the heap accordingly.
 * The element removed is temporarily placed at the end of the container. The
 * user must call the proper method to remove it. It is illegal to index the
 * element out of bounds.
 *
 * @param container The container that is used to store the elements.
 * @param index The index of the element to be removed from the heap
 * @param compare The object that provides the custom ordering of the elements.
 */
template<typename ContainerType, typename CompareFunction>
void remove_heap(ContainerType& container, size_t index,
                 const CompareFunction& compare);

}  // namespace chre

#include "chre/util/heap_impl.h"

#endif  // CHRE_UTIL_HEAP_H_
