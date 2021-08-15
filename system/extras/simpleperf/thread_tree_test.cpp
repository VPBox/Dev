/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "thread_tree.h"

#include <gtest/gtest.h>

using namespace simpleperf;

class ThreadTreeTest : public ::testing::Test {
 protected:
  void AddMap(uint64_t start, uint64_t end, const std::string& name) {
    thread_tree_.AddThreadMap(0, 0, start, end - start, start, name);
    if (expected_names_.size() < end) {
      expected_names_.resize(end);
    }
    for (uint64_t i = start; i < end; ++i) {
      expected_names_[i] = name;
    }
  }

  void CheckMaps() {
    std::vector<std::string> names;
    ThreadEntry* thread = thread_tree_.FindThreadOrNew(0, 0);
    ASSERT_TRUE(thread != nullptr);
    ASSERT_TRUE(thread->maps != nullptr);
    uint64_t prev_end = 0;
    for (auto& pair : thread->maps->maps) {
      ASSERT_GE(pair.first, prev_end);
      prev_end = pair.second->get_end_addr();
      ASSERT_EQ(pair.second->start_addr, pair.first);
      ASSERT_GT(pair.second->len, 0u);
      ASSERT_EQ(pair.second->pgoff, pair.first);
      if (names.size() < pair.second->get_end_addr()) {
        names.resize(pair.second->get_end_addr());
      }
      for (uint64_t i = pair.first; i < pair.second->get_end_addr(); ++i) {
        names[i] = pair.second->dso->Path();
      }
    }
    ASSERT_EQ(names, expected_names_);
    // Check result of ThreadTree::FindMap.
    for (size_t i = 0; i < expected_names_.size(); ++i) {
      const MapEntry* entry = thread_tree_.FindMap(thread, i, false);
      ASSERT_TRUE(entry != nullptr);
      if (expected_names_[i].empty()) {
        ASSERT_TRUE(thread_tree_.IsUnknownDso(entry->dso));
      } else {
        ASSERT_EQ(entry->dso->Path(), expected_names_[i]);
      }
    }
  }

  std::vector<std::string> expected_names_;
  ThreadTree thread_tree_;
};

TEST_F(ThreadTreeTest, maps_smoke) {
  AddMap(0, 5, "0");
  AddMap(10, 15, "1");
  CheckMaps();

  // Overlap left.
  AddMap(1, 6, "2");
  CheckMaps();
  AddMap(4, 5, "3");
  CheckMaps();

  // Overlap right.
  AddMap(9, 12, "4");
  CheckMaps();
  AddMap(8, 15, "5");
  CheckMaps();
  AddMap(7, 16, "6");
  CheckMaps();

  // Overlap all.
  AddMap(0, 17, "7");
  CheckMaps();
}

TEST_F(ThreadTreeTest, jit_maps_before_fork) {
  // Maps for JIT symfiles can arrive before fork records.
  thread_tree_.AddThreadMap(0, 0, 0, 1, 0, "0", map_flags::PROT_JIT_SYMFILE_MAP);
  thread_tree_.AddThreadMap(1, 1, 1, 1, 1, "1");
  thread_tree_.ForkThread(0, 0, 1, 1);
  expected_names_ = {"0", "1"};
  CheckMaps();
  ThreadEntry* thread = thread_tree_.FindThreadOrNew(0, 0);
  ASSERT_TRUE(thread != nullptr);
  const MapEntry* map = thread_tree_.FindMap(thread, 0);
  ASSERT_TRUE(map != nullptr);
  ASSERT_EQ(map->flags, map_flags::PROT_JIT_SYMFILE_MAP);
}
