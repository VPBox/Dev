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

#include "F2fs.h"
#include "Utils.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <fscrypt/fscrypt.h>

#include <string>
#include <vector>

#include <sys/mount.h>

using android::base::StringPrintf;

namespace android {
namespace vold {
namespace f2fs {

static const char* kMkfsPath = "/system/bin/make_f2fs";
static const char* kFsckPath = "/system/bin/fsck.f2fs";

bool IsSupported() {
    return access(kMkfsPath, X_OK) == 0 && access(kFsckPath, X_OK) == 0 &&
           IsFilesystemSupported("f2fs");
}

status_t Check(const std::string& source) {
    std::vector<std::string> cmd;
    cmd.push_back(kFsckPath);
    cmd.push_back("-a");
    cmd.push_back(source);

    // f2fs devices are currently always trusted
    return ForkExecvp(cmd, nullptr, sFsckContext);
}

status_t Mount(const std::string& source, const std::string& target) {
    const char* c_source = source.c_str();
    const char* c_target = target.c_str();
    unsigned long flags = MS_NOATIME | MS_NODEV | MS_NOSUID | MS_DIRSYNC;

    int res = mount(c_source, c_target, "f2fs", flags, NULL);
    if (res != 0) {
        PLOG(ERROR) << "Failed to mount " << source;
        if (errno == EROFS) {
            res = mount(c_source, c_target, "f2fs", flags | MS_RDONLY, NULL);
            if (res != 0) {
                PLOG(ERROR) << "Failed to mount read-only " << source;
            }
        }
    }

    return res;
}

status_t Format(const std::string& source) {
    std::vector<std::string> cmd;
    cmd.push_back(kMkfsPath);

    cmd.push_back("-f");
    cmd.push_back("-d1");

    if (android::base::GetBoolProperty("vold.has_quota", false)) {
        cmd.push_back("-O");
        cmd.push_back("quota");
    }
    if (fscrypt_is_native()) {
        cmd.push_back("-O");
        cmd.push_back("encrypt");
    }

    cmd.push_back("-O");
    cmd.push_back("verity");

    cmd.push_back(source);
    return ForkExecvp(cmd);
}

}  // namespace f2fs
}  // namespace vold
}  // namespace android
