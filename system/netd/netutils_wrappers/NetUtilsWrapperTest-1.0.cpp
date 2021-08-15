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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <android-base/strings.h>

#include "NetUtilsWrapper.h"

#define MAX_ARGS 128
#define VALID true
#define INVALID false

struct Command {
    bool valid;
    std::string cmdString;
};

std::vector<Command> COMMANDS = {
    {INVALID, "tc qdisc del dev root"},
    {VALID,   "/system/bin/tc qdisc del dev root"},
    {VALID,   "/system/bin/ip -6 addr add dev r_rmnet_data6 2001:db8::/64"},
    {INVALID, "/system/bin/ip -6 addr add dev wlan2 2001:db8::/64"},
    {VALID,   "/system/bin/ip6tables -w -A INPUT -j qcom_foo"},
    {INVALID, "/system/bin/ip6tables -w -A INPUT -j routectrl_MANGLE_INPUT"},
    {VALID,   "/system/bin/ip6tables -w -A INPUT -i rmnet_data9 -j routectrl_MANGLE_INPUT"},
    {VALID,   "/system/bin/ip6tables -w -F nm_pre_ip4"},
    {INVALID, "/system/bin/ip6tables -w -F INPUT"},
    {VALID,   "/system/bin/ndc network interface add oem10"},
    {VALID,   "/system/bin/ndc network interface add oem10 v_oem9"},
    {VALID,   "/system/bin/ndc network interface add oem10 oem9"},
    {INVALID, "/system/bin/ndc network interface add 100 v_oem9"},
    {VALID,   "/system/bin/ndc network interface add oem10 r_rmnet_data0"},
    {VALID,   "/system/bin/ndc network interface add handle42966108894 v_oem9"},
    {VALID,   "/system/bin/ndc network interface add handle42966108894 oem9"},
    {VALID,   "/system/bin/ndc network interface add handle42966108894 r_rmnet_data0"},
    {INVALID, "/system/bin/ndc network interface add handle42966108894"},
    {VALID,   "/system/bin/ip xfrm state"},
};

TEST(NetUtilsWrapperTest10, TestCommands) {
    // Overwritten by each test case.
    char *argv[MAX_ARGS];

    for (const Command& cmd : COMMANDS) {
        std::vector<std::string> pieces = android::base::Split(cmd.cmdString, " ");
        ASSERT_LE(pieces.size(), ARRAY_SIZE(argv));
        for (size_t i = 0; i < pieces.size(); i++) {
            argv[i] = const_cast<char*>(pieces[i].c_str());
        }
        EXPECT_EQ(cmd.valid, checkExpectedCommand(pieces.size(), argv)) <<
            "Expected command to be " <<
            (cmd.valid ? "valid" : "invalid") << ", but was " <<
            (cmd.valid ? "invalid" : "valid") << ": '" << cmd.cmdString << "'";
    }
}
