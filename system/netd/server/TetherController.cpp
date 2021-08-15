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

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <spawn.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <array>
#include <cstdlib>
#include <regex>
#include <string>
#include <vector>

#define LOG_TAG "TetherController"
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <cutils/properties.h>
#include <log/log.h>
#include <netdutils/StatusOr.h>

#include "Controllers.h"
#include "Fwmark.h"
#include "InterfaceController.h"
#include "NetdConstants.h"
#include "NetworkController.h"
#include "Permission.h"
#include "TetherController.h"

namespace android {
namespace net {

using android::base::Join;
using android::base::Pipe;
using android::base::StringPrintf;
using android::base::unique_fd;
using android::netdutils::statusFromErrno;
using android::netdutils::StatusOr;

namespace {

const char BP_TOOLS_MODE[] = "bp-tools";
const char IPV4_FORWARDING_PROC_FILE[] = "/proc/sys/net/ipv4/ip_forward";
const char IPV6_FORWARDING_PROC_FILE[] = "/proc/sys/net/ipv6/conf/all/forwarding";
const char SEPARATOR[] = "|";
constexpr const char kTcpBeLiberal[] = "/proc/sys/net/netfilter/nf_conntrack_tcp_be_liberal";

// Chosen to match AID_DNS_TETHER, as made "friendly" by fs_config_generator.py.
constexpr const char kDnsmasqUsername[] = "dns_tether";

bool writeToFile(const char* filename, const char* value) {
    int fd = open(filename, O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        ALOGE("Failed to open %s: %s", filename, strerror(errno));
        return false;
    }

    const ssize_t len = strlen(value);
    if (write(fd, value, len) != len) {
        ALOGE("Failed to write %s to %s: %s", value, filename, strerror(errno));
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

// TODO: Consider altering TCP and UDP timeouts as well.
void configureForTethering(bool enabled) {
    writeToFile(kTcpBeLiberal, enabled ? "1" : "0");
}

bool configureForIPv6Router(const char *interface) {
    return (InterfaceController::setEnableIPv6(interface, 0) == 0)
            && (InterfaceController::setAcceptIPv6Ra(interface, 0) == 0)
            && (InterfaceController::setAcceptIPv6Dad(interface, 0) == 0)
            && (InterfaceController::setIPv6DadTransmits(interface, "0") == 0)
            && (InterfaceController::setEnableIPv6(interface, 1) == 0);
}

void configureForIPv6Client(const char *interface) {
    InterfaceController::setAcceptIPv6Ra(interface, 1);
    InterfaceController::setAcceptIPv6Dad(interface, 1);
    InterfaceController::setIPv6DadTransmits(interface, "1");
    InterfaceController::setEnableIPv6(interface, 0);
}

bool inBpToolsMode() {
    // In BP tools mode, do not disable IP forwarding
    char bootmode[PROPERTY_VALUE_MAX] = {0};
    property_get("ro.bootmode", bootmode, "unknown");
    return !strcmp(BP_TOOLS_MODE, bootmode);
}

int setPosixSpawnFileActionsAddDup2(posix_spawn_file_actions_t* fa, int fd, int new_fd) {
    int res = posix_spawn_file_actions_init(fa);
    if (res) {
        return res;
    }
    return posix_spawn_file_actions_adddup2(fa, fd, new_fd);
}

int setPosixSpawnAttrFlags(posix_spawnattr_t* attr, short flags) {
    int res = posix_spawnattr_init(attr);
    if (res) {
        return res;
    }
    return posix_spawnattr_setflags(attr, flags);
}

}  // namespace

auto TetherController::iptablesRestoreFunction = execIptablesRestoreWithOutput;

const std::string GET_TETHER_STATS_COMMAND = StringPrintf(
    "*filter\n"
    "-nvx -L %s\n"
    "COMMIT\n", android::net::TetherController::LOCAL_TETHER_COUNTERS_CHAIN);

int TetherController::DnsmasqState::sendCmd(int daemonFd, const std::string& cmd) {
    if (cmd.empty()) return 0;

    gLog.log("Sending update msg to dnsmasq [%s]", cmd.c_str());
    // Send the trailing \0 as well.
    if (write(daemonFd, cmd.c_str(), cmd.size() + 1) < 0) {
        gLog.error("Failed to send update command to dnsmasq (%s)", strerror(errno));
        errno = EREMOTEIO;
        return -1;
    }
    return 0;
}

void TetherController::DnsmasqState::clear() {
    update_ifaces_cmd.clear();
    update_dns_cmd.clear();
}

int TetherController::DnsmasqState::sendAllState(int daemonFd) const {
    return sendCmd(daemonFd, update_ifaces_cmd) | sendCmd(daemonFd, update_dns_cmd);
}

TetherController::TetherController() {
    if (inBpToolsMode()) {
        enableForwarding(BP_TOOLS_MODE);
    } else {
        setIpFwdEnabled();
    }
}

bool TetherController::setIpFwdEnabled() {
    bool success = true;
    bool disable = mForwardingRequests.empty();
    const char* value = disable ? "0" : "1";
    ALOGD("Setting IP forward enable = %s", value);
    success &= writeToFile(IPV4_FORWARDING_PROC_FILE, value);
    success &= writeToFile(IPV6_FORWARDING_PROC_FILE, value);
    if (disable) {
        // Turning off the forwarding sysconf in the kernel has the side effect
        // of turning on ICMP redirect, which is a security hazard.
        // Turn ICMP redirect back off immediately.
        int rv = InterfaceController::disableIcmpRedirects();
        success &= (rv == 0);
    }
    return success;
}

bool TetherController::enableForwarding(const char* requester) {
    // Don't return an error if this requester already requested forwarding. Only return errors for
    // things that the caller caller needs to care about, such as "couldn't write to the file to
    // enable forwarding".
    mForwardingRequests.insert(requester);
    return setIpFwdEnabled();
}

bool TetherController::disableForwarding(const char* requester) {
    mForwardingRequests.erase(requester);
    return setIpFwdEnabled();
}

const std::set<std::string>& TetherController::getIpfwdRequesterList() const {
    return mForwardingRequests;
}

int TetherController::startTethering(int num_addrs, char **dhcp_ranges) {
    if (mDaemonPid != 0) {
        ALOGE("Tethering already started");
        errno = EBUSY;
        return -errno;
    }

    ALOGD("Starting tethering services");

    unique_fd pipeRead, pipeWrite;
    if (!Pipe(&pipeRead, &pipeWrite, O_CLOEXEC)) {
        int res = errno;
        ALOGE("pipe2() failed (%s)", strerror(errno));
        return -res;
    }

    // Set parameters
    Fwmark fwmark;
    fwmark.netId = NetworkController::LOCAL_NET_ID;
    fwmark.explicitlySelected = true;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;
    char markStr[UINT32_HEX_STRLEN];
    snprintf(markStr, sizeof(markStr), "0x%x", fwmark.intValue);

    std::vector<const std::string> argVector = {
            "/system/bin/dnsmasq",
            "--keep-in-foreground",
            "--no-resolv",
            "--no-poll",
            "--dhcp-authoritative",
            // TODO: pipe through metered status from ConnService
            "--dhcp-option-force=43,ANDROID_METERED",
            "--pid-file",
            "--listen-mark",
            markStr,
            "--user",
            kDnsmasqUsername,
    };

    // DHCP server will be disabled if num_addrs == 0 and no --dhcp-range is
    // passed.
    for (int addrIndex = 0; addrIndex < num_addrs; addrIndex += 2) {
        argVector.push_back(StringPrintf("--dhcp-range=%s,%s,1h", dhcp_ranges[addrIndex],
                                         dhcp_ranges[addrIndex + 1]));
    }

    std::vector<char*> args(argVector.size() + 1);
    for (unsigned i = 0; i < argVector.size(); i++) {
        args[i] = (char*)argVector[i].c_str();
    }

    /*
     * TODO: Create a monitoring thread to handle and restart
     * the daemon if it exits prematurely
     */

    // Note that don't modify any memory between vfork and execv.
    // Changing state of file descriptors would be fine. See posix_spawn_file_actions_add*
    // dup2 creates fd without CLOEXEC, dnsmasq will receive commands through the
    // duplicated fd.
    posix_spawn_file_actions_t fa;
    int res = setPosixSpawnFileActionsAddDup2(&fa, pipeRead.get(), STDIN_FILENO);
    if (res) {
        ALOGE("posix_spawn set fa failed (%s)", strerror(res));
        return -res;
    }

    posix_spawnattr_t attr;
    res = setPosixSpawnAttrFlags(&attr, POSIX_SPAWN_USEVFORK);
    if (res) {
        ALOGE("posix_spawn set attr flag failed (%s)", strerror(res));
        return -res;
    }

    pid_t pid;
    res = posix_spawn(&pid, args[0], &fa, &attr, &args[0], nullptr);
    posix_spawnattr_destroy(&attr);
    posix_spawn_file_actions_destroy(&fa);
    if (res) {
        ALOGE("posix_spawn failed (%s)", strerror(res));
        return -res;
    }
    mDaemonPid = pid;
    mDaemonFd = pipeWrite.release();
    configureForTethering(true);
    applyDnsInterfaces();
    ALOGD("Tethering services running");

    return 0;
}

std::vector<char*> TetherController::toCstrVec(const std::vector<std::string>& addrs) {
    std::vector<char*> addrsCstrVec{};
    addrsCstrVec.reserve(addrs.size());
    for (const auto& addr : addrs) {
        addrsCstrVec.push_back(const_cast<char*>(addr.data()));
    }
    return addrsCstrVec;
}

int TetherController::startTethering(const std::vector<std::string>& dhcpRanges) {
    struct in_addr v4_addr;
    for (const auto& dhcpRange : dhcpRanges) {
        if (!inet_aton(dhcpRange.c_str(), &v4_addr)) {
            return -EINVAL;
        }
    }
    auto dhcp_ranges = toCstrVec(dhcpRanges);
    return startTethering(dhcp_ranges.size(), dhcp_ranges.data());
}

int TetherController::stopTethering() {
    configureForTethering(false);

    if (mDaemonPid == 0) {
        ALOGE("Tethering already stopped");
        return 0;
    }

    ALOGD("Stopping tethering services");

    kill(mDaemonPid, SIGTERM);
    waitpid(mDaemonPid, nullptr, 0);
    mDaemonPid = 0;
    close(mDaemonFd);
    mDaemonFd = -1;
    mDnsmasqState.clear();
    ALOGD("Tethering services stopped");
    return 0;
}

bool TetherController::isTetheringStarted() {
    return (mDaemonPid == 0 ? false : true);
}

// dnsmasq can't parse commands larger than this due to the fixed-size buffer
// in check_android_listeners(). The receiving buffer is 1024 bytes long, but
// dnsmasq reads up to 1023 bytes.
const size_t MAX_CMD_SIZE = 1023;

// TODO: Remove overload function and update this after NDC migration.
int TetherController::setDnsForwarders(unsigned netId, char **servers, int numServers) {
    Fwmark fwmark;
    fwmark.netId = netId;
    fwmark.explicitlySelected = true;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;

    std::string daemonCmd = StringPrintf("update_dns%s0x%x", SEPARATOR, fwmark.intValue);

    mDnsForwarders.clear();
    for (int i = 0; i < numServers; i++) {
        ALOGD("setDnsForwarders(0x%x %d = '%s')", fwmark.intValue, i, servers[i]);

        addrinfo *res, hints = { .ai_flags = AI_NUMERICHOST };
        int ret = getaddrinfo(servers[i], nullptr, &hints, &res);
        freeaddrinfo(res);
        if (ret) {
            ALOGE("Failed to parse DNS server '%s'", servers[i]);
            mDnsForwarders.clear();
            errno = EINVAL;
            return -errno;
        }

        if (daemonCmd.size() + 1 + strlen(servers[i]) >= MAX_CMD_SIZE) {
            ALOGE("Too many DNS servers listed");
            break;
        }

        daemonCmd += SEPARATOR;
        daemonCmd += servers[i];
        mDnsForwarders.push_back(servers[i]);
    }

    mDnsNetId = netId;
    mDnsmasqState.update_dns_cmd = std::move(daemonCmd);
    if (mDaemonFd != -1) {
        if (mDnsmasqState.sendAllState(mDaemonFd) != 0) {
            mDnsForwarders.clear();
            errno = EREMOTEIO;
            return -errno;
        }
    }
    return 0;
}

int TetherController::setDnsForwarders(unsigned netId, const std::vector<std::string>& servers) {
    auto dnsServers = toCstrVec(servers);
    return setDnsForwarders(netId, dnsServers.data(), dnsServers.size());
}

unsigned TetherController::getDnsNetId() {
    return mDnsNetId;
}

const std::list<std::string> &TetherController::getDnsForwarders() const {
    return mDnsForwarders;
}

bool TetherController::applyDnsInterfaces() {
    std::string daemonCmd = "update_ifaces";
    bool haveInterfaces = false;

    for (const auto& ifname : mInterfaces) {
        if (daemonCmd.size() + 1 + ifname.size() >= MAX_CMD_SIZE) {
            ALOGE("Too many DNS servers listed");
            break;
        }

        daemonCmd += SEPARATOR;
        daemonCmd += ifname;
        haveInterfaces = true;
    }

    if (!haveInterfaces) {
        mDnsmasqState.update_ifaces_cmd.clear();
    } else {
        mDnsmasqState.update_ifaces_cmd = std::move(daemonCmd);
        if (mDaemonFd != -1) return (mDnsmasqState.sendAllState(mDaemonFd) == 0);
    }
    return true;
}

int TetherController::tetherInterface(const char *interface) {
    ALOGD("tetherInterface(%s)", interface);
    if (!isIfaceName(interface)) {
        errno = ENOENT;
        return -errno;
    }

    if (!configureForIPv6Router(interface)) {
        configureForIPv6Client(interface);
        return -EREMOTEIO;
    }
    mInterfaces.push_back(interface);

    if (!applyDnsInterfaces()) {
        mInterfaces.pop_back();
        configureForIPv6Client(interface);
        return -EREMOTEIO;
    } else {
        return 0;
    }
}

int TetherController::untetherInterface(const char *interface) {
    ALOGD("untetherInterface(%s)", interface);

    for (auto it = mInterfaces.cbegin(); it != mInterfaces.cend(); ++it) {
        if (!strcmp(interface, it->c_str())) {
            mInterfaces.erase(it);

            configureForIPv6Client(interface);
            return applyDnsInterfaces() ? 0 : -EREMOTEIO;
        }
    }
    errno = ENOENT;
    return -errno;
}

const std::list<std::string> &TetherController::getTetheredInterfaceList() const {
    return mInterfaces;
}

int TetherController::setupIptablesHooks() {
    int res;
    res = setDefaults();
    if (res < 0) {
        return res;
    }

    // Used to limit downstream mss to the upstream pmtu so we don't end up fragmenting every large
    // packet tethered devices send. This is IPv4-only, because in IPv6 we send the MTU in the RA.
    // This is no longer optional and tethering will fail to start if it fails.
    std::string mssRewriteCommand = StringPrintf(
        "*mangle\n"
        "-A %s -p tcp --tcp-flags SYN SYN -j TCPMSS --clamp-mss-to-pmtu\n"
        "COMMIT\n", LOCAL_MANGLE_FORWARD);

    // This is for tethering counters. This chain is reached via --goto, and then RETURNS.
    std::string defaultCommands = StringPrintf(
        "*filter\n"
        ":%s -\n"
        "COMMIT\n", LOCAL_TETHER_COUNTERS_CHAIN);

    res = iptablesRestoreFunction(V4, mssRewriteCommand, nullptr);
    if (res < 0) {
        return res;
    }

    res = iptablesRestoreFunction(V4V6, defaultCommands, nullptr);
    if (res < 0) {
        return res;
    }

    mFwdIfaces.clear();

    return 0;
}

int TetherController::setDefaults() {
    std::string v4Cmd = StringPrintf(
        "*filter\n"
        ":%s -\n"
        "-A %s -j DROP\n"
        "COMMIT\n"
        "*nat\n"
        ":%s -\n"
        "COMMIT\n", LOCAL_FORWARD, LOCAL_FORWARD, LOCAL_NAT_POSTROUTING);

    std::string v6Cmd = StringPrintf(
            "*filter\n"
            ":%s -\n"
            "COMMIT\n"
            "*raw\n"
            ":%s -\n"
            "COMMIT\n",
            LOCAL_FORWARD, LOCAL_RAW_PREROUTING);

    int res = iptablesRestoreFunction(V4, v4Cmd, nullptr);
    if (res < 0) {
        return res;
    }

    res = iptablesRestoreFunction(V6, v6Cmd, nullptr);
    if (res < 0) {
        return res;
    }

    return 0;
}

int TetherController::enableNat(const char* intIface, const char* extIface) {
    ALOGV("enableNat(intIface=<%s>, extIface=<%s>)",intIface, extIface);

    if (!isIfaceName(intIface) || !isIfaceName(extIface)) {
        return -ENODEV;
    }

    /* Bug: b/9565268. "enableNat wlan0 wlan0". For now we fail until java-land is fixed */
    if (!strcmp(intIface, extIface)) {
        ALOGE("Duplicate interface specified: %s %s", intIface, extIface);
        return -EINVAL;
    }

    if (isForwardingPairEnabled(intIface, extIface)) {
        return 0;
    }

    // add this if we are the first enabled nat for this upstream
    if (!isAnyForwardingEnabledOnUpstream(extIface)) {
        std::vector<std::string> v4Cmds = {
            "*nat",
            StringPrintf("-A %s -o %s -j MASQUERADE", LOCAL_NAT_POSTROUTING, extIface),
            "COMMIT\n"
        };

        if (iptablesRestoreFunction(V4, Join(v4Cmds, '\n'), nullptr) || setupIPv6CountersChain() ||
            setTetherGlobalAlertRule()) {
            ALOGE("Error setting postroute rule: iface=%s", extIface);
            if (!isAnyForwardingPairEnabled()) {
                // unwind what's been done, but don't care about success - what more could we do?
                setDefaults();
            }
            return -EREMOTEIO;
        }
    }

    if (setForwardRules(true, intIface, extIface) != 0) {
        ALOGE("Error setting forward rules");
        if (!isAnyForwardingPairEnabled()) {
            setDefaults();
        }
        return -ENODEV;
    }

    return 0;
}

int TetherController::setTetherGlobalAlertRule() {
    // Only add this if we are the first enabled nat
    if (isAnyForwardingPairEnabled()) {
        return 0;
    }
    const std::string cmds =
            "*filter\n" +
            StringPrintf("-I %s -j %s\n", LOCAL_FORWARD, BandwidthController::LOCAL_GLOBAL_ALERT) +
            "COMMIT\n";

    return iptablesRestoreFunction(V4V6, cmds, nullptr);
}

int TetherController::setupIPv6CountersChain() {
    // Only add this if we are the first enabled nat
    if (isAnyForwardingPairEnabled()) {
        return 0;
    }

    /*
     * IPv6 tethering doesn't need the state-based conntrack rules, so
     * it unconditionally jumps to the tether counters chain all the time.
     */
    const std::string v6Cmds =
            "*filter\n" +
            StringPrintf("-A %s -g %s\n", LOCAL_FORWARD, LOCAL_TETHER_COUNTERS_CHAIN) + "COMMIT\n";

    return iptablesRestoreFunction(V6, v6Cmds, nullptr);
}

// Gets a pointer to the ForwardingDownstream for an interface pair in the map, or nullptr
TetherController::ForwardingDownstream* TetherController::findForwardingDownstream(
        const std::string& intIface, const std::string& extIface) {
    auto extIfaceMatches = mFwdIfaces.equal_range(extIface);
    for (auto it = extIfaceMatches.first; it != extIfaceMatches.second; ++it) {
        if (it->second.iface == intIface) {
            return &(it->second);
        }
    }
    return nullptr;
}

void TetherController::addForwardingPair(const std::string& intIface, const std::string& extIface) {
    ForwardingDownstream* existingEntry = findForwardingDownstream(intIface, extIface);
    if (existingEntry != nullptr) {
        existingEntry->active = true;
        return;
    }

    mFwdIfaces.insert(std::pair<std::string, ForwardingDownstream>(extIface, {
        .iface = intIface,
        .active = true
    }));
}

void TetherController::markForwardingPairDisabled(
        const std::string& intIface, const std::string& extIface) {
    ForwardingDownstream* existingEntry = findForwardingDownstream(intIface, extIface);
    if (existingEntry == nullptr) {
        return;
    }

    existingEntry->active = false;
}

bool TetherController::isForwardingPairEnabled(
        const std::string& intIface, const std::string& extIface) {
    ForwardingDownstream* existingEntry = findForwardingDownstream(intIface, extIface);
    return existingEntry != nullptr && existingEntry->active;
}

bool TetherController::isAnyForwardingEnabledOnUpstream(const std::string& extIface) {
    auto extIfaceMatches = mFwdIfaces.equal_range(extIface);
    for (auto it = extIfaceMatches.first; it != extIfaceMatches.second; ++it) {
        if (it->second.active) {
            return true;
        }
    }
    return false;
}

bool TetherController::isAnyForwardingPairEnabled() {
    for (auto& it : mFwdIfaces) {
        if (it.second.active) {
            return true;
        }
    }
    return false;
}

bool TetherController::tetherCountingRuleExists(
        const std::string& iface1, const std::string& iface2) {
    // A counting rule exists if NAT was ever enabled for this interface pair, so if the pair
    // is in the map regardless of its active status. Rules are added both ways so we check with
    // the 2 combinations.
    return findForwardingDownstream(iface1, iface2) != nullptr
        || findForwardingDownstream(iface2, iface1) != nullptr;
}

/* static */
std::string TetherController::makeTetherCountingRule(const char *if1, const char *if2) {
    return StringPrintf("-A %s -i %s -o %s -j RETURN", LOCAL_TETHER_COUNTERS_CHAIN, if1, if2);
}

int TetherController::setForwardRules(bool add, const char *intIface, const char *extIface) {
    const char *op = add ? "-A" : "-D";

    std::string rpfilterCmd = StringPrintf(
        "*raw\n"
        "%s %s -i %s -m rpfilter --invert ! -s fe80::/64 -j DROP\n"
        "COMMIT\n", op, LOCAL_RAW_PREROUTING, intIface);
    if (iptablesRestoreFunction(V6, rpfilterCmd, nullptr) == -1 && add) {
        return -EREMOTEIO;
    }

    std::vector<std::string> v4 = {
            "*raw",
            StringPrintf("%s %s -p tcp --dport 21 -i %s -j CT --helper ftp", op,
                         LOCAL_RAW_PREROUTING, intIface),
            StringPrintf("%s %s -p tcp --dport 1723 -i %s -j CT --helper pptp", op,
                         LOCAL_RAW_PREROUTING, intIface),
            "COMMIT",
            "*filter",
            StringPrintf("%s %s -i %s -o %s -m state --state ESTABLISHED,RELATED -g %s", op,
                         LOCAL_FORWARD, extIface, intIface, LOCAL_TETHER_COUNTERS_CHAIN),
            StringPrintf("%s %s -i %s -o %s -m state --state INVALID -j DROP", op, LOCAL_FORWARD,
                         intIface, extIface),
            StringPrintf("%s %s -i %s -o %s -g %s", op, LOCAL_FORWARD, intIface, extIface,
                         LOCAL_TETHER_COUNTERS_CHAIN),
    };

    std::vector<std::string> v6 = {
        "*filter",
    };

    // We only ever add tethering quota rules so that they stick.
    if (add && !tetherCountingRuleExists(intIface, extIface)) {
        v4.push_back(makeTetherCountingRule(intIface, extIface));
        v4.push_back(makeTetherCountingRule(extIface, intIface));
        v6.push_back(makeTetherCountingRule(intIface, extIface));
        v6.push_back(makeTetherCountingRule(extIface, intIface));
    }

    // Always make sure the drop rule is at the end.
    // TODO: instead of doing this, consider just rebuilding LOCAL_FORWARD completely from scratch
    // every time, starting with ":tetherctrl_FORWARD -\n". This would likely be a bit simpler.
    if (add) {
        v4.push_back(StringPrintf("-D %s -j DROP", LOCAL_FORWARD));
        v4.push_back(StringPrintf("-A %s -j DROP", LOCAL_FORWARD));
    }

    v4.push_back("COMMIT\n");
    v6.push_back("COMMIT\n");

    // We only add IPv6 rules here, never remove them.
    if (iptablesRestoreFunction(V4, Join(v4, '\n'), nullptr) == -1 ||
        (add && iptablesRestoreFunction(V6, Join(v6, '\n'), nullptr) == -1)) {
        // unwind what's been done, but don't care about success - what more could we do?
        if (add) {
            setForwardRules(false, intIface, extIface);
        }
        return -EREMOTEIO;
    }

    if (add) {
        addForwardingPair(intIface, extIface);
    } else {
        markForwardingPairDisabled(intIface, extIface);
    }

    return 0;
}

int TetherController::disableNat(const char* intIface, const char* extIface) {
    if (!isIfaceName(intIface) || !isIfaceName(extIface)) {
        errno = ENODEV;
        return -errno;
    }

    setForwardRules(false, intIface, extIface);
    if (!isAnyForwardingPairEnabled()) {
        setDefaults();
    }
    return 0;
}

void TetherController::addStats(TetherStatsList& statsList, const TetherStats& stats) {
    for (TetherStats& existing : statsList) {
        if (existing.addStatsIfMatch(stats)) {
            return;
        }
    }
    // No match. Insert a new interface pair.
    statsList.push_back(stats);
}

/*
 * Parse the ptks and bytes out of:
 *   Chain tetherctrl_counters (4 references)
 *       pkts      bytes target     prot opt in     out     source               destination
 *         26     2373 RETURN     all  --  wlan0  rmnet0  0.0.0.0/0            0.0.0.0/0
 *         27     2002 RETURN     all  --  rmnet0 wlan0   0.0.0.0/0            0.0.0.0/0
 *       1040   107471 RETURN     all  --  bt-pan rmnet0  0.0.0.0/0            0.0.0.0/0
 *       1450  1708806 RETURN     all  --  rmnet0 bt-pan  0.0.0.0/0            0.0.0.0/0
 * or:
 *   Chain tetherctrl_counters (0 references)
 *       pkts      bytes target     prot opt in     out     source               destination
 *          0        0 RETURN     all      wlan0  rmnet_data0  ::/0                 ::/0
 *          0        0 RETURN     all      rmnet_data0 wlan0   ::/0                 ::/0
 *
 */
int TetherController::addForwardChainStats(TetherStatsList& statsList,
                                           const std::string& statsOutput,
                                           std::string &extraProcessingInfo) {
    enum IndexOfIptChain {
        ORIG_LINE,
        PACKET_COUNTS,
        BYTE_COUNTS,
        HYPHEN,
        IFACE0_NAME,
        IFACE1_NAME,
        SOURCE,
        DESTINATION
    };
    TetherStats stats;
    const TetherStats empty;

    static const std::string NUM = "(\\d+)";
    static const std::string IFACE = "([^\\s]+)";
    static const std::string DST = "(0.0.0.0/0|::/0)";
    static const std::string COUNTERS = "\\s*" + NUM + "\\s+" + NUM +
                                        " RETURN     all(  --  |      )" + IFACE + "\\s+" + IFACE +
                                        "\\s+" + DST + "\\s+" + DST;
    static const std::regex IP_RE(COUNTERS);

    const std::vector<std::string> lines = base::Split(statsOutput, "\n");
    int headerLine = 0;
    for (const std::string& line : lines) {
        // Skip headers.
        if (headerLine < 2) {
            if (line.empty()) {
                ALOGV("Empty header while parsing tethering stats");
                return -EREMOTEIO;
            }
            headerLine++;
            continue;
        }

        if (line.empty()) continue;

        extraProcessingInfo = line;
        std::smatch matches;
        if (!std::regex_search(line, matches, IP_RE)) return -EREMOTEIO;
        // Here use IP_RE to distiguish IPv4 and IPv6 iptables.
        // IPv4 has "--" indicating what to do with fragments...
        //		 26 	2373 RETURN     all  --  wlan0	rmnet0	0.0.0.0/0			 0.0.0.0/0
        // ... but IPv6 does not.
        //		 26 	2373 RETURN 	all      wlan0	rmnet0	::/0				 ::/0
        // TODO: Replace strtoXX() calls with ParseUint() /ParseInt()
        int64_t packets = strtoul(matches[PACKET_COUNTS].str().c_str(), nullptr, 10);
        int64_t bytes = strtoul(matches[BYTE_COUNTS].str().c_str(), nullptr, 10);
        std::string iface0 = matches[IFACE0_NAME].str();
        std::string iface1 = matches[IFACE1_NAME].str();
        std::string rest = matches[SOURCE].str();

        ALOGV("parse iface0=<%s> iface1=<%s> pkts=%" PRId64 " bytes=%" PRId64
              " rest=<%s> orig line=<%s>",
              iface0.c_str(), iface1.c_str(), packets, bytes, rest.c_str(), line.c_str());
        /*
         * The following assumes that the 1st rule has in:extIface out:intIface,
         * which is what TetherController sets up.
         * The 1st matches rx, and sets up the pair for the tx side.
         */
        if (!stats.intIface[0]) {
            ALOGV("0Filter RX iface_in=%s iface_out=%s rx_bytes=%" PRId64 " rx_packets=%" PRId64
                  " ", iface0.c_str(), iface1.c_str(), bytes, packets);
            stats.intIface = iface0;
            stats.extIface = iface1;
            stats.txPackets = packets;
            stats.txBytes = bytes;
        } else if (stats.intIface == iface1 && stats.extIface == iface0) {
            ALOGV("0Filter TX iface_in=%s iface_out=%s rx_bytes=%" PRId64 " rx_packets=%" PRId64
                  " ", iface0.c_str(), iface1.c_str(), bytes, packets);
            stats.rxPackets = packets;
            stats.rxBytes = bytes;
        }
        if (stats.rxBytes != -1 && stats.txBytes != -1) {
            ALOGV("rx_bytes=%" PRId64" tx_bytes=%" PRId64, stats.rxBytes, stats.txBytes);
            addStats(statsList, stats);
            stats = empty;
        }
    }

    /* It is always an error to find only one side of the stats. */
    if (((stats.rxBytes == -1) != (stats.txBytes == -1))) {
        return -EREMOTEIO;
    }
    return 0;
}

StatusOr<TetherController::TetherStatsList> TetherController::getTetherStats() {
    TetherStatsList statsList;
    std::string parsedIptablesOutput;

    for (const IptablesTarget target : {V4, V6}) {
        std::string statsString;
        if (int ret = iptablesRestoreFunction(target, GET_TETHER_STATS_COMMAND, &statsString)) {
            return statusFromErrno(-ret, StringPrintf("failed to fetch tether stats (%d): %d",
                                                      target, ret));
        }

        if (int ret = addForwardChainStats(statsList, statsString, parsedIptablesOutput)) {
            return statusFromErrno(-ret, StringPrintf("failed to parse %s tether stats:\n%s",
                                                      target == V4 ? "IPv4": "IPv6",
                                                      parsedIptablesOutput.c_str()));
        }
    }

    return statsList;
}

}  // namespace net
}  // namespace android
