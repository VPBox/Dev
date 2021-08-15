/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef NETUTILS_MOCK_SYSCALLS_H
#define NETUTILS_MOCK_SYSCALLS_H

#include <atomic>
#include <cassert>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "netdutils/Syscalls.h"

namespace android {
namespace netdutils {

class MockSyscalls : public Syscalls {
  public:
    virtual ~MockSyscalls() = default;
    // Use Return(ByMove(...)) to deal with movable return types.
    MOCK_CONST_METHOD3(open,
                       StatusOr<UniqueFd>(const std::string& pathname, int flags, mode_t mode));
    MOCK_CONST_METHOD3(socket, StatusOr<UniqueFd>(int domain, int type, int protocol));
    MOCK_CONST_METHOD3(getsockname, Status(Fd sock, sockaddr* addr, socklen_t* addrlen));
    MOCK_CONST_METHOD5(getsockopt, Status(Fd sock, int level, int optname, void* optval,
                                          socklen_t *optlen));
    MOCK_CONST_METHOD5(setsockopt, Status(Fd sock, int level, int optname, const void* optval,
                                          socklen_t optlen));

    MOCK_CONST_METHOD3(bind, Status(Fd sock, const sockaddr* addr, socklen_t addrlen));
    MOCK_CONST_METHOD3(connect, Status(Fd sock, const sockaddr* addr, socklen_t addrlen));
    MOCK_CONST_METHOD3(ioctl, StatusOr<ifreq>(Fd sock, unsigned long request, ifreq* ifr));

    // Use Return(ByMove(...)) to deal with movable return types.
    MOCK_CONST_METHOD2(eventfd, StatusOr<UniqueFd>(unsigned int initval, int flags));
    MOCK_CONST_METHOD3(ppoll, StatusOr<int>(pollfd* fds, nfds_t nfds, double timeout));

    MOCK_CONST_METHOD2(writev, StatusOr<size_t>(Fd fd, const std::vector<iovec>& iov));
    MOCK_CONST_METHOD2(write, StatusOr<size_t>(Fd fd, const Slice buf));
    MOCK_CONST_METHOD2(read, StatusOr<Slice>(Fd fd, const Slice buf));
    MOCK_CONST_METHOD5(sendto, StatusOr<size_t>(Fd sock, const Slice buf, int flags,
                                                const sockaddr* dst, socklen_t dstlen));
    MOCK_CONST_METHOD5(recvfrom, StatusOr<Slice>(Fd sock, const Slice dst, int flags, sockaddr* src,
                                                 socklen_t* srclen));
    MOCK_CONST_METHOD2(shutdown, Status(Fd fd, int how));
    MOCK_CONST_METHOD1(close, Status(Fd fd));

    MOCK_CONST_METHOD2(fopen,
                       StatusOr<UniqueFile>(const std::string& path, const std::string& mode));
    MOCK_CONST_METHOD3(vfprintf, StatusOr<int>(FILE* file, const char* format, va_list ap));
    MOCK_CONST_METHOD3(vfscanf, StatusOr<int>(FILE* file, const char* format, va_list ap));
    MOCK_CONST_METHOD1(fclose, Status(FILE* file));
    MOCK_CONST_METHOD0(fork, StatusOr<pid_t>());
};

// For the lifetime of this mock, replace the contents of sSyscalls
// with a pointer to this mock. Behavior is undefined if multiple
// ScopedMockSyscalls instances exist concurrently.
class ScopedMockSyscalls : public MockSyscalls {
  public:
    ScopedMockSyscalls() : mOld(sSyscalls.swap(*this)) { assert((mRefcount++) == 1); }
    virtual ~ScopedMockSyscalls() {
        sSyscalls.swap(mOld);
        assert((mRefcount--) == 0);
    }

  private:
    std::atomic<int> mRefcount{0};
    Syscalls& mOld;
};

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_MOCK_SYSCALLS_H */
