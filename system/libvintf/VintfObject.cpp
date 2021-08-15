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

#include "VintfObject.h"

#include <dirent.h>

#include <functional>
#include <memory>
#include <mutex>

#include <android-base/logging.h>

#include "CompatibilityMatrix.h"
#include "VintfObjectAfterUpdate.h"
#include "parse_string.h"
#include "parse_xml.h"
#include "utils.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace android {
namespace vintf {

using namespace details;

#ifdef LIBVINTF_TARGET
static constexpr bool kIsTarget = true;
#else
static constexpr bool kIsTarget = false;
#endif

template <typename T, typename F>
static std::shared_ptr<const T> Get(
        LockedSharedPtr<T> *ptr,
        bool skipCache,
        const F &fetchAllInformation) {
    std::unique_lock<std::mutex> _lock(ptr->mutex);
    if (skipCache || !ptr->fetchedOnce) {
        ptr->object = std::make_unique<T>();
        std::string error;
        if (fetchAllInformation(ptr->object.get(), &error) != OK) {
            LOG(WARNING) << error;
            ptr->object = nullptr; // frees the old object
        }
        ptr->fetchedOnce = true;
    }
    return ptr->object;
}

static std::unique_ptr<FileSystem> createDefaultFileSystem() {
    std::unique_ptr<FileSystem> fileSystem;
    if (kIsTarget) {
        fileSystem = std::make_unique<details::FileSystemImpl>();
    } else {
        fileSystem = std::make_unique<details::FileSystemNoOp>();
    }
    return fileSystem;
}

static std::unique_ptr<PropertyFetcher> createDefaultPropertyFetcher() {
    std::unique_ptr<PropertyFetcher> propertyFetcher;
    if (kIsTarget) {
        propertyFetcher = std::make_unique<details::PropertyFetcherImpl>();
    } else {
        propertyFetcher = std::make_unique<details::PropertyFetcherNoOp>();
    }
    return propertyFetcher;
}

details::LockedSharedPtr<VintfObject> VintfObject::sInstance{};
std::shared_ptr<VintfObject> VintfObject::GetInstance() {
    std::unique_lock<std::mutex> lock(sInstance.mutex);
    if (sInstance.object == nullptr) {
        sInstance.object = std::shared_ptr<VintfObject>(VintfObject::Builder().build().release());
    }
    return sInstance.object;
}

std::shared_ptr<const HalManifest> VintfObject::GetDeviceHalManifest(bool skipCache) {
    return GetInstance()->getDeviceHalManifest(skipCache);
}

std::shared_ptr<const HalManifest> VintfObject::getDeviceHalManifest(bool skipCache) {
    return Get(&mDeviceManifest, skipCache,
               std::bind(&VintfObject::fetchDeviceHalManifest, this, _1, _2));
}

std::shared_ptr<const HalManifest> VintfObject::GetFrameworkHalManifest(bool skipCache) {
    return GetInstance()->getFrameworkHalManifest(skipCache);
}

std::shared_ptr<const HalManifest> VintfObject::getFrameworkHalManifest(bool skipCache) {
    return Get(&mFrameworkManifest, skipCache,
               std::bind(&VintfObject::fetchFrameworkHalManifest, this, _1, _2));
}

std::shared_ptr<const CompatibilityMatrix> VintfObject::GetDeviceCompatibilityMatrix(bool skipCache) {
    return GetInstance()->getDeviceCompatibilityMatrix(skipCache);
}

std::shared_ptr<const CompatibilityMatrix> VintfObject::getDeviceCompatibilityMatrix(
    bool skipCache) {
    return Get(&mDeviceMatrix, skipCache, std::bind(&VintfObject::fetchDeviceMatrix, this, _1, _2));
}

std::shared_ptr<const CompatibilityMatrix> VintfObject::GetFrameworkCompatibilityMatrix(bool skipCache) {
    return GetInstance()->getFrameworkCompatibilityMatrix(skipCache);
}

std::shared_ptr<const CompatibilityMatrix> VintfObject::getFrameworkCompatibilityMatrix(
    bool skipCache) {
    // To avoid deadlock, get device manifest before any locks.
    auto deviceManifest = getDeviceHalManifest();

    std::unique_lock<std::mutex> _lock(mFrameworkCompatibilityMatrixMutex);

    auto combined =
        Get(&mCombinedFrameworkMatrix, skipCache,
            std::bind(&VintfObject::getCombinedFrameworkMatrix, this, deviceManifest, _1, _2));
    if (combined != nullptr) {
        return combined;
    }

    return Get(&mFrameworkMatrix, skipCache,
               std::bind(&CompatibilityMatrix::fetchAllInformation, _1, getFileSystem().get(),
                         kSystemLegacyMatrix, _2));
}

status_t VintfObject::getCombinedFrameworkMatrix(
    const std::shared_ptr<const HalManifest>& deviceManifest, CompatibilityMatrix* out,
    std::string* error) {
    std::vector<Named<CompatibilityMatrix>> matrixFragments;
    auto matrixFragmentsStatus = getAllFrameworkMatrixLevels(&matrixFragments, error);
    if (matrixFragmentsStatus != OK) {
        return matrixFragmentsStatus;
    }
    if (matrixFragments.empty()) {
        if (error && error->empty()) {
            *error = "Cannot get framework matrix for each FCM version for unknown error.";
        }
        return NAME_NOT_FOUND;
    }

    Level deviceLevel = Level::UNSPECIFIED;

    if (deviceManifest != nullptr) {
        deviceLevel = deviceManifest->level();
    }

    // TODO(b/70628538): Do not infer from Shipping API level.
    if (deviceLevel == Level::UNSPECIFIED) {
        auto shippingApi = getPropertyFetcher()->getUintProperty("ro.product.first_api_level", 0u);
        if (shippingApi != 0u) {
            deviceLevel = details::convertFromApiLevel(shippingApi);
        }
    }

    if (deviceLevel == Level::UNSPECIFIED) {
        // Cannot infer FCM version. Combine all matrices by assuming
        // Shipping FCM Version == min(all supported FCM Versions in the framework)
        for (auto&& pair : matrixFragments) {
            Level fragmentLevel = pair.object.level();
            if (fragmentLevel != Level::UNSPECIFIED && deviceLevel > fragmentLevel) {
                deviceLevel = fragmentLevel;
            }
        }
    }

    if (deviceLevel == Level::UNSPECIFIED) {
        // None of the fragments specify any FCM version. Should never happen except
        // for inconsistent builds.
        if (error) {
            *error = "No framework compatibility matrix files under " + kSystemVintfDir +
                     " declare FCM version.";
        }
        return NAME_NOT_FOUND;
    }

    auto combined = CompatibilityMatrix::combine(deviceLevel, &matrixFragments, error);
    if (combined == nullptr) {
        return BAD_VALUE;
    }
    *out = std::move(*combined);
    return OK;
}

// Load and combine all of the manifests in a directory
status_t VintfObject::addDirectoryManifests(const std::string& directory, HalManifest* manifest,
                                            std::string* error) {
    std::vector<std::string> fileNames;
    status_t err = getFileSystem()->listFiles(directory, &fileNames, error);
    // if the directory isn't there, that's okay
    if (err == NAME_NOT_FOUND) return OK;
    if (err != OK) return err;

    for (const std::string& file : fileNames) {
        // Only adds HALs because all other things are added by libvintf
        // itself for now.
        HalManifest fragmentManifest;
        err = fetchOneHalManifest(directory + file, &fragmentManifest, error);
        if (err != OK) return err;

        if (!manifest->addAll(&fragmentManifest, error)) {
            if (error) {
                error->insert(0, "Cannot add manifest fragment " + directory + file + ":");
            }
            return UNKNOWN_ERROR;
        }
    }

    return OK;
}

// Priority for loading vendor manifest:
// 1. /vendor/etc/vintf/manifest.xml + device fragments + ODM manifest (optional) + odm fragments
// 2. /vendor/etc/vintf/manifest.xml + device fragments
// 3. ODM manifest (optional) + odm fragments
// 4. /vendor/manifest.xml (legacy, no fragments)
// where:
// A + B means unioning <hal> tags from A and B. If B declares an override, then this takes priority
// over A.
status_t VintfObject::fetchDeviceHalManifest(HalManifest* out, std::string* error) {
    status_t vendorStatus = fetchOneHalManifest(kVendorManifest, out, error);
    if (vendorStatus != OK && vendorStatus != NAME_NOT_FOUND) {
        return vendorStatus;
    }

    if (vendorStatus == OK) {
        status_t fragmentStatus = addDirectoryManifests(kVendorManifestFragmentDir, out, error);
        if (fragmentStatus != OK) {
            return fragmentStatus;
        }
    }

    HalManifest odmManifest;
    status_t odmStatus = fetchOdmHalManifest(&odmManifest, error);
    if (odmStatus != OK && odmStatus != NAME_NOT_FOUND) {
        return odmStatus;
    }

    if (vendorStatus == OK) {
        if (odmStatus == OK) {
            if (!out->addAll(&odmManifest, error)) {
                if (error) {
                    error->insert(0, "Cannot add ODM manifest :");
                }
                return UNKNOWN_ERROR;
            }
        }
        return addDirectoryManifests(kOdmManifestFragmentDir, out, error);
    }

    // vendorStatus != OK, "out" is not changed.
    if (odmStatus == OK) {
        *out = std::move(odmManifest);
        return addDirectoryManifests(kOdmManifestFragmentDir, out, error);
    }

    // Use legacy /vendor/manifest.xml
    return out->fetchAllInformation(getFileSystem().get(), kVendorLegacyManifest, error);
}

// "out" is written to iff return status is OK.
// Priority:
// 1. if {sku} is defined, /odm/etc/vintf/manifest_{sku}.xml
// 2. /odm/etc/vintf/manifest.xml
// 3. if {sku} is defined, /odm/etc/manifest_{sku}.xml
// 4. /odm/etc/manifest.xml
// where:
// {sku} is the value of ro.boot.product.hardware.sku
status_t VintfObject::fetchOdmHalManifest(HalManifest* out, std::string* error) {
    status_t status;

    std::string productModel;
    productModel = getPropertyFetcher()->getProperty("ro.boot.product.hardware.sku", "");

    if (!productModel.empty()) {
        status =
            fetchOneHalManifest(kOdmVintfDir + "manifest_" + productModel + ".xml", out, error);
        if (status == OK || status != NAME_NOT_FOUND) {
            return status;
        }
    }

    status = fetchOneHalManifest(kOdmManifest, out, error);
    if (status == OK || status != NAME_NOT_FOUND) {
        return status;
    }

    if (!productModel.empty()) {
        status = fetchOneHalManifest(kOdmLegacyVintfDir + "manifest_" + productModel + ".xml", out,
                                     error);
        if (status == OK || status != NAME_NOT_FOUND) {
            return status;
        }
    }

    status = fetchOneHalManifest(kOdmLegacyManifest, out, error);
    if (status == OK || status != NAME_NOT_FOUND) {
        return status;
    }

    return NAME_NOT_FOUND;
}

// Fetch one manifest.xml file. "out" is written to iff return status is OK.
// Returns NAME_NOT_FOUND if file is missing.
status_t VintfObject::fetchOneHalManifest(const std::string& path, HalManifest* out,
                                          std::string* error) {
    HalManifest ret;
    status_t status = ret.fetchAllInformation(getFileSystem().get(), path, error);
    if (status == OK) {
        *out = std::move(ret);
    }
    return status;
}

status_t VintfObject::fetchDeviceMatrix(CompatibilityMatrix* out, std::string* error) {
    CompatibilityMatrix etcMatrix;
    if (etcMatrix.fetchAllInformation(getFileSystem().get(), kVendorMatrix, error) == OK) {
        *out = std::move(etcMatrix);
        return OK;
    }
    return out->fetchAllInformation(getFileSystem().get(), kVendorLegacyMatrix, error);
}

// Priority:
// 1. /system/etc/vintf/manifest.xml
//    + /system/etc/vintf/manifest/*.xml if they exist
//    + /product/etc/vintf/manifest.xml if it exists
//    + /product/etc/vintf/manifest/*.xml if they exist
// 2. (deprecated) /system/manifest.xml
status_t VintfObject::fetchFrameworkHalManifest(HalManifest* out, std::string* error) {
    auto systemEtcStatus = fetchOneHalManifest(kSystemManifest, out, error);
    if (systemEtcStatus == OK) {
        auto dirStatus = addDirectoryManifests(kSystemManifestFragmentDir, out, error);
        if (dirStatus != OK) {
            return dirStatus;
        }

        HalManifest productManifest;
        auto productStatus = fetchOneHalManifest(kProductManifest, &productManifest, error);
        if (productStatus != OK && productStatus != NAME_NOT_FOUND) {
            return productStatus;
        }
        if (productStatus == OK) {
            if (!out->addAll(&productManifest, error)) {
                if (error) {
                    error->insert(0, "Cannot add " + kProductManifest + ":");
                }
                return UNKNOWN_ERROR;
            }
        }

        return addDirectoryManifests(kProductManifestFragmentDir, out, error);
    } else {
        LOG(WARNING) << "Cannot fetch " << kSystemManifest << ": "
                     << (error ? *error : strerror(-systemEtcStatus));
    }

    return out->fetchAllInformation(getFileSystem().get(), kSystemLegacyManifest, error);
}

static void appendLine(std::string* error, const std::string& message) {
    if (error != nullptr) {
        if (!error->empty()) *error += "\n";
        *error += message;
    }
}

status_t VintfObject::getOneMatrix(const std::string& path, Named<CompatibilityMatrix>* out,
                                   std::string* error) {
    std::string content;
    status_t status = getFileSystem()->fetch(path, &content, error);
    if (status != OK) {
        return status;
    }
    if (!gCompatibilityMatrixConverter(&out->object, content, error)) {
        if (error) {
            error->insert(0, "Cannot parse " + path + ": ");
        }
        return BAD_VALUE;
    }
    out->name = path;
    return OK;
}

status_t VintfObject::getAllFrameworkMatrixLevels(std::vector<Named<CompatibilityMatrix>>* results,
                                                  std::string* error) {
    std::vector<std::string> fileNames;

    status_t listStatus = getFileSystem()->listFiles(kSystemVintfDir, &fileNames, error);
    if (listStatus != OK) {
        return listStatus;
    }
    for (const std::string& fileName : fileNames) {
        std::string path = kSystemVintfDir + fileName;
        Named<CompatibilityMatrix> namedMatrix;
        std::string matrixError;
        status_t matrixStatus = getOneMatrix(path, &namedMatrix, &matrixError);
        if (matrixStatus != OK) {
            // System manifests and matrices share the same dir. Client may not have enough
            // permissions to read system manifests, or may not be able to parse it.
            auto logLevel = matrixStatus == BAD_VALUE ? base::DEBUG : base::ERROR;
            LOG(logLevel) << "Framework Matrix: Ignore file " << path << ": " << matrixError;
            continue;
        }
        results->emplace_back(std::move(namedMatrix));
    }

    Named<CompatibilityMatrix> productMatrix;
    std::string productError;
    status_t productStatus = getOneMatrix(kProductMatrix, &productMatrix, &productError);
    if (productStatus == OK) {
        results->emplace_back(std::move(productMatrix));
    } else if (productStatus == NAME_NOT_FOUND) {
        LOG(DEBUG) << "Framework Matrix: missing " << kProductMatrix;
    } else {
        if (error) *error = std::move(productError);
        return productStatus;
    }

    if (results->empty()) {
        if (error) {
            *error =
                "No framework matrices under " + kSystemVintfDir + " can be fetched or parsed.\n";
        }
        return NAME_NOT_FOUND;
    }
    return OK;
}

std::shared_ptr<const RuntimeInfo> VintfObject::GetRuntimeInfo(bool skipCache,
                                                               RuntimeInfo::FetchFlags flags) {
    return GetInstance()->getRuntimeInfo(skipCache, flags);
}
std::shared_ptr<const RuntimeInfo> VintfObject::getRuntimeInfo(bool skipCache,
                                                               RuntimeInfo::FetchFlags flags) {
    std::unique_lock<std::mutex> _lock(mDeviceRuntimeInfo.mutex);

    if (!skipCache) {
        flags &= (~mDeviceRuntimeInfo.fetchedFlags);
    }

    if (mDeviceRuntimeInfo.object == nullptr) {
        mDeviceRuntimeInfo.object = getRuntimeInfoFactory()->make_shared();
    }

    status_t status = mDeviceRuntimeInfo.object->fetchAllInformation(flags);
    if (status != OK) {
        mDeviceRuntimeInfo.fetchedFlags &= (~flags);  // mark the fields as "not fetched"
        return nullptr;
    }

    mDeviceRuntimeInfo.fetchedFlags |= flags;
    return mDeviceRuntimeInfo.object;
}

namespace details {

enum class ParseStatus {
    OK,
    PARSE_ERROR,
    DUPLICATED_FWK_ENTRY,
    DUPLICATED_DEV_ENTRY,
};

static std::string toString(ParseStatus status) {
    switch(status) {
        case ParseStatus::OK:                   return "OK";
        case ParseStatus::PARSE_ERROR:          return "parse error";
        case ParseStatus::DUPLICATED_FWK_ENTRY: return "duplicated framework";
        case ParseStatus::DUPLICATED_DEV_ENTRY: return "duplicated device";
    }
    return "";
}

template <typename T>
static ParseStatus tryParse(const std::string& xml, const XmlConverter<T>& parse,
                            VintfObjectAfterUpdate* afterUpdate) {
    std::shared_ptr<T> ret = std::make_shared<T>();
    if (!parse(ret.get(), xml, nullptr /* error */)) {
        return ParseStatus::PARSE_ERROR;
    }
    if (!afterUpdate->set(ret)) {
        if (ret->type() == SchemaType::FRAMEWORK) {
            return ParseStatus::DUPLICATED_FWK_ENTRY;
        } else if (ret->type() == SchemaType::DEVICE) {
            return ParseStatus::DUPLICATED_DEV_ENTRY;
        }
        LOG(FATAL) << "unknown SchemaType: "
                   << static_cast<std::underlying_type_t<SchemaType>>(ret->type());
    }
    return ParseStatus::OK;
}

}  // namespace details

// Simulate applying xmls to VintfObject, then checkCompatibility as usual.
int32_t VintfObject::checkCompatibility(const std::vector<std::string>& xmls, std::string* error,
                                        CheckFlags::Type flags) {
    VintfObjectAfterUpdate afterUpdate(this);
    ParseStatus parseStatus = ParseStatus::OK;

    // parse all information from package
    for (const auto &xml : xmls) {
        parseStatus = tryParse(xml, gHalManifestConverter, &afterUpdate);
        if (parseStatus == ParseStatus::OK) {
            continue; // work on next one
        }
        if (parseStatus != ParseStatus::PARSE_ERROR) {
            appendLine(error, toString(parseStatus) + " manifest");
            return ALREADY_EXISTS;
        }
        parseStatus = tryParse(xml, gCompatibilityMatrixConverter, &afterUpdate);
        if (parseStatus == ParseStatus::OK) {
            continue; // work on next one
        }
        if (parseStatus != ParseStatus::PARSE_ERROR) {
            appendLine(error, toString(parseStatus) + " matrix");
            return ALREADY_EXISTS;
        }
        appendLine(error, toString(parseStatus));  // parse error
        return BAD_VALUE;
    }

    return afterUpdate.checkCompatibility(error, flags);
}

int32_t VintfObject::checkCompatibility(std::string* error, CheckFlags::Type flags) {
    status_t status = OK;
    // null checks for files and runtime info
    if (getFrameworkHalManifest() == nullptr) {
        appendLine(error, "No framework manifest file from device or from update package");
        status = NO_INIT;
    }
    if (getDeviceHalManifest() == nullptr) {
        appendLine(error, "No device manifest file from device or from update package");
        status = NO_INIT;
    }
    if (getFrameworkCompatibilityMatrix() == nullptr) {
        appendLine(error, "No framework matrix file from device or from update package");
        status = NO_INIT;
    }
    if (getDeviceCompatibilityMatrix() == nullptr) {
        appendLine(error, "No device matrix file from device or from update package");
        status = NO_INIT;
    }

    if (flags.isRuntimeInfoEnabled()) {
        if (getRuntimeInfo() == nullptr) {
            appendLine(error, "No runtime info from device");
            status = NO_INIT;
        }
    }
    if (status != OK) return status;

    // compatiblity check.
    if (!getDeviceHalManifest()->checkCompatibility(*getFrameworkCompatibilityMatrix(), error)) {
        if (error) {
            error->insert(0,
                          "Device manifest and framework compatibility matrix are incompatible: ");
        }
        return INCOMPATIBLE;
    }
    if (!getFrameworkHalManifest()->checkCompatibility(*getDeviceCompatibilityMatrix(), error)) {
        if (error) {
            error->insert(0,
                          "Framework manifest and device compatibility matrix are incompatible: ");
        }
        return INCOMPATIBLE;
    }

    CheckFlags::Type runtimeInfoCheckFlags = flags;
    if (!!getDeviceHalManifest()->kernel()) {
        // Use kernel from incoming OTA package, but not on the device.
        runtimeInfoCheckFlags = runtimeInfoCheckFlags.disableKernel();
    }

    if (flags.isRuntimeInfoEnabled()) {
        if (!getRuntimeInfo()->checkCompatibility(*getFrameworkCompatibilityMatrix(), error,
                                                  runtimeInfoCheckFlags)) {
            if (error) {
                error->insert(0,
                              "Runtime info and framework compatibility matrix are incompatible: ");
            }
            return INCOMPATIBLE;
        }
    }

    return COMPATIBLE;
}

namespace details {

const std::string kSystemVintfDir = "/system/etc/vintf/";
const std::string kVendorVintfDir = "/vendor/etc/vintf/";
const std::string kOdmVintfDir = "/odm/etc/vintf/";
const std::string kProductVintfDir = "/product/etc/vintf/";

const std::string kVendorManifest = kVendorVintfDir + "manifest.xml";
const std::string kSystemManifest = kSystemVintfDir + "manifest.xml";
const std::string kVendorMatrix = kVendorVintfDir + "compatibility_matrix.xml";
const std::string kOdmManifest = kOdmVintfDir + "manifest.xml";
const std::string kProductMatrix = kProductVintfDir + "compatibility_matrix.xml";
const std::string kProductManifest = kProductVintfDir + "manifest.xml";

const std::string kVendorManifestFragmentDir = kVendorVintfDir + "manifest/";
const std::string kSystemManifestFragmentDir = kSystemVintfDir + "manifest/";
const std::string kOdmManifestFragmentDir = kOdmVintfDir + "manifest/";
const std::string kProductManifestFragmentDir = kProductVintfDir + "manifest/";

const std::string kVendorLegacyManifest = "/vendor/manifest.xml";
const std::string kVendorLegacyMatrix = "/vendor/compatibility_matrix.xml";
const std::string kSystemLegacyManifest = "/system/manifest.xml";
const std::string kSystemLegacyMatrix = "/system/compatibility_matrix.xml";
const std::string kOdmLegacyVintfDir = "/odm/etc/";
const std::string kOdmLegacyManifest = kOdmLegacyVintfDir + "manifest.xml";

std::vector<std::string> dumpFileList() {
    return {
        // clang-format off
        kSystemVintfDir,
        kVendorVintfDir,
        kOdmVintfDir,
        kProductVintfDir,
        kOdmLegacyVintfDir,
        kVendorLegacyManifest,
        kVendorLegacyMatrix,
        kSystemLegacyManifest,
        kSystemLegacyMatrix,
        // clang-format on
    };
}

}  // namespace details

int32_t VintfObject::CheckCompatibility(const std::vector<std::string>& xmls, std::string* error,
                                        CheckFlags::Type flags) {
    return GetInstance()->checkCompatibility(xmls, error, flags);
}

bool VintfObject::IsHalDeprecated(const MatrixHal& oldMatrixHal,
                                  const CompatibilityMatrix& targetMatrix,
                                  const ListInstances& listInstances, std::string* error) {
    bool isDeprecated = false;
    oldMatrixHal.forEachInstance([&](const MatrixInstance& oldMatrixInstance) {
        if (IsInstanceDeprecated(oldMatrixInstance, targetMatrix, listInstances, error)) {
            isDeprecated = true;
        }
        return !isDeprecated;  // continue if no deprecated instance is found.
    });
    return isDeprecated;
}

// Let oldMatrixInstance = package@x.y-w::interface with instancePattern.
// If any "servedInstance" in listInstances(package@x.y::interface) matches instancePattern, return
// true iff:
// 1. package@x.?::interface/servedInstance is not in targetMatrix; OR
// 2. package@x.z::interface/servedInstance is in targetMatrix but
//    servedInstance is not in listInstances(package@x.z::interface)
bool VintfObject::IsInstanceDeprecated(const MatrixInstance& oldMatrixInstance,
                                       const CompatibilityMatrix& targetMatrix,
                                       const ListInstances& listInstances, std::string* error) {
    const std::string& package = oldMatrixInstance.package();
    const Version& version = oldMatrixInstance.versionRange().minVer();
    const std::string& interface = oldMatrixInstance.interface();

    std::vector<std::string> instanceHint;
    if (!oldMatrixInstance.isRegex()) {
        instanceHint.push_back(oldMatrixInstance.exactInstance());
    }

    auto list = listInstances(package, version, interface, instanceHint);
    for (const auto& pair : list) {
        const std::string& servedInstance = pair.first;
        Version servedVersion = pair.second;
        if (!oldMatrixInstance.matchInstance(servedInstance)) {
            continue;
        }

        // Find any package@x.? in target matrix, and check if instance is in target matrix.
        bool foundInstance = false;
        Version targetMatrixMinVer;
        targetMatrix.forEachInstanceOfPackage(package, [&](const auto& targetMatrixInstance) {
            if (targetMatrixInstance.versionRange().majorVer == version.majorVer &&
                targetMatrixInstance.interface() == interface &&
                targetMatrixInstance.matchInstance(servedInstance)) {
                targetMatrixMinVer = targetMatrixInstance.versionRange().minVer();
                foundInstance = true;
            }
            return !foundInstance;  // continue if not found
        });
        if (!foundInstance) {
            if (error) {
                *error = toFQNameString(package, servedVersion, interface, servedInstance) +
                         " is deprecated in compatibility matrix at FCM Version " +
                         to_string(targetMatrix.level()) + "; it should not be served.";
            }
            return true;
        }

        // Assuming that targetMatrix requires @x.u-v, require that at least @x.u is served.
        bool targetVersionServed = false;
        for (const auto& newPair :
             listInstances(package, targetMatrixMinVer, interface, instanceHint)) {
            if (newPair.first == servedInstance) {
                targetVersionServed = true;
                break;
            }
        }

        if (!targetVersionServed) {
            appendLine(error, toFQNameString(package, servedVersion, interface, servedInstance) +
                                  " is deprecated; requires at least " +
                                  to_string(targetMatrixMinVer));
            return true;
        }
    }

    return false;
}

int32_t VintfObject::CheckDeprecation(const ListInstances& listInstances, std::string* error) {
    return GetInstance()->checkDeprecation(listInstances, error);
}
int32_t VintfObject::checkDeprecation(const ListInstances& listInstances, std::string* error) {
    std::vector<Named<CompatibilityMatrix>> matrixFragments;
    auto matrixFragmentsStatus = getAllFrameworkMatrixLevels(&matrixFragments, error);
    if (matrixFragmentsStatus != OK) {
        return matrixFragmentsStatus;
    }
    if (matrixFragments.empty()) {
        if (error && error->empty()) {
            *error = "Cannot get framework matrix for each FCM version for unknown error.";
        }
        return NAME_NOT_FOUND;
    }
    auto deviceManifest = getDeviceHalManifest();
    if (deviceManifest == nullptr) {
        if (error) *error = "No device manifest.";
        return NAME_NOT_FOUND;
    }
    Level deviceLevel = deviceManifest->level();
    if (deviceLevel == Level::UNSPECIFIED) {
        if (error) *error = "Device manifest does not specify Shipping FCM Version.";
        return BAD_VALUE;
    }

    const CompatibilityMatrix* targetMatrix = nullptr;
    for (const auto& namedMatrix : matrixFragments) {
        if (namedMatrix.object.level() == deviceLevel) {
            targetMatrix = &namedMatrix.object;
        }
    }
    if (targetMatrix == nullptr) {
        if (error)
            *error = "Cannot find framework matrix at FCM version " + to_string(deviceLevel) + ".";
        return NAME_NOT_FOUND;
    }

    bool hasDeprecatedHals = false;
    for (const auto& namedMatrix : matrixFragments) {
        if (namedMatrix.object.level() == Level::UNSPECIFIED) continue;
        if (namedMatrix.object.level() >= deviceLevel) continue;

        const auto& oldMatrix = namedMatrix.object;
        for (const MatrixHal& hal : oldMatrix.getHals()) {
            hasDeprecatedHals |= IsHalDeprecated(hal, *targetMatrix, listInstances, error);
        }
    }

    return hasDeprecatedHals ? DEPRECATED : NO_DEPRECATED_HALS;
}

int32_t VintfObject::CheckDeprecation(std::string* error) {
    return GetInstance()->checkDeprecation(error);
}
int32_t VintfObject::checkDeprecation(std::string* error) {
    using namespace std::placeholders;
    auto deviceManifest = getDeviceHalManifest();
    ListInstances inManifest =
        [&deviceManifest](const std::string& package, Version version, const std::string& interface,
                          const std::vector<std::string>& /* hintInstances */) {
            std::vector<std::pair<std::string, Version>> ret;
            deviceManifest->forEachInstanceOfInterface(
                package, version, interface, [&ret](const ManifestInstance& manifestInstance) {
                    ret.push_back(
                        std::make_pair(manifestInstance.instance(), manifestInstance.version()));
                    return true;
                });
            return ret;
        };
    return checkDeprecation(inManifest, error);
}

const std::unique_ptr<FileSystem>& VintfObject::getFileSystem() {
    return mFileSystem;
}

const std::unique_ptr<PropertyFetcher>& VintfObject::getPropertyFetcher() {
    return mPropertyFetcher;
}

const std::unique_ptr<ObjectFactory<RuntimeInfo>>& VintfObject::getRuntimeInfoFactory() {
    return mRuntimeInfoFactory;
}

// make_unique does not work because VintfObject constructor is private.
VintfObject::Builder::Builder() : mObject(std::unique_ptr<VintfObject>(new VintfObject())) {}

VintfObject::Builder& VintfObject::Builder::setFileSystem(std::unique_ptr<FileSystem>&& e) {
    mObject->mFileSystem = std::move(e);
    return *this;
}

VintfObject::Builder& VintfObject::Builder::setRuntimeInfoFactory(
    std::unique_ptr<ObjectFactory<RuntimeInfo>>&& e) {
    mObject->mRuntimeInfoFactory = std::move(e);
    return *this;
}

VintfObject::Builder& VintfObject::Builder::setPropertyFetcher(
    std::unique_ptr<PropertyFetcher>&& e) {
    mObject->mPropertyFetcher = std::move(e);
    return *this;
}

std::unique_ptr<VintfObject> VintfObject::Builder::build() {
    if (!mObject->mFileSystem) mObject->mFileSystem = createDefaultFileSystem();
    if (!mObject->mRuntimeInfoFactory)
        mObject->mRuntimeInfoFactory = std::make_unique<ObjectFactory<RuntimeInfo>>();
    if (!mObject->mPropertyFetcher) mObject->mPropertyFetcher = createDefaultPropertyFetcher();
    return std::move(mObject);
}

} // namespace vintf
} // namespace android
