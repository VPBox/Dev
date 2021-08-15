/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <android-base/logging.h>
#include <android/ashmemd/IAshmemDeviceService.h>
#include <binder/IServiceManager.h>
#include <cutils/android_filesystem_config.h>

using android::IBinder;
using android::IServiceManager;
using android::String16;
using android::ashmemd::IAshmemDeviceService;
using android::os::ParcelFileDescriptor;

namespace android {
namespace ashmemd {

static bool checkBinderAccess() {
    // Isolated apps are potentially subject to seccomp policy that restricts use of access()
    // (b/129483782). However, apps always have access to binder, so return true.
    auto uid = getuid() % AID_USER;
    if (AID_ISOLATED_START <= uid && uid <= AID_ISOLATED_END) {
        return true;
    }
    if (access("/dev/binder", R_OK | W_OK) == 0) {
        return true;
    }
    return false;
}

sp<IAshmemDeviceService> getAshmemService() {
    // Calls to defaultServiceManager() crash the process if it doesn't have appropriate
    // binder permissions. Check these permissions proactively.
    if (!checkBinderAccess()) {
        return nullptr;
    }
    sp<IServiceManager> sm = android::defaultServiceManager();
    sp<IBinder> binder = sm->checkService(String16("ashmem_device_service"));
    return interface_cast<IAshmemDeviceService>(binder);
}

extern "C" int openAshmemdFd() {
    static sp<IAshmemDeviceService> ashmemService = getAshmemService();
    if (!ashmemService) {
        LOG(ERROR) << "Failed to get IAshmemDeviceService.";
        return -1;
    }

    ParcelFileDescriptor fd;
    auto status = ashmemService->open(&fd);
    if (!status.isOk()) {
        LOG(ERROR) << "Failed IAshmemDeviceService::open()";
        return -1;
    }

    // unique_fd is the underlying type of ParcelFileDescriptor, i.e. fd is
    // closed when it falls out of scope, so we make a dup.
    return dup(fd.get());
}

}  // namespace ashmemd
}  // namespace android
