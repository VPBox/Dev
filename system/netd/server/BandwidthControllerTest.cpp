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
 * BandwidthControllerTest.cpp - unit tests for BandwidthController.cpp
 */

#include <string>
#include <vector>

#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <gtest/gtest.h>

#include <android-base/strings.h>
#include <android-base/stringprintf.h>

#include <netdutils/MockSyscalls.h>
#include "BandwidthController.h"
#include "Fwmark.h"
#include "IptablesBaseTest.h"
#include "bpf/BpfUtils.h"
#include "netdbpf/bpf_shared.h"
#include "tun_interface.h"

using ::testing::_;
using ::testing::ByMove;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

using android::base::Join;
using android::base::StringPrintf;
using android::net::TunInterface;
using android::netdutils::UniqueFile;
using android::netdutils::status::ok;

const std::string ACCOUNT_RULES_WITHOUT_BPF =
        "*filter\n"
        "-A bw_INPUT -j bw_global_alert\n"
        "-A bw_INPUT -p esp -j RETURN\n"
        "-A bw_INPUT -m mark --mark 0x100000/0x100000 -j RETURN\n"
        "-A bw_INPUT -m owner --socket-exists\n"
        "-A bw_INPUT -j MARK --or-mark 0x100000\n"
        "-A bw_OUTPUT -j bw_global_alert\n"
        "-A bw_OUTPUT -o ipsec+ -j RETURN\n"
        "-A bw_OUTPUT -m policy --pol ipsec --dir out -j RETURN\n"
        "-A bw_OUTPUT -m owner --socket-exists\n"
        "-A bw_costly_shared --jump bw_penalty_box\n"
        "\n"
        "-A bw_penalty_box --jump bw_happy_box\n"
        "-A bw_happy_box --jump bw_data_saver\n"
        "-A bw_data_saver -j RETURN\n"
        "-I bw_happy_box -m owner --uid-owner 0-9999 --jump RETURN\n"
        "COMMIT\n"
        "*raw\n"
        "-A bw_raw_PREROUTING -i ipsec+ -j RETURN\n"
        "-A bw_raw_PREROUTING -m policy --pol ipsec --dir in -j RETURN\n"
        "-A bw_raw_PREROUTING -m owner --socket-exists\n"
        "COMMIT\n"
        "*mangle\n"
        "-A bw_mangle_POSTROUTING -o ipsec+ -j RETURN\n"
        "-A bw_mangle_POSTROUTING -m policy --pol ipsec --dir out -j RETURN\n"
        "-A bw_mangle_POSTROUTING -m owner --socket-exists\n"
        "-A bw_mangle_POSTROUTING -j MARK --set-mark 0x0/0x100000\n"
        "\n"
        "COMMIT\n";

const std::string ACCOUNT_RULES_WITH_BPF =
        "*filter\n"
        "-A bw_INPUT -j bw_global_alert\n"
        "-A bw_INPUT -p esp -j RETURN\n"
        "-A bw_INPUT -m mark --mark 0x100000/0x100000 -j RETURN\n"
        "\n"
        "-A bw_INPUT -j MARK --or-mark 0x100000\n"
        "-A bw_OUTPUT -j bw_global_alert\n"
        "-A bw_OUTPUT -o ipsec+ -j RETURN\n"
        "-A bw_OUTPUT -m policy --pol ipsec --dir out -j RETURN\n"
        "\n"
        "-A bw_costly_shared --jump bw_penalty_box\n" +
        StringPrintf("-I bw_penalty_box -m bpf --object-pinned %s -j REJECT\n",
                     XT_BPF_BLACKLIST_PROG_PATH) +
        "-A bw_penalty_box --jump bw_happy_box\n"
        "-A bw_happy_box --jump bw_data_saver\n"
        "-A bw_data_saver -j RETURN\n" +
        StringPrintf("-I bw_happy_box -m bpf --object-pinned %s -j RETURN\n",
                     XT_BPF_WHITELIST_PROG_PATH) +
        "COMMIT\n"
        "*raw\n"
        "-A bw_raw_PREROUTING -i ipsec+ -j RETURN\n"
        "-A bw_raw_PREROUTING -m policy --pol ipsec --dir in -j RETURN\n" +
        StringPrintf("-A bw_raw_PREROUTING -m bpf --object-pinned %s\n", XT_BPF_INGRESS_PROG_PATH) +
        "COMMIT\n"
        "*mangle\n"
        "-A bw_mangle_POSTROUTING -o ipsec+ -j RETURN\n"
        "-A bw_mangle_POSTROUTING -m policy --pol ipsec --dir out -j RETURN\n"
        "\n"
        "-A bw_mangle_POSTROUTING -j MARK --set-mark 0x0/0x100000\n" +
        StringPrintf("-A bw_mangle_POSTROUTING -m bpf --object-pinned %s\n",
                     XT_BPF_EGRESS_PROG_PATH) +
        "COMMIT\n";

class BandwidthControllerTest : public IptablesBaseTest {
protected:
    BandwidthControllerTest() {
        BandwidthController::iptablesRestoreFunction = fakeExecIptablesRestoreWithOutput;
    }
    BandwidthController mBw;
    TunInterface mTun;

    void SetUp() {
        ASSERT_EQ(0, mTun.init());
    }

    void TearDown() {
        mTun.destroy();
    }

    void expectSetupCommands(const std::string& expectedClean,
                             const std::string& expectedAccounting) {
        std::string expectedList =
            "*filter\n"
            "-S\n"
            "COMMIT\n";

        std::string expectedFlush =
                "*filter\n"
                ":bw_INPUT -\n"
                ":bw_OUTPUT -\n"
                ":bw_FORWARD -\n"
                ":bw_happy_box -\n"
                ":bw_penalty_box -\n"
                ":bw_data_saver -\n"
                ":bw_costly_shared -\n"
                ":bw_global_alert -\n"
                "COMMIT\n"
                "*raw\n"
                ":bw_raw_PREROUTING -\n"
                "COMMIT\n"
                "*mangle\n"
                ":bw_mangle_POSTROUTING -\n"
                "COMMIT\n";

        ExpectedIptablesCommands expected = {{ V4, expectedList }};
        if (expectedClean.size()) {
            expected.push_back({ V4V6, expectedClean });
        }
        expected.push_back({ V4V6, expectedFlush });
        if (expectedAccounting.size()) {
            expected.push_back({ V4V6, expectedAccounting });
        }

        expectIptablesRestoreCommands(expected);
    }

    using IptOp = BandwidthController::IptOp;

    int runIptablesAlertCmd(IptOp a, const char* b, int64_t c) {
        return mBw.runIptablesAlertCmd(a, b, c);
    }

    int setCostlyAlert(const std::string& a, int64_t b, int64_t* c) {
        return mBw.setCostlyAlert(a, b, c);
    }

    int removeCostlyAlert(const std::string& a, int64_t* b) { return mBw.removeCostlyAlert(a, b); }

    void expectUpdateQuota(uint64_t quota) {
        uintptr_t dummy;
        FILE* dummyFile = reinterpret_cast<FILE*>(&dummy);

        EXPECT_CALL(mSyscalls, fopen(_, _)).WillOnce(Return(ByMove(UniqueFile(dummyFile))));
        EXPECT_CALL(mSyscalls, vfprintf(dummyFile, _, _))
            .WillOnce(Invoke([quota](FILE*, const std::string&, va_list ap) {
                EXPECT_EQ(quota, va_arg(ap, uint64_t));
                return 0;
            }));
        EXPECT_CALL(mSyscalls, fclose(dummyFile)).WillOnce(Return(ok));
    }

    void checkBandwithControl(bool useBpf) {
        // Pretend no bw_costly_shared_<iface> rules already exist...
        addIptablesRestoreOutput(
                "-P OUTPUT ACCEPT\n"
                "-N bw_costly_shared\n"
                "-N unrelated\n");

        // ... so none are flushed or deleted.
        std::string expectedClean = "";

        std::string expectedAccounting =
                useBpf ? ACCOUNT_RULES_WITH_BPF : ACCOUNT_RULES_WITHOUT_BPF;
        mBw.setBpfEnabled(useBpf);
        mBw.enableBandwidthControl();
        expectSetupCommands(expectedClean, expectedAccounting);
    }

    StrictMock<android::netdutils::ScopedMockSyscalls> mSyscalls;
};

TEST_F(BandwidthControllerTest, TestSetupIptablesHooks) {
    // Pretend some bw_costly_shared_<iface> rules already exist...
    addIptablesRestoreOutput(
        "-P OUTPUT ACCEPT\n"
        "-N bw_costly_rmnet_data0\n"
        "-N bw_costly_shared\n"
        "-N unrelated\n"
        "-N bw_costly_rmnet_data7\n");

    // ... and expect that they be flushed and deleted.
    std::string expectedCleanCmds =
        "*filter\n"
        ":bw_costly_rmnet_data0 -\n"
        "-X bw_costly_rmnet_data0\n"
        ":bw_costly_rmnet_data7 -\n"
        "-X bw_costly_rmnet_data7\n"
        "COMMIT\n";

    mBw.setupIptablesHooks();
    expectSetupCommands(expectedCleanCmds, "");
}

TEST_F(BandwidthControllerTest, TestCheckUidBillingMask) {
    uint32_t uidBillingMask = Fwmark::getUidBillingMask();

    // If mask is non-zero, and mask & mask-1 is equal to 0, then the mask is a power of two.
    bool isPowerOfTwo = uidBillingMask && (uidBillingMask & (uidBillingMask - 1)) == 0;

    // Must be exactly a power of two
    EXPECT_TRUE(isPowerOfTwo);
}

TEST_F(BandwidthControllerTest, TestEnableBandwidthControlWithBpf) {
    checkBandwithControl(true);
}

TEST_F(BandwidthControllerTest, TestEnableBandwidthControlWithoutBpf) {
    checkBandwithControl(false);
}

TEST_F(BandwidthControllerTest, TestDisableBandwidthControl) {
    // Pretend some bw_costly_shared_<iface> rules already exist...
    addIptablesRestoreOutput(
        "-P OUTPUT ACCEPT\n"
        "-N bw_costly_rmnet_data0\n"
        "-N bw_costly_shared\n"
        "-N unrelated\n"
        "-N bw_costly_rmnet_data7\n");

    // ... and expect that they be flushed.
    std::string expectedCleanCmds =
        "*filter\n"
        ":bw_costly_rmnet_data0 -\n"
        ":bw_costly_rmnet_data7 -\n"
        "COMMIT\n";

    mBw.disableBandwidthControl();
    expectSetupCommands(expectedCleanCmds, "");
}

TEST_F(BandwidthControllerTest, TestEnableDataSaver) {
    mBw.enableDataSaver(true);
    std::string expected4 =
        "*filter\n"
        ":bw_data_saver -\n"
        "-A bw_data_saver --jump REJECT\n"
        "COMMIT\n";
    std::string expected6 =
        "*filter\n"
        ":bw_data_saver -\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type packet-too-big -j RETURN\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type router-solicitation -j RETURN\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type router-advertisement -j RETURN\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type neighbour-solicitation -j RETURN\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type neighbour-advertisement -j RETURN\n"
        "-A bw_data_saver -p icmpv6 --icmpv6-type redirect -j RETURN\n"
        "-A bw_data_saver --jump REJECT\n"
        "COMMIT\n";
    expectIptablesRestoreCommands({
        {V4, expected4},
        {V6, expected6},
    });

    mBw.enableDataSaver(false);
    std::string expected = {
        "*filter\n"
        ":bw_data_saver -\n"
        "-A bw_data_saver --jump RETURN\n"
        "COMMIT\n"
    };
    expectIptablesRestoreCommands({
        {V4, expected},
        {V6, expected},
    });
}

const std::vector<std::string> makeInterfaceQuotaCommands(const std::string& iface, int ruleIndex,
                                                          int64_t quota) {
    const std::string chain = "bw_costly_" + iface;
    const char* c_chain = chain.c_str();
    const char* c_iface = iface.c_str();
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf(":%s -", c_chain),
        StringPrintf("-A %s -j bw_penalty_box", c_chain),
        StringPrintf("-I bw_INPUT %d -i %s --jump %s", ruleIndex, c_iface, c_chain),
        StringPrintf("-I bw_OUTPUT %d -o %s --jump %s", ruleIndex, c_iface, c_chain),
        StringPrintf("-A bw_FORWARD -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-A bw_FORWARD -o %s --jump %s", c_iface, c_chain),
        StringPrintf("-A %s -m quota2 ! --quota %" PRIu64 " --name %s --jump REJECT", c_chain,
                     quota, c_iface),
        "COMMIT\n",
    };
    return {Join(cmds, "\n")};
}

const std::vector<std::string> removeInterfaceQuotaCommands(const std::string& iface) {
    const std::string chain = "bw_costly_" + iface;
    const char* c_chain = chain.c_str();
    const char* c_iface = iface.c_str();
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf("-D bw_INPUT -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_OUTPUT -o %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_FORWARD -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_FORWARD -o %s --jump %s", c_iface, c_chain),
        StringPrintf("-F %s", c_chain),
        StringPrintf("-X %s", c_chain),
        "COMMIT\n",
    };
    return {Join(cmds, "\n")};
}

TEST_F(BandwidthControllerTest, TestSetInterfaceQuota) {
    constexpr uint64_t kOldQuota = 123456;
    const std::string iface = mTun.name();
    std::vector<std::string> expected = makeInterfaceQuotaCommands(iface, 1, kOldQuota);

    EXPECT_EQ(0, mBw.setInterfaceQuota(iface, kOldQuota));
    expectIptablesRestoreCommands(expected);

    constexpr uint64_t kNewQuota = kOldQuota + 1;
    expected = {};
    expectUpdateQuota(kNewQuota);
    EXPECT_EQ(0, mBw.setInterfaceQuota(iface, kNewQuota));
    expectIptablesRestoreCommands(expected);

    expected = removeInterfaceQuotaCommands(iface);
    EXPECT_EQ(0, mBw.removeInterfaceQuota(iface));
    expectIptablesRestoreCommands(expected);
}

const std::vector<std::string> makeInterfaceSharedQuotaCommands(const std::string& iface,
                                                                int ruleIndex, int64_t quota,
                                                                bool insertQuota) {
    const std::string chain = "bw_costly_shared";
    const char* c_chain = chain.c_str();
    const char* c_iface = iface.c_str();
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf("-I bw_INPUT %d -i %s --jump %s", ruleIndex, c_iface, c_chain),
        StringPrintf("-I bw_OUTPUT %d -o %s --jump %s", ruleIndex, c_iface, c_chain),
        StringPrintf("-A bw_FORWARD -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-A bw_FORWARD -o %s --jump %s", c_iface, c_chain),
    };
    if (insertQuota) {
        cmds.push_back(StringPrintf(
            "-I %s -m quota2 ! --quota %" PRIu64 " --name shared --jump REJECT", c_chain, quota));
    }
    cmds.push_back("COMMIT\n");
    return {Join(cmds, "\n")};
}

const std::vector<std::string> removeInterfaceSharedQuotaCommands(const std::string& iface,
                                                                  int64_t quota, bool deleteQuota) {
    const std::string chain = "bw_costly_shared";
    const char* c_chain = chain.c_str();
    const char* c_iface = iface.c_str();
    std::vector<std::string> cmds = {
        "*filter",
        StringPrintf("-D bw_INPUT -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_OUTPUT -o %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_FORWARD -i %s --jump %s", c_iface, c_chain),
        StringPrintf("-D bw_FORWARD -o %s --jump %s", c_iface, c_chain),
    };
    if (deleteQuota) {
        cmds.push_back(StringPrintf(
            "-D %s -m quota2 ! --quota %" PRIu64 " --name shared --jump REJECT", c_chain, quota));
    }
    cmds.push_back("COMMIT\n");
    return {Join(cmds, "\n")};
}

TEST_F(BandwidthControllerTest, TestSetInterfaceSharedQuotaDuplicate) {
    constexpr uint64_t kQuota = 123456;
    const std::string iface = mTun.name();
    std::vector<std::string> expected = makeInterfaceSharedQuotaCommands(iface, 1, 123456, true);
    EXPECT_EQ(0, mBw.setInterfaceSharedQuota(iface, kQuota));
    expectIptablesRestoreCommands(expected);

    expected = {};
    EXPECT_EQ(0, mBw.setInterfaceSharedQuota(iface, kQuota));
    expectIptablesRestoreCommands(expected);

    expected = removeInterfaceSharedQuotaCommands(iface, kQuota, true);
    EXPECT_EQ(0, mBw.removeInterfaceSharedQuota(iface));
    expectIptablesRestoreCommands(expected);
}

TEST_F(BandwidthControllerTest, TestSetInterfaceSharedQuotaUpdate) {
    constexpr uint64_t kOldQuota = 123456;
    const std::string iface = mTun.name();
    std::vector<std::string> expected = makeInterfaceSharedQuotaCommands(iface, 1, kOldQuota, true);
    EXPECT_EQ(0, mBw.setInterfaceSharedQuota(iface, kOldQuota));
    expectIptablesRestoreCommands(expected);

    constexpr uint64_t kNewQuota = kOldQuota + 1;
    expected = {};
    expectUpdateQuota(kNewQuota);
    EXPECT_EQ(0, mBw.setInterfaceSharedQuota(iface, kNewQuota));
    expectIptablesRestoreCommands(expected);

    expected = removeInterfaceSharedQuotaCommands(iface, kNewQuota, true);
    EXPECT_EQ(0, mBw.removeInterfaceSharedQuota(iface));
    expectIptablesRestoreCommands(expected);
}

TEST_F(BandwidthControllerTest, TestSetInterfaceSharedQuotaTwoInterfaces) {
    constexpr uint64_t kQuota = 123456;
    const std::vector<std::string> ifaces{
        {"a" + mTun.name()},
        {"b" + mTun.name()},
    };

    for (const auto& iface : ifaces) {
        // Quota rule is only added when the total number of
        // interfaces transitions from 0 -> 1.
        bool first = (iface == ifaces[0]);
        auto expected = makeInterfaceSharedQuotaCommands(iface, 1, kQuota, first);
        EXPECT_EQ(0, mBw.setInterfaceSharedQuota(iface, kQuota));
        expectIptablesRestoreCommands(expected);
    }

    for (const auto& iface : ifaces) {
        // Quota rule is only removed when the total number of
        // interfaces transitions from 1 -> 0.
        bool last = (iface == ifaces[1]);
        auto expected = removeInterfaceSharedQuotaCommands(iface, kQuota, last);
        EXPECT_EQ(0, mBw.removeInterfaceSharedQuota(iface));
        expectIptablesRestoreCommands(expected);
    }
}

TEST_F(BandwidthControllerTest, IptablesAlertCmd) {
    std::vector<std::string> expected = {
            "*filter\n"
            "-I bw_global_alert -m quota2 ! --quota 123456 --name MyWonderfulAlert\n"
            "COMMIT\n"};
    EXPECT_EQ(0, runIptablesAlertCmd(IptOp::IptOpInsert, "MyWonderfulAlert", 123456));
    expectIptablesRestoreCommands(expected);

    expected = {
            "*filter\n"
            "-D bw_global_alert -m quota2 ! --quota 123456 --name MyWonderfulAlert\n"
            "COMMIT\n"};
    EXPECT_EQ(0, runIptablesAlertCmd(IptOp::IptOpDelete, "MyWonderfulAlert", 123456));
    expectIptablesRestoreCommands(expected);
}

TEST_F(BandwidthControllerTest, CostlyAlert) {
    const int64_t kQuota = 123456;
    int64_t alertBytes = 0;

    std::vector<std::string> expected = {
        "*filter\n"
        "-A bw_costly_shared -m quota2 ! --quota 123456 --name sharedAlert\n"
        "COMMIT\n"
    };
    EXPECT_EQ(0, setCostlyAlert("shared", kQuota, &alertBytes));
    EXPECT_EQ(kQuota, alertBytes);
    expectIptablesRestoreCommands(expected);

    expected = {};
    expectUpdateQuota(kQuota);
    EXPECT_EQ(0, setCostlyAlert("shared", kQuota + 1, &alertBytes));
    EXPECT_EQ(kQuota + 1, alertBytes);
    expectIptablesRestoreCommands(expected);

    expected = {
        "*filter\n"
        "-D bw_costly_shared -m quota2 ! --quota 123457 --name sharedAlert\n"
        "COMMIT\n"
    };
    EXPECT_EQ(0, removeCostlyAlert("shared", &alertBytes));
    EXPECT_EQ(0, alertBytes);
    expectIptablesRestoreCommands(expected);
}

TEST_F(BandwidthControllerTest, ManipulateSpecialApps) {
    std::vector<const char *> appUids = { "1000", "1001", "10012" };

    std::vector<std::string> expected = {
        "*filter\n"
        "-I bw_happy_box -m owner --uid-owner 1000 --jump RETURN\n"
        "-I bw_happy_box -m owner --uid-owner 1001 --jump RETURN\n"
        "-I bw_happy_box -m owner --uid-owner 10012 --jump RETURN\n"
        "COMMIT\n"
    };
    EXPECT_EQ(0, mBw.addNiceApps(appUids.size(), const_cast<char**>(&appUids[0])));
    expectIptablesRestoreCommands(expected);

    expected = {
        "*filter\n"
        "-D bw_penalty_box -m owner --uid-owner 1000 --jump REJECT\n"
        "-D bw_penalty_box -m owner --uid-owner 1001 --jump REJECT\n"
        "-D bw_penalty_box -m owner --uid-owner 10012 --jump REJECT\n"
        "COMMIT\n"
    };
    EXPECT_EQ(0, mBw.removeNaughtyApps(appUids.size(), const_cast<char**>(&appUids[0])));
    expectIptablesRestoreCommands(expected);
}
