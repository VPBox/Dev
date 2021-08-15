/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef DNS_RESPONDER_CLIENT_H
#define DNS_RESPONDER_CLIENT_H

#include <memory>
#include <string>
#include <vector>

#include <cutils/sockets.h>

#include <private/android_filesystem_config.h>
#include <utils/StrongPointer.h>

#include "NetdClient.h"
#include "android/net/IDnsResolver.h"
#include "android/net/INetd.h"
#include "binder/IServiceManager.h"
#include "dns_responder.h"

#define SKIP_IF_RESOLVER_VERSION_NEWER_THAN(service, version) \
    do {                                                      \
        if ((service)->getInterfaceVersion() > (version)) {   \
            return;                                           \
        }                                                     \
    } while (0)

inline const std::vector<std::string> kDefaultServers = {"127.0.0.3"};
inline const std::vector<std::string> kDefaultSearchDomains = {"example.com"};
inline const std::vector<int> kDefaultParams = {
        300,      // sample validity in seconds
        25,       // success threshod in percent
        8,    8,  // {MIN,MAX}_SAMPLES
        1000,     // BASE_TIMEOUT_MSEC
        2,        // retry count
};

class DnsResponderClient {
public:
    struct Mapping {
        std::string host;
        std::string entry;
        std::string ip4;
        std::string ip6;
    };

    virtual ~DnsResponderClient() = default;

    static void SetupMappings(unsigned num_hosts, const std::vector<std::string>& domains,
            std::vector<Mapping>* mappings);

    bool SetResolversForNetwork(const std::vector<std::string>& servers = kDefaultServers,
                                const std::vector<std::string>& domains = kDefaultSearchDomains,
                                const std::vector<int>& params = kDefaultParams);

    bool SetResolversForNetwork(const std::vector<std::string>& servers,
            const std::vector<std::string>& searchDomains,
            const std::string& params);

    bool SetResolversWithTls(const std::vector<std::string>& servers,
            const std::vector<std::string>& searchDomains,
            const std::vector<int>& params,
            const std::string& name,
            const std::vector<std::string>& fingerprints) {
        // Pass servers as both network-assigned and TLS servers.  Tests can
        // determine on which server and by which protocol queries arrived.
        return SetResolversWithTls(servers, searchDomains, params,
                                   servers, name, fingerprints);
    }

    bool SetResolversWithTls(const std::vector<std::string>& servers,
            const std::vector<std::string>& searchDomains,
            const std::vector<int>& params,
            const std::vector<std::string>& tlsServers,
            const std::string& name,
            const std::vector<std::string>& fingerprints);

    static void SetupDNSServers(unsigned num_servers, const std::vector<Mapping>& mappings,
            std::vector<std::unique_ptr<test::DNSResponder>>* dns,
            std::vector<std::string>* servers);

    int SetupOemNetwork();

    void TearDownOemNetwork(int oemNetId);

    virtual void SetUp();
    virtual void TearDown();

    android::net::IDnsResolver* resolvService() const { return mDnsResolvSrv.get(); }
    android::net::INetd* netdService() const { return mNetdSrv.get(); }

  private:
    android::sp<android::net::INetd> mNetdSrv = nullptr;
    android::sp<android::net::IDnsResolver> mDnsResolvSrv = nullptr;
    int mOemNetId = -1;
};

#endif  // DNS_RESPONDER_CLIENT_H
