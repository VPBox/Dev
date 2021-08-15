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

#ifndef _GTS_NANOAPPS_SHARED_DUMB_ALLOCATOR_H_
#define _GTS_NANOAPPS_SHARED_DUMB_ALLOCATOR_H_

#include <cstddef>
#include <cstdint>

namespace nanoapp_testing {

// Implementation Note: We chose the pattern of having DumbAllocatorBase to
// reduce the code duplication from multiple instances of DumbAllocator with
// different template parameters.
// See DumbAllocator below for usage and API documentation.
class DumbAllocatorBase {
 protected:
  DumbAllocatorBase(size_t allocSize, size_t slotCount, uint8_t *rawMemory);

  void *alloc(size_t bytes);
  bool free(void *ptr);
  bool contains(const void *ptr) const;

  static constexpr size_t MaxSlotCount() {
    // Our mAllocatedSlots is treated as a bit array, so we get 8 slots for
    // each byte it has.
    return (sizeof(mAllocatedSlots) * 8);
  }

 private:
  const size_t mAllocSize;
  const size_t mSlotCount;
  uint8_t * const mRawMemory;
  uint32_t mAllocatedSlots;

  bool getSlot(const void *ptr, size_t *slot) const;
};


/**
 * This dumb allocator is designed to allow us to easily get chunks of
 * memory without needing to go through heap allocation.  The idea is to
 * reduce our dependency on CHRE for some aspects of our tests.
 *
 * This allocator is non-reentrant.  It's also inefficient and a bad idea
 * for shipping code, but useful for reducing dependencies during testing.
 *
 * This will allow up to kSlotCount allocations of up to kAllocSize bytes
 * each, and costs (kSlotCount * kAllocSize) bytes of underlying storage.
 */
template<size_t kAllocSize, size_t kSlotCount>
class DumbAllocator : DumbAllocatorBase {
 public:
  DumbAllocator()
      : DumbAllocatorBase(kAllocSize, kSlotCount, mRawMemoryArray) {}

  /**
   * If "bytes" <= kAllocSize, and there are less than kSlotCount allocations,
   * return a valid pointer.  Otherwise, nullptr.
   *
   * Reminder this is non-reentrant.
   */
  void *alloc(size_t bytes) {
    return DumbAllocatorBase::alloc(bytes);
  }

  /**
   * If contains(ptr) is true, free the allocation and return true.
   * Otherwise, do nothing and return false.
   *
   * Reminder this is non-reentrant.
   */
  bool free(void *ptr) {
    return DumbAllocatorBase::free(ptr);
  }

  /**
   * If "ptr" was a non-null pointer returned from alloc() on this instance,
   * return true.  Otherwise, do nothing and return false.
   */
  bool contains(const void *ptr) const {
    return DumbAllocatorBase::contains(ptr);
  }

 private:
  uint8_t mRawMemoryArray[kAllocSize * kSlotCount];

  static_assert(kSlotCount <= MaxSlotCount(), "kSlotCount is too high");
};


}  // namespace nanoapp_testing

#endif  // _GTS_NANOAPPS_SHARED_DUMB_ALLOCATOR_H_
