/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless requied by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define LOG_TAG "resolv_integration_test"

#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h> /* poll */
#include <resolv.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <numeric>
#include <thread>

#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>
#include <android/multinetwork.h>  // ResNsendFlags
#include <cutils/sockets.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <openssl/base64.h>
#include <private/android_filesystem_config.h>
#include <utils/Log.h>

#include "NetdClient.h"
#include "netid_client.h"  // NETID_UNSET
#include "netd_resolv/params.h"  // MAX_NS

#include "dns_responder/dns_responder.h"
#include "dns_responder/dns_responder_client.h"
#include "dns_responder/dns_tls_frontend.h"

#include "NetdConstants.h"
#include "ResolverStats.h"

#include "android/net/IDnsResolver.h"
#include "binder/IServiceManager.h"
#include "netdutils/ResponseCode.h"
#include "netdutils/SocketOption.h"

// TODO: make this dynamic and stop depending on implementation details.
constexpr int TEST_NETID = 30;
// Valid VPN netId range is 100 ~ 65535
constexpr int TEST_VPN_NETID = 65502;
constexpr int MAXPACKET = (8 * 1024);

// Semi-public Bionic hook used by the NDK (frameworks/base/native/android/net.c)
// Tested here for convenience.
extern "C" int android_getaddrinfofornet(const char* hostname, const char* servname,
                                         const addrinfo* hints, unsigned netid, unsigned mark,
                                         struct addrinfo** result);

using android::base::ParseInt;
using android::base::StringPrintf;
using android::base::unique_fd;
using android::net::ResolverStats;
using android::netdutils::enableSockopt;
using android::netdutils::ResponseCode;

// TODO: move into libnetdutils?
namespace {
ScopedAddrinfo safe_getaddrinfo(const char* node, const char* service,
                                const struct addrinfo* hints) {
    addrinfo* result = nullptr;
    if (getaddrinfo(node, service, hints, &result) != 0) {
        result = nullptr;  // Should already be the case, but...
    }
    return ScopedAddrinfo(result);
}
}  // namespace

class ResolverTest : public ::testing::Test {
  protected:
    struct DnsRecord {
        std::string host_name;  // host name
        ns_type type;           // record type
        std::string addr;       // ipv4/v6 address
    };

    void SetUp() { mDnsClient.SetUp(); }
    void TearDown() {
        mDnsClient.TearDown();
    }

    bool GetResolverInfo(std::vector<std::string>* servers, std::vector<std::string>* domains,
                         std::vector<std::string>* tlsServers, res_params* params,
                         std::vector<ResolverStats>* stats,
                         int* wait_for_pending_req_timeout_count) {
        using android::net::IDnsResolver;
        std::vector<int32_t> params32;
        std::vector<int32_t> stats32;
        std::vector<int32_t> wait_for_pending_req_timeout_count32{0};
        auto rv = mDnsClient.resolvService()->getResolverInfo(
                TEST_NETID, servers, domains, tlsServers, &params32, &stats32,
                &wait_for_pending_req_timeout_count32);

        if (!rv.isOk() ||
            params32.size() != static_cast<size_t>(IDnsResolver::RESOLVER_PARAMS_COUNT)) {
            return false;
        }
        *params = res_params{
                .sample_validity = static_cast<uint16_t>(
                        params32[IDnsResolver::RESOLVER_PARAMS_SAMPLE_VALIDITY]),
                .success_threshold = static_cast<uint8_t>(
                        params32[IDnsResolver::RESOLVER_PARAMS_SUCCESS_THRESHOLD]),
                .min_samples =
                        static_cast<uint8_t>(params32[IDnsResolver::RESOLVER_PARAMS_MIN_SAMPLES]),
                .max_samples =
                        static_cast<uint8_t>(params32[IDnsResolver::RESOLVER_PARAMS_MAX_SAMPLES]),
                .base_timeout_msec = params32[IDnsResolver::RESOLVER_PARAMS_BASE_TIMEOUT_MSEC],
                .retry_count = params32[IDnsResolver::RESOLVER_PARAMS_RETRY_COUNT],
        };
        *wait_for_pending_req_timeout_count = wait_for_pending_req_timeout_count32[0];
        return ResolverStats::decodeAll(stats32, stats);
    }

    static std::string ToString(const hostent* he) {
        if (he == nullptr) return "<null>";
        char buffer[INET6_ADDRSTRLEN];
        if (!inet_ntop(he->h_addrtype, he->h_addr_list[0], buffer, sizeof(buffer))) {
            return "<invalid>";
        }
        return buffer;
    }

    static std::string ToString(const addrinfo* ai) {
        if (!ai)
            return "<null>";
        for (const auto* aip = ai ; aip != nullptr ; aip = aip->ai_next) {
            char host[NI_MAXHOST];
            int rv = getnameinfo(aip->ai_addr, aip->ai_addrlen, host, sizeof(host), nullptr, 0,
                    NI_NUMERICHOST);
            if (rv != 0)
                return gai_strerror(rv);
            return host;
        }
        return "<invalid>";
    }

    static std::string ToString(const ScopedAddrinfo& ai) { return ToString(ai.get()); }

    static std::vector<std::string> ToStrings(const addrinfo* ai) {
        std::vector<std::string> hosts;
        if (!ai) {
            hosts.push_back("<null>");
            return hosts;
        }
        for (const auto* aip = ai; aip != nullptr; aip = aip->ai_next) {
            char host[NI_MAXHOST];
            int rv = getnameinfo(aip->ai_addr, aip->ai_addrlen, host, sizeof(host), nullptr, 0,
                                 NI_NUMERICHOST);
            if (rv != 0) {
                hosts.clear();
                hosts.push_back(gai_strerror(rv));
                return hosts;
            } else {
                hosts.push_back(host);
            }
        }
        if (hosts.empty()) hosts.push_back("<invalid>");
        return hosts;
    }

    static std::vector<std::string> ToStrings(const ScopedAddrinfo& ai) {
        return ToStrings(ai.get());
    }

    size_t GetNumQueries(const test::DNSResponder& dns, const char* name) const {
        auto queries = dns.queries();
        size_t found = 0;
        for (const auto& p : queries) {
            if (p.first == name) {
                ++found;
            }
        }
        return found;
    }

    size_t GetNumQueriesForType(const test::DNSResponder& dns, ns_type type,
                                const char* name) const {
        auto queries = dns.queries();
        size_t found = 0;
        for (const auto& p : queries) {
            if (p.second == type && p.first == name) {
                ++found;
            }
        }
        return found;
    }

    bool WaitForPrefix64Detected(int netId, int timeoutMs) {
        constexpr int intervalMs = 2;
        const int limit = timeoutMs / intervalMs;
        for (int count = 0; count <= limit; ++count) {
            std::string prefix;
            auto rv = mDnsClient.resolvService()->getPrefix64(netId, &prefix);
            if (rv.isOk()) {
                return true;
            }
            usleep(intervalMs * 1000);
        }
        return false;
    }

    void RunGetAddrInfoStressTest_Binder(unsigned num_hosts, unsigned num_threads,
            unsigned num_queries) {
        std::vector<std::string> domains = { "example.com" };
        std::vector<std::unique_ptr<test::DNSResponder>> dns;
        std::vector<std::string> servers;
        std::vector<DnsResponderClient::DnsResponderClient::Mapping> mappings;
        ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupMappings(num_hosts, domains, &mappings));
        ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupDNSServers(MAXNS, mappings, &dns, &servers));

        ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, domains, kDefaultParams));

        auto t0 = std::chrono::steady_clock::now();
        std::vector<std::thread> threads(num_threads);
        for (std::thread& thread : threads) {
            thread = std::thread([&mappings, num_queries]() {
                for (unsigned i = 0 ; i < num_queries ; ++i) {
                    uint32_t ofs = arc4random_uniform(mappings.size());
                    auto& mapping = mappings[ofs];
                    addrinfo* result = nullptr;
                    int rv = getaddrinfo(mapping.host.c_str(), nullptr, nullptr, &result);
                    EXPECT_EQ(0, rv) << "error [" << rv << "] " << gai_strerror(rv);
                    if (rv == 0) {
                        std::string result_str = ToString(result);
                        EXPECT_TRUE(result_str == mapping.ip4 || result_str == mapping.ip6)
                            << "result='" << result_str << "', ip4='" << mapping.ip4
                            << "', ip6='" << mapping.ip6;
                    }
                    if (result) {
                        freeaddrinfo(result);
                        result = nullptr;
                    }
                }
            });
        }

        for (std::thread& thread : threads) {
            thread.join();
        }
        auto t1 = std::chrono::steady_clock::now();
        ALOGI("%u hosts, %u threads, %u queries, %Es", num_hosts, num_threads, num_queries,
                std::chrono::duration<double>(t1 - t0).count());

        std::vector<std::string> res_servers;
        std::vector<std::string> res_domains;
        std::vector<std::string> res_tls_servers;
        res_params res_params;
        std::vector<ResolverStats> res_stats;
        int wait_for_pending_req_timeout_count;
        ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                    &res_stats, &wait_for_pending_req_timeout_count));
        EXPECT_EQ(0, wait_for_pending_req_timeout_count);
    }

    void StartDns(test::DNSResponder& dns, const std::vector<DnsRecord>& records) {
        for (const auto& r : records) {
            dns.addMapping(r.host_name, r.type, r.addr);
        }

        ASSERT_TRUE(dns.startServer());
        dns.clearQueries();
    }

    DnsResponderClient mDnsClient;

    static constexpr char kLocalHost[] = "localhost";
    static constexpr char kLocalHostAddr[] = "127.0.0.1";
    static constexpr char kIp6LocalHost[] = "ip6-localhost";
    static constexpr char kIp6LocalHostAddr[] = "::1";
    static constexpr char kHelloExampleCom[] = "hello.example.com.";
};

TEST_F(ResolverTest, GetHostByName) {
    constexpr char nonexistent_host_name[] = "nonexistent.example.com.";

    test::DNSResponder dns;
    StartDns(dns, {{kHelloExampleCom, ns_type::ns_t_a, "1.2.3.3"}});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    const hostent* result;
    result = gethostbyname("nonexistent");
    EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, nonexistent_host_name));
    ASSERT_TRUE(result == nullptr);
    ASSERT_EQ(HOST_NOT_FOUND, h_errno);

    dns.clearQueries();
    result = gethostbyname("hello");
    EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, kHelloExampleCom));
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);
}

TEST_F(ResolverTest, GetHostByName_cnames) {
    constexpr char host_name[] = "host.example.com.";
    size_t cnamecount = 0;
    test::DNSResponder dns;

    const std::vector<DnsRecord> records = {
            {kHelloExampleCom, ns_type::ns_t_cname, "a.example.com."},
            {"a.example.com.", ns_type::ns_t_cname, "b.example.com."},
            {"b.example.com.", ns_type::ns_t_cname, "c.example.com."},
            {"c.example.com.", ns_type::ns_t_cname, "d.example.com."},
            {"d.example.com.", ns_type::ns_t_cname, "e.example.com."},
            {"e.example.com.", ns_type::ns_t_cname, host_name},
            {host_name, ns_type::ns_t_a, "1.2.3.3"},
            {host_name, ns_type::ns_t_aaaa, "2001:db8::42"},
    };
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    // using gethostbyname2() to resolve ipv4 hello.example.com. to 1.2.3.3
    // Ensure the v4 address and cnames are correct
    const hostent* result;
    result = gethostbyname2("hello", AF_INET);
    ASSERT_FALSE(result == nullptr);

    for (int i = 0; result != nullptr && result->h_aliases[i] != nullptr; i++) {
        std::string domain_name = records[i].host_name.substr(0, records[i].host_name.size() - 1);
        EXPECT_EQ(result->h_aliases[i], domain_name);
        cnamecount++;
    }
    // The size of "Non-cname type" record in DNS records is 2
    ASSERT_EQ(cnamecount, records.size() - 2);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);
    EXPECT_EQ(1U, dns.queries().size()) << dns.dumpQueries();

    // using gethostbyname2() to resolve ipv6 hello.example.com. to 2001:db8::42
    // Ensure the v6 address and cnames are correct
    cnamecount = 0;
    dns.clearQueries();
    result = gethostbyname2("hello", AF_INET6);
    for (unsigned i = 0; result != nullptr && result->h_aliases[i] != nullptr; i++) {
        std::string domain_name = records[i].host_name.substr(0, records[i].host_name.size() - 1);
        EXPECT_EQ(result->h_aliases[i], domain_name);
        cnamecount++;
    }
    // The size of "Non-cname type" DNS record in records is 2
    ASSERT_EQ(cnamecount, records.size() - 2);
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(16, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ("2001:db8::42", ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);
}

TEST_F(ResolverTest, GetHostByName_cnamesInfiniteLoop) {
    test::DNSResponder dns;
    const std::vector<DnsRecord> records = {
            {kHelloExampleCom, ns_type::ns_t_cname, "a.example.com."},
            {"a.example.com.", ns_type::ns_t_cname, kHelloExampleCom},
    };
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    const hostent* result;
    result = gethostbyname2("hello", AF_INET);
    ASSERT_TRUE(result == nullptr);

    dns.clearQueries();
    result = gethostbyname2("hello", AF_INET6);
    ASSERT_TRUE(result == nullptr);
}

TEST_F(ResolverTest, GetHostByName_localhost) {
    constexpr char name_camelcase[] = "LocalHost";
    constexpr char name_ip6_dot[] = "ip6-localhost.";
    constexpr char name_ip6_fqdn[] = "ip6-localhost.example.com.";

    // Add a dummy nameserver which shouldn't receive any queries
    test::DNSResponder dns;
    StartDns(dns, {});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    // Expect no DNS queries; localhost is resolved via /etc/hosts
    const hostent* result = gethostbyname(kLocalHost);
    EXPECT_TRUE(dns.queries().empty()) << dns.dumpQueries();
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(kLocalHostAddr, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);

    // Ensure the hosts file resolver ignores case of hostnames
    result = gethostbyname(name_camelcase);
    EXPECT_TRUE(dns.queries().empty()) << dns.dumpQueries();
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(kLocalHostAddr, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);

    // The hosts file also contains ip6-localhost, but gethostbyname() won't
    // return it unless the RES_USE_INET6 option is set. This would be easy to
    // change, but there's no point in changing the legacy behavior; new code
    // should be calling getaddrinfo() anyway.
    // So we check the legacy behavior, which results in amusing A-record
    // lookups for ip6-localhost, with and without search domains appended.
    dns.clearQueries();
    result = gethostbyname(kIp6LocalHost);
    EXPECT_EQ(2U, dns.queries().size()) << dns.dumpQueries();
    EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, name_ip6_dot)) << dns.dumpQueries();
    EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, name_ip6_fqdn)) << dns.dumpQueries();
    ASSERT_TRUE(result == nullptr);

    // Finally, use gethostbyname2() to resolve ip6-localhost to ::1 from
    // the hosts file.
    dns.clearQueries();
    result = gethostbyname2(kIp6LocalHost, AF_INET6);
    EXPECT_TRUE(dns.queries().empty()) << dns.dumpQueries();
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(16, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(kIp6LocalHostAddr, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);
}

TEST_F(ResolverTest, GetHostByName_numeric) {
    // Add a dummy nameserver which shouldn't receive any queries
    test::DNSResponder dns;
    StartDns(dns, {});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    // Numeric v4 address: expect no DNS queries
    constexpr char numeric_v4[] = "192.168.0.1";
    const hostent* result = gethostbyname(numeric_v4);
    EXPECT_EQ(0U, dns.queries().size());
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);  // v4
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(numeric_v4, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);

    // gethostbyname() recognizes a v6 address, and fails with no DNS queries
    constexpr char numeric_v6[] = "2001:db8::42";
    dns.clearQueries();
    result = gethostbyname(numeric_v6);
    EXPECT_EQ(0U, dns.queries().size());
    EXPECT_TRUE(result == nullptr);

    // Numeric v6 address with gethostbyname2(): succeeds with no DNS queries
    dns.clearQueries();
    result = gethostbyname2(numeric_v6, AF_INET6);
    EXPECT_EQ(0U, dns.queries().size());
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(16, result->h_length);  // v6
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(numeric_v6, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);

    // Numeric v6 address with scope work with getaddrinfo(),
    // but gethostbyname2() does not understand them; it issues two dns
    // queries, then fails. This hardly ever happens, there's no point
    // in fixing this. This test simply verifies the current (bogus)
    // behavior to avoid further regressions (like crashes, or leaks).
    constexpr char numeric_v6_scope[] = "fe80::1%lo";
    dns.clearQueries();
    result = gethostbyname2(numeric_v6_scope, AF_INET6);
    EXPECT_EQ(2U, dns.queries().size());  // OUCH!
    ASSERT_TRUE(result == nullptr);
}

TEST_F(ResolverTest, BinderSerialization) {
    using android::net::IDnsResolver;
    std::vector<int> params_offsets = {
            IDnsResolver::RESOLVER_PARAMS_SAMPLE_VALIDITY,
            IDnsResolver::RESOLVER_PARAMS_SUCCESS_THRESHOLD,
            IDnsResolver::RESOLVER_PARAMS_MIN_SAMPLES,
            IDnsResolver::RESOLVER_PARAMS_MAX_SAMPLES,
            IDnsResolver::RESOLVER_PARAMS_BASE_TIMEOUT_MSEC,
            IDnsResolver::RESOLVER_PARAMS_RETRY_COUNT,
    };
    const int size = static_cast<int>(params_offsets.size());
    EXPECT_EQ(size, IDnsResolver::RESOLVER_PARAMS_COUNT);
    std::sort(params_offsets.begin(), params_offsets.end());
    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(params_offsets[i], i);
    }
}

TEST_F(ResolverTest, GetHostByName_Binder) {
    using android::net::IDnsResolver;

    std::vector<std::string> domains = { "example.com" };
    std::vector<std::unique_ptr<test::DNSResponder>> dns;
    std::vector<std::string> servers;
    std::vector<DnsResponderClient::Mapping> mappings;
    ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupMappings(1, domains, &mappings));
    ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupDNSServers(4, mappings, &dns, &servers));
    ASSERT_EQ(1U, mappings.size());
    const DnsResponderClient::Mapping& mapping = mappings[0];

    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, domains, kDefaultParams));

    const hostent* result = gethostbyname(mapping.host.c_str());
    const size_t total_queries =
            std::accumulate(dns.begin(), dns.end(), 0, [this, &mapping](size_t total, auto& d) {
                return total + GetNumQueriesForType(*d, ns_type::ns_t_a, mapping.entry.c_str());
            });

    EXPECT_LE(1U, total_queries);
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ(mapping.ip4, ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);

    std::vector<std::string> res_servers;
    std::vector<std::string> res_domains;
    std::vector<std::string> res_tls_servers;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int wait_for_pending_req_timeout_count;
    ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                &res_stats, &wait_for_pending_req_timeout_count));
    EXPECT_EQ(servers.size(), res_servers.size());
    EXPECT_EQ(domains.size(), res_domains.size());
    EXPECT_EQ(0U, res_tls_servers.size());
    ASSERT_EQ(static_cast<size_t>(IDnsResolver::RESOLVER_PARAMS_COUNT), kDefaultParams.size());
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_SAMPLE_VALIDITY],
              res_params.sample_validity);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_SUCCESS_THRESHOLD],
              res_params.success_threshold);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_MIN_SAMPLES], res_params.min_samples);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_MAX_SAMPLES], res_params.max_samples);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_BASE_TIMEOUT_MSEC],
              res_params.base_timeout_msec);
    EXPECT_EQ(servers.size(), res_stats.size());

    EXPECT_THAT(res_servers, testing::UnorderedElementsAreArray(servers));
    EXPECT_THAT(res_domains, testing::UnorderedElementsAreArray(domains));
}

TEST_F(ResolverTest, GetAddrInfo) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char listen_addr2[] = "127.0.0.5";
    constexpr char host_name[] = "howdy.example.com.";

    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };
    test::DNSResponder dns(listen_addr);
    test::DNSResponder dns2(listen_addr2);
    StartDns(dns, records);
    StartDns(dns2, records);

    ASSERT_TRUE(mDnsClient.SetResolversForNetwork({listen_addr}));
    dns.clearQueries();
    dns2.clearQueries();

    ScopedAddrinfo result = safe_getaddrinfo("howdy", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    size_t found = GetNumQueries(dns, host_name);
    EXPECT_LE(1U, found);
    // Could be A or AAAA
    std::string result_str = ToString(result);
    EXPECT_TRUE(result_str == "1.2.3.4" || result_str == "::1.2.3.4")
        << ", result_str='" << result_str << "'";

    // Verify that the name is cached.
    size_t old_found = found;
    result = safe_getaddrinfo("howdy", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    found = GetNumQueries(dns, host_name);
    EXPECT_LE(1U, found);
    EXPECT_EQ(old_found, found);
    result_str = ToString(result);
    EXPECT_TRUE(result_str == "1.2.3.4" || result_str == "::1.2.3.4")
        << result_str;

    // Change the DNS resolver, ensure that queries are still cached.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork({listen_addr2}));
    dns.clearQueries();
    dns2.clearQueries();

    result = safe_getaddrinfo("howdy", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    found = GetNumQueries(dns, host_name);
    size_t found2 = GetNumQueries(dns2, host_name);
    EXPECT_EQ(0U, found);
    EXPECT_LE(0U, found2);

    // Could be A or AAAA
    result_str = ToString(result);
    EXPECT_TRUE(result_str == "1.2.3.4" || result_str == "::1.2.3.4")
        << ", result_str='" << result_str << "'";
}

TEST_F(ResolverTest, GetAddrInfoV4) {
    test::DNSResponder dns;
    StartDns(dns, {{kHelloExampleCom, ns_type::ns_t_a, "1.2.3.5"}});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    const addrinfo hints = {.ai_family = AF_INET};
    ScopedAddrinfo result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(1U, GetNumQueries(dns, kHelloExampleCom));
    EXPECT_EQ("1.2.3.5", ToString(result));
}

TEST_F(ResolverTest, GetAddrInfo_localhost) {
    // Add a dummy nameserver which shouldn't receive any queries
    test::DNSResponder dns;
    StartDns(dns, {});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    ScopedAddrinfo result = safe_getaddrinfo(kLocalHost, nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    // Expect no DNS queries; localhost is resolved via /etc/hosts
    EXPECT_TRUE(dns.queries().empty()) << dns.dumpQueries();
    EXPECT_EQ(kLocalHostAddr, ToString(result));

    result = safe_getaddrinfo(kIp6LocalHost, nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    // Expect no DNS queries; ip6-localhost is resolved via /etc/hosts
    EXPECT_TRUE(dns.queries().empty()) << dns.dumpQueries();
    EXPECT_EQ(kIp6LocalHostAddr, ToString(result));
}

// Verify if the resolver correctly handle multiple queries simultaneously
// step 1: set dns server#1 into deferred responding mode.
// step 2: thread#1 query "hello.example.com." --> resolver send query to server#1.
// step 3: thread#2 query "hello.example.com." --> resolver hold the request and wait for
//           response of previous pending query sent by thread#1.
// step 4: thread#3 query "konbanha.example.com." --> resolver send query to server#3. Server
//           respond to resolver immediately.
// step 5: check if server#1 get 1 query by thread#1, server#2 get 0 query, server#3 get 1 query.
// step 6: resume dns server#1 to respond dns query in step#2.
// step 7: thread#1 and #2 should get returned from DNS query after step#6. Also, check the
//           number of queries in server#2 is 0 to ensure thread#2 does not wake up unexpectedly
//           before signaled by thread#1.
TEST_F(ResolverTest, GetAddrInfoV4_deferred_resp) {
    const char* listen_addr1 = "127.0.0.9";
    const char* listen_addr2 = "127.0.0.10";
    const char* listen_addr3 = "127.0.0.11";
    const char* listen_srv = "53";
    const char* host_name_deferred = "hello.example.com.";
    const char* host_name_normal = "konbanha.example.com.";
    test::DNSResponder dns1(listen_addr1, listen_srv, 250, ns_rcode::ns_r_servfail);
    test::DNSResponder dns2(listen_addr2, listen_srv, 250, ns_rcode::ns_r_servfail);
    test::DNSResponder dns3(listen_addr3, listen_srv, 250, ns_rcode::ns_r_servfail);
    dns1.addMapping(host_name_deferred, ns_type::ns_t_a, "1.2.3.4");
    dns2.addMapping(host_name_deferred, ns_type::ns_t_a, "1.2.3.4");
    dns3.addMapping(host_name_normal, ns_type::ns_t_a, "1.2.3.5");
    ASSERT_TRUE(dns1.startServer());
    ASSERT_TRUE(dns2.startServer());
    ASSERT_TRUE(dns3.startServer());
    const std::vector<std::string> servers_for_t1 = {listen_addr1};
    const std::vector<std::string> servers_for_t2 = {listen_addr2};
    const std::vector<std::string> servers_for_t3 = {listen_addr3};
    addrinfo hints = {.ai_family = AF_INET};
    const std::vector<int> params = {300, 25, 8, 8, 5000};
    bool t3_task_done = false;

    dns1.setDeferredResp(true);
    std::thread t1([&, this]() {
        ASSERT_TRUE(
                mDnsClient.SetResolversForNetwork(servers_for_t1, kDefaultSearchDomains, params));
        ScopedAddrinfo result = safe_getaddrinfo(host_name_deferred, nullptr, &hints);
        // t3's dns query should got returned first
        EXPECT_TRUE(t3_task_done);
        EXPECT_EQ(1U, GetNumQueries(dns1, host_name_deferred));
        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ("1.2.3.4", ToString(result));
    });

    // ensuring t1 and t2 handler functions are processed in order
    usleep(100 * 1000);
    std::thread t2([&, this]() {
        ASSERT_TRUE(
                mDnsClient.SetResolversForNetwork(servers_for_t2, kDefaultSearchDomains, params));
        ScopedAddrinfo result = safe_getaddrinfo(host_name_deferred, nullptr, &hints);
        EXPECT_TRUE(t3_task_done);
        EXPECT_EQ(0U, GetNumQueries(dns2, host_name_deferred));
        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ("1.2.3.4", ToString(result));

        std::vector<std::string> res_servers;
        std::vector<std::string> res_domains;
        std::vector<std::string> res_tls_servers;
        res_params res_params;
        std::vector<ResolverStats> res_stats;
        int wait_for_pending_req_timeout_count;
        ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                    &res_stats, &wait_for_pending_req_timeout_count));
        EXPECT_EQ(0, wait_for_pending_req_timeout_count);
    });

    // ensuring t2 and t3 handler functions are processed in order
    usleep(100 * 1000);
    std::thread t3([&, this]() {
        ASSERT_TRUE(
                mDnsClient.SetResolversForNetwork(servers_for_t3, kDefaultSearchDomains, params));
        ScopedAddrinfo result = safe_getaddrinfo(host_name_normal, nullptr, &hints);
        EXPECT_EQ(1U, GetNumQueries(dns1, host_name_deferred));
        EXPECT_EQ(0U, GetNumQueries(dns2, host_name_deferred));
        EXPECT_EQ(1U, GetNumQueries(dns3, host_name_normal));
        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ("1.2.3.5", ToString(result));

        t3_task_done = true;
        dns1.setDeferredResp(false);
    });
    t3.join();
    t1.join();
    t2.join();
}

TEST_F(ResolverTest, GetAddrInfo_cnames) {
    constexpr char host_name[] = "host.example.com.";
    test::DNSResponder dns;
    const std::vector<DnsRecord> records = {
            {kHelloExampleCom, ns_type::ns_t_cname, "a.example.com."},
            {"a.example.com.", ns_type::ns_t_cname, "b.example.com."},
            {"b.example.com.", ns_type::ns_t_cname, "c.example.com."},
            {"c.example.com.", ns_type::ns_t_cname, "d.example.com."},
            {"d.example.com.", ns_type::ns_t_cname, "e.example.com."},
            {"e.example.com.", ns_type::ns_t_cname, host_name},
            {host_name, ns_type::ns_t_a, "1.2.3.3"},
            {host_name, ns_type::ns_t_aaaa, "2001:db8::42"},
    };
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    addrinfo hints = {.ai_family = AF_INET};
    ScopedAddrinfo result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));

    dns.clearQueries();
    hints = {.ai_family = AF_INET6};
    result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ("2001:db8::42", ToString(result));
}

TEST_F(ResolverTest, GetAddrInfo_cnamesNoIpAddress) {
    test::DNSResponder dns;
    const std::vector<DnsRecord> records = {
            {kHelloExampleCom, ns_type::ns_t_cname, "a.example.com."},
    };
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    addrinfo hints = {.ai_family = AF_INET};
    ScopedAddrinfo result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result == nullptr);

    dns.clearQueries();
    hints = {.ai_family = AF_INET6};
    result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result == nullptr);
}

TEST_F(ResolverTest, GetAddrInfo_cnamesIllegalRdata) {
    test::DNSResponder dns;
    const std::vector<DnsRecord> records = {
            {kHelloExampleCom, ns_type::ns_t_cname, ".!#?"},
    };
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    addrinfo hints = {.ai_family = AF_INET};
    ScopedAddrinfo result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result == nullptr);

    dns.clearQueries();
    hints = {.ai_family = AF_INET6};
    result = safe_getaddrinfo("hello", nullptr, &hints);
    EXPECT_TRUE(result == nullptr);
}

TEST_F(ResolverTest, MultidomainResolution) {
    constexpr char host_name[] = "nihao.example2.com.";
    std::vector<std::string> searchDomains = { "example1.com", "example2.com", "example3.com" };

    test::DNSResponder dns("127.0.0.6");
    StartDns(dns, {{host_name, ns_type::ns_t_a, "1.2.3.3"}});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork({"127.0.0.6"}, searchDomains));

    const hostent* result = gethostbyname("nihao");

    EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, host_name));
    ASSERT_FALSE(result == nullptr);
    ASSERT_EQ(4, result->h_length);
    ASSERT_FALSE(result->h_addr_list[0] == nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));
    EXPECT_TRUE(result->h_addr_list[1] == nullptr);
}

TEST_F(ResolverTest, GetAddrInfoV6_numeric) {
    constexpr char host_name[] = "ohayou.example.com.";
    constexpr char numeric_addr[] = "fe80::1%lo";

    test::DNSResponder dns;
    dns.setResponseProbability(0.0);
    StartDns(dns, {{host_name, ns_type::ns_t_aaaa, "2001:db8::5"}});
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    addrinfo hints = {.ai_family = AF_INET6};
    ScopedAddrinfo result = safe_getaddrinfo(numeric_addr, nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(numeric_addr, ToString(result));
    EXPECT_TRUE(dns.queries().empty());  // Ensure no DNS queries were sent out

    // Now try a non-numeric hostname query with the AI_NUMERICHOST flag set.
    // We should fail without sending out a DNS query.
    hints.ai_flags |= AI_NUMERICHOST;
    result = safe_getaddrinfo(host_name, nullptr, &hints);
    EXPECT_TRUE(result == nullptr);
    EXPECT_TRUE(dns.queries().empty());  // Ensure no DNS queries were sent out
}

TEST_F(ResolverTest, GetAddrInfoV6_failing) {
    constexpr char listen_addr0[] = "127.0.0.7";
    constexpr char listen_addr1[] = "127.0.0.8";
    const char* host_name = "ohayou.example.com.";

    test::DNSResponder dns0(listen_addr0);
    test::DNSResponder dns1(listen_addr1);
    dns0.setResponseProbability(0.0);
    StartDns(dns0, {{host_name, ns_type::ns_t_aaaa, "2001:db8::5"}});
    StartDns(dns1, {{host_name, ns_type::ns_t_aaaa, "2001:db8::6"}});

    std::vector<std::string> servers = { listen_addr0, listen_addr1 };
    // <sample validity in s> <success threshold in percent> <min samples> <max samples>
    int sample_count = 8;
    const std::vector<int> params = { 300, 25, sample_count, sample_count };
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, kDefaultSearchDomains, params));

    // Repeatedly perform resolutions for non-existing domains until MAXNSSAMPLES resolutions have
    // reached the dns0, which is set to fail. No more requests should then arrive at that server
    // for the next sample_lifetime seconds.
    // TODO: This approach is implementation-dependent, change once metrics reporting is available.
    const addrinfo hints = {.ai_family = AF_INET6};
    for (int i = 0; i < sample_count; ++i) {
        std::string domain = StringPrintf("nonexistent%d", i);
        ScopedAddrinfo result = safe_getaddrinfo(domain.c_str(), nullptr, &hints);
    }
    // Due to 100% errors for all possible samples, the server should be ignored from now on and
    // only the second one used for all following queries, until NSSAMPLE_VALIDITY is reached.
    dns0.clearQueries();
    dns1.clearQueries();
    ScopedAddrinfo result = safe_getaddrinfo("ohayou", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(0U, GetNumQueries(dns0, host_name));
    EXPECT_EQ(1U, GetNumQueries(dns1, host_name));
}

TEST_F(ResolverTest, GetAddrInfoV6_nonresponsive) {
    constexpr char listen_addr0[] = "127.0.0.7";
    constexpr char listen_addr1[] = "127.0.0.8";
    constexpr char listen_srv[] = "53";
    constexpr char host_name1[] = "ohayou.example.com.";
    constexpr char host_name2[] = "ciao.example.com.";
    const std::vector<DnsRecord> records0 = {
            {host_name1, ns_type::ns_t_aaaa, "2001:db8::5"},
            {host_name2, ns_type::ns_t_aaaa, "2001:db8::5"},
    };
    const std::vector<DnsRecord> records1 = {
            {host_name1, ns_type::ns_t_aaaa, "2001:db8::6"},
            {host_name2, ns_type::ns_t_aaaa, "2001:db8::6"},
    };

    // dns0 does not respond with 100% probability, while
    // dns1 responds normally, at least initially.
    test::DNSResponder dns0(listen_addr0, listen_srv, 250, static_cast<ns_rcode>(-1));
    test::DNSResponder dns1(listen_addr1, listen_srv, 250, static_cast<ns_rcode>(-1));
    dns0.setResponseProbability(0.0);
    StartDns(dns0, records0);
    StartDns(dns1, records1);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork({listen_addr0, listen_addr1}));

    const addrinfo hints = {.ai_family = AF_INET6};

    // dns0 will ignore the request, and we'll fallback to dns1 after the first
    // retry.
    ScopedAddrinfo result = safe_getaddrinfo(host_name1, nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(1U, GetNumQueries(dns0, host_name1));
    EXPECT_EQ(1U, GetNumQueries(dns1, host_name1));

    // Now make dns1 also ignore 100% requests... The resolve should alternate
    // retries between the nameservers and fail after 4 attempts.
    dns1.setResponseProbability(0.0);
    addrinfo* result2 = nullptr;
    EXPECT_EQ(EAI_NODATA, getaddrinfo(host_name2, nullptr, &hints, &result2));
    EXPECT_EQ(nullptr, result2);
    EXPECT_EQ(4U, GetNumQueries(dns0, host_name2));
    EXPECT_EQ(4U, GetNumQueries(dns1, host_name2));
}

TEST_F(ResolverTest, GetAddrInfoV6_concurrent) {
    constexpr char listen_addr0[] = "127.0.0.9";
    constexpr char listen_addr1[] = "127.0.0.10";
    constexpr char listen_addr2[] = "127.0.0.11";
    constexpr char host_name[] = "konbanha.example.com.";

    test::DNSResponder dns0(listen_addr0);
    test::DNSResponder dns1(listen_addr1);
    test::DNSResponder dns2(listen_addr2);
    StartDns(dns0, {{host_name, ns_type::ns_t_aaaa, "2001:db8::5"}});
    StartDns(dns1, {{host_name, ns_type::ns_t_aaaa, "2001:db8::6"}});
    StartDns(dns2, {{host_name, ns_type::ns_t_aaaa, "2001:db8::7"}});

    const std::vector<std::string> servers = { listen_addr0, listen_addr1, listen_addr2 };
    std::vector<std::thread> threads(10);
    for (std::thread& thread : threads) {
       thread = std::thread([this, &servers]() {
            unsigned delay = arc4random_uniform(1*1000*1000); // <= 1s
            usleep(delay);
            std::vector<std::string> serverSubset;
            for (const auto& server : servers) {
                if (arc4random_uniform(2)) {
                    serverSubset.push_back(server);
                }
            }
            if (serverSubset.empty()) serverSubset = servers;
            ASSERT_TRUE(mDnsClient.SetResolversForNetwork(serverSubset));
            const addrinfo hints = {.ai_family = AF_INET6};
            addrinfo* result = nullptr;
            int rv = getaddrinfo("konbanha", nullptr, &hints, &result);
            EXPECT_EQ(0, rv) << "error [" << rv << "] " << gai_strerror(rv);
            if (result) {
                freeaddrinfo(result);
                result = nullptr;
            }
        });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }

    std::vector<std::string> res_servers;
    std::vector<std::string> res_domains;
    std::vector<std::string> res_tls_servers;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int wait_for_pending_req_timeout_count;
    ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                &res_stats, &wait_for_pending_req_timeout_count));
    EXPECT_EQ(0, wait_for_pending_req_timeout_count);
}

TEST_F(ResolverTest, GetAddrInfoStressTest_Binder_100) {
    const unsigned num_hosts = 100;
    const unsigned num_threads = 100;
    const unsigned num_queries = 100;
    ASSERT_NO_FATAL_FAILURE(RunGetAddrInfoStressTest_Binder(num_hosts, num_threads, num_queries));
}

TEST_F(ResolverTest, GetAddrInfoStressTest_Binder_100000) {
    const unsigned num_hosts = 100000;
    const unsigned num_threads = 100;
    const unsigned num_queries = 100;
    ASSERT_NO_FATAL_FAILURE(RunGetAddrInfoStressTest_Binder(num_hosts, num_threads, num_queries));
}

TEST_F(ResolverTest, EmptySetup) {
    using android::net::IDnsResolver;
    std::vector<std::string> servers;
    std::vector<std::string> domains;
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, domains));
    std::vector<std::string> res_servers;
    std::vector<std::string> res_domains;
    std::vector<std::string> res_tls_servers;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int wait_for_pending_req_timeout_count;
    ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                &res_stats, &wait_for_pending_req_timeout_count));
    EXPECT_EQ(0U, res_servers.size());
    EXPECT_EQ(0U, res_domains.size());
    EXPECT_EQ(0U, res_tls_servers.size());
    ASSERT_EQ(static_cast<size_t>(IDnsResolver::RESOLVER_PARAMS_COUNT), kDefaultParams.size());
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_SAMPLE_VALIDITY],
              res_params.sample_validity);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_SUCCESS_THRESHOLD],
              res_params.success_threshold);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_MIN_SAMPLES], res_params.min_samples);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_MAX_SAMPLES], res_params.max_samples);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_BASE_TIMEOUT_MSEC],
              res_params.base_timeout_msec);
    EXPECT_EQ(kDefaultParams[IDnsResolver::RESOLVER_PARAMS_RETRY_COUNT], res_params.retry_count);
}

TEST_F(ResolverTest, SearchPathChange) {
    constexpr char listen_addr[] = "127.0.0.13";
    constexpr char host_name1[] = "test13.domain1.org.";
    constexpr char host_name2[] = "test13.domain2.org.";
    std::vector<std::string> servers = { listen_addr };
    std::vector<std::string> domains = { "domain1.org" };

    const std::vector<DnsRecord> records = {
            {host_name1, ns_type::ns_t_aaaa, "2001:db8::13"},
            {host_name2, ns_type::ns_t_aaaa, "2001:db8::1:13"},
    };
    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, domains));

    const addrinfo hints = {.ai_family = AF_INET6};
    ScopedAddrinfo result = safe_getaddrinfo("test13", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(1U, dns.queries().size());
    EXPECT_EQ(1U, GetNumQueries(dns, host_name1));
    EXPECT_EQ("2001:db8::13", ToString(result));

    // Test that changing the domain search path on its own works.
    domains = { "domain2.org" };
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, domains));
    dns.clearQueries();

    result = safe_getaddrinfo("test13", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(1U, dns.queries().size());
    EXPECT_EQ(1U, GetNumQueries(dns, host_name2));
    EXPECT_EQ("2001:db8::1:13", ToString(result));
}

static std::string base64Encode(const std::vector<uint8_t>& input) {
    size_t out_len;
    EXPECT_EQ(1, EVP_EncodedLength(&out_len, input.size()));
    // out_len includes the trailing NULL.
    uint8_t output_bytes[out_len];
    EXPECT_EQ(out_len - 1, EVP_EncodeBlock(output_bytes, input.data(), input.size()));
    return std::string(reinterpret_cast<char*>(output_bytes));
}

// If we move this function to dns_responder_client, it will complicate the dependency need of
// dns_tls_frontend.h.
static void setupTlsServers(const std::vector<std::string>& servers,
                            std::vector<std::unique_ptr<test::DnsTlsFrontend>>* tls,
                            std::vector<std::string>* fingerprints) {
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";

    for (const auto& server : servers) {
        auto t = std::make_unique<test::DnsTlsFrontend>(server, listen_tls, server, listen_udp);
        t = std::make_unique<test::DnsTlsFrontend>(server, listen_tls, server, listen_udp);
        t->startServer();
        fingerprints->push_back(base64Encode(t->fingerprint()));
        tls->push_back(std::move(t));
    }
}

TEST_F(ResolverTest, MaxServerPrune_Binder) {
    std::vector<std::string> domains;
    std::vector<std::unique_ptr<test::DNSResponder>> dns;
    std::vector<std::unique_ptr<test::DnsTlsFrontend>> tls;
    std::vector<std::string> servers;
    std::vector<std::string> fingerprints;
    std::vector<DnsResponderClient::Mapping> mappings;

    for (unsigned i = 0; i < MAXDNSRCH + 1; i++) {
        domains.push_back(StringPrintf("example%u.com", i));
    }
    ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupMappings(1, domains, &mappings));
    ASSERT_NO_FATAL_FAILURE(mDnsClient.SetupDNSServers(MAXNS + 1, mappings, &dns, &servers));
    ASSERT_NO_FATAL_FAILURE(setupTlsServers(servers, &tls, &fingerprints));

    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, domains, kDefaultParams, "", fingerprints));

    // If the private DNS validation hasn't completed yet before backend DNS servers stop,
    // TLS servers will get stuck in handleOneRequest(), which causes this test stuck in
    // ~DnsTlsFrontend() because the TLS server loop threads can't be terminated.
    // So, wait for private DNS validation done before stopping backend DNS servers.
    for (int i = 0; i < MAXNS; i++) {
        ALOGI("Waiting for private DNS validation on %s.", tls[i]->listen_address().c_str());
        EXPECT_TRUE(tls[i]->waitForQueries(1, 5000));
        ALOGI("private DNS validation on %s done.", tls[i]->listen_address().c_str());
    }

    std::vector<std::string> res_servers;
    std::vector<std::string> res_domains;
    std::vector<std::string> res_tls_servers;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int wait_for_pending_req_timeout_count;
    ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                &res_stats, &wait_for_pending_req_timeout_count));

    // Check the size of the stats and its contents.
    EXPECT_EQ(static_cast<size_t>(MAXNS), res_servers.size());
    EXPECT_EQ(static_cast<size_t>(MAXNS), res_tls_servers.size());
    EXPECT_EQ(static_cast<size_t>(MAXDNSRCH), res_domains.size());
    EXPECT_TRUE(std::equal(servers.begin(), servers.begin() + MAXNS, res_servers.begin()));
    EXPECT_TRUE(std::equal(servers.begin(), servers.begin() + MAXNS, res_tls_servers.begin()));
    EXPECT_TRUE(std::equal(domains.begin(), domains.begin() + MAXDNSRCH, res_domains.begin()));
}

TEST_F(ResolverTest, ResolverStats) {
    constexpr char listen_addr1[] = "127.0.0.4";
    constexpr char listen_addr2[] = "127.0.0.5";
    constexpr char listen_addr3[] = "127.0.0.6";

    // Set server 1 timeout.
    test::DNSResponder dns1(listen_addr1, "53", 250, static_cast<ns_rcode>(-1));
    dns1.setResponseProbability(0.0);
    ASSERT_TRUE(dns1.startServer());

    // Set server 2 responding server failure.
    test::DNSResponder dns2(listen_addr2);
    dns2.setResponseProbability(0.0);
    ASSERT_TRUE(dns2.startServer());

    // Set server 3 workable.
    test::DNSResponder dns3(listen_addr3);
    dns3.addMapping(kHelloExampleCom, ns_type::ns_t_a, "1.2.3.4");
    ASSERT_TRUE(dns3.startServer());

    std::vector<std::string> servers = {listen_addr1, listen_addr2, listen_addr3};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    dns3.clearQueries();
    const addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};
    ScopedAddrinfo result = safe_getaddrinfo("hello", nullptr, &hints);
    size_t found = GetNumQueries(dns3, kHelloExampleCom);
    EXPECT_LE(1U, found);
    std::string result_str = ToString(result);
    EXPECT_TRUE(result_str == "1.2.3.4") << ", result_str='" << result_str << "'";

    std::vector<std::string> res_servers;
    std::vector<std::string> res_domains;
    std::vector<std::string> res_tls_servers;
    res_params res_params;
    std::vector<ResolverStats> res_stats;
    int wait_for_pending_req_timeout_count;
    ASSERT_TRUE(GetResolverInfo(&res_servers, &res_domains, &res_tls_servers, &res_params,
                                &res_stats, &wait_for_pending_req_timeout_count));

    EXPECT_EQ(1, res_stats[0].timeouts);
    EXPECT_EQ(1, res_stats[1].errors);
    EXPECT_EQ(1, res_stats[2].successes);
}

// Test what happens if the specified TLS server is nonexistent.
TEST_F(ResolverTest, GetHostByName_TlsMissing) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char host_name[] = "tlsmissing.example.com.";

    test::DNSResponder dns;
    StartDns(dns, {{host_name, ns_type::ns_t_a, "1.2.3.3"}});
    std::vector<std::string> servers = { listen_addr };

    // There's nothing listening on this address, so validation will either fail or
    /// hang.  Either way, queries will continue to flow to the DNSResponder.
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));

    const hostent* result;

    result = gethostbyname("tlsmissing");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

// Test what happens if the specified TLS server replies with garbage.
TEST_F(ResolverTest, GetHostByName_TlsBroken) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char host_name1[] = "tlsbroken1.example.com.";
    constexpr char host_name2[] = "tlsbroken2.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name1, ns_type::ns_t_a, "1.2.3.1"},
            {host_name2, ns_type::ns_t_a, "1.2.3.2"},
    };

    test::DNSResponder dns;
    StartDns(dns, records);
    std::vector<std::string> servers = { listen_addr };

    // Bind the specified private DNS socket but don't respond to any client sockets yet.
    int s = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    ASSERT_TRUE(s >= 0);
    struct sockaddr_in tlsServer = {
        .sin_family = AF_INET,
        .sin_port = htons(853),
    };
    ASSERT_TRUE(inet_pton(AF_INET, listen_addr, &tlsServer.sin_addr));
    ASSERT_TRUE(enableSockopt(s, SOL_SOCKET, SO_REUSEPORT).ok());
    ASSERT_TRUE(enableSockopt(s, SOL_SOCKET, SO_REUSEADDR).ok());
    ASSERT_FALSE(bind(s, reinterpret_cast<struct sockaddr*>(&tlsServer), sizeof(tlsServer)));
    ASSERT_FALSE(listen(s, 1));

    // Trigger TLS validation.
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));

    struct sockaddr_storage cliaddr;
    socklen_t sin_size = sizeof(cliaddr);
    int new_fd = accept4(s, reinterpret_cast<struct sockaddr *>(&cliaddr), &sin_size, SOCK_CLOEXEC);
    ASSERT_TRUE(new_fd > 0);

    // We've received the new file descriptor but not written to it or closed, so the
    // validation is still pending.  Queries should still flow correctly because the
    // server is not used until validation succeeds.
    const hostent* result;
    result = gethostbyname("tlsbroken1");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.1", ToString(result));

    // Now we cause the validation to fail.
    std::string garbage = "definitely not a valid TLS ServerHello";
    write(new_fd, garbage.data(), garbage.size());
    close(new_fd);

    // Validation failure shouldn't interfere with lookups, because lookups won't be sent
    // to the TLS server unless validation succeeds.
    result = gethostbyname("tlsbroken2");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.2", ToString(result));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
    close(s);
}

TEST_F(ResolverTest, GetHostByName_Tls) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name1[] = "tls1.example.com.";
    constexpr char host_name2[] = "tls2.example.com.";
    constexpr char host_name3[] = "tls3.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name1, ns_type::ns_t_a, "1.2.3.1"},
            {host_name2, ns_type::ns_t_a, "1.2.3.2"},
            {host_name3, ns_type::ns_t_a, "1.2.3.3"},
    };

    test::DNSResponder dns;
    StartDns(dns, records);
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));

    const hostent* result;

    // Wait for validation to complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));

    result = gethostbyname("tls1");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.1", ToString(result));

    // Wait for query to get counted.
    EXPECT_TRUE(tls.waitForQueries(2, 5000));

    // Stop the TLS server.  Since we're in opportunistic mode, queries will
    // fall back to the locally-assigned (clear text) nameservers.
    tls.stopServer();

    dns.clearQueries();
    result = gethostbyname("tls2");
    EXPECT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.2", ToString(result));
    const auto queries = dns.queries();
    EXPECT_EQ(1U, queries.size());
    EXPECT_EQ("tls2.example.com.", queries[0].first);
    EXPECT_EQ(ns_t_a, queries[0].second);

    // Reset the resolvers without enabling TLS.  Queries should still be routed
    // to the UDP endpoint.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());

    result = gethostbyname("tls3");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.3", ToString(result));
}

TEST_F(ResolverTest, GetHostByName_TlsFingerprint) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    test::DNSResponder dns;
    ASSERT_TRUE(dns.startServer());
    for (int chain_length = 1; chain_length <= 3; ++chain_length) {
        std::string host_name = StringPrintf("tlsfingerprint%d.example.com.", chain_length);
        dns.addMapping(host_name, ns_type::ns_t_a, "1.2.3.1");
        std::vector<std::string> servers = { listen_addr };

        test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
        tls.set_chain_length(chain_length);
        ASSERT_TRUE(tls.startServer());
        ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams,
                                                   "", {base64Encode(tls.fingerprint())}));

        const hostent* result;

        // Wait for validation to complete.
        EXPECT_TRUE(tls.waitForQueries(1, 5000));

        result = gethostbyname(StringPrintf("tlsfingerprint%d", chain_length).c_str());
        EXPECT_FALSE(result == nullptr);
        if (result) {
            EXPECT_EQ("1.2.3.1", ToString(result));

            // Wait for query to get counted.
            EXPECT_TRUE(tls.waitForQueries(2, 5000));
        }

        // Clear TLS bit to ensure revalidation.
        ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
        tls.stopServer();
    }
}

TEST_F(ResolverTest, GetHostByName_BadTlsFingerprint) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name[] = "badtlsfingerprint.example.com.";

    test::DNSResponder dns;
    StartDns(dns, {{host_name, ns_type::ns_t_a, "1.2.3.1"}});
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    std::vector<uint8_t> bad_fingerprint = tls.fingerprint();
    bad_fingerprint[5] += 1;  // Corrupt the fingerprint.
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "",
                                               {base64Encode(bad_fingerprint)}));

    // The initial validation should fail at the fingerprint check before
    // issuing a query.
    EXPECT_FALSE(tls.waitForQueries(1, 500));

    // A fingerprint was provided and failed to match, so the query should fail.
    EXPECT_EQ(nullptr, gethostbyname("badtlsfingerprint"));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

// Test that we can pass two different fingerprints, and connection succeeds as long as
// at least one of them matches the server.
TEST_F(ResolverTest, GetHostByName_TwoTlsFingerprints) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name[] = "twotlsfingerprints.example.com.";

    test::DNSResponder dns;
    StartDns(dns, {{host_name, ns_type::ns_t_a, "1.2.3.1"}});
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    std::vector<uint8_t> bad_fingerprint = tls.fingerprint();
    bad_fingerprint[5] += 1;  // Corrupt the fingerprint.
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(
            servers, kDefaultSearchDomains, kDefaultParams, "",
            {base64Encode(bad_fingerprint), base64Encode(tls.fingerprint())}));

    const hostent* result;

    // Wait for validation to complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));

    result = gethostbyname("twotlsfingerprints");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.1", ToString(result));

    // Wait for query to get counted.
    EXPECT_TRUE(tls.waitForQueries(2, 5000));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

TEST_F(ResolverTest, GetHostByName_TlsFingerprintGoesBad) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name1[] = "tlsfingerprintgoesbad1.example.com.";
    constexpr char host_name2[] = "tlsfingerprintgoesbad2.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name1, ns_type::ns_t_a, "1.2.3.1"},
            {host_name2, ns_type::ns_t_a, "1.2.3.2"},
    };

    test::DNSResponder dns;
    StartDns(dns, records);
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "",
                                               {base64Encode(tls.fingerprint())}));

    const hostent* result;

    // Wait for validation to complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));

    result = gethostbyname("tlsfingerprintgoesbad1");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.1", ToString(result));

    // Wait for query to get counted.
    EXPECT_TRUE(tls.waitForQueries(2, 5000));

    // Restart the TLS server.  This will generate a new certificate whose fingerprint
    // no longer matches the stored fingerprint.
    tls.stopServer();
    tls.startServer();

    result = gethostbyname("tlsfingerprintgoesbad2");
    ASSERT_TRUE(result == nullptr);
    EXPECT_EQ(HOST_NOT_FOUND, h_errno);

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

TEST_F(ResolverTest, GetHostByName_TlsFailover) {
    constexpr char listen_addr1[] = "127.0.0.3";
    constexpr char listen_addr2[] = "127.0.0.4";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name1[] = "tlsfailover1.example.com.";
    constexpr char host_name2[] = "tlsfailover2.example.com.";
    const std::vector<DnsRecord> records1 = {
            {host_name1, ns_type::ns_t_a, "1.2.3.1"},
            {host_name2, ns_type::ns_t_a, "1.2.3.2"},
    };
    const std::vector<DnsRecord> records2 = {
            {host_name1, ns_type::ns_t_a, "1.2.3.3"},
            {host_name2, ns_type::ns_t_a, "1.2.3.4"},
    };

    test::DNSResponder dns1(listen_addr1);
    test::DNSResponder dns2(listen_addr2);
    StartDns(dns1, records1);
    StartDns(dns2, records2);

    std::vector<std::string> servers = { listen_addr1, listen_addr2 };

    test::DnsTlsFrontend tls1(listen_addr1, listen_tls, listen_addr1, listen_udp);
    test::DnsTlsFrontend tls2(listen_addr2, listen_tls, listen_addr2, listen_udp);
    ASSERT_TRUE(tls1.startServer());
    ASSERT_TRUE(tls2.startServer());
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(
            servers, kDefaultSearchDomains, kDefaultParams, "",
            {base64Encode(tls1.fingerprint()), base64Encode(tls2.fingerprint())}));

    const hostent* result;

    // Wait for validation to complete.
    EXPECT_TRUE(tls1.waitForQueries(1, 5000));
    EXPECT_TRUE(tls2.waitForQueries(1, 5000));

    result = gethostbyname("tlsfailover1");
    ASSERT_FALSE(result == nullptr);
    EXPECT_EQ("1.2.3.1", ToString(result));

    // Wait for query to get counted.
    EXPECT_TRUE(tls1.waitForQueries(2, 5000));
    // No new queries should have reached tls2.
    EXPECT_EQ(1, tls2.queries());

    // Stop tls1.  Subsequent queries should attempt to reach tls1, fail, and retry to tls2.
    tls1.stopServer();

    result = gethostbyname("tlsfailover2");
    EXPECT_EQ("1.2.3.4", ToString(result));

    // Wait for query to get counted.
    EXPECT_TRUE(tls2.waitForQueries(2, 5000));

    // No additional queries should have reached the insecure servers.
    EXPECT_EQ(2U, dns1.queries().size());
    EXPECT_EQ(2U, dns2.queries().size());

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));
}

TEST_F(ResolverTest, GetHostByName_BadTlsName) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name[] = "badtlsname.example.com.";

    test::DNSResponder dns;
    StartDns(dns, {{host_name, ns_type::ns_t_a, "1.2.3.1"}});
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams,
                                               "www.example.com", {}));

    // The TLS server's certificate doesn't chain to a known CA, and a nonempty name was specified,
    // so the client should fail the TLS handshake before ever issuing a query.
    EXPECT_FALSE(tls.waitForQueries(1, 500));

    // The query should fail hard, because a name was specified.
    EXPECT_EQ(nullptr, gethostbyname("badtlsname"));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

TEST_F(ResolverTest, GetAddrInfo_Tls) {
    constexpr char listen_addr[] = "127.0.0.3";
    constexpr char listen_udp[] = "53";
    constexpr char listen_tls[] = "853";
    constexpr char host_name[] = "addrinfotls.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns;
    StartDns(dns, records);
    std::vector<std::string> servers = { listen_addr };

    test::DnsTlsFrontend tls(listen_addr, listen_tls, listen_addr, listen_udp);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "",
                                               {base64Encode(tls.fingerprint())}));

    // Wait for validation to complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));

    dns.clearQueries();
    ScopedAddrinfo result = safe_getaddrinfo("addrinfotls", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    size_t found = GetNumQueries(dns, host_name);
    EXPECT_LE(1U, found);
    // Could be A or AAAA
    std::string result_str = ToString(result);
    EXPECT_TRUE(result_str == "1.2.3.4" || result_str == "::1.2.3.4")
        << ", result_str='" << result_str << "'";
    // Wait for both A and AAAA queries to get counted.
    EXPECT_TRUE(tls.waitForQueries(3, 5000));

    // Clear TLS bit.
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork());
}

TEST_F(ResolverTest, TlsBypass) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    const char OFF[] = "off";
    const char OPPORTUNISTIC[] = "opportunistic";
    const char STRICT[] = "strict";

    const char GETHOSTBYNAME[] = "gethostbyname";
    const char GETADDRINFO[] = "getaddrinfo";
    const char GETADDRINFOFORNET[] = "getaddrinfofornet";

    const unsigned BYPASS_NETID = NETID_USE_LOCAL_NAMESERVERS | TEST_NETID;

    const std::vector<uint8_t> NOOP_FINGERPRINT(SHA256_SIZE, 0U);

    const char ADDR4[] = "192.0.2.1";
    const char ADDR6[] = "2001:db8::1";

    const char cleartext_addr[] = "127.0.0.53";
    const char cleartext_port[] = "53";
    const char tls_port[] = "853";
    const std::vector<std::string> servers = { cleartext_addr };

    test::DNSResponder dns(cleartext_addr);
    ASSERT_TRUE(dns.startServer());

    test::DnsTlsFrontend tls(cleartext_addr, tls_port, cleartext_addr, cleartext_port);

    struct TestConfig {
        const std::string mode;
        const bool withWorkingTLS;
        const std::string method;

        std::string asHostName() const {
            return StringPrintf("%s.%s.%s.",
                                mode.c_str(),
                                withWorkingTLS ? "tlsOn" : "tlsOff",
                                method.c_str());
        }
    } testConfigs[]{
        {OFF,           false, GETHOSTBYNAME},
        {OPPORTUNISTIC, false, GETHOSTBYNAME},
        {STRICT,        false, GETHOSTBYNAME},
        {OFF,           true,  GETHOSTBYNAME},
        {OPPORTUNISTIC, true,  GETHOSTBYNAME},
        {STRICT,        true,  GETHOSTBYNAME},
        {OFF,           false, GETADDRINFO},
        {OPPORTUNISTIC, false, GETADDRINFO},
        {STRICT,        false, GETADDRINFO},
        {OFF,           true,  GETADDRINFO},
        {OPPORTUNISTIC, true,  GETADDRINFO},
        {STRICT,        true,  GETADDRINFO},
        {OFF,           false, GETADDRINFOFORNET},
        {OPPORTUNISTIC, false, GETADDRINFOFORNET},
        {STRICT,        false, GETADDRINFOFORNET},
        {OFF,           true,  GETADDRINFOFORNET},
        {OPPORTUNISTIC, true,  GETADDRINFOFORNET},
        {STRICT,        true,  GETADDRINFOFORNET},
    };

    for (const auto& config : testConfigs) {
        const std::string testHostName = config.asHostName();
        SCOPED_TRACE(testHostName);

        // Don't tempt test bugs due to caching.
        const char* host_name = testHostName.c_str();
        dns.addMapping(host_name, ns_type::ns_t_a, ADDR4);
        dns.addMapping(host_name, ns_type::ns_t_aaaa, ADDR6);

        if (config.withWorkingTLS) ASSERT_TRUE(tls.startServer());

        if (config.mode == OFF) {
            ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers, kDefaultSearchDomains,
                                                          kDefaultParams));
        } else if (config.mode == OPPORTUNISTIC) {
            ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains,
                                                       kDefaultParams, "", {}));
            // Wait for validation to complete.
            if (config.withWorkingTLS) EXPECT_TRUE(tls.waitForQueries(1, 5000));
        } else if (config.mode == STRICT) {
            // We use the existence of fingerprints to trigger strict mode,
            // rather than hostname validation.
            const auto& fingerprint =
                    (config.withWorkingTLS) ? tls.fingerprint() : NOOP_FINGERPRINT;
            ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains,
                                                       kDefaultParams, "",
                                                       {base64Encode(fingerprint)}));
            // Wait for validation to complete.
            if (config.withWorkingTLS) EXPECT_TRUE(tls.waitForQueries(1, 5000));
        } else {
            FAIL() << "Unsupported Private DNS mode: " << config.mode;
        }

        const int tlsQueriesBefore = tls.queries();

        const hostent* h_result = nullptr;
        ScopedAddrinfo ai_result;

        if (config.method == GETHOSTBYNAME) {
            ASSERT_EQ(0, setNetworkForResolv(BYPASS_NETID));
            h_result = gethostbyname(host_name);

            EXPECT_EQ(1U, GetNumQueriesForType(dns, ns_type::ns_t_a, host_name));
            ASSERT_FALSE(h_result == nullptr);
            ASSERT_EQ(4, h_result->h_length);
            ASSERT_FALSE(h_result->h_addr_list[0] == nullptr);
            EXPECT_EQ(ADDR4, ToString(h_result));
            EXPECT_TRUE(h_result->h_addr_list[1] == nullptr);
        } else if (config.method == GETADDRINFO) {
            ASSERT_EQ(0, setNetworkForResolv(BYPASS_NETID));
            ai_result = safe_getaddrinfo(host_name, nullptr, nullptr);
            EXPECT_TRUE(ai_result != nullptr);

            EXPECT_LE(1U, GetNumQueries(dns, host_name));
            // Could be A or AAAA
            const std::string result_str = ToString(ai_result);
            EXPECT_TRUE(result_str == ADDR4 || result_str == ADDR6)
                << ", result_str='" << result_str << "'";
        } else if (config.method == GETADDRINFOFORNET) {
            addrinfo* raw_ai_result = nullptr;
            EXPECT_EQ(0, android_getaddrinfofornet(host_name, /*servname=*/nullptr,
                                                   /*hints=*/nullptr, BYPASS_NETID, MARK_UNSET,
                                                   &raw_ai_result));
            ai_result.reset(raw_ai_result);

            EXPECT_LE(1U, GetNumQueries(dns, host_name));
            // Could be A or AAAA
            const std::string result_str = ToString(ai_result);
            EXPECT_TRUE(result_str == ADDR4 || result_str == ADDR6)
                << ", result_str='" << result_str << "'";
        } else {
            FAIL() << "Unsupported query method: " << config.method;
        }

        const int tlsQueriesAfter = tls.queries();
        EXPECT_EQ(0, tlsQueriesAfter - tlsQueriesBefore);

        // Clear per-process resolv netid.
        ASSERT_EQ(0, setNetworkForResolv(NETID_UNSET));
        tls.stopServer();
        dns.clearQueries();
    }
}

TEST_F(ResolverTest, StrictMode_NoTlsServers) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    const std::vector<uint8_t> NOOP_FINGERPRINT(SHA256_SIZE, 0U);
    constexpr char cleartext_addr[] = "127.0.0.53";
    const std::vector<std::string> servers = { cleartext_addr };
    constexpr char host_name[] = "strictmode.notlsips.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(cleartext_addr);
    StartDns(dns, records);

    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, {},
                                               "", {base64Encode(NOOP_FINGERPRINT)}));

    addrinfo* ai_result = nullptr;
    EXPECT_NE(0, getaddrinfo(host_name, nullptr, nullptr, &ai_result));
    EXPECT_EQ(0U, GetNumQueries(dns, host_name));
}

namespace {

int getAsyncResponse(int fd, int* rcode, uint8_t* buf, int bufLen) {
    struct pollfd wait_fd[1];
    wait_fd[0].fd = fd;
    wait_fd[0].events = POLLIN;
    short revents;
    int ret;

    ret = poll(wait_fd, 1, -1);
    revents = wait_fd[0].revents;
    if (revents & POLLIN) {
        int n = resNetworkResult(fd, rcode, buf, bufLen);
        // Verify that resNetworkResult() closed the fd
        char dummy;
        EXPECT_EQ(-1, read(fd, &dummy, sizeof dummy));
        EXPECT_EQ(EBADF, errno);
        return n;
    }
    return -1;
}

std::string toString(uint8_t* buf, int bufLen, int ipType) {
    ns_msg handle;
    int ancount, n = 0;
    ns_rr rr;

    if (ns_initparse((const uint8_t*) buf, bufLen, &handle) >= 0) {
        ancount = ns_msg_count(handle, ns_s_an);
        if (ns_parserr(&handle, ns_s_an, n, &rr) == 0) {
            const uint8_t* rdata = ns_rr_rdata(rr);
            char buffer[INET6_ADDRSTRLEN];
            if (inet_ntop(ipType, (const char*) rdata, buffer, sizeof(buffer))) {
                return buffer;
            }
        }
    }
    return "";
}

int dns_open_proxy() {
    int s = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (s == -1) {
        return -1;
    }
    const int one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    static const struct sockaddr_un proxy_addr = {
            .sun_family = AF_UNIX,
            .sun_path = "/dev/socket/dnsproxyd",
    };

    if (TEMP_FAILURE_RETRY(connect(s, (const struct sockaddr*) &proxy_addr, sizeof(proxy_addr))) !=
        0) {
        close(s);
        return -1;
    }

    return s;
}

void expectAnswersValid(int fd, int ipType, const std::string& expectedAnswer) {
    int rcode = -1;
    uint8_t buf[MAXPACKET] = {};

    int res = getAsyncResponse(fd, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ(expectedAnswer, toString(buf, res, ipType));
}

void expectAnswersNotValid(int fd, int expectedErrno) {
    int rcode = -1;
    uint8_t buf[MAXPACKET] = {};

    int res = getAsyncResponse(fd, &rcode, buf, MAXPACKET);
    EXPECT_EQ(expectedErrno, res);
}

}  // namespace

TEST_F(ResolverTest, Async_NormalQueryV4V6) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    int fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    int fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);
    EXPECT_TRUE(fd1 != -1);
    EXPECT_TRUE(fd2 != -1);

    uint8_t buf[MAXPACKET] = {};
    int rcode;
    int res = getAsyncResponse(fd2, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("::1.2.3.4", toString(buf, res, AF_INET6));

    res = getAsyncResponse(fd1, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("1.2.3.4", toString(buf, res, AF_INET));

    EXPECT_EQ(2U, GetNumQueries(dns, host_name));

    // Re-query verify cache works
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);

    EXPECT_TRUE(fd1 != -1);
    EXPECT_TRUE(fd2 != -1);

    res = getAsyncResponse(fd2, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("::1.2.3.4", toString(buf, res, AF_INET6));

    res = getAsyncResponse(fd1, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("1.2.3.4", toString(buf, res, AF_INET));

    EXPECT_EQ(2U, GetNumQueries(dns, host_name));
}

TEST_F(ResolverTest, Async_BadQuery) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    static struct {
        int fd;
        const char* dname;
        const int queryType;
        const int expectRcode;
    } kTestData[] = {
            {-1, "", ns_t_aaaa, 0},
            {-1, "as65ass46", ns_t_aaaa, 0},
            {-1, "454564564564", ns_t_aaaa, 0},
            {-1, "h645235", ns_t_a, 0},
            {-1, "www.google.com", ns_t_a, 0},
    };

    for (auto& td : kTestData) {
        SCOPED_TRACE(td.dname);
        td.fd = resNetworkQuery(TEST_NETID, td.dname, ns_c_in, td.queryType, 0);
        EXPECT_TRUE(td.fd != -1);
    }

    // dns_responder return empty resp(packet only contains query part) with no error currently
    for (const auto& td : kTestData) {
        uint8_t buf[MAXPACKET] = {};
        int rcode;
        SCOPED_TRACE(td.dname);
        int res = getAsyncResponse(td.fd, &rcode, buf, MAXPACKET);
        EXPECT_GT(res, 0);
        EXPECT_EQ(rcode, td.expectRcode);
    }
}

TEST_F(ResolverTest, Async_EmptyAnswer) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // TODO: Disable retry to make this test explicit.
    auto& cv = dns.getCv();
    auto& cvMutex = dns.getCvMutex();
    int fd1;
    // Wait on the condition variable to ensure that the DNS server has handled our first query.
    {
        std::unique_lock lk(cvMutex);
        fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);
        EXPECT_TRUE(fd1 != -1);
        EXPECT_EQ(std::cv_status::no_timeout, cv.wait_for(lk, std::chrono::seconds(1)));
    }

    dns.setResponseProbability(0.0);

    int fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    EXPECT_TRUE(fd2 != -1);

    int fd3 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    EXPECT_TRUE(fd3 != -1);

    uint8_t buf[MAXPACKET] = {};
    int rcode;

    // expect no response
    int res = getAsyncResponse(fd3, &rcode, buf, MAXPACKET);
    EXPECT_EQ(-ETIMEDOUT, res);

    // expect no response
    memset(buf, 0, MAXPACKET);
    res = getAsyncResponse(fd2, &rcode, buf, MAXPACKET);
    EXPECT_EQ(-ETIMEDOUT, res);

    dns.setResponseProbability(1.0);

    int fd4 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    EXPECT_TRUE(fd4 != -1);

    memset(buf, 0, MAXPACKET);
    res = getAsyncResponse(fd4, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("1.2.3.4", toString(buf, res, AF_INET));

    memset(buf, 0, MAXPACKET);
    res = getAsyncResponse(fd1, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("::1.2.3.4", toString(buf, res, AF_INET6));
}

TEST_F(ResolverTest, Async_MalformedQuery) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    int fd = dns_open_proxy();
    EXPECT_TRUE(fd > 0);

    const std::string badMsg = "16-52512#";
    static const struct {
        const std::string cmd;
        const int expectErr;
    } kTestData[] = {
            // Too few arguments
            {"resnsend " + badMsg + '\0', -EINVAL},
            // Bad netId
            {"resnsend badnetId 0 " + badMsg + '\0', -EINVAL},
            // Bad raw data
            {"resnsend " + std::to_string(TEST_NETID) + " 0 " + badMsg + '\0', -EILSEQ},
    };

    for (unsigned int i = 0; i < std::size(kTestData); i++) {
        auto& td = kTestData[i];
        SCOPED_TRACE(td.cmd);
        ssize_t rc = TEMP_FAILURE_RETRY(write(fd, td.cmd.c_str(), td.cmd.size()));
        EXPECT_EQ(rc, static_cast<ssize_t>(td.cmd.size()));

        int32_t tmp;
        rc = TEMP_FAILURE_RETRY(read(fd, &tmp, sizeof(tmp)));
        EXPECT_TRUE(rc > 0);
        EXPECT_EQ(static_cast<int>(ntohl(tmp)), td.expectErr);
    }
    // Normal query with answer buffer
    // This is raw data of query "howdy.example.com" type 1 class 1
    std::string query = "81sBAAABAAAAAAAABWhvd2R5B2V4YW1wbGUDY29tAAABAAE=";
    std::string cmd = "resnsend " + std::to_string(TEST_NETID) + " 0 " + query + '\0';
    ssize_t rc = TEMP_FAILURE_RETRY(write(fd, cmd.c_str(), cmd.size()));
    EXPECT_EQ(rc, static_cast<ssize_t>(cmd.size()));

    uint8_t smallBuf[1] = {};
    int rcode;
    rc = getAsyncResponse(fd, &rcode, smallBuf, 1);
    EXPECT_EQ(-EMSGSIZE, rc);

    // Do the normal test with large buffer again
    fd = dns_open_proxy();
    EXPECT_TRUE(fd > 0);
    rc = TEMP_FAILURE_RETRY(write(fd, cmd.c_str(), cmd.size()));
    EXPECT_EQ(rc, static_cast<ssize_t>(cmd.size()));
    uint8_t buf[MAXPACKET] = {};
    rc = getAsyncResponse(fd, &rcode, buf, MAXPACKET);
    EXPECT_EQ("1.2.3.4", toString(buf, rc, AF_INET));
}

TEST_F(ResolverTest, Async_CacheFlags) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    constexpr char another_host_name[] = "howdy.example2.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
            {another_host_name, ns_type::ns_t_a, "1.2.3.5"},
            {another_host_name, ns_type::ns_t_aaaa, "::1.2.3.5"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // ANDROID_RESOLV_NO_CACHE_STORE
    int fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                              ANDROID_RESOLV_NO_CACHE_STORE);
    EXPECT_TRUE(fd1 != -1);
    int fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                              ANDROID_RESOLV_NO_CACHE_STORE);
    EXPECT_TRUE(fd2 != -1);
    int fd3 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                              ANDROID_RESOLV_NO_CACHE_STORE);
    EXPECT_TRUE(fd3 != -1);

    expectAnswersValid(fd3, AF_INET, "1.2.3.4");
    expectAnswersValid(fd2, AF_INET, "1.2.3.4");
    expectAnswersValid(fd1, AF_INET, "1.2.3.4");

    // No cache exists, expect 3 queries
    EXPECT_EQ(3U, GetNumQueries(dns, host_name));

    // Re-query and cache
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);

    EXPECT_TRUE(fd1 != -1);

    expectAnswersValid(fd1, AF_INET, "1.2.3.4");

    // Now we have cache, expect 4 queries
    EXPECT_EQ(4U, GetNumQueries(dns, host_name));

    // ANDROID_RESOLV_NO_CACHE_LOOKUP
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                          ANDROID_RESOLV_NO_CACHE_LOOKUP);
    fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                          ANDROID_RESOLV_NO_CACHE_LOOKUP);

    EXPECT_TRUE(fd1 != -1);
    EXPECT_TRUE(fd2 != -1);

    expectAnswersValid(fd2, AF_INET, "1.2.3.4");
    expectAnswersValid(fd1, AF_INET, "1.2.3.4");

    // Skip cache, expect 6 queries
    EXPECT_EQ(6U, GetNumQueries(dns, host_name));

    // Re-query verify cache works
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                          ANDROID_RESOLV_NO_CACHE_STORE);
    EXPECT_TRUE(fd1 != -1);
    expectAnswersValid(fd1, AF_INET, "1.2.3.4");

    // Cache hits,  expect still 6 queries
    EXPECT_EQ(6U, GetNumQueries(dns, host_name));

    // Start to verify if ANDROID_RESOLV_NO_CACHE_LOOKUP does write response into cache
    dns.clearQueries();

    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa,
                          ANDROID_RESOLV_NO_CACHE_LOOKUP);
    fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa,
                          ANDROID_RESOLV_NO_CACHE_LOOKUP);

    EXPECT_TRUE(fd1 != -1);
    EXPECT_TRUE(fd2 != -1);

    expectAnswersValid(fd2, AF_INET6, "::1.2.3.4");
    expectAnswersValid(fd1, AF_INET6, "::1.2.3.4");

    // Skip cache, expect 2 queries
    EXPECT_EQ(2U, GetNumQueries(dns, host_name));

    // Re-query without flags
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);
    fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);

    EXPECT_TRUE(fd1 != -1);
    EXPECT_TRUE(fd2 != -1);

    expectAnswersValid(fd2, AF_INET6, "::1.2.3.4");
    expectAnswersValid(fd1, AF_INET6, "::1.2.3.4");

    // Cache hits, expect still 2 queries
    EXPECT_EQ(2U, GetNumQueries(dns, host_name));

    // Test both ANDROID_RESOLV_NO_CACHE_STORE and ANDROID_RESOLV_NO_CACHE_LOOKUP are set
    dns.clearQueries();

    // Make sure that the cache of "howdy.example2.com" exists.
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example2.com", ns_c_in, ns_t_aaaa, 0);
    EXPECT_TRUE(fd1 != -1);
    expectAnswersValid(fd1, AF_INET6, "::1.2.3.5");
    EXPECT_EQ(1U, GetNumQueries(dns, another_host_name));

    // Re-query with testFlags
    const int testFlag = ANDROID_RESOLV_NO_CACHE_STORE | ANDROID_RESOLV_NO_CACHE_LOOKUP;
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example2.com", ns_c_in, ns_t_aaaa, testFlag);
    EXPECT_TRUE(fd1 != -1);
    expectAnswersValid(fd1, AF_INET6, "::1.2.3.5");
    // Expect cache lookup is skipped.
    EXPECT_EQ(2U, GetNumQueries(dns, another_host_name));

    // Do another query with testFlags
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example2.com", ns_c_in, ns_t_a, testFlag);
    EXPECT_TRUE(fd1 != -1);
    expectAnswersValid(fd1, AF_INET, "1.2.3.5");
    // Expect cache lookup is skipped.
    EXPECT_EQ(3U, GetNumQueries(dns, another_host_name));

    // Re-query with no flags
    fd1 = resNetworkQuery(TEST_NETID, "howdy.example2.com", ns_c_in, ns_t_a, 0);
    EXPECT_TRUE(fd1 != -1);
    expectAnswersValid(fd1, AF_INET, "1.2.3.5");
    // Expect no cache hit because cache storing is also skipped in previous query.
    EXPECT_EQ(4U, GetNumQueries(dns, another_host_name));
}

TEST_F(ResolverTest, Async_NoRetryFlag) {
    constexpr char listen_addr0[] = "127.0.0.4";
    constexpr char listen_addr1[] = "127.0.0.6";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns0(listen_addr0);
    test::DNSResponder dns1(listen_addr1);
    StartDns(dns0, records);
    StartDns(dns1, records);
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork({listen_addr0, listen_addr1}));

    dns0.clearQueries();
    dns1.clearQueries();

    dns0.setResponseProbability(0.0);
    dns1.setResponseProbability(0.0);

    int fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a,
                              ANDROID_RESOLV_NO_RETRY);
    EXPECT_TRUE(fd1 != -1);

    int fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa,
                              ANDROID_RESOLV_NO_RETRY);
    EXPECT_TRUE(fd2 != -1);

    // expect no response
    expectAnswersNotValid(fd1, -ETIMEDOUT);
    expectAnswersNotValid(fd2, -ETIMEDOUT);

    // No retry case, expect total 2 queries. The server is selected randomly.
    EXPECT_EQ(2U, GetNumQueries(dns0, host_name) + GetNumQueries(dns1, host_name));

    dns0.clearQueries();
    dns1.clearQueries();

    fd1 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_a, 0);
    EXPECT_TRUE(fd1 != -1);

    fd2 = resNetworkQuery(TEST_NETID, "howdy.example.com", ns_c_in, ns_t_aaaa, 0);
    EXPECT_TRUE(fd2 != -1);

    // expect no response
    expectAnswersNotValid(fd1, -ETIMEDOUT);
    expectAnswersNotValid(fd2, -ETIMEDOUT);

    // Retry case, expect 4 queries
    EXPECT_EQ(4U, GetNumQueries(dns0, host_name));
    EXPECT_EQ(4U, GetNumQueries(dns1, host_name));
}

TEST_F(ResolverTest, Async_VerifyQueryID) {
    constexpr char listen_addr[] = "127.0.0.4";
    constexpr char host_name[] = "howdy.example.com.";
    const std::vector<DnsRecord> records = {
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    const uint8_t queryBuf1[] = {
            /* Header */
            0x55, 0x66, /* Transaction ID */
            0x01, 0x00, /* Flags */
            0x00, 0x01, /* Questions */
            0x00, 0x00, /* Answer RRs */
            0x00, 0x00, /* Authority RRs */
            0x00, 0x00, /* Additional RRs */
            /* Queries */
            0x05, 0x68, 0x6f, 0x77, 0x64, 0x79, 0x07, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65,
            0x03, 0x63, 0x6f, 0x6d, 0x00, /* Name */
            0x00, 0x01,                   /* Type */
            0x00, 0x01                    /* Class */
    };

    int fd = resNetworkSend(TEST_NETID, queryBuf1, sizeof(queryBuf1), 0);
    EXPECT_TRUE(fd != -1);

    uint8_t buf[MAXPACKET] = {};
    int rcode;

    int res = getAsyncResponse(fd, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("1.2.3.4", toString(buf, res, AF_INET));

    auto hp = reinterpret_cast<HEADER*>(buf);
    EXPECT_EQ(21862U, htons(hp->id));

    EXPECT_EQ(1U, GetNumQueries(dns, host_name));

    const uint8_t queryBuf2[] = {
            /* Header */
            0x00, 0x53, /* Transaction ID */
            0x01, 0x00, /* Flags */
            0x00, 0x01, /* Questions */
            0x00, 0x00, /* Answer RRs */
            0x00, 0x00, /* Authority RRs */
            0x00, 0x00, /* Additional RRs */
            /* Queries */
            0x05, 0x68, 0x6f, 0x77, 0x64, 0x79, 0x07, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65,
            0x03, 0x63, 0x6f, 0x6d, 0x00, /* Name */
            0x00, 0x01,                   /* Type */
            0x00, 0x01                    /* Class */
    };

    // Re-query verify cache works and query id is correct
    fd = resNetworkSend(TEST_NETID, queryBuf2, sizeof(queryBuf2), 0);

    EXPECT_TRUE(fd != -1);

    res = getAsyncResponse(fd, &rcode, buf, MAXPACKET);
    EXPECT_GT(res, 0);
    EXPECT_EQ("1.2.3.4", toString(buf, res, AF_INET));

    EXPECT_EQ(0x0053U, htons(hp->id));

    EXPECT_EQ(1U, GetNumQueries(dns, host_name));
}

// This test checks that the resolver should not generate the request containing OPT RR when using
// cleartext DNS. If we query the DNS server not supporting EDNS0 and it reponds with
// FORMERR_ON_EDNS, we will fallback to no EDNS0 and try again. If the server does no response, we
// won't retry so that we get no answer.
TEST_F(ResolverTest, BrokenEdns) {
    // Certificate fingerprints are no longer supported by the module.
    SKIP_IF_RESOLVER_VERSION_NEWER_THAN(mDnsClient.resolvService(), 2);

    typedef test::DNSResponder::Edns Edns;
    enum ExpectResult { EXPECT_FAILURE, EXPECT_SUCCESS };

    const char OFF[] = "off";
    const char OPPORTUNISTIC_UDP[] = "opportunistic_udp";
    const char OPPORTUNISTIC_TLS[] = "opportunistic_tls";
    const char STRICT[] = "strict";
    const char GETHOSTBYNAME[] = "gethostbyname";
    const char GETADDRINFO[] = "getaddrinfo";
    const std::vector<uint8_t> NOOP_FINGERPRINT(SHA256_SIZE, 0U);
    const char ADDR4[] = "192.0.2.1";
    const char CLEARTEXT_ADDR[] = "127.0.0.53";
    const char CLEARTEXT_PORT[] = "53";
    const char TLS_PORT[] = "853";
    const std::vector<std::string> servers = { CLEARTEXT_ADDR };

    test::DNSResponder dns(CLEARTEXT_ADDR, CLEARTEXT_PORT, 250, ns_rcode::ns_r_servfail);
    ASSERT_TRUE(dns.startServer());

    test::DnsTlsFrontend tls(CLEARTEXT_ADDR, TLS_PORT, CLEARTEXT_ADDR, CLEARTEXT_PORT);

    static const struct TestConfig {
        std::string mode;
        std::string method;
        Edns edns;
        ExpectResult expectResult;

        std::string asHostName() const {
            const char* ednsString;
            switch (edns) {
                case Edns::ON:
                    ednsString = "ednsOn";
                    break;
                case Edns::FORMERR_ON_EDNS:
                    ednsString = "ednsFormerr";
                    break;
                case Edns::DROP:
                    ednsString = "ednsDrop";
                    break;
                default:
                    ednsString = "";
                    break;
            }
            return StringPrintf("%s.%s.%s.", mode.c_str(), method.c_str(), ednsString);
        }
    } testConfigs[] = {
            // In OPPORTUNISTIC_TLS, we get no answer if the DNS server supports TLS but not EDNS0.
            // Could such server exist? if so, we might need to fallback to query cleartext DNS.
            // Another thing is that {OPPORTUNISTIC_TLS, Edns::DROP} and {STRICT, Edns::DROP} are
            // commented out since TLS timeout is not configurable.
            // TODO: Uncomment them after TLS timeout is configurable.
            {OFF,               GETHOSTBYNAME, Edns::ON,      EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETHOSTBYNAME, Edns::ON,      EXPECT_SUCCESS},
            {OPPORTUNISTIC_TLS, GETHOSTBYNAME, Edns::ON,      EXPECT_SUCCESS},
            {STRICT,            GETHOSTBYNAME, Edns::ON,      EXPECT_SUCCESS},
            {OFF,               GETHOSTBYNAME, Edns::FORMERR_ON_EDNS, EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETHOSTBYNAME, Edns::FORMERR_ON_EDNS, EXPECT_SUCCESS},
            {OPPORTUNISTIC_TLS, GETHOSTBYNAME, Edns::FORMERR_ON_EDNS, EXPECT_FAILURE},
            {STRICT,            GETHOSTBYNAME, Edns::FORMERR_ON_EDNS, EXPECT_FAILURE},
            {OFF,               GETHOSTBYNAME, Edns::DROP,    EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETHOSTBYNAME, Edns::DROP,    EXPECT_SUCCESS},
            //{OPPORTUNISTIC_TLS, GETHOSTBYNAME, Edns::DROP,    EXPECT_FAILURE},
            //{STRICT,            GETHOSTBYNAME, Edns::DROP,    EXPECT_FAILURE},
            {OFF,               GETADDRINFO,   Edns::ON,      EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETADDRINFO,   Edns::ON,      EXPECT_SUCCESS},
            {OPPORTUNISTIC_TLS, GETADDRINFO,   Edns::ON,      EXPECT_SUCCESS},
            {STRICT,            GETADDRINFO,   Edns::ON,      EXPECT_SUCCESS},
            {OFF,               GETADDRINFO,   Edns::FORMERR_ON_EDNS, EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETADDRINFO,   Edns::FORMERR_ON_EDNS, EXPECT_SUCCESS},
            {OPPORTUNISTIC_TLS, GETADDRINFO,   Edns::FORMERR_ON_EDNS, EXPECT_FAILURE},
            {STRICT,            GETADDRINFO,   Edns::FORMERR_ON_EDNS, EXPECT_FAILURE},
            {OFF,               GETADDRINFO,   Edns::DROP,    EXPECT_SUCCESS},
            {OPPORTUNISTIC_UDP, GETADDRINFO,   Edns::DROP,    EXPECT_SUCCESS},
            //{OPPORTUNISTIC_TLS, GETADDRINFO,   Edns::DROP,   EXPECT_FAILURE},
            //{STRICT,            GETADDRINFO,   Edns::DROP,   EXPECT_FAILURE},
    };

    for (const auto& config : testConfigs) {
        const std::string testHostName = config.asHostName();
        SCOPED_TRACE(testHostName);

        const char* host_name = testHostName.c_str();
        dns.addMapping(host_name, ns_type::ns_t_a, ADDR4);
        dns.setEdns(config.edns);

        if (config.mode == OFF) {
            ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));
        } else if (config.mode == OPPORTUNISTIC_UDP) {
            ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains,
                                                       kDefaultParams, "", {}));
        } else if (config.mode == OPPORTUNISTIC_TLS) {
            ASSERT_TRUE(tls.startServer());
            ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains,
                                                       kDefaultParams, "", {}));
            // Wait for validation to complete.
            EXPECT_TRUE(tls.waitForQueries(1, 5000));
        } else if (config.mode == STRICT) {
            ASSERT_TRUE(tls.startServer());
            ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains,
                                                       kDefaultParams, "",
                                                       {base64Encode(tls.fingerprint())}));
            // Wait for validation to complete.
            EXPECT_TRUE(tls.waitForQueries(1, 5000));
        }

        if (config.method == GETHOSTBYNAME) {
            const hostent* h_result = gethostbyname(host_name);
            if (config.expectResult == EXPECT_SUCCESS) {
                EXPECT_LE(1U, GetNumQueries(dns, host_name));
                ASSERT_TRUE(h_result != nullptr);
                ASSERT_EQ(4, h_result->h_length);
                ASSERT_FALSE(h_result->h_addr_list[0] == nullptr);
                EXPECT_EQ(ADDR4, ToString(h_result));
                EXPECT_TRUE(h_result->h_addr_list[1] == nullptr);
            } else {
                EXPECT_EQ(0U, GetNumQueriesForType(dns, ns_type::ns_t_a, host_name));
                ASSERT_TRUE(h_result == nullptr);
                ASSERT_EQ(HOST_NOT_FOUND, h_errno);
            }
        } else if (config.method == GETADDRINFO) {
            ScopedAddrinfo ai_result;
            addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};
            ai_result = safe_getaddrinfo(host_name, nullptr, &hints);
            if (config.expectResult == EXPECT_SUCCESS) {
                EXPECT_TRUE(ai_result != nullptr);
                EXPECT_EQ(1U, GetNumQueries(dns, host_name));
                const std::string result_str = ToString(ai_result);
                EXPECT_EQ(ADDR4, result_str);
            } else {
                EXPECT_TRUE(ai_result == nullptr);
                EXPECT_EQ(0U, GetNumQueries(dns, host_name));
            }
        } else {
            FAIL() << "Unsupported query method: " << config.method;
        }

        tls.stopServer();
        dns.clearQueries();
    }
}

// DNS-over-TLS validation success, but server does not respond to TLS query after a while.
// Resolver should have a reasonable number of retries instead of spinning forever. We don't have
// an efficient way to know if resolver is stuck in an infinite loop. However, test case will be
// failed due to timeout.
TEST_F(ResolverTest, UnstableTls) {
    const char CLEARTEXT_ADDR[] = "127.0.0.53";
    const char CLEARTEXT_PORT[] = "53";
    const char TLS_PORT[] = "853";
    const char* host_name1 = "nonexistent1.example.com.";
    const char* host_name2 = "nonexistent2.example.com.";
    const std::vector<std::string> servers = {CLEARTEXT_ADDR};

    test::DNSResponder dns(CLEARTEXT_ADDR, CLEARTEXT_PORT, 250, ns_rcode::ns_r_servfail);
    ASSERT_TRUE(dns.startServer());
    dns.setEdns(test::DNSResponder::Edns::FORMERR_ON_EDNS);
    test::DnsTlsFrontend tls(CLEARTEXT_ADDR, TLS_PORT, CLEARTEXT_ADDR, CLEARTEXT_PORT);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));
    // Wait for validation complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));
    // Shutdown TLS server to get an error. It's similar to no response case but without waiting.
    tls.stopServer();

    const hostent* h_result = gethostbyname(host_name1);
    EXPECT_EQ(1U, GetNumQueries(dns, host_name1));
    ASSERT_TRUE(h_result == nullptr);
    ASSERT_EQ(HOST_NOT_FOUND, h_errno);

    addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};
    ScopedAddrinfo ai_result = safe_getaddrinfo(host_name2, nullptr, &hints);
    EXPECT_TRUE(ai_result == nullptr);
    EXPECT_EQ(1U, GetNumQueries(dns, host_name2));
}

// DNS-over-TLS validation success, but server does not respond to TLS query after a while.
// Moreover, server responds RCODE=FORMERR even on non-EDNS query.
TEST_F(ResolverTest, BogusDnsServer) {
    const char CLEARTEXT_ADDR[] = "127.0.0.53";
    const char CLEARTEXT_PORT[] = "53";
    const char TLS_PORT[] = "853";
    const char* host_name1 = "nonexistent1.example.com.";
    const char* host_name2 = "nonexistent2.example.com.";
    const std::vector<std::string> servers = {CLEARTEXT_ADDR};

    test::DNSResponder dns(CLEARTEXT_ADDR, CLEARTEXT_PORT, 250, ns_rcode::ns_r_servfail);
    ASSERT_TRUE(dns.startServer());
    test::DnsTlsFrontend tls(CLEARTEXT_ADDR, TLS_PORT, CLEARTEXT_ADDR, CLEARTEXT_PORT);
    ASSERT_TRUE(tls.startServer());
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));
    // Wait for validation complete.
    EXPECT_TRUE(tls.waitForQueries(1, 5000));
    // Shutdown TLS server to get an error. It's similar to no response case but without waiting.
    tls.stopServer();
    dns.setEdns(test::DNSResponder::Edns::FORMERR_UNCOND);

    const hostent* h_result = gethostbyname(host_name1);
    EXPECT_EQ(0U, GetNumQueries(dns, host_name1));
    ASSERT_TRUE(h_result == nullptr);
    ASSERT_EQ(HOST_NOT_FOUND, h_errno);

    addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};
    ScopedAddrinfo ai_result = safe_getaddrinfo(host_name2, nullptr, &hints);
    EXPECT_TRUE(ai_result == nullptr);
    EXPECT_EQ(0U, GetNumQueries(dns, host_name2));
}

TEST_F(ResolverTest, GetAddrInfo_Dns64Synthesize) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4only.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);

    std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // hints are necessary in order to let netd know which type of addresses the caller is
    // interested in.
    const addrinfo hints = {.ai_family = AF_UNSPEC};
    ScopedAddrinfo result = safe_getaddrinfo("v4only", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    // TODO: BUG: there should only be two queries, one AAAA (which returns no records) and one A
    // (which returns 1.2.3.4). But there is an extra AAAA.
    EXPECT_EQ(3U, GetNumQueries(dns, host_name));

    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");

    // Stopping NAT64 prefix discovery disables synthesis.
    EXPECT_TRUE(mDnsClient.resolvService()->stopPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_FALSE(WaitForPrefix64Detected(TEST_NETID, 300));

    dns.clearQueries();

    result = safe_getaddrinfo("v4only", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    // TODO: BUG: there should only be one query, an AAAA (which returns no records), because the
    // A is already cached. But there is an extra AAAA.
    EXPECT_EQ(2U, GetNumQueries(dns, host_name));

    result_str = ToString(result);
    EXPECT_EQ(result_str, "1.2.3.4");
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QuerySpecified) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4only.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Ensure to synthesize AAAA if AF_INET6 is specified, and not to synthesize AAAA
    // in AF_INET case.
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    ScopedAddrinfo result = safe_getaddrinfo("v4only", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");

    hints.ai_family = AF_INET;
    result = safe_getaddrinfo("v4only", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_LE(2U, GetNumQueries(dns, host_name));
    result_str = ToString(result);
    EXPECT_EQ(result_str, "1.2.3.4");
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QueryUnspecifiedV6) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4v6.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "2001:db8::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    const addrinfo hints = {.ai_family = AF_UNSPEC};
    ScopedAddrinfo result = safe_getaddrinfo("v4v6", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_LE(2U, GetNumQueries(dns, host_name));

    // In AF_UNSPEC case, do not synthesize AAAA if there's at least one AAAA answer.
    const std::vector<std::string> result_strs = ToStrings(result);
    for (const auto& str : result_strs) {
        EXPECT_TRUE(str == "1.2.3.4" || str == "2001:db8::102:304")
                << ", result_str='" << str << "'";
    }
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QueryUnspecifiedNoV6) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4v6.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    const addrinfo hints = {.ai_family = AF_UNSPEC};
    ScopedAddrinfo result = safe_getaddrinfo("v4v6", nullptr, &hints);
    EXPECT_TRUE(result != nullptr);
    EXPECT_LE(2U, GetNumQueries(dns, host_name));

    // In AF_UNSPEC case, synthesize AAAA if there's no AAAA answer.
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QuerySpecialUseIPv4Addresses) {
    constexpr char THIS_NETWORK[] = "this_network";
    constexpr char LOOPBACK[] = "loopback";
    constexpr char LINK_LOCAL[] = "link_local";
    constexpr char MULTICAST[] = "multicast";
    constexpr char LIMITED_BROADCAST[] = "limited_broadcast";

    constexpr char ADDR_THIS_NETWORK[] = "0.0.0.1";
    constexpr char ADDR_LOOPBACK[] = "127.0.0.1";
    constexpr char ADDR_LINK_LOCAL[] = "169.254.0.1";
    constexpr char ADDR_MULTICAST[] = "224.0.0.1";
    constexpr char ADDR_LIMITED_BROADCAST[] = "255.255.255.255";

    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::"}});
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    static const struct TestConfig {
        std::string name;
        std::string addr;

        std::string asHostName() const { return StringPrintf("%s.example.com.", name.c_str()); }
    } testConfigs[]{
        {THIS_NETWORK,      ADDR_THIS_NETWORK},
        {LOOPBACK,          ADDR_LOOPBACK},
        {LINK_LOCAL,        ADDR_LINK_LOCAL},
        {MULTICAST,         ADDR_MULTICAST},
        {LIMITED_BROADCAST, ADDR_LIMITED_BROADCAST}
    };

    for (const auto& config : testConfigs) {
        const std::string testHostName = config.asHostName();
        SCOPED_TRACE(testHostName);

        const char* host_name = testHostName.c_str();
        dns.addMapping(host_name, ns_type::ns_t_a, config.addr.c_str());

        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET6;
        ScopedAddrinfo result = safe_getaddrinfo(config.name.c_str(), nullptr, &hints);
        // In AF_INET6 case, don't return IPv4 answers
        EXPECT_TRUE(result == nullptr);
        EXPECT_LE(2U, GetNumQueries(dns, host_name));
        dns.clearQueries();

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        result = safe_getaddrinfo(config.name.c_str(), nullptr, &hints);
        EXPECT_TRUE(result != nullptr);
        // Expect IPv6 query only. IPv4 answer has been cached in previous query.
        EXPECT_LE(1U, GetNumQueries(dns, host_name));
        // In AF_UNSPEC case, don't synthesize special use IPv4 address.
        std::string result_str = ToString(result);
        EXPECT_EQ(result_str, config.addr.c_str());
        dns.clearQueries();
    }
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QueryWithNullArgumentHints) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4only.example.com.";
    constexpr char host_name2[] = "v4v6.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name2, ns_type::ns_t_a, "1.2.3.4"},
            {host_name2, ns_type::ns_t_aaaa, "2001:db8::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Assign argument hints of getaddrinfo() as null is equivalent to set ai_family AF_UNSPEC.
    // In AF_UNSPEC case, synthesize AAAA if there has A answer only.
    ScopedAddrinfo result = safe_getaddrinfo("v4only", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    EXPECT_LE(2U, GetNumQueries(dns, host_name));
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");
    dns.clearQueries();

    // In AF_UNSPEC case, do not synthesize AAAA if there's at least one AAAA answer.
    result = safe_getaddrinfo("v4v6", nullptr, nullptr);
    EXPECT_TRUE(result != nullptr);
    EXPECT_LE(2U, GetNumQueries(dns, host_name2));
    std::vector<std::string> result_strs = ToStrings(result);
    for (const auto& str : result_strs) {
        EXPECT_TRUE(str == "1.2.3.4" || str == "2001:db8::102:304")
                << ", result_str='" << str << "'";
    }
}

TEST_F(ResolverTest, GetAddrInfo_Dns64QueryNullArgumentNode) {
    constexpr char ADDR_ANYADDR_V4[] = "0.0.0.0";
    constexpr char ADDR_ANYADDR_V6[] = "::";
    constexpr char ADDR_LOCALHOST_V4[] = "127.0.0.1";
    constexpr char ADDR_LOCALHOST_V6[] = "::1";

    constexpr char PORT_NAME_HTTP[] = "http";
    constexpr char PORT_NUMBER_HTTP[] = "80";

    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::"}});
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // If node is null, return address is listed by libc/getaddrinfo.c as follows.
    // - passive socket -> anyaddr (0.0.0.0 or ::)
    // - non-passive socket -> localhost (127.0.0.1 or ::1)
    static const struct TestConfig {
        int flag;
        std::string addr_v4;
        std::string addr_v6;

        std::string asParameters() const {
            return StringPrintf("flag=%d, addr_v4=%s, addr_v6=%s", flag, addr_v4.c_str(),
                                addr_v6.c_str());
        }
    } testConfigs[]{
        {0 /* non-passive */, ADDR_LOCALHOST_V4, ADDR_LOCALHOST_V6},
        {AI_PASSIVE,          ADDR_ANYADDR_V4,   ADDR_ANYADDR_V6}
    };

    for (const auto& config : testConfigs) {
        SCOPED_TRACE(config.asParameters());

        addrinfo hints = {
                .ai_family = AF_UNSPEC,  // any address family
                .ai_socktype = 0,        // any type
                .ai_protocol = 0,        // any protocol
                .ai_flags = config.flag,
        };

        // Assign hostname as null and service as port name.
        ScopedAddrinfo result = safe_getaddrinfo(nullptr, PORT_NAME_HTTP, &hints);
        ASSERT_TRUE(result != nullptr);

        // Can't be synthesized because it should not get into Netd.
        std::vector<std::string> result_strs = ToStrings(result);
        for (const auto& str : result_strs) {
            EXPECT_TRUE(str == config.addr_v4 || str == config.addr_v6)
                    << ", result_str='" << str << "'";
        }

        // Assign hostname as null and service as numeric port number.
        hints.ai_flags = config.flag | AI_NUMERICSERV;
        result = safe_getaddrinfo(nullptr, PORT_NUMBER_HTTP, &hints);
        ASSERT_TRUE(result != nullptr);

        // Can't be synthesized because it should not get into Netd.
        result_strs = ToStrings(result);
        for (const auto& str : result_strs) {
            EXPECT_TRUE(str == config.addr_v4 || str == config.addr_v6)
                    << ", result_str='" << str << "'";
        }
    }
}

TEST_F(ResolverTest, GetHostByAddr_ReverseDnsQueryWithHavingNat64Prefix) {
    struct hostent* result = nullptr;
    struct in_addr v4addr;
    struct in6_addr v6addr;

    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char ptr_name[] = "v4v6.example.com.";
    // PTR record for IPv4 address 1.2.3.4
    constexpr char ptr_addr_v4[] = "4.3.2.1.in-addr.arpa.";
    // PTR record for IPv6 address 2001:db8::102:304
    constexpr char ptr_addr_v6[] =
            "4.0.3.0.2.0.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {ptr_addr_v4, ns_type::ns_t_ptr, ptr_name},
            {ptr_addr_v6, ns_type::ns_t_ptr, ptr_name},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Reverse IPv4 DNS query. Prefix should have no effect on it.
    inet_pton(AF_INET, "1.2.3.4", &v4addr);
    result = gethostbyaddr(&v4addr, sizeof(v4addr), AF_INET);
    ASSERT_TRUE(result != nullptr);
    std::string result_str = result->h_name ? result->h_name : "null";
    EXPECT_EQ(result_str, "v4v6.example.com");

    // Reverse IPv6 DNS query. Prefix should have no effect on it.
    inet_pton(AF_INET6, "2001:db8::102:304", &v6addr);
    result = gethostbyaddr(&v6addr, sizeof(v6addr), AF_INET6);
    ASSERT_TRUE(result != nullptr);
    result_str = result->h_name ? result->h_name : "null";
    EXPECT_EQ(result_str, "v4v6.example.com");
}

TEST_F(ResolverTest, GetHostByAddr_ReverseDns64Query) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char ptr_name[] = "v4only.example.com.";
    // PTR record for IPv4 address 1.2.3.4
    constexpr char ptr_addr_v4[] = "4.3.2.1.in-addr.arpa.";
    // PTR record for IPv6 address 64:ff9b::1.2.3.4
    constexpr char ptr_addr_v6_nomapping[] =
            "4.0.3.0.2.0.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.b.9.f.f.4.6.0.0.ip6.arpa.";
    constexpr char ptr_name_v6_synthesis[] = "v6synthesis.example.com.";
    // PTR record for IPv6 address 64:ff9b::5.6.7.8
    constexpr char ptr_addr_v6_synthesis[] =
            "8.0.7.0.6.0.5.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.b.9.f.f.4.6.0.0.ip6.arpa.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {ptr_addr_v4, ns_type::ns_t_ptr, ptr_name},
            {ptr_addr_v6_synthesis, ns_type::ns_t_ptr, ptr_name_v6_synthesis},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    // "ptr_addr_v6_nomapping" is not mapped in DNS server
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Synthesized PTR record doesn't exist on DNS server
    // Reverse IPv6 DNS64 query while DNS server doesn't have an answer for synthesized address.
    // After querying synthesized address failed, expect that prefix is removed from IPv6
    // synthesized address and do reverse IPv4 query instead.
    struct in6_addr v6addr;
    inet_pton(AF_INET6, "64:ff9b::1.2.3.4", &v6addr);
    struct hostent* result = gethostbyaddr(&v6addr, sizeof(v6addr), AF_INET6);
    ASSERT_TRUE(result != nullptr);
    EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v6_nomapping));  // PTR record not exist
    EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v4));            // PTR record exist
    std::string result_str = result->h_name ? result->h_name : "null";
    EXPECT_EQ(result_str, "v4only.example.com");
    // Check that return address has been mapped from IPv4 to IPv6 address because Netd
    // removes NAT64 prefix and does IPv4 DNS reverse lookup in this case. Then, Netd
    // fakes the return IPv4 address as original queried IPv6 address.
    result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");
    dns.clearQueries();

    // Synthesized PTR record exists on DNS server
    // Reverse IPv6 DNS64 query while DNS server has an answer for synthesized address.
    // Expect to Netd pass through synthesized address for DNS queries.
    inet_pton(AF_INET6, "64:ff9b::5.6.7.8", &v6addr);
    result = gethostbyaddr(&v6addr, sizeof(v6addr), AF_INET6);
    ASSERT_TRUE(result != nullptr);
    EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v6_synthesis));
    result_str = result->h_name ? result->h_name : "null";
    EXPECT_EQ(result_str, "v6synthesis.example.com");
}

TEST_F(ResolverTest, GetHostByAddr_ReverseDns64QueryFromHostFile) {
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "localhost";
    // The address is synthesized by prefix64:localhost.
    constexpr char host_addr[] = "64:ff9b::7f00:1";
    constexpr char listen_addr[] = "::1";

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"}});
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Using synthesized "localhost" address to be a trick for resolving host name
    // from host file /etc/hosts and "localhost" is the only name in /etc/hosts. Note that this is
    // not realistic: the code never synthesizes AAAA records for addresses in 127.0.0.0/8.
    struct in6_addr v6addr;
    inet_pton(AF_INET6, host_addr, &v6addr);
    struct hostent* result = gethostbyaddr(&v6addr, sizeof(v6addr), AF_INET6);
    ASSERT_TRUE(result != nullptr);
    // Expect no DNS queries; localhost is resolved via /etc/hosts.
    EXPECT_EQ(0U, GetNumQueries(dns, host_name));

    ASSERT_EQ(sizeof(in6_addr), (unsigned) result->h_length);
    ASSERT_EQ(AF_INET6, result->h_addrtype);
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, host_addr);
    result_str = result->h_name ? result->h_name : "null";
    EXPECT_EQ(result_str, host_name);
}

TEST_F(ResolverTest, GetNameInfo_ReverseDnsQueryWithHavingNat64Prefix) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char ptr_name[] = "v4v6.example.com.";
    // PTR record for IPv4 address 1.2.3.4
    constexpr char ptr_addr_v4[] = "4.3.2.1.in-addr.arpa.";
    // PTR record for IPv6 address 2001:db8::102:304
    constexpr char ptr_addr_v6[] =
            "4.0.3.0.2.0.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {ptr_addr_v4, ns_type::ns_t_ptr, ptr_name},
            {ptr_addr_v6, ns_type::ns_t_ptr, ptr_name},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    static const struct TestConfig {
        int flag;
        int family;
        std::string addr;
        std::string host;

        std::string asParameters() const {
            return StringPrintf("flag=%d, family=%d, addr=%s, host=%s", flag, family, addr.c_str(),
                                host.c_str());
        }
    } testConfigs[]{
        {NI_NAMEREQD,    AF_INET,  "1.2.3.4",           "v4v6.example.com"},
        {NI_NUMERICHOST, AF_INET,  "1.2.3.4",           "1.2.3.4"},
        {0,              AF_INET,  "1.2.3.4",           "v4v6.example.com"},
        {0,              AF_INET,  "5.6.7.8",           "5.6.7.8"},           // unmapped
        {NI_NAMEREQD,    AF_INET6, "2001:db8::102:304", "v4v6.example.com"},
        {NI_NUMERICHOST, AF_INET6, "2001:db8::102:304", "2001:db8::102:304"},
        {0,              AF_INET6, "2001:db8::102:304", "v4v6.example.com"},
        {0,              AF_INET6, "2001:db8::506:708", "2001:db8::506:708"}, // unmapped
    };

    // Reverse IPv4/IPv6 DNS query. Prefix should have no effect on it.
    for (const auto& config : testConfigs) {
        SCOPED_TRACE(config.asParameters());

        int rv;
        char host[NI_MAXHOST];
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        if (config.family == AF_INET) {
            memset(&sin, 0, sizeof(sin));
            sin.sin_family = AF_INET;
            inet_pton(AF_INET, config.addr.c_str(), &sin.sin_addr);
            rv = getnameinfo((const struct sockaddr*) &sin, sizeof(sin), host, sizeof(host),
                             nullptr, 0, config.flag);
            if (config.flag == NI_NAMEREQD) EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v4));
        } else if (config.family == AF_INET6) {
            memset(&sin6, 0, sizeof(sin6));
            sin6.sin6_family = AF_INET6;
            inet_pton(AF_INET6, config.addr.c_str(), &sin6.sin6_addr);
            rv = getnameinfo((const struct sockaddr*) &sin6, sizeof(sin6), host, sizeof(host),
                             nullptr, 0, config.flag);
            if (config.flag == NI_NAMEREQD) EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v6));
        }
        ASSERT_EQ(0, rv);
        std::string result_str = host;
        EXPECT_EQ(result_str, config.host);
        dns.clearQueries();
    }
}

TEST_F(ResolverTest, GetNameInfo_ReverseDns64Query) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char ptr_name[] = "v4only.example.com.";
    // PTR record for IPv4 address 1.2.3.4
    constexpr char ptr_addr_v4[] = "4.3.2.1.in-addr.arpa.";
    // PTR record for IPv6 address 64:ff9b::1.2.3.4
    constexpr char ptr_addr_v6_nomapping[] =
            "4.0.3.0.2.0.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.b.9.f.f.4.6.0.0.ip6.arpa.";
    constexpr char ptr_name_v6_synthesis[] = "v6synthesis.example.com.";
    // PTR record for IPv6 address 64:ff9b::5.6.7.8
    constexpr char ptr_addr_v6_synthesis[] =
            "8.0.7.0.6.0.5.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.b.9.f.f.4.6.0.0.ip6.arpa.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {ptr_addr_v4, ns_type::ns_t_ptr, ptr_name},
            {ptr_addr_v6_synthesis, ns_type::ns_t_ptr, ptr_name_v6_synthesis},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    static const struct TestConfig {
        bool hasSynthesizedPtrRecord;
        int flag;
        std::string addr;
        std::string host;

        std::string asParameters() const {
            return StringPrintf("hasSynthesizedPtrRecord=%d, flag=%d, addr=%s, host=%s",
                                hasSynthesizedPtrRecord, flag, addr.c_str(), host.c_str());
        }
    } testConfigs[]{
        {false, NI_NAMEREQD,    "64:ff9b::102:304", "v4only.example.com"},
        {false, NI_NUMERICHOST, "64:ff9b::102:304", "64:ff9b::102:304"},
        {false, 0,              "64:ff9b::102:304", "v4only.example.com"},
        {true,  NI_NAMEREQD,    "64:ff9b::506:708", "v6synthesis.example.com"},
        {true,  NI_NUMERICHOST, "64:ff9b::506:708", "64:ff9b::506:708"},
        {true,  0,              "64:ff9b::506:708", "v6synthesis.example.com"}
    };

    // hasSynthesizedPtrRecord = false
    //   Synthesized PTR record doesn't exist on DNS server
    //   Reverse IPv6 DNS64 query while DNS server doesn't have an answer for synthesized address.
    //   After querying synthesized address failed, expect that prefix is removed from IPv6
    //   synthesized address and do reverse IPv4 query instead.
    //
    // hasSynthesizedPtrRecord = true
    //   Synthesized PTR record exists on DNS server
    //   Reverse IPv6 DNS64 query while DNS server has an answer for synthesized address.
    //   Expect to just pass through synthesized address for DNS queries.
    for (const auto& config : testConfigs) {
        SCOPED_TRACE(config.asParameters());

        char host[NI_MAXHOST];
        struct sockaddr_in6 sin6;
        memset(&sin6, 0, sizeof(sin6));
        sin6.sin6_family = AF_INET6;
        inet_pton(AF_INET6, config.addr.c_str(), &sin6.sin6_addr);
        int rv = getnameinfo((const struct sockaddr*) &sin6, sizeof(sin6), host, sizeof(host),
                             nullptr, 0, config.flag);
        ASSERT_EQ(0, rv);
        if (config.flag == NI_NAMEREQD) {
            if (config.hasSynthesizedPtrRecord) {
                EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v6_synthesis));
            } else {
                EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v6_nomapping));  // PTR record not exist.
                EXPECT_LE(1U, GetNumQueries(dns, ptr_addr_v4));            // PTR record exist.
            }
        }
        std::string result_str = host;
        EXPECT_EQ(result_str, config.host);
        dns.clearQueries();
    }
}

TEST_F(ResolverTest, GetNameInfo_ReverseDns64QueryFromHostFile) {
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "localhost";
    // The address is synthesized by prefix64:localhost.
    constexpr char host_addr[] = "64:ff9b::7f00:1";
    constexpr char listen_addr[] = "::1";

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"}});
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Using synthesized "localhost" address to be a trick for resolving host name
    // from host file /etc/hosts and "localhost" is the only name in /etc/hosts. Note that this is
    // not realistic: the code never synthesizes AAAA records for addresses in 127.0.0.0/8.
    char host[NI_MAXHOST];
    struct sockaddr_in6 sin6 = {.sin6_family = AF_INET6};
    inet_pton(AF_INET6, host_addr, &sin6.sin6_addr);
    int rv = getnameinfo((const struct sockaddr*) &sin6, sizeof(sin6), host, sizeof(host), nullptr,
                         0, NI_NAMEREQD);
    ASSERT_EQ(0, rv);
    // Expect no DNS queries; localhost is resolved via /etc/hosts.
    EXPECT_EQ(0U, GetNumQueries(dns, host_name));

    std::string result_str = host;
    EXPECT_EQ(result_str, host_name);
}

TEST_F(ResolverTest, GetHostByName2_Dns64Synthesize) {
    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "ipv4only.example.com.";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Query an IPv4-only hostname. Expect that gets a synthesized address.
    struct hostent* result = gethostbyname2("ipv4only", AF_INET6);
    ASSERT_TRUE(result != nullptr);
    EXPECT_LE(1U, GetNumQueries(dns, host_name));
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "64:ff9b::102:304");
}

TEST_F(ResolverTest, GetHostByName2_DnsQueryWithHavingNat64Prefix) {
    constexpr char dns64_name[] = "ipv4only.arpa.";
    constexpr char host_name[] = "v4v6.example.com.";
    constexpr char listen_addr[] = "::1";
    const std::vector<DnsRecord> records = {
            {dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"},
            {host_name, ns_type::ns_t_a, "1.2.3.4"},
            {host_name, ns_type::ns_t_aaaa, "2001:db8::1.2.3.4"},
    };

    test::DNSResponder dns(listen_addr);
    StartDns(dns, records);
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // IPv4 DNS query. Prefix should have no effect on it.
    struct hostent* result = gethostbyname2("v4v6", AF_INET);
    ASSERT_TRUE(result != nullptr);
    EXPECT_LE(1U, GetNumQueries(dns, host_name));
    std::string result_str = ToString(result);
    EXPECT_EQ(result_str, "1.2.3.4");
    dns.clearQueries();

    // IPv6 DNS query. Prefix should have no effect on it.
    result = gethostbyname2("v4v6", AF_INET6);
    ASSERT_TRUE(result != nullptr);
    EXPECT_LE(1U, GetNumQueries(dns, host_name));
    result_str = ToString(result);
    EXPECT_EQ(result_str, "2001:db8::102:304");
}

TEST_F(ResolverTest, GetHostByName2_Dns64QuerySpecialUseIPv4Addresses) {
    constexpr char THIS_NETWORK[] = "this_network";
    constexpr char LOOPBACK[] = "loopback";
    constexpr char LINK_LOCAL[] = "link_local";
    constexpr char MULTICAST[] = "multicast";
    constexpr char LIMITED_BROADCAST[] = "limited_broadcast";

    constexpr char ADDR_THIS_NETWORK[] = "0.0.0.1";
    constexpr char ADDR_LOOPBACK[] = "127.0.0.1";
    constexpr char ADDR_LINK_LOCAL[] = "169.254.0.1";
    constexpr char ADDR_MULTICAST[] = "224.0.0.1";
    constexpr char ADDR_LIMITED_BROADCAST[] = "255.255.255.255";

    constexpr char listen_addr[] = "::1";
    constexpr char dns64_name[] = "ipv4only.arpa.";

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::"}});
    const std::vector<std::string> servers = {listen_addr};
    ASSERT_TRUE(mDnsClient.SetResolversForNetwork(servers));

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    static const struct TestConfig {
        std::string name;
        std::string addr;

        std::string asHostName() const {
            return StringPrintf("%s.example.com.",
                                name.c_str());
        }
    } testConfigs[]{
        {THIS_NETWORK,      ADDR_THIS_NETWORK},
        {LOOPBACK,          ADDR_LOOPBACK},
        {LINK_LOCAL,        ADDR_LINK_LOCAL},
        {MULTICAST,         ADDR_MULTICAST},
        {LIMITED_BROADCAST, ADDR_LIMITED_BROADCAST}
    };

    for (const auto& config : testConfigs) {
        const std::string testHostName = config.asHostName();
        SCOPED_TRACE(testHostName);

        const char* host_name = testHostName.c_str();
        dns.addMapping(host_name, ns_type::ns_t_a, config.addr.c_str());

        struct hostent* result = gethostbyname2(config.name.c_str(), AF_INET6);
        EXPECT_LE(1U, GetNumQueries(dns, host_name));

        // In AF_INET6 case, don't synthesize special use IPv4 address.
        // Expect to have no answer
        EXPECT_EQ(nullptr, result);

        dns.clearQueries();
    }
}

TEST_F(ResolverTest, PrefixDiscoveryBypassTls) {
    constexpr char listen_addr[] = "::1";
    constexpr char cleartext_port[] = "53";
    constexpr char tls_port[] = "853";
    constexpr char dns64_name[] = "ipv4only.arpa.";
    const std::vector<std::string> servers = {listen_addr};

    test::DNSResponder dns(listen_addr);
    StartDns(dns, {{dns64_name, ns_type::ns_t_aaaa, "64:ff9b::192.0.0.170"}});
    test::DnsTlsFrontend tls(listen_addr, tls_port, listen_addr, cleartext_port);
    ASSERT_TRUE(tls.startServer());

    // Setup OPPORTUNISTIC mode and wait for the validation complete.
    ASSERT_TRUE(
            mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "", {}));
    EXPECT_TRUE(tls.waitForQueries(1, 5000));
    tls.clearQueries();

    // Start NAT64 prefix discovery and wait for it complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Verify it bypassed TLS even though there's a TLS server available.
    EXPECT_EQ(0, tls.queries());
    EXPECT_EQ(1U, GetNumQueries(dns, dns64_name));

    // Restart the testing network to reset the cache.
    mDnsClient.TearDown();
    mDnsClient.SetUp();
    dns.clearQueries();

    // Setup STRICT mode and wait for the validation complete.
    ASSERT_TRUE(mDnsClient.SetResolversWithTls(servers, kDefaultSearchDomains, kDefaultParams, "",
                                               {base64Encode(tls.fingerprint())}));
    EXPECT_TRUE(tls.waitForQueries(1, 5000));
    tls.clearQueries();

    // Start NAT64 prefix discovery and wait for it to complete.
    EXPECT_TRUE(mDnsClient.resolvService()->startPrefix64Discovery(TEST_NETID).isOk());
    EXPECT_TRUE(WaitForPrefix64Detected(TEST_NETID, 1000));

    // Verify it bypassed TLS despite STRICT mode.
    EXPECT_EQ(0, tls.queries());
    EXPECT_EQ(1U, GetNumQueries(dns, dns64_name));
}

namespace {

class ScopedSetNetworkForProcess {
  public:
    explicit ScopedSetNetworkForProcess(unsigned netId) {
        mStoredNetId = getNetworkForProcess();
        if (netId == mStoredNetId) return;
        EXPECT_EQ(0, setNetworkForProcess(netId));
    }
    ~ScopedSetNetworkForProcess() { EXPECT_EQ(0, setNetworkForProcess(mStoredNetId)); }

  private:
    unsigned mStoredNetId;
};

class ScopedSetNetworkForResolv {
  public:
    explicit ScopedSetNetworkForResolv(unsigned netId) { EXPECT_EQ(0, setNetworkForResolv(netId)); }
    ~ScopedSetNetworkForResolv() { EXPECT_EQ(0, setNetworkForResolv(NETID_UNSET)); }
};

void sendCommand(int fd, const std::string& cmd) {
    ssize_t rc = TEMP_FAILURE_RETRY(write(fd, cmd.c_str(), cmd.size() + 1));
    EXPECT_EQ(rc, static_cast<ssize_t>(cmd.size() + 1));
}

int32_t readBE32(int fd) {
    int32_t tmp;
    int n = TEMP_FAILURE_RETRY(read(fd, &tmp, sizeof(tmp)));
    EXPECT_TRUE(n > 0);
    return ntohl(tmp);
}

int readResponseCode(int fd) {
    char buf[4];
    int n = TEMP_FAILURE_RETRY(read(fd, &buf, sizeof(buf)));
    EXPECT_TRUE(n > 0);
    // The format of response code is that 4 bytes for the code & null.
    buf[3] = '\0';
    int result;
    EXPECT_TRUE(ParseInt(buf, &result));
    return result;
}

bool checkAndClearUseLocalNameserversFlag(unsigned* netid) {
    if (netid == nullptr || ((*netid) & NETID_USE_LOCAL_NAMESERVERS) == 0) {
        return false;
    }
    *netid = (*netid) & ~NETID_USE_LOCAL_NAMESERVERS;
    return true;
}

android::net::UidRangeParcel makeUidRangeParcel(int start, int stop) {
    android::net::UidRangeParcel res;
    res.start = start;
    res.stop = stop;

    return res;
}

void expectNetIdWithLocalNameserversFlag(unsigned netId) {
    unsigned dnsNetId = 0;
    EXPECT_EQ(0, getNetworkForDns(&dnsNetId));
    EXPECT_TRUE(checkAndClearUseLocalNameserversFlag(&dnsNetId));
    EXPECT_EQ(netId, static_cast<unsigned>(dnsNetId));
}

void expectDnsNetIdEquals(unsigned netId) {
    unsigned dnsNetId = 0;
    EXPECT_EQ(0, getNetworkForDns(&dnsNetId));
    EXPECT_EQ(netId, static_cast<unsigned>(dnsNetId));
}

void expectDnsNetIdIsDefaultNetwork(android::net::INetd* netdService) {
    int currentNetid;
    EXPECT_TRUE(netdService->networkGetDefault(&currentNetid).isOk());
    expectDnsNetIdEquals(currentNetid);
}

void expectDnsNetIdWithVpn(android::net::INetd* netdService, unsigned vpnNetId,
                           unsigned expectedNetId) {
    EXPECT_TRUE(netdService->networkCreateVpn(vpnNetId, false /* secure */).isOk());
    uid_t uid = getuid();
    // Add uid to VPN
    EXPECT_TRUE(netdService->networkAddUidRanges(vpnNetId, {makeUidRangeParcel(uid, uid)}).isOk());
    expectDnsNetIdEquals(expectedNetId);
    EXPECT_TRUE(netdService->networkDestroy(vpnNetId).isOk());
}

}  // namespace

TEST_F(ResolverTest, getDnsNetId) {
    // We've called setNetworkForProcess in SetupOemNetwork, so reset to default first.
    setNetworkForProcess(NETID_UNSET);

    expectDnsNetIdIsDefaultNetwork(mDnsClient.netdService());
    expectDnsNetIdWithVpn(mDnsClient.netdService(), TEST_VPN_NETID, TEST_VPN_NETID);

    // Test with setNetworkForProcess
    {
        ScopedSetNetworkForProcess scopedSetNetworkForProcess(TEST_NETID);
        expectDnsNetIdEquals(TEST_NETID);
    }

    // Test with setNetworkForProcess with NETID_USE_LOCAL_NAMESERVERS
    {
        ScopedSetNetworkForProcess scopedSetNetworkForProcess(TEST_NETID |
                                                              NETID_USE_LOCAL_NAMESERVERS);
        expectNetIdWithLocalNameserversFlag(TEST_NETID);
    }

    // Test with setNetworkForResolv
    {
        ScopedSetNetworkForResolv scopedSetNetworkForResolv(TEST_NETID);
        expectDnsNetIdEquals(TEST_NETID);
    }

    // Test with setNetworkForResolv with NETID_USE_LOCAL_NAMESERVERS
    {
        ScopedSetNetworkForResolv scopedSetNetworkForResolv(TEST_NETID |
                                                            NETID_USE_LOCAL_NAMESERVERS);
        expectNetIdWithLocalNameserversFlag(TEST_NETID);
    }

    // Test with setNetworkForResolv under bypassable vpn
    {
        ScopedSetNetworkForResolv scopedSetNetworkForResolv(TEST_NETID);
        expectDnsNetIdWithVpn(mDnsClient.netdService(), TEST_VPN_NETID, TEST_NETID);
    }

    // Create socket connected to DnsProxyListener
    int fd = dns_open_proxy();
    EXPECT_TRUE(fd > 0);
    unique_fd ufd(fd);

    // Test command with wrong netId
    sendCommand(fd, "getdnsnetid abc");
    EXPECT_EQ(ResponseCode::DnsProxyQueryResult, readResponseCode(fd));
    EXPECT_EQ(-EINVAL, readBE32(fd));

    // Test unsupported command
    sendCommand(fd, "getdnsnetidNotSupported");
    // Keep in sync with FrameworkListener.cpp (500, "Command not recognized")
    EXPECT_EQ(500, readResponseCode(fd));
}
