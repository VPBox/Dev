
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

#include "chre/util/non_copyable.h"
#include "chre/util/optional.h"

using chre::Optional;

class DestructorTestingObject {
 public:
  ~DestructorTestingObject() {
    if (valueToFlipWhenDestruct != nullptr) {
      *valueToFlipWhenDestruct = !*valueToFlipWhenDestruct;
    }
  }

  void setValueToFlipWhenDestruct(bool *value) {
    valueToFlipWhenDestruct = value;
  }

 private:
  bool *valueToFlipWhenDestruct = nullptr;
};

TEST(Optional, ShouldDestructContainedObject) {
  bool destructed = false;
  {
    Optional<DestructorTestingObject> object(DestructorTestingObject{});
    object.value().setValueToFlipWhenDestruct(&destructed);
  }
  EXPECT_TRUE(destructed);
}

TEST(Optional, NoValueByDefault) {
  Optional<int> myInt;
  EXPECT_FALSE(myInt.has_value());
}

TEST(Optional, NonDefaultValueByDefault) {
  Optional<int> myInt(0x1337);
  EXPECT_TRUE(myInt.has_value());
  EXPECT_EQ(*myInt, 0x1337);
}

TEST(Optional, NonDefaultMovedValueByDefault) {
  Optional<int> myInt(std::move(0x1337));
  EXPECT_TRUE(myInt.has_value());
  EXPECT_EQ(*myInt, 0x1337);
}

TEST(Optional, CopyConstruct) {
  Optional<int> myInt(0x1337);
  Optional<int> myNewInt(myInt);
  EXPECT_TRUE(myNewInt.has_value());
  EXPECT_EQ(*myNewInt, 0x1337);
}

TEST(Optional, CopyConstructConst) {
  const Optional<int> myInt(0x1337);
  Optional<int> myNewInt(myInt);
  EXPECT_TRUE(myNewInt.has_value());
  EXPECT_EQ(*myNewInt, 0x1337);
}

TEST(Optional, CopyAssignAndRead) {
  Optional<int> myInt;
  EXPECT_FALSE(myInt.has_value());
  myInt = 0x1337;
  EXPECT_EQ(*myInt, 0x1337);
  EXPECT_TRUE(myInt.has_value());
  myInt.reset();
  EXPECT_FALSE(myInt.has_value());
}

TEST(Optional, MoveAssignAndRead) {
  Optional<int> myInt;
  EXPECT_FALSE(myInt.has_value());
  myInt = std::move(0xcafe);
  EXPECT_TRUE(myInt.has_value());
  EXPECT_EQ(*myInt, 0xcafe);
}

TEST(Optional, OptionalMoveAssignAndRead) {
  Optional<int> myInt(0x1337);
  Optional<int> myMovedInt;
  EXPECT_FALSE(myMovedInt.has_value());
  myMovedInt = std::move(myInt);
  EXPECT_TRUE(myInt.has_value());
  EXPECT_TRUE(myMovedInt.has_value());
  EXPECT_EQ(*myMovedInt, 0x1337);
}

TEST(Optional, OptionalCopyAssignAndRead) {
  Optional<int> myInt(0x1337);
  Optional<int> myCopiedInt;
  EXPECT_FALSE(myCopiedInt.has_value());
  myCopiedInt = myInt;
  EXPECT_TRUE(myInt.has_value());
  EXPECT_TRUE(myCopiedInt.has_value());
  EXPECT_EQ(*myInt, 0x1337);
  EXPECT_EQ(*myCopiedInt, 0x1337);
}

static constexpr int kInvalidValue = -1;

class MovableButNonCopyable : public chre::NonCopyable {
 public:
  MovableButNonCopyable() = default;
  MovableButNonCopyable(int value) : mValue(value) {}
  MovableButNonCopyable(MovableButNonCopyable&& other) {
    mValue = other.mValue;
    other.mValue = kInvalidValue;
  }

  MovableButNonCopyable& operator=(MovableButNonCopyable&& other) {
    assert(mMagic == kConstructedMagic);
    mValue = other.mValue;
    other.mValue = kInvalidValue;
    return *this;
  }

  ~MovableButNonCopyable() {
    mMagic = kUninitializedMagic;
    mValue = kUninitializedMagic;
  }

  int getValue() const {
    return mValue;
  }

 private:
  static constexpr int kConstructedMagic = 0xfeedc0fe;
  static constexpr int kUninitializedMagic = 0xdeadbeef;

  int mMagic = kConstructedMagic;
  int mValue = kInvalidValue;
};

TEST(Optional, UninitializedAssignment) {
  constexpr int kValue1 = 0xd00d;
  constexpr int kValue2 = 0xcafe;
  MovableButNonCopyable transferee1(kValue1);
  MovableButNonCopyable transferee2(kValue2);

  Optional<MovableButNonCopyable> container;
  EXPECT_FALSE(container.has_value());

  container = std::move(transferee1);
  EXPECT_TRUE(container.has_value());
  EXPECT_EQ(container->getValue(), kValue1);
  EXPECT_EQ(transferee1.getValue(), kInvalidValue);

  container.reset();
  EXPECT_FALSE(container.has_value());

  container = std::move(transferee2);
  EXPECT_TRUE(container.has_value());
  EXPECT_EQ(container->getValue(), kValue2);
  EXPECT_EQ(transferee2.getValue(), kInvalidValue);
}

// TODO: should add some tests to cover the possible assignment outcomes between
// two Optional instances (e.g. assign one w/o value to one w/value, etc)
