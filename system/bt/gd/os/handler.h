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
#include <queue>

#include "os/thread.h"
#include "os/utils.h"

namespace bluetooth {
namespace os {

// A message-queue style handler for reactor-based thread to handle incoming events from different threads. When it's
// constructed, it will register a reactable on the specified thread; when it's destroyed, it will unregister itself
// from the thread.
class Handler {
 public:
  // Create and register a handler on given thread
  explicit Handler(Thread* thread);

  // Unregister this handler from the thread and release resource. Unhandled events will be discarded and not executed.
  ~Handler();

  DISALLOW_COPY_AND_ASSIGN(Handler);

  // Enqueue a closure to the queue of this handler
  void Post(Closure closure);

  // Remove all pending events from the queue of this handler
  void Clear();

 private:
  std::queue<Closure> tasks_;
  Thread* thread_;
  int fd_;
  Reactor::Reactable* reactable_;
  mutable std::mutex mutex_;
  void handle_next_event();
};

}  // namespace os
}  // namespace bluetooth
