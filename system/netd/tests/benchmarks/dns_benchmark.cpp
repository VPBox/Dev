/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "dns_benchmark"

/*
 * See README.md for general notes.
 *
 * This set of benchmarks measures the throughput of getaddrinfo() on between 1 and 32 threads for
 * the purpose of keeping track of the maximum load that netd can reasonably handle.
 *
 * Useful measurements
 * ===================
 *
 *  - real_time: the average time taken to make a single getaddrinfo lookup on a local DNS resolver
 *               run by DnsFixture. This will usually be higher on multithreaded tests as threads
 *               block on DNS lookups and Binder connections.
 *
 *  - iterations: total number of runs finished within the time limit. Higher is better. This is
 *                roughly proportional to MinTime * nThreads / real_time.
 *
 */

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <android-base/stringprintf.h>
#include <benchmark/benchmark.h>

#include "NetdClient.h"
#include "dns_responder_client.h"
#include "netd_resolv/params.h"  // MAXNS

using android::base::StringPrintf;

constexpr int MIN_THREADS = 1;
constexpr int MAX_THREADS = 32;

class DnsFixture : public ::benchmark::Fixture {
protected:
    static constexpr unsigned num_hosts = 1000;
    DnsResponderClient dns;
    std::vector<DnsResponderClient::Mapping> mappings;
    std::vector<std::unique_ptr<test::DNSResponder>> mDns;

public:
    void SetUp(const ::benchmark::State& state) override {
        if (state.thread_index == 0) {
            dns.SetUp();

            std::vector<std::string> domains = { "example.com" };
            std::vector<std::string> servers;
            dns.SetupMappings(num_hosts, domains, &mappings);

            dns.SetupDNSServers(MAXNS, mappings, &mDns, &servers);

            const std::vector<int> mDefaultParams_Binder = {300, 25, 8, 8, 1000};
            dns.SetResolversForNetwork(servers, domains, mDefaultParams_Binder);
        }
    }

    void TearDown(const ::benchmark::State& state) override {
        if (state.thread_index == 0) {
            dns.TearDown();
        }
    }

    std::vector<DnsResponderClient::Mapping> const& getMappings() const {
        return mappings;
    }

    void benchmark(benchmark::State& state) {
        while (state.KeepRunning()) {
            const uint32_t ofs = arc4random_uniform(getMappings().size());
            const auto& mapping = getMappings()[ofs];
            addrinfo* result = nullptr;
            if (getaddrinfo(mapping.host.c_str(), nullptr, nullptr, &result)) {
                state.SkipWithError(StringPrintf("getaddrinfo failed with errno=%d",
                        errno).c_str());
                break;
            }
            if (result) {
                freeaddrinfo(result);
                result = nullptr;
            }
        }
    }
};

BENCHMARK_DEFINE_F(DnsFixture, getaddrinfo)(benchmark::State& state) {
    benchmark(state);
}
BENCHMARK_REGISTER_F(DnsFixture, getaddrinfo)
    ->ThreadRange(MIN_THREADS, MAX_THREADS)
    ->UseRealTime();
