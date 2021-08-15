#include "gtest/gtest.h"
#include "chre/util/heap.h"

#include <algorithm>
#include <array>

using chre::DynamicVector;
using chre::FixedSizeVector;

TEST(HeapDeathTest, PushHeapWhenEmpty) {
  DynamicVector<int> v;
  std::less<int> comp;
  EXPECT_DEATH(chre::push_heap(v, comp), "");
}

TEST(HeapDeathTest, PopHeapWhenEmpty) {
  DynamicVector<int> v;
  std::less<int> comp;
  EXPECT_DEATH(chre::pop_heap(v, comp), "");
}

TEST(HeapTest, NestedPushPopHeap) {
  DynamicVector<int> v;
  std::less<int> comp;

  // generate random test data
  const size_t MaxSize = 1000;
  std::array<int, MaxSize> array;
  std::array<int, MaxSize> array_sorted;
  std::srand(0xcafe);
  for (size_t i = 0; i < MaxSize; ++i) {
    array[i] = std::rand();
    array_sorted[i] = array[i];
  }

  // make sure the popped data is in the right order of various array sizes.
  for (size_t s = 1; s < MaxSize + 1; ++s) {
    for (size_t i = 0; i < s; ++i) {
      v.push_back(array[i]);
      chre::push_heap(v, comp);
    }

    std::sort(array_sorted.begin(), array_sorted.begin() + s, std::greater<int>());

    for (size_t i = 0; i < s; ++i) {
      chre::pop_heap(v, comp);
      EXPECT_EQ(array_sorted[i], v[v.size() - 1]);
      v.erase(v.size() - 1);
    }
  }
}

TEST(HeapDeathTest, RemoveHeapWithInvalidIndex) {
  DynamicVector<int> v;
  std::less<int> comp;

  v.push_back(0);
  chre::push_heap(v, comp);
  EXPECT_DEATH(chre::remove_heap(v, 1, comp), "");
}

TEST(HeapTest, NestedRemoveHeap) {
  DynamicVector<int> v;
  std::less<int> comp;

  // generate random test data
  const size_t MaxSize = 1000;
  std::array<int, MaxSize> array;
  std::array<int, MaxSize> array_sorted;
  std::srand(0xcafe);
  for (size_t i = 0; i < MaxSize; ++i) {
    array[i] = std::rand();
    array_sorted[i] = array[i];
  }

  for (size_t s = 1; s < MaxSize + 1; ++s) {
    for (size_t i = 0; i < s; ++i) {
      v.push_back(array[i]);
      chre::push_heap(v, comp);
    }

    std::sort(array_sorted.begin(), array_sorted.begin() + s, std::greater<int>());

    // randomly remove one
    chre::remove_heap(v, std::rand() % s, comp);
    int data = v[v.size() - 1];
    v.erase(v.size() - 1);

    for (size_t i = 0; i < s; ++i) {
      if (array_sorted[i] == data) {
        continue;
      }

      chre::pop_heap(v, comp);
      EXPECT_EQ(array_sorted[i], v[v.size() - 1]);
      v.erase(v.size() - 1);
    }
  }
}

TEST(HeapTest, FixedSizeVectorMinHeap) {
  FixedSizeVector<int, 16> v;

  for (size_t i = 0; i < 5; ++i) {
    v.push_back(i);
    chre::push_heap(v, std::greater<int>());
  }

  for (size_t i = 0; i < 5; ++i) {
    chre::pop_heap(v, std::greater<int>());
    EXPECT_EQ(i, v[v.size() - 1]);
    v.erase(v.size() - 1);
  }
}
