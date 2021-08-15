/*
 * Copyright (C) 2011 The Android Open Source Project
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

// #define LOG_NDEBUG 0

/*
 * The CommandListener, FrameworkListener don't allow for
 * multiple calls in parallel to reach the BandwidthController.
 * If they ever were to allow it, then netd/ would need some tweaking.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/pkt_sched.h>

#include "android-base/stringprintf.h"
#include "android-base/strings.h"
#define LOG_TAG "BandwidthController"
#include <cutils/properties.h>
#include <log/log.h>
#include <logwrap/logwrap.h>

#include <netdutils/Syscalls.h>
#include "BandwidthController.h"
#include "Controllers.h"
#include "FirewallController.h" /* For makeCriticalCommands */
#include "Fwmark.h"
#include "NetdConstants.h"
#include "TrafficController.h"
#include "bpf/BpfUtils.h"

/* Alphabetical */
#define ALERT_IPT_TEMPLATE "%s %s -m quota2 ! --quota %" PRId64" --name %s\n"
const char BandwidthController::LOCAL_INPUT[] = "bw_INPUT";
const char BandwidthController::LOCAL_FORWARD[] = "bw_FORWARD";
const char BandwidthController::LOCAL_OUTPUT[] = "bw_OUTPUT";
const char BandwidthController::LOCAL_RAW_PREROUTING[] = "bw_raw_PREROUTING";
const char BandwidthController::LOCAL_MANGLE_POSTROUTING[] = "bw_mangle_POSTROUTING";
const char BandwidthController::LOCAL_GLOBAL_ALERT[] = "bw_global_alert";

auto BandwidthController::iptablesRestoreFunction = execIptablesRestoreWithOutput;

using android::base::Join;
using android::base::StringAppendF;
using android::base::StringPrintf;
using android::net::FirewallController;
using android::net::gCtls;
using android::netdutils::Status;
using android::netdutils::StatusOr;
using android::netdutils::UniqueFile;

namespace {

const char ALERT_GLOBAL_NAME[] = "globalAlert";
const std::string NEW_CHAIN_COMMAND = "-N ";

const char NAUGHTY_CHAIN[] = "bw_penalty_box";
const char NICE_CHAIN[] = "bw_happy_box";

/**
 * Some comments about the rules:
 *  * Ordering
 *    - when an interface is marked as costly it should be INSERTED into the INPUT/OUTPUT chains.
 *      E.g. "-I bw_INPUT -i rmnet0 --jump costly"
 *    - quota'd rules in the costly chain should be before bw_penalty_box lookups.
 *    - the qtaguid counting is done at the end of the bw_INPUT/bw_OUTPUT user chains.
 *
 * * global quota vs per interface quota
 *   - global quota for all costly interfaces uses a single costly chain:
 *    . initial rules
 *      iptables -N bw_costly_shared
 *      iptables -I bw_INPUT -i iface0 --jump bw_costly_shared
 *      iptables -I bw_OUTPUT -o iface0 --jump bw_costly_shared
 *      iptables -I bw_costly_shared -m quota \! --quota 500000 \
 *          --jump REJECT --reject-with icmp-net-prohibited
 *      iptables -A bw_costly_shared --jump bw_penalty_box
 *      iptables -A bw_penalty_box --jump bw_happy_box
 *      iptables -A bw_happy_box --jump bw_data_saver
 *
 *    . adding a new iface to this, E.g.:
 *      iptables -I bw_INPUT -i iface1 --jump bw_costly_shared
 *      iptables -I bw_OUTPUT -o iface1 --jump bw_costly_shared
 *
 *   - quota per interface. This is achieve by having "costly" chains per quota.
 *     E.g. adding a new costly interface iface0 with its own quota:
 *      iptables -N bw_costly_iface0
 *      iptables -I bw_INPUT -i iface0 --jump bw_costly_iface0
 *      iptables -I bw_OUTPUT -o iface0 --jump bw_costly_iface0
 *      iptables -A bw_costly_iface0 -m quota \! --quota 500000 \
 *          --jump REJECT --reject-with icmp-port-unreachable
 *      iptables -A bw_costly_iface0 --jump bw_penalty_box
 *
 * * Penalty box, happy box and data saver.
 *   - bw_penalty box is a blacklist of apps that are rejected.
 *   - bw_happy_box is a whitelist of apps. It always includes all system apps
 *   - bw_data_saver implements data usage restrictions.
 *   - Via the UI the user can add and remove apps from the whitelist and
 *     blacklist, and turn on/off data saver.
 *   - The blacklist takes precedence over the whitelist and the whitelist
 *     takes precedence over data saver.
 *
 * * bw_penalty_box handling:
 *  - only one bw_penalty_box for all interfaces
 *   E.g  Adding an app:
 *    iptables -I bw_penalty_box -m owner --uid-owner app_3 \
 *        --jump REJECT --reject-with icmp-port-unreachable
 *
 * * bw_happy_box handling:
 *  - The bw_happy_box comes after the penalty box.
 *   E.g  Adding a happy app,
 *    iptables -I bw_happy_box -m owner --uid-owner app_3 \
 *        --jump RETURN
 *
 * * bw_data_saver handling:
 *  - The bw_data_saver comes after the happy box.
 *    Enable data saver:
 *      iptables -R 1 bw_data_saver --jump REJECT --reject-with icmp-port-unreachable
 *    Disable data saver:
 *      iptables -R 1 bw_data_saver --jump RETURN
 */

const std::string COMMIT_AND_CLOSE = "COMMIT\n";
const std::string HAPPY_BOX_MATCH_WHITELIST_COMMAND =
        StringPrintf("-I bw_happy_box -m owner --uid-owner %d-%d --jump RETURN", 0, MAX_SYSTEM_UID);
const std::string BPF_HAPPY_BOX_MATCH_WHITELIST_COMMAND = StringPrintf(
        "-I bw_happy_box -m bpf --object-pinned %s -j RETURN", XT_BPF_WHITELIST_PROG_PATH);
const std::string BPF_PENALTY_BOX_MATCH_BLACKLIST_COMMAND = StringPrintf(
        "-I bw_penalty_box -m bpf --object-pinned %s -j REJECT", XT_BPF_BLACKLIST_PROG_PATH);

static const std::vector<std::string> IPT_FLUSH_COMMANDS = {
        /*
         * Cleanup rules.
         * Should normally include bw_costly_<iface>, but we rely on the way they are setup
         * to allow coexistance.
         */
        "*filter",
        ":bw_INPUT -",
        ":bw_OUTPUT -",
        ":bw_FORWARD -",
        ":bw_happy_box -",
        ":bw_penalty_box -",
        ":bw_data_saver -",
        ":bw_costly_shared -",
        ":bw_global_alert -",
        "COMMIT",
        "*raw",
        ":bw_raw_PREROUTING -",
        "COMMIT",
        "*mangle",
        ":bw_mangle_POSTROUTING -",
        COMMIT_AND_CLOSE};

static const uint32_t uidBillingMask = Fwmark::getUidBillingMask();

/**
 * Basic commands for creation of hooks into data accounting and data boxes.
 *
 * Included in these commands are rules to prevent the double-counting of IPsec
 * packets. The general overview is as follows:
 * > All interface counters (counted in PREROUTING, POSTROUTING) must be
 *     completely accurate, and count only the outer packet. As such, the inner
 *     packet must be ignored, which is done through the use of two rules: use
 *     of the policy module (for tunnel mode), and VTI interface checks (for
 *     tunnel or transport-in-tunnel mode). The VTI interfaces should be named
 *     ipsec*
 * > Outbound UID billing can always be done with the outer packets, due to the
 *     ability to always find the correct UID (based on the skb->sk). As such,
 *     the inner packets should be ignored based on the policy module, or the
 *     output interface if a VTI (ipsec+)
 * > Inbound UDP-encap-ESP packets can be correctly mapped to the UID that
 *     opened the encap socket, and as such, should be billed as early as
 *     possible (for transport mode; tunnel mode usage should be billed to
 *     sending/receiving application). Due to the inner packet being
 *     indistinguishable from the inner packet of ESP, a uidBillingDone mark
 *     has to be applied to prevent counting a second time.
 * > Inbound ESP has no socket, and as such must be accounted later. ESP
 *     protocol packets are skipped via a blanket rule.
 * > Note that this solution is asymmetrical. Adding the VTI or policy matcher
 *     ignore rule in the input chain would actually break the INPUT chain;
 *     Those rules are designed to ignore inner packets, and in the tunnel
 *     mode UDP, or any ESP case, we would not have billed the outer packet.
 *
 * See go/ipsec-data-accounting for more information.
 */

const std::vector<std::string> getBasicAccountingCommands(const bool useBpf) {
    const std::vector<std::string> ipt_basic_accounting_commands = {
            "*filter",

            "-A bw_INPUT -j bw_global_alert",
            // Prevents IPSec double counting (ESP and UDP-encap-ESP respectively)
            "-A bw_INPUT -p esp -j RETURN",
            StringPrintf("-A bw_INPUT -m mark --mark 0x%x/0x%x -j RETURN", uidBillingMask,
                         uidBillingMask),
            useBpf ? "" : "-A bw_INPUT -m owner --socket-exists",
            StringPrintf("-A bw_INPUT -j MARK --or-mark 0x%x", uidBillingMask),

            "-A bw_OUTPUT -j bw_global_alert",
            // Prevents IPSec double counting (Tunnel mode and Transport mode,
            // respectively)
            "-A bw_OUTPUT -o " IPSEC_IFACE_PREFIX "+ -j RETURN",
            "-A bw_OUTPUT -m policy --pol ipsec --dir out -j RETURN",
            useBpf ? "" : "-A bw_OUTPUT -m owner --socket-exists",

            "-A bw_costly_shared --jump bw_penalty_box",
            useBpf ? BPF_PENALTY_BOX_MATCH_BLACKLIST_COMMAND : "",
            "-A bw_penalty_box --jump bw_happy_box", "-A bw_happy_box --jump bw_data_saver",
            "-A bw_data_saver -j RETURN",
            useBpf ? BPF_HAPPY_BOX_MATCH_WHITELIST_COMMAND : HAPPY_BOX_MATCH_WHITELIST_COMMAND,
            "COMMIT",

            "*raw",
            // Prevents IPSec double counting (Tunnel mode and Transport mode,
            // respectively)
            "-A bw_raw_PREROUTING -i " IPSEC_IFACE_PREFIX "+ -j RETURN",
            "-A bw_raw_PREROUTING -m policy --pol ipsec --dir in -j RETURN",
            useBpf ? StringPrintf("-A bw_raw_PREROUTING -m bpf --object-pinned %s",
                                  XT_BPF_INGRESS_PROG_PATH)
                   : "-A bw_raw_PREROUTING -m owner --socket-exists",
            "COMMIT",

            "*mangle",
            // Prevents IPSec double counting (Tunnel mode and Transport mode,
            // respectively)
            "-A bw_mangle_POSTROUTING -o " IPSEC_IFACE_PREFIX "+ -j RETURN",
            "-A bw_mangle_POSTROUTING -m policy --pol ipsec --dir out -j RETURN",
            useBpf ? "" : "-A bw_mangle_POSTROUTING -m owner --socket-exists",
            StringPrintf("-A bw_mangle_POSTROUTING -j MARK --set-mark 0x0/0x%x",
                         uidBillingMask),  // Clear the mark before sending this packet
            useBpf ? StringPrintf("-A bw_mangle_POSTROUTING -m bpf --object-pinned %s",
                                  XT_BPF_EGRESS_PROG_PATH)
                   : "",
            COMMIT_AND_CLOSE};
    return ipt_basic_accounting_commands;
}


std::vector<std::string> toStrVec(int num, const char* const strs[]) {
    return std::vector<std::string>(strs, strs + num);
}

}  // namespace

void BandwidthController::setBpfEnabled(bool isEnabled) {
    mBpfSupported = isEnabled;
}

BandwidthController::BandwidthController() {
}

void BandwidthController::flushCleanTables(bool doClean) {
    /* Flush and remove the bw_costly_<iface> tables */
    flushExistingCostlyTables(doClean);

    std::string commands = Join(IPT_FLUSH_COMMANDS, '\n');
    iptablesRestoreFunction(V4V6, commands, nullptr);
}

int BandwidthController::setupIptablesHooks() {
    /* flush+clean is allowed to fail */
    flushCleanTables(true);
    return 0;
}

int BandwidthController::enableBandwidthControl() {
    /* Let's pretend we started from scratch ... */
    mSharedQuotaIfaces.clear();
    mQuotaIfaces.clear();
    mGlobalAlertBytes = 0;
    mSharedQuotaBytes = mSharedAlertBytes = 0;

    flushCleanTables(false);

    std::string commands = Join(getBasicAccountingCommands(mBpfSupported), '\n');
    return iptablesRestoreFunction(V4V6, commands, nullptr);
}

int BandwidthController::disableBandwidthControl() {

    flushCleanTables(false);
    return 0;
}

std::string BandwidthController::makeDataSaverCommand(IptablesTarget target, bool enable) {
    std::string cmd;
    const char *chainName = "bw_data_saver";
    const char *op = jumpToString(enable ? IptJumpReject : IptJumpReturn);
    std::string criticalCommands = enable ?
            FirewallController::makeCriticalCommands(target, chainName) : "";
    StringAppendF(&cmd,
        "*filter\n"
        ":%s -\n"
        "%s"
        "-A %s%s\n"
        "COMMIT\n", chainName, criticalCommands.c_str(), chainName, op);
    return cmd;
}

int BandwidthController::enableDataSaver(bool enable) {
    int ret = iptablesRestoreFunction(V4, makeDataSaverCommand(V4, enable), nullptr);
    ret |= iptablesRestoreFunction(V6, makeDataSaverCommand(V6, enable), nullptr);
    return ret;
}

// TODO: Remove after removing these commands in CommandListener
int BandwidthController::addNaughtyApps(int numUids, const char* const appUids[]) {
    return manipulateSpecialApps(toStrVec(numUids, appUids), NAUGHTY_CHAIN,
                                 IptJumpReject, IptOpInsert);
}

// TODO: Remove after removing these commands in CommandListener
int BandwidthController::removeNaughtyApps(int numUids, const char* const appUids[]) {
    return manipulateSpecialApps(toStrVec(numUids, appUids), NAUGHTY_CHAIN,
                                 IptJumpReject, IptOpDelete);
}

// TODO: Remove after removing these commands in CommandListener
int BandwidthController::addNiceApps(int numUids, const char* const appUids[]) {
    return manipulateSpecialApps(toStrVec(numUids, appUids), NICE_CHAIN,
                                 IptJumpReturn, IptOpInsert);
}

// TODO: Remove after removing these commands in CommandListener
int BandwidthController::removeNiceApps(int numUids, const char* const appUids[]) {
    return manipulateSpecialApps(toStrVec(numUids, appUids), NICE_CHAIN,
                                 IptJumpReturn, IptOpDelete);
}

int BandwidthController::addNaughtyApps(const std::vector<std::string>& appStrUid) {
    return manipulateSpecialApps(appStrUid, NAUGHTY_CHAIN, IptJumpReject, IptOpInsert);
}

int BandwidthController::removeNaughtyApps(const std::vector<std::string>& appStrUid) {
    return manipulateSpecialApps(appStrUid, NAUGHTY_CHAIN, IptJumpReject, IptOpDelete);
}

int BandwidthController::addNiceApps(const std::vector<std::string>& appStrUid) {
    return manipulateSpecialApps(appStrUid, NICE_CHAIN, IptJumpReturn, IptOpInsert);
}

int BandwidthController::removeNiceApps(const std::vector<std::string>& appStrUid) {
    return manipulateSpecialApps(appStrUid, NICE_CHAIN, IptJumpReturn, IptOpDelete);
}

int BandwidthController::manipulateSpecialApps(const std::vector<std::string>& appStrUids,
                                               const std::string& chain, IptJumpOp jumpHandling,
                                               IptOp op) {
    if (mBpfSupported) {
        Status status = gCtls->trafficCtrl.updateUidOwnerMap(appStrUids, jumpHandling, op);
        if (!isOk(status)) {
            ALOGE("unable to update the Bandwidth Uid Map: %s", toString(status).c_str());
      }
      return status.code();
    }
    std::string cmd = "*filter\n";
    for (const auto& appStrUid : appStrUids) {
        StringAppendF(&cmd, "%s %s -m owner --uid-owner %s%s\n", opToString(op), chain.c_str(),
                      appStrUid.c_str(), jumpToString(jumpHandling));
    }
    StringAppendF(&cmd, "COMMIT\n");
    return iptablesRestoreFunction(V4V6, cmd, nullptr);
}

int BandwidthController::setInterfaceSharedQuota(const std::string& iface, int64_t maxBytes) {
    int res = 0;
    std::string quotaCmd;
    constexpr char cost[] = "shared";
    constexpr char chain[] = "bw_costly_shared";

    if (!maxBytes) {
        /* Don't talk about -1, deprecate it. */
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -1;
    }
    if (!isIfaceName(iface))
        return -1;

    if (maxBytes == -1) {
        return removeInterfaceSharedQuota(iface);
    }

    auto it = mSharedQuotaIfaces.find(iface);

    if (it == mSharedQuotaIfaces.end()) {
        const int ruleInsertPos = (mGlobalAlertBytes) ? 2 : 1;
        std::vector<std::string> cmds = {
            "*filter",
            StringPrintf("-I bw_INPUT %d -i %s --jump %s", ruleInsertPos, iface.c_str(), chain),
            StringPrintf("-I bw_OUTPUT %d -o %s --jump %s", ruleInsertPos, iface.c_str(), chain),
            StringPrintf("-A bw_FORWARD -i %s --jump %s", iface.c_str(), chain),
            StringPrintf("-A bw_FORWARD -o %s --jump %s", iface.c_str(), chain),
        };
        if (mSharedQuotaIfaces.empty()) {
            cmds.push_back(StringPrintf("-I %s -m quota2 ! --quota %" PRId64
                                        " --name %s --jump REJECT",
                                        chain, maxBytes, cost));
        }
        cmds.push_back("COMMIT\n");

        res |= iptablesRestoreFunction(V4V6, Join(cmds, "\n"), nullptr);
        if (res) {
            ALOGE("Failed set quota rule");
            removeInterfaceSharedQuota(iface);
            return -1;
        }
        mSharedQuotaBytes = maxBytes;
        mSharedQuotaIfaces.insert(iface);
    }

    if (maxBytes != mSharedQuotaBytes) {
        res |= updateQuota(cost, maxBytes);
        if (res) {
            ALOGE("Failed update quota for %s", cost);
            removeInterfaceSharedQuota(iface);
            return -1;
        }
        mSharedQuotaBytes = maxBytes;
    }
    return 0;
}

/* It will also cleanup any shared alerts */
int BandwidthController::removeInterfaceSharedQuota(const std::string& iface) {
    constexpr char cost[] = "shared";
    constexpr char chain[] = "bw_costly_shared";

    if (!isIfaceName(iface))
        return -1;

    auto it = mSharedQuotaIfaces.find(iface);

    if (it == mSharedQuotaIfaces.end()) {
        ALOGE("No such iface %s to delete", iface.c_str());
        return -1;
    }

    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf("-D bw_INPUT -i %s --jump %s", iface.c_str(), chain),
        StringPrintf("-D bw_OUTPUT -o %s --jump %s", iface.c_str(), chain),
        StringPrintf("-D bw_FORWARD -i %s --jump %s", iface.c_str(), chain),
        StringPrintf("-D bw_FORWARD -o %s --jump %s", iface.c_str(), chain),
    };
    if (mSharedQuotaIfaces.size() == 1) {
        cmds.push_back(StringPrintf("-D %s -m quota2 ! --quota %" PRIu64
                                    " --name %s --jump REJECT",
                                    chain, mSharedQuotaBytes, cost));
    }
    cmds.push_back("COMMIT\n");

    if (iptablesRestoreFunction(V4V6, Join(cmds, "\n"), nullptr) != 0) {
        ALOGE("Failed to remove shared quota on %s", iface.c_str());
        return -1;
    }

    int res = 0;
    mSharedQuotaIfaces.erase(it);
    if (mSharedQuotaIfaces.empty()) {
        mSharedQuotaBytes = 0;
        if (mSharedAlertBytes) {
            res = removeSharedAlert();
            if (res == 0) {
                mSharedAlertBytes = 0;
            }
        }
    }

    return res;

}

int BandwidthController::setInterfaceQuota(const std::string& iface, int64_t maxBytes) {
    const std::string& cost = iface;

    if (!isIfaceName(iface)) return -EINVAL;

    if (!maxBytes) {
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -ERANGE;
    }
    if (maxBytes == -1) {
        return removeInterfaceQuota(iface);
    }

    /* Insert ingress quota. */
    auto it = mQuotaIfaces.find(iface);

    if (it != mQuotaIfaces.end()) {
        if (int res = updateQuota(cost, maxBytes)) {
            ALOGE("Failed update quota for %s", iface.c_str());
            removeInterfaceQuota(iface);
            return res;
        }
        it->second.quota = maxBytes;
        return 0;
    }

    const std::string chain = "bw_costly_" + iface;
    const int ruleInsertPos = (mGlobalAlertBytes) ? 2 : 1;
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf(":%s -", chain.c_str()),
        StringPrintf("-A %s -j bw_penalty_box", chain.c_str()),
        StringPrintf("-I bw_INPUT %d -i %s --jump %s", ruleInsertPos, iface.c_str(),
                     chain.c_str()),
        StringPrintf("-I bw_OUTPUT %d -o %s --jump %s", ruleInsertPos, iface.c_str(),
                     chain.c_str()),
        StringPrintf("-A bw_FORWARD -i %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-A bw_FORWARD -o %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-A %s -m quota2 ! --quota %" PRId64 " --name %s --jump REJECT",
                     chain.c_str(), maxBytes, cost.c_str()),
        "COMMIT\n",
    };
    if (iptablesRestoreFunction(V4V6, Join(cmds, "\n"), nullptr) != 0) {
        ALOGE("Failed set quota rule");
        removeInterfaceQuota(iface);
        return -EREMOTEIO;
    }

    mQuotaIfaces[iface] = QuotaInfo{maxBytes, 0};
    return 0;
}

int BandwidthController::getInterfaceSharedQuota(int64_t *bytes) {
    return getInterfaceQuota("shared", bytes);
}

int BandwidthController::getInterfaceQuota(const std::string& iface, int64_t* bytes) {
    const auto& sys = android::netdutils::sSyscalls.get();
    const std::string fname = "/proc/net/xt_quota/" + iface;

    if (!isIfaceName(iface)) return -1;

    StatusOr<UniqueFile> file = sys.fopen(fname, "re");
    if (!isOk(file)) {
        ALOGE("Reading quota %s failed (%s)", iface.c_str(), toString(file).c_str());
        return -1;
    }
    auto rv = sys.fscanf(file.value().get(), "%" SCNd64, bytes);
    if (!isOk(rv)) {
        ALOGE("Reading quota %s failed (%s)", iface.c_str(), toString(rv).c_str());
        return -1;
    }
    ALOGV("Read quota res=%d bytes=%" PRId64, rv.value(), *bytes);
    return rv.value() == 1 ? 0 : -1;
}

int BandwidthController::removeInterfaceQuota(const std::string& iface) {
    if (!isIfaceName(iface)) return -EINVAL;

    auto it = mQuotaIfaces.find(iface);

    if (it == mQuotaIfaces.end()) {
        ALOGE("No such iface %s to delete", iface.c_str());
        return -ENODEV;
    }

    const std::string chain = "bw_costly_" + iface;
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf("-D bw_INPUT -i %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-D bw_OUTPUT -o %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-D bw_FORWARD -i %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-D bw_FORWARD -o %s --jump %s", iface.c_str(), chain.c_str()),
        StringPrintf("-F %s", chain.c_str()),
        StringPrintf("-X %s", chain.c_str()),
        "COMMIT\n",
    };

    const int res = iptablesRestoreFunction(V4V6, Join(cmds, "\n"), nullptr);

    if (res == 0) {
        mQuotaIfaces.erase(it);
    }

    return res ? -EREMOTEIO : 0;
}

int BandwidthController::updateQuota(const std::string& quotaName, int64_t bytes) {
    const auto& sys = android::netdutils::sSyscalls.get();
    const std::string fname = "/proc/net/xt_quota/" + quotaName;

    if (!isIfaceName(quotaName)) {
        ALOGE("updateQuota: Invalid quotaName \"%s\"", quotaName.c_str());
        return -EINVAL;
    }

    StatusOr<UniqueFile> file = sys.fopen(fname, "we");
    if (!isOk(file)) {
        int res = errno;
        ALOGE("Updating quota %s failed (%s)", quotaName.c_str(), toString(file).c_str());
        return -res;
    }
    // TODO: should we propagate this error?
    sys.fprintf(file.value().get(), "%" PRId64 "\n", bytes).ignoreError();
    return 0;
}

int BandwidthController::runIptablesAlertCmd(IptOp op, const std::string& alertName,
                                             int64_t bytes) {
    const char *opFlag = opToString(op);
    std::string alertQuotaCmd = "*filter\n";

    // TODO: consider using an alternate template for the delete that does not include the --quota
    // value. This code works because the --quota value is ignored by deletes

    /*
     * Add alert rule in bw_global_alert chain, 3 chains might reference bw_global_alert.
     * bw_INPUT, bw_OUTPUT (added by BandwidthController in enableBandwidthControl)
     * bw_FORWARD (added by TetherController in setTetherGlobalAlertRule if nat enable/disable)
     */
    StringAppendF(&alertQuotaCmd, ALERT_IPT_TEMPLATE, opFlag, LOCAL_GLOBAL_ALERT, bytes,
                  alertName.c_str());
    StringAppendF(&alertQuotaCmd, "COMMIT\n");

    return iptablesRestoreFunction(V4V6, alertQuotaCmd, nullptr);
}

int BandwidthController::setGlobalAlert(int64_t bytes) {
    const char *alertName = ALERT_GLOBAL_NAME;

    if (!bytes) {
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -ERANGE;
    }

    int res = 0;
    if (mGlobalAlertBytes) {
        res = updateQuota(alertName, bytes);
    } else {
        res = runIptablesAlertCmd(IptOpInsert, alertName, bytes);
        if (res) {
            res = -EREMOTEIO;
        }
    }
    mGlobalAlertBytes = bytes;
    return res;
}

int BandwidthController::removeGlobalAlert() {

    const char *alertName = ALERT_GLOBAL_NAME;

    if (!mGlobalAlertBytes) {
        ALOGE("No prior alert set");
        return -1;
    }

    int res = 0;
    res = runIptablesAlertCmd(IptOpDelete, alertName, mGlobalAlertBytes);
    mGlobalAlertBytes = 0;
    return res;
}

int BandwidthController::setSharedAlert(int64_t bytes) {
    if (!mSharedQuotaBytes) {
        ALOGE("Need to have a prior shared quota set to set an alert");
        return -1;
    }
    if (!bytes) {
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -1;
    }
    return setCostlyAlert("shared", bytes, &mSharedAlertBytes);
}

int BandwidthController::removeSharedAlert() {
    return removeCostlyAlert("shared", &mSharedAlertBytes);
}

int BandwidthController::setInterfaceAlert(const std::string& iface, int64_t bytes) {
    if (!isIfaceName(iface)) {
        ALOGE("setInterfaceAlert: Invalid iface \"%s\"", iface.c_str());
        return -EINVAL;
    }

    if (!bytes) {
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -ERANGE;
    }
    auto it = mQuotaIfaces.find(iface);

    if (it == mQuotaIfaces.end()) {
        ALOGE("Need to have a prior interface quota set to set an alert");
        return -ENOENT;
    }

    return setCostlyAlert(iface, bytes, &it->second.alert);
}

int BandwidthController::removeInterfaceAlert(const std::string& iface) {
    if (!isIfaceName(iface)) {
        ALOGE("removeInterfaceAlert: Invalid iface \"%s\"", iface.c_str());
        return -EINVAL;
    }

    auto it = mQuotaIfaces.find(iface);

    if (it == mQuotaIfaces.end()) {
        ALOGE("No prior alert set for interface %s", iface.c_str());
        return -ENOENT;
    }

    return removeCostlyAlert(iface, &it->second.alert);
}

int BandwidthController::setCostlyAlert(const std::string& costName, int64_t bytes,
                                        int64_t* alertBytes) {
    int res = 0;

    if (!isIfaceName(costName)) {
        ALOGE("setCostlyAlert: Invalid costName \"%s\"", costName.c_str());
        return -EINVAL;
    }

    if (!bytes) {
        ALOGE("Invalid bytes value. 1..max_int64.");
        return -ERANGE;
    }

    std::string alertName = costName + "Alert";
    std::string chainName = "bw_costly_" + costName;
    if (*alertBytes) {
        res = updateQuota(alertName, *alertBytes);
    } else {
        std::vector<std::string> commands = {
            "*filter\n",
            StringPrintf(ALERT_IPT_TEMPLATE, "-A", chainName.c_str(), bytes, alertName.c_str()),
            "COMMIT\n"
        };
        res = iptablesRestoreFunction(V4V6, Join(commands, ""), nullptr);
        if (res) {
            ALOGE("Failed to set costly alert for %s", costName.c_str());
            res = -EREMOTEIO;
        }
    }
    if (res == 0) {
        *alertBytes = bytes;
    }
    return res;
}

int BandwidthController::removeCostlyAlert(const std::string& costName, int64_t* alertBytes) {
    if (!isIfaceName(costName)) {
        ALOGE("removeCostlyAlert: Invalid costName \"%s\"", costName.c_str());
        return -EINVAL;
    }

    if (!*alertBytes) {
        ALOGE("No prior alert set for %s alert", costName.c_str());
        return -ENOENT;
    }

    std::string alertName = costName + "Alert";
    std::string chainName = "bw_costly_" + costName;
    std::vector<std::string> commands = {
        "*filter\n",
        StringPrintf(ALERT_IPT_TEMPLATE, "-D", chainName.c_str(), *alertBytes, alertName.c_str()),
        "COMMIT\n"
    };
    if (iptablesRestoreFunction(V4V6, Join(commands, ""), nullptr) != 0) {
        ALOGE("Failed to remove costly alert %s", costName.c_str());
        return -EREMOTEIO;
    }

    *alertBytes = 0;
    return 0;
}

void BandwidthController::flushExistingCostlyTables(bool doClean) {
    std::string fullCmd = "*filter\n-S\nCOMMIT\n";
    std::string ruleList;

    /* Only lookup ip4 table names as ip6 will have the same tables ... */
    if (int ret = iptablesRestoreFunction(V4, fullCmd, &ruleList)) {
        ALOGE("Failed to list existing costly tables ret=%d", ret);
        return;
    }
    /* ... then flush/clean both ip4 and ip6 iptables. */
    parseAndFlushCostlyTables(ruleList, doClean);
}

void BandwidthController::parseAndFlushCostlyTables(const std::string& ruleList, bool doRemove) {
    std::stringstream stream(ruleList);
    std::string rule;
    std::vector<std::string> clearCommands = { "*filter" };
    std::string chainName;

    // Find and flush all rules starting with "-N bw_costly_<iface>" except "-N bw_costly_shared".
    while (std::getline(stream, rule, '\n')) {
        if (rule.find(NEW_CHAIN_COMMAND) != 0) continue;
        chainName = rule.substr(NEW_CHAIN_COMMAND.size());
        ALOGV("parse chainName=<%s> orig line=<%s>", chainName.c_str(), rule.c_str());

        if (chainName.find("bw_costly_") != 0 || chainName == std::string("bw_costly_shared")) {
            continue;
        }

        clearCommands.push_back(StringPrintf(":%s -", chainName.c_str()));
        if (doRemove) {
            clearCommands.push_back(StringPrintf("-X %s", chainName.c_str()));
        }
    }

    if (clearCommands.size() == 1) {
        // No rules found.
        return;
    }

    clearCommands.push_back("COMMIT\n");
    iptablesRestoreFunction(V4V6, Join(clearCommands, '\n'), nullptr);
}

inline const char *BandwidthController::opToString(IptOp op) {
    switch (op) {
    case IptOpInsert:
        return "-I";
    case IptOpDelete:
        return "-D";
    }
}

inline const char *BandwidthController::jumpToString(IptJumpOp jumpHandling) {
    /*
     * Must be careful what one rejects with, as upper layer protocols will just
     * keep on hammering the device until the number of retries are done.
     * For port-unreachable (default), TCP should consider as an abort (RFC1122).
     */
    switch (jumpHandling) {
    case IptJumpNoAdd:
        return "";
    case IptJumpReject:
        return " --jump REJECT";
    case IptJumpReturn:
        return " --jump RETURN";
    }
}
