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

#include <arpa/inet.h>

#include "netdutils/Slice.h"
#include "netdutils/Socket.h"

namespace android {
namespace netdutils {

StatusOr<std::string> toString(const in6_addr& addr) {
    std::array<char, INET6_ADDRSTRLEN> out = {};
    auto* rv = inet_ntop(AF_INET6, &addr, out.data(), out.size());
    if (rv == nullptr) {
        return statusFromErrno(errno, "inet_ntop() failed");
    }
    return std::string(out.data());
}

}  // namespace netdutils
}  // namespace android
