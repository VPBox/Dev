/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANDROID_HARDWARE_MANAGER_SERVICEMANAGER_H
#define ANDROID_HARDWARE_MANAGER_SERVICEMANAGER_H

#include <android/hidl/manager/1.2/IServiceManager.h>
#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>
#include <map>

#include "AccessControl.h"
#include "HidlService.h"

namespace android {
namespace hidl {
namespace manager {
namespace implementation {

using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hidl::base::V1_0::IBase;
using ::android::hidl::manager::V1_0::IServiceNotification;
using ::android::hidl::manager::V1_2::IClientCallback;
using ::android::sp;
using ::android::wp;

struct ServiceManager : public V1_2::IServiceManager, hidl_death_recipient {
    // Methods from ::android::hidl::manager::V1_0::IServiceManager follow.
    Return<sp<IBase>> get(const hidl_string& fqName,
                          const hidl_string& name) override;
    Return<bool> add(const hidl_string& name,
                     const sp<IBase>& service) override;

    Return<Transport> getTransport(const hidl_string& fqName,
                                   const hidl_string& name);

    Return<void> list(list_cb _hidl_cb) override;
    Return<void> listByInterface(const hidl_string& fqInstanceName,
                                 listByInterface_cb _hidl_cb) override;

    Return<bool> registerForNotifications(const hidl_string& fqName,
                                          const hidl_string& name,
                                          const sp<IServiceNotification>& callback) override;

    Return<void> debugDump(debugDump_cb _cb) override;
    Return<void> registerPassthroughClient(const hidl_string &fqName,
            const hidl_string &name) override;

    // Methods from ::android::hidl::manager::V1_1::IServiceManager follow.
    Return<bool> unregisterForNotifications(const hidl_string& fqName,
                                            const hidl_string& name,
                                            const sp<IServiceNotification>& callback) override;

    // Methods from ::android::hidl::manager::V1_2::IServiceManager follow.
    Return<bool> registerClientCallback(const hidl_string& fqName,
                                        const hidl_string& name,
                                        const sp<IBase>& server,
                                        const sp<IClientCallback>& cb) override;
    Return<bool> unregisterClientCallback(const sp<IBase>& server,
                                          const sp<IClientCallback>& cb) override;
    Return<bool> addWithChain(const hidl_string& name,
                              const sp<IBase>& service,
                              const hidl_vec<hidl_string>& chain) override;
    Return<void> listManifestByInterface(const hidl_string& fqInstanceName,
                                         listManifestByInterface_cb _hidl_cb) override;
    Return<bool> tryUnregister(const hidl_string& fqName,
                               const hidl_string& name,
                               const sp<IBase>& service) override;

    void handleClientCallbacks();

    virtual void serviceDied(uint64_t cookie, const wp<IBase>& who);
private:
    bool addImpl(const std::string& name,
                 const sp<IBase>& service,
                 const hidl_vec<hidl_string>& interfaceChain,
                 const AccessControl::CallingContext& callingContext);

    // if restrictToInstanceName is nullptr, remove all, otherwise only those services
    // which match this instance name. Returns whether all instances were removed.
    bool removeService(const wp<IBase>& who, const std::string* restrictToInstanceName);
    bool removePackageListener(const wp<IBase>& who);
    bool removeServiceListener(const wp<IBase>& who);
    size_t countExistingService() const;

    // true = continue, false = break
    void forEachExistingService(std::function<bool(const HidlService *)> f) const;
    void forEachExistingService(std::function<bool(HidlService *)> f);
    void forEachServiceEntry(std::function<bool(const HidlService *)> f) const;
    void forEachServiceEntry(std::function<bool(HidlService *)> f);

    HidlService* lookup(const std::string& fqName, const std::string& name);

    using InstanceMap = std::map<
            std::string, // instance name e.x. "manager"
            std::unique_ptr<HidlService>
        >;

    struct PackageInterfaceMap {
        InstanceMap &getInstanceMap();
        const InstanceMap &getInstanceMap() const;

        /**
         * Finds a HidlService with the desired name. If none,
         * returns nullptr. HidlService::getService() might also be nullptr
         * if there are registered IServiceNotification objects for it. Return
         * value should be treated as a temporary reference.
         */
        HidlService *lookup(
            const std::string &name);
        const HidlService *lookup(
            const std::string &name) const;

        void insertService(std::unique_ptr<HidlService> &&service);

        void addPackageListener(sp<IServiceNotification> listener);
        bool removePackageListener(const wp<IBase>& who);
        bool removeServiceListener(const wp<IBase>& who);

        void sendPackageRegistrationNotification(
            const hidl_string &fqName,
            const hidl_string &instanceName);

    private:
        InstanceMap mInstanceMap{};

        std::vector<sp<IServiceNotification>> mPackageListeners{};
    };

    AccessControl mAcl;

    /**
     * Access to this map doesn't need to be locked, since hwservicemanager
     * is single-threaded.
     *
     * e.x.
     * mServiceMap["android.hidl.manager@1.0::IServiceManager"]["manager"]
     *     -> HidlService object
     */
    std::map<
        std::string, // package::interface e.x. "android.hidl.manager@1.0::IServiceManager"
        PackageInterfaceMap
    > mServiceMap;
};

}  // namespace implementation
}  // namespace manager
}  // namespace hidl
}  // namespace android

#endif  // ANDROID_HARDWARE_MANAGER_SERVICEMANAGER_H
