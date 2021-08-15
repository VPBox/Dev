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

#include "os/thread.h"

#include <sys/eventfd.h>

#include "gtest/gtest.h"
#include "os/reactor.h"

namespace bluetooth {
namespace os {
namespace {

constexpr int kCheckIsSameThread = 1;

class SampleReactable {
 public:
  explicit SampleReactable(Thread* thread) : thread_(thread), fd_(eventfd(0, 0)), is_same_thread_checked_(false) {
    EXPECT_NE(fd_, 0);
  }

  ~SampleReactable() {
    close(fd_);
  }

  void OnReadReady() {
    EXPECT_TRUE(thread_->IsSameThread());
    is_same_thread_checked_ = true;
    uint64_t val;
    eventfd_read(fd_, &val);
  }

  bool IsSameThreadCheckDone() {
    return is_same_thread_checked_;
  }

  Thread* thread_;
  int fd_;
  bool is_same_thread_checked_;
};

class ThreadTest : public ::testing::Test {
 protected:
  void SetUp() override {
    thread = new Thread("test", Thread::Priority::NORMAL);
  }

  void TearDown() override {
    delete thread;
  }
  Thread* thread = nullptr;
};

TEST_F(ThreadTest, just_stop_no_op) {
  thread->Stop();
}

TEST_F(ThreadTest, thread_name) {
  EXPECT_EQ(thread->GetThreadName(), "test");
}

TEST_F(ThreadTest, thread_to_string) {
  EXPECT_NE(thread->ToString().find("test"), std::string::npos);
}

TEST_F(ThreadTest, not_same_thread) {
  EXPECT_FALSE(thread->IsSameThread());
}

TEST_F(ThreadTest, same_thread) {
  Reactor* reactor = thread->GetReactor();
  SampleReactable sample_reactable(thread);
  auto* reactable =
      reactor->Register(sample_reactable.fd_, std::bind(&SampleReactable::OnReadReady, &sample_reactable), nullptr);
  int fd = sample_reactable.fd_;
  int write_result = eventfd_write(fd, kCheckIsSameThread);
  EXPECT_EQ(write_result, 0);
  while (!sample_reactable.IsSameThreadCheckDone()) std::this_thread::yield();
  reactor->Unregister(reactable);
}

}  // namespace
}  // namespace os
}  // namespace bluetooth
