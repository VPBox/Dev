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

#include <future>
#include <memory>
#include <thread>

#include "benchmark/benchmark.h"

#include "os/handler.h"
#include "os/thread.h"

using ::benchmark::State;
using ::bluetooth::os::Handler;
using ::bluetooth::os::Thread;

#define NUM_MESSAGES_TO_SEND 100000

class BM_ThreadPerformance : public ::benchmark::Fixture {
 protected:
  void SetUp(State& st) override {
    benchmark::Fixture::SetUp(st);
    counter_promise_ = std::promise<void>();
    counter_ = 0;
  }
  void TearDown(State& st) override {
    benchmark::Fixture::TearDown(st);
  }
  void callback_batch() {
    counter_++;
    if (counter_ >= num_messages_to_send_) {
      counter_promise_.set_value();
    }
  }

  void callback() {
    counter_promise_.set_value();
  }

  int64_t num_messages_to_send_;
  int64_t counter_;
  std::promise<void> counter_promise_;
};

class BM_ReactorThread : public BM_ThreadPerformance {
 protected:
  void SetUp(State& st) override {
    BM_ThreadPerformance::SetUp(st);
    thread_ = std::make_unique<Thread>("BM_ReactorThread thread", Thread::Priority::NORMAL);
    handler_ = std::make_unique<Handler>(thread_.get());
  }
  void TearDown(State& st) override {
    handler_ = nullptr;
    thread_->Stop();
    thread_ = nullptr;
    BM_ThreadPerformance::TearDown(st);
  }
  std::unique_ptr<Thread> thread_;
  std::unique_ptr<Handler> handler_;
};

BENCHMARK_DEFINE_F(BM_ReactorThread, batch_enque_dequeue)(State& state) {
  for (auto _ : state) {
    num_messages_to_send_ = state.range(0);
    counter_ = 0;
    counter_promise_ = std::promise<void>();
    std::future<void> counter_future = counter_promise_.get_future();
    for (int i = 0; i < num_messages_to_send_; i++) {
      handler_->Post([this]() { callback_batch(); });
    }
    counter_future.wait();
  }
};

BENCHMARK_REGISTER_F(BM_ReactorThread, batch_enque_dequeue)
    ->Arg(10)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Iterations(1)
    ->UseRealTime();

BENCHMARK_DEFINE_F(BM_ReactorThread, sequential_execution)(State& state) {
  for (auto _ : state) {
    num_messages_to_send_ = state.range(0);
    for (int i = 0; i < num_messages_to_send_; i++) {
      counter_promise_ = std::promise<void>();
      std::future<void> counter_future = counter_promise_.get_future();
      handler_->Post([this]() { callback(); });
      counter_future.wait();
    }
  }
};

BENCHMARK_REGISTER_F(BM_ReactorThread, sequential_execution)
    ->Arg(10)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Iterations(1)
    ->UseRealTime();
