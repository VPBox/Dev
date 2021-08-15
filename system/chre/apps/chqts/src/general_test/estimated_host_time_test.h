/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or aggreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GTS_NANOAPPS_GENERAL_TEST_ESTIMATED_HOST_TIME_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_ESTIMATED_HOST_TIME_TEST_H_

#include <general_test/test.h>

#include <cstdint>

namespace general_test {

/*
 * Verify estimated host time from nanoapp.
 *
 * Fundamentally, there are two phases to this test:
 *   1) Verify that time does increase at some point
 *   2) Verify that AP time is "close" to what nanoapp can get
 *
 * Protocol:
 * host to app: ESITMATED_HOST_TIME, no data
 * app to host: CONTINUE
 * host to app: CONTINUE, 64-bit time
 * app to host: SUCCESS
 */
class EstimatedHostTimeTest : public Test {
 public:
  EstimatedHostTimeTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void *eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

 private:
  void verifyIncreasingTime();

  uint32_t mTimerHandle;
  uint32_t mRemainingIterations;
  uint64_t mPriorHostTime;
};

} // namespace general_test

#endif // _GTS_NANOAPPS_GENERAL_TEST_ESTIMATED_HOST_TIME_TEST_H_
