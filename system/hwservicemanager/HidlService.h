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

#ifndef ANDROID_HARDWARE_MANAGER_HIDLSERVICE_H
#define ANDROID_HARDWARE_MANAGER_HIDLSERVICE_H

#include <set>

#include <android/hidl/manager/1.2/IServiceManager.h>
#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

namespace android {
namespace hidl {
namespace manager {
namespace implementation {

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hidl::base::V1_0::IBase;
using ::android::hidl::manager::V1_0::IServiceNotification;
using ::android::hidl::manager::V1_1::IServiceManager;
using ::android::hidl::manager::V1_2::IClientCallback;
using ::android::sp;

struct HidlService {
    HidlService(const std::string &interfaceName,
                const std::string &instanceName,
                const sp<IBase> &service,
                const pid_t pid);
    HidlService(const std::string &interfaceName,
                const std::string &instanceName)
    : HidlService(
        interfaceName,
        instanceName,
        nullptr,
        static_cast<pid_t>(IServiceManager::PidConstant::NO_PID))
    {}
    virtual ~HidlService() {}

    /**
     * Note, getService() can be nullptr. This is because you can have a HidlService
     * with registered IServiceNotification objects but no service registered yet.
     */
    sp<IBase> getService() const;
    void setService(sp<IBase> service, pid_t pid);
    pid_t getDebugPid() const;
    const std::string &getInterfaceName() const;
    const std::string &getInstanceName() const;

    void addListener(const sp<IServiceNotification> &listener);
    bool removeListener(const wp<IBase> &listener);
    void registerPassthroughClient(pid_t pid);

    // also sends onClients(true) if we have clients
    void addClientCallback(const sp<IClientCallback>& callback);
    bool removeClientCallback(const sp<IClientCallback>& callback);

    // return is number of clients (-1 means this is not implemented or we didn't check)
    // count includes one held by hwservicemanager
    ssize_t handleClientCallbacks(bool isCalledOnInterval);

    // Updates client callbacks (even if mClientCallbacks is emtpy)
    // see handleClientCallbacks
    ssize_t forceHandleClientCallbacks(bool isCalledOnInterval);

    // when giving out a handle to a client, but the kernel might not know this yet
    void guaranteeClient();

    std::string string() const; // e.x. "android.hidl.manager@1.0::IServiceManager/manager"
    const std::set<pid_t> &getPassthroughClients() const;

protected:
    // mockable number of clients including hwservicemanager. -1 if not implemented or unavailable.
    virtual ssize_t getNodeStrongRefCount();

private:
    void sendRegistrationNotifications();

    // Also updates mHasClients (of what the last callback was)
    void sendClientCallbackNotifications(bool hasClients);

    // Only sends notification
    void sendClientCallbackNotification(const sp<IClientCallback>& callback, bool hasClients);

    const std::string                     mInterfaceName; // e.x. "android.hidl.manager@1.0::IServiceManager"
    const std::string                     mInstanceName;  // e.x. "manager"
    sp<IBase>                             mService;

    std::vector<sp<IServiceNotification>> mListeners{};
    std::set<pid_t>                       mPassthroughClients{};
    pid_t                                 mPid = static_cast<pid_t>(IServiceManager::PidConstant::NO_PID);

    std::vector<sp<IClientCallback>>      mClientCallbacks{};
    bool                                  mHasClients = false; // notifications sent on true -> false.
    bool                                  mGuaranteeClient = false; // whenever a client is handed out
    size_t                                mNoClientsCounter = 0;
};

}  // namespace implementation
}  // namespace manager
}  // namespace hidl
}  // namespace android

#endif // ANDROID_HARDWARE_MANAGER_HIDLSERVICE_H
