/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include "DnsProxyListener.h"

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <linux/if.h>
#include <math.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define LOG_TAG "DnsProxyListener"

#include <algorithm>
#include <vector>

#include <android-base/stringprintf.h>
#include <android/multinetwork.h>  // ResNsendFlags
#include <cutils/misc.h>           // FIRST_APPLICATION_UID
#include <netdutils/InternetAddresses.h>
#include <netdutils/OperationLimiter.h>
#include <netdutils/ResponseCode.h>
#include <netdutils/Slice.h>
#include <netdutils/Stopwatch.h>
#include <netdutils/ThreadUtil.h>
#include <private/android_filesystem_config.h>  // AID_SYSTEM
#include <statslog_resolv.h>
#include <sysutils/SocketClient.h>

#include "DnsResolver.h"
#include "NetdClient.h"  // NETID_USE_LOCAL_NAMESERVERS
#include "NetdPermissions.h"
#include "PrivateDnsConfiguration.h"
#include "ResolverEventReporter.h"
#include "getaddrinfo.h"
#include "gethnamaddr.h"
#include "netd_resolv/stats.h"  // RCODE_TIMEOUT
#include "res_send.h"
#include "resolv_cache.h"
#include "resolv_private.h"
#include "stats.pb.h"

using aidl::android::net::metrics::INetdEventListener;
using android::net::NetworkDnsEventReported;

namespace android {

using netdutils::ResponseCode;
using netdutils::Stopwatch;

namespace net {
namespace {

// Limits the number of outstanding DNS queries by client UID.
constexpr int MAX_QUERIES_PER_UID = 256;

// Max packet size for answer, sync with getaddrinfo.c
constexpr int MAXPACKET = 8 * 1024;

android::netdutils::OperationLimiter<uid_t> queryLimiter(MAX_QUERIES_PER_UID);

void logArguments(int argc, char** argv) {
    if (!WOULD_LOG(VERBOSE)) return;
    for (int i = 0; i < argc; i++) {
        LOG(VERBOSE) << __func__ << ": argv[" << i << "]=" << (argv[i] ? argv[i] : "null");
    }
}

template<typename T>
void tryThreadOrError(SocketClient* cli, T* handler) {
    cli->incRef();

    const int rval = netdutils::threadLaunch(handler);
    if (rval == 0) {
        // SocketClient decRef() happens in the handler's run() method.
        return;
    }

    char* msg = nullptr;
    asprintf(&msg, "%s (%d)", strerror(-rval), -rval);
    cli->sendMsg(ResponseCode::OperationFailed, msg, false);
    free(msg);

    delete handler;
    cli->decRef();
}

bool checkAndClearUseLocalNameserversFlag(unsigned* netid) {
    if (netid == nullptr || ((*netid) & NETID_USE_LOCAL_NAMESERVERS) == 0) {
        return false;
    }
    *netid = (*netid) & ~NETID_USE_LOCAL_NAMESERVERS;
    return true;
}

constexpr bool requestingUseLocalNameservers(unsigned flags) {
    return (flags & NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS) != 0;
}

inline bool queryingViaTls(unsigned dns_netid) {
    // TODO: The simpler PrivateDnsStatus should suffice here.
    ExternalPrivateDnsStatus privateDnsStatus = {PrivateDnsMode::OFF, 0, {}};
    gPrivateDnsConfiguration.getStatus(dns_netid, &privateDnsStatus);
    switch (static_cast<PrivateDnsMode>(privateDnsStatus.mode)) {
        case PrivateDnsMode::OPPORTUNISTIC:
            for (int i = 0; i < privateDnsStatus.numServers; i++) {
                if (privateDnsStatus.serverStatus[i].validation == Validation::success) {
                    return true;
                }
            }
            return false;
        case PrivateDnsMode::STRICT:
            return true;
        default:
            return false;
    }
}

bool hasPermissionToBypassPrivateDns(uid_t uid) {
    static_assert(AID_SYSTEM >= 0 && AID_SYSTEM < FIRST_APPLICATION_UID,
        "Calls from AID_SYSTEM must not result in a permission check to avoid deadlock.");
    if (uid >= 0 && uid < FIRST_APPLICATION_UID) {
        return true;
    }

    for (const char* const permission :
         {PERM_CONNECTIVITY_USE_RESTRICTED_NETWORKS, PERM_NETWORK_BYPASS_PRIVATE_DNS,
          PERM_MAINLINE_NETWORK_STACK}) {
        if (gResNetdCallbacks.check_calling_permission(permission)) {
            return true;
        }
    }
    return false;
}

void maybeFixupNetContext(android_net_context* ctx) {
    if (requestingUseLocalNameservers(ctx->flags) && !hasPermissionToBypassPrivateDns(ctx->uid)) {
        // Not permitted; clear the flag.
        ctx->flags &= ~NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
    }

    if (!requestingUseLocalNameservers(ctx->flags)) {
        // If we're not explicitly bypassing DNS-over-TLS servers, check whether
        // DNS-over-TLS is in use as an indicator for when to use more modern
        // DNS resolution mechanics.
        if (queryingViaTls(ctx->dns_netid)) {
            ctx->flags |= NET_CONTEXT_FLAG_USE_EDNS;
        }
    }
}

void addIpAddrWithinLimit(std::vector<std::string>* ip_addrs, const sockaddr* addr,
                          socklen_t addrlen);

int extractResNsendAnswers(const uint8_t* answer, size_t anslen, int ipType,
                           std::vector<std::string>* ip_addrs) {
    int total_ip_addr_count = 0;
    ns_msg handle;
    if (ns_initparse((const uint8_t*) answer, anslen, &handle) < 0) {
        return 0;
    }
    int ancount = ns_msg_count(handle, ns_s_an);
    ns_rr rr;
    for (int i = 0; i < ancount; i++) {
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0) {
            continue;
        }
        const uint8_t* rdata = ns_rr_rdata(rr);
        if (ipType == ns_t_a) {
            sockaddr_in sin = {.sin_family = AF_INET};
            memcpy(&sin.sin_addr, rdata, sizeof(sin.sin_addr));
            addIpAddrWithinLimit(ip_addrs, (sockaddr*) &sin, sizeof(sin));
            total_ip_addr_count++;
        } else if (ipType == ns_t_aaaa) {
            sockaddr_in6 sin6 = {.sin6_family = AF_INET6};
            memcpy(&sin6.sin6_addr, rdata, sizeof(sin6.sin6_addr));
            addIpAddrWithinLimit(ip_addrs, (sockaddr*) &sin6, sizeof(sin6));
            total_ip_addr_count++;
        }
    }

    return total_ip_addr_count;
}

int extractGetAddrInfoAnswers(const addrinfo* result, std::vector<std::string>* ip_addrs) {
    int total_ip_addr_count = 0;
    if (result == nullptr) {
        return 0;
    }
    for (const addrinfo* ai = result; ai; ai = ai->ai_next) {
        sockaddr* ai_addr = ai->ai_addr;
        if (ai_addr) {
            addIpAddrWithinLimit(ip_addrs, ai_addr, ai->ai_addrlen);
            total_ip_addr_count++;
        }
    }
    return total_ip_addr_count;
}

int extractGetHostByNameAnswers(const hostent* hp, std::vector<std::string>* ip_addrs) {
    int total_ip_addr_count = 0;
    if (hp == nullptr) {
        return 0;
    }
    if (hp->h_addrtype == AF_INET) {
        in_addr** list = (in_addr**) hp->h_addr_list;
        for (int i = 0; list[i] != nullptr; i++) {
            sockaddr_in sin = {.sin_family = AF_INET, .sin_addr = *list[i]};
            addIpAddrWithinLimit(ip_addrs, (sockaddr*) &sin, sizeof(sin));
            total_ip_addr_count++;
        }
    } else if (hp->h_addrtype == AF_INET6) {
        in6_addr** list = (in6_addr**) hp->h_addr_list;
        for (int i = 0; list[i] != nullptr; i++) {
            sockaddr_in6 sin6 = {.sin6_family = AF_INET6, .sin6_addr = *list[i]};
            addIpAddrWithinLimit(ip_addrs, (sockaddr*) &sin6, sizeof(sin6));
            total_ip_addr_count++;
        }
    }
    return total_ip_addr_count;
}

int rcodeToAiError(int rcode) {
    switch (rcode) {
        case NOERROR:
            return 0;
        case RCODE_TIMEOUT:
            return NETD_RESOLV_TIMEOUT;
        default:
            return EAI_NODATA;
    }
}

int resNSendToAiError(int err, int rcode) {
    if (err > 0) {
        return rcodeToAiError(rcode);
    }
    if (err == -ETIMEDOUT) {
        return NETD_RESOLV_TIMEOUT;
    }
    return EAI_SYSTEM;
}

template <typename IntegralType>
bool simpleStrtoul(const char* input, IntegralType* output, int base = 10) {
    char* endPtr;
    errno = 0;
    auto result = strtoul(input, &endPtr, base);
    // Check the length in order to ensure there is no "-" sign
    if (!*input || *endPtr || (endPtr - input) != static_cast<ptrdiff_t>(strlen(input)) ||
        (errno == ERANGE && (result == ULONG_MAX))) {
        return false;
    }
    *output = result;
    return true;
}

bool setQueryId(uint8_t* msg, size_t msgLen, uint16_t query_id) {
    if (msgLen < sizeof(HEADER)) {
        errno = EINVAL;
        return false;
    }
    auto hp = reinterpret_cast<HEADER*>(msg);
    hp->id = htons(query_id);
    return true;
}

bool parseQuery(const uint8_t* msg, size_t msgLen, uint16_t* query_id, int* rr_type,
                std::string* rr_name) {
    ns_msg handle;
    ns_rr rr;
    if (ns_initparse((const uint8_t*)msg, msgLen, &handle) < 0 ||
        ns_parserr(&handle, ns_s_qd, 0, &rr) < 0) {
        return false;
    }
    *query_id = ns_msg_id(handle);
    *rr_name = ns_rr_name(rr);
    *rr_type = ns_rr_type(rr);
    return true;
}

void initDnsEvent(NetworkDnsEventReported* event) {
    // The value 0 has the special meaning of unset/unknown in Westworld atoms.
    event->set_hints_ai_flags(-1);
    event->set_res_nsend_flags(-1);
}

// Return 0 if the event should not be logged.
// Otherwise, return subsampling_denom
uint32_t getDnsEventSubsamplingRate(int netid, int returnCode) {
    uint32_t subsampling_denom = resolv_cache_get_subsampling_denom(netid, returnCode);
    if (subsampling_denom == 0) return 0;
    // Sample the event with a chance of 1 / denom.
    return (arc4random_uniform(subsampling_denom) == 0) ? subsampling_denom : 0;
}

void reportDnsEvent(int eventType, const android_net_context& netContext, int latencyUs,
                    int returnCode, NetworkDnsEventReported& event, const std::string& query_name,
                    const std::vector<std::string>& ip_addrs = {}, int total_ip_addr_count = 0) {
    if (uint32_t rate = getDnsEventSubsamplingRate(netContext.dns_netid, returnCode)) {
        const std::string& dnsQueryStats = event.dns_query_events().SerializeAsString();
        stats::BytesField dnsQueryBytesField{dnsQueryStats.c_str(), dnsQueryStats.size()};
        event.set_return_code(static_cast<ReturnCode>(returnCode));
        android::net::stats::stats_write(android::net::stats::NETWORK_DNS_EVENT_REPORTED,
                                         event.event_type(), event.return_code(),
                                         event.latency_micros(), event.hints_ai_flags(),
                                         event.res_nsend_flags(), event.network_type(),
                                         event.private_dns_modes(), dnsQueryBytesField, rate);
    }

    const auto& listeners = ResolverEventReporter::getInstance().getListeners();
    if (listeners.size() == 0) {
        LOG(ERROR) << __func__
                   << ": DNS event not sent since no INetdEventListener receiver is available.";
        return;
    }
    const int latencyMs = latencyUs / 1000;
    for (const auto& it : listeners) {
        it->onDnsEvent(netContext.dns_netid, eventType, returnCode, latencyMs, query_name, ip_addrs,
                       total_ip_addr_count, netContext.uid);
    }
}

bool onlyIPv4Answers(const addrinfo* res) {
    // Null addrinfo pointer isn't checked because the caller doesn't pass null pointer.

    for (const addrinfo* ai = res; ai; ai = ai->ai_next)
        if (ai->ai_family != AF_INET) return false;

    return true;
}

bool isSpecialUseIPv4Address(const struct in_addr& ia) {
    const uint32_t addr = ntohl(ia.s_addr);

    // Only check necessary IP ranges in RFC 5735 section 4
    return ((addr & 0xff000000) == 0x00000000) ||  // "This" Network
           ((addr & 0xff000000) == 0x7f000000) ||  // Loopback
           ((addr & 0xffff0000) == 0xa9fe0000) ||  // Link Local
           ((addr & 0xf0000000) == 0xe0000000) ||  // Multicast
           (addr == INADDR_BROADCAST);             // Limited Broadcast
}

bool isSpecialUseIPv4Address(const struct sockaddr* sa) {
    if (sa->sa_family != AF_INET) return false;

    return isSpecialUseIPv4Address(((struct sockaddr_in*) sa)->sin_addr);
}

bool onlyNonSpecialUseIPv4Addresses(struct hostent* hp) {
    // Null hostent pointer isn't checked because the caller doesn't pass null pointer.

    if (hp->h_addrtype != AF_INET) return false;

    for (int i = 0; hp->h_addr_list[i] != nullptr; i++)
        if (isSpecialUseIPv4Address(*(struct in_addr*) hp->h_addr_list[i])) return false;

    return true;
}

bool onlyNonSpecialUseIPv4Addresses(const addrinfo* res) {
    // Null addrinfo pointer isn't checked because the caller doesn't pass null pointer.

    for (const addrinfo* ai = res; ai; ai = ai->ai_next) {
        if (ai->ai_family != AF_INET) return false;
        if (isSpecialUseIPv4Address(ai->ai_addr)) return false;
    }

    return true;
}

void logDnsQueryResult(const struct hostent* hp) {
    if (!WOULD_LOG(DEBUG)) return;
    if (hp == nullptr) return;

    LOG(DEBUG) << __func__ << ": DNS records:";
    for (int i = 0; hp->h_addr_list[i] != nullptr; i++) {
        char ip_addr[INET6_ADDRSTRLEN];
        if (inet_ntop(hp->h_addrtype, hp->h_addr_list[i], ip_addr, sizeof(ip_addr)) != nullptr) {
            LOG(DEBUG) << __func__ << ": [" << i << "] " << hp->h_addrtype;
        } else {
            PLOG(DEBUG) << __func__ << ": [" << i << "] numeric hostname translation fail";
        }
    }
}

void logDnsQueryResult(const addrinfo* res) {
    if (!WOULD_LOG(DEBUG)) return;
    if (res == nullptr) return;

    int i;
    const addrinfo* ai;
    LOG(DEBUG) << __func__ << ": DNS records:";
    for (ai = res, i = 0; ai; ai = ai->ai_next, i++) {
        if ((ai->ai_family != AF_INET) && (ai->ai_family != AF_INET6)) continue;
        char ip_addr[INET6_ADDRSTRLEN];
        int ret = getnameinfo(ai->ai_addr, ai->ai_addrlen, ip_addr, sizeof(ip_addr), nullptr, 0,
                              NI_NUMERICHOST);
        if (!ret) {
            LOG(DEBUG) << __func__ << ": [" << i << "] " << ai->ai_flags << " " << ai->ai_family
                       << " " << ai->ai_socktype << " " << ai->ai_protocol;
        } else {
            LOG(DEBUG) << __func__ << ": [" << i << "] numeric hostname translation fail " << ret;
        }
    }
}

bool isValidNat64Prefix(const netdutils::IPPrefix prefix) {
    if (prefix.family() != AF_INET6) {
        LOG(ERROR) << __func__ << ": Only IPv6 NAT64 prefixes are supported " << prefix.family();
        return false;
    }
    if (prefix.length() != 96) {
        LOG(ERROR) << __func__ << ": Only /96 NAT64 prefixes are supported " << prefix.length();
        return false;
    }
    return true;
}

bool synthesizeNat64PrefixWithARecord(const netdutils::IPPrefix& prefix, struct hostent* hp) {
    if (hp == nullptr) return false;
    if (!onlyNonSpecialUseIPv4Addresses(hp)) return false;
    if (!isValidNat64Prefix(prefix)) return false;

    struct sockaddr_storage ss = netdutils::IPSockAddr(prefix.ip());
    struct sockaddr_in6* v6prefix = (struct sockaddr_in6*) &ss;
    for (int i = 0; hp->h_addr_list[i] != nullptr; i++) {
        struct in_addr iaOriginal = *(struct in_addr*) hp->h_addr_list[i];
        struct in6_addr* ia6 = (struct in6_addr*) hp->h_addr_list[i];
        memset(ia6, 0, sizeof(struct in6_addr));

        // Synthesize /96 NAT64 prefix in place. The space has reserved by getanswer() and
        // _hf_gethtbyname2() in system/netd/resolv/gethnamaddr.cpp and
        // system/netd/resolv/sethostent.cpp.
        *ia6 = v6prefix->sin6_addr;
        ia6->s6_addr32[3] = iaOriginal.s_addr;

        if (WOULD_LOG(DEBUG)) {
            char buf[INET6_ADDRSTRLEN];  // big enough for either IPv4 or IPv6
            inet_ntop(AF_INET, &iaOriginal.s_addr, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": DNS A record: " << buf;
            inet_ntop(AF_INET6, &v6prefix->sin6_addr, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": NAT64 prefix: " << buf;
            inet_ntop(AF_INET6, ia6, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": DNS64 Synthesized AAAA record: " << buf;
        }
    }
    hp->h_addrtype = AF_INET6;
    hp->h_length = sizeof(in6_addr);

    logDnsQueryResult(hp);
    return true;
}

bool synthesizeNat64PrefixWithARecord(const netdutils::IPPrefix& prefix, addrinfo* result) {
    if (result == nullptr) return false;
    if (!onlyNonSpecialUseIPv4Addresses(result)) return false;
    if (!isValidNat64Prefix(prefix)) return false;

    struct sockaddr_storage ss = netdutils::IPSockAddr(prefix.ip());
    struct sockaddr_in6* v6prefix = (struct sockaddr_in6*) &ss;
    for (addrinfo* ai = result; ai; ai = ai->ai_next) {
        struct sockaddr_in sinOriginal = *(struct sockaddr_in*) ai->ai_addr;
        struct sockaddr_in6* sin6 = (struct sockaddr_in6*) ai->ai_addr;
        memset(sin6, 0, sizeof(sockaddr_in6));

        // Synthesize /96 NAT64 prefix in place. The space has reserved by get_ai() in
        // system/netd/resolv/getaddrinfo.cpp.
        sin6->sin6_addr = v6prefix->sin6_addr;
        sin6->sin6_addr.s6_addr32[3] = sinOriginal.sin_addr.s_addr;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = sinOriginal.sin_port;
        ai->ai_addrlen = sizeof(struct sockaddr_in6);
        ai->ai_family = AF_INET6;

        if (WOULD_LOG(DEBUG)) {
            char buf[INET6_ADDRSTRLEN];  // big enough for either IPv4 or IPv6
            inet_ntop(AF_INET, &sinOriginal.sin_addr.s_addr, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": DNS A record: " << buf;
            inet_ntop(AF_INET6, &v6prefix->sin6_addr, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": NAT64 prefix: " << buf;
            inet_ntop(AF_INET6, &sin6->sin6_addr, buf, sizeof(buf));
            LOG(DEBUG) << __func__ << ": DNS64 Synthesized AAAA record: " << buf;
        }
    }
    logDnsQueryResult(result);
    return true;
}

bool getDns64Prefix(unsigned netId, netdutils::IPPrefix* prefix) {
    return !gDnsResolv->resolverCtrl.getPrefix64(netId, prefix);
}

}  // namespace

DnsProxyListener::DnsProxyListener() : FrameworkListener(SOCKET_NAME) {
    registerCmd(new GetAddrInfoCmd());
    registerCmd(new GetHostByAddrCmd());
    registerCmd(new GetHostByNameCmd());
    registerCmd(new ResNSendCommand());
    registerCmd(new GetDnsNetIdCommand());
}

DnsProxyListener::GetAddrInfoHandler::GetAddrInfoHandler(SocketClient* c, char* host, char* service,
                                                         addrinfo* hints,
                                                         const android_net_context& netcontext)
    : mClient(c), mHost(host), mService(service), mHints(hints), mNetContext(netcontext) {}

DnsProxyListener::GetAddrInfoHandler::~GetAddrInfoHandler() {
    free(mHost);
    free(mService);
    free(mHints);
}

static bool sendBE32(SocketClient* c, uint32_t data) {
    uint32_t be_data = htonl(data);
    return c->sendData(&be_data, sizeof(be_data)) == 0;
}

// Sends 4 bytes of big-endian length, followed by the data.
// Returns true on success.
static bool sendLenAndData(SocketClient* c, const int len, const void* data) {
    return sendBE32(c, len) && (len == 0 || c->sendData(data, len) == 0);
}

// Returns true on success
static bool sendhostent(SocketClient* c, hostent* hp) {
    bool success = true;
    int i;
    if (hp->h_name != nullptr) {
        success &= sendLenAndData(c, strlen(hp->h_name)+1, hp->h_name);
    } else {
        success &= sendLenAndData(c, 0, "") == 0;
    }

    for (i=0; hp->h_aliases[i] != nullptr; i++) {
        success &= sendLenAndData(c, strlen(hp->h_aliases[i])+1, hp->h_aliases[i]);
    }
    success &= sendLenAndData(c, 0, ""); // null to indicate we're done

    uint32_t buf = htonl(hp->h_addrtype);
    success &= c->sendData(&buf, sizeof(buf)) == 0;

    buf = htonl(hp->h_length);
    success &= c->sendData(&buf, sizeof(buf)) == 0;

    for (i=0; hp->h_addr_list[i] != nullptr; i++) {
        success &= sendLenAndData(c, 16, hp->h_addr_list[i]);
    }
    success &= sendLenAndData(c, 0, ""); // null to indicate we're done
    return success;
}

static bool sendaddrinfo(SocketClient* c, addrinfo* ai) {
    // struct addrinfo {
    //      int     ai_flags;       /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
    //      int     ai_family;      /* PF_xxx */
    //      int     ai_socktype;    /* SOCK_xxx */
    //      int     ai_protocol;    /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
    //      socklen_t ai_addrlen;   /* length of ai_addr */
    //      char    *ai_canonname;  /* canonical name for hostname */
    //      struct  sockaddr *ai_addr;      /* binary address */
    //      struct  addrinfo *ai_next;      /* next structure in linked list */
    // };

    // Write the struct piece by piece because we might be a 64-bit netd
    // talking to a 32-bit process.
    bool success =
            sendBE32(c, ai->ai_flags) &&
            sendBE32(c, ai->ai_family) &&
            sendBE32(c, ai->ai_socktype) &&
            sendBE32(c, ai->ai_protocol);
    if (!success) {
        return false;
    }

    // ai_addrlen and ai_addr.
    if (!sendLenAndData(c, ai->ai_addrlen, ai->ai_addr)) {
        return false;
    }

    // strlen(ai_canonname) and ai_canonname.
    if (!sendLenAndData(c, ai->ai_canonname ? strlen(ai->ai_canonname) + 1 : 0, ai->ai_canonname)) {
        return false;
    }

    return true;
}

void DnsProxyListener::GetAddrInfoHandler::doDns64Synthesis(int32_t* rv, addrinfo** res,
                                                            NetworkDnsEventReported* event) {
    if (mHost == nullptr) return;

    const bool ipv6WantedButNoData = (mHints && mHints->ai_family == AF_INET6 && *rv == EAI_NODATA);
    const bool unspecWantedButNoIPv6 =
            ((!mHints || mHints->ai_family == AF_UNSPEC) && *rv == 0 && onlyIPv4Answers(*res));

    if (!ipv6WantedButNoData && !unspecWantedButNoIPv6) {
        return;
    }

    netdutils::IPPrefix prefix{};
    if (!getDns64Prefix(mNetContext.dns_netid, &prefix)) {
        return;
    }

    if (ipv6WantedButNoData) {
        // If caller wants IPv6 answers but no data, try to query IPv4 answers for synthesis
        const uid_t uid = mClient->getUid();
        if (queryLimiter.start(uid)) {
            mHints->ai_family = AF_INET;
            // Don't need to do freeaddrinfo(res) before starting new DNS lookup because previous
            // DNS lookup is failed with error EAI_NODATA.
            *rv = android_getaddrinfofornetcontext(mHost, mService, mHints, &mNetContext, res,
                                                   event);
            queryLimiter.finish(uid);
            if (*rv) {
                *rv = EAI_NODATA;  // return original error code
                return;
            }
        } else {
            LOG(ERROR) << __func__ << ": from UID " << uid << ", max concurrent queries reached";
            return;
        }
    }

    if (!synthesizeNat64PrefixWithARecord(prefix, *res)) {
        if (ipv6WantedButNoData) {
            // If caller wants IPv6 answers but no data and failed to synthesize IPv6 answers,
            // don't return the IPv4 answers.
            *rv = EAI_NODATA;  // return original error code
            if (*res) {
                freeaddrinfo(*res);
                *res = nullptr;
            }
        }
    }
}

void DnsProxyListener::GetAddrInfoHandler::run() {
    LOG(DEBUG) << "GetAddrInfoHandler::run: {" << mNetContext.app_netid << " "
               << mNetContext.app_mark << " " << mNetContext.dns_netid << " "
               << mNetContext.dns_mark << " " << mNetContext.uid << " " << mNetContext.flags << "}";

    addrinfo* result = nullptr;
    Stopwatch s;
    maybeFixupNetContext(&mNetContext);
    const uid_t uid = mClient->getUid();
    int32_t rv = 0;
    NetworkDnsEventReported event;
    initDnsEvent(&event);
    if (queryLimiter.start(uid)) {
        rv = android_getaddrinfofornetcontext(mHost, mService, mHints, &mNetContext, &result,
                                              &event);
        queryLimiter.finish(uid);
    } else {
        // Note that this error code is currently not passed down to the client.
        // android_getaddrinfo_proxy() returns EAI_NODATA on any error.
        rv = EAI_MEMORY;
        LOG(ERROR) << "GetAddrInfoHandler::run: from UID " << uid
                   << ", max concurrent queries reached";
    }

    doDns64Synthesis(&rv, &result, &event);
    const int32_t latencyUs = saturate_cast<int32_t>(s.timeTakenUs());
    event.set_latency_micros(latencyUs);
    event.set_event_type(EVENT_GETADDRINFO);
    event.set_hints_ai_flags((mHints ? mHints->ai_flags : 0));

    if (rv) {
        // getaddrinfo failed
        mClient->sendBinaryMsg(ResponseCode::DnsProxyOperationFailed, &rv, sizeof(rv));
    } else {
        bool success = !mClient->sendCode(ResponseCode::DnsProxyQueryResult);
        addrinfo* ai = result;
        while (ai && success) {
            success = sendBE32(mClient, 1) && sendaddrinfo(mClient, ai);
            ai = ai->ai_next;
        }
        success = success && sendBE32(mClient, 0);
        if (!success) {
            LOG(WARNING) << "GetAddrInfoHandler::run: Error writing DNS result to client";
        }
    }
    std::vector<std::string> ip_addrs;
    const int total_ip_addr_count = extractGetAddrInfoAnswers(result, &ip_addrs);
    reportDnsEvent(INetdEventListener::EVENT_GETADDRINFO, mNetContext, latencyUs, rv, event, mHost,
                   ip_addrs, total_ip_addr_count);
    freeaddrinfo(result);
    mClient->decRef();
}

namespace {

void addIpAddrWithinLimit(std::vector<std::string>* ip_addrs, const sockaddr* addr,
                          socklen_t addrlen) {
    // ipAddresses array is limited to first INetdEventListener::DNS_REPORTED_IP_ADDRESSES_LIMIT
    // addresses for A and AAAA. Total count of addresses is provided, to be able to tell whether
    // some addresses didn't get logged.
    if (ip_addrs->size() < INetdEventListener::DNS_REPORTED_IP_ADDRESSES_LIMIT) {
        char ip_addr[INET6_ADDRSTRLEN];
        if (getnameinfo(addr, addrlen, ip_addr, sizeof(ip_addr), nullptr, 0, NI_NUMERICHOST) == 0) {
            ip_addrs->push_back(std::string(ip_addr));
        }
    }
}

}  // namespace

DnsProxyListener::GetAddrInfoCmd::GetAddrInfoCmd() : FrameworkCommand("getaddrinfo") {}

int DnsProxyListener::GetAddrInfoCmd::runCommand(SocketClient *cli,
                                            int argc, char **argv) {
    logArguments(argc, argv);

    if (argc != 8) {
        char* msg = nullptr;
        asprintf( &msg, "Invalid number of arguments to getaddrinfo: %i", argc);
        LOG(WARNING) << "GetAddrInfoCmd::runCommand: " << (msg ? msg : "null");
        cli->sendMsg(ResponseCode::CommandParameterError, msg, false);
        free(msg);
        return -1;
    }

    char* name = argv[1];
    if (strcmp("^", name) == 0) {
        name = nullptr;
    } else {
        name = strdup(name);
    }

    char* service = argv[2];
    if (strcmp("^", service) == 0) {
        service = nullptr;
    } else {
        service = strdup(service);
    }

    addrinfo* hints = nullptr;
    int ai_flags = strtol(argv[3], nullptr, 10);
    int ai_family = strtol(argv[4], nullptr, 10);
    int ai_socktype = strtol(argv[5], nullptr, 10);
    int ai_protocol = strtol(argv[6], nullptr, 10);
    unsigned netId = strtoul(argv[7], nullptr, 10);
    const bool useLocalNameservers = checkAndClearUseLocalNameserversFlag(&netId);
    const uid_t uid = cli->getUid();

    android_net_context netcontext;
    gResNetdCallbacks.get_network_context(netId, uid, &netcontext);

    if (useLocalNameservers) {
        netcontext.flags |= NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
    }

    if (ai_flags != -1 || ai_family != -1 ||
        ai_socktype != -1 || ai_protocol != -1) {
        hints = (addrinfo*) calloc(1, sizeof(addrinfo));
        hints->ai_flags = ai_flags;
        hints->ai_family = ai_family;
        hints->ai_socktype = ai_socktype;
        hints->ai_protocol = ai_protocol;
    }

    DnsProxyListener::GetAddrInfoHandler* handler =
            new DnsProxyListener::GetAddrInfoHandler(cli, name, service, hints, netcontext);
    tryThreadOrError(cli, handler);
    return 0;
}

/*******************************************************
 *                  ResNSendCommand                    *
 *******************************************************/
DnsProxyListener::ResNSendCommand::ResNSendCommand() : FrameworkCommand("resnsend") {}

int DnsProxyListener::ResNSendCommand::runCommand(SocketClient* cli, int argc, char** argv) {
    logArguments(argc, argv);

    const uid_t uid = cli->getUid();
    if (argc != 4) {
        LOG(WARNING) << "ResNSendCommand::runCommand: resnsend: from UID " << uid
                     << ", invalid number of arguments to resnsend: " << argc;
        sendBE32(cli, -EINVAL);
        return -1;
    }

    unsigned netId;
    if (!simpleStrtoul(argv[1], &netId)) {
        LOG(WARNING) << "ResNSendCommand::runCommand: resnsend: from UID " << uid
                     << ", invalid netId";
        sendBE32(cli, -EINVAL);
        return -1;
    }

    uint32_t flags;
    if (!simpleStrtoul(argv[2], &flags)) {
        LOG(WARNING) << "ResNSendCommand::runCommand: resnsend: from UID " << uid
                     << ", invalid flags";
        sendBE32(cli, -EINVAL);
        return -1;
    }

    const bool useLocalNameservers = checkAndClearUseLocalNameserversFlag(&netId);

    android_net_context netcontext;
    gResNetdCallbacks.get_network_context(netId, uid, &netcontext);

    if (useLocalNameservers) {
        netcontext.flags |= NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
    }

    DnsProxyListener::ResNSendHandler* handler =
            new DnsProxyListener::ResNSendHandler(cli, argv[3], flags, netcontext);
    tryThreadOrError(cli, handler);
    return 0;
}

DnsProxyListener::ResNSendHandler::ResNSendHandler(SocketClient* c, std::string msg, uint32_t flags,
                                                   const android_net_context& netcontext)
    : mClient(c), mMsg(std::move(msg)), mFlags(flags), mNetContext(netcontext) {}

DnsProxyListener::ResNSendHandler::~ResNSendHandler() {
    mClient->decRef();
}

void DnsProxyListener::ResNSendHandler::run() {
    LOG(DEBUG) << "ResNSendHandler::run: " << mFlags << " / {" << mNetContext.app_netid << " "
               << mNetContext.app_mark << " " << mNetContext.dns_netid << " "
               << mNetContext.dns_mark << " " << mNetContext.uid << " " << mNetContext.flags << "}";

    Stopwatch s;
    maybeFixupNetContext(&mNetContext);

    // Decode
    std::vector<uint8_t> msg(MAXPACKET, 0);

    // Max length of mMsg is less than 1024 since the CMD_BUF_SIZE in FrameworkListener is 1024
    int msgLen = b64_pton(mMsg.c_str(), msg.data(), MAXPACKET);
    if (msgLen == -1) {
        // Decode fail
        sendBE32(mClient, -EILSEQ);
        return;
    }

    const uid_t uid = mClient->getUid();
    int rr_type = 0;
    std::string rr_name;
    uint16_t original_query_id = 0;

    // TODO: Handle the case which is msg contains more than one query
    if (!parseQuery(msg.data(), msgLen, &original_query_id, &rr_type, &rr_name) ||
        !setQueryId(msg.data(), msgLen, arc4random_uniform(65536))) {
        // If the query couldn't be parsed, block the request.
        LOG(WARNING) << "ResNSendHandler::run: resnsend: from UID " << uid << ", invalid query";
        sendBE32(mClient, -EINVAL);
        return;
    }

    // Send DNS query
    std::vector<uint8_t> ansBuf(MAXPACKET, 0);
    int arcode, nsendAns = -1;
    NetworkDnsEventReported event;
    initDnsEvent(&event);
    if (queryLimiter.start(uid)) {
        nsendAns = resolv_res_nsend(&mNetContext, msg.data(), msgLen, ansBuf.data(), MAXPACKET,
                                    &arcode, static_cast<ResNsendFlags>(mFlags), &event);
        queryLimiter.finish(uid);
    } else {
        LOG(WARNING) << "ResNSendHandler::run: resnsend: from UID " << uid
                     << ", max concurrent queries reached";
        nsendAns = -EBUSY;
    }

    const int32_t latencyUs = saturate_cast<int32_t>(s.timeTakenUs());
    event.set_latency_micros(latencyUs);
    event.set_event_type(EVENT_RES_NSEND);
    event.set_res_nsend_flags(static_cast<ResNsendFlags>(mFlags));

    // Fail, send -errno
    if (nsendAns < 0) {
        sendBE32(mClient, nsendAns);
        if (rr_type == ns_t_a || rr_type == ns_t_aaaa) {
            reportDnsEvent(INetdEventListener::EVENT_RES_NSEND, mNetContext, latencyUs,
                           resNSendToAiError(nsendAns, arcode), event, rr_name);
        }
        return;
    }

    // Send rcode
    if (!sendBE32(mClient, arcode)) {
        PLOG(WARNING) << "ResNSendHandler::run: resnsend: failed to send rcode to uid " << uid;
        return;
    }

    // Restore query id and send answer
    if (!setQueryId(ansBuf.data(), nsendAns, original_query_id) ||
        !sendLenAndData(mClient, nsendAns, ansBuf.data())) {
        PLOG(WARNING) << "ResNSendHandler::run: resnsend: failed to send answer to uid " << uid;
        return;
    }

    if (rr_type == ns_t_a || rr_type == ns_t_aaaa) {
        std::vector<std::string> ip_addrs;
        const int total_ip_addr_count =
                extractResNsendAnswers((uint8_t*) ansBuf.data(), nsendAns, rr_type, &ip_addrs);
        reportDnsEvent(INetdEventListener::EVENT_RES_NSEND, mNetContext, latencyUs,
                       resNSendToAiError(nsendAns, arcode), event, rr_name, ip_addrs,
                       total_ip_addr_count);
    }
}

namespace {

bool sendCodeAndBe32(SocketClient* c, int code, int data) {
    return !c->sendCode(code) && sendBE32(c, data);
}

}  // namespace

/*******************************************************
 *                  GetDnsNetId                        *
 *******************************************************/
DnsProxyListener::GetDnsNetIdCommand::GetDnsNetIdCommand() : FrameworkCommand("getdnsnetid") {}

int DnsProxyListener::GetDnsNetIdCommand::runCommand(SocketClient* cli, int argc, char** argv) {
    logArguments(argc, argv);

    const uid_t uid = cli->getUid();
    if (argc != 2) {
        LOG(WARNING) << "GetDnsNetIdCommand::runCommand: getdnsnetid: from UID " << uid
                     << ", invalid number of arguments to getdnsnetid: " << argc;
        sendCodeAndBe32(cli, ResponseCode::DnsProxyQueryResult, -EINVAL);
        return -1;
    }

    unsigned netId;
    if (!simpleStrtoul(argv[1], &netId)) {
        LOG(WARNING) << "GetDnsNetIdCommand::runCommand: getdnsnetid: from UID " << uid
                     << ", invalid netId";
        sendCodeAndBe32(cli, ResponseCode::DnsProxyQueryResult, -EINVAL);
        return -1;
    }

    const bool useLocalNameservers = checkAndClearUseLocalNameserversFlag(&netId);
    android_net_context netcontext;
    gResNetdCallbacks.get_network_context(netId, uid, &netcontext);

    if (useLocalNameservers) {
        netcontext.app_netid |= NETID_USE_LOCAL_NAMESERVERS;
    }

    return sendCodeAndBe32(cli, ResponseCode::DnsProxyQueryResult, netcontext.app_netid) ? 0 : -1;
}

/*******************************************************
 *                  GetHostByName                      *
 *******************************************************/
DnsProxyListener::GetHostByNameCmd::GetHostByNameCmd() : FrameworkCommand("gethostbyname") {}

int DnsProxyListener::GetHostByNameCmd::runCommand(SocketClient *cli,
                                            int argc, char **argv) {
    logArguments(argc, argv);

    if (argc != 4) {
        char* msg = nullptr;
        asprintf(&msg, "Invalid number of arguments to gethostbyname: %i", argc);
        LOG(WARNING) << "GetHostByNameCmd::runCommand: " << (msg ? msg : "null");
        cli->sendMsg(ResponseCode::CommandParameterError, msg, false);
        free(msg);
        return -1;
    }

    uid_t uid = cli->getUid();
    unsigned netId = strtoul(argv[1], nullptr, 10);
    const bool useLocalNameservers = checkAndClearUseLocalNameserversFlag(&netId);
    char* name = argv[2];
    int af = strtol(argv[3], nullptr, 10);

    if (strcmp(name, "^") == 0) {
        name = nullptr;
    } else {
        name = strdup(name);
    }

    android_net_context netcontext;
    gResNetdCallbacks.get_network_context(netId, uid, &netcontext);

    if (useLocalNameservers) {
        netcontext.flags |= NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
    }

    DnsProxyListener::GetHostByNameHandler* handler =
            new DnsProxyListener::GetHostByNameHandler(cli, name, af, netcontext);
    tryThreadOrError(cli, handler);
    return 0;
}

DnsProxyListener::GetHostByNameHandler::GetHostByNameHandler(SocketClient* c, char* name, int af,
                                                             const android_net_context& netcontext)
    : mClient(c), mName(name), mAf(af), mNetContext(netcontext) {}

DnsProxyListener::GetHostByNameHandler::~GetHostByNameHandler() {
    free(mName);
}

void DnsProxyListener::GetHostByNameHandler::doDns64Synthesis(int32_t* rv, struct hostent** hpp,
                                                              NetworkDnsEventReported* event) {
    // Don't have to consider family AF_UNSPEC case because gethostbyname{, 2} only supports
    // family AF_INET or AF_INET6.
    const bool ipv6WantedButNoData = (mAf == AF_INET6 && *rv == EAI_NODATA);

    if (!ipv6WantedButNoData) {
        return;
    }

    netdutils::IPPrefix prefix{};
    if (!getDns64Prefix(mNetContext.dns_netid, &prefix)) {
        return;
    }

    // If caller wants IPv6 answers but no data, try to query IPv4 answers for synthesis
    const uid_t uid = mClient->getUid();
    if (queryLimiter.start(uid)) {
        *rv = android_gethostbynamefornetcontext(mName, AF_INET, &mNetContext, hpp, event);
        queryLimiter.finish(uid);
        if (*rv) {
            *rv = EAI_NODATA;  // return original error code
            return;
        }
    } else {
        LOG(ERROR) << __func__ << ": from UID " << uid << ", max concurrent queries reached";
        return;
    }

    if (!synthesizeNat64PrefixWithARecord(prefix, *hpp)) {
        // If caller wants IPv6 answers but no data and failed to synthesize IPv4 answers,
        // don't return the IPv4 answers.
        *hpp = nullptr;
    }
}

void DnsProxyListener::GetHostByNameHandler::run() {
    Stopwatch s;
    maybeFixupNetContext(&mNetContext);
    const uid_t uid = mClient->getUid();
    hostent* hp = nullptr;
    int32_t rv = 0;
    NetworkDnsEventReported event;
    initDnsEvent(&event);
    if (queryLimiter.start(uid)) {
        rv = android_gethostbynamefornetcontext(mName, mAf, &mNetContext, &hp, &event);
        queryLimiter.finish(uid);
    } else {
        rv = EAI_MEMORY;
        LOG(ERROR) << "GetHostByNameHandler::run: from UID " << uid
                   << ", max concurrent queries reached";
    }

    doDns64Synthesis(&rv, &hp, &event);
    const int32_t latencyUs = saturate_cast<int32_t>(s.timeTakenUs());
    event.set_latency_micros(latencyUs);
    event.set_event_type(EVENT_GETHOSTBYNAME);

    LOG(DEBUG) << "GetHostByNameHandler::run: errno: " << (hp ? "success" : strerror(errno));

    bool success = true;
    if (hp) {
        // hp is not nullptr iff. rv is 0.
        success = mClient->sendCode(ResponseCode::DnsProxyQueryResult) == 0;
        success &= sendhostent(mClient, hp);
    } else {
        success = mClient->sendBinaryMsg(ResponseCode::DnsProxyOperationFailed, nullptr, 0) == 0;
    }

    if (!success) {
        LOG(WARNING) << "GetHostByNameHandler::run: Error writing DNS result to client";
    }

    std::vector<std::string> ip_addrs;
    const int total_ip_addr_count = extractGetHostByNameAnswers(hp, &ip_addrs);
    reportDnsEvent(INetdEventListener::EVENT_GETHOSTBYNAME, mNetContext, latencyUs, rv, event,
                   mName, ip_addrs, total_ip_addr_count);
    mClient->decRef();
}


/*******************************************************
 *                  GetHostByAddr                      *
 *******************************************************/
DnsProxyListener::GetHostByAddrCmd::GetHostByAddrCmd() : FrameworkCommand("gethostbyaddr") {}

int DnsProxyListener::GetHostByAddrCmd::runCommand(SocketClient *cli,
                                            int argc, char **argv) {
    logArguments(argc, argv);

    if (argc != 5) {
        char* msg = nullptr;
        asprintf(&msg, "Invalid number of arguments to gethostbyaddr: %i", argc);
        LOG(WARNING) << "GetHostByAddrCmd::runCommand: " << (msg ? msg : "null");
        cli->sendMsg(ResponseCode::CommandParameterError, msg, false);
        free(msg);
        return -1;
    }

    char* addrStr = argv[1];
    int addrLen = strtol(argv[2], nullptr, 10);
    int addrFamily = strtol(argv[3], nullptr, 10);
    uid_t uid = cli->getUid();
    unsigned netId = strtoul(argv[4], nullptr, 10);
    const bool useLocalNameservers = checkAndClearUseLocalNameserversFlag(&netId);

    void* addr = malloc(sizeof(in6_addr));
    errno = 0;
    int result = inet_pton(addrFamily, addrStr, addr);
    if (result <= 0) {
        char* msg = nullptr;
        asprintf(&msg, "inet_pton(\"%s\") failed %s", addrStr, strerror(errno));
        LOG(WARNING) << "GetHostByAddrCmd::runCommand: " << (msg ? msg : "null");
        cli->sendMsg(ResponseCode::OperationFailed, msg, false);
        free(addr);
        free(msg);
        return -1;
    }

    android_net_context netcontext;
    gResNetdCallbacks.get_network_context(netId, uid, &netcontext);

    if (useLocalNameservers) {
        netcontext.flags |= NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS;
    }

    DnsProxyListener::GetHostByAddrHandler* handler = new DnsProxyListener::GetHostByAddrHandler(
            cli, addr, addrLen, addrFamily, netcontext);
    tryThreadOrError(cli, handler);
    return 0;
}

DnsProxyListener::GetHostByAddrHandler::GetHostByAddrHandler(SocketClient* c, void* address,
                                                             int addressLen, int addressFamily,
                                                             const android_net_context& netcontext)
    : mClient(c),
      mAddress(address),
      mAddressLen(addressLen),
      mAddressFamily(addressFamily),
      mNetContext(netcontext) {}

DnsProxyListener::GetHostByAddrHandler::~GetHostByAddrHandler() {
    free(mAddress);
}

void DnsProxyListener::GetHostByAddrHandler::doDns64ReverseLookup(struct hostent** hpp,
                                                                  NetworkDnsEventReported* event) {
    if (*hpp != nullptr || mAddressFamily != AF_INET6 || !mAddress) {
        return;
    }

    netdutils::IPPrefix prefix{};
    if (!getDns64Prefix(mNetContext.dns_netid, &prefix)) {
        return;
    }

    if (!isValidNat64Prefix(prefix)) {
        return;
    }

    struct sockaddr_storage ss = netdutils::IPSockAddr(prefix.ip());
    struct sockaddr_in6* v6prefix = (struct sockaddr_in6*) &ss;
    struct in6_addr v6addr = *(in6_addr*) mAddress;
    // Check if address has NAT64 prefix. Only /96 IPv6 NAT64 prefixes are supported
    if ((v6addr.s6_addr32[0] != v6prefix->sin6_addr.s6_addr32[0]) ||
        (v6addr.s6_addr32[1] != v6prefix->sin6_addr.s6_addr32[1]) ||
        (v6addr.s6_addr32[2] != v6prefix->sin6_addr.s6_addr32[2])) {
        return;
    }

    const uid_t uid = mClient->getUid();
    if (queryLimiter.start(uid)) {
        // Remove NAT64 prefix and do reverse DNS query
        struct in_addr v4addr = {.s_addr = v6addr.s6_addr32[3]};
        android_gethostbyaddrfornetcontext(&v4addr, sizeof(v4addr), AF_INET, &mNetContext, hpp,
                                           event);
        queryLimiter.finish(uid);
        if (*hpp) {
            // Replace IPv4 address with original queried IPv6 address in place. The space has
            // reserved by dns_gethtbyaddr() and netbsd_gethostent_r() in
            // system/netd/resolv/gethnamaddr.cpp.
            // Note that android_gethostbyaddrfornetcontext returns only one entry in result.
            memcpy((*hpp)->h_addr_list[0], &v6addr, sizeof(v6addr));
            (*hpp)->h_addrtype = AF_INET6;
            (*hpp)->h_length = sizeof(struct in6_addr);
        }
    } else {
        LOG(ERROR) << __func__ << ": from UID " << uid << ", max concurrent queries reached";
    }
}

void DnsProxyListener::GetHostByAddrHandler::run() {
    Stopwatch s;
    maybeFixupNetContext(&mNetContext);
    const uid_t uid = mClient->getUid();
    hostent* hp = nullptr;
    int32_t rv = 0;
    NetworkDnsEventReported event;
    initDnsEvent(&event);
    if (queryLimiter.start(uid)) {
        rv = android_gethostbyaddrfornetcontext(mAddress, mAddressLen, mAddressFamily, &mNetContext,
                                                &hp, &event);
        queryLimiter.finish(uid);
    } else {
        rv = EAI_MEMORY;
        LOG(ERROR) << "GetHostByAddrHandler::run: from UID " << uid
                   << ", max concurrent queries reached";
    }

    doDns64ReverseLookup(&hp, &event);
    const int32_t latencyUs = saturate_cast<int32_t>(s.timeTakenUs());
    event.set_latency_micros(latencyUs);
    event.set_event_type(EVENT_GETHOSTBYADDR);

    LOG(DEBUG) << "GetHostByAddrHandler::run: result: " << (hp ? "success" : gai_strerror(rv));

    bool success = true;
    if (hp) {
        success = mClient->sendCode(ResponseCode::DnsProxyQueryResult) == 0;
        success &= sendhostent(mClient, hp);
    } else {
        success = mClient->sendBinaryMsg(ResponseCode::DnsProxyOperationFailed, nullptr, 0) == 0;
    }

    if (!success) {
        LOG(WARNING) << "GetHostByAddrHandler::run: Error writing DNS result to client";
    }

    reportDnsEvent(INetdEventListener::EVENT_GETHOSTBYADDR, mNetContext, latencyUs, rv, event,
                   (hp && hp->h_name) ? hp->h_name : "null", {}, 0);
    mClient->decRef();
}

}  // namespace net
}  // namespace android
