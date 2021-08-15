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

#include "chre/platform/slpi/nanoapp_load_manager.h"

namespace chre {

bool NanoappLoadManager::prepareForLoad(
    uint16_t hostClientId, uint32_t transactionId, uint64_t appId,
    uint32_t appVersion, size_t totalBinaryLen) {
  if (hasPendingLoadTransaction()) {
    LOGW("Pending load transaction already exists. Overriding previous"
         " transaction.");
  }
  mCurrentLoadInfo.hostClientId = hostClientId;
  mCurrentLoadInfo.transactionId = transactionId;
  mCurrentLoadInfo.nextFragmentId = 1;
  mNanoapp = MakeUnique<Nanoapp>();

  bool success = false;
  if (mNanoapp.isNull()) {
    LOG_OOM();
  } else {
    success = mNanoapp->reserveBuffer(appId, appVersion, totalBinaryLen);
  }

  if (!success) {
    markFailure();
  }

  return success;
}

bool NanoappLoadManager::copyNanoappFragment(
    uint16_t hostClientId, uint32_t transactionId, uint32_t fragmentId,
    const void *buffer, size_t bufferLen) {
  bool success = false;
  if (validateFragment(hostClientId, transactionId, fragmentId)) {
    success = mNanoapp->copyNanoappFragment(buffer, bufferLen);
    if (success) {
      mCurrentLoadInfo.nextFragmentId++;
    } else {
      markFailure();
    }
  }

  return success;
}

bool NanoappLoadManager::validateFragment(
    uint16_t hostClientId, uint32_t transactionId, uint32_t fragmentId) const {
  bool valid = false;
  if (!hasPendingLoadTransaction()) {
    LOGE("No pending load transaction exists");
  } else {
    const FragmentedLoadInfo& info = mCurrentLoadInfo;
    valid = (info.hostClientId == hostClientId
        && info.transactionId == transactionId
        && info.nextFragmentId == fragmentId);
    if (!valid) {
      LOGE("Unexpected load fragment: expected host %" PRIu16
          "transaction %" PRIu32 " fragment %" PRIu32 ", received host %"
           PRIu16 " transaction %" PRIu32 " fragment %" PRIu32,
           info.hostClientId, info.transactionId, info.nextFragmentId,
           hostClientId, transactionId, fragmentId);
    }
  }

  return valid;
}

}  // namespace chre
