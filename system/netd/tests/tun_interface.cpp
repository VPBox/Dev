/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * tun_interface.cpp - creates tun interfaces for testing purposes
 */

#include <string>

#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <netutils/ifc.h>

#include "tun_interface.h"

#define TUN_DEV "/dev/tun"

using android::base::StringPrintf;
using android::base::unique_fd;

namespace android {
namespace net {

int TunInterface::init(const std::string& ifName) {
    // Generate a random ULA address pair.
    arc4random_buf(&mSrcAddr, sizeof(mSrcAddr));
    mSrcAddr.s6_addr[0] = 0xfd;
    memcpy(&mDstAddr, &mSrcAddr, sizeof(mDstAddr));
    mDstAddr.s6_addr[15] ^= 1;

    // Convert the addresses to strings because that's what ifc_add_address takes.
    char srcStr[INET6_ADDRSTRLEN], dstStr[INET6_ADDRSTRLEN];
    sockaddr_in6 src6 = { .sin6_family = AF_INET6, .sin6_addr = mSrcAddr, };
    sockaddr_in6 dst6 = { .sin6_family = AF_INET6, .sin6_addr = mDstAddr, };
    int flags = NI_NUMERICHOST;
    if (getnameinfo((sockaddr *) &src6, sizeof(src6), srcStr, sizeof(srcStr), nullptr, 0, flags) ||
        getnameinfo((sockaddr *) &dst6, sizeof(dst6), dstStr, sizeof(dstStr), nullptr, 0, flags)) {
        return -EINVAL;
    }

    // Create a tun interface with a name based on a random number.
    // In order to fit the size of interface alert name , resize ifname to 9
    // Alert name format in netd: ("%sAlert", ifname)
    // Limitation in kernel: char name[15] in struct xt_quota_mtinfo2

    // Note that this form of alert doesn't actually appear to be used for interface alerts.
    // It can only be created by BandwidthController::setInterfaceAlert, but that appears to have no
    // actual callers in the framework, because mActiveAlerts is always empty.
    // TODO: remove setInterfaceAlert and use a longer interface name.
    mIfName = ifName;
    if (mIfName.empty()) {
        mIfName = StringPrintf("netd%x", arc4random());
    }
    mIfName.resize(9);

    struct ifreq ifr = {
        .ifr_ifru = { .ifru_flags = IFF_TUN },
    };
    strlcpy(ifr.ifr_name, mIfName.c_str(), sizeof(ifr.ifr_name));

    mFd = open(TUN_DEV, O_RDWR | O_NONBLOCK | O_CLOEXEC);
    if (mFd == -1) return -errno;

    int ret = ioctl(mFd, TUNSETIFF, &ifr, sizeof(ifr));
    if (ret == -1) {
        ret = -errno;
        close(mFd);
        return ret;
    }

    mIfIndex = if_nametoindex(ifr.ifr_name);

    if (addAddress(srcStr, 64) || addAddress(dstStr, 64)) {
        ret = -errno;
        close(mFd);
        return ret;
    }

    if (int ret = ifc_enable(ifr.ifr_name)) {
        return ret;
    }
    return 0;
}

void TunInterface::destroy() {
    if (mFd != -1) {
        ifc_disable(mIfName.c_str());
        close(mFd);
        mFd = -1;
    }
}

int TunInterface::addAddress(const std::string& addr, int prefixlen) {
    // Wait for an RTM_NEWADDR indicating that the address has been created.
    // This is because IPv6 addresses, even addresses that are optimistic or created with
    // IFA_F_NODAD, are not immediately usable when the netlink ACK returns.
    // This is not generally necessary in device code because the framework hears about IP addresses
    // asynchronously via netlink, but it is necessary to ensure tests aren't flaky.
    unique_fd s(socket(AF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, 0));
    if (s == -1) return -errno;

    sockaddr_nl groups = {.nl_family = AF_NETLINK,
                          .nl_groups = RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR};
    if (bind(s, reinterpret_cast<sockaddr*>(&groups), sizeof(groups)) == -1) return -errno;

    sockaddr_nl kernel = {.nl_family = AF_NETLINK};
    if (connect(s, reinterpret_cast<sockaddr*>(&kernel), sizeof(kernel)) == -1) return -errno;

    // Wait up to 200ms for address to arrive.
    timeval timeout = {.tv_usec = 200 * 1000};
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) return -errno;

    if (ifc_add_address(mIfName.c_str(), addr.c_str(), prefixlen)) return -errno;

    int family;
    size_t addrlen;
    union {
        in_addr ip4;
        in6_addr ip6;
    } ip;
    if (addr.find(':') != std::string::npos) {
        family = AF_INET6;
        inet_pton(AF_INET6, addr.c_str(), &ip.ip6);
        addrlen = sizeof(ip.ip6);
    } else {
        family = AF_INET;
        inet_pton(AF_INET, addr.c_str(), &ip.ip4);
        addrlen = sizeof(ip.ip4);
    }

    while (1) {
        char buf[4096];
        ssize_t len = recv(s, buf, sizeof(buf), 0);

        if (len == -1) break;
        if (len < static_cast<ssize_t>(NLMSG_SPACE(sizeof(ifaddrmsg)))) continue;

        nlmsghdr* nlmsg = reinterpret_cast<nlmsghdr*>(buf);
        if (nlmsg->nlmsg_type != RTM_NEWADDR) continue;

        ifaddrmsg* ifaddr = reinterpret_cast<ifaddrmsg*>(NLMSG_DATA(nlmsg));
        if (ifaddr->ifa_family != family) continue;
        if (ifaddr->ifa_prefixlen != prefixlen) continue;
        if (ifaddr->ifa_index != static_cast<uint32_t>(mIfIndex)) continue;

        int ifalen = IFA_PAYLOAD(nlmsg);
        for (rtattr* rta = IFA_RTA(ifaddr); RTA_OK(rta, ifalen); rta = RTA_NEXT(rta, ifalen)) {
            if (rta->rta_type != IFA_LOCAL && rta->rta_type != IFA_ADDRESS) continue;
            if (RTA_PAYLOAD(rta) != addrlen) continue;
            if (!memcmp(RTA_DATA(rta), &ip, addrlen)) {
                return 0;
            }
        }
    }

    return -errno;
}

}  // namespace net
}  // namespace android
