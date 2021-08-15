/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/util/conditional_lock_guard.h"

using chre::ConditionalLockGuard;

class MockMutex {
 public:
  MockMutex() : mLockCount(0) {}

  void lock() {
    mLockCount++;
  }

  void unlock() {
    mLockCount--;
  }

  int getLockCount() {
    return mLockCount;
  }

 private:
  int mLockCount;
};

TEST(ConditionalLockGuard, LockUnlock) {
  MockMutex mutex;
  ASSERT_EQ(mutex.getLockCount(), 0);

  {
    ConditionalLockGuard<MockMutex> lock(mutex, true);
    EXPECT_EQ(mutex.getLockCount(), 1);
  }

  EXPECT_EQ(mutex.getLockCount(), 0);
}

TEST(ConditionalLockGuard, DontLock) {
  MockMutex mutex;
  ASSERT_EQ(mutex.getLockCount(), 0);

  {
    ConditionalLockGuard<MockMutex> lock(mutex, false);
    EXPECT_EQ(mutex.getLockCount(), 0);
  }

  EXPECT_EQ(mutex.getLockCount(), 0);
}
