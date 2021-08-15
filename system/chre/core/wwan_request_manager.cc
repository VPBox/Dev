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

#include "chre/core/wwan_request_manager.h"

#include "chre/core/event_loop_manager.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/util/system/debug_dump.h"

namespace chre {

void WwanRequestManager::init() {
  return mPlatformWwan.init();
}

uint32_t WwanRequestManager::getCapabilities() {
  return mPlatformWwan.getCapabilities();
}

bool WwanRequestManager::requestCellInfo(Nanoapp *nanoapp,
                                         const void *cookie) {
  CHRE_ASSERT(nanoapp);

  bool success = false;
  if (!mCellInfoRequestingNanoappInstanceId.has_value()) {
    success = mPlatformWwan.requestCellInfo();
    if (success) {
      nanoapp->registerForBroadcastEvent(CHRE_EVENT_WWAN_CELL_INFO_RESULT);
      mCellInfoRequestingNanoappInstanceId = nanoapp->getInstanceId();
      mCellInfoRequestingNanoappCookie = cookie;
    }
  } else {
    LOGE("Cell info request made while a request is in flight");
  }

  return success;
}

void WwanRequestManager::handleCellInfoResult(chreWwanCellInfoResult *result) {
  auto callback = [](uint16_t /* eventType */, void *eventData) {
    auto *cellInfoResult = static_cast<chreWwanCellInfoResult *>(eventData);
    EventLoopManagerSingleton::get()->getWwanRequestManager()
        .handleCellInfoResultSync(cellInfoResult);
  };

  EventLoopManagerSingleton::get()->deferCallback(
      SystemCallbackType::WwanHandleCellInfoResult, result, callback);
}

void WwanRequestManager::handleCellInfoResultSync(
    chreWwanCellInfoResult *result) {
  if (mCellInfoRequestingNanoappInstanceId.has_value()) {
    result->cookie = mCellInfoRequestingNanoappCookie;
    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_WWAN_CELL_INFO_RESULT, result, freeCellInfoResultCallback,
        mCellInfoRequestingNanoappInstanceId.value());
  } else {
    LOGE("Cell info results received unexpectedly");
  }
}

void WwanRequestManager::logStateToBuffer(char *buffer, size_t *bufferPos,
                                          size_t bufferSize) const {
  debugDumpPrint(buffer, bufferPos, bufferSize, "\nWWAN:\n");
  if (mCellInfoRequestingNanoappInstanceId.has_value()) {
    debugDumpPrint(buffer, bufferPos, bufferSize,
                   " WWAN request pending nanoappId=%" PRIu32 "\n",
                   mCellInfoRequestingNanoappInstanceId.value());
  }
}

void WwanRequestManager::handleFreeCellInfoResult(
    chreWwanCellInfoResult *result) {
  if (mCellInfoRequestingNanoappInstanceId.has_value()) {
    Nanoapp *nanoapp = EventLoopManagerSingleton::get()->getEventLoop()
        .findNanoappByInstanceId(*mCellInfoRequestingNanoappInstanceId);
    if (nanoapp != nullptr) {
      nanoapp->unregisterForBroadcastEvent(CHRE_EVENT_WWAN_CELL_INFO_RESULT);
    } else {
      LOGE("Freeing cell info for non-existent nanoapp");
    }

    mCellInfoRequestingNanoappInstanceId.reset();
  } else {
    LOGE("Cell info released with no pending request");
  }

  mPlatformWwan.releaseCellInfoResult(result);
}

void WwanRequestManager::freeCellInfoResultCallback(uint16_t eventType,
                                                    void *eventData) {
  auto *result = static_cast<chreWwanCellInfoResult *>(eventData);
  EventLoopManagerSingleton::get()->getWwanRequestManager()
      .handleFreeCellInfoResult(result);
}

}  // namespace chre
