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

#include "DnsTlsServer.h"

#include <algorithm>

namespace {

// Returns a tuple of references to the elements of a.
auto make_tie(const sockaddr_in& a) {
    return std::tie(a.sin_port, a.sin_addr.s_addr);
}

// Returns a tuple of references to the elements of a.
auto make_tie(const sockaddr_in6& a) {
    // Skip flowinfo, which is not relevant.
    return std::tie(
        a.sin6_port,
        a.sin6_addr,
        a.sin6_scope_id
    );
}

} // namespace

// These binary operators make sockaddr_storage comparable.  They need to be
// in the global namespace so that the std::tuple < and == operators can see them.
static bool operator <(const in6_addr& x, const in6_addr& y) {
    return std::lexicographical_compare(
            std::begin(x.s6_addr), std::end(x.s6_addr),
            std::begin(y.s6_addr), std::end(y.s6_addr));
}

static bool operator ==(const in6_addr& x, const in6_addr& y) {
    return std::equal(
            std::begin(x.s6_addr), std::end(x.s6_addr),
            std::begin(y.s6_addr), std::end(y.s6_addr));
}

static bool operator <(const sockaddr_storage& x, const sockaddr_storage& y) {
    if (x.ss_family != y.ss_family) {
        return x.ss_family < y.ss_family;
    }
    // Same address family.
    if (x.ss_family == AF_INET) {
        const sockaddr_in& x_sin = reinterpret_cast<const sockaddr_in&>(x);
        const sockaddr_in& y_sin = reinterpret_cast<const sockaddr_in&>(y);
        return make_tie(x_sin) < make_tie(y_sin);
    } else if (x.ss_family == AF_INET6) {
        const sockaddr_in6& x_sin6 = reinterpret_cast<const sockaddr_in6&>(x);
        const sockaddr_in6& y_sin6 = reinterpret_cast<const sockaddr_in6&>(y);
        return make_tie(x_sin6) < make_tie(y_sin6);
    }
    return false;  // Unknown address type.  This is an error.
}

static bool operator ==(const sockaddr_storage& x, const sockaddr_storage& y) {
    if (x.ss_family != y.ss_family) {
        return false;
    }
    // Same address family.
    if (x.ss_family == AF_INET) {
        const sockaddr_in& x_sin = reinterpret_cast<const sockaddr_in&>(x);
        const sockaddr_in& y_sin = reinterpret_cast<const sockaddr_in&>(y);
        return make_tie(x_sin) == make_tie(y_sin);
    } else if (x.ss_family == AF_INET6) {
        const sockaddr_in6& x_sin6 = reinterpret_cast<const sockaddr_in6&>(x);
        const sockaddr_in6& y_sin6 = reinterpret_cast<const sockaddr_in6&>(y);
        return make_tie(x_sin6) == make_tie(y_sin6);
    }
    return false;  // Unknown address type.  This is an error.
}

namespace android {
namespace net {

// This comparison ignores ports and fingerprints.
bool AddressComparator::operator() (const DnsTlsServer& x, const DnsTlsServer& y) const {
    if (x.ss.ss_family != y.ss.ss_family) {
        return x.ss.ss_family < y.ss.ss_family;
    }
    // Same address family.
    if (x.ss.ss_family == AF_INET) {
        const sockaddr_in& x_sin = reinterpret_cast<const sockaddr_in&>(x.ss);
        const sockaddr_in& y_sin = reinterpret_cast<const sockaddr_in&>(y.ss);
        return x_sin.sin_addr.s_addr < y_sin.sin_addr.s_addr;
    } else if (x.ss.ss_family == AF_INET6) {
        const sockaddr_in6& x_sin6 = reinterpret_cast<const sockaddr_in6&>(x.ss);
        const sockaddr_in6& y_sin6 = reinterpret_cast<const sockaddr_in6&>(y.ss);
        return std::tie(x_sin6.sin6_addr, x_sin6.sin6_scope_id) <
                std::tie(y_sin6.sin6_addr, y_sin6.sin6_scope_id);
    }
    return false;  // Unknown address type.  This is an error.
}

// Returns a tuple of references to the elements of s.
auto make_tie(const DnsTlsServer& s) {
    return std::tie(
        s.ss,
        s.name,
        s.fingerprints,
        s.protocol
    );
}

bool DnsTlsServer::operator <(const DnsTlsServer& other) const {
    return make_tie(*this) < make_tie(other);
}

bool DnsTlsServer::operator ==(const DnsTlsServer& other) const {
    return make_tie(*this) == make_tie(other);
}

bool DnsTlsServer::wasExplicitlyConfigured() const {
    return !name.empty() || !fingerprints.empty();
}

}  // namespace net
}  // namespace android
