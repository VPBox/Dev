/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * StrictControllerTest.cpp - unit tests for StrictController.cpp
 */

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <android-base/strings.h>

#include "StrictController.h"
#include "IptablesBaseTest.h"

class StrictControllerTest : public IptablesBaseTest {
public:
    StrictControllerTest() {
        StrictController::execIptablesRestore = fakeExecIptablesRestore;
    }
    StrictController mStrictCtrl;
};

TEST_F(StrictControllerTest, TestSetupIptablesHooks) {
    mStrictCtrl.setupIptablesHooks();

    std::vector<std::string> common = {
        "*filter",
        ":st_OUTPUT -",
        ":st_penalty_log -",
        ":st_penalty_reject -",
        ":st_clear_caught -",
        ":st_clear_detect -",
        "COMMIT\n"
    };

    std::vector<std::string> v4 = {
        "*filter",
        "-A st_penalty_log -j CONNMARK --or-mark 0x1000000",
        "-A st_penalty_log -j NFLOG --nflog-group 0",
        "-A st_penalty_reject -j CONNMARK --or-mark 0x2000000",
        "-A st_penalty_reject -j NFLOG --nflog-group 0",
        "-A st_penalty_reject -j REJECT",
        "-A st_clear_detect -m connmark --mark 0x2000000/0x2000000 -j REJECT",
        "-A st_clear_detect -m connmark --mark 0x1000000/0x1000000 -j RETURN",
        "-A st_clear_detect -p tcp -m u32 --u32 \""
            "0>>22&0x3C@ 12>>26&0x3C@ 0&0xFFFF0000=0x16030000 &&"
            "0>>22&0x3C@ 12>>26&0x3C@ 4&0x00FF0000=0x00010000"
            "\" -j CONNMARK --or-mark 0x1000000",
        "-A st_clear_detect -p udp -m u32 --u32 \""
            "0>>22&0x3C@ 8&0xFFFF0000=0x16FE0000 &&"
            "0>>22&0x3C@ 20&0x00FF0000=0x00010000"
            "\" -j CONNMARK --or-mark 0x1000000",
        "-A st_clear_detect -m connmark --mark 0x1000000/0x1000000 -j RETURN",
        "-A st_clear_detect -p tcp -m state --state ESTABLISHED -m u32 --u32 "
            "\"0>>22&0x3C@ 12>>26&0x3C@ 0&0x0=0x0\" -j st_clear_caught",
        "-A st_clear_detect -p udp -j st_clear_caught",
        "COMMIT\n"
    };

    std::vector<std::string> v6 = {
        "*filter",
        "-A st_penalty_log -j CONNMARK --or-mark 0x1000000",
        "-A st_penalty_log -j NFLOG --nflog-group 0",
        "-A st_penalty_reject -j CONNMARK --or-mark 0x2000000",
        "-A st_penalty_reject -j NFLOG --nflog-group 0",
        "-A st_penalty_reject -j REJECT",
        "-A st_clear_detect -m connmark --mark 0x2000000/0x2000000 -j REJECT",
        "-A st_clear_detect -m connmark --mark 0x1000000/0x1000000 -j RETURN",

        "-A st_clear_detect -p tcp -m u32 --u32 \""
            "52>>26&0x3C@ 40&0xFFFF0000=0x16030000 &&"
            "52>>26&0x3C@ 44&0x00FF0000=0x00010000"
            "\" -j CONNMARK --or-mark 0x1000000",
        "-A st_clear_detect -p udp -m u32 --u32 \""
            "48&0xFFFF0000=0x16FE0000 &&"
            "60&0x00FF0000=0x00010000"
            "\" -j CONNMARK --or-mark 0x1000000",
        "-A st_clear_detect -m connmark --mark 0x1000000/0x1000000 -j RETURN",
        "-A st_clear_detect -p tcp -m state --state ESTABLISHED -m u32 --u32 "
            "\"52>>26&0x3C@ 40&0x0=0x0\" -j st_clear_caught",
        "-A st_clear_detect -p udp -j st_clear_caught",
        "COMMIT\n"
    };

    std::string commandsCommon = android::base::Join(common, '\n');
    std::string commands4 = android::base::Join(v4, '\n');
    std::string commands6 = android::base::Join(v6, '\n');

    std::vector<std::pair<IptablesTarget, std::string>> expected = {
        { V4V6, commandsCommon },
        { V4, commands4 },
        { V6, commands6 },
    };
    expectIptablesRestoreCommands(expected);
}

TEST_F(StrictControllerTest, TestResetChains) {
    mStrictCtrl.resetChains();

    const std::string expected =
        "*filter\n"
        ":st_OUTPUT -\n"
        ":st_penalty_log -\n"
        ":st_penalty_reject -\n"
        ":st_clear_caught -\n"
        ":st_clear_detect -\n"
        "COMMIT\n";
    expectIptablesRestoreCommands({ expected });
}

TEST_F(StrictControllerTest, TestSetUidCleartextPenalty) {
    std::vector<std::string> acceptCommands = {
        "*filter\n"
        "-D st_OUTPUT -m owner --uid-owner 12345 -j st_clear_detect\n"
        "-D st_clear_caught -m owner --uid-owner 12345 -j st_clear_caught_12345\n"
        "-F st_clear_caught_12345\n"
        "-X st_clear_caught_12345\n"
        "COMMIT\n"
    };
    std::vector<std::string> logCommands = {
        "*filter\n"
        ":st_clear_caught_12345 -\n"
        "-I st_OUTPUT -m owner --uid-owner 12345 -j st_clear_detect\n"
        "-I st_clear_caught -m owner --uid-owner 12345 -j st_clear_caught_12345\n"
        "-A st_clear_caught_12345 -j st_penalty_log\n"
        "COMMIT\n"
    };
    std::vector<std::string> rejectCommands = {
        "*filter\n"
        ":st_clear_caught_12345 -\n"
        "-I st_OUTPUT -m owner --uid-owner 12345 -j st_clear_detect\n"
        "-I st_clear_caught -m owner --uid-owner 12345 -j st_clear_caught_12345\n"
        "-A st_clear_caught_12345 -j st_penalty_reject\n"
        "COMMIT\n"
    };

    mStrictCtrl.setUidCleartextPenalty(12345, LOG);
    expectIptablesRestoreCommands(logCommands);

    mStrictCtrl.setUidCleartextPenalty(12345, ACCEPT);
    expectIptablesRestoreCommands(acceptCommands);

    // StrictController doesn't keep any state and it is not correct to call its methods in the
    // wrong order (e.g., to go from LOG to REJECT without passing through ACCEPT).
    // NetworkManagementService does keep state (not just to ensure correctness, but also so it can
    // reprogram the rules when netd crashes).
    mStrictCtrl.setUidCleartextPenalty(12345, REJECT);
    expectIptablesRestoreCommands(rejectCommands);

    mStrictCtrl.setUidCleartextPenalty(12345, ACCEPT);
    expectIptablesRestoreCommands(acceptCommands);
}
