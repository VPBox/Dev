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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_H_
#define _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_H_

#include <cstdint>

#include <chre.h>

namespace general_test {

class NanoappInfo {
 public:
  NanoappInfo();

  /**
   * Send the nanoapp information to the host
   */
  void sendToHost();

  /**
   * Validate that provided values match gathered data.
   *
   * @param appId The app ID to validate against
   * @param instanceId The instance ID to validate against
   * @return true if provided values match data, false otherwise
   */
  bool validate(uint64_t appId, uint32_t instanceId);

  /**
   * Query system for running nanoapp information by gathered application Id
   *
   * @param info The chreNanoappInfo to populate with query results
   * @return true If nanoapp with application id is running on system
   */
  bool queryByAppId(struct chreNanoappInfo *info);

  /**
   * Query system for running nanoapp information by gathered instance Id
   *
   * @param info The chreNanoappInfo to populate with query results
   * @return true If nanoapp with instance id is running on system
   */
  bool queryByInstanceId(struct chreNanoappInfo *info);

 private:
  uint64_t mAppId;
  uint32_t mInstanceId;
};

} // namespace general_test

#endif // _GTS_NANOAPPS_GENERAL_TEST_NANOAPP_INFO_H_
