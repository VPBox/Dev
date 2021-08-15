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

#ifndef CHRE_PLATFORM_SLPI_MEMORY_H_
#define CHRE_PLATFORM_SLPI_MEMORY_H_

#include <cstddef>

namespace chre {

/**
 * Memory allocation specifically using the big image heap.
 * The semantics are the same as malloc.
 */
void *memoryAllocBigImage(size_t size);

/**
 * Memory free from memory allocated using the big image heap.
 * The semantics are the same as free.
 */
void memoryFreeBigImage(void *pointer);

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_MEMORY_H_
