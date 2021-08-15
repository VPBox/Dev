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

#include "FileDeviceUtils.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/fiemap.h>
#include <linux/fs.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>

namespace {

std::unique_ptr<struct fiemap> alloc_fiemap(uint32_t extent_count);

}

namespace android {
namespace vold {

// Given a file path, look for the corresponding block device in /proc/mount
std::string BlockDeviceForPath(const std::string& path) {
    std::unique_ptr<FILE, int (*)(FILE*)> mnts(setmntent("/proc/mounts", "re"), endmntent);
    if (!mnts) {
        PLOG(ERROR) << "Unable to open /proc/mounts";
        return "";
    }
    std::string result;
    size_t best_length = 0;
    struct mntent* mnt;  // getmntent returns a thread local, so it's safe.
    while ((mnt = getmntent(mnts.get())) != nullptr) {
        auto l = strlen(mnt->mnt_dir);
        if (l > best_length && path.size() > l && path[l] == '/' &&
            path.compare(0, l, mnt->mnt_dir) == 0) {
            result = mnt->mnt_fsname;
            best_length = l;
        }
    }
    if (result.empty()) {
        LOG(ERROR) << "Didn't find a mountpoint to match path " << path;
        return "";
    }
    return result;
}

std::unique_ptr<struct fiemap> PathFiemap(const std::string& path, uint32_t extent_count) {
    android::base::unique_fd fd(TEMP_FAILURE_RETRY(open(path.c_str(), O_RDONLY | O_CLOEXEC, 0)));
    if (fd == -1) {
        if (errno == ENOENT) {
            PLOG(DEBUG) << "Unable to open " << path;
        } else {
            PLOG(ERROR) << "Unable to open " << path;
        }
        return nullptr;
    }
    auto fiemap = alloc_fiemap(extent_count);
    if (ioctl(fd.get(), FS_IOC_FIEMAP, fiemap.get()) != 0) {
        PLOG(ERROR) << "Unable to FIEMAP " << path;
        return nullptr;
    }
    auto mapped = fiemap->fm_mapped_extents;
    if (mapped < 1 || mapped > extent_count) {
        LOG(ERROR) << "Extent count not in bounds 1 <= " << mapped << " <= " << extent_count
                   << " in " << path;
        return nullptr;
    }
    return fiemap;
}

}  // namespace vold
}  // namespace android

namespace {

std::unique_ptr<struct fiemap> alloc_fiemap(uint32_t extent_count) {
    size_t allocsize = offsetof(struct fiemap, fm_extents[extent_count]);
    std::unique_ptr<struct fiemap> res(new (::operator new(allocsize)) struct fiemap);
    memset(res.get(), 0, allocsize);
    res->fm_start = 0;
    res->fm_length = UINT64_MAX;
    res->fm_flags = 0;
    res->fm_extent_count = extent_count;
    res->fm_mapped_extents = 0;
    return res;
}

}  // namespace
