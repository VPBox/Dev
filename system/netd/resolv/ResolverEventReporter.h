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

#ifndef NETD_RESOLV_EVENT_REPORTER_H
#define NETD_RESOLV_EVENT_REPORTER_H

#include <set>

#include <android-base/thread_annotations.h>

#include "aidl/android/net/metrics/INetdEventListener.h"

/*
 * This class can be used to get the binder reference to the netd events listener service
 * via stable runtime ABI which is achieved from libbinder_ndk. It also allows that
 * register an event listener.
 */
class ResolverEventReporter {
  public:
    ResolverEventReporter(ResolverEventReporter const&) = delete;
    ResolverEventReporter(ResolverEventReporter&&) = delete;
    ResolverEventReporter& operator=(ResolverEventReporter const&) = delete;
    ResolverEventReporter& operator=(ResolverEventReporter&&) = delete;

    using ListenerSet = std::set<std::shared_ptr<aidl::android::net::metrics::INetdEventListener>>;

    // Get the instance of the singleton ResolverEventReporter.
    static ResolverEventReporter& getInstance();

    // Return the binder from the singleton ResolverEventReporter. This method is threadsafe.
    ListenerSet getListeners() const;

    // Add the binder to the singleton ResolverEventReporter. This method is threadsafe.
    int addListener(
            const std::shared_ptr<aidl::android::net::metrics::INetdEventListener>& listener);

  private:
    ResolverEventReporter() = default;
    ~ResolverEventReporter() = default;

    void addDefaultListener() EXCLUDES(mMutex);
    int addListenerImpl(
            const std::shared_ptr<aidl::android::net::metrics::INetdEventListener>& listener)
            EXCLUDES(mMutex);
    int addListenerImplLocked(
            const std::shared_ptr<aidl::android::net::metrics::INetdEventListener>& listener)
            REQUIRES(mMutex);
    ListenerSet getListenersImpl() const EXCLUDES(mMutex);
    void handleBinderDied(const void* who) EXCLUDES(mMutex);

    mutable std::mutex mMutex;
    ListenerSet mListeners GUARDED_BY(mMutex);
};

#endif  // NETD_RESOLV_EVENT_REPORTER_H
