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

#include <general_test/test.h>

#include <shared/abort.h>
#include <shared/send_message.h>
#include <shared/time_util.h>

#include <chre.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendFatalFailureToHostUint8;

namespace general_test {

Test::Test(uint32_t minSupportedVersion)
    : mApiVersion(chreGetApiVersion())
      , mIsSupported(mApiVersion >= minSupportedVersion) {
}

void Test::testSetUp(uint32_t messageSize, const void *message) {
  if (mIsSupported) {
    setUp(messageSize, message);
  } else {
    sendMessageToHost(nanoapp_testing::MessageType::kSkipped);
  }
}

void Test::testHandleEvent(uint32_t senderInstanceId, uint16_t eventType,
                           const void *eventData) {
  if (mIsSupported) {
    handleEvent(senderInstanceId, eventType, eventData);
  }
}

void Test::unexpectedEvent(uint16_t eventType) {
  uint32_t localEvent = eventType;
  sendFatalFailureToHost("Test received unexpected event:", &localEvent);
}

void Test::validateChreAsyncResult(const chreAsyncResult *result,
                                   const chreAsyncRequest& request) {
  if (!result->success) {
    sendFatalFailureToHostUint8(
        "chre async result error: %d", result->errorCode);
  }
  if (result->success && result->errorCode != CHRE_ERROR_NONE) {
    sendFatalFailureToHostUint8(
        "Request was successfully processed, but got errorCode: %d",
        result->errorCode);
  }
  if (result->reserved != 0) {
    sendFatalFailureToHostUint8(
        "reserved should be 0, got: %d", result->reserved);
  }
  if (result->cookie != request.cookie) {
    chreLog(CHRE_LOG_ERROR, "Request cookie is %p, got %p",
            request.cookie, result->cookie);
    sendFatalFailureToHost("Request cookie mismatch");
  }
  if (result->requestType != request.requestType) {
    chreLog(CHRE_LOG_ERROR, "Request requestType is %d, got %d",
            request.requestType, result->requestType);
    sendFatalFailureToHost("Request requestType mismatch");
  }
  if (chreGetTime() - request.requestTimeNs > request.timeoutNs) {
    nanoapp_testing::sendFatalFailureToHostUint8(
        "Did not receive chreWifiAsyncEvent within %d seconds.",
        request.timeoutNs / nanoapp_testing::kOneSecondInNanoseconds);
  }
}

const void *Test::getMessageDataFromHostEvent(uint32_t senderInstanceId,
                                              uint16_t eventType, const void* eventData,
                                              nanoapp_testing::MessageType expectedMessageType,
                                              uint32_t expectedMessageSize) {
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    sendFatalFailureToHost("Unexpected sender ID:", &senderInstanceId);
  }
  if (eventType != CHRE_EVENT_MESSAGE_FROM_HOST) {
    unexpectedEvent(eventType);
  }
  if (eventData == nullptr) {
    sendFatalFailureToHost("NULL eventData given");
  }
  auto data = static_cast<const chreMessageFromHostData*>(eventData);
  if (data->reservedMessageType != uint32_t(expectedMessageType)) {
    sendFatalFailureToHost("Unexpected reservedMessageType:",
                           &(data->reservedMessageType));
  }
  if (data->messageSize != expectedMessageSize) {
    sendFatalFailureToHost("Unexpected messageSize:", &(data->messageSize));
  }
  return data->message;
}


}  // namespace general_test
