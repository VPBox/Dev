/*
 * Copyright (C) 2007 The Android Open Source Project
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

#ifndef ANDROID_HIDL_MEMORY_DEALER_H
#define ANDROID_HIDL_MEMORY_DEALER_H

#include <android/hidl/memory/1.0/IMemory.h>
#include <android/hidl/memory/block/1.0/types.h>
#include <stdint.h>
#include <sys/types.h>
namespace android {
namespace hardware {

class SimpleBestFitAllocator;

// MemoryDealer allocates/deallocates blocks from a continuous memory region.
// It operates on size and offset and does not depend on any specific data types.
class MemoryDealer : public RefBase {
   public:
    /// Allocate a block with size. The allocated block is identified with an
    /// offset. For example:
    /// ssize_t K = dealer->allocateOffset(size);
    /// On success, K is positive and points to a subregion K ~ (K+size-1) in the heap.
    /// It's negative if the allocation fails.
    virtual ssize_t allocateOffset(size_t size);
    /// @param offset It points to the block that allocated with allocateOffset previously.
    virtual void deallocate(size_t offset);
    /// @param tag a string tag used to mark the dump message
    virtual void dump(const char* tag) const;

    // allocations are aligned to some value. return that value so clients can account for it.
    static size_t getAllocationAlignment();

    MemoryDealer(size_t size);
    virtual ~MemoryDealer();

   protected:
    SimpleBestFitAllocator* mAllocator;
};

// It extends the generic MemoryDealer and uses
//  - sp<IMemory> to represent the main memory region.
//  - MemoryBlock to represent the the block to allocate/deallocate
class HidlMemoryDealer : public MemoryDealer {
    using IMemory = ::android::hidl::memory::V1_0::IMemory;
    using IMemoryToken = ::android::hidl::memory::token::V1_0::IMemoryToken;
    using MemoryBlock = ::android::hidl::memory::block::V1_0::MemoryBlock;

   public:
    static bool isOk(const MemoryBlock& memblk);
    /// @param memory The memory size must align to 4096 bytes
    static sp<HidlMemoryDealer> getInstance(const hidl_memory& memory);
    virtual MemoryBlock allocate(size_t size);
    virtual sp<IMemory> heap();

   protected:
    /// @param heap It must be acquired with mapMemory(memory) with its
    /// argument corresponds to the 2nd argument passed to HidlMemoryDealer.
    HidlMemoryDealer(sp<IMemory> heap, const hidl_memory& memory);
    sp<IMemory> mHeap;
    sp<IMemoryToken> mToken;
};

};  // namespace hardware
};  // namespace android

#endif  // ANDROID_HIDL_MEMORY_DEALER_H
