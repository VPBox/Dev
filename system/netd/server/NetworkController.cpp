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

// THREAD-SAFETY
// -------------
// The methods in this file are called from multiple threads (from CommandListener, FwmarkServer
// and DnsProxyListener). So, all accesses to shared state are guarded by a lock.
//
// Public functions accessible by external callers should be thread-safe and are responsible for
// acquiring the lock. Private functions in this file should call xxxLocked() methods and access
// internal state directly.

#define LOG_TAG "Netd"

#include "NetworkController.h"

#include <android-base/strings.h>
#include <cutils/misc.h>  // FIRST_APPLICATION_UID
#include <netd_resolv/resolv.h>
#include <netd_resolv/resolv_stub.h>
#include "log/log.h"

#include "Controllers.h"
#include "DummyNetwork.h"
#include "Fwmark.h"
#include "LocalNetwork.h"
#include "PhysicalNetwork.h"
#include "RouteController.h"
#include "VirtualNetwork.h"
#include "netdutils/DumpWriter.h"
#include "netid_client.h"

#define DBG 0

using android::netdutils::DumpWriter;

namespace android {
namespace net {

namespace {

// Keep these in sync with ConnectivityService.java.
const unsigned MIN_NET_ID = 100;
const unsigned MAX_NET_ID = 65535;

}  // namespace

// All calls to methods here are made while holding a write lock on mRWLock.
// They are mostly not called directly from this class, but from methods in PhysicalNetwork.cpp.
// However, we're the only user of that class, so all calls to those methods come from here and are
// made under lock.
// For example, PhysicalNetwork::setPermission ends up calling addFallthrough and removeFallthrough,
// but it's only called from here under lock (specifically, from createPhysicalNetworkLocked and
// setPermissionForNetworks).
// TODO: use std::mutex and GUARDED_BY instead of manual inspection.
class NetworkController::DelegateImpl : public PhysicalNetwork::Delegate {
public:
    explicit DelegateImpl(NetworkController* networkController);
    virtual ~DelegateImpl();

    int modifyFallthrough(unsigned vpnNetId, const std::string& physicalInterface,
                          Permission permission, bool add) WARN_UNUSED_RESULT;

private:
    int addFallthrough(const std::string& physicalInterface,
                       Permission permission) override WARN_UNUSED_RESULT;
    int removeFallthrough(const std::string& physicalInterface,
                          Permission permission) override WARN_UNUSED_RESULT;

    int modifyFallthrough(const std::string& physicalInterface, Permission permission,
                          bool add) WARN_UNUSED_RESULT;

    NetworkController* const mNetworkController;
};

NetworkController::DelegateImpl::DelegateImpl(NetworkController* networkController) :
        mNetworkController(networkController) {
}

NetworkController::DelegateImpl::~DelegateImpl() {
}

int NetworkController::DelegateImpl::modifyFallthrough(unsigned vpnNetId,
                                                       const std::string& physicalInterface,
                                                       Permission permission, bool add) {
    if (add) {
        if (int ret = RouteController::addVirtualNetworkFallthrough(vpnNetId,
                                                                    physicalInterface.c_str(),
                                                                    permission)) {
            ALOGE("failed to add fallthrough to %s for VPN netId %u", physicalInterface.c_str(),
                  vpnNetId);
            return ret;
        }
    } else {
        if (int ret = RouteController::removeVirtualNetworkFallthrough(vpnNetId,
                                                                       physicalInterface.c_str(),
                                                                       permission)) {
            ALOGE("failed to remove fallthrough to %s for VPN netId %u", physicalInterface.c_str(),
                  vpnNetId);
            return ret;
        }
    }
    return 0;
}

int NetworkController::DelegateImpl::addFallthrough(const std::string& physicalInterface,
                                                    Permission permission) {
    return modifyFallthrough(physicalInterface, permission, true);
}

int NetworkController::DelegateImpl::removeFallthrough(const std::string& physicalInterface,
                                                       Permission permission) {
    return modifyFallthrough(physicalInterface, permission, false);
}

int NetworkController::DelegateImpl::modifyFallthrough(const std::string& physicalInterface,
                                                       Permission permission, bool add) {
    for (const auto& entry : mNetworkController->mNetworks) {
        if (entry.second->getType() == Network::VIRTUAL) {
            if (int ret = modifyFallthrough(entry.first, physicalInterface, permission, add)) {
                return ret;
            }
        }
    }
    return 0;
}

NetworkController::NetworkController() :
        mDelegateImpl(new NetworkController::DelegateImpl(this)), mDefaultNetId(NETID_UNSET),
        mProtectableUsers({AID_VPN}) {
    mNetworks[LOCAL_NET_ID] = new LocalNetwork(LOCAL_NET_ID);
    mNetworks[DUMMY_NET_ID] = new DummyNetwork(DUMMY_NET_ID);
}

unsigned NetworkController::getDefaultNetwork() const {
    ScopedRLock lock(mRWLock);
    return mDefaultNetId;
}

int NetworkController::setDefaultNetwork(unsigned netId) {
    ScopedWLock lock(mRWLock);

    if (netId == mDefaultNetId) {
        return 0;
    }

    if (netId != NETID_UNSET) {
        Network* network = getNetworkLocked(netId);
        if (!network) {
            ALOGE("no such netId %u", netId);
            return -ENONET;
        }
        if (network->getType() != Network::PHYSICAL) {
            ALOGE("cannot set default to non-physical network with netId %u", netId);
            return -EINVAL;
        }
        if (int ret = static_cast<PhysicalNetwork*>(network)->addAsDefault()) {
            return ret;
        }
    }

    if (mDefaultNetId != NETID_UNSET) {
        Network* network = getNetworkLocked(mDefaultNetId);
        if (!network || network->getType() != Network::PHYSICAL) {
            ALOGE("cannot find previously set default network with netId %u", mDefaultNetId);
            return -ESRCH;
        }
        if (int ret = static_cast<PhysicalNetwork*>(network)->removeAsDefault()) {
            return ret;
        }
    }

    mDefaultNetId = netId;
    return 0;
}

uint32_t NetworkController::getNetworkForDnsLocked(unsigned* netId, uid_t uid) const {
    Fwmark fwmark;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;

    // Common case: there is no VPN that applies to the user, and the query did not specify a netId.
    // Therefore, it is safe to set the explicit bit on this query and skip all the complex logic
    // below. While this looks like a special case, it is actually the one that handles the vast
    // majority of DNS queries.
    // TODO: untangle this code.
    if (*netId == NETID_UNSET && getVirtualNetworkForUserLocked(uid) == nullptr) {
        *netId = mDefaultNetId;
        fwmark.netId = *netId;
        fwmark.explicitlySelected = true;
        return fwmark.intValue;
    }

    if (checkUserNetworkAccessLocked(uid, *netId) == 0) {
        // If a non-zero NetId was explicitly specified, and the user has permission for that
        // network, use that network's DNS servers. (possibly falling through the to the default
        // network if the VPN doesn't provide a route to them).
        fwmark.explicitlySelected = true;

        // If the network is a VPN and it doesn't have DNS servers, use the default network's DNS
        // servers (through the default network). Otherwise, the query is guaranteed to fail.
        // http://b/29498052
        Network *network = getNetworkLocked(*netId);
        if (network && network->getType() == Network::VIRTUAL &&
            !RESOLV_STUB.resolv_has_nameservers(*netId)) {
            *netId = mDefaultNetId;
        }
    } else {
        // If the user is subject to a VPN and the VPN provides DNS servers, use those servers
        // (possibly falling through to the default network if the VPN doesn't provide a route to
        // them). Otherwise, use the default network's DNS servers.
        // TODO: Consider if we should set the explicit bit here.
        VirtualNetwork* virtualNetwork = getVirtualNetworkForUserLocked(uid);
        if (virtualNetwork && RESOLV_STUB.resolv_has_nameservers(virtualNetwork->getNetId())) {
            *netId = virtualNetwork->getNetId();
        } else {
            // TODO: return an error instead of silently doing the DNS lookup on the wrong network.
            // http://b/27560555
            *netId = mDefaultNetId;
        }
    }
    fwmark.netId = *netId;
    return fwmark.intValue;
}

uint32_t NetworkController::getNetworkForDns(unsigned* netId, uid_t uid) const {
    ScopedRLock lock(mRWLock);
    return getNetworkForDnsLocked(netId, uid);
}

// Returns the NetId that a given UID would use if no network is explicitly selected. Specifically,
// the VPN that applies to the UID if any; otherwise, the default network.
unsigned NetworkController::getNetworkForUser(uid_t uid) const {
    ScopedRLock lock(mRWLock);
    if (VirtualNetwork* virtualNetwork = getVirtualNetworkForUserLocked(uid)) {
        return virtualNetwork->getNetId();
    }
    return mDefaultNetId;
}

// Returns the NetId that will be set when a socket connect()s. This is the bypassable VPN that
// applies to the user if any; otherwise, the default network.
//
// In general, we prefer to always set the default network's NetId in connect(), so that if the VPN
// is a split-tunnel and disappears later, the socket continues working (since the default network's
// NetId is still valid). Secure VPNs will correctly grab the socket's traffic since they have a
// high-priority routing rule that doesn't care what NetId the socket has.
//
// But bypassable VPNs have a very low priority rule, so we need to mark the socket with the
// bypassable VPN's NetId if we expect it to get any traffic at all. If the bypassable VPN is a
// split-tunnel, that's okay, because we have fallthrough rules that will direct the fallthrough
// traffic to the default network. But it does mean that if the bypassable VPN goes away (and thus
// the fallthrough rules also go away), the socket that used to fallthrough to the default network
// will stop working.
unsigned NetworkController::getNetworkForConnectLocked(uid_t uid) const {
    VirtualNetwork* virtualNetwork = getVirtualNetworkForUserLocked(uid);
    if (virtualNetwork && !virtualNetwork->isSecure()) {
        return virtualNetwork->getNetId();
    }
    return mDefaultNetId;
}

unsigned NetworkController::getNetworkForConnect(uid_t uid) const {
    ScopedRLock lock(mRWLock);
    return getNetworkForConnectLocked(uid);
}

void NetworkController::getNetworkContext(
        unsigned netId, uid_t uid, struct android_net_context* netcontext) const {
    ScopedRLock lock(mRWLock);

    struct android_net_context nc = {
            .app_netid = netId,
            .app_mark = MARK_UNSET,
            .dns_netid = netId,
            .dns_mark = MARK_UNSET,
            .uid = uid,
    };

    // |netId| comes directly (via dnsproxyd) from the value returned by netIdForResolv() in the
    // client process. This value is nonzero iff.:
    //
    // 1. The app specified a netid/nethandle to a DNS resolution method such as:
    //        - [Java] android.net.Network#getAllByName()
    //        - [C/++] android_getaddrinfofornetwork()
    // 2. The app specified a netid/nethandle to be used as a process default via:
    //        - [Java] android.net.ConnectivityManager#bindProcessToNetwork()
    //        - [C/++] android_setprocnetwork()
    // 3. The app called android.net.ConnectivityManager#startUsingNetworkFeature().
    //
    // In all these cases (with the possible exception of #3), the right thing to do is to treat
    // such cases as explicitlySelected.
    const bool explicitlySelected = (nc.app_netid != NETID_UNSET);
    if (!explicitlySelected) {
        nc.app_netid = getNetworkForConnectLocked(uid);
    }

    Fwmark fwmark;
    fwmark.netId = nc.app_netid;
    fwmark.explicitlySelected = explicitlySelected;
    fwmark.protectedFromVpn = explicitlySelected && canProtectLocked(uid);
    fwmark.permission = getPermissionForUserLocked(uid);
    nc.app_mark = fwmark.intValue;

    nc.dns_mark = getNetworkForDnsLocked(&(nc.dns_netid), uid);

    if (DBG) {
        ALOGD("app_netid:0x%x app_mark:0x%x dns_netid:0x%x dns_mark:0x%x uid:%d",
              nc.app_netid, nc.app_mark, nc.dns_netid, nc.dns_mark, uid);
    }

    if (netcontext) {
        *netcontext = nc;
    }
}

unsigned NetworkController::getNetworkForInterfaceLocked(const char* interface) const {
    for (const auto& entry : mNetworks) {
        if (entry.second->hasInterface(interface)) {
            return entry.first;
        }
    }
    return NETID_UNSET;
}

unsigned NetworkController::getNetworkForInterface(const char* interface) const {
    ScopedRLock lock(mRWLock);
    return getNetworkForInterfaceLocked(interface);
}

bool NetworkController::isVirtualNetwork(unsigned netId) const {
    ScopedRLock lock(mRWLock);
    return isVirtualNetworkLocked(netId);
}

bool NetworkController::isVirtualNetworkLocked(unsigned netId) const {
    Network* network = getNetworkLocked(netId);
    return network && network->getType() == Network::VIRTUAL;
}

int NetworkController::createPhysicalNetworkLocked(unsigned netId, Permission permission) {
    if (!((MIN_NET_ID <= netId && netId <= MAX_NET_ID) ||
          (MIN_OEM_ID <= netId && netId <= MAX_OEM_ID))) {
        ALOGE("invalid netId %u", netId);
        return -EINVAL;
    }

    if (isValidNetworkLocked(netId)) {
        ALOGE("duplicate netId %u", netId);
        return -EEXIST;
    }

    PhysicalNetwork* physicalNetwork = new PhysicalNetwork(netId, mDelegateImpl);
    if (int ret = physicalNetwork->setPermission(permission)) {
        ALOGE("inconceivable! setPermission cannot fail on an empty network");
        delete physicalNetwork;
        return ret;
    }

    mNetworks[netId] = physicalNetwork;

    updateTcpSocketMonitorPolling();

    return 0;
}

int NetworkController::createPhysicalNetwork(unsigned netId, Permission permission) {
    ScopedWLock lock(mRWLock);
    return createPhysicalNetworkLocked(netId, permission);
}

int NetworkController::createPhysicalOemNetwork(Permission permission, unsigned *pNetId) {
    if (pNetId == nullptr) {
        return -EINVAL;
    }

    ScopedWLock lock(mRWLock);
    for (*pNetId = MIN_OEM_ID; *pNetId <= MAX_OEM_ID; (*pNetId)++) {
        if (!isValidNetworkLocked(*pNetId)) {
            break;
        }
    }

    if (*pNetId > MAX_OEM_ID) {
        ALOGE("No free network ID");
        *pNetId = 0;
        return -ENONET;
    }

    int ret = createPhysicalNetworkLocked(*pNetId, permission);
    if (ret) {
        *pNetId = 0;
    }

    return ret;
}

int NetworkController::createVirtualNetwork(unsigned netId, bool secure) {
    ScopedWLock lock(mRWLock);

    if (!(MIN_NET_ID <= netId && netId <= MAX_NET_ID)) {
        ALOGE("invalid netId %u", netId);
        return -EINVAL;
    }

    if (isValidNetworkLocked(netId)) {
        ALOGE("duplicate netId %u", netId);
        return -EEXIST;
    }

    if (int ret = modifyFallthroughLocked(netId, true)) {
        return ret;
    }
    mNetworks[netId] = new VirtualNetwork(netId, secure);
    return 0;
}

int NetworkController::destroyNetwork(unsigned netId) {
    ScopedWLock lock(mRWLock);

    if (netId == LOCAL_NET_ID) {
        ALOGE("cannot destroy local network");
        return -EINVAL;
    }
    if (!isValidNetworkLocked(netId)) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }

    // TODO: ioctl(SIOCKILLADDR, ...) to kill all sockets on the old network.

    Network* network = getNetworkLocked(netId);

    // If we fail to destroy a network, things will get stuck badly. Therefore, unlike most of the
    // other network code, ignore failures and attempt to clear out as much state as possible, even
    // if we hit an error on the way. Return the first error that we see.
    int ret = network->clearInterfaces();

    if (mDefaultNetId == netId) {
        if (int err = static_cast<PhysicalNetwork*>(network)->removeAsDefault()) {
            ALOGE("inconceivable! removeAsDefault cannot fail on an empty network");
            if (!ret) {
                ret = err;
            }
        }
        mDefaultNetId = NETID_UNSET;
    } else if (network->getType() == Network::VIRTUAL) {
        if (int err = modifyFallthroughLocked(netId, false)) {
            if (!ret) {
                ret = err;
            }
        }
    }
    mNetworks.erase(netId);
    delete network;

    for (auto iter = mIfindexToLastNetId.begin(); iter != mIfindexToLastNetId.end();) {
        if (iter->second == netId) {
            iter = mIfindexToLastNetId.erase(iter);
        } else {
            ++iter;
        }
    }

    updateTcpSocketMonitorPolling();

    return ret;
}

int NetworkController::addInterfaceToNetwork(unsigned netId, const char* interface) {
    ScopedWLock lock(mRWLock);

    if (!isValidNetworkLocked(netId)) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }

    unsigned existingNetId = getNetworkForInterfaceLocked(interface);
    if (existingNetId != NETID_UNSET && existingNetId != netId) {
        ALOGE("interface %s already assigned to netId %u", interface, existingNetId);
        return -EBUSY;
    }
    if (int ret = getNetworkLocked(netId)->addInterface(interface)) {
        return ret;
    }

    int ifIndex = RouteController::getIfIndex(interface);
    if (ifIndex) {
        mIfindexToLastNetId[ifIndex] = netId;
    } else {
        // Cannot happen, since addInterface() above will have failed.
        ALOGE("inconceivable! added interface %s with no index", interface);
    }
    return 0;
}

int NetworkController::removeInterfaceFromNetwork(unsigned netId, const char* interface) {
    ScopedWLock lock(mRWLock);

    if (!isValidNetworkLocked(netId)) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }

    return getNetworkLocked(netId)->removeInterface(interface);
}

Permission NetworkController::getPermissionForUser(uid_t uid) const {
    ScopedRLock lock(mRWLock);
    return getPermissionForUserLocked(uid);
}

void NetworkController::setPermissionForUsers(Permission permission,
                                              const std::vector<uid_t>& uids) {
    ScopedWLock lock(mRWLock);
    for (uid_t uid : uids) {
        mUsers[uid] = permission;
    }
}

int NetworkController::checkUserNetworkAccess(uid_t uid, unsigned netId) const {
    ScopedRLock lock(mRWLock);
    return checkUserNetworkAccessLocked(uid, netId);
}

int NetworkController::setPermissionForNetworks(Permission permission,
                                                const std::vector<unsigned>& netIds) {
    ScopedWLock lock(mRWLock);
    for (unsigned netId : netIds) {
        Network* network = getNetworkLocked(netId);
        if (!network) {
            ALOGE("no such netId %u", netId);
            return -ENONET;
        }
        if (network->getType() != Network::PHYSICAL) {
            ALOGE("cannot set permissions on non-physical network with netId %u", netId);
            return -EINVAL;
        }

        if (int ret = static_cast<PhysicalNetwork*>(network)->setPermission(permission)) {
            return ret;
        }
    }
    return 0;
}

int NetworkController::addUsersToNetwork(unsigned netId, const UidRanges& uidRanges) {
    ScopedWLock lock(mRWLock);
    Network* network = getNetworkLocked(netId);
    if (!network) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }
    if (network->getType() != Network::VIRTUAL) {
        ALOGE("cannot add users to non-virtual network with netId %u", netId);
        return -EINVAL;
    }
    if (int ret = static_cast<VirtualNetwork*>(network)->addUsers(uidRanges, mProtectableUsers)) {
        return ret;
    }
    return 0;
}

int NetworkController::removeUsersFromNetwork(unsigned netId, const UidRanges& uidRanges) {
    ScopedWLock lock(mRWLock);
    Network* network = getNetworkLocked(netId);
    if (!network) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }
    if (network->getType() != Network::VIRTUAL) {
        ALOGE("cannot remove users from non-virtual network with netId %u", netId);
        return -EINVAL;
    }
    if (int ret = static_cast<VirtualNetwork*>(network)->removeUsers(uidRanges,
                                                                     mProtectableUsers)) {
        return ret;
    }
    return 0;
}

int NetworkController::addRoute(unsigned netId, const char* interface, const char* destination,
                                const char* nexthop, bool legacy, uid_t uid) {
    return modifyRoute(netId, interface, destination, nexthop, true, legacy, uid);
}

int NetworkController::removeRoute(unsigned netId, const char* interface, const char* destination,
                                   const char* nexthop, bool legacy, uid_t uid) {
    return modifyRoute(netId, interface, destination, nexthop, false, legacy, uid);
}

void NetworkController::addInterfaceAddress(unsigned ifIndex, const char* address) {
    ScopedWLock lock(mRWLock);
    if (ifIndex == 0) {
        ALOGE("Attempting to add address %s without ifindex", address);
        return;
    }
    mAddressToIfindices[address].insert(ifIndex);
}

// Returns whether we should call SOCK_DESTROY on the removed address.
bool NetworkController::removeInterfaceAddress(unsigned ifindex, const char* address) {
    ScopedWLock lock(mRWLock);
    // First, update mAddressToIfindices map
    auto ifindicesIter = mAddressToIfindices.find(address);
    if (ifindicesIter == mAddressToIfindices.end()) {
        ALOGE("Removing unknown address %s from ifindex %u", address, ifindex);
        return true;
    }
    std::unordered_set<unsigned>& ifindices = ifindicesIter->second;
    if (ifindices.erase(ifindex) > 0) {
        if (ifindices.size() == 0) {
            mAddressToIfindices.erase(ifindicesIter);  // Invalidates ifindices
            // The address is no longer configured on any interface.
            return true;
        }
    } else {
        ALOGE("No record of address %s on interface %u", address, ifindex);
        return true;
    }
    // Then, check for VPN handover condition
    if (mIfindexToLastNetId.find(ifindex) == mIfindexToLastNetId.end()) {
        ALOGE("Interface index %u was never in a currently-connected netId", ifindex);
        return true;
    }
    unsigned lastNetId = mIfindexToLastNetId[ifindex];
    for (unsigned idx : ifindices) {
        unsigned activeNetId = mIfindexToLastNetId[idx];
        // If this IP address is still assigned to another interface in the same network,
        // then we don't need to destroy sockets on it because they are likely still valid.
        // For now we do this only on VPNs.
        // TODO: evaluate extending this to all network types.
        if (lastNetId == activeNetId && isVirtualNetworkLocked(activeNetId)) {
            return false;
        }
    }
    return true;
}

bool NetworkController::canProtectLocked(uid_t uid) const {
    return ((getPermissionForUserLocked(uid) & PERMISSION_SYSTEM) == PERMISSION_SYSTEM) ||
           mProtectableUsers.find(uid) != mProtectableUsers.end();
}

bool NetworkController::canProtect(uid_t uid) const {
    ScopedRLock lock(mRWLock);
    return canProtectLocked(uid);
}

void NetworkController::allowProtect(const std::vector<uid_t>& uids) {
    ScopedWLock lock(mRWLock);
    mProtectableUsers.insert(uids.begin(), uids.end());
}

void NetworkController::denyProtect(const std::vector<uid_t>& uids) {
    ScopedWLock lock(mRWLock);
    for (uid_t uid : uids) {
        mProtectableUsers.erase(uid);
    }
}

void NetworkController::dump(DumpWriter& dw) {
    ScopedRLock lock(mRWLock);

    dw.incIndent();
    dw.println("NetworkController");

    dw.incIndent();
    dw.println("Default network: %u", mDefaultNetId);

    dw.blankline();
    dw.println("Networks:");
    dw.incIndent();
    for (const auto& i : mNetworks) {
        Network* network = i.second;
        dw.println(network->toString());
        if (network->getType() == Network::PHYSICAL) {
            dw.incIndent();
            Permission permission = reinterpret_cast<PhysicalNetwork*>(network)->getPermission();
            dw.println("Required permission: %s", permissionToName(permission));
            dw.decIndent();
        }
        dw.blankline();
    }
    dw.decIndent();

    dw.blankline();
    dw.println("Interface <-> last network map:");
    dw.incIndent();
    for (const auto& i : mIfindexToLastNetId) {
        dw.println("Ifindex: %u NetId: %u", i.first, i.second);
    }
    dw.decIndent();

    dw.blankline();
    dw.println("Interface addresses:");
    dw.incIndent();
    for (const auto& i : mAddressToIfindices) {
        dw.println("address: %s ifindices: [%s]", i.first.c_str(),
                android::base::Join(i.second, ", ").c_str());
    }
    dw.decIndent();

    dw.decIndent();

    dw.decIndent();
}

bool NetworkController::isValidNetworkLocked(unsigned netId) const {
    return getNetworkLocked(netId);
}

Network* NetworkController::getNetworkLocked(unsigned netId) const {
    auto iter = mNetworks.find(netId);
    return iter == mNetworks.end() ? nullptr : iter->second;
}

VirtualNetwork* NetworkController::getVirtualNetworkForUserLocked(uid_t uid) const {
    for (const auto& entry : mNetworks) {
        if (entry.second->getType() == Network::VIRTUAL) {
            VirtualNetwork* virtualNetwork = static_cast<VirtualNetwork*>(entry.second);
            if (virtualNetwork->appliesToUser(uid)) {
                return virtualNetwork;
            }
        }
    }
    return nullptr;
}

Permission NetworkController::getPermissionForUserLocked(uid_t uid) const {
    auto iter = mUsers.find(uid);
    if (iter != mUsers.end()) {
        return iter->second;
    }
    return uid < FIRST_APPLICATION_UID ? PERMISSION_SYSTEM : PERMISSION_NONE;
}

int NetworkController::checkUserNetworkAccessLocked(uid_t uid, unsigned netId) const {
    Network* network = getNetworkLocked(netId);
    if (!network) {
        return -ENONET;
    }

    // If uid is INVALID_UID, this likely means that we were unable to retrieve the UID of the peer
    // (using SO_PEERCRED). Be safe and deny access to the network, even if it's valid.
    if (uid == INVALID_UID) {
        return -EREMOTEIO;
    }
    Permission userPermission = getPermissionForUserLocked(uid);
    if ((userPermission & PERMISSION_SYSTEM) == PERMISSION_SYSTEM) {
        return 0;
    }
    if (network->getType() == Network::VIRTUAL) {
        return static_cast<VirtualNetwork*>(network)->appliesToUser(uid) ? 0 : -EPERM;
    }
    VirtualNetwork* virtualNetwork = getVirtualNetworkForUserLocked(uid);
    if (virtualNetwork && virtualNetwork->isSecure() &&
            mProtectableUsers.find(uid) == mProtectableUsers.end()) {
        return -EPERM;
    }
    Permission networkPermission = static_cast<PhysicalNetwork*>(network)->getPermission();
    return ((userPermission & networkPermission) == networkPermission) ? 0 : -EACCES;
}

int NetworkController::modifyRoute(unsigned netId, const char* interface, const char* destination,
                                   const char* nexthop, bool add, bool legacy, uid_t uid) {
    ScopedRLock lock(mRWLock);

    if (!isValidNetworkLocked(netId)) {
        ALOGE("no such netId %u", netId);
        return -ENONET;
    }
    unsigned existingNetId = getNetworkForInterfaceLocked(interface);
    if (existingNetId == NETID_UNSET) {
        ALOGE("interface %s not assigned to any netId", interface);
        return -ENODEV;
    }
    if (existingNetId != netId) {
        ALOGE("interface %s assigned to netId %u, not %u", interface, existingNetId, netId);
        return -ENOENT;
    }

    RouteController::TableType tableType;
    if (netId == LOCAL_NET_ID) {
        tableType = RouteController::LOCAL_NETWORK;
    } else if (legacy) {
        if ((getPermissionForUserLocked(uid) & PERMISSION_SYSTEM) == PERMISSION_SYSTEM) {
            tableType = RouteController::LEGACY_SYSTEM;
        } else {
            tableType = RouteController::LEGACY_NETWORK;
        }
    } else {
        tableType = RouteController::INTERFACE;
    }

    return add ? RouteController::addRoute(interface, destination, nexthop, tableType) :
                 RouteController::removeRoute(interface, destination, nexthop, tableType);
}

int NetworkController::modifyFallthroughLocked(unsigned vpnNetId, bool add) {
    if (mDefaultNetId == NETID_UNSET) {
        return 0;
    }
    Network* network = getNetworkLocked(mDefaultNetId);
    if (!network) {
        ALOGE("cannot find previously set default network with netId %u", mDefaultNetId);
        return -ESRCH;
    }
    if (network->getType() != Network::PHYSICAL) {
        ALOGE("inconceivable! default network must be a physical network");
        return -EINVAL;
    }
    Permission permission = static_cast<PhysicalNetwork*>(network)->getPermission();
    for (const auto& physicalInterface : network->getInterfaces()) {
        if (int ret = mDelegateImpl->modifyFallthrough(vpnNetId, physicalInterface, permission,
                                                       add)) {
            return ret;
        }
    }
    return 0;
}

void NetworkController::updateTcpSocketMonitorPolling() {
    bool physicalNetworkExists = false;
    for (const auto& entry : mNetworks) {
        const auto& network = entry.second;
        if (network->getType() == Network::PHYSICAL && network->getNetId() >= MIN_NET_ID) {
            physicalNetworkExists = true;
            break;
        }
    }

    if (physicalNetworkExists) {
        android::net::gCtls->tcpSocketMonitor.resumePolling();
    } else {
        android::net::gCtls->tcpSocketMonitor.suspendPolling();
    }
}

}  // namespace net
}  // namespace android
