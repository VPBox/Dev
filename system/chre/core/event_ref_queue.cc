/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/core/event_ref_queue.h"

#include "chre/platform/assert.h"

namespace chre {

EventRefQueue::~EventRefQueue() {
  CHRE_ASSERT_LOG(empty(), "Potentially leaking events if queue not empty "
                  "when destroyed");
}

bool EventRefQueue::push(Event *event) {
  CHRE_ASSERT(event != nullptr);

  bool pushed = mQueue.push(event);
  if (pushed) {
    event->incrementRefCount();
  }

  return pushed;
}

Event *EventRefQueue::pop() {
  CHRE_ASSERT(!mQueue.empty());

  Event *event = mQueue.front();
  mQueue.pop();
  event->decrementRefCount();

  return event;
}

}  // namespace chre
