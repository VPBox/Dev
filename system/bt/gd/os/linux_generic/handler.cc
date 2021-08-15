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

#include "os/handler.h"

#include <sys/eventfd.h>
#include <cstring>
#include <unistd.h>

#include "os/log.h"
#include "os/reactor.h"
#include "os/utils.h"

#ifndef EFD_SEMAPHORE
#define EFD_SEMAPHORE 1
#endif

namespace bluetooth {
namespace os {

Handler::Handler(Thread* thread)
  : thread_(thread),
    fd_(eventfd(0, EFD_SEMAPHORE | EFD_NONBLOCK)) {
  ASSERT(fd_ != -1);

  reactable_ = thread_->GetReactor()->Register(fd_, [this] { this->handle_next_event(); }, nullptr);
}

Handler::~Handler() {
  thread_->GetReactor()->Unregister(reactable_);
  reactable_ = nullptr;

  int close_status;
  RUN_NO_INTR(close_status = close(fd_));
  ASSERT(close_status != -1);
}

void Handler::Post(Closure closure) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.emplace(std::move(closure));
  }
  uint64_t val = 1;
  auto write_result = eventfd_write(fd_, val);
  ASSERT(write_result != -1);
}

void Handler::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);

  std::queue<Closure> empty;
  std::swap(tasks_, empty);

  uint64_t val;
  while (eventfd_read(fd_, &val) == 0) {
  }
}

void Handler::handle_next_event() {
  Closure closure;
  uint64_t val = 0;
  auto read_result = eventfd_read(fd_, &val);
  if (read_result == -1 && errno == EAGAIN) {
    // We were told there was an item, but it was removed before we got there
    // (aka the queue was cleared). Not a fatal error, so just bail.
    return;
  }

  ASSERT(read_result != -1);

  {
    std::lock_guard<std::mutex> lock(mutex_);
    closure = std::move(tasks_.front());
    tasks_.pop();
  }
  closure();
}

}  // namespace os
}  // namespace bluetooth
