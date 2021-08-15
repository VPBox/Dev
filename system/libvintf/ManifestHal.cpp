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

#include "ManifestHal.h"
#include <unordered_set>

#include "MapValueIterator.h"
#include "parse_string.h"

namespace android {
namespace vintf {

bool ManifestHal::isValid() const {
    std::unordered_set<size_t> existing;
    for (const auto &v : versions) {
        if (existing.find(v.majorVer) != existing.end()) {
            return false;
        }
        existing.insert(v.majorVer);
    }
    return transportArch.isValid();
}

bool ManifestHal::operator==(const ManifestHal &other) const {
    if (format != other.format)
        return false;
    if (name != other.name)
        return false;
    if (versions != other.versions)
        return false;
    if (!(transportArch == other.transportArch)) return false;
    if (interfaces != other.interfaces) return false;
    if (isOverride() != other.isOverride()) return false;
    if (mAdditionalInstances != other.mAdditionalInstances) return false;
    return true;
}

bool ManifestHal::forEachInstance(const std::function<bool(const ManifestInstance&)>& func) const {
    for (const auto& v : versions) {
        for (const auto& intf : iterateValues(interfaces)) {
            bool cont = intf.forEachInstance([&](const auto& interface, const auto& instance,
                                                 bool /* isRegex */) {
                // TODO(b/73556059): Store ManifestInstance as well to avoid creating temps
                FqInstance fqInstance;
                if (fqInstance.setTo(getName(), v.majorVer, v.minorVer, interface, instance)) {
                    if (!func(ManifestInstance(std::move(fqInstance), TransportArch{transportArch},
                                               format))) {
                        return false;
                    }
                }
                return true;
            });
            if (!cont) {
                return false;
            }
        }
    }

    for (const auto& manifestInstance : mAdditionalInstances) {
        if (!func(manifestInstance)) {
            return false;
        }
    }

    return true;
}

bool ManifestHal::isDisabledHal() const {
    if (!isOverride()) return false;
    bool hasInstance = false;
    forEachInstance([&hasInstance](const auto&) {
        hasInstance = true;
        return false;  // has at least one instance, stop here.
    });
    return !hasInstance;
}

void ManifestHal::appendAllVersions(std::set<Version>* ret) const {
    ret->insert(versions.begin(), versions.end());
    forEachInstance([&](const auto& e) {
        ret->insert(e.version());
        return true;
    });
}

bool ManifestHal::verifyInstance(const FqInstance& fqInstance, std::string* error) const {
    if (fqInstance.hasPackage() && fqInstance.getPackage() != this->getName()) {
        if (error) {
            *error = "Should not add \"" + fqInstance.string() + "\" to a HAL with name " +
                     this->getName();
        }
        return false;
    }
    if (!fqInstance.hasVersion()) {
        if (error) *error = "Should specify version: \"" + fqInstance.string() + "\"";
        return false;
    }
    if (!fqInstance.hasInterface()) {
        if (error) *error = "Should specify interface: \"" + fqInstance.string() + "\"";
        return false;
    }
    if (!fqInstance.hasInstance()) {
        if (error) *error = "Should specify instance: \"" + fqInstance.string() + "\"";
        return false;
    }
    return true;
}

bool ManifestHal::insertInstances(const std::set<FqInstance>& fqInstances, std::string* error) {
    for (const FqInstance& e : fqInstances) {
        if (!insertInstance(e, error)) {
            return false;
        }
    }
    return true;
}

bool ManifestHal::insertInstance(const FqInstance& e, std::string* error) {
    if (!verifyInstance(e, error)) {
        return false;
    }

    size_t minorVer = e.getMinorVersion();
    for (auto it = mAdditionalInstances.begin(); it != mAdditionalInstances.end();) {
        if (it->version().majorVer == e.getMajorVersion() && it->interface() == e.getInterface() &&
            it->instance() == e.getInstance()) {
            minorVer = std::max(minorVer, it->version().minorVer);
            it = mAdditionalInstances.erase(it);
        } else {
            ++it;
        }
    }

    FqInstance toAdd;
    if (!toAdd.setTo(this->getName(), e.getMajorVersion(), minorVer, e.getInterface(),
                     e.getInstance())) {
        if (error) {
            *error = "Cannot create FqInstance with package='" + this->getName() + "', version='" +
                     to_string(Version(e.getMajorVersion(), minorVer)) + "', interface='" +
                     e.getInterface() + "', instance='" + e.getInstance() + "'";
        }
        return false;
    }

    mAdditionalInstances.emplace(std::move(toAdd), this->transportArch, this->format);
    return true;
}

} // namespace vintf
} // namespace android
