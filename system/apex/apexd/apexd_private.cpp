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

#define LOG_TAG "apexd"

#include "apexd_private.h"

#include <sys/mount.h>
#include <sys/stat.h>

#include <android-base/logging.h>
#include <android-base/macros.h>

#include "string_log.h"

namespace android {
namespace apex {
namespace apexd_private {

Status BindMount(const std::string& target, const std::string& source) {
  LOG(VERBOSE) << "Creating bind-mount for " << target << " for " << source;
  // Ensure the directory exists, try to unmount.
  {
    bool exists;
    bool is_dir;
    {
      struct stat buf;
      if (stat(target.c_str(), &buf) != 0) {
        if (errno == ENOENT) {
          exists = false;
          is_dir = false;
        } else {
          PLOG(ERROR) << "Could not stat target directory " << target;
          // Still attempt to bind-mount.
          exists = true;
          is_dir = true;
        }
      } else {
        exists = true;
        is_dir = S_ISDIR(buf.st_mode);
      }
    }

    // Ensure that it is a folder.
    if (exists && !is_dir) {
      LOG(WARNING) << target << " is not a directory, attempting to fix";
      if (unlink(target.c_str()) != 0) {
        PLOG(ERROR) << "Failed to unlink " << target;
        // Try mkdir, anyways.
      }
      exists = false;
    }
    // And create it if necessary.
    if (!exists) {
      LOG(VERBOSE) << "Creating mountpoint " << target;
      if (mkdir(target.c_str(), kMkdirMode) != 0) {
        return Status::Fail(PStringLog()
                            << "Could not create mountpoint " << target);
      }
    };
    // Unmount any active bind-mount.
    if (exists) {
      int rc = umount2(target.c_str(), UMOUNT_NOFOLLOW | MNT_DETACH);
      if (rc != 0 && errno != EINVAL) {
        // Log error but ignore.
        PLOG(ERROR) << "Could not unmount " << target;
      }
    }
  }

  LOG(VERBOSE) << "Bind-mounting " << source << " to " << target;
  if (mount(source.c_str(), target.c_str(), nullptr, MS_BIND, nullptr) == 0) {
    return Status::Success();
  }
  return Status::Fail(PStringLog()
                      << "Could not bind-mount " << source << " to " << target);
}

}  // namespace apexd_private
}  // namespace apex
}  // namespace android
