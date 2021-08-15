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
#include "KernelInfo.h"

#include "parse_string.h"

namespace android {
namespace vintf {

const KernelVersion& KernelInfo::version() const {
    return mVersion;
}

const std::map<std::string, std::string>& KernelInfo::configs() const {
    return mConfigs;
}

bool KernelInfo::matchKernelConfigs(const std::vector<KernelConfig>& matrixConfigs,
                                    std::string* error) const {
    for (const KernelConfig& matrixConfig : matrixConfigs) {
        const std::string& key = matrixConfig.first;
        auto it = this->mConfigs.find(key);
        if (it == this->mConfigs.end()) {
            // special case: <value type="tristate">n</value> matches if the config doesn't exist.
            if (matrixConfig.second == KernelConfigTypedValue::gMissingConfig) {
                continue;
            }
            if (error != nullptr) {
                *error = "Missing config " + key;
            }
            return false;
        }
        const std::string& kernelValue = it->second;
        if (!matrixConfig.second.matchValue(kernelValue)) {
            if (error != nullptr) {
                *error = "For config " + key + ", value = " + kernelValue + " but required " +
                         to_string(matrixConfig.second);
            }
            return false;
        }
    }
    return true;
}

bool KernelInfo::matchKernelVersion(const KernelVersion& minLts) const {
    return minLts.version == mVersion.version && minLts.majorRev == mVersion.majorRev &&
           minLts.minorRev <= mVersion.minorRev;
}

bool KernelInfo::matchKernelRequirements(const std::vector<MatrixKernel>& kernels,
                                         std::string* error) const {
    bool foundMatchedKernelVersion = false;
    bool foundMatchedConditions = false;
    for (const MatrixKernel& matrixKernel : kernels) {
        if (!matchKernelVersion(matrixKernel.minLts())) {
            continue;
        }
        foundMatchedKernelVersion = true;
        // ignore this fragment if not all conditions are met.
        if (!matchKernelConfigs(matrixKernel.conditions(), error)) {
            continue;
        }
        foundMatchedConditions = true;
        if (!matchKernelConfigs(matrixKernel.configs(), error)) {
            return false;
        }
    }
    if (!foundMatchedKernelVersion) {
        if (error != nullptr) {
            std::stringstream ss;
            ss << "Framework is incompatible with kernel version " << version()
               << ", compatible kernel versions are";
            for (const MatrixKernel& matrixKernel : kernels) ss << " " << matrixKernel.minLts();
            *error = ss.str();
        }
        return false;
    }
    if (!foundMatchedConditions) {
        // This should not happen because first <conditions> for each <kernel> must be
        // empty. Reject here for inconsistency.
        if (error != nullptr) {
            error->insert(0, "Framework match kernel version with unmet conditions:");
        }
        return false;
    }
    if (error != nullptr) {
        error->clear();
    }
    return true;
}

bool KernelInfo::operator==(const KernelInfo& other) const {
    return mVersion == other.mVersion && mConfigs == other.mConfigs;
}

}  // namespace vintf
}  // namespace android
