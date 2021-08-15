/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_SYSTEM_SYSTEM_SUSPEND_V1_0_H
#define ANDROID_SYSTEM_SYSTEM_SUSPEND_V1_0_H

#include "SuspendControlService.h"

#include <android-base/unique_fd.h>
#include <android/system/suspend/1.0/ISystemSuspend.h>
#include <hidl/HidlTransportSupport.h>
#include <system/hardware/interfaces/suspend/1.0/default/SystemSuspendStats.pb.h>

#include <condition_variable>
#include <mutex>
#include <string>

namespace android {
namespace system {
namespace suspend {
namespace V1_0 {

using ::android::base::unique_fd;
using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::interfacesEqual;
using ::android::hardware::Return;

using TimestampType = uint64_t;
using WakeLockIdType = std::string;

using namespace std::chrono_literals;

class SystemSuspend;

std::string readFd(int fd);
TimestampType getEpochTimeNow();

class WakeLock : public IWakeLock {
   public:
    WakeLock(SystemSuspend* systemSuspend, const WakeLockIdType& id, const std::string& name);
    ~WakeLock();

    Return<void> release();

   private:
    inline void releaseOnce();
    std::once_flag mReleased;

    SystemSuspend* mSystemSuspend;
    WakeLockIdType mId;
    std::string mName;
};

class SystemSuspend : public ISystemSuspend {
   public:
    SystemSuspend(unique_fd wakeupCountFd, unique_fd stateFd, size_t maxStatsEntries,
                  std::chrono::milliseconds baseSleepTime,
                  const sp<SuspendControlService>& controlService, bool useSuspendCounter = true);
    Return<sp<IWakeLock>> acquireWakeLock(WakeLockType type, const hidl_string& name) override;
    Return<void> debug(const hidl_handle& handle, const hidl_vec<hidl_string>& options) override;
    void incSuspendCounter(const std::string& name);
    void decSuspendCounter(const std::string& name);
    void deleteWakeLockStatsEntry(WakeLockIdType id);
    bool enableAutosuspend();
    bool forceSuspend();

   private:
    void initAutosuspend();

    std::mutex mCounterLock;
    std::condition_variable mCounterCondVar;
    uint32_t mSuspendCounter;
    unique_fd mWakeupCountFd;
    unique_fd mStateFd;

    // mStats can be inconsistent with with mSuspendCounter since we use two separate locks to
    // protect these. However, since mStats is only for debugging we prioritize performance.
    // Never hold both locks at the same time to avoid deadlock.
    std::mutex mStatsLock;
    //  We don't want mStats to grow unboundedly in memory. This constant limits amount of
    //  information mStats can collect on the device.
    size_t mMaxStatsEntries;
    // Used to evict the least recently used wake lock stats entry in case mMaxStatsEntries is
    // reached.
    std::map<TimestampType, WakeLockIdType> mLruWakeLockId;
    SystemSuspendStats mStats;

    // Amount of sleep time between consecutive iterations of the suspend loop.
    std::chrono::milliseconds mBaseSleepTime;
    std::chrono::milliseconds mSleepTime;
    // Updates sleep time depending on the result of suspend attempt.
    void updateSleepTime(bool success);

    sp<SuspendControlService> mControlService;

    // If true, use mSuspendCounter to keep track of native wake locks. Otherwise, rely on
    // /sys/power/wake_lock interface to block suspend.
    // TODO(b/128923994): remove dependency on /sys/power/wake_lock interface.
    bool mUseSuspendCounter;
    unique_fd mWakeLockFd;
    unique_fd mWakeUnlockFd;
};

}  // namespace V1_0
}  // namespace suspend
}  // namespace system
}  // namespace android

#endif  // ANDROID_SYSTEM_SYSTEM_SUSPEND_V1_0_H
