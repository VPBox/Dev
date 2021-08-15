/*
 * Copyright 2017 The Android Open Source Project
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
 * ControllersTest.cpp - unit tests for Controllers.cpp
 */

#include <set>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <android-base/strings.h>

#include "Controllers.h"
#include "IptablesBaseTest.h"

using testing::ContainerEq;

namespace android {
namespace net {

class ControllersTest : public IptablesBaseTest {
  public:
    ControllersTest() {
        Controllers::execIptablesRestore = fakeExecIptablesRestore;
        Controllers::execIptablesRestoreWithOutput = fakeExecIptablesRestoreWithOutput;
    }

  protected:
    void initChildChains() { Controllers::initChildChains(); };
    std::set<std::string> findExistingChildChains(IptablesTarget a, const char* b, const char*c) {
        return Controllers::findExistingChildChains(a, b, c);
    }
};

TEST_F(ControllersTest, TestFindExistingChildChains) {
    ExpectedIptablesCommands expectedCmds = {
        { V6, "*raw\n-S PREROUTING\nCOMMIT\n" },
    };
    sIptablesRestoreOutput.push_back(
        "-P PREROUTING ACCEPT\n"
        "-A PREROUTING -j bw_raw_PREROUTING\n"
        "-A PREROUTING -j idletimer_raw_PREROUTING\n"
        "-A PREROUTING -j tetherctrl_raw_PREROUTING\n"
    );
    std::set<std::string> expectedChains = {
        "bw_raw_PREROUTING",
        "idletimer_raw_PREROUTING",
        "tetherctrl_raw_PREROUTING",
    };
    std::set<std::string> actual = findExistingChildChains(V6, "raw", "PREROUTING");
    EXPECT_THAT(expectedChains, ContainerEq(actual));
    expectIptablesRestoreCommands(expectedCmds);
}

TEST_F(ControllersTest, TestInitIptablesRules) {
    // Test what happens when we boot and there are no rules.
    ExpectedIptablesCommands expected = {
            {V4V6,
             "*filter\n"
             ":INPUT -\n"
             "-F INPUT\n"
             ":bw_INPUT -\n"
             "-A INPUT -j bw_INPUT\n"
             ":fw_INPUT -\n"
             "-A INPUT -j fw_INPUT\n"
             "COMMIT\n"},
            {V4V6,
             "*filter\n"
             ":FORWARD -\n"
             "-F FORWARD\n"
             ":oem_fwd -\n"
             "-A FORWARD -j oem_fwd\n"
             ":fw_FORWARD -\n"
             "-A FORWARD -j fw_FORWARD\n"
             ":bw_FORWARD -\n"
             "-A FORWARD -j bw_FORWARD\n"
             ":tetherctrl_FORWARD -\n"
             "-A FORWARD -j tetherctrl_FORWARD\n"
             "COMMIT\n"},
            {V4V6,
             "*raw\n"
             ":PREROUTING -\n"
             "-F PREROUTING\n"
             ":clat_raw_PREROUTING -\n"
             "-A PREROUTING -j clat_raw_PREROUTING\n"
             ":bw_raw_PREROUTING -\n"
             "-A PREROUTING -j bw_raw_PREROUTING\n"
             ":idletimer_raw_PREROUTING -\n"
             "-A PREROUTING -j idletimer_raw_PREROUTING\n"
             ":tetherctrl_raw_PREROUTING -\n"
             "-A PREROUTING -j tetherctrl_raw_PREROUTING\n"
             "COMMIT\n"},
            {V4V6,
             "*mangle\n"
             ":FORWARD -\n"
             "-F FORWARD\n"
             ":tetherctrl_mangle_FORWARD -\n"
             "-A FORWARD -j tetherctrl_mangle_FORWARD\n"
             "COMMIT\n"},
            {V4V6,
             "*mangle\n"
             ":INPUT -\n"
             "-F INPUT\n"
             ":wakeupctrl_mangle_INPUT -\n"
             "-A INPUT -j wakeupctrl_mangle_INPUT\n"
             ":routectrl_mangle_INPUT -\n"
             "-A INPUT -j routectrl_mangle_INPUT\n"
             "COMMIT\n"},
            {V4,
             "*nat\n"
             ":PREROUTING -\n"
             "-F PREROUTING\n"
             ":oem_nat_pre -\n"
             "-A PREROUTING -j oem_nat_pre\n"
             "COMMIT\n"},
            {V4,
             "*nat\n"
             ":POSTROUTING -\n"
             "-F POSTROUTING\n"
             ":tetherctrl_nat_POSTROUTING -\n"
             "-A POSTROUTING -j tetherctrl_nat_POSTROUTING\n"
             "COMMIT\n"},
            {V4,
             "*filter\n"
             "-S OUTPUT\n"
             "COMMIT\n"},
            {V4,
             "*filter\n"
             ":oem_out -\n"
             "-A OUTPUT -j oem_out\n"
             ":fw_OUTPUT -\n"
             "-A OUTPUT -j fw_OUTPUT\n"
             ":st_OUTPUT -\n"
             "-A OUTPUT -j st_OUTPUT\n"
             ":bw_OUTPUT -\n"
             "-A OUTPUT -j bw_OUTPUT\n"
             "COMMIT\n"},
            {V6,
             "*filter\n"
             "-S OUTPUT\n"
             "COMMIT\n"},
            {V6,
             "*filter\n"
             ":oem_out -\n"
             "-A OUTPUT -j oem_out\n"
             ":fw_OUTPUT -\n"
             "-A OUTPUT -j fw_OUTPUT\n"
             ":st_OUTPUT -\n"
             "-A OUTPUT -j st_OUTPUT\n"
             ":bw_OUTPUT -\n"
             "-A OUTPUT -j bw_OUTPUT\n"
             "COMMIT\n"},
            {V4,
             "*mangle\n"
             "-S POSTROUTING\n"
             "COMMIT\n"},
            {V4,
             "*mangle\n"
             ":oem_mangle_post -\n"
             "-A POSTROUTING -j oem_mangle_post\n"
             ":bw_mangle_POSTROUTING -\n"
             "-A POSTROUTING -j bw_mangle_POSTROUTING\n"
             ":idletimer_mangle_POSTROUTING -\n"
             "-A POSTROUTING -j idletimer_mangle_POSTROUTING\n"
             "COMMIT\n"},
            {V6,
             "*mangle\n"
             "-S POSTROUTING\n"
             "COMMIT\n"},
            {V6,
             "*mangle\n"
             ":oem_mangle_post -\n"
             "-A POSTROUTING -j oem_mangle_post\n"
             ":bw_mangle_POSTROUTING -\n"
             "-A POSTROUTING -j bw_mangle_POSTROUTING\n"
             ":idletimer_mangle_POSTROUTING -\n"
             "-A POSTROUTING -j idletimer_mangle_POSTROUTING\n"
             "COMMIT\n"},
    };

    // Check that we run these commands and these only.
    initChildChains();
    expectIptablesRestoreCommands(expected);
    expectIptablesRestoreCommands(ExpectedIptablesCommands{});

    // Now test what happens when some rules exist (e.g., if we crash and restart).

    // First, explicitly tell the iptables test code to return empty output to all the commands we
    // send. This allows us to tell it to return non-empty output to particular commands in the
    // following code.
    for (size_t i = 0; i < expected.size(); i++) {
        sIptablesRestoreOutput.push_back("");
    }

    // Define a macro to remove a substring from a string. We use a macro instead of a function so
    // we can assert in it. In the following code, we use ASSERT_* to check for programming errors
    // in the test code, and EXPECT_* to check for errors in the actual code.
#define DELETE_SUBSTRING(substr, str) {                      \
        size_t start = (str).find((substr));                 \
        ASSERT_NE(std::string::npos, start);                 \
        (str).erase(start, strlen((substr)));                \
        ASSERT_EQ(std::string::npos, (str).find((substr)));  \
    }

    // Now set test expectations.

    // 1. Test that if we find rules that we don't create ourselves, we ignore them.
    // First check that command #7 is where we list the OUTPUT chain in the (IPv4) filter table:
    ASSERT_NE(std::string::npos, expected[7].second.find("*filter\n-S OUTPUT\n"));
    // ... and pretend that when we run that command, we find the following rules. Because we don't
    // create any of these rules ourselves, our behaviour is unchanged.
    sIptablesRestoreOutput[7] =
        "-P OUTPUT ACCEPT\n"
        "-A OUTPUT -o r_rmnet_data8 -p udp -m udp --dport 1900 -j DROP\n";

    // 2. Test that rules that we create ourselves are not added if they already exist.
    // Pretend that when we list the OUTPUT chain in the (IPv6) filter table, we find the oem_out
    // and st_OUTPUT chains:
    ASSERT_NE(std::string::npos, expected[9].second.find("*filter\n-S OUTPUT\n"));
    sIptablesRestoreOutput[9] =
        "-A OUTPUT -j oem_out\n"
        "-A OUTPUT -j st_OUTPUT\n";
    // ... and expect that when we populate the OUTPUT chain, we do not re-add them.
    DELETE_SUBSTRING("-A OUTPUT -j oem_out\n", expected[10].second);
    DELETE_SUBSTRING("-A OUTPUT -j st_OUTPUT\n", expected[10].second);

    // 3. Now test that when we list the POSTROUTING chain in the mangle table, we find a mixture of
    // netd-created rules and vendor rules:
    ASSERT_NE(std::string::npos, expected[13].second.find("*mangle\n-S POSTROUTING\n"));
    sIptablesRestoreOutput[13] =
        "-P POSTROUTING ACCEPT\n"
        "-A POSTROUTING -j oem_mangle_post\n"
        "-A POSTROUTING -j bw_mangle_POSTROUTING\n"
        "-A POSTROUTING -j idletimer_mangle_POSTROUTING\n"
        "-A POSTROUTING -j qcom_qos_reset_POSTROUTING\n"
        "-A POSTROUTING -j qcom_qos_filter_POSTROUTING\n";
    // and expect that we don't re-add the netd-created rules that already exist.
    DELETE_SUBSTRING("-A POSTROUTING -j oem_mangle_post\n", expected[14].second);
    DELETE_SUBSTRING("-A POSTROUTING -j bw_mangle_POSTROUTING\n", expected[14].second);
    DELETE_SUBSTRING("-A POSTROUTING -j idletimer_mangle_POSTROUTING\n", expected[14].second);

    // In this last case, also check that our expectations are reasonable.
    std::string expectedCmd14 =
        "*mangle\n"
        ":oem_mangle_post -\n"
        ":bw_mangle_POSTROUTING -\n"
        ":idletimer_mangle_POSTROUTING -\n"
        "COMMIT\n";
    ASSERT_EQ(expectedCmd14, expected[14].second);

    // Finally, actually test that initChildChains runs the expected commands, and nothing more.
    initChildChains();
    expectIptablesRestoreCommands(expected);
    expectIptablesRestoreCommands(ExpectedIptablesCommands{});
}

}  // namespace net
}  // namespace android
