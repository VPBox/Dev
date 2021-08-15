//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common/utils.h"

#include <stdint.h>

#include <dirent.h>
#include <elf.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <utility>
#include <vector>

#include <base/callback.h>
#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/files/scoped_file.h>
#include <base/format_macros.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/posix/eintr_wrapper.h>
#include <base/rand_util.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <brillo/data_encoding.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/subprocess.h"
#include "update_engine/payload_consumer/file_descriptor.h"

using base::Time;
using base::TimeDelta;
using std::min;
using std::numeric_limits;
using std::pair;
using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {

// The following constants control how UnmountFilesystem should retry if
// umount() fails with an errno EBUSY, i.e. retry 5 times over the course of
// one second.
const int kUnmountMaxNumOfRetries = 5;
const int kUnmountRetryIntervalInMicroseconds = 200 * 1000;  // 200 ms

// Number of bytes to read from a file to attempt to detect its contents. Used
// in GetFileFormat.
const int kGetFileFormatMaxHeaderSize = 32;

// The path to the kernel's boot_id.
const char kBootIdPath[] = "/proc/sys/kernel/random/boot_id";

// Return true if |disk_name| is an MTD or a UBI device. Note that this test is
// simply based on the name of the device.
bool IsMtdDeviceName(const string& disk_name) {
  return base::StartsWith(
             disk_name, "/dev/ubi", base::CompareCase::SENSITIVE) ||
         base::StartsWith(disk_name, "/dev/mtd", base::CompareCase::SENSITIVE);
}

// Return the device name for the corresponding partition on a NAND device.
// WARNING: This function returns device names that are not mountable.
string MakeNandPartitionName(int partition_num) {
  switch (partition_num) {
    case 2:
    case 4:
    case 6: {
      return base::StringPrintf("/dev/mtd%d", partition_num);
    }
    default: {
      return base::StringPrintf("/dev/ubi%d_0", partition_num);
    }
  }
}

// Return the device name for the corresponding partition on a NAND device that
// may be mountable (but may not be writable).
string MakeNandPartitionNameForMount(int partition_num) {
  switch (partition_num) {
    case 2:
    case 4:
    case 6: {
      return base::StringPrintf("/dev/mtd%d", partition_num);
    }
    case 3:
    case 5:
    case 7: {
      return base::StringPrintf("/dev/ubiblock%d_0", partition_num);
    }
    default: {
      return base::StringPrintf("/dev/ubi%d_0", partition_num);
    }
  }
}

// If |path| is absolute, or explicit relative to the current working directory,
// leaves it as is. Otherwise, uses the system's temp directory, as defined by
// base::GetTempDir() and prepends it to |path|. On success stores the full
// temporary path in |template_path| and returns true.
bool GetTempName(const string& path, base::FilePath* template_path) {
  if (path[0] == '/' ||
      base::StartsWith(path, "./", base::CompareCase::SENSITIVE) ||
      base::StartsWith(path, "../", base::CompareCase::SENSITIVE)) {
    *template_path = base::FilePath(path);
    return true;
  }

  base::FilePath temp_dir;
#ifdef __ANDROID__
  temp_dir = base::FilePath(constants::kNonVolatileDirectory).Append("tmp");
#else
  TEST_AND_RETURN_FALSE(base::GetTempDir(&temp_dir));
#endif  // __ANDROID__
  if (!base::PathExists(temp_dir))
    TEST_AND_RETURN_FALSE(base::CreateDirectory(temp_dir));
  *template_path = temp_dir.Append(path);
  return true;
}

}  // namespace

namespace utils {

string ParseECVersion(string input_line) {
  base::TrimWhitespaceASCII(input_line, base::TRIM_ALL, &input_line);

  // At this point we want to convert the format key=value pair from mosys to
  // a vector of key value pairs.
  vector<pair<string, string>> kv_pairs;
  if (base::SplitStringIntoKeyValuePairs(input_line, '=', ' ', &kv_pairs)) {
    for (const pair<string, string>& kv_pair : kv_pairs) {
      // Finally match against the fw_verion which may have quotes.
      if (kv_pair.first == "fw_version") {
        string output;
        // Trim any quotes.
        base::TrimString(kv_pair.second, "\"", &output);
        return output;
      }
    }
  }
  LOG(ERROR) << "Unable to parse fwid from ec info.";
  return "";
}

bool WriteFile(const char* path, const void* data, size_t data_len) {
  int fd = HANDLE_EINTR(open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600));
  TEST_AND_RETURN_FALSE_ERRNO(fd >= 0);
  ScopedFdCloser fd_closer(&fd);
  return WriteAll(fd, data, data_len);
}

bool ReadAll(
    int fd, void* buf, size_t count, size_t* out_bytes_read, bool* eof) {
  char* c_buf = static_cast<char*>(buf);
  size_t bytes_read = 0;
  *eof = false;
  while (bytes_read < count) {
    ssize_t rc = HANDLE_EINTR(read(fd, c_buf + bytes_read, count - bytes_read));
    if (rc < 0) {
      // EAGAIN and EWOULDBLOCK are normal return values when there's no more
      // input and we are in non-blocking mode.
      if (errno != EWOULDBLOCK && errno != EAGAIN) {
        PLOG(ERROR) << "Error reading fd " << fd;
        *out_bytes_read = bytes_read;
        return false;
      }
      break;
    } else if (rc == 0) {
      // A value of 0 means that we reached EOF and there is nothing else to
      // read from this fd.
      *eof = true;
      break;
    } else {
      bytes_read += rc;
    }
  }
  *out_bytes_read = bytes_read;
  return true;
}

bool WriteAll(int fd, const void* buf, size_t count) {
  const char* c_buf = static_cast<const char*>(buf);
  ssize_t bytes_written = 0;
  while (bytes_written < static_cast<ssize_t>(count)) {
    ssize_t rc = write(fd, c_buf + bytes_written, count - bytes_written);
    TEST_AND_RETURN_FALSE_ERRNO(rc >= 0);
    bytes_written += rc;
  }
  return true;
}

bool PWriteAll(int fd, const void* buf, size_t count, off_t offset) {
  const char* c_buf = static_cast<const char*>(buf);
  size_t bytes_written = 0;
  int num_attempts = 0;
  while (bytes_written < count) {
    num_attempts++;
    ssize_t rc = pwrite(fd,
                        c_buf + bytes_written,
                        count - bytes_written,
                        offset + bytes_written);
    // TODO(garnold) for debugging failure in chromium-os:31077; to be removed.
    if (rc < 0) {
      PLOG(ERROR) << "pwrite error; num_attempts=" << num_attempts
                  << " bytes_written=" << bytes_written << " count=" << count
                  << " offset=" << offset;
    }
    TEST_AND_RETURN_FALSE_ERRNO(rc >= 0);
    bytes_written += rc;
  }
  return true;
}

bool WriteAll(const FileDescriptorPtr& fd, const void* buf, size_t count) {
  const char* c_buf = static_cast<const char*>(buf);
  ssize_t bytes_written = 0;
  while (bytes_written < static_cast<ssize_t>(count)) {
    ssize_t rc = fd->Write(c_buf + bytes_written, count - bytes_written);
    TEST_AND_RETURN_FALSE_ERRNO(rc >= 0);
    bytes_written += rc;
  }
  return true;
}

bool PWriteAll(const FileDescriptorPtr& fd,
               const void* buf,
               size_t count,
               off_t offset) {
  TEST_AND_RETURN_FALSE_ERRNO(fd->Seek(offset, SEEK_SET) !=
                              static_cast<off_t>(-1));
  return WriteAll(fd, buf, count);
}

bool PReadAll(
    int fd, void* buf, size_t count, off_t offset, ssize_t* out_bytes_read) {
  char* c_buf = static_cast<char*>(buf);
  ssize_t bytes_read = 0;
  while (bytes_read < static_cast<ssize_t>(count)) {
    ssize_t rc =
        pread(fd, c_buf + bytes_read, count - bytes_read, offset + bytes_read);
    TEST_AND_RETURN_FALSE_ERRNO(rc >= 0);
    if (rc == 0) {
      break;
    }
    bytes_read += rc;
  }
  *out_bytes_read = bytes_read;
  return true;
}

bool PReadAll(const FileDescriptorPtr& fd,
              void* buf,
              size_t count,
              off_t offset,
              ssize_t* out_bytes_read) {
  TEST_AND_RETURN_FALSE_ERRNO(fd->Seek(offset, SEEK_SET) !=
                              static_cast<off_t>(-1));
  char* c_buf = static_cast<char*>(buf);
  ssize_t bytes_read = 0;
  while (bytes_read < static_cast<ssize_t>(count)) {
    ssize_t rc = fd->Read(c_buf + bytes_read, count - bytes_read);
    TEST_AND_RETURN_FALSE_ERRNO(rc >= 0);
    if (rc == 0) {
      break;
    }
    bytes_read += rc;
  }
  *out_bytes_read = bytes_read;
  return true;
}

// Append |nbytes| of content from |buf| to the vector pointed to by either
// |vec_p| or |str_p|.
static void AppendBytes(const uint8_t* buf,
                        size_t nbytes,
                        brillo::Blob* vec_p) {
  CHECK(buf);
  CHECK(vec_p);
  vec_p->insert(vec_p->end(), buf, buf + nbytes);
}
static void AppendBytes(const uint8_t* buf, size_t nbytes, string* str_p) {
  CHECK(buf);
  CHECK(str_p);
  str_p->append(buf, buf + nbytes);
}

// Reads from an open file |fp|, appending the read content to the container
// pointer to by |out_p|.  Returns true upon successful reading all of the
// file's content, false otherwise. If |size| is not -1, reads up to |size|
// bytes.
template <class T>
static bool Read(FILE* fp, off_t size, T* out_p) {
  CHECK(fp);
  CHECK(size == -1 || size >= 0);
  uint8_t buf[1024];
  while (size == -1 || size > 0) {
    off_t bytes_to_read = sizeof(buf);
    if (size > 0 && bytes_to_read > size) {
      bytes_to_read = size;
    }
    size_t nbytes = fread(buf, 1, bytes_to_read, fp);
    if (!nbytes) {
      break;
    }
    AppendBytes(buf, nbytes, out_p);
    if (size != -1) {
      CHECK(size >= static_cast<off_t>(nbytes));
      size -= nbytes;
    }
  }
  if (ferror(fp)) {
    return false;
  }
  return size == 0 || feof(fp);
}

// Opens a file |path| for reading and appends its the contents to a container
// |out_p|. Starts reading the file from |offset|. If |offset| is beyond the end
// of the file, returns success. If |size| is not -1, reads up to |size| bytes.
template <class T>
static bool ReadFileChunkAndAppend(const string& path,
                                   off_t offset,
                                   off_t size,
                                   T* out_p) {
  CHECK_GE(offset, 0);
  CHECK(size == -1 || size >= 0);
  base::ScopedFILE fp(fopen(path.c_str(), "r"));
  if (!fp.get())
    return false;
  if (offset) {
    // Return success without appending any data if a chunk beyond the end of
    // the file is requested.
    if (offset >= FileSize(path)) {
      return true;
    }
    TEST_AND_RETURN_FALSE_ERRNO(fseek(fp.get(), offset, SEEK_SET) == 0);
  }
  return Read(fp.get(), size, out_p);
}

// TODO(deymo): This is only used in unittest, but requires the private
// Read<string>() defined here. Expose Read<string>() or move to base/ version.
bool ReadPipe(const string& cmd, string* out_p) {
  FILE* fp = popen(cmd.c_str(), "r");
  if (!fp)
    return false;
  bool success = Read(fp, -1, out_p);
  return (success && pclose(fp) >= 0);
}

bool ReadFile(const string& path, brillo::Blob* out_p) {
  return ReadFileChunkAndAppend(path, 0, -1, out_p);
}

bool ReadFile(const string& path, string* out_p) {
  return ReadFileChunkAndAppend(path, 0, -1, out_p);
}

bool ReadFileChunk(const string& path,
                   off_t offset,
                   off_t size,
                   brillo::Blob* out_p) {
  return ReadFileChunkAndAppend(path, offset, size, out_p);
}

off_t BlockDevSize(int fd) {
  uint64_t dev_size;
  int rc = ioctl(fd, BLKGETSIZE64, &dev_size);
  if (rc == -1) {
    dev_size = -1;
    PLOG(ERROR) << "Error running ioctl(BLKGETSIZE64) on " << fd;
  }
  return dev_size;
}

off_t FileSize(int fd) {
  struct stat stbuf;
  int rc = fstat(fd, &stbuf);
  CHECK_EQ(rc, 0);
  if (rc < 0) {
    PLOG(ERROR) << "Error stat-ing " << fd;
    return rc;
  }
  if (S_ISREG(stbuf.st_mode))
    return stbuf.st_size;
  if (S_ISBLK(stbuf.st_mode))
    return BlockDevSize(fd);
  LOG(ERROR) << "Couldn't determine the type of " << fd;
  return -1;
}

off_t FileSize(const string& path) {
  int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    PLOG(ERROR) << "Error opening " << path;
    return fd;
  }
  off_t size = FileSize(fd);
  if (size == -1)
    PLOG(ERROR) << "Error getting file size of " << path;
  close(fd);
  return size;
}

void HexDumpArray(const uint8_t* const arr, const size_t length) {
  LOG(INFO) << "Logging array of length: " << length;
  const unsigned int bytes_per_line = 16;
  for (uint32_t i = 0; i < length; i += bytes_per_line) {
    const unsigned int bytes_remaining = length - i;
    const unsigned int bytes_per_this_line =
        min(bytes_per_line, bytes_remaining);
    char header[100];
    int r = snprintf(header, sizeof(header), "0x%08x : ", i);
    TEST_AND_RETURN(r == 13);
    string line = header;
    for (unsigned int j = 0; j < bytes_per_this_line; j++) {
      char buf[20];
      uint8_t c = arr[i + j];
      r = snprintf(buf, sizeof(buf), "%02x ", static_cast<unsigned int>(c));
      TEST_AND_RETURN(r == 3);
      line += buf;
    }
    LOG(INFO) << line;
  }
}

bool SplitPartitionName(const string& partition_name,
                        string* out_disk_name,
                        int* out_partition_num) {
  if (!base::StartsWith(
          partition_name, "/dev/", base::CompareCase::SENSITIVE)) {
    LOG(ERROR) << "Invalid partition device name: " << partition_name;
    return false;
  }

  size_t last_nondigit_pos = partition_name.find_last_not_of("0123456789");
  if (last_nondigit_pos == string::npos ||
      (last_nondigit_pos + 1) == partition_name.size()) {
    LOG(ERROR) << "Unable to parse partition device name: " << partition_name;
    return false;
  }

  size_t partition_name_len = string::npos;
  if (partition_name[last_nondigit_pos] == '_') {
    // NAND block devices have weird naming which could be something
    // like "/dev/ubiblock2_0". We discard "_0" in such a case.
    size_t prev_nondigit_pos =
        partition_name.find_last_not_of("0123456789", last_nondigit_pos - 1);
    if (prev_nondigit_pos == string::npos ||
        (prev_nondigit_pos + 1) == last_nondigit_pos) {
      LOG(ERROR) << "Unable to parse partition device name: " << partition_name;
      return false;
    }

    partition_name_len = last_nondigit_pos - prev_nondigit_pos;
    last_nondigit_pos = prev_nondigit_pos;
  }

  if (out_disk_name) {
    // Special case for MMC devices which have the following naming scheme:
    // mmcblk0p2
    size_t disk_name_len = last_nondigit_pos;
    if (partition_name[last_nondigit_pos] != 'p' || last_nondigit_pos == 0 ||
        !isdigit(partition_name[last_nondigit_pos - 1])) {
      disk_name_len++;
    }
    *out_disk_name = partition_name.substr(0, disk_name_len);
  }

  if (out_partition_num) {
    string partition_str =
        partition_name.substr(last_nondigit_pos + 1, partition_name_len);
    *out_partition_num = atoi(partition_str.c_str());
  }
  return true;
}

string MakePartitionName(const string& disk_name, int partition_num) {
  if (partition_num < 1) {
    LOG(ERROR) << "Invalid partition number: " << partition_num;
    return string();
  }

  if (!base::StartsWith(disk_name, "/dev/", base::CompareCase::SENSITIVE)) {
    LOG(ERROR) << "Invalid disk name: " << disk_name;
    return string();
  }

  if (IsMtdDeviceName(disk_name)) {
    // Special case for UBI block devices.
    //   1. ubiblock is not writable, we need to use plain "ubi".
    //   2. There is a "_0" suffix.
    return MakeNandPartitionName(partition_num);
  }

  string partition_name = disk_name;
  if (isdigit(partition_name.back())) {
    // Special case for devices with names ending with a digit.
    // Add "p" to separate the disk name from partition number,
    // e.g. "/dev/loop0p2"
    partition_name += 'p';
  }

  partition_name += std::to_string(partition_num);

  return partition_name;
}

string MakePartitionNameForMount(const string& part_name) {
  if (IsMtdDeviceName(part_name)) {
    int partition_num;
    if (!SplitPartitionName(part_name, nullptr, &partition_num)) {
      return "";
    }
    return MakeNandPartitionNameForMount(partition_num);
  }
  return part_name;
}

string ErrnoNumberAsString(int err) {
  char buf[100];
  buf[0] = '\0';
  return strerror_r(err, buf, sizeof(buf));
}

bool FileExists(const char* path) {
  struct stat stbuf;
  return 0 == lstat(path, &stbuf);
}

bool IsSymlink(const char* path) {
  struct stat stbuf;
  return lstat(path, &stbuf) == 0 && S_ISLNK(stbuf.st_mode) != 0;
}

bool TryAttachingUbiVolume(int volume_num, int timeout) {
  const string volume_path = base::StringPrintf("/dev/ubi%d_0", volume_num);
  if (FileExists(volume_path.c_str())) {
    return true;
  }

  int exit_code;
  vector<string> cmd = {"ubiattach",
                        "-m",
                        base::StringPrintf("%d", volume_num),
                        "-d",
                        base::StringPrintf("%d", volume_num)};
  TEST_AND_RETURN_FALSE(Subprocess::SynchronousExec(cmd, &exit_code, nullptr));
  TEST_AND_RETURN_FALSE(exit_code == 0);

  cmd = {"ubiblock", "--create", volume_path};
  TEST_AND_RETURN_FALSE(Subprocess::SynchronousExec(cmd, &exit_code, nullptr));
  TEST_AND_RETURN_FALSE(exit_code == 0);

  while (timeout > 0 && !FileExists(volume_path.c_str())) {
    sleep(1);
    timeout--;
  }

  return FileExists(volume_path.c_str());
}

bool MakeTempFile(const string& base_filename_template,
                  string* filename,
                  int* fd) {
  base::FilePath filename_template;
  TEST_AND_RETURN_FALSE(
      GetTempName(base_filename_template, &filename_template));
  DCHECK(filename || fd);
  vector<char> buf(filename_template.value().size() + 1);
  memcpy(buf.data(),
         filename_template.value().data(),
         filename_template.value().size());
  buf[filename_template.value().size()] = '\0';

  int mkstemp_fd = mkstemp(buf.data());
  TEST_AND_RETURN_FALSE_ERRNO(mkstemp_fd >= 0);
  if (filename) {
    *filename = buf.data();
  }
  if (fd) {
    *fd = mkstemp_fd;
  } else {
    close(mkstemp_fd);
  }
  return true;
}

bool SetBlockDeviceReadOnly(const string& device, bool read_only) {
  int fd = HANDLE_EINTR(open(device.c_str(), O_RDONLY | O_CLOEXEC));
  if (fd < 0) {
    PLOG(ERROR) << "Opening block device " << device;
    return false;
  }
  ScopedFdCloser fd_closer(&fd);
  // We take no action if not needed.
  int read_only_flag;
  int expected_flag = read_only ? 1 : 0;
  int rc = ioctl(fd, BLKROGET, &read_only_flag);
  // In case of failure reading the setting we will try to set it anyway.
  if (rc == 0 && read_only_flag == expected_flag)
    return true;

  rc = ioctl(fd, BLKROSET, &expected_flag);
  if (rc != 0) {
    PLOG(ERROR) << "Marking block device " << device
                << " as read_only=" << expected_flag;
    return false;
  }
  return true;
}

bool MountFilesystem(const string& device,
                     const string& mountpoint,
                     unsigned long mountflags,  // NOLINT(runtime/int)
                     const string& type,
                     const string& fs_mount_options) {
  vector<const char*> fstypes;
  if (type.empty()) {
    fstypes = {"ext2", "ext3", "ext4", "squashfs"};
  } else {
    fstypes = {type.c_str()};
  }
  for (const char* fstype : fstypes) {
    int rc = mount(device.c_str(),
                   mountpoint.c_str(),
                   fstype,
                   mountflags,
                   fs_mount_options.c_str());
    if (rc == 0)
      return true;

    PLOG(WARNING) << "Unable to mount destination device " << device << " on "
                  << mountpoint << " as " << fstype;
  }
  if (!type.empty()) {
    LOG(ERROR) << "Unable to mount " << device << " with any supported type";
  }
  return false;
}

bool UnmountFilesystem(const string& mountpoint) {
  int num_retries = 1;
  for (;; ++num_retries) {
    if (umount(mountpoint.c_str()) == 0)
      return true;
    if (errno != EBUSY || num_retries >= kUnmountMaxNumOfRetries)
      break;
    usleep(kUnmountRetryIntervalInMicroseconds);
  }
  if (errno == EINVAL) {
    LOG(INFO) << "Not a mountpoint: " << mountpoint;
    return false;
  }
  PLOG(WARNING) << "Error unmounting " << mountpoint << " after " << num_retries
                << " attempts. Lazy unmounting instead, error was";
  if (umount2(mountpoint.c_str(), MNT_DETACH) != 0) {
    PLOG(ERROR) << "Lazy unmount failed";
    return false;
  }
  return true;
}

bool IsMountpoint(const std::string& mountpoint) {
  struct stat stdir, stparent;

  // Check whether the passed mountpoint is a directory and the /.. is in the
  // same device or not. If mountpoint/.. is in a different device it means that
  // there is a filesystem mounted there. If it is not, but they both point to
  // the same inode it basically is the special case of /.. pointing to /. This
  // test doesn't play well with bind mount but that's out of the scope of what
  // we want to detect here.
  if (lstat(mountpoint.c_str(), &stdir) != 0) {
    PLOG(ERROR) << "Error stat'ing " << mountpoint;
    return false;
  }
  if (!S_ISDIR(stdir.st_mode))
    return false;

  base::FilePath parent(mountpoint);
  parent = parent.Append("..");
  if (lstat(parent.value().c_str(), &stparent) != 0) {
    PLOG(ERROR) << "Error stat'ing " << parent.value();
    return false;
  }
  return S_ISDIR(stparent.st_mode) &&
         (stparent.st_dev != stdir.st_dev || stparent.st_ino == stdir.st_ino);
}

// Tries to parse the header of an ELF file to obtain a human-readable
// description of it on the |output| string.
static bool GetFileFormatELF(const uint8_t* buffer,
                             size_t size,
                             string* output) {
  // 0x00: EI_MAG - ELF magic header, 4 bytes.
  if (size < SELFMAG || memcmp(buffer, ELFMAG, SELFMAG) != 0)
    return false;
  *output = "ELF";

  // 0x04: EI_CLASS, 1 byte.
  if (size < EI_CLASS + 1)
    return true;
  switch (buffer[EI_CLASS]) {
    case ELFCLASS32:
      *output += " 32-bit";
      break;
    case ELFCLASS64:
      *output += " 64-bit";
      break;
    default:
      *output += " ?-bit";
  }

  // 0x05: EI_DATA, endianness, 1 byte.
  if (size < EI_DATA + 1)
    return true;
  uint8_t ei_data = buffer[EI_DATA];
  switch (ei_data) {
    case ELFDATA2LSB:
      *output += " little-endian";
      break;
    case ELFDATA2MSB:
      *output += " big-endian";
      break;
    default:
      *output += " ?-endian";
      // Don't parse anything after the 0x10 offset if endianness is unknown.
      return true;
  }

  const Elf32_Ehdr* hdr = reinterpret_cast<const Elf32_Ehdr*>(buffer);
  // 0x12: e_machine, 2 byte endianness based on ei_data. The position (0x12)
  // and size is the same for both 32 and 64 bits.
  if (size < offsetof(Elf32_Ehdr, e_machine) + sizeof(hdr->e_machine))
    return true;
  uint16_t e_machine;
  // Fix endianness regardless of the host endianness.
  if (ei_data == ELFDATA2LSB)
    e_machine = le16toh(hdr->e_machine);
  else
    e_machine = be16toh(hdr->e_machine);

  switch (e_machine) {
    case EM_386:
      *output += " x86";
      break;
    case EM_MIPS:
      *output += " mips";
      break;
    case EM_ARM:
      *output += " arm";
      break;
    case EM_X86_64:
      *output += " x86-64";
      break;
    default:
      *output += " unknown-arch";
  }
  return true;
}

string GetFileFormat(const string& path) {
  brillo::Blob buffer;
  if (!ReadFileChunkAndAppend(path, 0, kGetFileFormatMaxHeaderSize, &buffer))
    return "File not found.";

  string result;
  if (GetFileFormatELF(buffer.data(), buffer.size(), &result))
    return result;

  return "data";
}

int FuzzInt(int value, unsigned int range) {
  int min = value - range / 2;
  int max = value + range - range / 2;
  return base::RandInt(min, max);
}

string FormatSecs(unsigned secs) {
  return FormatTimeDelta(TimeDelta::FromSeconds(secs));
}

string FormatTimeDelta(TimeDelta delta) {
  string str;

  // Handle negative durations by prefixing with a minus.
  if (delta.ToInternalValue() < 0) {
    delta *= -1;
    str = "-";
  }

  // Canonicalize into days, hours, minutes, seconds and microseconds.
  unsigned days = delta.InDays();
  delta -= TimeDelta::FromDays(days);
  unsigned hours = delta.InHours();
  delta -= TimeDelta::FromHours(hours);
  unsigned mins = delta.InMinutes();
  delta -= TimeDelta::FromMinutes(mins);
  unsigned secs = delta.InSeconds();
  delta -= TimeDelta::FromSeconds(secs);
  unsigned usecs = delta.InMicroseconds();

  if (days)
    base::StringAppendF(&str, "%ud", days);
  if (days || hours)
    base::StringAppendF(&str, "%uh", hours);
  if (days || hours || mins)
    base::StringAppendF(&str, "%um", mins);
  base::StringAppendF(&str, "%u", secs);
  if (usecs) {
    int width = 6;
    while ((usecs / 10) * 10 == usecs) {
      usecs /= 10;
      width--;
    }
    base::StringAppendF(&str, ".%0*u", width, usecs);
  }
  base::StringAppendF(&str, "s");
  return str;
}

string ToString(const Time utc_time) {
  Time::Exploded exp_time;
  utc_time.UTCExplode(&exp_time);
  return base::StringPrintf("%d/%d/%d %d:%02d:%02d GMT",
                            exp_time.month,
                            exp_time.day_of_month,
                            exp_time.year,
                            exp_time.hour,
                            exp_time.minute,
                            exp_time.second);
}

string ToString(bool b) {
  return (b ? "true" : "false");
}

string ToString(DownloadSource source) {
  switch (source) {
    case kDownloadSourceHttpsServer:
      return "HttpsServer";
    case kDownloadSourceHttpServer:
      return "HttpServer";
    case kDownloadSourceHttpPeer:
      return "HttpPeer";
    case kNumDownloadSources:
      return "Unknown";
      // Don't add a default case to let the compiler warn about newly added
      // download sources which should be added here.
  }

  return "Unknown";
}

string ToString(PayloadType payload_type) {
  switch (payload_type) {
    case kPayloadTypeDelta:
      return "Delta";
    case kPayloadTypeFull:
      return "Full";
    case kPayloadTypeForcedFull:
      return "ForcedFull";
    case kNumPayloadTypes:
      return "Unknown";
      // Don't add a default case to let the compiler warn about newly added
      // payload types which should be added here.
  }

  return "Unknown";
}

ErrorCode GetBaseErrorCode(ErrorCode code) {
  // Ignore the higher order bits in the code by applying the mask as
  // we want the enumerations to be in the small contiguous range
  // with values less than ErrorCode::kUmaReportedMax.
  ErrorCode base_code = static_cast<ErrorCode>(
      static_cast<int>(code) & ~static_cast<int>(ErrorCode::kSpecialFlags));

  // Make additional adjustments required for UMA and error classification.
  // TODO(jaysri): Move this logic to UeErrorCode.cc when we fix
  // chromium-os:34369.
  if (base_code >= ErrorCode::kOmahaRequestHTTPResponseBase) {
    // Since we want to keep the enums to a small value, aggregate all HTTP
    // errors into this one bucket for UMA and error classification purposes.
    LOG(INFO) << "Converting error code " << base_code
              << " to ErrorCode::kOmahaErrorInHTTPResponse";
    base_code = ErrorCode::kOmahaErrorInHTTPResponse;
  }

  return base_code;
}

string StringVectorToString(const vector<string> &vec_str) {
  string str = "[";
  for (vector<string>::const_iterator i = vec_str.begin(); i != vec_str.end();
       ++i) {
    if (i != vec_str.begin())
      str += ", ";
    str += '"';
    str += *i;
    str += '"';
  }
  str += "]";
  return str;
}

// The P2P file id should be the same for devices running new version and old
// version so that they can share it with each other. The hash in the response
// was base64 encoded, but now that we switched to use "hash_sha256" field which
// is hex encoded, we have to convert them back to base64 for P2P. However, the
// base64 encoded hash was base64 encoded here again historically for some
// reason, so we keep the same behavior here.
string CalculateP2PFileId(const brillo::Blob& payload_hash,
                          size_t payload_size) {
  string encoded_hash = brillo::data_encoding::Base64Encode(
      brillo::data_encoding::Base64Encode(payload_hash));
  return base::StringPrintf("cros_update_size_%" PRIuS "_hash_%s",
                            payload_size,
                            encoded_hash.c_str());
}

bool ConvertToOmahaInstallDate(Time time, int *out_num_days) {
  time_t unix_time = time.ToTimeT();
  // Output of: date +"%s" --date="Jan 1, 2007 0:00 PST".
  const time_t kOmahaEpoch = 1167638400;
  const int64_t kNumSecondsPerWeek = 7 * 24 * 3600;
  const int64_t kNumDaysPerWeek = 7;

  time_t omaha_time = unix_time - kOmahaEpoch;

  if (omaha_time < 0)
    return false;

  // Note, as per the comment in utils.h we are deliberately not
  // handling DST correctly.

  int64_t num_weeks_since_omaha_epoch = omaha_time / kNumSecondsPerWeek;
  *out_num_days = num_weeks_since_omaha_epoch * kNumDaysPerWeek;

  return true;
}

bool GetMinorVersion(const brillo::KeyValueStore& store,
                     uint32_t* minor_version) {
  string result;
  if (store.GetString("PAYLOAD_MINOR_VERSION", &result)) {
    if (!base::StringToUint(result, minor_version)) {
      LOG(ERROR) << "StringToUint failed when parsing delta minor version.";
      return false;
    }
    return true;
  }
  return false;
}

bool ReadExtents(const string& path,
                 const vector<Extent>& extents,
                 brillo::Blob* out_data,
                 ssize_t out_data_size,
                 size_t block_size) {
  brillo::Blob data(out_data_size);
  ssize_t bytes_read = 0;
  int fd = open(path.c_str(), O_RDONLY);
  TEST_AND_RETURN_FALSE_ERRNO(fd >= 0);
  ScopedFdCloser fd_closer(&fd);

  for (const Extent& extent : extents) {
    ssize_t bytes_read_this_iteration = 0;
    ssize_t bytes = extent.num_blocks() * block_size;
    TEST_AND_RETURN_FALSE(bytes_read + bytes <= out_data_size);
    TEST_AND_RETURN_FALSE(utils::PReadAll(fd,
                                          &data[bytes_read],
                                          bytes,
                                          extent.start_block() * block_size,
                                          &bytes_read_this_iteration));
    TEST_AND_RETURN_FALSE(bytes_read_this_iteration == bytes);
    bytes_read += bytes_read_this_iteration;
  }
  TEST_AND_RETURN_FALSE(out_data_size == bytes_read);
  *out_data = data;
  return true;
}

bool GetBootId(string* boot_id) {
  TEST_AND_RETURN_FALSE(
      base::ReadFileToString(base::FilePath(kBootIdPath), boot_id));
  base::TrimWhitespaceASCII(*boot_id, base::TRIM_TRAILING, boot_id);
  return true;
}

int VersionPrefix(const std::string& version) {
  if (version.empty()) {
    return 0;
  }
  vector<string> tokens = base::SplitString(
      version, ".", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);
  int value;
  if (tokens.empty() || !base::StringToInt(tokens[0], &value))
    return -1;  // Target version is invalid.
  return value;
}

void ParseRollbackKeyVersion(const string& raw_version,
                             uint16_t* high_version,
                             uint16_t* low_version) {
  DCHECK(high_version);
  DCHECK(low_version);
  *high_version = numeric_limits<uint16_t>::max();
  *low_version = numeric_limits<uint16_t>::max();

  vector<string> parts = base::SplitString(
      raw_version, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  if (parts.size() != 2) {
    // The version string must have exactly one period.
    return;
  }

  int high;
  int low;
  if (!(base::StringToInt(parts[0], &high) &&
        base::StringToInt(parts[1], &low))) {
    // Both parts of the version could not be parsed correctly.
    return;
  }

  if (high >= 0 && high < numeric_limits<uint16_t>::max() && low >= 0 &&
      low < numeric_limits<uint16_t>::max()) {
    *high_version = static_cast<uint16_t>(high);
    *low_version = static_cast<uint16_t>(low);
  }
}

}  // namespace utils

}  // namespace chromeos_update_engine
