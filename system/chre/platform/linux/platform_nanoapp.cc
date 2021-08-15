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

#include "chre/platform/platform_nanoapp.h"

#include <cinttypes>
#include <dlfcn.h>

#include "chre_api/chre/version.h"
#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/shared/nanoapp_dso_util.h"

namespace chre {

PlatformNanoapp::~PlatformNanoapp() {
  closeNanoapp();
}

bool PlatformNanoapp::start() {
  return openNanoapp() && mAppInfo->entryPoints.start();
}

void PlatformNanoapp::handleEvent(uint32_t senderInstanceId,
                                  uint16_t eventType,
                                  const void *eventData) {
  mAppInfo->entryPoints.handleEvent(senderInstanceId, eventType, eventData);
}

void PlatformNanoapp::end() {
  mAppInfo->entryPoints.end();
  closeNanoapp();
}

uint64_t PlatformNanoapp::getAppId() const {
  return (mAppInfo == nullptr) ? 0 : mAppInfo->appId;
}

uint32_t PlatformNanoapp::getAppVersion() const {
  return mAppInfo->appVersion;
}

uint32_t PlatformNanoapp::getTargetApiVersion() const {
  return CHRE_API_VERSION;
}

bool PlatformNanoapp::isSystemNanoapp() const {
  return (mAppInfo != nullptr && mAppInfo->isSystemNanoapp);
}

void PlatformNanoapp::logStateToBuffer(char *buffer, size_t *bufferPos,
                                       size_t bufferSize) const {}

void PlatformNanoappBase::loadFromFile(const std::string& filename) {
  CHRE_ASSERT(!isLoaded());
  mFilename = filename;
}

void PlatformNanoappBase::loadStatic(const struct chreNslNanoappInfo *appInfo) {
  CHRE_ASSERT(!isLoaded());
  mIsStatic = true;
  mAppInfo = appInfo;
}

bool PlatformNanoappBase::isLoaded() const {
  return (mIsStatic || mDsoHandle != nullptr);
}

bool PlatformNanoappBase::openNanoapp() {
  bool success = false;

  if (mIsStatic) {
    success = true;
  } else if (!mFilename.empty()) {
    success = openNanoappFromFile();
  } else {
    CHRE_ASSERT(false);
  }

  return success;
}

bool PlatformNanoappBase::openNanoappFromFile() {
  CHRE_ASSERT(!mFilename.empty());
  CHRE_ASSERT_LOG(mDsoHandle == nullptr, "Re-opening nanoapp");
  bool success = false;

  mDsoHandle = dlopen(mFilename.c_str(), RTLD_NOW | RTLD_GLOBAL);
  if (mDsoHandle == nullptr) {
    LOGE("Failed to load nanoapp from file %s: %s",
         mFilename.c_str(), dlerror());
  } else {
    mAppInfo = static_cast<const struct chreNslNanoappInfo *>(
        dlsym(mDsoHandle, CHRE_NSL_DSO_NANOAPP_INFO_SYMBOL_NAME));
    if (mAppInfo == nullptr) {
      LOGE("Failed to find app info symbol in %s: %s",
           mFilename.c_str(), dlerror());
    } else {
      // TODO(b/120778991): reenable this check after adding support for passing
      // in the .napp_header to the simulator
      //success = validateAppInfo(0 /* skip ID validation */, 0, mAppInfo);
      success = true;
      if (!success) {
        mAppInfo = nullptr;
      } else {
        LOGI("Successfully loaded nanoapp %s (0x%016" PRIx64 ") version 0x%"
             PRIx32 " uimg %d system %d from file %s", mAppInfo->name,
             mAppInfo->appId, mAppInfo->appVersion, mAppInfo->isTcmNanoapp,
             mAppInfo->isSystemNanoapp, mFilename.c_str());
      }
    }
  }

  return success;
}

void PlatformNanoappBase::closeNanoapp() {
  if (mDsoHandle != nullptr) {
    mAppInfo = nullptr;
    if (dlclose(mDsoHandle) != 0) {
      LOGE("dlclose failed: %s", dlerror());
    }
    mDsoHandle = nullptr;
  }
}

}  // namespace chre
