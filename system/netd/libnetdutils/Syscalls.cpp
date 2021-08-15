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

#include "netdutils/Syscalls.h"

#include <atomic>
#include <type_traits>
#include <utility>

namespace android {
namespace netdutils {
namespace {

// Retry syscall fn as long as it returns -1 with errno == EINTR
template <typename FnT, typename... Params>
typename std::result_of<FnT(Params...)>::type syscallRetry(FnT fn, Params&&... params) {
    auto rv = fn(std::forward<Params>(params)...);
    while ((rv == -1) && (errno == EINTR)) {
        rv = fn(std::forward<Params>(params)...);
    }
    return rv;
}

}  // namespace

// Production implementation of Syscalls that forwards to libc syscalls.
class RealSyscalls final : public Syscalls {
  public:
    ~RealSyscalls() override = default;

    StatusOr<UniqueFd> open(const std::string& pathname, int flags, mode_t mode) const override {
        UniqueFd fd(::open(pathname.c_str(), flags, mode));
        if (!isWellFormed(fd)) {
            return statusFromErrno(errno, "open(\"" + pathname + "\"...) failed");
        }
        return fd;
    }

    StatusOr<UniqueFd> socket(int domain, int type, int protocol) const override {
        UniqueFd sock(::socket(domain, type, protocol));
        if (!isWellFormed(sock)) {
            return statusFromErrno(errno, "socket() failed");
        }
        return sock;
    }

    Status getsockname(Fd sock, sockaddr* addr, socklen_t* addrlen) const override {
        auto rv = ::getsockname(sock.get(), addr, addrlen);
        if (rv == -1) {
            return statusFromErrno(errno, "getsockname() failed");
        }
        return status::ok;
    }

    Status getsockopt(Fd sock, int level, int optname, void* optval,
                      socklen_t* optlen) const override {
        auto rv = ::getsockopt(sock.get(), level, optname, optval, optlen);
        if (rv == -1) {
            return statusFromErrno(errno, "getsockopt() failed");
        }
        return status::ok;
    }

    Status setsockopt(Fd sock, int level, int optname, const void* optval,
                      socklen_t optlen) const override {
        auto rv = ::setsockopt(sock.get(), level, optname, optval, optlen);
        if (rv == -1) {
            return statusFromErrno(errno, "setsockopt() failed");
        }
        return status::ok;
    }

    Status bind(Fd sock, const sockaddr* addr, socklen_t addrlen) const override {
        auto rv = ::bind(sock.get(), addr, addrlen);
        if (rv == -1) {
            return statusFromErrno(errno, "bind() failed");
        }
        return status::ok;
    }

    Status connect(Fd sock, const sockaddr* addr, socklen_t addrlen) const override {
        auto rv = syscallRetry(::connect, sock.get(), addr, addrlen);
        if (rv == -1) {
            return statusFromErrno(errno, "connect() failed");
        }
        return status::ok;
    }

    StatusOr<ifreq> ioctl(Fd sock, unsigned long request, ifreq* ifr) const override {
        auto rv = ::ioctl(sock.get(), request, ifr);
        if (rv == -1) {
            return statusFromErrno(errno, "ioctl() failed");
        }
        return *ifr;
    }

    StatusOr<UniqueFd> eventfd(unsigned int initval, int flags) const override {
        UniqueFd fd(::eventfd(initval, flags));
        if (!isWellFormed(fd)) {
            return statusFromErrno(errno, "eventfd() failed");
        }
        return fd;
    }

    StatusOr<int> ppoll(pollfd* fds, nfds_t nfds, double timeout) const override {
        timespec ts = {};
        ts.tv_sec = timeout;
        ts.tv_nsec = (timeout - ts.tv_sec) * 1e9;
        auto rv = syscallRetry(::ppoll, fds, nfds, &ts, nullptr);
        if (rv == -1) {
            return statusFromErrno(errno, "ppoll() failed");
        }
        return rv;
    }

    StatusOr<size_t> writev(Fd fd, const std::vector<iovec>& iov) const override {
        auto rv = syscallRetry(::writev, fd.get(), iov.data(), iov.size());
        if (rv == -1) {
            return statusFromErrno(errno, "writev() failed");
        }
        return rv;
    }

    StatusOr<size_t> write(Fd fd, const Slice buf) const override {
        auto rv = syscallRetry(::write, fd.get(), buf.base(), buf.size());
        if (rv == -1) {
            return statusFromErrno(errno, "write() failed");
        }
        return static_cast<size_t>(rv);
    }

    StatusOr<Slice> read(Fd fd, const Slice buf) const override {
        auto rv = syscallRetry(::read, fd.get(), buf.base(), buf.size());
        if (rv == -1) {
            return statusFromErrno(errno, "read() failed");
        }
        return Slice(buf.base(), rv);
    }

    StatusOr<size_t> sendto(Fd sock, const Slice buf, int flags, const sockaddr* dst,
                            socklen_t dstlen) const override {
        auto rv = syscallRetry(::sendto, sock.get(), buf.base(), buf.size(), flags, dst, dstlen);
        if (rv == -1) {
            return statusFromErrno(errno, "sendto() failed");
        }
        return static_cast<size_t>(rv);
    }

    StatusOr<Slice> recvfrom(Fd sock, const Slice dst, int flags, sockaddr* src,
                             socklen_t* srclen) const override {
        auto rv = syscallRetry(::recvfrom, sock.get(), dst.base(), dst.size(), flags, src, srclen);
        if (rv == -1) {
            return statusFromErrno(errno, "recvfrom() failed");
        }
        if (rv == 0) {
            return status::eof;
        }
        return take(dst, rv);
    }

    Status shutdown(Fd fd, int how) const override {
        auto rv = ::shutdown(fd.get(), how);
        if (rv == -1) {
            return statusFromErrno(errno, "shutdown() failed");
        }
        return status::ok;
    }

    Status close(Fd fd) const override {
        auto rv = ::close(fd.get());
        if (rv == -1) {
            return statusFromErrno(errno, "close() failed");
        }
        return status::ok;
    }

    StatusOr<UniqueFile> fopen(const std::string& path, const std::string& mode) const override {
        UniqueFile file(::fopen(path.c_str(), mode.c_str()));
        if (file == nullptr) {
            return statusFromErrno(errno, "fopen(\"" + path + "\", \"" + mode + "\") failed");
        }
        return file;
    }

    StatusOr<pid_t> fork() const override {
        pid_t rv = ::fork();
        if (rv == -1) {
            return statusFromErrno(errno, "fork() failed");
        }
        return rv;
    }

    StatusOr<int> vfprintf(FILE* file, const char* format, va_list ap) const override {
        auto rv = ::vfprintf(file, format, ap);
        if (rv == -1) {
            return statusFromErrno(errno, "vfprintf() failed");
        }
        return rv;
    }

    StatusOr<int> vfscanf(FILE* file, const char* format, va_list ap) const override {
        auto rv = ::vfscanf(file, format, ap);
        if (rv == -1) {
            return statusFromErrno(errno, "vfscanf() failed");
        }
        return rv;
    }

    Status fclose(FILE* file) const override {
        auto rv = ::fclose(file);
        if (rv == -1) {
            return statusFromErrno(errno, "fclose() failed");
        }
        return status::ok;
    }
};

SyscallsHolder::~SyscallsHolder() {
    delete &get();
}

Syscalls& SyscallsHolder::get() {
    while (true) {
        // memory_order_relaxed gives the compiler and hardware more
        // freedom. If we get a stale value (this should only happen
        // early in the execution of a program) the exchange code below
        // will loop until we get the most current value.
        auto* syscalls = mSyscalls.load(std::memory_order_relaxed);
        // Common case returns existing syscalls
        if (syscalls) {
            return *syscalls;
        }

        // This code will execute on first get()
        std::unique_ptr<Syscalls> tmp(new RealSyscalls());
        Syscalls* expected = nullptr;
        bool success = mSyscalls.compare_exchange_strong(expected, tmp.get());
        if (success) {
            // Ownership was transferred to mSyscalls already, must release()
            return *tmp.release();
        }
    }
}

Syscalls& SyscallsHolder::swap(Syscalls& syscalls) {
    return *mSyscalls.exchange(&syscalls);
}

SyscallsHolder sSyscalls;

}  // namespace netdutils
}  // namespace android
