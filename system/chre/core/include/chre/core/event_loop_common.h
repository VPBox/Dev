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

#ifndef CHRE_CORE_EVENT_LOOP_COMMON_H_
#define CHRE_CORE_EVENT_LOOP_COMMON_H_

#include "chre_api/chre/event.h"

#include <cstdint>

namespace chre {

//! An identifier for a system callback, which is mapped into a CHRE event type
//! in the user-defined range.
enum class SystemCallbackType : uint16_t {
  FirstCallbackType = CHRE_EVENT_FIRST_USER_VALUE,

  MessageToHostComplete,
  WifiScanMonitorStateChange,
  WifiRequestScanResponse,
  WifiHandleScanEvent,
  NanoappListResponse,
  SensorLastEventUpdate,
  FinishLoadingNanoapp,
  WwanHandleCellInfoResult,
  HandleUnloadNanoapp,
  GnssSessionStatusChange,
  SensorStatusUpdate,
  PerformDebugDump,
  TimerPoolTick,
  AudioHandleDataEvent,
  WifiHandleFailedRanging,
  WifiHandleRangingEvent,
  AudioAvailabilityChange,
  AudioHandleHostAwake,
  SensorFlushComplete,
  SensorFlushTimeout,
  SensorStatusInfoResponse,
};

//! The function signature of a system callback mirrors the CHRE event free
//! callback to allow it to use the same event infrastructure.
typedef chreEventCompleteFunction SystemCallbackFunction;

/**
 * Generic event free callback that can be used by any event where the event
 * data is allocated via memoryAlloc, and no special processing is needed in the
 * event complete callback other than freeing the event data.
 */
void freeEventDataCallback(uint16_t eventType, void *eventData);

}  // namespace chre

#endif  // CHRE_CORE_EVENT_LOOP_COMMON_H_
