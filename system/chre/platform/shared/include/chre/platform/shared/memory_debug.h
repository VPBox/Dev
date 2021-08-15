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

#ifndef CHRE_PLATFORM_SHARED_MEMORY_DEBUG_H_
#define CHRE_PLATFORM_SHARED_MEMORY_DEBUG_H_

/**
 * @file
 * Debug-only code that can be used to catch dynamic memory errors at runtime.
 * To use, replace calls to malloc/free (or their equivalents) with:
 *
 *  void *buf = debugAlloc(malloc, allocSize);
 *  ..
 *  debugFree(free, buf);
 */

#include <cstddef>
#include <cstdint>
#include <string.h>

#include "chre/platform/fatal_error.h"

namespace chre {

constexpr uint64_t kMagicAllocated = UINT64_C(0x997af173b998b686);
constexpr uint64_t kMagicFreed     = UINT64_C(0x1a16b89bf3d69842);
static_assert(sizeof(kMagicAllocated) == sizeof(kMagicFreed),
              "Trailer magic values need to be the same size");

constexpr size_t kMaxAllocSize = 8 * 1024 * 1024;  // 8 MiB
constexpr uint32_t kPreambleGuard = UINT32_C(0x16b89bf3);

struct alignas(alignof(max_align_t)) MemoryDebugPreamble {
  size_t allocSize;
  uint32_t guard;
};

typedef void *(MemoryAllocFunction)(size_t);
typedef void (MemoryFreeFunction)(void *);

/**
 * Allocate memory, and prepend + append debugging information to help detect
 * dynamic memory errors (double-free, buffer overflow, etc).
 *
 * @param allocFunc Function to use to allocate memory (e.g. malloc)
 * @param size Number of bytes to allocate
 *
 * @return Aligned memory buffer for caller to use directly; note that
 *         this pointer must be given to debugFree to release it
 */
inline void *debugAlloc(MemoryAllocFunction *allocFunc, size_t size) {
  constexpr size_t kDebugOverhead =
      sizeof(MemoryDebugPreamble) + sizeof(kMagicAllocated);

  void *mem = nullptr;
  if (size > kMaxAllocSize) {
    LOGE("Invalid allocation size %zu (max %zu)", size, kMaxAllocSize);
  } else {
    mem = allocFunc(size + kDebugOverhead);
    if (mem != nullptr) {
      // Prepend size of the allocation
      auto *preamble = static_cast<MemoryDebugPreamble *>(mem);
      preamble->allocSize = size;
      preamble->guard = kPreambleGuard;
      mem = static_cast<void *>(preamble + 1);

      // Append a guard
      uint8_t *trailer = static_cast<uint8_t *>(mem) + size;
      memcpy(trailer, &kMagicAllocated, sizeof(kMagicAllocated));
    }
  }

  return mem;
}

/**
 * Free memory previously allocated from debugAlloc, but first perform some
 * sanity checks, triggering ERR_FATAL if they fail.
 *
 * @param freeFunc Function to call to actually free memory (e.g. free)
 * @param pointer Pointer previously returned by debugAlloc
 */
inline void debugFree(MemoryFreeFunction *freeFunc, void *pointer) {
  if (pointer != nullptr) {
    auto *preamble = static_cast<MemoryDebugPreamble *>(pointer) - 1;
    if (preamble->allocSize > kMaxAllocSize) {
      FATAL_ERROR("Invalid allocation size %zu", preamble->allocSize);
    } else if (preamble->guard != kPreambleGuard) {
      FATAL_ERROR("Corruption of preamble detected");
    }

    uint8_t *trailer = static_cast<uint8_t *>(pointer) + preamble->allocSize;
    if (memcmp(trailer, &kMagicFreed, sizeof(kMagicFreed)) == 0) {
      FATAL_ERROR("Double-free detected");
    } else if (memcmp(trailer, &kMagicAllocated, sizeof(kMagicAllocated))
                   != 0) {
      FATAL_ERROR("Buffer overflow detected (or maybe double free)");
    }

    memcpy(trailer, &kMagicFreed, sizeof(kMagicFreed));
    freeFunc(preamble);
  }
}

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_MEMORY_DEBUG_H_
