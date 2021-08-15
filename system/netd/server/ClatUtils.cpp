/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "ClatUtils.h"

#include <arpa/inet.h>
#include <errno.h>
#include <linux/if.h>
#include <linux/netlink.h>
#include <linux/pkt_cls.h>
#include <linux/pkt_sched.h>
#include <linux/rtnetlink.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_TAG "ClatUtils"
#include <log/log.h>

#include "NetlinkCommands.h"
#include "android-base/unique_fd.h"
#include "bpf/BpfUtils.h"
#include "netdbpf/bpf_shared.h"

namespace android {
namespace net {

int hardwareAddressType(const std::string& interface) {
    base::unique_fd ufd(socket(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0));

    if (ufd < 0) {
        const int err = errno;
        ALOGE("socket(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0)");
        return -err;
    };

    struct ifreq ifr = {};
    // We use strncpy() instead of strlcpy() since kernel has to be able
    // to handle non-zero terminated junk passed in by userspace anyway,
    // and this way too long interface names (more than IFNAMSIZ-1 = 15
    // characters plus terminating NULL) will not get truncated to 15
    // characters and zero-terminated and thus potentially erroneously
    // match a truncated interface if one were to exist.
    strncpy(ifr.ifr_name, interface.c_str(), sizeof(ifr.ifr_name));

    if (ioctl(ufd, SIOCGIFHWADDR, &ifr, sizeof(ifr))) return -errno;

    return ifr.ifr_hwaddr.sa_family;
}

int getClatIngressMapFd(void) {
    const int fd = bpf::bpfFdGet(CLAT_INGRESS_MAP_PATH, 0);
    return (fd == -1) ? -errno : fd;
}

int getClatIngressProgFd(bool with_ethernet_header) {
    const int fd = bpf::bpfFdGet(
            with_ethernet_header ? CLAT_INGRESS_PROG_ETHER_PATH : CLAT_INGRESS_PROG_RAWIP_PATH, 0);
    return (fd == -1) ? -errno : fd;
}

// TODO: use //system/netd/server/NetlinkCommands.cpp:openNetlinkSocket(protocol)
int openNetlinkSocket(void) {
    base::unique_fd fd(socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE));
    if (fd == -1) {
        const int err = errno;
        ALOGE("socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE)");
        return -err;
    }

    int rv;

    const int on = 1;
    rv = setsockopt(fd, SOL_NETLINK, NETLINK_CAP_ACK, &on, sizeof(on));
    if (rv) ALOGE("setsockopt(fd, SOL_NETLINK, NETLINK_CAP_ACK, %d)", on);

    // this is needed to get sane strace netlink parsing, it allocates the pid
    rv = bind(fd, (const struct sockaddr*)&KERNEL_NLADDR, sizeof(KERNEL_NLADDR));
    if (rv) {
        const int err = errno;
        ALOGE("bind(fd, {AF_NETLINK, 0, 0})");
        return -err;
    }

    // we do not want to receive messages from anyone besides the kernel
    rv = connect(fd, (const struct sockaddr*)&KERNEL_NLADDR, sizeof(KERNEL_NLADDR));
    if (rv) {
        const int err = errno;
        ALOGE("connect(fd, {AF_NETLINK, 0, 0})");
        return -err;
    }

    return fd.release();
}

// TODO: merge with //system/netd/server/SockDiag.cpp:checkError(fd)
int processNetlinkResponse(int fd) {
    struct {
        nlmsghdr h;
        nlmsgerr e;
        char buf[256];
    } resp = {};

    const int rv = recv(fd, &resp, sizeof(resp), MSG_TRUNC);

    if (rv == -1) {
        const int err = errno;
        ALOGE("recv() failed");
        return -err;
    }

    if (rv < (int)NLMSG_SPACE(sizeof(struct nlmsgerr))) {
        ALOGE("recv() returned short packet: %d", rv);
        return -EMSGSIZE;
    }

    if (resp.h.nlmsg_len != (unsigned)rv) {
        ALOGE("recv() returned invalid header length: %d != %d", resp.h.nlmsg_len, rv);
        return -EBADMSG;
    }

    if (resp.h.nlmsg_type != NLMSG_ERROR) {
        ALOGE("recv() did not return NLMSG_ERROR message: %d", resp.h.nlmsg_type);
        return -EBADMSG;
    }

    return resp.e.error;  // returns 0 on success
}

// ADD:     nlMsgType=RTM_NEWQDISC nlMsgFlags=NLM_F_EXCL|NLM_F_CREATE
// REPLACE: nlMsgType=RTM_NEWQDISC nlMsgFlags=NLM_F_CREATE|NLM_F_REPLACE
// DEL:     nlMsgType=RTM_DELQDISC nlMsgFlags=0
int doTcQdiscClsact(int fd, int ifIndex, __u16 nlMsgType, __u16 nlMsgFlags) {
    // This is the name of the qdisc we are attaching.
    // Some hoop jumping to make this compile time constant with known size,
    // so that the structure declaration is well defined at compile time.
#define CLSACT "clsact"
    static const char clsact[] = CLSACT;
    // sizeof() includes the terminating NULL
#define ASCIIZ_LEN_CLSACT sizeof(clsact)

    const struct {
        nlmsghdr n;
        tcmsg t;
        struct {
            nlattr attr;
            char str[NLMSG_ALIGN(ASCIIZ_LEN_CLSACT)];
        } kind;
    } req = {
            .n =
                    {
                            .nlmsg_len = sizeof(req),
                            .nlmsg_type = nlMsgType,
                            .nlmsg_flags = static_cast<__u16>(NETLINK_REQUEST_FLAGS | nlMsgFlags),
                    },
            .t =
                    {
                            .tcm_family = AF_UNSPEC,
                            .tcm_ifindex = ifIndex,
                            .tcm_handle = TC_H_MAKE(TC_H_CLSACT, 0),
                            .tcm_parent = TC_H_CLSACT,
                    },
            .kind =
                    {
                            .attr =
                                    {
                                            .nla_len = NLA_HDRLEN + ASCIIZ_LEN_CLSACT,
                                            .nla_type = TCA_KIND,
                                    },
                            .str = CLSACT,
                    },
    };
#undef ASCIIZ_LEN_CLSACT
#undef CLSACT

    const int rv = send(fd, &req, sizeof(req), 0);
    if (rv == -1) return -errno;
    if (rv != sizeof(req)) return -EMSGSIZE;

    return processNetlinkResponse(fd);
}

int tcQdiscAddDevClsact(int fd, int ifIndex) {
    return doTcQdiscClsact(fd, ifIndex, RTM_NEWQDISC, NLM_F_EXCL | NLM_F_CREATE);
}

int tcQdiscReplaceDevClsact(int fd, int ifIndex) {
    return doTcQdiscClsact(fd, ifIndex, RTM_NEWQDISC, NLM_F_CREATE | NLM_F_REPLACE);
}

int tcQdiscDelDevClsact(int fd, int ifIndex) {
    return doTcQdiscClsact(fd, ifIndex, RTM_DELQDISC, 0);
}

// tc filter add dev .. ingress prio 1 protocol ipv6 bpf object-pinned /sys/fs/bpf/... direct-action
int tcFilterAddDevBpf(int fd, int ifIndex, int bpfFd, bool ethernet) {
    // The priority doesn't matter until we actually start attaching multiple
    // things to the same interface's ingress point.
    const int prio = 1;

    // This is the name of the filter we're attaching (ie. this is the 'bpf'
    // packet classifier enabled by kernel config option CONFIG_NET_CLS_BPF.
    //
    // We go through some hoops in order to make this compile time constants
    // so that we can define the struct further down the function with the
    // field for this sized correctly already during the build.
#define BPF "bpf"
    const char bpf[] = BPF;
    // sizeof() includes the terminating NULL
#define ASCIIZ_LEN_BPF sizeof(bpf)

    // This is to replicate program name suffix used by 'tc' Linux cli
    // when it attaches programs.
#define FSOBJ_SUFFIX ":[*fsobj]"

    // This macro expands (from header files) to:
    //   prog_clatd_schedcls_ingress_clat_rawip:[*fsobj]
    // and is the name of the pinned ebpf program for ARPHRD_RAWIP interfaces.
    // (also compatible with anything that has 0 size L2 header)
#define NAME_RAWIP CLAT_INGRESS_PROG_RAWIP_NAME FSOBJ_SUFFIX
    const char name_rawip[] = NAME_RAWIP;

    // This macro expands (from header files) to:
    //   prog_clatd_schedcls_ingress_clat_ether:[*fsobj]
    // and is the name of the pinned ebpf program for ARPHRD_ETHER interfaces.
    // (also compatible with anything that has standard ethernet header)
#define NAME_ETHER CLAT_INGRESS_PROG_ETHER_NAME FSOBJ_SUFFIX
    const char name_ether[] = NAME_ETHER;

    // The actual name we'll use is determined at run time via 'ethernet'
    // boolean.  We need to compile time allocate enough space in the struct
    // hence this macro magic to make sure we have enough space for either
    // possibility.  In practice both are actually the same size.
#define ASCIIZ_MAXLEN_NAME \
    ((sizeof(name_rawip) > sizeof(name_ether)) ? sizeof(name_rawip) : sizeof(name_ether))

    // This is not a compile time constant and is used in strcpy below
#define NAME (ethernet ? NAME_ETHER : NAME_RAWIP)

    struct {
        nlmsghdr n;
        tcmsg t;
        struct {
            nlattr attr;
            char str[NLMSG_ALIGN(ASCIIZ_LEN_BPF)];
        } kind;
        struct {
            nlattr attr;
            struct {
                nlattr attr;
                __u32 u32;
            } fd;
            struct {
                nlattr attr;
                char str[NLMSG_ALIGN(ASCIIZ_MAXLEN_NAME)];
            } name;
            struct {
                nlattr attr;
                __u32 u32;
            } flags;
        } options;
    } req = {
            .n =
                    {
                            .nlmsg_len = sizeof(req),
                            .nlmsg_type = RTM_NEWTFILTER,
                            .nlmsg_flags = NETLINK_REQUEST_FLAGS | NLM_F_EXCL | NLM_F_CREATE,
                    },
            .t =
                    {
                            .tcm_family = AF_UNSPEC,
                            .tcm_ifindex = ifIndex,
                            .tcm_handle = TC_H_UNSPEC,
                            .tcm_parent = TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_INGRESS),
                            .tcm_info = (prio << 16) | htons(ETH_P_IPV6),
                    },
            .kind =
                    {
                            .attr =
                                    {
                                            .nla_len = sizeof(req.kind),
                                            .nla_type = TCA_KIND,
                                    },
                            .str = BPF,
                    },
            .options =
                    {
                            .attr =
                                    {
                                            .nla_len = sizeof(req.options),
                                            .nla_type = TCA_OPTIONS,
                                    },
                            .fd =
                                    {
                                            .attr =
                                                    {
                                                            .nla_len = sizeof(req.options.fd),
                                                            .nla_type = TCA_BPF_FD,
                                                    },
                                            .u32 = static_cast<__u32>(bpfFd),
                                    },
                            .name =
                                    {
                                            .attr =
                                                    {
                                                            .nla_len = sizeof(req.options.name),
                                                            .nla_type = TCA_BPF_NAME,
                                                    },
                                            // Visible via 'tc filter show', but
                                            // is overwritten by strcpy below
                                            .str = "placeholder",
                                    },
                            .flags =
                                    {
                                            .attr =
                                                    {
                                                            .nla_len = sizeof(req.options.flags),
                                                            .nla_type = TCA_BPF_FLAGS,
                                                    },
                                            .u32 = TCA_BPF_FLAG_ACT_DIRECT,
                                    },
                    },
    };

    strncpy(req.options.name.str, NAME, sizeof(req.options.name.str));

#undef NAME
#undef ASCIIZ_MAXLEN_NAME
#undef NAME_ETHER
#undef NAME_RAWIP
#undef NAME
#undef ASCIIZ_LEN_BPF
#undef BPF

    const int rv = send(fd, &req, sizeof(req), 0);
    if (rv == -1) return -errno;
    if (rv != sizeof(req)) return -EMSGSIZE;

    return processNetlinkResponse(fd);
}

}  // namespace net
}  // namespace android
