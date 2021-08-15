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

#include <general_test/send_event_stress_test.h>

#include <cstddef>

#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * We stress the system by sending more and more events until it runs out.
 * Then we wait for all the events to be delivered, and all the completion
 * callbacks to be invoked.
 */

constexpr uint16_t kEventType = CHRE_EVENT_FIRST_USER_VALUE;
void *const kEventData = reinterpret_cast<void *>(-1);

// If the system keeps claiming it can send more events, we don't let it
// continue forever.  Instead, we'll cut it off at this limit.  And then
// we'll call its bluff, and make sure that all of these events get
// delivered.  While it won't be an actual exhaustion test (we never took the
// system down to no more events available), it will still give us confidence
// that this CHRE can properly handle any semi-reasonable event load properly.
// 1030 is an arbitrary number, slightly over 2^10.  The hope is this
// balances between catching incorrect behavior and the test taking too long.
constexpr int32_t kMaxEventsToSend = INT32_C(1030);

namespace general_test {

bool SendEventStressTest::sInMethod = false;
bool SendEventStressTest::sInitTime = false;

int32_t SendEventStressTest::sEventsLeft = 0;
int32_t SendEventStressTest::sCompleteCallbacksLeft = 0;

SendEventStressTest::SendEventStressTest()
    : Test(CHRE_API_VERSION_1_0) {
}

void SendEventStressTest::setUp(uint32_t messageSize,
                                const void * /* message */) {
  sInMethod = true;

  if (messageSize != 0) {
    sendFatalFailureToHost(
        "SendEventStress message expects 0 additional bytes, got ",
        &messageSize);
  }

  mInstanceId = chreGetInstanceId();

  // When our chreSendEvent() call fails, the CHRE is allowed to
  // directly invoke our completeCallback.  We special case this
  // with sInitTime, so we can ignore sInMethod for that case only.
  sCompleteCallbacksLeft = 1;
  sInitTime = true;

  // We anticipate most CHREs will not reach kMaxEventsToSend.
  while ((sEventsLeft < kMaxEventsToSend) &&
         chreSendEvent(kEventType, kEventData, completeCallback,
                       mInstanceId)) {
    sEventsLeft++;
  }
  sInitTime = false;

  // We want at least 2 events for this to pretend to be an exhaustion test.
  if (sEventsLeft < 2) {
    sendFatalFailureToHost("Insufficient events available");
  }

  // sCompleteCallbacksLeft may be 0 or 1 at this point.  We don't care.
  // We just know we also expect all the sEventsLeft to have callbacks.
  sCompleteCallbacksLeft += sEventsLeft;

  sInMethod = false;
}

void SendEventStressTest::handleEvent(uint32_t senderInstanceId,
                                      uint16_t eventType,
                                      const void* eventData) {
  if (sInMethod) {
    sendFatalFailureToHost("handleEvent invoked while another nanoapp "
                           "method is running");
  }
  sInMethod = true;
  if (senderInstanceId != mInstanceId) {
    sendFatalFailureToHost("handleEvent got event from unexpected sender:",
                           &senderInstanceId);
  }
  sanityCheck(eventType, eventData, 0);

  --sEventsLeft;
  if (sEventsLeft < 0) {
    sendFatalFailureToHost("Too many events delivered");
  }

  sInMethod = false;
}

void SendEventStressTest::sanityCheck(uint16_t eventType, const void *data,
                                      uint32_t num) {
  if (eventType != kEventType) {
    unexpectedEvent(eventType);
  }
  if (data != kEventData) {
    // 0: handleEvent, 1: completeCallback
    sendFatalFailureToHost("bad event data:", &num);
  }
}


void SendEventStressTest::completeCallback(uint16_t eventType, void *data) {
  if (sInitTime) {
    // The CHRE must be directly calling this from within
    // chreSendEvent(), after it failed.  We only allow a
    // single one of these calls.
    sInitTime = false;
    sanityCheck(eventType, data, 1);
    sCompleteCallbacksLeft--;
    return;
  }

  if (sInMethod) {
    sendFatalFailureToHost("completeCallback invoked while another nanoapp "
                           "method is running");
  }
  sanityCheck(eventType, data, 1);

  --sCompleteCallbacksLeft;
  if (sCompleteCallbacksLeft == 0) {
    if (sEventsLeft != 0) {
      sendFatalFailureToHost("completeCallbacks delivered before events");
    }
    sendSuccessToHost();
  } else if (sCompleteCallbacksLeft < 0) {
    // It's too late for the Host to catch this failure, but perhaps
    // the abort will screw up our unload, and trigger a failure that way.
    sendFatalFailureToHost("completeCallback called too many times");
  }
}


}  // namespace general_test
