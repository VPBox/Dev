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

#include "message_loop_thread.h"

#include <sys/syscall.h>
#include <unistd.h>
#include <thread>

#include <base/strings/stringprintf.h>

namespace bluetooth {

namespace common {

static constexpr int kRealTimeFifoSchedulingPriority = 1;

MessageLoopThread::MessageLoopThread(const std::string& thread_name)
    : thread_name_(thread_name),
      message_loop_(nullptr),
      run_loop_(nullptr),
      thread_(nullptr),
      thread_id_(-1),
      linux_tid_(-1),
      weak_ptr_factory_(this),
      shutting_down_(false) {}

MessageLoopThread::~MessageLoopThread() { ShutDown(); }

void MessageLoopThread::StartUp() {
  std::promise<void> start_up_promise;
  std::future<void> start_up_future = start_up_promise.get_future();
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    if (thread_ != nullptr) {
      LOG(WARNING) << __func__ << ": thread " << *this << " is already started";

      return;
    }
    thread_ = new std::thread(&MessageLoopThread::RunThread, this,
                              std::move(start_up_promise));
  }
  start_up_future.wait();
}

bool MessageLoopThread::DoInThread(const base::Location& from_here,
                                   base::OnceClosure task) {
  return DoInThreadDelayed(from_here, std::move(task), base::TimeDelta());
}

bool MessageLoopThread::DoInThreadDelayed(const base::Location& from_here,
                                          base::OnceClosure task,
                                          const base::TimeDelta& delay) {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  if (message_loop_ == nullptr) {
    LOG(ERROR) << __func__ << ": message loop is null for thread " << *this
               << ", from " << from_here.ToString();
    return false;
  }
  if (!message_loop_->task_runner()->PostDelayedTask(from_here, std::move(task),
                                                     delay)) {
    LOG(ERROR) << __func__
               << ": failed to post task to message loop for thread " << *this
               << ", from " << from_here.ToString();
    return false;
  }
  return true;
}

void MessageLoopThread::ShutDown() {
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    if (thread_ == nullptr) {
      LOG(INFO) << __func__ << ": thread " << *this << " is already stopped";
      return;
    }
    if (message_loop_ == nullptr) {
      LOG(INFO) << __func__ << ": message_loop_ is null. Already stopping";
      return;
    }
    if (shutting_down_) {
      LOG(INFO) << __func__ << ": waiting for thread to join";
      return;
    }
    shutting_down_ = true;
    CHECK_NE(thread_id_, base::PlatformThread::CurrentId())
        << __func__ << " should not be called on the thread itself. "
        << "Otherwise, deadlock may happen.";
    if (!message_loop_->task_runner()->PostTask(
            FROM_HERE, run_loop_->QuitWhenIdleClosure())) {
      LOG(FATAL) << __func__
                 << ": failed to post task to message loop for thread "
                 << *this;
    }
  }
  thread_->join();
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    delete thread_;
    thread_ = nullptr;
    shutting_down_ = false;
  }
}

base::PlatformThreadId MessageLoopThread::GetThreadId() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return thread_id_;
}

std::string MessageLoopThread::GetName() const {
  return thread_name_;
}

std::string MessageLoopThread::ToString() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return base::StringPrintf("%s(%d)", thread_name_.c_str(), thread_id_);
}

bool MessageLoopThread::IsRunning() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return message_loop_ != nullptr;
}

// Non API method, should not be protected by API mutex
void MessageLoopThread::RunThread(MessageLoopThread* thread,
                                  std::promise<void> start_up_promise) {
  thread->Run(std::move(start_up_promise));
}

base::MessageLoop* MessageLoopThread::message_loop() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return message_loop_;
}

bool MessageLoopThread::EnableRealTimeScheduling() {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  if (!IsRunning()) {
    LOG(ERROR) << __func__ << ": thread " << *this << " is not running";
    return false;
  }
  struct sched_param rt_params = {.sched_priority =
                                      kRealTimeFifoSchedulingPriority};
  int rc = sched_setscheduler(linux_tid_, SCHED_FIFO, &rt_params);
  if (rc != 0) {
    LOG(ERROR) << __func__ << ": unable to set SCHED_FIFO priority "
               << kRealTimeFifoSchedulingPriority << " for linux_tid "
               << std::to_string(linux_tid_) << ", thread " << *this
               << ", error: " << strerror(errno);
    return false;
  }
  return true;
}

base::WeakPtr<MessageLoopThread> MessageLoopThread::GetWeakPtr() {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return weak_ptr_factory_.GetWeakPtr();
}

void MessageLoopThread::Run(std::promise<void> start_up_promise) {
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    LOG(INFO) << __func__ << ": message loop starting for thread "
              << thread_name_;
    base::PlatformThread::SetName(thread_name_);
    message_loop_ = new base::MessageLoop();
    run_loop_ = new base::RunLoop();
    thread_id_ = base::PlatformThread::CurrentId();
    linux_tid_ = static_cast<pid_t>(syscall(SYS_gettid));
    start_up_promise.set_value();
  }

  // Blocking until ShutDown() is called
  run_loop_->Run();

  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    thread_id_ = -1;
    linux_tid_ = -1;
    delete message_loop_;
    message_loop_ = nullptr;
    delete run_loop_;
    run_loop_ = nullptr;
    LOG(INFO) << __func__ << ": message loop finished for thread "
              << thread_name_;
  }
}

}  // namespace common

}  // namespace bluetooth
