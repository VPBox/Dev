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

#define LOG_TAG "PrivateDnsConfiguration"
#define DBG 0

#include "PrivateDnsConfiguration.h"

#include <log/log.h>
#include <netdb.h>
#include <sys/socket.h>

#include "DnsTlsTransport.h"
#include "ResolverEventReporter.h"
#include "netd_resolv/resolv.h"
#include "netdutils/BackoffSequence.h"

namespace android {
namespace net {

std::string addrToString(const sockaddr_storage* addr) {
    char out[INET6_ADDRSTRLEN] = {0};
    getnameinfo((const sockaddr*) addr, sizeof(sockaddr_storage), out, INET6_ADDRSTRLEN, nullptr, 0,
                NI_NUMERICHOST);
    return std::string(out);
}

bool parseServer(const char* server, sockaddr_storage* parsed) {
    addrinfo hints = {.ai_family = AF_UNSPEC, .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV};
    addrinfo* res;

    int err = getaddrinfo(server, "853", &hints, &res);
    if (err != 0) {
        ALOGW("Failed to parse server address (%s): %s", server, gai_strerror(err));
        return false;
    }

    memcpy(parsed, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    return true;
}

int PrivateDnsConfiguration::set(int32_t netId, uint32_t mark,
                                 const std::vector<std::string>& servers, const std::string& name,
                                 const std::set<std::vector<uint8_t>>& fingerprints) {
    if (DBG) {
        ALOGD("PrivateDnsConfiguration::set(%u, 0x%x, %zu, %s, %zu)", netId, mark, servers.size(),
              name.c_str(), fingerprints.size());
    }

    const bool explicitlyConfigured = !name.empty() || !fingerprints.empty();

    // Parse the list of servers that has been passed in
    std::set<DnsTlsServer> tlsServers;
    for (size_t i = 0; i < servers.size(); ++i) {
        sockaddr_storage parsed;
        if (!parseServer(servers[i].c_str(), &parsed)) {
            return -EINVAL;
        }
        DnsTlsServer server(parsed);
        server.name = name;
        server.fingerprints = fingerprints;
        tlsServers.insert(server);
    }

    std::lock_guard guard(mPrivateDnsLock);
    if (explicitlyConfigured) {
        mPrivateDnsModes[netId] = PrivateDnsMode::STRICT;
    } else if (!tlsServers.empty()) {
        mPrivateDnsModes[netId] = PrivateDnsMode::OPPORTUNISTIC;
    } else {
        mPrivateDnsModes[netId] = PrivateDnsMode::OFF;
        mPrivateDnsTransports.erase(netId);
        return 0;
    }

    // Create the tracker if it was not present
    auto netPair = mPrivateDnsTransports.find(netId);
    if (netPair == mPrivateDnsTransports.end()) {
        // No TLS tracker yet for this netId.
        bool added;
        std::tie(netPair, added) = mPrivateDnsTransports.emplace(netId, PrivateDnsTracker());
        if (!added) {
            ALOGE("Memory error while recording private DNS for netId %d", netId);
            return -ENOMEM;
        }
    }
    auto& tracker = netPair->second;

    // Remove any servers from the tracker that are not in |servers| exactly.
    for (auto it = tracker.begin(); it != tracker.end();) {
        if (tlsServers.count(it->first) == 0) {
            it = tracker.erase(it);
        } else {
            ++it;
        }
    }

    // Add any new or changed servers to the tracker, and initiate async checks for them.
    for (const auto& server : tlsServers) {
        if (needsValidation(tracker, server)) {
            validatePrivateDnsProvider(server, tracker, netId, mark);
        }
    }
    return 0;
}

PrivateDnsStatus PrivateDnsConfiguration::getStatus(unsigned netId) {
    PrivateDnsStatus status{PrivateDnsMode::OFF, {}};
    std::lock_guard guard(mPrivateDnsLock);

    const auto mode = mPrivateDnsModes.find(netId);
    if (mode == mPrivateDnsModes.end()) return status;
    status.mode = mode->second;

    const auto netPair = mPrivateDnsTransports.find(netId);
    if (netPair != mPrivateDnsTransports.end()) {
        for (const auto& serverPair : netPair->second) {
            if (serverPair.second == Validation::success) {
                status.validatedServers.push_back(serverPair.first);
            }
        }
    }

    return status;
}

void PrivateDnsConfiguration::getStatus(unsigned netId, ExternalPrivateDnsStatus* status) {
    std::lock_guard guard(mPrivateDnsLock);

    const auto mode = mPrivateDnsModes.find(netId);
    if (mode == mPrivateDnsModes.end()) return;
    status->mode = mode->second;

    const auto netPair = mPrivateDnsTransports.find(netId);
    if (netPair != mPrivateDnsTransports.end()) {
        int count = 0;
        for (const auto& serverPair : netPair->second) {
            status->serverStatus[count].ss = serverPair.first.ss;
            status->serverStatus[count].hostname =
                    serverPair.first.name.empty() ? "" : serverPair.first.name.c_str();
            status->serverStatus[count].validation = serverPair.second;
            count++;
            if (count >= MAXNS) break;  // Lose the rest
        }
        status->numServers = count;
    }
}

void PrivateDnsConfiguration::clear(unsigned netId) {
    if (DBG) {
        ALOGD("PrivateDnsConfiguration::clear(%u)", netId);
    }
    std::lock_guard guard(mPrivateDnsLock);
    mPrivateDnsModes.erase(netId);
    mPrivateDnsTransports.erase(netId);
}

void PrivateDnsConfiguration::validatePrivateDnsProvider(const DnsTlsServer& server,
                                                         PrivateDnsTracker& tracker, unsigned netId,
                                                         uint32_t mark) REQUIRES(mPrivateDnsLock) {
    if (DBG) {
        ALOGD("validatePrivateDnsProvider(%s, %u)", addrToString(&server.ss).c_str(), netId);
    }

    tracker[server] = Validation::in_process;
    if (DBG) {
        ALOGD("Server %s marked as in_process.  Tracker now has size %zu",
              addrToString(&server.ss).c_str(), tracker.size());
    }
    // Note that capturing |server| and |netId| in this lambda create copies.
    std::thread validate_thread([this, server, netId, mark] {
        // cat /proc/sys/net/ipv4/tcp_syn_retries yields "6".
        //
        // Start with a 1 minute delay and backoff to once per hour.
        //
        // Assumptions:
        //     [1] Each TLS validation is ~10KB of certs+handshake+payload.
        //     [2] Network typically provision clients with <=4 nameservers.
        //     [3] Average month has 30 days.
        //
        // Each validation pass in a given hour is ~1.2MB of data. And 24
        // such validation passes per day is about ~30MB per month, in the
        // worst case. Otherwise, this will cost ~600 SYNs per month
        // (6 SYNs per ip, 4 ips per validation pass, 24 passes per day).
        auto backoff = netdutils::BackoffSequence<>::Builder()
                               .withInitialRetransmissionTime(std::chrono::seconds(60))
                               .withMaximumRetransmissionTime(std::chrono::seconds(3600))
                               .build();

        while (true) {
            // ::validate() is a blocking call that performs network operations.
            // It can take milliseconds to minutes, up to the SYN retry limit.
            const bool success = DnsTlsTransport::validate(server, netId, mark);
            if (DBG) {
                ALOGD("validateDnsTlsServer returned %d for %s", success,
                      addrToString(&server.ss).c_str());
            }

            const bool needs_reeval = this->recordPrivateDnsValidation(server, netId, success);
            if (!needs_reeval) {
                break;
            }

            if (backoff.hasNextTimeout()) {
                std::this_thread::sleep_for(backoff.getNextTimeout());
            } else {
                break;
            }
        }
    });
    validate_thread.detach();
}

bool PrivateDnsConfiguration::recordPrivateDnsValidation(const DnsTlsServer& server, unsigned netId,
                                                         bool success) {
    constexpr bool NEEDS_REEVALUATION = true;
    constexpr bool DONT_REEVALUATE = false;

    std::lock_guard guard(mPrivateDnsLock);

    auto netPair = mPrivateDnsTransports.find(netId);
    if (netPair == mPrivateDnsTransports.end()) {
        ALOGW("netId %u was erased during private DNS validation", netId);
        return DONT_REEVALUATE;
    }

    const auto mode = mPrivateDnsModes.find(netId);
    if (mode == mPrivateDnsModes.end()) {
        ALOGW("netId %u has no private DNS validation mode", netId);
        return DONT_REEVALUATE;
    }
    const bool modeDoesReevaluation = (mode->second == PrivateDnsMode::STRICT);

    bool reevaluationStatus =
            (success || !modeDoesReevaluation) ? DONT_REEVALUATE : NEEDS_REEVALUATION;

    auto& tracker = netPair->second;
    auto serverPair = tracker.find(server);
    if (serverPair == tracker.end()) {
        ALOGW("Server %s was removed during private DNS validation",
              addrToString(&server.ss).c_str());
        success = false;
        reevaluationStatus = DONT_REEVALUATE;
    } else if (!(serverPair->first == server)) {
        // TODO: It doesn't seem correct to overwrite the tracker entry for
        // |server| down below in this circumstance... Fix this.
        ALOGW("Server %s was changed during private DNS validation",
              addrToString(&server.ss).c_str());
        success = false;
        reevaluationStatus = DONT_REEVALUATE;
    }

    // Send a validation event to NetdEventListenerService.
    const auto& listeners = ResolverEventReporter::getInstance().getListeners();
    if (listeners.size() != 0) {
        for (const auto& it : listeners) {
            it->onPrivateDnsValidationEvent(netId, addrToString(&server.ss), server.name, success);
        }
        if (DBG) {
            ALOGD("Sent validation %s event on netId %u for %s with hostname %s",
                  success ? "success" : "failure", netId, addrToString(&server.ss).c_str(),
                  server.name.c_str());
        }
    } else {
        ALOGE("Validation event not sent since no INetdEventListener receiver is available.");
    }

    if (success) {
        tracker[server] = Validation::success;
        if (DBG) {
            ALOGD("Validation succeeded for %s! Tracker now has %zu entries.",
                  addrToString(&server.ss).c_str(), tracker.size());
        }
    } else {
        // Validation failure is expected if a user is on a captive portal.
        // TODO: Trigger a second validation attempt after captive portal login
        // succeeds.
        tracker[server] = (reevaluationStatus == NEEDS_REEVALUATION) ? Validation::in_process
                                                                     : Validation::fail;
        if (DBG) {
            ALOGD("Validation failed for %s!", addrToString(&server.ss).c_str());
        }
    }

    return reevaluationStatus;
}

// Start validation for newly added servers as well as any servers that have
// landed in Validation::fail state. Note that servers that have failed
// multiple validation attempts but for which there is still a validating
// thread running are marked as being Validation::in_process.
bool PrivateDnsConfiguration::needsValidation(const PrivateDnsTracker& tracker,
                                              const DnsTlsServer& server) {
    const auto& iter = tracker.find(server);
    return (iter == tracker.end()) || (iter->second == Validation::fail);
}

PrivateDnsConfiguration gPrivateDnsConfiguration;

}  // namespace net
}  // namespace android
