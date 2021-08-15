//
//  Copyright 2016 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <base/macros.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "service/adapter.h"
#include "service/hal/fake_bluetooth_gatt_interface.h"
#include "service/low_energy_advertiser.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"
#include "test/mock_adapter.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Matcher;
using status_cb = base::Callback<void(uint8_t)>;
using reg_cb =
    base::Callback<void(uint8_t /* advertiser_id */, uint8_t /* status */)>;

namespace bluetooth {
namespace {

class MockAdvertiserHandler : public BleAdvertiserInterface {
 public:
  MockAdvertiserHandler() {}
  ~MockAdvertiserHandler() override = default;

  MOCK_METHOD1(RegisterAdvertiser, void(IdStatusCallback));
  MOCK_METHOD1(Unregister, void(uint8_t));
  MOCK_METHOD2(GetOwnAddress, void(uint8_t, GetAddressCallback));
  MOCK_METHOD3(SetParameters,
               void(uint8_t, AdvertiseParameters, ParametersCallback));
  MOCK_METHOD4(SetData, void(int, bool, std::vector<uint8_t>, StatusCallback));
  MOCK_METHOD6(Enable, void(uint8_t, bool, StatusCallback, uint16_t, uint8_t,
                            StatusCallback));
  MOCK_METHOD7(StartAdvertising,
               void(uint8_t advertiser_id, StatusCallback cb,
                    AdvertiseParameters, std::vector<uint8_t>,
                    std::vector<uint8_t>, int, StatusCallback));
  MOCK_METHOD9(StartAdvertisingSet,
               void(IdTxPowerStatusCallback cb, AdvertiseParameters params,
                    std::vector<uint8_t> advertise_data,
                    std::vector<uint8_t> scan_response_data,
                    PeriodicAdvertisingParameters periodic_params,
                    std::vector<uint8_t> periodic_data, uint16_t duration,
                    uint8_t maxExtAdvEvents, IdStatusCallback timeout_cb));
  MOCK_METHOD3(SetPeriodicAdvertisingParameters,
               void(int, PeriodicAdvertisingParameters, StatusCallback));
  MOCK_METHOD3(SetPeriodicAdvertisingData,
               void(int, std::vector<uint8_t>, StatusCallback));
  MOCK_METHOD3(SetPeriodicAdvertisingEnable, void(int, bool, StatusCallback));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockAdvertiserHandler);
};

class LowEnergyAdvertiserTest : public ::testing::Test {
 public:
  LowEnergyAdvertiserTest() = default;
  ~LowEnergyAdvertiserTest() override = default;

  void SetUp() override {
    // Only set |mock_handler_| if a test hasn't set it.
    if (!mock_handler_) mock_handler_.reset(new MockAdvertiserHandler());
    hal::BluetoothGattInterface::InitializeForTesting(
        new hal::FakeBluetoothGattInterface(
            std::static_pointer_cast<BleAdvertiserInterface>(mock_handler_),
            nullptr, nullptr, nullptr));
    ble_advertiser_factory_.reset(new LowEnergyAdvertiserFactory());
  }

  void TearDown() override {
    ble_advertiser_factory_.reset();
    hal::BluetoothGattInterface::CleanUp();
  }

 protected:
  std::shared_ptr<MockAdvertiserHandler> mock_handler_;
  std::unique_ptr<LowEnergyAdvertiserFactory> ble_advertiser_factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LowEnergyAdvertiserTest);
};

// Used for tests that operate on a pre-registered advertiser.
class LowEnergyAdvertiserPostRegisterTest : public LowEnergyAdvertiserTest {
 public:
  LowEnergyAdvertiserPostRegisterTest() : next_client_id_(0) {}
  ~LowEnergyAdvertiserPostRegisterTest() override = default;

  void SetUp() override {
    LowEnergyAdvertiserTest::SetUp();
    auto callback = [&](std::unique_ptr<LowEnergyAdvertiser> advertiser) {
      le_advertiser_ = std::move(advertiser);
    };
    RegisterTestAdvertiser(callback);
  }

  void TearDown() override {
    EXPECT_CALL(*mock_handler_, Enable(_, false, _, _, _, _)).Times(1);
    EXPECT_CALL(*mock_handler_, Unregister(_)).Times(1);
    le_advertiser_.reset();
    LowEnergyAdvertiserTest::TearDown();
  }

  void RegisterTestAdvertiser(
      const std::function<void(std::unique_ptr<LowEnergyAdvertiser> advertiser)>
          callback) {
    Uuid uuid = Uuid::GetRandom();
    auto api_callback = [&](BLEStatus status, const Uuid& in_uuid,
                            std::unique_ptr<BluetoothInstance> in_client) {
      CHECK(in_uuid == uuid);
      CHECK(in_client.get());
      CHECK(status == BLE_STATUS_SUCCESS);

      callback(std::unique_ptr<LowEnergyAdvertiser>(
          static_cast<LowEnergyAdvertiser*>(in_client.release())));
    };

    reg_cb reg_adv_cb;
    EXPECT_CALL(*mock_handler_, RegisterAdvertiser(_))
        .Times(1)
        .WillOnce(SaveArg<0>(&reg_adv_cb));

    ble_advertiser_factory_->RegisterInstance(uuid, api_callback);

    reg_adv_cb.Run(next_client_id_++, BT_STATUS_SUCCESS);
    ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
  }

  void StartAdvertising() {
    ASSERT_FALSE(le_advertiser_->IsAdvertisingStarted());
    ASSERT_FALSE(le_advertiser_->IsStartingAdvertising());
    ASSERT_FALSE(le_advertiser_->IsStoppingAdvertising());

    status_cb start_advertising_cb;
    EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
        .Times(1)
        .WillOnce(SaveArg<1>(&start_advertising_cb));
    status_cb set_data_cb;

    AdvertiseSettings settings;
    AdvertiseData adv, scan_rsp;
    ASSERT_TRUE(le_advertiser_->StartAdvertising(
        settings, adv, scan_rsp, LowEnergyAdvertiser::StatusCallback()));
    ASSERT_TRUE(le_advertiser_->IsStartingAdvertising());

    start_advertising_cb.Run(BT_STATUS_SUCCESS);

    ASSERT_TRUE(le_advertiser_->IsAdvertisingStarted());
    ASSERT_FALSE(le_advertiser_->IsStartingAdvertising());
    ASSERT_FALSE(le_advertiser_->IsStoppingAdvertising());
  }

  void AdvertiseDataTestHelper(AdvertiseData data,
                               std::function<void(BLEStatus)> callback,
                               status_cb* set_data_cb) {
    AdvertiseSettings settings;

    LOG_ASSERT(set_data_cb) << "set_data_cb must be set";

    EXPECT_TRUE(le_advertiser_->StartAdvertising(settings, data,
                                                 AdvertiseData(), callback));

    set_data_cb->Run(BT_STATUS_SUCCESS);

    status_cb disable_cb;
    EXPECT_CALL(*mock_handler_, Enable(_, false, _, _, _, _))
        .Times(1)
        .WillOnce(SaveArg<2>(&disable_cb));

    EXPECT_TRUE(
        le_advertiser_->StopAdvertising(LowEnergyAdvertiser::StatusCallback()));
    disable_cb.Run(BT_STATUS_SUCCESS);
  }

 protected:
  std::unique_ptr<LowEnergyAdvertiser> le_advertiser_;

 private:
  int next_client_id_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyAdvertiserPostRegisterTest);
};

TEST_F(LowEnergyAdvertiserTest, RegisterInstance) {
  // These will be asynchronously populated with a result when the callback
  // executes.
  BLEStatus status = BLE_STATUS_SUCCESS;
  Uuid cb_uuid;
  std::unique_ptr<LowEnergyAdvertiser> advertiser;
  int callback_count = 0;

  auto callback = [&](BLEStatus in_status, const Uuid& uuid,
                      std::unique_ptr<BluetoothInstance> in_client) {
    status = in_status;
    cb_uuid = uuid;
    advertiser = std::unique_ptr<LowEnergyAdvertiser>(
        static_cast<LowEnergyAdvertiser*>(in_client.release()));
    callback_count++;
  };

  Uuid uuid0 = Uuid::GetRandom();

  reg_cb reg_adv1_cb;
  EXPECT_CALL(*mock_handler_, RegisterAdvertiser(_))
      .Times(1)
      .WillOnce(SaveArg<0>(&reg_adv1_cb));

  // Success.
  EXPECT_TRUE(ble_advertiser_factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // Calling twice with the same Uuid should fail with no additional call into
  // the stack.
  EXPECT_FALSE(ble_advertiser_factory_->RegisterInstance(uuid0, callback));

  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Call with a different Uuid while one is pending.
  Uuid uuid1 = Uuid::GetRandom();
  reg_cb reg_adv2_cb;
  EXPECT_CALL(*mock_handler_, RegisterAdvertiser(_))
      .Times(1)
      .WillOnce(SaveArg<0>(&reg_adv2_cb));
  EXPECT_TRUE(ble_advertiser_factory_->RegisterInstance(uuid1, callback));

  // |uuid0| succeeds.
  int client_if0 = 2;  // Pick something that's not 0.
  reg_adv1_cb.Run(client_if0, BT_STATUS_SUCCESS);

  EXPECT_EQ(1, callback_count);
  ASSERT_TRUE(advertiser.get() !=
              nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_SUCCESS, status);
  EXPECT_EQ(client_if0, advertiser->GetInstanceId());
  EXPECT_EQ(uuid0, advertiser->GetAppIdentifier());
  EXPECT_EQ(uuid0, cb_uuid);

  // The advertiser should unregister itself when deleted.
  EXPECT_CALL(*mock_handler_, Enable(client_if0, false, _, _, _, _)).Times(1);
  EXPECT_CALL(*mock_handler_, Unregister(client_if0)).Times(1);
  advertiser.reset();
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // |uuid1| fails.
  uint8_t client_if1 = 10;
  reg_adv2_cb.Run(client_if1, BT_STATUS_FAIL);

  EXPECT_EQ(2, callback_count);
  ASSERT_TRUE(advertiser.get() ==
              nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_FAILURE, status);
  EXPECT_EQ(uuid1, cb_uuid);
}

TEST_F(LowEnergyAdvertiserPostRegisterTest, StartAdvertisingBasic) {
  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());

  // Use default advertising settings and data.
  AdvertiseSettings settings;
  AdvertiseData adv_data, scan_rsp;
  int callback_count = 0;
  BLEStatus last_status = BLE_STATUS_FAILURE;
  auto callback = [&](BLEStatus status) {
    last_status = status;
    callback_count++;
  };

  status_cb start_advertising_cb;
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(2)
      .WillRepeatedly(SaveArg<1>(&start_advertising_cb));

  // Stack call returns success.
  EXPECT_TRUE(
      le_advertiser_->StartAdvertising(settings, adv_data, scan_rsp, callback));

  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_TRUE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(0, callback_count);

  // Already starting.
  EXPECT_FALSE(
      le_advertiser_->StartAdvertising(settings, adv_data, scan_rsp, callback));

  // Notify failure.
  start_advertising_cb.Run(BT_STATUS_FAIL);

  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(1, callback_count);
  EXPECT_EQ(BLE_STATUS_FAILURE, last_status);

  // Try again.
  EXPECT_TRUE(
      le_advertiser_->StartAdvertising(settings, adv_data, scan_rsp, callback));
  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_TRUE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(1, callback_count);

  start_advertising_cb.Run(BT_STATUS_SUCCESS);

  EXPECT_TRUE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(2, callback_count);
  EXPECT_EQ(BLE_STATUS_SUCCESS, last_status);

  // Already started.
  EXPECT_FALSE(
      le_advertiser_->StartAdvertising(settings, adv_data, scan_rsp, callback));
}

TEST_F(LowEnergyAdvertiserPostRegisterTest, StopAdvertisingBasic) {
  AdvertiseSettings settings;

  // Not enabled.
  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(
      le_advertiser_->StopAdvertising(LowEnergyAdvertiser::StatusCallback()));

  // Start advertising for testing.
  StartAdvertising();

  int callback_count = 0;
  BLEStatus last_status = BLE_STATUS_FAILURE;
  auto callback = [&](BLEStatus status) {
    last_status = status;
    callback_count++;
  };

  status_cb enable_cb;
  EXPECT_CALL(*mock_handler_, Enable(_, false, _, _, _, _))
      .Times(2)
      .WillRepeatedly(SaveArg<2>(&enable_cb));

  // Stack returns success.
  EXPECT_TRUE(le_advertiser_->StopAdvertising(callback));
  EXPECT_TRUE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_TRUE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(0, callback_count);

  // Already disabling.
  EXPECT_FALSE(le_advertiser_->StopAdvertising(callback));
  EXPECT_TRUE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_TRUE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(0, callback_count);

  // Notify failure.
  enable_cb.Run(BT_STATUS_FAIL);
  EXPECT_TRUE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(1, callback_count);
  EXPECT_EQ(BLE_STATUS_FAILURE, last_status);

  // Try again.
  EXPECT_TRUE(le_advertiser_->StopAdvertising(callback));
  EXPECT_TRUE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_TRUE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(1, callback_count);

  // Notify success.
  enable_cb.Run(BT_STATUS_SUCCESS);
  EXPECT_FALSE(le_advertiser_->IsAdvertisingStarted());
  EXPECT_FALSE(le_advertiser_->IsStartingAdvertising());
  EXPECT_FALSE(le_advertiser_->IsStoppingAdvertising());
  EXPECT_EQ(2, callback_count);
  EXPECT_EQ(BLE_STATUS_SUCCESS, last_status);

  // Already stopped.
  EXPECT_FALSE(le_advertiser_->StopAdvertising(callback));
}

TEST_F(LowEnergyAdvertiserPostRegisterTest, InvalidAdvertiseData) {
  const std::vector<uint8_t> data0{0x02, HCI_EIR_FLAGS_TYPE, 0x00};
  const std::vector<uint8_t> data1{0x04, HCI_EIR_MANUFACTURER_SPECIFIC_TYPE,
                                   0x01, 0x02, 0x00};
  AdvertiseData invalid_adv(data0);
  AdvertiseData valid_adv(data1);

  AdvertiseSettings settings;

  EXPECT_FALSE(le_advertiser_->StartAdvertising(
      settings, valid_adv, invalid_adv, LowEnergyAdvertiser::StatusCallback()));
  EXPECT_FALSE(le_advertiser_->StartAdvertising(
      settings, invalid_adv, valid_adv, LowEnergyAdvertiser::StatusCallback()));

  // Manufacturer data not correctly formatted according to spec. We let the
  // stack handle this case.
  const std::vector<uint8_t> data2{0x01, HCI_EIR_MANUFACTURER_SPECIFIC_TYPE};
  AdvertiseData invalid_mfc(data2);

  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _)).Times(1);
  EXPECT_TRUE(le_advertiser_->StartAdvertising(
      settings, invalid_mfc, valid_adv, LowEnergyAdvertiser::StatusCallback()));
}

TEST_F(LowEnergyAdvertiserPostRegisterTest, AdvertiseDataParsing) {
  const std::vector<uint8_t> kUuid16BitData{
      0x03, HCI_EIR_COMPLETE_16BITS_UUID_TYPE, 0xDE, 0xAD,
  };

  const std::vector<uint8_t> kUuid32BitData{
      0x05, HCI_EIR_COMPLETE_32BITS_UUID_TYPE, 0xDE, 0xAD, 0x01, 0x02};

  const std::vector<uint8_t> kUUID128BitData{
      0x11, HCI_EIR_COMPLETE_128BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0x03, 0x04,
      0x05, 0x06,
      0x07, 0x08,
      0x09, 0x0A,
      0x0B, 0x0C,
      0x0D, 0x0E};

  const std::vector<uint8_t> kMultiUuidData{
      0x11, HCI_EIR_COMPLETE_128BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0x03, 0x04,
      0x05, 0x06,
      0x07, 0x08,
      0x09, 0x0A,
      0x0B, 0x0C,
      0x0D, 0x0E,
      0x05, HCI_EIR_COMPLETE_32BITS_UUID_TYPE,
      0xDE, 0xAD,
      0xBE, 0xEF};

  const std::vector<uint8_t> kServiceData16Bit{
      0x05, HCI_EIR_SERVICE_DATA_16BITS_UUID_TYPE, 0xDE, 0xAD, 0xBE, 0xEF};

  const std::vector<uint8_t> kServiceData32Bit{
      0x07, HCI_EIR_SERVICE_DATA_32BITS_UUID_TYPE, 0xDE, 0xAD, 0x01, 0x02, 0xBE,
      0xEF};

  const std::vector<uint8_t> kServiceData128Bit{
      0x13, HCI_EIR_SERVICE_DATA_128BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0x03, 0x04,
      0x05, 0x06,
      0x07, 0x08,
      0x09, 0x0A,
      0x0B, 0x0C,
      0x0D, 0x0E,
      0xBE, 0xEF};

  const std::vector<uint8_t> kMultiServiceData{
      0x13, HCI_EIR_SERVICE_DATA_128BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0x03, 0x04,
      0x05, 0x06,
      0xBE, 0xEF,
      0xDE, 0xAD,
      0x01, 0x02,
      0x03, 0x04,
      0x05, 0x06,
      0x05, HCI_EIR_SERVICE_DATA_16BITS_UUID_TYPE,
      0xDE, 0xAD,
      0xBE, 0xEF};

  const std::vector<uint8_t> kServiceUuidMatch{
      0x05, HCI_EIR_COMPLETE_32BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0x07, HCI_EIR_SERVICE_DATA_32BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0xBE, 0xEF};

  const std::vector<uint8_t> kServiceUuidMismatch{
      0x05, HCI_EIR_COMPLETE_32BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x01,
      0x07, HCI_EIR_SERVICE_DATA_32BITS_UUID_TYPE,
      0xDE, 0xAD,
      0x01, 0x02,
      0xBE, 0xEF};

  AdvertiseData uuid_16bit_adv(kUuid16BitData);
  AdvertiseData uuid_32bit_adv(kUuid32BitData);
  AdvertiseData uuid_128bit_adv(kUUID128BitData);
  AdvertiseData multi_uuid_adv(kMultiUuidData);

  AdvertiseData service_16bit_adv(kServiceData16Bit);
  AdvertiseData service_32bit_adv(kServiceData32Bit);
  AdvertiseData service_128bit_adv(kServiceData128Bit);
  AdvertiseData multi_service_adv(kMultiServiceData);

  AdvertiseData service_uuid_match(kServiceUuidMatch);
  AdvertiseData service_uuid_mismatch(kServiceUuidMismatch);

  AdvertiseSettings settings;

  int callback_count = 0;
  BLEStatus last_status = BLE_STATUS_FAILURE;
  auto callback = [&](BLEStatus status) {
    last_status = status;
    callback_count++;
  };

  status_cb start_advertising_cb;
  // Multiple Uuid test
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(multi_uuid_adv, callback, &start_advertising_cb);
  EXPECT_EQ(1, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Multiple Service Data test
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(multi_service_adv, callback, &start_advertising_cb);
  EXPECT_EQ(2, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // 16bit uuid test, should succeed with correctly parsed uuid in little-endian
  // 128-bit format.
  const std::vector<uint8_t> uuid_16bit_canonical{
      0xFB, 0x34, 0x9b, 0x5F, 0x80, 0x00, 0x00, 0x80,
      0x00, 0x10, 0x00, 0x00, 0xDE, 0xAD, 0x00, 0x00};
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(uuid_16bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(3, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // 32bit uuid test, should succeed with correctly parsed uuid
  const std::vector<uint8_t> uuid_32bit_canonical{
      0xFB, 0x34, 0x9b, 0x5F, 0x80, 0x00, 0x00, 0x80,
      0x00, 0x10, 0x00, 0x00, 0xDE, 0xAD, 0x01, 0x02};
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(uuid_32bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(4, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // 128bit uuid test, should succeed with correctly parsed uuid
  const std::vector<uint8_t> uuid_128bit{0xDE, 0xAD, 0x01, 0x02, 0x03, 0x04,
                                         0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
                                         0x0B, 0x0C, 0x0D, 0x0E};
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(uuid_128bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(5, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  const std::vector<uint8_t> service_data{0xBE, 0xEF};

  // Service data with 16bit uuid included, should succeed with
  // uuid and service data parsed out
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(service_16bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(6, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Service data with 32bit uuid included, should succeed with
  // uuid and service data parsed out
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(service_32bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(7, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Service data with 128bit uuid included, should succeed with
  // uuid and service data parsed out
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(service_128bit_adv, callback, &start_advertising_cb);
  EXPECT_EQ(8, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Service data and Uuid where the Uuid for both match, should succeed.
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(service_uuid_match, callback, &start_advertising_cb);
  EXPECT_EQ(9, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Service data and Uuid where the Uuid for dont match, should fail
  EXPECT_CALL(*mock_handler_, StartAdvertising(_, _, _, _, _, _, _))
      .Times(1)
      .WillOnce(SaveArg<1>(&start_advertising_cb));
  AdvertiseDataTestHelper(service_uuid_match, callback, &start_advertising_cb);
  EXPECT_EQ(10, callback_count);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
}

MATCHER_P(BitEq, x, std::string(negation ? "isn't" : "is") +
                        " bitwise equal to " + ::testing::PrintToString(x)) {
  static_assert(sizeof(x) == sizeof(arg), "Size mismatch");
  return std::memcmp(&arg, &x, sizeof(x)) == 0;
}

}  // namespace
}  // namespace bluetooth
