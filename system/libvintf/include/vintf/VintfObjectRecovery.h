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

#ifndef ANDROID_VINTF_VINTF_OBJECT_RECOVERY_H_
#define ANDROID_VINTF_VINTF_OBJECT_RECOVERY_H_

#include "VintfObject.h"

#include "utils/Errors.h"

namespace android {
namespace vintf {

// Recovery version of VintfObject. It mount partitions if necessary. Partitions
// are unmounted afterwards.

class VintfObjectRecovery {
   public:
    /**
     * Check compatibility, given a set of manifests / matrices in packageInfo.
     * They will be checked against the manifests / matrices on the device.
     * Partitions (/system, /vendor) are mounted if necessary.
     * Note: this is NOT thread-safe.
     *
     * @param packageInfo a list of XMLs of HalManifest /
     * CompatibilityMatrix objects.
     *
     * @return = 0 if success (compatible)
     *         > 0 if incompatible
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    static int32_t CheckCompatibility(const std::vector<std::string>& packageInfo,
                                      std::string* error = nullptr);
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_VINTF_OBJECT_RECOVERY_H_
