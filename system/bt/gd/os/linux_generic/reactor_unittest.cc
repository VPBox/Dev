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

#include "os/reactor.h"

#include <sys/eventfd.h>
#include <chrono>
#include <future>
#include <thread>

#include "gtest/gtest.h"

namespace bluetooth {
namespace os {
namespace {

constexpr int kReadReadyValue = 100;

std::promise<int>* g_promise;

class ReactorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    g_promise = new std::promise<int>;
    reactor_ = new Reactor;
  }

  void TearDown() override {
    delete g_promise;
    g_promise = nullptr;
    delete reactor_;
    reactor_ = nullptr;
  }

  Reactor* reactor_;
};

class SampleReactable {
 public:
  SampleReactable() : fd_(eventfd(0, EFD_NONBLOCK)) {
    EXPECT_NE(fd_, 0);
  }

  ~SampleReactable() {
    close(fd_);
  }

  void OnReadReady() {}

  void OnWriteReady() {}

  int fd_;
};

class FakeReactable {
 public:
  enum EventFdValue {
    kSetPromise = 1,
    kRegisterSampleReactable,
    kUnregisterSampleReactable,
    kSampleOutputValue,
  };
  FakeReactable() : fd_(eventfd(0, 0)), reactor_(nullptr) {
    EXPECT_NE(fd_, 0);
  }

  FakeReactable(Reactor* reactor) : fd_(eventfd(0, 0)), reactor_(reactor) {
    EXPECT_NE(fd_, 0);
  }

  ~FakeReactable() {
    close(fd_);
  }

  void OnReadReady() {
    uint64_t value = 0;
    auto read_result = eventfd_read(fd_, &value);
    EXPECT_EQ(read_result, 0);
    if (value == kSetPromise && g_promise != nullptr) {
      g_promise->set_value(kReadReadyValue);
    }
    if (value == kRegisterSampleReactable) {
      reactable_ = reactor_->Register(sample_reactable_.fd_, [this] { this->sample_reactable_.OnReadReady(); },
                                      [this] { this->sample_reactable_.OnWriteReady(); });
      g_promise->set_value(kReadReadyValue);
    }
    if (value == kUnregisterSampleReactable) {
      reactor_->Unregister(reactable_);
      g_promise->set_value(kReadReadyValue);
    }
  }

  void OnWriteReady() {
    auto write_result = eventfd_write(fd_, output_data_);
    output_data_ = 0;
    EXPECT_EQ(write_result, 0);
  }

  SampleReactable sample_reactable_;
  Reactor::Reactable* reactable_ = nullptr;
  int fd_;

 private:
  Reactor* reactor_;
  uint64_t output_data_ = kSampleOutputValue;
};

TEST_F(ReactorTest, start_and_stop) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  reactor_->Stop();
  reactor_thread.join();
}

TEST_F(ReactorTest, stop_and_start) {
  auto reactor_thread = std::thread(&Reactor::Stop, reactor_);
  auto another_thread = std::thread(&Reactor::Run, reactor_);
  reactor_thread.join();
  another_thread.join();
}

TEST_F(ReactorTest, stop_multi_times) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  for (int i = 0; i < 5; i++) {
    reactor_->Stop();
  }
  reactor_thread.join();
}

TEST_F(ReactorTest, cold_register_only) {
  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);

  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, cold_register) {
  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  auto future = g_promise->get_future();

  auto write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kSetPromise);
  EXPECT_EQ(write_result, 0);
  EXPECT_EQ(future.get(), kReadReadyValue);
  reactor_->Stop();
  reactor_thread.join();
  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, hot_register_from_different_thread) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  auto future = g_promise->get_future();

  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  auto write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kSetPromise);
  EXPECT_EQ(write_result, 0);
  EXPECT_EQ(future.get(), kReadReadyValue);
  reactor_->Stop();
  reactor_thread.join();

  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, hot_unregister_from_different_thread) {
  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  reactor_->Unregister(reactable);
  auto future = g_promise->get_future();

  auto write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kSetPromise);
  EXPECT_EQ(write_result, 0);
  future.wait_for(std::chrono::milliseconds(10));
  g_promise->set_value(2);
  EXPECT_EQ(future.get(), 2);
  reactor_->Stop();
  reactor_thread.join();
}

TEST_F(ReactorTest, hot_register_from_same_thread) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  auto future = g_promise->get_future();

  FakeReactable fake_reactable(reactor_);
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  auto write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kRegisterSampleReactable);
  EXPECT_EQ(write_result, 0);
  EXPECT_EQ(future.get(), kReadReadyValue);
  reactor_->Stop();
  reactor_thread.join();

  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, hot_unregister_from_same_thread) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  auto future = g_promise->get_future();

  FakeReactable fake_reactable(reactor_);
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  auto write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kRegisterSampleReactable);
  EXPECT_EQ(write_result, 0);
  EXPECT_EQ(future.get(), kReadReadyValue);
  delete g_promise;
  g_promise = new std::promise<int>;
  future = g_promise->get_future();
  write_result = eventfd_write(fake_reactable.fd_, FakeReactable::kUnregisterSampleReactable);
  EXPECT_EQ(write_result, 0);
  EXPECT_EQ(future.get(), kReadReadyValue);
  reactor_->Stop();
  reactor_thread.join();

  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, start_and_stop_multi_times) {
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  reactor_->Stop();
  reactor_thread.join();
  for (int i = 0; i < 5; i++) {
    reactor_thread = std::thread(&Reactor::Run, reactor_);
    reactor_->Stop();
    reactor_thread.join();
  }
}

TEST_F(ReactorTest, on_write_ready) {
  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, nullptr, std::bind(&FakeReactable::OnWriteReady, &fake_reactable));
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  uint64_t value = 0;
  auto read_result = eventfd_read(fake_reactable.fd_, &value);
  EXPECT_EQ(read_result, 0);
  EXPECT_EQ(value, FakeReactable::kSampleOutputValue);

  reactor_->Stop();
  reactor_thread.join();

  reactor_->Unregister(reactable);
}

TEST_F(ReactorTest, modify_registration) {
  FakeReactable fake_reactable;
  auto* reactable =
      reactor_->Register(fake_reactable.fd_, std::bind(&FakeReactable::OnReadReady, &fake_reactable), nullptr);
  reactor_->ModifyRegistration(reactable, nullptr, std::bind(&FakeReactable::OnWriteReady, &fake_reactable));
  auto reactor_thread = std::thread(&Reactor::Run, reactor_);
  uint64_t value = 0;
  auto read_result = eventfd_read(fake_reactable.fd_, &value);
  EXPECT_EQ(read_result, 0);
  EXPECT_EQ(value, FakeReactable::kSampleOutputValue);

  reactor_->Stop();
  reactor_thread.join();
}

}  // namespace
}  // namespace os
}  // namespace bluetooth
