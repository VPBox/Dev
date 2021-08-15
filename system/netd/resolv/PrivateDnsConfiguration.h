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

#ifndef NETD_RESOLV_PRIVATEDNSCONFIGURATION_H
#define NETD_RESOLV_PRIVATEDNSCONFIGURATION_H

#include <list>
#include <map>
#include <mutex>
#include <vector>

#include <android-base/thread_annotations.h>

#include "DnsTlsServer.h"

namespace android {
namespace net {

// The DNS over TLS mode on a specific netId.
enum class PrivateDnsMode : uint8_t { OFF, OPPORTUNISTIC, STRICT };

// Validation status of a DNS over TLS server (on a specific netId).
enum class Validation : uint8_t { in_process, success, fail, unknown_server, unknown_netid };

struct PrivateDnsStatus {
    PrivateDnsMode mode;
    std::list<DnsTlsServer> validatedServers;
};

// TODO: remove this C-style struct and use PrivateDnsStatus everywhere.
struct ExternalPrivateDnsStatus {
    PrivateDnsMode mode;
    int numServers;
    struct PrivateDnsInfo {
        sockaddr_storage ss;
        const char* hostname;
        Validation validation;
    } serverStatus[MAXNS];
};

class PrivateDnsConfiguration {
  public:
    int set(int32_t netId, uint32_t mark, const std::vector<std::string>& servers,
            const std::string& name, const std::set<std::vector<uint8_t>>& fingerprints);

    PrivateDnsStatus getStatus(unsigned netId);

    // DEPRECATED, use getStatus() above.
    void getStatus(unsigned netId, ExternalPrivateDnsStatus* status);

    void clear(unsigned netId);

  private:
    typedef std::map<DnsTlsServer, Validation, AddressComparator> PrivateDnsTracker;

    void validatePrivateDnsProvider(const DnsTlsServer& server, PrivateDnsTracker& tracker,
                                    unsigned netId, uint32_t mark) REQUIRES(mPrivateDnsLock);

    bool recordPrivateDnsValidation(const DnsTlsServer& server, unsigned netId, bool success);

    // Start validation for newly added servers as well as any servers that have
    // landed in Validation::fail state. Note that servers that have failed
    // multiple validation attempts but for which there is still a validating
    // thread running are marked as being Validation::in_process.
    bool needsValidation(const PrivateDnsTracker& tracker, const DnsTlsServer& server);

    std::mutex mPrivateDnsLock;
    std::map<unsigned, PrivateDnsMode> mPrivateDnsModes GUARDED_BY(mPrivateDnsLock);
    // Structure for tracking the validation status of servers on a specific netId.
    // Using the AddressComparator ensures at most one entry per IP address.
    std::map<unsigned, PrivateDnsTracker> mPrivateDnsTransports GUARDED_BY(mPrivateDnsLock);
};

extern PrivateDnsConfiguration gPrivateDnsConfiguration;

}  // namespace net
}  // namespace android

#endif /* NETD_RESOLV_PRIVATEDNSCONFIGURATION_H */
