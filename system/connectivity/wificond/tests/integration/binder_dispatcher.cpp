/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <ctime>

#include <private/binder/Static.h>

#include "binder_dispatcher.h"

using std::chrono::steady_clock;

namespace {
int convertDurationToMillis(const steady_clock::time_point& start,
                            const steady_clock::time_point& end) {
  return static_cast<int>(
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count());
}
}
namespace android {
namespace wificond {
namespace tests {
namespace integration {

BinderDispatcher::BinderDispatcher()
    : event_dispatcher_(new LooperBackedEventLoop()),
      binder_fd_(-1),
      needs_init_(true),
      was_interrupted_(false) {}

BinderDispatcher::~BinderDispatcher() {
  // This is ugly! We want to cleanup the binder driver state at
  // the end of each test (if binder driver was initialized during test).
  // Destroying the instance of |ProcessState| should trigger the cleanup
  // of the binder fd for this process. So, the next test will reinitialize
  // a new instance of |ProcessState| and a new binder fd.
  if (!needs_init_) {
    event_dispatcher_->StopWatchFileDescriptor(binder_fd_);
    gProcess.clear();
  }
}

bool BinderDispatcher::DispatchFor(int timeout_millis) {
  // Initialize the looper and binder if not already done.
  if (needs_init_) {
    Init();
  }

  was_interrupted_ = false;
  steady_clock::time_point start_time = steady_clock::now();
  int millis_left = timeout_millis;
  do {
    event_dispatcher_->PollForOne(millis_left);
    millis_left = timeout_millis -
                  convertDurationToMillis(start_time, steady_clock::now());
  } while (millis_left > 0 && !was_interrupted_);
  return was_interrupted_;
}

void BinderDispatcher::InterruptDispatch() {
  was_interrupted_ = true;
}

void BinderDispatcher::Init() {
  // Initilize the binder fd for polling.
  android::ProcessState::self()->setThreadPoolMaxThreadCount(0);
  android::IPCThreadState::self()->disableBackgroundScheduling(true);
  int err = android::IPCThreadState::self()->setupPolling(&binder_fd_);
  CHECK_EQ(err, 0) << "Error setting up binder polling: " << strerror(-err);
  CHECK_GE(binder_fd_, 0) << "Invalid binder FD: " << binder_fd_;

  auto binder_event_handler =
      std::bind(&BinderDispatcher::OnBinderEvent, this, std::placeholders::_1);
  // Add the binder fd to the looper watch list.
  CHECK(event_dispatcher_->WatchFileDescriptor(
      binder_fd_,
      android::wificond::EventLoop::kModeInput,
      binder_event_handler))
      << "Failed to watch binder FD";
  needs_init_ = false;
}

void BinderDispatcher::OnBinderEvent(int /* fd */) {
  android::IPCThreadState::self()->handlePolledCommands();
}
}  // namespace integration
}  // namespace tests
}  // namespace wificond
}  // namespace android
