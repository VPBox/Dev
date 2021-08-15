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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_SEND_MESSAGE_TO_HOST_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_SEND_MESSAGE_TO_HOST_TEST_H_

#include <general_test/test.h>

#include <chre.h>

namespace general_test {

/**
 * Check chreSendMessageToHost() works, along with an empty message from the
 * host to the nanoapp.
 *
 * TODO(b/32114261): This test is way more complicated than it should be.
 *     Specifically, the standard workaround for this bug involves
 *     putting more data within the 'message' to/from host/nanoapp.  But
 *     since we're specifically testing that data, we can't use that
 *     workaround.  When that workaround is gone, we can make this test
 *     much simpler.
 *
 * Protocol:
 * Host:    kSendMessageToHostTest, no data
 * Nanoapp: 3 bytes of 0xFE
 * Nanoapp: 3 bytes of 0xFE
 * Nanoapp: 3 bytes of 0xFE
 * Nanoapp: 3 bytes of 0xFE
 * Nanoapp: 0 bytes
 * Nanoapp: kContinue, 4 bytes (little endian) with <MessageMaxSize>
 * Nanoapp: <MessageMaxSize> bytes of 0xFE
 * Host:    0 bytes
 * [nanoapp waits for all 'freeCallback's to have been invoked]
 * Nanoapp: kSuccess
 */
class SendMessageToHostTest : public Test {
 public:
  SendMessageToHostTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  // Note, most of our data and methods are static, because much of our test
  // logic happens in callbacks which must be static members.  This class
  // instance is a singleton, so there's no issue with this approach.

  static constexpr uint8_t kDataByte = UINT8_C(0xFE);

  static constexpr uint32_t kSmallMessageSize = 3;
  static constexpr size_t kSmallMessageTestCount = 4;
  static uint8_t sSmallMessageData[kSmallMessageTestCount][kSmallMessageSize];

  static constexpr uint32_t kLargeSizes[2] = {
    CHRE_MESSAGE_TO_HOST_MAX_SIZE + 1,
    CHRE_MESSAGE_TO_HOST_MAX_SIZE
  };
  static void *sLargeMessageData[2];

  // Catch if CHRE implementation illegally reenters nanoapp code.
  static bool sInMethod;

  // We have nine stages.  We set a bit in our finishedBitmask
  // when each has succeeded.
  static constexpr uint32_t kAllFinished = (1 << 9) - 1;
  static uint32_t sFinishedBitmask;

  template<uint8_t kCallbackIndex>
  static void smallMessageCallback(void *message, size_t messageSize);

  static void smallMessageCallback0(void *message, size_t messageSize);
  static void smallMessageCallback1(void *message, size_t messageSize);

  static void largeMessageCallback(void *message, size_t messageSize);

  static uint32_t getSmallDataIndex(const uint8_t *data);

  static void markSuccess(uint32_t stage);

  static bool sendMessageToHost(void *message, uint32_t messageSize,
                                uint32_t reservedMessageType,
                                chreMessageFreeFunction *freeCallback);

  void prepTestMemory();
  void sendMessageMaxSize();
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_SEND_MESSAGE_TO_HOST_TEST_H_
