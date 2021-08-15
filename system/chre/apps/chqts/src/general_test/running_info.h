/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_RUNNING_INFO_H_
#define _GTS_NANOAPPS_GENERAL_TEST_RUNNING_INFO_H_

#include <general_test/nanoapp_info.h>

#include <cstdint>

#include <chre.h>

namespace general_test {

class RunningInfo {
 public:
  bool queryByAppId();
  bool queryByInstanceId();

  /**
   * Validate that provided values match gathered data.
   *
   * @param appVersion The app version to validate against
   * @return true if provided value matches data, false otherwise
   */
  bool validate(uint32_t appVersion);

 private:
  NanoappInfo mPlatformInfo;
  struct chreNanoappInfo mRunningInfo;
};

} // namespace general_test

#endif // _GTS_NANOAPPS_GENERAL_TEST_RUNNING_INFO_H_
