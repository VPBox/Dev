/**
 * Copyright (c) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "Netd"

#include <cinttypes>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Status.h>
#include <cutils/properties.h>
#include <json/value.h>
#include <json/writer.h>
#include <log/log.h>
#include <netdutils/DumpWriter.h>
#include <utils/Errors.h>
#include <utils/String16.h>

#include "BinderUtil.h"
#include "Controllers.h"
#include "InterfaceController.h"
#include "NetdConstants.h"  // SHA256_SIZE
#include "NetdNativeService.h"
#include "NetdPermissions.h"
#include "OemNetdListener.h"
#include "Permission.h"
#include "Process.h"
#include "RouteController.h"
#include "SockDiag.h"
#include "UidRanges.h"
#include "android/net/BnNetd.h"
#include "netid_client.h"  // NETID_UNSET

using android::base::StringPrintf;
using android::base::WriteStringToFile;
using android::net::TetherStatsParcel;
using android::net::UidRangeParcel;
using android::netdutils::DumpWriter;
using android::netdutils::ScopedIndent;
using android::os::ParcelFileDescriptor;

namespace android {
namespace net {

namespace {
const char OPT_SHORT[] = "--short";

binder::Status checkAnyPermission(const std::vector<const char*>& permissions) {
    pid_t pid = IPCThreadState::self()->getCallingPid();
    uid_t uid = IPCThreadState::self()->getCallingUid();

    // If the caller is the system UID, don't check permissions.
    // Otherwise, if the system server's binder thread pool is full, and all the threads are
    // blocked on a thread that's waiting for us to complete, we deadlock. http://b/69389492
    //
    // From a security perspective, there is currently no difference, because:
    // 1. The only permissions we check in netd's binder interface are CONNECTIVITY_INTERNAL
    //    and NETWORK_STACK, which the system server always has (or MAINLINE_NETWORK_STACK, which
    //    is equivalent to having both CONNECTIVITY_INTERNAL and NETWORK_STACK).
    // 2. AID_SYSTEM always has all permissions. See ActivityManager#checkComponentPermission.
    if (uid == AID_SYSTEM) {
        return binder::Status::ok();
    }

    for (const char* permission : permissions) {
        if (checkPermission(String16(permission), pid, uid)) {
            return binder::Status::ok();
        }
    }

    auto err = StringPrintf("UID %d / PID %d does not have any of the following permissions: %s",
                            uid, pid, android::base::Join(permissions, ',').c_str());
    return binder::Status::fromExceptionCode(binder::Status::EX_SECURITY, err.c_str());
}

#define ENFORCE_ANY_PERMISSION(...)                                \
    do {                                                           \
        binder::Status status = checkAnyPermission({__VA_ARGS__}); \
        if (!status.isOk()) {                                      \
            return status;                                         \
        }                                                          \
    } while (0)

#define NETD_LOCKING_RPC(lock, ... /* permissions */) \
    ENFORCE_ANY_PERMISSION(__VA_ARGS__);              \
    std::lock_guard _lock(lock);

#define NETD_BIG_LOCK_RPC(... /* permissions */) NETD_LOCKING_RPC(gBigNetdLock, __VA_ARGS__)

#define RETURN_BINDER_STATUS_IF_NOT_OK(logEntry, res) \
    do {                                              \
        if (!isOk((res))) {                           \
            logErrorStatus((logEntry), (res));        \
            return asBinderStatus((res));             \
        }                                             \
    } while (0)

#define ENFORCE_INTERNAL_PERMISSIONS() \
    ENFORCE_ANY_PERMISSION(PERM_CONNECTIVITY_INTERNAL, PERM_MAINLINE_NETWORK_STACK)

#define ENFORCE_NETWORK_STACK_PERMISSIONS() \
    ENFORCE_ANY_PERMISSION(PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK)

void logErrorStatus(netdutils::LogEntry& logEntry, const netdutils::Status& status) {
    gLog.log(logEntry.returns(status.code()).withAutomaticDuration());
}

binder::Status asBinderStatus(const netdutils::Status& status) {
    if (isOk(status)) {
        return binder::Status::ok();
    }
    return binder::Status::fromServiceSpecificError(status.code(), status.msg().c_str());
}

inline binder::Status statusFromErrcode(int ret) {
    if (ret) {
        return binder::Status::fromServiceSpecificError(-ret, strerror(-ret));
    }
    return binder::Status::ok();
}

bool contains(const Vector<String16>& words, const String16& word) {
    for (const auto& w : words) {
        if (w == word) return true;
    }

    return false;
}

}  // namespace

NetdNativeService::NetdNativeService() {
    // register log callback to BnNetd::logFunc
    BnNetd::logFunc = std::bind(binderCallLogFn, std::placeholders::_1,
                                [](const std::string& msg) { gLog.info("%s", msg.c_str()); });
}

status_t NetdNativeService::start() {
    IPCThreadState::self()->disableBackgroundScheduling(true);
    const status_t ret = BinderService<NetdNativeService>::publish();
    if (ret != android::OK) {
        return ret;
    }
    sp<ProcessState> ps(ProcessState::self());
    ps->startThreadPool();
    ps->giveThreadPoolName();

    return android::OK;
}

status_t NetdNativeService::dump(int fd, const Vector<String16> &args) {
    const binder::Status dump_permission = checkAnyPermission({PERM_DUMP});
    if (!dump_permission.isOk()) {
        const String8 msg(dump_permission.toString8());
        write(fd, msg.string(), msg.size());
        return PERMISSION_DENIED;
    }

    // This method does not grab any locks. If individual classes need locking
    // their dump() methods MUST handle locking appropriately.

    DumpWriter dw(fd);

    if (!args.isEmpty() && args[0] == TcpSocketMonitor::DUMP_KEYWORD) {
      dw.blankline();
      gCtls->tcpSocketMonitor.dump(dw);
      dw.blankline();
      return NO_ERROR;
    }

    if (!args.isEmpty() && args[0] == TrafficController::DUMP_KEYWORD) {
        dw.blankline();
        gCtls->trafficCtrl.dump(dw, true);
        dw.blankline();
        return NO_ERROR;
    }

    process::dump(dw);
    dw.blankline();
    gCtls->netCtrl.dump(dw);
    dw.blankline();

    gCtls->trafficCtrl.dump(dw, false);
    dw.blankline();

    gCtls->xfrmCtrl.dump(dw);
    dw.blankline();

    gCtls->clatdCtrl.dump(dw);
    dw.blankline();

    {
        ScopedIndent indentLog(dw);
        if (contains(args, String16(OPT_SHORT))) {
            dw.println("Log: <omitted>");
        } else {
            dw.println("Log:");
            ScopedIndent indentLogEntries(dw);
            gLog.forEachEntry([&dw](const std::string& entry) mutable { dw.println(entry); });
        }
        dw.blankline();
    }

    {
        ScopedIndent indentLog(dw);
        if (contains(args, String16(OPT_SHORT))) {
            dw.println("UnsolicitedLog: <omitted>");
        } else {
            dw.println("UnsolicitedLog:");
            ScopedIndent indentLogEntries(dw);
            gUnsolicitedLog.forEachEntry(
                    [&dw](const std::string& entry) mutable { dw.println(entry); });
        }
        dw.blankline();
    }

    return NO_ERROR;
}

binder::Status NetdNativeService::isAlive(bool *alive) {
    NETD_BIG_LOCK_RPC(PERM_CONNECTIVITY_INTERNAL, PERM_MAINLINE_NETWORK_STACK);

    *alive = true;

    return binder::Status::ok();
}

binder::Status NetdNativeService::firewallReplaceUidChain(const std::string& chainName,
        bool isWhitelist, const std::vector<int32_t>& uids, bool *ret) {
    NETD_LOCKING_RPC(gCtls->firewallCtrl.lock, PERM_CONNECTIVITY_INTERNAL,
                     PERM_MAINLINE_NETWORK_STACK);
    int err = gCtls->firewallCtrl.replaceUidChain(chainName, isWhitelist, uids);
    *ret = (err == 0);
    return binder::Status::ok();
}

binder::Status NetdNativeService::bandwidthEnableDataSaver(bool enable, bool *ret) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_CONNECTIVITY_INTERNAL,
                     PERM_MAINLINE_NETWORK_STACK);
    int err = gCtls->bandwidthCtrl.enableDataSaver(enable);
    *ret = (err == 0);
    return binder::Status::ok();
}

binder::Status NetdNativeService::bandwidthSetInterfaceQuota(const std::string& ifName,
                                                             int64_t bytes) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->bandwidthCtrl.setInterfaceQuota(ifName, bytes);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthRemoveInterfaceQuota(const std::string& ifName) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->bandwidthCtrl.removeInterfaceQuota(ifName);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthSetInterfaceAlert(const std::string& ifName,
                                                             int64_t bytes) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->bandwidthCtrl.setInterfaceAlert(ifName, bytes);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthRemoveInterfaceAlert(const std::string& ifName) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->bandwidthCtrl.removeInterfaceAlert(ifName);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthSetGlobalAlert(int64_t bytes) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->bandwidthCtrl.setGlobalAlert(bytes);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthAddNaughtyApp(int32_t uid) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    std::vector<std::string> appStrUids = {std::to_string(abs(uid))};
    int res = gCtls->bandwidthCtrl.addNaughtyApps(appStrUids);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthRemoveNaughtyApp(int32_t uid) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    std::vector<std::string> appStrUids = {std::to_string(abs(uid))};
    int res = gCtls->bandwidthCtrl.removeNaughtyApps(appStrUids);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthAddNiceApp(int32_t uid) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    std::vector<std::string> appStrUids = {std::to_string(abs(uid))};
    int res = gCtls->bandwidthCtrl.addNiceApps(appStrUids);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::bandwidthRemoveNiceApp(int32_t uid) {
    NETD_LOCKING_RPC(gCtls->bandwidthCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    std::vector<std::string> appStrUids = {std::to_string(abs(uid))};
    int res = gCtls->bandwidthCtrl.removeNiceApps(appStrUids);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkCreatePhysical(int32_t netId, int32_t permission) {
    ENFORCE_INTERNAL_PERMISSIONS();
    int ret = gCtls->netCtrl.createPhysicalNetwork(netId, convertPermission(permission));
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkCreateVpn(int32_t netId, bool secure) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    int ret = gCtls->netCtrl.createVirtualNetwork(netId, secure);
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkDestroy(int32_t netId) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    // NetworkController::destroyNetwork is thread-safe.
    const int ret = gCtls->netCtrl.destroyNetwork(netId);
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkAddInterface(int32_t netId, const std::string& iface) {
    ENFORCE_INTERNAL_PERMISSIONS();
    int ret = gCtls->netCtrl.addInterfaceToNetwork(netId, iface.c_str());
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkRemoveInterface(int32_t netId, const std::string& iface) {
    ENFORCE_INTERNAL_PERMISSIONS();
    int ret = gCtls->netCtrl.removeInterfaceFromNetwork(netId, iface.c_str());
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkAddUidRanges(
        int32_t netId, const std::vector<UidRangeParcel>& uidRangeArray) {
    // NetworkController::addUsersToNetwork is thread-safe.
    ENFORCE_INTERNAL_PERMISSIONS();
    int ret = gCtls->netCtrl.addUsersToNetwork(netId, UidRanges(uidRangeArray));
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkRemoveUidRanges(
        int32_t netId, const std::vector<UidRangeParcel>& uidRangeArray) {
    // NetworkController::removeUsersFromNetwork is thread-safe.
    ENFORCE_INTERNAL_PERMISSIONS();
    int ret = gCtls->netCtrl.removeUsersFromNetwork(netId, UidRanges(uidRangeArray));
    return statusFromErrcode(ret);
}

binder::Status NetdNativeService::networkRejectNonSecureVpn(
        bool add, const std::vector<UidRangeParcel>& uidRangeArray) {
    // TODO: elsewhere RouteController is only used from the tethering and network controllers, so
    // it should be possible to use the same lock as NetworkController. However, every call through
    // the CommandListener "network" command will need to hold this lock too, not just the ones that
    // read/modify network internal state (that is sufficient for ::dump() because it doesn't
    // look at routes, but it's not enough here).
    NETD_BIG_LOCK_RPC(PERM_CONNECTIVITY_INTERNAL, PERM_MAINLINE_NETWORK_STACK);
    UidRanges uidRanges(uidRangeArray);

    int err;
    if (add) {
        err = RouteController::addUsersToRejectNonSecureNetworkRule(uidRanges);
    } else {
        err = RouteController::removeUsersFromRejectNonSecureNetworkRule(uidRanges);
    }
    return statusFromErrcode(err);
}

binder::Status NetdNativeService::socketDestroy(const std::vector<UidRangeParcel>& uids,
                                                const std::vector<int32_t>& skipUids) {
    ENFORCE_INTERNAL_PERMISSIONS();

    SockDiag sd;
    if (!sd.open()) {
        return binder::Status::fromServiceSpecificError(EIO,
                String8("Could not open SOCK_DIAG socket"));
    }

    UidRanges uidRanges(uids);
    int err = sd.destroySockets(uidRanges, std::set<uid_t>(skipUids.begin(), skipUids.end()),
                                true /* excludeLoopback */);
    if (err) {
        return binder::Status::fromServiceSpecificError(-err,
                String8::format("destroySockets: %s", strerror(-err)));
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::tetherApplyDnsInterfaces(bool *ret) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    *ret = gCtls->tetherCtrl.applyDnsInterfaces();
    return binder::Status::ok();
}

namespace {

void tetherAddStatsByInterface(TetherController::TetherStats* tetherStatsParcel,
                               const TetherController::TetherStats& tetherStats) {
    if (tetherStatsParcel->extIface == tetherStats.extIface) {
        tetherStatsParcel->rxBytes += tetherStats.rxBytes;
        tetherStatsParcel->rxPackets += tetherStats.rxPackets;
        tetherStatsParcel->txBytes += tetherStats.txBytes;
        tetherStatsParcel->txPackets += tetherStats.txPackets;
    }
}

TetherStatsParcel toTetherStatsParcel(const TetherController::TetherStats& stats) {
    TetherStatsParcel result;
    result.iface = stats.extIface;
    result.rxBytes = stats.rxBytes;
    result.rxPackets = stats.rxPackets;
    result.txBytes = stats.txBytes;
    result.txPackets = stats.txPackets;
    return result;
}

void setTetherStatsParcelVecByInterface(std::vector<TetherStatsParcel>* tetherStatsVec,
                                        const TetherController::TetherStatsList& statsList) {
    std::map<std::string, TetherController::TetherStats> statsMap;
    for (const auto& stats : statsList) {
        auto iter = statsMap.find(stats.extIface);
        if (iter != statsMap.end()) {
            tetherAddStatsByInterface(&(iter->second), stats);
        } else {
            statsMap.insert(
                    std::pair<std::string, TetherController::TetherStats>(stats.extIface, stats));
        }
    }
    for (auto iter = statsMap.begin(); iter != statsMap.end(); iter++) {
        tetherStatsVec->push_back(toTetherStatsParcel(iter->second));
    }
}

std::vector<std::string> tetherStatsParcelVecToStringVec(std::vector<TetherStatsParcel>* tVec) {
    std::vector<std::string> result;
    for (const auto& t : *tVec) {
        result.push_back(StringPrintf("%s:%" PRId64 ",%" PRId64 ",%" PRId64 ",%" PRId64,
                                      t.iface.c_str(), t.rxBytes, t.rxPackets, t.txBytes,
                                      t.txPackets));
    }
    return result;
}

}  // namespace

binder::Status NetdNativeService::tetherGetStats(
        std::vector<TetherStatsParcel>* tetherStatsParcelVec) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    const auto& statsList = gCtls->tetherCtrl.getTetherStats();
    if (!isOk(statsList)) {
        return asBinderStatus(statsList);
    }
    setTetherStatsParcelVecByInterface(tetherStatsParcelVec, statsList.value());
    auto statsResults = tetherStatsParcelVecToStringVec(tetherStatsParcelVec);
    return binder::Status::ok();
}

binder::Status NetdNativeService::interfaceAddAddress(const std::string &ifName,
        const std::string &addrString, int prefixLength) {
    ENFORCE_INTERNAL_PERMISSIONS();
    const int err = InterfaceController::addAddress(
            ifName.c_str(), addrString.c_str(), prefixLength);
    if (err != 0) {
        return binder::Status::fromServiceSpecificError(-err,
                String8::format("InterfaceController error: %s", strerror(-err)));
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::interfaceDelAddress(const std::string &ifName,
        const std::string &addrString, int prefixLength) {
    ENFORCE_INTERNAL_PERMISSIONS();
    const int err = InterfaceController::delAddress(
            ifName.c_str(), addrString.c_str(), prefixLength);
    if (err != 0) {
        return binder::Status::fromServiceSpecificError(-err,
                String8::format("InterfaceController error: %s", strerror(-err)));
    }
    return binder::Status::ok();
}

namespace {

std::tuple<binder::Status, const char*, const char*> getPathComponents(int32_t ipversion,
                                                                       int32_t category) {
    const char* ipversionStr = nullptr;
    switch (ipversion) {
        case INetd::IPV4:
            ipversionStr = "ipv4";
            break;
        case INetd::IPV6:
            ipversionStr = "ipv6";
            break;
        default:
            return {binder::Status::fromServiceSpecificError(EAFNOSUPPORT, "Bad IP version"),
                    nullptr, nullptr};
    }

    const char* whichStr = nullptr;
    switch (category) {
        case INetd::CONF:
            whichStr = "conf";
            break;
        case INetd::NEIGH:
            whichStr = "neigh";
            break;
        default:
            return {binder::Status::fromServiceSpecificError(EINVAL, "Bad category"), nullptr,
                    nullptr};
    }

    return {binder::Status::ok(), ipversionStr, whichStr};
}

}  // namespace

binder::Status NetdNativeService::getProcSysNet(int32_t ipversion, int32_t which,
                                                const std::string& ifname,
                                                const std::string& parameter, std::string* value) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    const auto pathParts = getPathComponents(ipversion, which);
    const auto& pathStatus = std::get<0>(pathParts);
    if (!pathStatus.isOk()) {
        return pathStatus;
    }

    const int err = InterfaceController::getParameter(std::get<1>(pathParts),
                                                      std::get<2>(pathParts), ifname.c_str(),
                                                      parameter.c_str(), value);
    return statusFromErrcode(err);
}

binder::Status NetdNativeService::setProcSysNet(int32_t ipversion, int32_t which,
                                                const std::string& ifname,
                                                const std::string& parameter,
                                                const std::string& value) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    const auto pathParts = getPathComponents(ipversion, which);
    const auto& pathStatus = std::get<0>(pathParts);
    if (!pathStatus.isOk()) {
        return pathStatus;
    }

    const int err = InterfaceController::setParameter(std::get<1>(pathParts),
                                                      std::get<2>(pathParts), ifname.c_str(),
                                                      parameter.c_str(), value.c_str());
    return statusFromErrcode(err);
}

binder::Status NetdNativeService::ipSecSetEncapSocketOwner(const ParcelFileDescriptor& socket,
                                                           int newUid) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gLog.log("ipSecSetEncapSocketOwner()");

    uid_t callerUid = IPCThreadState::self()->getCallingUid();
    return asBinderStatus(
            gCtls->xfrmCtrl.ipSecSetEncapSocketOwner(socket.get(), newUid, callerUid));
}

binder::Status NetdNativeService::ipSecAllocateSpi(
        int32_t transformId,
        const std::string& sourceAddress,
        const std::string& destinationAddress,
        int32_t inSpi,
        int32_t* outSpi) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_INTERNAL_PERMISSIONS();
    gLog.log("ipSecAllocateSpi()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecAllocateSpi(
                    transformId,
                    sourceAddress,
                    destinationAddress,
                    inSpi,
                    outSpi));
}

binder::Status NetdNativeService::ipSecAddSecurityAssociation(
        int32_t transformId, int32_t mode, const std::string& sourceAddress,
        const std::string& destinationAddress, int32_t underlyingNetId, int32_t spi,
        int32_t markValue, int32_t markMask, const std::string& authAlgo,
        const std::vector<uint8_t>& authKey, int32_t authTruncBits, const std::string& cryptAlgo,
        const std::vector<uint8_t>& cryptKey, int32_t cryptTruncBits, const std::string& aeadAlgo,
        const std::vector<uint8_t>& aeadKey, int32_t aeadIcvBits, int32_t encapType,
        int32_t encapLocalPort, int32_t encapRemotePort, int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_INTERNAL_PERMISSIONS();
    gLog.log("ipSecAddSecurityAssociation()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecAddSecurityAssociation(
            transformId, mode, sourceAddress, destinationAddress, underlyingNetId, spi, markValue,
            markMask, authAlgo, authKey, authTruncBits, cryptAlgo, cryptKey, cryptTruncBits,
            aeadAlgo, aeadKey, aeadIcvBits, encapType, encapLocalPort, encapRemotePort,
            interfaceId));
}

binder::Status NetdNativeService::ipSecDeleteSecurityAssociation(
        int32_t transformId, const std::string& sourceAddress,
        const std::string& destinationAddress, int32_t spi, int32_t markValue, int32_t markMask,
        int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_INTERNAL_PERMISSIONS();
    gLog.log("ipSecDeleteSecurityAssociation()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecDeleteSecurityAssociation(
            transformId, sourceAddress, destinationAddress, spi, markValue, markMask, interfaceId));
}

binder::Status NetdNativeService::ipSecApplyTransportModeTransform(
        const ParcelFileDescriptor& socket, int32_t transformId, int32_t direction,
        const std::string& sourceAddress, const std::string& destinationAddress, int32_t spi) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_INTERNAL_PERMISSIONS();
    gLog.log("ipSecApplyTransportModeTransform()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecApplyTransportModeTransform(
            socket.get(), transformId, direction, sourceAddress, destinationAddress, spi));
}

binder::Status NetdNativeService::ipSecRemoveTransportModeTransform(
        const ParcelFileDescriptor& socket) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_INTERNAL_PERMISSIONS();
    gLog.log("ipSecRemoveTransportModeTransform()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecRemoveTransportModeTransform(socket.get()));
}

binder::Status NetdNativeService::ipSecAddSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                                         int32_t direction,
                                                         const std::string& tmplSrcAddress,
                                                         const std::string& tmplDstAddress,
                                                         int32_t spi, int32_t markValue,
                                                         int32_t markMask, int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gLog.log("ipSecAddSecurityPolicy()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecAddSecurityPolicy(
            transformId, selAddrFamily, direction, tmplSrcAddress, tmplDstAddress, spi, markValue,
            markMask, interfaceId));
}

binder::Status NetdNativeService::ipSecUpdateSecurityPolicy(
        int32_t transformId, int32_t selAddrFamily, int32_t direction,
        const std::string& tmplSrcAddress, const std::string& tmplDstAddress, int32_t spi,
        int32_t markValue, int32_t markMask, int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gLog.log("ipSecAddSecurityPolicy()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecUpdateSecurityPolicy(
            transformId, selAddrFamily, direction, tmplSrcAddress, tmplDstAddress, spi, markValue,
            markMask, interfaceId));
}

binder::Status NetdNativeService::ipSecDeleteSecurityPolicy(int32_t transformId,
                                                            int32_t selAddrFamily,
                                                            int32_t direction, int32_t markValue,
                                                            int32_t markMask, int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gLog.log("ipSecAddSecurityPolicy()");
    return asBinderStatus(gCtls->xfrmCtrl.ipSecDeleteSecurityPolicy(
            transformId, selAddrFamily, direction, markValue, markMask, interfaceId));
}

binder::Status NetdNativeService::ipSecAddTunnelInterface(const std::string& deviceName,
                                                          const std::string& localAddress,
                                                          const std::string& remoteAddress,
                                                          int32_t iKey, int32_t oKey,
                                                          int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    netdutils::Status result = gCtls->xfrmCtrl.ipSecAddTunnelInterface(
            deviceName, localAddress, remoteAddress, iKey, oKey, interfaceId, false);
    return binder::Status::ok();
}

binder::Status NetdNativeService::ipSecUpdateTunnelInterface(const std::string& deviceName,
                                                             const std::string& localAddress,
                                                             const std::string& remoteAddress,
                                                             int32_t iKey, int32_t oKey,
                                                             int32_t interfaceId) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    netdutils::Status result = gCtls->xfrmCtrl.ipSecAddTunnelInterface(
            deviceName, localAddress, remoteAddress, iKey, oKey, interfaceId, true);
    return binder::Status::ok();
}

binder::Status NetdNativeService::ipSecRemoveTunnelInterface(const std::string& deviceName) {
    // Necessary locking done in IpSecService and kernel
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    netdutils::Status result = gCtls->xfrmCtrl.ipSecRemoveTunnelInterface(deviceName);
    return binder::Status::ok();
}

binder::Status NetdNativeService::setIPv6AddrGenMode(const std::string& ifName,
                                                     int32_t mode) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    return asBinderStatus(InterfaceController::setIPv6AddrGenMode(ifName, mode));
}

binder::Status NetdNativeService::wakeupAddInterface(const std::string& ifName,
                                                     const std::string& prefix, int32_t mark,
                                                     int32_t mask) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    return asBinderStatus(gCtls->wakeupCtrl.addInterface(ifName, prefix, mark, mask));
}

binder::Status NetdNativeService::wakeupDelInterface(const std::string& ifName,
                                                     const std::string& prefix, int32_t mark,
                                                     int32_t mask) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    return asBinderStatus(gCtls->wakeupCtrl.delInterface(ifName, prefix, mark, mask));
}

binder::Status NetdNativeService::trafficSwapActiveStatsMap() {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    return asBinderStatus(gCtls->trafficCtrl.swapActiveStatsMap());
}

binder::Status NetdNativeService::idletimerAddInterface(const std::string& ifName, int32_t timeout,
                                                        const std::string& classLabel) {
    NETD_LOCKING_RPC(gCtls->idletimerCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res =
            gCtls->idletimerCtrl.addInterfaceIdletimer(ifName.c_str(), timeout, classLabel.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::idletimerRemoveInterface(const std::string& ifName,
                                                           int32_t timeout,
                                                           const std::string& classLabel) {
    NETD_LOCKING_RPC(gCtls->idletimerCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->idletimerCtrl.removeInterfaceIdletimer(ifName.c_str(), timeout,
                                                            classLabel.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::strictUidCleartextPenalty(int32_t uid, int32_t policyPenalty) {
    NETD_LOCKING_RPC(gCtls->strictCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    StrictPenalty penalty;
    switch (policyPenalty) {
        case INetd::PENALTY_POLICY_REJECT:
            penalty = REJECT;
            break;
        case INetd::PENALTY_POLICY_LOG:
            penalty = LOG;
            break;
        case INetd::PENALTY_POLICY_ACCEPT:
            penalty = ACCEPT;
            break;
        default:
            return statusFromErrcode(-EINVAL);
            break;
    }
    int res = gCtls->strictCtrl.setUidCleartextPenalty((uid_t) uid, penalty);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::clatdStart(const std::string& ifName,
                                             const std::string& nat64Prefix, std::string* v6Addr) {
    ENFORCE_ANY_PERMISSION(PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->clatdCtrl.startClatd(ifName.c_str(), nat64Prefix, v6Addr);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::clatdStop(const std::string& ifName) {
    ENFORCE_ANY_PERMISSION(PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->clatdCtrl.stopClatd(ifName.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::ipfwdEnabled(bool* status) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    *status = (gCtls->tetherCtrl.getIpfwdRequesterList().size() > 0) ? true : false;
    return binder::Status::ok();
}

binder::Status NetdNativeService::ipfwdGetRequesterList(std::vector<std::string>* requesterList) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    for (const auto& requester : gCtls->tetherCtrl.getIpfwdRequesterList()) {
        requesterList->push_back(requester);
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::ipfwdEnableForwarding(const std::string& requester) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = (gCtls->tetherCtrl.enableForwarding(requester.c_str())) ? 0 : -EREMOTEIO;
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::ipfwdDisableForwarding(const std::string& requester) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = (gCtls->tetherCtrl.disableForwarding(requester.c_str())) ? 0 : -EREMOTEIO;
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::ipfwdAddInterfaceForward(const std::string& fromIface,
                                                           const std::string& toIface) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    int res = RouteController::enableTethering(fromIface.c_str(), toIface.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::ipfwdRemoveInterfaceForward(const std::string& fromIface,
                                                              const std::string& toIface) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    int res = RouteController::disableTethering(fromIface.c_str(), toIface.c_str());
    return statusFromErrcode(res);
}

namespace {
std::string addCurlyBrackets(const std::string& s) {
    return "{" + s + "}";
}

}  // namespace

binder::Status NetdNativeService::interfaceGetList(std::vector<std::string>* interfaceListResult) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    const auto& ifaceList = InterfaceController::getIfaceNames();

    interfaceListResult->clear();
    interfaceListResult->reserve(ifaceList.value().size());
    interfaceListResult->insert(end(*interfaceListResult), begin(ifaceList.value()),
                                end(ifaceList.value()));

    return binder::Status::ok();
}

std::string interfaceConfigurationParcelToString(const InterfaceConfigurationParcel& cfg) {
    std::vector<std::string> result{cfg.ifName, cfg.hwAddr, cfg.ipv4Addr,
                                    std::to_string(cfg.prefixLength)};
    result.insert(end(result), begin(cfg.flags), end(cfg.flags));
    return addCurlyBrackets(base::Join(result, ", "));
}

binder::Status NetdNativeService::interfaceGetCfg(
        const std::string& ifName, InterfaceConfigurationParcel* interfaceGetCfgResult) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto entry = gLog.newEntry().prettyFunction(__PRETTY_FUNCTION__).arg(ifName);

    const auto& cfgRes = InterfaceController::getCfg(ifName);
    RETURN_BINDER_STATUS_IF_NOT_OK(entry, cfgRes);

    *interfaceGetCfgResult = cfgRes.value();
    gLog.log(entry.returns(interfaceConfigurationParcelToString(*interfaceGetCfgResult))
                     .withAutomaticDuration());
    return binder::Status::ok();
}

binder::Status NetdNativeService::interfaceSetCfg(const InterfaceConfigurationParcel& cfg) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto entry = gLog.newEntry()
                         .prettyFunction(__PRETTY_FUNCTION__)
                         .arg(interfaceConfigurationParcelToString(cfg));

    const auto& res = InterfaceController::setCfg(cfg);
    RETURN_BINDER_STATUS_IF_NOT_OK(entry, res);

    gLog.log(entry.withAutomaticDuration());
    return binder::Status::ok();
}

binder::Status NetdNativeService::interfaceSetIPv6PrivacyExtensions(const std::string& ifName,
                                                                    bool enable) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = InterfaceController::setIPv6PrivacyExtensions(ifName.c_str(), enable);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::interfaceClearAddrs(const std::string& ifName) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = InterfaceController::clearAddrs(ifName.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::interfaceSetEnableIPv6(const std::string& ifName, bool enable) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = InterfaceController::setEnableIPv6(ifName.c_str(), enable);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::interfaceSetMtu(const std::string& ifName, int32_t mtuValue) {
    NETD_LOCKING_RPC(InterfaceController::mutex, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    std::string mtu = std::to_string(mtuValue);
    int res = InterfaceController::setMtu(ifName.c_str(), mtu.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherStart(const std::vector<std::string>& dhcpRanges) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    if (dhcpRanges.size() % 2 == 1) {
        return statusFromErrcode(-EINVAL);
    }
    int res = gCtls->tetherCtrl.startTethering(dhcpRanges);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherStop() {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->tetherCtrl.stopTethering();
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherIsEnabled(bool* enabled) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    *enabled = gCtls->tetherCtrl.isTetheringStarted();
    return binder::Status::ok();
}

binder::Status NetdNativeService::tetherInterfaceAdd(const std::string& ifName) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->tetherCtrl.tetherInterface(ifName.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherInterfaceRemove(const std::string& ifName) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->tetherCtrl.untetherInterface(ifName.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherInterfaceList(std::vector<std::string>* ifList) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    for (const auto& ifname : gCtls->tetherCtrl.getTetheredInterfaceList()) {
        ifList->push_back(ifname);
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::tetherDnsSet(int32_t netId,
                                               const std::vector<std::string>& dnsAddrs) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->tetherCtrl.setDnsForwarders(netId, dnsAddrs);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherDnsList(std::vector<std::string>* dnsList) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    for (const auto& fwdr : gCtls->tetherCtrl.getDnsForwarders()) {
        dnsList->push_back(fwdr);
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::networkAddRoute(int32_t netId, const std::string& ifName,
                                                  const std::string& destination,
                                                  const std::string& nextHop) {
    // Public methods of NetworkController are thread-safe.
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    bool legacy = false;
    uid_t uid = 0;  // UID is only meaningful for legacy routes.
    int res = gCtls->netCtrl.addRoute(netId, ifName.c_str(), destination.c_str(),
                                      nextHop.empty() ? nullptr : nextHop.c_str(), legacy, uid);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkRemoveRoute(int32_t netId, const std::string& ifName,
                                                     const std::string& destination,
                                                     const std::string& nextHop) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    bool legacy = false;
    uid_t uid = 0;  // UID is only meaningful for legacy routes.
    int res = gCtls->netCtrl.removeRoute(netId, ifName.c_str(), destination.c_str(),
                                         nextHop.empty() ? nullptr : nextHop.c_str(), legacy, uid);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkAddLegacyRoute(int32_t netId, const std::string& ifName,
                                                        const std::string& destination,
                                                        const std::string& nextHop, int32_t uid) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    bool legacy = true;
    int res = gCtls->netCtrl.addRoute(netId, ifName.c_str(), destination.c_str(),
                                      nextHop.empty() ? nullptr : nextHop.c_str(), legacy,
                                      (uid_t) uid);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkRemoveLegacyRoute(int32_t netId, const std::string& ifName,
                                                           const std::string& destination,
                                                           const std::string& nextHop,
                                                           int32_t uid) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    bool legacy = true;
    int res = gCtls->netCtrl.removeRoute(netId, ifName.c_str(), destination.c_str(),
                                         nextHop.empty() ? nullptr : nextHop.c_str(), legacy,
                                         (uid_t) uid);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkGetDefault(int32_t* netId) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    *netId = gCtls->netCtrl.getDefaultNetwork();
    return binder::Status::ok();
}

binder::Status NetdNativeService::networkSetDefault(int32_t netId) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    int res = gCtls->netCtrl.setDefaultNetwork(netId);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkClearDefault() {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    unsigned netId = NETID_UNSET;
    int res = gCtls->netCtrl.setDefaultNetwork(netId);
    return statusFromErrcode(res);
}

std::vector<uid_t> NetdNativeService::intsToUids(const std::vector<int32_t>& intUids) {
    return {begin(intUids), end(intUids)};
}

Permission NetdNativeService::convertPermission(int32_t permission) {
    switch (permission) {
        case INetd::PERMISSION_NETWORK:
            return Permission::PERMISSION_NETWORK;
        case INetd::PERMISSION_SYSTEM:
            return Permission::PERMISSION_SYSTEM;
        default:
            return Permission::PERMISSION_NONE;
    }
}

binder::Status NetdNativeService::networkSetPermissionForNetwork(int32_t netId,
                                                                 int32_t permission) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    std::vector<unsigned> netIds = {(unsigned) netId};
    int res = gCtls->netCtrl.setPermissionForNetworks(convertPermission(permission), netIds);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::networkSetPermissionForUser(int32_t permission,
                                                              const std::vector<int32_t>& uids) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gCtls->netCtrl.setPermissionForUsers(convertPermission(permission), intsToUids(uids));
    return binder::Status::ok();
}

binder::Status NetdNativeService::networkClearPermissionForUser(const std::vector<int32_t>& uids) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    Permission permission = Permission::PERMISSION_NONE;
    gCtls->netCtrl.setPermissionForUsers(permission, intsToUids(uids));
    return binder::Status::ok();
}

binder::Status NetdNativeService::NetdNativeService::networkSetProtectAllow(int32_t uid) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    std::vector<uid_t> uids = {(uid_t) uid};
    gCtls->netCtrl.allowProtect(uids);
    return binder::Status::ok();
}

binder::Status NetdNativeService::networkSetProtectDeny(int32_t uid) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    std::vector<uid_t> uids = {(uid_t) uid};
    gCtls->netCtrl.denyProtect(uids);
    return binder::Status::ok();
}

binder::Status NetdNativeService::networkCanProtect(int32_t uid, bool* ret) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    *ret = gCtls->netCtrl.canProtect((uid_t) uid);
    return binder::Status::ok();
}

binder::Status NetdNativeService::trafficSetNetPermForUids(int32_t permission,
                                                           const std::vector<int32_t>& uids) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gCtls->trafficCtrl.setPermissionForUids(permission, intsToUids(uids));
    return binder::Status::ok();
}

binder::Status NetdNativeService::firewallSetFirewallType(int32_t firewallType) {
    NETD_LOCKING_RPC(gCtls->firewallCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto type = static_cast<FirewallType>(firewallType);

    int res = gCtls->firewallCtrl.setFirewallType(type);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::firewallSetInterfaceRule(const std::string& ifName,
                                                           int32_t firewallRule) {
    NETD_LOCKING_RPC(gCtls->firewallCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto rule = static_cast<FirewallRule>(firewallRule);

    int res = gCtls->firewallCtrl.setInterfaceRule(ifName.c_str(), rule);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::firewallSetUidRule(int32_t childChain, int32_t uid,
                                                     int32_t firewallRule) {
    NETD_LOCKING_RPC(gCtls->firewallCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto chain = static_cast<ChildChain>(childChain);
    auto rule = static_cast<FirewallRule>(firewallRule);

    int res = gCtls->firewallCtrl.setUidRule(chain, uid, rule);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::firewallEnableChildChain(int32_t childChain, bool enable) {
    NETD_LOCKING_RPC(gCtls->firewallCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    auto chain = static_cast<ChildChain>(childChain);

    int res = gCtls->firewallCtrl.enableChildChains(chain, enable);
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::firewallAddUidInterfaceRules(const std::string& ifName,
                                                               const std::vector<int32_t>& uids) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    return asBinderStatus(gCtls->trafficCtrl.addUidInterfaceRules(
            RouteController::getIfIndex(ifName.c_str()), uids));
}

binder::Status NetdNativeService::firewallRemoveUidInterfaceRules(
        const std::vector<int32_t>& uids) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    return asBinderStatus(gCtls->trafficCtrl.removeUidInterfaceRules(uids));
}

binder::Status NetdNativeService::tetherAddForward(const std::string& intIface,
                                                   const std::string& extIface) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);

    int res = gCtls->tetherCtrl.enableNat(intIface.c_str(), extIface.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::tetherRemoveForward(const std::string& intIface,
                                                      const std::string& extIface) {
    NETD_LOCKING_RPC(gCtls->tetherCtrl.lock, PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK);
    int res = gCtls->tetherCtrl.disableNat(intIface.c_str(), extIface.c_str());
    return statusFromErrcode(res);
}

binder::Status NetdNativeService::setTcpRWmemorySize(const std::string& rmemValues,
                                                     const std::string& wmemValues) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    if (!WriteStringToFile(rmemValues, TCP_RMEM_PROC_FILE)) {
        int ret = -errno;
        return statusFromErrcode(ret);
    }

    if (!WriteStringToFile(wmemValues, TCP_WMEM_PROC_FILE)) {
        int ret = -errno;
        return statusFromErrcode(ret);
    }
    return binder::Status::ok();
}

binder::Status NetdNativeService::registerUnsolicitedEventListener(
        const android::sp<android::net::INetdUnsolicitedEventListener>& listener) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    gCtls->eventReporter.registerUnsolEventListener(listener);
    return binder::Status::ok();
}

binder::Status NetdNativeService::getOemNetd(android::sp<android::IBinder>* listener) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();
    *listener = com::android::internal::net::OemNetdListener::getListener();

    return binder::Status::ok();
}

}  // namespace net
}  // namespace android
