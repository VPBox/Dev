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

#include "CompatibilityMatrix.h"

#include <iostream>
#include <utility>

#include <android-base/strings.h>

#include "parse_string.h"
#include "parse_xml.h"
#include "utils.h"

namespace android {
namespace vintf {

using details::mergeField;

bool CompatibilityMatrix::addKernel(MatrixKernel&& kernel, std::string* error) {
    if (mType != SchemaType::FRAMEWORK) {
        if (error) {
            *error = "Cannot add <kernel> to a " + to_string(mType) + " compatibility matrix.";
        }
        return false;
    }

    auto it = framework.mKernels.begin();
    for (; it != framework.mKernels.end(); ++it) {
        if (it->minLts() == kernel.minLts()) {
            break;
        }
        if (it->minLts().version == kernel.minLts().version &&
            it->minLts().majorRev == kernel.minLts().majorRev) {
            if (error) {
                *error = "Kernel version mismatch; cannot add " + to_string(kernel.minLts()) +
                         " because " + to_string(it->minLts()) + " was added.";
            }
            return false;
        }
    }

    bool seenVersion = it != framework.mKernels.end();

    if (seenVersion) {
        // If no conditions, must be the first among the same minLts
        // because O libvintf only checks the first <kernel> tag that version matches.
        if (kernel.conditions().empty()) {
            // Found first <kernel> with the same minLts.
            // Append config if it does not have <condition>s, else error.
            if (it->conditions().empty()) {
                const auto& configs = kernel.configs();
                it->mConfigs.insert(it->mConfigs.end(), configs.begin(), configs.end());
            } else {
                if (error) {
                    *error =
                        "Base compatibility matrix has <condition> for the first <kernel> "
                        "with minlts " +
                        to_string(kernel.minLts()) + " for unknown reason.";
                }
                return false;
            }
            return true;
        }
    } else {
        // First <kernel> of a minLts must not have <condition>'s for backwards compatibility
        // with O libvintf.
        if (!kernel.conditions().empty()) {
            framework.mKernels.push_back(MatrixKernel(KernelVersion{kernel.minLts()}, {}));
        }
    }

    framework.mKernels.push_back(std::move(kernel));
    return true;
}

SchemaType CompatibilityMatrix::type() const {
    return mType;
}

Level CompatibilityMatrix::level() const {
    return mLevel;
}

Version CompatibilityMatrix::getMinimumMetaVersion() const {
    // TODO(b/62801658): this needs to depend on whether there are 1.1 requirements
    // (e.g. required <xmlfile> entry)
    return {1, 0};
}

status_t CompatibilityMatrix::fetchAllInformation(const FileSystem* fileSystem,
                                                  const std::string& path, std::string* error) {
    return details::fetchAllInformation(fileSystem, path, gCompatibilityMatrixConverter, this,
                                        error);
}

std::string CompatibilityMatrix::getXmlSchemaPath(const std::string& xmlFileName,
                                                  const Version& version) const {
    using std::literals::string_literals::operator""s;
    auto range = getXmlFiles(xmlFileName);
    for (auto it = range.first; it != range.second; ++it) {
        const MatrixXmlFile& matrixXmlFile = it->second;
        if (matrixXmlFile.versionRange().contains(version)) {
            if (!matrixXmlFile.overriddenPath().empty()) {
                return matrixXmlFile.overriddenPath();
            }
            return "/"s + (type() == SchemaType::DEVICE ? "vendor" : "system") + "/etc/" +
                   xmlFileName + "_V" + std::to_string(matrixXmlFile.versionRange().majorVer) +
                   "_" + std::to_string(matrixXmlFile.versionRange().maxMinor) + "." +
                   to_string(matrixXmlFile.format());
        }
    }
    return "";
}

// Split existingHal into a HAL that contains only interface/instance and a HAL
// that does not contain it. Return the HAL that contains only interface/instance.
// - Return nullptr if existingHal does not contain interface/instance
// - Return existingHal if existingHal contains only interface/instance
// - Remove interface/instance from existingHal, and return a new MatrixHal (that is added
//   to "this") that contains only interface/instance.
MatrixHal* CompatibilityMatrix::splitInstance(MatrixHal* existingHal, const std::string& interface,
                                              const std::string& instanceOrPattern, bool isRegex) {
    bool found = false;
    bool foundOthers = false;
    existingHal->forEachInstance([&](const auto& matrixInstance) {
        bool interfaceMatch = matrixInstance.interface() == interface;
        bool instanceMatch = false;
        if (matrixInstance.isRegex() && isRegex) {
            instanceMatch = (matrixInstance.regexPattern() == instanceOrPattern);
        } else if (!matrixInstance.isRegex() && !isRegex) {
            instanceMatch = (matrixInstance.exactInstance() == instanceOrPattern);
        }

        bool match = interfaceMatch && instanceMatch;

        found |= match;
        foundOthers |= (!match);

        return !found || !foundOthers;
    });

    if (!found) {
        return nullptr;
    }

    if (!foundOthers) {
        return existingHal;
    }

    existingHal->removeInstance(interface, instanceOrPattern, isRegex);
    MatrixHal copy = *existingHal;
    copy.clearInstances();
    copy.insertInstance(interface, instanceOrPattern, isRegex);

    return addInternal(std::move(copy));
}

// Add all package@other_version::interface/instance as an optional instance.
// If package@this_version::interface/instance is in this (that is, some instance
// with the same package and interface and instance exists), then other_version is
// considered a possible replacement to this_version.
// See LibVintfTest.AddOptionalHal* tests for details.
bool CompatibilityMatrix::addAllHalsAsOptional(CompatibilityMatrix* other, std::string* error) {
    if (other == nullptr || other->level() <= level()) {
        return true;
    }

    for (auto& pair : other->mHals) {
        const std::string& name = pair.first;
        MatrixHal& halToAdd = pair.second;

        std::set<std::pair<std::string, std::string>> insertedInstances;
        std::set<std::pair<std::string, std::string>> insertedRegex;
        auto existingHals = getHals(name);

        halToAdd.forEachInstance([&](const std::vector<VersionRange>& versionRanges,
                                     const std::string& interface,
                                     const std::string& instanceOrPattern, bool isRegex) {
            for (auto* existingHal : existingHals) {
                MatrixHal* splitInstance =
                    this->splitInstance(existingHal, interface, instanceOrPattern, isRegex);
                if (splitInstance != nullptr) {
                    splitInstance->insertVersionRanges(versionRanges);
                    if (isRegex) {
                        insertedRegex.insert(std::make_pair(interface, instanceOrPattern));
                    } else {
                        insertedInstances.insert(std::make_pair(interface, instanceOrPattern));
                    }
                }
            }
            return true;
        });

        // Add the remaining instances.
        for (const auto& pair : insertedInstances) {
            halToAdd.removeInstance(pair.first, pair.second, false /* isRegex */);
        }
        for (const auto& pair : insertedRegex) {
            halToAdd.removeInstance(pair.first, pair.second, true /* isRegex */);
        }

        if (halToAdd.instancesCount() > 0) {
            halToAdd.setOptional(true);
            if (!add(std::move(halToAdd))) {
                if (error) {
                    *error = "Cannot add HAL " + name + " for unknown reason.";
                }
                return false;
            }
        }
    }
    return true;
}

bool CompatibilityMatrix::addAllXmlFilesAsOptional(CompatibilityMatrix* other, std::string* error) {
    if (other == nullptr || other->level() <= level()) {
        return true;
    }
    for (auto& pair : other->mXmlFiles) {
        const std::string& name = pair.first;
        MatrixXmlFile& xmlFileToAdd = pair.second;

        xmlFileToAdd.mOptional = true;
        if (!addXmlFile(std::move(xmlFileToAdd))) {
            if (error) {
                *error = "Cannot add XML File " + name + " for unknown reason.";
            }
            return false;
        }
    }
    return true;
}

// Merge Kernel.
// Add <kernel> from exact "level", then optionally add <kernel> from high levels to low levels.
// For example, (each letter is a kernel version x.y.z)
// 1.xml: A1, B1
// 2.xml: B2, C2, D2
// 3.xml: D3, E3
// Then the combined 1.xml should have
// A1, B1 (from 1.xml, required), C2, D2, E3 (optional, use earliest possible).
bool CompatibilityMatrix::addAllKernels(CompatibilityMatrix* other, std::string* error) {
    for (MatrixKernel& kernel : other->framework.mKernels) {
        KernelVersion ver = kernel.minLts();
        if (!addKernel(std::move(kernel), error)) {
            if (error) {
                *error = "Cannot add kernel version " + to_string(ver) + ": " + *error;
            }
            return false;
        }
    }
    return true;
}

bool CompatibilityMatrix::addAllKernelsAsOptional(CompatibilityMatrix* other, std::string* error) {
    if (other == nullptr || other->level() <= level()) {
        return true;
    }

    for (MatrixKernel& kernelToAdd : other->framework.mKernels) {
        bool exists =
            std::any_of(this->framework.mKernels.begin(), this->framework.mKernels.end(),
                        [&kernelToAdd](const MatrixKernel& existing) {
                            return kernelToAdd.minLts().version == existing.minLts().version &&
                                   kernelToAdd.minLts().majorRev == existing.minLts().majorRev;
                        });

        if (exists) {
            // Shouldn't retroactively add requirements to minLts(), so ignore this.
            // This happens even when kernelToAdd.conditions() != existing.conditions().
            continue;
        }

        KernelVersion minLts = kernelToAdd.minLts();
        if (!addKernel(std::move(kernelToAdd), error)) {
            if (error) {
                *error = "Cannot add " + to_string(minLts) + ": " + *error;
            }
            return false;
        }
    }
    return true;
}

bool CompatibilityMatrix::addSepolicy(CompatibilityMatrix* other, std::string* error) {
    bool success = mergeField(&this->framework.mSepolicy, &other->framework.mSepolicy);
    if (!success && error) *error = "<sepolicy> is already defined";
    return success;
}

bool CompatibilityMatrix::addAvbMetaVersion(CompatibilityMatrix* other, std::string* error) {
    bool success = mergeField(&this->framework.mAvbMetaVersion, &other->framework.mAvbMetaVersion);
    if (!success && error) *error = "<avb><vbmeta-version> is already defined";
    return success;
}

bool CompatibilityMatrix::addVndk(CompatibilityMatrix* other, std::string* error) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    bool success = mergeField(&this->device.mVndk, &other->device.mVndk);
#pragma clang diagnostic pop
    if (!success && error) *error = "<vndk> is already defined";
    return success;
}

bool CompatibilityMatrix::addVendorNdk(CompatibilityMatrix* other, std::string* error) {
    bool success = mergeField(&this->device.mVendorNdk, &other->device.mVendorNdk);
    if (!success && error) *error = "<vendor-ndk> is already defined";
    return success;
}

bool CompatibilityMatrix::addSystemSdk(CompatibilityMatrix* other, std::string* /* error */) {
    this->device.mSystemSdk.addAll(&other->device.mSystemSdk);
    return true;
}

bool operator==(const CompatibilityMatrix &lft, const CompatibilityMatrix &rgt) {
    return lft.mType == rgt.mType && lft.mLevel == rgt.mLevel && lft.mHals == rgt.mHals &&
           lft.mXmlFiles == rgt.mXmlFiles &&
           (lft.mType != SchemaType::DEVICE ||
            (
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                lft.device.mVndk == rgt.device.mVndk &&
#pragma clang diagnostic pop
                lft.device.mVendorNdk == rgt.device.mVendorNdk &&
                lft.device.mSystemSdk == rgt.device.mSystemSdk)) &&
           (lft.mType != SchemaType::FRAMEWORK ||
            (lft.framework.mKernels == rgt.framework.mKernels &&
             lft.framework.mSepolicy == rgt.framework.mSepolicy &&
             lft.framework.mAvbMetaVersion == rgt.framework.mAvbMetaVersion));
}

std::unique_ptr<CompatibilityMatrix> CompatibilityMatrix::combine(
    Level deviceLevel, std::vector<Named<CompatibilityMatrix>>* matrices, std::string* error) {
    // Check type.
    for (const auto& e : *matrices) {
        if (e.object.type() != SchemaType::FRAMEWORK) {
            if (error) {
                *error = "File \"" + e.name + "\" is not a framework compatibility matrix.";
                return nullptr;
            }
        }
    }

    // Matrices with unspecified (empty) level are auto-filled with deviceLevel.
    for (auto& e : *matrices) {
        if (e.object.level() == Level::UNSPECIFIED) {
            e.object.mLevel = deviceLevel;
        }
    }

    // Add from low to high FCM version so that optional <kernel> requirements are added correctly.
    // See comment in addAllAsOptional.
    std::sort(matrices->begin(), matrices->end(),
              [](const auto& x, const auto& y) { return x.object.level() < y.object.level(); });

    auto baseMatrix = std::make_unique<CompatibilityMatrix>();
    baseMatrix->mLevel = deviceLevel;
    baseMatrix->mType = SchemaType::FRAMEWORK;

    std::vector<std::string> parsedFiles;
    for (auto& e : *matrices) {
        if (e.object.level() < deviceLevel) {
            continue;
        }

        bool success = false;
        if (e.object.level() == deviceLevel) {
            success = baseMatrix->addAll(&e, error);
        } else {
            success = baseMatrix->addAllAsOptional(&e, error);
        }
        if (!success) {
            if (error) {
                *error = "Conflict when merging \"" + e.name + "\": " + *error + "\n" +
                         "Previous files:\n" + base::Join(parsedFiles, "\n");
            }
            return nullptr;
        }
        parsedFiles.push_back(e.name);
    }

    return baseMatrix;
}

std::unique_ptr<CompatibilityMatrix> CompatibilityMatrix::combineDeviceMatrices(
    std::vector<Named<CompatibilityMatrix>>* matrices, std::string* error) {
    auto baseMatrix = std::make_unique<CompatibilityMatrix>();
    baseMatrix->mType = SchemaType::DEVICE;

    std::vector<std::string> parsedFiles;
    for (auto& e : *matrices) {
        bool success = baseMatrix->addAll(&e, error);
        if (!success) {
            if (error) {
                *error = "Conflict when merging \"" + e.name + "\": " + *error + "\n" +
                         "Previous files:\n" + base::Join(parsedFiles, "\n");
            }
            return nullptr;
        }
        parsedFiles.push_back(e.name);
    }
    return baseMatrix;
}

bool CompatibilityMatrix::addAll(Named<CompatibilityMatrix>* inputMatrix, std::string* error) {
    if (!addAllHals(&inputMatrix->object, error) || !addAllXmlFiles(&inputMatrix->object, error) ||
        !addAllKernels(&inputMatrix->object, error) || !addSepolicy(&inputMatrix->object, error) ||
        !addAvbMetaVersion(&inputMatrix->object, error) || !addVndk(&inputMatrix->object, error) ||
        !addVendorNdk(&inputMatrix->object, error) || !addSystemSdk(&inputMatrix->object, error)) {
        if (error) {
            *error = "File \"" + inputMatrix->name + "\" cannot be added: " + *error + ".";
        }
        return false;
    }
    return true;
}

bool CompatibilityMatrix::addAllAsOptional(Named<CompatibilityMatrix>* inputMatrix,
                                           std::string* error) {
    if (!addAllHalsAsOptional(&inputMatrix->object, error) ||
        !addAllXmlFilesAsOptional(&inputMatrix->object, error) ||
        !addAllKernelsAsOptional(&inputMatrix->object, error)) {
        if (error) {
            *error = "File \"" + inputMatrix->name + "\" cannot be added: " + *error;
        }
        return false;
    }
    // ignore <sepolicy> requirement from higher level
    // ignore <avb> requirement from higher level
    return true;
}

bool CompatibilityMatrix::forEachInstanceOfVersion(
    const std::string& package, const Version& expectVersion,
    const std::function<bool(const MatrixInstance&)>& func) const {
    for (const MatrixHal* hal : getHals(package)) {
        bool cont = hal->forEachInstance([&](const MatrixInstance& matrixInstance) {
            if (matrixInstance.versionRange().contains(expectVersion)) {
                return func(matrixInstance);
            }
            return true;
        });
        if (!cont) return false;
    }
    return true;
}

bool CompatibilityMatrix::matchInstance(const std::string& halName, const Version& version,
                                        const std::string& interfaceName,
                                        const std::string& instance) const {
    bool found = false;
    (void)forEachInstanceOfInterface(halName, version, interfaceName,
                                     [&found, &instance](const auto& e) {
                                         found |= (e.matchInstance(instance));
                                         return !found;  // if not found, continue
                                     });
    return found;
}

std::string CompatibilityMatrix::getVendorNdkVersion() const {
    return type() == SchemaType::DEVICE ? device.mVendorNdk.version() : "";
}

} // namespace vintf
} // namespace android
