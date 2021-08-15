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

#include "chre/util/singleton.h"

using chre::Singleton;

TEST(Singleton, Lifecycle) {
  EXPECT_FALSE(Singleton<int>::isInitialized());
  EXPECT_EQ(Singleton<int>::safeGet(), nullptr);

  Singleton<int>::init(0x1337);
  ASSERT_NE(Singleton<int>::get(), nullptr);
  EXPECT_TRUE(Singleton<int>::isInitialized());
  EXPECT_EQ(*Singleton<int>::get(), 0x1337);
  Singleton<int>::init(0xcafe);
  ASSERT_NE(Singleton<int>::get(), nullptr);
  EXPECT_TRUE(Singleton<int>::isInitialized());
  EXPECT_EQ(*Singleton<int>::get(), 0x1337);

  Singleton<int>::deinit();
  EXPECT_EQ(Singleton<int>::safeGet(), nullptr);
  EXPECT_FALSE(Singleton<int>::isInitialized());

  Singleton<int>::init(0xface);
  ASSERT_NE(Singleton<int>::get(), nullptr);
  EXPECT_TRUE(Singleton<int>::isInitialized());
  EXPECT_EQ(*Singleton<int>::get(), 0xface);
}
