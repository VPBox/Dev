/*
 * Copyright 2018 The Android Open Source Project
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
 * ClatdControllerTest.cpp - unit tests for ClatdController.cpp
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include <gtest/gtest.h>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <netutils/ifc.h>

extern "C" {
#include <netutils/checksum.h>
}

#include "ClatdController.h"
#include "IptablesBaseTest.h"
#include "NetworkController.h"
#include "tun_interface.h"

static const char kIPv4LocalAddr[] = "192.0.0.4";

namespace android {
namespace net {

using android::base::StringPrintf;

// Mock functions for isIpv4AddressFree.
bool neverFree(in_addr_t /* addr */) {
    return 0;
}
bool alwaysFree(in_addr_t /* addr */) {
    return 1;
}
bool only2Free(in_addr_t addr) {
    return (ntohl(addr) & 0xff) == 2;
}
bool over6Free(in_addr_t addr) {
    return (ntohl(addr) & 0xff) >= 6;
}
bool only10Free(in_addr_t addr) {
    return (ntohl(addr) & 0xff) == 10;
}

class ClatdControllerTest : public IptablesBaseTest {
  public:
    ClatdControllerTest() : mClatdCtrl(nullptr) {
        ClatdController::iptablesRestoreFunction = fakeExecIptablesRestore;
    }

    void SetUp() { resetIpv4AddressFreeFunc(); }

  protected:
    ClatdController mClatdCtrl;
    bool isEbpfDisabled() { return mClatdCtrl.getEbpfMode() == ClatdController::ClatEbpfDisabled; }
    void maybeSetIptablesDropRule(bool a, const char* b, const char* c) {
        std::lock_guard guard(mClatdCtrl.mutex);
        return mClatdCtrl.maybeSetIptablesDropRule(a, b, c);
    }
    void setIpv4AddressFreeFunc(bool (*func)(in_addr_t)) {
        ClatdController::isIpv4AddressFreeFunc = func;
    }
    void resetIpv4AddressFreeFunc() {
        ClatdController::isIpv4AddressFreeFunc = ClatdController::isIpv4AddressFree;
    }
    in_addr_t selectIpv4Address(const in_addr a, int16_t b) {
        return ClatdController::selectIpv4Address(a, b);
    }
    void makeChecksumNeutral(in6_addr* a, const in_addr b, const in6_addr& c) {
        ClatdController::makeChecksumNeutral(a, b, c);
    }
};

TEST_F(ClatdControllerTest, SelectIpv4Address) {
    struct in_addr addr;

    inet_pton(AF_INET, kIPv4LocalAddr, &addr);

    // If no addresses are free, return INADDR_NONE.
    setIpv4AddressFreeFunc(neverFree);
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 29));
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 16));

    // If the configured address is free, pick that. But a prefix that's too big is invalid.
    setIpv4AddressFreeFunc(alwaysFree);
    EXPECT_EQ(inet_addr(kIPv4LocalAddr), selectIpv4Address(addr, 29));
    EXPECT_EQ(inet_addr(kIPv4LocalAddr), selectIpv4Address(addr, 20));
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 15));

    // A prefix length of 32 works, but anything above it is invalid.
    EXPECT_EQ(inet_addr(kIPv4LocalAddr), selectIpv4Address(addr, 32));
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 33));

    // If another address is free, pick it.
    setIpv4AddressFreeFunc(over6Free);
    EXPECT_EQ(inet_addr("192.0.0.6"), selectIpv4Address(addr, 29));

    // Check that we wrap around to addresses that are lower than the first address.
    setIpv4AddressFreeFunc(only2Free);
    EXPECT_EQ(inet_addr("192.0.0.2"), selectIpv4Address(addr, 29));
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 30));

    // If a free address exists outside the prefix, we don't pick it.
    setIpv4AddressFreeFunc(only10Free);
    EXPECT_EQ(INADDR_NONE, selectIpv4Address(addr, 29));
    EXPECT_EQ(inet_addr("192.0.0.10"), selectIpv4Address(addr, 24));

    // Now try using the real function which sees if IP addresses are free using bind().
    // Assume that the machine running the test has the address 127.0.0.1, but not 8.8.8.8.
    resetIpv4AddressFreeFunc();
    addr.s_addr = inet_addr("8.8.8.8");
    EXPECT_EQ(inet_addr("8.8.8.8"), selectIpv4Address(addr, 29));

    addr.s_addr = inet_addr("127.0.0.1");
    EXPECT_EQ(inet_addr("127.0.0.2"), selectIpv4Address(addr, 29));
}

TEST_F(ClatdControllerTest, MakeChecksumNeutral) {
    // We can't test generateIPv6Address here since it requires manipulating routing, which we can't
    // do without talking to the real netd on the system.
    uint32_t rand = arc4random_uniform(0xffffffff);
    uint16_t rand1 = rand & 0xffff;
    uint16_t rand2 = (rand >> 16) & 0xffff;
    std::string v6PrefixStr = StringPrintf("2001:db8:%x:%x", rand1, rand2);
    std::string v6InterfaceAddrStr = StringPrintf("%s::%x:%x", v6PrefixStr.c_str(), rand2, rand1);
    std::string nat64PrefixStr = StringPrintf("2001:db8:%x:%x::", rand2, rand1);

    in_addr v4 = {inet_addr(kIPv4LocalAddr)};
    in6_addr v6InterfaceAddr;
    ASSERT_TRUE(inet_pton(AF_INET6, v6InterfaceAddrStr.c_str(), &v6InterfaceAddr));
    in6_addr nat64Prefix;
    ASSERT_TRUE(inet_pton(AF_INET6, nat64PrefixStr.c_str(), &nat64Prefix));

    // Generate a boatload of random IIDs.
    int onebits = 0;
    uint64_t prev_iid = 0;
    for (int i = 0; i < 100000; i++) {
        in6_addr v6 = v6InterfaceAddr;
        makeChecksumNeutral(&v6, v4, nat64Prefix);

        // Check the generated IP address is in the same prefix as the interface IPv6 address.
        EXPECT_EQ(0, memcmp(&v6, &v6InterfaceAddr, 8));

        // Check that consecutive IIDs are not the same.
        uint64_t iid = *(uint64_t*)(&v6.s6_addr[8]);
        ASSERT_TRUE(iid != prev_iid)
                << "Two consecutive random IIDs are the same: " << std::showbase << std::hex << iid
                << "\n";
        prev_iid = iid;

        // Check that the IID is checksum-neutral with the NAT64 prefix and the
        // local prefix.
        uint16_t c1 = ip_checksum_finish(ip_checksum_add(0, &v4, sizeof(v4)));
        uint16_t c2 = ip_checksum_finish(ip_checksum_add(0, &nat64Prefix, sizeof(nat64Prefix)) +
                                         ip_checksum_add(0, &v6, sizeof(v6)));

        if (c1 != c2) {
            char v6Str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &v6, v6Str, sizeof(v6Str));
            FAIL() << "Bad IID: " << v6Str << " not checksum-neutral with " << kIPv4LocalAddr
                   << " and " << nat64PrefixStr.c_str() << std::showbase << std::hex
                   << "\n  IPv4 checksum: " << c1 << "\n  IPv6 checksum: " << c2 << "\n";
        }

        // Check that IIDs are roughly random and use all the bits by counting the
        // total number of bits set to 1 in a random sample of 100000 generated IIDs.
        onebits += __builtin_popcountll(*(uint64_t*)&iid);
    }
    EXPECT_LE(3190000, onebits);
    EXPECT_GE(3210000, onebits);
}

TEST_F(ClatdControllerTest, AddRemoveIptablesRule) {
    if (isEbpfDisabled()) return;

    ExpectedIptablesCommands expected = {
            {V6,
             "*raw\n"
             "-A clat_raw_PREROUTING -s 64:ff9b::/96 -d 2001:db8::1:2:3:4 -j DROP\n"
             "COMMIT\n"},
    };
    maybeSetIptablesDropRule(true, "64:ff9b::", "2001:db8::1:2:3:4");
    expectIptablesRestoreCommands(expected);

    expected = {
            {V6,
             "*raw\n"
             "-D clat_raw_PREROUTING -s 64:ff9b::/96 -d 2001:db8::a:b:c:d -j DROP\n"
             "COMMIT\n"},
    };
    maybeSetIptablesDropRule(false, "64:ff9b::", "2001:db8::a:b:c:d");
    expectIptablesRestoreCommands(expected);
}

}  // namespace net
}  // namespace android
