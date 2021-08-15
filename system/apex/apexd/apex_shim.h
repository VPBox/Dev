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

#include "apex_file.h"

#include "status.h"
#include "status_or.h"
#include "string_log.h"

#include <android-base/logging.h>

namespace android {
namespace apex {
namespace shim {

constexpr const char* kSystemShimApexName = "com.android.apex.cts.shim.apex";

bool IsShimApex(const ApexFile& apex_file);

Status ValidateShimApex(const std::string& mount_point,
                        const ApexFile& apex_file);

Status ValidateUpdate(const std::string& system_apex_path,
                      const std::string& new_apex_path);

}  // namespace shim
}  // namespace apex
}  // namespace android
