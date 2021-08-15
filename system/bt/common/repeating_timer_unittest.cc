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
#include "repeating_timer.h"

using bluetooth::common::MessageLoopThread;
using bluetooth::common::RepeatingTimer;

// Allowed error between the expected and actual delay for DoInThreadDelayed().
constexpr uint32_t delay_error_ms = 3;

/**
 * Unit tests to verify Task Scheduler.
 */
class RepeatingTimerTest : public ::testing::Test {
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

  void VerifyDelayTimeAndSleep(std::chrono::steady_clock::time_point start_time,
                               int interval_ms, int scheduled_tasks,
                               int task_length_ms,
                               std::promise<void>* promise) {
    auto end_time = std::chrono::steady_clock::now();
    auto actual_delay = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    counter_++;
    int64_t scheduled_delay_ms = interval_ms * counter_;
    if (counter_ == scheduled_tasks) {
      promise->set_value();
    }
    ASSERT_NEAR(scheduled_delay_ms, actual_delay.count(), delay_error_ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(task_length_ms));
  }

  void VerifyMultipleDelayedTasks(int scheduled_tasks, int task_length_ms,
                                  int interval_between_tasks_ms) {
    std::string name = "test_thread";
    MessageLoopThread message_loop_thread(name);
    message_loop_thread.StartUp();
    message_loop_thread.EnableRealTimeScheduling();
    auto future = promise_->get_future();
    auto start_time = std::chrono::steady_clock::now();
    timer_->SchedulePeriodic(
        message_loop_thread.GetWeakPtr(), FROM_HERE,
        base::BindRepeating(&RepeatingTimerTest::VerifyDelayTimeAndSleep,
                            base::Unretained(this), start_time,
                            interval_between_tasks_ms, scheduled_tasks,
                            task_length_ms, promise_),
        base::TimeDelta::FromMilliseconds(interval_between_tasks_ms));
    future.get();
    timer_->CancelAndWait();
  }

  void CancelRepeatingTimerAndWait() { timer_->CancelAndWait(); }

 protected:
  void SetUp() override {
    ::testing::Test::SetUp();
    counter_ = 0;
    timer_ = new RepeatingTimer();
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
  RepeatingTimer* timer_;
  std::promise<void>* promise_;
};

TEST_F(RepeatingTimerTest, initial_is_not_scheduled) {
  ASSERT_FALSE(timer_->IsScheduled());
}

TEST_F(RepeatingTimerTest, cancel_without_scheduling) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();

  EXPECT_FALSE(timer_->IsScheduled());
  timer_->CancelAndWait();
  EXPECT_FALSE(timer_->IsScheduled());
}

TEST_F(RepeatingTimerTest, periodic_run) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  auto future = promise_->get_future();
  uint32_t delay_ms = 5;
  int num_tasks = 200;

  timer_->SchedulePeriodic(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindRepeating(&RepeatingTimerTest::IncreaseTaskCounter,
                          base::Unretained(this), num_tasks, promise_),
      base::TimeDelta::FromMilliseconds(delay_ms));
  future.get();
  ASSERT_GE(counter_, num_tasks);
  timer_->CancelAndWait();
}

TEST_F(RepeatingTimerTest, schedule_periodic_task_zero_interval) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t interval_ms = 0;

  ASSERT_FALSE(timer_->SchedulePeriodic(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindRepeating(&RepeatingTimerTest::ShouldNotHappen,
                          base::Unretained(this)),
      base::TimeDelta::FromMilliseconds(interval_ms)));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_error_ms));
}

// Verify that deleting the timer without cancelling it will cancel the task
TEST_F(RepeatingTimerTest, periodic_delete_without_cancel) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  timer_->SchedulePeriodic(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindRepeating(&RepeatingTimerTest::ShouldNotHappen,
                          base::Unretained(this)),
      base::TimeDelta::FromMilliseconds(delay_ms));
  delete timer_;
  timer_ = nullptr;
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_error_ms));
}

TEST_F(RepeatingTimerTest, cancel_single_task_near_fire_no_race_condition) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  timer_->SchedulePeriodic(message_loop_thread.GetWeakPtr(), FROM_HERE,
                           base::DoNothing(),
                           base::TimeDelta::FromMilliseconds(delay_ms));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
  timer_->CancelAndWait();
}

TEST_F(RepeatingTimerTest, cancel_periodic_task) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  uint32_t delay_ms = 5;
  int num_tasks = 5;
  auto future = promise_->get_future();

  timer_->SchedulePeriodic(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindRepeating(&RepeatingTimerTest::IncreaseTaskCounter,
                          base::Unretained(this), num_tasks, promise_),
      base::TimeDelta::FromMilliseconds(delay_ms));
  future.wait();
  timer_->CancelAndWait();
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay_ms + delay_error_ms));
  int counter = counter_;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay_ms + delay_error_ms));
  ASSERT_EQ(counter, counter_);
}

// Schedule 10 short periodic tasks with interval 1 ms between each; verify the
// functionality
TEST_F(RepeatingTimerTest, schedule_multiple_delayed_tasks) {
  VerifyMultipleDelayedTasks(10, 0, 1);
}

// Schedule 10 periodic tasks with interval 2 ms between each and each takes 1
// ms; verify the functionality
TEST_F(RepeatingTimerTest, schedule_multiple_delayed_slow_tasks) {
  VerifyMultipleDelayedTasks(10, 1, 2);
}

TEST_F(RepeatingTimerTest,
       message_loop_thread_down_cancel_scheduled_periodic_task) {
  std::string name = "test_thread";
  MessageLoopThread message_loop_thread(name);
  message_loop_thread.StartUp();
  std::string my_name;
  auto future = promise_->get_future();
  uint32_t delay_ms = 5;
  int num_tasks = 5;

  timer_->SchedulePeriodic(
      message_loop_thread.GetWeakPtr(), FROM_HERE,
      base::BindRepeating(&RepeatingTimerTest::IncreaseTaskCounter,
                          base::Unretained(this), num_tasks, promise_),
      base::TimeDelta::FromMilliseconds(delay_ms));
  future.wait();
  message_loop_thread.ShutDown();
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay_ms + delay_error_ms));
  int counter = counter_;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay_ms + delay_error_ms));
  ASSERT_EQ(counter, counter_);
}
