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

#include <shared/dumb_allocator.h>

#include <shared/nano_string.h>

namespace nanoapp_testing {

DumbAllocatorBase::DumbAllocatorBase(size_t allocSize, size_t slotCount,
                                     uint8_t *rawMemory)
    : mAllocSize(allocSize),
      mSlotCount(slotCount),
      mRawMemory(rawMemory),
      mAllocatedSlots(0) {
  // We're not worried about runtime efficiency, since this is testing
  // code.  In case of an issue within the tests, though, we do want
  // to have consistent behavior.  Thus, we initialize this memory to
  // aid tracking problems.
  memset(mRawMemory, 0xCD, mSlotCount * mAllocSize);
}

void *DumbAllocatorBase::alloc(size_t bytes) {
  if (bytes > mAllocSize) {
    // Oversized for our allocator.
    return nullptr;
  }
  size_t slot = 0;
  for (uint32_t mask = 1; slot < mSlotCount; slot++, mask <<= 1) {
    if ((mAllocatedSlots & mask) == 0) {
      // This space is available, let's claim it.
      mAllocatedSlots |= mask;
      break;
    }
  }
  if (slot == mSlotCount) {
    // We're out of space.
    return nullptr;
  }
  return mRawMemory + (slot * mAllocSize);
}

bool DumbAllocatorBase::free(void *pointer) {
  size_t slot;
  if (!getSlot(pointer, &slot)) {
    return false;
  }
  mAllocatedSlots &= ~(1 << slot);
  return true;
}

bool DumbAllocatorBase::contains(const void *pointer) const {
  size_t slot;
  return getSlot(pointer, &slot);
}

bool DumbAllocatorBase::getSlot(const void *pointer, size_t *slot) const {
  const uint8_t *ptr = static_cast<const uint8_t *>(pointer);
  if (ptr < mRawMemory) {
    // Out of range.
    return false;
  }
  *slot = static_cast<size_t>(ptr - mRawMemory) / mAllocSize;
  if (*slot >= mSlotCount) {
    // Out of range.
    return false;
  }
  // Also confirm alignment.
  return ((mRawMemory + (*slot * mAllocSize)) == ptr);
}


}  // namespace nanoapp_testing
