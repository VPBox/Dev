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

#ifndef ANDROID_VINTF_RUNTIME_INFO_H
#define ANDROID_VINTF_RUNTIME_INFO_H

#include "Version.h"

#include <map>
#include <string>
#include <vector>

#include <utils/Errors.h>

#include "CheckFlags.h"
#include "KernelInfo.h"
#include "MatrixKernel.h"
#include "Version.h"

namespace android {
namespace vintf {

struct CompatibilityMatrix;

// Runtime Info sent to OTA server
struct RuntimeInfo {

    RuntimeInfo() {}
    virtual ~RuntimeInfo() = default;

    // /proc/version
    // utsname.sysname
    const std::string &osName() const;
    // utsname.nodename
    const std::string &nodeName() const;
    // utsname.release
    const std::string &osRelease() const;
    // utsname.version
    const std::string &osVersion() const;
    // utsname.machine
    const std::string &hardwareId() const;
    // extract from utsname.release
    const KernelVersion &kernelVersion() const;

    const std::map<std::string, std::string> &kernelConfigs() const;

    const Version &bootVbmetaAvbVersion() const;
    const Version &bootAvbVersion() const;

    // /proc/cpuinfo
    const std::string &cpuInfo() const;

    // /sys/fs/selinux/policyvers
    size_t kernelSepolicyVersion() const;

    // Return whether this RuntimeInfo works with the given compatibility matrix. Return true if:
    // - mat is a framework compat-mat
    // - sepolicy.kernel-sepolicy-version == kernelSepolicyVersion()
    // - /proc/config.gz matches the requirements. Note that /proc/config.gz is read when the
    //   RuntimeInfo object is created (the first time VintfObject::GetRuntimeInfo is called),
    //   not when RuntimeInfo::checkCompatibility is called.
    // - avb-vbmetaversion matches related sysprops
    bool checkCompatibility(const CompatibilityMatrix& mat, std::string* error = nullptr,
                            CheckFlags::Type flags = CheckFlags::DEFAULT) const;

    using FetchFlags = uint32_t;
    enum FetchFlag : FetchFlags {
        CPU_VERSION     = 1 << 0,
        CONFIG_GZ       = 1 << 1,
        CPU_INFO        = 1 << 2,
        POLICYVERS      = 1 << 3,
        AVB             = 1 << 4,
        LAST_PLUS_ONE,

        NONE = 0,
        ALL = ((LAST_PLUS_ONE - 1) << 1) - 1,
    };

   protected:
    virtual status_t fetchAllInformation(FetchFlags flags);

    friend struct RuntimeInfoFetcher;
    friend class VintfObject;
    friend struct LibVintfTest;
    friend std::string dump(const RuntimeInfo& ki, bool);

    // /proc/config.gz
    // Key: CONFIG_xxx; Value: the value after = sign.
    KernelInfo mKernel;
    std::string mOsName;
    std::string mNodeName;
    std::string mOsRelease;
    std::string mOsVersion;
    std::string mHardwareId;

    std::vector<std::string> mSepolicyFilePaths;
    std::string mCpuInfo;
    Version mBootVbmetaAvbVersion;
    Version mBootAvbVersion;

    size_t mKernelSepolicyVersion = 0u;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_RUNTIME_INFO_H
