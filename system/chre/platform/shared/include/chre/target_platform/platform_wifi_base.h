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

#ifndef CHRE_PLATFORM_SHARED_PLATFORM_WIFI_BASE_H_
#define CHRE_PLATFORM_SHARED_PLATFORM_WIFI_BASE_H_

#include "chre/platform/shared/platform_pal.h"
#include "chre/pal/wifi.h"

namespace chre {

/**
 * Provides an instance of the PlatformWifiBase class that uses the CHRE PAL to
 * access the wifi subsystem.
 */
class PlatformWifiBase : public PlatformPal {
 protected:
  //! The instance of callbacks that are provided to the CHRE PAL.
  static const chrePalWifiCallbacks sWifiCallbacks;

  //! The instance of the CHRE PAL API. This will be set to nullptr if the
  //! platform does not supply an implementation.
  const chrePalWifiApi *mWifiApi;

  // Event handlers for the CHRE WiFi PAL. Refer to chre/pal/wifi.h for futher
  // information.
  static void scanMonitorStatusChangeCallback(bool enabled, uint8_t errorCode);
  static void scanResponseCallback(bool pending, uint8_t errorCode);
  static void scanEventCallback(struct chreWifiScanEvent *event);
  static void rangingEventCallback(uint8_t errorCode,
                                   struct chreWifiRangingEvent *event);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_PLATFORM_WIFI_BASE_H_
