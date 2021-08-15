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

#ifndef UPDATE_ENGINE_COMMON_UTILS_H_
#define UPDATE_ENGINE_COMMON_UTILS_H_

#include <errno.h>
#include <unistd.h>

#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <base/files/file_path.h>
#include <base/posix/eintr_wrapper.h>
#include <base/time/time.h>
#include <brillo/key_value_store.h>
#include <brillo/secure_blob.h>

#include "update_engine/common/action.h"
#include "update_engine/common/action_processor.h"
#include "update_engine/common/constants.h"
#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

namespace utils {

// Formats |vec_str| as a string of the form ["<elem1>", "<elem2>"].
// Does no escaping, only use this for presentation in error messages.
std::string StringVectorToString(const std::vector<std::string>& vec_str);

// Calculates the p2p file id from payload hash and size
std::string CalculateP2PFileId(const brillo::Blob& payload_hash,
                               size_t payload_size);

// Parse the firmware version from one line of output from the
// "mosys" command.
std::string ParseECVersion(std::string input_line);

// Writes the data passed to path. The file at path will be overwritten if it
// exists. Returns true on success, false otherwise.
bool WriteFile(const char* path, const void* data, size_t data_len);

// Calls write() or pwrite() repeatedly until all count bytes at buf are
// written to fd or an error occurs. Returns true on success.
bool WriteAll(int fd, const void* buf, size_t count);
bool PWriteAll(int fd, const void* buf, size_t count, off_t offset);

bool WriteAll(const FileDescriptorPtr& fd, const void* buf, size_t count);
bool PWriteAll(const FileDescriptorPtr& fd,
               const void* buf,
               size_t count,
               off_t offset);

// Calls read() repeatedly until |count| bytes are read or EOF or EWOULDBLOCK
// is reached. Returns whether all read() calls succeeded (including EWOULDBLOCK
// as a success case), sets |eof| to whether the eof was reached and sets
// |out_bytes_read| to the actual number of bytes read regardless of the return
// value.
bool ReadAll(
    int fd, void* buf, size_t count, size_t* out_bytes_read, bool* eof);

// Calls pread() repeatedly until count bytes are read, or EOF is reached.
// Returns number of bytes read in *bytes_read. Returns true on success.
bool PReadAll(
    int fd, void* buf, size_t count, off_t offset, ssize_t* out_bytes_read);

bool PReadAll(const FileDescriptorPtr& fd,
              void* buf,
              size_t count,
              off_t offset,
              ssize_t* out_bytes_read);

// Opens |path| for reading and appends its entire content to the container
// pointed to by |out_p|. Returns true upon successfully reading all of the
// file's content, false otherwise, in which case the state of the output
// container is unknown. ReadFileChunk starts reading the file from |offset|; if
// |size| is not -1, only up to |size| bytes are read in.
bool ReadFile(const std::string& path, brillo::Blob* out_p);
bool ReadFile(const std::string& path, std::string* out_p);
bool ReadFileChunk(const std::string& path,
                   off_t offset,
                   off_t size,
                   brillo::Blob* out_p);

// Invokes |cmd| in a pipe and appends its stdout to the container pointed to by
// |out_p|. Returns true upon successfully reading all of the output, false
// otherwise, in which case the state of the output container is unknown.
bool ReadPipe(const std::string& cmd, std::string* out_p);

// Returns the size of the block device at the file descriptor fd. If an error
// occurs, -1 is returned.
off_t BlockDevSize(int fd);

// Returns the size of the file at path, or the file descriptor fd. If the file
// is actually a block device, this function will automatically call
// BlockDevSize. If the file doesn't exist or some error occurrs, -1 is
// returned.
off_t FileSize(const std::string& path);
off_t FileSize(int fd);

std::string ErrnoNumberAsString(int err);

// Returns true if the file exists for sure. Returns false if it doesn't exist,
// or an error occurs.
bool FileExists(const char* path);

// Returns true if |path| exists and is a symbolic link.
bool IsSymlink(const char* path);

// Try attaching UBI |volume_num|. If there is any error executing required
// commands to attach the volume, this function returns false. This function
// only returns true if "/dev/ubi%d_0" becomes available in |timeout| seconds.
bool TryAttachingUbiVolume(int volume_num, int timeout);

// If |base_filename_template| is neither absolute (starts with "/") nor
// explicitly relative to the current working directory (starts with "./" or
// "../"), then it is prepended the system's temporary directory. On success,
// stores the name of the new temporary file in |filename|. If |fd| is
// non-null, the file descriptor returned by mkstemp is written to it and
// kept open; otherwise, it is closed. The template must end with "XXXXXX".
// Returns true on success.
bool MakeTempFile(const std::string& base_filename_template,
                  std::string* filename,
                  int* fd);

// Splits the partition device name into the block device name and partition
// number. For example, "/dev/sda3" will be split into {"/dev/sda", 3} and
// "/dev/mmcblk0p2" into {"/dev/mmcblk0", 2}
// Returns false when malformed device name is passed in.
// If both output parameters are omitted (null), can be used
// just to test the validity of the device name. Note that the function
// simply checks if the device name looks like a valid device, no other
// checks are performed (i.e. it doesn't check if the device actually exists).
bool SplitPartitionName(const std::string& partition_name,
                        std::string* out_disk_name,
                        int* out_partition_num);

// Builds a partition device name from the block device name and partition
// number. For example:
// {"/dev/sda", 1} => "/dev/sda1"
// {"/dev/mmcblk2", 12} => "/dev/mmcblk2p12"
// Returns empty string when invalid parameters are passed in
std::string MakePartitionName(const std::string& disk_name, int partition_num);

// Similar to "MakePartitionName" but returns a name that is suitable for
// mounting. On NAND system we can write to "/dev/ubiX_0", which is what
// MakePartitionName returns, but we cannot mount that device. To mount, we
// have to use "/dev/ubiblockX_0" for rootfs. Stateful and OEM partitions are
// mountable with "/dev/ubiX_0". The input is a partition device such as
// /dev/sda3. Return empty string on error.
std::string MakePartitionNameForMount(const std::string& part_name);

// Set the read-only attribute on the block device |device| to the value passed
// in |read_only|. Return whether the operation succeeded.
bool SetBlockDeviceReadOnly(const std::string& device, bool read_only);

// Synchronously mount or unmount a filesystem. Return true on success.
// When mounting, it will attempt to mount the device as the passed filesystem
// type |type|, with the passed |flags| options. If |type| is empty, "ext2",
// "ext3", "ext4" and "squashfs" will be tried.
bool MountFilesystem(const std::string& device,
                     const std::string& mountpoint,
                     unsigned long flags,  // NOLINT(runtime/int)
                     const std::string& type,
                     const std::string& fs_mount_options);
bool UnmountFilesystem(const std::string& mountpoint);

// Return whether the passed |mountpoint| path is a directory where a filesystem
// is mounted. Due to detection mechanism limitations, when used on directories
// where another part of the tree was bind mounted returns true only if bind
// mounted on top of a different filesystem (not inside the same filesystem).
bool IsMountpoint(const std::string& mountpoint);

// Returns a human-readable string with the file format based on magic constants
// on the header of the file.
std::string GetFileFormat(const std::string& path);

// Returns the string representation of the given UTC time.
// such as "11/14/2011 14:05:30 GMT".
std::string ToString(const base::Time utc_time);

// Returns true or false depending on the value of b.
std::string ToString(bool b);

// Returns a string representation of the given enum.
std::string ToString(DownloadSource source);

// Returns a string representation of the given enum.
std::string ToString(PayloadType payload_type);

// Fuzzes an integer |value| randomly in the range:
// [value - range / 2, value + range - range / 2]
int FuzzInt(int value, unsigned int range);

// Log a string in hex to LOG(INFO). Useful for debugging.
void HexDumpArray(const uint8_t* const arr, const size_t length);
inline void HexDumpString(const std::string& str) {
  HexDumpArray(reinterpret_cast<const uint8_t*>(str.data()), str.size());
}
inline void HexDumpVector(const brillo::Blob& vect) {
  HexDumpArray(vect.data(), vect.size());
}

template <typename T>
bool VectorIndexOf(const std::vector<T>& vect,
                   const T& value,
                   typename std::vector<T>::size_type* out_index) {
  typename std::vector<T>::const_iterator it =
      std::find(vect.begin(), vect.end(), value);
  if (it == vect.end()) {
    return false;
  } else {
    *out_index = it - vect.begin();
    return true;
  }
}

// Return the total number of blocks in the passed |extents| collection.
template <class T>
uint64_t BlocksInExtents(const T& extents) {
  uint64_t sum = 0;
  for (const auto& ext : extents) {
    sum += ext.num_blocks();
  }
  return sum;
}

// Converts seconds into human readable notation including days, hours, minutes
// and seconds. For example, 185 will yield 3m5s, 4300 will yield 1h11m40s, and
// 360000 will yield 4d4h0m0s.  Zero padding not applied. Seconds are always
// shown in the result.
std::string FormatSecs(unsigned secs);

// Converts a TimeDelta into human readable notation including days, hours,
// minutes, seconds and fractions of a second down to microsecond granularity,
// as necessary; for example, an output of 5d2h0m15.053s means that the input
// time was precise to the milliseconds only. Zero padding not applied, except
// for fractions. Seconds are always shown, but fractions thereof are only shown
// when applicable. If |delta| is negative, the output will have a leading '-'
// followed by the absolute duration.
std::string FormatTimeDelta(base::TimeDelta delta);

// This method transforms the given error code to be suitable for UMA and
// for error classification purposes by removing the higher order bits and
// aggregating error codes beyond the enum range, etc. This method is
// idempotent, i.e. if called with a value previously returned by this method,
// it'll return the same value again.
ErrorCode GetBaseErrorCode(ErrorCode code);

// Converts |time| to an Omaha InstallDate which is defined as "the
// number of PST8PDT calendar weeks since Jan 1st 2007 0:00 PST, times
// seven" with PST8PDT defined as "Pacific Time" (e.g. UTC-07:00 if
// daylight savings is observed and UTC-08:00 otherwise.)
//
// If the passed in |time| variable is before Monday January 1st 2007
// 0:00 PST, False is returned and the value returned in
// |out_num_days| is undefined. Otherwise the number of PST8PDT
// calendar weeks since that date times seven is returned in
// |out_num_days| and the function returns True.
//
// (NOTE: This function does not currently take daylight savings time
// into account so the result may up to one hour off. This is because
// the glibc date and timezone routines depend on the TZ environment
// variable and changing environment variables is not thread-safe.
bool ConvertToOmahaInstallDate(base::Time time, int* out_num_days);

// Look for the minor version value in the passed |store| and set
// |minor_version| to that value. Return whether the value was found and valid.
bool GetMinorVersion(const brillo::KeyValueStore& store,
                     uint32_t* minor_version);

// This function reads the specified data in |extents| into |out_data|. The
// extents are read from the file at |path|. |out_data_size| is the size of
// |out_data|. Returns false if the number of bytes to read given in
// |extents| does not equal |out_data_size|.
bool ReadExtents(const std::string& path,
                 const std::vector<Extent>& extents,
                 brillo::Blob* out_data,
                 ssize_t out_data_size,
                 size_t block_size);

// Read the current boot identifier and store it in |boot_id|. This identifier
// is constants during the same boot of the kernel and is regenerated after
// reboot. Returns whether it succeeded getting the boot_id.
bool GetBootId(std::string* boot_id);

// Divide |x| by |y| and round up to the nearest integer.
constexpr uint64_t DivRoundUp(uint64_t x, uint64_t y) {
  return (x + y - 1) / y;
}

// Round |x| up to be a multiple of |y|.
constexpr uint64_t RoundUp(uint64_t x, uint64_t y) {
  return DivRoundUp(x, y) * y;
}

// Returns the integer value of the first section of |version|. E.g. for
//  "10575.39." returns 10575. Returns 0 if |version| is empty, returns -1 if
// first section of |version| is invalid (e.g. not a number).
int VersionPrefix(const std::string& version);

// Parses a string in the form high.low, where high and low are 16 bit unsigned
// integers. If there is more than 1 dot, or if either of the two parts are
// not valid 16 bit unsigned numbers, then 0xffff is returned for both.
void ParseRollbackKeyVersion(const std::string& raw_version,
                             uint16_t* high_version,
                             uint16_t* low_version);

}  // namespace utils

// Utility class to close a file descriptor
class ScopedFdCloser {
 public:
  explicit ScopedFdCloser(int* fd) : fd_(fd) {}
  ~ScopedFdCloser() {
    if (should_close_ && fd_ && (*fd_ >= 0) && !IGNORE_EINTR(close(*fd_)))
      *fd_ = -1;
  }
  void set_should_close(bool should_close) { should_close_ = should_close; }

 private:
  int* fd_;
  bool should_close_ = true;
  DISALLOW_COPY_AND_ASSIGN(ScopedFdCloser);
};

// Utility class to delete a file when it goes out of scope.
class ScopedPathUnlinker {
 public:
  explicit ScopedPathUnlinker(const std::string& path)
      : path_(path), should_remove_(true) {}
  ~ScopedPathUnlinker() {
    if (should_remove_ && unlink(path_.c_str()) < 0) {
      PLOG(ERROR) << "Unable to unlink path " << path_;
    }
  }
  void set_should_remove(bool should_remove) { should_remove_ = should_remove; }

 private:
  const std::string path_;
  bool should_remove_;
  DISALLOW_COPY_AND_ASSIGN(ScopedPathUnlinker);
};

// A little object to call ActionComplete on the ActionProcessor when
// it's destructed.
class ScopedActionCompleter {
 public:
  explicit ScopedActionCompleter(ActionProcessor* processor,
                                 AbstractAction* action)
      : processor_(processor),
        action_(action),
        code_(ErrorCode::kError),
        should_complete_(true) {
    CHECK(processor_);
  }
  ~ScopedActionCompleter() {
    if (should_complete_)
      processor_->ActionComplete(action_, code_);
  }
  void set_code(ErrorCode code) { code_ = code; }
  void set_should_complete(bool should_complete) {
    should_complete_ = should_complete;
  }
  ErrorCode get_code() const { return code_; }

 private:
  ActionProcessor* processor_;
  AbstractAction* action_;
  ErrorCode code_;
  bool should_complete_;
  DISALLOW_COPY_AND_ASSIGN(ScopedActionCompleter);
};

}  // namespace chromeos_update_engine

#define TEST_AND_RETURN_FALSE_ERRNO(_x)                              \
  do {                                                               \
    bool _success = static_cast<bool>(_x);                           \
    if (!_success) {                                                 \
      std::string _msg =                                             \
          chromeos_update_engine::utils::ErrnoNumberAsString(errno); \
      LOG(ERROR) << #_x " failed: " << _msg;                         \
      return false;                                                  \
    }                                                                \
  } while (0)

#define TEST_AND_RETURN_FALSE(_x)          \
  do {                                     \
    bool _success = static_cast<bool>(_x); \
    if (!_success) {                       \
      LOG(ERROR) << #_x " failed.";        \
      return false;                        \
    }                                      \
  } while (0)

#define TEST_AND_RETURN_ERRNO(_x)                                    \
  do {                                                               \
    bool _success = static_cast<bool>(_x);                           \
    if (!_success) {                                                 \
      std::string _msg =                                             \
          chromeos_update_engine::utils::ErrnoNumberAsString(errno); \
      LOG(ERROR) << #_x " failed: " << _msg;                         \
      return;                                                        \
    }                                                                \
  } while (0)

#define TEST_AND_RETURN(_x)                \
  do {                                     \
    bool _success = static_cast<bool>(_x); \
    if (!_success) {                       \
      LOG(ERROR) << #_x " failed.";        \
      return;                              \
    }                                      \
  } while (0)

#define TEST_AND_RETURN_FALSE_ERRCODE(_x)      \
  do {                                         \
    errcode_t _error = (_x);                   \
    if (_error) {                              \
      errno = _error;                          \
      LOG(ERROR) << #_x " failed: " << _error; \
      return false;                            \
    }                                          \
  } while (0)

#endif  // UPDATE_ENGINE_COMMON_UTILS_H_
