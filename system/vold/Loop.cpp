/*
 * Copyright (C) 2008 The Android Open Source Project
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

#define ATRACE_TAG ATRACE_TAG_PACKAGE_MANAGER

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <linux/kdev_t.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <utils/Trace.h>

#include "Loop.h"
#include "VoldUtil.h"
#include "sehandle.h"

using android::base::StringPrintf;
using android::base::unique_fd;

static const char* kVoldPrefix = "vold:";
static constexpr size_t kLoopDeviceRetryAttempts = 3u;

int Loop::create(const std::string& target, std::string& out_device) {
    unique_fd ctl_fd(open("/dev/loop-control", O_RDWR | O_CLOEXEC));
    if (ctl_fd.get() == -1) {
        PLOG(ERROR) << "Failed to open loop-control";
        return -errno;
    }

    int num = ioctl(ctl_fd.get(), LOOP_CTL_GET_FREE);
    if (num == -1) {
        PLOG(ERROR) << "Failed LOOP_CTL_GET_FREE";
        return -errno;
    }

    out_device = StringPrintf("/dev/block/loop%d", num);

    unique_fd target_fd;
    for (size_t i = 0; i != kLoopDeviceRetryAttempts; ++i) {
        target_fd.reset(open(target.c_str(), O_RDWR | O_CLOEXEC));
        if (target_fd.get() != -1) {
            break;
        }
        usleep(50000);
    }
    if (target_fd.get() == -1) {
        PLOG(ERROR) << "Failed to open " << target;
        return -errno;
    }
    unique_fd device_fd(open(out_device.c_str(), O_RDWR | O_CLOEXEC));
    if (device_fd.get() == -1) {
        PLOG(ERROR) << "Failed to open " << out_device;
        return -errno;
    }

    if (ioctl(device_fd.get(), LOOP_SET_FD, target_fd.get()) == -1) {
        PLOG(ERROR) << "Failed to LOOP_SET_FD";
        return -errno;
    }

    struct loop_info64 li;
    memset(&li, 0, sizeof(li));
    strlcpy((char*)li.lo_crypt_name, kVoldPrefix, LO_NAME_SIZE);
    if (ioctl(device_fd.get(), LOOP_SET_STATUS64, &li) == -1) {
        PLOG(ERROR) << "Failed to LOOP_SET_STATUS64";
        return -errno;
    }

    return 0;
}

int Loop::destroyByDevice(const char* loopDevice) {
    int device_fd;

    device_fd = open(loopDevice, O_RDONLY | O_CLOEXEC);
    if (device_fd < 0) {
        PLOG(ERROR) << "Failed to open " << loopDevice;
        return -1;
    }

    if (ioctl(device_fd, LOOP_CLR_FD, 0) < 0) {
        PLOG(ERROR) << "Failed to destroy " << loopDevice;
        close(device_fd);
        return -1;
    }

    close(device_fd);
    return 0;
}

int Loop::destroyAll() {
    ATRACE_NAME("Loop::destroyAll");

    std::string root = "/dev/block/";
    auto dirp = std::unique_ptr<DIR, int (*)(DIR*)>(opendir(root.c_str()), closedir);
    if (!dirp) {
        PLOG(ERROR) << "Failed to opendir";
        return -1;
    }

    // Poke through all devices looking for loops
    struct dirent* de;
    while ((de = readdir(dirp.get()))) {
        auto test = std::string(de->d_name);
        if (!android::base::StartsWith(test, "loop")) continue;

        auto path = root + de->d_name;
        unique_fd fd(open(path.c_str(), O_RDWR | O_CLOEXEC));
        if (fd.get() == -1) {
            if (errno != ENOENT) {
                PLOG(WARNING) << "Failed to open " << path;
            }
            continue;
        }

        struct loop_info64 li;
        if (ioctl(fd.get(), LOOP_GET_STATUS64, &li) < 0) {
            PLOG(WARNING) << "Failed to LOOP_GET_STATUS64 " << path;
            continue;
        }

        auto id = std::string((char*)li.lo_crypt_name);
        if (android::base::StartsWith(id, kVoldPrefix)) {
            LOG(DEBUG) << "Tearing down stale loop device at " << path << " named " << id;

            if (ioctl(fd.get(), LOOP_CLR_FD, 0) < 0) {
                PLOG(WARNING) << "Failed to LOOP_CLR_FD " << path;
            }
        } else {
            LOG(DEBUG) << "Found unmanaged loop device at " << path << " named " << id;
        }
    }

    return 0;
}

int Loop::createImageFile(const char* file, unsigned long numSectors) {
    unique_fd fd(open(file, O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0600));
    if (fd.get() == -1) {
        PLOG(ERROR) << "Failed to create image " << file;
        return -errno;
    }
    if (fallocate(fd.get(), 0, 0, numSectors * 512) == -1) {
        PLOG(WARNING) << "Failed to fallocate; falling back to ftruncate";
        if (ftruncate(fd, numSectors * 512) == -1) {
            PLOG(ERROR) << "Failed to ftruncate";
            return -errno;
        }
    }
    return 0;
}

int Loop::resizeImageFile(const char* file, unsigned long numSectors) {
    int fd;

    if ((fd = open(file, O_RDWR | O_CLOEXEC)) < 0) {
        PLOG(ERROR) << "Failed to open " << file;
        return -1;
    }

    LOG(DEBUG) << "Attempting to increase " << file << " to " << numSectors;

    if (fallocate(fd, 0, 0, numSectors * 512)) {
        if (errno == ENOSYS || errno == ENOTSUP) {
            PLOG(WARNING) << "fallocate not found. Falling back to ftruncate.";
            if (ftruncate(fd, numSectors * 512) < 0) {
                PLOG(ERROR) << "Failed to ftruncate";
                close(fd);
                return -1;
            }
        } else {
            PLOG(ERROR) << "Failed to fallocate";
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}
