/******************************************************************************
 *
 *  Copyright 2016 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <array>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "device/include/controller.h"
#include "stack/btm/ble_advertiser_hci_interface.h"
#include "stack/include/ble_advertiser.h"

using ::testing::Args;
using ::testing::Contains;
using ::testing::ElementsAreArray;
using ::testing::Exactly;
using ::testing::Field;
using ::testing::IsEmpty;
using ::testing::SaveArg;
using ::testing::SizeIs;
using ::testing::_;
using base::Bind;
using status_cb = BleAdvertiserHciInterface::status_cb;
using parameters_cb = BleAdvertiserHciInterface::parameters_cb;
using SetEnableData = BleAdvertiserHciInterface::SetEnableData;

const int num_adv_instances = 16;

/* Below are methods that must be implemented if we don't want to compile the
 * whole stack. They will be removed, or changed into mocks one by one in the
 * future, as the refactoring progresses */
bool BTM_BleLocalPrivacyEnabled() { return true; }
uint16_t BTM_ReadDiscoverability(uint16_t* p_window, uint16_t* p_interval) {
  return true;
}
void btm_acl_update_conn_addr(uint16_t conn_handle, const RawAddress& address) {
}
void btm_gen_resolvable_private_addr(
    base::Callback<void(const RawAddress& rpa)> cb) {
  cb.Run(RawAddress::kEmpty);
}

alarm_callback_t last_alarm_cb = nullptr;
void* last_alarm_data = nullptr;
void alarm_set_on_mloop(alarm_t* alarm, uint64_t interval_ms,
                        alarm_callback_t cb, void* data) {
  last_alarm_cb = cb;
  last_alarm_data = data;
}

void alarm_cancel(alarm_t* alarm) {}
alarm_t* alarm_new_periodic(const char* name) { return nullptr; }
alarm_t* alarm_new(const char* name) { return nullptr; }
void alarm_free(alarm_t* alarm) {}
const controller_t* controller_get_interface() { return nullptr; }

namespace {
void DoNothing(uint8_t) {}

void DoNothing2(uint8_t, uint8_t) {}

void TriggerRandomAddressUpdate() {
  // Call to StartAdvertisingSet set the last_alarm_cb to random address timeout
  // callback. Call it now in order to trigger address update
  last_alarm_cb(last_alarm_data);
}

constexpr uint8_t INTERMEDIATE =
    0x00;                           // Intermediate fragment of fragmented data
constexpr uint8_t FIRST = 0x01;     // First fragment of fragmented data
constexpr uint8_t LAST = 0x02;      // Last fragment of fragmented data
constexpr uint8_t COMPLETE = 0x03;  // Complete extended advertising data

class AdvertiserHciMock : public BleAdvertiserHciInterface {
 public:
  AdvertiserHciMock() = default;
  ~AdvertiserHciMock() override = default;

  MOCK_METHOD1(ReadInstanceCount,
               void(base::Callback<void(uint8_t /* inst_cnt*/)>));
  MOCK_METHOD1(SetAdvertisingEventObserver,
               void(AdvertisingEventObserver* observer));
  MOCK_METHOD6(SetAdvertisingData,
               void(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, status_cb));
  MOCK_METHOD6(SetScanResponseData,
               void(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, status_cb));
  MOCK_METHOD3(SetRandomAddress, void(uint8_t, const RawAddress&, status_cb));
  MOCK_METHOD3(Enable, void(uint8_t, std::vector<SetEnableData>, status_cb));
  MOCK_METHOD5(SetPeriodicAdvertisingParameters,
               void(uint8_t, uint16_t, uint16_t, uint16_t, status_cb));
  MOCK_METHOD5(SetPeriodicAdvertisingData,
               void(uint8_t, uint8_t, uint8_t, uint8_t*, status_cb));
  MOCK_METHOD3(SetPeriodicAdvertisingEnable, void(uint8_t, uint8_t, status_cb));
  MOCK_METHOD2(RemoveAdvertisingSet, void(uint8_t, status_cb));
  MOCK_METHOD1(ClearAdvertisingSets, void(status_cb));

  MOCK_METHOD9(SetParameters1,
               void(uint8_t, uint16_t, uint32_t, uint32_t, uint8_t, uint8_t,
                    const RawAddress&, uint8_t, const RawAddress&));
  MOCK_METHOD8(SetParameters2, void(uint8_t, int8_t, uint8_t, uint8_t, uint8_t,
                                    uint8_t, uint8_t, parameters_cb));

  void SetParameters(uint8_t handle, uint16_t properties, uint32_t adv_int_min,
                     uint32_t adv_int_max, uint8_t channel_map,
                     uint8_t own_address_type, const RawAddress& own_address,
                     uint8_t peer_address_type, const RawAddress& peer_address,
                     uint8_t filter_policy, int8_t tx_power,
                     uint8_t primary_phy, uint8_t secondary_max_skip,
                     uint8_t secondary_phy, uint8_t advertising_sid,
                     uint8_t scan_request_notify_enable,
                     parameters_cb cmd_complete) override {
    SetParameters1(handle, properties, adv_int_min, adv_int_max, channel_map,
                   own_address_type, own_address, peer_address_type,
                   peer_address);
    SetParameters2(filter_policy, tx_power, primary_phy, secondary_max_skip,
                   secondary_phy, advertising_sid, scan_request_notify_enable,
                   cmd_complete);
  };

  bool QuirkAdvertiserZeroHandle() { return false; }

 private:
  DISALLOW_COPY_AND_ASSIGN(AdvertiserHciMock);
};

}  // namespace

class BleAdvertisingManagerTest : public testing::Test {
 protected:
  int reg_inst_id = -1;
  int reg_status = -1;
  int set_params_status = -1;
  int set_data_status = -1;
  int enable_status = -1;
  int start_advertising_status = -1;
  int start_advertising_set_advertiser_id = -1;
  int start_advertising_set_tx_power = -1;
  int start_advertising_set_status = -1;

  std::unique_ptr<AdvertiserHciMock> hci_mock;

  virtual void SetUp() {
    hci_mock.reset(new AdvertiserHciMock());

    base::Callback<void(uint8_t)> inst_cnt_Cb;
    EXPECT_CALL(*hci_mock, ReadInstanceCount(_))
        .Times(Exactly(1))
        .WillOnce(SaveArg<0>(&inst_cnt_Cb));

    BleAdvertisingManager::Initialize(hci_mock.get());
    ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

    // we are a truly gracious fake controller, let the command succeed!
    inst_cnt_Cb.Run(num_adv_instances);
  }

  virtual void TearDown() {
    BleAdvertisingManager::CleanUp();
    hci_mock.reset();
  }

 public:
  void RegistrationCb(uint8_t inst_id, uint8_t status) {
    reg_inst_id = inst_id;
    reg_status = status;
  }

  void SetParametersCb(uint8_t status, int8_t tx_power) {
    set_params_status = status;
  }
  void SetDataCb(uint8_t status) { set_data_status = status; }
  void EnableCb(uint8_t status) { enable_status = status; }
  void StartAdvertisingCb(uint8_t status) { start_advertising_status = status; }
  void StartAdvertisingSetCb(uint8_t advertiser_id, int8_t tx_power,
                             uint8_t status) {
    start_advertising_set_advertiser_id = advertiser_id;
    start_advertising_set_tx_power = tx_power;
    start_advertising_set_status = status;
  }
};

TEST_F(BleAdvertisingManagerTest, test_registration) {
  for (int i = 0; i < num_adv_instances; i++) {
    BleAdvertisingManager::Get()->RegisterAdvertiser(Bind(
        &BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
    EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
    EXPECT_EQ(i, reg_inst_id);
  }

  // This call should return an error - no more advertisers left.
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(ADVERTISE_FAILED_TOO_MANY_ADVERTISERS, reg_status);
  // Don't bother checking inst_id, it doesn't matter

  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(_, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(5);
  remove_cb.Run(0);

  // One advertiser was freed, so should be able to register one now
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  EXPECT_EQ(5, reg_inst_id);
}

/* This test verifies that the following flow is working correctly: register,
 * set parameters, set data, enable, ... (advertise) ..., unregister*/
TEST_F(BleAdvertisingManagerTest, test_android_flow) {
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  int advertiser_id = reg_inst_id;

  parameters_cb set_params_cb;
  tBTM_BLE_ADV_PARAMS params;
  EXPECT_CALL(*hci_mock, SetParameters1(advertiser_id, _, _, _, _, _, _, _, _))
      .Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  BleAdvertisingManager::Get()->SetParameters(
      advertiser_id, &params,
      Bind(&BleAdvertisingManagerTest::SetParametersCb,
           base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // we are a truly gracious fake controller, let the command succeed!
  set_params_cb.Run(0, 0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_params_status);

  status_cb set_data_cb;
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, std::vector<uint8_t>(),
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  set_data_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  status_cb enable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x01 /* enable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));
  BleAdvertisingManager::Get()->Enable(
      advertiser_id, true,
      Bind(&BleAdvertisingManagerTest::EnableCb, base::Unretained(this)), 0, 0,
      base::Callback<void(uint8_t)>());
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  enable_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, enable_status);

  /* fake controller should be advertising */

  EXPECT_CALL(*hci_mock,
              Enable(0x00 /* disable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));
  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(_, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(advertiser_id);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  enable_cb.Run(0);
  remove_cb.Run(0);
}

/* This test verifies that when advertising data is set, tx power and flags will
 * be properly filled. */
TEST_F(BleAdvertisingManagerTest, test_adv_data_filling) {
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  int advertiser_id = reg_inst_id;

  parameters_cb set_params_cb;
  tBTM_BLE_ADV_PARAMS params;
  params.advertising_event_properties =
      BleAdvertisingManager::advertising_prop_legacy_connectable;
  params.tx_power = -15;
  EXPECT_CALL(*hci_mock, SetParameters1(advertiser_id, _, _, _, _, _, _, _, _))
      .Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, params.tx_power, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  BleAdvertisingManager::Get()->SetParameters(
      advertiser_id, &params,
      Bind(&BleAdvertisingManagerTest::SetParametersCb,
           base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // let the set parameters command succeed!
  set_params_cb.Run(0, 0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_params_status);

  status_cb set_data_cb;
  /* verify that flags will be added, and tx power filled, if call to SetData
   * contained only tx power, and the advertisement is connectable */
  uint8_t expected_adv_data[] = {
      0x02 /* len */,         0x01 /* flags */,
      0x02 /* flags value */, 0x02 /* len */,
      0x0A /* tx_power */,    static_cast<uint8_t>(params.tx_power)};
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, _, _, _, _, _))
      .With(Args<4, 3>(ElementsAreArray(expected_adv_data)))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false,
      std::vector<uint8_t>({0x02 /* len */, 0x0A /* tx_power */, 0x00}),
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  set_data_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);
}

/* This test verifies that when advertising is non-connectable, flags will not
 * be added. */
TEST_F(BleAdvertisingManagerTest, test_adv_data_not_filling) {
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  int advertiser_id = reg_inst_id;

  parameters_cb set_params_cb;
  tBTM_BLE_ADV_PARAMS params;
  params.advertising_event_properties =
      BleAdvertisingManager::advertising_prop_legacy_non_connectable;
  params.tx_power = -15;
  EXPECT_CALL(*hci_mock, SetParameters1(advertiser_id, _, _, _, _, _, _, _, _))
      .Times(1);
  EXPECT_CALL(*hci_mock,
              SetParameters2(_, (uint8_t)params.tx_power, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  BleAdvertisingManager::Get()->SetParameters(
      advertiser_id, &params,
      Bind(&BleAdvertisingManagerTest::SetParametersCb,
           base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // let the set parameters command succeed!
  set_params_cb.Run(0, -15);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_params_status);

  status_cb set_data_cb;
  /* verify that flags will not be added */
  uint8_t expected_adv_data[] = {
      0x02 /* len */, 0xFF /* manufacturer specific */, 0x01 /* data */};
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, _, _, _, _, _))
      .With(Args<4, 3>(ElementsAreArray(expected_adv_data)))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, std::vector<uint8_t>({0x02 /* len */, 0xFF, 0x01}),
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  set_data_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);
}

TEST_F(BleAdvertisingManagerTest, test_reenabling) {
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  EXPECT_EQ(0, reg_inst_id);

  uint8_t advertiser_id = reg_inst_id;
  status_cb enable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x01 /* enable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));
  BleAdvertisingManager::Get()->Enable(advertiser_id, true, Bind(DoNothing), 0,
                                       0, Bind(DoNothing));
  enable_cb.Run(0);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  EXPECT_CALL(*hci_mock,
              Enable(0x01 /* enable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));
  BleAdvertisingManager::Get()->OnAdvertisingSetTerminated(advertiser_id, 0x00,
                                                           0x01ed, 0x00);
  enable_cb.Run(0);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
}

/* Make sure that instance is not reenabled if it's already disabled */
TEST_F(BleAdvertisingManagerTest, test_reenabling_disabled_instance) {
  uint8_t advertiser_id = 1;  // any unregistered value

  EXPECT_CALL(*hci_mock, Enable(_, _, _)).Times(Exactly(0));
  BleAdvertisingManager::Get()->OnAdvertisingSetTerminated(advertiser_id, 0x00,
                                                           0x05, 0x00);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
}

/* This test verifies that the only flow that is currently used on Android, is
 * working correctly in happy case scenario. */
TEST_F(BleAdvertisingManagerTest, test_start_advertising_set) {
  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  periodic_params.enable = false;
  std::vector<uint8_t> periodic_data;

  parameters_cb set_params_cb;
  status_cb set_address_cb;
  status_cb set_data_cb;
  status_cb set_scan_resp_data_cb;
  status_cb enable_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(_, _, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetScanResponseData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_scan_resp_data_cb));
  EXPECT_CALL(*hci_mock, Enable(0x01 /* enable */, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));

  BleAdvertisingManager::Get()->StartAdvertisingSet(
      Bind(&BleAdvertisingManagerTest::StartAdvertisingSetCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, &periodic_params, periodic_data,
      0 /* duration */, 0 /* maxExtAdvEvents */, Bind(DoNothing2));

  // we are a truly gracious fake controller, let the commands succeed!
  int selected_tx_power = -15;
  set_params_cb.Run(0, selected_tx_power);
  set_address_cb.Run(0);
  set_data_cb.Run(0);
  set_scan_resp_data_cb.Run(0);
  enable_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, start_advertising_set_status);
  EXPECT_EQ(selected_tx_power, start_advertising_set_tx_power);
  int advertiser_id = start_advertising_set_advertiser_id;
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // ... advertising ...

  // Disable advertiser
  status_cb disable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x00 /* disable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_cb));
  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(advertiser_id, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(advertiser_id);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  disable_cb.Run(0);
  remove_cb.Run(0);
}

TEST_F(BleAdvertisingManagerTest, test_start_advertising_set_params_failed) {
  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  int advertiser_id = reg_inst_id;

  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;

  parameters_cb set_params_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(advertiser_id, _, _, _, _, _, _, _, _))
      .Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));

  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, _, _, _, _, _))
      .Times(Exactly(0));

  BleAdvertisingManager::Get()->StartAdvertising(
      advertiser_id,
      Bind(&BleAdvertisingManagerTest::StartAdvertisingCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, 0, base::Callback<void(uint8_t)>());
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // set params failed
  set_params_cb.Run(0x01, 0);

  // Expect the whole flow to fail right away
  EXPECT_EQ(BTM_BLE_MULTI_ADV_FAILURE, start_advertising_status);
}

TEST_F(BleAdvertisingManagerTest, test_data_sender) {
  // prepare test input vector
  const int max_data_size = 1650;
  std::vector<uint8_t> data(max_data_size);
  for (int i = 0; i < max_data_size; i++) data[i] = i;

  BleAdvertisingManager::Get()->RegisterAdvertiser(
      Bind(&BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
  int advertiser_id = reg_inst_id;

  status_cb set_data_cb;
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, FIRST, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock,
              SetAdvertisingData(advertiser_id, INTERMEDIATE, _, 251, _, _))
      .Times(5)
      .WillRepeatedly(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, LAST, _, 144, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  for (int i = 0; i < 7; i++) {
    set_data_cb.Run(0x00);
  }
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(503);
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, FIRST, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock,
              SetAdvertisingData(advertiser_id, INTERMEDIATE, _, 251, _, _))
      .Times(1)
      .WillRepeatedly(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, LAST, _, 1, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  for (int i = 0; i < 3; i++) {
    set_data_cb.Run(0x00);
  }
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(502);
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, FIRST, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, LAST, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  for (int i = 0; i < 2; i++) {
    set_data_cb.Run(0x00);
  }
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(501);
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, FIRST, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(advertiser_id, LAST, _, 250, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  for (int i = 0; i < 2; i++) {
    set_data_cb.Run(0x00);
  }
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(251);
  EXPECT_CALL(*hci_mock,
              SetAdvertisingData(advertiser_id, COMPLETE, _, 251, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  set_data_cb.Run(0x00);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(120);
  EXPECT_CALL(*hci_mock,
              SetAdvertisingData(advertiser_id, COMPLETE, _, 120, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  set_data_cb.Run(0x00);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);

  // ***************** Try again with different data size *********************
  data.resize(0);
  EXPECT_CALL(*hci_mock,
              SetAdvertisingData(advertiser_id, COMPLETE, _, 0, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  BleAdvertisingManager::Get()->SetData(
      advertiser_id, false, data,
      Bind(&BleAdvertisingManagerTest::SetDataCb, base::Unretained(this)));
  set_data_cb.Run(0x00);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  // Expect the whole flow to succeed
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, set_data_status);
}

/* This test makes sure that conectable advertisment with timeout will get it's
 * address updated once the timeout passes and one tries to enable it again.*/
TEST_F(BleAdvertisingManagerTest,
       test_connectable_address_update_during_timeout) {
  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;
  params.advertising_event_properties = 0x1 /* connectable */;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  periodic_params.enable = false;
  std::vector<uint8_t> periodic_data;

  uint8_t maxExtAdvEvents = 50;

  parameters_cb set_params_cb;
  status_cb set_address_cb;
  status_cb set_data_cb;
  status_cb set_scan_resp_data_cb;
  status_cb enable_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(_, _, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetScanResponseData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_scan_resp_data_cb));
  EXPECT_CALL(
      *hci_mock,
      Enable(
          0x01 /* enable */,
          AllOf(SizeIs(1),
                Contains(Field(&SetEnableData::max_extended_advertising_events,
                               maxExtAdvEvents))),
          _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));

  BleAdvertisingManager::Get()->StartAdvertisingSet(
      Bind(&BleAdvertisingManagerTest::StartAdvertisingSetCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, &periodic_params, periodic_data,
      0 /* duration */, maxExtAdvEvents, Bind(DoNothing2));

  // we are a truly gracious fake controller, let the commands succeed!
  int selected_tx_power = -15;
  set_params_cb.Run(0, selected_tx_power);
  set_address_cb.Run(0);
  set_data_cb.Run(0);
  set_scan_resp_data_cb.Run(0);
  enable_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, start_advertising_set_status);
  EXPECT_EQ(selected_tx_power, start_advertising_set_tx_power);
  int advertiser_id = start_advertising_set_advertiser_id;
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // ... advertising ...

  // No HCI calls should be triggered, becuase there is a timeout on a
  // connectable advertisement.
  TriggerRandomAddressUpdate();
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // Set terminated because we advertised maxExtAdvEvents times!
  BleAdvertisingManager::Get()->OnAdvertisingSetTerminated(
      0x43 /*status */, advertiser_id, 0x00 /* conn_handle*/, maxExtAdvEvents);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // Try to Enable the advertiser. It should first update it's random address.
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(
      *hci_mock,
      Enable(
          0x01 /* enable */,
          AllOf(SizeIs(1),
                Contains(Field(&SetEnableData::max_extended_advertising_events,
                               maxExtAdvEvents))),
          _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));
  BleAdvertisingManager::Get()->Enable(
      advertiser_id, true,
      Bind(&BleAdvertisingManagerTest::EnableCb, base::Unretained(this)), 0,
      maxExtAdvEvents, Bind(DoNothing));
  set_address_cb.Run(0);
  enable_cb.Run(0);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // Disable advertiser
  status_cb disable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x00 /* disable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_cb));
  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(advertiser_id, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(advertiser_id);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  disable_cb.Run(0);
  remove_cb.Run(0);
}

/* This test makes sure that periodic advertising is stopped before
 * unregistering the advertiser, if it was enabled. */
TEST_F(BleAdvertisingManagerTest, test_periodic_adv_disable_on_unregister) {
  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;
  params.advertising_event_properties = 0x1 /* connectable */;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  periodic_params.enable = true;  // enable periodic advertising
  std::vector<uint8_t> periodic_data;

  parameters_cb set_params_cb;
  status_cb set_address_cb;
  status_cb set_data_cb;
  status_cb set_scan_resp_data_cb;
  status_cb enable_cb;
  status_cb set_periodic_params_cb;
  status_cb set_periodic_data_cb;
  status_cb set_periodic_enable_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(_, _, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetScanResponseData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_scan_resp_data_cb));
  EXPECT_CALL(*hci_mock, SetPeriodicAdvertisingParameters(_, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<4>(&set_periodic_params_cb));
  EXPECT_CALL(*hci_mock, SetPeriodicAdvertisingData(_, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<4>(&set_periodic_data_cb));
  EXPECT_CALL(*hci_mock, SetPeriodicAdvertisingEnable(0x01 /* enable */, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_periodic_enable_cb));
  EXPECT_CALL(*hci_mock, Enable(0x01 /* enable */, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));

  BleAdvertisingManager::Get()->StartAdvertisingSet(
      Bind(&BleAdvertisingManagerTest::StartAdvertisingSetCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, &periodic_params, periodic_data,
      0 /* duration */, 0 /* maxExtAdvEvents */, Bind(DoNothing2));

  // we are a truly gracious fake controller, let the commands succeed!
  int selected_tx_power = -15;
  set_params_cb.Run(0, selected_tx_power);
  set_address_cb.Run(0);
  set_data_cb.Run(0);
  set_scan_resp_data_cb.Run(0);
  set_periodic_params_cb.Run(0);
  set_periodic_data_cb.Run(0);
  set_periodic_enable_cb.Run(0);
  enable_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, start_advertising_set_status);
  EXPECT_EQ(selected_tx_power, start_advertising_set_tx_power);
  int advertiser_id = start_advertising_set_advertiser_id;
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // ... advertising ...

  // Unregister advertiser - should disable periodic advertising
  status_cb disable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x00 /* disable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_cb));
  status_cb disable_periodic_cb;
  EXPECT_CALL(*hci_mock, SetPeriodicAdvertisingEnable(0x00 /* disable */,
                                                      advertiser_id, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_periodic_cb));
  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(advertiser_id, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(advertiser_id);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  disable_cb.Run(0);
  disable_periodic_cb.Run(0);
  remove_cb.Run(0);
}

TEST_F(BleAdvertisingManagerTest, test_suspend_resume) {
  for (int i = 0; i < 10; i++) {
    BleAdvertisingManager::Get()->RegisterAdvertiser(Bind(
        &BleAdvertisingManagerTest::RegistrationCb, base::Unretained(this)));
    EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, reg_status);
    EXPECT_EQ(i, reg_inst_id);
  }

  std::array<int, 3> enabled = {{1, 3, 9}};

  for (int advertiser_id : enabled) {
    status_cb enable_cb;
    EXPECT_CALL(*hci_mock,
                Enable(0x01 /* enable */,
                       AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                       advertiser_id))),
                       _))
        .Times(1)
        .WillOnce(SaveArg<2>(&enable_cb));
    BleAdvertisingManager::Get()->Enable(
        advertiser_id, true,
        Bind(&BleAdvertisingManagerTest::EnableCb, base::Unretained(this)), 0,
        0, base::Callback<void(uint8_t)>());
    enable_cb.Run(0);
    ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
  }

  // we have some advertisers registered, three advertising.

  // Call to Suspend() should disable all running advertisers
  status_cb disable_cb;
  EXPECT_CALL(
      *hci_mock,
      Enable(0x00 /* disable */,
             AllOf(SizeIs(3), Contains(Field(&SetEnableData::handle, 1)),
                   Contains(Field(&SetEnableData::handle, 3)),
                   Contains(Field(&SetEnableData::handle, 9))),
             _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_cb));

  BleAdvertisingManager::Get()->Suspend();

  disable_cb.Run(0);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // Call to Resume() should re-enable advertisers
  status_cb enable_cb;
  EXPECT_CALL(
      *hci_mock,
      Enable(0x01 /* enable */,
             AllOf(SizeIs(3), Contains(Field(&SetEnableData::handle, 1)),
                   Contains(Field(&SetEnableData::handle, 3)),
                   Contains(Field(&SetEnableData::handle, 9))),
             _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));

  BleAdvertisingManager::Get()->Resume();

  enable_cb.Run(0);

  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
}

/* This test makes sure that conectable advertisment with timeout will get it's
 * duration and maxExtAdvEvents updated, when it's terminated due to incoming
 * connection.*/
TEST_F(BleAdvertisingManagerTest, test_duration_update_during_timeout) {
  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;
  params.advertising_event_properties = 0x1 /* connectable */;
  params.adv_int_min = params.adv_int_max = 0xA0 /* 100ms */;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  periodic_params.enable = false;
  std::vector<uint8_t> periodic_data;

  uint8_t maxExtAdvEvents = 50;
  uint16_t duration = 500 /* 5s */;

  parameters_cb set_params_cb;
  status_cb set_address_cb;
  status_cb set_data_cb;
  status_cb set_scan_resp_data_cb;
  status_cb enable_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(_, _, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));
  EXPECT_CALL(*hci_mock, SetScanResponseData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_scan_resp_data_cb));
  EXPECT_CALL(
      *hci_mock,
      Enable(0x01 /* enable */,
             AllOf(SizeIs(1),
                   Contains(AllOf(
                       Field(&SetEnableData::max_extended_advertising_events,
                             maxExtAdvEvents),
                       Field(&SetEnableData::duration, duration)))),
             _))
      .Times(1)
      .WillOnce(SaveArg<2>(&enable_cb));

  BleAdvertisingManager::Get()->StartAdvertisingSet(
      Bind(&BleAdvertisingManagerTest::StartAdvertisingSetCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, &periodic_params, periodic_data, duration,
      maxExtAdvEvents, Bind(DoNothing2));

  // we are a truly gracious fake controller, let the commands succeed!
  int selected_tx_power = -15;
  set_params_cb.Run(0, selected_tx_power);
  set_address_cb.Run(0);
  set_data_cb.Run(0);
  set_scan_resp_data_cb.Run(0);
  enable_cb.Run(0);
  EXPECT_EQ(BTM_BLE_MULTI_ADV_SUCCESS, start_advertising_set_status);
  EXPECT_EQ(selected_tx_power, start_advertising_set_tx_power);
  int advertiser_id = start_advertising_set_advertiser_id;
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  // ... advertising ...

  sleep(1);

  std::vector<SetEnableData> setEnableData;
  // Set terminated because we received connect request! Should trigger
  // re-enabling of the set
  EXPECT_CALL(*hci_mock, Enable(0x01 /* enable */, _, _))
      .Times(1)
      .WillOnce(DoAll(SaveArg<1>(&setEnableData), SaveArg<2>(&enable_cb)));

  BleAdvertisingManager::Get()->OnAdvertisingSetTerminated(
      0x00 /* Advertising successfully ended with a connection being created */,
      advertiser_id, 0x01fe /* conn_handle*/, 20 /* completed ExtAdvEvents */);
  enable_cb.Run(0);

  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  uint16_t new_duration = setEnableData[0].duration;
  uint8_t new_extAdvEvents = setEnableData[0].max_extended_advertising_events;

  // Sleep is not super-accurate, so assume the recomputed timeouts are around
  // 4s +/- 100ms
  EXPECT_NEAR((duration - new_duration), 100 /*4s */, 10);
  EXPECT_NEAR((maxExtAdvEvents - new_extAdvEvents), 10, 1);

  // Disable advertiser
  status_cb disable_cb;
  EXPECT_CALL(*hci_mock,
              Enable(0x00 /* disable */,
                     AllOf(SizeIs(1), Contains(Field(&SetEnableData::handle,
                                                     advertiser_id))),
                     _))
      .Times(1)
      .WillOnce(SaveArg<2>(&disable_cb));
  status_cb remove_cb;
  EXPECT_CALL(*hci_mock, RemoveAdvertisingSet(advertiser_id, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&remove_cb));
  BleAdvertisingManager::Get()->Unregister(advertiser_id);
  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());

  disable_cb.Run(0);
  remove_cb.Run(0);
}

/* This test verifies that stack cleanup, and shutdown happening while there is
 * outstanding HCI command is not triggering the callback */
TEST_F(BleAdvertisingManagerTest, test_cleanup_during_execution) {
  std::vector<uint8_t> adv_data;
  std::vector<uint8_t> scan_resp;
  tBTM_BLE_ADV_PARAMS params;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  periodic_params.enable = false;
  std::vector<uint8_t> periodic_data;

  parameters_cb set_params_cb;
  status_cb set_address_cb;
  status_cb set_data_cb;
  EXPECT_CALL(*hci_mock, SetParameters1(_, _, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(*hci_mock, SetParameters2(_, _, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<7>(&set_params_cb));
  EXPECT_CALL(*hci_mock, SetRandomAddress(_, _, _))
      .Times(1)
      .WillOnce(SaveArg<2>(&set_address_cb));
  EXPECT_CALL(*hci_mock, SetAdvertisingData(_, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<5>(&set_data_cb));

  BleAdvertisingManager::Get()->StartAdvertisingSet(
      Bind(&BleAdvertisingManagerTest::StartAdvertisingSetCb,
           base::Unretained(this)),
      &params, adv_data, scan_resp, &periodic_params, periodic_data,
      0 /* duration */, 0 /* maxExtAdvEvents */, Bind(DoNothing2));

  // we are a truly gracious fake controller, let the commands succeed!
  int selected_tx_power = -15;
  set_params_cb.Run(0, selected_tx_power);
  set_address_cb.Run(0);

  // Someone shut down the stack in the middle of flow, when the HCI Set
  // Advertise Data was scheduled!
  BleAdvertisingManager::Get()->CleanUp();

  // The HCI call returns with status, and tries to execute the callback. This
  // should just silently drop the call. If it got executed, we would get crash,
  // because BleAdvertisingManager object was already deleted.
  set_data_cb.Run(0);

  ::testing::Mock::VerifyAndClearExpectations(hci_mock.get());
}

extern void testRecomputeTimeout1();
extern void testRecomputeTimeout2();
extern void testRecomputeTimeout3();

TEST_F(BleAdvertisingManagerTest, test_recompute_timeout) {
  testRecomputeTimeout1();
  testRecomputeTimeout2();
  testRecomputeTimeout3();
}