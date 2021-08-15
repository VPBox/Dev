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

#include <general_test/send_message_to_host_test.h>

#include <cinttypes>
#include <cstddef>

#include <shared/nano_endian.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::MessageType;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendInternalFailureToHost;
using nanoapp_testing::sendSuccessToHost;


/*
 * Our test essentially has nine stages.  The first eight stages all involve
 * sending data to the Host.  Here is a table describing them:
 *
 * Stage | Data length | Callback
 * ------|-------------|--------------
 * 0     | small       | smallMessage0
 * 1     | small       | smallMessage1
 * 2     | small       | nullptr
 * 3     | small       | smallMessage0
 * 4     | nullptr     | nullptr
 * 5     | 4 bytes     | nullptr
 * 6     | MAX + 1     | largeMessage
 * 7     | MAX         | largeMessage
 *
 * Stage 8 involves waiting for an incoming zero-sized message from the Host.
 *
 * The focus of the first four stages is making sure the correct callback
 * gets invoked and a nullptr callback works.
 *
 * Stage 4 tests sending a null message to the Host (that should send).
 *
 * Stage 5 is not testing anything, but it's necessary to get data
 * to the host to confirm the message in stage 7 is correct.
 *
 * Stage 6 tests that we properly reject oversized messages.  This
 * data should _not_ make it to the host.
 *
 * Stage 7 tests that we can send the maximum claimed size to the host.
 *
 * Every single stage which has a non-null callback is not considered a
 * "success" until that callback has been invoked.  There is no CHRE
 * requirement in terms of the order in which these callbacks are
 * invoked, which is why the markSuccess() method uses a bitmask and
 * checks for overall success every time we gets success from a single
 * stage.
 *
 * We consider the test successful only when all stages have reported success.
 * Note that the Host will not perform Stage 8 until after it has received
 * all the expected messages from the nanoapp.  That's how we can confirm
 * all messages actually made it through to the Host.
 */

// TODO(b/32114261): Remove this and actually test a variety of message types.
constexpr uint32_t kUntestedMessageType = UINT32_C(0x51501984);

namespace general_test {

// TODO(b/32114261): Remove this variable.
extern bool gUseNycMessageHack;

uint8_t SendMessageToHostTest::sSmallMessageData[kSmallMessageTestCount][kSmallMessageSize];
void *SendMessageToHostTest::sLargeMessageData[2];
constexpr uint32_t SendMessageToHostTest::kLargeSizes[2];

bool SendMessageToHostTest::sInMethod = false;
uint32_t SendMessageToHostTest::sFinishedBitmask = 0;

template<uint8_t kCallbackIndex>
void SendMessageToHostTest::smallMessageCallback(void *message,
                                                 size_t messageSize) {
  if (sInMethod) {
    sendFatalFailureToHost("smallMessageCallback called while another "
                           "nanoapp method is running");
  }
  sInMethod = true;
  if (message == nullptr) {
    sendFatalFailureToHost("smallMessageCallback given null message");
  }
  if (messageSize != kSmallMessageSize) {
    uint32_t size = static_cast<uint32_t>(messageSize);
    sendFatalFailureToHost("smallMessageCallback given bad messageSize:",
                           &size);
  }
  const uint8_t *msg = static_cast<const uint8_t*>(message);
  for (size_t i = 0; i < messageSize; i++) {
    if (msg[i] != kDataByte) {
      sendFatalFailureToHost("Corrupt data in smallMessageCallback");
    }
  }

  uint32_t stage = getSmallDataIndex(msg);
  uint8_t expectedCallbackIndex = 2;
  switch (stage) {
    case 0:  // fall-through
    case 3:
      expectedCallbackIndex = 0;
      break;
    case 1:
      expectedCallbackIndex = 1;
      break;
    case 2:
      sendFatalFailureToHost("callback invoked when null callback "
                             "given");
      break;
    default:
      sendInternalFailureToHost("Invalid index", &stage);
  }
  if (expectedCallbackIndex != kCallbackIndex) {
    sendFatalFailureToHost("Incorrect callback function called.");
  }

  markSuccess(stage);
  sInMethod = false;
}

void SendMessageToHostTest::smallMessageCallback0(void *message,
                                                  size_t messageSize) {
  smallMessageCallback<0>(message, messageSize);
}

void SendMessageToHostTest::smallMessageCallback1(void *message,
                                                  size_t messageSize) {
  smallMessageCallback<1>(message, messageSize);
}

uint32_t SendMessageToHostTest::getSmallDataIndex(const uint8_t *data) {
  // O(N) is fine.  N is small and this is test code.
  for (uint32_t i = 0; i < kSmallMessageTestCount; i++) {
    if (data == sSmallMessageData[i]) {
      return i;
    }
  }
  sendFatalFailureToHost("Bad memory sent to smallMessageCallback");
  // We should never get here.
  return kSmallMessageTestCount;
}

void SendMessageToHostTest::largeMessageCallback(void *message,
                                                 size_t messageSize) {
  if (sInMethod) {
    sendFatalFailureToHost("largeMessageCallback called while another "
                           "nanoapp method is running");
  }
  sInMethod = true;
  if (message == nullptr) {
    sendFatalFailureToHost("largeMessageCallback given null message");
  }
  uint32_t index = 2;
  if (message == sLargeMessageData[0]) {
    index = 0;
  } else if (message == sLargeMessageData[1]) {
    index = 1;
  } else {
    sendFatalFailureToHost("largeMessageCallback given bad message");
  }
  if (messageSize != kLargeSizes[index]) {
    sendFatalFailureToHost("largeMessageCallback given incorrect "
                           "messageSize");
  }
  const uint8_t *msg = static_cast<const uint8_t*>(message);
  for (size_t i = 0; i < messageSize; i++) {
    if (msg[i] != kDataByte) {
      sendFatalFailureToHost("Corrupt data in largeMessageCallback");
    }
  }
  chreHeapFree(sLargeMessageData[index]);
  // index 0 == stage 6, index 1 == stage 7
  markSuccess(index + 6);

  sInMethod = false;
}

void SendMessageToHostTest::markSuccess(uint32_t stage) {
  chreLog(CHRE_LOG_DEBUG, "Stage %" PRIu32 " succeeded", stage);
  uint32_t finishedBit = (1 << stage);
  if (sFinishedBitmask & finishedBit) {
    sendFatalFailureToHost("callback called multiple times for stage:",
                           &stage);
  }
  if ((kAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess bad stage", &stage);
  }
  sFinishedBitmask |= finishedBit;
  if (sFinishedBitmask == kAllFinished) {
    sendSuccessToHost();
  }
}

void SendMessageToHostTest::prepTestMemory() {
  nanoapp_testing::memset(sSmallMessageData, kDataByte,
                          sizeof(sSmallMessageData));

  for (size_t i = 0; i < 2; i++) {
    sLargeMessageData[i] = chreHeapAlloc(kLargeSizes[i]);
    if (sLargeMessageData[i] == nullptr) {
      sendFatalFailureToHost("Insufficient heap memory for test");
    }
    nanoapp_testing::memset(sLargeMessageData[i], kDataByte,
                            kLargeSizes[i]);
  }
}

void SendMessageToHostTest::sendMessageMaxSize() {
  // Our focus here is just sending this data; we're not trying to
  // test anything.  So we use the helper function.
  uint32_t maxSize = nanoapp_testing::hostToLittleEndian(
      static_cast<uint32_t>(CHRE_MESSAGE_TO_HOST_MAX_SIZE));
  // TODO(b/32114261): We intentionally don't have a namespace using
  //     declaration for sendMessageToHost because it's generally
  //     incorrect to use while we're working around this bug.  When the
  //     bug is fixed, we'll add this declaration, and use the method
  //     widely.
  nanoapp_testing::sendMessageToHost(MessageType::kContinue,
                                     &maxSize, sizeof(maxSize));
}

// Wrapper for chreSendMessageToHost() that sets sInMethod to false during its
// execution, to allow for inline callbacks (this CHRE API is allowed to call
// the free callback either within the function, or at an unspecified later time
// when this nanoapp is not otherwise executing).
bool SendMessageToHostTest::sendMessageToHost(
    void *message, uint32_t messageSize, uint32_t reservedMessageType,
    chreMessageFreeFunction *freeCallback) {
  sInMethod = false;
  bool success = chreSendMessageToHost(message, messageSize,
                                       reservedMessageType, freeCallback);
  sInMethod = true;

  return success;
}

SendMessageToHostTest::SendMessageToHostTest()
  : Test(CHRE_API_VERSION_1_0) {
}

void SendMessageToHostTest::setUp(uint32_t messageSize,
                                  const void * /* message */) {
  // TODO(b/32114261): We need this hackery so we can get the raw bytes
  //     from the host, without the test infrastructure trying to
  //     interpret them.  This won't be necessary when messageType is
  //     properly sent.
  gUseNycMessageHack = false;

  sInMethod = true;
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "SendMessageToHost message expects 0 additional bytes, got ",
        &messageSize);
  }

  prepTestMemory();

  // stage: 0
  if (!sendMessageToHost(sSmallMessageData[0], kSmallMessageSize,
                         kUntestedMessageType, smallMessageCallback0)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 0");
  }

  // stage: 1
  if (!sendMessageToHost(sSmallMessageData[1], kSmallMessageSize,
                         kUntestedMessageType, smallMessageCallback1)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 1");
  }

  // stage: 2
  if (!sendMessageToHost(sSmallMessageData[2], kSmallMessageSize,
                         kUntestedMessageType, nullptr)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 2");
  }
  // There's no callback, so we mark this as a success.
  markSuccess(2);

  // stage: 3
  if (!sendMessageToHost(sSmallMessageData[3], kSmallMessageSize,
                         kUntestedMessageType, smallMessageCallback0)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 3");
  }

  // stage: 4
  if (!sendMessageToHost(nullptr, 0, kUntestedMessageType, nullptr)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 4");
  }
  // There's no callback, so we mark this as a success.
  markSuccess(4);

  // stage: 5
  sendMessageMaxSize();
  // There's no callback, so we mark this as a success.
  markSuccess(5);

  // stage: 6
  if (sendMessageToHost(sLargeMessageData[0], kLargeSizes[0],
                        kUntestedMessageType, largeMessageCallback)) {
    sendFatalFailureToHost("Oversized data to chreSendMessageToHost "
                           "claimed success");
  }

  // stage: 7
  if (!sendMessageToHost(sLargeMessageData[1], kLargeSizes[1],
                         kUntestedMessageType, largeMessageCallback)) {
    sendFatalFailureToHost("Failed chreSendMessageToHost stage 7");
  }

  sInMethod = false;
}

void SendMessageToHostTest::handleEvent(uint32_t senderInstanceId,
                                        uint16_t eventType,
                                        const void* eventData) {
  if (sInMethod) {
    sendFatalFailureToHost("handleEvent invoked while another nanoapp "
                           "method is running");
  }
  sInMethod = true;

  // TODO(b/32114261): Use getMessageDataFromHostEvent().  We can't do
  //     that now because our messageType is probably wrong.
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    sendFatalFailureToHost("handleEvent got event from unexpected sender:",
                           &senderInstanceId);
  }
  if (eventType != CHRE_EVENT_MESSAGE_FROM_HOST) {
    unexpectedEvent(eventType);
  }

  auto dataStruct = static_cast<const chreMessageFromHostData *>(eventData);
  // TODO(b/32114261): Test the message type.
  if (dataStruct->messageSize != 0) {
    sendFatalFailureToHost("handleEvent got non-zero message size",
                           &dataStruct->messageSize);
  }
  // We don't test dataStruct->message.  We don't require this to be
  // nullptr.  If a CHRE choses to deal in 0-sized memory blocks, that's
  // acceptable.

  // Stage 8 was successful.  Note that other stages might still be waiting
  // for freeCallbacks.  So we don't send success to the host, but just
  // mark our stage as a success.
  markSuccess(8);

  sInMethod = false;
}

}  // namespace general_test
