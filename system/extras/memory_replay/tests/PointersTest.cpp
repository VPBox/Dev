/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <gtest/gtest.h>

#include "Pointers.h"

TEST(PointersTest, smoke) {
  Pointers pointers(1);

  pointers.Add(0x1234, reinterpret_cast<void*>(0xabcd));
  void* memory_pointer = pointers.Remove(0x1234);
  ASSERT_EQ(reinterpret_cast<void*>(0xabcd), memory_pointer);
}

TEST(PointersTest, readd_pointer) {
  Pointers pointers(1);

  pointers.Add(0x1234, reinterpret_cast<void*>(0xabcd));
  void* memory_pointer = pointers.Remove(0x1234);
  ASSERT_EQ(reinterpret_cast<void*>(0xabcd), memory_pointer);
  pointers.Add(0x1234, reinterpret_cast<void*>(0x5555));
  memory_pointer = pointers.Remove(0x1234);
  ASSERT_EQ(reinterpret_cast<void*>(0x5555), memory_pointer);
}


TEST(PointersTest, expect_collision) {
  Pointers pointers(2);

  // This assumes the simple hash being used will result in a collision
  // hitting the same entry.
  pointers.Add(0x1234, reinterpret_cast<void*>(0xabcd));
  pointers.Add(0x11234, reinterpret_cast<void*>(0xabcf));
  void* memory_pointer = pointers.Remove(0x11234);
  ASSERT_EQ(reinterpret_cast<void*>(0xabcf), memory_pointer);
  memory_pointer = pointers.Remove(0x1234);
  ASSERT_EQ(reinterpret_cast<void*>(0xabcd), memory_pointer);
}

TEST(PointersTest, multiple_add_removes) {
  Pointers pointers(4);

  pointers.Add(0x1234, reinterpret_cast<void*>(0xabcd));
  pointers.Add(0x1235, reinterpret_cast<void*>(0xabcf));
  pointers.Add(0x1236, reinterpret_cast<void*>(0xabc1));
  pointers.Add(0x1237, reinterpret_cast<void*>(0xabc2));

  void* memory_pointer = pointers.Remove(0x1236);
  ASSERT_EQ(reinterpret_cast<void*>(0xabc1), memory_pointer);

  pointers.Add(0x2349, reinterpret_cast<void*>(0x2abcd));

  memory_pointer = pointers.Remove(0x1234);
  ASSERT_EQ(reinterpret_cast<void*>(0xabcd), memory_pointer);
  memory_pointer = pointers.Remove(0x1237);
  ASSERT_EQ(reinterpret_cast<void*>(0xabc2), memory_pointer);

  pointers.Add(0x3500, reinterpret_cast<void*>(0x3abcd));

  memory_pointer = pointers.Remove(0x3500);
  ASSERT_EQ(reinterpret_cast<void*>(0x3abcd), memory_pointer);
  memory_pointer = pointers.Remove(0x2349);
  ASSERT_EQ(reinterpret_cast<void*>(0x2abcd), memory_pointer);
}

static void TestNoEntriesLeft() {
  Pointers pointers(1);

  // Even though we've requested only one pointer, we get more due
  // to the way the data is allocated.
  for (size_t i = 0; i <= pointers.max_pointers(); i++) {
    pointers.Add(0x1234 + i, reinterpret_cast<void*>(0xabcd + i));
  }
}

TEST(PointersTest_DeathTest, no_entries_left) {
  ASSERT_EXIT(TestNoEntriesLeft(), ::testing::ExitedWithCode(1), "");
}

static void TestFindNoPointer() {
  Pointers pointers(1);

  pointers.Remove(0x1234);
}

TEST(PointersTest_DeathTest, find_no_pointer) {
  ASSERT_EXIT(TestFindNoPointer(), ::testing::ExitedWithCode(1), "");
}

static void TestRemoveZeroValue() {
  Pointers pointers(1);

  void* memory = pointers.Remove(0);
  if (memory) {}
}

TEST(PointersTest_DeathTest, remove_zero_value) {
  ASSERT_EXIT(TestRemoveZeroValue(), ::testing::ExitedWithCode(1), "");
}
