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

#define LOG_TAG "HidlMemoryCache"
#include "HidlMemoryCache.h"
#include <android/hidl/memory/1.0/IMemory.h>
#include <android/hidl/memory/token/1.0/IMemoryToken.h>
#include <hidlmemory/mapping.h>
#include <sys/mman.h>
#include <utils/Log.h>

namespace android {
namespace hardware {

using IMemoryToken = ::android::hidl::memory::token::V1_0::IMemoryToken;
using IMemory = ::android::hidl::memory::V1_0::IMemory;

class MemoryDecorator : public virtual IMemory {
   public:
    MemoryDecorator(const sp<IMemory>& heap) : mHeap(heap) {}
    virtual ~MemoryDecorator() {}
    Return<void> update() override { return mHeap->update(); }
    Return<void> read() override { return mHeap->read(); }
    Return<void> updateRange(uint64_t start, uint64_t length) override {
        return mHeap->updateRange(start, length);
    }
    Return<void> readRange(uint64_t start, uint64_t length) override {
        return mHeap->readRange(start, length);
    }
    Return<void> commit() override { return mHeap->commit(); }

    Return<void*> getPointer() override { return mHeap->getPointer(); }
    Return<uint64_t> getSize() override { return mHeap->getSize(); }

   protected:
    sp<IMemory> mHeap;
};

class MemoryCacheable : public virtual MemoryDecorator {
   public:
    MemoryCacheable(const sp<IMemory>& heap, sp<IMemoryToken> key)
        : MemoryDecorator(heap), mKey(key) {}
    virtual ~MemoryCacheable() { HidlMemoryCache::getInstance()->flush(mKey); }

   protected:
    sp<IMemoryToken> mKey;
};

class MemoryBlockImpl : public virtual IMemory {
   public:
    MemoryBlockImpl(const sp<IMemory>& heap, uint64_t size, uint64_t offset)
        : mHeap(heap), mSize(size), mOffset(offset), mHeapSize(heap->getSize()) {}
    bool validRange(uint64_t start, uint64_t length) {
        return (start + length <= mSize) && (start + length >= start) &&
               (mOffset + mSize <= mHeapSize);
    }
    Return<void> readRange(uint64_t start, uint64_t length) override {
        if (!validRange(start, length)) {
            ALOGE("IMemoryBlock::readRange: out of range");
            return Void();
        }
        return mHeap->readRange(mOffset + start, length);
    }
    Return<void> updateRange(uint64_t start, uint64_t length) override {
        if (!validRange(start, length)) {
            ALOGE("IMemoryBlock::updateRange: out of range");
            return Void();
        }
        return mHeap->updateRange(mOffset + start, length);
    }
    Return<void> read() override { return this->readRange(0, mSize); }
    Return<void> update() override { return this->updateRange(0, mSize); }
    Return<void> commit() override { return mHeap->commit(); }
    Return<uint64_t> getSize() override { return mSize; }
    Return<void*> getPointer() override {
        void* p = mHeap->getPointer();
        return (static_cast<char*>(p) + mOffset);
    }

   protected:
    sp<IMemory> mHeap;
    uint64_t mSize;
    uint64_t mOffset;
    uint64_t mHeapSize;
};

sp<HidlMemoryCache> HidlMemoryCache::getInstance() {
    static sp<HidlMemoryCache> instance = new HidlMemoryCache();
    return instance;
}

sp<IMemory> HidlMemoryCache::fillLocked(const sp<IMemoryToken>& key) {
    sp<IMemory> memory = nullptr;
    Return<void> ret = key->get(
        [&](const hidl_memory& mem) { memory = new MemoryCacheable(mapMemory(mem), key); });
    if (!ret.isOk()) {
        ALOGE("HidlMemoryCache::fill: cannot IMemoryToken::get.");
        return nullptr;
    }
    mCached[key] = memory;
    return memory;
}

sp<IMemory> HidlMemoryCache::map(const MemoryBlock& memblk) {
    sp<IMemoryToken> token = memblk.token;
    sp<IMemory> heap = fetch(token);
    if (heap == nullptr) {
        return nullptr;
    }
    return new MemoryBlockImpl(heap, memblk.size, memblk.offset);
}

}  // namespace hardware
}  // namespace android
