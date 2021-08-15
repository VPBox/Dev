//
// Copyright (C) 2017 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_POLICY_TEST_UTILS_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_POLICY_TEST_UTILS_H_

#include <memory>
#include <string>

#include <base/time/time.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/update_manager/evaluation_context.h"
#include "update_engine/update_manager/fake_state.h"
#include "update_engine/update_manager/policy_utils.h"

namespace chromeos_update_manager {

class UmPolicyTestBase : public ::testing::Test {
 protected:
  UmPolicyTestBase() = default;

  void SetUp() override;

  void TearDown() override;

  // Sets the clock to fixed values.
  virtual void SetUpDefaultClock();

  // Sets the fake time provider to the time given by the fake clock.
  virtual void SetUpDefaultTimeProvider();

  // Sets up the default state in fake_state_.  override to add Policy-specific
  // items, but only after calling this class's implementation.
  virtual void SetUpDefaultState();

  // Returns a default UpdateState structure:
  virtual UpdateState GetDefaultUpdateState(base::TimeDelta first_seen_period);

  // Runs the passed |method| after resetting the EvaluationContext and expects
  // it to return the |expected| return value.
  template <typename T, typename R, typename... Args>
  void ExpectStatus(EvalStatus expected, T method, R* result, Args... args) {
    std::string error = "<None>";
    eval_ctx_->ResetEvaluation();
    EXPECT_EQ(expected,
              (*method)(eval_ctx_.get(), &fake_state_, &error, result, args...))
        << "Returned error: " << error
        << "\nEvaluation context: " << eval_ctx_->DumpContext();
  }

  // Runs the passed |method| after resetting the EvaluationContext, in order
  // to use the method to get a value for other testing (doesn't validate the
  // return value, just returns it).
  template <typename T, typename R, typename... Args>
  EvalStatus CallMethodWithContext(T method, R* result, Args... args) {
    std::string error = "<None>";
    eval_ctx_->ResetEvaluation();
    return (*method)(eval_ctx_.get(), &fake_state_, &error, result, args...);
  }

  // Runs the passed |policy_method| on the framework policy and expects it to
  // return the |expected| return value.
  template <typename T, typename R, typename... Args>
  void ExpectPolicyStatus(EvalStatus expected,
                          T policy_method,
                          R* result,
                          Args... args) {
    std::string error = "<None>";
    eval_ctx_->ResetEvaluation();
    EXPECT_EQ(expected,
              (policy_.get()->*policy_method)(
                  eval_ctx_.get(), &fake_state_, &error, result, args...))
        << "Returned error: " << error
        << "\nEvaluation context: " << eval_ctx_->DumpContext();
  }

  brillo::FakeMessageLoop loop_{nullptr};
  chromeos_update_engine::FakeClock fake_clock_;
  FakeState fake_state_;
  scoped_refptr<EvaluationContext> eval_ctx_;
  std::unique_ptr<Policy> policy_;
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_POLICY_TEST_UTILS_H_
