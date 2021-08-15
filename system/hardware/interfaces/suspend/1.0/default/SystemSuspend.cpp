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

#include "SystemSuspend.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <google/protobuf/text_format.h>
#include <hidl/Status.h>
#include <hwbinder/IPCThreadState.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <ctime>
#include <string>
#include <thread>

using ::android::base::ReadFdToString;
using ::android::base::WriteStringToFd;
using ::android::hardware::Void;
using ::std::string;

namespace android {
namespace system {
namespace suspend {
namespace V1_0 {

static const char kSleepState[] = "mem";
// TODO(b/128923994): we only need /sys/power/wake_[un]lock to export debugging info via
// /sys/kernel/debug/wakeup_sources.
static constexpr char kSysPowerWakeLock[] = "/sys/power/wake_lock";
static constexpr char kSysPowerWakeUnlock[] = "/sys/power/wake_unlock";

// This function assumes that data in fd is small enough that it can be read in one go.
// We use this function instead of the ones available in libbase because it doesn't block
// indefinitely when reading from socket streams which are used for testing.
string readFd(int fd) {
    char buf[BUFSIZ];
    ssize_t n = TEMP_FAILURE_RETRY(read(fd, &buf[0], sizeof(buf)));
    if (n < 0) return "";
    return string{buf, static_cast<size_t>(n)};
}

static inline int getCallingPid() {
    return ::android::hardware::IPCThreadState::self()->getCallingPid();
}

static inline WakeLockIdType getWakeLockId(int pid, const string& name) {
    // Doesn't guarantee unique ids, but for debuging purposes this is adequate.
    return std::to_string(pid) + "/" + name;
}

TimestampType getEpochTimeNow() {
    auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(timeSinceEpoch).count();
}

WakeLock::WakeLock(SystemSuspend* systemSuspend, const WakeLockIdType& id, const string& name)
    : mReleased(), mSystemSuspend(systemSuspend), mId(id), mName(name) {
    mSystemSuspend->incSuspendCounter(mName);
}

WakeLock::~WakeLock() {
    releaseOnce();
}

Return<void> WakeLock::release() {
    releaseOnce();
    return Void();
}

void WakeLock::releaseOnce() {
    std::call_once(mReleased, [this]() {
        mSystemSuspend->decSuspendCounter(mName);
        mSystemSuspend->deleteWakeLockStatsEntry(mId);
    });
}

SystemSuspend::SystemSuspend(unique_fd wakeupCountFd, unique_fd stateFd, size_t maxStatsEntries,
                             std::chrono::milliseconds baseSleepTime,
                             const sp<SuspendControlService>& controlService,
                             bool useSuspendCounter)
    : mSuspendCounter(0),
      mWakeupCountFd(std::move(wakeupCountFd)),
      mStateFd(std::move(stateFd)),
      mMaxStatsEntries(maxStatsEntries),
      mBaseSleepTime(baseSleepTime),
      mSleepTime(baseSleepTime),
      mControlService(controlService),
      mUseSuspendCounter(useSuspendCounter),
      mWakeLockFd(-1),
      mWakeUnlockFd(-1) {
    mControlService->setSuspendService(this);

    if (!mUseSuspendCounter) {
        mWakeLockFd.reset(TEMP_FAILURE_RETRY(open(kSysPowerWakeLock, O_CLOEXEC | O_RDWR)));
        if (mWakeLockFd < 0) {
            PLOG(ERROR) << "error opening " << kSysPowerWakeLock;
        }
        mWakeUnlockFd.reset(TEMP_FAILURE_RETRY(open(kSysPowerWakeUnlock, O_CLOEXEC | O_RDWR)));
        if (mWakeUnlockFd < 0) {
            PLOG(ERROR) << "error opening " << kSysPowerWakeUnlock;
        }
    }
}

bool SystemSuspend::enableAutosuspend() {
    static bool initialized = false;
    if (initialized) {
        LOG(ERROR) << "Autosuspend already started.";
        return false;
    }

    initAutosuspend();
    initialized = true;
    return true;
}

bool SystemSuspend::forceSuspend() {
    //  We are forcing the system to suspend. This particular call ignores all
    //  existing wakelocks (full or partial). It does not cancel the wakelocks
    //  or reset mSuspendCounter, it just ignores them.  When the system
    //  returns from suspend, the wakelocks and SuspendCounter will not have
    //  changed.
    auto counterLock = std::unique_lock(mCounterLock);
    bool success = WriteStringToFd(kSleepState, mStateFd);
    counterLock.unlock();

    if (!success) {
        PLOG(VERBOSE) << "error writing to /sys/power/state for forceSuspend";
    }
    return success;
}

Return<sp<IWakeLock>> SystemSuspend::acquireWakeLock(WakeLockType /* type */,
                                                     const hidl_string& name) {
    auto pid = getCallingPid();
    auto wlId = getWakeLockId(pid, name);
    IWakeLock* wl = new WakeLock{this, wlId, name};
    {
        auto l = std::lock_guard(mStatsLock);

        auto& wlStatsEntry = (*mStats.mutable_wl_stats())[wlId];
        auto lastUpdated = wlStatsEntry.last_updated();
        auto timeNow = getEpochTimeNow();
        mLruWakeLockId.erase(lastUpdated);
        mLruWakeLockId[timeNow] = wlId;

        wlStatsEntry.set_name(name);
        wlStatsEntry.set_pid(pid);
        wlStatsEntry.set_active(true);
        wlStatsEntry.set_last_updated(timeNow);

        if (mStats.wl_stats().size() > mMaxStatsEntries) {
            auto lruWakeLockId = mLruWakeLockId.begin()->second;
            mLruWakeLockId.erase(mLruWakeLockId.begin());
            mStats.mutable_wl_stats()->erase(lruWakeLockId);
        }
    }
    return wl;
}

Return<void> SystemSuspend::debug(const hidl_handle& handle,
                                  const hidl_vec<hidl_string>& /* options */) {
    if (handle == nullptr || handle->numFds < 1 || handle->data[0] < 0) {
        LOG(ERROR) << "no valid fd";
        return Void();
    }
    int fd = handle->data[0];
    string debugStr;
    {
        auto l = std::lock_guard(mStatsLock);
        google::protobuf::TextFormat::PrintToString(mStats, &debugStr);
    }
    WriteStringToFd(debugStr, fd);
    fsync(fd);
    return Void();
}

void SystemSuspend::incSuspendCounter(const string& name) {
    auto l = std::lock_guard(mCounterLock);
    if (mUseSuspendCounter) {
        mSuspendCounter++;
    } else {
        if (!WriteStringToFd(name, mWakeLockFd)) {
            PLOG(ERROR) << "error writing " << name << " to " << kSysPowerWakeLock;
        }
    }
}

void SystemSuspend::decSuspendCounter(const string& name) {
    auto l = std::lock_guard(mCounterLock);
    if (mUseSuspendCounter) {
        if (--mSuspendCounter == 0) {
            mCounterCondVar.notify_one();
        }
    } else {
        if (!WriteStringToFd(name, mWakeUnlockFd)) {
            PLOG(ERROR) << "error writing " << name << " to " << kSysPowerWakeUnlock;
        }
    }
}

void SystemSuspend::deleteWakeLockStatsEntry(WakeLockIdType id) {
    auto l = std::lock_guard(mStatsLock);
    auto* wlStats = mStats.mutable_wl_stats();
    if (wlStats->find(id) != wlStats->end()) {
        auto& wlStatsEntry = (*wlStats)[id];
        auto timeNow = getEpochTimeNow();
        auto lastUpdated = wlStatsEntry.last_updated();
        wlStatsEntry.set_active(false);
        wlStatsEntry.set_last_updated(timeNow);
        mLruWakeLockId.erase(lastUpdated);
        mLruWakeLockId[timeNow] = id;
    }
}

void SystemSuspend::initAutosuspend() {
    std::thread autosuspendThread([this] {
        while (true) {
            std::this_thread::sleep_for(mSleepTime);
            lseek(mWakeupCountFd, 0, SEEK_SET);
            const string wakeupCount = readFd(mWakeupCountFd);
            if (wakeupCount.empty()) {
                PLOG(ERROR) << "error reading from /sys/power/wakeup_count";
                continue;
            }

            auto counterLock = std::unique_lock(mCounterLock);
            mCounterCondVar.wait(counterLock, [this] { return mSuspendCounter == 0; });
            // The mutex is locked and *MUST* remain locked until we write to /sys/power/state.
            // Otherwise, a WakeLock might be acquired after we check mSuspendCounter and before we
            // write to /sys/power/state.

            if (!WriteStringToFd(wakeupCount, mWakeupCountFd)) {
                PLOG(VERBOSE) << "error writing from /sys/power/wakeup_count";
                continue;
            }
            bool success = WriteStringToFd(kSleepState, mStateFd);
            counterLock.unlock();

            if (!success) {
                PLOG(VERBOSE) << "error writing to /sys/power/state";
            }

            mControlService->notifyWakeup(success);

            updateSleepTime(success);
        }
    });
    autosuspendThread.detach();
    LOG(INFO) << "automatic system suspend enabled";
}

void SystemSuspend::updateSleepTime(bool success) {
    static constexpr std::chrono::milliseconds kMaxSleepTime = 1min;
    if (success) {
        mSleepTime = mBaseSleepTime;
        return;
    }
    // Double sleep time after each failure up to one minute.
    mSleepTime = std::min(mSleepTime * 2, kMaxSleepTime);
}

}  // namespace V1_0
}  // namespace suspend
}  // namespace system
}  // namespace android
