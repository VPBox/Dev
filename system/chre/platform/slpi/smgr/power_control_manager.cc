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

#include "chre/platform/power_control_manager.h"

#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/slpi/power_control_util.h"

namespace chre {

PowerControlManagerBase::PowerControlManagerBase() {
#ifdef CHRE_SLPI_UIMG_ENABLED
  char kClientName[] = "CHRE";
  sns_pm_err_code_e result = sns_pm_client_init(
      &mClientHandle, apSuspendCallback, kClientName, SNS_PM_CLIENT_ID_CHRE);
  if (result != SNS_PM_SUCCESS) {
    FATAL_ERROR("Power manager client init failed");
  }
#endif // CHRE_SLPI_UIMG_ENABLED
}

PowerControlManagerBase::~PowerControlManagerBase() {
#ifdef CHRE_SLPI_UIMG_ENABLED
  sns_pm_client_close(mClientHandle);
#endif // CHRE_SLPI_UIMG_ENABLED
}

bool PowerControlManagerBase::voteBigImage(bool bigImage) {
#ifdef CHRE_SLPI_UIMG_ENABLED
  sns_pm_img_mode_e mode = bigImage ? SNS_IMG_MODE_BIG : SNS_IMG_MODE_NOCLIENT;
  sns_pm_err_code_e result = sns_pm_vote_img_mode(mClientHandle, mode);
  if (result != SNS_PM_SUCCESS) {
    LOGE("Failed to vote for power mode %d with result %d", mode, result);
  }

  return (result == SNS_PM_SUCCESS);
#else
  return true;
#endif // CHRE_SLPI_UIMG_ENABLED
}

void PowerControlManager::postEventLoopProcess(size_t numPendingEvents) {
  if (numPendingEvents == 0 && !slpiInUImage()) {
    voteBigImage(false);
  }
}

void PowerControlManagerBase::apSuspendCallback(bool apSuspended) {
  EventLoopManagerSingleton::get()->getEventLoop()
      .getPowerControlManager().mHostIsAwake = !apSuspended;
  if (apSuspended) {
    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_HOST_ASLEEP, nullptr, nullptr);
  } else {
    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_HOST_AWAKE, nullptr, nullptr);
  }
}

bool PowerControlManager::hostIsAwake() {
  return mHostIsAwake;
}

} // namespace chre
