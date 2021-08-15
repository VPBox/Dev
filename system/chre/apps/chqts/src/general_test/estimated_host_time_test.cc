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

#include <general_test/estimated_host_time_test.h>

#include <shared/nano_endian.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

#include <chre.h>

namespace general_test {

EstimatedHostTimeTest::EstimatedHostTimeTest()
    : Test(CHRE_API_VERSION_1_1),
      mTimerHandle(CHRE_TIMER_INVALID),
      mRemainingIterations(25) {
}

void EstimatedHostTimeTest::setUp(uint32_t /* messageSize */,
                                  const void * /* message */) {
  mPriorHostTime = chreGetEstimatedHostTime();

  constexpr uint64_t timerInterval = 100000000; // 100 ms

  mTimerHandle = chreTimerSet(timerInterval, &mTimerHandle,
                              false /* oneShot */);

  if (mTimerHandle == CHRE_TIMER_INVALID) {
    nanoapp_testing::sendFatalFailureToHost(
        "Unable to set timer for time verification");
  }
}

void EstimatedHostTimeTest::handleEvent(uint32_t senderInstanceId,
                                        uint16_t eventType,
                                        const void *eventData) {
  if (eventType == CHRE_EVENT_TIMER) {
    verifyIncreasingTime();
  } else {
    // Verify application processor time is within reason
    uint64_t currentHostTime = chreGetEstimatedHostTime();

    // TODO: Estimate message RTT to allow stricter accuracy check
    constexpr uint64_t timeDelta = 50000000; // 50 ms

    uint64_t givenHostTime;
    const void *message =
        getMessageDataFromHostEvent(senderInstanceId, eventType,
                                    eventData,
                                    nanoapp_testing::MessageType::kContinue,
                                    sizeof(givenHostTime));

    nanoapp_testing::memcpy(&givenHostTime, message, sizeof(givenHostTime));
    givenHostTime = nanoapp_testing::littleEndianToHost(givenHostTime);

    if (currentHostTime >= givenHostTime) {
      if ((currentHostTime - givenHostTime) <= timeDelta) {
        nanoapp_testing::sendSuccessToHost();
      } else {
        nanoapp_testing::sendFatalFailureToHost(
            "Current time is too far behind of host time");
      }
    } else if ((givenHostTime - currentHostTime) <= timeDelta) {
      nanoapp_testing::sendSuccessToHost();
    } else {
      nanoapp_testing::sendFatalFailureToHost(
          "Current time is too far ahead of host time");
    }
  }
}

void EstimatedHostTimeTest::verifyIncreasingTime() {
  if (mRemainingIterations > 0) {
    uint64_t currentHostTime = chreGetEstimatedHostTime();

    if (currentHostTime > mPriorHostTime) {
      chreTimerCancel(mTimerHandle);
      nanoapp_testing::sendMessageToHost(
          nanoapp_testing::MessageType::kContinue);
    } else {
      mPriorHostTime = currentHostTime;
    }

    --mRemainingIterations;
  } else {
    nanoapp_testing::sendFatalFailureToHost(
        "Unable to verify increasing time");
  }
}

} // namespace general_test
