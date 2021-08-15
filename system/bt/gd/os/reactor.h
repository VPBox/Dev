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

#include <sys/epoll.h>
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "os/utils.h"

namespace bluetooth {
namespace os {

// Format of closure to be used in the entire stack
using Closure = std::function<void()>;

// A simple implementation of reactor-style looper.
// When a reactor is running, the main loop is polling and blocked until at least one registered reactable is ready to
// read or write. It will invoke on_read_ready() or on_write_ready(), which is registered with the reactor. Then, it
// blocks again until ready event.
class Reactor {
 public:
  // An object used for Unregister() and ModifyRegistration()
  class Reactable;

  // Construct a reactor on the current thread
  Reactor();

  // Destruct this reactor and release its resources
  ~Reactor();

  DISALLOW_COPY_AND_ASSIGN(Reactor);

  // Start the reactor. The current thread will be blocked until Stop() is invoked and handled.
  void Run();

  // Stop the reactor. Must be invoked from a different thread. Note: all registered reactables will not be unregistered
  // by Stop(). If the reactor is not running, it will be stopped once it's started.
  void Stop();

  // Register a reactable fd to this reactor. Returns a pointer to a Reactable. Caller must use this object to
  // unregister or modify registration. Ownership of the memory space is NOT transferred to user.
  Reactable* Register(int fd, Closure on_read_ready, Closure on_write_ready);

  // Unregister a reactable from this reactor
  void Unregister(Reactable* reactable);

  // Modify the registration for a reactable with given reactable
  void ModifyRegistration(Reactable* reactable, Closure on_read_ready, Closure on_write_ready);

 private:
  mutable std::mutex mutex_;
  int epoll_fd_;
  int control_fd_;
  std::atomic<bool> is_running_;
  std::list<Reactable*> invalidation_list_;
  bool reactable_removed_;
};

}  // namespace os
}  // namespace bluetooth
