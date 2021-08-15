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
 * IptablesBaseTest.cpp - utility class for tests that use iptables
 */

#include <deque>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <android-base/stringprintf.h>

#include "IptablesBaseTest.h"
#include "NetdConstants.h"

#define LOG_TAG "IptablesBaseTest"
#include <log/log.h>

using android::base::StringPrintf;

IptablesBaseTest::IptablesBaseTest() {
    sCmds.clear();
    sRestoreCmds.clear();
    sReturnValues.clear();
}

int IptablesBaseTest::fake_android_fork_exec(int argc, char* argv[], int *status, bool, bool) {
    std::string cmd = argv[0];
    for (int i = 1; i < argc; i++) {
        if (argv[i] == nullptr) break;
        cmd += " ";
        cmd += argv[i];
    }
    sCmds.push_back(cmd);

    int ret;
    if (sReturnValues.size()) {
        ret = sReturnValues.front();
        sReturnValues.pop_front();
    } else {
        ret = 0;
    }

    if (status) {
        *status = ret;
    }
    return ret;
}

FILE *IptablesBaseTest::fake_popen(const char * /* cmd */, const char *type) {
    if (sPopenContents.empty() || strcmp(type, "r") != 0) {
        return nullptr;
    }

    std::string realCmd = StringPrintf("echo '%s'", sPopenContents.front().c_str());
    sPopenContents.pop_front();
    return popen(realCmd.c_str(), "r");  // NOLINT(cert-env33-c)
}

int IptablesBaseTest::fakeExecIptablesRestoreWithOutput(IptablesTarget target,
                                                        const std::string& commands,
                                                        std::string *output) {
    sRestoreCmds.push_back({ target, commands });
    if (output != nullptr) {
        *output = sIptablesRestoreOutput.size() ? sIptablesRestoreOutput.front().c_str() : "";
    }
    if (sIptablesRestoreOutput.size()) {
        sIptablesRestoreOutput.pop_front();
    }
    return 0;
}

int IptablesBaseTest::fakeExecIptablesRestore(IptablesTarget target, const std::string& commands) {
    return fakeExecIptablesRestoreWithOutput(target, commands, nullptr);
}

int IptablesBaseTest::fakeExecIptablesRestoreCommand(IptablesTarget target,
                                                     const std::string& table,
                                                     const std::string& command,
                                                     std::string *output) {
    std::string fullCmd = StringPrintf("-t %s %s", table.c_str(), command.c_str());
    return fakeExecIptablesRestoreWithOutput(target, fullCmd, output);
}

void IptablesBaseTest::expectIptablesRestoreCommands(const std::vector<std::string>& expectedCmds) {
    ExpectedIptablesCommands expected;
    for (const auto& cmd : expectedCmds) {
        expected.push_back({ V4V6, cmd });
    }
    expectIptablesRestoreCommands(expected);
}

void IptablesBaseTest::expectIptablesRestoreCommands(const ExpectedIptablesCommands& expectedCmds) {
    EXPECT_EQ(expectedCmds.size(), sRestoreCmds.size());
    for (size_t i = 0; i < expectedCmds.size(); i++) {
        EXPECT_EQ(expectedCmds[i], sRestoreCmds[i]) <<
            "iptables-restore command " << i << " differs";
    }
    sRestoreCmds.clear();
}

std::vector<std::string> IptablesBaseTest::sCmds = {};
IptablesBaseTest::ExpectedIptablesCommands IptablesBaseTest::sRestoreCmds = {};
std::deque<std::string> IptablesBaseTest::sPopenContents = {};
std::deque<std::string> IptablesBaseTest::sIptablesRestoreOutput = {};
std::deque<int> IptablesBaseTest::sReturnValues = {};
