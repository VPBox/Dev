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

#include "update_engine/update_manager/update_manager.h"

#include <unistd.h>

#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/test/simple_test_clock.h>
#include <base/time/time.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/update_manager/default_policy.h"
#include "update_engine/update_manager/fake_state.h"
#include "update_engine/update_manager/mock_policy.h"
#include "update_engine/update_manager/umtest_utils.h"

using base::Bind;
using base::Callback;
using base::Time;
using base::TimeDelta;
using brillo::MessageLoop;
using brillo::MessageLoopRunMaxIterations;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::FakeClock;
using std::pair;
using std::string;
using std::tuple;
using std::unique_ptr;
using std::vector;

namespace {

// Generates a fixed timestamp for use in faking the current time.
Time FixedTime() {
  Time::Exploded now_exp;
  now_exp.year = 2014;
  now_exp.month = 3;
  now_exp.day_of_week = 2;
  now_exp.day_of_month = 18;
  now_exp.hour = 8;
  now_exp.minute = 5;
  now_exp.second = 33;
  now_exp.millisecond = 675;
  Time time;
  ignore_result(Time::FromLocalExploded(now_exp, &time));
  return time;
}

}  // namespace

namespace chromeos_update_manager {

class UmUpdateManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    loop_.SetAsCurrent();
    fake_state_ = new FakeState();
    umut_.reset(new UpdateManager(&fake_clock_,
                                  TimeDelta::FromSeconds(5),
                                  TimeDelta::FromSeconds(1),
                                  fake_state_));
  }

  void TearDown() override { EXPECT_FALSE(loop_.PendingTasks()); }

  base::SimpleTestClock test_clock_;
  brillo::FakeMessageLoop loop_{&test_clock_};
  FakeState* fake_state_;  // Owned by the umut_.
  FakeClock fake_clock_;
  unique_ptr<UpdateManager> umut_;
};

// The FailingPolicy implements a single method and make it always fail. This
// class extends the DefaultPolicy class to allow extensions of the Policy
// class without extending nor changing this test.
class FailingPolicy : public DefaultPolicy {
 public:
  explicit FailingPolicy(int* num_called_p) : num_called_p_(num_called_p) {}
  FailingPolicy() : FailingPolicy(nullptr) {}
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                string* error,
                                UpdateCheckParams* result) const override {
    if (num_called_p_)
      (*num_called_p_)++;
    *error = "FailingPolicy failed.";
    return EvalStatus::kFailed;
  }

 protected:
  string PolicyName() const override { return "FailingPolicy"; }

 private:
  int* num_called_p_;
};

// The LazyPolicy always returns EvalStatus::kAskMeAgainLater.
class LazyPolicy : public DefaultPolicy {
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                string* error,
                                UpdateCheckParams* result) const override {
    return EvalStatus::kAskMeAgainLater;
  }

 protected:
  string PolicyName() const override { return "LazyPolicy"; }
};

// A policy that sleeps for a predetermined amount of time, then checks for a
// wallclock-based time threshold (if given) and returns
// EvalStatus::kAskMeAgainLater if not passed; otherwise, returns
// EvalStatus::kSucceeded. Increments a counter every time it is being queried,
// if a pointer to it is provided.
class DelayPolicy : public DefaultPolicy {
 public:
  DelayPolicy(int sleep_secs, Time time_threshold, int* num_called_p)
      : sleep_secs_(sleep_secs),
        time_threshold_(time_threshold),
        num_called_p_(num_called_p) {}
  EvalStatus UpdateCheckAllowed(EvaluationContext* ec,
                                State* state,
                                string* error,
                                UpdateCheckParams* result) const override {
    if (num_called_p_)
      (*num_called_p_)++;

    // Sleep for a predetermined amount of time.
    if (sleep_secs_ > 0)
      sleep(sleep_secs_);

    // Check for a time threshold. This can be used to ensure that the policy
    // has some non-constant dependency.
    if (time_threshold_ < Time::Max() &&
        ec->IsWallclockTimeGreaterThan(time_threshold_))
      return EvalStatus::kSucceeded;

    return EvalStatus::kAskMeAgainLater;
  }

 protected:
  string PolicyName() const override { return "DelayPolicy"; }

 private:
  int sleep_secs_;
  Time time_threshold_;
  int* num_called_p_;
};

// AccumulateCallsCallback() adds to the passed |acc| accumulator vector pairs
// of EvalStatus and T instances. This allows to create a callback that keeps
// track of when it is called and the arguments passed to it, to be used with
// the UpdateManager::AsyncPolicyRequest().
template <typename T>
static void AccumulateCallsCallback(vector<pair<EvalStatus, T>>* acc,
                                    EvalStatus status,
                                    const T& result) {
  acc->push_back(std::make_pair(status, result));
}

// Tests that policy requests are completed successfully. It is important that
// this tests cover all policy requests as defined in Policy.
TEST_F(UmUpdateManagerTest, PolicyRequestCallUpdateCheckAllowed) {
  UpdateCheckParams result;
  EXPECT_EQ(EvalStatus::kSucceeded,
            umut_->PolicyRequest(&Policy::UpdateCheckAllowed, &result));
}

TEST_F(UmUpdateManagerTest, PolicyRequestCallUpdateCanStart) {
  UpdateState update_state = UpdateState();
  update_state.interactive = true;
  update_state.is_delta_payload = false;
  update_state.first_seen = FixedTime();
  update_state.num_checks = 1;
  update_state.num_failures = 0;
  update_state.failures_last_updated = Time();
  update_state.download_urls = vector<string>{"http://fake/url/"};
  update_state.download_errors_max = 10;
  update_state.p2p_downloading_disabled = false;
  update_state.p2p_sharing_disabled = false;
  update_state.p2p_num_attempts = 0;
  update_state.p2p_first_attempted = Time();
  update_state.last_download_url_idx = -1;
  update_state.last_download_url_num_errors = 0;
  update_state.download_errors = vector<tuple<int, ErrorCode, Time>>();
  update_state.backoff_expiry = Time();
  update_state.is_backoff_disabled = false;
  update_state.scatter_wait_period = TimeDelta::FromSeconds(15);
  update_state.scatter_check_threshold = 4;
  update_state.scatter_wait_period_max = TimeDelta::FromSeconds(60);
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 8;

  UpdateDownloadParams result;
  EXPECT_EQ(
      EvalStatus::kSucceeded,
      umut_->PolicyRequest(&Policy::UpdateCanStart, &result, update_state));
}

TEST_F(UmUpdateManagerTest, PolicyRequestCallsDefaultOnError) {
  umut_->set_policy(new FailingPolicy());

  // Tests that the DefaultPolicy instance is called when the method fails,
  // which will set this as true.
  UpdateCheckParams result;
  result.updates_enabled = false;
  EvalStatus status =
      umut_->PolicyRequest(&Policy::UpdateCheckAllowed, &result);
  EXPECT_EQ(EvalStatus::kSucceeded, status);
  EXPECT_TRUE(result.updates_enabled);
}

// This test only applies to debug builds where DCHECK is enabled.
#if DCHECK_IS_ON
TEST_F(UmUpdateManagerTest, PolicyRequestDoesntBlockDeathTest) {
  // The update manager should die (DCHECK) if a policy called synchronously
  // returns a kAskMeAgainLater value.
  UpdateCheckParams result;
  umut_->set_policy(new LazyPolicy());
  EXPECT_DEATH(umut_->PolicyRequest(&Policy::UpdateCheckAllowed, &result), "");
}
#endif  // DCHECK_IS_ON

TEST_F(UmUpdateManagerTest, AsyncPolicyRequestDelaysEvaluation) {
  // To avoid differences in code execution order between an AsyncPolicyRequest
  // call on a policy that returns AskMeAgainLater the first time and one that
  // succeeds the first time, we ensure that the passed callback is called from
  // the main loop in both cases even when we could evaluate it right now.
  umut_->set_policy(new FailingPolicy());

  vector<pair<EvalStatus, UpdateCheckParams>> calls;
  Callback<void(EvalStatus, const UpdateCheckParams&)> callback =
      Bind(AccumulateCallsCallback<UpdateCheckParams>, &calls);

  umut_->AsyncPolicyRequest(callback, &Policy::UpdateCheckAllowed);
  // The callback should wait until we run the main loop for it to be executed.
  EXPECT_EQ(0U, calls.size());
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_EQ(1U, calls.size());
}

TEST_F(UmUpdateManagerTest, AsyncPolicyRequestTimeoutDoesNotFire) {
  // Set up an async policy call to return immediately, then wait a little and
  // ensure that the timeout event does not fire.
  int num_called = 0;
  umut_->set_policy(new FailingPolicy(&num_called));

  vector<pair<EvalStatus, UpdateCheckParams>> calls;
  Callback<void(EvalStatus, const UpdateCheckParams&)> callback =
      Bind(AccumulateCallsCallback<UpdateCheckParams>, &calls);

  umut_->AsyncPolicyRequest(callback, &Policy::UpdateCheckAllowed);
  // Run the main loop, ensure that policy was attempted once before deferring
  // to the default.
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_EQ(1, num_called);
  ASSERT_EQ(1U, calls.size());
  EXPECT_EQ(EvalStatus::kSucceeded, calls[0].first);
  // Wait for the timeout to expire, run the main loop again, ensure that
  // nothing happened.
  test_clock_.Advance(TimeDelta::FromSeconds(2));
  MessageLoopRunMaxIterations(MessageLoop::current(), 10);
  EXPECT_EQ(1, num_called);
  EXPECT_EQ(1U, calls.size());
}

TEST_F(UmUpdateManagerTest, AsyncPolicyRequestTimesOut) {
  // Set up an async policy call to exceed its expiration timeout, make sure
  // that the default policy was not used (no callback) and that evaluation is
  // reattempted.
  int num_called = 0;
  umut_->set_policy(new DelayPolicy(
      0,
      fake_clock_.GetWallclockTime() + TimeDelta::FromSeconds(3),
      &num_called));

  vector<pair<EvalStatus, UpdateCheckParams>> calls;
  Callback<void(EvalStatus, const UpdateCheckParams&)> callback =
      Bind(AccumulateCallsCallback<UpdateCheckParams>, &calls);

  umut_->AsyncPolicyRequest(callback, &Policy::UpdateCheckAllowed);
  // Run the main loop, ensure that policy was attempted once but the callback
  // was not invoked.
  MessageLoopRunMaxIterations(MessageLoop::current(), 100);
  EXPECT_EQ(1, num_called);
  EXPECT_EQ(0U, calls.size());
  // Wait for the expiration timeout to expire, run the main loop again,
  // ensure that reevaluation occurred but callback was not invoked (i.e.
  // default policy was not consulted).
  test_clock_.Advance(TimeDelta::FromSeconds(2));
  fake_clock_.SetWallclockTime(fake_clock_.GetWallclockTime() +
                               TimeDelta::FromSeconds(2));
  MessageLoopRunMaxIterations(MessageLoop::current(), 10);
  EXPECT_EQ(2, num_called);
  EXPECT_EQ(0U, calls.size());
  // Wait for reevaluation due to delay to happen, ensure that it occurs and
  // that the callback is invoked.
  test_clock_.Advance(TimeDelta::FromSeconds(2));
  fake_clock_.SetWallclockTime(fake_clock_.GetWallclockTime() +
                               TimeDelta::FromSeconds(2));
  MessageLoopRunMaxIterations(MessageLoop::current(), 10);
  EXPECT_EQ(3, num_called);
  ASSERT_EQ(1U, calls.size());
  EXPECT_EQ(EvalStatus::kSucceeded, calls[0].first);
}

}  // namespace chromeos_update_manager
