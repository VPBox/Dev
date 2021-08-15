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

#include "gtest/gtest.h"

#include "chre/util/dynamic_vector.h"
#include "chre/util/macros.h"

#include <stdint.h>

using chre::DynamicVector;

namespace {
constexpr int kMaxTestCapacity = 10;
int gDestructorCount[kMaxTestCapacity];

class Dummy {
 public:
  ~Dummy() {
    if (mValue >= 0) {
      gDestructorCount[mValue]++;
    }
  };
  void setValue(int value) {
    mValue = value;
  }
  int getValue() {
    return mValue;
  }

 private:
  int mValue = -1;
};

void resetDestructorCounts() {
  for (size_t i = 0; i < ARRAY_SIZE(gDestructorCount); i++) {
    gDestructorCount[i] = 0;
  }
}
}

TEST(DynamicVector, EmptyByDefault) {
  DynamicVector<int> vector;
  EXPECT_EQ(vector.data(), nullptr);
  EXPECT_TRUE(vector.empty());
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);
  vector.clear();
}

TEST(DynamicVector, PushBackAndRead) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0x1337));
  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector.capacity(), 1);
  EXPECT_EQ(vector.data(), &vector[0]);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 0x1337);
}

TEST(DynamicVector, PushBackReserveAndReadTrivialType) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.emplace_back(0x1337));
  ASSERT_TRUE(vector.push_back(0xface));
  int x = 0xcafe;
  ASSERT_TRUE(vector.push_back(std::move(x)));
  ASSERT_TRUE(vector.insert(vector.size(), 0xd00d));
  EXPECT_EQ(vector.size(), 4);
  EXPECT_EQ(vector.capacity(), 4);
  EXPECT_EQ(vector[0], 0x1337);
  EXPECT_EQ(vector[1], 0xface);
  EXPECT_EQ(vector[2], 0xcafe);
  EXPECT_EQ(vector[3], 0xd00d);

  ASSERT_TRUE(vector.reserve(8));
  EXPECT_EQ(vector.size(), 4);
  EXPECT_EQ(vector.capacity(), 8);
  EXPECT_EQ(vector[0], 0x1337);
  EXPECT_EQ(vector[1], 0xface);
  EXPECT_EQ(vector[2], 0xcafe);
  EXPECT_EQ(vector[3], 0xd00d);
}

TEST(DynamicVector, CompareEqual) {
  DynamicVector<int> lhs;
  ASSERT_TRUE(lhs.push_back(0x1337));
  ASSERT_TRUE(lhs.push_back(0xface));
  DynamicVector<int> rhs;
  ASSERT_TRUE(rhs.push_back(0x1337));
  ASSERT_TRUE(rhs.push_back(0xface));

  ASSERT_EQ(lhs, rhs); // equal vectors

  ASSERT_TRUE(lhs.push_back(0xb00c));
  ASSERT_FALSE(lhs == rhs); // different size

  ASSERT_TRUE(rhs.push_back(0xc00b));
  ASSERT_FALSE(lhs == rhs); // equal size different elements
}

constexpr int kConstructedMagic = 0xdeadbeef;

class MovableButNonCopyable : public chre::NonCopyable {
 public:
  MovableButNonCopyable(int value) : mValue(value) {}

  MovableButNonCopyable(MovableButNonCopyable&& other) {
    mValue = other.mValue;
    other.mValue = -1;
  }

  MovableButNonCopyable& operator=(MovableButNonCopyable&& other) {
    assert(mMagic == kConstructedMagic);
    mValue = other.mValue;
    other.mValue = -1;
    return *this;
  }

  int getValue() const {
    return mValue;
  }

 private:
  int mMagic = kConstructedMagic;
  int mValue;
};

TEST(DynamicVector, PushBackReserveAndReadMovableButNonCopyable) {
  DynamicVector<MovableButNonCopyable> vector;
  ASSERT_TRUE(vector.emplace_back(0x1337));
  ASSERT_TRUE(vector.emplace_back(0xface));
  MovableButNonCopyable mbnc(0xcafe);
  ASSERT_TRUE(vector.push_back(std::move(mbnc)));
  EXPECT_EQ(mbnc.getValue(), -1);
  MovableButNonCopyable mbnc2(0xd00d);
  ASSERT_TRUE(vector.insert(vector.size(), std::move(mbnc2)));
  EXPECT_EQ(mbnc2.getValue(), -1);

  ASSERT_TRUE(vector.reserve(8));
  EXPECT_EQ(vector[0].getValue(), 0x1337);
  EXPECT_EQ(vector[1].getValue(), 0xface);
  EXPECT_EQ(vector[2].getValue(), 0xcafe);
  EXPECT_EQ(vector[3].getValue(), 0xd00d);
  EXPECT_EQ(vector.size(), 4);
  EXPECT_EQ(vector.capacity(), 8);
}

class CopyableButNonMovable {
 public:
  CopyableButNonMovable(int value) : mValue(value) {}

  CopyableButNonMovable(const CopyableButNonMovable& other) {
    mValue = other.mValue;
  }

  CopyableButNonMovable& operator=(const CopyableButNonMovable& other) {
    assert(mMagic == kConstructedMagic);
    mValue = other.mValue;
    return *this;
  }

  CopyableButNonMovable(CopyableButNonMovable&& other) = delete;
  CopyableButNonMovable& operator=(CopyableButNonMovable&& other) = delete;

  int getValue() const {
    return mValue;
  }

 private:
  int mMagic = kConstructedMagic;
  int mValue;
};

TEST(DynamicVector, PushBackReserveAndReadCopyableButNonMovable) {
  DynamicVector<CopyableButNonMovable> vector;
  ASSERT_TRUE(vector.emplace_back(0x1337));
  ASSERT_TRUE(vector.emplace_back(0xface));
  CopyableButNonMovable cbnm(0xcafe);
  ASSERT_TRUE(vector.push_back(cbnm));
  CopyableButNonMovable cbnm2(0xd00d);
  ASSERT_TRUE(vector.insert(vector.size(), cbnm2));

  ASSERT_TRUE(vector.reserve(8));
  EXPECT_EQ(vector[0].getValue(), 0x1337);
  EXPECT_EQ(vector[1].getValue(), 0xface);
  EXPECT_EQ(vector[2].getValue(), 0xcafe);
  EXPECT_EQ(vector[3].getValue(), 0xd00d);
  EXPECT_EQ(vector.size(), 4);
  EXPECT_EQ(vector.capacity(), 8);
}

class MovableAndCopyable {
 public:
  MovableAndCopyable(int value) : mValue(value) {}

  MovableAndCopyable(const MovableAndCopyable& other) {
    mValue = other.mValue;
  }

  MovableAndCopyable(MovableAndCopyable&& other) {
    // The move constructor multiplies the value by 2 so that we can see that it
    // was used
    mValue = other.mValue * 2;
  }

  MovableAndCopyable& operator=(const MovableAndCopyable& other) {
    assert(mMagic == kConstructedMagic);
    mValue = other.mValue;
    return *this;
  }

  MovableAndCopyable& operator=(MovableAndCopyable&& other) {
    assert(mMagic == kConstructedMagic);
    mValue = other.mValue * 2;
    other.mValue = -1;
    return *this;
  }

  int getValue() const {
    return mValue;
  }

 private:
  int mMagic = kConstructedMagic;
  int mValue;
};

TEST(DynamicVector, ReservePrefersMove) {
  // Ensure that preference is given to std::move in reserve()
  DynamicVector<MovableAndCopyable> vector;

  // Reserve enough space for the first two elements.
  ASSERT_TRUE(vector.reserve(2));
  ASSERT_TRUE(vector.emplace_back(1000));
  ASSERT_TRUE(vector.emplace_back(2000));

  // Reserve more than enough space causing a move to be required.
  ASSERT_TRUE(vector.reserve(4));

  // Move on this type results in a multiplication by 2. Verify that all
  // elements have been multiplied by 2.
  EXPECT_EQ(vector[0].getValue(), 2000);
  EXPECT_EQ(vector[1].getValue(), 4000);
}

/**
 * A simple test helper object to count number of construction and destructions.
 */
class Foo {
 public:
  /**
   * Construct an object storing a simple integer. Increment the number of
   * objects that have been constructed of this type.
   */
  Foo(int value) : value(value) {
    sConstructedCounter++;
  }

  Foo(const Foo& other) {
    value = other.value;
    sConstructedCounter++;
  }

  Foo(Foo&& other) = delete;

  /**
   * Tear down the object, decrementing the number of objects that have been
   * constructed of this type.
   */
  ~Foo() {
    sConstructedCounter--;
  }

  //! The number of objects of this type that have been constructed.
  static ssize_t sConstructedCounter;

  //! The value stored in the object to verify the contents of this object after
  //! construction.
  int value;
};

//! Storage for the Foo reference counter.
ssize_t Foo::sConstructedCounter = 0;

TEST(DynamicVector, EmplaceBackAndDestruct) {
  Foo::sConstructedCounter = 0;
  {
    DynamicVector<Foo> vector;
    ASSERT_TRUE(vector.emplace_back(1000));
    ASSERT_TRUE(vector.emplace_back(2000));
    ASSERT_TRUE(vector.emplace_back(3000));
    ASSERT_TRUE(vector.emplace_back(4000));

    ASSERT_EQ(vector[0].value, 1000);
    ASSERT_EQ(vector[1].value, 2000);
    ASSERT_EQ(vector[2].value, 3000);
    ASSERT_EQ(vector[3].value, 4000);

    EXPECT_EQ(Foo::sConstructedCounter, 4);
  }

  EXPECT_EQ(Foo::sConstructedCounter, 0);
}

TEST(DynamicVector, InsertEmpty) {
  DynamicVector<int> vector;
  EXPECT_CHRE_ASSERT(EXPECT_FALSE(vector.insert(1, 0x1337)));

  // Insert to empty vector
  ASSERT_TRUE(vector.insert(0, 0x1337));
  EXPECT_EQ(vector[0], 0x1337);

  // Insert at end triggering grow
  ASSERT_EQ(vector.capacity(), 1);
  EXPECT_TRUE(vector.insert(1, 0xface));
  EXPECT_EQ(vector[0], 0x1337);
  EXPECT_EQ(vector[1], 0xface);

  // Insert at beginning triggering grow
  ASSERT_EQ(vector.capacity(), 2);
  EXPECT_TRUE(vector.insert(0, 0xcafe));
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector[1], 0x1337);
  EXPECT_EQ(vector[2], 0xface);

  // Insert at middle with spare capacity
  ASSERT_EQ(vector.capacity(), 4);
  EXPECT_TRUE(vector.insert(1, 0xdead));
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector[1], 0xdead);
  EXPECT_EQ(vector[2], 0x1337);
  EXPECT_EQ(vector[3], 0xface);

  // Insert at middle triggering grow
  ASSERT_EQ(vector.capacity(), 4);
  EXPECT_TRUE(vector.insert(2, 0xbeef));
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector[1], 0xdead);
  EXPECT_EQ(vector[2], 0xbeef);
  EXPECT_EQ(vector[3], 0x1337);
  EXPECT_EQ(vector[4], 0xface);

  // Insert at beginning with spare capacity
  ASSERT_EQ(vector.capacity(), 8);
  ASSERT_EQ(vector.size(), 5);
  EXPECT_TRUE(vector.insert(0, 0xabad));
  EXPECT_EQ(vector[0], 0xabad);
  EXPECT_EQ(vector[1], 0xcafe);
  EXPECT_EQ(vector[2], 0xdead);
  EXPECT_EQ(vector[3], 0xbeef);
  EXPECT_EQ(vector[4], 0x1337);
  EXPECT_EQ(vector[5], 0xface);

  // Insert at end with spare capacity
  ASSERT_EQ(vector.size(), 6);
  EXPECT_TRUE(vector.insert(vector.size(), 0xc0de));
  EXPECT_EQ(vector[0], 0xabad);
  EXPECT_EQ(vector[1], 0xcafe);
  EXPECT_EQ(vector[2], 0xdead);
  EXPECT_EQ(vector[3], 0xbeef);
  EXPECT_EQ(vector[4], 0x1337);
  EXPECT_EQ(vector[5], 0xface);
  EXPECT_EQ(vector[6], 0xc0de);
}

TEST(DynamicVector, PushBackInsertInMiddleAndRead) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0x1337));
  ASSERT_TRUE(vector.push_back(0xface));
  ASSERT_TRUE(vector.push_back(0xcafe));
  ASSERT_TRUE(vector.insert(1, 0xbeef));

  ASSERT_EQ(vector[0], 0x1337);
  ASSERT_EQ(vector[1], 0xbeef);
  ASSERT_EQ(vector[2], 0xface);
  ASSERT_EQ(vector[3], 0xcafe);
}

TEST(DynamicVector, PushBackAndErase) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0x1337));
  ASSERT_TRUE(vector.push_back(0xcafe));
  ASSERT_TRUE(vector.push_back(0xbeef));
  ASSERT_TRUE(vector.push_back(0xface));

  vector.erase(1);

  ASSERT_EQ(vector[0], 0x1337);
  ASSERT_EQ(vector[1], 0xbeef);
  ASSERT_EQ(vector[2], 0xface);
  ASSERT_EQ(vector.size(), 3);
}

TEST(DynamicVector, FindEmpty) {
  DynamicVector<int> vector;
  ASSERT_EQ(vector.find(0), 0);
}

TEST(DynamicVector, FindWithElements) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0x1337));
  ASSERT_TRUE(vector.push_back(0xcafe));
  ASSERT_TRUE(vector.push_back(0xbeef));

  ASSERT_EQ(vector.find(0x1337), 0);
  ASSERT_EQ(vector.find(0xcafe), 1);
  ASSERT_EQ(vector.find(0xbeef), 2);
  ASSERT_EQ(vector.find(1000), 3);
}

TEST(DynamicVector, EraseDestructorCalled) {
  resetDestructorCounts();

  DynamicVector<Dummy> vector;
  vector.reserve(4);
  for (size_t i = 0; i < 4; ++i) {
    vector.emplace_back();
    vector[i].setValue(i);
  }

  // last item before erase is '3'.
  vector.erase(1);
  EXPECT_EQ(0, gDestructorCount[0]);
  EXPECT_EQ(0, gDestructorCount[1]);
  EXPECT_EQ(0, gDestructorCount[2]);
  EXPECT_EQ(1, gDestructorCount[3]);

  // last item before erase is still '3'.
  vector.erase(2);
  EXPECT_EQ(0, gDestructorCount[0]);
  EXPECT_EQ(0, gDestructorCount[1]);
  EXPECT_EQ(0, gDestructorCount[2]);
  EXPECT_EQ(2, gDestructorCount[3]);

  // last item before erase is now '2'.
  vector.erase(0);
  EXPECT_EQ(0, gDestructorCount[0]);
  EXPECT_EQ(0, gDestructorCount[1]);
  EXPECT_EQ(1, gDestructorCount[2]);
  EXPECT_EQ(2, gDestructorCount[3]);
}

TEST(DynamicVector, Clear) {
  resetDestructorCounts();

  DynamicVector<Dummy> vector;
  vector.reserve(4);
  for (size_t i = 0; i < 4; ++i) {
    vector.emplace_back();
    vector[i].setValue(i);
  }

  vector.clear();
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 4);

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(gDestructorCount[i], 1);
  }
}

TEST(DynamicVectorDeathTest, SwapWithInvalidIndex) {
  DynamicVector<int> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  EXPECT_DEATH(vector.swap(0, 2), "");
}

TEST(DynamicVectorDeathTest, SwapWithInvalidIndices) {
  DynamicVector<int> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  EXPECT_DEATH(vector.swap(2, 3), "");
}

TEST(DynamicVector, Swap) {
  DynamicVector<int> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);

  vector.swap(0, 1);
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector[1], 0x1337);
}

TEST(DynamicVector, BackFront) {
  DynamicVector<int> vector;
  vector.push_back(0x1337);
  EXPECT_EQ(vector.front(), 0x1337);
  EXPECT_EQ(vector.back(), 0x1337);
  vector.push_back(0xcafe);
  EXPECT_EQ(vector.front(), 0x1337);
  EXPECT_EQ(vector.back(), 0xcafe);
  vector.erase(0);
  EXPECT_EQ(vector.front(), 0xcafe);
  EXPECT_EQ(vector.back(), 0xcafe);
}

TEST(DynamicVector, Iterator) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  size_t index = 0;
  for (DynamicVector<int>::iterator it = vector.begin();
       it != vector.end(); ++it) {
    EXPECT_EQ(vector[index++], *it);
  }

  DynamicVector<int>::iterator it = vector.begin() + vector.size() - 1;
  EXPECT_EQ(vector[vector.size() - 1], *it);

  it = vector.begin() + vector.size();
  EXPECT_TRUE(it == vector.end());
}

TEST(DynamicVector, ConstIterator) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  size_t index = 0;
  for (DynamicVector<int>::const_iterator cit = vector.cbegin();
       cit != vector.cend(); ++cit) {
    EXPECT_EQ(vector[index++], *cit);
  }

  DynamicVector<int>::const_iterator cit = vector.cbegin() + vector.size() - 1;
  EXPECT_EQ(vector[vector.size() - 1], *cit);

  cit = vector.cbegin() + vector.size();
  EXPECT_TRUE(cit == vector.cend());
}

TEST(DynamicVector, IteratorAndPushBack) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  size_t oldCapacity = vector.capacity();

  DynamicVector<int>::iterator it_b = vector.begin();
  DynamicVector<int>::iterator it_e = vector.end();

  vector.push_back(3);
  ASSERT_TRUE(oldCapacity == vector.capacity());

  size_t index = 0;
  for (; it_b != it_e; ++it_b) {
    EXPECT_EQ(vector[index++], *it_b);
  }
}

TEST(DynamicVector, IteratorAndEmplaceBack) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  size_t oldCapacity = vector.capacity();

  DynamicVector<int>::iterator it_b = vector.begin();
  DynamicVector<int>::iterator it_e = vector.end();

  vector.emplace_back(3);
  ASSERT_TRUE(oldCapacity == vector.capacity());

  size_t index = 0;
  for (; it_b != it_e; ++it_b) {
    EXPECT_EQ(vector[index++], *it_b);
  }
}

TEST(DynamicVector, IteratorAndReserve) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  size_t oldCapacity = vector.capacity();

  DynamicVector<int>::iterator it_b = vector.begin();
  DynamicVector<int>::iterator it_e = vector.end();

  vector.reserve(oldCapacity);
  ASSERT_TRUE(oldCapacity == vector.capacity());

  size_t index = 0;
  for (; it_b != it_e; ++it_b) {
    EXPECT_EQ(vector[index++], *it_b);
  }
}

TEST(DynamicVector, IteratorAndInsert) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  size_t oldCapacity = vector.capacity();

  DynamicVector<int>::iterator it_b = vector.begin();

  vector.insert(2, 3);
  ASSERT_TRUE(oldCapacity == vector.capacity());

  size_t index = 0;
  while (index < 2) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}

TEST(DynamicVector, IteratorAndErase) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  DynamicVector<int>::iterator it_b = vector.begin();

  vector.erase(2);

  size_t index = 0;
  while (index < 2) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}

TEST(DynamicVector, IteratorAndSwap) {
  DynamicVector<int> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  DynamicVector<int>::iterator it_b = vector.begin();

  vector.swap(1, 3);

  size_t index = 0;
  while (index < 4) {
    if (index != 1 && index != 3) {
      EXPECT_EQ(vector[index], *it_b);
    }
    index++;
    it_b++;
  }
}

TEST(DynamicVector, MoveConstruct) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0));
  ASSERT_TRUE(vector.push_back(1));
  ASSERT_TRUE(vector.push_back(2));

  DynamicVector<int> movedVector(std::move(vector));
  EXPECT_EQ(vector.data(), nullptr);
  EXPECT_NE(movedVector.data(), nullptr);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(movedVector.size(), 3);
  EXPECT_EQ(vector.capacity(), 0);
  EXPECT_EQ(movedVector.capacity(), 4);
}

TEST(DynamicVector, MoveAssignmentConstruct) {
  DynamicVector<int> vector;
  ASSERT_TRUE(vector.push_back(0));
  ASSERT_TRUE(vector.push_back(1));
  ASSERT_TRUE(vector.push_back(2));

  DynamicVector<int> movedVector;
  movedVector = std::move(vector);
  EXPECT_EQ(vector.data(), nullptr);
  EXPECT_NE(movedVector.data(), nullptr);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(movedVector.size(), 3);
  EXPECT_EQ(vector.capacity(), 0);
  EXPECT_EQ(movedVector.capacity(), 4);
}

TEST(DynamicVector, PrepareForPush) {
  DynamicVector<int> vector;
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);

  // Perform an initial prepareForPush operation which causes a size of one.
  ASSERT_TRUE(vector.prepareForPush());
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 1);
  ASSERT_TRUE(vector.push_back(0xcafe));
  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector.capacity(), 1);

  // Verify that it becomes larger
  ASSERT_TRUE(vector.prepareForPush());
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector.capacity(), 2);

  // The vector should not become any larger than necessary.
  ASSERT_TRUE(vector.prepareForPush());
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector.capacity(), 2);
}

// TODO: Add a test for when memory allocation returns nullptr.

TEST(DynamicVector, PopBack) {
  DynamicVector<int> vector;
  constexpr size_t kSize = 4;
  for (int i = 0; i < kSize; i++) {
    vector.push_back(i);
  }

  for (int i = kSize - 1; i >= 0; i--) {
    EXPECT_EQ(vector.back(), i);
    vector.pop_back();
  }
  EXPECT_TRUE(vector.empty());
}

/**
 * A test class to default construct an integer with an incrementing value.
 */
struct FancyInt {
  static int index;
  int value;

  FancyInt() : value(index++) {}
};

int FancyInt::index = 0;

TEST(DynamicVector, Resize) {
  DynamicVector<FancyInt> vector;
  ASSERT_TRUE(vector.resize(4));
  ASSERT_EQ(vector.size(), 4);

  EXPECT_EQ(vector[0].value, 0);
  EXPECT_EQ(vector[1].value, 1);
  EXPECT_EQ(vector[2].value, 2);
  EXPECT_EQ(vector[3].value, 3);

  ASSERT_TRUE(vector.resize(2));
  ASSERT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0].value, 0);
  EXPECT_EQ(vector[1].value, 1);

  ASSERT_TRUE(vector.resize(4));
  ASSERT_EQ(vector.size(), 4);

  EXPECT_EQ(vector[0].value, 0);
  EXPECT_EQ(vector[1].value, 1);
  EXPECT_EQ(vector[2].value, 4);
  EXPECT_EQ(vector[3].value, 5);
}
