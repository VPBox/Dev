/*
 * Copyright 2018 The Android Open Source Project
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

#include "once_timer.h"

#include "message_loop_thread.h"
#include "time_util.h"

namespace bluetooth {

namespace common {

// This runs on user thread
OnceTimer::~OnceTimer() {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  if (message_loop_thread_ != nullptr && message_loop_thread_->IsRunning()) {
    CancelAndWait();
  }
}

// This runs on user thread
bool OnceTimer::Schedule(const base::WeakPtr<MessageLoopThread>& thread,
                         const base::Location& from_here,
                         base::OnceClosure task, base::TimeDelta delay) {
  uint64_t time_now_us = time_get_os_boottime_us();
  uint64_t time_next_task_us = time_now_us + delay.InMicroseconds();
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  if (thread == nullptr) {
    LOG(ERROR) << __func__ << ": thread must be non-null";
    return false;
  }
  CancelAndWait();
  task_ = std::move(task);
  task_wrapper_.Reset(
      base::BindOnce(&OnceTimer::RunTask, base::Unretained(this)));
  message_loop_thread_ = thread;
  delay_ = delay;
  uint64_t time_until_next_us = time_next_task_us - time_get_os_boottime_us();
  if (!thread->DoInThreadDelayed(
          from_here, task_wrapper_.callback(),
          base::TimeDelta::FromMicroseconds(time_until_next_us))) {
    LOG(ERROR) << __func__
               << ": failed to post task to message loop for thread " << *thread
               << ", from " << from_here.ToString();
    task_wrapper_.Cancel();
    message_loop_thread_ = nullptr;
    delay_ = {};
    return false;
  }
  return true;
}

// This runs on user thread
void OnceTimer::Cancel() {
  std::promise<void> promise;
  CancelHelper(std::move(promise));
}

// This runs on user thread
void OnceTimer::CancelAndWait() {
  std::promise<void> promise;
  auto future = promise.get_future();
  CancelHelper(std::move(promise));
  future.wait();
}

// This runs on user thread
void OnceTimer::CancelHelper(std::promise<void> promise) {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  MessageLoopThread* scheduled_thread = message_loop_thread_.get();
  if (scheduled_thread == nullptr) {
    promise.set_value();
    return;
  }
  if (scheduled_thread->GetThreadId() == base::PlatformThread::CurrentId()) {
    CancelClosure(std::move(promise));
    return;
  }
  scheduled_thread->DoInThread(
      FROM_HERE, base::BindOnce(&OnceTimer::CancelClosure,
                                base::Unretained(this), std::move(promise)));
}

// This runs on message loop thread
void OnceTimer::CancelClosure(std::promise<void> promise) {
  message_loop_thread_ = nullptr;
  task_wrapper_.Cancel();
  std::move(task_);
  delay_ = base::TimeDelta();
  promise.set_value();
}

// This runs on user thread
bool OnceTimer::IsScheduled() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return message_loop_thread_ != nullptr && message_loop_thread_->IsRunning();
}

// This runs on message loop thread
void OnceTimer::RunTask() {
  if (message_loop_thread_ == nullptr || !message_loop_thread_->IsRunning()) {
    LOG(ERROR) << __func__
               << ": message_loop_thread_ is null or is not running";
    return;
  }
  CHECK_EQ(message_loop_thread_->GetThreadId(),
           base::PlatformThread::CurrentId())
      << ": task must run on message loop thread";

  task_wrapper_.Cancel();
  std::move(task_).Run();
  message_loop_thread_ = nullptr;
}

}  // namespace common

}  // namespace bluetooth
