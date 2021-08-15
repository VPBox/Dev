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

#ifndef CHRE_CORE_EVENT_H_
#define CHRE_CORE_EVENT_H_

#include "chre_api/chre/event.h"
#include "chre/platform/assert.h"
#include "chre/util/non_copyable.h"

#include <cstdint>

namespace chre {

//! Instance ID used for events sent by the system
constexpr uint32_t kSystemInstanceId = 0;

//! Target instance ID used to deliver a message to all nanoapps registered for
//! the event
constexpr uint32_t kBroadcastInstanceId = UINT32_MAX;

//! This value can be used in a nanoapp's own instance ID to indicate that the
//! ID is invalid/not assigned yet
constexpr uint32_t kInvalidInstanceId = kBroadcastInstanceId;

class Event : public NonCopyable {
 public:
  Event(uint16_t eventType_, uint16_t receivedTimeMillis_, void *eventData_,
        chreEventCompleteFunction *freeCallback_,
        uint32_t senderInstanceId = kSystemInstanceId,
        uint32_t targetInstanceId = kBroadcastInstanceId);

  void incrementRefCount() {
    mRefCount++;
    CHRE_ASSERT(mRefCount != 0);
  }

  void decrementRefCount() {
    CHRE_ASSERT(mRefCount > 0);
    mRefCount--;
  }

  bool isUnreferenced() const {
    return (mRefCount == 0);
  }

  const uint16_t eventType;
  //! This value can serve as a proxy for how fast CHRE is processing events
  //! in its queue by substracting the newest event timestamp by the oldest one.
  const uint16_t receivedTimeMillis;
  void * const eventData;
  chreEventCompleteFunction * const freeCallback;
  const uint32_t senderInstanceId;
  const uint32_t targetInstanceId;

 private:
  size_t mRefCount = 0;
};

}

#endif  // CHRE_CORE_EVENT_H_
