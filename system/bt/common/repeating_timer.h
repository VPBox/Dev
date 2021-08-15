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

#pragma once

#include <base/bind.h>
#include <base/cancelable_callback.h>
#include <base/location.h>
#include <future>

namespace bluetooth {

namespace common {

class MessageLoopThread;

/**
 * An alarm clock that posts a delayed task to a specified MessageLoopThread
 * periodically.
 *
 * Warning: MessageLoopThread must be running when any task is scheduled or
 * being executed
 */
class RepeatingTimer final {
 public:
  RepeatingTimer() : expected_time_next_task_us_(0) {}
  ~RepeatingTimer();

  /**
   * Schedule a delayed periodic task to the MessageLoopThread. Only one task
   * can be scheduled at a time. If another task is scheduled, it will cancel
   * the previous task synchronously and schedule the new periodic task; this
   * blocks until the previous task is cancelled.
   *
   * @param thread thread to run the task
   * @param from_here location where this task is originated
   * @param task task created through base::Bind()
   * @param period period for the task to be executed
   * @return true iff task is scheduled successfully
   */
  bool SchedulePeriodic(const base::WeakPtr<MessageLoopThread>& thread,
                        const base::Location& from_here,
                        base::RepeatingClosure task, base::TimeDelta period);

  /**
   * Post an event which cancels the current task asynchronously
   */
  void Cancel();

  /**
   * Post an event which cancels the current task and wait for the cancellation
   * to be completed
   */
  void CancelAndWait();

  /**
   * Returns true when there is a pending task scheduled on a running thread,
   * otherwise false.
   */
  bool IsScheduled() const;

 private:
  base::WeakPtr<MessageLoopThread> message_loop_thread_;
  base::CancelableClosure task_wrapper_;
  base::RepeatingClosure task_;
  base::TimeDelta period_;
  uint64_t expected_time_next_task_us_;  // Using clock boot time in time_util.h
  mutable std::recursive_mutex api_mutex_;
  void CancelHelper(std::promise<void> promise);
  void CancelClosure(std::promise<void> promise);

  void RunTask();

  DISALLOW_COPY_AND_ASSIGN(RepeatingTimer);
};

}  // namespace common

}  // namespace bluetooth
