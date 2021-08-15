/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstring>

#include "android-base/logging.h"

#include "wifilogd/local_utils.h"
#include "wifilogd/os.h"

namespace android {
namespace wifilogd {

using local_utils::GetMaxVal;

namespace {
constexpr auto kMaxNanoSeconds = 1000 * 1000 * 1000 - 1;
}

constexpr int Os::kInvalidFd;

Os::Os() : raw_os_(new RawOs()) {}
Os::Os(std::unique_ptr<RawOs> raw_os) : raw_os_(std::move(raw_os)) {}
Os::~Os() {}

std::tuple<int, Os::Errno> Os::GetControlSocket(
    const std::string& socket_name) {
  int sock_fd = raw_os_->GetControlSocket(socket_name.c_str());
  if (sock_fd < 0) {
    return {kInvalidFd, errno};
  } else {
    return {sock_fd, 0};
  }
}

Os::Timestamp Os::GetTimestamp(clockid_t clock_id) const {
  struct timespec now_timespec;
  int failed = raw_os_->ClockGettime(clock_id, &now_timespec);
  if (failed) {
    LOG(FATAL) << "Unexpected error: " << std::strerror(errno);
  }
  CHECK(now_timespec.tv_nsec <= kMaxNanoSeconds);

  Timestamp now_timestamp;
  now_timestamp.secs = SAFELY_CLAMP(
      now_timespec.tv_sec, uint32_t, 0,
      // The upper-bound comes from the source-type on 32-bit platforms,
      // and the dest-type on 64-bit platforms. Using min(), we can figure out
      // which type to use for the upper bound, without resorting to macros.
      std::min(static_cast<uintmax_t>(GetMaxVal(now_timestamp.secs)),
               static_cast<uintmax_t>(GetMaxVal(now_timespec.tv_sec))));
  now_timestamp.nsecs =
      SAFELY_CLAMP(now_timespec.tv_nsec, uint32_t, 0, kMaxNanoSeconds);
  return now_timestamp;
}

void Os::Nanosleep(uint32_t sleep_time_nsec) {
  struct timespec sleep_timespec = {
      0,  // tv_sec
      SAFELY_CLAMP(sleep_time_nsec, decltype(timespec::tv_nsec), 0, kMaxNanos)};

  int failed = 0;
  do {
    struct timespec remaining_timespec;
    failed = raw_os_->Nanosleep(&sleep_timespec, &remaining_timespec);
    sleep_timespec = remaining_timespec;
  } while (failed && errno == EINTR && sleep_timespec.tv_nsec > 0);

  if (failed && errno != EINTR) {
    // The only other documented errors for the underlying nanosleep() call are
    // EFAULT and EINVAL. But we always pass valid pointers, and the values in
    // |sleep_timespec| are always valid.
    LOG(FATAL) << "Unexpected error: " << std::strerror(errno);
  }
}

std::tuple<size_t, Os::Errno> Os::ReceiveDatagram(int fd, void* buf,
                                                  size_t buflen) {
  // recv() takes a size_t, but returns an ssize_t. That means that the largest
  // successful read that recv() can report is the maximal ssize_t. Passing a
  // larger |buflen| risks mistakenly reporting a truncated read.
  CHECK(buflen <= GetMaxVal<ssize_t>());

  const ssize_t res = raw_os_->Recv(fd, buf, buflen, MSG_TRUNC);
  if (res < 0) {
    return {0, errno};
  }

  // Due to the MSG_TRUNC flag, |res| may reasonably be larger than
  // |buflen|. In such cases, |res| indicates the full size of the datagram,
  // before being truncated to fit our buffer. Hence, we omit the
  // buffer-overflow CHECK that exists in Write().
  return {res, 0};
}

std::tuple<size_t, Os::Errno> Os::Write(int fd, const void* buf,
                                        size_t buflen) {
  // write() takes a size_t, but returns an ssize_t. That means that the
  // largest successful write that write() can report is the maximal ssize_t.
  // Passing a larger |buflen| risks mistakenly reporting a truncated write.
  CHECK(buflen <= GetMaxVal<ssize_t>());

  const ssize_t res = raw_os_->Write(fd, buf, buflen);
  if (res < 0) {
    return {0, errno};
  }

  CHECK(res <=
        SAFELY_CLAMP(buflen, ssize_t, 0,
                     GetMaxVal<ssize_t>()));  // Abort on buffer overflow.

  // Note that |res| may be less than buflen. However, a) a short write is
  // not an error, and b) |errno| may be stale, as |errno| is only guaranteed to
  // be set if an error occurred. Hence, we return Errno of 0 unconditionally.
  // See http://yarchive.net/comp/linux/write_error_return.html
  return {res, 0};
}

}  // namespace wifilogd
}  // namespace android
