/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "NetdClient.h"

#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <resolv.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <atomic>
#include <string>
#include <vector>

#include <android-base/parseint.h>
#include <android-base/unique_fd.h>

#include "Fwmark.h"
#include "FwmarkClient.h"
#include "FwmarkCommand.h"
#include "netdclient_priv.h"
#include "netdutils/ResponseCode.h"
#include "netdutils/Stopwatch.h"
#include "netid_client.h"

using android::base::ParseInt;
using android::base::unique_fd;
using android::netdutils::ResponseCode;
using android::netdutils::Stopwatch;

namespace {

// Keep this in sync with CMD_BUF_SIZE in FrameworkListener.cpp.
constexpr size_t MAX_CMD_SIZE = 4096;

std::atomic_uint netIdForProcess(NETID_UNSET);
std::atomic_uint netIdForResolv(NETID_UNSET);

typedef int (*Accept4FunctionType)(int, sockaddr*, socklen_t*, int);
typedef int (*ConnectFunctionType)(int, const sockaddr*, socklen_t);
typedef int (*SocketFunctionType)(int, int, int);
typedef unsigned (*NetIdForResolvFunctionType)(unsigned);
typedef int (*DnsOpenProxyType)();

// These variables are only modified at startup (when libc.so is loaded) and never afterwards, so
// it's okay that they are read later at runtime without a lock.
Accept4FunctionType libcAccept4 = nullptr;
ConnectFunctionType libcConnect = nullptr;
SocketFunctionType libcSocket = nullptr;

int checkSocket(int socketFd) {
    if (socketFd < 0) {
        return -EBADF;
    }
    int family;
    socklen_t familyLen = sizeof(family);
    if (getsockopt(socketFd, SOL_SOCKET, SO_DOMAIN, &family, &familyLen) == -1) {
        return -errno;
    }
    if (!FwmarkClient::shouldSetFwmark(family)) {
        return -EAFNOSUPPORT;
    }
    return 0;
}

bool shouldMarkSocket(int socketFd, const sockaddr* dst) {
    // Only mark inet sockets that are connecting to inet destinations. This excludes, for example,
    // inet sockets connecting to AF_UNSPEC (i.e., being disconnected), and non-inet sockets that
    // for some reason the caller wants to attempt to connect to an inet destination.
    return dst && FwmarkClient::shouldSetFwmark(dst->sa_family) && (checkSocket(socketFd) == 0);
}

int closeFdAndSetErrno(int fd, int error) {
    close(fd);
    errno = -error;
    return -1;
}

int netdClientAccept4(int sockfd, sockaddr* addr, socklen_t* addrlen, int flags) {
    int acceptedSocket = libcAccept4(sockfd, addr, addrlen, flags);
    if (acceptedSocket == -1) {
        return -1;
    }
    int family;
    if (addr) {
        family = addr->sa_family;
    } else {
        socklen_t familyLen = sizeof(family);
        if (getsockopt(acceptedSocket, SOL_SOCKET, SO_DOMAIN, &family, &familyLen) == -1) {
            return closeFdAndSetErrno(acceptedSocket, -errno);
        }
    }
    if (FwmarkClient::shouldSetFwmark(family)) {
        FwmarkCommand command = {FwmarkCommand::ON_ACCEPT, 0, 0, 0};
        if (int error = FwmarkClient().send(&command, acceptedSocket, nullptr)) {
            return closeFdAndSetErrno(acceptedSocket, error);
        }
    }
    return acceptedSocket;
}

int netdClientConnect(int sockfd, const sockaddr* addr, socklen_t addrlen) {
    const bool shouldSetFwmark = shouldMarkSocket(sockfd, addr);
    if (shouldSetFwmark) {
        FwmarkCommand command = {FwmarkCommand::ON_CONNECT, 0, 0, 0};
        if (int error = FwmarkClient().send(&command, sockfd, nullptr)) {
            errno = -error;
            return -1;
        }
    }
    // Latency measurement does not include time of sending commands to Fwmark
    Stopwatch s;
    const int ret = libcConnect(sockfd, addr, addrlen);
    // Save errno so it isn't clobbered by sending ON_CONNECT_COMPLETE
    const int connectErrno = errno;
    const unsigned latencyMs = lround(s.timeTaken());
    // Send an ON_CONNECT_COMPLETE command that includes sockaddr and connect latency for reporting
    if (shouldSetFwmark) {
        FwmarkConnectInfo connectInfo(ret == 0 ? 0 : connectErrno, latencyMs, addr);
        // TODO: get the netId from the socket mark once we have continuous benchmark runs
        FwmarkCommand command = {FwmarkCommand::ON_CONNECT_COMPLETE, /* netId (ignored) */ 0,
                                 /* uid (filled in by the server) */ 0, 0};
        // Ignore return value since it's only used for logging
        FwmarkClient().send(&command, sockfd, &connectInfo);
    }
    errno = connectErrno;
    return ret;
}

int netdClientSocket(int domain, int type, int protocol) {
    int socketFd = libcSocket(domain, type, protocol);
    if (socketFd == -1) {
        return -1;
    }
    unsigned netId = netIdForProcess & ~NETID_USE_LOCAL_NAMESERVERS;
    if (netId != NETID_UNSET && FwmarkClient::shouldSetFwmark(domain)) {
        if (int error = setNetworkForSocket(netId, socketFd)) {
            return closeFdAndSetErrno(socketFd, error);
        }
    }
    return socketFd;
}

unsigned getNetworkForResolv(unsigned netId) {
    if (netId != NETID_UNSET) {
        return netId;
    }
    // Special case for DNS-over-TLS bypass; b/72345192 .
    if ((netIdForResolv & ~NETID_USE_LOCAL_NAMESERVERS) != NETID_UNSET) {
        return netIdForResolv;
    }
    netId = netIdForProcess;
    if (netId != NETID_UNSET) {
        return netId;
    }
    return netIdForResolv;
}

int setNetworkForTarget(unsigned netId, std::atomic_uint* target) {
    const unsigned requestedNetId = netId;
    netId &= ~NETID_USE_LOCAL_NAMESERVERS;

    if (netId == NETID_UNSET) {
        *target = netId;
        return 0;
    }
    // Verify that we are allowed to use |netId|, by creating a socket and trying to have it marked
    // with the netId. Call libcSocket() directly; else the socket creation (via netdClientSocket())
    // might itself cause another check with the fwmark server, which would be wasteful.

    const auto socketFunc = libcSocket ? libcSocket : socket;
    int socketFd = socketFunc(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (socketFd < 0) {
        return -errno;
    }
    int error = setNetworkForSocket(netId, socketFd);
    if (!error) {
        *target = requestedNetId;
    }
    close(socketFd);
    return error;
}

int dns_open_proxy() {
    const char* cache_mode = getenv("ANDROID_DNS_MODE");
    const bool use_proxy = (cache_mode == NULL || strcmp(cache_mode, "local") != 0);
    if (!use_proxy) {
        errno = ENOSYS;
        return -1;
    }

    const auto socketFunc = libcSocket ? libcSocket : socket;
    int s = socketFunc(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (s == -1) {
        return -1;
    }
    const int one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    static const struct sockaddr_un proxy_addr = {
            .sun_family = AF_UNIX,
            .sun_path = "/dev/socket/dnsproxyd",
    };

    const auto connectFunc = libcConnect ? libcConnect : connect;
    if (TEMP_FAILURE_RETRY(
                connectFunc(s, (const struct sockaddr*) &proxy_addr, sizeof(proxy_addr))) != 0) {
        // Store the errno for connect because we only care about why we can't connect to dnsproxyd
        int storedErrno = errno;
        close(s);
        errno = storedErrno;
        return -1;
    }

    return s;
}

auto divCeil(size_t dividend, size_t divisor) {
    return ((dividend + divisor - 1) / divisor);
}

// FrameworkListener only does only read() call, and fails if the read doesn't contain \0
// Do single write here
int sendData(int fd, const void* buf, size_t size) {
    if (fd < 0) {
        return -EBADF;
    }

    ssize_t rc = TEMP_FAILURE_RETRY(write(fd, (char*) buf, size));
    if (rc > 0) {
        return rc;
    } else if (rc == 0) {
        return -EIO;
    } else {
        return -errno;
    }
}

int readData(int fd, void* buf, size_t size) {
    if (fd < 0) {
        return -EBADF;
    }

    size_t current = 0;
    for (;;) {
        ssize_t rc = TEMP_FAILURE_RETRY(read(fd, (char*) buf + current, size - current));
        if (rc > 0) {
            current += rc;
            if (current == size) {
                break;
            }
        } else if (rc == 0) {
            return -EIO;
        } else {
            return -errno;
        }
    }
    return 0;
}

bool readBE32(int fd, int32_t* result) {
    int32_t tmp;
    ssize_t n = TEMP_FAILURE_RETRY(read(fd, &tmp, sizeof(tmp)));
    if (n < static_cast<ssize_t>(sizeof(tmp))) {
        return false;
    }
    *result = ntohl(tmp);
    return true;
}

bool readResponseCode(int fd, int* result) {
    char buf[4];
    ssize_t n = TEMP_FAILURE_RETRY(read(fd, &buf, sizeof(buf)));
    if (n < static_cast<ssize_t>(sizeof(buf))) {
        return false;
    }

    // The format of response code is 3 bytes followed by a space.
    buf[3] = '\0';
    if (!ParseInt(buf, result)) {
        errno = EINVAL;
        return false;
    }

    return true;
}

}  // namespace

#define CHECK_SOCKET_IS_MARKABLE(sock)          \
    do {                                        \
        int err;                                \
        if ((err = checkSocket(sock)) != 0) {   \
            return err;                         \
        }                                       \
    } while (false);

// accept() just calls accept4(..., 0), so there's no need to handle accept() separately.
extern "C" void netdClientInitAccept4(Accept4FunctionType* function) {
    if (function && *function) {
        libcAccept4 = *function;
        *function = netdClientAccept4;
    }
}

extern "C" void netdClientInitConnect(ConnectFunctionType* function) {
    if (function && *function) {
        libcConnect = *function;
        *function = netdClientConnect;
    }
}

extern "C" void netdClientInitSocket(SocketFunctionType* function) {
    if (function && *function) {
        libcSocket = *function;
        *function = netdClientSocket;
    }
}

extern "C" void netdClientInitNetIdForResolv(NetIdForResolvFunctionType* function) {
    if (function) {
        *function = getNetworkForResolv;
    }
}

extern "C" void netdClientInitDnsOpenProxy(DnsOpenProxyType* function) {
    if (function) {
        *function = dns_open_proxy;
    }
}

extern "C" int getNetworkForSocket(unsigned* netId, int socketFd) {
    if (!netId || socketFd < 0) {
        return -EBADF;
    }
    Fwmark fwmark;
    socklen_t fwmarkLen = sizeof(fwmark.intValue);
    if (getsockopt(socketFd, SOL_SOCKET, SO_MARK, &fwmark.intValue, &fwmarkLen) == -1) {
        return -errno;
    }
    *netId = fwmark.netId;
    return 0;
}

extern "C" unsigned getNetworkForProcess() {
    return netIdForProcess & ~NETID_USE_LOCAL_NAMESERVERS;
}

extern "C" int setNetworkForSocket(unsigned netId, int socketFd) {
    CHECK_SOCKET_IS_MARKABLE(socketFd);
    FwmarkCommand command = {FwmarkCommand::SELECT_NETWORK, netId, 0, 0};
    return FwmarkClient().send(&command, socketFd, nullptr);
}

extern "C" int setNetworkForProcess(unsigned netId) {
    return setNetworkForTarget(netId, &netIdForProcess);
}

extern "C" int setNetworkForResolv(unsigned netId) {
    return setNetworkForTarget(netId, &netIdForResolv);
}

extern "C" int protectFromVpn(int socketFd) {
    if (socketFd < 0) {
        return -EBADF;
    }
    FwmarkCommand command = {FwmarkCommand::PROTECT_FROM_VPN, 0, 0, 0};
    return FwmarkClient().send(&command, socketFd, nullptr);
}

extern "C" int setNetworkForUser(uid_t uid, int socketFd) {
    CHECK_SOCKET_IS_MARKABLE(socketFd);
    FwmarkCommand command = {FwmarkCommand::SELECT_FOR_USER, 0, uid, 0};
    return FwmarkClient().send(&command, socketFd, nullptr);
}

extern "C" int queryUserAccess(uid_t uid, unsigned netId) {
    FwmarkCommand command = {FwmarkCommand::QUERY_USER_ACCESS, netId, uid, 0};
    return FwmarkClient().send(&command, -1, nullptr);
}

extern "C" int tagSocket(int socketFd, uint32_t tag, uid_t uid) {
    CHECK_SOCKET_IS_MARKABLE(socketFd);
    FwmarkCommand command = {FwmarkCommand::TAG_SOCKET, 0, uid, tag};
    return FwmarkClient().send(&command, socketFd, nullptr);
}

extern "C" int untagSocket(int socketFd) {
    CHECK_SOCKET_IS_MARKABLE(socketFd);
    FwmarkCommand command = {FwmarkCommand::UNTAG_SOCKET, 0, 0, 0};
    return FwmarkClient().send(&command, socketFd, nullptr);
}

extern "C" int setCounterSet(uint32_t counterSet, uid_t uid) {
    FwmarkCommand command = {FwmarkCommand::SET_COUNTERSET, 0, uid, counterSet};
    return FwmarkClient().send(&command, -1, nullptr);
}

extern "C" int deleteTagData(uint32_t tag, uid_t uid) {
    FwmarkCommand command = {FwmarkCommand::DELETE_TAGDATA, 0, uid, tag};
    return FwmarkClient().send(&command, -1, nullptr);
}

extern "C" int resNetworkQuery(unsigned netId, const char* dname, int ns_class, int ns_type,
                               uint32_t flags) {
    std::vector<uint8_t> buf(MAX_CMD_SIZE, 0);
    int len = res_mkquery(ns_o_query, dname, ns_class, ns_type, nullptr, 0, nullptr, buf.data(),
                          MAX_CMD_SIZE);

    return resNetworkSend(netId, buf.data(), len, flags);
}

extern "C" int resNetworkSend(unsigned netId, const uint8_t* msg, size_t msglen, uint32_t flags) {
    // Encode
    // Base 64 encodes every 3 bytes into 4 characters, but then adds padding to the next
    // multiple of 4 and a \0
    const size_t encodedLen = divCeil(msglen, 3) * 4 + 1;
    std::string encodedQuery(encodedLen - 1, 0);
    int enLen = b64_ntop(msg, msglen, encodedQuery.data(), encodedLen);

    if (enLen < 0) {
        // Unexpected behavior, encode failed
        // b64_ntop only fails when size is too long.
        return -EMSGSIZE;
    }
    // Send
    netId = getNetworkForResolv(netId);
    const std::string cmd = "resnsend " + std::to_string(netId) + " " + std::to_string(flags) +
                            " " + encodedQuery + '\0';
    if (cmd.size() > MAX_CMD_SIZE) {
        // Cmd size must less than buffer size of FrameworkListener
        return -EMSGSIZE;
    }
    int fd = dns_open_proxy();
    if (fd == -1) {
        return -errno;
    }
    ssize_t rc = sendData(fd, cmd.c_str(), cmd.size());
    if (rc < 0) {
        close(fd);
        return rc;
    }
    shutdown(fd, SHUT_WR);
    return fd;
}

extern "C" int resNetworkResult(int fd, int* rcode, uint8_t* answer, size_t anslen) {
    int32_t result = 0;
    unique_fd ufd(fd);
    // Read -errno/rcode
    if (!readBE32(fd, &result)) {
        // Unexpected behavior, read -errno/rcode fail
        return -errno;
    }
    if (result < 0) {
        // result < 0, it's -errno
        return result;
    }
    // result >= 0, it's rcode
    *rcode = result;

    // Read answer
    int32_t size = 0;
    if (!readBE32(fd, &size)) {
        // Unexpected behavior, read ans len fail
        return -EREMOTEIO;
    }
    if (anslen < static_cast<size_t>(size)) {
        // Answer buffer is too small
        return -EMSGSIZE;
    }
    int rc = readData(fd, answer, size);
    if (rc < 0) {
        // Reading the answer failed.
        return rc;
    }
    return size;
}

extern "C" void resNetworkCancel(int fd) {
    close(fd);
}

extern "C" int getNetworkForDns(unsigned* dnsNetId) {
    if (dnsNetId == nullptr) return -EFAULT;
    int fd = dns_open_proxy();
    if (fd == -1) {
        return -errno;
    }
    unique_fd ufd(fd);
    return getNetworkForDnsInternal(fd, dnsNetId);
}

int getNetworkForDnsInternal(int fd, unsigned* dnsNetId) {
    if (fd == -1) {
        return -EBADF;
    }

    unsigned resolvNetId = getNetworkForResolv(NETID_UNSET);

    const std::string cmd = "getdnsnetid " + std::to_string(resolvNetId);
    ssize_t rc = sendData(fd, cmd.c_str(), cmd.size() + 1);
    if (rc < 0) {
        return rc;
    }

    int responseCode = 0;
    // Read responseCode
    if (!readResponseCode(fd, &responseCode)) {
        // Unexpected behavior, read responseCode fail
        return -errno;
    }

    if (responseCode != ResponseCode::DnsProxyQueryResult) {
        return -EOPNOTSUPP;
    }

    int32_t result = 0;
    // Read -errno/dnsnetid
    if (!readBE32(fd, &result)) {
        // Unexpected behavior, read -errno/dnsnetid fail
        return -errno;
    }

    *dnsNetId = result;

    return 0;
}
