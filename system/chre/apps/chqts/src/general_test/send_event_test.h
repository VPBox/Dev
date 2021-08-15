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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_SEND_EVENT_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_SEND_EVENT_TEST_H_

#include <general_test/test.h>

namespace general_test {

/**
 * Checks that chreSendEvent() works by sending events to ourself.
 *
 * We send a number of events with different combinations of types
 * and data and callbacks, and make sure things come through as we
 * expect.
 *
 * Simple Protocol.
 */
class SendEventTest : public Test {
 public:
  SendEventTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  uint32_t mNextNum;
  uint32_t mData[6];

  static bool sInMethod;
  static uint8_t sCallbacksInvoked;

  template<uint8_t kCallbackIndex>
  static void completeCallback(uint16_t eventType, void *data);

  static void completeCallback0(uint16_t eventType, void *data);
  static void completeCallback1(uint16_t eventType, void *data);
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_SEND_EVENT_TEST_H_
