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

#ifndef NETDUTILS_SOCKET_H
#define NETDUTILS_SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#include "netdutils/StatusOr.h"

namespace android {
namespace netdutils {

inline sockaddr* asSockaddrPtr(void* addr) {
    return reinterpret_cast<sockaddr*>(addr);
}

inline const sockaddr* asSockaddrPtr(const void* addr) {
    return reinterpret_cast<const sockaddr*>(addr);
}

// Return a string representation of addr or Status if there was a
// failure during conversion.
StatusOr<std::string> toString(const in6_addr& addr);

}  // namespace netdutils
}  // namespace android

#endif /* NETDUTILS_SOCKET_H */
