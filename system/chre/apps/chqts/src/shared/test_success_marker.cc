/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <shared/test_success_marker.h>

#include <chre/re.h>
#include <shared/send_message.h>

namespace nanoapp_testing {

TestSuccessMarker::TestSuccessMarker(uint32_t numStages) {
  if (numStages > 32) {
    sendFatalFailureToHost(
      "Total number of stage should be less than 33, got %d", &numStages);
  }
  mAllFinished = (UINT64_C(1) << numStages) - 1;
}

void TestSuccessMarker::markStage(uint32_t stage) {
  uint32_t finishedBit = (1 << stage);
  if ((mAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess invalid stage", &stage);
  }
  if ((mFinishedBitmask & finishedBit) == 0) {
    chreLog(CHRE_LOG_DEBUG, "Stage %d succeeded", stage);
    mFinishedBitmask |= finishedBit;
  }
}

bool TestSuccessMarker::isAllFinished() {
  return (mFinishedBitmask == mAllFinished);
}

bool TestSuccessMarker::isStageMarked(uint32_t stage) {
  bool marked = false;
  if (stage <= 32) {
    uint32_t finishedBit = (1 << stage);
    marked = ((mFinishedBitmask & finishedBit) != 0);
  }
  return marked;
}

void TestSuccessMarker::markStageAndSuccessOnFinish(uint32_t stage) {
  if (!isStageMarked(stage)) {
    markStage(stage);
    if (isAllFinished()) {
      sendSuccessToHost();
    }
  }
}

} // namespace nanoapp_testing
