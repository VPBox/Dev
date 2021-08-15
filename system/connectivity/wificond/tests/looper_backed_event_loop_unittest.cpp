/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>

#include <memory>

#include <android-base/logging.h>
#include <android-base/unique_fd.h>
#include <gtest/gtest.h>
#include <utils/Errors.h>
#include <utils/StopWatch.h>

#include "wificond/looper_backed_event_loop.h"

namespace {

const int kTimingToleranceMs = 25;

// Adapt from libutils/tests/TestHelpers.h
class Pipe {
public:
  android::base::unique_fd send_fd;
  android::base::unique_fd receive_fd;

  Pipe() {
    int fds[2];
    ::pipe(fds);

    receive_fd = android::base::unique_fd(fds[0]);
    send_fd = android::base::unique_fd(fds[1]);
  }

  bool writeSignal() {
    ssize_t n_written = ::write(send_fd, "*", 1);
    if (n_written != 1) {
      PLOG(ERROR) << "Failed to write signal to pipe";
      return false;
    }
    return true;
  }

  bool readSignal() {
    char buf[1];
    ssize_t n_read = ::read(receive_fd, buf, 1);
    if (n_read != 1) {
      if (n_read == 0) {
        LOG(ERROR) << "No data from pipe";
      } else {
        PLOG(ERROR) << "Failed to read signal from pipe";
      }
      return false;
    }
    return true;
  }
};

}  // namespace

namespace android {
namespace wificond {

class WificondLooperBackedEventLoopTest : public ::testing::Test {
 protected:
  std::unique_ptr<LooperBackedEventLoop> event_loop_;

  virtual void SetUp() {
    event_loop_.reset(new LooperBackedEventLoop());
  }
};

TEST_F(WificondLooperBackedEventLoopTest, LooperBackedEventLoopPostTaskTest) {
  bool task_executed = false;
  event_loop_->PostTask([this, &task_executed]() mutable {
      task_executed = true; event_loop_->TriggerExit();});
  EXPECT_FALSE(task_executed);
  event_loop_->Poll();
  EXPECT_TRUE(task_executed);
}

TEST_F(WificondLooperBackedEventLoopTest,
       LooperBackedEventLoopPostDelayedTaskTest) {
  bool task_executed = false;
  event_loop_->PostDelayedTask([this, &task_executed]() mutable {
      task_executed = true; event_loop_->TriggerExit();}, 500);
  EXPECT_FALSE(task_executed);
  StopWatch stopWatch("DelayedTask");
  event_loop_->Poll();
  int32_t elapsedMillis = ns2ms(stopWatch.elapsedTime());
  EXPECT_NEAR(500, elapsedMillis, kTimingToleranceMs);
  EXPECT_TRUE(task_executed);
}

TEST_F(WificondLooperBackedEventLoopTest, LooperBackedEventLoopWatchFdInputReadyTest) {
  Pipe pipe;
  bool read_result = false;
  bool write_result = false;
  event_loop_->PostTask([&write_result, &pipe]() {write_result = pipe.writeSignal();});
  // Read data from pipe when fd is ready for input.
  EXPECT_TRUE(event_loop_->WatchFileDescriptor(
      pipe.receive_fd,
      EventLoop::kModeInput,
      [&read_result, &pipe, this](int fd) {
          read_result = pipe.readSignal();
          event_loop_->TriggerExit();}));
  event_loop_->Poll();
  EXPECT_EQ(true, read_result);
  EXPECT_EQ(true, write_result);
}

TEST_F(WificondLooperBackedEventLoopTest, LooperBackedEventLoopWatchFdOutputReadyTest) {
  Pipe pipe;
  bool write_result = false;
  // Write data to pipe when fd is ready for output.
  EXPECT_TRUE(event_loop_->WatchFileDescriptor(
      pipe.send_fd,
      EventLoop::kModeOutput,
      [&write_result, &pipe, this](int fd) {
          write_result = pipe.writeSignal();
          event_loop_->TriggerExit();}));
  event_loop_->Poll();
  EXPECT_EQ(true, write_result);
  EXPECT_EQ(true, pipe.readSignal());
  EXPECT_TRUE(event_loop_->StopWatchFileDescriptor(pipe.send_fd));
}

TEST_F(WificondLooperBackedEventLoopTest, LooperBackedEventLoopStopWatchFdTest) {
  Pipe pipe;
  bool read_result = false;
  bool write_result = false;
  event_loop_->PostTask([&write_result, &pipe]() {write_result = pipe.writeSignal();});
  // Read data from pipe when fd is ready for input.
  EXPECT_TRUE(event_loop_->WatchFileDescriptor(
      pipe.receive_fd,
      EventLoop::kModeInput,
      [&read_result, &pipe, this](int fd) {
          read_result = pipe.readSignal();
          event_loop_->TriggerExit();}));
  // Stop watching the file descriptor.
  EXPECT_TRUE(event_loop_->StopWatchFileDescriptor(pipe.receive_fd));
  // If the lambda for |WatchFileDescriptor| is not triggered, we need this to
  // terminate the event loop.
  event_loop_->PostDelayedTask([this]() { event_loop_->TriggerExit();}, 500);
  event_loop_->Poll();
  // We wrote to pipe successfully.
  EXPECT_EQ(true, write_result);
  // No data was read from the pipe because we stopped watching the file
  // descriptor. |read_result| is not set to true;
  EXPECT_EQ(false, read_result);
}

}  // namespace wificond
}  // namespace android
