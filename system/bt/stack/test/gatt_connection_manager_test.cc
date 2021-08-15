#include "stack/gatt/connection_manager.h"

#include <base/bind.h>
#include <base/callback.h>
#include <base/location.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include "osi/include/alarm.h"
#include "osi/test/alarm_mock.h"

using testing::_;
using testing::Mock;
using testing::Return;
using testing::SaveArg;

using connection_manager::tAPP_ID;

namespace {
// convinience mock, for verifying whitelist operaitons on lower layer are
// actually scheduled
class WhiteListMock {
 public:
  MOCK_METHOD1(WhiteListAdd, bool(const RawAddress&));
  MOCK_METHOD1(WhiteListRemove, void(const RawAddress&));
  MOCK_METHOD0(WhiteListClear, void());
  MOCK_METHOD0(SetLeConnectionModeToFast, bool());
  MOCK_METHOD0(SetLeConnectionModeToSlow, void());
  MOCK_METHOD2(OnConnectionTimedOut, void(uint8_t, const RawAddress&));
};

std::unique_ptr<WhiteListMock> localWhiteListMock;
}  // namespace

RawAddress address1{{0x01, 0x01, 0x01, 0x01, 0x01, 0x01}};
RawAddress address2{{0x22, 0x22, 0x02, 0x22, 0x33, 0x22}};

constexpr tAPP_ID CLIENT1 = 1;
constexpr tAPP_ID CLIENT2 = 2;
constexpr tAPP_ID CLIENT3 = 3;
constexpr tAPP_ID CLIENT10 = 10;

// Implementation of btm_ble_bgconn.h API for test.
bool BTM_WhiteListAdd(const RawAddress& address) {
  return localWhiteListMock->WhiteListAdd(address);
}

void BTM_WhiteListRemove(const RawAddress& address) {
  return localWhiteListMock->WhiteListRemove(address);
}

void BTM_WhiteListClear() { return localWhiteListMock->WhiteListClear(); }

bool BTM_SetLeConnectionModeToFast() {
  return localWhiteListMock->SetLeConnectionModeToFast();
}

void BTM_SetLeConnectionModeToSlow() {
  localWhiteListMock->SetLeConnectionModeToSlow();
}

namespace connection_manager {
class BleConnectionManager : public testing::Test {
  virtual void SetUp() {
    localWhiteListMock = std::make_unique<WhiteListMock>();
  }

  virtual void TearDown() {
    connection_manager::reset(true);
    AlarmMock::Reset();
    localWhiteListMock.reset();
  }
};

void on_connection_timed_out(uint8_t app_id, const RawAddress& address) {
  localWhiteListMock->OnConnectionTimedOut(app_id, address);
}

/** Verify that app can add a device to white list, it is returned as interested
 * app, and then can remove the device later. */
TEST_F(BleConnectionManager, test_background_connection_add_remove) {
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(0);

  EXPECT_TRUE(background_connect_add(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  std::set<tAPP_ID> apps = get_apps_connecting_to(address1);
  EXPECT_EQ(apps.size(), 1UL);
  EXPECT_EQ(apps.count(CLIENT1), 1UL);

  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(_)).Times(0);
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(address1)).Times(1);

  EXPECT_TRUE(background_connect_remove(CLIENT1, address1));

  EXPECT_EQ(get_apps_connecting_to(address1).size(), 0UL);

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());
}

/** Verify that multiple clients adding same device multiple times, result in
 * device being added to whtie list only once, also, that device is removed only
 * after last client removes it. */
TEST_F(BleConnectionManager, test_background_connection_multiple_clients) {
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(0);
  EXPECT_TRUE(background_connect_add(CLIENT1, address1));
  EXPECT_TRUE(background_connect_add(CLIENT1, address1));
  EXPECT_TRUE(background_connect_add(CLIENT2, address1));
  EXPECT_TRUE(background_connect_add(CLIENT3, address1));

  EXPECT_EQ(get_apps_connecting_to(address1).size(), 3UL);

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(_)).Times(0);

  // removing from nonexisting client, should fail
  EXPECT_FALSE(background_connect_remove(CLIENT10, address1));

  EXPECT_TRUE(background_connect_remove(CLIENT1, address1));
  // already removed,  removing from same client twice should return false;
  EXPECT_FALSE(background_connect_remove(CLIENT1, address1));
  EXPECT_TRUE(background_connect_remove(CLIENT2, address1));

  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(address1)).Times(1);
  EXPECT_TRUE(background_connect_remove(CLIENT3, address1));

  EXPECT_EQ(get_apps_connecting_to(address1).size(), 0UL);

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());
}

/** Verify adding/removing device to direct connection. */
TEST_F(BleConnectionManager, test_direct_connection_client) {
  // Direct connect attempt: use faster scan parameters, add to white list,
  // start 30 timeout
  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToFast()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(0);
  EXPECT_CALL(*AlarmMock::Get(), AlarmNew(_)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmSetOnMloop(_, _, _, _)).Times(1);
  EXPECT_TRUE(direct_connect_add(CLIENT1, address1));

  // App already doing a direct connection, attempt to re-add result in failure
  EXPECT_FALSE(direct_connect_add(CLIENT1, address1));

  // Client that don't do direct connection should fail attempt to stop it
  EXPECT_FALSE(direct_connect_remove(CLIENT2, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToSlow()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmFree(_)).Times(1);

  // Removal should lower the connection parameters, and free the alarm.
  // Even though we call WhiteListRemove, it won't be executed over HCI until
  // whitelist is in use, i.e. next connection attempt
  EXPECT_TRUE(direct_connect_remove(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());
}

/** Verify direct connection timeout does remove device from white list, and
 * lower the connection scan parameters */
TEST_F(BleConnectionManager, test_direct_connect_timeout) {
  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToFast()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*AlarmMock::Get(), AlarmNew(_)).Times(1);
  alarm_callback_t alarm_callback = nullptr;
  void* alarm_data = nullptr;

  EXPECT_CALL(*AlarmMock::Get(), AlarmSetOnMloop(_, _, _, _))
      .Times(1)
      .WillOnce(DoAll(SaveArg<2>(&alarm_callback), SaveArg<3>(&alarm_data)));

  // Start direct connect attempt...
  EXPECT_TRUE(direct_connect_add(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToSlow()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(1);
  EXPECT_CALL(*localWhiteListMock, OnConnectionTimedOut(CLIENT1, address1)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmFree(_)).Times(1);

  // simulate timeout seconds passed, alarm executing
  alarm_callback(alarm_data);

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());
}

/** Verify that we properly handle successfull direct connection */
TEST_F(BleConnectionManager, test_direct_connection_success) {
  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToFast()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*AlarmMock::Get(), AlarmNew(_)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmSetOnMloop(_, _, _, _)).Times(1);

  // Start direct connect attempt...
  EXPECT_TRUE(direct_connect_add(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToSlow()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(address1)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmFree(_)).Times(1);
  // simulate event from lower layers - connections was established
  // successfully.
  on_connection_complete(address1);
}

/** Verify that we properly handle application unregistration */
TEST_F(BleConnectionManager, test_app_unregister) {
  /* Test scenario:
   * - Client 1 connecting to address1 and address2.
   * - Client 2 connecting to address2
   * - unregistration of Client1 should trigger address1 removal from white list
   * - unregistration of Client2 should trigger address2 removal
   */

  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address2))
      .WillOnce(Return(true));
  EXPECT_TRUE(direct_connect_add(CLIENT1, address1));
  EXPECT_TRUE(background_connect_add(CLIENT1, address2));
  EXPECT_TRUE(direct_connect_add(CLIENT2, address2));
  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(address1)).Times(1);
  on_app_deregistered(CLIENT1);
  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(address2)).Times(1);
  on_app_deregistered(CLIENT2);
}

/** Verify adding device to both direct connection and background connection. */
TEST_F(BleConnectionManager, test_direct_and_background_connect) {
  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToFast()).Times(1);
  EXPECT_CALL(*localWhiteListMock, WhiteListAdd(address1))
      .WillOnce(Return(true));
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(0);
  EXPECT_CALL(*AlarmMock::Get(), AlarmNew(_)).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmSetOnMloop(_, _, _, _)).Times(1);
  // add device as both direct and background connection
  EXPECT_TRUE(direct_connect_add(CLIENT1, address1));
  EXPECT_TRUE(background_connect_add(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());

  EXPECT_CALL(*localWhiteListMock, SetLeConnectionModeToSlow()).Times(1);
  EXPECT_CALL(*AlarmMock::Get(), AlarmFree(_)).Times(1);
  // not removing from white list yet, as the background connection is still
  // pending.
  EXPECT_TRUE(direct_connect_remove(CLIENT1, address1));

  // remove from white list, because no more interest in device.
  EXPECT_CALL(*localWhiteListMock, WhiteListRemove(_)).Times(1);
  EXPECT_TRUE(background_connect_remove(CLIENT1, address1));

  Mock::VerifyAndClearExpectations(localWhiteListMock.get());
}

}  // namespace connection_manager
