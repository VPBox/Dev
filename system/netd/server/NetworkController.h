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

#ifndef NETD_SERVER_NETWORK_CONTROLLER_H
#define NETD_SERVER_NETWORK_CONTROLLER_H

#include <android-base/thread_annotations.h>
#include <android/multinetwork.h>


#include "NetdConstants.h"
#include "Permission.h"
#include "android/net/INetd.h"
#include "netdutils/DumpWriter.h"

#include <sys/types.h>
#include <list>
#include <map>
#include <set>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct android_net_context;

namespace android {
namespace net {

typedef std::shared_lock<std::shared_mutex> ScopedRLock;
typedef std::lock_guard<std::shared_mutex> ScopedWLock;

constexpr uint32_t kHandleMagic = 0xcafed00d;

// Utility to convert from netId to net_handle_t. Doing this here as opposed to exporting
// from net.c as it may have NDK implications. Besides no conversion available in net.c for
// obtaining handle given netId.
// TODO: Use getnetidfromhandle() in net.c.
static inline unsigned netHandleToNetId(net_handle_t fromNetHandle) {
    const uint32_t k32BitMask = 0xffffffff;
    // This value MUST be kept in sync with the corresponding value in
    // the android.net.Network#getNetworkHandle() implementation.

    // Check for minimum acceptable version of the API in the low bits.
    if (fromNetHandle != NETWORK_UNSPECIFIED &&
        (fromNetHandle & k32BitMask) != kHandleMagic) {
        return 0;
    }

    return ((fromNetHandle >> (CHAR_BIT * sizeof(k32BitMask))) & k32BitMask);
}

// Utility to convert from nethandle to netid, keep in sync with getNetworkHandle
// in Network.java.
static inline net_handle_t netIdToNetHandle(unsigned fromNetId) {
    if (!fromNetId) {
        return NETWORK_UNSPECIFIED;
    }
    return (((net_handle_t)fromNetId << 32) | kHandleMagic);
}

class Network;
class UidRanges;
class VirtualNetwork;

/*
 * Keeps track of default, per-pid, and per-uid-range network selection, as
 * well as the mark associated with each network. Networks are identified
 * by netid. In all set* commands netid == 0 means "unspecified" and is
 * equivalent to clearing the mapping.
 */
class NetworkController {
public:
    // NetIds 52..98 are reserved for future use.
    static constexpr int MIN_OEM_ID = 1;
    static constexpr int MAX_OEM_ID = 50;
    static constexpr int LOCAL_NET_ID = INetd::LOCAL_NET_ID;
    static constexpr int DUMMY_NET_ID = 51;

    NetworkController();

    unsigned getDefaultNetwork() const;
    int setDefaultNetwork(unsigned netId) WARN_UNUSED_RESULT;

    // Sets |*netId| to an appropriate NetId to use for DNS for the given user. Call with |*netId|
    // set to a non-NETID_UNSET value if the user already has indicated a preference. Returns the
    // fwmark value to set on the socket when performing the DNS request.
    uint32_t getNetworkForDns(unsigned* netId, uid_t uid) const;
    unsigned getNetworkForUser(uid_t uid) const;
    unsigned getNetworkForConnect(uid_t uid) const;
    void getNetworkContext(unsigned netId, uid_t uid, struct android_net_context* netcontext) const;
    unsigned getNetworkForInterface(const char* interface) const;
    bool isVirtualNetwork(unsigned netId) const;

    int createPhysicalNetwork(unsigned netId, Permission permission) WARN_UNUSED_RESULT;
    int createPhysicalOemNetwork(Permission permission, unsigned *netId) WARN_UNUSED_RESULT;
    int createVirtualNetwork(unsigned netId, bool secure) WARN_UNUSED_RESULT;
    int destroyNetwork(unsigned netId) WARN_UNUSED_RESULT;

    int addInterfaceToNetwork(unsigned netId, const char* interface) WARN_UNUSED_RESULT;
    int removeInterfaceFromNetwork(unsigned netId, const char* interface) WARN_UNUSED_RESULT;

    Permission getPermissionForUser(uid_t uid) const;
    void setPermissionForUsers(Permission permission, const std::vector<uid_t>& uids);
    int checkUserNetworkAccess(uid_t uid, unsigned netId) const;
    int setPermissionForNetworks(Permission permission,
                                 const std::vector<unsigned>& netIds) WARN_UNUSED_RESULT;

    int addUsersToNetwork(unsigned netId, const UidRanges& uidRanges) WARN_UNUSED_RESULT;
    int removeUsersFromNetwork(unsigned netId, const UidRanges& uidRanges) WARN_UNUSED_RESULT;

    // |nexthop| can be NULL (to indicate a directly-connected route), "unreachable" (to indicate a
    // route that's blocked), "throw" (to indicate the lack of a match), or a regular IP address.
    //
    // Routes are added to tables determined by the interface, so only |interface| is actually used.
    // |netId| is given only to sanity check that the interface has the correct netId.
    int addRoute(unsigned netId, const char* interface, const char* destination,
                 const char* nexthop, bool legacy, uid_t uid) WARN_UNUSED_RESULT;
    int removeRoute(unsigned netId, const char* interface, const char* destination,
                    const char* nexthop, bool legacy, uid_t uid) WARN_UNUSED_RESULT;

    // Notes that the specified address has appeared on the specified interface.
    void addInterfaceAddress(unsigned ifIndex, const char* address);
    // Notes that the specified address has been removed from the specified interface.
    // Returns true if we should destroy sockets on this address.
    bool removeInterfaceAddress(unsigned ifIndex, const char* address);

    bool canProtect(uid_t uid) const;
    void allowProtect(const std::vector<uid_t>& uids);
    void denyProtect(const std::vector<uid_t>& uids);

    void dump(netdutils::DumpWriter& dw);

  private:
    bool isValidNetworkLocked(unsigned netId) const;
    Network* getNetworkLocked(unsigned netId) const;
    uint32_t getNetworkForDnsLocked(unsigned* netId, uid_t uid) const;
    unsigned getNetworkForUserLocked(uid_t uid) const;
    unsigned getNetworkForConnectLocked(uid_t uid) const;
    unsigned getNetworkForInterfaceLocked(const char* interface) const;
    bool canProtectLocked(uid_t uid) const;
    bool isVirtualNetworkLocked(unsigned netId) const;
    VirtualNetwork* getVirtualNetworkForUserLocked(uid_t uid) const;
    Permission getPermissionForUserLocked(uid_t uid) const;
    int checkUserNetworkAccessLocked(uid_t uid, unsigned netId) const;
    int createPhysicalNetworkLocked(unsigned netId, Permission permission) WARN_UNUSED_RESULT;

    int modifyRoute(unsigned netId, const char* interface, const char* destination,
                    const char* nexthop, bool add, bool legacy, uid_t uid) WARN_UNUSED_RESULT;
    int modifyFallthroughLocked(unsigned vpnNetId, bool add) WARN_UNUSED_RESULT;
    void updateTcpSocketMonitorPolling();

    class DelegateImpl;
    DelegateImpl* const mDelegateImpl;

    // mRWLock guards all accesses to mDefaultNetId, mNetworks, mUsers, mProtectableUsers,
    // mIfindexToLastNetId and mAddressToIfindices.
    mutable std::shared_mutex mRWLock;
    unsigned mDefaultNetId;
    std::map<unsigned, Network*> mNetworks;  // Map keys are NetIds.
    std::map<uid_t, Permission> mUsers;
    std::set<uid_t> mProtectableUsers;
    // Map interface (ifIndex) to its current NetId, or the last NetId if the interface was removed
    // from the network and not added to another network. This state facilitates the interface to
    // NetId lookup during RTM_DELADDR (NetworkController::removeInterfaceAddress), when the
    // interface in question might already have been removed from the network.
    // An interface is added to this map when it is added to a network and removed from this map
    // when its network is destroyed.
    std::unordered_map<unsigned, unsigned> mIfindexToLastNetId;
    // Map IP address to the list of active interfaces (ifIndex) that have that address.
    // Also contains IP addresses configured on interfaces that have not been added to any network.
    // TODO: Does not track IP addresses present when netd is started or restarts after a crash.
    // This is not a problem for its intended use (tracking IP addresses on VPN interfaces), but
    // we should fix it.
    std::unordered_map<std::string, std::unordered_set<unsigned>> mAddressToIfindices;

};

}  // namespace net
}  // namespace android

#endif  // NETD_SERVER_NETWORK_CONTROLLER_H
