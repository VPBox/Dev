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
 * binder_test.cpp - unit tests for netd binder RPCs.
 */

#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <set>
#include <vector>

#include <dirent.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/base64.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <android-base/file.h>
#include <android-base/macros.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <binder/IPCThreadState.h>
#include <bpf/BpfMap.h>
#include <bpf/BpfUtils.h>
#include <com/android/internal/net/BnOemNetdUnsolicitedEventListener.h>
#include <com/android/internal/net/IOemNetd.h>
#include <cutils/multiuser.h>
#include <gtest/gtest.h>
#include <logwrap/logwrap.h>
#include <netdbpf/bpf_shared.h>
#include <netutils/ifc.h>
#include "Fwmark.h"
#include "InterfaceController.h"
#include "NetdClient.h"
#include "NetdConstants.h"
#include "TestUnsolService.h"
#include "XfrmController.h"
#include "android/net/INetd.h"
#include "binder/IServiceManager.h"
#include "netdutils/Stopwatch.h"
#include "netdutils/Syscalls.h"
#include "netid_client.h"  // NETID_UNSET
#include "tun_interface.h"

#define IP_PATH "/system/bin/ip"
#define IP6TABLES_PATH "/system/bin/ip6tables"
#define IPTABLES_PATH "/system/bin/iptables"
#define TUN_DEV "/dev/tun"
#define RAW_TABLE "raw"
#define MANGLE_TABLE "mangle"
#define FILTER_TABLE "filter"
#define NAT_TABLE "nat"

namespace binder = android::binder;

using android::IBinder;
using android::IServiceManager;
using android::sp;
using android::String16;
using android::String8;
using android::base::Join;
using android::base::ReadFileToString;
using android::base::StartsWith;
using android::base::StringPrintf;
using android::base::Trim;
using android::net::INetd;
using android::net::InterfaceConfigurationParcel;
using android::net::InterfaceController;
using android::net::TetherStatsParcel;
using android::net::TunInterface;
using android::net::UidRangeParcel;
using android::netdutils::sSyscalls;
using android::netdutils::Stopwatch;

static const char* IP_RULE_V4 = "-4";
static const char* IP_RULE_V6 = "-6";
static const int TEST_NETID1 = 65501;
static const int TEST_NETID2 = 65502;

// Use maximum reserved appId for applications to avoid conflict with existing
// uids.
static const int TEST_UID1 = 99999;
static const int TEST_UID2 = 99998;

constexpr int BASE_UID = AID_USER_OFFSET * 5;

static const std::string NO_SOCKET_ALLOW_RULE("! owner UID match 0-4294967294");
static const std::string ESP_ALLOW_RULE("esp");

class BinderTest : public ::testing::Test {
  public:
    BinderTest() {
        sp<IServiceManager> sm = android::defaultServiceManager();
        sp<IBinder> binder = sm->getService(String16("netd"));
        if (binder != nullptr) {
            mNetd = android::interface_cast<INetd>(binder);
        }
    }

    void SetUp() override {
        ASSERT_NE(nullptr, mNetd.get());
    }

    void TearDown() override {
        mNetd->networkDestroy(TEST_NETID1);
        mNetd->networkDestroy(TEST_NETID2);
        setNetworkForProcess(NETID_UNSET);
        // Restore default network
        if (mStoredDefaultNetwork >= 0) mNetd->networkSetDefault(mStoredDefaultNetwork);
    }

    bool allocateIpSecResources(bool expectOk, int32_t* spi);

    // Static because setting up the tun interface takes about 40ms.
    static void SetUpTestCase() {
        ASSERT_EQ(0, sTun.init());
        ASSERT_EQ(0, sTun2.init());
        ASSERT_LE(sTun.name().size(), static_cast<size_t>(IFNAMSIZ));
        ASSERT_LE(sTun2.name().size(), static_cast<size_t>(IFNAMSIZ));
    }

    static void TearDownTestCase() {
        // Closing the socket removes the interface and IP addresses.
        sTun.destroy();
        sTun2.destroy();
    }

    static void fakeRemoteSocketPair(int *clientSocket, int *serverSocket, int *acceptedSocket);

    void createVpnNetworkWithUid(bool secure, uid_t uid, int vpnNetId = TEST_NETID2,
                                 int fallthroughNetId = TEST_NETID1);

  protected:
    // Use -1 to represent that default network was not modified because
    // real netId must be an unsigned value.
    int mStoredDefaultNetwork = -1;
    sp<INetd> mNetd;
    static TunInterface sTun;
    static TunInterface sTun2;
};

TunInterface BinderTest::sTun;
TunInterface BinderTest::sTun2;

class TimedOperation : public Stopwatch {
  public:
    explicit TimedOperation(const std::string &name): mName(name) {}
    virtual ~TimedOperation() {
        fprintf(stderr, "    %s: %6.1f ms\n", mName.c_str(), timeTaken());
    }

  private:
    std::string mName;
};

TEST_F(BinderTest, IsAlive) {
    TimedOperation t("isAlive RPC");
    bool isAlive = false;
    mNetd->isAlive(&isAlive);
    ASSERT_TRUE(isAlive);
}

static int randomUid() {
    return 100000 * arc4random_uniform(7) + 10000 + arc4random_uniform(5000);
}

static std::vector<std::string> runCommand(const std::string& command) {
    std::vector<std::string> lines;
    FILE *f = popen(command.c_str(), "r");  // NOLINT(cert-env33-c)
    if (f == nullptr) {
        perror("popen");
        return lines;
    }

    char *line = nullptr;
    size_t bufsize = 0;
    ssize_t linelen = 0;
    while ((linelen = getline(&line, &bufsize, f)) >= 0) {
        lines.push_back(std::string(line, linelen));
        free(line);
        line = nullptr;
    }

    pclose(f);
    return lines;
}

static std::vector<std::string> listIpRules(const char *ipVersion) {
    std::string command = StringPrintf("%s %s rule list", IP_PATH, ipVersion);
    return runCommand(command);
}

static std::vector<std::string> listIptablesRule(const char *binary, const char *chainName) {
    std::string command = StringPrintf("%s -w -n -L %s", binary, chainName);
    return runCommand(command);
}

static int iptablesRuleLineLength(const char *binary, const char *chainName) {
    return listIptablesRule(binary, chainName).size();
}

static bool iptablesRuleExists(const char *binary,
                               const char *chainName,
                               const std::string& expectedRule) {
    std::vector<std::string> rules = listIptablesRule(binary, chainName);
    for (const auto& rule : rules) {
        if(rule.find(expectedRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

static bool iptablesNoSocketAllowRuleExists(const char *chainName){
    return iptablesRuleExists(IPTABLES_PATH, chainName, NO_SOCKET_ALLOW_RULE) &&
           iptablesRuleExists(IP6TABLES_PATH, chainName, NO_SOCKET_ALLOW_RULE);
}

static bool iptablesEspAllowRuleExists(const char *chainName){
    return iptablesRuleExists(IPTABLES_PATH, chainName, ESP_ALLOW_RULE) &&
           iptablesRuleExists(IP6TABLES_PATH, chainName, ESP_ALLOW_RULE);
}

TEST_F(BinderTest, FirewallReplaceUidChain) {
    SKIP_IF_BPF_SUPPORTED;

    std::string chainName = StringPrintf("netd_binder_test_%u", arc4random_uniform(10000));
    const int kNumUids = 500;
    std::vector<int32_t> noUids(0);
    std::vector<int32_t> uids(kNumUids);
    for (int i = 0; i < kNumUids; i++) {
        uids[i] = randomUid();
    }

    bool ret;
    {
        TimedOperation op(StringPrintf("Programming %d-UID whitelist chain", kNumUids));
        mNetd->firewallReplaceUidChain(chainName, true, uids, &ret);
    }
    EXPECT_EQ(true, ret);
    EXPECT_EQ((int) uids.size() + 9, iptablesRuleLineLength(IPTABLES_PATH, chainName.c_str()));
    EXPECT_EQ((int) uids.size() + 15, iptablesRuleLineLength(IP6TABLES_PATH, chainName.c_str()));
    EXPECT_EQ(true, iptablesNoSocketAllowRuleExists(chainName.c_str()));
    EXPECT_EQ(true, iptablesEspAllowRuleExists(chainName.c_str()));
    {
        TimedOperation op("Clearing whitelist chain");
        mNetd->firewallReplaceUidChain(chainName, false, noUids, &ret);
    }
    EXPECT_EQ(true, ret);
    EXPECT_EQ(5, iptablesRuleLineLength(IPTABLES_PATH, chainName.c_str()));
    EXPECT_EQ(5, iptablesRuleLineLength(IP6TABLES_PATH, chainName.c_str()));

    {
        TimedOperation op(StringPrintf("Programming %d-UID blacklist chain", kNumUids));
        mNetd->firewallReplaceUidChain(chainName, false, uids, &ret);
    }
    EXPECT_EQ(true, ret);
    EXPECT_EQ((int) uids.size() + 5, iptablesRuleLineLength(IPTABLES_PATH, chainName.c_str()));
    EXPECT_EQ((int) uids.size() + 5, iptablesRuleLineLength(IP6TABLES_PATH, chainName.c_str()));
    EXPECT_EQ(false, iptablesNoSocketAllowRuleExists(chainName.c_str()));
    EXPECT_EQ(false, iptablesEspAllowRuleExists(chainName.c_str()));

    {
        TimedOperation op("Clearing blacklist chain");
        mNetd->firewallReplaceUidChain(chainName, false, noUids, &ret);
    }
    EXPECT_EQ(true, ret);
    EXPECT_EQ(5, iptablesRuleLineLength(IPTABLES_PATH, chainName.c_str()));
    EXPECT_EQ(5, iptablesRuleLineLength(IP6TABLES_PATH, chainName.c_str()));

    // Check that the call fails if iptables returns an error.
    std::string veryLongStringName = "netd_binder_test_UnacceptablyLongIptablesChainName";
    mNetd->firewallReplaceUidChain(veryLongStringName, true, noUids, &ret);
    EXPECT_EQ(false, ret);
}

TEST_F(BinderTest, IpSecTunnelInterface) {
    const struct TestData {
        const std::string family;
        const std::string deviceName;
        const std::string localAddress;
        const std::string remoteAddress;
        int32_t iKey;
        int32_t oKey;
        int32_t ifId;
    } kTestData[] = {
            {"IPV4", "ipsec_test", "127.0.0.1", "8.8.8.8", 0x1234 + 53, 0x1234 + 53, 0xFFFE},
            {"IPV6", "ipsec_test6", "::1", "2001:4860:4860::8888", 0x1234 + 50, 0x1234 + 50,
             0xFFFE},
    };

    for (size_t i = 0; i < std::size(kTestData); i++) {
        const auto& td = kTestData[i];

        binder::Status status;

        // Create Tunnel Interface.
        status = mNetd->ipSecAddTunnelInterface(td.deviceName, td.localAddress, td.remoteAddress,
                                                td.iKey, td.oKey, td.ifId);
        EXPECT_TRUE(status.isOk()) << td.family << status.exceptionMessage();

        // Check that the interface exists
        EXPECT_NE(0U, if_nametoindex(td.deviceName.c_str()));

        // Update Tunnel Interface.
        status = mNetd->ipSecUpdateTunnelInterface(td.deviceName, td.localAddress, td.remoteAddress,
                                                   td.iKey, td.oKey, td.ifId);
        EXPECT_TRUE(status.isOk()) << td.family << status.exceptionMessage();

        // Remove Tunnel Interface.
        status = mNetd->ipSecRemoveTunnelInterface(td.deviceName);
        EXPECT_TRUE(status.isOk()) << td.family << status.exceptionMessage();

        // Check that the interface no longer exists
        EXPECT_EQ(0U, if_nametoindex(td.deviceName.c_str()));
    }
}

TEST_F(BinderTest, IpSecSetEncapSocketOwner) {
    android::base::unique_fd uniqueFd(socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0));
    android::os::ParcelFileDescriptor sockFd(std::move(uniqueFd));

    int sockOptVal = UDP_ENCAP_ESPINUDP;
    setsockopt(sockFd.get(), IPPROTO_UDP, UDP_ENCAP, &sockOptVal, sizeof(sockOptVal));

    binder::Status res = mNetd->ipSecSetEncapSocketOwner(sockFd, 1001);
    EXPECT_TRUE(res.isOk());

    struct stat info;
    EXPECT_EQ(0, fstat(sockFd.get(), &info));
    EXPECT_EQ(1001, (int) info.st_uid);
}

// IPsec tests are not run in 32 bit mode; both 32-bit kernels and
// mismatched ABIs (64-bit kernel with 32-bit userspace) are unsupported.
#if INTPTR_MAX != INT32_MAX

using android::net::XfrmController;

static const int XFRM_DIRECTIONS[] = {static_cast<int>(android::net::XfrmDirection::IN),
                                      static_cast<int>(android::net::XfrmDirection::OUT)};
static const int ADDRESS_FAMILIES[] = {AF_INET, AF_INET6};

#define RETURN_FALSE_IF_NEQ(_expect_, _ret_) \
        do { if ((_expect_) != (_ret_)) return false; } while(false)
bool BinderTest::allocateIpSecResources(bool expectOk, int32_t* spi) {
    android::netdutils::Status status = XfrmController::ipSecAllocateSpi(0, "::", "::1", 123, spi);
    SCOPED_TRACE(status);
    RETURN_FALSE_IF_NEQ(status.ok(), expectOk);

    // Add a policy
    status = XfrmController::ipSecAddSecurityPolicy(0, AF_INET6, 0, "::", "::1", 123, 0, 0, 0);
    SCOPED_TRACE(status);
    RETURN_FALSE_IF_NEQ(status.ok(), expectOk);

    // Add an ipsec interface
    return expectOk == XfrmController::ipSecAddTunnelInterface("ipsec_test", "::", "::1", 0xF00D,
                                                               0xD00D, 0xE00D, false)
                               .ok();
}

TEST_F(BinderTest, XfrmDualSelectorTunnelModePoliciesV4) {
    android::binder::Status status;

    // Repeat to ensure cleanup and recreation works correctly
    for (int i = 0; i < 2; i++) {
        for (int direction : XFRM_DIRECTIONS) {
            for (int addrFamily : ADDRESS_FAMILIES) {
                status = mNetd->ipSecAddSecurityPolicy(0, addrFamily, direction, "127.0.0.5",
                                                       "127.0.0.6", 123, 0, 0, 0);
                EXPECT_TRUE(status.isOk())
                        << " family: " << addrFamily << " direction: " << direction;
            }
        }

        // Cleanup
        for (int direction : XFRM_DIRECTIONS) {
            for (int addrFamily : ADDRESS_FAMILIES) {
                status = mNetd->ipSecDeleteSecurityPolicy(0, addrFamily, direction, 0, 0, 0);
                EXPECT_TRUE(status.isOk());
            }
        }
    }
}

TEST_F(BinderTest, XfrmDualSelectorTunnelModePoliciesV6) {
    binder::Status status;

    // Repeat to ensure cleanup and recreation works correctly
    for (int i = 0; i < 2; i++) {
        for (int direction : XFRM_DIRECTIONS) {
            for (int addrFamily : ADDRESS_FAMILIES) {
                status = mNetd->ipSecAddSecurityPolicy(0, addrFamily, direction, "2001:db8::f00d",
                                                       "2001:db8::d00d", 123, 0, 0, 0);
                EXPECT_TRUE(status.isOk())
                        << " family: " << addrFamily << " direction: " << direction;
            }
        }

        // Cleanup
        for (int direction : XFRM_DIRECTIONS) {
            for (int addrFamily : ADDRESS_FAMILIES) {
                status = mNetd->ipSecDeleteSecurityPolicy(0, addrFamily, direction, 0, 0, 0);
                EXPECT_TRUE(status.isOk());
            }
        }
    }
}

TEST_F(BinderTest, XfrmControllerInit) {
    android::netdutils::Status status;
    status = XfrmController::Init();
    SCOPED_TRACE(status);

    // Older devices or devices with mismatched Kernel/User ABI cannot support the IPsec
    // feature.
    if (status.code() == EOPNOTSUPP) return;

    ASSERT_TRUE(status.ok());

    int32_t spi = 0;

    ASSERT_TRUE(allocateIpSecResources(true, &spi));
    ASSERT_TRUE(allocateIpSecResources(false, &spi));

    status = XfrmController::Init();
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(allocateIpSecResources(true, &spi));

    // Clean up
    status = XfrmController::ipSecDeleteSecurityAssociation(0, "::", "::1", 123, spi, 0, 0);
    SCOPED_TRACE(status);
    ASSERT_TRUE(status.ok());

    status = XfrmController::ipSecDeleteSecurityPolicy(0, AF_INET6, 0, 0, 0, 0);
    SCOPED_TRACE(status);
    ASSERT_TRUE(status.ok());

    // Remove Virtual Tunnel Interface.
    ASSERT_TRUE(XfrmController::ipSecRemoveTunnelInterface("ipsec_test").ok());
}

#endif  // INTPTR_MAX != INT32_MAX

static int bandwidthDataSaverEnabled(const char *binary) {
    std::vector<std::string> lines = listIptablesRule(binary, "bw_data_saver");

    // Output looks like this:
    //
    // Chain bw_data_saver (1 references)
    // target     prot opt source               destination
    // RETURN     all  --  0.0.0.0/0            0.0.0.0/0
    //
    // or:
    //
    // Chain bw_data_saver (1 references)
    // target     prot opt source               destination
    // ... possibly connectivity critical packet rules here ...
    // REJECT     all  --  ::/0            ::/0

    EXPECT_GE(lines.size(), 3U);

    if (lines.size() == 3 && StartsWith(lines[2], "RETURN ")) {
        // Data saver disabled.
        return 0;
    }

    size_t minSize = (std::string(binary) == IPTABLES_PATH) ? 3 : 9;

    if (lines.size() >= minSize && StartsWith(lines[lines.size() -1], "REJECT ")) {
        // Data saver enabled.
        return 1;
    }

    return -1;
}

bool enableDataSaver(sp<INetd>& netd, bool enable) {
    TimedOperation op(enable ? " Enabling data saver" : "Disabling data saver");
    bool ret;
    netd->bandwidthEnableDataSaver(enable, &ret);
    return ret;
}

int getDataSaverState() {
    const int enabled4 = bandwidthDataSaverEnabled(IPTABLES_PATH);
    const int enabled6 = bandwidthDataSaverEnabled(IP6TABLES_PATH);
    EXPECT_EQ(enabled4, enabled6);
    EXPECT_NE(-1, enabled4);
    EXPECT_NE(-1, enabled6);
    if (enabled4 != enabled6 || (enabled6 != 0 && enabled6 != 1)) {
        return -1;
    }
    return enabled6;
}

TEST_F(BinderTest, BandwidthEnableDataSaver) {
    const int wasEnabled = getDataSaverState();
    ASSERT_NE(-1, wasEnabled);

    if (wasEnabled) {
        ASSERT_TRUE(enableDataSaver(mNetd, false));
        EXPECT_EQ(0, getDataSaverState());
    }

    ASSERT_TRUE(enableDataSaver(mNetd, false));
    EXPECT_EQ(0, getDataSaverState());

    ASSERT_TRUE(enableDataSaver(mNetd, true));
    EXPECT_EQ(1, getDataSaverState());

    ASSERT_TRUE(enableDataSaver(mNetd, true));
    EXPECT_EQ(1, getDataSaverState());

    if (!wasEnabled) {
        ASSERT_TRUE(enableDataSaver(mNetd, false));
        EXPECT_EQ(0, getDataSaverState());
    }
}

static bool ipRuleExistsForRange(const uint32_t priority, const UidRangeParcel& range,
                                 const std::string& action, const char* ipVersion) {
    // Output looks like this:
    //   "12500:\tfrom all fwmark 0x0/0x20000 iif lo uidrange 1000-2000 prohibit"
    std::vector<std::string> rules = listIpRules(ipVersion);

    std::string prefix = StringPrintf("%" PRIu32 ":", priority);
    std::string suffix =
            StringPrintf(" iif lo uidrange %d-%d %s\n", range.start, range.stop, action.c_str());
    for (const auto& line : rules) {
        if (android::base::StartsWith(line, prefix) && android::base::EndsWith(line, suffix)) {
            return true;
        }
    }
    return false;
}

static bool ipRuleExistsForRange(const uint32_t priority, const UidRangeParcel& range,
                                 const std::string& action) {
    bool existsIp4 = ipRuleExistsForRange(priority, range, action, IP_RULE_V4);
    bool existsIp6 = ipRuleExistsForRange(priority, range, action, IP_RULE_V6);
    EXPECT_EQ(existsIp4, existsIp6);
    return existsIp4;
}

namespace {

UidRangeParcel makeUidRangeParcel(int start, int stop) {
    UidRangeParcel res;
    res.start = start;
    res.stop = stop;

    return res;
}

}  // namespace

TEST_F(BinderTest, NetworkInterfaces) {
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_EQ(EEXIST, mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE)
                              .serviceSpecificErrorCode());
    EXPECT_EQ(EEXIST, mNetd->networkCreateVpn(TEST_NETID1, true).serviceSpecificErrorCode());
    EXPECT_TRUE(mNetd->networkCreateVpn(TEST_NETID2, true).isOk());

    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());
    EXPECT_EQ(EBUSY,
              mNetd->networkAddInterface(TEST_NETID2, sTun.name()).serviceSpecificErrorCode());

    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID2, sTun.name()).isOk());
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID2).isOk());
    EXPECT_EQ(ENONET, mNetd->networkDestroy(TEST_NETID1).serviceSpecificErrorCode());
}

TEST_F(BinderTest, NetworkUidRules) {
    const uint32_t RULE_PRIORITY_SECURE_VPN = 12000;

    EXPECT_TRUE(mNetd->networkCreateVpn(TEST_NETID1, true).isOk());
    EXPECT_EQ(EEXIST, mNetd->networkCreateVpn(TEST_NETID1, true).serviceSpecificErrorCode());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    std::vector<UidRangeParcel> uidRanges = {makeUidRangeParcel(BASE_UID + 8005, BASE_UID + 8012),
                                             makeUidRangeParcel(BASE_UID + 8090, BASE_UID + 8099)};
    UidRangeParcel otherRange = makeUidRangeParcel(BASE_UID + 8190, BASE_UID + 8299);
    std::string suffix = StringPrintf("lookup %s ", sTun.name().c_str());

    EXPECT_TRUE(mNetd->networkAddUidRanges(TEST_NETID1, uidRanges).isOk());

    EXPECT_TRUE(ipRuleExistsForRange(RULE_PRIORITY_SECURE_VPN, uidRanges[0], suffix));
    EXPECT_FALSE(ipRuleExistsForRange(RULE_PRIORITY_SECURE_VPN, otherRange, suffix));
    EXPECT_TRUE(mNetd->networkRemoveUidRanges(TEST_NETID1, uidRanges).isOk());
    EXPECT_FALSE(ipRuleExistsForRange(RULE_PRIORITY_SECURE_VPN, uidRanges[0], suffix));

    EXPECT_TRUE(mNetd->networkAddUidRanges(TEST_NETID1, uidRanges).isOk());
    EXPECT_TRUE(ipRuleExistsForRange(RULE_PRIORITY_SECURE_VPN, uidRanges[1], suffix));
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
    EXPECT_FALSE(ipRuleExistsForRange(RULE_PRIORITY_SECURE_VPN, uidRanges[1], suffix));

    EXPECT_EQ(ENONET, mNetd->networkDestroy(TEST_NETID1).serviceSpecificErrorCode());
}

TEST_F(BinderTest, NetworkRejectNonSecureVpn) {
    constexpr uint32_t RULE_PRIORITY = 12500;

    std::vector<UidRangeParcel> uidRanges = {makeUidRangeParcel(BASE_UID + 150, BASE_UID + 224),
                                             makeUidRangeParcel(BASE_UID + 226, BASE_UID + 300)};

    const std::vector<std::string> initialRulesV4 = listIpRules(IP_RULE_V4);
    const std::vector<std::string> initialRulesV6 = listIpRules(IP_RULE_V6);

    // Create two valid rules.
    ASSERT_TRUE(mNetd->networkRejectNonSecureVpn(true, uidRanges).isOk());
    EXPECT_EQ(initialRulesV4.size() + 2, listIpRules(IP_RULE_V4).size());
    EXPECT_EQ(initialRulesV6.size() + 2, listIpRules(IP_RULE_V6).size());
    for (auto const& range : uidRanges) {
        EXPECT_TRUE(ipRuleExistsForRange(RULE_PRIORITY, range, "prohibit"));
    }

    // Remove the rules.
    ASSERT_TRUE(mNetd->networkRejectNonSecureVpn(false, uidRanges).isOk());
    EXPECT_EQ(initialRulesV4.size(), listIpRules(IP_RULE_V4).size());
    EXPECT_EQ(initialRulesV6.size(), listIpRules(IP_RULE_V6).size());
    for (auto const& range : uidRanges) {
        EXPECT_FALSE(ipRuleExistsForRange(RULE_PRIORITY, range, "prohibit"));
    }

    // Fail to remove the rules a second time after they are already deleted.
    binder::Status status = mNetd->networkRejectNonSecureVpn(false, uidRanges);
    ASSERT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
    EXPECT_EQ(ENOENT, status.serviceSpecificErrorCode());

    // All rules should be the same as before.
    EXPECT_EQ(initialRulesV4, listIpRules(IP_RULE_V4));
    EXPECT_EQ(initialRulesV6, listIpRules(IP_RULE_V6));
}

// Create a socket pair that isLoopbackSocket won't think is local.
void BinderTest::fakeRemoteSocketPair(int *clientSocket, int *serverSocket, int *acceptedSocket) {
    *serverSocket = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
    struct sockaddr_in6 server6 = { .sin6_family = AF_INET6, .sin6_addr = sTun.dstAddr() };
    ASSERT_EQ(0, bind(*serverSocket, (struct sockaddr *) &server6, sizeof(server6)));

    socklen_t addrlen = sizeof(server6);
    ASSERT_EQ(0, getsockname(*serverSocket, (struct sockaddr *) &server6, &addrlen));
    ASSERT_EQ(0, listen(*serverSocket, 10));

    *clientSocket = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
    struct sockaddr_in6 client6 = { .sin6_family = AF_INET6, .sin6_addr = sTun.srcAddr() };
    ASSERT_EQ(0, bind(*clientSocket, (struct sockaddr *) &client6, sizeof(client6)));
    ASSERT_EQ(0, connect(*clientSocket, (struct sockaddr *) &server6, sizeof(server6)));
    ASSERT_EQ(0, getsockname(*clientSocket, (struct sockaddr *) &client6, &addrlen));

    *acceptedSocket = accept4(*serverSocket, (struct sockaddr *) &server6, &addrlen, SOCK_CLOEXEC);
    ASSERT_NE(-1, *acceptedSocket);

    ASSERT_EQ(0, memcmp(&client6, &server6, sizeof(client6)));
}

void checkSocketpairOpen(int clientSocket, int acceptedSocket) {
    char buf[4096];
    EXPECT_EQ(4, write(clientSocket, "foo", sizeof("foo")));
    EXPECT_EQ(4, read(acceptedSocket, buf, sizeof(buf)));
    EXPECT_EQ(0, memcmp(buf, "foo", sizeof("foo")));
}

void checkSocketpairClosed(int clientSocket, int acceptedSocket) {
    // Check that the client socket was closed with ECONNABORTED.
    int ret = write(clientSocket, "foo", sizeof("foo"));
    int err = errno;
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(ECONNABORTED, err);

    // Check that it sent a RST to the server.
    ret = write(acceptedSocket, "foo", sizeof("foo"));
    err = errno;
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(ECONNRESET, err);
}

TEST_F(BinderTest, SocketDestroy) {
    int clientSocket, serverSocket, acceptedSocket;
    ASSERT_NO_FATAL_FAILURE(fakeRemoteSocketPair(&clientSocket, &serverSocket, &acceptedSocket));

    // Pick a random UID in the system UID range.
    constexpr int baseUid = AID_APP - 2000;
    static_assert(baseUid > 0, "Not enough UIDs? Please fix this test.");
    int uid = baseUid + 500 + arc4random_uniform(1000);
    EXPECT_EQ(0, fchown(clientSocket, uid, -1));

    // UID ranges that don't contain uid.
    std::vector<UidRangeParcel> uidRanges = {
            makeUidRangeParcel(baseUid + 42, baseUid + 449),
            makeUidRangeParcel(baseUid + 1536, AID_APP - 4),
            makeUidRangeParcel(baseUid + 498, uid - 1),
            makeUidRangeParcel(uid + 1, baseUid + 1520),
    };
    // A skip list that doesn't contain UID.
    std::vector<int32_t> skipUids { baseUid + 123, baseUid + 1600 };

    // Close sockets. Our test socket should be intact.
    EXPECT_TRUE(mNetd->socketDestroy(uidRanges, skipUids).isOk());
    checkSocketpairOpen(clientSocket, acceptedSocket);

    // UID ranges that do contain uid.
    uidRanges = {
            makeUidRangeParcel(baseUid + 42, baseUid + 449),
            makeUidRangeParcel(baseUid + 1536, AID_APP - 4),
            makeUidRangeParcel(baseUid + 498, baseUid + 1520),
    };
    // Add uid to the skip list.
    skipUids.push_back(uid);

    // Close sockets. Our test socket should still be intact because it's in the skip list.
    EXPECT_TRUE(mNetd->socketDestroy(uidRanges, skipUids).isOk());
    checkSocketpairOpen(clientSocket, acceptedSocket);

    // Now remove uid from skipUids, and close sockets. Our test socket should have been closed.
    skipUids.resize(skipUids.size() - 1);
    EXPECT_TRUE(mNetd->socketDestroy(uidRanges, skipUids).isOk());
    checkSocketpairClosed(clientSocket, acceptedSocket);

    close(clientSocket);
    close(serverSocket);
    close(acceptedSocket);
}

namespace {

int netmaskToPrefixLength(const uint8_t *buf, size_t buflen) {
    if (buf == nullptr) return -1;

    int prefixLength = 0;
    bool endOfContiguousBits = false;
    for (unsigned int i = 0; i < buflen; i++) {
        const uint8_t value = buf[i];

        // Bad bit sequence: check for a contiguous set of bits from the high
        // end by verifying that the inverted value + 1 is a power of 2
        // (power of 2 iff. (v & (v - 1)) == 0).
        const uint8_t inverse = ~value + 1;
        if ((inverse & (inverse - 1)) != 0) return -1;

        prefixLength += (value == 0) ? 0 : CHAR_BIT - ffs(value) + 1;

        // Bogus netmask.
        if (endOfContiguousBits && value != 0) return -1;

        if (value != 0xff) endOfContiguousBits = true;
    }

    return prefixLength;
}

template<typename T>
int netmaskToPrefixLength(const T *p) {
    return netmaskToPrefixLength(reinterpret_cast<const uint8_t*>(p), sizeof(T));
}


static bool interfaceHasAddress(
        const std::string &ifname, const char *addrString, int prefixLength) {
    struct addrinfo *addrinfoList = nullptr;

    const struct addrinfo hints = {
        .ai_flags    = AI_NUMERICHOST,
        .ai_family   = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM,
    };
    if (getaddrinfo(addrString, nullptr, &hints, &addrinfoList) != 0 ||
        addrinfoList == nullptr || addrinfoList->ai_addr == nullptr) {
        return false;
    }
    ScopedAddrinfo addrinfoCleanup(addrinfoList);

    struct ifaddrs *ifaddrsList = nullptr;
    ScopedIfaddrs ifaddrsCleanup(ifaddrsList);

    if (getifaddrs(&ifaddrsList) != 0) {
        return false;
    }

    for (struct ifaddrs *addr = ifaddrsList; addr != nullptr; addr = addr->ifa_next) {
        if (std::string(addr->ifa_name) != ifname ||
            addr->ifa_addr == nullptr ||
            addr->ifa_addr->sa_family != addrinfoList->ai_addr->sa_family) {
            continue;
        }

        switch (addr->ifa_addr->sa_family) {
        case AF_INET: {
            auto *addr4 = reinterpret_cast<const struct sockaddr_in*>(addr->ifa_addr);
            auto *want = reinterpret_cast<const struct sockaddr_in*>(addrinfoList->ai_addr);
            if (memcmp(&addr4->sin_addr, &want->sin_addr, sizeof(want->sin_addr)) != 0) {
                continue;
            }

            if (prefixLength < 0) return true;  // not checking prefix lengths

            if (addr->ifa_netmask == nullptr) return false;
            auto *nm = reinterpret_cast<const struct sockaddr_in*>(addr->ifa_netmask);
            EXPECT_EQ(prefixLength, netmaskToPrefixLength(&nm->sin_addr));
            return (prefixLength == netmaskToPrefixLength(&nm->sin_addr));
        }
        case AF_INET6: {
            auto *addr6 = reinterpret_cast<const struct sockaddr_in6*>(addr->ifa_addr);
            auto *want = reinterpret_cast<const struct sockaddr_in6*>(addrinfoList->ai_addr);
            if (memcmp(&addr6->sin6_addr, &want->sin6_addr, sizeof(want->sin6_addr)) != 0) {
                continue;
            }

            if (prefixLength < 0) return true;  // not checking prefix lengths

            if (addr->ifa_netmask == nullptr) return false;
            auto *nm = reinterpret_cast<const struct sockaddr_in6*>(addr->ifa_netmask);
            EXPECT_EQ(prefixLength, netmaskToPrefixLength(&nm->sin6_addr));
            return (prefixLength == netmaskToPrefixLength(&nm->sin6_addr));
        }
        default:
            // Cannot happen because we have already screened for matching
            // address families at the top of each iteration.
            continue;
        }
    }

    return false;
}

}  // namespace

TEST_F(BinderTest, InterfaceAddRemoveAddress) {
    static const struct TestData {
        const char *addrString;
        const int   prefixLength;
        const bool  expectSuccess;
    } kTestData[] = {
        { "192.0.2.1", 24, true },
        { "192.0.2.2", 25, true },
        { "192.0.2.3", 32, true },
        { "192.0.2.4", 33, false },
        { "192.not.an.ip", 24, false },
        { "2001:db8::1", 64, true },
        { "2001:db8::2", 65, true },
        { "2001:db8::3", 128, true },
        { "2001:db8::4", 129, false },
        { "foo:bar::bad", 64, false },
    };

    for (size_t i = 0; i < std::size(kTestData); i++) {
        const auto &td = kTestData[i];

        // [1.a] Add the address.
        binder::Status status = mNetd->interfaceAddAddress(
                sTun.name(), td.addrString, td.prefixLength);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        } else {
            ASSERT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            ASSERT_NE(0, status.serviceSpecificErrorCode());
        }

        // [1.b] Verify the addition meets the expectation.
        if (td.expectSuccess) {
            EXPECT_TRUE(interfaceHasAddress(sTun.name(), td.addrString, td.prefixLength));
        } else {
            EXPECT_FALSE(interfaceHasAddress(sTun.name(), td.addrString, -1));
        }

        // [2.a] Try to remove the address.  If it was not previously added, removing it fails.
        status = mNetd->interfaceDelAddress(sTun.name(), td.addrString, td.prefixLength);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        } else {
            ASSERT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            ASSERT_NE(0, status.serviceSpecificErrorCode());
        }

        // [2.b] No matter what, the address should not be present.
        EXPECT_FALSE(interfaceHasAddress(sTun.name(), td.addrString, -1));
    }
}

TEST_F(BinderTest, GetProcSysNet) {
    const char LOOPBACK[] = "lo";
    static const struct {
        const int ipversion;
        const int which;
        const char* ifname;
        const char* parameter;
        const char* expectedValue;
        const int expectedReturnCode;
    } kTestData[] = {
            {INetd::IPV4, INetd::CONF, LOOPBACK, "arp_ignore", "0", 0},
            {-1, INetd::CONF, sTun.name().c_str(), "arp_ignore", nullptr, EAFNOSUPPORT},
            {INetd::IPV4, -1, sTun.name().c_str(), "arp_ignore", nullptr, EINVAL},
            {INetd::IPV4, INetd::CONF, "..", "conf/lo/arp_ignore", nullptr, EINVAL},
            {INetd::IPV4, INetd::CONF, ".", "lo/arp_ignore", nullptr, EINVAL},
            {INetd::IPV4, INetd::CONF, sTun.name().c_str(), "../all/arp_ignore", nullptr, EINVAL},
            {INetd::IPV6, INetd::NEIGH, LOOPBACK, "ucast_solicit", "3", 0},
    };

    for (size_t i = 0; i < std::size(kTestData); i++) {
        const auto& td = kTestData[i];

        std::string value;
        const binder::Status status =
                mNetd->getProcSysNet(td.ipversion, td.which, td.ifname, td.parameter, &value);

        if (td.expectedReturnCode == 0) {
            SCOPED_TRACE(String8::format("test case %zu should have passed", i));
            EXPECT_EQ(0, status.exceptionCode());
            EXPECT_EQ(0, status.serviceSpecificErrorCode());
            EXPECT_EQ(td.expectedValue, value);
        } else {
            SCOPED_TRACE(String8::format("test case %zu should have failed", i));
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_EQ(td.expectedReturnCode, status.serviceSpecificErrorCode());
        }
    }
}

TEST_F(BinderTest, SetProcSysNet) {
    static const struct {
        const int ipversion;
        const int which;
        const char* ifname;
        const char* parameter;
        const char* value;
        const int expectedReturnCode;
    } kTestData[] = {
            {INetd::IPV4, INetd::CONF, sTun.name().c_str(), "arp_ignore", "1", 0},
            {-1, INetd::CONF, sTun.name().c_str(), "arp_ignore", "1", EAFNOSUPPORT},
            {INetd::IPV4, -1, sTun.name().c_str(), "arp_ignore", "1", EINVAL},
            {INetd::IPV4, INetd::CONF, "..", "conf/lo/arp_ignore", "1", EINVAL},
            {INetd::IPV4, INetd::CONF, ".", "lo/arp_ignore", "1", EINVAL},
            {INetd::IPV4, INetd::CONF, sTun.name().c_str(), "../all/arp_ignore", "1", EINVAL},
            {INetd::IPV6, INetd::NEIGH, sTun.name().c_str(), "ucast_solicit", "7", 0},
    };

    for (size_t i = 0; i < std::size(kTestData); i++) {
        const auto& td = kTestData[i];
        const binder::Status status =
                mNetd->setProcSysNet(td.ipversion, td.which, td.ifname, td.parameter, td.value);

        if (td.expectedReturnCode == 0) {
            SCOPED_TRACE(String8::format("test case %zu should have passed", i));
            EXPECT_EQ(0, status.exceptionCode());
            EXPECT_EQ(0, status.serviceSpecificErrorCode());
        } else {
            SCOPED_TRACE(String8::format("test case %zu should have failed", i));
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_EQ(td.expectedReturnCode, status.serviceSpecificErrorCode());
        }
    }
}

TEST_F(BinderTest, GetSetProcSysNet) {
    const int ipversion = INetd::IPV6;
    const int category = INetd::NEIGH;
    const std::string& tun = sTun.name();
    const std::string parameter("ucast_solicit");

    std::string value{};
    EXPECT_TRUE(mNetd->getProcSysNet(ipversion, category, tun, parameter, &value).isOk());
    ASSERT_FALSE(value.empty());
    const int ival = std::stoi(value);
    EXPECT_GT(ival, 0);
    // Try doubling the parameter value (always best!).
    EXPECT_TRUE(mNetd->setProcSysNet(ipversion, category, tun, parameter, std::to_string(2 * ival))
            .isOk());
    EXPECT_TRUE(mNetd->getProcSysNet(ipversion, category, tun, parameter, &value).isOk());
    EXPECT_EQ(2 * ival, std::stoi(value));
    // Try resetting the parameter.
    EXPECT_TRUE(mNetd->setProcSysNet(ipversion, category, tun, parameter, std::to_string(ival))
            .isOk());
    EXPECT_TRUE(mNetd->getProcSysNet(ipversion, category, tun, parameter, &value).isOk());
    EXPECT_EQ(ival, std::stoi(value));
}

namespace {

void expectNoTestCounterRules() {
    for (const auto& binary : { IPTABLES_PATH, IP6TABLES_PATH }) {
        std::string command = StringPrintf("%s -w -nvL tetherctrl_counters", binary);
        std::string allRules = Join(runCommand(command), "\n");
        EXPECT_EQ(std::string::npos, allRules.find("netdtest_"));
    }
}

void addTetherCounterValues(const char* path, const std::string& if1, const std::string& if2,
                            int byte, int pkt) {
    runCommand(StringPrintf("%s -w -A tetherctrl_counters -i %s -o %s -j RETURN -c %d %d",
                            path, if1.c_str(), if2.c_str(), pkt, byte));
}

void delTetherCounterValues(const char* path, const std::string& if1, const std::string& if2) {
    runCommand(StringPrintf("%s -w -D tetherctrl_counters -i %s -o %s -j RETURN",
                            path, if1.c_str(), if2.c_str()));
    runCommand(StringPrintf("%s -w -D tetherctrl_counters -i %s -o %s -j RETURN",
                            path, if2.c_str(), if1.c_str()));
}

std::vector<int64_t> getStatsVectorByIf(const std::vector<TetherStatsParcel>& statsVec,
                                        const std::string& iface) {
    for (auto& stats : statsVec) {
        if (stats.iface == iface) {
            return {stats.rxBytes, stats.rxPackets, stats.txBytes, stats.txPackets};
        }
    }
    return {};
}

}  // namespace

TEST_F(BinderTest, TetherGetStats) {
    expectNoTestCounterRules();

    // TODO: fold this into more comprehensive tests once we have binder RPCs for enabling and
    // disabling tethering. We don't check the return value because these commands will fail if
    // tethering is already enabled.
    runCommand(StringPrintf("%s -w -N tetherctrl_counters", IPTABLES_PATH));
    runCommand(StringPrintf("%s -w -N tetherctrl_counters", IP6TABLES_PATH));

    std::string intIface1 = StringPrintf("netdtest_%u", arc4random_uniform(10000));
    std::string intIface2 = StringPrintf("netdtest_%u", arc4random_uniform(10000));
    std::string intIface3 = StringPrintf("netdtest_%u", arc4random_uniform(10000));
    std::string extIface1 = StringPrintf("netdtest_%u", arc4random_uniform(10000));
    std::string extIface2 = StringPrintf("netdtest_%u", arc4random_uniform(10000));

    addTetherCounterValues(IPTABLES_PATH,  intIface1, extIface1, 123, 111);
    addTetherCounterValues(IP6TABLES_PATH, intIface1, extIface1, 456,  10);
    addTetherCounterValues(IPTABLES_PATH,  extIface1, intIface1, 321, 222);
    addTetherCounterValues(IP6TABLES_PATH, extIface1, intIface1, 654,  20);
    // RX is from external to internal, and TX is from internal to external.
    // So rxBytes is 321 + 654  = 975, txBytes is 123 + 456 = 579, etc.
    std::vector<int64_t> expected1 = { 975, 242, 579, 121 };

    addTetherCounterValues(IPTABLES_PATH,  intIface2, extIface2, 1000, 333);
    addTetherCounterValues(IP6TABLES_PATH, intIface2, extIface2, 3000,  30);

    addTetherCounterValues(IPTABLES_PATH,  extIface2, intIface2, 2000, 444);
    addTetherCounterValues(IP6TABLES_PATH, extIface2, intIface2, 4000,  40);

    addTetherCounterValues(IP6TABLES_PATH, intIface3, extIface2, 1000,  25);
    addTetherCounterValues(IP6TABLES_PATH, extIface2, intIface3, 2000,  35);
    std::vector<int64_t> expected2 = { 8000, 519, 5000, 388 };

    std::vector<TetherStatsParcel> statsVec;
    binder::Status status = mNetd->tetherGetStats(&statsVec);
    EXPECT_TRUE(status.isOk()) << "Getting tethering stats failed: " << status;

    EXPECT_EQ(expected1, getStatsVectorByIf(statsVec, extIface1));

    EXPECT_EQ(expected2, getStatsVectorByIf(statsVec, extIface2));

    for (const auto& path : { IPTABLES_PATH, IP6TABLES_PATH }) {
        delTetherCounterValues(path, intIface1, extIface1);
        delTetherCounterValues(path, intIface2, extIface2);
        if (path == IP6TABLES_PATH) {
            delTetherCounterValues(path, intIface3, extIface2);
        }
    }

    expectNoTestCounterRules();
}

namespace {

constexpr char IDLETIMER_RAW_PREROUTING[] = "idletimer_raw_PREROUTING";
constexpr char IDLETIMER_MANGLE_POSTROUTING[] = "idletimer_mangle_POSTROUTING";

static std::vector<std::string> listIptablesRuleByTable(const char* binary, const char* table,
                                                        const char* chainName) {
    std::string command = StringPrintf("%s -t %s -w -n -v -L %s", binary, table, chainName);
    return runCommand(command);
}

// TODO: It is a duplicate function, need to remove it
bool iptablesIdleTimerInterfaceRuleExists(const char* binary, const char* chainName,
                                          const std::string& expectedInterface,
                                          const std::string& expectedRule, const char* table) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, table, chainName);
    for (const auto& rule : rules) {
        if (rule.find(expectedInterface) != std::string::npos) {
            if (rule.find(expectedRule) != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}

void expectIdletimerInterfaceRuleExists(const std::string& ifname, int timeout,
                                        const std::string& classLabel) {
    std::string IdletimerRule =
            StringPrintf("timeout:%u label:%s send_nl_msg:1", timeout, classLabel.c_str());
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesIdleTimerInterfaceRuleExists(binary, IDLETIMER_RAW_PREROUTING, ifname,
                                                         IdletimerRule, RAW_TABLE));
        EXPECT_TRUE(iptablesIdleTimerInterfaceRuleExists(binary, IDLETIMER_MANGLE_POSTROUTING,
                                                         ifname, IdletimerRule, MANGLE_TABLE));
    }
}

void expectIdletimerInterfaceRuleNotExists(const std::string& ifname, int timeout,
                                           const std::string& classLabel) {
    std::string IdletimerRule =
            StringPrintf("timeout:%u label:%s send_nl_msg:1", timeout, classLabel.c_str());
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesIdleTimerInterfaceRuleExists(binary, IDLETIMER_RAW_PREROUTING, ifname,
                                                          IdletimerRule, RAW_TABLE));
        EXPECT_FALSE(iptablesIdleTimerInterfaceRuleExists(binary, IDLETIMER_MANGLE_POSTROUTING,
                                                          ifname, IdletimerRule, MANGLE_TABLE));
    }
}

}  // namespace

TEST_F(BinderTest, IdletimerAddRemoveInterface) {
    // TODO: We will get error in if expectIdletimerInterfaceRuleNotExists if there are the same
    // rule in the table. Because we only check the result after calling remove function. We might
    // check the actual rule which is removed by our function (maybe compare the results between
    // calling function before and after)
    binder::Status status;
    const struct TestData {
        const std::string ifname;
        int32_t timeout;
        const std::string classLabel;
    } idleTestData[] = {
            {"wlan0", 1234, "happyday"},
            {"rmnet_data0", 4567, "friday"},
    };
    for (const auto& td : idleTestData) {
        status = mNetd->idletimerAddInterface(td.ifname, td.timeout, td.classLabel);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        expectIdletimerInterfaceRuleExists(td.ifname, td.timeout, td.classLabel);

        status = mNetd->idletimerRemoveInterface(td.ifname, td.timeout, td.classLabel);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        expectIdletimerInterfaceRuleNotExists(td.ifname, td.timeout, td.classLabel);
    }
}

namespace {

constexpr char STRICT_OUTPUT[] = "st_OUTPUT";
constexpr char STRICT_CLEAR_CAUGHT[] = "st_clear_caught";

void expectStrictSetUidAccept(const int uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    std::string perUidChain = StringPrintf("st_clear_caught_%u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesRuleExists(binary, STRICT_OUTPUT, uidRule));
        EXPECT_FALSE(iptablesRuleExists(binary, STRICT_CLEAR_CAUGHT, uidRule));
        EXPECT_EQ(0, iptablesRuleLineLength(binary, perUidChain.c_str()));
    }
}

void expectStrictSetUidLog(const int uid) {
    static const char logRule[] = "st_penalty_log  all";
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    std::string perUidChain = StringPrintf("st_clear_caught_%u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesRuleExists(binary, STRICT_OUTPUT, uidRule));
        EXPECT_TRUE(iptablesRuleExists(binary, STRICT_CLEAR_CAUGHT, uidRule));
        EXPECT_TRUE(iptablesRuleExists(binary, perUidChain.c_str(), logRule));
    }
}

void expectStrictSetUidReject(const int uid) {
    static const char rejectRule[] = "st_penalty_reject  all";
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    std::string perUidChain = StringPrintf("st_clear_caught_%u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesRuleExists(binary, STRICT_OUTPUT, uidRule));
        EXPECT_TRUE(iptablesRuleExists(binary, STRICT_CLEAR_CAUGHT, uidRule));
        EXPECT_TRUE(iptablesRuleExists(binary, perUidChain.c_str(), rejectRule));
    }
}

}  // namespace

TEST_F(BinderTest, StrictSetUidCleartextPenalty) {
    binder::Status status;
    int32_t uid = randomUid();

    // setUidCleartextPenalty Policy:Log with randomUid
    status = mNetd->strictUidCleartextPenalty(uid, INetd::PENALTY_POLICY_LOG);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectStrictSetUidLog(uid);

    // setUidCleartextPenalty Policy:Accept with randomUid
    status = mNetd->strictUidCleartextPenalty(uid, INetd::PENALTY_POLICY_ACCEPT);
    expectStrictSetUidAccept(uid);

    // setUidCleartextPenalty Policy:Reject with randomUid
    status = mNetd->strictUidCleartextPenalty(uid, INetd::PENALTY_POLICY_REJECT);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectStrictSetUidReject(uid);

    // setUidCleartextPenalty Policy:Accept with randomUid
    status = mNetd->strictUidCleartextPenalty(uid, INetd::PENALTY_POLICY_ACCEPT);
    expectStrictSetUidAccept(uid);

    // test wrong policy
    int32_t wrongPolicy = -123;
    status = mNetd->strictUidCleartextPenalty(uid, wrongPolicy);
    EXPECT_EQ(EINVAL, status.serviceSpecificErrorCode());
}

namespace {

std::vector<std::string> tryToFindProcesses(const std::string& processName, uint32_t maxTries = 1,
                                            uint32_t intervalMs = 50) {
    // Output looks like:(clatd)
    // clat          4963   850 1 12:16:51 ?     00:00:00 clatd-netd10a88 -i netd10a88 ...
    // ...
    // root          5221  5219 0 12:18:12 ?     00:00:00 sh -c ps -Af | grep ' clatd-netdcc1a0'

    // (dnsmasq)
    // dns_tether    4620   792 0 16:51:28 ?     00:00:00 dnsmasq --keep-in-foreground ...

    if (maxTries == 0) return {};

    std::string cmd = StringPrintf("ps -Af | grep '[0-9] %s'", processName.c_str());
    std::vector<std::string> result;
    for (uint32_t run = 1;;) {
        result = runCommand(cmd);
        if (result.size() || ++run > maxTries) {
            break;
        }

        usleep(intervalMs * 1000);
    }
    return result;
}

void expectProcessExists(const std::string& processName) {
    EXPECT_EQ(1U, tryToFindProcesses(processName, 5 /*maxTries*/).size());
}

void expectProcessDoesNotExist(const std::string& processName) {
    EXPECT_FALSE(tryToFindProcesses(processName).size());
}

}  // namespace

TEST_F(BinderTest, ClatdStartStop) {
    binder::Status status;

    const std::string clatdName = StringPrintf("clatd-%s", sTun.name().c_str());
    std::string clatAddress;
    std::string nat64Prefix = "2001:db8:cafe:f00d:1:2::/96";

    // Can't start clatd on an interface that's not part of any network...
    status = mNetd->clatdStart(sTun.name(), nat64Prefix, &clatAddress);
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(ENODEV, status.serviceSpecificErrorCode());

    // ... so create a test physical network and add our tun to it.
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    // Prefix must be 96 bits long.
    status = mNetd->clatdStart(sTun.name(), "2001:db8:cafe:f00d::/64", &clatAddress);
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(EINVAL, status.serviceSpecificErrorCode());

    // Can't start clatd unless there's a default route...
    status = mNetd->clatdStart(sTun.name(), nat64Prefix, &clatAddress);
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(EADDRNOTAVAIL, status.serviceSpecificErrorCode());

    // so add a default route.
    EXPECT_TRUE(mNetd->networkAddRoute(TEST_NETID1, sTun.name(), "::/0", "").isOk());

    // Can't start clatd unless there's a global address...
    status = mNetd->clatdStart(sTun.name(), nat64Prefix, &clatAddress);
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(EADDRNOTAVAIL, status.serviceSpecificErrorCode());

    // ... so add a global address.
    const std::string v6 = "2001:db8:1:2:f076:ae99:124e:aa99";
    EXPECT_EQ(0, sTun.addAddress(v6.c_str(), 64));

    // Now expect clatd to start successfully.
    status = mNetd->clatdStart(sTun.name(), nat64Prefix, &clatAddress);
    EXPECT_TRUE(status.isOk());
    EXPECT_EQ(0, status.serviceSpecificErrorCode());

    // Starting it again returns EBUSY.
    status = mNetd->clatdStart(sTun.name(), nat64Prefix, &clatAddress);
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(EBUSY, status.serviceSpecificErrorCode());

    expectProcessExists(clatdName);

    // Expect clatd to stop successfully.
    status = mNetd->clatdStop(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectProcessDoesNotExist(clatdName);

    // Stopping a clatd that doesn't exist returns ENODEV.
    status = mNetd->clatdStop(sTun.name());
    EXPECT_FALSE(status.isOk());
    EXPECT_EQ(ENODEV, status.serviceSpecificErrorCode());
    expectProcessDoesNotExist(clatdName);

    // Clean up.
    EXPECT_TRUE(mNetd->networkRemoveRoute(TEST_NETID1, sTun.name(), "::/0", "").isOk());
    EXPECT_EQ(0, ifc_del_address(sTun.name().c_str(), v6.c_str(), 64));
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

namespace {

bool getIpfwdV4Enable() {
    static const char ipv4IpfwdCmd[] = "cat /proc/sys/net/ipv4/ip_forward";
    std::vector<std::string> result = runCommand(ipv4IpfwdCmd);
    EXPECT_TRUE(!result.empty());
    int v4Enable = std::stoi(result[0]);
    return v4Enable;
}

bool getIpfwdV6Enable() {
    static const char ipv6IpfwdCmd[] = "cat /proc/sys/net/ipv6/conf/all/forwarding";
    std::vector<std::string> result = runCommand(ipv6IpfwdCmd);
    EXPECT_TRUE(!result.empty());
    int v6Enable = std::stoi(result[0]);
    return v6Enable;
}

void expectIpfwdEnable(bool enable) {
    int enableIPv4 = getIpfwdV4Enable();
    int enableIPv6 = getIpfwdV6Enable();
    EXPECT_EQ(enable, enableIPv4);
    EXPECT_EQ(enable, enableIPv6);
}

bool ipRuleIpfwdExists(const char* ipVersion, const std::string& ipfwdRule) {
    std::vector<std::string> rules = listIpRules(ipVersion);
    for (const auto& rule : rules) {
        if (rule.find(ipfwdRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void expectIpfwdRuleExists(const char* fromIf, const char* toIf) {
    std::string ipfwdRule = StringPrintf("18000:\tfrom all iif %s lookup %s ", fromIf, toIf);

    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_TRUE(ipRuleIpfwdExists(ipVersion, ipfwdRule));
    }
}

void expectIpfwdRuleNotExists(const char* fromIf, const char* toIf) {
    std::string ipfwdRule = StringPrintf("18000:\tfrom all iif %s lookup %s ", fromIf, toIf);

    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_FALSE(ipRuleIpfwdExists(ipVersion, ipfwdRule));
    }
}

}  // namespace

TEST_F(BinderTest, TestIpfwdEnableDisableStatusForwarding) {
    // Get ipfwd requester list from Netd
    std::vector<std::string> requesterList;
    binder::Status status = mNetd->ipfwdGetRequesterList(&requesterList);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    bool ipfwdEnabled;
    if (requesterList.size() == 0) {
        // No requester in Netd, ipfwd should be disabled
        // So add one test requester and verify
        status = mNetd->ipfwdEnableForwarding("TestRequester");
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

        expectIpfwdEnable(true);
        status = mNetd->ipfwdEnabled(&ipfwdEnabled);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        EXPECT_TRUE(ipfwdEnabled);

        // Remove test one, verify again
        status = mNetd->ipfwdDisableForwarding("TestRequester");
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

        expectIpfwdEnable(false);
        status = mNetd->ipfwdEnabled(&ipfwdEnabled);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        EXPECT_FALSE(ipfwdEnabled);
    } else {
        // Disable all requesters
        for (const auto& requester : requesterList) {
            status = mNetd->ipfwdDisableForwarding(requester);
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        }

        // After disable all requester, ipfwd should be disabled
        expectIpfwdEnable(false);
        status = mNetd->ipfwdEnabled(&ipfwdEnabled);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        EXPECT_FALSE(ipfwdEnabled);

        // Enable them back
        for (const auto& requester : requesterList) {
            status = mNetd->ipfwdEnableForwarding(requester);
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        }

        // ipfwd should be enabled
        expectIpfwdEnable(true);
        status = mNetd->ipfwdEnabled(&ipfwdEnabled);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        EXPECT_TRUE(ipfwdEnabled);
    }
}

TEST_F(BinderTest, TestIpfwdAddRemoveInterfaceForward) {
  // Add test physical network
  EXPECT_TRUE(
      mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
  EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());
  EXPECT_TRUE(
      mNetd->networkCreatePhysical(TEST_NETID2, INetd::PERMISSION_NONE).isOk());
  EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID2, sTun2.name()).isOk());

  binder::Status status =
      mNetd->ipfwdAddInterfaceForward(sTun.name(), sTun2.name());
  EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
  expectIpfwdRuleExists(sTun.name().c_str(), sTun2.name().c_str());

  status = mNetd->ipfwdRemoveInterfaceForward(sTun.name(), sTun2.name());
  EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
  expectIpfwdRuleNotExists(sTun.name().c_str(), sTun2.name().c_str());
}

namespace {

constexpr char BANDWIDTH_INPUT[] = "bw_INPUT";
constexpr char BANDWIDTH_OUTPUT[] = "bw_OUTPUT";
constexpr char BANDWIDTH_FORWARD[] = "bw_FORWARD";
constexpr char BANDWIDTH_NAUGHTY[] = "bw_penalty_box";
constexpr char BANDWIDTH_NICE[] = "bw_happy_box";
constexpr char BANDWIDTH_ALERT[] = "bw_global_alert";

// TODO: Move iptablesTargetsExists and listIptablesRuleByTable to the top.
//       Use either a std::vector<std::string> of things to match, or a variadic function.
bool iptablesTargetsExists(const char* binary, int expectedCount, const char* table,
                           const char* chainName, const std::string& expectedTargetA,
                           const std::string& expectedTargetB) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, table, chainName);
    int matchCount = 0;

    for (const auto& rule : rules) {
        if (rule.find(expectedTargetA) != std::string::npos) {
            if (rule.find(expectedTargetB) != std::string::npos) {
                matchCount++;
            }
        }
    }
    return matchCount == expectedCount;
}

void expectXtQuotaValueEqual(const char* ifname, long quotaBytes) {
    std::string path = StringPrintf("/proc/net/xt_quota/%s", ifname);
    std::string result = "";

    EXPECT_TRUE(ReadFileToString(path, &result));
    // Quota value might be decreased while matching packets
    EXPECT_GE(quotaBytes, std::stol(Trim(result)));
}

void expectBandwidthInterfaceQuotaRuleExists(const char* ifname, long quotaBytes) {
    std::string BANDWIDTH_COSTLY_IF = StringPrintf("bw_costly_%s", ifname);
    std::string quotaRule = StringPrintf("quota %s", ifname);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesTargetsExists(binary, 1, FILTER_TABLE, BANDWIDTH_INPUT, ifname,
                                          BANDWIDTH_COSTLY_IF));
        EXPECT_TRUE(iptablesTargetsExists(binary, 1, FILTER_TABLE, BANDWIDTH_OUTPUT, ifname,
                                          BANDWIDTH_COSTLY_IF));
        EXPECT_TRUE(iptablesTargetsExists(binary, 2, FILTER_TABLE, BANDWIDTH_FORWARD, ifname,
                                          BANDWIDTH_COSTLY_IF));
        EXPECT_TRUE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), BANDWIDTH_NAUGHTY));
        EXPECT_TRUE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), quotaRule));
    }
    expectXtQuotaValueEqual(ifname, quotaBytes);
}

void expectBandwidthInterfaceQuotaRuleDoesNotExist(const char* ifname) {
    std::string BANDWIDTH_COSTLY_IF = StringPrintf("bw_costly_%s", ifname);
    std::string quotaRule = StringPrintf("quota %s", ifname);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesTargetsExists(binary, 1, FILTER_TABLE, BANDWIDTH_INPUT, ifname,
                                           BANDWIDTH_COSTLY_IF));
        EXPECT_FALSE(iptablesTargetsExists(binary, 1, FILTER_TABLE, BANDWIDTH_OUTPUT, ifname,
                                           BANDWIDTH_COSTLY_IF));
        EXPECT_FALSE(iptablesTargetsExists(binary, 2, FILTER_TABLE, BANDWIDTH_FORWARD, ifname,
                                           BANDWIDTH_COSTLY_IF));
        EXPECT_FALSE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), BANDWIDTH_NAUGHTY));
        EXPECT_FALSE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), quotaRule));
    }
}

void expectBandwidthInterfaceAlertRuleExists(const char* ifname, long alertBytes) {
    std::string BANDWIDTH_COSTLY_IF = StringPrintf("bw_costly_%s", ifname);
    std::string alertRule = StringPrintf("quota %sAlert", ifname);
    std::string alertName = StringPrintf("%sAlert", ifname);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), alertRule));
    }
    expectXtQuotaValueEqual(alertName.c_str(), alertBytes);
}

void expectBandwidthInterfaceAlertRuleDoesNotExist(const char* ifname) {
    std::string BANDWIDTH_COSTLY_IF = StringPrintf("bw_costly_%s", ifname);
    std::string alertRule = StringPrintf("quota %sAlert", ifname);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesRuleExists(binary, BANDWIDTH_COSTLY_IF.c_str(), alertRule));
    }
}

void expectBandwidthGlobalAlertRuleExists(long alertBytes) {
    static const char globalAlertRule[] = "quota globalAlert";
    static const char globalAlertName[] = "globalAlert";

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesRuleExists(binary, BANDWIDTH_ALERT, globalAlertRule));
    }
    expectXtQuotaValueEqual(globalAlertName, alertBytes);
}

void expectBandwidthManipulateSpecialAppRuleExists(const char* chain, const char* target, int uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesTargetsExists(binary, 1, FILTER_TABLE, chain, target, uidRule));
    }
}

void expectBandwidthManipulateSpecialAppRuleDoesNotExist(const char* chain, int uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesRuleExists(binary, chain, uidRule));
    }
}

}  // namespace

TEST_F(BinderTest, BandwidthSetRemoveInterfaceQuota) {
    long testQuotaBytes = 5550;

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    binder::Status status = mNetd->bandwidthSetInterfaceQuota(sTun.name(), testQuotaBytes);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthInterfaceQuotaRuleExists(sTun.name().c_str(), testQuotaBytes);

    status = mNetd->bandwidthRemoveInterfaceQuota(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthInterfaceQuotaRuleDoesNotExist(sTun.name().c_str());

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, BandwidthSetRemoveInterfaceAlert) {
    long testAlertBytes = 373;
    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());
    // Need to have a prior interface quota set to set an alert
    binder::Status status = mNetd->bandwidthSetInterfaceQuota(sTun.name(), testAlertBytes);
    status = mNetd->bandwidthSetInterfaceAlert(sTun.name(), testAlertBytes);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthInterfaceAlertRuleExists(sTun.name().c_str(), testAlertBytes);

    status = mNetd->bandwidthRemoveInterfaceAlert(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthInterfaceAlertRuleDoesNotExist(sTun.name().c_str());

    // Remove interface quota
    status = mNetd->bandwidthRemoveInterfaceQuota(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthInterfaceQuotaRuleDoesNotExist(sTun.name().c_str());

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, BandwidthSetGlobalAlert) {
    long testAlertBytes = 2097149;

    binder::Status status = mNetd->bandwidthSetGlobalAlert(testAlertBytes);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthGlobalAlertRuleExists(testAlertBytes);

    testAlertBytes = 2097152;
    status = mNetd->bandwidthSetGlobalAlert(testAlertBytes);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthGlobalAlertRuleExists(testAlertBytes);
}

TEST_F(BinderTest, BandwidthManipulateSpecialApp) {
    SKIP_IF_BPF_SUPPORTED;

    int32_t uid = randomUid();
    static const char targetReject[] = "REJECT";
    static const char targetReturn[] = "RETURN";

    // add NaughtyApp
    binder::Status status = mNetd->bandwidthAddNaughtyApp(uid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthManipulateSpecialAppRuleExists(BANDWIDTH_NAUGHTY, targetReject, uid);

    // remove NaughtyApp
    status = mNetd->bandwidthRemoveNaughtyApp(uid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthManipulateSpecialAppRuleDoesNotExist(BANDWIDTH_NAUGHTY, uid);

    // add NiceApp
    status = mNetd->bandwidthAddNiceApp(uid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthManipulateSpecialAppRuleExists(BANDWIDTH_NICE, targetReturn, uid);

    // remove NiceApp
    status = mNetd->bandwidthRemoveNiceApp(uid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectBandwidthManipulateSpecialAppRuleDoesNotExist(BANDWIDTH_NICE, uid);
}

namespace {

std::vector<std::string> listIpRoutes(const char* ipVersion, const char* table) {
    std::string command = StringPrintf("%s %s route ls table %s", IP_PATH, ipVersion, table);
    return runCommand(command);
}

bool ipRouteExists(const char* ipVersion, const char* table, const std::string& ipRoute) {
    std::vector<std::string> routes = listIpRoutes(ipVersion, table);
    for (const auto& route : routes) {
        if (route.find(ipRoute) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string ipRouteString(const std::string& ifName, const std::string& dst,
                          const std::string& nextHop) {
    std::string dstString = (dst == "0.0.0.0/0" || dst == "::/0") ? "default" : dst;

    if (!nextHop.empty()) {
        dstString += " via " + nextHop;
    }

    return dstString + " dev " + ifName;
}

void expectNetworkRouteExists(const char* ipVersion, const std::string& ifName,
                              const std::string& dst, const std::string& nextHop,
                              const char* table) {
    EXPECT_TRUE(ipRouteExists(ipVersion, table, ipRouteString(ifName, dst, nextHop)));
}

void expectNetworkRouteDoesNotExist(const char* ipVersion, const std::string& ifName,
                                    const std::string& dst, const std::string& nextHop,
                                    const char* table) {
    EXPECT_FALSE(ipRouteExists(ipVersion, table, ipRouteString(ifName, dst, nextHop)));
}

bool ipRuleExists(const char* ipVersion, const std::string& ipRule) {
    std::vector<std::string> rules = listIpRules(ipVersion);
    for (const auto& rule : rules) {
        if (rule.find(ipRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void expectNetworkDefaultIpRuleExists(const char* ifName) {
    std::string networkDefaultRule =
            StringPrintf("22000:\tfrom all fwmark 0x0/0xffff iif lo lookup %s", ifName);

    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_TRUE(ipRuleExists(ipVersion, networkDefaultRule));
    }
}

void expectNetworkDefaultIpRuleDoesNotExist() {
    static const char networkDefaultRule[] = "22000:\tfrom all fwmark 0x0/0xffff iif lo";

    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_FALSE(ipRuleExists(ipVersion, networkDefaultRule));
    }
}

void expectNetworkPermissionIpRuleExists(const char* ifName, int permission) {
    std::string networkPermissionRule = "";
    switch (permission) {
        case INetd::PERMISSION_NONE:
            networkPermissionRule = StringPrintf(
                    "13000:\tfrom all fwmark 0x1ffdd/0x1ffff iif lo lookup %s", ifName);
            break;
        case INetd::PERMISSION_NETWORK:
            networkPermissionRule = StringPrintf(
                    "13000:\tfrom all fwmark 0x5ffdd/0x5ffff iif lo lookup %s", ifName);
            break;
        case INetd::PERMISSION_SYSTEM:
            networkPermissionRule = StringPrintf(
                    "13000:\tfrom all fwmark 0xdffdd/0xdffff iif lo lookup %s", ifName);
            break;
    }

    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_TRUE(ipRuleExists(ipVersion, networkPermissionRule));
    }
}

// TODO: It is a duplicate function, need to remove it
bool iptablesNetworkPermissionIptablesRuleExists(const char* binary, const char* chainName,
                                                 const std::string& expectedInterface,
                                                 const std::string& expectedRule,
                                                 const char* table) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, table, chainName);
    for (const auto& rule : rules) {
        if (rule.find(expectedInterface) != std::string::npos) {
            if (rule.find(expectedRule) != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}

void expectNetworkPermissionIptablesRuleExists(const char* ifName, int permission) {
    static const char ROUTECTRL_INPUT[] = "routectrl_mangle_INPUT";
    std::string networkIncomingPacketMarkRule = "";
    switch (permission) {
        case INetd::PERMISSION_NONE:
            networkIncomingPacketMarkRule = "MARK xset 0x3ffdd/0xffefffff";
            break;
        case INetd::PERMISSION_NETWORK:
            networkIncomingPacketMarkRule = "MARK xset 0x7ffdd/0xffefffff";
            break;
        case INetd::PERMISSION_SYSTEM:
            networkIncomingPacketMarkRule = "MARK xset 0xfffdd/0xffefffff";
            break;
    }

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesNetworkPermissionIptablesRuleExists(
                binary, ROUTECTRL_INPUT, ifName, networkIncomingPacketMarkRule, MANGLE_TABLE));
    }
}

}  // namespace

TEST_F(BinderTest, NetworkAddRemoveRouteUserPermission) {
    static const struct {
        const char* ipVersion;
        const char* testDest;
        const char* testNextHop;
        const bool expectSuccess;
    } kTestData[] = {
            {IP_RULE_V4, "0.0.0.0/0", "", true},
            {IP_RULE_V4, "0.0.0.0/0", "10.251.10.0", true},
            {IP_RULE_V4, "10.251.0.0/16", "", true},
            {IP_RULE_V4, "10.251.0.0/16", "10.251.10.0", true},
            {IP_RULE_V4, "10.251.0.0/16", "fe80::/64", false},
            {IP_RULE_V6, "::/0", "", true},
            {IP_RULE_V6, "::/0", "2001:db8::", true},
            {IP_RULE_V6, "2001:db8:cafe::/64", "2001:db8::", true},
            {IP_RULE_V4, "fe80::/64", "0.0.0.0", false},
    };

    static const struct {
        const char* ipVersion;
        const char* testDest;
        const char* testNextHop;
    } kTestDataWithNextHop[] = {
            {IP_RULE_V4, "10.251.10.0/30", ""},
            {IP_RULE_V6, "2001:db8::/32", ""},
    };

    static const char testTableLegacySystem[] = "legacy_system";
    static const char testTableLegacyNetwork[] = "legacy_network";
    const int testUid = randomUid();
    const std::vector<int32_t> testUids = {testUid};

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    // Setup route for testing nextHop
    for (size_t i = 0; i < std::size(kTestDataWithNextHop); i++) {
        const auto& td = kTestDataWithNextHop[i];

        // All route for test tun will disappear once the tun interface is deleted.
        binder::Status status =
                mNetd->networkAddRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                 sTun.name().c_str());

        // Add system permission for test uid, setup route in legacy system table.
        EXPECT_TRUE(mNetd->networkSetPermissionForUser(INetd::PERMISSION_SYSTEM, testUids).isOk());

        status = mNetd->networkAddLegacyRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop,
                                              testUid);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                 testTableLegacySystem);

        // Remove system permission for test uid, setup route in legacy network table.
        EXPECT_TRUE(mNetd->networkClearPermissionForUser(testUids).isOk());

        status = mNetd->networkAddLegacyRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop,
                                              testUid);
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
        expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                 testTableLegacyNetwork);
    }

    for (size_t i = 0; i < std::size(kTestData); i++) {
        const auto& td = kTestData[i];

        binder::Status status =
                mNetd->networkAddRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                     sTun.name().c_str());
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }

        status = mNetd->networkRemoveRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteDoesNotExist(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                           sTun.name().c_str());
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }

        // Add system permission for test uid, route will be added into legacy system table.
        EXPECT_TRUE(mNetd->networkSetPermissionForUser(INetd::PERMISSION_SYSTEM, testUids).isOk());

        status = mNetd->networkAddLegacyRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop,
                                              testUid);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                     testTableLegacySystem);
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }

        status = mNetd->networkRemoveLegacyRoute(TEST_NETID1, sTun.name(), td.testDest,
                                                 td.testNextHop, testUid);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteDoesNotExist(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                           testTableLegacySystem);
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }

        // Remove system permission for test uid, route will be added into legacy network table.
        EXPECT_TRUE(mNetd->networkClearPermissionForUser(testUids).isOk());

        status = mNetd->networkAddLegacyRoute(TEST_NETID1, sTun.name(), td.testDest, td.testNextHop,
                                              testUid);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                     testTableLegacyNetwork);
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }

        status = mNetd->networkRemoveLegacyRoute(TEST_NETID1, sTun.name(), td.testDest,
                                                 td.testNextHop, testUid);
        if (td.expectSuccess) {
            EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
            expectNetworkRouteDoesNotExist(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                           testTableLegacyNetwork);
        } else {
            EXPECT_EQ(binder::Status::EX_SERVICE_SPECIFIC, status.exceptionCode());
            EXPECT_NE(0, status.serviceSpecificErrorCode());
        }
    }

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, NetworkPermissionDefault) {
    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    // Get current default network NetId
    binder::Status status = mNetd->networkGetDefault(&mStoredDefaultNetwork);
    ASSERT_TRUE(status.isOk()) << status.exceptionMessage();

    // Test SetDefault
    status = mNetd->networkSetDefault(TEST_NETID1);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNetworkDefaultIpRuleExists(sTun.name().c_str());

    status = mNetd->networkClearDefault();
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNetworkDefaultIpRuleDoesNotExist();

    // Set default network back
    status = mNetd->networkSetDefault(mStoredDefaultNetwork);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    // Test SetPermission
    status = mNetd->networkSetPermissionForNetwork(TEST_NETID1, INetd::PERMISSION_SYSTEM);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNetworkPermissionIpRuleExists(sTun.name().c_str(), INetd::PERMISSION_SYSTEM);
    expectNetworkPermissionIptablesRuleExists(sTun.name().c_str(), INetd::PERMISSION_SYSTEM);

    status = mNetd->networkSetPermissionForNetwork(TEST_NETID1, INetd::PERMISSION_NONE);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNetworkPermissionIpRuleExists(sTun.name().c_str(), INetd::PERMISSION_NONE);
    expectNetworkPermissionIptablesRuleExists(sTun.name().c_str(), INetd::PERMISSION_NONE);

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, NetworkSetProtectAllowDeny) {
    const int testUid = randomUid();
    binder::Status status = mNetd->networkSetProtectAllow(testUid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    bool ret = false;
    status = mNetd->networkCanProtect(testUid, &ret);
    EXPECT_TRUE(ret);

    status = mNetd->networkSetProtectDeny(testUid);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    status = mNetd->networkCanProtect(testUid, &ret);
    EXPECT_FALSE(ret);
}

namespace {

int readIntFromPath(const std::string& path) {
    std::string result = "";
    EXPECT_TRUE(ReadFileToString(path, &result));
    return std::stoi(result);
}

int getTetherAcceptIPv6Ra(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/accept_ra", ifName.c_str());
    return readIntFromPath(path);
}

bool getTetherAcceptIPv6Dad(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/accept_dad", ifName.c_str());
    return readIntFromPath(path);
}

int getTetherIPv6DadTransmits(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/dad_transmits", ifName.c_str());
    return readIntFromPath(path);
}

bool getTetherEnableIPv6(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/disable_ipv6", ifName.c_str());
    int disableIPv6 = readIntFromPath(path);
    return !disableIPv6;
}

bool interfaceListContains(const std::vector<std::string>& ifList, const std::string& ifName) {
    for (const auto& iface : ifList) {
        if (iface == ifName) {
            return true;
        }
    }
    return false;
}

void expectTetherInterfaceConfigureForIPv6Router(const std::string& ifName) {
    EXPECT_EQ(getTetherAcceptIPv6Ra(ifName), 0);
    EXPECT_FALSE(getTetherAcceptIPv6Dad(ifName));
    EXPECT_EQ(getTetherIPv6DadTransmits(ifName), 0);
    EXPECT_TRUE(getTetherEnableIPv6(ifName));
}

void expectTetherInterfaceConfigureForIPv6Client(const std::string& ifName) {
    EXPECT_EQ(getTetherAcceptIPv6Ra(ifName), 2);
    EXPECT_TRUE(getTetherAcceptIPv6Dad(ifName));
    EXPECT_EQ(getTetherIPv6DadTransmits(ifName), 1);
    EXPECT_FALSE(getTetherEnableIPv6(ifName));
}

void expectTetherInterfaceExists(const std::vector<std::string>& ifList,
                                 const std::string& ifName) {
    EXPECT_TRUE(interfaceListContains(ifList, ifName));
}

void expectTetherInterfaceNotExists(const std::vector<std::string>& ifList,
                                    const std::string& ifName) {
    EXPECT_FALSE(interfaceListContains(ifList, ifName));
}

void expectTetherDnsListEquals(const std::vector<std::string>& dnsList,
                               const std::vector<std::string>& testDnsAddrs) {
    EXPECT_TRUE(dnsList == testDnsAddrs);
}

}  // namespace

TEST_F(BinderTest, TetherStartStopStatus) {
    std::vector<std::string> noDhcpRange = {};
    static const char dnsdName[] = "dnsmasq";

    binder::Status status = mNetd->tetherStart(noDhcpRange);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectProcessExists(dnsdName);

    bool tetherEnabled;
    status = mNetd->tetherIsEnabled(&tetherEnabled);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    EXPECT_TRUE(tetherEnabled);

    status = mNetd->tetherStop();
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectProcessDoesNotExist(dnsdName);

    status = mNetd->tetherIsEnabled(&tetherEnabled);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    EXPECT_FALSE(tetherEnabled);
}

TEST_F(BinderTest, TetherInterfaceAddRemoveList) {
    // TODO: verify if dnsmasq update interface successfully

    binder::Status status = mNetd->tetherInterfaceAdd(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectTetherInterfaceConfigureForIPv6Router(sTun.name());

    std::vector<std::string> ifList;
    status = mNetd->tetherInterfaceList(&ifList);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectTetherInterfaceExists(ifList, sTun.name());

    status = mNetd->tetherInterfaceRemove(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectTetherInterfaceConfigureForIPv6Client(sTun.name());

    status = mNetd->tetherInterfaceList(&ifList);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectTetherInterfaceNotExists(ifList, sTun.name());
}

TEST_F(BinderTest, TetherDnsSetList) {
    // TODO: verify if dnsmasq update dns successfully
    std::vector<std::string> testDnsAddrs = {"192.168.1.37", "213.137.100.3",
                                             "fe80::1%" + sTun.name()};

    binder::Status status = mNetd->tetherDnsSet(TEST_NETID1, testDnsAddrs);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    std::vector<std::string> dnsList;
    status = mNetd->tetherDnsList(&dnsList);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectTetherDnsListEquals(dnsList, testDnsAddrs);
}

namespace {

constexpr char FIREWALL_INPUT[] = "fw_INPUT";
constexpr char FIREWALL_OUTPUT[] = "fw_OUTPUT";
constexpr char FIREWALL_FORWARD[] = "fw_FORWARD";
constexpr char FIREWALL_DOZABLE[] = "fw_dozable";
constexpr char FIREWALL_POWERSAVE[] = "fw_powersave";
constexpr char FIREWALL_STANDBY[] = "fw_standby";
constexpr char targetReturn[] = "RETURN";
constexpr char targetDrop[] = "DROP";

void expectFirewallWhitelistMode() {
    static const char dropRule[] = "DROP       all";
    static const char rejectRule[] = "REJECT     all";
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesRuleExists(binary, FIREWALL_INPUT, dropRule));
        EXPECT_TRUE(iptablesRuleExists(binary, FIREWALL_OUTPUT, rejectRule));
        EXPECT_TRUE(iptablesRuleExists(binary, FIREWALL_FORWARD, rejectRule));
    }
}

void expectFirewallBlacklistMode() {
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_EQ(2, iptablesRuleLineLength(binary, FIREWALL_INPUT));
        EXPECT_EQ(2, iptablesRuleLineLength(binary, FIREWALL_OUTPUT));
        EXPECT_EQ(2, iptablesRuleLineLength(binary, FIREWALL_FORWARD));
    }
}

bool iptablesFirewallInterfaceFirstRuleExists(const char* binary, const char* chainName,
                                              const std::string& expectedInterface,
                                              const std::string& expectedRule) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, FILTER_TABLE, chainName);
    // Expected rule:
    // Chain fw_INPUT (1 references)
    // pkts bytes target     prot opt in     out     source               destination
    // 0     0 RETURN     all  --  expectedInterface *       0.0.0.0/0            0.0.0.0/0
    // 0     0 DROP       all  --  *      *       0.0.0.0/0            0.0.0.0/0
    int firstRuleIndex = 2;
    if (rules.size() < 4) return false;
    if (rules[firstRuleIndex].find(expectedInterface) != std::string::npos) {
        if (rules[firstRuleIndex].find(expectedRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// TODO: It is a duplicate function, need to remove it
bool iptablesFirewallInterfaceRuleExists(const char* binary, const char* chainName,
                                         const std::string& expectedInterface,
                                         const std::string& expectedRule) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, FILTER_TABLE, chainName);
    for (const auto& rule : rules) {
        if (rule.find(expectedInterface) != std::string::npos) {
            if (rule.find(expectedRule) != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}

void expectFirewallInterfaceRuleAllowExists(const std::string& ifname) {
    static const char returnRule[] = "RETURN     all";
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesFirewallInterfaceFirstRuleExists(binary, FIREWALL_INPUT, ifname,
                                                             returnRule));
        EXPECT_TRUE(iptablesFirewallInterfaceFirstRuleExists(binary, FIREWALL_OUTPUT, ifname,
                                                             returnRule));
    }
}

void expectFireWallInterfaceRuleAllowDoesNotExist(const std::string& ifname) {
    static const char returnRule[] = "RETURN     all";
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(
                iptablesFirewallInterfaceRuleExists(binary, FIREWALL_INPUT, ifname, returnRule));
        EXPECT_FALSE(
                iptablesFirewallInterfaceRuleExists(binary, FIREWALL_OUTPUT, ifname, returnRule));
    }
}

bool iptablesFirewallUidFirstRuleExists(const char* binary, const char* chainName,
                                        const std::string& expectedTarget,
                                        const std::string& expectedRule) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, FILTER_TABLE, chainName);
    int firstRuleIndex = 2;
    if (rules.size() < 4) return false;
    if (rules[firstRuleIndex].find(expectedTarget) != std::string::npos) {
        if (rules[firstRuleIndex].find(expectedRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool iptablesFirewallUidLastRuleExists(const char* binary, const char* chainName,
                                       const std::string& expectedTarget,
                                       const std::string& expectedRule) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, FILTER_TABLE, chainName);
    int lastRuleIndex = rules.size() - 1;
    if (lastRuleIndex < 0) return false;
    if (rules[lastRuleIndex].find(expectedTarget) != std::string::npos) {
        if (rules[lastRuleIndex].find(expectedRule) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void expectFirewallUidFirstRuleExists(const char* chainName, int32_t uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH})
        EXPECT_TRUE(iptablesFirewallUidFirstRuleExists(binary, chainName, targetReturn, uidRule));
}

void expectFirewallUidFirstRuleDoesNotExist(const char* chainName, int32_t uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH})
        EXPECT_FALSE(iptablesFirewallUidFirstRuleExists(binary, chainName, targetReturn, uidRule));
}

void expectFirewallUidLastRuleExists(const char* chainName, int32_t uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH})
        EXPECT_TRUE(iptablesFirewallUidLastRuleExists(binary, chainName, targetDrop, uidRule));
}

void expectFirewallUidLastRuleDoesNotExist(const char* chainName, int32_t uid) {
    std::string uidRule = StringPrintf("owner UID match %u", uid);
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH})
        EXPECT_FALSE(iptablesFirewallUidLastRuleExists(binary, chainName, targetDrop, uidRule));
}

bool iptablesFirewallChildChainsLastRuleExists(const char* binary, const char* chainName) {
    std::vector<std::string> inputRules =
            listIptablesRuleByTable(binary, FILTER_TABLE, FIREWALL_INPUT);
    std::vector<std::string> outputRules =
            listIptablesRuleByTable(binary, FILTER_TABLE, FIREWALL_OUTPUT);
    int inputLastRuleIndex = inputRules.size() - 1;
    int outputLastRuleIndex = outputRules.size() - 1;

    if (inputLastRuleIndex < 0 || outputLastRuleIndex < 0) return false;
    if (inputRules[inputLastRuleIndex].find(chainName) != std::string::npos) {
        if (outputRules[outputLastRuleIndex].find(chainName) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void expectFirewallChildChainsLastRuleExists(const char* chainRule) {
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH})
        EXPECT_TRUE(iptablesFirewallChildChainsLastRuleExists(binary, chainRule));
}

void expectFirewallChildChainsLastRuleDoesNotExist(const char* chainRule) {
    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_FALSE(iptablesRuleExists(binary, FIREWALL_INPUT, chainRule));
        EXPECT_FALSE(iptablesRuleExists(binary, FIREWALL_OUTPUT, chainRule));
    }
}

}  // namespace

TEST_F(BinderTest, FirewallSetFirewallType) {
    binder::Status status = mNetd->firewallSetFirewallType(INetd::FIREWALL_WHITELIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallWhitelistMode();

    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallBlacklistMode();

    // set firewall type blacklist twice
    mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallBlacklistMode();

    // set firewall type whitelist twice
    mNetd->firewallSetFirewallType(INetd::FIREWALL_WHITELIST);
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_WHITELIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallWhitelistMode();

    // reset firewall type to default
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallBlacklistMode();
}

TEST_F(BinderTest, FirewallSetInterfaceRule) {
    // setinterfaceRule is not supported in BLACKLIST MODE
    binder::Status status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    status = mNetd->firewallSetInterfaceRule(sTun.name(), INetd::FIREWALL_RULE_ALLOW);
    EXPECT_FALSE(status.isOk()) << status.exceptionMessage();

    // set WHITELIST mode first
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_WHITELIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    status = mNetd->firewallSetInterfaceRule(sTun.name(), INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallInterfaceRuleAllowExists(sTun.name());

    status = mNetd->firewallSetInterfaceRule(sTun.name(), INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFireWallInterfaceRuleAllowDoesNotExist(sTun.name());

    // reset firewall mode to default
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallBlacklistMode();
}

TEST_F(BinderTest, FirewallSetUidRule) {
    SKIP_IF_BPF_SUPPORTED;

    int32_t uid = randomUid();

    // Doze allow
    binder::Status status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_DOZABLE, uid,
                                                      INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleExists(FIREWALL_DOZABLE, uid);

    // Doze deny
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_DOZABLE, uid,
                                       INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleDoesNotExist(FIREWALL_DOZABLE, uid);

    // Powersave allow
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_POWERSAVE, uid,
                                       INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleExists(FIREWALL_POWERSAVE, uid);

    // Powersave deny
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_POWERSAVE, uid,
                                       INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleDoesNotExist(FIREWALL_POWERSAVE, uid);

    // Standby deny
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_STANDBY, uid,
                                       INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidLastRuleExists(FIREWALL_STANDBY, uid);

    // Standby allow
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_STANDBY, uid,
                                       INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidLastRuleDoesNotExist(FIREWALL_STANDBY, uid);

    // None deny in BLACKLIST
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_NONE, uid, INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidLastRuleExists(FIREWALL_INPUT, uid);
    expectFirewallUidLastRuleExists(FIREWALL_OUTPUT, uid);

    // None allow in BLACKLIST
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_NONE, uid, INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidLastRuleDoesNotExist(FIREWALL_INPUT, uid);
    expectFirewallUidLastRuleDoesNotExist(FIREWALL_OUTPUT, uid);

    // set firewall type whitelist twice
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_WHITELIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallWhitelistMode();

    // None allow in WHITELIST
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_NONE, uid, INetd::FIREWALL_RULE_ALLOW);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleExists(FIREWALL_INPUT, uid);
    expectFirewallUidFirstRuleExists(FIREWALL_OUTPUT, uid);

    // None deny in WHITELIST
    status = mNetd->firewallSetUidRule(INetd::FIREWALL_CHAIN_NONE, uid, INetd::FIREWALL_RULE_DENY);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallUidFirstRuleDoesNotExist(FIREWALL_INPUT, uid);
    expectFirewallUidFirstRuleDoesNotExist(FIREWALL_OUTPUT, uid);

    // reset firewall mode to default
    status = mNetd->firewallSetFirewallType(INetd::FIREWALL_BLACKLIST);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallBlacklistMode();
}

TEST_F(BinderTest, FirewallEnableDisableChildChains) {
    SKIP_IF_BPF_SUPPORTED;

    binder::Status status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_DOZABLE, true);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleExists(FIREWALL_DOZABLE);

    status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_STANDBY, true);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleExists(FIREWALL_STANDBY);

    status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_POWERSAVE, true);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleExists(FIREWALL_POWERSAVE);

    status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_DOZABLE, false);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleDoesNotExist(FIREWALL_DOZABLE);

    status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_STANDBY, false);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleDoesNotExist(FIREWALL_STANDBY);

    status = mNetd->firewallEnableChildChain(INetd::FIREWALL_CHAIN_POWERSAVE, false);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectFirewallChildChainsLastRuleDoesNotExist(FIREWALL_POWERSAVE);
}

namespace {

std::string hwAddrToStr(unsigned char* hwaddr) {
    return StringPrintf("%02x:%02x:%02x:%02x:%02x:%02x", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3],
                        hwaddr[4], hwaddr[5]);
}

int ipv4NetmaskToPrefixLength(in_addr_t mask) {
    int prefixLength = 0;
    uint32_t m = ntohl(mask);
    while (m & (1 << 31)) {
        prefixLength++;
        m = m << 1;
    }
    return prefixLength;
}

std::string toStdString(const String16& s) {
    return std::string(String8(s.string()));
}

android::netdutils::StatusOr<ifreq> ioctlByIfName(const std::string& ifName, unsigned long flag) {
    const auto& sys = sSyscalls.get();
    auto fd = sys.socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    EXPECT_TRUE(isOk(fd.status()));

    struct ifreq ifr = {};
    strlcpy(ifr.ifr_name, ifName.c_str(), IFNAMSIZ);

    return sys.ioctl(fd.value(), flag, &ifr);
}

std::string getInterfaceHwAddr(const std::string& ifName) {
    auto res = ioctlByIfName(ifName, SIOCGIFHWADDR);

    unsigned char hwaddr[ETH_ALEN] = {};
    if (isOk(res.status())) {
        memcpy((void*) hwaddr, &res.value().ifr_hwaddr.sa_data, ETH_ALEN);
    }

    return hwAddrToStr(hwaddr);
}

int getInterfaceIPv4Prefix(const std::string& ifName) {
    auto res = ioctlByIfName(ifName, SIOCGIFNETMASK);

    int prefixLength = 0;
    if (isOk(res.status())) {
        prefixLength = ipv4NetmaskToPrefixLength(
                ((struct sockaddr_in*) &res.value().ifr_addr)->sin_addr.s_addr);
    }

    return prefixLength;
}

std::string getInterfaceIPv4Addr(const std::string& ifName) {
    auto res = ioctlByIfName(ifName, SIOCGIFADDR);

    struct in_addr addr = {};
    if (isOk(res.status())) {
        addr.s_addr = ((struct sockaddr_in*) &res.value().ifr_addr)->sin_addr.s_addr;
    }

    return std::string(inet_ntoa(addr));
}

std::vector<std::string> getInterfaceFlags(const std::string& ifName) {
    auto res = ioctlByIfName(ifName, SIOCGIFFLAGS);

    unsigned flags = 0;
    if (isOk(res.status())) {
        flags = res.value().ifr_flags;
    }

    std::vector<std::string> ifFlags;
    ifFlags.push_back(flags & IFF_UP ? toStdString(INetd::IF_STATE_UP())
                                     : toStdString(INetd::IF_STATE_DOWN()));

    if (flags & IFF_BROADCAST) ifFlags.push_back(toStdString(INetd::IF_FLAG_BROADCAST()));
    if (flags & IFF_LOOPBACK) ifFlags.push_back(toStdString(INetd::IF_FLAG_LOOPBACK()));
    if (flags & IFF_POINTOPOINT) ifFlags.push_back(toStdString(INetd::IF_FLAG_POINTOPOINT()));
    if (flags & IFF_RUNNING) ifFlags.push_back(toStdString(INetd::IF_FLAG_RUNNING()));
    if (flags & IFF_MULTICAST) ifFlags.push_back(toStdString(INetd::IF_FLAG_MULTICAST()));

    return ifFlags;
}

bool compareListInterface(const std::vector<std::string>& interfaceList) {
    const auto& res = InterfaceController::getIfaceNames();
    EXPECT_TRUE(isOk(res));

    std::vector<std::string> resIfList;
    resIfList.reserve(res.value().size());
    resIfList.insert(end(resIfList), begin(res.value()), end(res.value()));

    return resIfList == interfaceList;
}

int getInterfaceIPv6PrivacyExtensions(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/use_tempaddr", ifName.c_str());
    return readIntFromPath(path);
}

bool getInterfaceEnableIPv6(const std::string& ifName) {
    std::string path = StringPrintf("/proc/sys/net/ipv6/conf/%s/disable_ipv6", ifName.c_str());

    int disableIPv6 = readIntFromPath(path);
    return !disableIPv6;
}

int getInterfaceMtu(const std::string& ifName) {
    std::string path = StringPrintf("/sys/class/net/%s/mtu", ifName.c_str());
    return readIntFromPath(path);
}

void expectInterfaceList(const std::vector<std::string>& interfaceList) {
    EXPECT_TRUE(compareListInterface(interfaceList));
}

void expectCurrentInterfaceConfigurationEquals(const std::string& ifName,
                                               const InterfaceConfigurationParcel& interfaceCfg) {
    EXPECT_EQ(getInterfaceIPv4Addr(ifName), interfaceCfg.ipv4Addr);
    EXPECT_EQ(getInterfaceIPv4Prefix(ifName), interfaceCfg.prefixLength);
    EXPECT_EQ(getInterfaceHwAddr(ifName), interfaceCfg.hwAddr);
    EXPECT_EQ(getInterfaceFlags(ifName), interfaceCfg.flags);
}

void expectCurrentInterfaceConfigurationAlmostEqual(const InterfaceConfigurationParcel& setCfg) {
    EXPECT_EQ(getInterfaceIPv4Addr(setCfg.ifName), setCfg.ipv4Addr);
    EXPECT_EQ(getInterfaceIPv4Prefix(setCfg.ifName), setCfg.prefixLength);

    const auto& ifFlags = getInterfaceFlags(setCfg.ifName);
    for (const auto& flag : setCfg.flags) {
        EXPECT_TRUE(std::find(ifFlags.begin(), ifFlags.end(), flag) != ifFlags.end());
    }
}

void expectInterfaceIPv6PrivacyExtensions(const std::string& ifName, bool enable) {
    int v6PrivacyExtensions = getInterfaceIPv6PrivacyExtensions(ifName);
    EXPECT_EQ(v6PrivacyExtensions, enable ? 2 : 0);
}

void expectInterfaceNoAddr(const std::string& ifName) {
    // noAddr
    EXPECT_EQ(getInterfaceIPv4Addr(ifName), "0.0.0.0");
    // noPrefix
    EXPECT_EQ(getInterfaceIPv4Prefix(ifName), 0);
}

void expectInterfaceEnableIPv6(const std::string& ifName, bool enable) {
    int enableIPv6 = getInterfaceEnableIPv6(ifName);
    EXPECT_EQ(enableIPv6, enable);
}

void expectInterfaceMtu(const std::string& ifName, const int mtu) {
    int mtuSize = getInterfaceMtu(ifName);
    EXPECT_EQ(mtu, mtuSize);
}

InterfaceConfigurationParcel makeInterfaceCfgParcel(const std::string& ifName,
                                                    const std::string& addr, int prefixLength,
                                                    const std::vector<std::string>& flags) {
    InterfaceConfigurationParcel cfg;
    cfg.ifName = ifName;
    cfg.hwAddr = "";
    cfg.ipv4Addr = addr;
    cfg.prefixLength = prefixLength;
    cfg.flags = flags;
    return cfg;
}

void expectTunFlags(const InterfaceConfigurationParcel& interfaceCfg) {
    std::vector<std::string> expectedFlags = {"up", "point-to-point", "running", "multicast"};
    std::vector<std::string> unexpectedFlags = {"down", "broadcast"};

    for (const auto& flag : expectedFlags) {
        EXPECT_TRUE(std::find(interfaceCfg.flags.begin(), interfaceCfg.flags.end(), flag) !=
                    interfaceCfg.flags.end());
    }

    for (const auto& flag : unexpectedFlags) {
        EXPECT_TRUE(std::find(interfaceCfg.flags.begin(), interfaceCfg.flags.end(), flag) ==
                    interfaceCfg.flags.end());
    }
}

}  // namespace

TEST_F(BinderTest, InterfaceList) {
    std::vector<std::string> interfaceListResult;

    binder::Status status = mNetd->interfaceGetList(&interfaceListResult);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceList(interfaceListResult);
}

TEST_F(BinderTest, InterfaceGetCfg) {
    InterfaceConfigurationParcel interfaceCfgResult;

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    binder::Status status = mNetd->interfaceGetCfg(sTun.name(), &interfaceCfgResult);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectCurrentInterfaceConfigurationEquals(sTun.name(), interfaceCfgResult);
    expectTunFlags(interfaceCfgResult);

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, InterfaceSetCfg) {
    const std::string testAddr = "192.0.2.3";
    const int testPrefixLength = 24;
    std::vector<std::string> upFlags = {"up"};
    std::vector<std::string> downFlags = {"down"};

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    // Set tun interface down.
    auto interfaceCfg = makeInterfaceCfgParcel(sTun.name(), testAddr, testPrefixLength, downFlags);
    binder::Status status = mNetd->interfaceSetCfg(interfaceCfg);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectCurrentInterfaceConfigurationAlmostEqual(interfaceCfg);

    // Set tun interface up again.
    interfaceCfg = makeInterfaceCfgParcel(sTun.name(), testAddr, testPrefixLength, upFlags);
    status = mNetd->interfaceSetCfg(interfaceCfg);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    status = mNetd->interfaceClearAddrs(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, InterfaceSetIPv6PrivacyExtensions) {
    // enable
    binder::Status status = mNetd->interfaceSetIPv6PrivacyExtensions(sTun.name(), true);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceIPv6PrivacyExtensions(sTun.name(), true);

    // disable
    status = mNetd->interfaceSetIPv6PrivacyExtensions(sTun.name(), false);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceIPv6PrivacyExtensions(sTun.name(), false);
}

TEST_F(BinderTest, InterfaceClearAddr) {
    const std::string testAddr = "192.0.2.3";
    const int testPrefixLength = 24;
    std::vector<std::string> noFlags{};

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    auto interfaceCfg = makeInterfaceCfgParcel(sTun.name(), testAddr, testPrefixLength, noFlags);
    binder::Status status = mNetd->interfaceSetCfg(interfaceCfg);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectCurrentInterfaceConfigurationAlmostEqual(interfaceCfg);

    status = mNetd->interfaceClearAddrs(sTun.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceNoAddr(sTun.name());

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, InterfaceSetEnableIPv6) {
    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    // disable
    binder::Status status = mNetd->interfaceSetEnableIPv6(sTun.name(), false);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceEnableIPv6(sTun.name(), false);

    // enable
    status = mNetd->interfaceSetEnableIPv6(sTun.name(), true);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceEnableIPv6(sTun.name(), true);

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, InterfaceSetMtu) {
    const int testMtu = 1200;

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    binder::Status status = mNetd->interfaceSetMtu(sTun.name(), testMtu);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectInterfaceMtu(sTun.name(), testMtu);

    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

namespace {

constexpr const char TETHER_FORWARD[] = "tetherctrl_FORWARD";
constexpr const char TETHER_NAT_POSTROUTING[] = "tetherctrl_nat_POSTROUTING";
constexpr const char TETHER_RAW_PREROUTING[] = "tetherctrl_raw_PREROUTING";
constexpr const char TETHER_COUNTERS_CHAIN[] = "tetherctrl_counters";

int iptablesCountRules(const char* binary, const char* table, const char* chainName) {
    return listIptablesRuleByTable(binary, table, chainName).size();
}

bool iptablesChainMatch(const char* binary, const char* table, const char* chainName,
                        const std::vector<std::string>& targetVec) {
    std::vector<std::string> rules = listIptablesRuleByTable(binary, table, chainName);
    if (targetVec.size() != rules.size() - 2) {
        return false;
    }

    /*
     * Check that the rules match. Note that this function matches substrings, not entire rules,
     * because otherwise rules where "pkts" or "bytes" are nonzero would not match.
     * Skip first two lines since rules start from third line.
     * Chain chainName (x references)
     * pkts bytes target     prot opt in     out     source               destination
     * ...
     */
    int rIndex = 2;
    for (const auto& target : targetVec) {
        if (rules[rIndex].find(target) == std::string::npos) {
            return false;
        }
        rIndex++;
    }
    return true;
}

void expectNatEnable(const std::string& intIf, const std::string& extIf) {
    std::vector<std::string> postroutingV4Match = {"MASQUERADE"};
    std::vector<std::string> preroutingV4Match = {"CT helper ftp", "CT helper pptp"};
    std::vector<std::string> forwardV4Match = {
            "bw_global_alert", "state RELATED", "state INVALID",
            StringPrintf("tetherctrl_counters  all  --  %s %s", intIf.c_str(), extIf.c_str()),
            "DROP"};

    // V4
    EXPECT_TRUE(iptablesChainMatch(IPTABLES_PATH, NAT_TABLE, TETHER_NAT_POSTROUTING,
                                   postroutingV4Match));
    EXPECT_TRUE(
            iptablesChainMatch(IPTABLES_PATH, RAW_TABLE, TETHER_RAW_PREROUTING, preroutingV4Match));
    EXPECT_TRUE(iptablesChainMatch(IPTABLES_PATH, FILTER_TABLE, TETHER_FORWARD, forwardV4Match));

    std::vector<std::string> forwardV6Match = {"bw_global_alert", "tetherctrl_counters"};
    std::vector<std::string> preroutingV6Match = {"rpfilter invert"};

    // V6
    EXPECT_TRUE(iptablesChainMatch(IP6TABLES_PATH, FILTER_TABLE, TETHER_FORWARD, forwardV6Match));
    EXPECT_TRUE(iptablesChainMatch(IP6TABLES_PATH, RAW_TABLE, TETHER_RAW_PREROUTING,
                                   preroutingV6Match));

    for (const auto& binary : {IPTABLES_PATH, IP6TABLES_PATH}) {
        EXPECT_TRUE(iptablesTargetsExists(binary, 2, FILTER_TABLE, TETHER_COUNTERS_CHAIN, intIf,
                                          extIf));
    }
}

void expectNatDisable() {
    // It is the default DROP rule with tethering disable.
    // Chain tetherctrl_FORWARD (1 references)
    // pkts bytes target     prot opt in     out     source               destination
    //    0     0 DROP       all  --  *      *       0.0.0.0/0            0.0.0.0/0
    std::vector<std::string> forwardV4Match = {"DROP"};
    EXPECT_TRUE(iptablesChainMatch(IPTABLES_PATH, FILTER_TABLE, TETHER_FORWARD, forwardV4Match));

    // We expect that these chains should be empty.
    EXPECT_EQ(2, iptablesCountRules(IPTABLES_PATH, NAT_TABLE, TETHER_NAT_POSTROUTING));
    EXPECT_EQ(2, iptablesCountRules(IPTABLES_PATH, RAW_TABLE, TETHER_RAW_PREROUTING));

    EXPECT_EQ(2, iptablesCountRules(IP6TABLES_PATH, FILTER_TABLE, TETHER_FORWARD));
    EXPECT_EQ(2, iptablesCountRules(IP6TABLES_PATH, RAW_TABLE, TETHER_RAW_PREROUTING));

    // Netd won't clear tether quota rule, we don't care rule in tetherctrl_counters.
}

}  // namespace

TEST_F(BinderTest, TetherForwardAddRemove) {
    binder::Status status = mNetd->tetherAddForward(sTun.name(), sTun2.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNatEnable(sTun.name(), sTun2.name());

    status = mNetd->tetherRemoveForward(sTun.name(), sTun2.name());
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    expectNatDisable();
}

namespace {

using TripleInt = std::array<int, 3>;

TripleInt readProcFileToTripleInt(const std::string& path) {
    std::string valueString;
    int min, def, max;
    EXPECT_TRUE(ReadFileToString(path, &valueString));
    EXPECT_EQ(3, sscanf(valueString.c_str(), "%d %d %d", &min, &def, &max));
    return {min, def, max};
}

void updateAndCheckTcpBuffer(sp<INetd>& netd, TripleInt& rmemValues, TripleInt& wmemValues) {
    std::string testRmemValues =
            StringPrintf("%u %u %u", rmemValues[0], rmemValues[1], rmemValues[2]);
    std::string testWmemValues =
            StringPrintf("%u %u %u", wmemValues[0], wmemValues[1], wmemValues[2]);
    EXPECT_TRUE(netd->setTcpRWmemorySize(testRmemValues, testWmemValues).isOk());

    TripleInt newRmemValues = readProcFileToTripleInt(TCP_RMEM_PROC_FILE);
    TripleInt newWmemValues = readProcFileToTripleInt(TCP_WMEM_PROC_FILE);

    for (int i = 0; i < 3; i++) {
        SCOPED_TRACE(StringPrintf("tcp_mem value %d should be equal", i));
        EXPECT_EQ(rmemValues[i], newRmemValues[i]);
        EXPECT_EQ(wmemValues[i], newWmemValues[i]);
    }
}

}  // namespace

TEST_F(BinderTest, TcpBufferSet) {
    TripleInt rmemValue = readProcFileToTripleInt(TCP_RMEM_PROC_FILE);
    TripleInt testRmemValue{rmemValue[0] + 42, rmemValue[1] + 42, rmemValue[2] + 42};
    TripleInt wmemValue = readProcFileToTripleInt(TCP_WMEM_PROC_FILE);
    TripleInt testWmemValue{wmemValue[0] + 42, wmemValue[1] + 42, wmemValue[2] + 42};

    updateAndCheckTcpBuffer(mNetd, testRmemValue, testWmemValue);
    updateAndCheckTcpBuffer(mNetd, rmemValue, wmemValue);
}

namespace {

void checkUidsInPermissionMap(std::vector<int32_t>& uids, bool exist) {
    android::bpf::BpfMap<uint32_t, uint8_t> uidPermissionMap(
            android::bpf::mapRetrieve(UID_PERMISSION_MAP_PATH, 0));
    for (int32_t uid : uids) {
        android::netdutils::StatusOr<uint8_t> permission = uidPermissionMap.readValue(uid);
        if (exist) {
            EXPECT_TRUE(isOk(permission));
            EXPECT_EQ(INetd::PERMISSION_NONE, permission.value());
        } else {
            EXPECT_FALSE(isOk(permission));
            EXPECT_EQ(ENOENT, permission.status().code());
        }
    }
}

}  // namespace

TEST_F(BinderTest, TestInternetPermission) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    std::vector<int32_t> appUids = {TEST_UID1, TEST_UID2};

    mNetd->trafficSetNetPermForUids(INetd::PERMISSION_INTERNET, appUids);
    checkUidsInPermissionMap(appUids, false);
    mNetd->trafficSetNetPermForUids(INetd::PERMISSION_NONE, appUids);
    checkUidsInPermissionMap(appUids, true);
    mNetd->trafficSetNetPermForUids(INetd::PERMISSION_UNINSTALLED, appUids);
    checkUidsInPermissionMap(appUids, false);
}

TEST_F(BinderTest, UnsolEvents) {
    auto testUnsolService = android::net::TestUnsolService::start();
    std::string oldTunName = sTun.name();
    std::string newTunName = "unsolTest";
    testUnsolService->tarVec.push_back(oldTunName);
    testUnsolService->tarVec.push_back(newTunName);
    auto& cv = testUnsolService->getCv();
    auto& cvMutex = testUnsolService->getCvMutex();
    binder::Status status = mNetd->registerUnsolicitedEventListener(
            android::interface_cast<android::net::INetdUnsolicitedEventListener>(testUnsolService));
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

    // TODO: Add test for below events
    //       StrictCleartextDetected / InterfaceDnsServersAdded
    //       InterfaceClassActivity / QuotaLimitReached / InterfaceAddressRemoved

    {
        std::unique_lock lock(cvMutex);

        // Re-init test Tun, and we expect that we will get some unsol events.
        // Use the test Tun device name to verify if we receive its unsol events.
        sTun.destroy();
        // Use predefined name
        sTun.init(newTunName);

        EXPECT_EQ(std::cv_status::no_timeout, cv.wait_for(lock, std::chrono::seconds(2)));
    }

    // bit mask 1101101000
    // Test only covers below events currently
    const uint32_t kExpectedEvents = InterfaceAddressUpdated | InterfaceAdded | InterfaceRemoved |
                                     InterfaceLinkStatusChanged | RouteChanged;
    EXPECT_EQ(kExpectedEvents, testUnsolService->getReceived());

    // Re-init sTun to clear predefined name
    sTun.destroy();
    sTun.init();
}

TEST_F(BinderTest, NDC) {
    struct Command {
        const std::string cmdString;
        const std::string expectedResult;
    };

    // clang-format off
    // Do not change the commands order
    const Command networkCmds[] = {
            {StringPrintf("ndc network create %d", TEST_NETID1),
             "200 0 success"},
            {StringPrintf("ndc network interface add %d %s", TEST_NETID1, sTun.name().c_str()),
             "200 0 success"},
            {StringPrintf("ndc network interface remove %d %s", TEST_NETID1, sTun.name().c_str()),
             "200 0 success"},
            {StringPrintf("ndc network interface add %d %s", TEST_NETID2, sTun.name().c_str()),
             "400 0 addInterfaceToNetwork() failed (Machine is not on the network)"},
            {StringPrintf("ndc network destroy %d", TEST_NETID1),
             "200 0 success"},
    };

    const std::vector<Command> ipfwdCmds = {
            {"ndc ipfwd enable " + sTun.name(),
             "200 0 ipfwd operation succeeded"},
            {"ndc ipfwd disable " + sTun.name(),
             "200 0 ipfwd operation succeeded"},
            {"ndc ipfwd add lo2 lo3",
             "400 0 ipfwd operation failed (No such process)"},
            {"ndc ipfwd add " + sTun.name() + " " + sTun2.name(),
             "200 0 ipfwd operation succeeded"},
            {"ndc ipfwd remove " + sTun.name() + " " + sTun2.name(),
             "200 0 ipfwd operation succeeded"},
    };

    static const struct {
        const char* ipVersion;
        const char* testDest;
        const char* testNextHop;
        const bool expectSuccess;
        const std::string expectedResult;
    } kTestData[] = {
            {IP_RULE_V4, "0.0.0.0/0",          "",            true,
             "200 0 success"},
            {IP_RULE_V4, "10.251.0.0/16",      "",            true,
             "200 0 success"},
            {IP_RULE_V4, "10.251.0.0/16",      "fe80::/64",   false,
             "400 0 addRoute() failed (Invalid argument)",},
            {IP_RULE_V6, "::/0",               "",            true,
             "200 0 success"},
            {IP_RULE_V6, "2001:db8:cafe::/64", "",            true,
             "200 0 success"},
            {IP_RULE_V6, "fe80::/64",          "0.0.0.0",     false,
             "400 0 addRoute() failed (Invalid argument)"},
    };
    // clang-format on

    for (const auto& cmd : networkCmds) {
        const std::vector<std::string> result = runCommand(cmd.cmdString);
        SCOPED_TRACE(cmd.cmdString);
        EXPECT_EQ(result.size(), 1U);
        EXPECT_EQ(cmd.expectedResult, Trim(result[0]));
    }

    for (const auto& cmd : ipfwdCmds) {
        const std::vector<std::string> result = runCommand(cmd.cmdString);
        SCOPED_TRACE(cmd.cmdString);
        EXPECT_EQ(result.size(), 1U);
        EXPECT_EQ(cmd.expectedResult, Trim(result[0]));
    }

    // Add test physical network
    EXPECT_TRUE(mNetd->networkCreatePhysical(TEST_NETID1, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(TEST_NETID1, sTun.name()).isOk());

    for (const auto& td : kTestData) {
        const std::string routeAddCmd =
                StringPrintf("ndc network route add %d %s %s %s", TEST_NETID1, sTun.name().c_str(),
                             td.testDest, td.testNextHop);
        const std::string routeRemoveCmd =
                StringPrintf("ndc network route remove %d %s %s %s", TEST_NETID1,
                             sTun.name().c_str(), td.testDest, td.testNextHop);
        std::vector<std::string> result = runCommand(routeAddCmd);
        SCOPED_TRACE(routeAddCmd);
        EXPECT_EQ(result.size(), 1U);
        EXPECT_EQ(td.expectedResult, Trim(result[0]));
        if (td.expectSuccess) {
            expectNetworkRouteExists(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                     sTun.name().c_str());
            result = runCommand(routeRemoveCmd);
            EXPECT_EQ(result.size(), 1U);
            EXPECT_EQ(td.expectedResult, Trim(result[0]));
            expectNetworkRouteDoesNotExist(td.ipVersion, sTun.name(), td.testDest, td.testNextHop,
                                           sTun.name().c_str());
        }
    }
    // Remove test physical network
    EXPECT_TRUE(mNetd->networkDestroy(TEST_NETID1).isOk());
}

TEST_F(BinderTest, OemNetdRelated) {
    sp<IBinder> binder;
    binder::Status status = mNetd->getOemNetd(&binder);
    EXPECT_TRUE(status.isOk()) << status.exceptionMessage();
    sp<com::android::internal::net::IOemNetd> oemNetd;
    if (binder != nullptr) {
        oemNetd = android::interface_cast<com::android::internal::net::IOemNetd>(binder);
    }
    ASSERT_NE(nullptr, oemNetd.get());

    TimedOperation t("OemNetd isAlive RPC");
    bool isAlive = false;
    oemNetd->isAlive(&isAlive);
    ASSERT_TRUE(isAlive);

    class TestOemUnsolListener
        : public com::android::internal::net::BnOemNetdUnsolicitedEventListener {
      public:
        android::binder::Status onRegistered() override {
            std::lock_guard lock(mCvMutex);
            mCv.notify_one();
            return android::binder::Status::ok();
        }
        std::condition_variable& getCv() { return mCv; }
        std::mutex& getCvMutex() { return mCvMutex; }

      private:
        std::mutex mCvMutex;
        std::condition_variable mCv;
    };

    // Start the Binder thread pool.
    android::ProcessState::self()->startThreadPool();

    android::sp<TestOemUnsolListener> testListener = new TestOemUnsolListener();

    auto& cv = testListener->getCv();
    auto& cvMutex = testListener->getCvMutex();

    {
        std::unique_lock lock(cvMutex);

        status = oemNetd->registerOemUnsolicitedEventListener(
                ::android::interface_cast<
                        com::android::internal::net::IOemNetdUnsolicitedEventListener>(
                        testListener));
        EXPECT_TRUE(status.isOk()) << status.exceptionMessage();

        // Wait for receiving expected events.
        EXPECT_EQ(std::cv_status::no_timeout, cv.wait_for(lock, std::chrono::seconds(2)));
    }
}

void BinderTest::createVpnNetworkWithUid(bool secure, uid_t uid, int vpnNetId,
                                         int fallthroughNetId) {
    // Re-init sTun* to ensure route rule exists.
    sTun.destroy();
    sTun.init();
    sTun2.destroy();
    sTun2.init();

    // Create physical network with fallthroughNetId but not set it as default network
    EXPECT_TRUE(mNetd->networkCreatePhysical(fallthroughNetId, INetd::PERMISSION_NONE).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(fallthroughNetId, sTun.name()).isOk());

    // Create VPN with vpnNetId
    EXPECT_TRUE(mNetd->networkCreateVpn(vpnNetId, secure).isOk());

    // Add uid to VPN
    EXPECT_TRUE(mNetd->networkAddUidRanges(vpnNetId, {makeUidRangeParcel(uid, uid)}).isOk());
    EXPECT_TRUE(mNetd->networkAddInterface(vpnNetId, sTun2.name()).isOk());

    // Add default route to fallthroughNetwork
    EXPECT_TRUE(mNetd->networkAddRoute(TEST_NETID1, sTun.name(), "::/0", "").isOk());
    // Add limited route
    EXPECT_TRUE(mNetd->networkAddRoute(TEST_NETID2, sTun2.name(), "2001:db8::/32", "").isOk());
}

namespace {

class ScopedUidChange {
  public:
    explicit ScopedUidChange(uid_t uid) : mInputUid(uid) {
        mStoredUid = getuid();
        if (mInputUid == mStoredUid) return;
        EXPECT_TRUE(seteuid(uid) == 0);
    }
    ~ScopedUidChange() {
        if (mInputUid == mStoredUid) return;
        EXPECT_TRUE(seteuid(mStoredUid) == 0);
    }

  private:
    uid_t mInputUid;
    uid_t mStoredUid;
};

constexpr uint32_t RULE_PRIORITY_VPN_FALLTHROUGH = 21000;

void clearQueue(int tunFd) {
    char buf[4096];
    int ret;
    do {
        ret = read(tunFd, buf, sizeof(buf));
    } while (ret > 0);
}

void checkDataReceived(int udpSocket, int tunFd) {
    char buf[4096] = {};
    // Clear tunFd's queue before write something because there might be some
    // arbitrary packets in the queue. (e.g. ICMPv6 packet)
    clearQueue(tunFd);
    EXPECT_EQ(4, write(udpSocket, "foo", sizeof("foo")));
    // TODO: extract header and verify data
    EXPECT_GT(read(tunFd, buf, sizeof(buf)), 0);
}

bool sendIPv6PacketFromUid(uid_t uid, const in6_addr& dstAddr, Fwmark* fwmark, int tunFd) {
    ScopedUidChange scopedUidChange(uid);
    android::base::unique_fd testSocket(socket(AF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0));
    if (testSocket < 0) return false;

    const sockaddr_in6 dst6 = {.sin6_family = AF_INET6, .sin6_addr = dstAddr, .sin6_port = 42};
    int res = connect(testSocket, (sockaddr*)&dst6, sizeof(dst6));
    socklen_t fwmarkLen = sizeof(fwmark->intValue);
    EXPECT_NE(-1, getsockopt(testSocket, SOL_SOCKET, SO_MARK, &(fwmark->intValue), &fwmarkLen));
    if (res == -1) return false;

    char addr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &dstAddr, addr, INET6_ADDRSTRLEN);
    SCOPED_TRACE(StringPrintf("sendIPv6PacketFromUid, addr: %s, uid: %u", addr, uid));
    checkDataReceived(testSocket, tunFd);
    return true;
}

void expectVpnFallthroughRuleExists(const std::string& ifName, int vpnNetId) {
    std::string vpnFallthroughRule =
            StringPrintf("%d:\tfrom all fwmark 0x%x/0xffff lookup %s",
                         RULE_PRIORITY_VPN_FALLTHROUGH, vpnNetId, ifName.c_str());
    for (const auto& ipVersion : {IP_RULE_V4, IP_RULE_V6}) {
        EXPECT_TRUE(ipRuleExists(ipVersion, vpnFallthroughRule));
    }
}

void expectVpnFallthroughWorks(android::net::INetd* netdService, bool bypassable, uid_t uid,
                               const TunInterface& fallthroughNetwork,
                               const TunInterface& vpnNetwork, int vpnNetId = TEST_NETID2,
                               int fallthroughNetId = TEST_NETID1) {
    // Set default network to NETID_UNSET
    EXPECT_TRUE(netdService->networkSetDefault(NETID_UNSET).isOk());

    // insideVpnAddr based on the route we added in createVpnNetworkWithUid
    in6_addr insideVpnAddr = {
            {// 2001:db8:cafe::1
             .u6_addr8 = {0x20, 0x01, 0x0d, 0xb8, 0xca, 0xfe, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}}};
    // outsideVpnAddr will hit the route in the fallthrough network route table
    // because we added default route in createVpnNetworkWithUid
    in6_addr outsideVpnAddr = {
            {// 2607:f0d0:1002::4
             .u6_addr8 = {0x26, 0x07, 0xf0, 0xd0, 0x10, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4}}};

    int fallthroughFd = fallthroughNetwork.getFdForTesting();
    int vpnFd = vpnNetwork.getFdForTesting();
    // Expect all connections to fail because UID 0 is not routed to the VPN and there is no
    // default network.
    Fwmark fwmark;
    EXPECT_FALSE(sendIPv6PacketFromUid(0, outsideVpnAddr, &fwmark, fallthroughFd));
    EXPECT_FALSE(sendIPv6PacketFromUid(0, insideVpnAddr, &fwmark, fallthroughFd));

    // Set default network
    EXPECT_TRUE(netdService->networkSetDefault(fallthroughNetId).isOk());

    // Connections go on the default network because UID 0 is not subject to the VPN.
    EXPECT_TRUE(sendIPv6PacketFromUid(0, outsideVpnAddr, &fwmark, fallthroughFd));
    EXPECT_EQ(fallthroughNetId | 0xC0000, static_cast<int>(fwmark.intValue));
    EXPECT_TRUE(sendIPv6PacketFromUid(0, insideVpnAddr, &fwmark, fallthroughFd));
    EXPECT_EQ(fallthroughNetId | 0xC0000, static_cast<int>(fwmark.intValue));

    // Check if fallthrough rule exists
    expectVpnFallthroughRuleExists(fallthroughNetwork.name(), vpnNetId);

    // Expect fallthrough to default network
    // The fwmark differs depending on whether the VPN is bypassable or not.
    EXPECT_TRUE(sendIPv6PacketFromUid(uid, outsideVpnAddr, &fwmark, fallthroughFd));
    EXPECT_EQ(bypassable ? vpnNetId : fallthroughNetId, static_cast<int>(fwmark.intValue));

    // Expect connect success, packet will be sent to vpnFd.
    EXPECT_TRUE(sendIPv6PacketFromUid(uid, insideVpnAddr, &fwmark, vpnFd));
    EXPECT_EQ(bypassable ? vpnNetId : fallthroughNetId, static_cast<int>(fwmark.intValue));

    // Explicitly select vpn network
    setNetworkForProcess(vpnNetId);

    // Expect fallthrough to default network
    EXPECT_TRUE(sendIPv6PacketFromUid(0, outsideVpnAddr, &fwmark, fallthroughFd));
    // Expect the mark contains all the bit because we've selected network.
    EXPECT_EQ(vpnNetId | 0xF0000, static_cast<int>(fwmark.intValue));

    // Expect connect success, packet will be sent to vpnFd.
    EXPECT_TRUE(sendIPv6PacketFromUid(0, insideVpnAddr, &fwmark, vpnFd));
    // Expect the mark contains all the bit because we've selected network.
    EXPECT_EQ(vpnNetId | 0xF0000, static_cast<int>(fwmark.intValue));

    // Explicitly select fallthrough network
    setNetworkForProcess(fallthroughNetId);

    // The mark is set to fallthrough network because we've selected it.
    EXPECT_TRUE(sendIPv6PacketFromUid(0, outsideVpnAddr, &fwmark, fallthroughFd));
    EXPECT_TRUE(sendIPv6PacketFromUid(0, insideVpnAddr, &fwmark, fallthroughFd));

    // If vpn is BypassableVPN, connections can also go on the fallthrough network under vpn uid.
    if (bypassable) {
        EXPECT_TRUE(sendIPv6PacketFromUid(uid, outsideVpnAddr, &fwmark, fallthroughFd));
        EXPECT_TRUE(sendIPv6PacketFromUid(uid, insideVpnAddr, &fwmark, fallthroughFd));
    } else {
        // If not, no permission to bypass vpn.
        EXPECT_FALSE(sendIPv6PacketFromUid(uid, outsideVpnAddr, &fwmark, fallthroughFd));
        EXPECT_FALSE(sendIPv6PacketFromUid(uid, insideVpnAddr, &fwmark, fallthroughFd));
    }
}

}  // namespace

TEST_F(BinderTest, SecureVPNFallthrough) {
    createVpnNetworkWithUid(true /* secure */, TEST_UID1);
    // Get current default network NetId
    ASSERT_TRUE(mNetd->networkGetDefault(&mStoredDefaultNetwork).isOk());
    expectVpnFallthroughWorks(mNetd.get(), false /* bypassable */, TEST_UID1, sTun, sTun2);
}

TEST_F(BinderTest, BypassableVPNFallthrough) {
    createVpnNetworkWithUid(false /* secure */, TEST_UID1);
    // Get current default network NetId
    ASSERT_TRUE(mNetd->networkGetDefault(&mStoredDefaultNetwork).isOk());
    expectVpnFallthroughWorks(mNetd.get(), true /* bypassable */, TEST_UID1, sTun, sTun2);
}