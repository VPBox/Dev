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

#define LOG_TAG "libvintf"
#include <android-base/logging.h>

#include "HalManifest.h"

#include <dirent.h>

#include <mutex>
#include <set>

#include <android-base/strings.h>

#include "parse_string.h"
#include "parse_xml.h"
#include "utils.h"
#include "CompatibilityMatrix.h"

namespace android {
namespace vintf {

using details::Instances;
using details::InstancesOfVersion;
using details::mergeField;

// Check <version> tag for all <hal> with the same name.
bool HalManifest::shouldAdd(const ManifestHal& hal) const {
    if (!hal.isValid()) {
        return false;
    }
    if (hal.isOverride()) {
        return true;
    }
    auto existingHals = mHals.equal_range(hal.name);
    std::set<size_t> existingMajorVersions;
    for (auto it = existingHals.first; it != existingHals.second; ++it) {
        for (const auto& v : it->second.versions) {
            // Assume integrity on existingHals, so no check on emplace().second
            existingMajorVersions.insert(v.majorVer);
        }
    }
    for (const auto& v : hal.versions) {
        if (!existingMajorVersions.emplace(v.majorVer).second /* no insertion */) {
            return false;
        }
    }
    return true;
}

// Remove elements from "list" if p(element) returns true.
template <typename List, typename Predicate>
static void removeIf(List& list, Predicate predicate) {
    for (auto it = list.begin(); it != list.end();) {
        if (predicate(*it)) {
            it = list.erase(it);
        } else {
            ++it;
        }
    }
}

void HalManifest::removeHals(const std::string& name, size_t majorVer) {
    removeIf(mHals, [&name, majorVer](auto& existingHalPair) {
        auto& existingHal = existingHalPair.second;
        if (existingHal.name != name) {
            return false;
        }
        auto& existingVersions = existingHal.versions;
        removeIf(existingVersions, [majorVer](const auto& existingVersion) {
            return existingVersion.majorVer == majorVer;
        });
        return existingVersions.empty();
    });
}

bool HalManifest::add(ManifestHal&& halToAdd) {
    if (halToAdd.isOverride()) {
        if (halToAdd.isDisabledHal()) {
            // Special syntax when there are no instances at all. Remove all existing HALs
            // with the given name.
            mHals.erase(halToAdd.name);
        }
        // If there are <version> tags, remove all existing major versions that causes a conflict.
        for (const Version& versionToAdd : halToAdd.versions) {
            removeHals(halToAdd.name, versionToAdd.majorVer);
        }
    }

    return HalGroup::add(std::move(halToAdd));
}

bool HalManifest::shouldAddXmlFile(const ManifestXmlFile& xmlFile) const {
    auto existingXmlFiles = getXmlFiles(xmlFile.name());
    for (auto it = existingXmlFiles.first; it != existingXmlFiles.second; ++it) {
        if (xmlFile.version() == it->second.version()) {
            return false;
        }
    }
    return true;
}

std::set<std::string> HalManifest::getHalNames() const {
    std::set<std::string> names{};
    for (const auto &hal : mHals) {
        names.insert(hal.first);
    }
    return names;
}

std::set<std::string> HalManifest::getHalNamesAndVersions() const {
    std::set<std::string> names{};
    forEachInstance([&names](const ManifestInstance& e) {
        names.insert(toFQNameString(e.package(), e.version()));
        return true;
    });
    return names;
}

Transport HalManifest::getTransport(const std::string &package, const Version &v,
            const std::string &interfaceName, const std::string &instanceName) const {
    Transport transport{Transport::EMPTY};
    forEachInstanceOfInterface(package, v, interfaceName, [&](const auto& e) {
        if (e.instance() == instanceName) {
            transport = e.transport();
        }
        return transport == Transport::EMPTY;  // if not found, continue
    });
    if (transport == Transport::EMPTY) {
        LOG(DEBUG) << "HalManifest::getTransport(" << mType << "): Cannot find "
                   << toFQNameString(package, v, interfaceName, instanceName);
    }
    return transport;
}

bool HalManifest::forEachInstanceOfVersion(
    const std::string& package, const Version& expectVersion,
    const std::function<bool(const ManifestInstance&)>& func) const {
    for (const ManifestHal* hal : getHals(package)) {
        bool cont = hal->forEachInstance([&](const ManifestInstance& manifestInstance) {
            if (manifestInstance.version().minorAtLeast(expectVersion)) {
                return func(manifestInstance);
            }
            return true;
        });
        if (!cont) return false;
    }
    return true;
}

// indent = 2, {"foo"} => "foo"
// indent = 2, {"foo", "bar"} => "\n  foo\n  bar";
template <typename Container>
void multilineIndent(std::ostream& os, size_t indent, const Container& lines) {
    if (lines.size() == 1) {
        os << *lines.begin();
        return;
    }
    for (const auto& line : lines) {
        os << "\n";
        for (size_t i = 0; i < indent; ++i) os << " ";
        os << line;
    }
}

// For each hal in mat, there must be a hal in manifest that supports this.
std::vector<std::string> HalManifest::checkIncompatibleHals(const CompatibilityMatrix& mat) const {
    std::vector<std::string> ret;
    for (const MatrixHal &matrixHal : mat.getHals()) {
        if (matrixHal.optional) {
            continue;
        }

        std::set<FqInstance> manifestInstances;
        std::set<FqInstance> manifestInstancesNoPackage;
        std::set<Version> versions;
        for (const ManifestHal* manifestHal : getHals(matrixHal.name)) {
            manifestHal->forEachInstance([&](const auto& manifestInstance) {
                manifestInstances.insert(manifestInstance.getFqInstance());
                manifestInstancesNoPackage.insert(manifestInstance.getFqInstanceNoPackage());
                return true;
            });
            manifestHal->appendAllVersions(&versions);
        }

        if (!matrixHal.isCompatible(manifestInstances, versions)) {
            std::ostringstream oss;
            oss << matrixHal.name << ":\n    required: ";
            multilineIndent(oss, 8, android::vintf::expandInstances(matrixHal));
            oss << "\n    provided: ";
            if (manifestInstances.empty()) {
                multilineIndent(oss, 8, versions);
            } else {
                multilineIndent(oss, 8, manifestInstancesNoPackage);
            }

            ret.insert(ret.end(), oss.str());
        }
    }
    return ret;
}

std::set<std::string> HalManifest::checkUnusedHals(const CompatibilityMatrix& mat) const {
    std::set<std::string> ret;

    forEachInstance([&ret, &mat](const auto& manifestInstance) {
        const auto& fqInstance = manifestInstance.getFqInstance();
        if (!mat.matchInstance(fqInstance.getPackage(), fqInstance.getVersion(),
                               fqInstance.getInterface(), fqInstance.getInstance())) {
            ret.insert(fqInstance.string());
        }
        return true;
    });

    return ret;
}

static bool checkVendorNdkCompatibility(const VendorNdk& matVendorNdk,
                                        const std::vector<VendorNdk>& manifestVendorNdk,
                                        std::string* error) {
    // For pre-P vendor images, device compatibility matrix does not specify <vendor-ndk>
    // tag. Ignore the check for these devices.
    if (matVendorNdk.version().empty()) {
        return true;
    }
    for (const auto& vndk : manifestVendorNdk) {
        if (vndk.version() != matVendorNdk.version()) {
            continue;
        }
        // version matches, check libraries
        std::vector<std::string> diff;
        std::set_difference(matVendorNdk.libraries().begin(), matVendorNdk.libraries().end(),
                            vndk.libraries().begin(), vndk.libraries().end(),
                            std::inserter(diff, diff.begin()));
        if (!diff.empty()) {
            if (error != nullptr) {
                *error = "Vndk libs incompatible for version " + matVendorNdk.version() +
                         ". These libs are not in framework manifest:";
                for (const auto& name : diff) {
                    *error += " " + name;
                }
            }
            return false;
        }
        return true;
    }

    // no match is found.
    if (error != nullptr) {
        *error = "Vndk version " + matVendorNdk.version() + " is not supported. " +
                 "Supported versions in framework manifest are:";
        for (const auto& vndk : manifestVendorNdk) {
            *error += " " + vndk.version();
        }
    }
    return false;
}

static bool checkSystemSdkCompatibility(const SystemSdk& matSystemSdk,
                                        const SystemSdk& manifestSystemSdk, std::string* error) {
    SystemSdk notSupported = matSystemSdk.removeVersions(manifestSystemSdk);
    if (!notSupported.empty()) {
        if (error) {
            *error =
                "The following System SDK versions are required by device "
                "compatibility matrix but not supported by the framework manifest: [" +
                base::Join(notSupported.versions(), ", ") + "]. Supported versions are: [" +
                base::Join(manifestSystemSdk.versions(), ", ") + "].";
        }
        return false;
    }
    return true;
}

bool HalManifest::checkCompatibility(const CompatibilityMatrix &mat, std::string *error) const {
    if (mType == mat.mType) {
        if (error != nullptr) {
            *error = "Wrong type; checking " + to_string(mType) + " manifest against "
                    + to_string(mat.mType) + " compatibility matrix";
        }
        return false;
    }
    auto incompatibleHals = checkIncompatibleHals(mat);
    if (!incompatibleHals.empty()) {
        if (error != nullptr) {
            *error = "HALs incompatible.";
            if (mat.level() != Level::UNSPECIFIED)
                *error += " Matrix level = " + to_string(mat.level()) + ".";
            if (level() != Level::UNSPECIFIED)
                *error += " Manifest level = " + to_string(level()) + ".";
            *error += " The following requirements are not met:\n";
            for (const auto& e : incompatibleHals) {
                *error += e + "\n";
            }
        }
        return false;
    }
    if (mType == SchemaType::FRAMEWORK) {
        if (!checkVendorNdkCompatibility(mat.device.mVendorNdk, framework.mVendorNdks, error)) {
            return false;
        }

        if (!checkSystemSdkCompatibility(mat.device.mSystemSdk, framework.mSystemSdk, error)) {
            return false;
        }
    } else if (mType == SchemaType::DEVICE) {
        bool sepolicyMatch = false;
        for (const auto &range : mat.framework.mSepolicy.sepolicyVersions()) {
            if (range.supportedBy(device.mSepolicyVersion)) {
                sepolicyMatch = true;
                break;
            }
        }
        if (!sepolicyMatch) {
            if (error != nullptr) {
                *error = "Sepolicy version " + to_string(device.mSepolicyVersion)
                        + " doesn't satisify the requirements.";
            }
            return false;
        }

        if (!!kernel() && !kernel()->matchKernelRequirements(mat.framework.mKernels, error)) {
            return false;
        }
    }

    return true;
}

CompatibilityMatrix HalManifest::generateCompatibleMatrix() const {
    CompatibilityMatrix matrix;

    forEachInstance([&matrix](const ManifestInstance& e) {
        matrix.add(MatrixHal{
            .format = e.format(),
            .name = e.package(),
            .optional = true,
            .versionRanges = {VersionRange{e.version().majorVer, e.version().minorVer}},
            .interfaces = {{e.interface(), HalInterface{e.interface(), {e.instance()}}}}});
        return true;
    });
    if (mType == SchemaType::FRAMEWORK) {
        matrix.mType = SchemaType::DEVICE;
        // VNDK does not need to be added for compatibility
    } else if (mType == SchemaType::DEVICE) {
        matrix.mType = SchemaType::FRAMEWORK;
        matrix.framework.mSepolicy = Sepolicy(0u /* kernelSepolicyVersion */,
                {{device.mSepolicyVersion.majorVer, device.mSepolicyVersion.minorVer}});
    }

    return matrix;
}

status_t HalManifest::fetchAllInformation(const FileSystem* fileSystem, const std::string& path,
                                          std::string* error) {
    return details::fetchAllInformation(fileSystem, path, gHalManifestConverter, this, error);
}

SchemaType HalManifest::type() const {
    return mType;
}

void HalManifest::setType(SchemaType type) {
    mType = type;
}

Level HalManifest::level() const {
    return mLevel;
}

Version HalManifest::getMetaVersion() const {
    return mMetaVersion;
}

const Version &HalManifest::sepolicyVersion() const {
    CHECK(mType == SchemaType::DEVICE);
    return device.mSepolicyVersion;
}

const std::vector<VendorNdk>& HalManifest::vendorNdks() const {
    CHECK(mType == SchemaType::FRAMEWORK);
    return framework.mVendorNdks;
}

std::string HalManifest::getXmlFilePath(const std::string& xmlFileName,
                                        const Version& version) const {
    using std::literals::string_literals::operator""s;
    auto range = getXmlFiles(xmlFileName);
    for (auto it = range.first; it != range.second; ++it) {
        const ManifestXmlFile& manifestXmlFile = it->second;
        if (manifestXmlFile.version() == version) {
            if (!manifestXmlFile.overriddenPath().empty()) {
                return manifestXmlFile.overriddenPath();
            }
            return "/"s + (type() == SchemaType::DEVICE ? "vendor" : "system") + "/etc/" +
                   xmlFileName + "_V" + std::to_string(version.majorVer) + "_" +
                   std::to_string(version.minorVer) + ".xml";
        }
    }
    return "";
}

bool operator==(const HalManifest &lft, const HalManifest &rgt) {
    return lft.mType == rgt.mType && lft.mLevel == rgt.mLevel && lft.mHals == rgt.mHals &&
           lft.mXmlFiles == rgt.mXmlFiles &&
           (lft.mType != SchemaType::DEVICE ||
            (lft.device.mSepolicyVersion == rgt.device.mSepolicyVersion &&
             lft.device.mKernel == rgt.device.mKernel)) &&
           (lft.mType != SchemaType::FRAMEWORK ||
            (
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                lft.framework.mVndks == rgt.framework.mVndks &&
#pragma clang diagnostic pop
                lft.framework.mVendorNdks == rgt.framework.mVendorNdks &&
                lft.framework.mSystemSdk == rgt.framework.mSystemSdk));
}

// Alternative to forEachInstance if you just need a set of instance names instead.
std::set<std::string> HalManifest::getInstances(const std::string& halName, const Version& version,
                                                const std::string& interfaceName) const {
    std::set<std::string> ret;
    (void)forEachInstanceOfInterface(halName, version, interfaceName, [&ret](const auto& e) {
        ret.insert(e.instance());
        return true;
    });
    return ret;
}

// Return whether instance is in getInstances(...).
bool HalManifest::hasInstance(const std::string& halName, const Version& version,
                              const std::string& interfaceName, const std::string& instance) const {
    bool found = false;
    (void)forEachInstanceOfInterface(halName, version, interfaceName,
                                     [&found, &instance](const auto& e) {
                                         found |= (instance == e.instance());
                                         return !found;  // if not found, continue
                                     });
    return found;
}

bool HalManifest::insertInstance(const FqInstance& fqInstance, Transport transport, Arch arch,
                                 HalFormat format, std::string* error) {
    for (ManifestHal& hal : getHals()) {
        if (hal.name == fqInstance.getPackage() && hal.format == format &&
            hal.transport() == transport && hal.arch() == arch) {
            return hal.insertInstance(fqInstance, error);
        }
    }

    ManifestHal hal;
    hal.name = fqInstance.getPackage();
    hal.format = format;
    hal.transportArch = TransportArch(transport, arch);
    if (!hal.insertInstance(fqInstance, error)) return false;
    return add(std::move(hal));
}

bool HalManifest::empty() const {
    HalManifest emptyManifest;
    emptyManifest.setType(type());
    return (*this) == emptyManifest;
}

const std::optional<KernelInfo>& HalManifest::kernel() const {
    return device.mKernel;
}

bool HalManifest::addAll(HalManifest* other, std::string* error) {
    if (other->mMetaVersion.majorVer != mMetaVersion.majorVer) {
        if (error) {
            *error = "Cannot merge manifest version " + to_string(mMetaVersion) + " and " +
                     to_string(other->mMetaVersion);
        }
        return false;
    }
    mMetaVersion.minorVer = std::max(mMetaVersion.minorVer, other->mMetaVersion.minorVer);

    if (type() != other->type()) {
        if (error) {
            *error = "Cannot add a " + to_string(other->type()) + " manifest to a " +
                     to_string(type()) + " manifest";
        }
        return false;
    }

    if (!addAllHals(other, error)) {
        return false;
    }

    if (!addAllXmlFiles(other, error)) {
        return false;
    }

    if (!mergeField(&mLevel, &other->mLevel, Level::UNSPECIFIED)) {
        if (error) {
            *error = "Conflicting target-level: " + to_string(level()) + " vs. " +
                     to_string(other->level());
        }
        return false;
    }

    if (type() == SchemaType::DEVICE) {
        if (!mergeField(&device.mSepolicyVersion, &other->device.mSepolicyVersion)) {
            if (error) {
                *error = "Conflicting sepolicy version: " + to_string(sepolicyVersion()) + " vs. " +
                         to_string(other->sepolicyVersion());
            }
            return false;
        }

        if (!mergeField(&device.mKernel, &other->device.mKernel)) {
            // If fails, both have values.
            if (error) {
                *error = "Conflicting kernel: " + to_string(device.mKernel->version()) + " vs. " +
                         to_string(other->device.mKernel->version());
            }
            return false;
        }
    } else if (type() == SchemaType::FRAMEWORK) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        framework.mVndks.insert(framework.mVndks.end(), other->framework.mVndks.begin(),
                                other->framework.mVndks.end());
        other->framework.mVndks.clear();
#pragma clang diagnostic pop

        framework.mVendorNdks.insert(framework.mVendorNdks.end(),
                                     other->framework.mVendorNdks.begin(),
                                     other->framework.mVendorNdks.end());
        other->framework.mVendorNdks.clear();

        framework.mSystemSdk.addAll(&other->framework.mSystemSdk);
    } else {
        LOG(FATAL) << "unknown SchemaType: "
                   << static_cast<std::underlying_type_t<SchemaType>>(type());
    }

    if (!other->empty()) {
        if (error) {
            *error =
                "Cannot add another manifest because it contains extraneous entries that "
                "are not recognized.";
        }
        return false;
    }

    return true;
}

} // namespace vintf
} // namespace android
