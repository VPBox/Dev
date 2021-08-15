//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/update_manager/prng.h"

#include <vector>

#include <gtest/gtest.h>

using std::vector;

namespace chromeos_update_manager {

TEST(UmPRNGTest, ShouldBeDeterministic) {
  PRNG a(42);
  PRNG b(42);

  for (int i = 0; i < 1000; ++i) {
    EXPECT_EQ(a.Rand(), b.Rand()) << "Iteration i=" << i;
  }
}

TEST(UmPRNGTest, SeedChangesGeneratedSequence) {
  PRNG a(42);
  PRNG b(5);

  vector<uint32_t> values_a;
  vector<uint32_t> values_b;

  for (int i = 0; i < 100; ++i) {
    values_a.push_back(a.Rand());
    values_b.push_back(b.Rand());
  }
  EXPECT_NE(values_a, values_b);
}

TEST(UmPRNGTest, IsNotConstant) {
  PRNG prng(5);

  uint32_t initial_value = prng.Rand();
  bool prng_is_constant = true;
  for (int i = 0; i < 100; ++i) {
    if (prng.Rand() != initial_value) {
      prng_is_constant = false;
      break;
    }
  }
  EXPECT_FALSE(prng_is_constant) << "After 100 iterations.";
}

TEST(UmPRNGTest, RandCoversRange) {
  PRNG a(42);
  int hits[11] = {0};

  for (int i = 0; i < 1000; i++) {
    int r = a.RandMinMax(0, 10);
    ASSERT_LE(0, r);
    ASSERT_GE(10, r);
    hits[r]++;
  }

  for (auto& hit : hits)
    EXPECT_LT(0, hit);
}

}  // namespace chromeos_update_manager
