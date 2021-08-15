/*
 * Copyright (C) 2019 The Android Open Source Project
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

#define LOG_TAG "ResolverController"

#include "ResolverController.h"

#include <set>
#include <string>
#include <vector>

#include <netdb.h>

#include <aidl/android/net/IDnsResolver.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "Dns64Configuration.h"
#include "DnsResolver.h"
#include "Fwmark.h"
#include "PrivateDnsConfiguration.h"
#include "ResolverEventReporter.h"
#include "ResolverStats.h"
#include "netd_resolv/stats.h"
#include "resolv_cache.h"

using namespace std::placeholders;
using aidl::android::net::ResolverParamsParcel;

namespace android {

using netdutils::DumpWriter;

namespace net {

namespace {

std::string addrToString(const sockaddr_storage* addr) {
    char out[INET6_ADDRSTRLEN] = {0};
    getnameinfo((const sockaddr*)addr, sizeof(sockaddr_storage), out, INET6_ADDRSTRLEN, nullptr, 0,
                NI_NUMERICHOST);
    return std::string(out);
}

const char* getPrivateDnsModeString(PrivateDnsMode mode) {
    switch (mode) {
        case PrivateDnsMode::OFF:
            return "OFF";
        case PrivateDnsMode::OPPORTUNISTIC:
            return "OPPORTUNISTIC";
        case PrivateDnsMode::STRICT:
            return "STRICT";
    }
}

constexpr const char* validationStatusToString(Validation value) {
    switch (value) {
        case Validation::in_process:
            return "in_process";
        case Validation::success:
            return "success";
        case Validation::fail:
            return "fail";
        case Validation::unknown_server:
            return "unknown_server";
        case Validation::unknown_netid:
            return "unknown_netid";
        default:
            return "unknown_status";
    }
}

void sendNat64PrefixEvent(const Dns64Configuration::Nat64PrefixInfo& args) {
    const auto& listeners = ResolverEventReporter::getInstance().getListeners();
    if (listeners.size() == 0) {
        LOG(ERROR) << __func__ << ": No available listener. dropping NAT64 prefix event";
        return;
    }
    for (const auto& it : listeners) {
        it->onNat64PrefixEvent(args.netId, args.added, args.prefixString, args.prefixLength);
    }
}

int getDnsInfo(unsigned netId, std::vector<std::string>* servers, std::vector<std::string>* domains,
               res_params* params, std::vector<android::net::ResolverStats>* stats,
               std::vector<int32_t>* wait_for_pending_req_timeout_count) {
    using aidl::android::net::IDnsResolver;
    using android::net::ResolverStats;
    static_assert(ResolverStats::STATS_SUCCESSES == IDnsResolver::RESOLVER_STATS_SUCCESSES &&
                          ResolverStats::STATS_ERRORS == IDnsResolver::RESOLVER_STATS_ERRORS &&
                          ResolverStats::STATS_TIMEOUTS == IDnsResolver::RESOLVER_STATS_TIMEOUTS &&
                          ResolverStats::STATS_INTERNAL_ERRORS ==
                                  IDnsResolver::RESOLVER_STATS_INTERNAL_ERRORS &&
                          ResolverStats::STATS_RTT_AVG == IDnsResolver::RESOLVER_STATS_RTT_AVG &&
                          ResolverStats::STATS_LAST_SAMPLE_TIME ==
                                  IDnsResolver::RESOLVER_STATS_LAST_SAMPLE_TIME &&
                          ResolverStats::STATS_USABLE == IDnsResolver::RESOLVER_STATS_USABLE &&
                          ResolverStats::STATS_COUNT == IDnsResolver::RESOLVER_STATS_COUNT,
                  "AIDL and ResolverStats.h out of sync");
    int nscount = -1;
    sockaddr_storage res_servers[MAXNS];
    int dcount = -1;
    char res_domains[MAXDNSRCH][MAXDNSRCHPATH];
    res_stats res_stats[MAXNS];
    servers->clear();
    domains->clear();
    *params = res_params{};
    stats->clear();
    int res_wait_for_pending_req_timeout_count;
    int revision_id = android_net_res_stats_get_info_for_net(
            netId, &nscount, res_servers, &dcount, res_domains, params, res_stats,
            &res_wait_for_pending_req_timeout_count);

    // If the netId is unknown (which can happen for valid net IDs for which no DNS servers have
    // yet been configured), there is no revision ID. In this case there is no data to return.
    if (revision_id < 0) {
        return 0;
    }

    // Verify that the returned data is sane.
    if (nscount < 0 || nscount > MAXNS || dcount < 0 || dcount > MAXDNSRCH) {
        LOG(ERROR) << __func__ << ": nscount = " << nscount << ", dcount = " << dcount;
        return -ENOTRECOVERABLE;
    }

    // Determine which servers are considered usable by the resolver.
    bool valid_servers[MAXNS];
    std::fill_n(valid_servers, MAXNS, false);
    android_net_res_stats_get_usable_servers(params, res_stats, nscount, valid_servers);

    // Convert the server sockaddr structures to std::string.
    stats->resize(nscount);
    for (int i = 0; i < nscount; ++i) {
        char hbuf[NI_MAXHOST];
        int rv =
                getnameinfo(reinterpret_cast<const sockaddr*>(&res_servers[i]),
                            sizeof(res_servers[i]), hbuf, sizeof(hbuf), nullptr, 0, NI_NUMERICHOST);
        std::string server_str;
        if (rv == 0) {
            server_str.assign(hbuf);
        } else {
            LOG(ERROR) << "getnameinfo() failed for server #" << i << ": " << gai_strerror(rv);
            server_str.assign("<invalid>");
        }
        servers->push_back(std::move(server_str));
        android::net::ResolverStats& cur_stats = (*stats)[i];
        android_net_res_stats_aggregate(&res_stats[i], &cur_stats.successes, &cur_stats.errors,
                                        &cur_stats.timeouts, &cur_stats.internal_errors,
                                        &cur_stats.rtt_avg, &cur_stats.last_sample_time);
        cur_stats.usable = valid_servers[i];
    }

    // Convert the stack-allocated search domain strings to std::string.
    for (int i = 0; i < dcount; ++i) {
        domains->push_back(res_domains[i]);
    }

    (*wait_for_pending_req_timeout_count)[0] = res_wait_for_pending_req_timeout_count;
    return 0;
}

}  // namespace

ResolverController::ResolverController()
    : mDns64Configuration(
              [](uint32_t netId, uint32_t uid, android_net_context* netcontext) {
                  gResNetdCallbacks.get_network_context(netId, uid, netcontext);
              },
              std::bind(sendNat64PrefixEvent, _1)) {}

void ResolverController::destroyNetworkCache(unsigned netId) {
    LOG(VERBOSE) << __func__ << ": netId = " << netId;

    resolv_delete_cache_for_net(netId);
    mDns64Configuration.stopPrefixDiscovery(netId);
    gPrivateDnsConfiguration.clear(netId);
}

int ResolverController::createNetworkCache(unsigned netId) {
    LOG(VERBOSE) << __func__ << ": netId = " << netId;

    return resolv_create_cache_for_net(netId);
}

int ResolverController::setResolverConfiguration(
        const ResolverParamsParcel& resolverParams,
        const std::set<std::vector<uint8_t>>& tlsFingerprints) {
    using aidl::android::net::IDnsResolver;

    // At private DNS validation time, we only know the netId, so we have to guess/compute the
    // corresponding socket mark.
    Fwmark fwmark;
    fwmark.netId = resolverParams.netId;
    fwmark.explicitlySelected = true;
    fwmark.protectedFromVpn = true;
    fwmark.permission = PERMISSION_SYSTEM;

    // Allow at most MAXNS private DNS servers in a network to prevent too many broken servers.
    std::vector<std::string> tlsServers = resolverParams.tlsServers;
    if (tlsServers.size() > MAXNS) {
        tlsServers.resize(MAXNS);
    }
    const int err = gPrivateDnsConfiguration.set(resolverParams.netId, fwmark.intValue, tlsServers,
                                                 resolverParams.tlsName, tlsFingerprints);
    if (err != 0) {
        return err;
    }

    // Convert network-assigned server list to bionic's format.
    const size_t serverCount = std::min<size_t>(MAXNS, resolverParams.servers.size());
    std::vector<const char*> server_ptrs;
    for (size_t i = 0; i < serverCount; ++i) {
        server_ptrs.push_back(resolverParams.servers[i].c_str());
    }

    std::string domains_str = android::base::Join(resolverParams.domains, " ");

    // TODO: Change resolv_set_nameservers_for_net() to use ResolverParamsParcel directly.
    res_params res_params = {};
    res_params.sample_validity = resolverParams.sampleValiditySeconds;
    res_params.success_threshold = resolverParams.successThreshold;
    res_params.min_samples = resolverParams.minSamples;
    res_params.max_samples = resolverParams.maxSamples;
    res_params.base_timeout_msec = resolverParams.baseTimeoutMsec;
    res_params.retry_count = resolverParams.retryCount;

    LOG(VERBOSE) << "setDnsServers netId = " << resolverParams.netId
                 << ", numservers = " << resolverParams.domains.size();

    return -resolv_set_nameservers_for_net(resolverParams.netId, server_ptrs.data(),
                                           server_ptrs.size(), domains_str.c_str(), &res_params);
}

int ResolverController::getResolverInfo(int32_t netId, std::vector<std::string>* servers,
                                        std::vector<std::string>* domains,
                                        std::vector<std::string>* tlsServers,
                                        std::vector<int32_t>* params, std::vector<int32_t>* stats,
                                        std::vector<int32_t>* wait_for_pending_req_timeout_count) {
    using aidl::android::net::IDnsResolver;
    using android::net::ResolverStats;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int ret = getDnsInfo(netId, servers, domains, &res_params, &res_stats,
                         wait_for_pending_req_timeout_count);
    if (ret != 0) {
        return ret;
    }

    // Serialize the information for binder.
    ResolverStats::encodeAll(res_stats, stats);

    ExternalPrivateDnsStatus privateDnsStatus = {PrivateDnsMode::OFF, 0, {}};
    gPrivateDnsConfiguration.getStatus(netId, &privateDnsStatus);
    for (int i = 0; i < privateDnsStatus.numServers; i++) {
        std::string tlsServer_str = addrToString(&(privateDnsStatus.serverStatus[i].ss));
        tlsServers->push_back(std::move(tlsServer_str));
    }

    params->resize(IDnsResolver::RESOLVER_PARAMS_COUNT);
    (*params)[IDnsResolver::RESOLVER_PARAMS_SAMPLE_VALIDITY] = res_params.sample_validity;
    (*params)[IDnsResolver::RESOLVER_PARAMS_SUCCESS_THRESHOLD] = res_params.success_threshold;
    (*params)[IDnsResolver::RESOLVER_PARAMS_MIN_SAMPLES] = res_params.min_samples;
    (*params)[IDnsResolver::RESOLVER_PARAMS_MAX_SAMPLES] = res_params.max_samples;
    (*params)[IDnsResolver::RESOLVER_PARAMS_BASE_TIMEOUT_MSEC] = res_params.base_timeout_msec;
    (*params)[IDnsResolver::RESOLVER_PARAMS_RETRY_COUNT] = res_params.retry_count;
    return 0;
}

void ResolverController::startPrefix64Discovery(int32_t netId) {
    mDns64Configuration.startPrefixDiscovery(netId);
}

void ResolverController::stopPrefix64Discovery(int32_t netId) {
    return mDns64Configuration.stopPrefixDiscovery(netId);
}

// TODO: use StatusOr<T> to wrap the result.
int ResolverController::getPrefix64(unsigned netId, netdutils::IPPrefix* prefix) {
    netdutils::IPPrefix p = mDns64Configuration.getPrefix64(netId);
    if (p.family() != AF_INET6 || p.length() == 0) {
        LOG(ERROR) << "No valid NAT64 prefix (" << netId << ", " << p.toString().c_str() << ")";

        return -ENOENT;
    }
    *prefix = p;
    return 0;
}

void ResolverController::dump(DumpWriter& dw, unsigned netId) {
    // No lock needed since Bionic's resolver locks all accessed data structures internally.
    using android::net::ResolverStats;
    std::vector<std::string> servers;
    std::vector<std::string> domains;
    res_params params = {};
    std::vector<ResolverStats> stats;
    std::vector<int32_t> wait_for_pending_req_timeout_count(1, 0);
    time_t now = time(nullptr);
    int rv = getDnsInfo(netId, &servers, &domains, &params, &stats,
                        &wait_for_pending_req_timeout_count);
    dw.incIndent();
    if (rv != 0) {
        dw.println("getDnsInfo() failed for netid %u", netId);
    } else {
        if (servers.empty()) {
            dw.println("No DNS servers defined");
        } else {
            dw.println("DnsEvent subsampling map: " +
                       android::base::Join(resolv_cache_dump_subsampling_map(netId), ' '));
            dw.println(
                    "DNS servers: # IP (total, successes, errors, timeouts, internal errors, "
                    "RTT avg, last sample)");
            dw.incIndent();
            for (size_t i = 0; i < servers.size(); ++i) {
                if (i < stats.size()) {
                    const ResolverStats& s = stats[i];
                    int total = s.successes + s.errors + s.timeouts + s.internal_errors;
                    if (total > 0) {
                        int time_delta = (s.last_sample_time > 0) ? now - s.last_sample_time : -1;
                        dw.println("%s (%d, %d, %d, %d, %d, %dms, %ds)%s", servers[i].c_str(),
                                   total, s.successes, s.errors, s.timeouts, s.internal_errors,
                                   s.rtt_avg, time_delta, s.usable ? "" : " BROKEN");
                    } else {
                        dw.println("%s <no data>", servers[i].c_str());
                    }
                } else {
                    dw.println("%s <no stats>", servers[i].c_str());
                }
            }
            dw.decIndent();
        }
        if (domains.empty()) {
            dw.println("No search domains defined");
        } else {
            std::string domains_str = android::base::Join(domains, ", ");
            dw.println("search domains: %s", domains_str.c_str());
        }
        if (params.sample_validity != 0) {
            dw.println(
                    "DNS parameters: sample validity = %us, success threshold = %u%%, "
                    "samples (min, max) = (%u, %u), base_timeout = %dmsec, retry count = "
                    "%dtimes",
                    params.sample_validity, params.success_threshold, params.min_samples,
                    params.max_samples, params.base_timeout_msec, params.retry_count);
        }

        mDns64Configuration.dump(dw, netId);
        ExternalPrivateDnsStatus privateDnsStatus = {PrivateDnsMode::OFF, 0, {}};
        gPrivateDnsConfiguration.getStatus(netId, &privateDnsStatus);
        dw.println("Private DNS mode: %s", getPrivateDnsModeString(privateDnsStatus.mode));
        if (!privateDnsStatus.numServers) {
            dw.println("No Private DNS servers configured");
        } else {
            dw.println("Private DNS configuration (%u entries)", privateDnsStatus.numServers);
            dw.incIndent();
            for (int i = 0; i < privateDnsStatus.numServers; i++) {
                dw.println("%s name{%s} status{%s}",
                           addrToString(&(privateDnsStatus.serverStatus[i].ss)).c_str(),
                           privateDnsStatus.serverStatus[i].hostname,
                           validationStatusToString(static_cast<Validation>(
                                   privateDnsStatus.serverStatus[i].validation)));
            }
            dw.decIndent();
        }
        dw.println("Concurrent DNS query timeout: %d", wait_for_pending_req_timeout_count[0]);
    }
    dw.decIndent();
}

}  // namespace net
}  // namespace android
