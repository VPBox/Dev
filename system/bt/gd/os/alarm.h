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

// A single-shot alarm for reactor-based thread, implemented by Linux timerfd.
// When it's constructed, it will register a reactable on the specified thread; when it's destroyed, it will unregister
// itself from the thread.
class Alarm {
 public:
  // Create and register a single-shot alarm on given thread
  explicit Alarm(Thread* thread);

  // Unregister this alarm from the thread and release resource
  ~Alarm();

  DISALLOW_COPY_AND_ASSIGN(Alarm);

  // Schedule the alarm with given delay
  void Schedule(Closure task, std::chrono::milliseconds delay);

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
