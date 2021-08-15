/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <binder/IPCThreadState.h>
#include <hidl/HidlTransportSupport.h>
#include "Controllers.h"
#include "Fwmark.h"
#include "NetdHwService.h"
#include "RouteController.h"
#include "TetherController.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::Void;

// Tells TetherController::enableForwarding who is requesting forwarding, so that TetherController
// can manage/refcount requests to enable forwarding by multiple parties such as the framework, this
// hwbinder interface, and the legacy "ndc ipfwd enable <requester>" commands.
namespace {
constexpr const char* FORWARDING_REQUESTER = "NetdHwService";
}

namespace android {
namespace net {

static StatusCode toHalStatus(int ret) {
    switch(ret) {
        case 0:
            return StatusCode::OK;
        case -EINVAL:
            return StatusCode::INVALID_ARGUMENTS;
        case -EEXIST:
            return StatusCode::ALREADY_EXISTS;
        case -ENONET:
            return StatusCode::NO_NETWORK;
        case -EPERM:
            return StatusCode::PERMISSION_DENIED;
        default:
            ALOGE("HAL service error=%d", ret);
            return StatusCode::UNKNOWN_ERROR;
    }
}

// Minimal service start.
status_t NetdHwService::start() {
    IPCThreadState::self()->disableBackgroundScheduling(true);
    // Usage of this HAL is anticipated to be thin; one thread should suffice.
    configureRpcThreadpool(1, false /* callerWillNotJoin */);
    // Register hardware service with ServiceManager.
    return INetd::registerAsService();
}

Return<void> NetdHwService::createOemNetwork(createOemNetwork_cb _hidl_cb) {
    unsigned netId;
    Permission permission = PERMISSION_SYSTEM;

    int ret = gCtls->netCtrl.createPhysicalOemNetwork(permission, &netId);

    Fwmark fwmark;
    fwmark.netId = netId;
    fwmark.explicitlySelected = true;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;
    _hidl_cb(netIdToNetHandle(netId), fwmark.intValue, toHalStatus(ret));

    return Void();
}

// Vendor code can only modify OEM networks. All other networks are managed by ConnectivityService.
#define RETURN_IF_NOT_OEM_NETWORK(netId)              \
    if (((netId) < NetworkController::MIN_OEM_ID) ||  \
        ((netId) > NetworkController::MAX_OEM_ID)) {  \
        return StatusCode::INVALID_ARGUMENTS;  \
    }

Return<StatusCode> NetdHwService::destroyOemNetwork(uint64_t netHandle) {
    unsigned netId = netHandleToNetId(netHandle);
    RETURN_IF_NOT_OEM_NETWORK(netId);

    return toHalStatus(gCtls->netCtrl.destroyNetwork(netId));
}

const char* maybeNullString(const hidl_string& nexthop) {
    // HIDL strings can't be null, but RouteController wants null instead of an empty string.
    const char* nh = nexthop.c_str();
    if (nh && !*nh) {
        nh = nullptr;
    }
    return nh;
}

Return <StatusCode> NetdHwService::addRouteToOemNetwork(
        uint64_t networkHandle, const hidl_string& ifname, const hidl_string& destination,
        const hidl_string& nexthop) {
    unsigned netId = netHandleToNetId(networkHandle);
    RETURN_IF_NOT_OEM_NETWORK(netId);

    return toHalStatus(gCtls->netCtrl.addRoute(netId, ifname.c_str(), destination.c_str(),
                                               maybeNullString(nexthop), false, INVALID_UID));
}

Return <StatusCode> NetdHwService::removeRouteFromOemNetwork(
        uint64_t networkHandle, const hidl_string& ifname, const hidl_string& destination,
        const hidl_string& nexthop) {
    unsigned netId = netHandleToNetId(networkHandle);
    RETURN_IF_NOT_OEM_NETWORK(netId);

    return toHalStatus(gCtls->netCtrl.removeRoute(netId, ifname.c_str(), destination.c_str(),
                                                  maybeNullString(nexthop), false, INVALID_UID));
}

Return <StatusCode> NetdHwService::addInterfaceToOemNetwork(uint64_t networkHandle,
                                                            const hidl_string& ifname) {
    unsigned netId = netHandleToNetId(networkHandle);
    RETURN_IF_NOT_OEM_NETWORK(netId);

    return toHalStatus(gCtls->netCtrl.addInterfaceToNetwork(netId, ifname.c_str()));
}

Return <StatusCode> NetdHwService::removeInterfaceFromOemNetwork(uint64_t networkHandle,
                                                                 const hidl_string& ifname) {
    unsigned netId = netHandleToNetId(networkHandle);
    RETURN_IF_NOT_OEM_NETWORK(netId);

    return toHalStatus(gCtls->netCtrl.removeInterfaceFromNetwork(netId, ifname.c_str()));
}

Return <StatusCode> NetdHwService::setIpForwardEnable(bool enable) {
    std::lock_guard _lock(gCtls->tetherCtrl.lock);

    bool success = enable ? gCtls->tetherCtrl.enableForwarding(FORWARDING_REQUESTER) :
                            gCtls->tetherCtrl.disableForwarding(FORWARDING_REQUESTER);

    return success ? StatusCode::OK : StatusCode::UNKNOWN_ERROR;
}

Return <StatusCode> NetdHwService::setForwardingBetweenInterfaces(
        const hidl_string& inputIfName, const hidl_string& outputIfName, bool enable) {
    std::lock_guard _lock(gCtls->tetherCtrl.lock);

    // TODO: check that one interface is an OEM interface and the other is another OEM interface, an
    // IPsec interface or a dummy interface.
    int ret = enable ? RouteController::enableTethering(inputIfName.c_str(), outputIfName.c_str()) :
                       RouteController::disableTethering(inputIfName.c_str(), outputIfName.c_str());
    return toHalStatus(ret);
}

}  // namespace net
}  // namespace android
