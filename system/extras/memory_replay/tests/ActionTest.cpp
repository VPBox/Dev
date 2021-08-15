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
#include <stdint.h>
#include <string.h>

#include "Action.h"
#include "Pointers.h"

TEST(ActionTest, malloc) {
  uint8_t memory[Action::MaxActionSize()];
  const char* line = "1024";
  Action* action = Action::CreateAction(0x1234, "malloc", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  Pointers pointers(1);
  action->Execute(&pointers);
  void* pointer = pointers.Remove(0x1234);
  ASSERT_TRUE(pointer != nullptr);
  free(pointer);
}

TEST(ActionTest, malloc_malformed) {
  uint8_t memory[128];
  const char* line = "";
  Action* action = Action::CreateAction(0x1234, "malloc", line, memory);
  ASSERT_FALSE(action != NULL);
}

TEST(ActionTest, free) {
  uint8_t memory[128];
  const char* line = "";
  Action* action = Action::CreateAction(0x1234, "free", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_TRUE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  Pointers pointers(1);
  pointers.Add(0x1234, malloc(10));
  action->Execute(&pointers);
}

TEST(ActionTest, calloc) {
  uint8_t memory[128];
  const char* line = "100 10";
  Action* action = Action::CreateAction(0x1234, "calloc", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  Pointers pointers(1);
  action->Execute(&pointers);
  void* pointer = pointers.Remove(0x1234);
  ASSERT_TRUE(pointer != nullptr);
  free(pointer);
}

TEST(ActionTest, free_zero) {
  uint8_t memory[128];
  const char* line = "";
  Action* action = Action::CreateAction(0, "free", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());
  // Should be a nop.
  action->Execute(nullptr);
}

TEST(ActionTest, calloc_malformed) {
  uint8_t memory[128];
  const char* line1 = "100";
  Action* action = Action::CreateAction(0x1234, "calloc", line1, memory);
  ASSERT_FALSE(action != NULL);

  const char* line2 = "";
  action = Action::CreateAction(0x1234, "calloc", line2, memory);
  ASSERT_FALSE(action != NULL);
}

TEST(ActionTest, realloc) {
  uint8_t memory[128];
  const char* line = "0xabcd 100";
  Action* action = Action::CreateAction(0x1234, "realloc", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_TRUE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  Pointers pointers(1);
  pointers.Add(0xabcd, malloc(10));
  action->Execute(&pointers);
  void* pointer = pointers.Remove(0x1234);
  ASSERT_TRUE(pointer != nullptr);
  free(pointer);

  const char* null_line = "0x0 100";
  action = Action::CreateAction(0x1234, "realloc", null_line, memory);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  action->Execute(&pointers);
  pointer = pointers.Remove(0x1234);
  ASSERT_TRUE(pointer != nullptr);
  free(pointer);
}

TEST(ActionTest, realloc_malformed) {
  uint8_t memory[128];
  const char* line1 = "0x100";
  Action* action = Action::CreateAction(0x1234, "realloc", line1, memory);
  ASSERT_FALSE(action != NULL);

  const char* line2 = "";
  action = Action::CreateAction(0x1234, "realloc", line2, memory);
  ASSERT_FALSE(action != NULL);
}

TEST(ActionTest, memalign) {
  uint8_t memory[128];
  const char* line = "16 300";
  Action* action = Action::CreateAction(0x1234, "memalign", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_FALSE(action->EndThread());

  Pointers pointers(1);
  action->Execute(&pointers);
  void* pointer = pointers.Remove(0x1234);
  ASSERT_TRUE(pointer != nullptr);
  free(pointer);
}

TEST(ActionTest, memalign_malformed) {
  uint8_t memory[128];
  const char* line1 = "100";
  Action* action = Action::CreateAction(0x1234, "memalign", line1, memory);
  ASSERT_FALSE(action != NULL);

  const char* line2 = "";
  action = Action::CreateAction(0x1234, "memalign", line2, memory);
  ASSERT_FALSE(action != NULL);
}

TEST(ActionTest, endthread) {
  uint8_t memory[128];
  const char* line = "";
  Action* action = Action::CreateAction(0x0, "thread_done", line, memory);
  ASSERT_TRUE(action != NULL);
  ASSERT_FALSE(action->DoesFree());
  ASSERT_TRUE(action->EndThread());

  action->Execute(nullptr);
}
