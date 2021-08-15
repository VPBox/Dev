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

#ifndef CHRE_PLATFORM_SLPI_SEE_POWER_CONTROL_MANAGER_BASE_H_
#define CHRE_PLATFORM_SLPI_SEE_POWER_CONTROL_MANAGER_BASE_H_

#ifdef CHRE_THREAD_UTIL_ENABLED
extern "C" {
#include "sns_client_thread_util.h"
} // extern "C"
#endif  // CHRE_THREAD_UTIL_ENABLED

namespace chre {

class PowerControlManagerBase {
 public:
   PowerControlManagerBase();
  ~PowerControlManagerBase();

  /**
   * Makes a power mode request. An actual vote to the SLPI power manager may
   * not be cast depending on current power mode and mBigImageRefCount.
   *
   * @param bigImage Whether to request bigImage or not.
   *
   * @return true if the vote returned success.
   */
  bool voteBigImage(bool bigImage);

  /**
   * Sets the AP awake/suspended state and posts an event to interested
   * nanoapps. This method should only be invoked by the SEE helper as a
   * result of an event from the remote_proc_state sensor.
   *
   * @param awake true if the AP is awake, false otherwise
   */
  void onHostWakeSuspendEvent(bool awake);

 protected:
  //! Set to true if the host is awake, false if suspended.
  bool mHostIsAwake = true;

#ifdef CHRE_THREAD_UTIL_ENABLED
  //! Set to true if the thread is currently idle (no pending events),
  //! false otherwise.
  bool mIsThreadIdle = true;

  //! A pointer to the client to compute thread utilization
  sns_thread_util_client *mThreadUtilClient = nullptr;
#endif  // CHRE_THREAD_UTIL_ENABLED
};

} // namespace chre

#endif // CHRE_PLATFORM_SLPI_SEE_POWER_CONTROL_MANAGER_BASE_H_
