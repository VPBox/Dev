/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dirent.h>
#include <errno.h>
#include <malloc.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/socket.h>

#include <functional>

#define LOG_TAG "InterfaceController"
#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <linux/if_ether.h>
#include <log/log.h>
#include <logwrap/logwrap.h>
#include <netutils/ifc.h>

#include <android/net/INetd.h>
#include <netdutils/Misc.h>
#include <netdutils/Slice.h>
#include <netdutils/Syscalls.h>

#include "InterfaceController.h"
#include "RouteController.h"

using android::base::ReadFileToString;
using android::base::StringPrintf;
using android::base::Trim;
using android::base::WriteStringToFile;
using android::net::INetd;
using android::net::RouteController;
using android::netdutils::isOk;
using android::netdutils::makeSlice;
using android::netdutils::sSyscalls;
using android::netdutils::Status;
using android::netdutils::statusFromErrno;
using android::netdutils::StatusOr;
using android::netdutils::toString;
using android::netdutils::status::ok;

#define RETURN_STATUS_IF_IFCERROR(exp)                           \
    do {                                                         \
        if ((exp) == -1) {                                       \
            return statusFromErrno(errno, "Failed to add addr"); \
        }                                                        \
    } while (0);

namespace {

const char ipv4_proc_path[] = "/proc/sys/net/ipv4/conf";
const char ipv6_proc_path[] = "/proc/sys/net/ipv6/conf";

const char ipv4_neigh_conf_dir[] = "/proc/sys/net/ipv4/neigh";
const char ipv6_neigh_conf_dir[] = "/proc/sys/net/ipv6/neigh";

const char proc_net_path[] = "/proc/sys/net";
const char sys_net_path[] = "/sys/class/net";

constexpr int kRouteInfoMinPrefixLen = 48;

// RFC 7421 prefix length.
constexpr int kRouteInfoMaxPrefixLen = 64;

// Property used to persist RFC 7217 stable secret. Protected by SELinux policy.
const char kStableSecretProperty[] = "persist.netd.stable_secret";

// RFC 7217 stable secret on linux is formatted as an IPv6 address.
// This function uses 128 bits of high quality entropy to generate an
// address for this purpose. This function should be not be called
// frequently.
StatusOr<std::string> randomIPv6Address() {
    in6_addr addr = {};
    const auto& sys = sSyscalls.get();
    ASSIGN_OR_RETURN(auto fd, sys.open("/dev/random", O_RDONLY));
    RETURN_IF_NOT_OK(sys.read(fd, makeSlice(addr)));
    return toString(addr);
}

inline bool isNormalPathComponent(const char *component) {
    return (strcmp(component, ".") != 0) &&
           (strcmp(component, "..") != 0) &&
           (strchr(component, '/') == nullptr);
}

inline bool isAddressFamilyPathComponent(const char *component) {
    return strcmp(component, "ipv4") == 0 || strcmp(component, "ipv6") == 0;
}

inline bool isInterfaceName(const char *name) {
    return isNormalPathComponent(name) &&
           (strcmp(name, "default") != 0) &&
           (strcmp(name, "all") != 0);
}

int writeValueToPath(
        const char* dirname, const char* subdirname, const char* basename,
        const char* value) {
    std::string path(StringPrintf("%s/%s/%s", dirname, subdirname, basename));
    return WriteStringToFile(value, path) ? 0 : -EREMOTEIO;
}

// Run @fn on each interface as well as 'default' in the path @dirname.
void forEachInterface(
        const std::string& dirname,
        const std::function<void(const std::string& path, const std::string& iface)>& fn) {
    // Run on default, which controls the behavior of any interfaces that are created in the future.
    fn(dirname, "default");
    DIR* dir = opendir(dirname.c_str());
    if (!dir) {
        ALOGE("Can't list %s: %s", dirname.c_str(), strerror(errno));
        return;
    }
    while (true) {
        const dirent *ent = readdir(dir);
        if (!ent) {
            break;
        }
        if ((ent->d_type != DT_DIR) || !isInterfaceName(ent->d_name)) {
            continue;
        }
        fn(dirname, ent->d_name);
    }
    closedir(dir);
}

void setOnAllInterfaces(const char* dirname, const char* basename, const char* value) {
    auto fn = [basename, value](const std::string& path, const std::string& iface) {
        writeValueToPath(path.c_str(), iface.c_str(), basename, value);
    };
    forEachInterface(dirname, fn);
}

void setIPv6UseOutgoingInterfaceAddrsOnly(const char *value) {
    setOnAllInterfaces(ipv6_proc_path, "use_oif_addrs_only", value);
}

std::string getParameterPathname(
        const char *family, const char *which, const char *interface, const char *parameter) {
    if (!isAddressFamilyPathComponent(family)) {
        errno = EAFNOSUPPORT;
        return "";
    } else if (!isNormalPathComponent(which) ||
               !isInterfaceName(interface) ||
               !isNormalPathComponent(parameter)) {
        errno = EINVAL;
        return "";
    }

    return StringPrintf("%s/%s/%s/%s/%s", proc_net_path, family, which, interface, parameter);
}

void setAcceptIPv6RIO(int min, int max) {
    auto fn = [min, max](const std::string& prefix, const std::string& iface) {
        int rv = writeValueToPath(prefix.c_str(), iface.c_str(), "accept_ra_rt_info_min_plen",
                                  std::to_string(min).c_str());
        if (rv != 0) {
            // Only update max_plen if the write to min_plen succeeded. This ordering will prevent
            // RIOs from being accepted unless both min and max are written successfully.
            return;
        }
        writeValueToPath(prefix.c_str(), iface.c_str(), "accept_ra_rt_info_max_plen",
                         std::to_string(max).c_str());
    };
    forEachInterface(ipv6_proc_path, fn);
}

// Ideally this function would return StatusOr<std::string>, however
// there is no safe value for dflt that will always differ from the
// stored property. Bugs code could conceivably end up persisting the
// reserved value resulting in surprising behavior.
std::string getProperty(const std::string& key, const std::string& dflt) {
    return android::base::GetProperty(key, dflt);
};

Status setProperty(const std::string& key, const std::string& val) {
    // SetProperty does not dependably set errno to a meaningful value. Use our own error code so
    // callers don't get confused.
    return android::base::SetProperty(key, val)
        ? ok
        : statusFromErrno(EREMOTEIO, "SetProperty failed, see libc logs");
};


}  // namespace

namespace android {
namespace net {
std::mutex InterfaceController::mutex;

android::netdutils::Status InterfaceController::enableStablePrivacyAddresses(
        const std::string& iface,
        const GetPropertyFn& getProperty,
        const SetPropertyFn& setProperty) {
    const auto& sys = sSyscalls.get();
    const std::string procTarget = std::string(ipv6_proc_path) + "/" + iface + "/stable_secret";
    auto procFd = sys.open(procTarget, O_CLOEXEC | O_WRONLY);

    // Devices with old kernels (typically < 4.4) don't support
    // RFC 7217 stable privacy addresses.
    if (equalToErrno(procFd, ENOENT)) {
        return statusFromErrno(EOPNOTSUPP,
                               "Failed to open stable_secret. Assuming unsupported kernel version");
    }

    // If stable_secret exists but we can't open it, something strange is going on.
    RETURN_IF_NOT_OK(procFd);

    const char kUninitialized[] = "uninitialized";
    const auto oldSecret = getProperty(kStableSecretProperty, kUninitialized);
    std::string secret = oldSecret;

    // Generate a new secret if no persistent property existed.
    if (oldSecret == kUninitialized) {
        ASSIGN_OR_RETURN(secret, randomIPv6Address());
    }

    // Ask the OS to generate SLAAC addresses on iface using secret.
    RETURN_IF_NOT_OK(sys.write(procFd.value(), makeSlice(secret)));

    // Don't persist an existing secret.
    if (oldSecret != kUninitialized) {
        return ok;
    }

    return setProperty(kStableSecretProperty, secret);
}

void InterfaceController::initializeAll() {
    // Initial IPv6 settings.
    // By default, accept_ra is set to 1 (accept RAs unless forwarding is on) on all interfaces.
    // This causes RAs to work or not work based on whether forwarding is on, and causes routes
    // learned from RAs to go away when forwarding is turned on. Make this behaviour predictable
    // by always setting accept_ra to 2.
    setAcceptRA("2");

    // Accept RIOs with prefix length in the closed interval [48, 64].
    setAcceptIPv6RIO(kRouteInfoMinPrefixLen, kRouteInfoMaxPrefixLen);

    setAcceptRARouteTable(-RouteController::ROUTE_TABLE_OFFSET_FROM_INDEX);

    // Enable optimistic DAD for IPv6 addresses on all interfaces.
    setIPv6OptimisticMode("1");

    // Reduce the ARP/ND base reachable time from the default (30sec) to 15sec.
    setBaseReachableTimeMs(15 * 1000);

    // When sending traffic via a given interface use only addresses configured
    // on that interface as possible source addresses.
    setIPv6UseOutgoingInterfaceAddrsOnly("1");

    // Ensure that ICMP redirects are rejected globally on all interfaces.
    disableIcmpRedirects();
}

int InterfaceController::setEnableIPv6(const char *interface, const int on) {
    if (!isIfaceName(interface)) {
        return -ENOENT;
    }
    // When disable_ipv6 changes from 1 to 0, the kernel starts autoconf.
    // When disable_ipv6 changes from 0 to 1, the kernel clears all autoconf
    // addresses and routes and disables IPv6 on the interface.
    const char *disable_ipv6 = on ? "0" : "1";
    return writeValueToPath(ipv6_proc_path, interface, "disable_ipv6", disable_ipv6);
}

// Changes to addrGenMode will not fully take effect until the next
// time disable_ipv6 transitions from 1 to 0.
Status InterfaceController::setIPv6AddrGenMode(const std::string& interface, int mode) {
    if (!isIfaceName(interface)) {
        return statusFromErrno(ENOENT, "invalid iface name: " + interface);
    }

    switch (mode) {
        case INetd::IPV6_ADDR_GEN_MODE_EUI64:
            // Ignore return value. If /proc/.../stable_secret is
            // missing we're probably in EUI64 mode already.
            writeValueToPath(ipv6_proc_path, interface.c_str(), "stable_secret", "");
            break;
        case INetd::IPV6_ADDR_GEN_MODE_STABLE_PRIVACY: {
            return enableStablePrivacyAddresses(interface, getProperty, setProperty);
        }
        case INetd::IPV6_ADDR_GEN_MODE_NONE:
        case INetd::IPV6_ADDR_GEN_MODE_RANDOM:
        default:
            return statusFromErrno(EOPNOTSUPP, "unsupported addrGenMode");
    }

    return ok;
}

int InterfaceController::setAcceptIPv6Ra(const char *interface, const int on) {
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -1;
    }
    // Because forwarding can be enabled even when tethering is off, we always
    // use mode "2" (accept RAs, even if forwarding is enabled).
    const char *accept_ra = on ? "2" : "0";
    return writeValueToPath(ipv6_proc_path, interface, "accept_ra", accept_ra);
}

int InterfaceController::setAcceptIPv6Dad(const char *interface, const int on) {
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -1;
    }
    const char *accept_dad = on ? "1" : "0";
    return writeValueToPath(ipv6_proc_path, interface, "accept_dad", accept_dad);
}

int InterfaceController::setIPv6DadTransmits(const char *interface, const char *value) {
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -1;
    }
    return writeValueToPath(ipv6_proc_path, interface, "dad_transmits", value);
}

int InterfaceController::setIPv6PrivacyExtensions(const char *interface, const int on) {
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -errno;
    }
    // 0: disable IPv6 privacy addresses
    // 2: enable IPv6 privacy addresses and prefer them over non-privacy ones.
    return writeValueToPath(ipv6_proc_path, interface, "use_tempaddr", on ? "2" : "0");
}

void InterfaceController::setAcceptRA(const char *value) {
    setOnAllInterfaces(ipv6_proc_path, "accept_ra", value);
}

// |tableOrOffset| is interpreted as:
//     If == 0: default. Routes go into RT6_TABLE_MAIN.
//     If > 0: user set. Routes go into the specified table.
//     If < 0: automatic. The absolute value is intepreted as an offset and added to the interface
//             ID to get the table. If it's set to -1000, routes from interface ID 5 will go into
//             table 1005, etc.
void InterfaceController::setAcceptRARouteTable(int tableOrOffset) {
    std::string value(StringPrintf("%d", tableOrOffset));
    setOnAllInterfaces(ipv6_proc_path, "accept_ra_rt_table", value.c_str());
}

int InterfaceController::setMtu(const char *interface, const char *mtu)
{
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -errno;
    }
    return writeValueToPath(sys_net_path, interface, "mtu", mtu);
}

int InterfaceController::addAddress(const char *interface,
        const char *addrString, int prefixLength) {
    return ifc_add_address(interface, addrString, prefixLength);
}

int InterfaceController::delAddress(const char *interface,
        const char *addrString, int prefixLength) {
    return ifc_del_address(interface, addrString, prefixLength);
}

int InterfaceController::disableIcmpRedirects() {
    int rv = 0;
    rv |= writeValueToPath(ipv4_proc_path, "all", "accept_redirects", "0");
    rv |= writeValueToPath(ipv6_proc_path, "all", "accept_redirects", "0");
    setOnAllInterfaces(ipv4_proc_path, "accept_redirects", "0");
    setOnAllInterfaces(ipv6_proc_path, "accept_redirects", "0");
    return rv;
}

int InterfaceController::getParameter(
        const char *family, const char *which, const char *interface, const char *parameter,
        std::string *value) {
    const std::string path(getParameterPathname(family, which, interface, parameter));
    if (path.empty()) {
        return -errno;
    }
    if (ReadFileToString(path, value)) {
        *value = Trim(*value);
        return 0;
    }
    return -errno;
}

int InterfaceController::setParameter(
        const char *family, const char *which, const char *interface, const char *parameter,
        const char *value) {
    const std::string path(getParameterPathname(family, which, interface, parameter));
    if (path.empty()) {
        return -errno;
    }
    return WriteStringToFile(value, path) ? 0 : -errno;
}

void InterfaceController::setBaseReachableTimeMs(unsigned int millis) {
    std::string value(StringPrintf("%u", millis));
    setOnAllInterfaces(ipv4_neigh_conf_dir, "base_reachable_time_ms", value.c_str());
    setOnAllInterfaces(ipv6_neigh_conf_dir, "base_reachable_time_ms", value.c_str());
}

void InterfaceController::setIPv6OptimisticMode(const char *value) {
    setOnAllInterfaces(ipv6_proc_path, "optimistic_dad", value);
    setOnAllInterfaces(ipv6_proc_path, "use_optimistic", value);
}

StatusOr<std::vector<std::string>> InterfaceController::getIfaceNames() {
    std::vector<std::string> ifaceNames;
    DIR* d;
    struct dirent* de;

    if (!(d = opendir("/sys/class/net"))) {
        return statusFromErrno(errno, "Cannot open iface directory");
    }
    while ((de = readdir(d))) {
        if ((de->d_type != DT_DIR) && (de->d_type != DT_LNK)) continue;
        if (de->d_name[0] == '.') continue;
        ifaceNames.push_back(std::string(de->d_name));
    }
    closedir(d);
    return ifaceNames;
}

StatusOr<std::map<std::string, uint32_t>> InterfaceController::getIfaceList() {
    std::map<std::string, uint32_t> ifacePairs;

    ASSIGN_OR_RETURN(auto ifaceNames, getIfaceNames());

    for (const auto& name : ifaceNames) {
        uint32_t ifaceIndex = if_nametoindex(name.c_str());
        if (ifaceIndex) {
            ifacePairs.insert(std::pair<std::string, uint32_t>(name, ifaceIndex));
        }
    }
    return ifacePairs;
}

namespace {

std::string hwAddrToStr(unsigned char* hwaddr) {
    return StringPrintf("%02x:%02x:%02x:%02x:%02x:%02x", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3],
                        hwaddr[4], hwaddr[5]);
}

int ipv4NetmaskToPrefixLength(in_addr_t mask) {
    int prefixLength = 0;
    uint32_t m = ntohl(mask);
    while (m & (1 << 31)) {
        prefixLength++;
        m = m << 1;
    }
    return prefixLength;
}

std::string toStdString(const String16& s) {
    return std::string(String8(s.string()));
}

}  // namespace

Status InterfaceController::setCfg(const InterfaceConfigurationParcel& cfg) {
    const auto& sys = sSyscalls.get();
    ASSIGN_OR_RETURN(auto fd, sys.socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0));
    struct ifreq ifr = {
            .ifr_addr = {.sa_family = AF_INET},  // Clear the IPv4 address.
    };
    strlcpy(ifr.ifr_name, cfg.ifName.c_str(), IFNAMSIZ);

    // Make sure that clear IPv4 address before set flag
    // SIOCGIFFLAGS might override ifr and caused clear IPv4 addr ioctl error
    RETURN_IF_NOT_OK(sys.ioctl(fd, SIOCSIFADDR, &ifr));

    if (!cfg.flags.empty()) {
        RETURN_IF_NOT_OK(sys.ioctl(fd, SIOCGIFFLAGS, &ifr));
        uint16_t flags = ifr.ifr_flags;

        for (const auto& flag : cfg.flags) {
            if (flag == toStdString(INetd::IF_STATE_UP())) {
                ifr.ifr_flags = ifr.ifr_flags | IFF_UP;
            } else if (flag == toStdString(INetd::IF_STATE_DOWN())) {
                ifr.ifr_flags = (ifr.ifr_flags & (~IFF_UP));
            }
        }

        if (ifr.ifr_flags != flags) {
            RETURN_IF_NOT_OK(sys.ioctl(fd, SIOCSIFFLAGS, &ifr));
        }
    }

    RETURN_STATUS_IF_IFCERROR(
            ifc_add_address(cfg.ifName.c_str(), cfg.ipv4Addr.c_str(), cfg.prefixLength));

    return ok;
}

StatusOr<InterfaceConfigurationParcel> InterfaceController::getCfg(const std::string& ifName) {
    struct in_addr addr = {};
    int prefixLength = 0;
    unsigned char hwaddr[ETH_ALEN] = {};
    unsigned flags = 0;
    InterfaceConfigurationParcel cfgResult;

    const auto& sys = sSyscalls.get();
    ASSIGN_OR_RETURN(auto fd, sys.socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0));

    struct ifreq ifr = {};
    strlcpy(ifr.ifr_name, ifName.c_str(), IFNAMSIZ);

    if (isOk(sys.ioctl(fd, SIOCGIFADDR, &ifr))) {
        addr.s_addr = ((struct sockaddr_in*) &ifr.ifr_addr)->sin_addr.s_addr;
    }

    if (isOk(sys.ioctl(fd, SIOCGIFNETMASK, &ifr))) {
        prefixLength =
                ipv4NetmaskToPrefixLength(((struct sockaddr_in*) &ifr.ifr_addr)->sin_addr.s_addr);
    }

    if (isOk(sys.ioctl(fd, SIOCGIFFLAGS, &ifr))) {
        flags = ifr.ifr_flags;
    }

    // ETH_ALEN is for ARPHRD_ETHER, it is better to check the sa_family.
    // However, we keep old design for the consistency.
    if (isOk(sys.ioctl(fd, SIOCGIFHWADDR, &ifr))) {
        memcpy((void*) hwaddr, &ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    } else {
        ALOGW("Failed to retrieve HW addr for %s (%s)", ifName.c_str(), strerror(errno));
    }

    cfgResult.ifName = ifName;
    cfgResult.hwAddr = hwAddrToStr(hwaddr);
    cfgResult.ipv4Addr = std::string(inet_ntoa(addr));
    cfgResult.prefixLength = prefixLength;
    cfgResult.flags.push_back(flags & IFF_UP ? toStdString(INetd::IF_STATE_UP())
                                             : toStdString(INetd::IF_STATE_DOWN()));

    if (flags & IFF_BROADCAST) cfgResult.flags.push_back(toStdString(INetd::IF_FLAG_BROADCAST()));
    if (flags & IFF_LOOPBACK) cfgResult.flags.push_back(toStdString(INetd::IF_FLAG_LOOPBACK()));
    if (flags & IFF_POINTOPOINT)
        cfgResult.flags.push_back(toStdString(INetd::IF_FLAG_POINTOPOINT()));
    if (flags & IFF_RUNNING) cfgResult.flags.push_back(toStdString(INetd::IF_FLAG_RUNNING()));
    if (flags & IFF_MULTICAST) cfgResult.flags.push_back(toStdString(INetd::IF_FLAG_MULTICAST()));

    return cfgResult;
}

int InterfaceController::clearAddrs(const std::string& ifName) {
    return ifc_clear_addresses(ifName.c_str());
}

}  // namespace net
}  // namespace android
