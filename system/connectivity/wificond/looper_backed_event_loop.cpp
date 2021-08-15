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

#include "wificond/looper_backed_event_loop.h"

#include <android-base/logging.h>
#include <utils/Looper.h>
#include <utils/Timers.h>

namespace {

class EventLoopCallback : public android::MessageHandler {
 public:
  explicit EventLoopCallback(const std::function<void()>& callback)
      : callback_(callback) {
  }

  ~EventLoopCallback() override = default;

  virtual void handleMessage(const android::Message& message) {
    callback_();
  }

 private:
  const std::function<void()> callback_;

  DISALLOW_COPY_AND_ASSIGN(EventLoopCallback);
};

class WatchFdCallback : public android::LooperCallback {
 public:
  explicit WatchFdCallback(const std::function<void(int)>& callback)
      : callback_(callback) {
  }

  ~WatchFdCallback() override = default;

  virtual int handleEvent(int fd, int events, void* data) {
    callback_(fd);
    // Returning 1 means Looper keeps watching this file descriptor after
    // callback is called.
    // See Looper.h for details.
    return 1;
  }

 private:
  const std::function<void(int)> callback_;

  DISALLOW_COPY_AND_ASSIGN(WatchFdCallback);
};

}  // namespace

namespace android {
namespace wificond {


LooperBackedEventLoop::LooperBackedEventLoop()
    : should_continue_(true) {
  looper_ = android::Looper::prepare(Looper::PREPARE_ALLOW_NON_CALLBACKS);
}

LooperBackedEventLoop::~LooperBackedEventLoop() {
}

void LooperBackedEventLoop::PostTask(const std::function<void()>& callback) {
  sp<android::MessageHandler> event_loop_callback =
      new EventLoopCallback(callback);
  looper_->sendMessage(event_loop_callback, NULL);
}

void LooperBackedEventLoop::PostDelayedTask(
    const std::function<void()>& callback,
    int64_t delay_ms) {
  sp<android::MessageHandler> looper_callback = new EventLoopCallback(callback);
  looper_->sendMessageDelayed(ms2ns(delay_ms), looper_callback, NULL);
}

bool LooperBackedEventLoop::WatchFileDescriptor(
    int fd,
    ReadyMode mode,
    const std::function<void(int)>& callback) {
  sp<android::LooperCallback>  watch_fd_callback = new WatchFdCallback(callback);
  int event;
  if (mode == kModeInput) {
    event = Looper::EVENT_INPUT;
  } else if (mode == kModeOutput) {
    event = Looper::EVENT_OUTPUT;
  } else {
    LOG(ERROR) << "Invalid mode for WatchFileDescriptor().";
    return false;
  }
  // addFd() returns 1 if descriptor was added, 0 if arguments were invalid.
  // Since we are using non-NULL callback, the second parameter 'ident' will
  // always be ignored. It is OK to use 0 for 'ident'.
  // See Looper.h for more details.
  if (looper_->addFd(fd, 0, event, watch_fd_callback, NULL) == 0) {
    LOG(ERROR) << "Invalid arguments for Looper::addFd().";
    return false;
  }
  return true;
}

bool LooperBackedEventLoop::StopWatchFileDescriptor(int fd) {
  if (looper_->removeFd(fd) == 1) {
    return true;
  }
  return false;
}

void LooperBackedEventLoop::Poll() {
  while (should_continue_) {
    looper_->pollOnce(-1);
  }
}

void LooperBackedEventLoop::PollForOne(int timeout_millis) {
  looper_->pollOnce(timeout_millis);
}

void LooperBackedEventLoop::TriggerExit() {
  PostTask([this](){ should_continue_ = false; });
}

}  // namespace wificond
}  // namespace android
