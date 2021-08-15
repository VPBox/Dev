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

#include <regex>
#include <string>

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <android-base/strings.h>

#define LOG_TAG "NetUtilsWrapper"
#include <log/log.h>

#include "NetUtilsWrapper.h"

#define SYSTEM_DIRNAME  "/system/bin/"

#define OEM_IFACE "[^ ]*oem[0-9]+"
#define RMNET_IFACE "(r_)?rmnet_(data)?[0-9]+"
#define CCMNI_IFACE "cc(3)?mni[0-9]+"
#define VENDOR_IFACE "(" OEM_IFACE "|" RMNET_IFACE "|" CCMNI_IFACE ")"
#define VENDOR_CHAIN "(oem_.*|nm_.*|qcom_.*)"

// List of net utils wrapped by this program
// The list MUST be in descending order of string length
const char *netcmds[] = {
    "ip6tables",
    "iptables",
    "ndc",
    "tc",
    "ip",
    nullptr,
};

// List of regular expressions of expected commands.
const char *EXPECTED_REGEXPS[] = {
#define CMD "^" SYSTEM_DIRNAME
    // Create, delete, and manage OEM networks.
    CMD "ndc network (create|destroy) (oem|handle)[0-9]+( |$)",
    CMD "ndc network interface (add|remove) (oem|handle)[0-9]+ " VENDOR_IFACE,
    CMD "ndc network route (add|remove) (oem|handle)[0-9]+ ",
    CMD "ndc ipfwd (enable|disable) ",
    CMD "ndc ipfwd (add|remove) .*" VENDOR_IFACE,

    // Manage vendor iptables rules.
    CMD "ip(6)?tables -w.* (-A|-D|-F|-I|-N|-X) " VENDOR_CHAIN,
    CMD "ip(6)?tables -w.* (-i|-o) " VENDOR_IFACE,

    // Manage IPsec state.
    CMD "ip xfrm .*",

    // Manage vendor interfaces.
    CMD "tc .* dev " VENDOR_IFACE,
    CMD "ip( -4| -6)? (addr|address) (add|del|delete|flush).* dev " VENDOR_IFACE,

    // Other activities observed on current devices. In future releases, these should be supported
    // in a way that is less likely to interfere with general Android networking behaviour.
    CMD "tc qdisc del dev root",
    CMD "ip( -4| -6)? rule .* goto 13000 prio 11999",
    CMD "ip( -4| -6)? rule .* prio 25000",
    CMD "ip(6)?tables -w .* -j " VENDOR_CHAIN,
    CMD "iptables -w -t mangle -[AD] PREROUTING -m socket --nowildcard --restore-skmark -j ACCEPT",
    CMD "ndc network interface (add|remove) oem[0-9]+$",  // Invalid command: no interface removed.
#undef CMD
};

bool checkExpectedCommand(int argc, char **argv) {
    static bool loggedError = false;
    std::vector<const char*> allArgs(argc);
    for (int i = 0; i < argc; i++) {
        allArgs[i] = argv[i];
    }
    std::string fullCmd = android::base::Join(allArgs, ' ');
    for (size_t i = 0; i < ARRAY_SIZE(EXPECTED_REGEXPS); i++) {
        const std::regex expectedRegexp(EXPECTED_REGEXPS[i], std::regex_constants::extended);
        if (std::regex_search(fullCmd, expectedRegexp)) {
            return true;
        }
    }
    if (!loggedError) {
        ALOGI("Unexpected command: %s", fullCmd.c_str());
        fprintf(stderr, LOG_TAG ": Unexpected command: %s\n", fullCmd.c_str());
        loggedError = true;
    }
    return false;
}


// This is the only gateway for vendor programs to reach net utils.
int doMain(int argc, char **argv) {
    char *progname = argv[0];
    char *basename = nullptr;

    basename = strrchr(progname, '/');
    basename = basename ? basename + 1 : progname;

    for (int i = 0; netcmds[i]; ++i) {
        size_t len = strlen(netcmds[i]);
        if (!strncmp(basename, netcmds[i], len)) {
            // truncate to match netcmds[i]
            basename[len] = '\0';

            // hardcode the path to /system so it cannot be overwritten
            char *cmd;
            if (asprintf(&cmd, "%s%s", SYSTEM_DIRNAME, basename) == -1) {
                perror("asprintf");
                exit(EXIT_FAILURE);
            }
            argv[0] = cmd;
            if (checkExpectedCommand(argc, argv)) {
                execv(cmd, argv);
            }
        }
    }

    // Invalid command. Reject and fail.
    exit(EXIT_FAILURE);
}
