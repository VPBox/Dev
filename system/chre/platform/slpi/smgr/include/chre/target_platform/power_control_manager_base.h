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

#ifndef CHRE_PLATFORM_SLPI_SMGR_POWER_CONTROL_MANAGER_BASE_H_
#define CHRE_PLATFORM_SLPI_SMGR_POWER_CONTROL_MANAGER_BASE_H_

extern "C" {

#include "qurt.h"
#ifdef CHRE_SLPI_UIMG_ENABLED
#include "sns_pm.h"
#endif // CHRE_SLPI_UIMG_ENABLED

} // extern "C"

namespace chre {

class PowerControlManagerBase {
 public:
  PowerControlManagerBase();
  ~PowerControlManagerBase();

  /**
   * Votes for a power mode to the SLPI power manager.
   *
   * @param bigImage Whether to vote for bigImage or not.
   *
   * @return true if the vote returned SNS_PM_SUCCESS.
   */
  bool voteBigImage(bool bigImage);

 protected:
#ifdef CHRE_SLPI_UIMG_ENABLED
  //! Client handle for the subscription to the power manager
  sns_pm_handle_t mClientHandle = nullptr;
#endif // CHRE_SLPI_UIMG_ENABLED

  //! Set to true if the host is awake, false if asleep.
  bool mHostIsAwake = true;

  /**
   * Invoked by the SNS Power Manager when the AP has suspended or resumed.
   *
   * @param apSuspended set to true when the AP has suspended, false on resume.
   */
  static void apSuspendCallback(bool apSuspended);
};

} // namespace chre

#endif // CHRE_PLATFORM_SLPI_SMGR_POWER_CONTROL_MANAGER_BASE_H_
