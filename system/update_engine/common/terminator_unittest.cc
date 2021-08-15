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

#include "update_engine/common/terminator.h"

#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

using testing::ExitedWithCode;

namespace chromeos_update_engine {

class TerminatorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    Terminator::Init();
    ASSERT_FALSE(Terminator::exit_blocked());
    ASSERT_FALSE(Terminator::exit_requested());
  }
  void TearDown() override {
    // Makes sure subsequent non-Terminator tests don't get accidentally
    // terminated.
    Terminator::Init();
  }
};

typedef TerminatorTest TerminatorDeathTest;

namespace {
void UnblockExitThroughUnblocker() {
  ScopedTerminatorExitUnblocker unblocker = ScopedTerminatorExitUnblocker();
}

void RaiseSIGTERM() {
  ASSERT_EXIT(raise(SIGTERM), ExitedWithCode(2), "");
}
}  // namespace

TEST_F(TerminatorTest, HandleSignalTest) {
  Terminator::set_exit_blocked(true);
  Terminator::HandleSignal(SIGTERM);
  ASSERT_TRUE(Terminator::exit_requested());
}

TEST_F(TerminatorTest, ScopedTerminatorExitUnblockerTest) {
  Terminator::set_exit_blocked(true);
  ASSERT_TRUE(Terminator::exit_blocked());
  ASSERT_FALSE(Terminator::exit_requested());
  UnblockExitThroughUnblocker();
  ASSERT_FALSE(Terminator::exit_blocked());
  ASSERT_FALSE(Terminator::exit_requested());
}

TEST_F(TerminatorDeathTest, ExitTest) {
  ASSERT_EXIT(Terminator::Exit(), ExitedWithCode(2), "");
  Terminator::set_exit_blocked(true);
  ASSERT_EXIT(Terminator::Exit(), ExitedWithCode(2), "");
}

TEST_F(TerminatorDeathTest, RaiseSignalTest) {
  RaiseSIGTERM();
  Terminator::set_exit_blocked(true);
  EXPECT_FATAL_FAILURE(RaiseSIGTERM(), "");
}

TEST_F(TerminatorDeathTest, ScopedTerminatorExitUnblockerExitTest) {
  Terminator::set_exit_blocked(true);
  Terminator::exit_requested_ = 1;
  ASSERT_EXIT(UnblockExitThroughUnblocker(), ExitedWithCode(2), "");
}

}  // namespace chromeos_update_engine
