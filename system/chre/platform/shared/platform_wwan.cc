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

#include "chre/platform/platform_wwan.h"

#include <cinttypes>

#include "chre/core/event_loop_manager.h"
#include "chre/platform/shared/pal_system_api.h"
#include "chre/platform/log.h"

namespace chre {

const chrePalWwanCallbacks PlatformWwanBase::sWwanCallbacks = {
  PlatformWwanBase::cellInfoResultCallback,
};

PlatformWwan::~PlatformWwan() {
  if (mWwanApi != nullptr) {
    LOGD("Platform WWAN closing");
    prePalApiCall();
    mWwanApi->close();
    LOGD("Platform WWAN closed");
  }
}

void PlatformWwan::init() {
  prePalApiCall();
  mWwanApi = chrePalWwanGetApi(CHRE_PAL_WWAN_API_CURRENT_VERSION);
  if (mWwanApi != nullptr) {
    if (!mWwanApi->open(&gChrePalSystemApi, &sWwanCallbacks)) {
      LOGE("WWAN PAL open returned false");
      mWwanApi = nullptr;
    }  else {
      LOGD("Opened WWAN PAL version 0x%08" PRIx32, mWwanApi->moduleVersion);
    }
  } else {
    LOGW("Requested WWAN PAL (version 0x%08" PRIx32 ") not found",
         CHRE_PAL_WWAN_API_CURRENT_VERSION);
  }
}

uint32_t PlatformWwan::getCapabilities() {
  if (mWwanApi != nullptr) {
    prePalApiCall();
    return mWwanApi->getCapabilities();
  } else {
    return CHRE_WWAN_CAPABILITIES_NONE;
  }
}

bool PlatformWwan::requestCellInfo() {
  if (mWwanApi != nullptr) {
    prePalApiCall();
    return mWwanApi->requestCellInfo();
  } else {
    return false;
  }
}

void PlatformWwan::releaseCellInfoResult(chreWwanCellInfoResult *result) {
  if (mWwanApi != nullptr) {
    prePalApiCall();
    mWwanApi->releaseCellInfoResult(result);
  }
}

void PlatformWwanBase::cellInfoResultCallback(
    struct chreWwanCellInfoResult *result) {
  EventLoopManagerSingleton::get()->getWwanRequestManager()
      .handleCellInfoResult(result);
}

}  // namespace chre
