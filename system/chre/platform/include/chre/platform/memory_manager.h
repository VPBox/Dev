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

#ifndef CHRE_PLATFORM_MEMORY_MANAGER_H_
#define CHRE_PLATFORM_MEMORY_MANAGER_H_

#include <cstddef>
#include <cstdint>

#include "chre/core/nanoapp.h"
#include "chre/util/non_copyable.h"

// This default value can be overridden in the variant-specific makefile.
#ifndef CHRE_MAX_ALLOCATION_BYTES
#define CHRE_MAX_ALLOCATION_BYTES 262144  // 256 * 1024
#endif

namespace chre {

/**
 * The MemoryManager keeps track of heap memory allocated/deallocated by all
 * nanoapps.
 *
 * TODO: Free memory space when nanoapps are unloaded.
 */
class MemoryManager : public NonCopyable {
 public:
  /**
   * Allocate heap memory in CHRE.
   *
   * @param app The pointer to the nanoapp requesting memory.
   * @param bytes The size in bytes to allocate.
   * @return the allocated memory pointer. nullptr if the allocation fails.
   */
  void *nanoappAlloc(Nanoapp *app, uint32_t bytes);

  /**
   * Free heap memory in CHRE.
   *
   * @param app The pointer to the nanoapp requesting memory free.
   * @param ptr The pointer to the memory to deallocate.
   */
  void nanoappFree(Nanoapp *app, void *ptr);

  /**
   * @return current total allocated memory in bytes.
   */
  size_t getTotalAllocatedBytes() const {
    return mTotalAllocatedBytes;
  }

  /**
   * @return peak total allocated memory in bytes.
   */
  size_t getPeakAllocatedBytes() const {
    return mPeakAllocatedBytes;
  }

  /**
   * @return current count of allocated memory spaces.
   */
  size_t getAllocationCount() const {
    return mAllocationCount;
  }

  /**
   * @return max total allocatable memory in bytes.
   */
  size_t getMaxAllocationBytes() const {
    return kMaxAllocationBytes;
  }

  /**
   * @return max allocatable memory counts.
   */
  size_t getMaxAllocationCount() const {
    return kMaxAllocationCount;
  }

  /**
   * Prints state in a string buffer. Must only be called from the context of
   * the main CHRE thread.
   *
   * @param buffer Pointer to the start of the buffer.
   * @param bufferPos Pointer to buffer position to start the print (in-out).
   * @param size Size of the buffer in bytes.
   */
  void logStateToBuffer(char *buffer, size_t *bufferPos,
                        size_t bufferSize) const;

 private:
  /**
   * Header to store allocation details for tracking.
   * We use a union to ensure proper memory alignment.
   */
  union AllocHeader {
    struct {
      //! The amount of memory in bytes allocated (not including header).
      uint32_t bytes;

      //! The ID of nanoapp requesting memory allocation.
      uint32_t instanceId;
    } data;

    //! Makes sure header is a multiple of the size of max_align_t
    max_align_t aligner;
  };

  //! The total allocated memory in bytes (not including header).
  size_t mTotalAllocatedBytes = 0;

  //! The peak allocated memory in bytes (not including header).
  size_t mPeakAllocatedBytes = 0;

  //! Stores total number of allocated memory spaces.
  size_t mAllocationCount = 0;

  //! The maximum allowable total allocated memory in bytes for all nanoapps.
  static constexpr size_t kMaxAllocationBytes = CHRE_MAX_ALLOCATION_BYTES;

  //! The maximum allowable count of memory allocations for all nanoapps.
  static constexpr size_t kMaxAllocationCount = (8 * 1024);

  /**
   * Called by nanoappAlloc to perform the appropriate call to memory alloc.
   *
   * The semantics are the same as nanoappAlloc.
   */
  void *doAlloc(Nanoapp *app, uint32_t size);

  /**
   * Called by nanoappFree to perform the appropriate call to memory free.
   *
   * The sematics are the same as nanoappFree.
   */
  void doFree(Nanoapp *app, void *ptr);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_MEMORY_MANAGER_H_
