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


#ifndef ANDROID_VINTF_MANIFEST_HAL_H
#define ANDROID_VINTF_MANIFEST_HAL_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include <hidl-util/FqInstance.h>

#include "HalFormat.h"
#include "HalInterface.h"
#include "ManifestInstance.h"
#include "TransportArch.h"
#include "Version.h"

namespace android {
namespace vintf {

// A component of HalManifest.
struct ManifestHal {
    using InstanceType = ManifestInstance;

    ManifestHal() = default;

    ManifestHal(HalFormat fmt, std::string&& n, std::vector<Version>&& vs, TransportArch ta,
                std::map<std::string, HalInterface>&& intf)
        : format(fmt),
          name(std::move(n)),
          versions(std::move(vs)),
          transportArch(ta),
          interfaces(std::move(intf)) {}

    bool operator==(const ManifestHal &other) const;

    HalFormat format = HalFormat::HIDL;
    std::string name;
    std::vector<Version> versions;
    TransportArch transportArch;
    std::map<std::string, HalInterface> interfaces;

    inline Transport transport() const {
        return transportArch.transport;
    }

    inline Arch arch() const { return transportArch.arch; }

    inline const std::string& getName() const { return name; }
    bool forEachInstance(const std::function<bool(const ManifestInstance&)>& func) const;

    bool isOverride() const { return mIsOverride; }

    // When true, the existence of this <hal> tag means the component does NOT
    // exist on the device. This is useful for ODM manifests to specify that
    // a HAL is disabled on certain products.
    bool isDisabledHal() const;

   private:
    friend struct LibVintfTest;
    friend struct ManifestHalConverter;
    friend struct HalManifest;
    friend bool parse(const std::string &s, ManifestHal *hal);

    // Whether this hal is a valid one. Note that an empty ManifestHal
    // (constructed via ManifestHal()) is valid.
    bool isValid() const;

    // Return all versions mentioned by <version>s and <fqname>s.
    void appendAllVersions(std::set<Version>* ret) const;

    bool mIsOverride = false;
    // Additional instances to <version> x <interface> x <instance>.
    std::set<ManifestInstance> mAdditionalInstances;

    // insert instances to mAdditionalInstances.
    // Existing instances will be ignored.
    // Pre: all instances to be inserted must satisfy
    // !hasPackage() && hasVersion() && hasInterface() && hasInstance()
    bool insertInstance(const FqInstance& fqInstance, std::string* error = nullptr);
    bool insertInstances(const std::set<FqInstance>& fqInstances, std::string* error = nullptr);

    // Verify instance before inserting.
    bool verifyInstance(const FqInstance& fqInstance, std::string* error = nullptr) const;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_MANIFEST_HAL_H
