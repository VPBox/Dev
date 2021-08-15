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

#ifndef CHRE_PLATFORM_POWER_CONTROL_UTIL_H
#define CHRE_PLATFORM_POWER_CONTROL_UTIL_H

extern "C" {

#include "qurt_island.h"

}  // extern "C"

#include "chre/core/event_loop_manager.h"
#include "chre/platform/slpi/uimg_util.h"

namespace chre {

/**
 * @return true if we're currently running in micro-image, aka island mode.
 */
inline bool slpiInUImage() {
  return (qurt_island_get_status() == 1);
}

/**
 * @return true if the successfully made a micro to big image transition.
 */
inline bool slpiForceBigImage() {
  bool success = false;
  if (isSlpiUimgSupported() && slpiInUImage()) {
    success = EventLoopManagerSingleton::get()->getEventLoop().
        getPowerControlManager().voteBigImage(true);
  }

  return success;
}

/**
 * Removes a big image vote from CHRE. Should only be called when the system is
 * idle.
 *
 * @return true if the vote succeeds.
 */
inline bool slpiRemoveBigImageVote() {
  return EventLoopManagerSingleton::get()->getEventLoop().
      getPowerControlManager().voteBigImage(false);
}

} // namespace chre

#endif // CHRE_PLATFORM_POWER_CONTROL_UTIL_H
