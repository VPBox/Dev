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


#ifndef ANDROID_VINTF_HAL_MANIFEST_H
#define ANDROID_VINTF_HAL_MANIFEST_H

#include <utils/Errors.h>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "FileSystem.h"
#include "HalGroup.h"
#include "KernelInfo.h"
#include "Level.h"
#include "ManifestHal.h"
#include "ManifestInstance.h"
#include "MapValueIterator.h"
#include "SchemaType.h"
#include "SystemSdk.h"
#include "VendorNdk.h"
#include "Version.h"
#include "Vndk.h"
#include "XmlFileGroup.h"

namespace android {
namespace vintf {

struct MatrixHal;
struct CompatibilityMatrix;

namespace details {
using InstancesOfVersion =
    std::map<std::string /* interface */, std::set<std::string /* instance */>>;
using Instances = std::map<Version, InstancesOfVersion>;
}  // namespace details

// A HalManifest is reported by the hardware and query-able from
// framework code. This is the API for the framework.
struct HalManifest : public HalGroup<ManifestHal>, public XmlFileGroup<ManifestXmlFile> {
   public:

    // Construct a device HAL manifest.
    HalManifest() : mType(SchemaType::DEVICE) {}

    bool add(ManifestHal&& hal) override;

    // Given a component name (e.g. "android.hardware.camera"),
    // return getHal(name)->transport if the component exist and v exactly matches
    // one of the versions in that component, else EMPTY
    Transport getTransport(const std::string &name, const Version &v,
            const std::string &interfaceName, const std::string &instanceName) const;

    // Check compatibility against a compatibility matrix. Considered compatible if
    // - framework manifest vs. device compat-mat
    //     - checkIncompatibility for HALs returns only optional HALs
    //     - one of manifest.vndk match compat-mat.vndk
    // - device manifest vs. framework compat-mat
    //     - checkIncompatibility for HALs returns only optional HALs
    //     - manifest.sepolicy.version match one of compat-mat.sepolicy.sepolicy-version
    bool checkCompatibility(const CompatibilityMatrix &mat, std::string *error = nullptr) const;

    // Generate a compatibility matrix such that checkCompatibility will return true.
    CompatibilityMatrix generateCompatibleMatrix() const;

    // Returns all component names.
    std::set<std::string> getHalNames() const;

    // Returns all component names and versions, e.g.
    // "android.hardware.camera.device@1.0", "android.hardware.camera.device@3.2",
    // "android.hardware.nfc@1.0"]
    std::set<std::string> getHalNamesAndVersions() const;

    // Type of the manifest. FRAMEWORK or DEVICE.
    SchemaType type() const;
    void setType(SchemaType type);

    // FCM version that it implements.
    Level level() const;

    // device.mSepolicyVersion. Assume type == device.
    // Abort if type != device.
    const Version &sepolicyVersion() const;

    // framework.mVendorNdks. Assume type == framework.
    // Abort if type != framework.
    const std::vector<VendorNdk>& vendorNdks() const;

    // If the corresponding <xmlfile> with the given version exists,
    // - Return the overridden <path> if it is present,
    // - otherwise the default value: /{system,vendor}/etc/<name>_V<major>_<minor>.xml
    // Otherwise if the <xmlfile> entry does not exist, "" is returned.
    std::string getXmlFilePath(const std::string& xmlFileName, const Version& version) const;

    // Get metaversion of this manifest.
    Version getMetaVersion() const;

    bool forEachInstanceOfVersion(
        const std::string& package, const Version& expectVersion,
        const std::function<bool(const ManifestInstance&)>& func) const override;

    // Alternative to forEachInstance if you just need a set of instance names instead.
    std::set<std::string> getInstances(const std::string& halName, const Version& version,
                                       const std::string& interfaceName) const;

    // Return whether instance is in getInstances(...).
    bool hasInstance(const std::string& halName, const Version& version,
                     const std::string& interfaceName, const std::string& instance) const;

    // Insert the given instance. After inserting it, the instance will be available via
    // forEachInstance* functions. This modifies the manifest.
    // Return whether this operation is successful.
    bool insertInstance(const FqInstance& fqInstance, Transport transport, Arch arch, HalFormat fmt,
                        std::string* error = nullptr);

    // Get the <kernel> tag. Assumes type() == DEVICE.
    const std::optional<KernelInfo>& kernel() const;

    // Add everything from another manifest. If no errors (return true), it is guaranteed
    // that other->empty() == true after execution.
    [[nodiscard]] bool addAll(HalManifest* other, std::string* error = nullptr);

   protected:
    // Check before add()
    bool shouldAdd(const ManifestHal& toAdd) const override;
    bool shouldAddXmlFile(const ManifestXmlFile& toAdd) const override;

   private:
    friend struct HalManifestConverter;
    friend class VintfObject;
    friend class AssembleVintfImpl;
    friend struct LibVintfTest;
    friend std::string dump(const HalManifest &vm);
    friend bool operator==(const HalManifest &lft, const HalManifest &rgt);

    status_t fetchAllInformation(const FileSystem* fileSystem, const std::string& path,
                                 std::string* error = nullptr);

    details::Instances expandInstances(const std::string& name) const;
    // Check if all instances in matrixHal is supported in this manifest.
    bool isCompatible(const details::Instances& instances, const MatrixHal& matrixHal) const;

    // Return a list of error messages (for each <hal> name) that does NOT conform to
    // the given compatibility matrix. It does not contain components that are optional.
    // That is, return empty list iff
    // (instance in matrix) => (instance in manifest).
    std::vector<std::string> checkIncompatibleHals(const CompatibilityMatrix& mat) const;

    void removeHals(const std::string& name, size_t majorVer);

    // Returns a list of instance names that are in this manifest but
    // are not specified in the given matrix, whether the HAL is specified as an optional or
    // required HAL.
    // That is, return empty list iff
    // (instance in manifest) => (instance in matrix).
    std::set<std::string> checkUnusedHals(const CompatibilityMatrix& mat) const;

    // Check that manifest has no entries.
    bool empty() const;

    SchemaType mType;
    Level mLevel = Level::UNSPECIFIED;
    // version attribute. Default is 1.0 for manifests created programatically.
    Version mMetaVersion{1, 0};

    // entries for device hal manifest only
    struct {
        Version mSepolicyVersion;
        std::optional<KernelInfo> mKernel;
    } device;

    // entries for framework hal manifest only
    struct {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        std::vector<Vndk> mVndks;
#pragma clang diagnostic pop

        std::vector<VendorNdk> mVendorNdks;
        SystemSdk mSystemSdk;
    } framework;
};


} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_HAL_MANIFEST_H
