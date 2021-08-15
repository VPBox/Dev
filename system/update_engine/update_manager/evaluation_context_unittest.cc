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

#include "update_engine/update_manager/evaluation_context.h"

#include <memory>
#include <string>

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/update_manager/fake_variable.h"
#include "update_engine/update_manager/generic_variables.h"
#include "update_engine/update_manager/mock_variable.h"
#include "update_engine/update_manager/umtest_utils.h"

using base::Bind;
using base::Closure;
using base::Time;
using base::TimeDelta;
using brillo::MessageLoop;
using brillo::MessageLoopRunMaxIterations;
using brillo::MessageLoopRunUntil;
using chromeos_update_engine::FakeClock;
using std::string;
using std::unique_ptr;
using testing::_;
using testing::Return;
using testing::StrictMock;

namespace chromeos_update_manager {

namespace {

// Sets the value of the passed pointer to true.
void SetTrue(bool* value) {
  *value = true;
}

bool GetBoolean(bool* value) {
  return *value;
}

template <typename T>
void ReadVar(scoped_refptr<EvaluationContext> ec, Variable<T>* var) {
  ec->GetValue(var);
}

// Runs |evaluation|; if the value pointed by |count_p| is greater than zero,
// decrement it and schedule a reevaluation; otherwise, writes true to |done_p|.
void EvaluateRepeatedly(Closure evaluation,
                        scoped_refptr<EvaluationContext> ec,
                        int* count_p,
                        bool* done_p) {
  evaluation.Run();

  // Schedule reevaluation if needed.
  if (*count_p > 0) {
    Closure closure = Bind(EvaluateRepeatedly, evaluation, ec, count_p, done_p);
    ASSERT_TRUE(ec->RunOnValueChangeOrTimeout(closure))
        << "Failed to schedule reevaluation, count_p=" << *count_p;
    (*count_p)--;
  } else {
    *done_p = true;
  }
}

}  // namespace

class UmEvaluationContextTest : public ::testing::Test {
 protected:
  void SetUp() override {
    loop_.SetAsCurrent();
    // Apr 22, 2009 19:25:00 UTC (this is a random reference point).
    fake_clock_.SetMonotonicTime(Time::FromTimeT(1240428300));
    // Mar 2, 2006 1:23:45 UTC.
    fake_clock_.SetWallclockTime(Time::FromTimeT(1141262625));
    eval_ctx_ = new EvaluationContext(
        &fake_clock_,
        default_timeout_,
        default_timeout_,
        unique_ptr<base::Callback<void(EvaluationContext*)>>(nullptr));
  }

  void TearDown() override {
    // Ensure that the evaluation context did not leak and is actually being
    // destroyed.
    if (eval_ctx_) {
      base::WeakPtr<EvaluationContext> eval_ctx_weak_alias =
          eval_ctx_->weak_ptr_factory_.GetWeakPtr();
      ASSERT_NE(nullptr, eval_ctx_weak_alias.get());
      eval_ctx_ = nullptr;
      EXPECT_EQ(nullptr, eval_ctx_weak_alias.get())
          << "The evaluation context was not destroyed! This is likely a bug "
             "in how the test was written, look for leaking handles to the EC, "
             "possibly through closure objects.";
    }

    // Check that the evaluation context removed all the observers.
    EXPECT_TRUE(fake_int_var_.observer_list_.empty());
    EXPECT_TRUE(fake_async_var_.observer_list_.empty());
    EXPECT_TRUE(fake_const_var_.observer_list_.empty());
    EXPECT_TRUE(fake_poll_var_.observer_list_.empty());

    EXPECT_FALSE(loop_.PendingTasks());
  }

  TimeDelta default_timeout_ = TimeDelta::FromSeconds(5);

  brillo::FakeMessageLoop loop_{nullptr};
  FakeClock fake_clock_;
  scoped_refptr<EvaluationContext> eval_ctx_;

  // FakeVariables used for testing the EvaluationContext. These are required
  // here to prevent them from going away *before* the EvaluationContext under
  // test does, which keeps a reference to them.
  FakeVariable<bool> fail_var_ = {"fail_var", kVariableModePoll};
  FakeVariable<int> fake_int_var_ = {"fake_int", kVariableModePoll};
  FakeVariable<string> fake_async_var_ = {"fake_async", kVariableModeAsync};
  FakeVariable<string> fake_const_var_ = {"fake_const", kVariableModeConst};
  FakeVariable<string> fake_poll_var_ = {"fake_poll",
                                         TimeDelta::FromSeconds(1)};
  StrictMock<MockVariable<string>> mock_var_async_{"mock_var_async",
                                                   kVariableModeAsync};
  StrictMock<MockVariable<string>> mock_var_poll_{"mock_var_poll",
                                                  kVariableModePoll};
};

TEST_F(UmEvaluationContextTest, GetValueFails) {
  // FakeVariable is initialized as returning null.
  EXPECT_EQ(nullptr, eval_ctx_->GetValue(&fake_int_var_));
}

TEST_F(UmEvaluationContextTest, GetValueFailsWithInvalidVar) {
  EXPECT_EQ(nullptr, eval_ctx_->GetValue(static_cast<Variable<int>*>(nullptr)));
}

TEST_F(UmEvaluationContextTest, GetValueReturns) {
  const int* p_fake_int;

  fake_int_var_.reset(new int(42));
  p_fake_int = eval_ctx_->GetValue(&fake_int_var_);
  ASSERT_NE(nullptr, p_fake_int);
  EXPECT_EQ(42, *p_fake_int);
}

TEST_F(UmEvaluationContextTest, GetValueCached) {
  const int* p_fake_int;

  fake_int_var_.reset(new int(42));
  p_fake_int = eval_ctx_->GetValue(&fake_int_var_);

  // Check that if the variable changes, the EvaluationContext keeps returning
  // the cached value.
  fake_int_var_.reset(new int(5));

  p_fake_int = eval_ctx_->GetValue(&fake_int_var_);
  ASSERT_NE(nullptr, p_fake_int);
  EXPECT_EQ(42, *p_fake_int);
}

TEST_F(UmEvaluationContextTest, GetValueCachesNull) {
  const int* p_fake_int = eval_ctx_->GetValue(&fake_int_var_);
  EXPECT_EQ(nullptr, p_fake_int);

  fake_int_var_.reset(new int(42));
  // A second attempt to read the variable should not work because this
  // EvaluationContext already got a null value.
  p_fake_int = eval_ctx_->GetValue(&fake_int_var_);
  EXPECT_EQ(nullptr, p_fake_int);
}

TEST_F(UmEvaluationContextTest, GetValueMixedTypes) {
  const int* p_fake_int;
  const string* p_fake_string;

  fake_int_var_.reset(new int(42));
  fake_poll_var_.reset(new string("Hello world!"));
  // Check that the EvaluationContext can handle multiple Variable types. This
  // is mostly a compile-time check due to the template nature of this method.
  p_fake_int = eval_ctx_->GetValue(&fake_int_var_);
  p_fake_string = eval_ctx_->GetValue(&fake_poll_var_);

  ASSERT_NE(nullptr, p_fake_int);
  EXPECT_EQ(42, *p_fake_int);

  ASSERT_NE(nullptr, p_fake_string);
  EXPECT_EQ("Hello world!", *p_fake_string);
}

// Test that we don't schedule an event if there's no variable to wait for.
TEST_F(UmEvaluationContextTest, RunOnValueChangeOrTimeoutWithoutVariables) {
  fake_const_var_.reset(new string("Hello world!"));
  EXPECT_EQ(*eval_ctx_->GetValue(&fake_const_var_), "Hello world!");

  EXPECT_FALSE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

// Test that reevaluation occurs when an async variable it depends on changes.
TEST_F(UmEvaluationContextTest, RunOnValueChangeOrTimeoutWithVariables) {
  fake_async_var_.reset(new string("Async value"));
  eval_ctx_->GetValue(&fake_async_var_);

  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  // Check that the scheduled callback isn't run until we signal a ValueChaged.
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_FALSE(value);

  fake_async_var_.NotifyValueChanged();
  EXPECT_FALSE(value);
  // Ensure that the scheduled callback isn't run until we are back on the main
  // loop.
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_TRUE(value);
}

// Test that we don't re-schedule the events if we are attending one.
TEST_F(UmEvaluationContextTest, RunOnValueChangeOrTimeoutCalledTwice) {
  fake_async_var_.reset(new string("Async value"));
  eval_ctx_->GetValue(&fake_async_var_);

  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  EXPECT_FALSE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));

  // The scheduled event should still work.
  fake_async_var_.NotifyValueChanged();
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_TRUE(value);
}

// Test that reevaluation occurs when a polling timeout fires.
TEST_F(UmEvaluationContextTest, RunOnValueChangeOrTimeoutRunsFromTimeout) {
  fake_poll_var_.reset(new string("Polled value"));
  eval_ctx_->GetValue(&fake_poll_var_);

  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  // Check that the scheduled callback isn't run until the timeout occurs.
  MessageLoopRunMaxIterations(MessageLoop::current(), 10);
  EXPECT_FALSE(value);
  MessageLoopRunUntil(MessageLoop::current(),
                      TimeDelta::FromSeconds(10),
                      Bind(&GetBoolean, &value));
  EXPECT_TRUE(value);
}

// Test that callback is called when evaluation context expires, and that it
// cannot be used again unless the expiration deadline is reset.
TEST_F(UmEvaluationContextTest, RunOnValueChangeOrTimeoutExpires) {
  fake_async_var_.reset(new string("Async value"));
  eval_ctx_->GetValue(&fake_async_var_);

  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  // Check that the scheduled callback isn't run until the timeout occurs.
  MessageLoopRunMaxIterations(MessageLoop::current(), 10);
  EXPECT_FALSE(value);
  MessageLoopRunUntil(MessageLoop::current(),
                      TimeDelta::FromSeconds(10),
                      Bind(&GetBoolean, &value));
  EXPECT_TRUE(value);

  // Ensure that we cannot reschedule an evaluation.
  EXPECT_FALSE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));

  // Ensure that we can reschedule an evaluation after resetting expiration.
  eval_ctx_->ResetExpiration();
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

// Test that we clear the events when destroying the EvaluationContext.
TEST_F(UmEvaluationContextTest, RemoveObserversAndTimeoutTest) {
  fake_async_var_.reset(new string("Async value"));
  eval_ctx_->GetValue(&fake_async_var_);

  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  eval_ctx_ = nullptr;

  // This should not trigger the callback since the EvaluationContext waiting
  // for it is gone, and it should have remove all its observers.
  fake_async_var_.NotifyValueChanged();
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_FALSE(value);
}

// Scheduling two reevaluations from the callback should succeed.
TEST_F(UmEvaluationContextTest,
       RunOnValueChangeOrTimeoutReevaluatesRepeatedly) {
  fake_poll_var_.reset(new string("Polled value"));
  Closure evaluation = Bind(ReadVar<string>, eval_ctx_, &fake_poll_var_);
  int num_reevaluations = 2;
  bool done = false;

  // Run the evaluation once.
  evaluation.Run();

  // Schedule repeated reevaluations.
  Closure closure = Bind(
      EvaluateRepeatedly, evaluation, eval_ctx_, &num_reevaluations, &done);
  ASSERT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(closure));
  MessageLoopRunUntil(MessageLoop::current(),
                      TimeDelta::FromSeconds(10),
                      Bind(&GetBoolean, &done));
  EXPECT_EQ(0, num_reevaluations);
}

// Test that we can delete the EvaluationContext while having pending events.
TEST_F(UmEvaluationContextTest, ObjectDeletedWithPendingEventsTest) {
  fake_async_var_.reset(new string("Async value"));
  fake_poll_var_.reset(new string("Polled value"));
  eval_ctx_->GetValue(&fake_async_var_);
  eval_ctx_->GetValue(&fake_poll_var_);
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
  // TearDown() checks for leaked observers on this async_variable, which means
  // that our object is still alive after removing its reference.
}

// Test that timed events fired after removal of the EvaluationContext don't
// crash.
TEST_F(UmEvaluationContextTest, TimeoutEventAfterDeleteTest) {
  FakeVariable<string> fake_short_poll_var = {"fake_short_poll",
                                              TimeDelta::FromSeconds(1)};
  fake_short_poll_var.reset(new string("Polled value"));
  eval_ctx_->GetValue(&fake_short_poll_var);
  bool value = false;
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(Bind(&SetTrue, &value)));
  // Remove the last reference to the EvaluationContext and run the loop for
  // 10 seconds to give time to the main loop to trigger the timeout Event (of 1
  // second). Our callback should not be called because the EvaluationContext
  // was removed before the timeout event is attended.
  eval_ctx_ = nullptr;
  MessageLoopRunUntil(MessageLoop::current(),
                      TimeDelta::FromSeconds(10),
                      Bind(&GetBoolean, &value));
  EXPECT_FALSE(value);
}

TEST_F(UmEvaluationContextTest, DefaultTimeout) {
  // Test that the evaluation timeout calculation uses the default timeout on
  // setup.
  EXPECT_CALL(mock_var_async_, GetValue(default_timeout_, _))
      .WillOnce(Return(nullptr));
  EXPECT_EQ(nullptr, eval_ctx_->GetValue(&mock_var_async_));
}

TEST_F(UmEvaluationContextTest, TimeoutUpdatesWithMonotonicTime) {
  fake_clock_.SetMonotonicTime(fake_clock_.GetMonotonicTime() +
                               TimeDelta::FromSeconds(1));

  TimeDelta timeout = default_timeout_ - TimeDelta::FromSeconds(1);

  EXPECT_CALL(mock_var_async_, GetValue(timeout, _)).WillOnce(Return(nullptr));
  EXPECT_EQ(nullptr, eval_ctx_->GetValue(&mock_var_async_));
}

TEST_F(UmEvaluationContextTest, ResetEvaluationResetsTimesWallclock) {
  Time cur_time = fake_clock_.GetWallclockTime();
  // Advance the time on the clock but don't call ResetEvaluation yet.
  fake_clock_.SetWallclockTime(cur_time + TimeDelta::FromSeconds(4));

  EXPECT_TRUE(eval_ctx_->IsWallclockTimeGreaterThan(cur_time -
                                                    TimeDelta::FromSeconds(1)));
  EXPECT_FALSE(eval_ctx_->IsWallclockTimeGreaterThan(cur_time));
  EXPECT_FALSE(eval_ctx_->IsWallclockTimeGreaterThan(
      cur_time + TimeDelta::FromSeconds(1)));
  // Call ResetEvaluation now, which should use the new evaluation time.
  eval_ctx_->ResetEvaluation();

  cur_time = fake_clock_.GetWallclockTime();
  EXPECT_TRUE(eval_ctx_->IsWallclockTimeGreaterThan(cur_time -
                                                    TimeDelta::FromSeconds(1)));
  EXPECT_FALSE(eval_ctx_->IsWallclockTimeGreaterThan(cur_time));
  EXPECT_FALSE(eval_ctx_->IsWallclockTimeGreaterThan(
      cur_time + TimeDelta::FromSeconds(1)));
}

TEST_F(UmEvaluationContextTest, ResetEvaluationResetsTimesMonotonic) {
  Time cur_time = fake_clock_.GetMonotonicTime();
  // Advance the time on the clock but don't call ResetEvaluation yet.
  fake_clock_.SetMonotonicTime(cur_time + TimeDelta::FromSeconds(4));

  EXPECT_TRUE(eval_ctx_->IsMonotonicTimeGreaterThan(cur_time -
                                                    TimeDelta::FromSeconds(1)));
  EXPECT_FALSE(eval_ctx_->IsMonotonicTimeGreaterThan(cur_time));
  EXPECT_FALSE(eval_ctx_->IsMonotonicTimeGreaterThan(
      cur_time + TimeDelta::FromSeconds(1)));
  // Call ResetEvaluation now, which should use the new evaluation time.
  eval_ctx_->ResetEvaluation();

  cur_time = fake_clock_.GetMonotonicTime();
  EXPECT_TRUE(eval_ctx_->IsMonotonicTimeGreaterThan(cur_time -
                                                    TimeDelta::FromSeconds(1)));
  EXPECT_FALSE(eval_ctx_->IsMonotonicTimeGreaterThan(cur_time));
  EXPECT_FALSE(eval_ctx_->IsMonotonicTimeGreaterThan(
      cur_time + TimeDelta::FromSeconds(1)));
}

TEST_F(UmEvaluationContextTest,
       IsWallclockTimeGreaterThanSignalsTriggerReevaluation) {
  EXPECT_FALSE(eval_ctx_->IsWallclockTimeGreaterThan(
      fake_clock_.GetWallclockTime() + TimeDelta::FromSeconds(1)));

  // The "false" from IsWallclockTimeGreaterThan means that's not that timestamp
  // yet, so this should schedule a callback for when that happens.
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

TEST_F(UmEvaluationContextTest,
       IsMonotonicTimeGreaterThanSignalsTriggerReevaluation) {
  EXPECT_FALSE(eval_ctx_->IsMonotonicTimeGreaterThan(
      fake_clock_.GetMonotonicTime() + TimeDelta::FromSeconds(1)));

  // The "false" from IsMonotonicTimeGreaterThan means that's not that timestamp
  // yet, so this should schedule a callback for when that happens.
  EXPECT_TRUE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

TEST_F(UmEvaluationContextTest,
       IsWallclockTimeGreaterThanDoesntRecordPastTimestamps) {
  // IsWallclockTimeGreaterThan() should ignore timestamps on the past for
  // reevaluation.
  EXPECT_TRUE(eval_ctx_->IsWallclockTimeGreaterThan(
      fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(20)));
  EXPECT_TRUE(eval_ctx_->IsWallclockTimeGreaterThan(
      fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(1)));

  // Callback should not be scheduled.
  EXPECT_FALSE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

TEST_F(UmEvaluationContextTest,
       IsMonotonicTimeGreaterThanDoesntRecordPastTimestamps) {
  // IsMonotonicTimeGreaterThan() should ignore timestamps on the past for
  // reevaluation.
  EXPECT_TRUE(eval_ctx_->IsMonotonicTimeGreaterThan(
      fake_clock_.GetMonotonicTime() - TimeDelta::FromSeconds(20)));
  EXPECT_TRUE(eval_ctx_->IsMonotonicTimeGreaterThan(
      fake_clock_.GetMonotonicTime() - TimeDelta::FromSeconds(1)));

  // Callback should not be scheduled.
  EXPECT_FALSE(eval_ctx_->RunOnValueChangeOrTimeout(
#if BASE_VER < 576279
      Bind(&base::DoNothing)
#else
      base::DoNothing()
#endif
          ));
}

TEST_F(UmEvaluationContextTest, DumpContext) {
  // |fail_var_| yield "(no value)" since it is unset.
  eval_ctx_->GetValue(&fail_var_);

  // Check that this is included.
  fake_int_var_.reset(new int(42));
  eval_ctx_->GetValue(&fake_int_var_);

  // Check that double-quotes are escaped properly.
  fake_poll_var_.reset(new string("Hello \"world\"!"));
  eval_ctx_->GetValue(&fake_poll_var_);

  // Note that the variables are printed in alphabetical order. Also
  // see UmEvaluationContextText::SetUp() where the values used for
  // |evaluation_start_{monotonic,wallclock| are set.
  EXPECT_EQ(
      "{\n"
      "   \"evaluation_start_monotonic\": \"4/22/2009 19:25:00 GMT\",\n"
      "   \"evaluation_start_wallclock\": \"3/2/2006 1:23:45 GMT\",\n"
      "   \"variables\": {\n"
      "      \"fail_var\": \"(no value)\",\n"
      "      \"fake_int\": \"42\",\n"
      "      \"fake_poll\": \"Hello \\\"world\\\"!\"\n"
      "   }\n"
      "}",
      eval_ctx_->DumpContext());
}

}  // namespace chromeos_update_manager
