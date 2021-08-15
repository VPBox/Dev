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

#include <general_test/nanoapp_info.h>

#include <shared/nano_endian.h>
#include <shared/send_message.h>

#include <chre.h>

namespace general_test {

struct AppInfo {
  uint64_t appId;
  uint32_t instanceId;
} __attribute__((packed));

NanoappInfo::NanoappInfo()
  : mAppId(chreGetAppId()), mInstanceId(chreGetInstanceId()) {
    if (mInstanceId == CHRE_INSTANCE_ID) {
      nanoapp_testing::sendFatalFailureToHost(
          "Given CHRE_INSTANCE_ID for my instance ID");
    }
}

void NanoappInfo::sendToHost() {
  AppInfo info = {
    .appId = nanoapp_testing::hostToLittleEndian(mAppId),
    .instanceId = nanoapp_testing::hostToLittleEndian(mInstanceId),
  };

  sendMessageToHost(nanoapp_testing::MessageType::kContinue,
                    &info, sizeof(info));
}

bool NanoappInfo::validate(uint64_t appId, uint32_t instanceId) {
  bool result = true;
  if (appId != mAppId) {
    nanoapp_testing::sendFatalFailureToHost(
        "app IDs do not match");
    result = false;
  } else if (instanceId != mInstanceId) {
    nanoapp_testing::sendFatalFailureToHost(
        "instance IDs do not match");
    result = false;
  }

  return result;
}

bool NanoappInfo::queryByAppId(struct chreNanoappInfo *info) {
  bool result = chreGetNanoappInfoByAppId(mAppId, info);

  if (!result) {
    nanoapp_testing::sendFatalFailureToHost(
        "Unable to get nanoapp info by app ID");
  }

  return result;
}

bool NanoappInfo::queryByInstanceId(struct chreNanoappInfo *info) {
  bool result = chreGetNanoappInfoByInstanceId(mInstanceId, info);

  if (!result) {
    nanoapp_testing::sendFatalFailureToHost(
        "Unable to get nanoapp info by instance ID");
  }

  return result;
}

} // namespace general_test
