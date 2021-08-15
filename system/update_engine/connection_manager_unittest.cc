//
// Copyright (C) 2012 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/connection_manager.h"

#include <memory>
#include <set>
#include <string>
#include <utility>

#include <base/logging.h>
#include <brillo/any.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/variant_dictionary.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <shill/dbus-constants.h>
#include <shill/dbus-proxies.h>
#include <shill/dbus-proxy-mocks.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/fake_shill_proxy.h"
#include "update_engine/fake_system_state.h"

using chromeos_update_engine::connection_utils::StringForConnectionType;
using org::chromium::flimflam::ManagerProxyMock;
using org::chromium::flimflam::ServiceProxyMock;
using std::set;
using std::string;
using testing::_;
using testing::Return;
using testing::SetArgPointee;

namespace chromeos_update_engine {

class ConnectionManagerTest : public ::testing::Test {
 public:
  ConnectionManagerTest() : fake_shill_proxy_(new FakeShillProxy()) {}

  void SetUp() override {
    loop_.SetAsCurrent();
    fake_system_state_.set_connection_manager(&cmut_);
  }

  void TearDown() override { EXPECT_FALSE(loop_.PendingTasks()); }

 protected:
  // Sets the default_service object path in the response from the
  // ManagerProxyMock instance.
  void SetManagerReply(const char* default_service, bool reply_succeeds);

  // Sets the |service_type|, |physical_technology| and |service_tethering|
  // properties in the mocked service |service_path|. If any of the three
  // const char* is a nullptr, the corresponding property will not be included
  // in the response.
  void SetServiceReply(const string& service_path,
                       const char* service_type,
                       const char* physical_technology,
                       const char* service_tethering);

  void TestWithServiceType(const char* service_type,
                           const char* physical_technology,
                           ConnectionType expected_type);

  void TestWithServiceDisconnected(ConnectionType expected_type);

  void TestWithServiceTethering(const char* service_tethering,
                                ConnectionTethering expected_tethering);

  brillo::FakeMessageLoop loop_{nullptr};
  FakeSystemState fake_system_state_;
  FakeShillProxy* fake_shill_proxy_;

  // ConnectionManager under test.
  ConnectionManager cmut_{fake_shill_proxy_, &fake_system_state_};
};

void ConnectionManagerTest::SetManagerReply(const char* default_service,
                                            bool reply_succeeds) {
  ManagerProxyMock* manager_proxy_mock = fake_shill_proxy_->GetManagerProxy();
  if (!reply_succeeds) {
    EXPECT_CALL(*manager_proxy_mock, GetProperties(_, _, _))
        .WillOnce(Return(false));
    return;
  }

  // Create a dictionary of properties and optionally include the default
  // service.
  brillo::VariantDictionary reply_dict;
  reply_dict["SomeOtherProperty"] = 0xC0FFEE;

  if (default_service) {
    reply_dict[shill::kDefaultServiceProperty] =
        dbus::ObjectPath(default_service);
  }
  EXPECT_CALL(*manager_proxy_mock, GetProperties(_, _, _))
      .WillOnce(DoAll(SetArgPointee<0>(reply_dict), Return(true)));
}

void ConnectionManagerTest::SetServiceReply(const string& service_path,
                                            const char* service_type,
                                            const char* physical_technology,
                                            const char* service_tethering) {
  brillo::VariantDictionary reply_dict;
  reply_dict["SomeOtherProperty"] = 0xC0FFEE;

  if (service_type)
    reply_dict[shill::kTypeProperty] = string(service_type);

  if (physical_technology) {
    reply_dict[shill::kPhysicalTechnologyProperty] =
        string(physical_technology);
  }

  if (service_tethering)
    reply_dict[shill::kTetheringProperty] = string(service_tethering);

  std::unique_ptr<ServiceProxyMock> service_proxy_mock(new ServiceProxyMock());

  // Plumb return value into mock object.
  EXPECT_CALL(*service_proxy_mock.get(), GetProperties(_, _, _))
      .WillOnce(DoAll(SetArgPointee<0>(reply_dict), Return(true)));

  fake_shill_proxy_->SetServiceForPath(dbus::ObjectPath(service_path),
                                       std::move(service_proxy_mock));
}

void ConnectionManagerTest::TestWithServiceType(const char* service_type,
                                                const char* physical_technology,
                                                ConnectionType expected_type) {
  SetManagerReply("/service/guest/network", true);
  SetServiceReply("/service/guest/network",
                  service_type,
                  physical_technology,
                  shill::kTetheringNotDetectedState);

  ConnectionType type;
  ConnectionTethering tethering;
  EXPECT_TRUE(cmut_.GetConnectionProperties(&type, &tethering));
  EXPECT_EQ(expected_type, type);
  testing::Mock::VerifyAndClearExpectations(
      fake_shill_proxy_->GetManagerProxy());
}

void ConnectionManagerTest::TestWithServiceTethering(
    const char* service_tethering, ConnectionTethering expected_tethering) {
  SetManagerReply("/service/guest/network", true);
  SetServiceReply(
      "/service/guest/network", shill::kTypeWifi, nullptr, service_tethering);

  ConnectionType type;
  ConnectionTethering tethering;
  EXPECT_TRUE(cmut_.GetConnectionProperties(&type, &tethering));
  EXPECT_EQ(expected_tethering, tethering);
  testing::Mock::VerifyAndClearExpectations(
      fake_shill_proxy_->GetManagerProxy());
}

void ConnectionManagerTest::TestWithServiceDisconnected(
    ConnectionType expected_type) {
  SetManagerReply("/", true);

  ConnectionType type;
  ConnectionTethering tethering;
  EXPECT_TRUE(cmut_.GetConnectionProperties(&type, &tethering));
  EXPECT_EQ(expected_type, type);
  testing::Mock::VerifyAndClearExpectations(
      fake_shill_proxy_->GetManagerProxy());
}

TEST_F(ConnectionManagerTest, SimpleTest) {
  TestWithServiceType(shill::kTypeEthernet, nullptr, ConnectionType::kEthernet);
  TestWithServiceType(shill::kTypeWifi, nullptr, ConnectionType::kWifi);
  TestWithServiceType(shill::kTypeWimax, nullptr, ConnectionType::kWimax);
  TestWithServiceType(
      shill::kTypeBluetooth, nullptr, ConnectionType::kBluetooth);
  TestWithServiceType(shill::kTypeCellular, nullptr, ConnectionType::kCellular);
}

TEST_F(ConnectionManagerTest, PhysicalTechnologyTest) {
  TestWithServiceType(shill::kTypeVPN, nullptr, ConnectionType::kUnknown);
  TestWithServiceType(
      shill::kTypeVPN, shill::kTypeVPN, ConnectionType::kUnknown);
  TestWithServiceType(shill::kTypeVPN, shill::kTypeWifi, ConnectionType::kWifi);
  TestWithServiceType(
      shill::kTypeVPN, shill::kTypeWimax, ConnectionType::kWimax);
}

TEST_F(ConnectionManagerTest, TetheringTest) {
  TestWithServiceTethering(shill::kTetheringConfirmedState,
                           ConnectionTethering::kConfirmed);
  TestWithServiceTethering(shill::kTetheringNotDetectedState,
                           ConnectionTethering::kNotDetected);
  TestWithServiceTethering(shill::kTetheringSuspectedState,
                           ConnectionTethering::kSuspected);
  TestWithServiceTethering("I'm not a valid property value =)",
                           ConnectionTethering::kUnknown);
}

TEST_F(ConnectionManagerTest, UnknownTest) {
  TestWithServiceType("foo", nullptr, ConnectionType::kUnknown);
}

TEST_F(ConnectionManagerTest, DisconnectTest) {
  TestWithServiceDisconnected(ConnectionType::kDisconnected);
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverEthernetTest) {
  // Updates over Ethernet are allowed even if there's no policy.
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kEthernet,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverWifiTest) {
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWifi,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverWimaxTest) {
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWimax,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, BlockUpdatesOverBluetoothTest) {
  EXPECT_FALSE(cmut_.IsUpdateAllowedOver(ConnectionType::kBluetooth,
                                         ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOnlyOver3GPerPolicyTest) {
  policy::MockDevicePolicy allow_3g_policy;

  fake_system_state_.set_device_policy(&allow_3g_policy);

  // This test tests cellular (3G) being the only connection type being allowed.
  set<string> allowed_set;
  allowed_set.insert(StringForConnectionType(ConnectionType::kCellular));

  EXPECT_CALL(allow_3g_policy, GetAllowedConnectionTypesForUpdate(_))
      .Times(1)
      .WillOnce(DoAll(SetArgPointee<0>(allowed_set), Return(true)));

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOver3GAndOtherTypesPerPolicyTest) {
  policy::MockDevicePolicy allow_3g_policy;

  fake_system_state_.set_device_policy(&allow_3g_policy);

  // This test tests multiple connection types being allowed, with
  // 3G one among them. Only Cellular is currently enforced by the policy
  // setting, the others are ignored (see Bluetooth for example).
  set<string> allowed_set;
  allowed_set.insert(StringForConnectionType(ConnectionType::kCellular));
  allowed_set.insert(StringForConnectionType(ConnectionType::kBluetooth));

  EXPECT_CALL(allow_3g_policy, GetAllowedConnectionTypesForUpdate(_))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgPointee<0>(allowed_set), Return(true)));

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kEthernet,
                                        ConnectionTethering::kUnknown));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kEthernet,
                                        ConnectionTethering::kNotDetected));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                        ConnectionTethering::kUnknown));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWifi,
                                        ConnectionTethering::kUnknown));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWimax,
                                        ConnectionTethering::kUnknown));
  EXPECT_FALSE(cmut_.IsUpdateAllowedOver(ConnectionType::kBluetooth,
                                         ConnectionTethering::kUnknown));

  // Tethered networks are treated in the same way as Cellular networks and
  // thus allowed.
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kEthernet,
                                        ConnectionTethering::kConfirmed));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWifi,
                                        ConnectionTethering::kConfirmed));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverCellularByDefaultTest) {
  policy::MockDevicePolicy device_policy;
  // Set an empty device policy.
  fake_system_state_.set_device_policy(&device_policy);

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverTetheredNetworkByDefaultTest) {
  policy::MockDevicePolicy device_policy;
  // Set an empty device policy.
  fake_system_state_.set_device_policy(&device_policy);

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWifi,
                                        ConnectionTethering::kConfirmed));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kEthernet,
                                        ConnectionTethering::kConfirmed));
  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kWifi,
                                        ConnectionTethering::kSuspected));
}

TEST_F(ConnectionManagerTest, BlockUpdatesOver3GPerPolicyTest) {
  policy::MockDevicePolicy block_3g_policy;

  fake_system_state_.set_device_policy(&block_3g_policy);

  // Test that updates for 3G are blocked while updates are allowed
  // over several other types.
  set<string> allowed_set;
  allowed_set.insert(StringForConnectionType(ConnectionType::kEthernet));
  allowed_set.insert(StringForConnectionType(ConnectionType::kWifi));
  allowed_set.insert(StringForConnectionType(ConnectionType::kWimax));

  EXPECT_CALL(block_3g_policy, GetAllowedConnectionTypesForUpdate(_))
      .Times(1)
      .WillOnce(DoAll(SetArgPointee<0>(allowed_set), Return(true)));

  EXPECT_FALSE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                         ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOver3GIfPolicyIsNotSet) {
  policy::MockDevicePolicy device_policy;

  fake_system_state_.set_device_policy(&device_policy);

  // Return false for GetAllowedConnectionTypesForUpdate and see
  // that updates are allowed as device policy is not set. Further
  // check is left to |OmahaRequestAction|.
  EXPECT_CALL(device_policy, GetAllowedConnectionTypesForUpdate(_))
      .Times(1)
      .WillOnce(Return(false));

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, AllowUpdatesOverCellularIfPolicyFailsToBeLoaded) {
  fake_system_state_.set_device_policy(nullptr);

  EXPECT_TRUE(cmut_.IsUpdateAllowedOver(ConnectionType::kCellular,
                                        ConnectionTethering::kUnknown));
}

TEST_F(ConnectionManagerTest, StringForConnectionTypeTest) {
  EXPECT_STREQ(shill::kTypeEthernet,
               StringForConnectionType(ConnectionType::kEthernet));
  EXPECT_STREQ(shill::kTypeWifi,
               StringForConnectionType(ConnectionType::kWifi));
  EXPECT_STREQ(shill::kTypeWimax,
               StringForConnectionType(ConnectionType::kWimax));
  EXPECT_STREQ(shill::kTypeBluetooth,
               StringForConnectionType(ConnectionType::kBluetooth));
  EXPECT_STREQ(shill::kTypeCellular,
               StringForConnectionType(ConnectionType::kCellular));
  EXPECT_STREQ("Unknown", StringForConnectionType(ConnectionType::kUnknown));
  EXPECT_STREQ("Unknown",
               StringForConnectionType(static_cast<ConnectionType>(999999)));
}

TEST_F(ConnectionManagerTest, MalformedServiceList) {
  SetManagerReply("/service/guest/network", false);

  ConnectionType type;
  ConnectionTethering tethering;
  EXPECT_FALSE(cmut_.GetConnectionProperties(&type, &tethering));
}

}  // namespace chromeos_update_engine
