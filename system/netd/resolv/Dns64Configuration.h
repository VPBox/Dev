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

#ifndef DNS_DNS64CONFIGURATION_H_
#define DNS_DNS64CONFIGURATION_H_

#include <netinet/in.h>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <unordered_map>

#include <android-base/thread_annotations.h>
#include "netdutils/DumpWriter.h"
#include "netdutils/InternetAddresses.h"

struct android_net_context;

namespace android {
namespace net {

/**
 * This class handles RFC 7050 -style DNS64 prefix discovery.
 *
 * The ResolverController starts DNS64 prefix discovery when it observes a
 * a network with only IPv6 nameservers. (It stops discovery whenever an IPv4
 * nameserver is added or the network is deleted.)
 *
 * Each time prefix discovery is started, a new discoveryId is generated so
 * that running resolution threads can notice they are no longer the most
 * recent resolution attempt. This results in the backoff schedule of resolution
 * being reset.
 *
 * Thread-safety: All public methods in this class MUST be thread-safe.
 * (In other words: this class handles all its locking privately.)
 */
class Dns64Configuration {
  public:
    // Simple data struct for passing back packet NAT64 prefix event information to the
    // Dns64PrefixCallback callback.
    struct Nat64PrefixInfo {
        unsigned netId;
        bool added;
        std::string prefixString;
        uint8_t prefixLength;
    };

    // Callback that is triggered for every NAT64 prefix event.
    using Nat64PrefixCallback = std::function<void(const Nat64PrefixInfo&)>;

    using GetNetworkContextCallback = std::function<void(uint32_t, uint32_t, android_net_context*)>;

    // Parameters from RFC 7050 section 8.
    static const char kIPv4OnlyHost[];  // "ipv4only.arpa."
    static const char kIPv4Literal1[];  // 192.0.0.170
    static const char kIPv4Literal2[];  // 192.0.0.171

    Dns64Configuration() = delete;
    Dns64Configuration(GetNetworkContextCallback getNetworkCallback,
                       Nat64PrefixCallback prefixCallback)
        : mGetNetworkContextCallback(std::move(getNetworkCallback)),
          mPrefixCallback(std::move(prefixCallback)) {}
    Dns64Configuration(const Dns64Configuration&) = delete;
    Dns64Configuration(Dns64Configuration&&) = delete;
    Dns64Configuration& operator=(const Dns64Configuration&) = delete;
    Dns64Configuration& operator=(Dns64Configuration&&) = delete;

    void startPrefixDiscovery(unsigned netId);
    void stopPrefixDiscovery(unsigned netId);
    netdutils::IPPrefix getPrefix64(unsigned netId) const;

    void dump(netdutils::DumpWriter& dw, unsigned netId);

  private:
    struct Dns64Config {
        Dns64Config(unsigned pseudoRandomId, unsigned network)
            : discoveryId(pseudoRandomId), netId(network) {}

        const unsigned int discoveryId;
        const unsigned int netId;
        netdutils::IPPrefix prefix64{};
    };

    enum { PREFIX_REMOVED, PREFIX_ADDED };

    static bool doRfc7050PrefixDiscovery(const android_net_context& netcontext, Dns64Config* cfg);

    unsigned getNextId() REQUIRES(mMutex) { return mNextId++; }
    netdutils::IPPrefix getPrefix64Locked(unsigned netId) const REQUIRES(mMutex);
    bool isDiscoveryInProgress(const Dns64Config& cfg) const REQUIRES(mMutex);
    bool reportNat64PrefixStatus(unsigned netId, bool added, const netdutils::IPPrefix& pfx)
            REQUIRES(mMutex);

    bool shouldContinueDiscovery(const Dns64Config& cfg);
    void recordDns64Config(const Dns64Config& cfg);
    void removeDns64Config(unsigned netId) REQUIRES(mMutex);

    mutable std::mutex mMutex;
    std::condition_variable mCv;
    unsigned int mNextId GUARDED_BY(mMutex);
    std::unordered_map<unsigned, Dns64Config> mDns64Configs GUARDED_BY(mMutex);
    const GetNetworkContextCallback mGetNetworkContextCallback;
    const Nat64PrefixCallback mPrefixCallback;
};

}  // namespace net
}  // namespace android

#endif  // DNS_DNS64CONFIGURATION_H_
