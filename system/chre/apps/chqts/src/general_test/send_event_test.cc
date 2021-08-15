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

#include <general_test/send_event_test.h>

#include <cstddef>

#include <shared/abort.h>
#include <shared/array_length.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * In a properly running test, we'll invoke chreSendEvent() a total of 12 times.
 * We initially send eight events upon startup.  And then for each of our four
 * events which has a non-nullptr completeCallback, we call chreSendEvent()
 * from that callback.
 *
 * For our first eight events, they will either be kEventType0 or kEventType1.
 * They will either use completeCallback0 or completeCallback1.  They have
 * various data.  This table describes them all:
 *
 * num | eventType | data       | Callback
 * ----|-----------|------------|---------
 * 0   | 0         | ptr to num | 0
 * 1   | 0         | ptr to num | 1
 * 2   | 1         | ptr to num | 0
 * 3   | 1         | ptr to num | 1
 * 4   | 0         | ptr to num | nullptr
 * 5   | 1         | ptr to num | nullptr
 * 6   | 0         | nullptr    | nullptr
 * 7   | 1         | kOddData   | nullptr
 *
 * The other four events are all kEventTypeCallback with nullptr data and
 * nullptr callback.
 */

constexpr uint16_t kEventType0 = CHRE_EVENT_FIRST_USER_VALUE + 0;
constexpr uint16_t kEventType1 = CHRE_EVENT_FIRST_USER_VALUE + 1;
constexpr uint16_t kEventTypeCallback = CHRE_EVENT_FIRST_USER_VALUE + 2;

// NOTE: This is not allowed to be constexpr, even if some version of g++/clang
//     allow it.
static void *kOddData = reinterpret_cast<void*>(-1);

namespace general_test {

bool SendEventTest::sInMethod = false;
uint8_t SendEventTest::sCallbacksInvoked = 0;

template<uint8_t kCallbackIndex>
void SendEventTest::completeCallback(uint16_t eventType, void *data) {
  if (sInMethod) {
    sendFatalFailureToHost("completeCallback called while another nanoapp "
                           "method is running.");
  }
  sInMethod = true;
  if ((data == nullptr) || (data == kOddData)) {
    sendFatalFailureToHost(
        "completeCallback called with nullptr or odd data.");
  }
  uint32_t num = *(reinterpret_cast<uint32_t*>(data));
  uint16_t expectedEventType = 0xFFFF;
  uint8_t expectedCallbackIndex = 0xFF;
  switch (num) {
    case 0:
      expectedEventType = kEventType0;
      expectedCallbackIndex = 0;
      break;
    case 1:
      expectedEventType = kEventType0;
      expectedCallbackIndex = 1;
      break;
    case 2:
      expectedEventType = kEventType1;
      expectedCallbackIndex = 0;
      break;
    case 3:
      expectedEventType = kEventType1;
      expectedCallbackIndex = 1;
      break;
    default:
      sendFatalFailureToHost("completeCallback given bad data.", &num);
  }
  if (expectedEventType != eventType) {
    sendFatalFailureToHost("completeCallback bad/eventType mismatch.");
  }
  if (expectedCallbackIndex != kCallbackIndex) {
    sendFatalFailureToHost("Incorrect callback function called.");
  }
  uint8_t mask = static_cast<uint8_t>(1 << num);
  if ((sCallbacksInvoked & mask) != 0) {
    sendFatalFailureToHost("Complete callback invoked multiple times for ",
                           &num);
  }
  sCallbacksInvoked |= mask;

  if (!chreSendEvent(kEventTypeCallback, nullptr, nullptr,
                     chreGetInstanceId())) {
    sendFatalFailureToHost("Failed chreSendEvent in callback.");
  }
  sInMethod = false;
}

void SendEventTest::completeCallback0(uint16_t eventType, void *data) {
  completeCallback<0>(eventType, data);
}

void SendEventTest::completeCallback1(uint16_t eventType, void *data) {
  completeCallback<1>(eventType, data);
}

SendEventTest::SendEventTest()
  : Test(CHRE_API_VERSION_1_0) , mNextNum(0) {
}

void SendEventTest::setUp(uint32_t messageSize, const void * /* message */) {
  sInMethod = true;
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "SendEvent message expects 0 additional bytes, got ",
        &messageSize);
  }

  const uint32_t id = chreGetInstanceId();
  for (uint32_t i = 0; i < arrayLength(mData); i++) {
    mData[i] = i;
  }

  // num: 0
  if (!chreSendEvent(kEventType0, &mData[0], completeCallback0, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 0");
  }

  // num: 1
  if (!chreSendEvent(kEventType0, &mData[1], completeCallback1, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 1");
  }

  // num: 2
  if (!chreSendEvent(kEventType1, &mData[2], completeCallback0, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 2");
  }

  // num: 3
  if (!chreSendEvent(kEventType1, &mData[3], completeCallback1, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 3");
  }

  // num: 4
  if (!chreSendEvent(kEventType0, &mData[4], nullptr, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 4");
  }

  // num: 5
  if (!chreSendEvent(kEventType1, &mData[5], nullptr, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 5");
  }

  // num: 6
  if (!chreSendEvent(kEventType0, nullptr, nullptr, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 6");
  }

  // num: 7
  if (!chreSendEvent(kEventType1, kOddData, nullptr, id)) {
    sendFatalFailureToHost("Failed chreSendEvent num 7");
  }

  sInMethod = false;
}

void SendEventTest::handleEvent(uint32_t senderInstanceId,
                                uint16_t eventType, const void* eventData) {
  if (sInMethod) {
    sendFatalFailureToHost("handleEvent invoked while another nanoapp "
                           "method is running");
  }
  sInMethod = true;
  if (senderInstanceId != chreGetInstanceId()) {
    sendFatalFailureToHost("handleEvent got event from unexpected sender:",
                           &senderInstanceId);
  }

  if (mNextNum < 8) {
    void *expectedData;
    if (mNextNum < 6) {
      expectedData = &mData[mNextNum];
    } else if (mNextNum == 6) {
      expectedData = nullptr;
    } else {
      expectedData = kOddData;
    }

    uint16_t expectedEventType = 0xFFFF;
    switch (mNextNum) {
      case 0:
      case 1:
      case 4:
      case 6:
        expectedEventType = kEventType0;
        break;
      case 2:
      case 3:
      case 5:
      case 7:
        expectedEventType = kEventType1;
        break;
    }

    if (expectedEventType != eventType) {
      sendFatalFailureToHost("Incorrect event type sent for num ",
                             &mNextNum);
    }
    if (expectedData != eventData) {
      sendFatalFailureToHost("Incorrect data sent for num ", &mNextNum);
    }

  } else {
    if (eventType != kEventTypeCallback) {
      sendFatalFailureToHost("Unexpected event type for num ", &mNextNum);
    }
    if (mNextNum == 11) {
      // This was our last callback.  Everything is good.
      sendSuccessToHost();
    }
  }

  mNextNum++;
  sInMethod = false;
}

}  // namespace general_test
