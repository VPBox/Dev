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

#include <time.h>
#include <unistd.h>

#include "cutils/sockets.h"

#include "wifilogd/raw_os.h"

namespace android {
namespace wifilogd {

RawOs::RawOs() {}
RawOs::~RawOs() {}

// All methods in RawOs should be thin wrappers over library/system calls.
// In particular, the methods in RawOs should not have any logic in them.
// Instead, all logic should be placed in Os, so that said logic can be tested.

int RawOs::ClockGettime(clockid_t clock_id, struct timespec* ts) const {
  return clock_gettime(clock_id, ts);
}

int RawOs::GetControlSocket(const char* socket_name) {
  return android_get_control_socket(socket_name);
}

int RawOs::Nanosleep(const struct timespec* req, struct timespec* rem) {
  return nanosleep(req, rem);
}

ssize_t RawOs::Recv(int sockfd, void* buf, size_t buflen, int flags) {
  return recv(sockfd, buf, buflen, flags);
}

ssize_t RawOs::Write(int fd, const void* buf, size_t buflen) {
  return write(fd, buf, buflen);
}

}  // namespace wifilogd
}  // namespace android
