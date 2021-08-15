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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_HEAP_EXHAUSTION_STABILITY_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_HEAP_EXHAUSTION_STABILITY_TEST_H_

#include <general_test/test.h>

namespace general_test {

/**
 * Exhaust the heap and confirm the platform remains stable when trying
 * various things.
 *
 * We don't require everything to be available when the heap is exhausted,
 * but we do require the system is honest about its capabilities, and doesn't
 * crash.
 *
 * Simple Protocol.
 */
class HeapExhaustionStabilityTest : public Test {
 public:
  HeapExhaustionStabilityTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  bool mInMethod;

  void **mExhaustionPtrs;
  size_t mExhaustionPtrCount;

  static constexpr size_t kStageCount = 2;
  static constexpr uint32_t kAllFinished = (1 << kStageCount) - 1;
  uint32_t mFinishedBitmask;

  void exhaustHeap();
  void freeMemory();

  void testLog(uint32_t zero);
  void testSetTimer();
  void testSendEvent();
  void testSensor();
  // TODO(b/32114261): This method currently doesn't test anything.
  void testMessageToHost();

  void handleTimer(uint32_t senderInstanceId, const void *eventData);
  void handleSelfEvent(uint32_t senderInstanceId, const void *eventData);

  void markSuccess(uint32_t stage);
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_HEAP_EXHAUSTION_STABILITY_TEST_H_
