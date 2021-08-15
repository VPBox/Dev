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

#ifndef WIFICOND_EVENT_LOOP_H_
#define WIFICOND_EVENT_LOOP_H_

#include <functional>

namespace android {
namespace wificond {

// Abstract class for dispatching tasks.
class EventLoop {
 public:
  enum ReadyMode {
      kModeInput,
      kModeOutput
  };

  virtual ~EventLoop() {}

  // Enqueues a callback.
  // This function can be called on any thread.
  virtual void PostTask(const std::function<void()>& callback) = 0;

  // Enqueues a callback to be processed after a specified period of time.
  // |delay_ms| is delay time in milliseconds. It should not be negative.
  // This function can be called on any thread.
  virtual void PostDelayedTask(const std::function<void()>& callback,
                               int64_t delay_ms) = 0;

  // Monitoring file descriptor for data.
  // Callback will be executed when specific file descriptor is ready.
  // File descriptor is provided as a parameter to this callback:
  // This function can be called on any thread.
  // This returns true upon success and returns false when it failed.
  virtual bool WatchFileDescriptor(
      int fd,
      ReadyMode mode,
      const std::function<void(int)>& callback_) = 0;

  // Stop monitoring file descriptor |fd|.
  // This function can be called on any thread.
  // This returns true upon success and returns false when it failed to
  // remove the file descriptor, or this file descriptor was not registered
  // for watching.
  virtual bool StopWatchFileDescriptor(int fd) = 0;
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_EVENT_LOOP_H_
