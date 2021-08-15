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

#ifndef ANDROID_VINTF_COMPATIBILITY_MATRIX_H
#define ANDROID_VINTF_COMPATIBILITY_MATRIX_H

#include <map>
#include <memory>
#include <string>

#include <utils/Errors.h>

#include "FileSystem.h"
#include "HalGroup.h"
#include "Level.h"
#include "MapValueIterator.h"
#include "MatrixHal.h"
#include "MatrixInstance.h"
#include "MatrixKernel.h"
#include "Named.h"
#include "SchemaType.h"
#include "Sepolicy.h"
#include "SystemSdk.h"
#include "VendorNdk.h"
#include "Vndk.h"
#include "XmlFileGroup.h"

namespace android {
namespace vintf {

// Compatibility matrix defines what hardware does the framework requires.
struct CompatibilityMatrix : public HalGroup<MatrixHal>, public XmlFileGroup<MatrixXmlFile> {
    // Create a framework compatibility matrix.
    CompatibilityMatrix() : mType(SchemaType::FRAMEWORK) {}

    SchemaType type() const;
    Level level() const;
    Version getMinimumMetaVersion() const;

    // If the corresponding <xmlfile> with the given version exists, for the first match,
    // - Return the overridden <path> if it is present,
    // - otherwise the default value: /{system,vendor}/etc/<name>_V<major>_<minor-max>.<format>
    // Otherwise if the <xmlfile> entry does not exist, "" is returned.
    // For example, if the matrix says ["audio@1.0-5" -> "foo.xml", "audio@1.3-7" -> bar.xml]
    // getXmlSchemaPath("audio", 1.0) -> foo.xml
    // getXmlSchemaPath("audio", 1.5) -> foo.xml
    // getXmlSchemaPath("audio", 1.7) -> bar.xml
    // (Normally, version ranges do not overlap, and the only match is returned.)
    std::string getXmlSchemaPath(const std::string& xmlFileName, const Version& version) const;

    bool forEachInstanceOfVersion(
        const std::string& package, const Version& expectVersion,
        const std::function<bool(const MatrixInstance&)>& func) const override;

    std::string getVendorNdkVersion() const;

   private:
    // Add everything in inputMatrix to "this" as requirements.
    bool addAll(Named<CompatibilityMatrix>* inputMatrix, std::string* error);

    // Add all <kernel> from other to "this". Error if there is a conflict.
    bool addAllKernels(CompatibilityMatrix* other, std::string* error);

    // Add a <kernel> tag to "this". Error if there is a conflict.
    bool addKernel(MatrixKernel&& kernel, std::string* error);

    // Merge <sepolicy> with other's <sepolicy>. Error if there is a conflict.
    bool addSepolicy(CompatibilityMatrix* other, std::string* error);

    // Merge <avb><vbmeta-version> with other's <avb><vbmeta-version>. Error if there is a conflict.
    bool addAvbMetaVersion(CompatibilityMatrix* other, std::string* error);

    // Merge <vndk> with other's <vndk>. Error if there is a conflict.
    bool addVndk(CompatibilityMatrix* other, std::string* error);

    // Merge <vendor-ndk> with other's <vendor-ndk>. Error if there is a conflict.
    bool addVendorNdk(CompatibilityMatrix* other, std::string* error);

    // Merge <system-sdk> with other's <system-sdk>.
    bool addSystemSdk(CompatibilityMatrix* other, std::string* error);

    // Add everything in inputMatrix to "this" as optional.
    bool addAllAsOptional(Named<CompatibilityMatrix>* inputMatrix, std::string* error);

    // Add all HALs as optional HALs from "other". This function moves MatrixHal objects
    // from "other".
    // Require other->level() > this->level(), otherwise do nothing.
    bool addAllHalsAsOptional(CompatibilityMatrix* other, std::string* error);

    // Similar to addAllHalsAsOptional but on <xmlfile> entries.
    bool addAllXmlFilesAsOptional(CompatibilityMatrix* other, std::string* error);

    // Similar to addAllHalsAsOptional but on <kernel> entries.
    bool addAllKernelsAsOptional(CompatibilityMatrix* other, std::string* error);

    // Combine a set of framework compatibility matrices. For each CompatibilityMatrix in matrices
    // (in the order of level(), where UNSPECIFIED (empty) is treated as deviceLevel)
    // - If level() < deviceLevel, ignore
    // - If level() == UNSPECIFIED or level() == deviceLevel,
    //   - Add as hard requirements. See combineSameFcmVersion
    // - If level() > deviceLevel,
    //   - all <hal> versions and <xmlfile>s are added as optional.
    //   - <kernel minlts="x.y.z"> is added only if x.y does not exist in a file
    //     with lower level()
    //   - <sepolicy>, <avb><vbmeta-version> is ignored
    // Return the combined matrix, nullptr if any error (e.g. conflict of information).
    static std::unique_ptr<CompatibilityMatrix> combine(
        Level deviceLevel, std::vector<Named<CompatibilityMatrix>>* matrices, std::string* error);

    // Combine a set of device compatibility matrices.
    static std::unique_ptr<CompatibilityMatrix> combineDeviceMatrices(
        std::vector<Named<CompatibilityMatrix>>* matrices, std::string* error);

    status_t fetchAllInformation(const FileSystem* fileSystem, const std::string& path,
                                 std::string* error = nullptr);

    MatrixHal* splitInstance(MatrixHal* existingHal, const std::string& interface,
                             const std::string& instance, bool isRegex);

    // Return whether instance is in "this"; that is, instance is in any <instance> tag or
    // matches any <regex-instance> tag.
    bool matchInstance(const std::string& halName, const Version& version,
                       const std::string& interfaceName, const std::string& instance) const;

    friend struct HalManifest;
    friend struct RuntimeInfo;
    friend struct CompatibilityMatrixConverter;
    friend struct LibVintfTest;
    friend struct FrameworkCompatibilityMatrixCombineTest;
    friend struct DeviceCompatibilityMatrixCombineTest;
    friend class VintfObject;
    friend class AssembleVintfImpl;
    friend bool operator==(const CompatibilityMatrix &, const CompatibilityMatrix &);

    SchemaType mType;
    Level mLevel = Level::UNSPECIFIED;

    // entries only for framework compatibility matrix.
    struct {
        std::vector<MatrixKernel> mKernels;
        Sepolicy mSepolicy;
        Version mAvbMetaVersion;
    } framework;

    // entries only for device compatibility matrix.
    struct {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        Vndk mVndk;
#pragma clang diagnostic pop

        VendorNdk mVendorNdk;
        SystemSdk mSystemSdk;
    } device;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_COMPATIBILITY_MATRIX_H
