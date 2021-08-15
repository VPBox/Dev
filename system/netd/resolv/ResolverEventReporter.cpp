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
#define LOG_TAG "ResolverEventReporter"

#include "ResolverEventReporter.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>

using aidl::android::net::metrics::INetdEventListener;

ResolverEventReporter& ResolverEventReporter::getInstance() {
    // It should be initialized only once.
    static ResolverEventReporter instance;

    // Add framework metrics listener. Because the binder service "netd_listener" may be launched
    // later than Netd, try to get binder handler in every instance query if any. The framework
    // metrics listener should be added only once if it has been already added successfully.
    instance.addDefaultListener();

    return instance;
}

ResolverEventReporter::ListenerSet ResolverEventReporter::getListeners() const {
    return getListenersImpl();
}

int ResolverEventReporter::addListener(const std::shared_ptr<INetdEventListener>& listener) {
    return addListenerImpl(listener);
}

// TODO: Consider registering metrics listener from framework and remove this function.
// Currently, the framework listener "netd_listener" is shared by netd and libnetd_resolv.
// Consider breaking it into two listeners. Once it has done, may let framework register
// the listener proactively.
void ResolverEventReporter::addDefaultListener() {
    std::lock_guard lock(mMutex);

    static bool added = false;
    if (added) return;

    // Use the non-blocking call AServiceManager_checkService in order not to delay DNS
    // lookup threads when the netd_listener service is not ready.
    ndk::SpAIBinder binder = ndk::SpAIBinder(AServiceManager_checkService("netd_listener"));
    std::shared_ptr<INetdEventListener> listener = INetdEventListener::fromBinder(binder);

    if (listener == nullptr) return;

    if (!addListenerImplLocked(listener)) added = true;
}

void ResolverEventReporter::handleBinderDied(const void* who) {
    std::lock_guard lock(mMutex);

    // Use the raw binder pointer address to be the identification of dead binder. Treat "who"
    // which passes the raw address of dead binder as an identification only.
    auto found = std::find_if(mListeners.begin(), mListeners.end(),
                              [=](const auto& it) { return static_cast<void*>(it.get()) == who; });

    if (found != mListeners.end()) mListeners.erase(found);
}

ResolverEventReporter::ListenerSet ResolverEventReporter::getListenersImpl() const {
    std::lock_guard lock(mMutex);
    return mListeners;
}

int ResolverEventReporter::addListenerImpl(const std::shared_ptr<INetdEventListener>& listener) {
    std::lock_guard lock(mMutex);
    return addListenerImplLocked(listener);
}

int ResolverEventReporter::addListenerImplLocked(
        const std::shared_ptr<INetdEventListener>& listener) {
    if (listener == nullptr) {
        LOG(ERROR) << "The listener should not be null";
        return -EINVAL;
    }

    // TODO: Perhaps ignore the listener which comes from the same binder.
    const auto& it = mListeners.find(listener);
    if (it != mListeners.end()) {
        LOG(WARNING) << "The listener was already subscribed";
        return -EEXIST;
    }

    static AIBinder_DeathRecipient* deathRecipient = nullptr;
    if (deathRecipient == nullptr) {
        // The AIBinder_DeathRecipient object is used to manage all death recipients for multiple
        // binder objects. It doesn't released because there should have at least one binder object
        // from framework.
        // TODO: Considering to remove death recipient for the binder object from framework because
        // it doesn't need death recipient actually.
        deathRecipient = AIBinder_DeathRecipient_new([](void* cookie) {
            ResolverEventReporter::getInstance().handleBinderDied(cookie);
        });
    }

    // Pass the raw binder pointer address to be the cookie of the death recipient. While the death
    // notification is fired, the cookie is used for identifying which binder was died. Because
    // the NDK binder doesn't pass dead binder pointer to binder death handler, the binder death
    // handler can't know who was died via wp<IBinder>. The reason for wp<IBinder> is not passed
    // is that NDK binder can't transform a wp<IBinder> to a wp<AIBinder> in some cases.
    // See more information in b/128712772.
    auto binder = listener->asBinder().get();
    auto cookie = static_cast<void*>(listener.get());  // Used for dead binder identification.
    binder_status_t status = AIBinder_linkToDeath(binder, deathRecipient, cookie);

    if (STATUS_OK != status) {
        LOG(ERROR) << "Failed to register death notification for INetdEventListener";
        return -EAGAIN;
    }

    mListeners.insert(listener);
    return 0;
}