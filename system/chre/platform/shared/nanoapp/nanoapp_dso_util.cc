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

#include "chre/platform/shared/nanoapp_dso_util.h"

#include <cinttypes>
#include <cstring>
#include <chre/version.h>

#include "chre/platform/log.h"

namespace chre {

bool validateAppInfo(uint64_t expectedAppId, uint32_t expectedAppVersion,
                     const struct chreNslNanoappInfo *appInfo) {
  uint32_t ourApiMajorVersion = CHRE_EXTRACT_MAJOR_VERSION(chreGetApiVersion());
  uint32_t targetApiMajorVersion = CHRE_EXTRACT_MAJOR_VERSION(
      appInfo->targetApiVersion);

  bool success = false;
  if (appInfo->magic != CHRE_NSL_NANOAPP_INFO_MAGIC) {
    LOGE("Invalid app info magic: got 0x%08" PRIx32 " expected 0x%08" PRIx32,
         appInfo->magic, static_cast<uint32_t>(CHRE_NSL_NANOAPP_INFO_MAGIC));
  } else if (appInfo->appId == 0) {
    LOGE("Rejecting invalid app ID 0");
  } else if (expectedAppId != 0 && expectedAppId != appInfo->appId) {
    LOGE("Expected app ID (0x%016" PRIx64 ") doesn't match internal one (0x%016"
         PRIx64 ")", expectedAppId, appInfo->appId);
  } else if (expectedAppVersion != appInfo->appVersion) {
    LOGE("Expected app version (0x%" PRIx32 ") doesn't match internal one (0x%"
         PRIx32 ")", expectedAppVersion, appInfo->appVersion);
  } else if (targetApiMajorVersion != ourApiMajorVersion) {
    LOGE("App targets a different major API version (%" PRIu32 ") than what we "
         "provide (%" PRIu32 ")", targetApiMajorVersion, ourApiMajorVersion);
  } else if (strlen(appInfo->name) > CHRE_NSL_DSO_NANOAPP_STRING_MAX_LEN) {
    LOGE("App name is too long");
  } else if (strlen(appInfo->vendor) > CHRE_NSL_DSO_NANOAPP_STRING_MAX_LEN) {
    LOGE("App vendor is too long");
  } else {
    success = true;
  }

  return success;
}

}  // namespace chre
