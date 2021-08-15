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


#define LOG_TAG "libvintf"
#include <android-base/logging.h>

#include "RuntimeInfo.h"

#include "CompatibilityMatrix.h"
#include "KernelConfigParser.h"
#include "parse_string.h"

#include <dirent.h>
#include <errno.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <android-base/properties.h>
#include <selinux/selinux.h>
#include <zlib.h>

#define PROC_CONFIG "/proc/config.gz"
#define BUFFER_SIZE sysconf(_SC_PAGESIZE)

namespace android {
namespace vintf {

struct RuntimeInfoFetcher {
    RuntimeInfoFetcher(RuntimeInfo *ki) : mRuntimeInfo(ki) { }
    status_t fetchAllInformation(RuntimeInfo::FetchFlags flags);

   private:
    status_t fetchVersion();
    status_t fetchKernelConfigs();
    status_t fetchCpuInfo();
    status_t fetchKernelSepolicyVers();
    status_t fetchAvb();
    status_t parseKernelVersion();
    RuntimeInfo *mRuntimeInfo;
    KernelConfigParser mConfigParser;
};

// decompress /proc/config.gz and read its contents.
status_t RuntimeInfoFetcher::fetchKernelConfigs() {
    gzFile f = gzopen(PROC_CONFIG, "rb");
    if (f == NULL) {
        LOG(ERROR) << "Could not open /proc/config.gz: " << errno;
        return -errno;
    }

    char buf[BUFFER_SIZE];
    int len;
    while ((len = gzread(f, buf, sizeof buf)) > 0) {
        mConfigParser.process(buf, len);
    }
    status_t err = OK;
    if (len < 0) {
        int errnum;
        const char *errmsg = gzerror(f, &errnum);
        LOG(ERROR) << "Could not read /proc/config.gz: " << errmsg;
        err = (errnum == Z_ERRNO ? -errno : errnum);
    }
    mConfigParser.finish();
    gzclose(f);
    mRuntimeInfo->mKernel.mConfigs = std::move(mConfigParser.configs());
    return err;
}

status_t RuntimeInfoFetcher::fetchCpuInfo() {
    // TODO implement this; 32-bit and 64-bit has different format.
    std::ifstream in{"/proc/cpuinfo"};
    if (!in.is_open()) {
        LOG(WARNING) << "Cannot read /proc/cpuinfo";
        return UNKNOWN_ERROR;
    }
    std::stringstream sstream;
    sstream << in.rdbuf();
    mRuntimeInfo->mCpuInfo = sstream.str();
    return OK;
}

status_t RuntimeInfoFetcher::fetchKernelSepolicyVers() {
    int pv;
#ifdef LIBVINTF_TARGET
    pv = security_policyvers();
#else
    pv = 0;
#endif
    if (pv < 0) {
        return pv;
    }
    mRuntimeInfo->mKernelSepolicyVersion = pv;
    return OK;
}

status_t RuntimeInfoFetcher::fetchVersion() {
    struct utsname buf;
    if (uname(&buf)) {
        return -errno;
    }
    mRuntimeInfo->mOsName = buf.sysname;
    mRuntimeInfo->mNodeName = buf.nodename;
    mRuntimeInfo->mOsRelease = buf.release;
    mRuntimeInfo->mOsVersion = buf.version;
    mRuntimeInfo->mHardwareId = buf.machine;

    status_t err = parseKernelVersion();
    if (err != OK) {
        LOG(ERROR) << "Could not parse kernel version from \""
                   << mRuntimeInfo->mOsRelease << "\"";
    }
    return err;
}

status_t RuntimeInfoFetcher::parseKernelVersion() {
    auto pos = mRuntimeInfo->mOsRelease.find('.');
    if (pos == std::string::npos) {
        return UNKNOWN_ERROR;
    }
    pos = mRuntimeInfo->mOsRelease.find('.', pos + 1);
    if (pos == std::string::npos) {
        return UNKNOWN_ERROR;
    }
    pos = mRuntimeInfo->mOsRelease.find_first_not_of("0123456789", pos + 1);
    // no need to check pos == std::string::npos, because substr will handle this
    if (!parse(mRuntimeInfo->mOsRelease.substr(0, pos), &mRuntimeInfo->mKernel.mVersion)) {
        return UNKNOWN_ERROR;
    }
    return OK;
}

status_t RuntimeInfoFetcher::fetchAvb() {
    std::string prop = android::base::GetProperty("ro.boot.vbmeta.avb_version", "0.0");
    if (!parse(prop, &mRuntimeInfo->mBootVbmetaAvbVersion)) {
        return UNKNOWN_ERROR;
    }
    prop = android::base::GetProperty("ro.boot.avb_version", "0.0");
    if (!parse(prop, &mRuntimeInfo->mBootAvbVersion)) {
        return UNKNOWN_ERROR;
    }
    return OK;
}

status_t RuntimeInfoFetcher::fetchAllInformation(RuntimeInfo::FetchFlags flags) {

    using F = RuntimeInfo::FetchFlag;
    using RF = RuntimeInfoFetcher;
    using FetchFunction = status_t(RF::*)();
    const static std::vector<std::tuple<F, FetchFunction, std::string>> gFetchFunctions({
        // flag          fetch function                 description
        {F::CPU_VERSION, &RF::fetchVersion,             "/proc/version"},
        {F::CONFIG_GZ,   &RF::fetchKernelConfigs,       "/proc/config.gz"},
        {F::CPU_INFO,    &RF::fetchCpuInfo,             "/proc/cpuinfo"},
        {F::POLICYVERS,  &RF::fetchKernelSepolicyVers,  "kernel sepolicy version"},
        {F::AVB,         &RF::fetchAvb,                 "avb version"},
    });

    status_t err;
    for (const auto& tuple : gFetchFunctions)
        if ((flags & std::get<0>(tuple)) && (err = (*this.*std::get<1>(tuple))()) != OK)
            LOG(WARNING) << "Cannot fetch or parse " << std::get<2>(tuple) << ": "
                         << strerror(-err);

    return OK;
}

status_t RuntimeInfo::fetchAllInformation(RuntimeInfo::FetchFlags flags) {
    return RuntimeInfoFetcher(this).fetchAllInformation(flags);
}

} // namespace vintf
} // namespace android
