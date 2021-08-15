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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_H_

#include <errno.h>
#include <sys/types.h>
#include <memory>

#include <base/logging.h>

// Abstraction for managing opening, reading, writing and closing of file
// descriptors. This includes an abstract class and one standard implementation
// based on POSIX system calls.
//
// TODO(garnold) this class is modeled after (and augments the functionality of)
// the FileWriter class; ultimately, the latter should be replaced by the former
// throughout the codebase.  A few deviations from the original FileWriter:
//
// * Providing two flavors of Open()
//
// * A FileDescriptor is reusable and can be used to read/write multiple files
//   as long as open/close preconditions are respected.
//
// * Write() returns the number of bytes written: this appears to be more useful
//   for clients, who may wish to retry or otherwise do something useful with
//   the remaining data that was not written.

namespace chromeos_update_engine {

class FileDescriptor;
using FileDescriptorPtr = std::shared_ptr<FileDescriptor>;

// An abstract class defining the file descriptor API.
class FileDescriptor {
 public:
  FileDescriptor() {}
  virtual ~FileDescriptor() {}

  // Opens a file descriptor. The descriptor must be in the closed state prior
  // to this call. Returns true on success, false otherwise. Specific
  // implementations may set errno accordingly.
  virtual bool Open(const char* path, int flags, mode_t mode) = 0;
  virtual bool Open(const char* path, int flags) = 0;

  // Reads from a file descriptor up to a given count. The descriptor must be
  // open prior to this call. Returns the number of bytes read, or -1 on error.
  // Specific implementations may set errno accordingly.
  virtual ssize_t Read(void* buf, size_t count) = 0;

  // Writes to a file descriptor. The descriptor must be open prior to this
  // call. Returns the number of bytes written, or -1 if an error occurred and
  // no bytes were written. Specific implementations may set errno accordingly.
  virtual ssize_t Write(const void* buf, size_t count) = 0;

  // Seeks to an offset. Returns the resulting offset location as measured in
  // bytes from the beginning. On error, return -1. Specific implementations
  // may set errno accordingly.
  virtual off64_t Seek(off64_t offset, int whence) = 0;

  // Return the size of the block device in bytes, or 0 if the device is not a
  // block device or an error occurred.
  virtual uint64_t BlockDevSize() = 0;

  // Runs a ioctl() on the file descriptor if supported. Returns whether
  // the operation is supported. The |request| can be one of BLKDISCARD,
  // BLKZEROOUT and BLKSECDISCARD to discard, write zeros or securely discard
  // the blocks. These ioctls accept a range of bytes (|start| and |length|)
  // over which they perform the operation. The return value from the ioctl is
  // stored in |result|.
  virtual bool BlkIoctl(int request,
                        uint64_t start,
                        uint64_t length,
                        int* result) = 0;

  // Flushes any cached data. The descriptor must be opened prior to this
  // call. Returns false if it fails to write data. Implementations may set
  // errno accrodingly.
  virtual bool Flush() = 0;

  // Closes a file descriptor. The descriptor must be open prior to this call.
  // Returns true on success, false otherwise. Specific implementations may set
  // errno accordingly.
  virtual bool Close() = 0;

  // Indicates whether or not an implementation sets meaningful errno.
  virtual bool IsSettingErrno() = 0;

  // Indicates whether the descriptor is currently open.
  virtual bool IsOpen() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileDescriptor);
};

// A simple EINTR-immune wrapper implementation around standard system calls.
class EintrSafeFileDescriptor : public FileDescriptor {
 public:
  EintrSafeFileDescriptor() : fd_(-1) {}

  // Interface methods.
  bool Open(const char* path, int flags, mode_t mode) override;
  bool Open(const char* path, int flags) override;
  ssize_t Read(void* buf, size_t count) override;
  ssize_t Write(const void* buf, size_t count) override;
  off64_t Seek(off64_t offset, int whence) override;
  uint64_t BlockDevSize() override;
  bool BlkIoctl(int request,
                uint64_t start,
                uint64_t length,
                int* result) override;
  bool Flush() override;
  bool Close() override;
  bool IsSettingErrno() override { return true; }
  bool IsOpen() override { return (fd_ >= 0); }

 protected:
  int fd_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_H_
