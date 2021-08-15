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

#ifndef RAW_OS_H_
#define RAW_OS_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include "android-base/macros.h"

#include "wifilogd/local_utils.h"

namespace android {
namespace wifilogd {

// Enables interposing on operating system calls.
class RawOs {
 public:
  RawOs();
  virtual ~RawOs();

  // See clock_gettime().
  virtual int ClockGettime(clockid_t clock_id,
                           NONNULL struct timespec* tspec) const;

  // See android_get_control_socket().
  virtual int GetControlSocket(const char* socket_name);

  // See nanosleep().
  virtual int Nanosleep(NONNULL const struct timespec* req,
                        struct timespec* rem);

  // See recv().
  virtual ssize_t Recv(int sockfd, void* buf, size_t buflen, int flags);

  // See write().
  virtual ssize_t Write(int fd, const void* buf, size_t buflen);

 private:
  DISALLOW_COPY_AND_ASSIGN(RawOs);
};

}  // namespace wifilogd
}  // namespace android

#endif  // RAW_OS_H_
