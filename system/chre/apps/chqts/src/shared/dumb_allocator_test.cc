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

#include <gtest/gtest.h>

static constexpr size_t kAllocSize = 128;
static constexpr size_t kSlotCount = 5;

typedef nanoapp_testing::DumbAllocator<kAllocSize, kSlotCount> DA;

static void ExpectGoodAlloc(const DA &da, const void *ptr) {
  EXPECT_NE(nullptr, ptr);
  EXPECT_TRUE(da.contains(ptr));
}

TEST(DumbAllocatorTests, SimpleAlloc) {
  DA da;
  void *ptr = da.alloc(kAllocSize);
  ExpectGoodAlloc(da, ptr);
  EXPECT_TRUE(da.free(ptr));
}

TEST(DumbAllocatorTests, AllocsAfterFree) {
  DA da;
  void *ptrs[kSlotCount];
  for (size_t i = 0; i < kSlotCount; i++) {
    ptrs[i] = da.alloc(kAllocSize);
    ExpectGoodAlloc(da, ptrs[i]);
    // Also confirm we're not doubly allocating the same pointer.
    for (size_t j = 0; j < i; j++) {
      EXPECT_NE(ptrs[j], ptrs[i]);
    }
  }
  // Out of slots, allocation should fail.
  EXPECT_EQ(nullptr, da.alloc(kAllocSize));

  constexpr size_t kFreeIndex = kSlotCount / 2;
  EXPECT_TRUE(da.free(ptrs[kFreeIndex]));
  ptrs[kFreeIndex] = nullptr;

  // Now our allocation should succeed.
  void *newPtr = da.alloc(kAllocSize);
  ExpectGoodAlloc(da, newPtr);
  for (size_t i = 0; i < kSlotCount; i++) {
    EXPECT_NE(newPtr, ptrs[i]);
  }
}

TEST(DumbAllocatorTests, ContainsIsFalseForBadPtrs) {
  DA da;
  uint8_t *ptr = static_cast<uint8_t*>(da.alloc(kAllocSize));
  ASSERT_NE(nullptr, ptr);
  EXPECT_FALSE(da.contains(ptr - 1));
  EXPECT_FALSE(da.contains(ptr + 1));
  EXPECT_FALSE(da.contains(nullptr));
}

TEST(DumbAllocatorTests, FailLargeAllocations) {
  DA da;
  EXPECT_EQ(nullptr, da.alloc(kAllocSize + 1));
  EXPECT_EQ(nullptr, da.alloc(kAllocSize * 2));
}

TEST(DumbAllocatorTests, SucceedSmallAllocations) {
  DA da;
  ExpectGoodAlloc(da, da.alloc(kAllocSize - 1));
  ExpectGoodAlloc(da, da.alloc(1));
  ExpectGoodAlloc(da, da.alloc(0));
}
