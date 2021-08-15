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

#include "SuspendControlService.h"
#include "SystemSuspend.h"

#include <android-base/logging.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <cutils/native_handle.h>
#include <hidl/HidlTransportSupport.h>
#include <hwbinder/ProcessState.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>

using android::sp;
using android::status_t;
using android::base::Socketpair;
using android::base::unique_fd;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::system::suspend::V1_0::ISystemSuspend;
using android::system::suspend::V1_0::SuspendControlService;
using android::system::suspend::V1_0::SystemSuspend;
using namespace std::chrono_literals;

static constexpr char kSysPowerWakeupCount[] = "/sys/power/wakeup_count";
static constexpr char kSysPowerState[] = "/sys/power/state";

int main() {
    unique_fd wakeupCountFd{TEMP_FAILURE_RETRY(open(kSysPowerWakeupCount, O_CLOEXEC | O_RDWR))};
    if (wakeupCountFd < 0) {
        PLOG(ERROR) << "error opening " << kSysPowerWakeupCount;
    }
    unique_fd stateFd{TEMP_FAILURE_RETRY(open(kSysPowerState, O_CLOEXEC | O_RDWR))};
    if (stateFd < 0) {
        PLOG(ERROR) << "error opening " << kSysPowerState;
    }

    // If either /sys/power/wakeup_count or /sys/power/state fail to open, we construct
    // SystemSuspend with blocking fds. This way this process will keep running, handle wake lock
    // requests, collect stats, but won't suspend the device. We want this behavior on devices
    // (hosts) where system suspend should not be handles by Android platform e.g. ARC++, Android
    // virtual devices.
    if (wakeupCountFd < 0 || stateFd < 0) {
        // This will block all reads/writes to these fds from the suspend thread.
        Socketpair(SOCK_STREAM, &wakeupCountFd, &stateFd);
    }

    configureRpcThreadpool(1, true /* callerWillJoin */);

    sp<SuspendControlService> suspendControl = new SuspendControlService();
    auto controlStatus = android::defaultServiceManager()->addService(
        android::String16("suspend_control"), suspendControl);
    if (controlStatus != android::OK) {
        LOG(FATAL) << "Unable to register suspend_control service: " << controlStatus;
    }

    // Create non-HW binder threadpool for SuspendControlService.
    sp<android::ProcessState> ps{android::ProcessState::self()};
    ps->startThreadPool();

    sp<SystemSuspend> suspend =
        new SystemSuspend(std::move(wakeupCountFd), std::move(stateFd), 100 /* maxStatsEntries */,
                          100ms /* baseSleepTime */, suspendControl, false /* mUseSuspendCounter*/);
    status_t status = suspend->registerAsService();
    if (android::OK != status) {
        LOG(FATAL) << "Unable to register system-suspend service: " << status;
    }

    joinRpcThreadpool();
    std::abort(); /* unreachable */
}
