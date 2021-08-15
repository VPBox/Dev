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

#include <general_test/event_between_apps_test.h>

#include <general_test/nanoapp_info.h>

#include <cstddef>

#include <shared/abort.h>
#include <shared/nano_endian.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::MessageType;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

// Arbitrary, just to confirm our data is properly sent.
const uint32_t EventBetweenApps0::kMagic = UINT32_C(0x51501984);

EventBetweenApps0::EventBetweenApps0()
    : Test(CHRE_API_VERSION_1_0), mContinueCount(0) {
}

void EventBetweenApps0::setUp(uint32_t messageSize,
                              const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "Initial message expects 0 additional bytes, got ",
        &messageSize);
  }

  NanoappInfo info;
  info.sendToHost();
}

void EventBetweenApps0::handleEvent(uint32_t senderInstanceId,
                                    uint16_t eventType, const void* eventData) {
  uint32_t app1InstanceId;
  const void *message =
      getMessageDataFromHostEvent(senderInstanceId, eventType, eventData,
                                  MessageType::kContinue,
                                  sizeof(app1InstanceId));
  if (mContinueCount > 0) {
    sendFatalFailureToHost("Multiple kContinue messages sent");
  }

  mContinueCount++;
  nanoapp_testing::memcpy(&app1InstanceId, message, sizeof(app1InstanceId));
  app1InstanceId = nanoapp_testing::littleEndianToHost(app1InstanceId);
  // It's safe to strip the 'const' because we're using nullptr for our
  // free callback.
  uint32_t *sendData = const_cast<uint32_t*>(&kMagic);
  // Send an event to app1.  Note since app1 is on the same system, there are
  // no endian concerns for our sendData.
  chreSendEvent(kEventType, sendData, nullptr, app1InstanceId);
}

EventBetweenApps1::EventBetweenApps1()
  : Test(CHRE_API_VERSION_1_0)
    , mApp0InstanceId(CHRE_INSTANCE_ID)
    , mReceivedInstanceId(CHRE_INSTANCE_ID) {
}

void EventBetweenApps1::setUp(uint32_t messageSize,
                              const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "Initial message expects 0 additional bytes, got ",
        &messageSize);
  }

  NanoappInfo appInfo;
  appInfo.sendToHost();
}

void EventBetweenApps1::handleEvent(uint32_t senderInstanceId,
                                    uint16_t eventType, const void* eventData) {
  if (eventType == CHRE_EVENT_MESSAGE_FROM_HOST) {
    const void *message =
        getMessageDataFromHostEvent(senderInstanceId, eventType, eventData,
                                    MessageType::kContinue,
                                    sizeof(mApp0InstanceId));
    // We expect kContinue once, with the app0's instance ID as data.
    if (mApp0InstanceId != CHRE_INSTANCE_ID) {
      // We know app0's instance ID can't be CHRE_INSTANCE_ID, otherwise
      // we would have aborted this test in commonInit().
      sendFatalFailureToHost("Multiple kContinue messages from host.");
    }
    nanoapp_testing::memcpy(&mApp0InstanceId, message,
                            sizeof(mApp0InstanceId));
    mApp0InstanceId = nanoapp_testing::littleEndianToHost(mApp0InstanceId);

  } else if (eventType == EventBetweenApps0::kEventType) {
    if (mReceivedInstanceId != CHRE_INSTANCE_ID) {
      sendFatalFailureToHost("Multiple messages from other nanoapp.");
    }
    if (senderInstanceId == CHRE_INSTANCE_ID) {
      sendFatalFailureToHost("Received event from other nanoapp with "
                             "CHRE_INSTANCE_ID for sender");
    }
    mReceivedInstanceId = senderInstanceId;
    uint32_t magic;
    nanoapp_testing::memcpy(&magic, eventData, sizeof(magic));
    if (magic != EventBetweenApps0::kMagic) {
      sendFatalFailureToHost("Got incorrect magic data: ", &magic);
    }

  } else {
    unexpectedEvent(eventType);
  }

  if ((mApp0InstanceId != CHRE_INSTANCE_ID)
      && (mReceivedInstanceId != CHRE_INSTANCE_ID)) {
    if (mApp0InstanceId == mReceivedInstanceId) {
      sendSuccessToHost();
    } else {
      sendFatalFailureToHost("Got bad sender instance ID for nanoapp "
                             "event: ", &mReceivedInstanceId);
    }
  }
}

}  // namespace general_test
