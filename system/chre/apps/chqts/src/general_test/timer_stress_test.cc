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

#include <general_test/timer_stress_test.h>

#include <cinttypes>
#include <cstddef>

#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendInternalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * We stress the system by setting more and more timers until the system
 * runs out.  We then cancel one (CT) and set a new timer post-cancel (NT).
 * We make sure all the timers we set fire.
 *
 * Our stages are:
 * Stage 0: Successfully cancel CT.
 * Stage 1: All of our "exhaustion" timers fire.
 * Stage 2: The new timer, NT, fires.
 *
 * After all of our stages have succeeded, we send success to the host.  Note
 * there is no system requirement that Stage 2 happens after Stage 1, so
 * we use markSuccess() to track this.
 */

// Allow 1000ms to create the large number of timers specified below. This
// equates to approximately 1ms per timer which should give ample time for
// timer creation to complete.
constexpr uint64_t kDuration = UINT64_C(1000000000);

// If the system keeps claiming it can set more timers, we don't let it
// continue forever.  Instead, we'll cut it off at this limit.  And then
// we'll call its bluff, and make sure that all of these timers legitimately
// fire.  While it won't be an actual exhaustion test (we never took the
// system down to no more timers available), it will still give us confidence
// that this CHRE can properly handle any semi-reasonable timer load properly.
// 1030 is an arbitrary number, slightly over 2^10.  The hope is this
// balances between catching incorrect behavior and the test taking too long.
constexpr int32_t kMaxTimersToSet = INT32_C(1030);

namespace general_test {

namespace {

const uint32_t kCookies[] = {0, 1, 2};

} // anonymous namespace

void TimerStressTest::startStages() {
  uint32_t cancelId = chreTimerSet(kDuration, &kCookies[0], true);
  if (cancelId == CHRE_TIMER_INVALID) {
    sendFatalFailureToHost("No timers available");
  }

  mStage1CallbacksLeft = 0;
  // We anticipate most CHREs will not reach kMaxTimersToSet.
  while (mStage1CallbacksLeft < kMaxTimersToSet) {
    if (chreTimerSet(kDuration, &kCookies[1], true) == CHRE_TIMER_INVALID) {
      break;
    }
    mStage1CallbacksLeft++;
  }
  if (mStage1CallbacksLeft == 0) {
    sendFatalFailureToHost("Insufficient timers available");
  }
  if (!chreTimerCancel(cancelId)) {
    sendFatalFailureToHost("Unable to cancel timer");
  }
  markSuccess(0);
  if (chreTimerSet(kDuration, &kCookies[2], true) == CHRE_TIMER_INVALID) {
    sendFatalFailureToHost("Unable to set new timer after successful "
                           "cancel.");
  }
}

TimerStressTest::TimerStressTest()
  : Test(CHRE_API_VERSION_1_0),
    mInMethod(false),
    mFinishedBitmask(0),
    mStage1CallbacksLeft(0) {
}

void TimerStressTest::setUp(uint32_t messageSize, const void * /* message */) {
  mInMethod = true;

  if (messageSize != 0) {
    sendFatalFailureToHost(
        "TimerStress message expects 0 additional bytes, got ",
        &messageSize);
  }

  startStages();

  mInMethod = false;
}

void TimerStressTest::handleStageEvent(uint32_t index) {
  switch (index) {
    case 0:
      sendFatalFailureToHost("Canceled timer fired:", &index);
      break;

    case 1:
      --mStage1CallbacksLeft;
      if (mStage1CallbacksLeft <= 0) {
        markSuccess(index);
      }
      break;

    case 2:
      markSuccess(index);
      break;

    default:
      sendFatalFailureToHost("Unexpected event stage:", &index);
  }
}

void TimerStressTest::handleEvent(uint32_t senderInstanceId,
                                  uint16_t eventType, const void* eventData) {
  if (mInMethod) {
    sendFatalFailureToHost("handleEvent invoked while another nanoapp "
                           "method is running");
  }
  mInMethod = true;
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    sendFatalFailureToHost("handleEvent got event from unexpected sender:",
                           &senderInstanceId);
  }
  if (eventType != CHRE_EVENT_TIMER) {
    unexpectedEvent(eventType);
  }

  const uint32_t *data = static_cast<const uint32_t *>(eventData);
  handleStageEvent(*data);

  mInMethod = false;
}

void TimerStressTest::markSuccess(uint32_t stage) {
  chreLog(CHRE_LOG_DEBUG, "Stage %" PRIu32 " succeeded", stage);
  uint32_t finishedBit = (1 << stage);
  if ((kAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess bad stage:", &stage);
  }
  if ((mFinishedBitmask & finishedBit) != 0) {
    sendFatalFailureToHost("timer over-triggered:", &stage);
  }
  mFinishedBitmask |= finishedBit;
  if (mFinishedBitmask == kAllFinished) {
    sendSuccessToHost();
  }
}


}  // namespace general_test
