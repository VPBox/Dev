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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_TIMER_SET_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_TIMER_SET_TEST_H_

#include <general_test/test.h>

namespace general_test {

/**
 * Checks that chreTimerSet() works by trying various timers.
 *
 * Simple Protocol.
 */
class TimerSetTest : public Test {
 public:
  TimerSetTest();
  ~TimerSetTest();

  void markSuccess(uint32_t stage);

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  class Stage {
   public:
    Stage(uint32_t stage, uint64_t duration, const void *cookie,
          bool oneShot);
    void start();
    // We take 'test' so we can call markSuccess if appropriate.
    void processEvent(uint64_t timestamp, TimerSetTest *test);

    const void *getCookie() const { return mCookie; }

   private:
    uint64_t mSetTime;
    uint64_t mDuration;
    uint32_t mStage;
    uint32_t mEventCount;
    const void *mCookie;
    bool mOneShot;
    uint32_t mTimerHandle;
  };
  // In the interest in keeping our static memory usage low (since we
  // are just one of many, many tests in this nanoapp), we get this
  // memory from the heap instead of statically declaring an array here.
  Stage *mStages;
  static constexpr size_t kStageCount = 6;

  bool mInMethod;
  static constexpr uint32_t kAllFinished = (1 << kStageCount) - 1;
  uint32_t mFinishedBitmask;

  void initStages();
  Stage *getStageFromCookie(const void *cookie);
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_TIMER_SET_TEST_H_
