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

#ifndef OS_H_
#define OS_H_

#include <time.h>

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "android-base/macros.h"

#include "wifilogd/local_utils.h"
#include "wifilogd/raw_os.h"

namespace android {
namespace wifilogd {

// Abstracts operating system calls.
//
// There are three reasons we want to abstract OS calls:
// 1. Allow tests to run hermetically.
// 2. Verify that the application logic invokes the OS calls as expected.
// 3. Provide interfaces that as easier to use, than the underlying OS calls.
class Os {
 public:
  using Errno = int;

  struct Timestamp {
    uint32_t secs;  // Sufficient through 2100.
    uint32_t nsecs;
  };

  static constexpr int kInvalidFd = -1;
  static constexpr auto kMaxNanos = 999'999'999;

  // Constructs an Os instance.
  Os();

  // Constructs an Os instance, with the caller-provided RawOs. This method
  // allows tests to provide a MockRawOs.
  explicit Os(std::unique_ptr<RawOs> raw_os);

  virtual ~Os();

  // Returns the Android control socket with name |socket_name|. If no such
  // socket exists, or the init daemon has not provided this process with
  // access to said socket, returns {kInvalidFd, errno}.
  virtual std::tuple<int, Errno> GetControlSocket(
      const std::string& socket_name);

  // Returns the current time, as reported by the clock with |clock_id|.
  virtual Timestamp GetTimestamp(clockid_t clock_id) const;

  // Suspends execution of this process, for |sleep_time_nsec|. The passed
  // value must not exceed kMaxNanos.
  virtual void Nanosleep(uint32_t sleep_time_nsec);

  // Receives a datagram of up to |buflen| from |fd|, writing the data to |buf|.
  // Returns the size of the datagram, and the result of the operation (0 for
  // success, |errno| otherwise).
  //
  // Notes:
  // - |buflen| may not exceed the maximal value for ssize_t.
  // - The call blocks until a datagram is available.
  // - If the datagram is larger than |buflen|, only |buflen| bytes will
  //   be received. The returned size_t will, however, reflect the full
  //   length of the datagram.
  virtual std::tuple<size_t, Errno> ReceiveDatagram(int fd, NONNULL void* buf,
                                                    size_t buflen);

  // Writes |buflen| bytes from |buf| to |fd|. Returns the number of bytes
  // written, and the result of the operation (0 for success, |errno|
  // otherwise).
  //
  // Notes:
  // - |buflen| may not exceed the maximal value for ssize_t.
  // - The returned size_t will not exceed |buflen|.
  virtual std::tuple<size_t, Errno> Write(int fd, NONNULL const void* buf,
                                          size_t buflen);

 private:
  const std::unique_ptr<RawOs> raw_os_;

  DISALLOW_COPY_AND_ASSIGN(Os);
};

}  // namespace wifilogd
}  // namespace android

#endif  // OS_H_
