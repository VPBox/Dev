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

#ifndef ANDROID_VINTF_VINTF_OBJECT_H_
#define ANDROID_VINTF_VINTF_OBJECT_H_

#include <memory>

#include "CheckFlags.h"
#include "CompatibilityMatrix.h"
#include "FileSystem.h"
#include "HalManifest.h"
#include "Named.h"
#include "ObjectFactory.h"
#include "PropertyFetcher.h"
#include "RuntimeInfo.h"

namespace android {
namespace vintf {

namespace details {
class VintfObjectAfterUpdate;

template <typename T>
struct LockedSharedPtr {
    std::shared_ptr<T> object;
    std::mutex mutex;
    bool fetchedOnce = false;
};

struct LockedRuntimeInfoCache {
    std::shared_ptr<RuntimeInfo> object;
    std::mutex mutex;
    RuntimeInfo::FetchFlags fetchedFlags = RuntimeInfo::FetchFlag::NONE;
};
}  // namespace details

namespace testing {
class VintfObjectTestBase;
class VintfObjectRuntimeInfoTest;
class VintfObjectCompatibleTest;
}  // namespace testing

/*
 * The top level class for libvintf.
 * An overall diagram of the public API:
 * VintfObject
 *   + GetDeviceHalManfiest
 *   |   + getTransport
 *   |   + checkCompatibility
 *   + GetFrameworkHalManifest
 *   |   + getTransport
 *   |   + checkCompatibility
 *   + GetRuntimeInfo
 *       + checkCompatibility
 *
 * Each of the function gathers all information and encapsulate it into the object.
 * If no error, it return the same singleton object in the future, and the HAL manifest
 * file won't be touched again.
 * If any error, nullptr is returned, and Get will try to parse the HAL manifest
 * again when it is called again.
 * All these operations are thread-safe.
 * If skipCache, always skip the cache in memory and read the files / get runtime information
 * again from the device.
 */
class VintfObject {
   public:
    virtual ~VintfObject() = default;

    /*
     * Return the API that access the device-side HAL manifests built from component pieces on the
     * vendor partition.
     */
    virtual std::shared_ptr<const HalManifest> getDeviceHalManifest(bool skipCache = false);

    /*
     * Return the API that access the framework-side HAL manifest built from component pieces on the
     * system partition.
     */
    virtual std::shared_ptr<const HalManifest> getFrameworkHalManifest(bool skipCache = false);

    /*
     * Return the API that access the device-side compatibility matrix built from component pieces
     * on the vendor partition.
     */
    virtual std::shared_ptr<const CompatibilityMatrix> getDeviceCompatibilityMatrix(
        bool skipCache = false);

    /*
     * Return the API that access the framework-side compatibility matrix built from component
     * pieces on the system partition.
     *
     * This automatically selects the right compatibility matrix according to the target-level
     * specified by the device.
     */
    virtual std::shared_ptr<const CompatibilityMatrix> getFrameworkCompatibilityMatrix(
        bool skipCache = false);

    /*
     * Return the API that access device runtime info.
     *
     * {skipCache == true, flags == ALL}: re-fetch everything
     * {skipCache == false, flags == ALL}: fetch everything if not previously fetched
     * {skipCache == true, flags == selected info}: re-fetch selected information
     *                                if not previously fetched.
     * {skipCache == false, flags == selected info}: fetch selected information
     *                                if not previously fetched.
     *
     * @param skipCache do not fetch if previously fetched
     * @param flags bitwise-or of RuntimeInfo::FetchFlag
     */
    std::shared_ptr<const RuntimeInfo> getRuntimeInfo(
        bool skipCache = false, RuntimeInfo::FetchFlags flags = RuntimeInfo::FetchFlag::ALL);

    /**
     * Check compatibility, given a set of manifests / matrices in packageInfo.
     * They will be checked against the manifests / matrices on the device.
     *
     * @param error error message
     * @param flags flags to disable certain checks. See CheckFlags.
     *
     * @return = 0 if success (compatible)
     *         > 0 if incompatible
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    int32_t checkCompatibility(std::string* error = nullptr,
                               CheckFlags::Type flags = CheckFlags::DEFAULT);

    /**
     * A std::function that abstracts a list of "provided" instance names. Given package, version
     * and interface, the function returns a list of instance names that matches.
     * This function can represent a manifest, an IServiceManager, etc.
     * If the source is passthrough service manager, a list of instance names cannot be provided.
     * Instead, the function should call getService on each of the "hintInstances", and
     * return those instances for which getService does not return a nullptr. This means that for
     * passthrough HALs, the deprecation on <regex-instance>s cannot be enforced; only <instance>s
     * can be enforced.
     */
    using ListInstances = std::function<std::vector<std::pair<std::string, Version>>(
        const std::string& package, Version version, const std::string& interface,
        const std::vector<std::string>& hintInstances)>;
    /**
     * Check deprecation on framework matrices with a provided predicate.
     *
     * @param listInstances predicate that takes parameter in this format:
     *        android.hardware.foo@1.0::IFoo
     *        and returns {{"default", version}...} if HAL is in use, where version =
     *        first version in interfaceChain where package + major version matches.
     *
     * @return = 0 if success (no deprecated HALs)
     *         > 0 if there is at least one deprecated HAL
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    int32_t checkDeprecation(const ListInstances& listInstances, std::string* error = nullptr);

    /**
     * Check deprecation on existing VINTF metadata. Use Device Manifest as the
     * predicate to check if a HAL is in use.
     *
     * @return = 0 if success (no deprecated HALs)
     *         > 0 if there is at least one deprecated HAL
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    int32_t checkDeprecation(std::string* error = nullptr);

   private:
    std::unique_ptr<FileSystem> mFileSystem;
    std::unique_ptr<ObjectFactory<RuntimeInfo>> mRuntimeInfoFactory;
    std::unique_ptr<PropertyFetcher> mPropertyFetcher;

    details::LockedSharedPtr<HalManifest> mDeviceManifest;
    details::LockedSharedPtr<HalManifest> mFrameworkManifest;
    details::LockedSharedPtr<CompatibilityMatrix> mDeviceMatrix;

    // Parent lock of the following fields. It should be acquired before locking the child locks.
    std::mutex mFrameworkCompatibilityMatrixMutex;
    details::LockedSharedPtr<CompatibilityMatrix> mFrameworkMatrix;
    details::LockedSharedPtr<CompatibilityMatrix> mCombinedFrameworkMatrix;
    // End of mFrameworkCompatibilityMatrixMutex

    details::LockedRuntimeInfoCache mDeviceRuntimeInfo;

    // Expose functions for testing and recovery
    friend class VintfObjectRecovery;
    friend class testing::VintfObjectTestBase;
    friend class testing::VintfObjectRuntimeInfoTest;
    friend class testing::VintfObjectCompatibleTest;

    // Expose functions to simulate dependency injection.
    friend class details::VintfObjectAfterUpdate;

   protected:
    virtual const std::unique_ptr<FileSystem>& getFileSystem();
    virtual const std::unique_ptr<PropertyFetcher>& getPropertyFetcher();
    virtual const std::unique_ptr<ObjectFactory<RuntimeInfo>>& getRuntimeInfoFactory();

   public:
    /*
     * Get global instance. By default, this fetches from root and cache results,
     * unless skipCache is specified.
     */
    static std::shared_ptr<VintfObject> GetInstance();

    // Static variants of member functions.

    /*
     * Return the API that access the device-side HAL manifest built from component pieces on the
     * vendor partition.
     */
    static std::shared_ptr<const HalManifest> GetDeviceHalManifest(bool skipCache = false);

    /*
     * Return the API that access the framework-side HAL manifest built from component pieces on the
     * system partition.
     */
    static std::shared_ptr<const HalManifest> GetFrameworkHalManifest(bool skipCache = false);

    /*
     * Return the API that access the device-side compatibility matrix built from component pieces
     * on the vendor partition.
     */
    static std::shared_ptr<const CompatibilityMatrix> GetDeviceCompatibilityMatrix(
        bool skipCache = false);

    /*
     * Return the API that access the framework-side compatibility matrix built from component
     * pieces on the system partition.
     */
    static std::shared_ptr<const CompatibilityMatrix> GetFrameworkCompatibilityMatrix(
        bool skipCache = false);

    /*
     * Return the API that access device runtime info.
     *
     * {skipCache == true, flags == ALL}: re-fetch everything
     * {skipCache == false, flags == ALL}: fetch everything if not previously fetched
     * {skipCache == true, flags == selected info}: re-fetch selected information
     *                                if not previously fetched.
     * {skipCache == false, flags == selected info}: fetch selected information
     *                                if not previously fetched.
     *
     * @param skipCache do not fetch if previously fetched
     * @param flags bitwise-or of RuntimeInfo::FetchFlag
     */
    static std::shared_ptr<const RuntimeInfo> GetRuntimeInfo(
        bool skipCache = false, RuntimeInfo::FetchFlags flags = RuntimeInfo::FetchFlag::ALL);

    /**
     * Check compatibility, given a set of manifests / matrices in packageInfo.
     * They will be checked against the manifests / matrices on the device.
     *
     * @param packageInfo a list of XMLs of HalManifest /
     * CompatibilityMatrix objects.
     * @param error error message
     * @param flags flags to disable certain checks. See CheckFlags.
     *
     * @return = 0 if success (compatible)
     *         > 0 if incompatible
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    static int32_t CheckCompatibility(const std::vector<std::string>& packageInfo,
                                      std::string* error = nullptr,
                                      CheckFlags::Type flags = CheckFlags::DEFAULT);

    /**
     * Check deprecation on framework matrices with a provided predicate.
     *
     * @param listInstances predicate that takes parameter in this format:
     *        android.hardware.foo@1.0::IFoo
     *        and returns {{"default", version}...} if HAL is in use, where version =
     *        first version in interfaceChain where package + major version matches.
     *
     * @return = 0 if success (no deprecated HALs)
     *         > 0 if there is at least one deprecated HAL
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    static int32_t CheckDeprecation(const ListInstances& listInstances,
                                    std::string* error = nullptr);

    /**
     * Check deprecation on existing VINTF metadata. Use Device Manifest as the
     * predicate to check if a HAL is in use.
     *
     * @return = 0 if success (no deprecated HALs)
     *         > 0 if there is at least one deprecated HAL
     *         < 0 if any error (mount partition fails, illformed XML, etc.)
     */
    static int32_t CheckDeprecation(std::string* error = nullptr);

   private:
    static details::LockedSharedPtr<VintfObject> sInstance;

    status_t getCombinedFrameworkMatrix(const std::shared_ptr<const HalManifest>& deviceManifest,
                                        CompatibilityMatrix* out, std::string* error = nullptr);
    status_t getAllFrameworkMatrixLevels(std::vector<Named<CompatibilityMatrix>>* out,
                                         std::string* error = nullptr);
    status_t getOneMatrix(const std::string& path, Named<CompatibilityMatrix>* out,
                          std::string* error = nullptr);
    status_t addDirectoryManifests(const std::string& directory, HalManifest* manifests,
                                   std::string* error = nullptr);
    status_t fetchDeviceHalManifest(HalManifest* out, std::string* error = nullptr);
    status_t fetchDeviceMatrix(CompatibilityMatrix* out, std::string* error = nullptr);
    status_t fetchOdmHalManifest(HalManifest* out, std::string* error = nullptr);
    status_t fetchOneHalManifest(const std::string& path, HalManifest* out,
                                 std::string* error = nullptr);
    status_t fetchFrameworkHalManifest(HalManifest* out, std::string* error = nullptr);
    // Helper to CheckCompatibility with dependency injection.
    int32_t checkCompatibility(const std::vector<std::string>& packageInfo,
                               std::string* error = nullptr,
                               CheckFlags::Type flags = CheckFlags::DEFAULT);

    static bool IsHalDeprecated(const MatrixHal& oldMatrixHal,
                                const CompatibilityMatrix& targetMatrix,
                                const ListInstances& listInstances, std::string* error);
    static bool IsInstanceDeprecated(const MatrixInstance& oldMatrixInstance,
                                     const CompatibilityMatrix& targetMatrix,
                                     const ListInstances& listInstances, std::string* error);

   public:
    /**
     * Builder of VintfObject. If a dependency is not specified, the default behavior is used.
     * - FileSystem fetch from "/" for target and fetch no files for host
     * - ObjectFactory<RuntimeInfo> fetches default RuntimeInfo for target and nothing for host
     * - PropertyFetcher fetches properties for target and nothing for host
     */
    class Builder {
       public:
        Builder();
        Builder& setFileSystem(std::unique_ptr<FileSystem>&&);
        Builder& setRuntimeInfoFactory(std::unique_ptr<ObjectFactory<RuntimeInfo>>&&);
        Builder& setPropertyFetcher(std::unique_ptr<PropertyFetcher>&&);
        std::unique_ptr<VintfObject> build();

       private:
        std::unique_ptr<VintfObject> mObject;
    };

   private:
    /* Empty VintfObject without any dependencies. Used by Builder. */
    VintfObject() = default;
};

enum : int32_t {
    COMPATIBLE = 0,
    INCOMPATIBLE = 1,

    NO_DEPRECATED_HALS = 0,
    DEPRECATED = 1,
};

// exposed for testing.
namespace details {

extern const std::string kSystemVintfDir;
extern const std::string kVendorVintfDir;
extern const std::string kOdmVintfDir;
extern const std::string kProductVintfDir;
extern const std::string kOdmLegacyVintfDir;
extern const std::string kOdmLegacyManifest;
extern const std::string kVendorManifest;
extern const std::string kSystemManifest;
extern const std::string kVendorMatrix;
extern const std::string kOdmManifest;
extern const std::string kProductMatrix;
extern const std::string kProductManifest;
extern const std::string kVendorManifestFragmentDir;
extern const std::string kSystemManifestFragmentDir;
extern const std::string kOdmManifestFragmentDir;
extern const std::string kProductManifestFragmentDir;
extern const std::string kVendorLegacyManifest;
extern const std::string kVendorLegacyMatrix;
extern const std::string kSystemLegacyManifest;
extern const std::string kSystemLegacyMatrix;

// Convenience function to dump all files and directories that could be read
// by calling Get(Framework|Device)(HalManifest|CompatibilityMatrix). The list
// include files that may not actually be read when the four functions are called
// because some files have a higher priority than others. The list does NOT
// include "files" (including kernel interfaces) that are read when GetRuntimeInfo
// is called.
std::vector<std::string> dumpFileList();

} // namespace details

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_VINTF_OBJECT_H_
