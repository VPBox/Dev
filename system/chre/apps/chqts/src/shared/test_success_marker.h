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

#ifndef _GTS_NANOAPPS_SHARED_TEST_SUCCESS_MARKER_H_
#define _GTS_NANOAPPS_SHARED_TEST_SUCCESS_MARKER_H_

#include <cinttypes>

namespace nanoapp_testing {

/**
 * A class which marks the success of each test stage.
 * Sends success to host, when all test stages are marked.
 */
class TestSuccessMarker {
 public:
  /**
   * @param numStages The total number of test stages (must be < 33)
   */
  TestSuccessMarker(uint32_t numStages);

  /**
   * Sends fatal failure to host, if a bad stage passed in.
   * Marks the bit of a stage to 1.
   *
   * @param stage test stage index
   */
  void markStage(uint32_t stage);

  /**
   * Marks success of a stage.
   * Sends success to host if all stages are finished.
   *
   * @param stage test stage index
   */
  void markStageAndSuccessOnFinish(uint32_t stage);

  /**
   * @return true if all stages are done, false otherwise
   */
  bool isAllFinished();

  /**
   * @param stage test stage index
   * @return true if the stage has been marked, false otherwise
   */
  bool isStageMarked(uint32_t stage);

 private:
  //! Flag to denote all test stages are finished.
  uint32_t mAllFinished;

  //! Used to mark which stage has been finished.
  uint32_t mFinishedBitmask = 0;
};

} // namespace nanoapp_testing

#endif // _GTS_NANOAPPS_SHARED_TEST_SUCCESS_MARKER_H_
