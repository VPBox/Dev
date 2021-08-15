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

#define LOG_TAG "hwservicemanager"
#include "HidlService.h"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <hwbinder/BpHwBinder.h>
#include <sstream>

using ::android::hardware::interfacesEqual;

namespace android {
namespace hidl {
namespace manager {
namespace implementation {

static constexpr int kNoClientRepeatLimit = 2;

HidlService::HidlService(
    const std::string &interfaceName,
    const std::string &instanceName,
    const sp<IBase> &service,
    pid_t pid)
: mInterfaceName(interfaceName),
  mInstanceName(instanceName),
  mService(service),
  mPid(pid)
{}

sp<IBase> HidlService::getService() const {
    return mService;
}
void HidlService::setService(sp<IBase> service, pid_t pid) {
    mService = service;
    mPid = pid;

    mClientCallbacks.clear();
    mHasClients = false;
    mGuaranteeClient = false;
    mNoClientsCounter = 0;

    sendRegistrationNotifications();
}

pid_t HidlService::getDebugPid() const {
    return mPid;
}
const std::string &HidlService::getInterfaceName() const {
    return mInterfaceName;
}
const std::string &HidlService::getInstanceName() const {
    return mInstanceName;
}

void HidlService::addListener(const sp<IServiceNotification> &listener) {
    if (mService != nullptr) {
        auto ret = listener->onRegistration(
            mInterfaceName, mInstanceName, true /* preexisting */);
        if (!ret.isOk()) {
            LOG(ERROR) << "Not adding listener for " << mInterfaceName << "/"
                       << mInstanceName << ": transport error when sending "
                       << "notification for already registered instance.";
            return;
        }
    }
    mListeners.push_back(listener);
}

bool HidlService::removeListener(const wp<IBase>& listener) {
    bool found = false;

    for (auto it = mListeners.begin(); it != mListeners.end();) {
        if (interfacesEqual(*it, listener.promote())) {
            it = mListeners.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    return found;
}

void HidlService::registerPassthroughClient(pid_t pid) {
    mPassthroughClients.insert(pid);
}

const std::set<pid_t> &HidlService::getPassthroughClients() const {
    return mPassthroughClients;
}

void HidlService::addClientCallback(const sp<IClientCallback>& callback) {
    if (mHasClients) {
        // we have this kernel feature, so make sure we're in an updated state
        forceHandleClientCallbacks(false /*onInterval*/);
    }

    if (mHasClients) {
        // make sure this callback is in the same state as all of the rest
        sendClientCallbackNotification(callback, true /*hasClients*/);
    }

    mClientCallbacks.push_back(callback);
}

bool HidlService::removeClientCallback(const sp<IClientCallback>& callback) {
    bool found = false;

    for (auto it = mClientCallbacks.begin(); it != mClientCallbacks.end();) {
        if (interfacesEqual(*it, callback)) {
            it = mClientCallbacks.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    return found;
}

ssize_t HidlService::handleClientCallbacks(bool isCalledOnInterval) {
    if (!mClientCallbacks.empty()) {
        return forceHandleClientCallbacks(isCalledOnInterval);
    }

    return -1;
}

ssize_t HidlService::forceHandleClientCallbacks(bool isCalledOnInterval) {
    ssize_t count = getNodeStrongRefCount();

    // binder driver doesn't support this feature
    if (count == -1) return count;

    bool hasClients = count > 1; // this process holds a strong count

    if (mGuaranteeClient) {
        // we have no record of this client
        if (!mHasClients && !hasClients) {
            sendClientCallbackNotifications(true);
        }

        // guarantee is temporary
        mGuaranteeClient = false;
    }

    if (hasClients && !mHasClients) {
        // client was retrieved in some other way
        sendClientCallbackNotifications(true);
    }

    // there are no more clients, but the callback has not been called yet
    if (!hasClients && mHasClients && isCalledOnInterval) {
        mNoClientsCounter++;

        if (mNoClientsCounter >= kNoClientRepeatLimit) {
            sendClientCallbackNotifications(false);
        }
    }

    return count;
}

void HidlService::guaranteeClient() {
    mGuaranteeClient = true;
}

std::string HidlService::string() const {
    std::stringstream ss;
    ss << mInterfaceName << "/" << mInstanceName;
    return ss.str();
}

ssize_t HidlService::getNodeStrongRefCount() {
    using ::android::hardware::toBinder;
    using ::android::hardware::BpHwBinder;
    using ::android::hardware::IBinder;

    if (mService == nullptr) return -1;

    // this justifies the bp cast below, no in-process HALs need this
    if (!mService->isRemote()) return -1;

    sp<IBinder> binder = toBinder(mService);
    if (binder == nullptr) return -1;

    sp<BpHwBinder> bpBinder = static_cast<BpHwBinder*>(binder.get());
    return bpBinder->getNodeStrongRefCount();
}

void HidlService::sendRegistrationNotifications() {
    if (mListeners.size() == 0 || mService == nullptr) {
        return;
    }

    hidl_string iface = mInterfaceName;
    hidl_string name = mInstanceName;

    for (auto it = mListeners.begin(); it != mListeners.end();) {
        auto ret = (*it)->onRegistration(iface, name, false /* preexisting */);
        if (ret.isOk()) {
            ++it;
        } else {
            LOG(ERROR) << "Dropping registration callback for " << iface << "/" << name
                       << ": transport error.";
            it = mListeners.erase(it);
        }
    }
}

void HidlService::sendClientCallbackNotifications(bool hasClients) {
    CHECK(hasClients != mHasClients) << "Record shows: " << mHasClients
        << " so we can't tell clients again that we have client: " << hasClients;

    LOG(INFO) << "Notifying " << string() << " they have clients: " << hasClients;

    for (const auto& cb : mClientCallbacks) {
        sendClientCallbackNotification(cb, hasClients);
    }

    mNoClientsCounter = 0;
    mHasClients = hasClients;
}

void HidlService::sendClientCallbackNotification(const sp<IClientCallback>& callback, bool hasClients) {
    Return<void> ret = callback->onClients(getService(), hasClients);
    if (!ret.isOk()) {
        LOG(WARNING) << "onClients callback failed for " << string() << ": " << ret.description();
    }
}


}  // namespace implementation
}  // namespace manager
}  // namespace hidl
}  // namespace android
