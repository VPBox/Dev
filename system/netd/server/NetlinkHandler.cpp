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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LOG_TAG "Netd"

#include <log/log.h>

#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <netutils/ifc.h>
#include <sysutils/NetlinkEvent.h>
#include "Controllers.h"
#include "NetlinkHandler.h"
#include "NetlinkManager.h"
#include "SockDiag.h"

#include <charconv>

#define BINDER_RETRY(exp)                                                       \
    ({                                                                          \
        bool res = true;                                                        \
        for (int attempt = 0; /*nop*/; ++attempt) {                             \
            auto _rc = (exp);                                                   \
            if (_rc.exceptionCode() == binder::Status::EX_TRANSACTION_FAILED && \
                attempt < RETRY_ATTEMPTS) {                                     \
                usleep(RETRY_INTERVAL_MICRO_S);                                 \
            } else {                                                            \
                res = _rc.isOk();                                               \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        res;                                                                    \
    })

#define LOG_EVENT_FUNC(retry, func, ...)                                                    \
    do {                                                                                    \
        const auto listenerMap = gCtls->eventReporter.getNetdUnsolicitedEventListenerMap(); \
        for (auto& listener : listenerMap) {                                                \
            auto entry = gUnsolicitedLog.newEntry().function(#func).args(__VA_ARGS__);      \
            if (retry(listener.first->func(__VA_ARGS__))) {                                 \
                gUnsolicitedLog.log(entry.withAutomaticDuration());                         \
            }                                                                               \
        }                                                                                   \
    } while (0)

namespace android {
namespace net {

constexpr int RETRY_ATTEMPTS = 2;
constexpr int RETRY_INTERVAL_MICRO_S = 100000;

NetlinkHandler::NetlinkHandler(NetlinkManager *nm, int listenerSocket,
                               int format) :
                        NetlinkListener(listenerSocket, format) {
    mNm = nm;
}

NetlinkHandler::~NetlinkHandler() {
}

int NetlinkHandler::start() {
    return this->startListener();
}

int NetlinkHandler::stop() {
    return this->stopListener();
}

static long parseIfIndex(const char* ifIndex) {
    if (ifIndex == nullptr) {
        return 0;
    }
    long ifaceIndex = strtol(ifIndex, nullptr, 10);
    // strtol returns 0 on error, which is fine because 0 is not a valid ifindex.
    if (errno == ERANGE && (ifaceIndex == LONG_MAX || ifaceIndex == LONG_MIN)) {
        return 0;
    }
    return ifaceIndex;
}

void NetlinkHandler::onEvent(NetlinkEvent *evt) {
    const char *subsys = evt->getSubsystem();
    if (!subsys) {
        ALOGW("No subsystem found in netlink event");
        return;
    }

    if (!strcmp(subsys, "net")) {
        NetlinkEvent::Action action = evt->getAction();
        const char *iface = evt->findParam("INTERFACE");
        if ((action == NetlinkEvent::Action::kAdd) ||
            (action == NetlinkEvent::Action::kLinkUp) ||
            (action == NetlinkEvent::Action::kLinkDown)) {
            const char *ifIndex = evt->findParam("IFINDEX");
            long ifaceIndex = parseIfIndex(ifIndex);
            if (ifaceIndex) {
                gCtls->trafficCtrl.addInterface(iface, ifaceIndex);
            } else {
                ALOGE("invalid interface index: %s(%s)", iface, ifIndex);
            }
        }

        if (action == NetlinkEvent::Action::kAdd) {
            notifyInterfaceAdded(iface);
        } else if (action == NetlinkEvent::Action::kRemove) {
            notifyInterfaceRemoved(iface);
        } else if (action == NetlinkEvent::Action::kChange) {
            evt->dump();
            notifyInterfaceChanged("nana", true);
        } else if (action == NetlinkEvent::Action::kLinkUp) {
            notifyInterfaceLinkChanged(iface, true);
        } else if (action == NetlinkEvent::Action::kLinkDown) {
            notifyInterfaceLinkChanged(iface, false);
        } else if (action == NetlinkEvent::Action::kAddressUpdated ||
                   action == NetlinkEvent::Action::kAddressRemoved) {
            const char *address = evt->findParam("ADDRESS");
            const char *flags = evt->findParam("FLAGS");
            const char *scope = evt->findParam("SCOPE");
            const char *ifIndex = evt->findParam("IFINDEX");
            char addrstr[INET6_ADDRSTRLEN + strlen("/128")];
            strlcpy(addrstr, address, sizeof(addrstr));
            char *slash = strchr(addrstr, '/');
            if (slash) {
                *slash = '\0';
            }

            long ifaceIndex = parseIfIndex(ifIndex);
            if (!ifaceIndex) {
                ALOGE("invalid interface index: %s(%s)", iface, ifIndex);
            }
            const bool addrUpdated = (action == NetlinkEvent::Action::kAddressUpdated);
            if (addrUpdated) {
                gCtls->netCtrl.addInterfaceAddress(ifaceIndex, address);
            } else {  // action == NetlinkEvent::Action::kAddressRemoved
                bool shouldDestroy = gCtls->netCtrl.removeInterfaceAddress(ifaceIndex, address);
                if (shouldDestroy) {
                    SockDiag sd;
                    if (sd.open()) {
                        int ret = sd.destroySockets(addrstr);
                        if (ret < 0) {
                            ALOGE("Error destroying sockets: %s", strerror(-ret));
                        }
                    } else {
                        ALOGE("Error opening NETLINK_SOCK_DIAG socket: %s", strerror(errno));
                    }
                }
            }
            // Note: if this interface was deleted, iface is "" and we don't notify.
            if (iface && iface[0] && address && flags && scope) {
                if (addrUpdated) {
                    notifyAddressUpdated(address, iface, std::stoi(flags), std::stoi(scope));
                } else {
                    notifyAddressRemoved(address, iface, std::stoi(flags), std::stoi(scope));
                }
            }
        } else if (action == NetlinkEvent::Action::kRdnss) {
            const char *lifetime = evt->findParam("LIFETIME");
            const char *servers = evt->findParam("SERVERS");
            if (lifetime && servers) {
                notifyInterfaceDnsServers(iface, strtol(lifetime, nullptr, 10),
                                          android::base::Split(servers, ","));
            }
        } else if (action == NetlinkEvent::Action::kRouteUpdated ||
                   action == NetlinkEvent::Action::kRouteRemoved) {
            const char *route = evt->findParam("ROUTE");
            const char *gateway = evt->findParam("GATEWAY");
            const char *iface = evt->findParam("INTERFACE");
            if (route && (gateway || iface)) {
                notifyRouteChange((action == NetlinkEvent::Action::kRouteUpdated) ? true : false,
                                  route, (gateway == nullptr) ? "" : gateway,
                                  (iface == nullptr) ? "" : iface);
            }
        }

    } else if (!strcmp(subsys, "qlog") || !strcmp(subsys, "xt_quota2")) {
        const char *alertName = evt->findParam("ALERT_NAME");
        const char *iface = evt->findParam("INTERFACE");
        if (alertName && iface) {
            notifyQuotaLimitReached(alertName, iface);
        }

    } else if (!strcmp(subsys, "strict")) {
        const char *uid = evt->findParam("UID");
        const char *hex = evt->findParam("HEX");
        if (uid && hex) {
            notifyStrictCleartext(strtol(uid, nullptr, 10), hex);
        }

    } else if (!strcmp(subsys, "xt_idletimer")) {
        const char *label = evt->findParam("INTERFACE");
        const char *state = evt->findParam("STATE");
        const char *timestamp = evt->findParam("TIME_NS");
        const char *uid = evt->findParam("UID");
        if (state) {
            bool isActive = !strcmp("active", state);
            int64_t processTimestamp = (timestamp == nullptr) ? 0 : strtoll(timestamp, nullptr, 10);
            int intLabel;
            // NMS only accepts interface class activity changes with integer labels, and only ever
            // creates idletimers with integer labels.
            if (android::base::ParseInt(label, &intLabel)) {
                const long reportedUid =
                        (uid != nullptr && isActive) ? strtol(uid, nullptr, 10) : -1;
                notifyInterfaceClassActivityChanged(intLabel, isActive, processTimestamp,
                                                    reportedUid);
            }
        }

#if !LOG_NDEBUG
    } else if (strcmp(subsys, "platform") && strcmp(subsys, "backlight")) {
        /* It is not a VSYNC or a backlight event */
        ALOGV("unexpected event from subsystem %s", subsys);
#endif
    }
}

void NetlinkHandler::notifyInterfaceAdded(const std::string& ifName) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceAdded, ifName);
}

void NetlinkHandler::notifyInterfaceRemoved(const std::string& ifName) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceRemoved, ifName);
}

void NetlinkHandler::notifyInterfaceChanged(const std::string& ifName, bool up) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceChanged, ifName, up);
}

void NetlinkHandler::notifyInterfaceLinkChanged(const std::string& ifName, bool up) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceLinkStateChanged, ifName, up);
}

void NetlinkHandler::notifyQuotaLimitReached(const std::string& labelName,
                                             const std::string& ifName) {
    LOG_EVENT_FUNC(BINDER_RETRY, onQuotaLimitReached, labelName, ifName);
}

void NetlinkHandler::notifyInterfaceClassActivityChanged(int label, bool isActive,
                                                         int64_t timestamp, int uid) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceClassActivityChanged, isActive, label, timestamp, uid);
}

void NetlinkHandler::notifyAddressUpdated(const std::string& addr, const std::string& ifName,
                                          int flags, int scope) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceAddressUpdated, addr, ifName, flags, scope);
}

void NetlinkHandler::notifyAddressRemoved(const std::string& addr, const std::string& ifName,
                                          int flags, int scope) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceAddressRemoved, addr, ifName, flags, scope);
}

void NetlinkHandler::notifyInterfaceDnsServers(const std::string& ifName, int64_t lifetime,
                                               const std::vector<std::string>& servers) {
    LOG_EVENT_FUNC(BINDER_RETRY, onInterfaceDnsServerInfo, ifName, lifetime, servers);
}

void NetlinkHandler::notifyRouteChange(bool updated, const std::string& route,
                                       const std::string& gateway, const std::string& ifName) {
    LOG_EVENT_FUNC(BINDER_RETRY, onRouteChanged, updated, route, gateway, ifName);
}

void NetlinkHandler::notifyStrictCleartext(uid_t uid, const std::string& hex) {
    LOG_EVENT_FUNC(BINDER_RETRY, onStrictCleartextDetected, uid, hex);
}

}  // namespace net
}  // namespace android
