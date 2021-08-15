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

#include <ifaddrs.h>
#include <net/if.h>
#include <sys/types.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <netdutils/MockSyscalls.h>

#include "InterfaceController.h"

using testing::ByMove;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::_;

namespace android {
namespace net {
namespace {

using netdutils::Fd;
using netdutils::ScopedMockSyscalls;
using netdutils::Slice;
using netdutils::Status;
using netdutils::StatusOr;
using netdutils::UniqueFd;
using netdutils::makeSlice;
using netdutils::status::ok;
using netdutils::statusFromErrno;

constexpr Fd kDevRandomFd(777);
constexpr Fd kStableSecretFd(9999);
const char kDevRandomPath[] = "/dev/random";
const char kTestIface[] = "wlan5";
const char kStableSecretProperty[] = "persist.netd.stable_secret";
const char kStableSecretPath[] = "/proc/sys/net/ipv6/conf/wlan5/stable_secret";
const char kTestIPv6Address[] = "\x20\x01\x0d\xb8\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10";
const char kTestIPv6AddressString[] = "2001:db8:506:708:90a:b0c:d0e:f10";

// getProperty() and setProperty() are forwarded to this mock
class MockProperties {
  public:
    MOCK_CONST_METHOD2(get, std::string(const std::string& key, const std::string& dflt));
    MOCK_CONST_METHOD2(set, Status(const std::string& key, const std::string& val));
};

}  // namespace

class StablePrivacyTest : public testing::Test {
  protected:
    void expectOpenFile(const std::string& path, const Fd fd, int err) {
        if (err == 0) {
            EXPECT_CALL(mSyscalls, open(path, _, _)).WillOnce(Return(ByMove(UniqueFd(fd))));
            EXPECT_CALL(mSyscalls, close(fd)).WillOnce(Return(ok));
        } else {
            EXPECT_CALL(mSyscalls, open(path, _, _))
                .WillOnce(Return(ByMove(statusFromErrno(err, "open() failed"))));
        }
    }

    void expectReadFromDevRandom(const std::string& data) {
        expectOpenFile(kDevRandomPath, kDevRandomFd, 0);
        EXPECT_CALL(mSyscalls, read(kDevRandomFd, _)).WillOnce(Invoke([data](Fd, const Slice buf) {
            EXPECT_EQ(data.size(), buf.size());
            return take(buf, copy(buf, makeSlice(data)));
        }));
    }

    void expectGetPropertyDefault(const std::string& key) {
        EXPECT_CALL(mProperties, get(key, _))
            .WillOnce(Invoke([](const std::string&, const std::string& dflt) { return dflt; }));
    }

    void expectGetProperty(const std::string& key, const std::string& val) {
        EXPECT_CALL(mProperties, get(key, _))
            .WillOnce(Invoke([val](const std::string&, const std::string&) { return val; }));
    }

    void expectSetProperty(const std::string& key, const std::string& val, Status status) {
        EXPECT_CALL(mProperties, set(key, val)).WillOnce(Return(std::move(status)));
    }

    void expectWriteToFile(const Fd fd, const std::string& val, int err) {
        EXPECT_CALL(mSyscalls, write(fd, _))
            .WillOnce(Invoke([val, err](Fd, const Slice buf) -> StatusOr<size_t> {
                EXPECT_EQ(val, toString(buf));
                if (err) {
                    return statusFromErrno(err, "write() failed");
                }
                return val.size();
            }));
    }

    Status enableStablePrivacyAddresses(const std::string& iface) {
        return InterfaceController::enableStablePrivacyAddresses(iface, mGet, mSet);
    }

    StrictMock<ScopedMockSyscalls> mSyscalls;
    StrictMock<MockProperties> mProperties;

    const std::function<std::string(const std::string&, const std::string&)> mGet =
        [this](const std::string& key, const std::string& dflt) {
            return mProperties.get(key, dflt);
        };
    const std::function<Status(const std::string&, const std::string&)> mSet =
        [this](const std::string& key, const std::string& val) {
            return mProperties.set(key, val);
        };
};

TEST_F(StablePrivacyTest, PropertyOpenEnoent) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, ENOENT);
    EXPECT_NE(ok, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, PropertyOpenEaccess) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, EACCES);
    EXPECT_NE(ok, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, FirstBootWriteOkSetPropertyOk) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, 0);
    expectGetPropertyDefault(kStableSecretProperty);
    expectReadFromDevRandom(kTestIPv6Address);
    expectWriteToFile(kStableSecretFd, kTestIPv6AddressString, 0);
    expectSetProperty(kStableSecretProperty, kTestIPv6AddressString, ok);
    EXPECT_EQ(ok, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, FirstBootWriteOkSetPropertyFail) {
    const auto kError = statusFromErrno(EINVAL, "");
    expectOpenFile(kStableSecretPath, kStableSecretFd, 0);
    expectGetPropertyDefault(kStableSecretProperty);
    expectReadFromDevRandom(kTestIPv6Address);
    expectWriteToFile(kStableSecretFd, kTestIPv6AddressString, 0);
    expectSetProperty(kStableSecretProperty, kTestIPv6AddressString, kError);
    EXPECT_EQ(kError, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, FirstBootWriteFail) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, 0);
    expectGetPropertyDefault(kStableSecretProperty);
    expectReadFromDevRandom(kTestIPv6Address);
    expectWriteToFile(kStableSecretFd, kTestIPv6AddressString, ENOSPC);
    EXPECT_NE(ok, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, ExistingPropertyWriteOk) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, 0);
    expectGetProperty(kStableSecretProperty, kTestIPv6AddressString);
    expectWriteToFile(kStableSecretFd, kTestIPv6AddressString, 0);
    EXPECT_EQ(ok, enableStablePrivacyAddresses(kTestIface));
}

TEST_F(StablePrivacyTest, ExistingPropertyWriteFail) {
    expectOpenFile(kStableSecretPath, kStableSecretFd, 0);
    expectGetProperty(kStableSecretProperty, kTestIPv6AddressString);
    expectWriteToFile(kStableSecretFd, kTestIPv6AddressString, EACCES);
    EXPECT_NE(ok, enableStablePrivacyAddresses(kTestIface));
}

class GetIfaceListTest : public testing::Test {};

TEST_F(GetIfaceListTest, IfaceNames) {
    StatusOr<std::vector<std::string>> ifaceNames = InterfaceController::getIfaceNames();
    EXPECT_EQ(ok, ifaceNames.status());
    struct ifaddrs *ifaddr, *ifa;
    EXPECT_EQ(0, getifaddrs(&ifaddr));
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        const auto val = std::find(
                ifaceNames.value().begin(), ifaceNames.value().end(), ifa->ifa_name);
        EXPECT_NE(ifaceNames.value().end(), val);
    }
    freeifaddrs(ifaddr);
}

TEST_F(GetIfaceListTest, IfaceExist) {
    StatusOr<std::map<std::string, uint32_t>> ifaceMap = InterfaceController::getIfaceList();
    EXPECT_EQ(ok, ifaceMap.status());
    struct ifaddrs *ifaddr, *ifa;
    EXPECT_EQ(0, getifaddrs(&ifaddr));
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        uint32_t ifaceIndex = if_nametoindex(ifa->ifa_name);
        const auto ifacePair = ifaceMap.value().find(ifa->ifa_name);
        EXPECT_NE(ifaceMap.value().end(), ifacePair);
        EXPECT_EQ(ifaceIndex, ifacePair->second);
    }
    freeifaddrs(ifaddr);
}

}  // namespace net
}  // namespace android
