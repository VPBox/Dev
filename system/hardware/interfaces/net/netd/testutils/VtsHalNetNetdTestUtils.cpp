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

#include <regex>
#include <string>
#include <vector>

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/stringprintf.h>
#include <android/multinetwork.h>

#include "VtsHalNetNetdTestUtils.h"

using android::base::StringPrintf;

int checkNetworkExists(net_handle_t netHandle) {
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1) {
        return -ENOTSOCK;
    }
    int ret = android_setsocknetwork(netHandle, sock);
    if (ret) {
        ret = -errno;
    }
    close(sock);
    return ret;
}

// TODO: deduplicate this with system/netd/tests/binder_test.cpp.
static std::vector<std::string> runCommand(const std::string& command) {
    std::vector<std::string> lines;
    FILE* f;

    if ((f = popen(command.c_str(), "r")) == nullptr) {
        perror("popen");
        return lines;
    }

    char* line = nullptr;
    size_t bufsize = 0;
    ssize_t linelen = 0;
    while ((linelen = getline(&line, &bufsize, f)) >= 0) {
        lines.push_back(std::string(line, linelen));
    }

    free(line);
    pclose(f);
    return lines;
}

static std::vector<std::string> listIpRules(const char* ipVersion) {
    std::string command = StringPrintf("%s %s rule list", IP_PATH, ipVersion);
    return runCommand(command);
}

int countMatchingIpRules(const std::string& regexString) {
    const std::regex regex(regexString, std::regex_constants::extended);
    int matches = 0;

    for (const char* version : {"-4", "-6"}) {
        std::vector<std::string> rules = listIpRules(version);
        for (const auto& rule : rules) {
            if (std::regex_search(rule, regex)) {
                matches++;
            }
        }
    }

    return matches;
}

int countRulesForFwmark(const uint32_t fwmark) {
    // Skip top nibble, which differs between rules.
    std::string regex =
        StringPrintf("from all fwmark 0x(%x|[0-9a-f]+%04x)/.* lookup ", fwmark, fwmark);
    return countMatchingIpRules(regex);
}

int checkReachability(net_handle_t netHandle, const char* addrStr) {
    addrinfo *ai, hints = {.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV};
    int ret = getaddrinfo(addrStr, "53", &hints, &ai);
    if (ret) {
        return -EINVAL;
    }

    int sock = socket(ai->ai_family, SOCK_DGRAM, 0);
    if (sock == -1 || android_setsocknetwork(netHandle, sock) == -1) {
        ret = -errno;
        freeaddrinfo(ai);
        return ret;
    }

    ret = connect(sock, ai->ai_addr, ai->ai_addrlen);
    close(sock);
    if (ret == -1) {
        ret = -errno;
    }
    freeaddrinfo(ai);
    return ret;
}
