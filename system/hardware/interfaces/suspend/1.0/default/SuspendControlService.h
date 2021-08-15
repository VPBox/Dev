/*
 * Copyright 2019 The Android Open Source Project
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

#ifndef ANDROID_SYSTEM_SYSTEM_SUSPEND_CONTROL_SERVICE_H
#define ANDROID_SYSTEM_SYSTEM_SUSPEND_CONTROL_SERVICE_H

#include <android/system/suspend/BnSuspendControlService.h>

using ::android::system::suspend::BnSuspendControlService;
using ::android::system::suspend::ISuspendCallback;

namespace android {
namespace system {
namespace suspend {
namespace V1_0 {

class SystemSuspend;

class SuspendControlService : public BnSuspendControlService,
                              public virtual IBinder::DeathRecipient {
   public:
    SuspendControlService() = default;
    ~SuspendControlService() override = default;

    binder::Status enableAutosuspend(bool* _aidl_return) override;
    binder::Status registerCallback(const sp<ISuspendCallback>& callback,
                                    bool* _aidl_return) override;
    binder::Status forceSuspend(bool* _aidl_return) override;
    void binderDied(const wp<IBinder>& who) override;

    void setSuspendService(const wp<SystemSuspend>& suspend);
    void notifyWakeup(bool success);

   private:
    wp<SystemSuspend> mSuspend;

    std::mutex mCallbackLock;
    std::vector<sp<ISuspendCallback> > mCallbacks;
    std::vector<sp<ISuspendCallback> >::iterator findCb(const wp<IBinder>& cb) {
        return std::find_if(
            mCallbacks.begin(), mCallbacks.end(),
            [&cb](const sp<ISuspendCallback> i) { return cb == IInterface::asBinder(i); });
    }
};

}  // namespace V1_0
}  // namespace suspend
}  // namespace system
}  // namespace android

#endif  // ANDROID_SYSTEM_SYSTEM_SUSPEND_CONTROL_SERVICE_H
