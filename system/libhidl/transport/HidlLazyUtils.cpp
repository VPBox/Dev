/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <hidl/HidlLazyUtils.h>
#include <hidl/HidlTransportSupport.h>

#include <android-base/logging.h>

#include <android/hidl/manager/1.2/IClientCallback.h>
#include <android/hidl/manager/1.2/IServiceManager.h>

namespace android {
namespace hardware {
namespace details {

using ::android::hidl::base::V1_0::IBase;

class ClientCounterCallback : public ::android::hidl::manager::V1_2::IClientCallback {
   public:
    ClientCounterCallback() : mNumConnectedServices(0) {}

    bool addRegisteredService(const sp<IBase>& service, const std::string& name);

   protected:
    Return<void> onClients(const sp<IBase>& service, bool clients) override;

   private:
    /**
     * Registers or re-registers services. Returns whether successful.
     */
    bool registerService(const sp<IBase>& service, const std::string& name);

    /**
     * Unregisters all services that we can. If we can't unregister all, re-register other
     * services.
     */
    void tryShutdown();

    /**
     * Counter of the number of services that currently have at least one client.
     */
    size_t mNumConnectedServices;

    struct Service {
        sp<IBase> service;
        std::string name;
    };
    /**
     * Number of services that have been registered.
     */
    std::vector<Service> mRegisteredServices;
};

class LazyServiceRegistrarImpl {
   public:
    LazyServiceRegistrarImpl() : mClientCallback(new ClientCounterCallback) {}

    status_t registerService(const sp<::android::hidl::base::V1_0::IBase>& service,
                             const std::string& name);

   private:
    sp<ClientCounterCallback> mClientCallback;
};

bool ClientCounterCallback::addRegisteredService(const sp<IBase>& service,
                                                 const std::string& name) {
    bool success = registerService(service, name);

    if (success) {
        mRegisteredServices.push_back({service, name});
    }

    return success;
}

bool ClientCounterCallback::registerService(const sp<IBase>& service, const std::string& name) {
    auto manager = hardware::defaultServiceManager1_2();

    const std::string descriptor = getDescriptor(service.get());

    LOG(INFO) << "Registering HAL: " << descriptor << " with name: " << name;

    status_t res = android::hardware::details::registerAsServiceInternal(service, name);
    if (res != android::OK) {
        LOG(ERROR) << "Failed to register as service.";
        return false;
    }

    bool ret = manager->registerClientCallback(getDescriptor(service.get()), name, service, this);
    if (!ret) {
        LOG(ERROR) << "Failed to add client callback.";
        return false;
    }

    return true;
}

/**
 * onClients is oneway, so no need to worry about multi-threading. Note that this means multiple
 * invocations could occur on different threads however.
 */
Return<void> ClientCounterCallback::onClients(const sp<::android::hidl::base::V1_0::IBase>& service,
                                              bool clients) {
    if (clients) {
        mNumConnectedServices++;
    } else {
        mNumConnectedServices--;
    }

    LOG(INFO) << "Process has " << mNumConnectedServices << " (of " << mRegisteredServices.size()
              << " available) client(s) in use after notification " << getDescriptor(service.get())
              << " has clients: " << clients;

    if (mNumConnectedServices == 0) {
        tryShutdown();
    }

    return Status::ok();
}

void ClientCounterCallback::tryShutdown() {
    LOG(INFO) << "Trying to exit HAL. No clients in use for any service in process.";

    auto manager = hardware::defaultServiceManager1_2();

    auto unRegisterIt = mRegisteredServices.begin();
    for (; unRegisterIt != mRegisteredServices.end(); ++unRegisterIt) {
        auto& entry = (*unRegisterIt);

        const std::string descriptor = getDescriptor(entry.service.get());
        bool success = manager->tryUnregister(descriptor, entry.name, entry.service);

        if (!success) {
            LOG(INFO) << "Failed to unregister HAL " << descriptor << "/" << entry.name;
            break;
        }
    }

    if (unRegisterIt == mRegisteredServices.end()) {
        LOG(INFO) << "Unregistered all clients and exiting";
        exit(EXIT_SUCCESS);
    }

    for (auto reRegisterIt = mRegisteredServices.begin(); reRegisterIt != unRegisterIt;
         reRegisterIt++) {
        auto& entry = (*reRegisterIt);

        // re-register entry
        if (!registerService(entry.service, entry.name)) {
            // Must restart. Otherwise, clients will never be able to get ahold of this service.
            LOG(FATAL) << "Bad state: could not re-register " << getDescriptor(entry.service.get());
        }
    }
}

status_t LazyServiceRegistrarImpl::registerService(
    const sp<::android::hidl::base::V1_0::IBase>& service, const std::string& name) {
    if (!mClientCallback->addRegisteredService(service, name)) {
        return ::android::UNKNOWN_ERROR;
    }

    return ::android::OK;
}

}  // namespace details

LazyServiceRegistrar::LazyServiceRegistrar() {
    mImpl = std::make_shared<details::LazyServiceRegistrarImpl>();
}

status_t LazyServiceRegistrar::registerService(
    const sp<::android::hidl::base::V1_0::IBase>& service, const std::string& name) {
    return mImpl->registerService(service, name);
}

}  // namespace hardware
}  // namespace android
