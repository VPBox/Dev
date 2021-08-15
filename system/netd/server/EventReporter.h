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

#ifndef NETD_SERVER_EVENT_REPORTER_H
#define NETD_SERVER_EVENT_REPORTER_H

#include <map>
#include <mutex>

#include <android-base/thread_annotations.h>
#include <binder/IServiceManager.h>
#include "android/net/INetd.h"
#include "android/net/INetdUnsolicitedEventListener.h"
#include "android/net/metrics/INetdEventListener.h"

/*
 * This class can be used to get the event listener service.
 */
class EventReporter {
  public:
    using UnsolListenerMap =
            std::map<const android::sp<android::net::INetdUnsolicitedEventListener>,
                     const android::sp<android::IBinder::DeathRecipient>>;

    // Returns the binder reference to the netd events listener service, attempting to fetch it if
    // we do not have it already. This method is threadsafe.
    android::sp<android::net::metrics::INetdEventListener> getNetdEventListener();

    // Returns a copy of the registered listeners.
    UnsolListenerMap getNetdUnsolicitedEventListenerMap() const EXCLUDES(mUnsolicitedMutex);

    void registerUnsolEventListener(
            const android::sp<android::net::INetdUnsolicitedEventListener>& listener)
            EXCLUDES(mUnsolicitedMutex);
    void unregisterUnsolEventListener(
            const android::sp<android::net::INetdUnsolicitedEventListener>& listener)
            EXCLUDES(mUnsolicitedMutex);

  private:
    std::mutex mEventMutex;
    mutable std::mutex mUnsolicitedMutex;
    android::sp<android::net::metrics::INetdEventListener> mNetdEventListener
            GUARDED_BY(mEventMutex);
    UnsolListenerMap mUnsolListenerMap GUARDED_BY(mUnsolicitedMutex);
};

#endif  // NETD_SERVER_EVENT_REPORTER_H
