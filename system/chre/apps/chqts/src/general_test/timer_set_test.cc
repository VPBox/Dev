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

#include <general_test/timer_set_test.h>

#include <cinttypes>
#include <cstddef>
#include <new>

#include <shared/send_message.h>
#include <shared/time_util.h>

#include <chre.h>

using nanoapp_testing::kOneMillisecondInNanoseconds;
using nanoapp_testing::kOneSecondInNanoseconds;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendInternalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * We have various "stages" for different timer setups we want to test.
 * To speed up the test, we run all our stages simultaneously.  That
 * requires having 6 timers available, but with a 32 timer minimum
 * and a presumption that tests aren't going to be run alongside a lot
 * of other nanoapps, this should be fine.
 *
 * See initStages() for the description of each stage.  Since these all
 * happen in parallel, we leave it to each stage to mark itself has having
 * succeeded, and have markSuccess() tell the Host when all stages have
 * reported in.
 *
 * Note that we intentionally place the one-shot timers first, to give
 * us more time to notice them (incorrectly) firing multiple times.
 */

static uint64_t kShortDuration = 10 * kOneMillisecondInNanoseconds;
static uint64_t kLongDuration = kOneSecondInNanoseconds;

namespace general_test {

TimerSetTest::Stage::Stage(uint32_t stage, uint64_t duration,
                           const void *cookie, bool oneShot)
    : mSetTime(0), mDuration(duration), mStage(stage), mEventCount(0),
      mCookie(cookie), mOneShot(oneShot) {}

void TimerSetTest::Stage::start() {
  mSetTime = chreGetTime();
  mTimerHandle = chreTimerSet(mDuration, mCookie, mOneShot);
  if (mTimerHandle == CHRE_TIMER_INVALID) {
    sendFatalFailureToHost("Unable to set timer ", &mStage);
  }
  if (mSetTime == 0) {
    sendFatalFailureToHost("chreGetTime() gave 0");
  }
}

void TimerSetTest::Stage::processEvent(uint64_t timestamp, TimerSetTest *test) {
  if (mSetTime == 0) {
    sendInternalFailureToHost("Didn't initialize mSetTime");
  }
  mEventCount++;

  uint64_t expectedTime = mSetTime + (mEventCount * mDuration);
  if (timestamp < expectedTime) {
    sendFatalFailureToHost("Timer triggered too soon ", &mStage);
  }
  // TODO(b/32179037): Make this check much stricter.
  if (timestamp > (expectedTime + kOneSecondInNanoseconds)) {
    sendFatalFailureToHost("Timer triggered over a second late ", &mStage);
  }

  if (mOneShot) {
    if (mEventCount > 1) {
      sendFatalFailureToHost("One shot timer called multiple times ",
                             &mStage);
    } else {
      test->markSuccess(mStage);
    }
  } else if (mEventCount == 3) {
    // We mark recurring timers as successful on their third firing, if we
    // can cancel it.
    if (chreTimerCancel(mTimerHandle)) {
      test->markSuccess(mStage);
    } else {
      sendFatalFailureToHost("Could not cancel recurring timer", &mStage);
    }
  }
}

void TimerSetTest::initStages() {
  // To avoid fragmentation, we do one large allocation, and use
  // placement new to initialize it.
  mStages = static_cast<Stage*>(chreHeapAlloc(sizeof(*mStages) *
                                              kStageCount));
  if (mStages == nullptr) {
    sendFatalFailureToHost("Insufficient heap");
  }

#define COOKIE(num) reinterpret_cast<const void*>(num)

  // Stage 0: Test NULL cookie
  new(&mStages[0]) Stage(0, kShortDuration, nullptr, true);
  // Stage 1: Test (void*)-1 cookie
  new(&mStages[1]) Stage(1, kShortDuration, COOKIE(-1), true);
  // Stage 2: Test one shot with short duration
  new(&mStages[2]) Stage(2, kShortDuration, COOKIE(2), true);
  // Stage 3: Test one shot with long duration
  new(&mStages[3]) Stage(3, kLongDuration,  COOKIE(3), true);
  // Stage 4: Test recurring with long duration
  new(&mStages[4]) Stage(4, kLongDuration,  COOKIE(4), false);
  // Stage 5: Test recurring with short duration
  new(&mStages[5]) Stage(5, kShortDuration, COOKIE(5), false);
  static_assert((5 + 1) == kStageCount, "Missized array");

#undef COOKIE
}

TimerSetTest::TimerSetTest()
  : Test(CHRE_API_VERSION_1_0), mInMethod(false), mFinishedBitmask(0) {
}

void TimerSetTest::setUp(uint32_t messageSize, const void * /* message */) {
  mInMethod = true;

  if (messageSize != 0) {
    sendFatalFailureToHost(
        "TimerSet message expects 0 additional bytes, got ",
        &messageSize);
  }

  initStages();
  for (size_t i = 0; i < kStageCount; i++) {
    mStages[i].start();
  }

  mInMethod = false;
}

TimerSetTest::~TimerSetTest() {
  chreHeapFree(mStages);
}

void TimerSetTest::handleEvent(uint32_t senderInstanceId,
                               uint16_t eventType, const void* eventData) {
  uint64_t timestamp = chreGetTime();
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
  Stage *stage = getStageFromCookie(eventData);
  if (stage == nullptr) {
    sendFatalFailureToHost("handleEvent got invalid eventData");
  }
  stage->processEvent(timestamp, this);

  mInMethod = false;
}

void TimerSetTest::markSuccess(uint32_t stage) {
  chreLog(CHRE_LOG_DEBUG, "Stage %" PRIu32 " succeeded", stage);
  uint32_t finishedBit = (1 << stage);
  if ((kAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess bad stage", &stage);
  }
  mFinishedBitmask |= finishedBit;
  if (mFinishedBitmask == kAllFinished) {
    sendSuccessToHost();
  }
}

TimerSetTest::Stage *TimerSetTest::getStageFromCookie(const void *cookie) {
  Stage *ret = nullptr;
  for (size_t i = 0; i < kStageCount; i++) {
    if (mStages[i].getCookie() == cookie) {
      if (ret != nullptr) {
        sendInternalFailureToHost("Multiple stages with the same "
                                  "cookie");
      }
      ret = &mStages[i];
      // It's cheap enough to go through the whole array, and will
      // catch if we screw up this test setup by duplicating a cookie.
    }
  }
  return ret;
}

}  // namespace general_test
