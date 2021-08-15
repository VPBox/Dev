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
 * RouteControllerTest.cpp - unit tests for RouteController.cpp
 */

#include <gtest/gtest.h>

#include "Fwmark.h"
#include "IptablesBaseTest.h"
#include "NetlinkCommands.h"
#include "RouteController.h"

#include <android-base/stringprintf.h>

using android::base::StringPrintf;

namespace android {
namespace net {

class RouteControllerTest : public IptablesBaseTest {
public:
    RouteControllerTest() {
        RouteController::iptablesRestoreCommandFunction = fakeExecIptablesRestoreCommand;
    }

    int flushRoutes(uint32_t a) {
        return RouteController::flushRoutes(a);
    }
};

TEST_F(RouteControllerTest, TestGetRulePriority) {
    // Expect a rule dump for these two families to contain at least the following priorities.
    for (int family : {AF_INET, AF_INET6 }) {
        std::set<uint32_t> expectedPriorities = {
            0,
            15000,  // RULE_PRIORITY_LEGACY_SYSTEM
            16000,  // RULE_PRIORITY_LEGACY_NETWORK
            32000,  // RULE_PRIORITY_UNREACHABLE
        };

        NetlinkDumpCallback callback = [&expectedPriorities] (const nlmsghdr *nlh) {
            expectedPriorities.erase(getRulePriority(nlh));
        };

        rtmsg rtm = { .rtm_family = static_cast<uint8_t>(family) };
        iovec iov[] = {
            { nullptr, 0           },
            { &rtm,    sizeof(rtm) },
        };

        ASSERT_EQ(0, sendNetlinkRequest(RTM_GETRULE, NETLINK_DUMP_FLAGS,
                                        iov, ARRAY_SIZE(iov), &callback));

        EXPECT_TRUE(expectedPriorities.empty()) <<
            "Did not see rule with priority " << *expectedPriorities.begin() <<
            " in dump for address family " << family;
    }
}

TEST_F(RouteControllerTest, TestRouteFlush) {
    // Pick a table number that's not used by the system.
    const uint32_t table1 = 500;
    const uint32_t table2 = 600;
    static_assert(table1 < RouteController::ROUTE_TABLE_OFFSET_FROM_INDEX,
                  "Test table1 number too large");
    static_assert(table2 < RouteController::ROUTE_TABLE_OFFSET_FROM_INDEX,
                  "Test table2 number too large");

    EXPECT_EQ(0, modifyIpRoute(RTM_NEWROUTE, table1, "lo", "192.0.2.2/32", nullptr));
    EXPECT_EQ(0, modifyIpRoute(RTM_NEWROUTE, table1, "lo", "192.0.2.3/32", nullptr));
    EXPECT_EQ(0, modifyIpRoute(RTM_NEWROUTE, table2, "lo", "192.0.2.4/32", nullptr));

    EXPECT_EQ(0, flushRoutes(table1));

    EXPECT_EQ(-ESRCH,
              modifyIpRoute(RTM_DELROUTE, table1, "lo", "192.0.2.2/32", nullptr));
    EXPECT_EQ(-ESRCH,
              modifyIpRoute(RTM_DELROUTE, table1, "lo", "192.0.2.3/32", nullptr));
    EXPECT_EQ(0,
              modifyIpRoute(RTM_DELROUTE, table2, "lo", "192.0.2.4/32", nullptr));
}

TEST_F(RouteControllerTest, TestModifyIncomingPacketMark) {
  uint32_t mask = ~Fwmark::getUidBillingMask();

  static constexpr int TEST_NETID = 30;
  EXPECT_EQ(0, modifyIncomingPacketMark(TEST_NETID, "netdtest0",
                                        PERMISSION_NONE, true));
  expectIptablesRestoreCommands({StringPrintf(
      "-t mangle -A routectrl_mangle_INPUT -i netdtest0 -j MARK --set-mark "
      "0x3001e/0x%x",
      mask)});

  EXPECT_EQ(0, modifyIncomingPacketMark(TEST_NETID, "netdtest0",
                                        PERMISSION_NONE, false));
  expectIptablesRestoreCommands({StringPrintf(
      "-t mangle -D routectrl_mangle_INPUT -i netdtest0 -j MARK --set-mark "
      "0x3001e/0x%x",
      mask)});
}

}  // namespace net
}  // namespace android
