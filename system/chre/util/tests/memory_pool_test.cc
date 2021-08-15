#include "gtest/gtest.h"

#include "chre/util/memory_pool.h"

#include <random>
#include <vector>

using chre::MemoryPool;

TEST(MemoryPool, ExhaustPool) {
  MemoryPool<int, 3> memoryPool;
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 3);
  EXPECT_NE(memoryPool.allocate(), nullptr);
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 2);
  EXPECT_NE(memoryPool.allocate(), nullptr);
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 1);
  EXPECT_NE(memoryPool.allocate(), nullptr);
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 0);
  EXPECT_EQ(memoryPool.allocate(), nullptr);
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 0);
}

TEST(MemoryPool, ExhaustPoolThenDeallocateOneAndAllocateOne) {
  MemoryPool<int, 3> memoryPool;

  // Exhaust the pool.
  int *element1 = memoryPool.allocate();
  int *element2 = memoryPool.allocate();
  int *element3 = memoryPool.allocate();

  // Perform some simple assignments. There is a chance we crash here if things
  // are not implemented correctly.
  *element1 = 0xcafe;
  *element2 = 0xbeef;
  *element3 = 0xface;

  // Free one element and then allocate another.
  memoryPool.deallocate(element1);
  EXPECT_EQ(memoryPool.getFreeBlockCount(), 1);
  element1 = memoryPool.allocate();
  EXPECT_NE(element1, nullptr);

  // Ensure that the pool remains exhausted.
  EXPECT_EQ(memoryPool.allocate(), nullptr);

  // Perform another simple assignment. There is a hope that this can crash if
  // the pointer returned is very bad (like nullptr).
  *element1 = 0xfade;

  // Verify that the values stored were not corrupted by the deallocate
  // allocate cycle.
  EXPECT_EQ(*element1, 0xfade);
  EXPECT_EQ(*element2, 0xbeef);
  EXPECT_EQ(*element3, 0xface);
}

/*
 * Pair an allocated pointer with the expected value that should be stored in
 * that location.
 */
struct AllocationExpectedValuePair {
  size_t *allocation;
  size_t expectedValue;
};

TEST(MemoryPool, ExhaustPoolThenRandomDeallocate) {
  // The number of times to allocate and deallocate in random order.
  const size_t kStressTestCount = 64;

  // Construct a memory pool and a vector to maintain a list of all allocations.
  const size_t kMemoryPoolSize = 64;
  MemoryPool<size_t, kMemoryPoolSize> memoryPool;
  std::vector<AllocationExpectedValuePair> allocations;

  for (size_t i = 0; i < kStressTestCount; i++) {
    // Exhaust the memory pool.
    for (size_t j = 0; j < kMemoryPoolSize; j++) {
      AllocationExpectedValuePair allocation = {
        .allocation = memoryPool.allocate(),
        .expectedValue = j,
      };

      *allocation.allocation = j;
      allocations.push_back(allocation);
    }

    // Seed a random number generator with the loop iteration so that order is
    // preserved across test runs.
    std::mt19937 randomGenerator(i);

    while (!allocations.empty()) {
      // Generate a number with a uniform distribution between zero and the number
      // of allocations remaining.
      std::uniform_int_distribution<> distribution(0, allocations.size() - 1);
      size_t deallocateIndex = distribution(randomGenerator);

      // Verify the expected value and free the allocation.
      EXPECT_EQ(*allocations[deallocateIndex].allocation,
                allocations[deallocateIndex].expectedValue);
      memoryPool.deallocate(allocations[deallocateIndex].allocation);

      // Remove the freed allocation from the allocation list.
      allocations.erase(allocations.begin() + deallocateIndex);
    }
  }
}

