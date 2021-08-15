/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <map>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_arp.h>
#include <linux/if_tun.h>
#include <linux/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LOG_TAG "ClatdController"
#include <log/log.h>

#include "ClatdController.h"

#include "android-base/properties.h"
#include "android-base/scopeguard.h"
#include "android-base/stringprintf.h"
#include "android-base/unique_fd.h"
#include "bpf/BpfMap.h"
#include "netdbpf/bpf_shared.h"
#include "netdutils/DumpWriter.h"

extern "C" {
#include "netutils/checksum.h"
}

#include "ClatUtils.h"
#include "Fwmark.h"
#include "NetdConstants.h"
#include "NetworkController.h"
#include "netid_client.h"

static const char* kClatdPath = "/system/bin/clatd";

// For historical reasons, start with 192.0.0.4, and after that, use all subsequent addresses in
// 192.0.0.0/29 (RFC 7335).
static const char* kV4AddrString = "192.0.0.4";
static const in_addr kV4Addr = {inet_addr(kV4AddrString)};
static const int kV4AddrLen = 29;

using android::base::StringPrintf;
using android::base::unique_fd;
using android::bpf::BpfMap;
using android::netdutils::DumpWriter;
using android::netdutils::ScopedIndent;

namespace android {
namespace net {

void ClatdController::init(void) {
    std::lock_guard guard(mutex);

    // TODO: should refactor into separate function for testability
    if (bpf::getBpfSupportLevel() == bpf::BpfLevel::NONE) {
        ALOGI("Pre-4.9 kernel or pre-P api shipping level - disabling clat ebpf.");
        mClatEbpfMode = ClatEbpfDisabled;
        return;
    }

    // We know the device initially shipped with at least P...,
    // but did it ship with at least Q?

    uint64_t api_level = base::GetUintProperty<uint64_t>("ro.product.first_api_level", 0);
    if (api_level == 0) {
        ALOGE("Cannot determine initial API level of the device.");
        api_level = base::GetUintProperty<uint64_t>("ro.build.version.sdk", 0);
    }

    // Note: MINIMUM_API_REQUIRED is for eBPF as a whole and is thus P
    if (api_level > bpf::MINIMUM_API_REQUIRED) {
        ALOGI("4.9+ kernel and device shipped with Q+ - clat ebpf should work.");
        mClatEbpfMode = ClatEbpfEnabled;
    } else {
        // We cannot guarantee that 4.9-P kernels will include NET_CLS_BPF support.
        ALOGI("4.9+ kernel and device shipped with P - clat ebpf might work.");
        mClatEbpfMode = ClatEbpfMaybe;
    }

    int rv = openNetlinkSocket();
    if (rv < 0) {
        ALOGE("openNetlinkSocket() failure: %s", strerror(-rv));
        mClatEbpfMode = ClatEbpfDisabled;
        return;
    }
    mNetlinkFd.reset(rv);

    rv = getClatIngressMapFd();
    if (rv < 0) {
        ALOGE("getClatIngressMapFd() failure: %s", strerror(-rv));
        mClatEbpfMode = ClatEbpfDisabled;
        mNetlinkFd.reset(-1);
        return;
    }
    mClatIngressMap.reset(rv);

    int netlinkFd = mNetlinkFd.get();

    // TODO: perhaps this initial cleanup should be in its own function?
    const auto del = [&netlinkFd](const ClatIngressKey& key,
                                  const BpfMap<ClatIngressKey, ClatIngressValue>&) {
        ALOGW("Removing stale clat config on interface %d.", key.iif);
        int rv = tcQdiscDelDevClsact(netlinkFd, key.iif);
        if (rv < 0) ALOGE("tcQdiscDelDevClsact() failure: %s", strerror(-rv));
        return netdutils::status::ok;  // keep on going regardless
    };
    auto ret = mClatIngressMap.iterate(del);
    if (!isOk(ret)) ALOGE("mClatIngressMap.iterate() failure: %s", strerror(ret.code()));
    ret = mClatIngressMap.clear();
    if (!isOk(ret)) ALOGE("mClatIngressMap.clear() failure: %s", strerror(ret.code()));
}

bool ClatdController::isIpv4AddressFree(in_addr_t addr) {
    int s = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (s == -1) {
        return 0;
    }

    // Attempt to connect to the address. If the connection succeeds and getsockname returns the
    // same then the address is already assigned to the system and we can't use it.
    struct sockaddr_in sin = {.sin_family = AF_INET, .sin_addr = {addr}, .sin_port = 53};
    socklen_t len = sizeof(sin);
    bool inuse = connect(s, (struct sockaddr*)&sin, sizeof(sin)) == 0 &&
                 getsockname(s, (struct sockaddr*)&sin, &len) == 0 && (size_t)len >= sizeof(sin) &&
                 sin.sin_addr.s_addr == addr;

    close(s);
    return !inuse;
}

// Picks a free IPv4 address, starting from ip and trying all addresses in the prefix in order.
//   ip        - the IP address from the configuration file
//   prefixlen - the length of the prefix from which addresses may be selected.
//   returns: the IPv4 address, or INADDR_NONE if no addresses were available
in_addr_t ClatdController::selectIpv4Address(const in_addr ip, int16_t prefixlen) {
    // Don't accept prefixes that are too large because we scan addresses one by one.
    if (prefixlen < 16 || prefixlen > 32) {
        return INADDR_NONE;
    }

    // All these are in host byte order.
    in_addr_t mask = 0xffffffff >> (32 - prefixlen) << (32 - prefixlen);
    in_addr_t ipv4 = ntohl(ip.s_addr);
    in_addr_t first_ipv4 = ipv4;
    in_addr_t prefix = ipv4 & mask;

    // Pick the first IPv4 address in the pool, wrapping around if necessary.
    // So, for example, 192.0.0.4 -> 192.0.0.5 -> 192.0.0.6 -> 192.0.0.7 -> 192.0.0.0.
    do {
        if (isIpv4AddressFreeFunc(htonl(ipv4))) {
            return htonl(ipv4);
        }
        ipv4 = prefix | ((ipv4 + 1) & ~mask);
    } while (ipv4 != first_ipv4);

    return INADDR_NONE;
}

// Alters the bits in the IPv6 address to make them checksum neutral with v4 and nat64Prefix.
void ClatdController::makeChecksumNeutral(in6_addr* v6, const in_addr v4,
                                          const in6_addr& nat64Prefix) {
    // Fill last 8 bytes of IPv6 address with random bits.
    arc4random_buf(&v6->s6_addr[8], 8);

    // Make the IID checksum-neutral. That is, make it so that:
    //   checksum(Local IPv4 | Remote IPv4) = checksum(Local IPv6 | Remote IPv6)
    // in other words (because remote IPv6 = NAT64 prefix | Remote IPv4):
    //   checksum(Local IPv4) = checksum(Local IPv6 | NAT64 prefix)
    // Do this by adjusting the two bytes in the middle of the IID.

    uint16_t middlebytes = (v6->s6_addr[11] << 8) + v6->s6_addr[12];

    uint32_t c1 = ip_checksum_add(0, &v4, sizeof(v4));
    uint32_t c2 = ip_checksum_add(0, &nat64Prefix, sizeof(nat64Prefix)) +
                  ip_checksum_add(0, v6, sizeof(*v6));

    uint16_t delta = ip_checksum_adjust(middlebytes, c1, c2);
    v6->s6_addr[11] = delta >> 8;
    v6->s6_addr[12] = delta & 0xff;
}

// Picks a random interface ID that is checksum neutral with the IPv4 address and the NAT64 prefix.
int ClatdController::generateIpv6Address(const char* iface, const in_addr v4,
                                         const in6_addr& nat64Prefix, in6_addr* v6) {
    unique_fd s(socket(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0));
    if (s == -1) return -errno;

    if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface) + 1) == -1) {
        return -errno;
    }

    sockaddr_in6 sin6 = {.sin6_family = AF_INET6, .sin6_addr = nat64Prefix};
    if (connect(s, reinterpret_cast<struct sockaddr*>(&sin6), sizeof(sin6)) == -1) {
        return -errno;
    }

    socklen_t len = sizeof(sin6);
    if (getsockname(s, reinterpret_cast<struct sockaddr*>(&sin6), &len) == -1) {
        return -errno;
    }

    *v6 = sin6.sin6_addr;

    if (IN6_IS_ADDR_UNSPECIFIED(v6) || IN6_IS_ADDR_LOOPBACK(v6) || IN6_IS_ADDR_LINKLOCAL(v6) ||
        IN6_IS_ADDR_SITELOCAL(v6) || IN6_IS_ADDR_ULA(v6)) {
        return -ENETUNREACH;
    }

    makeChecksumNeutral(v6, v4, nat64Prefix);

    return 0;
}

void ClatdController::maybeStartBpf(const ClatdTracker& tracker) {
    if (mClatEbpfMode == ClatEbpfDisabled) return;

    int rv = hardwareAddressType(tracker.iface);
    if (rv < 0) {
        ALOGE("hardwareAddressType(%s[%d]) failure: %s", tracker.iface, tracker.ifIndex,
              strerror(-rv));
        return;
    }

    bool isEthernet;
    switch (rv) {
        case ARPHRD_ETHER:
            isEthernet = true;
            break;
        case ARPHRD_RAWIP:  // in Linux 4.14+ rmnet support was upstreamed and this is 519
        case 530:           // this is ARPHRD_RAWIP on some Android 4.9 kernels with rmnet
            isEthernet = false;
            break;
        default:
            ALOGE("hardwareAddressType(%s[%d]) returned unknown type %d.", tracker.iface,
                  tracker.ifIndex, rv);
            return;
    }

    rv = getClatIngressProgFd(isEthernet);
    if (rv < 0) {
        ALOGE("getClatIngressProgFd(%d) failure: %s", isEthernet, strerror(-rv));
        return;
    }
    unique_fd progFd(rv);

    ClatIngressKey key = {
            .iif = tracker.ifIndex,
            .pfx96 = tracker.pfx96,
            .local6 = tracker.v6,
    };
    ClatIngressValue value = {
            // TODO: move all the clat code to eBPF and remove the tun interface entirely.
            .oif = tracker.v4ifIndex,
            .local4 = tracker.v4,
    };

    auto ret = mClatIngressMap.writeValue(key, value, BPF_ANY);
    if (!isOk(ret)) {
        ALOGE("mClatIngress.Map.writeValue failure: %s", strerror(ret.code()));
        return;
    }

    // We do tc setup *after* populating map, so scanning through map
    // can always be used to tell us what needs cleanup.

    rv = tcQdiscAddDevClsact(mNetlinkFd, tracker.ifIndex);
    if (rv) {
        ALOGE("tcQdiscAddDevClsact(%d[%s]) failure: %s", tracker.ifIndex, tracker.iface,
              strerror(-rv));
        ret = mClatIngressMap.deleteValue(key);
        if (!isOk(ret)) ALOGE("mClatIngressMap.deleteValue failure: %s", strerror(ret.code()));
        return;
    }

    rv = tcFilterAddDevBpf(mNetlinkFd, tracker.ifIndex, progFd, isEthernet);
    if (rv) {
        if ((rv == -ENOENT) && (mClatEbpfMode == ClatEbpfMaybe)) {
            ALOGI("tcFilterAddDevBpf(%d[%s], %d): %s", tracker.ifIndex, tracker.iface, isEthernet,
                  strerror(-rv));
        } else {
            ALOGE("tcFilterAddDevBpf(%d[%s], %d) failure: %s", tracker.ifIndex, tracker.iface,
                  isEthernet, strerror(-rv));
        }
        rv = tcQdiscDelDevClsact(mNetlinkFd, tracker.ifIndex);
        if (rv)
            ALOGE("tcQdiscDelDevClsact(%d[%s]) failure: %s", tracker.ifIndex, tracker.iface,
                  strerror(-rv));
        ret = mClatIngressMap.deleteValue(key);
        if (!isOk(ret)) ALOGE("mClatIngressMap.deleteValue failure: %s", strerror(ret.code()));
        return;
    }

    // success
}

void ClatdController::maybeSetIptablesDropRule(bool add, const char* pfx96Str, const char* v6Str) {
    if (mClatEbpfMode == ClatEbpfDisabled) return;

    std::string cmd = StringPrintf(
            "*raw\n"
            "%s %s -s %s/96 -d %s -j DROP\n"
            "COMMIT\n",
            (add ? "-A" : "-D"), LOCAL_RAW_PREROUTING, pfx96Str, v6Str);

    iptablesRestoreFunction(V6, cmd);
}

void ClatdController::maybeStopBpf(const ClatdTracker& tracker) {
    if (mClatEbpfMode == ClatEbpfDisabled) return;

    // No need to remove filter, since we remove qdisc it is attached to,
    // which automatically removes everything attached to the qdisc.
    int rv = tcQdiscDelDevClsact(mNetlinkFd, tracker.ifIndex);
    if (rv < 0)
        ALOGE("tcQdiscDelDevClsact(%d[%s]) failure: %s", tracker.ifIndex, tracker.iface,
              strerror(-rv));

    // We cleanup map last, so scanning through map can be used to
    // determine what still needs cleanup.

    ClatIngressKey key = {
            .iif = tracker.ifIndex,
            .pfx96 = tracker.pfx96,
            .local6 = tracker.v6,
    };

    auto ret = mClatIngressMap.deleteValue(key);
    if (!isOk(ret)) ALOGE("mClatIngressMap.deleteValue failure: %s", strerror(ret.code()));
}

// Finds the tracker of the clatd running on interface |interface|, or nullptr if clatd has not been
// started  on |interface|.
ClatdController::ClatdTracker* ClatdController::getClatdTracker(const std::string& interface) {
    auto it = mClatdTrackers.find(interface);
    return (it == mClatdTrackers.end() ? nullptr : &it->second);
}

// Initializes a ClatdTracker for the specified interface.
int ClatdController::ClatdTracker::init(unsigned networkId, const std::string& interface,
                                        const std::string& v4interface,
                                        const std::string& nat64Prefix) {
    netId = networkId;

    fwmark.netId = netId;
    fwmark.explicitlySelected = true;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;

    snprintf(fwmarkString, sizeof(fwmarkString), "0x%x", fwmark.intValue);
    snprintf(netIdString, sizeof(netIdString), "%u", netId);
    strlcpy(iface, interface.c_str(), sizeof(iface));
    ifIndex = if_nametoindex(iface);
    strlcpy(v4iface, v4interface.c_str(), sizeof(v4iface));
    v4ifIndex = if_nametoindex(v4iface);

    // Pass in everything that clatd needs: interface, a netid to use for DNS lookups, a fwmark for
    // outgoing packets, the NAT64 prefix, and the IPv4 and IPv6 addresses.
    // Validate the prefix and strip off the prefix length.
    uint8_t family;
    uint8_t prefixLen;
    int res = parsePrefix(nat64Prefix.c_str(), &family, &pfx96, sizeof(pfx96), &prefixLen);
    // clatd only supports /96 prefixes.
    if (res != sizeof(pfx96)) return res;
    if (family != AF_INET6) return -EAFNOSUPPORT;
    if (prefixLen != 96) return -EINVAL;
    if (!inet_ntop(AF_INET6, &pfx96, pfx96String, sizeof(pfx96String))) return -errno;

    // Pick an IPv4 address.
    // TODO: this picks the address based on other addresses that are assigned to interfaces, but
    // the address is only actually assigned to an interface once clatd starts up. So we could end
    // up with two clatd instances with the same IPv4 address.
    // Stop doing this and instead pick a free one from the kV4Addr pool.
    v4 = {selectIpv4Address(kV4Addr, kV4AddrLen)};
    if (v4.s_addr == INADDR_NONE) {
        ALOGE("No free IPv4 address in %s/%d", kV4AddrString, kV4AddrLen);
        return -EADDRNOTAVAIL;
    }
    if (!inet_ntop(AF_INET, &v4, v4Str, sizeof(v4Str))) return -errno;

    // Generate a checksum-neutral IID.
    if (generateIpv6Address(iface, v4, pfx96, &v6)) {
        ALOGE("Unable to find global source address on %s for %s", iface, pfx96String);
        return -EADDRNOTAVAIL;
    }
    if (!inet_ntop(AF_INET6, &v6, v6Str, sizeof(v6Str))) return -errno;

    ALOGD("starting clatd on %s v4=%s v6=%s pfx96=%s", iface, v4Str, v6Str, pfx96String);
    return 0;
}

int ClatdController::startClatd(const std::string& interface, const std::string& nat64Prefix,
                                std::string* v6Str) {
    std::lock_guard guard(mutex);

    // 1. fail if pre-existing tracker already exists
    ClatdTracker* existing = getClatdTracker(interface);
    if (existing != nullptr) {
        ALOGE("clatd pid=%d already started on %s", existing->pid, interface.c_str());
        return -EBUSY;
    }

    // 2. get network id associated with this external interface
    unsigned networkId = mNetCtrl->getNetworkForInterface(interface.c_str());
    if (networkId == NETID_UNSET) {
        ALOGE("Interface %s not assigned to any netId", interface.c_str());
        return -ENODEV;
    }

    // 3. open the tun device in non blocking mode as required by clatd
    int res = open("/dev/tun", O_RDWR | O_NONBLOCK | O_CLOEXEC);
    if (res == -1) {
        res = errno;
        ALOGE("open of tun device failed (%s)", strerror(res));
        return -res;
    }
    unique_fd tmpTunFd(res);

    // 4. create the v4-... tun interface
    std::string v4interface("v4-");
    v4interface += interface;

    struct ifreq ifr = {
            .ifr_flags = IFF_TUN,
    };
    strlcpy(ifr.ifr_name, v4interface.c_str(), sizeof(ifr.ifr_name));

    res = ioctl(tmpTunFd, TUNSETIFF, &ifr, sizeof(ifr));
    if (res == -1) {
        res = errno;
        ALOGE("ioctl(TUNSETIFF) failed (%s)", strerror(res));
        return -res;
    }

    // 5. initialize tracker object
    ClatdTracker tracker;
    int ret = tracker.init(networkId, interface, v4interface, nat64Prefix);
    if (ret) return ret;

    // 6. create a throwaway socket to reserve a file descriptor number
    res = socket(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (res == -1) {
        res = errno;
        ALOGE("socket(ipv6/udp) failed (%s)", strerror(res));
        return -res;
    }
    unique_fd passedTunFd(res);

    // 7. this is the FD we'll pass to clatd on the cli, so need it as a string
    char passedTunFdStr[INT32_STRLEN];
    snprintf(passedTunFdStr, sizeof(passedTunFdStr), "%d", passedTunFd.get());

    // 8. we're going to use this as argv[0] to clatd to make ps output more useful
    std::string progname("clatd-");
    progname += tracker.iface;

    // clang-format off
    const char* args[] = {progname.c_str(),
                          "-i", tracker.iface,
                          "-n", tracker.netIdString,
                          "-m", tracker.fwmarkString,
                          "-p", tracker.pfx96String,
                          "-4", tracker.v4Str,
                          "-6", tracker.v6Str,
                          "-t", passedTunFdStr,
                          nullptr};
    // clang-format on

    // 9. register vfork requirement
    posix_spawnattr_t attr;
    res = posix_spawnattr_init(&attr);
    if (res) {
        ALOGE("posix_spawnattr_init failed (%s)", strerror(res));
        return -res;
    }
    const android::base::ScopeGuard attrGuard = [&] { posix_spawnattr_destroy(&attr); };
    res = posix_spawnattr_setflags(&attr, POSIX_SPAWN_USEVFORK);
    if (res) {
        ALOGE("posix_spawnattr_setflags failed (%s)", strerror(res));
        return -res;
    }

    // 10. register dup2() action: this is what 'clears' the CLOEXEC flag
    // on the tun fd that we want the child clatd process to inherit
    // (this will happen after the vfork, and before the execve)
    posix_spawn_file_actions_t fa;
    res = posix_spawn_file_actions_init(&fa);
    if (res) {
        ALOGE("posix_spawn_file_actions_init failed (%s)", strerror(res));
        return -res;
    }
    const android::base::ScopeGuard faGuard = [&] { posix_spawn_file_actions_destroy(&fa); };
    res = posix_spawn_file_actions_adddup2(&fa, tmpTunFd, passedTunFd);
    if (res) {
        ALOGE("posix_spawn_file_actions_adddup2 failed (%s)", strerror(res));
        return -res;
    }

    // 11. If necessary, add the drop rule for iptables.
    maybeSetIptablesDropRule(true, tracker.pfx96String, tracker.v6Str);

    // 12. actually perform vfork/dup2/execve
    res = posix_spawn(&tracker.pid, kClatdPath, &fa, &attr, (char* const*)args, nullptr);
    if (res) {
        ALOGE("posix_spawn failed (%s)", strerror(res));
        return -res;
    }

    // 13. configure eBPF offload - if possible
    maybeStartBpf(tracker);

    mClatdTrackers[interface] = tracker;
    ALOGD("clatd started on %s", interface.c_str());

    *v6Str = tracker.v6Str;
    return 0;
}

int ClatdController::stopClatd(const std::string& interface) {
    std::lock_guard guard(mutex);
    ClatdTracker* tracker = getClatdTracker(interface);

    if (tracker == nullptr) {
        ALOGE("clatd already stopped");
        return -ENODEV;
    }

    ALOGD("Stopping clatd pid=%d on %s", tracker->pid, interface.c_str());

    maybeStopBpf(*tracker);

    kill(tracker->pid, SIGTERM);
    waitpid(tracker->pid, nullptr, 0);

    maybeSetIptablesDropRule(false, tracker->pfx96String, tracker->v6Str);
    mClatdTrackers.erase(interface);

    ALOGD("clatd on %s stopped", interface.c_str());

    return 0;
}

void ClatdController::dump(DumpWriter& dw) {
    std::lock_guard guard(mutex);

    ScopedIndent clatdIndent(dw);
    dw.println("ClatdController");

    {
        ScopedIndent trackerIndent(dw);
        dw.println("Trackers: iif[iface] nat64Prefix v6Addr -> v4Addr v4iif[v4iface] [netId]");

        ScopedIndent trackerDetailIndent(dw);
        for (const auto& pair : mClatdTrackers) {
            const ClatdTracker& tracker = pair.second;
            dw.println("%u[%s] %s/96 %s -> %s %u[%s] [%u]", tracker.ifIndex, tracker.iface,
                       tracker.pfx96String, tracker.v6Str, tracker.v4Str, tracker.v4ifIndex,
                       tracker.v4iface, tracker.netId);
        }
    }

    int mapFd = getClatIngressMapFd();
    if (mapFd < 0) return;  // if unsupported just don't dump anything
    BpfMap<ClatIngressKey, ClatIngressValue> configMap(mapFd);

    ScopedIndent bpfIndent(dw);
    dw.println("BPF ingress map: iif(iface) nat64Prefix v6Addr -> v4Addr oif(iface)");

    ScopedIndent bpfDetailIndent(dw);
    const auto printClatMap = [&dw](const ClatIngressKey& key, const ClatIngressValue& value,
                                    const BpfMap<ClatIngressKey, ClatIngressValue>&) {
        char iifStr[IFNAMSIZ] = "?";
        char pfx96Str[INET6_ADDRSTRLEN] = "?";
        char local6Str[INET6_ADDRSTRLEN] = "?";
        char local4Str[INET_ADDRSTRLEN] = "?";
        char oifStr[IFNAMSIZ] = "?";

        if_indextoname(key.iif, iifStr);
        inet_ntop(AF_INET6, &key.pfx96, pfx96Str, sizeof(pfx96Str));
        inet_ntop(AF_INET6, &key.local6, local6Str, sizeof(local6Str));
        inet_ntop(AF_INET, &value.local4, local4Str, sizeof(local4Str));
        if_indextoname(value.oif, oifStr);

        dw.println("%u(%s) %s/96 %s -> %s %u(%s)", key.iif, iifStr, pfx96Str, local6Str, local4Str,
                   value.oif, oifStr);
        return netdutils::status::ok;
    };
    auto res = configMap.iterateWithValue(printClatMap);
    if (!isOk(res)) {
        dw.println("Error printing BPF map: %s", res.msg().c_str());
    }
}

auto ClatdController::isIpv4AddressFreeFunc = isIpv4AddressFree;
auto ClatdController::iptablesRestoreFunction = execIptablesRestore;

}  // namespace net
}  // namespace android
