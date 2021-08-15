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

#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "os/thread.h"
#include "os/utils.h"

namespace bluetooth {
namespace os {

// A repeating alarm for reactor-based thread, implemented by Linux timerfd.
// When it's constructed, it will register a reactable on the specified thread; when it's destroyed, it will unregister
// itself from the thread.
class RepeatingAlarm {
 public:
  // Create and register a repeating alarm on given thread
  explicit RepeatingAlarm(Thread* thread);

  // Unregister this alarm from the thread and release resource
  ~RepeatingAlarm();

  DISALLOW_COPY_AND_ASSIGN(RepeatingAlarm);

  // Schedule a repeating alarm with given period
  void Schedule(Closure task, std::chrono::milliseconds period);

  // Cancel the alarm. No-op if it's not armed.
  void Cancel();

 private:
  Closure task_;
  Thread* thread_;
  int fd_ = 0;
  Reactor::Reactable* token_;
  mutable std::mutex mutex_;
  void on_fire();
};

}  // namespace os
}  // namespace bluetooth
