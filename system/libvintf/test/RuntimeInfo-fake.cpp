/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "utils-fake.h"

namespace android {
namespace vintf {
namespace details {
status_t MockRuntimeInfo::doFetch(RuntimeInfo::FetchFlags) {
    if (failNextFetch_) {
        failNextFetch_ = false;
        return android::UNKNOWN_ERROR;
    }
    mOsName = "Linux";
    mNodeName = "localhost";
    mOsRelease = "3.18.31-g936f9a479d0f";
    mOsVersion = "#4 SMP PREEMPT Wed Feb 1 18:10:52 PST 2017";
    mHardwareId = "aarch64";
    mKernelSepolicyVersion = 30;
    mKernel.mVersion = {3, 18, 31};
    mKernel.mConfigs = {{"CONFIG_64BIT", "y"},
                        {"CONFIG_ANDROID_BINDER_DEVICES", "\"binder,hwbinder\""},
                        {"CONFIG_ARCH_MMAP_RND_BITS", "24"},
                        {"CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE_NAMES", "\"\""},
                        {"CONFIG_ILLEGAL_POINTER_VALUE", "0xdead000000000000"}};

    return OK;
}

}  // namespace details
}  // namespace vintf
}  // namespace android
