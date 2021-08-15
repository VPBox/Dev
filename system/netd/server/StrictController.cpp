/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <string>
#include <vector>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_TAG "StrictController"
#define LOG_NDEBUG 0
#include <log/log.h>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "ConnmarkFlags.h"
#include "NetdConstants.h"
#include "StrictController.h"

auto StrictController::execIptablesRestore = ::execIptablesRestore;

const char* StrictController::LOCAL_OUTPUT = "st_OUTPUT";
const char* StrictController::LOCAL_CLEAR_DETECT = "st_clear_detect";
const char* StrictController::LOCAL_CLEAR_CAUGHT = "st_clear_caught";
const char* StrictController::LOCAL_PENALTY_LOG = "st_penalty_log";
const char* StrictController::LOCAL_PENALTY_REJECT = "st_penalty_reject";

using android::base::Join;
using android::base::StringPrintf;

StrictController::StrictController(void) {
}

int StrictController::setupIptablesHooks(void) {
    char connmarkFlagAccept[16];
    char connmarkFlagReject[16];
    char connmarkFlagTestAccept[32];
    char connmarkFlagTestReject[32];
    sprintf(connmarkFlagAccept, "0x%x", ConnmarkFlags::STRICT_RESOLVED_ACCEPT);
    sprintf(connmarkFlagReject, "0x%x", ConnmarkFlags::STRICT_RESOLVED_REJECT);
    sprintf(connmarkFlagTestAccept, "0x%x/0x%x",
            ConnmarkFlags::STRICT_RESOLVED_ACCEPT,
            ConnmarkFlags::STRICT_RESOLVED_ACCEPT);
    sprintf(connmarkFlagTestReject, "0x%x/0x%x",
            ConnmarkFlags::STRICT_RESOLVED_REJECT,
            ConnmarkFlags::STRICT_RESOLVED_REJECT);

    resetChains();

    int res = 0;
    std::vector<std::string> v4, v6;

#define CMD_V4(...) { auto cmd = StringPrintf(__VA_ARGS__); v4.push_back(cmd); }
#define CMD_V6(...) { auto cmd = StringPrintf(__VA_ARGS__); v6.push_back(cmd); }
#define CMD_V4V6(...) { CMD_V4(__VA_ARGS__); CMD_V6(__VA_ARGS__); };

    CMD_V4V6("*filter");

    // Chain triggered when cleartext socket detected and penalty is log
    CMD_V4V6("-A %s -j CONNMARK --or-mark %s", LOCAL_PENALTY_LOG, connmarkFlagAccept);
    CMD_V4V6("-A %s -j NFLOG --nflog-group 0", LOCAL_PENALTY_LOG);

    // Chain triggered when cleartext socket detected and penalty is reject
    CMD_V4V6("-A %s -j CONNMARK --or-mark %s", LOCAL_PENALTY_REJECT, connmarkFlagReject);
    CMD_V4V6("-A %s -j NFLOG --nflog-group 0", LOCAL_PENALTY_REJECT);
    CMD_V4V6("-A %s -j REJECT", LOCAL_PENALTY_REJECT);

    // We use a high-order mark bit to keep track of connections that we've already resolved.
    // Quickly skip connections that we've already resolved
    CMD_V4V6("-A %s -m connmark --mark %s -j REJECT", LOCAL_CLEAR_DETECT, connmarkFlagTestReject);
    CMD_V4V6("-A %s -m connmark --mark %s -j RETURN", LOCAL_CLEAR_DETECT, connmarkFlagTestAccept);

    // Look for IPv4 TCP/UDP connections with TLS/DTLS header
    const char *u32;
    u32 = "0>>22&0x3C@ 12>>26&0x3C@ 0&0xFFFF0000=0x16030000 &&"
          "0>>22&0x3C@ 12>>26&0x3C@ 4&0x00FF0000=0x00010000";
    CMD_V4("-A %s -p tcp -m u32 --u32 \"%s\" -j CONNMARK --or-mark %s",
           LOCAL_CLEAR_DETECT, u32, connmarkFlagAccept);

    u32 = "0>>22&0x3C@ 8&0xFFFF0000=0x16FE0000 &&"
          "0>>22&0x3C@ 20&0x00FF0000=0x00010000";
    CMD_V4("-A %s -p udp -m u32 --u32 \"%s\" -j CONNMARK --or-mark %s",
           LOCAL_CLEAR_DETECT, u32, connmarkFlagAccept);

    // Look for IPv6 TCP/UDP connections with TLS/DTLS header.  The IPv6 header
    // doesn't have an IHL field to shift with, so we have to manually add in
    // the 40-byte offset at every step.
    u32 = "52>>26&0x3C@ 40&0xFFFF0000=0x16030000 &&"
          "52>>26&0x3C@ 44&0x00FF0000=0x00010000";
    CMD_V6("-A %s -p tcp -m u32 --u32 \"%s\" -j CONNMARK --or-mark %s",
           LOCAL_CLEAR_DETECT, u32, connmarkFlagAccept);

    u32 = "48&0xFFFF0000=0x16FE0000 &&"
          "60&0x00FF0000=0x00010000";
    CMD_V6("-A %s -p udp -m u32 --u32 \"%s\" -j CONNMARK --or-mark %s",
           LOCAL_CLEAR_DETECT, u32, connmarkFlagAccept);

    // Skip newly classified connections from above
    CMD_V4V6("-A %s -m connmark --mark %s -j RETURN", LOCAL_CLEAR_DETECT, connmarkFlagTestAccept);

    // Handle TCP/UDP payloads that didn't match TLS/DTLS filters above,
    // which means we've probably found cleartext data.  The TCP variant
    // depends on u32 returning false when we try reading into the message
    // body to ignore empty ACK packets.
    u32 = "0>>22&0x3C@ 12>>26&0x3C@ 0&0x0=0x0";
    CMD_V4("-A %s -p tcp -m state --state ESTABLISHED -m u32 --u32 \"%s\" -j %s",
           LOCAL_CLEAR_DETECT, u32, LOCAL_CLEAR_CAUGHT);

    u32 = "52>>26&0x3C@ 40&0x0=0x0";
    CMD_V6("-A %s -p tcp -m state --state ESTABLISHED -m u32 --u32 \"%s\" -j %s",
           LOCAL_CLEAR_DETECT, u32, LOCAL_CLEAR_CAUGHT);

    CMD_V4V6("-A %s -p udp -j %s", LOCAL_CLEAR_DETECT, LOCAL_CLEAR_CAUGHT);
    CMD_V4V6("COMMIT\n");

    res |= execIptablesRestore(V4, Join(v4, '\n'));
    res |= execIptablesRestore(V6, Join(v6, '\n'));

#undef CMD_V4
#undef CMD_V6
#undef CMD_V4V6

    return res ? -EREMOTEIO : 0;
}

int StrictController::resetChains(void) {
    // Flush any existing rules
#define CLEAR_CHAIN(x) StringPrintf(":%s -", (x))
    std::vector<std::string> commandList = {
        "*filter",
        CLEAR_CHAIN(LOCAL_OUTPUT),
        CLEAR_CHAIN(LOCAL_PENALTY_LOG),
        CLEAR_CHAIN(LOCAL_PENALTY_REJECT),
        CLEAR_CHAIN(LOCAL_CLEAR_CAUGHT),
        CLEAR_CHAIN(LOCAL_CLEAR_DETECT),
        "COMMIT\n"
    };
    const std::string commands = Join(commandList, '\n');
    return (execIptablesRestore(V4V6, commands) == 0) ? 0 : -EREMOTEIO;
#undef CLEAR_CHAIN
}

int StrictController::setUidCleartextPenalty(uid_t uid, StrictPenalty penalty) {
    // When a penalty is set, we don't know what penalty the UID previously had. In order to be able
    // to clear the previous penalty without causing an iptables error by deleting rules that don't
    // exist, put each UID's rules in a chain specific to that UID. That way, the commands we need
    // to run to clear the previous penalty don't depend on what the penalty actually was - all we
    // need to do is clear the chain.
    std::string perUidChain = StringPrintf("st_clear_caught_%u", uid);

    std::vector<std::string> commands;
    if (penalty == ACCEPT) {
        // Clean up any old rules
        commands = {
            "*filter",
            StringPrintf("-D %s -m owner --uid-owner %d -j %s",
                         LOCAL_OUTPUT, uid, LOCAL_CLEAR_DETECT),
            StringPrintf("-D %s -m owner --uid-owner %d -j %s",
                         LOCAL_CLEAR_CAUGHT, uid, perUidChain.c_str()),
            StringPrintf("-F %s", perUidChain.c_str()),
            StringPrintf("-X %s", perUidChain.c_str()),
        };
    } else {
        // Always take a detour to investigate this UID
        commands.push_back("*filter");
        commands.push_back(StringPrintf(":%s -", perUidChain.c_str()));
        commands.push_back(StringPrintf("-I %s -m owner --uid-owner %d -j %s",
                                        LOCAL_OUTPUT, uid, LOCAL_CLEAR_DETECT));
        commands.push_back(StringPrintf("-I %s -m owner --uid-owner %d -j %s",
                                        LOCAL_CLEAR_CAUGHT, uid, perUidChain.c_str()));

        if (penalty == LOG) {
            commands.push_back(StringPrintf("-A %s -j %s", perUidChain.c_str(), LOCAL_PENALTY_LOG));
        } else if (penalty == REJECT) {
            commands.push_back(StringPrintf("-A %s -j %s", perUidChain.c_str(),
                                            LOCAL_PENALTY_REJECT));
        }
    }
    commands.push_back("COMMIT\n");

    return (execIptablesRestore(V4V6, Join(commands, "\n")) == 0) ? 0 : -EREMOTEIO;
}
