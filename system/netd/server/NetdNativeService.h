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

#ifndef _NETD_NATIVE_SERVICE_H_
#define _NETD_NATIVE_SERVICE_H_

#include <vector>

#include <binder/BinderService.h>
#include <netdutils/Log.h>

#include "android/net/BnNetd.h"

namespace android {
namespace net {

class NetdNativeService : public BinderService<NetdNativeService>, public BnNetd {
  public:
    NetdNativeService();
    static status_t start();
    static char const* getServiceName() { return "netd"; }
    virtual status_t dump(int fd, const Vector<String16> &args) override;

    binder::Status isAlive(bool *alive) override;

    // Firewall commands.
    binder::Status firewallReplaceUidChain(
            const std::string& chainName, bool isWhitelist,
            const std::vector<int32_t>& uids, bool *ret) override;
    binder::Status firewallSetFirewallType(int32_t firewallType) override;
    binder::Status firewallSetInterfaceRule(const std::string& ifName,
                                            int32_t firewallRule) override;
    binder::Status firewallSetUidRule(int32_t childChain, int32_t uid,
                                      int32_t firewallRule) override;
    binder::Status firewallEnableChildChain(int32_t childChain, bool enable) override;
    binder::Status firewallAddUidInterfaceRules(const std::string& ifName,
                                                const std::vector<int32_t>& uids) override;
    binder::Status firewallRemoveUidInterfaceRules(const std::vector<int32_t>& uids) override;

    // Bandwidth control commands.
    binder::Status bandwidthEnableDataSaver(bool enable, bool *ret) override;
    binder::Status bandwidthSetInterfaceQuota(const std::string& ifName, int64_t bytes) override;
    binder::Status bandwidthRemoveInterfaceQuota(const std::string& ifName) override;
    binder::Status bandwidthSetInterfaceAlert(const std::string& ifName, int64_t bytes) override;
    binder::Status bandwidthRemoveInterfaceAlert(const std::string& ifName) override;
    binder::Status bandwidthSetGlobalAlert(int64_t bytes) override;
    binder::Status bandwidthAddNaughtyApp(int32_t uid) override;
    binder::Status bandwidthRemoveNaughtyApp(int32_t uid) override;
    binder::Status bandwidthAddNiceApp(int32_t uid) override;
    binder::Status bandwidthRemoveNiceApp(int32_t uid) override;

    // Network and routing commands.
    binder::Status networkCreatePhysical(int32_t netId, int32_t permission) override;
    binder::Status networkCreateVpn(int32_t netId, bool secure) override;
    binder::Status networkDestroy(int32_t netId) override;

    binder::Status networkAddInterface(int32_t netId, const std::string& iface) override;
    binder::Status networkRemoveInterface(int32_t netId, const std::string& iface) override;

    binder::Status networkAddUidRanges(int32_t netId,
                                       const std::vector<UidRangeParcel>& uids) override;
    binder::Status networkRemoveUidRanges(int32_t netId,
                                          const std::vector<UidRangeParcel>& uids) override;
    binder::Status networkRejectNonSecureVpn(bool enable,
                                             const std::vector<UidRangeParcel>& uids) override;
    binder::Status networkAddRoute(int32_t netId, const std::string& ifName,
                                   const std::string& destination,
                                   const std::string& nextHop) override;
    binder::Status networkRemoveRoute(int32_t netId, const std::string& ifName,
                                      const std::string& destination,
                                      const std::string& nextHop) override;
    binder::Status networkAddLegacyRoute(int32_t netId, const std::string& ifName,
                                         const std::string& destination, const std::string& nextHop,
                                         int32_t uid) override;
    binder::Status networkRemoveLegacyRoute(int32_t netId, const std::string& ifName,
                                            const std::string& destination,
                                            const std::string& nextHop, int32_t uid) override;
    binder::Status networkSetDefault(int32_t netId) override;
    binder::Status networkClearDefault() override;
    binder::Status networkSetPermissionForNetwork(int32_t netId, int32_t permission) override;
    binder::Status networkSetPermissionForUser(int32_t permission,
                                               const std::vector<int32_t>& uids) override;
    binder::Status networkClearPermissionForUser(const std::vector<int32_t>& uids) override;
    binder::Status networkSetProtectAllow(int32_t uid) override;
    binder::Status networkSetProtectDeny(int32_t uid) override;
    // For test (internal use only).
    binder::Status networkGetDefault(int32_t* netId) override;
    binder::Status networkCanProtect(int32_t uid, bool* ret) override;

    binder::Status trafficSetNetPermForUids(int32_t permission,
                                            const std::vector<int32_t>& uids) override;

    // SOCK_DIAG commands.
    binder::Status socketDestroy(const std::vector<UidRangeParcel>& uids,
                                 const std::vector<int32_t>& skipUids) override;

    binder::Status setIPv6AddrGenMode(const std::string& ifName, int32_t mode) override;

    // NFLOG-related commands
    binder::Status wakeupAddInterface(const std::string& ifName, const std::string& prefix,
                                      int32_t mark, int32_t mask) override;

    binder::Status wakeupDelInterface(const std::string& ifName, const std::string& prefix,
                                      int32_t mark, int32_t mask) override;

    // Tethering-related commands.
    binder::Status tetherApplyDnsInterfaces(bool *ret) override;
    binder::Status tetherGetStats(
            std::vector<android::net::TetherStatsParcel>* tetherStatsVec) override;
    binder::Status tetherStart(const std::vector<std::string>& dhcpRanges) override;
    binder::Status tetherStop() override;
    binder::Status tetherIsEnabled(bool* enabled) override;
    binder::Status tetherInterfaceAdd(const std::string& ifName) override;
    binder::Status tetherInterfaceRemove(const std::string& ifName) override;
    binder::Status tetherInterfaceList(std::vector<std::string>* ifList) override;
    binder::Status tetherDnsSet(int32_t netId, const std::vector<std::string>& dnsAddrs) override;
    binder::Status tetherDnsList(std::vector<std::string>* dnsList) override;

    // Interface-related commands.
    binder::Status interfaceAddAddress(const std::string &ifName,
            const std::string &addrString, int prefixLength) override;
    binder::Status interfaceDelAddress(const std::string &ifName,
            const std::string &addrString, int prefixLength) override;
    binder::Status interfaceGetList(std::vector<std::string>* interfaceListResult) override;
    binder::Status interfaceGetCfg(const std::string& ifName,
                                   InterfaceConfigurationParcel* interfaceGetCfgResult) override;
    binder::Status interfaceSetCfg(const InterfaceConfigurationParcel& cfg) override;
    binder::Status interfaceSetIPv6PrivacyExtensions(const std::string& ifName,
                                                     bool enable) override;
    binder::Status interfaceClearAddrs(const std::string& ifName) override;
    binder::Status interfaceSetEnableIPv6(const std::string& ifName, bool enable) override;
    binder::Status interfaceSetMtu(const std::string& ifName, int32_t mtuValue) override;

    binder::Status getProcSysNet(int32_t ipversion, int32_t which, const std::string& ifname,
                                 const std::string& parameter, std::string* value) override;
    binder::Status setProcSysNet(int32_t ipversion, int32_t which, const std::string& ifname,
                                 const std::string& parameter, const std::string& value) override;

    binder::Status ipSecSetEncapSocketOwner(const os::ParcelFileDescriptor& socket, int newUid);

    binder::Status ipSecAllocateSpi(
            int32_t transformId,
            const std::string& localAddress,
            const std::string& remoteAddress,
            int32_t inSpi,
            int32_t* outSpi);

    binder::Status ipSecAddSecurityAssociation(
            int32_t transformId, int32_t mode, const std::string& sourceAddress,
            const std::string& destinationAddress, int32_t underlyingNetId, int32_t spi,
            int32_t markValue, int32_t markMask, const std::string& authAlgo,
            const std::vector<uint8_t>& authKey, int32_t authTruncBits,
            const std::string& cryptAlgo, const std::vector<uint8_t>& cryptKey,
            int32_t cryptTruncBits, const std::string& aeadAlgo,
            const std::vector<uint8_t>& aeadKey, int32_t aeadIcvBits, int32_t encapType,
            int32_t encapLocalPort, int32_t encapRemotePort, int32_t interfaceId);

    binder::Status ipSecDeleteSecurityAssociation(int32_t transformId,
                                                  const std::string& sourceAddress,
                                                  const std::string& destinationAddress,
                                                  int32_t spi, int32_t markValue, int32_t markMask,
                                                  int32_t interfaceId);

    binder::Status ipSecApplyTransportModeTransform(const os::ParcelFileDescriptor& socket,
                                                    int32_t transformId, int32_t direction,
                                                    const std::string& sourceAddress,
                                                    const std::string& destinationAddress,
                                                    int32_t spi);

    binder::Status ipSecRemoveTransportModeTransform(const os::ParcelFileDescriptor& socket);

    binder::Status ipSecAddSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                          int32_t direction, const std::string& tmplSrcAddress,
                                          const std::string& tmplDstAddress, int32_t spi,
                                          int32_t markValue, int32_t markMask, int32_t interfaceId);

    binder::Status ipSecUpdateSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                             int32_t direction, const std::string& tmplSrcAddress,
                                             const std::string& tmplDstAddress, int32_t spi,
                                             int32_t markValue, int32_t markMask,
                                             int32_t interfaceId);

    binder::Status ipSecDeleteSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                             int32_t direction, int32_t markValue, int32_t markMask,
                                             int32_t interfaceId);

    binder::Status trafficSwapActiveStatsMap() override;

    binder::Status ipSecAddTunnelInterface(const std::string& deviceName,
                                           const std::string& localAddress,
                                           const std::string& remoteAddress, int32_t iKey,
                                           int32_t oKey, int32_t interfaceId);

    binder::Status ipSecUpdateTunnelInterface(const std::string& deviceName,
                                              const std::string& localAddress,
                                              const std::string& remoteAddress, int32_t iKey,
                                              int32_t oKey, int32_t interfaceId);

    binder::Status ipSecRemoveTunnelInterface(const std::string& deviceName);

    // Idletimer-related commands
    binder::Status idletimerAddInterface(const std::string& ifName, int32_t timeout,
                                         const std::string& classLabel) override;
    binder::Status idletimerRemoveInterface(const std::string& ifName, int32_t timeout,
                                            const std::string& classLabel) override;

    // Strict-related commands
    binder::Status strictUidCleartextPenalty(int32_t uid, int32_t policyPenalty) override;

    // Clatd-related commands
    binder::Status clatdStart(const std::string& ifName, const std::string& nat64Prefix,
                              std::string* v6Address) override;
    binder::Status clatdStop(const std::string& ifName) override;

    // Ipfw-related commands
    binder::Status ipfwdEnabled(bool* status) override;
    binder::Status ipfwdGetRequesterList(std::vector<std::string>* requesterList) override;
    binder::Status ipfwdEnableForwarding(const std::string& requester) override;
    binder::Status ipfwdDisableForwarding(const std::string& requester) override;
    binder::Status ipfwdAddInterfaceForward(const std::string& fromIface,
                                            const std::string& toIface) override;
    binder::Status ipfwdRemoveInterfaceForward(const std::string& fromIface,
                                               const std::string& toIface) override;
    // Tether-forward-related commands
    binder::Status tetherAddForward(const std::string& intIface,
                                    const std::string& extIface) override;
    binder::Status tetherRemoveForward(const std::string& intIface,
                                       const std::string& extIface) override;

    // tcp_mem-config command
    binder::Status setTcpRWmemorySize(const std::string& rmemValues,
                                      const std::string& wmemValues) override;

    binder::Status registerUnsolicitedEventListener(
            const android::sp<android::net::INetdUnsolicitedEventListener>& listener) override;

    binder::Status getOemNetd(android::sp<android::IBinder>* listener) override;

  private:
    std::vector<uid_t> intsToUids(const std::vector<int32_t>& intUids);
    Permission convertPermission(int32_t permission);
    static FirewallRule parseRule(int32_t firewallRule);
    static ChildChain parseChildChain(int32_t childChain);
};

}  // namespace net
}  // namespace android

#endif  // _NETD_NATIVE_SERVICE_H_
