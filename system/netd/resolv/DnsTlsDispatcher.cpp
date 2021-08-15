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

#define LOG_TAG "DnsTlsDispatcher"
//#define LOG_NDEBUG 0

#include "DnsTlsDispatcher.h"
#include <netdutils/Stopwatch.h>
#include "DnsTlsSocketFactory.h"
#include "resolv_private.h"
#include "stats.pb.h"

#include "log/log.h"

namespace android {
namespace net {

using android::netdutils::Stopwatch;
using netdutils::Slice;

// static
std::mutex DnsTlsDispatcher::sLock;

DnsTlsDispatcher::DnsTlsDispatcher() {
    mFactory.reset(new DnsTlsSocketFactory());
}

std::list<DnsTlsServer> DnsTlsDispatcher::getOrderedServerList(
        const std::list<DnsTlsServer> &tlsServers, unsigned mark) const {
    // Our preferred DnsTlsServer order is:
    //     1) reuse existing IPv6 connections
    //     2) reuse existing IPv4 connections
    //     3) establish new IPv6 connections
    //     4) establish new IPv4 connections
    std::list<DnsTlsServer> existing6;
    std::list<DnsTlsServer> existing4;
    std::list<DnsTlsServer> new6;
    std::list<DnsTlsServer> new4;

    // Pull out any servers for which we might have existing connections and
    // place them at the from the list of servers to try.
    {
        std::lock_guard guard(sLock);

        for (const auto& tlsServer : tlsServers) {
            const Key key = std::make_pair(mark, tlsServer);
            if (mStore.find(key) != mStore.end()) {
                switch (tlsServer.ss.ss_family) {
                    case AF_INET:
                        existing4.push_back(tlsServer);
                        break;
                    case AF_INET6:
                        existing6.push_back(tlsServer);
                        break;
                }
            } else {
                switch (tlsServer.ss.ss_family) {
                    case AF_INET:
                        new4.push_back(tlsServer);
                        break;
                    case AF_INET6:
                        new6.push_back(tlsServer);
                        break;
                }
            }
        }
    }

    auto& out = existing6;
    out.splice(out.cend(), existing4);
    out.splice(out.cend(), new6);
    out.splice(out.cend(), new4);
    return out;
}

DnsTlsTransport::Response DnsTlsDispatcher::query(const std::list<DnsTlsServer>& tlsServers,
                                                  res_state statp, const Slice query,
                                                  const Slice ans, int* resplen) {
    const std::list<DnsTlsServer> orderedServers(getOrderedServerList(tlsServers, statp->_mark));

    if (orderedServers.empty()) ALOGW("Empty DnsTlsServer list");

    DnsTlsTransport::Response code = DnsTlsTransport::Response::internal_error;
    int serverCount = 0;
    for (const auto& server : orderedServers) {
        DnsQueryEvent* dnsQueryEvent =
                statp->event->mutable_dns_query_events()->add_dns_query_event();
        dnsQueryEvent->set_rcode(NS_R_INTERNAL_ERROR);
        Stopwatch query_stopwatch;
        code = this->query(server, statp->_mark, query, ans, resplen);

        dnsQueryEvent->set_latency_micros(saturate_cast<int32_t>(query_stopwatch.timeTakenUs()));
        dnsQueryEvent->set_dns_server_index(serverCount++);
        dnsQueryEvent->set_ip_version(ipFamilyToIPVersion(server.ss.ss_family));
        dnsQueryEvent->set_protocol(PROTO_DOT);
        dnsQueryEvent->set_type(getQueryType(query.base(), query.size()));

        switch (code) {
            // These response codes are valid responses and not expected to
            // change if another server is queried.
            case DnsTlsTransport::Response::success:
                dnsQueryEvent->set_rcode(
                        static_cast<NsRcode>(reinterpret_cast<HEADER*>(ans.base())->rcode));
                [[fallthrough]];
            case DnsTlsTransport::Response::limit_error:
                return code;
            // These response codes might differ when trying other servers, so
            // keep iterating to see if we can get a different (better) result.
            case DnsTlsTransport::Response::network_error:
                // Sync from res_tls_send in res_send.cpp
                dnsQueryEvent->set_rcode(NS_R_TIMEOUT);
                [[fallthrough]];
            case DnsTlsTransport::Response::internal_error:
                continue;
            // No "default" statement.
        }
    }

    return code;
}

DnsTlsTransport::Response DnsTlsDispatcher::query(const DnsTlsServer& server, unsigned mark,
                                                  const Slice query,
                                                  const Slice ans, int *resplen) {
    const Key key = std::make_pair(mark, server);
    Transport* xport;
    {
        std::lock_guard guard(sLock);
        auto it = mStore.find(key);
        if (it == mStore.end()) {
            xport = new Transport(server, mark, mFactory.get());
            mStore[key].reset(xport);
        } else {
            xport = it->second.get();
        }
        ++xport->useCount;
    }

    ALOGV("Sending query of length %zu", query.size());
    auto res = xport->transport.query(query);
    ALOGV("Awaiting response");
    const auto& result = res.get();
    DnsTlsTransport::Response code = result.code;
    if (code == DnsTlsTransport::Response::success) {
        if (result.response.size() > ans.size()) {
            ALOGV("Response too large: %zu > %zu", result.response.size(), ans.size());
            code = DnsTlsTransport::Response::limit_error;
        } else {
            ALOGV("Got response successfully");
            *resplen = result.response.size();
            netdutils::copy(ans, netdutils::makeSlice(result.response));
        }
    } else {
        ALOGV("Query failed: %u", (unsigned int) code);
    }

    auto now = std::chrono::steady_clock::now();
    {
        std::lock_guard guard(sLock);
        --xport->useCount;
        xport->lastUsed = now;
        cleanup(now);
    }
    return code;
}

// This timeout effectively controls how long to keep SSL session tickets.
static constexpr std::chrono::minutes IDLE_TIMEOUT(5);
void DnsTlsDispatcher::cleanup(std::chrono::time_point<std::chrono::steady_clock> now) {
    // To avoid scanning mStore after every query, return early if a cleanup has been
    // performed recently.
    if (now - mLastCleanup < IDLE_TIMEOUT) {
        return;
    }
    for (auto it = mStore.begin(); it != mStore.end();) {
        auto& s = it->second;
        if (s->useCount == 0 && now - s->lastUsed > IDLE_TIMEOUT) {
            it = mStore.erase(it);
        } else {
            ++it;
        }
    }
    mLastCleanup = now;
}

}  // end of namespace net
}  // end of namespace android
