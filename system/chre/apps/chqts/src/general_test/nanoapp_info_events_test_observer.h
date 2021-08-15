/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtian a copy of the License at
 *
 *      http://www.apache.org/licensed/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_EVENTS_TEST_OBSERVER_H_
#define _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_EVENTS_TEST_OBSERVER_H_

#include <general_test/test.h>

#include <cstdint>

#include <chre.h>

namespace general_test {

/**
 * Monitor CHRE_EVENT_NANOAPP_STARTED/CHRE_EVENT_NANOAPP_STOPPED events.
 *
 * This is the OBSERVER nanoapp for ContextHubNanoAppInfoEventsNanoAppTest
 *
 * Protocol:
 * Host to observer: NANOAPP_INFO_EVENT, no data
 * observer to Host: CONTINUE
 * [Host starts performer]
 * ...
 * [Host stops performer]
 * Host to observer: CONTINUE, performer's 32-bit instance ID
 * observer to host: SUCCESS
 */
struct HostActionMetadata {
  uint32_t instanceId;
  uint16_t eventType;
};

class NanoAppInfoEventsTestObserver : public Test {
 public:
  NanoAppInfoEventsTestObserver();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void *eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  /**
   * Search the Start/Stop message history looking for a Start/Stop
   * pair from a given instance id
   *
   * @param performerInstanceId The instance Id to look for
   */
  void processStartStopHistory(uint32_t performerInstanceId);

  static constexpr uint32_t kHistorySize = 8;
  HostActionMetadata mStartStopHistory[kHistorySize];
  uint32_t mHistoryIndex = 0;
};

} // namespace general_test

#endif // _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_EVENTS_TEST_OBSERVER_H_
