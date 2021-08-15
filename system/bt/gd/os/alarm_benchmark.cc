/*
 * Copyright 2019 The Android Open Source Project
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

#include <chrono>
#include <future>
#include <unordered_map>

#include "benchmark/benchmark.h"

#include "os/alarm.h"
#include "os/repeating_alarm.h"
#include "os/thread.h"

using ::benchmark::State;
using ::bluetooth::os::Alarm;
using ::bluetooth::os::RepeatingAlarm;
using ::bluetooth::os::Thread;

class BM_ReactableAlarm : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    ::benchmark::Fixture::SetUp(st);
    thread_ = std::make_unique<Thread>("timer_benchmark", Thread::Priority::REAL_TIME);
    alarm_ = std::make_unique<Alarm>(thread_.get());
    repeating_alarm_ = std::make_unique<RepeatingAlarm>(thread_.get());
    map_.clear();
    scheduled_tasks_ = 0;
    task_length_ = 0;
    task_interval_ = 0;
    task_counter_ = 0;
    promise_ = std::promise<void>();
  }

  void TearDown(State& st) override {
    alarm_ = nullptr;
    repeating_alarm_ = nullptr;
    thread_->Stop();
    thread_ = nullptr;
    ::benchmark::Fixture::TearDown(st);
  }

  void AlarmSleepAndCountDelayedTime() {
    auto end_time = std::chrono::steady_clock::now();
    auto duration_since_start = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_);
    task_counter_++;
    map_[duration_since_start.count() - task_counter_ * task_interval_]++;
    std::this_thread::sleep_for(std::chrono::milliseconds(task_length_));
    if (task_counter_ >= scheduled_tasks_) {
      promise_.set_value();
    }
  }

  void TimerFire() {
    promise_.set_value();
  }

  int64_t scheduled_tasks_;
  int64_t task_length_;
  int64_t task_interval_;
  int task_counter_;
  std::unordered_map<int, int> map_;
  std::promise<void> promise_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::unique_ptr<Thread> thread_;
  std::unique_ptr<Alarm> alarm_;
  std::unique_ptr<RepeatingAlarm> repeating_alarm_;
};

BENCHMARK_DEFINE_F(BM_ReactableAlarm, timer_performance_ms)(State& state) {
  auto milliseconds = static_cast<int>(state.range(0));
  for (auto _ : state) {
    auto start_time_point = std::chrono::steady_clock::now();
    alarm_->Schedule([this] { return TimerFire(); }, std::chrono::milliseconds(milliseconds));
    promise_.get_future().get();
    auto end_time_point = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_point - start_time_point);
    state.SetIterationTime(static_cast<double>(duration.count()) * 1e-6);
    alarm_->Cancel();
  }
};

BENCHMARK_REGISTER_F(BM_ReactableAlarm, timer_performance_ms)
    ->Arg(1)
    ->Arg(5)
    ->Arg(10)
    ->Arg(20)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(2000)
    ->Iterations(1)
    ->UseRealTime();

BENCHMARK_DEFINE_F(BM_ReactableAlarm, periodic_accuracy)(State& state) {
  for (auto _ : state) {
    scheduled_tasks_ = state.range(0);
    task_length_ = state.range(1);
    task_interval_ = state.range(2);
    start_time_ = std::chrono::steady_clock::now();
    repeating_alarm_->Schedule([this] { AlarmSleepAndCountDelayedTime(); }, std::chrono::milliseconds(task_interval_));
    promise_.get_future().get();
    repeating_alarm_->Cancel();
  }
  for (const auto& delay : map_) {
    state.counters[std::to_string(delay.first)] = delay.second;
  }
};

BENCHMARK_REGISTER_F(BM_ReactableAlarm, periodic_accuracy)
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
