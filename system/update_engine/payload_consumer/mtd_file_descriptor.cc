//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/payload_consumer/mtd_file_descriptor.h"

#include <fcntl.h>
#include <mtd/ubi-user.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <string>

#include <base/files/file_path.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/subprocess.h"
#include "update_engine/common/utils.h"

using std::string;

namespace {

static const char kSysfsClassUbi[] = "/sys/class/ubi/";
static const char kUsableEbSize[] = "/usable_eb_size";
static const char kReservedEbs[] = "/reserved_ebs";

using chromeos_update_engine::UbiVolumeInfo;
using chromeos_update_engine::utils::ReadFile;

// Return a UbiVolumeInfo pointer if |path| is a UBI volume. Otherwise, return
// a null unique pointer.
std::unique_ptr<UbiVolumeInfo> GetUbiVolumeInfo(const string& path) {
  base::FilePath device_node(path);
  base::FilePath ubi_name(device_node.BaseName());

  string sysfs_node(kSysfsClassUbi);
  sysfs_node.append(ubi_name.MaybeAsASCII());

  std::unique_ptr<UbiVolumeInfo> ret;

  // Obtain volume info from sysfs.
  string s_reserved_ebs;
  if (!ReadFile(sysfs_node + kReservedEbs, &s_reserved_ebs)) {
    LOG(ERROR) << "Cannot read " << sysfs_node + kReservedEbs;
    return ret;
  }
  string s_eb_size;
  if (!ReadFile(sysfs_node + kUsableEbSize, &s_eb_size)) {
    LOG(ERROR) << "Cannot read " << sysfs_node + kUsableEbSize;
    return ret;
  }

  base::TrimWhitespaceASCII(
      s_reserved_ebs, base::TRIM_TRAILING, &s_reserved_ebs);
  base::TrimWhitespaceASCII(s_eb_size, base::TRIM_TRAILING, &s_eb_size);

  uint64_t reserved_ebs, eb_size;
  if (!base::StringToUint64(s_reserved_ebs, &reserved_ebs)) {
    LOG(ERROR) << "Cannot parse reserved_ebs: " << s_reserved_ebs;
    return ret;
  }
  if (!base::StringToUint64(s_eb_size, &eb_size)) {
    LOG(ERROR) << "Cannot parse usable_eb_size: " << s_eb_size;
    return ret;
  }

  ret.reset(new UbiVolumeInfo);
  ret->reserved_ebs = reserved_ebs;
  ret->eraseblock_size = eb_size;
  return ret;
}

}  // namespace

namespace chromeos_update_engine {

MtdFileDescriptor::MtdFileDescriptor()
    : read_ctx_(nullptr, &mtd_read_close),
      write_ctx_(nullptr, &mtd_write_close) {}

bool MtdFileDescriptor::IsMtd(const char* path) {
  uint64_t size;
  return mtd_node_info(path, &size, nullptr, nullptr) == 0;
}

bool MtdFileDescriptor::Open(const char* path, int flags, mode_t mode) {
  // This File Descriptor does not support read and write.
  TEST_AND_RETURN_FALSE((flags & O_ACCMODE) != O_RDWR);
  // But we need to open the underlying file descriptor in O_RDWR mode because
  // during write, we need to read back to verify the write actually sticks or
  // we have to skip the block. That job is done by mtdutils library.
  if ((flags & O_ACCMODE) == O_WRONLY) {
    flags &= ~O_ACCMODE;
    flags |= O_RDWR;
  }
  TEST_AND_RETURN_FALSE(
      EintrSafeFileDescriptor::Open(path, flags | O_CLOEXEC, mode));

  if ((flags & O_ACCMODE) == O_RDWR) {
    write_ctx_.reset(mtd_write_descriptor(fd_, path));
    nr_written_ = 0;
  } else {
    read_ctx_.reset(mtd_read_descriptor(fd_, path));
  }

  if (!read_ctx_ && !write_ctx_) {
    Close();
    return false;
  }

  return true;
}

bool MtdFileDescriptor::Open(const char* path, int flags) {
  mode_t cur = umask(022);
  umask(cur);
  return Open(path, flags, 0777 & ~cur);
}

ssize_t MtdFileDescriptor::Read(void* buf, size_t count) {
  CHECK(read_ctx_);
  return mtd_read_data(read_ctx_.get(), static_cast<char*>(buf), count);
}

ssize_t MtdFileDescriptor::Write(const void* buf, size_t count) {
  CHECK(write_ctx_);
  ssize_t result =
      mtd_write_data(write_ctx_.get(), static_cast<const char*>(buf), count);
  if (result > 0) {
    nr_written_ += result;
  }
  return result;
}

off64_t MtdFileDescriptor::Seek(off64_t offset, int whence) {
  if (write_ctx_) {
    // Ignore seek in write mode.
    return nr_written_;
  }
  return EintrSafeFileDescriptor::Seek(offset, whence);
}

bool MtdFileDescriptor::Close() {
  read_ctx_.reset();
  write_ctx_.reset();
  return EintrSafeFileDescriptor::Close();
}

bool UbiFileDescriptor::IsUbi(const char* path) {
  base::FilePath device_node(path);
  base::FilePath ubi_name(device_node.BaseName());
  TEST_AND_RETURN_FALSE(base::StartsWith(
      ubi_name.MaybeAsASCII(), "ubi", base::CompareCase::SENSITIVE));

  return static_cast<bool>(GetUbiVolumeInfo(path));
}

bool UbiFileDescriptor::Open(const char* path, int flags, mode_t mode) {
  std::unique_ptr<UbiVolumeInfo> info = GetUbiVolumeInfo(path);
  if (!info) {
    return false;
  }

  // This File Descriptor does not support read and write.
  TEST_AND_RETURN_FALSE((flags & O_ACCMODE) != O_RDWR);
  TEST_AND_RETURN_FALSE(
      EintrSafeFileDescriptor::Open(path, flags | O_CLOEXEC, mode));

  usable_eb_blocks_ = info->reserved_ebs;
  eraseblock_size_ = info->eraseblock_size;
  volume_size_ = usable_eb_blocks_ * eraseblock_size_;

  if ((flags & O_ACCMODE) == O_WRONLY) {
    // It's best to use volume update ioctl so that UBI layer will mark the
    // volume as being updated, and only clear that mark if the update is
    // successful. We will need to pad to the whole volume size at close.
    uint64_t vsize = volume_size_;
    if (ioctl(fd_, UBI_IOCVOLUP, &vsize) != 0) {
      PLOG(ERROR) << "Cannot issue volume update ioctl";
      EintrSafeFileDescriptor::Close();
      return false;
    }
    mode_ = kWriteOnly;
    nr_written_ = 0;
  } else {
    mode_ = kReadOnly;
  }

  return true;
}

bool UbiFileDescriptor::Open(const char* path, int flags) {
  mode_t cur = umask(022);
  umask(cur);
  return Open(path, flags, 0777 & ~cur);
}

ssize_t UbiFileDescriptor::Read(void* buf, size_t count) {
  CHECK(mode_ == kReadOnly);
  return EintrSafeFileDescriptor::Read(buf, count);
}

ssize_t UbiFileDescriptor::Write(const void* buf, size_t count) {
  CHECK(mode_ == kWriteOnly);
  ssize_t nr_chunk = EintrSafeFileDescriptor::Write(buf, count);
  if (nr_chunk >= 0) {
    nr_written_ += nr_chunk;
  }
  return nr_chunk;
}

off64_t UbiFileDescriptor::Seek(off64_t offset, int whence) {
  if (mode_ == kWriteOnly) {
    // Ignore seek in write mode.
    return nr_written_;
  }
  return EintrSafeFileDescriptor::Seek(offset, whence);
}

bool UbiFileDescriptor::Close() {
  bool pad_ok = true;
  if (IsOpen() && mode_ == kWriteOnly) {
    char buf[1024];
    memset(buf, 0xFF, sizeof(buf));
    while (nr_written_ < volume_size_) {
      // We have written less than the whole volume. In order for us to clear
      // the update marker, we need to fill the rest. It is recommended to fill
      // UBI writes with 0xFF.
      uint64_t to_write = volume_size_ - nr_written_;
      if (to_write > sizeof(buf)) {
        to_write = sizeof(buf);
      }
      ssize_t nr_chunk = EintrSafeFileDescriptor::Write(buf, to_write);
      if (nr_chunk < 0) {
        LOG(ERROR) << "Cannot 0xFF-pad before closing.";
        // There is an error, but we can't really do any meaningful thing here.
        pad_ok = false;
        break;
      }
      nr_written_ += nr_chunk;
    }
  }
  return EintrSafeFileDescriptor::Close() && pad_ok;
}

}  // namespace chromeos_update_engine
