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

#include "chre/platform/memory_manager.h"
#include "chre/platform/memory.h"
#include "chre/platform/log.h"

using chre::MemoryManager;
using chre::Nanoapp;

namespace {
struct node {
  node *next;
};
}

TEST(MemoryManager, DefaultTotalMemoryAllocatedIsZero) {
  MemoryManager manager;
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
}

TEST(MemoryManager, BasicAllocationFree) {
  MemoryManager manager;
  Nanoapp app;
  void *ptr = manager.nanoappAlloc(&app, 1);
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 1);
  EXPECT_EQ(manager.getAllocationCount(), 1);
  manager.nanoappFree(&app, ptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
  EXPECT_EQ(manager.getAllocationCount(), 0);
}

TEST(MemoryManager, NullPointerFree) {
  MemoryManager manager;
  Nanoapp app;
  manager.nanoappFree(&app, nullptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
  EXPECT_EQ(manager.getAllocationCount(), 0);
}

TEST(MemoryManager, ZeroAllocationFails) {
  MemoryManager manager;
  Nanoapp app;
  void *ptr = manager.nanoappAlloc(&app, 0);
  EXPECT_EQ(ptr, nullptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
  EXPECT_EQ(manager.getAllocationCount(), 0);
}

TEST(MemoryManager, HugeAllocationFails) {
  MemoryManager manager;
  Nanoapp app;
  void *ptr = manager.nanoappAlloc(&app, manager.getMaxAllocationBytes() + 1);
  EXPECT_EQ(ptr, nullptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
}

TEST(MemoryManager, ManyAllocationsTest) {
  MemoryManager manager;
  Nanoapp app;
  size_t maxCount = manager.getMaxAllocationCount();
  node *head = static_cast<node*>(manager.nanoappAlloc(&app, sizeof(node)));
  node *curr = nullptr, *prev = head;
  for (size_t i = 0; i < maxCount-1; i++) {
    curr = static_cast<node*>(manager.nanoappAlloc(&app, sizeof(node)));
    EXPECT_NE(curr, nullptr);
    prev->next = curr;
    prev = curr;
  }
  EXPECT_EQ(manager.getTotalAllocatedBytes(), maxCount * sizeof(node));
  EXPECT_EQ(manager.getAllocationCount(), maxCount);
  EXPECT_EQ(manager.nanoappAlloc(&app, 1), nullptr);

  curr = head;
  for (size_t i = 0; i < maxCount; i++) {
    node *temp = curr->next;
    manager.nanoappFree(&app, curr);
    curr = temp;
  }
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
  EXPECT_EQ(manager.getAllocationCount(), 0);
}

TEST(MemoryManager, NegativeAllocationFails) {
  MemoryManager manager;
  Nanoapp app;
  void *ptr = manager.nanoappAlloc(&app, -1);
  EXPECT_EQ(ptr, nullptr);
  EXPECT_EQ(manager.getTotalAllocatedBytes(), 0);
  EXPECT_EQ(manager.getAllocationCount(), 0);
}

