/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "Dns64Configuration"
#define DBG 0

#include "Dns64Configuration.h"

#include <log/log.h>
#include <netdb.h>
#include <thread>
#include <utility>

#include <arpa/inet.h>

#include "DnsResolver.h"
#include "NetdConstants.h"  // ScopedAddrinfo
#include "getaddrinfo.h"
#include "netd_resolv/resolv.h"
#include "netdutils/BackoffSequence.h"
#include "netdutils/DumpWriter.h"
#include "netid_client.h"
#include "stats.pb.h"

namespace android {

using android::net::NetworkDnsEventReported;
using netdutils::DumpWriter;
using netdutils::IPAddress;
using netdutils::IPPrefix;

namespace net {

const char Dns64Configuration::kIPv4OnlyHost[] = "ipv4only.arpa.";
const char Dns64Configuration::kIPv4Literal1[] = "192.0.0.170";
const char Dns64Configuration::kIPv4Literal2[] = "192.0.0.171";

void Dns64Configuration::startPrefixDiscovery(unsigned netId) {
    std::lock_guard guard(mMutex);

    // TODO: Keep previous prefix for a while
    // Currently, we remove current prefix, if any, before starting a prefix discovery.
    // This causes that Netd and framework temporarily forgets DNS64 prefix even the prefix may be
    // discovered in a short time.
    removeDns64Config(netId);

    Dns64Config cfg(getNextId(), netId);
    // Emplace a copy of |cfg| in the map.
    mDns64Configs.emplace(std::make_pair(netId, cfg));

    // Note that capturing |cfg| in this lambda creates a copy.
    std::thread discovery_thread([this, cfg] {
        // Make a mutable copy rather than mark the whole lambda mutable.
        // No particular reason.
        Dns64Config evalCfg(cfg);

        auto backoff = netdutils::BackoffSequence<>::Builder()
                               .withInitialRetransmissionTime(std::chrono::seconds(1))
                               .withMaximumRetransmissionTime(std::chrono::seconds(3600))
                               .build();

        while (true) {
            if (!this->shouldContinueDiscovery(evalCfg)) break;

            android_net_context netcontext{};
            mGetNetworkContextCallback(evalCfg.netId, 0, &netcontext);

            // Prefix discovery must bypass private DNS because in strict mode
            // the server generally won't know the NAT64 prefix.
            netcontext.flags |= NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
            if (doRfc7050PrefixDiscovery(netcontext, &evalCfg)) {
                this->recordDns64Config(evalCfg);
                break;
            }

            if (!this->shouldContinueDiscovery(evalCfg)) break;

            if (!backoff.hasNextTimeout()) break;
            {
                std::unique_lock<std::mutex> cvGuard(mMutex);
                // TODO: Consider some chrono math, combined with wait_until()
                // perhaps, to prevent early re-resolves from the removal of
                // other netids with IPv6-only nameservers.
                mCv.wait_for(cvGuard, backoff.getNextTimeout());
            }
        }
    });
    discovery_thread.detach();
}

void Dns64Configuration::stopPrefixDiscovery(unsigned netId) {
    std::lock_guard guard(mMutex);
    removeDns64Config(netId);
    mCv.notify_all();
}

IPPrefix Dns64Configuration::getPrefix64Locked(unsigned netId) const REQUIRES(mMutex) {
    const auto& iter = mDns64Configs.find(netId);
    if (iter != mDns64Configs.end()) return iter->second.prefix64;

    return IPPrefix{};
}

IPPrefix Dns64Configuration::getPrefix64(unsigned netId) const {
    std::lock_guard guard(mMutex);
    return getPrefix64Locked(netId);
}

void Dns64Configuration::dump(DumpWriter& dw, unsigned netId) {
    static const char kLabel[] = "DNS64 config";

    std::lock_guard guard(mMutex);

    const auto& iter = mDns64Configs.find(netId);
    if (iter == mDns64Configs.end()) {
        dw.println("%s: none", kLabel);
        return;
    }

    const Dns64Config& cfg = iter->second;
    if (cfg.prefix64.length() == 0) {
        dw.println("%s: no prefix yet discovered", kLabel);
    } else {
        dw.println("%s: discovered prefix %s", kLabel, cfg.prefix64.toString().c_str());
    }
}

// NOTE: The full RFC 7050 DNS64 discovery process is not implemented here.
// Instead, and more simplistic version of the same thing is done, and it
// currently assumes the DNS64 prefix is a /96.
bool Dns64Configuration::doRfc7050PrefixDiscovery(const android_net_context& netcontext,
                                                  Dns64Config* cfg) {
    ALOGW("(%u, %u) Detecting NAT64 prefix from DNS...", cfg->netId, cfg->discoveryId);

    const struct addrinfo hints = {
            .ai_family = AF_INET6,
    };

    // TODO: Refactor so that netd can get all the regular getaddrinfo handling
    // that regular apps get. We bypass the UNIX socket connection back to
    // ourselves, which means we also bypass all the special netcontext flag
    // handling and the resolver event logging.
    struct addrinfo* res = nullptr;
    NetworkDnsEventReported event;
    const int status = android_getaddrinfofornetcontext(kIPv4OnlyHost, nullptr, &hints, &netcontext,
                                                        &res, &event);
    ScopedAddrinfo result(res);
    if (status != 0) {
        ALOGW("(%u, %u) plat_prefix/dns(%s) status = %d/%s", cfg->netId, cfg->discoveryId,
              kIPv4OnlyHost, status, gai_strerror(status));
        return false;
    }

    // Use only the first result.  If other records are present, possibly
    // with differing DNS64 prefixes they are ignored. Note that this is a
    // violation of https://tools.ietf.org/html/rfc7050#section-3
    //
    //     "A node MUST look through all of the received AAAA resource records
    //      to collect one or more Pref64::/n."
    //
    // TODO: Consider remedying this.
    if (result->ai_family != AF_INET6) {
        ALOGW("(%u, %u) plat_prefix/unexpected address family: %d", cfg->netId, cfg->discoveryId,
              result->ai_family);
        return false;
    }
    const IPAddress ipv6(reinterpret_cast<sockaddr_in6*>(result->ai_addr)->sin6_addr);
    // Only /96 DNS64 prefixes are supported at this time.
    cfg->prefix64 = IPPrefix(ipv6, 96);

    ALOGW("(%u, %u) Detected NAT64 prefix %s", cfg->netId, cfg->discoveryId,
          cfg->prefix64.toString().c_str());

    return true;
}

bool Dns64Configuration::isDiscoveryInProgress(const Dns64Config& cfg) const REQUIRES(mMutex) {
    const auto& iter = mDns64Configs.find(cfg.netId);
    if (iter == mDns64Configs.end()) return false;

    const Dns64Config& currentCfg = iter->second;
    return (currentCfg.discoveryId == cfg.discoveryId);
}

bool Dns64Configuration::reportNat64PrefixStatus(unsigned netId, bool added, const IPPrefix& pfx) {
    if (pfx.ip().family() != AF_INET6 || pfx.ip().scope_id() != 0) {
        ALOGW("Abort to send NAT64 prefix notification. Unexpected NAT64 prefix (%u, %d, %s).",
              netId, added, pfx.toString().c_str());
        return false;
    }
    Nat64PrefixInfo args = {netId, added, pfx.ip().toString(), (uint8_t)pfx.length()};
    mPrefixCallback(args);
    return true;
}

bool Dns64Configuration::shouldContinueDiscovery(const Dns64Config& cfg) {
    std::lock_guard guard(mMutex);
    return isDiscoveryInProgress(cfg);
}

void Dns64Configuration::removeDns64Config(unsigned netId) REQUIRES(mMutex) {
    IPPrefix prefix = getPrefix64Locked(netId);
    mDns64Configs.erase(netId);
    if (!prefix.isUninitialized()) {
        reportNat64PrefixStatus(netId, PREFIX_REMOVED, prefix);
    }
}

void Dns64Configuration::recordDns64Config(const Dns64Config& cfg) {
    std::lock_guard guard(mMutex);
    if (!isDiscoveryInProgress(cfg)) return;

    removeDns64Config(cfg.netId);
    mDns64Configs.emplace(std::make_pair(cfg.netId, cfg));

    reportNat64PrefixStatus(cfg.netId, PREFIX_ADDED, cfg.prefix64);

    // TODO: consider extending INetdEventListener to report the DNS64 prefix
    // up to ConnectivityService to potentially include this in LinkProperties.
}

}  // namespace net
}  // namespace android
