/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_EVENT_REF_QUEUE_H
#define CHRE_EVENT_REF_QUEUE_H

#include "chre/core/event.h"
#include "chre/util/array_queue.h"

namespace chre {

/**
 * A non-thread-safe, non-blocking wrapper around ArrayQueue that stores Event*
 * and manages the Event reference counter.
 * TODO: make this a template specialization? Or rework the ref count design?
 */
class EventRefQueue {
 public:
  ~EventRefQueue();

  /**
   * @return true if there are no events in the queue
   */
  bool empty() const {
    return mQueue.empty();
  }

  /**
   * Adds an event to the queue, and increments its reference counter
   *
   * @param event The event to add
   * @return true on success
   */
  bool push(Event *event);

  /**
   * Removes the oldest event from the queue, and decrements its reference
   * counter. Does not trigger freeing of the event if the reference count
   * reaches 0 as a result of this function call. The queue must be non-empty as
   * a precondition to calling this function, or undefined behavior will result.
   *
   * @return Pointer to the next event in the queue
   */
  Event *pop();

 private:
  //! The maximum number of events that can be outstanding for an app.
  static constexpr size_t kMaxPendingEvents = 16;

  //! The queue of incoming events.
  ArrayQueue<Event *, kMaxPendingEvents> mQueue;
};

}  // namespace chre

#endif  // CHRE_EVENT_REF_QUEUE_H
