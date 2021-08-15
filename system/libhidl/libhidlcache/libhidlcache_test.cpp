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

#define LOG_TAG "hidl-cache-test"

#include <android/hidl/allocator/1.0/IAllocator.h>
#include <android/hidl/memory/1.0/IMemory.h>
#include <android/hidl/memory/token/1.0/IMemoryToken.h>
#include <gtest/gtest.h>
#include <hidlcache/MemoryDealer.h>
#include <hidlcache/mapping.h>
#include <hidlmemory/HidlMemoryToken.h>
#include <hidlmemory/mapping.h>
#include "HidlMemoryCache.h"

#define EXPECT_OK(__ret__) EXPECT_TRUE(isOk(__ret__))

template <typename T>
static inline ::testing::AssertionResult isOk(const ::android::hardware::Return<T>& ret) {
    return ret.isOk() ? (::testing::AssertionSuccess() << ret.description())
                      : (::testing::AssertionFailure() << ret.description());
}

namespace android {

namespace hardware {
void HidlCacheWhiteBoxTest() {
    using ::android::hardware::HidlMemoryCache;
    using ::android::hardware::HidlMemoryToken;
    using ::android::hidl::allocator::V1_0::IAllocator;
    using ::android::hidl::memory::V1_0::IMemory;
    using ::android::hidl::memory::token::V1_0::IMemoryToken;
    using ::android::hidl::memory::block::V1_0::MemoryBlock;

    sp<IAllocator> ashmemAllocator;

    ashmemAllocator = IAllocator::getService("ashmem");
    ASSERT_NE(nullptr, ashmemAllocator.get());
    ASSERT_TRUE(ashmemAllocator->isRemote());  // allocator is always remote

    sp<HidlMemory> mem;
    EXPECT_OK(ashmemAllocator->allocate(1024, [&](bool success, const hidl_memory& _mem) {
        ASSERT_TRUE(success);
        mem = HidlMemory::getInstance(_mem);
    }));

    sp<IMemoryToken> token = new HidlMemoryToken(mem);

    MemoryBlock blk = {token, 0x200 /* size */, 0x100 /* offset */};
    sp<IMemoryToken> mtoken = blk.token;
    mtoken->get([&](const hidl_memory& mem) { sp<IMemory> memory = mapMemory(mem); });

    sp<HidlMemoryCache> cache = HidlMemoryCache::getInstance();
    EXPECT_FALSE(cache->cached(token));

    MemoryBlock blk2 = {token, 0x200 /* size */, 0x300 /* offset */};

    EXPECT_FALSE(cache->cached(token));

    {
        sp<IMemory> mem1 = cache->fetch(token);
        EXPECT_TRUE(cache->cached(token));
        EXPECT_NE(nullptr, cache->getCachedLocked(token).get());
        sp<IMemory> mem2 = cache->fetch(token);
        EXPECT_TRUE(cache->cached(token));
        EXPECT_NE(nullptr, cache->getCachedLocked(token).get());
    }
    EXPECT_FALSE(cache->cached(token));
    {
        sp<IMemory> mem1 = mapMemory(blk);
        EXPECT_TRUE(cache->cached(token));
        EXPECT_NE(nullptr, cache->getCachedLocked(token).get());
        uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(mem1->getPointer()));
        EXPECT_NE(nullptr, data);
    }
    {
        sp<IMemory> mem2 = mapMemory(blk);
        EXPECT_TRUE(cache->cached(token));
        EXPECT_NE(nullptr, cache->getCachedLocked(token).get());
    }
    EXPECT_FALSE(cache->cached(token));
    EXPECT_TRUE(cache->lock(token));
    EXPECT_TRUE(cache->cached(token));
    EXPECT_NE(nullptr, cache->getCachedLocked(token).get());
    EXPECT_TRUE(cache->unlock(token));
    EXPECT_FALSE(cache->cached(token));
}
}  // namespace hardware

class HidlCacheTest : public ::testing::Test {};

TEST_F(HidlCacheTest, TestAll) {
    hardware::HidlCacheWhiteBoxTest();
}

TEST_F(HidlCacheTest, MemoryDealer) {
    using ::android::hardware::HidlMemory;
    using ::android::hardware::hidl_memory;
    using ::android::hardware::HidlMemoryDealer;
    using ::android::hidl::allocator::V1_0::IAllocator;
    using ::android::hidl::memory::block::V1_0::MemoryBlock;

    sp<IAllocator> ashmemAllocator;

    ashmemAllocator = IAllocator::getService("ashmem");
    sp<HidlMemory> m1;
    sp<HidlMemory> m2;
    // test MemoryDealer
    EXPECT_OK(ashmemAllocator->allocate(2048, [&m1](bool success, const hidl_memory& mem) {
        ASSERT_TRUE(success);
        m1 = HidlMemory::getInstance(mem);
    }));

    EXPECT_OK(ashmemAllocator->allocate(4096, [&m2](bool success, const hidl_memory& mem) {
        ASSERT_TRUE(success);
        m2 = HidlMemory::getInstance(mem);
    }));

    sp<HidlMemoryDealer> dealer;

    // m1 does not statisfy the alignment requirement and should fail.
    dealer = HidlMemoryDealer::getInstance(*m1);
    EXPECT_EQ(nullptr, dealer.get());

    dealer = HidlMemoryDealer::getInstance(*m2);
    EXPECT_NE(nullptr, dealer.get());

    EXPECT_EQ(dealer->heap()->getSize(), 4096ULL);
    MemoryBlock blk = dealer->allocate(1024);
    EXPECT_TRUE(HidlMemoryDealer::isOk(blk));
    MemoryBlock blk2 = dealer->allocate(2048);
    EXPECT_TRUE(HidlMemoryDealer::isOk(blk2));

    MemoryBlock blk3 = dealer->allocate(2048);
    EXPECT_FALSE(HidlMemoryDealer::isOk(blk3));
    dealer->deallocate(blk2.offset);
    blk3 = dealer->allocate(2048);
    EXPECT_TRUE(HidlMemoryDealer::isOk(blk3));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace android