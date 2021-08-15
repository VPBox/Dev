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

#include <general_test/get_time_test.h>

#include <cstddef>

#include <shared/abort.h>
#include <shared/nano_endian.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::MessageType;
using nanoapp_testing::sendFatalFailureToHost;

namespace general_test {

GetTimeTest::GetTimeTest()
    : Test(CHRE_API_VERSION_1_0), mContinueCount(0) {
}

void GetTimeTest::setUp(uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "GetTime message expects 0 additional bytes, got ",
        &messageSize);
  }

  uint64_t firstTime = chreGetTime();
  if (firstTime == UINT64_C(0)) {
    sendFatalFailureToHost("chreGetTime() gave 0 well after system boot.");
  }

  uint64_t prevTime = firstTime;

  // Continuously call chreGetTime() to ensure timestamps are monotonically
  // increasing and increases at some point after a large number of calls.
  constexpr size_t kMaxGetTimeCount = 10000;
  for (size_t i = 0; (prevTime == firstTime && i < kMaxGetTimeCount); i++) {
    uint64_t nextTime = chreGetTime();

    // We don't require this to have increased, because maybe we're
    // on a relatively fast processor, or have a low resolution clock.
    if (nextTime < prevTime) {
      sendFatalFailureToHost(
          "chreGetTime() is not monotonically increasing");
    }

    prevTime = nextTime;
  }

  if (prevTime == firstTime) {
    sendFatalFailureToHost(
        "chreGetTime() is not increasing after a large number of calls");
  }

  prevTime = nanoapp_testing::hostToLittleEndian(prevTime);
  sendMessageToHost(MessageType::kContinue, &prevTime, sizeof(prevTime));

  // Now we'll wait to get a 'continue' from the host.
}

void GetTimeTest::handleEvent(uint32_t senderInstanceId,
                              uint16_t eventType, const void* eventData) {
  // We ignore the return value, since we expect no data.
  getMessageDataFromHostEvent(senderInstanceId, eventType, eventData,
                              MessageType::kContinue, 0);
  if (mContinueCount > 0) {
    sendFatalFailureToHost("Multiple kContinue messages sent");
  }

  mContinueCount++;
  uint64_t time = nanoapp_testing::hostToLittleEndian(chreGetTime());
  sendMessageToHost(MessageType::kContinue, &time, sizeof(time));
  // We do nothing else in the CHRE.  It's up to the Host to declare
  // if we've passed.
}

}  // namespace general_test
