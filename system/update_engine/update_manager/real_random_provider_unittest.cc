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

#include "update_engine/update_manager/real_random_provider.h"

#include <gtest/gtest.h>

#include <memory>

#include "update_engine/update_manager/umtest_utils.h"

using std::unique_ptr;

namespace chromeos_update_manager {

class UmRealRandomProviderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // The provider initializes correctly.
    provider_.reset(new RealRandomProvider());
    ASSERT_NE(nullptr, provider_.get());
    ASSERT_TRUE(provider_->Init());

    provider_->var_seed();
  }

  unique_ptr<RealRandomProvider> provider_;
};

TEST_F(UmRealRandomProviderTest, InitFinalize) {
  // The provider initializes all variables with valid objects.
  EXPECT_NE(nullptr, provider_->var_seed());
}

TEST_F(UmRealRandomProviderTest, GetRandomValues) {
  // Should not return the same random seed repeatedly.
  unique_ptr<const uint64_t> value(
      provider_->var_seed()->GetValue(UmTestUtils::DefaultTimeout(), nullptr));
  ASSERT_NE(nullptr, value.get());

  // Test that at least the returned values are different. This test fails,
  // by design, once every 2^320 runs.
  bool is_same_value = true;
  for (int i = 0; i < 5; i++) {
    unique_ptr<const uint64_t> other_value(provider_->var_seed()->GetValue(
        UmTestUtils::DefaultTimeout(), nullptr));
    ASSERT_NE(nullptr, other_value.get());
    is_same_value = is_same_value && *other_value == *value;
  }
  EXPECT_FALSE(is_same_value);
}

}  // namespace chromeos_update_manager
