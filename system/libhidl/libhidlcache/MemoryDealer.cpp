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

#define LOG_TAG "MemoryDealer"

#include <hidlcache/MemoryDealer.h>
#include <hidlmemory/HidlMemoryToken.h>
#include <hidlmemory/mapping.h>

#include <list>

#include <log/log.h>

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

using std::string;

namespace android {
namespace hardware {

class SimpleBestFitAllocator {
    enum { PAGE_ALIGNED = 0x00000001 };

   public:
    explicit SimpleBestFitAllocator(size_t size);
    ~SimpleBestFitAllocator();

    size_t allocate(size_t size, uint32_t flags = 0);
    status_t deallocate(size_t offset);
    size_t size() const;
    void dump(const char* tag) const;
    void dump(string& res, const char* tag) const;

    static size_t getAllocationAlignment() { return kMemoryAlign; }

   private:
    struct chunk_t {
        chunk_t(size_t start, size_t size) : start(start), size(size), free(1) {}
        size_t start;
        size_t size : 28;
        int free : 4;
    };
    using List = std::list<chunk_t*>;
    using Iterator = std::list<chunk_t*>::iterator;
    using IteratorConst = std::list<chunk_t*>::const_iterator;
    using Mutex = std::mutex;
    using Lock = std::lock_guard<Mutex>;

    ssize_t alloc(size_t size, uint32_t flags);
    chunk_t* dealloc(size_t start);
    void dump_l(const char* tag) const;
    void dump_l(string& res, const char* tag) const;

    static const int kMemoryAlign;
    mutable Mutex mLock;
    List mList;
    size_t mHeapSize;
};

MemoryDealer::MemoryDealer(size_t size) : mAllocator(new SimpleBestFitAllocator(size)) {}

MemoryDealer::~MemoryDealer() {
    delete mAllocator;
}

ssize_t MemoryDealer::allocateOffset(size_t size) {
    return mAllocator->allocate(size);
}

void MemoryDealer::deallocate(size_t offset) {
    mAllocator->deallocate(offset);
}

void MemoryDealer::dump(const char* tag) const {
    mAllocator->dump(tag);
}

size_t MemoryDealer::getAllocationAlignment() {
    return SimpleBestFitAllocator::getAllocationAlignment();
}

// align all the memory blocks on a cache-line boundary
const int SimpleBestFitAllocator::kMemoryAlign = 32;

SimpleBestFitAllocator::SimpleBestFitAllocator(size_t size) {
    size_t pagesize = getpagesize();
    mHeapSize = ((size + pagesize - 1) & ~(pagesize - 1));

    chunk_t* node = new chunk_t(0, mHeapSize / kMemoryAlign);
    mList.push_front(node);
}

SimpleBestFitAllocator::~SimpleBestFitAllocator() {
    while (mList.size() != 0) {
        chunk_t* removed = mList.front();
        mList.pop_front();
#ifdef __clang_analyzer__
        // Clang static analyzer gets confused in this loop
        // and generates a false positive warning about accessing
        // memory that is already freed.
        // Add an "assert" to avoid the confusion.
        LOG_ALWAYS_FATAL_IF(mList.front() == removed);
#endif
        delete removed;
    }
}

size_t SimpleBestFitAllocator::size() const {
    return mHeapSize;
}

size_t SimpleBestFitAllocator::allocate(size_t size, uint32_t flags) {
    Lock lock(mLock);
    ssize_t offset = alloc(size, flags);
    return offset;
}

status_t SimpleBestFitAllocator::deallocate(size_t offset) {
    Lock lock(mLock);
    chunk_t const* const freed = dealloc(offset);
    if (freed) {
        return NO_ERROR;
    }
    return NAME_NOT_FOUND;
}

ssize_t SimpleBestFitAllocator::alloc(size_t size, uint32_t flags) {
    if (size == 0) {
        return 0;
    }
    size = (size + kMemoryAlign - 1) / kMemoryAlign;
    size_t pagesize = getpagesize();

    Iterator free_chunk_p = mList.end();
    for (Iterator p = mList.begin(); p != mList.end(); p++) {
        chunk_t* cur = *p;
        int extra = 0;
        if (flags & PAGE_ALIGNED) extra = (-cur->start & ((pagesize / kMemoryAlign) - 1));

        // best fit
        if (cur->free && (cur->size >= (size + extra))) {
            if ((free_chunk_p == mList.end()) || (cur->size < (*free_chunk_p)->size)) {
                free_chunk_p = p;
            }
            if (cur->size == size) {
                break;
            }
        }
    }
    if (free_chunk_p != mList.end()) {
        chunk_t* free_chunk = *free_chunk_p;
        const size_t free_size = free_chunk->size;
        free_chunk->free = 0;
        free_chunk->size = size;
        if (free_size > size) {
            int extra = 0;
            if (flags & PAGE_ALIGNED)
                extra = (-free_chunk->start & ((pagesize / kMemoryAlign) - 1));
            if (extra) {
                chunk_t* split = new chunk_t(free_chunk->start, extra);
                free_chunk->start += extra;
                mList.insert(free_chunk_p, split);
            }

            ALOGE_IF(
                (flags & PAGE_ALIGNED) && ((free_chunk->start * kMemoryAlign) & (pagesize - 1)),
                "PAGE_ALIGNED requested, but page is not aligned!!!");

            const ssize_t tail_free = free_size - (size + extra);
            if (tail_free > 0) {
                chunk_t* split = new chunk_t(free_chunk->start + free_chunk->size, tail_free);
                mList.insert(++free_chunk_p, split);
            }
        }
        return (free_chunk->start) * kMemoryAlign;
    }
    return NO_MEMORY;
}

SimpleBestFitAllocator::chunk_t* SimpleBestFitAllocator::dealloc(size_t start) {
    start = start / kMemoryAlign;

    for (Iterator pos = mList.begin(); pos != mList.end(); pos++) {
        chunk_t* cur = *pos;
        if (cur->start == start) {
            LOG_FATAL_IF(cur->free, "block at offset 0x%08lX of size 0x%08lX already freed",
                         cur->start * kMemoryAlign, cur->size * kMemoryAlign);

            // merge freed blocks together
            chunk_t* freed = cur;
            cur->free = 1;
            do {
                if (pos != mList.begin()) {
                    pos--;
                    chunk_t* const p = *pos;
                    pos++;
                    if (p->free || !cur->size) {
                        freed = p;
                        p->size += cur->size;
                        pos = mList.erase(pos);
                        delete cur;
                        if (pos == mList.end()) break;
                    }
                }
                if (++pos == mList.end()) break;
                cur = *pos;
            } while (cur && cur->free);

#ifndef NDEBUG
            if (!freed->free) {
                dump_l("dealloc (!freed->free)");
            }
#endif
            LOG_FATAL_IF(!freed->free, "freed block at offset 0x%08lX of size 0x%08lX is not free!",
                         freed->start * kMemoryAlign, freed->size * kMemoryAlign);

            return freed;
        }
    }
    return nullptr;
}

void SimpleBestFitAllocator::dump(const char* tag) const {
    Lock lock(mLock);
    dump_l(tag);
}

void SimpleBestFitAllocator::dump_l(const char* tag) const {
    string result;
    dump_l(result, tag);
    ALOGD("%s", result.c_str());
}

void SimpleBestFitAllocator::dump(string& result, const char* tag) const {
    Lock lock(mLock);
    dump_l(result, tag);
}

void SimpleBestFitAllocator::dump_l(string& result, const char* tag) const {
    size_t size = 0;
    int32_t i = 0;
    const size_t SIZE = 256;
    char buffer[SIZE];
    snprintf(buffer, SIZE, "  %s (%p, size=%u)\n", tag, this, (unsigned int)mHeapSize);

    result.append(buffer);

    for (IteratorConst pos = mList.begin(); pos != mList.end(); pos++) {
        chunk_t const* cur = *pos;

        if (!cur->free) size += cur->size * kMemoryAlign;

        i++;
    }
    snprintf(buffer, SIZE, "  size allocated: %u (%u KB)\n", int(size), int(size / 1024));
    result.append(buffer);
}

bool HidlMemoryDealer::isOk(const MemoryBlock& memblk) {
    return memblk.token != nullptr;
}

sp<::android::hidl::memory::V1_0::IMemory> HidlMemoryDealer::heap() {
    return mHeap;
}

// The required heap size alignment is 4096 bytes
static const uint64_t kHeapSizeAlignment = (0x1ULL << 12);

sp<HidlMemoryDealer> HidlMemoryDealer::getInstance(const hidl_memory& mem) {
    uint64_t msk = (kHeapSizeAlignment - 1);
    if (mem.size() & msk || !(mem.size() & ~msk)) {
        ALOGE("size is not aligned to %x", static_cast<uint32_t>(kHeapSizeAlignment));
        return nullptr;
    }
    sp<IMemory> heap = mapMemory(mem);
    if (heap == nullptr) {
        ALOGE("fail to mapMemory");
        return nullptr;
    }
    return new HidlMemoryDealer(heap, mem);
}

HidlMemoryDealer::HidlMemoryDealer(sp<IMemory> heap, const hidl_memory& mem)
    : MemoryDealer(heap->getSize()),
      mHeap(heap),
      mToken(new HidlMemoryToken(HidlMemory::getInstance(mem))) {}

::android::hidl::memory::block::V1_0::MemoryBlock HidlMemoryDealer::allocate(size_t size) {
    MemoryBlock memblk = {nullptr, 0xFFFFFFFFULL, 0xFFFFFFFFULL};
    ssize_t offset = allocateOffset(size);
    if (offset >= 0) {
        memblk.token = mToken;
        memblk.size = size;
        memblk.offset = offset;
    }
    return memblk;
}

};  // namespace hardware
};  // namespace android
