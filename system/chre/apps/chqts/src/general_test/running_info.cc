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

#include <general_test/running_info.h>

#include <shared/send_message.h>

namespace general_test {

bool RunningInfo::queryByAppId() {
  return mPlatformInfo.queryByAppId(&mRunningInfo);
}

bool RunningInfo::queryByInstanceId() {
  return mPlatformInfo.queryByInstanceId(&mRunningInfo);
}

bool RunningInfo::validate(uint32_t appVersion) {
  bool result;
  if (mRunningInfo.version != appVersion) {
    nanoapp_testing::sendFatalFailureToHost(
        "Running info version does not match app version");
    result = false;
  } else if (mRunningInfo.appId != NANOAPP_ID) {
    nanoapp_testing::sendFatalFailureToHost(
        "Running info appId does not match build constant");
    result = false;
  } else if (mRunningInfo.version != NANOAPP_VERSION) {
    nanoapp_testing::sendFatalFailureToHost(
        "Running info version does not match build constant");
    result = false;
  } else {
    result = mPlatformInfo.validate(mRunningInfo.appId,
                                    mRunningInfo.instanceId);
  }

  return result;
}

} // namespace general_test
