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

#include <general_test/nanoapp_info_events_test_observer.h>

#include <shared/nano_endian.h>
#include <shared/nano_string.h>

namespace general_test {

NanoAppInfoEventsTestObserver::NanoAppInfoEventsTestObserver()
    : Test(CHRE_API_VERSION_1_1) {
}

void NanoAppInfoEventsTestObserver::setUp(uint32_t /* messageSize */,
                                          const void * /* message */) {
  chreConfigureNanoappInfoEvents(true /* enable */);
  nanoapp_testing::sendMessageToHost(nanoapp_testing::MessageType::kContinue);
}

void NanoAppInfoEventsTestObserver::handleEvent(uint32_t senderInstanceId,
                                                uint16_t eventType,
                                                const void *eventData) {
  if ((senderInstanceId == CHRE_INSTANCE_ID)
      && ((eventType == CHRE_EVENT_NANOAPP_STARTED)
          || (eventType == CHRE_EVENT_NANOAPP_STOPPED))) {

    const struct chreNanoappInfo *nanoAppInfo =
        static_cast<const struct chreNanoappInfo *>(eventData);

    mStartStopHistory[mHistoryIndex].instanceId =
        nanoAppInfo->instanceId;

    mStartStopHistory[mHistoryIndex].eventType = eventType;
    mHistoryIndex = (mHistoryIndex + 1) % kHistorySize;
  } else if ((senderInstanceId == CHRE_INSTANCE_ID)
             && (eventType == CHRE_EVENT_MESSAGE_FROM_HOST)) {
    uint32_t performerInstanceId;

    const void *message = getMessageDataFromHostEvent(
        senderInstanceId, eventType, eventData,
        nanoapp_testing::MessageType::kContinue, sizeof(performerInstanceId));

    nanoapp_testing::memcpy(&performerInstanceId, message,
                            sizeof(performerInstanceId));
    performerInstanceId =
        nanoapp_testing::littleEndianToHost(performerInstanceId);

    processStartStopHistory(performerInstanceId);
  } else {
    unexpectedEvent(eventType);
  }
}

void NanoAppInfoEventsTestObserver::processStartStopHistory(
    uint32_t performerInstanceId) {
  uint32_t startCount = 0;
  uint32_t stopCount = 0;
  bool seenFirstEvent = false;
  bool eventsOrdered = false;

  // The oldest data (if present) is at the insertion point in the
  // circular array (i.e. mHistoryIndex)
  for (uint32_t i = 0; i < kHistorySize; ++i) {
    HostActionMetadata& data =
        mStartStopHistory[(mHistoryIndex + i) % kHistorySize];

    if (data.instanceId == performerInstanceId) {
      if (data.eventType == CHRE_EVENT_NANOAPP_STARTED) {
        ++startCount;
      } else {
        ++stopCount;
      }

      if (!seenFirstEvent) {
        eventsOrdered = (data.eventType == CHRE_EVENT_NANOAPP_STARTED);
        seenFirstEvent = true;
      }
    }
  }

  if (startCount > 1) {
    nanoapp_testing::sendFatalFailureToHost(
        "Received too many Start events");
  } else if (startCount == 0) {
    nanoapp_testing::sendFatalFailureToHost(
        "Did not receive Start event");
  } else if (stopCount > 1) {
    nanoapp_testing::sendFatalFailureToHost(
        "Received too many Stop events");
  } else if (stopCount == 0) {
    nanoapp_testing::sendFatalFailureToHost(
        "Did not receive Stop event");
  } else if (!eventsOrdered) {
    nanoapp_testing::sendFatalFailureToHost(
        "Start and Stop events were not in order");
  } else {
    nanoapp_testing::sendSuccessToHost();
  }
}

} // namespace general_test
