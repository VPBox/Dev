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

#include "chre/platform/platform_wifi.h"

#include <cinttypes>

#include "chre/core/event_loop_manager.h"
#include "chre/platform/shared/pal_system_api.h"
#include "chre/platform/log.h"

namespace chre {

const chrePalWifiCallbacks PlatformWifiBase::sWifiCallbacks = {
  PlatformWifi::scanMonitorStatusChangeCallback,
  PlatformWifiBase::scanResponseCallback,
  PlatformWifiBase::scanEventCallback,
  PlatformWifiBase::rangingEventCallback,
};

PlatformWifi::~PlatformWifi() {
  if (mWifiApi != nullptr) {
    LOGD("Platform WiFi closing");
    prePalApiCall();
    mWifiApi->close();
    LOGD("Platform WiFi closed");
  }
}

void PlatformWifi::init() {
  prePalApiCall();
  mWifiApi = chrePalWifiGetApi(CHRE_PAL_WIFI_API_CURRENT_VERSION);
  if (mWifiApi != nullptr) {
    if (!mWifiApi->open(&gChrePalSystemApi, &sWifiCallbacks)) {
      LOGE("WiFi PAL open returned false");
      mWifiApi = nullptr;
    } else {
      LOGD("Opened WiFi PAL version 0x%08" PRIx32, mWifiApi->moduleVersion);
    }
  } else {
    LOGW("Requested Wifi PAL (version 0x%08" PRIx32 ") not found",
         CHRE_PAL_WIFI_API_CURRENT_VERSION);
  }
}

uint32_t PlatformWifi::getCapabilities() {
  if (mWifiApi != nullptr) {
    prePalApiCall();
    return mWifiApi->getCapabilities();
  } else {
    return CHRE_WIFI_CAPABILITIES_NONE;
  }
}

bool PlatformWifi::configureScanMonitor(bool enable) {
  if (mWifiApi != nullptr) {
    prePalApiCall();
    return mWifiApi->configureScanMonitor(enable);
  } else {
    return false;
  }
}

bool PlatformWifi::requestRanging(const struct chreWifiRangingParams *params) {
  if (mWifiApi != nullptr
      && mWifiApi->moduleVersion >= CHRE_PAL_WIFI_API_V1_2) {
    prePalApiCall();
    return mWifiApi->requestRanging(params);
  } else {
    return false;
  }
}

bool PlatformWifi::requestScan(const struct chreWifiScanParams *params) {
  if (mWifiApi != nullptr) {
    prePalApiCall();
    return mWifiApi->requestScan(params);
  } else {
    return false;
  }
}

void PlatformWifi::releaseRangingEvent(struct chreWifiRangingEvent *event) {
  prePalApiCall();
  mWifiApi->releaseRangingEvent(event);
}

void PlatformWifi::releaseScanEvent(struct chreWifiScanEvent *event) {
  prePalApiCall();
  mWifiApi->releaseScanEvent(event);
}

void PlatformWifiBase::rangingEventCallback(
    uint8_t errorCode, struct chreWifiRangingEvent *event) {
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .handleRangingEvent(errorCode, event);
}

void PlatformWifiBase::scanMonitorStatusChangeCallback(bool enabled,
                                                       uint8_t errorCode) {
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .handleScanMonitorStateChange(enabled, errorCode);
}

void PlatformWifiBase::scanResponseCallback(bool pending, uint8_t errorCode) {
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .handleScanResponse(pending, errorCode);
}

void PlatformWifiBase::scanEventCallback(struct chreWifiScanEvent *event) {
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .handleScanEvent(event);
}

}  // namespace chre
