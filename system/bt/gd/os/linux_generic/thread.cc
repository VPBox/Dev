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

#include "os/thread.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <cerrno>
#include <cstring>

#include "os/log.h"

namespace bluetooth {
namespace os {

namespace {
constexpr int kRealTimeFifoSchedulingPriority = 1;
}

Thread::Thread(const std::string& name, const Priority priority)
    : name_(name),
      reactor_(),
      running_thread_(&Thread::run, this, priority) {}

void Thread::run(Priority priority) {
  if (priority == Priority::REAL_TIME) {
    struct sched_param rt_params = {.sched_priority = kRealTimeFifoSchedulingPriority};
    auto linux_tid = static_cast<pid_t>(syscall(SYS_gettid));
    int rc;
    RUN_NO_INTR(rc = sched_setscheduler(linux_tid, SCHED_FIFO, &rt_params));
    if (rc != 0) {
      LOG_ERROR("unable to set SCHED_FIFO priority: %s", strerror(errno));
    }
  }
  reactor_.Run();
}

Thread::~Thread() {
  Stop();
}

bool Thread::Stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  ASSERT(std::this_thread::get_id() != running_thread_.get_id());

  if (!running_thread_.joinable()) {
    return false;
  }
  reactor_.Stop();
  running_thread_.join();
  return true;
}

bool Thread::IsSameThread() const {
  return std::this_thread::get_id() == running_thread_.get_id();
}

Reactor* Thread::GetReactor() const {
  return &reactor_;
}

std::string Thread::GetThreadName() const {
  return name_;
}

std::string Thread::ToString() const {
  return "Thread " + name_;
}

}  // namespace os
}  // namespace bluetooth
