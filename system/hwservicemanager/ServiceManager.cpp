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

#include "ServiceManager.h"
#include "Vintf.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <hwbinder/IPCThreadState.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <regex>
#include <sstream>
#include <thread>

using android::hardware::IPCThreadState;
using ::android::hardware::interfacesEqual;

namespace android {
namespace hidl {
namespace manager {
namespace implementation {

AccessControl::CallingContext getBinderCallingContext() {
    const auto& self = IPCThreadState::self();

    pid_t pid = self->getCallingPid();
    const char* sid = self->getCallingSid();

    if (sid == nullptr) {
        if (pid != getpid()) {
            android_errorWriteLog(0x534e4554, "121035042");
        }

        return AccessControl::getCallingContext(pid);
    } else {
        return { true, sid, pid };
    }
}

static constexpr uint64_t kServiceDiedCookie = 0;
static constexpr uint64_t kPackageListenerDiedCookie = 1;
static constexpr uint64_t kServiceListenerDiedCookie = 2;
static constexpr uint64_t kClientCallbackDiedCookie = 3;

size_t ServiceManager::countExistingService() const {
    size_t total = 0;
    forEachExistingService([&] (const HidlService *) {
        ++total;
        return true;  // continue
    });
    return total;
}

void ServiceManager::forEachExistingService(std::function<bool(const HidlService *)> f) const {
    forEachServiceEntry([&] (const HidlService *service) {
        if (service->getService() == nullptr) {
            return true;  // continue
        }
        return f(service);
    });
}

void ServiceManager::forEachExistingService(std::function<bool(HidlService *)> f) {
    forEachServiceEntry([&] (HidlService *service) {
        if (service->getService() == nullptr) {
            return true;  // continue
        }
        return f(service);
    });
}

void ServiceManager::forEachServiceEntry(std::function<bool(const HidlService *)> f) const {
    for (const auto& interfaceMapping : mServiceMap) {
        const auto& instanceMap = interfaceMapping.second.getInstanceMap();

        for (const auto& instanceMapping : instanceMap) {
            if (!f(instanceMapping.second.get())) {
                return;
            }
        }
    }
}

void ServiceManager::forEachServiceEntry(std::function<bool(HidlService *)> f) {
    for (auto& interfaceMapping : mServiceMap) {
        auto& instanceMap = interfaceMapping.second.getInstanceMap();

        for (auto& instanceMapping : instanceMap) {
            if (!f(instanceMapping.second.get())) {
                return;
            }
        }
    }
}

HidlService* ServiceManager::lookup(const std::string& fqName, const std::string& name) {
    auto ifaceIt = mServiceMap.find(fqName);
    if (ifaceIt == mServiceMap.end()) {
        return nullptr;
    }

    PackageInterfaceMap &ifaceMap = ifaceIt->second;

    HidlService *hidlService = ifaceMap.lookup(name);

    return hidlService;
}

void ServiceManager::serviceDied(uint64_t cookie, const wp<IBase>& who) {
    bool serviceRemoved = false;
    switch (cookie) {
        case kServiceDiedCookie:
            serviceRemoved = removeService(who, nullptr /* restrictToInstanceName */);
            break;
        case kPackageListenerDiedCookie:
            serviceRemoved = removePackageListener(who);
            break;
        case kServiceListenerDiedCookie:
            serviceRemoved = removeServiceListener(who);
            break;
        case kClientCallbackDiedCookie: {
            sp<IBase> base = who.promote();
            IClientCallback* callback = static_cast<IClientCallback*>(base.get());
            serviceRemoved = unregisterClientCallback(nullptr /*service*/,
                                                      sp<IClientCallback>(callback));
        } break;
    }

    if (!serviceRemoved) {
        LOG(ERROR) << "Received death notification but interface instance not removed. Cookie: "
                   << cookie << " Service pointer: " << who.promote().get();
    }
}

ServiceManager::InstanceMap &ServiceManager::PackageInterfaceMap::getInstanceMap() {
    return mInstanceMap;
}

const ServiceManager::InstanceMap &ServiceManager::PackageInterfaceMap::getInstanceMap() const {
    return mInstanceMap;
}

const HidlService *ServiceManager::PackageInterfaceMap::lookup(
        const std::string &name) const {
    auto it = mInstanceMap.find(name);

    if (it == mInstanceMap.end()) {
        return nullptr;
    }

    return it->second.get();
}

HidlService *ServiceManager::PackageInterfaceMap::lookup(
        const std::string &name) {

    return const_cast<HidlService*>(
        const_cast<const PackageInterfaceMap*>(this)->lookup(name));
}

void ServiceManager::PackageInterfaceMap::insertService(
        std::unique_ptr<HidlService> &&service) {
    mInstanceMap.insert({service->getInstanceName(), std::move(service)});
}

void ServiceManager::PackageInterfaceMap::sendPackageRegistrationNotification(
        const hidl_string &fqName,
        const hidl_string &instanceName) {

    for (auto it = mPackageListeners.begin(); it != mPackageListeners.end();) {
        auto ret = (*it)->onRegistration(fqName, instanceName, false /* preexisting */);
        if (ret.isOk()) {
            ++it;
        } else {
            LOG(ERROR) << "Dropping registration callback for " << fqName << "/" << instanceName
                       << ": transport error.";
            it = mPackageListeners.erase(it);
        }
    }
}

void ServiceManager::PackageInterfaceMap::addPackageListener(sp<IServiceNotification> listener) {
    for (const auto &instanceMapping : mInstanceMap) {
        const std::unique_ptr<HidlService> &service = instanceMapping.second;

        if (service->getService() == nullptr) {
            continue;
        }

        auto ret = listener->onRegistration(
            service->getInterfaceName(),
            service->getInstanceName(),
            true /* preexisting */);
        if (!ret.isOk()) {
            LOG(ERROR) << "Not adding package listener for " << service->getInterfaceName()
                       << "/" << service->getInstanceName() << ": transport error "
                       << "when sending notification for already registered instance.";
            return;
        }
    }
    mPackageListeners.push_back(listener);
}

bool ServiceManager::PackageInterfaceMap::removePackageListener(const wp<IBase>& who) {
    bool found = false;

    for (auto it = mPackageListeners.begin(); it != mPackageListeners.end();) {
        if (interfacesEqual(*it, who.promote())) {
            it = mPackageListeners.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    return found;
}

bool ServiceManager::PackageInterfaceMap::removeServiceListener(const wp<IBase>& who) {
    bool found = false;

    for (auto &servicePair : getInstanceMap()) {
        const std::unique_ptr<HidlService> &service = servicePair.second;
        found |= service->removeListener(who);
    }

    return found;
}

static void tryStartService(const std::string& fqName, const std::string& name) {
    using ::android::base::SetProperty;

    std::thread([=] {
        bool success = SetProperty("ctl.interface_start", fqName + "/" + name);

        if (!success) {
            LOG(ERROR) << "Failed to set property for starting " << fqName << "/" << name;
        }
    }).detach();
}

// Methods from ::android::hidl::manager::V1_0::IServiceManager follow.
Return<sp<IBase>> ServiceManager::get(const hidl_string& hidlFqName,
                                      const hidl_string& hidlName) {
    const std::string fqName = hidlFqName;
    const std::string name = hidlName;

    if (!mAcl.canGet(fqName, getBinderCallingContext())) {
        return nullptr;
    }

    HidlService* hidlService = lookup(fqName, name);
    if (hidlService == nullptr) {
        tryStartService(fqName, name);
        return nullptr;
    }

    sp<IBase> service = hidlService->getService();
    if (service == nullptr) {
        tryStartService(fqName, name);
        return nullptr;
    }

    // Let HidlService know that we handed out a client. If the client drops the service before the
    // next time handleClientCallbacks is called, it will still know that the service had been handed out.
    hidlService->guaranteeClient();

    // This is executed immediately after the binder driver confirms the transaction. The driver
    // will update the appropriate data structures to reflect the fact that the client now has the
    // service this function is returning. Nothing else can update the HidlService at the same
    // time. This will run before anything else can modify the HidlService which is owned by this
    // object, so it will be in the same state that it was when this function returns.
    hardware::addPostCommandTask([hidlService] {
        hidlService->handleClientCallbacks(false /* isCalledOnInterval */);
    });

    return service;
}

Return<bool> ServiceManager::add(const hidl_string& name, const sp<IBase>& service) {
    bool addSuccess = false;

    if (service == nullptr) {
        return false;
    }

    auto pidcon = getBinderCallingContext();

    auto ret = service->interfaceChain([&](const auto &interfaceChain) {
        addSuccess = addImpl(name, service, interfaceChain, pidcon);
    });

    if (!ret.isOk()) {
        LOG(ERROR) << "Failed to retrieve interface chain: " << ret.description();
        return false;
    }

    return addSuccess;
}

bool ServiceManager::addImpl(const std::string& name,
                             const sp<IBase>& service,
                             const hidl_vec<hidl_string>& interfaceChain,
                             const AccessControl::CallingContext& callingContext) {
    if (interfaceChain.size() == 0) {
        LOG(WARNING) << "Empty interface chain for " << name;
        return false;
    }

    // First, verify you're allowed to add() the whole interface hierarchy
    for(size_t i = 0; i < interfaceChain.size(); i++) {
        const std::string fqName = interfaceChain[i];

        if (!mAcl.canAdd(fqName, callingContext)) {
            return false;
        }
    }

    // Detect duplicate registration
    if (interfaceChain.size() > 1) {
        // second to last entry should be the highest base class other than IBase.
        const std::string baseFqName = interfaceChain[interfaceChain.size() - 2];
        const HidlService *hidlService = lookup(baseFqName, name);
        if (hidlService != nullptr && hidlService->getService() != nullptr) {
            // This shouldn't occur during normal operation. Here are some cases where
            // it might get hit:
            // - bad configuration (service installed on device multiple times)
            // - race between death notification and a new service being registered
            //     (previous logs should indicate a separate problem)
            const std::string childFqName = interfaceChain[0];
            pid_t newServicePid = IPCThreadState::self()->getCallingPid();
            pid_t oldServicePid = hidlService->getDebugPid();
            LOG(WARNING) << "Detected instance of " << childFqName << " (pid: " << newServicePid
                    << ") registering over instance of or with base of " << baseFqName << " (pid: "
                    << oldServicePid << ").";
        }
    }

    // Unregister superclass if subclass is registered over it
    {
        // For IBar extends IFoo if IFoo/default is being registered, remove
        // IBar/default. This makes sure the following two things are equivalent
        // 1). IBar::castFrom(IFoo::getService(X))
        // 2). IBar::getService(X)
        // assuming that IBar is declared in the device manifest and there
        // is also not an IBaz extends IFoo and there is no race.
        const std::string childFqName = interfaceChain[0];
        const HidlService *hidlService = lookup(childFqName, name);
        if (hidlService != nullptr) {
            const sp<IBase> remove = hidlService->getService();

            if (remove != nullptr) {
                const std::string instanceName = name;
                removeService(remove, &instanceName /* restrictToInstanceName */);
            }
        }
    }

    for(size_t i = 0; i < interfaceChain.size(); i++) {
        const std::string fqName = interfaceChain[i];

        PackageInterfaceMap &ifaceMap = mServiceMap[fqName];
        HidlService *hidlService = ifaceMap.lookup(name);

        if (hidlService == nullptr) {
            ifaceMap.insertService(
                std::make_unique<HidlService>(fqName, name, service, callingContext.pid));
        } else {
            hidlService->setService(service, callingContext.pid);
        }

        ifaceMap.sendPackageRegistrationNotification(fqName, name);
    }

    bool linkRet = service->linkToDeath(this, kServiceDiedCookie).withDefault(false);
    if (!linkRet) {
        LOG(ERROR) << "Could not link to death for " << interfaceChain[0] << "/" << name;
    }

    return true;
}

Return<ServiceManager::Transport> ServiceManager::getTransport(const hidl_string& fqName,
                                                               const hidl_string& name) {
    using ::android::hardware::getTransport;

    if (!mAcl.canGet(fqName, getBinderCallingContext())) {
        return Transport::EMPTY;
    }

    switch (getTransport(fqName, name)) {
        case vintf::Transport::HWBINDER:
             return Transport::HWBINDER;
        case vintf::Transport::PASSTHROUGH:
             return Transport::PASSTHROUGH;
        case vintf::Transport::EMPTY:
        default:
             return Transport::EMPTY;
    }
}

Return<void> ServiceManager::list(list_cb _hidl_cb) {
    if (!mAcl.canList(getBinderCallingContext())) {
        _hidl_cb({});
        return Void();
    }

    hidl_vec<hidl_string> list;

    list.resize(countExistingService());

    size_t idx = 0;
    forEachExistingService([&] (const HidlService *service) {
        list[idx++] = service->string();
        return true;  // continue
    });

    _hidl_cb(list);
    return Void();
}

Return<void> ServiceManager::listByInterface(const hidl_string& fqName,
                                             listByInterface_cb _hidl_cb) {
    if (!mAcl.canGet(fqName, getBinderCallingContext())) {
        _hidl_cb({});
        return Void();
    }

    auto ifaceIt = mServiceMap.find(fqName);
    if (ifaceIt == mServiceMap.end()) {
        _hidl_cb(hidl_vec<hidl_string>());
        return Void();
    }

    const auto &instanceMap = ifaceIt->second.getInstanceMap();

    hidl_vec<hidl_string> list;

    size_t total = 0;
    for (const auto &serviceMapping : instanceMap) {
        const std::unique_ptr<HidlService> &service = serviceMapping.second;
        if (service->getService() == nullptr) continue;

        ++total;
    }
    list.resize(total);

    size_t idx = 0;
    for (const auto &serviceMapping : instanceMap) {
        const std::unique_ptr<HidlService> &service = serviceMapping.second;
        if (service->getService() == nullptr) continue;

        list[idx++] = service->getInstanceName();
    }

    _hidl_cb(list);
    return Void();
}

Return<bool> ServiceManager::registerForNotifications(const hidl_string& fqName,
                                                      const hidl_string& name,
                                                      const sp<IServiceNotification>& callback) {
    if (callback == nullptr) {
        return false;
    }

    if (!mAcl.canGet(fqName, getBinderCallingContext())) {
        return false;
    }

    PackageInterfaceMap &ifaceMap = mServiceMap[fqName];

    if (name.empty()) {
        bool ret = callback->linkToDeath(this, kPackageListenerDiedCookie).withDefault(false);
        if (!ret) {
            LOG(ERROR) << "Failed to register death recipient for " << fqName << "/" << name;
            return false;
        }
        ifaceMap.addPackageListener(callback);
        return true;
    }

    HidlService *service = ifaceMap.lookup(name);

    bool ret = callback->linkToDeath(this, kServiceListenerDiedCookie).withDefault(false);
    if (!ret) {
        LOG(ERROR) << "Failed to register death recipient for " << fqName << "/" << name;
        return false;
    }

    if (service == nullptr) {
        auto adding = std::make_unique<HidlService>(fqName, name);
        adding->addListener(callback);
        ifaceMap.insertService(std::move(adding));
    } else {
        service->addListener(callback);
    }

    return true;
}

Return<bool> ServiceManager::unregisterForNotifications(const hidl_string& fqName,
                                                        const hidl_string& name,
                                                        const sp<IServiceNotification>& callback) {
    if (callback == nullptr) {
        LOG(ERROR) << "Cannot unregister null callback for " << fqName << "/" << name;
        return false;
    }

    // NOTE: don't need ACL since callback is binder token, and if someone has gotten it,
    // then they already have access to it.

    if (fqName.empty()) {
        bool success = false;
        success |= removePackageListener(callback);
        success |= removeServiceListener(callback);
        return success;
    }

    PackageInterfaceMap &ifaceMap = mServiceMap[fqName];

    if (name.empty()) {
        bool success = false;
        success |= ifaceMap.removePackageListener(callback);
        success |= ifaceMap.removeServiceListener(callback);
        return success;
    }

    HidlService *service = ifaceMap.lookup(name);

    if (service == nullptr) {
        return false;
    }

    return service->removeListener(callback);
}

Return<bool> ServiceManager::registerClientCallback(const hidl_string& hidlFqName,
                                                    const hidl_string& hidlName,
                                                    const sp<IBase>& server,
                                                    const sp<IClientCallback>& cb) {
    if (server == nullptr || cb == nullptr) return false;

    const std::string fqName = hidlFqName;
    const std::string name = hidlName;

    // only the server of the interface can register a client callback
    pid_t pid = IPCThreadState::self()->getCallingPid();
    if (!mAcl.canAdd(fqName, getBinderCallingContext())) {
        return false;
    }

    HidlService* registered = lookup(fqName, name);

    if (registered == nullptr) {
        return false;
    }

    // sanity
    if (registered->getDebugPid() != pid) {
        LOG(WARNING) << "Only a server can register for client callbacks (for " << fqName
            << "/" << name << ")";
        return false;
    }

    sp<IBase> service = registered->getService();

    if (!interfacesEqual(service, server)) {
        LOG(WARNING) << "Tried to register client callback for " << fqName << "/" << name
            << " but a different service is registered under this name.";
        return false;
    }

    bool linkRet = cb->linkToDeath(this, kClientCallbackDiedCookie).withDefault(false);
    if (!linkRet) {
        LOG(ERROR) << "Could not link to death for registerClientCallback";
        return false;
    }

    registered->addClientCallback(cb);

    return true;
}

Return<bool> ServiceManager::unregisterClientCallback(const sp<IBase>& server,
                                                      const sp<IClientCallback>& cb) {
    if (cb == nullptr) return false;

    bool removed = false;

    forEachExistingService([&] (HidlService *service) {
        if (server == nullptr || interfacesEqual(service->getService(), server)) {
            removed |= service->removeClientCallback(cb);
        }
        return true;  // continue
    });

    return removed;
}

void ServiceManager::handleClientCallbacks() {
    forEachServiceEntry([&] (HidlService *service) {
        service->handleClientCallbacks(true /* isCalledOnInterval */);
        return true;  // continue
    });
}

Return<bool> ServiceManager::addWithChain(const hidl_string& name,
                                          const sp<IBase>& service,
                                          const hidl_vec<hidl_string>& chain) {
    if (service == nullptr) {
        return false;
    }

    auto callingContext = getBinderCallingContext();

    return addImpl(name, service, chain, callingContext);
}

Return<void> ServiceManager::listManifestByInterface(const hidl_string& fqName,
                                                     listManifestByInterface_cb _hidl_cb) {
    if (!mAcl.canGet(fqName, getBinderCallingContext())) {
        _hidl_cb({});
        return Void();
    }

    std::set<std::string> instances = getInstances(fqName);
    hidl_vec<hidl_string> ret(instances.begin(), instances.end());

    _hidl_cb(ret);
    return Void();
}

Return<bool> ServiceManager::tryUnregister(const hidl_string& hidlFqName,
                                           const hidl_string& hidlName,
                                           const sp<IBase>& service) {
    const std::string fqName = hidlFqName;
    const std::string name = hidlName;

    if (service == nullptr) {
        return false;
    }

    if (!mAcl.canAdd(fqName, getBinderCallingContext())) {
        return false;
    }

    HidlService* registered = lookup(fqName, name);

    // sanity
    pid_t pid = IPCThreadState::self()->getCallingPid();
    if (registered->getDebugPid() != pid) {
        LOG(WARNING) << "Only a server can unregister itself (for " << fqName
            << "/" << name << ")";
        return false;
    }

    sp<IBase> server = registered->getService();

    if (!interfacesEqual(service, server)) {
        LOG(WARNING) << "Tried to unregister for " << fqName << "/" << name
            << " but a different service is registered under this name.";
        return false;
    }

    int clients = registered->forceHandleClientCallbacks(false /* isCalledOnInterval */);

    // clients < 0: feature not implemented or other error. Assume clients.
    // Otherwise:
    // - kernel driver will hold onto one refcount (during this transaction)
    // - hwservicemanager has a refcount (guaranteed by this transaction)
    // So, if clients > 2, then at least one other service on the system must hold a refcount.
    if (clients < 0 || clients > 2) {
        // client callbacks are either disabled or there are other clients
        LOG(INFO) << "Tried to unregister for " << fqName << "/" << name
            << " but there are clients: " << clients;
        return false;
    }

    // will remove entire parent hierarchy
    bool success = removeService(service, &name /*restrictToInstanceName*/);

    if (registered->getService() != nullptr) {
        LOG(ERROR) << "Bad state. Unregistration failed for " << fqName << "/" << name << ".";
        return false;
    }

    return success;
}

Return<void> ServiceManager::debugDump(debugDump_cb _cb) {
    if (!mAcl.canList(getBinderCallingContext())) {
        _cb({});
        return Void();
    }

    std::vector<IServiceManager::InstanceDebugInfo> list;
    forEachServiceEntry([&] (const HidlService *service) {
        hidl_vec<int32_t> clientPids;
        clientPids.resize(service->getPassthroughClients().size());

        size_t i = 0;
        for (pid_t p : service->getPassthroughClients()) {
            clientPids[i++] = p;
        }

        list.push_back({
            .pid = service->getDebugPid(),
            .interfaceName = service->getInterfaceName(),
            .instanceName = service->getInstanceName(),
            .clientPids = clientPids,
            .arch = ::android::hidl::base::V1_0::DebugInfo::Architecture::UNKNOWN
        });

        return true;  // continue
    });

    _cb(list);
    return Void();
}


Return<void> ServiceManager::registerPassthroughClient(const hidl_string &fqName,
        const hidl_string &name) {
    auto callingContext = getBinderCallingContext();

    if (!mAcl.canGet(fqName, callingContext)) {
        /* We guard this function with "get", because it's typically used in
         * the getService() path, albeit for a passthrough service in this
         * case
         */
        return Void();
    }

    PackageInterfaceMap &ifaceMap = mServiceMap[fqName];

    if (name.empty()) {
        LOG(WARNING) << "registerPassthroughClient encounters empty instance name for "
                     << fqName.c_str();
        return Void();
    }

    HidlService *service = ifaceMap.lookup(name);

    if (service == nullptr) {
        auto adding = std::make_unique<HidlService>(fqName, name);
        adding->registerPassthroughClient(callingContext.pid);
        ifaceMap.insertService(std::move(adding));
    } else {
        service->registerPassthroughClient(callingContext.pid);
    }
    return Void();
}

bool ServiceManager::removeService(const wp<IBase>& who, const std::string* restrictToInstanceName) {
    bool keepInstance = false;
    bool removed = false;
    for (auto &interfaceMapping : mServiceMap) {
        auto &instanceMap = interfaceMapping.second.getInstanceMap();

        for (auto &servicePair : instanceMap) {
            const std::string &instanceName = servicePair.first;
            const std::unique_ptr<HidlService> &service = servicePair.second;

            if (interfacesEqual(service->getService(), who.promote())) {
                if (restrictToInstanceName != nullptr && *restrictToInstanceName != instanceName) {
                    // We cannot remove all instances of this service, so we don't return that it
                    // has been entirely removed.
                    keepInstance = true;
                    continue;
                }

                service->setService(nullptr, static_cast<pid_t>(IServiceManager::PidConstant::NO_PID));
                removed = true;
            }
        }
    }

    return !keepInstance && removed;
}

bool ServiceManager::removePackageListener(const wp<IBase>& who) {
    bool found = false;

    for (auto &interfaceMapping : mServiceMap) {
        found |= interfaceMapping.second.removePackageListener(who);
    }

    return found;
}

bool ServiceManager::removeServiceListener(const wp<IBase>& who) {
    bool found = false;
    for (auto &interfaceMapping : mServiceMap) {
        auto &packageInterfaceMap = interfaceMapping.second;

        found |= packageInterfaceMap.removeServiceListener(who);
    }
    return found;
}
}  // namespace implementation
}  // namespace manager
}  // namespace hidl
}  // namespace android
