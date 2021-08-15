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
#include <base/run_loop.h>
#include <base/threading/thread.h>
#include <benchmark/benchmark.h>
#include <future>

#include "common/message_loop_thread.h"
#include "common/once_timer.h"
#include "common/repeating_timer.h"
#include "common/time_util.h"
#include "osi/include/alarm.h"

using ::benchmark::State;
using bluetooth::common::MessageLoopThread;
using bluetooth::common::OnceTimer;
using bluetooth::common::RepeatingTimer;
using bluetooth::common::time_get_os_boottime_us;

// fake get_main_message_loop implementation for alarm
base::MessageLoop* get_main_message_loop() { return nullptr; }

namespace {
std::unordered_map<int, int> g_map;
std::shared_ptr<std::promise<void>> g_promise;
uint64_t g_start_time;
int g_scheduled_tasks;
int g_task_length;
int g_task_interval;
int g_task_counter;

void TimerFire(void*) { g_promise->set_value(); }

void AlarmSleepAndCountDelayedTime(void*) {
  auto end_time_us = time_get_os_boottime_us();
  auto time_after_start_ms = (end_time_us - g_start_time) / 1000;
  g_task_counter++;
  g_map[time_after_start_ms - g_task_counter * g_task_interval]++;
  std::this_thread::sleep_for(std::chrono::milliseconds(g_task_length));
  if (g_task_counter >= g_scheduled_tasks) {
    g_promise->set_value();
  }
}

}  // namespace

class BM_OsiAlarmTimer : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    ::benchmark::Fixture::SetUp(st);
    alarm_ = alarm_new("osi_alarm_timer_test");
    g_promise = std::make_shared<std::promise<void>>();
  }

  void TearDown(State& st) override {
    g_promise = nullptr;
    alarm_free(alarm_);
    ::benchmark::Fixture::TearDown(st);
  }

  alarm_t* alarm_ = nullptr;
};

BENCHMARK_DEFINE_F(BM_OsiAlarmTimer, timer_performance_ms)(State& state) {
  auto milliseconds = static_cast<int>(state.range(0));
  for (auto _ : state) {
    auto start_time_point = time_get_os_boottime_us();
    alarm_set(alarm_, milliseconds, &TimerFire, nullptr);
    g_promise->get_future().get();
    auto end_time_point = time_get_os_boottime_us();
    auto duration = end_time_point - start_time_point;
    state.SetIterationTime(duration * 1e-6);
  }
};

BENCHMARK_REGISTER_F(BM_OsiAlarmTimer, timer_performance_ms)
    ->Arg(1)
    ->Arg(5)
    ->Arg(10)
    ->Arg(20)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(2000)
    ->Iterations(1)
    ->UseManualTime();

class BM_AlarmTaskTimer : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    ::benchmark::Fixture::SetUp(st);
    message_loop_thread_ = new MessageLoopThread("timer_benchmark");
    message_loop_thread_->StartUp();
    message_loop_thread_->EnableRealTimeScheduling();
    once_timer_ = new OnceTimer();
    repeating_timer_ = new RepeatingTimer();
    g_promise = std::make_shared<std::promise<void>>();
  }

  void TearDown(State& st) override {
    g_promise = nullptr;
    delete once_timer_;
    once_timer_ = nullptr;
    delete repeating_timer_;
    repeating_timer_ = nullptr;
    message_loop_thread_->ShutDown();
    delete message_loop_thread_;
    message_loop_thread_ = nullptr;
    ::benchmark::Fixture::TearDown(st);
  }

  MessageLoopThread* message_loop_thread_;
  OnceTimer* once_timer_;
  RepeatingTimer* repeating_timer_;
};

BENCHMARK_DEFINE_F(BM_AlarmTaskTimer, timer_performance_ms)(State& state) {
  auto milliseconds = static_cast<int>(state.range(0));
  for (auto _ : state) {
    auto start_time_point = time_get_os_boottime_us();
    once_timer_->Schedule(message_loop_thread_->GetWeakPtr(), FROM_HERE,
                          base::BindOnce(&TimerFire, nullptr),
                          base::TimeDelta::FromMilliseconds(milliseconds));
    g_promise->get_future().get();
    once_timer_->Cancel();
    auto end_time_point = time_get_os_boottime_us();
    auto duration = end_time_point - start_time_point;
    state.SetIterationTime(duration * 1e-6);
  }
};

BENCHMARK_REGISTER_F(BM_AlarmTaskTimer, timer_performance_ms)
    ->Arg(1)
    ->Arg(5)
    ->Arg(10)
    ->Arg(20)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(2000)
    ->Iterations(1)
    ->UseManualTime();

class BM_OsiPeriodicAlarmTimer : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    ::benchmark::Fixture::SetUp(st);
    alarm_ = alarm_new_periodic("osi_alarm_timer_test");
    g_map.clear();
    g_promise = std::make_shared<std::promise<void>>();
    g_scheduled_tasks = 0;
    g_task_length = 0;
    g_task_interval = 0;
    g_task_counter = 0;
  }

  void TearDown(State& st) override {
    g_promise = nullptr;
    alarm_free(alarm_);
    ::benchmark::Fixture::TearDown(st);
  }

  alarm_t* alarm_ = nullptr;
};

BENCHMARK_DEFINE_F(BM_OsiPeriodicAlarmTimer, periodic_accuracy)(State& state) {
  for (auto _ : state) {
    g_scheduled_tasks = state.range(0);
    g_task_length = state.range(1);
    g_task_interval = state.range(2);
    g_start_time = time_get_os_boottime_us();
    alarm_set(alarm_, g_task_interval, &AlarmSleepAndCountDelayedTime, nullptr);
    g_promise->get_future().get();
    alarm_cancel(alarm_);
  }
  for (const auto& delay : g_map) {
    state.counters[std::to_string(delay.first)] = delay.second;
  }
};

BENCHMARK_REGISTER_F(BM_OsiPeriodicAlarmTimer, periodic_accuracy)
    ->Args({2000, 1, 5})
    ->Args({2000, 3, 5})
    ->Args({2000, 1, 7})
    ->Args({2000, 3, 7})
    ->Args({2000, 1, 20})
    ->Args({2000, 5, 20})
    ->Args({2000, 10, 20})
    ->Args({2000, 15, 20})
    ->Iterations(1)
    ->UseRealTime();

class BM_AlarmTaskPeriodicTimer : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    ::benchmark::Fixture::SetUp(st);
    message_loop_thread_ = new MessageLoopThread("timer_benchmark");
    message_loop_thread_->StartUp();
    message_loop_thread_->EnableRealTimeScheduling();
    once_timer_ = new OnceTimer();
    repeating_timer_ = new RepeatingTimer();
    g_map.clear();
    g_promise = std::make_shared<std::promise<void>>();
    g_scheduled_tasks = 0;
    g_task_length = 0;
    g_task_interval = 0;
    g_task_counter = 0;
  }

  void TearDown(State& st) override {
    g_promise = nullptr;
    delete once_timer_;
    once_timer_ = nullptr;
    delete repeating_timer_;
    repeating_timer_ = nullptr;
    message_loop_thread_->ShutDown();
    delete message_loop_thread_;
    message_loop_thread_ = nullptr;
    ::benchmark::Fixture::TearDown(st);
  }

  MessageLoopThread* message_loop_thread_;
  OnceTimer* once_timer_;
  RepeatingTimer* repeating_timer_;
};

BENCHMARK_DEFINE_F(BM_AlarmTaskPeriodicTimer, periodic_accuracy)
(State& state) {
  for (auto _ : state) {
    g_scheduled_tasks = state.range(0);
    g_task_length = state.range(1);
    g_task_interval = state.range(2);
    g_start_time = time_get_os_boottime_us();
    repeating_timer_->SchedulePeriodic(
        message_loop_thread_->GetWeakPtr(), FROM_HERE,
        base::BindRepeating(&AlarmSleepAndCountDelayedTime, nullptr),
        base::TimeDelta::FromMilliseconds(g_task_interval));
    g_promise->get_future().get();
    repeating_timer_->Cancel();
  }
  for (const auto& delay : g_map) {
    state.counters[std::to_string(delay.first)] = delay.second;
  }
};

BENCHMARK_REGISTER_F(BM_AlarmTaskPeriodicTimer, periodic_accuracy)
    ->Args({2000, 1, 5})
    ->Args({2000, 3, 5})
    ->Args({2000, 1, 7})
    ->Args({2000, 3, 7})
    ->Args({2000, 1, 20})
    ->Args({2000, 5, 20})
    ->Args({2000, 10, 20})
    ->Args({2000, 15, 20})
    ->Iterations(1)
    ->UseRealTime();

int main(int argc, char** argv) {
  // Disable LOG() output from libchrome
  logging::LoggingSettings log_settings;
  log_settings.logging_dest = logging::LoggingDestination::LOG_NONE;
  CHECK(logging::InitLogging(log_settings)) << "Failed to set up logging";
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
    return 1;
  }
  ::benchmark::RunSpecifiedBenchmarks();
}
