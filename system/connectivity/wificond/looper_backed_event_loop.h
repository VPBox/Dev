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

#ifndef WIFICOND_LOOPER_BACKED_EVENT_LOOP_H_
#define WIFICOND_LOOPER_BACKED_EVENT_LOOP_H_

#include "event_loop.h"

#include <android-base/macros.h>
#include <utils/Looper.h>

namespace android {
namespace wificond {

class LooperBackedEventLoop: public EventLoop {
 public:
  LooperBackedEventLoop();
  ~LooperBackedEventLoop() override;

  // See event_loop.h
  void PostTask(const std::function<void()>& callback) override;

  // See event_loop.h
  void PostDelayedTask(const std::function<void()>& callback,
                       int64_t delay_ms) override;
  // See event_loop.h
  bool WatchFileDescriptor(
      int fd,
      ReadyMode mode,
      const std::function<void(int)>& callback) override;

  // See event_loop.h
  bool StopWatchFileDescriptor(int fd) override;

  // Performs all pending callbacks and waiting for new events until
  // TriggerExit() is called.
  // This method can be called from any thread context.
  void Poll();

  // Blocks for |timeout_millis| for the next event.
  // This method can be called from any thread context.
  void PollForOne(int timeout_millis);

  // Posts a task to stop event loop polling.
  // This method can be called from any thread context.
  void TriggerExit();

 private:
  sp<android::Looper> looper_;
  bool should_continue_;

  DISALLOW_COPY_AND_ASSIGN(LooperBackedEventLoop);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_LOOPER_BACKED_EVENT_LOOP_H_
