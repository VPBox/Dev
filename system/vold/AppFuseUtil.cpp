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

#include "AppFuseUtil.h"

#include <sys/mount.h>
#include <utils/Errors.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>

#include "Utils.h"

using android::base::StringPrintf;

namespace android {
namespace vold {

namespace {

static size_t kAppFuseMaxMountPointName = 32;

static android::status_t GetMountPath(uid_t uid, const std::string& name, std::string* path) {
    if (name.size() > kAppFuseMaxMountPointName) {
        LOG(ERROR) << "AppFuse mount name is too long.";
        return -EINVAL;
    }
    for (size_t i = 0; i < name.size(); i++) {
        if (!isalnum(name[i])) {
            LOG(ERROR) << "AppFuse mount name contains invalid character.";
            return -EINVAL;
        }
    }
    *path = StringPrintf("/mnt/appfuse/%d_%s", uid, name.c_str());
    return android::OK;
}

static android::status_t Mount(int device_fd, const std::string& path) {
    const auto opts = StringPrintf(
        "fd=%i,"
        "rootmode=40000,"
        "default_permissions,"
        "allow_other,"
        "user_id=0,group_id=0,"
        "context=\"u:object_r:app_fuse_file:s0\","
        "fscontext=u:object_r:app_fusefs:s0",
        device_fd);

    const int result =
        TEMP_FAILURE_RETRY(mount("/dev/fuse", path.c_str(), "fuse",
                                 MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, opts.c_str()));
    if (result != 0) {
        PLOG(ERROR) << "Failed to mount " << path;
        return -errno;
    }

    return android::OK;
}

static android::status_t RunCommand(const std::string& command, uid_t uid, const std::string& path,
                                    int device_fd) {
    if (DEBUG_APPFUSE) {
        LOG(DEBUG) << "Run app fuse command " << command << " for the path " << path << " and uid "
                   << uid;
    }

    if (command == "mount") {
        return Mount(device_fd, path);
    } else if (command == "unmount") {
        // If it's just after all FD opened on mount point are closed, umount2 can fail with
        // EBUSY. To avoid the case, specify MNT_DETACH.
        if (umount2(path.c_str(), UMOUNT_NOFOLLOW | MNT_DETACH) != 0 && errno != EINVAL &&
            errno != ENOENT) {
            PLOG(ERROR) << "Failed to unmount directory.";
            return -errno;
        }
        if (rmdir(path.c_str()) != 0) {
            PLOG(ERROR) << "Failed to remove the mount directory.";
            return -errno;
        }
        return android::OK;
    } else {
        LOG(ERROR) << "Unknown appfuse command " << command;
        return -EPERM;
    }

    return android::OK;
}

}  // namespace

int MountAppFuse(uid_t uid, int mountId, android::base::unique_fd* device_fd) {
    std::string name = std::to_string(mountId);

    // Check mount point name.
    std::string path;
    if (GetMountPath(uid, name, &path) != android::OK) {
        LOG(ERROR) << "Invalid mount point name";
        return -1;
    }

    // Forcibly remove the existing mount before we attempt to prepare the
    // directory. If we have a dangling mount, then PrepareDir may fail if the
    // indirection to FUSE doesn't work.
    android::vold::ForceUnmount(path);

    // Create directories.
    const android::status_t result = android::vold::PrepareDir(path, 0700, 0, 0);
    if (result != android::OK) {
        PLOG(ERROR) << "Failed to prepare directory " << path;
        return -1;
    }

    // Open device FD.
    // NOLINTNEXTLINE(android-cloexec-open): Deliberately not O_CLOEXEC
    device_fd->reset(open("/dev/fuse", O_RDWR));
    if (device_fd->get() == -1) {
        PLOG(ERROR) << "Failed to open /dev/fuse";
        return -1;
    }

    // Mount.
    return RunCommand("mount", uid, path, device_fd->get());
}

int UnmountAppFuse(uid_t uid, int mountId) {
    std::string name = std::to_string(mountId);

    // Check mount point name.
    std::string path;
    if (GetMountPath(uid, name, &path) != android::OK) {
        LOG(ERROR) << "Invalid mount point name";
        return -1;
    }

    return RunCommand("unmount", uid, path, -1 /* device_fd */);
}

int OpenAppFuseFile(uid_t uid, int mountId, int fileId, int flags) {
    std::string name = std::to_string(mountId);

    // Check mount point name.
    std::string mountPoint;
    if (GetMountPath(uid, name, &mountPoint) != android::OK) {
        LOG(ERROR) << "Invalid mount point name";
        return -1;
    }

    std::string path = StringPrintf("%s/%d", mountPoint.c_str(), fileId);
    return TEMP_FAILURE_RETRY(open(path.c_str(), flags));
}

}  // namespace vold
}  // namespace android
