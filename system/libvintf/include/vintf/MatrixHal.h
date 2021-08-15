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

#ifndef ANDROID_VINTF_MATRIX_HAL_H
#define ANDROID_VINTF_MATRIX_HAL_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "HalFormat.h"
#include "HalInterface.h"
#include "MatrixInstance.h"
#include "VersionRange.h"

namespace android {
namespace vintf {

// A HAL entry to a compatibility matrix
struct MatrixHal {
    using InstanceType = MatrixInstance;

    bool operator==(const MatrixHal &other) const;
    // Check whether the MatrixHal contains the given version.
    bool containsVersion(const Version& version) const;

    HalFormat format = HalFormat::HIDL;
    std::string name;
    std::vector<VersionRange> versionRanges;
    bool optional = false;
    std::map<std::string, HalInterface> interfaces;

    inline const std::string& getName() const { return name; }

    bool forEachInstance(const std::function<bool(const MatrixInstance&)>& func) const;

   private:
    friend struct HalManifest;
    friend struct CompatibilityMatrix;
    friend std::string expandInstances(const MatrixHal& req, const VersionRange& vr, bool brace);
    friend std::vector<std::string> expandInstances(const MatrixHal& req);

    // Loop over interface/instance for a specific VersionRange.
    bool forEachInstance(const VersionRange& vr,
                         const std::function<bool(const MatrixInstance&)>& func) const;
    // Loop over interface/instance. VersionRange is supplied to the function as a vector.
    bool forEachInstance(
        const std::function<bool(const std::vector<VersionRange>&, const std::string&,
                                 const std::string& instanceOrPattern, bool isRegex)>& func) const;

    bool isCompatible(const std::set<FqInstance>& providedInstances,
                      const std::set<Version>& providedVersions) const;
    bool isCompatible(const VersionRange& vr, const std::set<FqInstance>& providedInstances,
                      const std::set<Version>& providedVersions) const;

    void setOptional(bool o);
    void insertVersionRanges(const std::vector<VersionRange>& other);
    // Return size of all interface/instance pairs.
    size_t instancesCount() const;
    void insertInstance(const std::string& interface, const std::string& instance, bool isRegex);
    // Remove a specific interface/instances. Return true if removed, false otherwise.
    bool removeInstance(const std::string& interface, const std::string& instance, bool isRegex);
    // Remove all <interface> tags.
    void clearInstances();
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_MATRIX_HAL_H
