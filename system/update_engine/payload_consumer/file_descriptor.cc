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

#include "update_engine/payload_consumer/file_descriptor.h"

#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <base/posix/eintr_wrapper.h>

#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

bool EintrSafeFileDescriptor::Open(const char* path, int flags, mode_t mode) {
  CHECK_EQ(fd_, -1);
  return ((fd_ = HANDLE_EINTR(open(path, flags, mode))) >= 0);
}

bool EintrSafeFileDescriptor::Open(const char* path, int flags) {
  CHECK_EQ(fd_, -1);
  return ((fd_ = HANDLE_EINTR(open(path, flags))) >= 0);
}

ssize_t EintrSafeFileDescriptor::Read(void* buf, size_t count) {
  CHECK_GE(fd_, 0);
  return HANDLE_EINTR(read(fd_, buf, count));
}

ssize_t EintrSafeFileDescriptor::Write(const void* buf, size_t count) {
  CHECK_GE(fd_, 0);

  // Attempt repeated writes, as long as some progress is being made.
  char* char_buf = const_cast<char*>(reinterpret_cast<const char*>(buf));
  ssize_t written = 0;
  while (count > 0) {
    ssize_t ret = HANDLE_EINTR(write(fd_, char_buf, count));

    // Fail on either an error or no progress.
    if (ret <= 0)
      return (written ? written : ret);
    written += ret;
    count -= ret;
    char_buf += ret;
  }
  return written;
}

off64_t EintrSafeFileDescriptor::Seek(off64_t offset, int whence) {
  CHECK_GE(fd_, 0);
  return lseek64(fd_, offset, whence);
}

uint64_t EintrSafeFileDescriptor::BlockDevSize() {
  if (fd_ < 0)
    return 0;
  struct stat stbuf;
  if (fstat(fd_, &stbuf) < 0) {
    PLOG(ERROR) << "Error stat-ing fd " << fd_;
    return 0;
  }
  if (!S_ISBLK(stbuf.st_mode))
    return 0;
  off_t block_size = utils::BlockDevSize(fd_);
  return block_size < 0 ? 0 : block_size;
}

bool EintrSafeFileDescriptor::BlkIoctl(int request,
                                       uint64_t start,
                                       uint64_t length,
                                       int* result) {
  // If the ioctl BLKZEROOUT is not defined, just fail to perform any of these
  // operations.
#ifndef BLKZEROOUT
  return false;
#else   // defined(BLKZEROOUT)
  DCHECK(request == BLKDISCARD || request == BLKZEROOUT ||
         request == BLKSECDISCARD);
  // On some devices, the BLKDISCARD will actually read back as zeros, instead
  // of "undefined" data. The BLKDISCARDZEROES ioctl tells whether that's the
  // case, so we issue a BLKDISCARD in those cases to speed up the writes.
  unsigned int arg;
  if (request == BLKZEROOUT && ioctl(fd_, BLKDISCARDZEROES, &arg) == 0 && arg)
    request = BLKDISCARD;

  // Ensure the |fd_| is in O_DIRECT mode during this operation, so the write
  // cache for this region is invalidated. This is required since otherwise
  // reading back this region could consume stale data from the cache.
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    PLOG(WARNING) << "Couldn't get flags on fd " << fd_;
    return false;
  }
  if ((flags & O_DIRECT) == 0 && fcntl(fd_, F_SETFL, flags | O_DIRECT) == -1) {
    PLOG(WARNING) << "Couldn't set O_DIRECT on fd " << fd_;
    return false;
  }

  uint64_t range[2] = {start, length};
  *result = ioctl(fd_, request, range);

  if ((flags & O_DIRECT) == 0 && fcntl(fd_, F_SETFL, flags) == -1) {
    PLOG(WARNING) << "Couldn't remove O_DIRECT on fd " << fd_;
    return false;
  }
  return true;
#endif  // defined(BLKZEROOUT)
}

bool EintrSafeFileDescriptor::Flush() {
  CHECK_GE(fd_, 0);
  return true;
}

bool EintrSafeFileDescriptor::Close() {
  CHECK_GE(fd_, 0);
  if (IGNORE_EINTR(close(fd_)))
    return false;
  fd_ = -1;
  return true;
}

}  // namespace chromeos_update_engine
