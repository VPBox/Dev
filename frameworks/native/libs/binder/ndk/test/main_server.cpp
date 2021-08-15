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

#include <android-base/logging.h>
#include <android/binder_process.h>
#include <iface/iface.h>

using ::android::sp;

class MyFoo : public IFoo {
    binder_status_t doubleNumber(int32_t in, int32_t* out) override {
        *out = 2 * in;
        LOG(INFO) << "doubleNumber (" << in << ") => " << *out;
        return STATUS_OK;
    }

    binder_status_t die() override {
        LOG(FATAL) << "IFoo::die called!";
        return STATUS_UNKNOWN_ERROR;
    }
};

int service(const char* instance) {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    // Strong reference to MyFoo kept by service manager.
    binder_status_t status = (new MyFoo)->addService(instance);

    if (status != STATUS_OK) {
        LOG(FATAL) << "Could not register: " << status << " " << instance;
    }

    ABinderProcess_joinThreadPool();

    return 1;  // should not return
}

int main() {
    if (fork() == 0) {
        return service(IFoo::kInstanceNameToDieFor);
    }

    return service(IFoo::kSomeInstanceName);
}
