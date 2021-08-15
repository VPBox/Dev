/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef VTS_HAL_NET_NETD_TEST_UTILS_H
#define VTS_HAL_NET_NETD_TEST_UTILS_H

#include <android/multinetwork.h>

#define EXPECT_STATUS(expectedStatus, ret)  \
    do {                                    \
        EXPECT_TRUE((ret).isOk());          \
        EXPECT_EQ((expectedStatus), (ret)); \
    } while (0)

constexpr const char* IP_PATH = "/system/bin/ip";

// Checks that the given network exists.
// Returns 0 if it exists or -errno if it does not.
int checkNetworkExists(net_handle_t netHandle);

// Checks that the given network provides reachability to the given address.
// Returns 0 if it so or -errno if not.
int checkReachability(net_handle_t netHandle, const char* addrStr);

// Counts the number of IPv4 and IPv6 routing rules that match the given regexp string.
int countMatchingIpRules(const std::string& regexString);

// Counts the number of IPv4 and IPv6 routing rules that select the specified fwmark.
int countRulesForFwmark(const uint32_t fwmark);

#endif
