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

#ifndef _DNS_DNSTLSSERVER_H
#define _DNS_DNSTLSSERVER_H

#include <set>
#include <string>
#include <vector>

#include <netinet/in.h>

#include <netd_resolv/params.h>

namespace android {
namespace net {

// DnsTlsServer represents a recursive resolver that supports, or may support, a
// secure protocol.
struct DnsTlsServer {
    // Default constructor.
    DnsTlsServer() {}

    // Allow sockaddr_storage to be promoted to DnsTlsServer automatically.
    DnsTlsServer(const sockaddr_storage& ss) : ss(ss) {}

    enum class Response : uint8_t { success, network_error, limit_error, internal_error };

    struct Result {
        Response code;
        std::vector<uint8_t> response;
    };

    // The server location, including IP and port.
    sockaddr_storage ss = {};

    // A set of SHA256 public key fingerprints.  If this set is nonempty, the server
    // must present a self-consistent certificate chain that contains a certificate
    // whose public key matches one of these fingerprints.  Otherwise, the client will
    // terminate the connection.
    std::set<std::vector<uint8_t>> fingerprints;

    // The server's hostname.  If this string is nonempty, the server must present a
    // certificate that indicates this name and has a valid chain to a trusted root CA.
    std::string name;

    // Placeholder.  More protocols might be defined in the future.
    int protocol = IPPROTO_TCP;

    // Exact comparison of DnsTlsServer objects
    bool operator<(const DnsTlsServer& other) const;
    bool operator==(const DnsTlsServer& other) const;

    bool wasExplicitlyConfigured() const;
};

// This comparison only checks the IP address.  It ignores ports, names, and fingerprints.
struct AddressComparator {
    bool operator()(const DnsTlsServer& x, const DnsTlsServer& y) const;
};

}  // namespace net
}  // namespace android

#endif  // _DNS_DNSTLSSERVER_H
