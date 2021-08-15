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

#ifndef NETDUTILS_SYSCALLS_H
#define NETDUTILS_SYSCALLS_H

#include <memory>

#include <net/if.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "netdutils/Fd.h"
#include "netdutils/Slice.h"
#include "netdutils/Socket.h"
#include "netdutils/Status.h"
#include "netdutils/StatusOr.h"
#include "netdutils/UniqueFd.h"
#include "netdutils/UniqueFile.h"

namespace android {
namespace netdutils {

class Syscalls {
  public:
    virtual ~Syscalls() = default;

    virtual StatusOr<UniqueFd> open(const std::string& pathname, int flags,
                                    mode_t mode = 0) const = 0;

    virtual StatusOr<UniqueFd> socket(int domain, int type, int protocol) const = 0;

    virtual Status getsockname(Fd sock, sockaddr* addr, socklen_t* addrlen) const = 0;

    virtual Status getsockopt(Fd sock, int level, int optname, void *optval,
                              socklen_t *optlen) const = 0;

    virtual Status setsockopt(Fd sock, int level, int optname, const void* optval,
                              socklen_t optlen) const = 0;

    virtual Status bind(Fd sock, const sockaddr* addr, socklen_t addrlen) const = 0;

    virtual Status connect(Fd sock, const sockaddr* addr, socklen_t addrlen) const = 0;

    virtual StatusOr<ifreq> ioctl(Fd sock, unsigned long request, ifreq* ifr) const = 0;

    virtual StatusOr<UniqueFd> eventfd(unsigned int initval, int flags) const = 0;

    virtual StatusOr<int> ppoll(pollfd* fds, nfds_t nfds, double timeout) const = 0;

    virtual StatusOr<size_t> writev(Fd fd, const std::vector<iovec>& iov) const = 0;

    virtual StatusOr<size_t> write(Fd fd, const Slice buf) const = 0;

    virtual StatusOr<Slice> read(Fd fd, const Slice buf) const = 0;

    virtual StatusOr<size_t> sendto(Fd sock, const Slice buf, int flags, const sockaddr* dst,
                                    socklen_t dstlen) const = 0;

    virtual StatusOr<Slice> recvfrom(Fd sock, const Slice dst, int flags, sockaddr* src,
                                     socklen_t* srclen) const = 0;

    virtual Status shutdown(Fd fd, int how) const = 0;

    virtual Status close(Fd fd) const = 0;

    virtual StatusOr<UniqueFile> fopen(const std::string& path, const std::string& mode) const = 0;

    virtual StatusOr<int> vfprintf(FILE* file, const char* format, va_list ap) const = 0;

    virtual StatusOr<int> vfscanf(FILE* file, const char* format, va_list ap) const = 0;

    virtual Status fclose(FILE* file) const = 0;

    virtual StatusOr<pid_t> fork() const = 0;

    // va_args helpers
    // va_start doesn't work when the preceding argument is a reference
    // type so we're forced to use const char*.
    StatusOr<int> fprintf(FILE* file, const char* format, ...) const {
        va_list ap;
        va_start(ap, format);
        auto result = vfprintf(file, format, ap);
        va_end(ap);
        return result;
    }

    // va_start doesn't work when the preceding argument is a reference
    // type so we're forced to use const char*.
    StatusOr<int> fscanf(FILE* file, const char* format, ...) const {
        va_list ap;
        va_start(ap, format);
        auto result = vfscanf(file, format, ap);
        va_end(ap);
        return result;
    }

    // Templated helpers that forward directly to methods declared above
    template <typename SockaddrT>
    StatusOr<SockaddrT> getsockname(Fd sock) const {
        SockaddrT addr = {};
        socklen_t addrlen = sizeof(addr);
        RETURN_IF_NOT_OK(getsockname(sock, asSockaddrPtr(&addr), &addrlen));
        return addr;
    }

    template <typename SockoptT>
    Status getsockopt(Fd sock, int level, int optname, void* optval, socklen_t* optlen) const {
        return getsockopt(sock, level, optname, optval, optlen);
    }

    template <typename SockoptT>
    Status setsockopt(Fd sock, int level, int optname, const SockoptT& opt) const {
        return setsockopt(sock, level, optname, &opt, sizeof(opt));
    }

    template <typename SockaddrT>
    Status bind(Fd sock, const SockaddrT& addr) const {
        return bind(sock, asSockaddrPtr(&addr), sizeof(addr));
    }

    template <typename SockaddrT>
    Status connect(Fd sock, const SockaddrT& addr) const {
        return connect(sock, asSockaddrPtr(&addr), sizeof(addr));
    }

    template <size_t size>
    StatusOr<std::array<uint16_t, size>> ppoll(const std::array<Fd, size>& fds, uint16_t events,
                                               double timeout) const {
        std::array<pollfd, size> tmp;
        for (size_t i = 0; i < size; ++i) {
            tmp[i].fd = fds[i].get();
            tmp[i].events = events;
            tmp[i].revents = 0;
        }
        RETURN_IF_NOT_OK(ppoll(tmp.data(), tmp.size(), timeout).status());
        std::array<uint16_t, size> out;
        for (size_t i = 0; i < size; ++i) {
            out[i] = tmp[i].revents;
        }
        return out;
    }

    template <typename SockaddrT>
    StatusOr<size_t> sendto(Fd sock, const Slice buf, int flags, const SockaddrT& dst) const {
        return sendto(sock, buf, flags, asSockaddrPtr(&dst), sizeof(dst));
    }

    // Ignore src sockaddr
    StatusOr<Slice> recvfrom(Fd sock, const Slice dst, int flags) const {
        return recvfrom(sock, dst, flags, nullptr, nullptr);
    }

    template <typename SockaddrT>
    StatusOr<std::pair<Slice, SockaddrT>> recvfrom(Fd sock, const Slice dst, int flags) const {
        SockaddrT addr = {};
        socklen_t addrlen = sizeof(addr);
        ASSIGN_OR_RETURN(auto used, recvfrom(sock, dst, flags, asSockaddrPtr(&addr), &addrlen));
        return std::make_pair(used, addr);
    }
};

// Specialized singleton that supports zero initialization and runtime
// override of contained pointer.
class SyscallsHolder {
  public:
    ~SyscallsHolder();

    // Return a pointer to an unowned instance of Syscalls.
    Syscalls& get();

    // Testing only: set the value returned by getSyscalls. Return the old value.
    // Callers are responsible for restoring the previous value returned
    // by getSyscalls to avoid leaks.
    Syscalls& swap(Syscalls& syscalls);

  private:
    std::atomic<Syscalls*> mSyscalls{nullptr};
};

// Syscalls instance used throughout netdutils
extern SyscallsHolder sSyscalls;

}  // namespace netdutils
}  // namespace android

#endif /* NETDUTILS_SYSCALLS_H */
