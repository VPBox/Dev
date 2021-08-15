//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/real_device_policy_provider.h"

#include <memory>
#include <vector>

#include <base/memory/ptr_util.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <policy/mock_device_policy.h>
#include <policy/mock_libpolicy.h>
#if USE_DBUS
#include <session_manager/dbus-proxies.h>
#include <session_manager/dbus-proxy-mocks.h>
#endif  // USE_DBUS

#include "update_engine/common/test_utils.h"
#if USE_DBUS
#include "update_engine/dbus_test_utils.h"
#endif  // USE_DBUS
#include "update_engine/update_manager/umtest_utils.h"

using base::TimeDelta;
using brillo::MessageLoop;
using chromeos_update_engine::ConnectionType;
using policy::DevicePolicy;
#if USE_DBUS
using chromeos_update_engine::dbus_test_utils::MockSignalHandler;
#endif  // USE_DBUS
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;
using testing::_;
using testing::DoAll;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
using testing::SetArgPointee;

namespace chromeos_update_manager {

class UmRealDevicePolicyProviderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    loop_.SetAsCurrent();
#if USE_DBUS
    auto session_manager_proxy_mock =
        new org::chromium::SessionManagerInterfaceProxyMock();
    provider_.reset(new RealDevicePolicyProvider(
        base::WrapUnique(session_manager_proxy_mock), &mock_policy_provider_));
#else
    provider_.reset(new RealDevicePolicyProvider(&mock_policy_provider_));
#endif  // USE_DBUS
    // By default, we have a device policy loaded. Tests can call
    // SetUpNonExistentDevicePolicy() to override this.
    SetUpExistentDevicePolicy();

#if USE_DBUS
    // Setup the session manager_proxy such that it will accept the signal
    // handler and store it in the |property_change_complete_| once registered.
    MOCK_SIGNAL_HANDLER_EXPECT_SIGNAL_HANDLER(property_change_complete_,
                                              *session_manager_proxy_mock,
                                              PropertyChangeComplete);
#endif  // USE_DBUS
  }

  void TearDown() override {
    provider_.reset();
    // Check for leaked callbacks on the main loop.
    EXPECT_FALSE(loop_.PendingTasks());
  }

  void SetUpNonExistentDevicePolicy() {
    ON_CALL(mock_policy_provider_, Reload()).WillByDefault(Return(false));
    ON_CALL(mock_policy_provider_, device_policy_is_loaded())
        .WillByDefault(Return(false));
    EXPECT_CALL(mock_policy_provider_, GetDevicePolicy()).Times(0);
  }

  void SetUpExistentDevicePolicy() {
    // Setup the default behavior of the mocked PolicyProvider.
    ON_CALL(mock_policy_provider_, Reload()).WillByDefault(Return(true));
    ON_CALL(mock_policy_provider_, device_policy_is_loaded())
        .WillByDefault(Return(true));
    ON_CALL(mock_policy_provider_, GetDevicePolicy())
        .WillByDefault(ReturnRef(mock_device_policy_));
  }

  brillo::FakeMessageLoop loop_{nullptr};
  testing::NiceMock<policy::MockDevicePolicy> mock_device_policy_;
  testing::NiceMock<policy::MockPolicyProvider> mock_policy_provider_;
  unique_ptr<RealDevicePolicyProvider> provider_;

#if USE_DBUS
  // The registered signal handler for the signal.
  MockSignalHandler<void(const string&)> property_change_complete_;
#endif  // USE_DBUS
};

TEST_F(UmRealDevicePolicyProviderTest, RefreshScheduledTest) {
  // Check that the RefreshPolicy gets scheduled by checking the TaskId.
  EXPECT_TRUE(provider_->Init());
  EXPECT_NE(MessageLoop::kTaskIdNull, provider_->scheduled_refresh_);
  loop_.RunOnce(false);
}

TEST_F(UmRealDevicePolicyProviderTest, FirstReload) {
  // Checks that the policy is reloaded and the DevicePolicy is consulted twice:
  // once on Init() and once again when the signal is connected.
  EXPECT_CALL(mock_policy_provider_, Reload());
  EXPECT_TRUE(provider_->Init());
  Mock::VerifyAndClearExpectations(&mock_policy_provider_);
  // We won't be notified that signal is connected without DBus.
#if USE_DBUS
  EXPECT_CALL(mock_policy_provider_, Reload());
#endif  // USE_DBUS
  loop_.RunOnce(false);
}

TEST_F(UmRealDevicePolicyProviderTest, NonExistentDevicePolicyReloaded) {
  // Checks that the policy is reloaded by RefreshDevicePolicy().
  SetUpNonExistentDevicePolicy();
  // We won't be notified that signal is connected without DBus.
#if USE_DBUS
  EXPECT_CALL(mock_policy_provider_, Reload()).Times(3);
#else
  EXPECT_CALL(mock_policy_provider_, Reload()).Times(2);
#endif  // USE_DBUS
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);
  // Force the policy refresh.
  provider_->RefreshDevicePolicy();
}

#if USE_DBUS
TEST_F(UmRealDevicePolicyProviderTest, SessionManagerSignalForcesReload) {
  // Checks that a signal from the SessionManager forces a reload.
  SetUpNonExistentDevicePolicy();
  EXPECT_CALL(mock_policy_provider_, Reload()).Times(2);
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);
  Mock::VerifyAndClearExpectations(&mock_policy_provider_);

  EXPECT_CALL(mock_policy_provider_, Reload());
  ASSERT_TRUE(property_change_complete_.IsHandlerRegistered());
  property_change_complete_.signal_callback().Run("success");
}
#endif  // USE_DBUS

TEST_F(UmRealDevicePolicyProviderTest, NonExistentDevicePolicyEmptyVariables) {
  SetUpNonExistentDevicePolicy();
  EXPECT_CALL(mock_policy_provider_, GetDevicePolicy()).Times(0);
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(false,
                                      provider_->var_device_policy_is_loaded());

  UmTestUtils::ExpectVariableNotSet(provider_->var_release_channel());
  UmTestUtils::ExpectVariableNotSet(provider_->var_release_channel_delegated());
  UmTestUtils::ExpectVariableNotSet(provider_->var_update_disabled());
  UmTestUtils::ExpectVariableNotSet(provider_->var_target_version_prefix());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_rollback_to_target_version());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_rollback_allowed_milestones());
  UmTestUtils::ExpectVariableNotSet(provider_->var_scatter_factor());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_allowed_connection_types_for_update());
  UmTestUtils::ExpectVariableNotSet(provider_->var_owner());
  UmTestUtils::ExpectVariableNotSet(provider_->var_http_downloads_enabled());
  UmTestUtils::ExpectVariableNotSet(provider_->var_au_p2p_enabled());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_allow_kiosk_app_control_chrome_version());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_auto_launched_kiosk_app_id());
  UmTestUtils::ExpectVariableNotSet(provider_->var_disallowed_time_intervals());
}

TEST_F(UmRealDevicePolicyProviderTest, ValuesUpdated) {
  SetUpNonExistentDevicePolicy();
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);
  Mock::VerifyAndClearExpectations(&mock_policy_provider_);

  // Reload the policy with a good one and set some values as present. The
  // remaining values are false.
  SetUpExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetReleaseChannel(_))
      .WillOnce(DoAll(SetArgPointee<0>(string("mychannel")), Return(true)));
  EXPECT_CALL(mock_device_policy_, GetAllowedConnectionTypesForUpdate(_))
      .WillOnce(Return(false));
  EXPECT_CALL(mock_device_policy_, GetAllowKioskAppControlChromeVersion(_))
      .WillOnce(DoAll(SetArgPointee<0>(true), Return(true)));
  EXPECT_CALL(mock_device_policy_, GetAutoLaunchedKioskAppId(_))
      .WillOnce(DoAll(SetArgPointee<0>(string("myapp")), Return(true)));

  provider_->RefreshDevicePolicy();

  UmTestUtils::ExpectVariableHasValue(true,
                                      provider_->var_device_policy_is_loaded());

  // Test that at least one variable is set, to ensure the refresh occurred.
  UmTestUtils::ExpectVariableHasValue(string("mychannel"),
                                      provider_->var_release_channel());
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_allowed_connection_types_for_update());
  UmTestUtils::ExpectVariableHasValue(
      true, provider_->var_allow_kiosk_app_control_chrome_version());
  UmTestUtils::ExpectVariableHasValue(
      string("myapp"), provider_->var_auto_launched_kiosk_app_id());
}

TEST_F(UmRealDevicePolicyProviderTest, RollbackToTargetVersionConverted) {
  SetUpExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetRollbackToTargetVersion(_))
#if USE_DBUS
      .Times(2)
#else
      .Times(1)
#endif  // USE_DBUS
      .WillRepeatedly(DoAll(SetArgPointee<0>(2), Return(true)));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(
      RollbackToTargetVersion::kRollbackAndPowerwash,
      provider_->var_rollback_to_target_version());
}

TEST_F(UmRealDevicePolicyProviderTest, RollbackAllowedMilestonesOobe) {
  SetUpNonExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetRollbackAllowedMilestones(_)).Times(0);
  ON_CALL(mock_policy_provider_, IsConsumerDevice())
      .WillByDefault(Return(false));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableNotSet(
      provider_->var_rollback_allowed_milestones());
}

TEST_F(UmRealDevicePolicyProviderTest, RollbackAllowedMilestonesConsumer) {
  SetUpNonExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetRollbackAllowedMilestones(_)).Times(0);
  ON_CALL(mock_policy_provider_, IsConsumerDevice())
      .WillByDefault(Return(true));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(
      0, provider_->var_rollback_allowed_milestones());
}

TEST_F(UmRealDevicePolicyProviderTest,
       RollbackAllowedMilestonesEnterprisePolicySet) {
  SetUpExistentDevicePolicy();
  ON_CALL(mock_device_policy_, GetRollbackAllowedMilestones(_))
      .WillByDefault(DoAll(SetArgPointee<0>(2), Return(true)));
  ON_CALL(mock_policy_provider_, IsConsumerDevice())
      .WillByDefault(Return(false));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(
      2, provider_->var_rollback_allowed_milestones());
}

TEST_F(UmRealDevicePolicyProviderTest, ScatterFactorConverted) {
  SetUpExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetScatterFactorInSeconds(_))
#if USE_DBUS
      .Times(2)
#else
      .Times(1)
#endif  // USE_DBUS
      .WillRepeatedly(DoAll(SetArgPointee<0>(1234), Return(true)));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(TimeDelta::FromSeconds(1234),
                                      provider_->var_scatter_factor());
}

TEST_F(UmRealDevicePolicyProviderTest, NegativeScatterFactorIgnored) {
  SetUpExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetScatterFactorInSeconds(_))
#if USE_DBUS
      .Times(2)
#else
      .Times(1)
#endif  // USE_DBUS
      .WillRepeatedly(DoAll(SetArgPointee<0>(-1), Return(true)));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableNotSet(provider_->var_scatter_factor());
}

TEST_F(UmRealDevicePolicyProviderTest, AllowedTypesConverted) {
  SetUpExistentDevicePolicy();
  EXPECT_CALL(mock_device_policy_, GetAllowedConnectionTypesForUpdate(_))
#if USE_DBUS
      .Times(2)
#else
      .Times(1)
#endif  // USE_DBUS
      .WillRepeatedly(DoAll(
          SetArgPointee<0>(set<string>{"bluetooth", "wifi", "not-a-type"}),
          Return(true)));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(
      set<ConnectionType>{ConnectionType::kWifi, ConnectionType::kBluetooth},
      provider_->var_allowed_connection_types_for_update());
}

TEST_F(UmRealDevicePolicyProviderTest, DisallowedIntervalsConverted) {
  SetUpExistentDevicePolicy();

  vector<DevicePolicy::WeeklyTimeInterval> intervals = {
      {5, TimeDelta::FromHours(5), 6, TimeDelta::FromHours(8)},
      {1, TimeDelta::FromHours(1), 3, TimeDelta::FromHours(10)}};

  EXPECT_CALL(mock_device_policy_, GetDisallowedTimeIntervals(_))
      .WillRepeatedly(DoAll(SetArgPointee<0>(intervals), Return(true)));
  EXPECT_TRUE(provider_->Init());
  loop_.RunOnce(false);

  UmTestUtils::ExpectVariableHasValue(
      WeeklyTimeIntervalVector{
          WeeklyTimeInterval(WeeklyTime(5, TimeDelta::FromHours(5)),
                             WeeklyTime(6, TimeDelta::FromHours(8))),
          WeeklyTimeInterval(WeeklyTime(1, TimeDelta::FromHours(1)),
                             WeeklyTime(3, TimeDelta::FromHours(10)))},
      provider_->var_disallowed_time_intervals());
}

}  // namespace chromeos_update_manager
