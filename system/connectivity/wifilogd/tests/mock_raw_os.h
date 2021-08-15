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

#ifndef TESTS_MOCK_RAW_OS_H_
#define TESTS_MOCK_RAW_OS_H_

#include "android-base/macros.h"
#include "gmock/gmock.h"

#include "wifilogd/raw_os.h"

namespace android {
namespace wifilogd {

class MockRawOs : public RawOs {
 public:
  MockRawOs();
  virtual ~MockRawOs();

  MOCK_CONST_METHOD2(ClockGettime,
                     int(clockid_t clock_id, struct timespec* tspec));
  MOCK_METHOD1(GetControlSocket, int(const char* socket_name));
  MOCK_METHOD2(Nanosleep,
               int(const struct timespec* req, struct timespec* rem));
  MOCK_METHOD4(Recv, ssize_t(int sockfd, void* buf, size_t buflen, int flags));
  MOCK_METHOD3(Write, ssize_t(int fd, const void* buf, size_t buflen));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockRawOs);
};

}  // namespace wifilogd
}  // namespace android

#endif  // TESTS_MOCK_RAW_OS_H_
