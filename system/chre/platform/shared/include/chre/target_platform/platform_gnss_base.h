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

#ifndef CHRE_PLATFORM_SHARED_PLATFORM_GNSS_BASE_H_
#define CHRE_PLATFORM_SHARED_PLATFORM_GNSS_BASE_H_

#include "chre/platform/shared/platform_pal.h"
#include "chre/pal/gnss.h"

namespace chre {

/**
 * Provides an instance of the PlatformGnssBase class that uses the CHRE PAL to
 * access the wifi subsystem.
 */
class PlatformGnssBase : public PlatformPal {
 protected:
  //! The instance of callbacks that are provided to the CHRE PAL.
  static const chrePalGnssCallbacks sGnssCallbacks;

  //! The instance of the CHRE PAL API. This will be set to nullptr if the
  //! platform does not supply an implementation.
  const chrePalGnssApi *mGnssApi;

  //! Event handlers for the CHRE GNSS PAL. Refer to chre/pal/gnss.h for futher
  //! information.
  static void requestStateResyncCallback();
  static void locationStatusChangeCallback(bool enabled, uint8_t errorCode);
  static void locationEventCallback(struct chreGnssLocationEvent *event);
  static void measurementStatusChangeCallback(bool enabled, uint8_t errorCode);
  static void measurementEventCallback(struct chreGnssDataEvent *event);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_PLATFORM_GNSS_BASE_H_
