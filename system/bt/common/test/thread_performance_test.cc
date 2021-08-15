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
#include <base/logging.h>
#include <base/run_loop.h>
#include <base/threading/thread.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <unistd.h>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "common/message_loop_thread.h"
#include "osi/include/fixed_queue.h"
#include "osi/include/thread.h"

using bluetooth::common::MessageLoopThread;

#define NUM_MESSAGES_TO_SEND 100000

volatile static int g_counter = 0;
static std::unique_ptr<std::promise<void>> g_counter_promise = nullptr;

void callback_batch(fixed_queue_t* queue, void* data) {
  if (queue != nullptr) {
    fixed_queue_dequeue(queue);
  }
  g_counter++;
  if (g_counter >= NUM_MESSAGES_TO_SEND) {
    g_counter_promise->set_value();
  }
}

class PerformanceTest : public testing::Test {
 protected:
  void SetUp() override {
    set_up_promise_ = std::make_unique<std::promise<void>>();
    g_counter = 0;
    bt_msg_queue_ = fixed_queue_new(SIZE_MAX);
  }
  void TearDown() override {
    fixed_queue_free(bt_msg_queue_, nullptr);
    bt_msg_queue_ = nullptr;
    set_up_promise_.reset(nullptr);
    g_counter_promise.reset(nullptr);
  }
  fixed_queue_t* bt_msg_queue_ = nullptr;
  std::unique_ptr<std::promise<void>> set_up_promise_ = nullptr;
};

class MessageLoopPerformanceTest : public PerformanceTest {
 public:
  static void RunThread(void* context) {
    auto test = static_cast<MessageLoopPerformanceTest*>(context);
    test->RunMessageLoop();
  }
  static void* RunPThread(void* context) {
    auto test = static_cast<MessageLoopPerformanceTest*>(context);
    test->RunMessageLoop();
    return nullptr;
  }
  void RunMessageLoop() {
    message_loop_ = new base::MessageLoop();
    run_loop_ = new base::RunLoop();
    message_loop_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&std::promise<void>::set_value,
                              base::Unretained(set_up_promise_.get())));
    run_loop_->Run();
    delete message_loop_;
    message_loop_ = nullptr;
    delete run_loop_;
    run_loop_ = nullptr;
  }

 protected:
  base::MessageLoop* message_loop_ = nullptr;
  base::RunLoop* run_loop_ = nullptr;
};

class OsiThreadMessageLoopPerformanceTest : public MessageLoopPerformanceTest {
 protected:
  void SetUp() override {
    MessageLoopPerformanceTest::SetUp();
    std::future<void> set_up_future = set_up_promise_->get_future();
    thread_ = thread_new("OsiThreadMessageLoopPerformanceTest thread");
    thread_post(thread_, &MessageLoopPerformanceTest::RunThread, this);
    set_up_future.wait();
  }

  void TearDown() override {
    message_loop_->task_runner()->PostTask(FROM_HERE,
                                           run_loop_->QuitWhenIdleClosure());
    thread_free(thread_);
    thread_ = nullptr;
    MessageLoopPerformanceTest::TearDown();
  }

  thread_t* thread_ = nullptr;
};

TEST_F(OsiThreadMessageLoopPerformanceTest, message_loop_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();
  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
    message_loop_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&callback_batch, bt_msg_queue_, nullptr));
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "OsiThreadMessageLoopPerformanceTest, " << duration.count()
            << " ms, " << NUM_MESSAGES_TO_SEND << " messages";
}

class StlThreadMessageLoopPerformanceTest : public MessageLoopPerformanceTest {
 protected:
  void SetUp() override {
    MessageLoopPerformanceTest::SetUp();
    std::future<void> set_up_future = set_up_promise_->get_future();
    thread_ = new std::thread(&MessageLoopPerformanceTest::RunThread, this);
    set_up_future.wait();
  }

  void TearDown() override {
    message_loop_->task_runner()->PostTask(FROM_HERE,
                                           run_loop_->QuitWhenIdleClosure());
    thread_->join();
    delete thread_;
    thread_ = nullptr;
    MessageLoopPerformanceTest::TearDown();
  }

  std::thread* thread_ = nullptr;
};

TEST_F(StlThreadMessageLoopPerformanceTest, stl_thread_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();
  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
    message_loop_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&callback_batch, bt_msg_queue_, nullptr));
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "StlThreadMessageLoopPerformanceTest, " << duration.count()
            << " ms, " << NUM_MESSAGES_TO_SEND << " messages";
}

class PosixThreadMessageLoopPerformanceTest
    : public MessageLoopPerformanceTest {
 protected:
  void SetUp() override {
    MessageLoopPerformanceTest::SetUp();
    std::future<void> set_up_future = set_up_promise_->get_future();
    pthread_create(&thread_, nullptr, &MessageLoopPerformanceTest::RunPThread,
                   (void*)this);
    set_up_future.wait();
  }

  void TearDown() override {
    message_loop_->task_runner()->PostTask(FROM_HERE,
                                           run_loop_->QuitWhenIdleClosure());
    pthread_join(thread_, nullptr);
    MessageLoopPerformanceTest::TearDown();
  }

  pthread_t thread_ = -1;
};

TEST_F(PosixThreadMessageLoopPerformanceTest, stl_thread_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();

  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
    message_loop_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&callback_batch, bt_msg_queue_, nullptr));
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "PosixThreadMessageLoopPerformanceTest, " << duration.count()
            << " ms, " << NUM_MESSAGES_TO_SEND << " messages";
}

class ReactorPerformanceTest : public PerformanceTest {
 protected:
  void SetUp() override {
    PerformanceTest::SetUp();
    thread_ = thread_new("ReactorPerformanceTest thread");
  }

  void TearDown() override {
    thread_free(thread_);
    thread_ = nullptr;
    PerformanceTest::TearDown();
  }

  thread_t* thread_ = nullptr;
};

TEST_F(ReactorPerformanceTest, reactor_thread_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();
  fixed_queue_register_dequeue(bt_msg_queue_, thread_get_reactor(thread_),
                               callback_batch, nullptr);

  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "ReactorPerformanceTest, " << duration.count() << " ms, "
            << NUM_MESSAGES_TO_SEND << " messages";
}

class WorkerThreadPerformanceTest : public PerformanceTest {
 protected:
  void SetUp() override {
    PerformanceTest::SetUp();
    std::future<void> set_up_future = set_up_promise_->get_future();
    worker_thread_ =
        new MessageLoopThread("WorkerThreadPerformanceTest thread");
    worker_thread_->StartUp();
    worker_thread_->DoInThread(
        FROM_HERE, base::Bind(&std::promise<void>::set_value,
                              base::Unretained(set_up_promise_.get())));
    set_up_future.wait();
  }

  void TearDown() override {
    worker_thread_->ShutDown();
    delete worker_thread_;
    worker_thread_ = nullptr;
    PerformanceTest::TearDown();
  }

  MessageLoopThread* worker_thread_ = nullptr;
};

TEST_F(WorkerThreadPerformanceTest, worker_thread_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();

  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
    worker_thread_->DoInThread(
        FROM_HERE, base::Bind(&callback_batch, bt_msg_queue_, nullptr));
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "WorkerThreadPerformanceTest, " << duration.count() << " ms, "
            << NUM_MESSAGES_TO_SEND << " messages";
}

class LibChromeThreadPerformanceTest : public PerformanceTest {
 protected:
  void SetUp() override {
    PerformanceTest::SetUp();
    std::future<void> set_up_future = set_up_promise_->get_future();
    thread_ = new base::Thread("LibChromeThreadPerformanceTest thread");
    thread_->Start();
    thread_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&std::promise<void>::set_value,
                              base::Unretained(set_up_promise_.get())));
    set_up_future.wait();
  }

  void TearDown() override {
    thread_->Stop();
    delete thread_;
    thread_ = nullptr;
    PerformanceTest::TearDown();
  }

  base::Thread* thread_ = nullptr;
};

TEST_F(LibChromeThreadPerformanceTest, worker_thread_speed_test) {
  g_counter = 0;
  g_counter_promise = std::make_unique<std::promise<void>>();
  std::future<void> counter_future = g_counter_promise->get_future();

  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();

  for (int i = 0; i < NUM_MESSAGES_TO_SEND; i++) {
    fixed_queue_enqueue(bt_msg_queue_, (void*)&g_counter);
    thread_->task_runner()->PostTask(
        FROM_HERE, base::Bind(&callback_batch, bt_msg_queue_, nullptr));
  }
  counter_future.wait();

  std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                            start_time);

  LOG(INFO) << "LibChromeThreadPerformanceTest, " << duration.count() << " ms, "
            << NUM_MESSAGES_TO_SEND << " messages";
}
