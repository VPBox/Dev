//
// Copyright (C) 2009 The Android Open Source Project
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

#include "update_engine/common/action_pipe.h"

#include <gtest/gtest.h>
#include <string>
#include "update_engine/common/action.h"

using std::string;

namespace chromeos_update_engine {

using chromeos_update_engine::ActionPipe;

class ActionPipeTestAction;

template <>
class ActionTraits<ActionPipeTestAction> {
 public:
  typedef string OutputObjectType;
  typedef string InputObjectType;
};

// This is a simple Action class for testing.
class ActionPipeTestAction : public Action<ActionPipeTestAction> {
 public:
  typedef string InputObjectType;
  typedef string OutputObjectType;
  ActionPipe<string>* in_pipe() { return in_pipe_.get(); }
  ActionPipe<string>* out_pipe() { return out_pipe_.get(); }
  void PerformAction() {}
  string Type() const { return "ActionPipeTestAction"; }
};

class ActionPipeTest : public ::testing::Test {};

// This test creates two simple Actions and sends a message via an ActionPipe
// from one to the other.
TEST(ActionPipeTest, SimpleTest) {
  ActionPipeTestAction a, b;
  BondActions(&a, &b);
  a.out_pipe()->set_contents("foo");
  EXPECT_EQ("foo", b.in_pipe()->contents());
}

}  // namespace chromeos_update_engine
