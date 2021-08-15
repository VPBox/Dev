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

#include <android-base/unique_fd.h>
#include <binder/BinderService.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include <android/ashmemd/BnAshmemDeviceService.h>

using android::String16;
using android::base::unique_fd;

namespace android {
namespace ashmemd {

class AshmemDeviceService : public BnAshmemDeviceService {
  public:
    binder::Status open(os::ParcelFileDescriptor* ashmemFd) override {
        ashmemFd->reset(unique_fd(TEMP_FAILURE_RETRY(::open("/dev/ashmem", O_RDWR | O_CLOEXEC))));
        return binder::Status::ok();
    }
};

void CreateAndRegisterService() {
    sp<AshmemDeviceService> ashmemService = new AshmemDeviceService();
    defaultServiceManager()->addService(String16("ashmem_device_service"), ashmemService,
                                        true /* allowIsolated */);
}

void JoinThreadPool() {
    sp<ProcessState> ps = ProcessState::self();
    IPCThreadState::self()->joinThreadPool();  // should not return
}

}  // namespace ashmemd
}  // namespace android

int main() {
    android::ashmemd::CreateAndRegisterService();
    android::ashmemd::JoinThreadPool();
    std::abort();  // unreachable
}
