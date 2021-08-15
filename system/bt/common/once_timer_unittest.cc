/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/logging.h>
#include <gtest/gtest.h>
#include <future>

#include "message_loop_thread.h"
#include "once_timer.h"

using bluetooth::common::MessageLoopThread;
using bluetooth::common::OnceTimer;

// Allowed error between the expected and actual delay for DoInThreadDelayed().
constexpr uint32_t delay_error_ms = 3;

/**
 * Unit tests to verify Task Scheduler.
 */
class OnceTimerTest : public ::testing::Test {
 public:
  void ShouldNotHappen() { FAIL() << "Should not happen"; }

  void IncreaseTaskCounter(int scheduled_tasks, std::promise<void>* promise) {
    counter_++;
    if (counter_ == scheduled_tasks) {
      promise->set_value();
    }
  }

  void GetName(std::string* name, std::promise<void>* promise) {
    char my_name[256];
    pthread_getname_np(pthread_self(), my_name, sizeof(my_name));
    name->append(my_name);
    promise->set_value();
  }

  void SleepAndIncreaseCounter(std::promise<void>* promise, int sleep_ms) {
    promise->set_value();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    counter_++;
  }

  void CancelTimerAndWait() { timer_->CancelAndWait(); }

 protected:
  void SetUp() override {
    ::testing::Test::SetUp();
    counter_ = 0;
    timer_ = new OnceTimer();
    promise_ = new std::promise<void>();
  }

  void TearDown() override {
    if (promise_ != nullptr) {
      delete promise_;
      promise_ = nullptr;
    }
    if (timer_ != nullptr) {
      delete timer_;
      timer_ = nullptr;
    }
  }

  int counter_;
  OnceTimer* timer_;
  std::promise<void>* promise_;
};

TEST_F(OnceTimerTest, initial_is_not_scheduled) {
  ASSERT_FALSE(timer_->IsScheduled());
}

TEST_F(OnceTimerTest, schedule_task) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  auto future = promise_->get_future();
  std::string my_name;
  uint32_t delay_ms = 5;

  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::BindOnce(&OnceTimerTest::GetName,
                                  base::Unretained(this), &my_name, promise_),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  EXPECT_TRUE(timer_->IsScheduled());
  future.get();
  ASSERT_EQ(name, my_name);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_error_ms));
  EXPECT_FALSE(timer_->IsScheduled());
}

TEST_F(OnceTimerTest, cancel_without_scheduling) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();

  EXPECT_FALSE(timer_->IsScheduled());
  timer_->CancelAndWait();
  EXPECT_FALSE(timer_->IsScheduled());
}

TEST_F(OnceTimerTest, cancel_in_callback_no_deadlock) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;

  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::BindOnce(&OnceTimerTest::CancelTimerAndWait,
                                  base::Unretained(this)),
                   base::TimeDelta::FromMilliseconds(delay_ms));
}

TEST_F(OnceTimerTest, cancel_single_task) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 100000000;
  timer_->Schedule(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindOnce(&OnceTimerTest::ShouldNotHappen, base::Unretained(this)),
      base::TimeDelta::FromMilliseconds(delay_ms));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_error_ms));
  timer_->CancelAndWait();
}

TEST_F(OnceTimerTest, message_loop_thread_down_cancel_task) {
  std::string name = "test_thread";
  {
    MessageLoopThread message_loop_thread(name);
    message_loop_thread.StartUp();
    uint32_t delay_ms = 100000000;
    timer_->Schedule(
        message_loop_thread.GetWeakPtr(), FROM_HERE,
        base::BindOnce(&OnceTimerTest::ShouldNotHappen, base::Unretained(this)),
        base::TimeDelta::FromMilliseconds(delay_ms));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_error_ms));
  }
}

// Verify that if a task is being executed, then cancelling it is no-op
TEST_F(OnceTimerTest, cancel_current_task_no_effect) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  auto future = promise_->get_future();
  uint32_t delay_ms = 5;

  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::BindOnce(&OnceTimerTest::SleepAndIncreaseCounter,
                                  base::Unretained(this), promise_, delay_ms),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  EXPECT_TRUE(timer_->IsScheduled());
  future.get();
  timer_->CancelAndWait();
  ASSERT_EQ(counter_, 1);
  EXPECT_FALSE(timer_->IsScheduled());
}

TEST_F(OnceTimerTest, reschedule_task_dont_invoke_new_task_early) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::DoNothing(),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms - 2));
  timer_->Schedule(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindOnce(&OnceTimerTest::ShouldNotHappen, base::Unretained(this)),
      base::TimeDelta::FromMilliseconds(delay_ms + 1000));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

TEST_F(OnceTimerTest, reschedule_task_when_firing_dont_invoke_new_task_early) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::DoNothing(),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
  timer_->Schedule(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindOnce(&OnceTimerTest::ShouldNotHappen, base::Unretained(this)),
      base::TimeDelta::FromMilliseconds(delay_ms + 1000));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

TEST_F(OnceTimerTest, reschedule_task_when_firing_must_schedule_new_task) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  std::string my_name;
  auto future = promise_->get_future();
  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::DoNothing(),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
  timer_->Schedule(message_loop_thread.GetWeakPtr(), FROM_HERE,
                   base::BindOnce(&OnceTimerTest::GetName,
                                  base::Unretained(this), &my_name, promise_),
                   base::TimeDelta::FromMilliseconds(delay_ms));
  future.get();
  ASSERT_EQ(name, my_name);
}
