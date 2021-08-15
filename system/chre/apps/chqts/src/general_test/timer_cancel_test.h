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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_TIMER_CANCEL_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_TIMER_CANCEL_TEST_H_

#include <general_test/test.h>
#include <chre.h>

namespace general_test {

/**
 * Checks that chreTimerCancel() works by trying various usages.
 *
 * Simple Protocol.
 */
class TimerCancelTest : public Test {
 public:
  TimerCancelTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  struct Stage {
    uint32_t stage;
    uint32_t timerId;
    bool oneShot;
    bool expectCallback;

    // Leave timerId invalid.
    Stage(uint32_t stage_, bool oneShot_, bool expectCallback_) :
      stage(stage_), timerId(CHRE_TIMER_INVALID), oneShot(oneShot_),
      expectCallback(expectCallback_) {}
  };

  bool mInMethod;

  static constexpr size_t kStageCount = 4;
  Stage mStages[kStageCount];

  static constexpr uint32_t kAllFinished = (1 << kStageCount) - 1;
  uint32_t mFinishedBitmask;

  void startStages();
  void handleStageEvent(Stage *stage);
  void markSuccess(uint32_t stage);
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_TIMER_CANCEL_TEST_H_
