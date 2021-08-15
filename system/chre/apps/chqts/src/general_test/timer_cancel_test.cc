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

#include <general_test/timer_cancel_test.h>

#include <cinttypes>
#include <cstddef>

#include <shared/send_message.h>
#include <shared/time_util.h>

#include <chre.h>

using nanoapp_testing::kOneMillisecondInNanoseconds;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendInternalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * This test has four stages where we cancel one-shot and recurring timers,
 * before and after they're triggered.
 *
 * See the TimerCancelTest constructor to see which stage tests which setup.
 *
 * When all of our stages have succeeded, then we send success to the host.
 */

static uint64_t kDuration = 10 * kOneMillisecondInNanoseconds;

namespace general_test {

void TimerCancelTest::startStages() {
  for (uint32_t i = 0; i < kStageCount; i++) {
    Stage *stage = &mStages[i];
    stage->timerId = chreTimerSet(kDuration, stage, stage->oneShot);
    if (stage->timerId == CHRE_TIMER_INVALID) {
      sendFatalFailureToHost("Unable to set timer:", &i);
    }
    if (stage->expectCallback) {
      // Go on to the next stage.  Note this stage will markSuccess()
      // in handleStageEvent().
      continue;
    }
    if (!chreTimerCancel(stage->timerId)) {
      sendFatalFailureToHost("Unable to cancel timer:", &i);
    }
    if (chreTimerCancel(stage->timerId)) {
      sendFatalFailureToHost("Claimed success in second cancel:", &i);
    }
    markSuccess(i);
  }
}

TimerCancelTest::TimerCancelTest()
  : Test(CHRE_API_VERSION_1_0),
    mInMethod(false),
    mStages{
      // expectCallback:false ==> We're canceling before the timer fires.
      // expectCallback:true  ==> We'll cancel after the timer fires once.
      //
      //        stage, oneShot, expectCallback
        Stage(0,     false,   false),
        Stage(1,     true,    false),
        Stage(2,     false,   true  ),
        Stage(3,     true,    true  )},
    mFinishedBitmask(0) {
}

void TimerCancelTest::setUp(uint32_t messageSize, const void * /* message */) {
  mInMethod = true;

  if (messageSize != 0) {
    sendFatalFailureToHost(
        "TimerCancel message expects 0 additional bytes, got ",
        &messageSize);
  }

  constexpr uint32_t kUnownedTimer = 0;
  static_assert((kUnownedTimer != CHRE_TIMER_INVALID), "Bad test");
  if (chreTimerCancel(kUnownedTimer)) {
    sendFatalFailureToHost("Claimed success canceling timer we don't own");
  }

  startStages();

  // Now we wait for some events from the timers to fire.

  mInMethod = false;
}

void TimerCancelTest::handleStageEvent(Stage *stage) {
  if (!stage->expectCallback) {
    sendFatalFailureToHost("Timer didn't cancel:", &stage->stage);
  }
  // Now we're going to cancel the timer, so we don't expect an
  // additional call.
  stage->expectCallback = false;

  bool cancelSucceeded = chreTimerCancel(stage->timerId);
  if (stage->oneShot) {
    if (cancelSucceeded) {
      sendFatalFailureToHost("Claimed success canceling one-shot after "
                             "it fired:", &stage->stage);
    }
  } else {
    if (!cancelSucceeded) {
      sendFatalFailureToHost("Unable to cancel recurring timer:",
                             &stage->stage);
    }
  }
  if (chreTimerCancel(stage->timerId)) {
    sendFatalFailureToHost("Claimed success in second cancel:",
                           &stage->stage);
  }
  markSuccess(stage->stage);
}

void TimerCancelTest::handleEvent(uint32_t senderInstanceId,
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
  const Stage *stage = static_cast<const Stage*>(eventData);
  if (stage->stage >= kStageCount) {
    sendFatalFailureToHost("Invalid handleEvent data:", &stage->stage);
  }
  handleStageEvent(const_cast<Stage *>(stage));

  mInMethod = false;
}

void TimerCancelTest::markSuccess(uint32_t stage) {
  chreLog(CHRE_LOG_DEBUG, "Stage %" PRIu32 " succeeded", stage);
  uint32_t finishedBit = (1 << stage);
  if ((kAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess bad stage:", &stage);
  }
  if ((mFinishedBitmask & finishedBit) != 0) {
    sendInternalFailureToHost("markSuccess multiple times:", &stage);
  }
  mFinishedBitmask |= finishedBit;
  if (mFinishedBitmask == kAllFinished) {
    sendSuccessToHost();
  }
}

}  // namespace general_test
