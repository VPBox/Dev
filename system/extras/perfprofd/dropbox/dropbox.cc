/*
 *
 * Copyright 2017, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dropbox.h"

#include <cstdio>
#include <cstdlib>
#include <memory>

#include <inttypes.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>
#include <android/os/DropBoxManager.h>
#include <binder/Status.h>
#include <utils/String8.h>

#include "perfprofd_record.pb.h"

#include "perfprofd_io.h"

namespace android {
namespace perfprofd {
namespace dropbox {

namespace {

bool WriteDropboxFile(android::perfprofd::PerfprofdRecord* encodedProfile,
                      const std::string& temp_dir,
                      std::string* error_msg) {
  android::base::unique_fd tmp_fd;
  {
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/dropboxtmp-XXXXXX", temp_dir.c_str());
    tmp_fd.reset(mkstemp(path));
    if (tmp_fd.get() == -1) {
      *error_msg = android::base::StringPrintf("Could not create temp file %s: %s",
                                               path,
                                               strerror(errno));
      return false;
    }
    if (unlink(path) != 0) {
      PLOG(WARNING) << "Could not unlink binder temp file";
    }
  }

  // Dropbox takes ownership of the fd, and if it is not readonly,
  // a selinux violation will occur. Get a read-only version.
  android::base::unique_fd read_only;
  {
    char fdpath[64];
    snprintf(fdpath, arraysize(fdpath), "/proc/self/fd/%d", tmp_fd.get());
    read_only.reset(open(fdpath, O_RDONLY | O_CLOEXEC));
    if (read_only.get() < 0) {
      *error_msg = android::base::StringPrintf("Could not create read-only fd: %s",
                                               strerror(errno));
      return false;
    }
  }

  constexpr bool kCompress = true;  // Ignore the config here. Dropbox will always end up
                                    // compressing the data, might as well make the temp
                                    // file smaller and help it out.
  using DropBoxManager = android::os::DropBoxManager;
  constexpr int kDropboxFlags = DropBoxManager::IS_GZIPPED;

  if (!SerializeProtobuf(encodedProfile, std::move(tmp_fd), kCompress)) {
    *error_msg = "Could not serialize to temp file";
    return false;
  }

  sp<DropBoxManager> dropbox(new DropBoxManager());
  android::binder::Status status =  dropbox->addFile(String16("perfprofd"),
                                                     read_only.release(),
                                                     kDropboxFlags);
  if (!status.isOk()) {
    *error_msg = status.toString8();
    return false;
  }
  return true;
}

}  // namespace

bool SendToDropbox(android::perfprofd::PerfprofdRecord* profile,
                   const std::string& temp_directory,
                   std::string* error_msg) {
  size_t size = profile->ByteSize();
  if (size < 1024 * 1024) {
    // For a small size, send as a byte buffer directly.
    std::unique_ptr<uint8_t[]> data(new uint8_t[size]);
    profile->SerializeWithCachedSizesToArray(data.get());

    using DropBoxManager = android::os::DropBoxManager;
    sp<DropBoxManager> dropbox(new DropBoxManager());
    android::binder::Status status = dropbox->addData(String16("perfprofd"),
                                                      data.get(),
                                                      size,
                                                      0);
    if (!status.isOk()) {
      *error_msg = status.toString8();
      return false;
    }
    return true;
  } else {
    // For larger buffers, we need to go through the filesystem.
    return WriteDropboxFile(profile, temp_directory, error_msg);
  }
}

}  // namespace dropbox
}  // namespace perfprofd
}  // namespace android
