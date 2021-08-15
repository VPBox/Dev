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

#ifndef NETDUTILS_INTERNETADDRESSES_H_
#define NETDUTILS_INTERNETADDRESSES_H_

#include <netinet/in.h>
#include <stdint.h>
#include <cstring>
#include <limits>
#include <string>

#include "netdutils/NetworkConstants.h"

namespace android {
namespace netdutils {

namespace internal_ {

// A structure to hold data for dealing with Internet addresses (IPAddress) and
// related types such as IPSockAddr and IPPrefix.
struct compact_ipdata {
    uint8_t family{AF_UNSPEC};
    uint8_t cidrlen{0U};  // written and read in host-byte order
    in_port_t port{0U};   // written and read in host-byte order
    uint32_t scope_id{0U};
    union {
        in_addr v4;
        in6_addr v6;
    } ip{.v6 = IN6ADDR_ANY_INIT};  // written and read in network-byte order

    // Classes that use compact_ipdata and this method should be sure to clear
    // (i.e. zero or make uniform) any fields not relevant to the class.
    friend bool operator==(const compact_ipdata& a, const compact_ipdata& b) {
        if ((a.family != b.family) || (a.cidrlen != b.cidrlen) || (a.port != b.port) ||
            (a.scope_id != b.scope_id)) {
            return false;
        }
        switch (a.family) {
            case AF_UNSPEC:
                // After the above checks, two AF_UNSPEC objects can be
                // considered equal, for convenience.
                return true;
            case AF_INET: {
                const in_addr v4a = a.ip.v4;
                const in_addr v4b = b.ip.v4;
                return (v4a.s_addr == v4b.s_addr);
            }
            case AF_INET6: {
                const in6_addr v6a = a.ip.v6;
                const in6_addr v6b = b.ip.v6;
                return IN6_ARE_ADDR_EQUAL(&v6a, &v6b);
            }
        }
        return false;
    }

    // Classes that use compact_ipdata and this method should be sure to clear
    // (i.e. zero or make uniform) any fields not relevant to the class.
    friend bool operator!=(const compact_ipdata& a, const compact_ipdata& b) { return !(a == b); }

    // Classes that use compact_ipdata and this method should be sure to clear
    // (i.e. zero or make uniform) any fields not relevant to the class.
    friend bool operator<(const compact_ipdata& a, const compact_ipdata& b) {
        if (a.family != b.family) return (a.family < b.family);
        switch (a.family) {
            case AF_INET: {
                const in_addr v4a = a.ip.v4;
                const in_addr v4b = b.ip.v4;
                if (v4a.s_addr != v4b.s_addr) return (ntohl(v4a.s_addr) < ntohl(v4b.s_addr));
                break;
            }
            case AF_INET6: {
                const in6_addr v6a = a.ip.v6;
                const in6_addr v6b = b.ip.v6;
                const int cmp = std::memcmp(v6a.s6_addr, v6b.s6_addr, IPV6_ADDR_LEN);
                if (cmp != 0) return cmp < 0;
                break;
            }
        }
        if (a.cidrlen != b.cidrlen) return (a.cidrlen < b.cidrlen);
        if (a.port != b.port) return (a.port < b.port);
        return (a.scope_id < b.scope_id);
    }
};

static_assert(AF_UNSPEC <= std::numeric_limits<uint8_t>::max(), "AF_UNSPEC value too large");
static_assert(AF_INET <= std::numeric_limits<uint8_t>::max(), "AF_INET value too large");
static_assert(AF_INET6 <= std::numeric_limits<uint8_t>::max(), "AF_INET6 value too large");
static_assert(sizeof(compact_ipdata) == 24U, "compact_ipdata unexpectedly large");

}  // namespace internal_

inline bool usesScopedIds(const in6_addr& ipv6) {
    return (IN6_IS_ADDR_LINKLOCAL(&ipv6) || IN6_IS_ADDR_MC_LINKLOCAL(&ipv6));
}

class IPPrefix;
class IPSockAddr;

class IPAddress {
  public:
    static bool forString(const std::string& repr, IPAddress* ip);
    static IPAddress forString(const std::string& repr) {
        IPAddress ip;
        if (!forString(repr, &ip)) return IPAddress();
        return ip;
    }

    IPAddress() = default;
    IPAddress(const IPAddress&) = default;
    IPAddress(IPAddress&&) = default;

    explicit IPAddress(const in_addr& ipv4)
        : mData({AF_INET, IPV4_ADDR_BITS, 0U, 0U, {.v4 = ipv4}}) {}
    explicit IPAddress(const in6_addr& ipv6)
        : mData({AF_INET6, IPV6_ADDR_BITS, 0U, 0U, {.v6 = ipv6}}) {}
    IPAddress(const in6_addr& ipv6, uint32_t scope_id)
        : mData({AF_INET6,
                 IPV6_ADDR_BITS,
                 0U,
                 // Sanity check: scoped_ids only for link-local addresses.
                 usesScopedIds(ipv6) ? scope_id : 0U,
                 {.v6 = ipv6}}) {}
    IPAddress(const IPAddress& ip, uint32_t scope_id) : IPAddress(ip) {
        mData.scope_id = (family() == AF_INET6 && usesScopedIds(mData.ip.v6)) ? scope_id : 0U;
    }

    IPAddress& operator=(const IPAddress&) = default;
    IPAddress& operator=(IPAddress&&) = default;

    constexpr sa_family_t family() const noexcept { return mData.family; }
    constexpr uint32_t scope_id() const noexcept { return mData.scope_id; }

    std::string toString() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const IPAddress& ip) {
        os << ip.toString();
        return os;
    }
    friend bool operator==(const IPAddress& a, const IPAddress& b) { return (a.mData == b.mData); }
    friend bool operator!=(const IPAddress& a, const IPAddress& b) { return (a.mData != b.mData); }
    friend bool operator<(const IPAddress& a, const IPAddress& b) { return (a.mData < b.mData); }
    friend bool operator>(const IPAddress& a, const IPAddress& b) { return (b.mData < a.mData); }
    friend bool operator<=(const IPAddress& a, const IPAddress& b) { return (a < b) || (a == b); }
    friend bool operator>=(const IPAddress& a, const IPAddress& b) { return (b < a) || (a == b); }

  private:
    friend class IPPrefix;
    friend class IPSockAddr;

    explicit IPAddress(const internal_::compact_ipdata& ipdata) : mData(ipdata) {
        mData.port = 0U;
        switch (mData.family) {
            case AF_INET:
                mData.cidrlen = IPV4_ADDR_BITS;
                mData.scope_id = 0U;
                break;
            case AF_INET6:
                mData.cidrlen = IPV6_ADDR_BITS;
                if (usesScopedIds(ipdata.ip.v6)) mData.scope_id = ipdata.scope_id;
                break;
            default:
                mData.cidrlen = 0U;
                mData.scope_id = 0U;
                break;
        }
    }

    internal_::compact_ipdata mData{};
};

class IPPrefix {
  public:
    // TODO: "static forString(...)" using NetdConstants' parsePrefix().

    IPPrefix() = default;
    IPPrefix(const IPPrefix&) = default;
    IPPrefix(IPPrefix&&) = default;

    explicit IPPrefix(const IPAddress& ip) : mData(ip.mData) {}

    // Truncate the IP address |ip| at length |length|. Lengths greater than
    // the address-family-relevant maximum, along with negative values, are
    // interpreted as if the address-family-relevant maximum had been given.
    IPPrefix(const IPAddress& ip, int length);

    IPPrefix& operator=(const IPPrefix&) = default;
    IPPrefix& operator=(IPPrefix&&) = default;

    constexpr sa_family_t family() const noexcept { return mData.family; }
    IPAddress ip() const noexcept { return IPAddress(mData); }
    in_addr addr4() const noexcept { return mData.ip.v4; }
    in6_addr addr6() const noexcept { return mData.ip.v6; }
    constexpr int length() const noexcept { return mData.cidrlen; }

    bool isUninitialized() const noexcept;
    std::string toString() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const IPPrefix& prefix) {
        os << prefix.toString();
        return os;
    }
    friend bool operator==(const IPPrefix& a, const IPPrefix& b) { return (a.mData == b.mData); }
    friend bool operator!=(const IPPrefix& a, const IPPrefix& b) { return (a.mData != b.mData); }
    friend bool operator<(const IPPrefix& a, const IPPrefix& b) { return (a.mData < b.mData); }
    friend bool operator>(const IPPrefix& a, const IPPrefix& b) { return (b.mData < a.mData); }
    friend bool operator<=(const IPPrefix& a, const IPPrefix& b) { return (a < b) || (a == b); }
    friend bool operator>=(const IPPrefix& a, const IPPrefix& b) { return (b < a) || (a == b); }

  private:
    internal_::compact_ipdata mData{};
};

// An Internet socket address.
//
// Cannot represent other types of socket addresses (e.g. UNIX socket address, et cetera).
class IPSockAddr {
  public:
    // TODO: static forString

    IPSockAddr() = default;
    IPSockAddr(const IPSockAddr&) = default;
    IPSockAddr(IPSockAddr&&) = default;

    explicit IPSockAddr(const IPAddress& ip) : mData(ip.mData) {}
    IPSockAddr(const IPAddress& ip, in_port_t port) : mData(ip.mData) { mData.port = port; }
    explicit IPSockAddr(const sockaddr_in& ipv4sa)
        : IPSockAddr(IPAddress(ipv4sa.sin_addr), ntohs(ipv4sa.sin_port)) {}
    explicit IPSockAddr(const sockaddr_in6& ipv6sa)
        : IPSockAddr(IPAddress(ipv6sa.sin6_addr, ipv6sa.sin6_scope_id), ntohs(ipv6sa.sin6_port)) {}

    IPSockAddr& operator=(const IPSockAddr&) = default;
    IPSockAddr& operator=(IPSockAddr&&) = default;

    constexpr sa_family_t family() const noexcept { return mData.family; }
    IPAddress ip() const noexcept { return IPAddress(mData); }
    constexpr in_port_t port() const noexcept { return mData.port; }

    // Implicit conversion to sockaddr_storage.
    operator sockaddr_storage() const noexcept {
        sockaddr_storage ss;
        ss.ss_family = mData.family;
        switch (mData.family) {
            case AF_INET:
                reinterpret_cast<sockaddr_in*>(&ss)->sin_addr = mData.ip.v4;
                reinterpret_cast<sockaddr_in*>(&ss)->sin_port = htons(mData.port);
                break;
            case AF_INET6:
                reinterpret_cast<sockaddr_in6*>(&ss)->sin6_addr = mData.ip.v6;
                reinterpret_cast<sockaddr_in6*>(&ss)->sin6_port = htons(mData.port);
                reinterpret_cast<sockaddr_in6*>(&ss)->sin6_scope_id = mData.scope_id;
                break;
        }
        return ss;
    }

    std::string toString() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const IPSockAddr& prefix) {
        os << prefix.toString();
        return os;
    }
    friend bool operator==(const IPSockAddr& a, const IPSockAddr& b) {
        return (a.mData == b.mData);
    }
    friend bool operator!=(const IPSockAddr& a, const IPSockAddr& b) {
        return (a.mData != b.mData);
    }
    friend bool operator<(const IPSockAddr& a, const IPSockAddr& b) { return (a.mData < b.mData); }
    friend bool operator>(const IPSockAddr& a, const IPSockAddr& b) { return (b.mData < a.mData); }
    friend bool operator<=(const IPSockAddr& a, const IPSockAddr& b) { return (a < b) || (a == b); }
    friend bool operator>=(const IPSockAddr& a, const IPSockAddr& b) { return (b < a) || (a == b); }

  private:
    internal_::compact_ipdata mData{};
};

}  // namespace netdutils
}  // namespace android

#endif  // NETDUTILS_INTERNETADDRESSES_H_
