/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <memory>
#include <string>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <linux/fiemap.h>
#include <linux/fs.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/logging.h>
#include <android-base/unique_fd.h>

#include "FileDeviceUtils.h"

namespace {

struct Options {
    std::vector<std::string> targets;
    bool unlink{true};
};

constexpr uint32_t max_extents = 32;

bool read_command_line(int argc, const char* const argv[], Options& options);
void usage(const char* progname);
bool secdiscard_path(const std::string& path);
bool check_fiemap(const struct fiemap& fiemap, const std::string& path);
bool overwrite_with_zeros(int fd, off64_t start, off64_t length);

}  // namespace

int main(int argc, const char* const argv[]) {
    android::base::InitLogging(const_cast<char**>(argv));
    Options options;
    if (!read_command_line(argc, argv, options)) {
        usage(argv[0]);
        return -1;
    }

    for (auto const& target : options.targets) {
// F2FS-specific ioctl
// It requires the below kernel commit merged in v4.16-rc1.
//   1ad71a27124c ("f2fs: add an ioctl to disable GC for specific file")
// In android-4.4,
//   56ee1e817908 ("f2fs: updates on v4.16-rc1")
// In android-4.9,
//   2f17e34672a8 ("f2fs: updates on v4.16-rc1")
// In android-4.14,
//   ce767d9a55bc ("f2fs: updates on v4.16-rc1")
#ifndef F2FS_IOC_SET_PIN_FILE
#ifndef F2FS_IOCTL_MAGIC
#define F2FS_IOCTL_MAGIC 0xf5
#endif
#define F2FS_IOC_SET_PIN_FILE _IOW(F2FS_IOCTL_MAGIC, 13, __u32)
#define F2FS_IOC_GET_PIN_FILE _IOR(F2FS_IOCTL_MAGIC, 14, __u32)
#endif
        android::base::unique_fd fd(
            TEMP_FAILURE_RETRY(open(target.c_str(), O_WRONLY | O_CLOEXEC, 0)));
        if (fd == -1) {
            LOG(ERROR) << "Secure discard open failed for: " << target;
            return 0;
        }
        __u32 set = 1;
        ioctl(fd, F2FS_IOC_SET_PIN_FILE, &set);

        LOG(DEBUG) << "Securely discarding '" << target << "' unlink=" << options.unlink;
        if (!secdiscard_path(target)) {
            LOG(ERROR) << "Secure discard failed for: " << target;
        }
        if (options.unlink) {
            if (unlink(target.c_str()) != 0 && errno != ENOENT) {
                PLOG(ERROR) << "Unable to unlink: " << target;
            }
        }
        set = 0;
        ioctl(fd, F2FS_IOC_SET_PIN_FILE, &set);
    }
    return 0;
}

namespace {

bool read_command_line(int argc, const char* const argv[], Options& options) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp("--no-unlink", argv[i])) {
            options.unlink = false;
        } else if (!strcmp("--", argv[i])) {
            for (int j = i + 1; j < argc; j++) {
                if (argv[j][0] != '/') return false;  // Must be absolute path
                options.targets.emplace_back(argv[j]);
            }
            return options.targets.size() > 0;
        } else {
            return false;  // Unknown option
        }
    }
    return false;  // "--" not found
}

void usage(const char* progname) {
    fprintf(stderr, "Usage: %s [--no-unlink] -- <absolute path> ...\n", progname);
}

// BLKSECDISCARD all content in "path", if it's small enough.
bool secdiscard_path(const std::string& path) {
    auto fiemap = android::vold::PathFiemap(path, max_extents);
    if (!fiemap || !check_fiemap(*fiemap, path)) {
        return false;
    }
    auto block_device = android::vold::BlockDeviceForPath(path);
    if (block_device.empty()) {
        return false;
    }
    android::base::unique_fd fs_fd(
        TEMP_FAILURE_RETRY(open(block_device.c_str(), O_RDWR | O_LARGEFILE | O_CLOEXEC, 0)));
    if (fs_fd == -1) {
        PLOG(ERROR) << "Failed to open device " << block_device;
        return false;
    }
    for (uint32_t i = 0; i < fiemap->fm_mapped_extents; i++) {
        uint64_t range[2];
        range[0] = fiemap->fm_extents[i].fe_physical;
        range[1] = fiemap->fm_extents[i].fe_length;
        if (ioctl(fs_fd.get(), BLKSECDISCARD, range) == -1) {
            // Use zero overwrite as a fallback for BLKSECDISCARD
            if (!overwrite_with_zeros(fs_fd.get(), range[0], range[1])) return false;
        }
    }
    // Should wait for overwrites completion. Otherwise after unlink(),
    // filesystem can allocate these blocks and IO can be reordered, resulting
    // in making zero blocks to filesystem blocks.
    fsync(fs_fd.get());
    return true;
}

// Ensure that the FIEMAP covers the file and is OK to discard
bool check_fiemap(const struct fiemap& fiemap, const std::string& path) {
    auto mapped = fiemap.fm_mapped_extents;
    if (!(fiemap.fm_extents[mapped - 1].fe_flags & FIEMAP_EXTENT_LAST)) {
        LOG(ERROR) << "Extent " << mapped - 1 << " was not the last in " << path;
        return false;
    }
    for (uint32_t i = 0; i < mapped; i++) {
        auto flags = fiemap.fm_extents[i].fe_flags;
        if (flags & (FIEMAP_EXTENT_UNKNOWN | FIEMAP_EXTENT_DELALLOC | FIEMAP_EXTENT_NOT_ALIGNED)) {
            LOG(ERROR) << "Extent " << i << " has unexpected flags " << flags << ": " << path;
            return false;
        }
    }
    return true;
}

bool overwrite_with_zeros(int fd, off64_t start, off64_t length) {
    if (lseek64(fd, start, SEEK_SET) != start) {
        PLOG(ERROR) << "Seek failed for zero overwrite";
        return false;
    }
    char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    while (length > 0) {
        size_t wlen = static_cast<size_t>(std::min(static_cast<off64_t>(sizeof(buf)), length));
        auto written = write(fd, buf, wlen);
        if (written < 1) {
            PLOG(ERROR) << "Write of zeroes failed";
            return false;
        }
        length -= written;
    }
    return true;
}

}  // namespace
