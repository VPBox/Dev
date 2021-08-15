//
// Copyright (C) 2010 The Android Open Source Project
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

#include "update_engine/common/action.h"

#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "update_engine/common/action_processor.h"

using std::string;

namespace chromeos_update_engine {

using chromeos_update_engine::ActionPipe;

class ActionTestAction;

template <>
class ActionTraits<ActionTestAction> {
 public:
  typedef string OutputObjectType;
  typedef string InputObjectType;
};

// This is a simple Action class for testing.
class ActionTestAction : public Action<ActionTestAction> {
 public:
  typedef string InputObjectType;
  typedef string OutputObjectType;
  ActionPipe<string>* in_pipe() { return in_pipe_.get(); }
  ActionPipe<string>* out_pipe() { return out_pipe_.get(); }
  ActionProcessor* processor() { return processor_; }
  void PerformAction() {}
  void CompleteAction() {
    ASSERT_TRUE(processor());
    processor()->ActionComplete(this, ErrorCode::kSuccess);
  }
  string Type() const { return "ActionTestAction"; }
};

class ActionTest : public ::testing::Test {};

// This test creates two simple Actions and sends a message via an ActionPipe
// from one to the other.
TEST(ActionTest, SimpleTest) {
  auto action = std::make_unique<ActionTestAction>();
  auto action_ptr = action.get();
  EXPECT_FALSE(action->in_pipe());
  EXPECT_FALSE(action->out_pipe());
  EXPECT_FALSE(action->processor());
  EXPECT_FALSE(action->IsRunning());

  ActionProcessor action_processor;
  action_processor.EnqueueAction(std::move(action));
  EXPECT_EQ(&action_processor, action_ptr->processor());
  action_processor.StartProcessing();
  EXPECT_TRUE(action_ptr->IsRunning());
  action_ptr->CompleteAction();
}

}  // namespace chromeos_update_engine
