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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_TEST_H_

#include <shared/send_message.h>

#include <chre.h>

namespace general_test {

/**
 * Abstract base for all test cases.
 */
class Test {
 public:
  Test(uint32_t minSupportedVersion);
  virtual ~Test() {}

  void testHandleEvent(uint32_t senderInstanceId, uint16_t eventType,
                       const void *eventData);

  void testSetUp(uint32_t messageSize, const void *message);

 protected:
  /**
   * Report a test-ending error due to an unexpectedEvent.
   *
   * @param eventType  The event type
   * @returns Never.  This method aborts execution.
   */
  static void unexpectedEvent(uint16_t eventType);

  /**
   * Wrapper structure to store the current async request.
   */
  struct chreAsyncRequest {
    //! An opaque value that will be included in the chreAsyncResult
    //! sent in relation to a chre async request.
    const void *cookie;

    //! A type of request. Same field as that in {@link #chreAsyncResult}
    uint8_t requestType;

    //! Timestamp when mading the request.
    uint64_t requestTimeNs;

    //! Timeout to receive the chre async result.
    uint64_t timeoutNs;
  };

  /**
   * Reports a test-ending error due to failure in chreAsyncResult.
   *
   * 1. chre async result is not success.
   * 2. chre async result success, but errorCode is not CHRE_ERROR_NONE.
   * 3. request cookie mismatch.
   * 4. requestType mismatch.
   * 5. result timeout.
   *
   * @param result chreAsyncResult of an async request.
   * @param request lastest chre async request.
   */
  static void validateChreAsyncResult(const chreAsyncResult *result,
                                      const chreAsyncRequest& request);

  /**
   * Get the message data sent from the host, after performing sanity checks.
   *
   * The method centralizes a number of common sanity checks that tests
   * will perform in taking the given CHRE event data and extracting out
   * the raw data payload sent by the host.  This method is still useful
   * when no message data is expected from the host, as we'll still
   * perform the sanity checks.
   *
   * This method will end the test in failure if any of the following happen:
   * o 'senderInstanceId' != CHRE_INSTANCE_ID
   * o 'eventType' != CHRE_EVENT_MESSAGE_FROM_HOST
   * o 'eventData'->reservedMessageType != expectedMessageType
   * o 'eventData'->messageSize != expectedMessageSize
   *
   * @param senderInstanceId  From handleEvent()
   * @param eventType  From handleEvent()
   * @param eventData  From handleEvent()
   * @param expectedMessageType  The expected 'reservedMessageType' field
   *     when 'eventData' is seen as a chreMessageFromHostData.
   * @param expectedMessageSize  The expected 'messageSize' field
   *     when 'eventData' is seen as a chreMessageFromHostData.
   * @returns 'eventData'->message, assuming all the sanity checks pass.
   */
  static const void *getMessageDataFromHostEvent(
      uint32_t senderInstanceId, uint16_t eventType, const void* eventData,
      nanoapp_testing::MessageType expectedMessageType,
      uint32_t expectedMessageSize);

  virtual void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                           const void* eventData) = 0;
  virtual void setUp(uint32_t messageSize, const void *message) = 0;

  /**
   * The platform reported CHRE API version.
   *
   * Nanoapps may use this to determine what version they are running
   * on and perform any version specific behaviours.
   */
  const uint32_t mApiVersion;

 private:
  /**
   * Is the nanoapp supported by the platform reported CHRE API version.
   *
   * Nanoapps specify the minimum CHRE API version required during
   * construction. If it is at least the version that is being reported
   * by the platform then mIsSupported will be true. Else, the nanoapp
   * will skip the test.
   */
  const bool mIsSupported;
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_TEST_H_
