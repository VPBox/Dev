/*
 * Copyright (C) 2019 The Android Open Source Project
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

#define LOG_TAG "apexd"

#include "apexd_prop.h"

#include <android-base/logging.h>
#include <android-base/properties.h>

#include "apexd_utils.h"

namespace android {
namespace apex {
void waitForBootStatus(Status (&rollback_fn)(), void (&complete_fn)()) {
  while (true) {
    // Check for change in either crashing property or sys.boot_completed
    // Wait for updatable_crashing property change for most of the time
    // (arbitrary 30s), briefly check if boot has completed successfully,
    // if not continue waiting for updatable_crashing.
    // We use this strategy so that we can quickly detect if an updatable
    // process is crashing.
    if (android::base::WaitForProperty("ro.init.updatable_crashing", "1",
                                       std::chrono::seconds(30))) {
      LOG(INFO) << "Updatable crashing, attempting rollback";
      auto status = rollback_fn();
      if (!status.Ok()) {
        LOG(ERROR) << "Rollback failed : " << status.ErrorMessage();
      } else {
        LOG(INFO) << "Successfuly rolled back. Rebooting device";
        Reboot();
      }
      return;
    }
    if (android::base::GetProperty("sys.boot_completed", "") == "1") {
      // Boot completed we can return
      complete_fn();
      return;
    }
  }
}
}  // namespace apex
}  // namespace android
