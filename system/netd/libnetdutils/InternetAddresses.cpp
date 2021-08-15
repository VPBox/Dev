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

#include "netdutils/InternetAddresses.h"

#include <android-base/stringprintf.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace android {

using base::StringPrintf;

namespace netdutils {

std::string IPAddress::toString() const noexcept {
    char repr[INET6_ADDRSTRLEN] = "\0";

    switch (mData.family) {
        case AF_UNSPEC:
            return "<unspecified>";
        case AF_INET: {
            const in_addr v4 = mData.ip.v4;
            inet_ntop(AF_INET, &v4, repr, sizeof(repr));
            break;
        }
        case AF_INET6: {
            const in6_addr v6 = mData.ip.v6;
            inet_ntop(AF_INET6, &v6, repr, sizeof(repr));
            break;
        }
        default:
            return "<unknown_family>";
    }

    if (mData.family == AF_INET6 && mData.scope_id > 0) {
        return StringPrintf("%s%%%u", repr, mData.scope_id);
    }

    return repr;
}

bool IPAddress::forString(const std::string& repr, IPAddress* ip) {
    const addrinfo hints = {
            .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
    };
    addrinfo* res;
    const int ret = getaddrinfo(repr.c_str(), nullptr, &hints, &res);
    // TODO: move ScopedAddrinfo into libnetdutils and use it here.
    if (ret != 0) {
        freeaddrinfo(res);
        return false;
    }

    bool rval = true;
    switch (res[0].ai_family) {
        case AF_INET: {
            sockaddr_in* sin = (sockaddr_in*) res[0].ai_addr;
            if (ip) *ip = IPAddress(sin->sin_addr);
            break;
        }
        case AF_INET6: {
            sockaddr_in6* sin6 = (sockaddr_in6*) res[0].ai_addr;
            if (ip) *ip = IPAddress(sin6->sin6_addr, sin6->sin6_scope_id);
            break;
        }
        default:
            rval = false;
            break;
    }

    freeaddrinfo(res);
    return rval;
}

IPPrefix::IPPrefix(const IPAddress& ip, int length) : IPPrefix(ip) {
    // Silently treat CIDR lengths like "-1" as meaning the full bit length
    // appropriate to the address family.
    if (length < 0) return;
    if (length >= mData.cidrlen) return;

    switch (mData.family) {
        case AF_UNSPEC:
            break;
        case AF_INET: {
            const in_addr_t mask = (length > 0) ? (~0U) << (IPV4_ADDR_BITS - length) : 0U;
            mData.ip.v4.s_addr &= htonl(mask);
            mData.cidrlen = static_cast<uint8_t>(length);
            break;
        }
        case AF_INET6: {
            // The byte in which this CIDR length falls.
            const int which = length / 8;
            const int mask = (length % 8 == 0) ? 0 : 0xff << (8 - length % 8);
            mData.ip.v6.s6_addr[which] &= mask;
            for (int i = which + 1; i < IPV6_ADDR_LEN; i++) {
                mData.ip.v6.s6_addr[i] = 0U;
            }
            mData.cidrlen = static_cast<uint8_t>(length);
            break;
        }
        default:
            // TODO: Complain bitterly about possible data corruption?
            return;
    }
}

bool IPPrefix::isUninitialized() const noexcept {
    static const internal_::compact_ipdata empty{};
    return mData == empty;
}

std::string IPPrefix::toString() const noexcept {
    return StringPrintf("%s/%d", ip().toString().c_str(), mData.cidrlen);
}

std::string IPSockAddr::toString() const noexcept {
    switch (mData.family) {
        case AF_INET6:
            return StringPrintf("[%s]:%u", ip().toString().c_str(), mData.port);
        default:
            return StringPrintf("%s:%u", ip().toString().c_str(), mData.port);
    }
}

}  // namespace netdutils
}  // namespace android
