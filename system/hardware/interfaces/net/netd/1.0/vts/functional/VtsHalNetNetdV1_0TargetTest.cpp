/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_TAG "netd_hidl_test"

#include <VtsHalHidlTargetTestBase.h>
#include <android/system/net/netd/1.0/INetd.h>
#include <log/log.h>

#include "VtsHalNetNetdTestUtils.h"

using ::android::system::net::netd::V1_0::INetd;
using ::android::hardware::Return;
using ::android::sp;

// Test environment for Netd HIDL HAL.
class NetdHidlEnvironment : public ::testing::VtsHalHidlTargetTestEnvBase {
   public:
    // get the test environment singleton
    static NetdHidlEnvironment* Instance() {
        static NetdHidlEnvironment* instance = new NetdHidlEnvironment;
        return instance;
    }

    virtual void registerTestServices() override { registerTestService<INetd>(); }

   private:
    NetdHidlEnvironment() {}
};

class NetdHidlTest : public ::testing::VtsHalHidlTargetTestBase {
   public:
    virtual void SetUp() override {
        netd = ::testing::VtsHalHidlTargetTestBase::getService<INetd>(
            NetdHidlEnvironment::Instance()->getServiceName<INetd>());
        ASSERT_NE(netd, nullptr) << "Could not get HIDL instance";
    }

    sp<INetd> netd;
};

// positive test. Ensure netd creates an oem network and returns valid netHandle, and destroys it.
TEST_F(NetdHidlTest, TestCreateAndDestroyOemNetworkOk) {
    net_handle_t netHandle;
    uint32_t packetMark;
    INetd::StatusCode status;

    Return<void> ret = netd->createOemNetwork([&](net_handle_t n, uint32_t p, INetd::StatusCode s) {
        status = s;
        netHandle = n;
        packetMark = p;
    });

    ASSERT_TRUE(ret.isOk());
    ASSERT_EQ(INetd::StatusCode::OK, status);
    ASSERT_NE(NETWORK_UNSPECIFIED, netHandle);
    ASSERT_NE((uint32_t)0, packetMark);

    ASSERT_EQ(0, checkNetworkExists(netHandle));
    ASSERT_EQ(0, countRulesForFwmark(packetMark));

    Return<INetd::StatusCode> retStatus = netd->destroyOemNetwork(netHandle);
    ASSERT_EQ(INetd::StatusCode::OK, retStatus);

    ASSERT_EQ(-ENONET, checkNetworkExists(netHandle));
}

// negative test. Ensure destroy for invalid OEM network fails appropriately
TEST_F(NetdHidlTest, TestDestroyOemNetworkInvalid) {
    const uint64_t nh = 0x6600FACADE;

    Return<INetd::StatusCode> retStatus = netd->destroyOemNetwork(nh);
    ASSERT_EQ(INetd::StatusCode::INVALID_ARGUMENTS, retStatus);
}

int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(NetdHidlEnvironment::Instance());
    ::testing::InitGoogleTest(&argc, argv);
    NetdHidlEnvironment::Instance()->init(&argc, argv);
    int status = RUN_ALL_TESTS();
    ALOGE("Test result with status=%d", status);
    return status;
}
