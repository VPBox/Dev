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

#include <mutex>
#include <string>
#include <thread>

#include "os/reactor.h"
#include "os/utils.h"

namespace bluetooth {
namespace os {

// Reactor-based looper thread implementation. The thread runs immediately after it is constructed, and stops after
// Stop() is invoked. To assign task to this thread, user needs to register a reactable object to the underlying
// reactor.
class Thread {
 public:
  // Used by thread constructor. Suggest the priority to the kernel scheduler. Use REAL_TIME if we need (soft) real-time
  // scheduling guarantee for this thread; use NORMAL if no real-time guarantee is needed to save CPU time slice for
  // other threads
  enum class Priority {
    REAL_TIME,
    NORMAL,
  };

  // name: thread name for POSIX systems
  // priority: priority for kernel scheduler
  Thread(const std::string& name, Priority priority);

  // Stop and destroy this thread
  ~Thread();

  DISALLOW_COPY_AND_ASSIGN(Thread);

  // Stop this thread. Must be invoked from another thread. After this thread is stopped, it cannot be started again.
  bool Stop();

  // Return true if this function is invoked from this thread
  bool IsSameThread() const;

  // Return the POSIX thread name
  std::string GetThreadName() const;

  // Return a user-friendly string representation of this thread object
  std::string ToString() const;

  // Return the pointer of underlying reactor. The ownership is NOT transferred.
  Reactor* GetReactor() const;

 private:
  void run(Priority priority);
  mutable std::mutex mutex_;
  const std::string name_;
  mutable Reactor reactor_;
  std::thread running_thread_;
};

}  // namespace os
}  // namespace bluetooth
