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

#ifndef TESTS_MOCK_OS_H_
#define TESTS_MOCK_OS_H_

#include <string>
#include <tuple>

#include "android-base/macros.h"
#include "gmock/gmock.h"

#include "wifilogd/os.h"

namespace android {
namespace wifilogd {

class MockOs : public Os {
 public:
  MockOs();
  ~MockOs() override;

  MOCK_CONST_METHOD1(GetTimestamp, Timestamp(clockid_t clock_id));
  MOCK_METHOD1(GetControlSocket,
               std::tuple<int, Errno>(const std::string& socket_name));
  MOCK_METHOD1(Nanosleep, void(uint32_t sleep_time_nsec));
  MOCK_METHOD3(ReceiveDatagram,
               std::tuple<size_t, Errno>(int fd, void* buf, size_t buflen));
  MOCK_METHOD3(Write, std::tuple<size_t, Os::Errno>(int fd, const void* buf,
                                                    size_t buflen));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockOs);
};

}  // namespace wifilogd
}  // namespace android

#endif  // TESTS_MOCK_OS_H_
