//
// Copyright (C) 2018 The Android Open Source Project
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

#include "update_engine/update_boot_flags_action.h"

#include <memory>
#include <utility>

#include <base/bind.h>
#include <gtest/gtest.h>

#include "update_engine/fake_system_state.h"

namespace chromeos_update_engine {

class UpdateBootFlagsActionTest : public ::testing::Test {
 public:
  FakeSystemState fake_system_state_;
};

TEST_F(UpdateBootFlagsActionTest, SimpleTest) {
  auto boot_control = fake_system_state_.fake_boot_control();
  auto action = std::make_unique<UpdateBootFlagsAction>(boot_control);
  ActionProcessor processor;
  processor.EnqueueAction(std::move(action));

  EXPECT_FALSE(UpdateBootFlagsAction::updated_boot_flags_);
  EXPECT_FALSE(UpdateBootFlagsAction::is_running_);
  processor.StartProcessing();
  EXPECT_TRUE(UpdateBootFlagsAction::updated_boot_flags_);
  EXPECT_FALSE(UpdateBootFlagsAction::is_running_);
}

TEST_F(UpdateBootFlagsActionTest, DoubleActionTest) {
  // Reset the static flags.
  UpdateBootFlagsAction::updated_boot_flags_ = false;
  UpdateBootFlagsAction::is_running_ = false;

  auto boot_control = fake_system_state_.fake_boot_control();
  auto action1 = std::make_unique<UpdateBootFlagsAction>(boot_control);
  auto action2 = std::make_unique<UpdateBootFlagsAction>(boot_control);
  ActionProcessor processor1, processor2;
  processor1.EnqueueAction(std::move(action1));
  processor2.EnqueueAction(std::move(action2));

  EXPECT_FALSE(UpdateBootFlagsAction::updated_boot_flags_);
  EXPECT_FALSE(UpdateBootFlagsAction::is_running_);
  processor1.StartProcessing();
  EXPECT_TRUE(UpdateBootFlagsAction::updated_boot_flags_);
  EXPECT_FALSE(UpdateBootFlagsAction::is_running_);
  processor2.StartProcessing();
  EXPECT_TRUE(UpdateBootFlagsAction::updated_boot_flags_);
  EXPECT_FALSE(UpdateBootFlagsAction::is_running_);
}

}  // namespace chromeos_update_engine
