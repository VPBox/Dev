//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common/cpu_limiter.h"

#include <gtest/gtest.h>

namespace chromeos_update_engine {

class CPULimiterTest : public ::testing::Test {};

namespace {
// Compares cpu shares and returns an integer that is less
// than, equal to or greater than 0 if |shares_lhs| is,
// respectively, lower than, same as or higher than |shares_rhs|.
int CompareCpuShares(CpuShares shares_lhs, CpuShares shares_rhs) {
  return static_cast<int>(shares_lhs) - static_cast<int>(shares_rhs);
}
}  // namespace

// Tests the CPU shares enum is in the order we expect it.
TEST(CPULimiterTest, CompareCpuSharesTest) {
  EXPECT_LT(CompareCpuShares(CpuShares::kLow, CpuShares::kNormal), 0);
  EXPECT_GT(CompareCpuShares(CpuShares::kNormal, CpuShares::kLow), 0);
  EXPECT_EQ(CompareCpuShares(CpuShares::kNormal, CpuShares::kNormal), 0);
  EXPECT_GT(CompareCpuShares(CpuShares::kHigh, CpuShares::kNormal), 0);
}

}  // namespace chromeos_update_engine
