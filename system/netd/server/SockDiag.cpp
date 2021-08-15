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

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <linux/netlink.h>
#include <linux/sock_diag.h>
#include <linux/inet_diag.h>

#define LOG_TAG "Netd"

#include <android-base/strings.h>
#include <log/log.h>
#include <netdutils/Stopwatch.h>

#include "NetdConstants.h"
#include "Permission.h"
#include "SockDiag.h"

#ifndef SOCK_DESTROY
#define SOCK_DESTROY 21
#endif

#define INET_DIAG_BC_MARK_COND 10

namespace android {

using netdutils::Stopwatch;

namespace net {
namespace {

int checkError(int fd) {
    struct {
        nlmsghdr h;
        nlmsgerr err;
    } __attribute__((__packed__)) ack;
    ssize_t bytesread = recv(fd, &ack, sizeof(ack), MSG_DONTWAIT | MSG_PEEK);
    if (bytesread == -1) {
       // Read failed (error), or nothing to read (good).
       return (errno == EAGAIN) ? 0 : -errno;
    } else if (bytesread == (ssize_t) sizeof(ack) && ack.h.nlmsg_type == NLMSG_ERROR) {
        // We got an error. Consume it.
        recv(fd, &ack, sizeof(ack), 0);
        return ack.err.error;
    } else {
        // The kernel replied with something. Leave it to the caller.
        return 0;
    }
}

}  // namespace

bool SockDiag::open() {
    if (hasSocks()) {
        return false;
    }

    mSock = socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_INET_DIAG);
    mWriteSock = socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_INET_DIAG);
    if (!hasSocks()) {
        closeSocks();
        return false;
    }

    sockaddr_nl nl = { .nl_family = AF_NETLINK };
    if ((connect(mSock, reinterpret_cast<sockaddr *>(&nl), sizeof(nl)) == -1) ||
        (connect(mWriteSock, reinterpret_cast<sockaddr *>(&nl), sizeof(nl)) == -1)) {
        closeSocks();
        return false;
    }

    return true;
}

int SockDiag::sendDumpRequest(uint8_t proto, uint8_t family, uint8_t extensions, uint32_t states,
                              iovec *iov, int iovcnt) {
    struct {
        nlmsghdr nlh;
        inet_diag_req_v2 req;
    } __attribute__((__packed__)) request = {
        .nlh = {
            .nlmsg_type = SOCK_DIAG_BY_FAMILY,
            .nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP,
        },
        .req = {
            .sdiag_family = family,
            .sdiag_protocol = proto,
            .idiag_ext = extensions,
            .idiag_states = states,
        },
    };

    size_t len = 0;
    iov[0].iov_base = &request;
    iov[0].iov_len = sizeof(request);
    for (int i = 0; i < iovcnt; i++) {
        len += iov[i].iov_len;
    }
    request.nlh.nlmsg_len = len;

    if (writev(mSock, iov, iovcnt) != (ssize_t) len) {
        return -errno;
    }

    return checkError(mSock);
}

int SockDiag::sendDumpRequest(uint8_t proto, uint8_t family, uint32_t states) {
    iovec iov[] = {
        { nullptr, 0 },
    };
    return sendDumpRequest(proto, family, 0, states, iov, ARRAY_SIZE(iov));
}

int SockDiag::sendDumpRequest(uint8_t proto, uint8_t family, const char *addrstr) {
    addrinfo hints = { .ai_flags = AI_NUMERICHOST };
    addrinfo *res;
    in6_addr mapped = { .s6_addr32 = { 0, 0, htonl(0xffff), 0 } };
    int ret;

    // TODO: refactor the netlink parsing code out of system/core, bring it into netd, and stop
    // doing string conversions when they're not necessary.
    if ((ret = getaddrinfo(addrstr, nullptr, &hints, &res)) != 0) {
        return -EINVAL;
    }

    // So we don't have to call freeaddrinfo on every failure path.
    ScopedAddrinfo resP(res);

    void *addr;
    uint8_t addrlen;
    if (res->ai_family == AF_INET && family == AF_INET) {
        in_addr& ina = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;
        addr = &ina;
        addrlen = sizeof(ina);
    } else if (res->ai_family == AF_INET && family == AF_INET6) {
        in_addr& ina = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;
        mapped.s6_addr32[3] = ina.s_addr;
        addr = &mapped;
        addrlen = sizeof(mapped);
    } else if (res->ai_family == AF_INET6 && family == AF_INET6) {
        in6_addr& in6a = reinterpret_cast<sockaddr_in6*>(res->ai_addr)->sin6_addr;
        addr = &in6a;
        addrlen = sizeof(in6a);
    } else {
        return -EAFNOSUPPORT;
    }

    uint8_t prefixlen = addrlen * 8;
    uint8_t yesjump = sizeof(inet_diag_bc_op) + sizeof(inet_diag_hostcond) + addrlen;
    uint8_t nojump = yesjump + 4;

    struct {
        nlattr nla;
        inet_diag_bc_op op;
        inet_diag_hostcond cond;
    } __attribute__((__packed__)) attrs = {
        .nla = {
            .nla_type = INET_DIAG_REQ_BYTECODE,
        },
        .op = {
            INET_DIAG_BC_S_COND,
            yesjump,
            nojump,
        },
        .cond = {
            family,
            prefixlen,
            -1,
            {}
        },
    };

    attrs.nla.nla_len = sizeof(attrs) + addrlen;

    iovec iov[] = {
        { nullptr,           0 },
        { &attrs,            sizeof(attrs) },
        { addr,              addrlen },
    };

    uint32_t states = ~(1 << TCP_TIME_WAIT);
    return sendDumpRequest(proto, family, 0, states, iov, ARRAY_SIZE(iov));
}

int SockDiag::readDiagMsg(uint8_t proto, const SockDiag::DestroyFilter& shouldDestroy) {
    NetlinkDumpCallback callback = [this, proto, shouldDestroy] (nlmsghdr *nlh) {
        const inet_diag_msg *msg = reinterpret_cast<inet_diag_msg *>(NLMSG_DATA(nlh));
        if (shouldDestroy(proto, msg)) {
            sockDestroy(proto, msg);
        }
    };

    return processNetlinkDump(mSock, callback);
}

int SockDiag::readDiagMsgWithTcpInfo(const TcpInfoReader& tcpInfoReader) {
    NetlinkDumpCallback callback = [tcpInfoReader] (nlmsghdr *nlh) {
        if (nlh->nlmsg_type != SOCK_DIAG_BY_FAMILY) {
            ALOGE("expected nlmsg_type=SOCK_DIAG_BY_FAMILY, got nlmsg_type=%d", nlh->nlmsg_type);
            return;
        }
        Fwmark mark;
        struct tcp_info *tcpinfo = nullptr;
        uint32_t tcpinfoLength = 0;
        inet_diag_msg *msg = reinterpret_cast<inet_diag_msg *>(NLMSG_DATA(nlh));
        uint32_t attr_len = nlh->nlmsg_len - NLMSG_LENGTH(sizeof(*msg));
        struct rtattr *attr = reinterpret_cast<struct rtattr*>(msg+1);
        while (RTA_OK(attr, attr_len)) {
            if (attr->rta_type == INET_DIAG_INFO) {
                tcpinfo = reinterpret_cast<struct tcp_info*>(RTA_DATA(attr));
                tcpinfoLength = RTA_PAYLOAD(attr);
            }
            if (attr->rta_type == INET_DIAG_MARK) {
                mark.intValue = *reinterpret_cast<uint32_t*>(RTA_DATA(attr));
            }
            attr = RTA_NEXT(attr, attr_len);
        }

        tcpInfoReader(mark, msg, tcpinfo, tcpinfoLength);
    };

    return processNetlinkDump(mSock, callback);
}

// Determines whether a socket is a loopback socket. Does not check socket state.
bool SockDiag::isLoopbackSocket(const inet_diag_msg *msg) {
    switch (msg->idiag_family) {
        case AF_INET:
            // Old kernels only copy the IPv4 address and leave the other 12 bytes uninitialized.
            return IN_LOOPBACK(htonl(msg->id.idiag_src[0])) ||
                   IN_LOOPBACK(htonl(msg->id.idiag_dst[0])) ||
                   msg->id.idiag_src[0] == msg->id.idiag_dst[0];

        case AF_INET6: {
            const struct in6_addr *src = (const struct in6_addr *) &msg->id.idiag_src;
            const struct in6_addr *dst = (const struct in6_addr *) &msg->id.idiag_dst;
            return (IN6_IS_ADDR_V4MAPPED(src) && IN_LOOPBACK(src->s6_addr32[3])) ||
                   (IN6_IS_ADDR_V4MAPPED(dst) && IN_LOOPBACK(dst->s6_addr32[3])) ||
                   IN6_IS_ADDR_LOOPBACK(src) || IN6_IS_ADDR_LOOPBACK(dst) ||
                   !memcmp(src, dst, sizeof(*src));
        }
        default:
            return false;
    }
}

int SockDiag::sockDestroy(uint8_t proto, const inet_diag_msg *msg) {
    if (msg == nullptr) {
       return 0;
    }

    DestroyRequest request = {
        .nlh = {
            .nlmsg_type = SOCK_DESTROY,
            .nlmsg_flags = NLM_F_REQUEST,
        },
        .req = {
            .sdiag_family = msg->idiag_family,
            .sdiag_protocol = proto,
            .idiag_states = (uint32_t) (1 << msg->idiag_state),
            .id = msg->id,
        },
    };
    request.nlh.nlmsg_len = sizeof(request);

    if (write(mWriteSock, &request, sizeof(request)) < (ssize_t) sizeof(request)) {
        return -errno;
    }

    int ret = checkError(mWriteSock);
    if (!ret) mSocketsDestroyed++;
    return ret;
}

int SockDiag::destroySockets(uint8_t proto, int family, const char *addrstr) {
    if (!hasSocks()) {
        return -EBADFD;
    }

    if (int ret = sendDumpRequest(proto, family, addrstr)) {
        return ret;
    }

    auto destroyAll = [] (uint8_t, const inet_diag_msg*) { return true; };

    return readDiagMsg(proto, destroyAll);
}

int SockDiag::destroySockets(const char *addrstr) {
    Stopwatch s;
    mSocketsDestroyed = 0;

    if (!strchr(addrstr, ':')) {
        if (int ret = destroySockets(IPPROTO_TCP, AF_INET, addrstr)) {
            ALOGE("Failed to destroy IPv4 sockets on %s: %s", addrstr, strerror(-ret));
            return ret;
        }
    }
    if (int ret = destroySockets(IPPROTO_TCP, AF_INET6, addrstr)) {
        ALOGE("Failed to destroy IPv6 sockets on %s: %s", addrstr, strerror(-ret));
        return ret;
    }

    if (mSocketsDestroyed > 0) {
        ALOGI("Destroyed %d sockets on %s in %.1f ms", mSocketsDestroyed, addrstr, s.timeTaken());
    }

    return mSocketsDestroyed;
}

int SockDiag::destroyLiveSockets(const DestroyFilter& destroyFilter, const char *what,
                                 iovec *iov, int iovcnt) {
    const int proto = IPPROTO_TCP;
    const uint32_t states = (1 << TCP_ESTABLISHED) | (1 << TCP_SYN_SENT) | (1 << TCP_SYN_RECV);

    for (const int family : {AF_INET, AF_INET6}) {
        const char *familyName = (family == AF_INET) ? "IPv4" : "IPv6";
        if (int ret = sendDumpRequest(proto, family, 0, states, iov, iovcnt)) {
            ALOGE("Failed to dump %s sockets for %s: %s", familyName, what, strerror(-ret));
            return ret;
        }
        if (int ret = readDiagMsg(proto, destroyFilter)) {
            ALOGE("Failed to destroy %s sockets for %s: %s", familyName, what, strerror(-ret));
            return ret;
        }
    }

    return 0;
}

int SockDiag::getLiveTcpInfos(const TcpInfoReader& tcpInfoReader) {
    const int proto = IPPROTO_TCP;
    const uint32_t states = (1 << TCP_ESTABLISHED) | (1 << TCP_SYN_SENT) | (1 << TCP_SYN_RECV);
    const uint8_t extensions = (1 << INET_DIAG_MEMINFO); // flag for dumping struct tcp_info.

    iovec iov[] = {
        { nullptr, 0 },
    };

    for (const int family : {AF_INET, AF_INET6}) {
        const char *familyName = (family == AF_INET) ? "IPv4" : "IPv6";
        if (int ret = sendDumpRequest(proto, family, extensions, states, iov, ARRAY_SIZE(iov))) {
            ALOGE("Failed to dump %s sockets struct tcp_info: %s", familyName, strerror(-ret));
            return ret;
        }
        if (int ret = readDiagMsgWithTcpInfo(tcpInfoReader)) {
            ALOGE("Failed to read %s sockets struct tcp_info: %s", familyName, strerror(-ret));
            return ret;
        }
    }

    return 0;
}

int SockDiag::destroySockets(uint8_t proto, const uid_t uid, bool excludeLoopback) {
    mSocketsDestroyed = 0;
    Stopwatch s;

    auto shouldDestroy = [uid, excludeLoopback] (uint8_t, const inet_diag_msg *msg) {
        return msg != nullptr &&
               msg->idiag_uid == uid &&
               !(excludeLoopback && isLoopbackSocket(msg));
    };

    for (const int family : {AF_INET, AF_INET6}) {
        const char *familyName = family == AF_INET ? "IPv4" : "IPv6";
        uint32_t states = (1 << TCP_ESTABLISHED) | (1 << TCP_SYN_SENT) | (1 << TCP_SYN_RECV);
        if (int ret = sendDumpRequest(proto, family, states)) {
            ALOGE("Failed to dump %s sockets for UID: %s", familyName, strerror(-ret));
            return ret;
        }
        if (int ret = readDiagMsg(proto, shouldDestroy)) {
            ALOGE("Failed to destroy %s sockets for UID: %s", familyName, strerror(-ret));
            return ret;
        }
    }

    if (mSocketsDestroyed > 0) {
        ALOGI("Destroyed %d sockets for UID in %.1f ms", mSocketsDestroyed, s.timeTaken());
    }

    return 0;
}

int SockDiag::destroySockets(const UidRanges& uidRanges, const std::set<uid_t>& skipUids,
                             bool excludeLoopback) {
    mSocketsDestroyed = 0;
    Stopwatch s;

    auto shouldDestroy = [&] (uint8_t, const inet_diag_msg *msg) {
        return msg != nullptr &&
               uidRanges.hasUid(msg->idiag_uid) &&
               skipUids.find(msg->idiag_uid) == skipUids.end() &&
               !(excludeLoopback && isLoopbackSocket(msg));
    };

    iovec iov[] = {
        { nullptr, 0 },
    };

    if (int ret = destroyLiveSockets(shouldDestroy, "UID", iov, ARRAY_SIZE(iov))) {
        return ret;
    }

    if (mSocketsDestroyed > 0) {
        ALOGI("Destroyed %d sockets for %s skip={%s} in %.1f ms",
              mSocketsDestroyed, uidRanges.toString().c_str(),
              android::base::Join(skipUids, " ").c_str(), s.timeTaken());
    }

    return 0;
}

// Destroys all "live" (CONNECTED, SYN_SENT, SYN_RECV) TCP sockets on the specified netId where:
// 1. The opening app no longer has permission to use this network, or:
// 2. The opening app does have permission, but did not explicitly select this network.
//
// We destroy sockets without the explicit bit because we want to avoid the situation where a
// privileged app uses its privileges without knowing it is doing so. For example, a privileged app
// might have opened a socket on this network just because it was the default network at the
// time. If we don't kill these sockets, those apps could continue to use them without realizing
// that they are now sending and receiving traffic on a network that is now restricted.
int SockDiag::destroySocketsLackingPermission(unsigned netId, Permission permission,
                                              bool excludeLoopback) {
    struct markmatch {
        inet_diag_bc_op op;
        // TODO: switch to inet_diag_markcond
        __u32 mark;
        __u32 mask;
    } __attribute__((packed));
    constexpr uint8_t matchlen = sizeof(markmatch);

    Fwmark netIdMark, netIdMask;
    netIdMark.netId = netId;
    netIdMask.netId = 0xffff;

    Fwmark controlMark;
    controlMark.explicitlySelected = true;
    controlMark.permission = permission;

    // A SOCK_DIAG bytecode program that accepts the sockets we intend to destroy.
    struct bytecode {
        markmatch netIdMatch;
        markmatch controlMatch;
        inet_diag_bc_op controlJump;
    } __attribute__((packed)) bytecode;

    // The length of the INET_DIAG_BC_JMP instruction.
    constexpr uint8_t jmplen = sizeof(inet_diag_bc_op);
    // Jump exactly this far past the end of the program to reject.
    constexpr uint8_t rejectoffset = sizeof(inet_diag_bc_op);
    // Total length of the program.
    constexpr uint8_t bytecodelen = sizeof(bytecode);

    bytecode = (struct bytecode) {
        // If netId matches, continue, otherwise, reject (i.e., leave socket alone).
        { { INET_DIAG_BC_MARK_COND, matchlen, bytecodelen + rejectoffset },
          netIdMark.intValue, netIdMask.intValue },

        // If explicit and permission bits match, go to the JMP below which rejects the socket
        // (i.e., we leave it alone). Otherwise, jump to the end of the program, which accepts the
        // socket (so we destroy it).
        { { INET_DIAG_BC_MARK_COND, matchlen, matchlen + jmplen },
          controlMark.intValue, controlMark.intValue },

        // This JMP unconditionally rejects the packet by jumping to the reject target. It is
        // necessary to keep the kernel bytecode verifier happy. If we don't have a JMP the bytecode
        // is invalid because the target of every no jump must always be reachable by yes jumps.
        // Without this JMP, the accept target is not reachable by yes jumps and the program will
        // be rejected by the validator.
        { INET_DIAG_BC_JMP, jmplen, jmplen + rejectoffset },

        // We have reached the end of the program. Accept the socket, and destroy it below.
    };

    struct nlattr nla = {
        .nla_type = INET_DIAG_REQ_BYTECODE,
        .nla_len = sizeof(struct nlattr) + bytecodelen,
    };

    iovec iov[] = {
        { nullptr,   0 },
        { &nla,      sizeof(nla) },
        { &bytecode, bytecodelen },
    };

    mSocketsDestroyed = 0;
    Stopwatch s;

    auto shouldDestroy = [&] (uint8_t, const inet_diag_msg *msg) {
        return msg != nullptr && !(excludeLoopback && isLoopbackSocket(msg));
    };

    if (int ret = destroyLiveSockets(shouldDestroy, "permission change", iov, ARRAY_SIZE(iov))) {
        return ret;
    }

    if (mSocketsDestroyed > 0) {
        ALOGI("Destroyed %d sockets for netId %d permission=%d in %.1f ms",
              mSocketsDestroyed, netId, permission, s.timeTaken());
    }

    return 0;
}

}  // namespace net
}  // namespace android
