/**
 * Copyright (c) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NETD_SERVER_OEM_NETD_LISTENER_H
#define NETD_SERVER_OEM_NETD_LISTENER_H

#include <map>
#include <mutex>

#include <android-base/thread_annotations.h>
#include "com/android/internal/net/BnOemNetd.h"
#include "com/android/internal/net/IOemNetdUnsolicitedEventListener.h"

namespace com {
namespace android {
namespace internal {
namespace net {

class OemNetdListener : public BnOemNetd {
  public:
    using OemUnsolListenerMap = std::map<const ::android::sp<IOemNetdUnsolicitedEventListener>,
                                         const ::android::sp<::android::IBinder::DeathRecipient>>;

    OemNetdListener() = default;
    ~OemNetdListener() = default;
    static ::android::sp<::android::IBinder> getListener();

    ::android::binder::Status isAlive(bool* alive) override;
    ::android::binder::Status registerOemUnsolicitedEventListener(
            const ::android::sp<IOemNetdUnsolicitedEventListener>& listener) override;

  private:
    std::mutex mMutex;
    std::mutex mOemUnsolicitedMutex;

    ::android::sp<::android::IBinder> mIBinder GUARDED_BY(mMutex);
    OemUnsolListenerMap mOemUnsolListenerMap GUARDED_BY(mOemUnsolicitedMutex);

    ::android::sp<::android::IBinder> getIBinder() EXCLUDES(mMutex);

    void registerOemUnsolicitedEventListenerInternal(
            const ::android::sp<IOemNetdUnsolicitedEventListener>& listener)
            EXCLUDES(mOemUnsolicitedMutex);
    void unregisterOemUnsolicitedEventListenerInternal(
            const ::android::sp<IOemNetdUnsolicitedEventListener>& listener)
            EXCLUDES(mOemUnsolicitedMutex);
};

}  // namespace net
}  // namespace internal
}  // namespace android
}  // namespace com

#endif  // NETD_SERVER_OEM_NETD_LISTENER_H