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

#include "chre/util/fixed_size_vector.h"

using chre::FixedSizeVector;

namespace {
constexpr int kMaxTestCapacity = 10;
int destructor_count[kMaxTestCapacity];

class Foo {
 public:
  ~Foo() {
    if (mValue >= 0) {
      destructor_count[mValue]++;
    }
  };
  void setValue(int value) {
    mValue = value;
  }

 private:
  int mValue = -1;
};
}

TEST(FixedSizeVector, EmptyWithCapacityWithDefault) {
  FixedSizeVector<int, 8> testVector;
  ASSERT_NE(testVector.data(), nullptr);
  ASSERT_EQ(testVector.size(), 0);
  ASSERT_EQ(testVector.capacity(), 8);
  ASSERT_TRUE(testVector.empty());
  ASSERT_FALSE(testVector.full());
}

TEST(FixedSizeVector, PushBackOneAndRead) {
  FixedSizeVector<int, 8> testVector;
  testVector.push_back(0x1337);
  ASSERT_NE(testVector.data(), nullptr);
  ASSERT_EQ(testVector.size(), 1);
  ASSERT_EQ(testVector.capacity(), 8);
  ASSERT_FALSE(testVector.empty());
  ASSERT_FALSE(testVector.full());
  ASSERT_EQ(testVector[0], 0x1337);
}

TEST(FixedSizeVector, PushBackUntilFullAndRead) {
  FixedSizeVector<int, 4> testVector;
  testVector.push_back(1000);
  testVector.push_back(2000);
  testVector.push_back(3000);
  testVector.push_back(4000);

  ASSERT_NE(testVector.data(), nullptr);
  ASSERT_TRUE(testVector.full());
  ASSERT_FALSE(testVector.empty());
  ASSERT_EQ(testVector.size(), 4);
  ASSERT_EQ(testVector[0], 1000);
  ASSERT_EQ(testVector[1], 2000);
  ASSERT_EQ(testVector[2], 3000);
  ASSERT_EQ(testVector[3], 4000);

  ASSERT_EQ(testVector.data()[0], 1000);
  ASSERT_EQ(testVector.data()[1], 2000);
  ASSERT_EQ(testVector.data()[2], 3000);
  ASSERT_EQ(testVector.data()[3], 4000);
}

TEST(FixedSizeVector, PushBackAndErase) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  vector.push_back(0xbeef);
  vector.push_back(0xface);

  vector.erase(1);
  ASSERT_EQ(vector[0], 0x1337);
  ASSERT_EQ(vector.data()[0], 0x1337);
  ASSERT_EQ(vector[1], 0xbeef);
  ASSERT_EQ(vector.data()[1], 0xbeef);
  ASSERT_EQ(vector[2], 0xface);
  ASSERT_EQ(vector.data()[2], 0xface);
  ASSERT_EQ(vector.size(), 3);
}

TEST(FixedSizeVector, PushBackAndPopBack) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  vector.push_back(0xbeef);
  vector.push_back(0xface);

  ASSERT_EQ(vector.size(), 4);
  ASSERT_EQ(vector.back(), 0xface);
  vector.pop_back();
  ASSERT_EQ(vector.size(), 3);
  ASSERT_EQ(vector.back(), 0xbeef);
  vector.pop_back();
  ASSERT_EQ(vector.size(), 2);
  ASSERT_EQ(vector.back(), 0xcafe);
  vector.pop_back();
  ASSERT_EQ(vector.size(), 1);
  ASSERT_EQ(vector.back(), 0x1337);
  vector.pop_back();
  ASSERT_EQ(vector.size(), 0);
}

TEST(FixedSizeVector, EraseDestructorCalled) {
  FixedSizeVector<Foo, 4> vector;
  for (size_t i = 0; i < 4; ++i) {
    vector.push_back(Foo());
    vector[i].setValue(i);
  }

  // last item before erase is '3'.
  vector.erase(1);
  EXPECT_EQ(0, destructor_count[0]);
  EXPECT_EQ(0, destructor_count[1]);
  EXPECT_EQ(0, destructor_count[2]);
  EXPECT_EQ(1, destructor_count[3]);

  // last item before erase is still '3'.
  vector.erase(2);
  EXPECT_EQ(0, destructor_count[0]);
  EXPECT_EQ(0, destructor_count[1]);
  EXPECT_EQ(0, destructor_count[2]);
  EXPECT_EQ(2, destructor_count[3]);

  // last item before erase is now '2'.
  vector.erase(0);
  EXPECT_EQ(0, destructor_count[0]);
  EXPECT_EQ(0, destructor_count[1]);
  EXPECT_EQ(1, destructor_count[2]);
  EXPECT_EQ(2, destructor_count[3]);
}

TEST(FixedSizeVectorDeathTest, SwapWithInvalidIndex) {
  FixedSizeVector<int, 4> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  EXPECT_DEATH(vector.swap(0, 2), "");
}

TEST(FixedSizeVectorDeathTest, SwapWithInvalidIndices) {
  FixedSizeVector<int, 4> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);
  EXPECT_DEATH(vector.swap(2, 3), "");
}

TEST(FixedSizeVector, Swap) {
  FixedSizeVector<int, 4> vector;
  vector.push_back(0x1337);
  vector.push_back(0xcafe);

  vector.swap(0, 1);
  EXPECT_EQ(vector[0], 0xcafe);
  EXPECT_EQ(vector[1], 0x1337);
}

TEST(FixedSizeVector, ResizeLarger) {
  FixedSizeVector<int, 4> vector;
  vector.resize(4);
  EXPECT_EQ(vector.size(), 4);
}

TEST(FixedSizeVector, ResizeSmaller) {
  destructor_count[0] = 0;

  FixedSizeVector<Foo, 4> vector;
  for (size_t i = 0; i < 3; i++) {
    vector.push_back(Foo());
    vector[i].setValue(0);
  }

  EXPECT_EQ(vector.size(), 3);
  EXPECT_EQ(destructor_count[0], 0);
  vector.resize(2);
  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(destructor_count[0], 1);
}

TEST(FixedSizeVector, Iterator) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  size_t index = 0;
  for (FixedSizeVector<int, 8>::iterator it = vector.begin();
       it != vector.end(); ++it) {
    EXPECT_EQ(vector[index++], *it);
  }

  FixedSizeVector<int, 8>::iterator it = vector.begin() + vector.size() - 1;
  EXPECT_EQ(vector[vector.size() - 1], *it);

  it = vector.begin() + vector.size();
  EXPECT_TRUE(it == vector.end());
}

TEST(FixedSizeVector, ConstIterator) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  size_t index = 0;
  for (FixedSizeVector<int, 8>::const_iterator cit = vector.cbegin();
       cit != vector.cend(); ++cit) {
    EXPECT_EQ(vector[index++], *cit);
  }

  FixedSizeVector<int, 8>::const_iterator cit =
      vector.cbegin() + vector.size() - 1;
  EXPECT_EQ(vector[vector.size() - 1], *cit);

  cit = vector.cbegin() + vector.size();
  EXPECT_TRUE(cit == vector.cend());
}

TEST(FixedSizeVector, IteratorAndPushBack) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  FixedSizeVector<int, 8>::iterator it_b = vector.begin();
  FixedSizeVector<int, 8>::iterator it_e = vector.end();

  vector.push_back(3);

  size_t index = 0;
  while (it_b != it_e) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}

TEST(FixedSizeVector, IteratorAndEmplaceBack) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  FixedSizeVector<int, 8>::iterator it_b = vector.begin();
  FixedSizeVector<int, 8>::iterator it_e = vector.end();

  vector.emplace_back(3);

  size_t index = 0;
  while (it_b != it_e) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}

TEST(FixedSizeVector, IteratorAndErase) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);

  FixedSizeVector<int, 8>::iterator it_b = vector.begin();

  vector.erase(2);

  size_t index = 0;
  while (index < 2) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}

TEST(FixedSizeVector, IteratorAndSwap) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  FixedSizeVector<int, 8>::iterator it_b = vector.begin();

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

TEST(FixedSizeVector, IteratorAndResize) {
  FixedSizeVector<int, 8> vector;
  vector.push_back(0);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  FixedSizeVector<int, 8>::iterator it_b = vector.begin();

  vector.resize(2);

  size_t index = 0;
  while (index < 2) {
    EXPECT_EQ(vector[index++], *it_b++);
  }
}
