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

#ifndef _GTS_NANOAPPS_SHARED_ABORT_H_
#define _GTS_NANOAPPS_SHARED_ABORT_H_

#include <cstdint>

namespace nanoapp_testing {

/**
 * Enumeration of what should be blamed for why the test was aborted.
 */
enum class AbortBlame : uint16_t {
  /**
   * Most common failure mode, a presumed issue within the CHRE
   * implementation.
   *
   * The test should have sent out an error message prior to aborting.
   */
  kChre = 0,

  /**
   * A presumed issue within the CHRE implementation, discovered while
   * in the nanoappEnd() method of the nanoapp.
   *
   * This differs from kChre in that we're not able to send out an
   * error message prior to aborting, so we want a distinct value.
   */
  kChreInNanoappEnd = 1,

  /**
   * A presumed bug in the test framework itself.
   *
   * The test should have sent out a kInternalError message prior to
   * aborting.
   */
  kTestFramework = 2
};

/**
 * This should fatally abort the nanoapp, and hence the current test.
 *
 * This is useful in cases where we cannot proceed due to previous issues,
 * or in cases where we want to make unambigously clear that we've
 * experienced a failure.
 *
 * The presumption is that code will send out a message prior to calling
 * this method, if at all practical.
 *
 * This will invoke chreAbort() with the code (0x10000 + uint16_t(reason)),
 * preserving the lower two bytes for tests to use for testing of
 * chreAbort().
 *
 * @param reason  Optional.  The appropriate AbortBlame.  Defaults to kChre.
 * @returns This function should never return.
 */
void abort(AbortBlame reason = AbortBlame::kChre);

}  // namespace nanoapp_testing

#endif  // _GTS_NANOAPPS_SHARED_ABORT_H_
