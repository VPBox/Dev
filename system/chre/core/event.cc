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

#include "chre/core/event.h"


namespace chre {

Event::Event(uint16_t eventType_, uint16_t receivedTimeMillis_,
             void *eventData_, chreEventCompleteFunction *freeCallback_,
             uint32_t senderInstanceId_, uint32_t targetInstanceId_)
    : eventType(eventType_), receivedTimeMillis(receivedTimeMillis_),
      eventData(eventData_), freeCallback(freeCallback_),
      senderInstanceId(senderInstanceId_),
      targetInstanceId(targetInstanceId_) {}

}  // namespace chre
