//
//  Copyright 2016 The Android Open Source Project
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
#include <memory>

#include "service/adapter.h"
#include "service/hal/fake_bluetooth_gatt_interface.h"
#include "service/low_energy_scanner.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"
#include "test/mock_adapter.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Pointee;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::SaveArg;

namespace bluetooth {
namespace {

class MockScannerHandler : public BleScannerInterface {
 public:
  MockScannerHandler() {}
  ~MockScannerHandler() override = default;

  MOCK_METHOD1(RegisterScanner, void(BleScannerInterface::RegisterCallback));
  MOCK_METHOD1(Unregister, void(int));
  MOCK_METHOD1(Scan, void(bool));

  MOCK_METHOD5(ScanFilterParamSetupImpl,
               void(uint8_t client_if, uint8_t action, uint8_t filt_index,
                    btgatt_filt_param_setup_t* filt_param,
                    FilterParamSetupCallback cb));
  MOCK_METHOD2(ScanFilterClear, void(int filt_index, FilterConfigCallback cb));
  MOCK_METHOD2(ScanFilterEnable, void(bool enable, EnableCallback cb));
  MOCK_METHOD3(SetScanParameters,
               void(int scan_interval, int scan_window, Callback cb));

  MOCK_METHOD5(BatchscanConfigStorage,
               void(int client_if, int batch_scan_full_max,
                    int batch_scan_trunc_max, int batch_scan_notify_threshold,
                    Callback cb));

  MOCK_METHOD6(BatchscanEnable,
               void(int scan_mode, int scan_interval, int scan_window,
                    int addr_type, int discard_rule, Callback cb));

  MOCK_METHOD1(BatchscanDisable, void(Callback cb));

  MOCK_METHOD2(BatchscanReadReports, void(int client_if, int scan_mode));

  MOCK_METHOD7(StartSync, void(uint8_t, RawAddress, uint16_t, uint16_t,
                               StartSyncCb, SyncReportCb, SyncLostCb));
  MOCK_METHOD1(StopSync, void(uint16_t));

  void ScanFilterAdd(int filter_index, std::vector<ApcfCommand> filters,
                     FilterConfigCallback cb){};

  void ScanFilterParamSetup(
      uint8_t client_if, uint8_t action, uint8_t filt_index,
      std::unique_ptr<btgatt_filt_param_setup_t> filt_param,
      FilterParamSetupCallback cb) {
    ScanFilterParamSetupImpl(client_if, action, filt_index, filt_param.get(),
                             std::move(cb));
  }
};

class TestDelegate : public LowEnergyScanner::Delegate {
 public:
  TestDelegate() : scan_result_count_(0) {}

  ~TestDelegate() override = default;

  int scan_result_count() const { return scan_result_count_; }
  const ScanResult& last_scan_result() const { return last_scan_result_; }

  void OnScanResult(LowEnergyScanner* scanner, const ScanResult& scan_result) {
    ASSERT_TRUE(scanner);
    scan_result_count_++;
    last_scan_result_ = scan_result;
  }

 private:
  int scan_result_count_;
  ScanResult last_scan_result_;

  DISALLOW_COPY_AND_ASSIGN(TestDelegate);
};

class LowEnergyScannerTest : public ::testing::Test {
 public:
  LowEnergyScannerTest() = default;
  ~LowEnergyScannerTest() override = default;

  void SetUp() override {
    // Only set |mock_handler_| if a test hasn't set it.
    if (!mock_handler_) mock_handler_.reset(new MockScannerHandler());
    fake_hal_gatt_iface_ = new hal::FakeBluetoothGattInterface(
        nullptr, std::static_pointer_cast<BleScannerInterface>(mock_handler_),
        nullptr, nullptr);
    hal::BluetoothGattInterface::InitializeForTesting(fake_hal_gatt_iface_);
    ble_factory_.reset(new LowEnergyScannerFactory(mock_adapter_));
  }

  void TearDown() override {
    ble_factory_.reset();
    hal::BluetoothGattInterface::CleanUp();
  }

 protected:
  hal::FakeBluetoothGattInterface* fake_hal_gatt_iface_;
  testing::MockAdapter mock_adapter_;
  std::shared_ptr<MockScannerHandler> mock_handler_;
  std::unique_ptr<LowEnergyScannerFactory> ble_factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LowEnergyScannerTest);
};

// Used for tests that operate on a pre-registered scanner.
class LowEnergyScannerPostRegisterTest : public LowEnergyScannerTest {
 public:
  LowEnergyScannerPostRegisterTest() : next_scanner_id_(0) {}
  ~LowEnergyScannerPostRegisterTest() override = default;

  void SetUp() override {
    LowEnergyScannerTest::SetUp();
    auto callback = [&](std::unique_ptr<LowEnergyScanner> scanner) {
      le_scanner_ = std::move(scanner);
    };
    RegisterTestScanner(callback);
  }

  void TearDown() override {
    EXPECT_CALL(*mock_handler_, Unregister(_)).Times(1).WillOnce(Return());
    le_scanner_.reset();
    LowEnergyScannerTest::TearDown();
  }

  void RegisterTestScanner(
      const std::function<void(std::unique_ptr<LowEnergyScanner> scanner)>
          callback) {
    Uuid uuid = Uuid::GetRandom();
    auto api_callback = [&](BLEStatus status, const Uuid& in_uuid,
                            std::unique_ptr<BluetoothInstance> in_scanner) {
      CHECK(in_uuid == uuid);
      CHECK(in_scanner.get());
      CHECK(status == BLE_STATUS_SUCCESS);

      callback(std::unique_ptr<LowEnergyScanner>(
          static_cast<LowEnergyScanner*>(in_scanner.release())));
    };

    BleScannerInterface::RegisterCallback reg_scanner_cb;
    EXPECT_CALL(*mock_handler_, RegisterScanner(_))
        .Times(1)
        .WillOnce(SaveArg<0>(&reg_scanner_cb));

    ble_factory_->RegisterInstance(uuid, api_callback);

    reg_scanner_cb.Run(next_scanner_id_++, BT_STATUS_SUCCESS);
    ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
  }

 protected:
  std::unique_ptr<LowEnergyScanner> le_scanner_;

 private:
  int next_scanner_id_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyScannerPostRegisterTest);
};

TEST_F(LowEnergyScannerTest, RegisterInstance) {
  BleScannerInterface::RegisterCallback reg_scanner_cb1;
  EXPECT_CALL(*mock_handler_, RegisterScanner(_))
      .Times(1)
      .WillOnce(SaveArg<0>(&reg_scanner_cb1));

  // These will be asynchronously populated with a result when the callback
  // executes.
  BLEStatus status = BLE_STATUS_SUCCESS;
  Uuid cb_uuid;
  std::unique_ptr<LowEnergyScanner> scanner;
  int callback_count = 0;

  auto callback = [&](BLEStatus in_status, const Uuid& uuid,
                      std::unique_ptr<BluetoothInstance> in_scanner) {
    status = in_status;
    cb_uuid = uuid;
    scanner = std::unique_ptr<LowEnergyScanner>(
        static_cast<LowEnergyScanner*>(in_scanner.release()));
    callback_count++;
  };

  Uuid uuid0 = Uuid::GetRandom();

  // HAL returns success.
  EXPECT_TRUE(ble_factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // Calling twice with the same Uuid should fail with no additional call into
  // the stack.
  EXPECT_FALSE(ble_factory_->RegisterInstance(uuid0, callback));

  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Call with a different Uuid while one is pending.
  Uuid uuid1 = Uuid::GetRandom();
  BleScannerInterface::RegisterCallback reg_scanner_cb2;
  EXPECT_CALL(*mock_handler_, RegisterScanner(_))
      .Times(1)
      .WillOnce(SaveArg<0>(&reg_scanner_cb2));
  EXPECT_TRUE(ble_factory_->RegisterInstance(uuid1, callback));

  // |uuid0| succeeds.
  int scanner_if0 = 2;  // Pick something that's not 0.
  reg_scanner_cb1.Run(scanner_if0, BT_STATUS_SUCCESS);

  EXPECT_EQ(1, callback_count);
  ASSERT_TRUE(scanner.get() !=
              nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_SUCCESS, status);
  EXPECT_EQ(scanner_if0, scanner->GetInstanceId());
  EXPECT_EQ(uuid0, scanner->GetAppIdentifier());
  EXPECT_EQ(uuid0, cb_uuid);

  // The scanner should unregister itself when deleted.
  EXPECT_CALL(*mock_handler_, Unregister(scanner_if0))
      .Times(1)
      .WillOnce(Return());
  scanner.reset();
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // |uuid1| fails.
  int scanner_if1 = 3;
  reg_scanner_cb2.Run(scanner_if1, BT_STATUS_FAIL);

  EXPECT_EQ(2, callback_count);
  ASSERT_TRUE(scanner.get() ==
              nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_FAILURE, status);
  EXPECT_EQ(uuid1, cb_uuid);
}

TEST_F(LowEnergyScannerPostRegisterTest, ScanSettings) {
  EXPECT_CALL(mock_adapter_, IsEnabled())
      .WillOnce(Return(false))
      .WillRepeatedly(Return(true));

  ScanSettings settings;
  std::vector<ScanFilter> filters;

  // Adapter is not enabled.
  EXPECT_FALSE(le_scanner_->StartScan(settings, filters));

  // TODO(jpawlowski): add tests checking settings and filter parsing when
  // implemented

  // These should succeed and result in a HAL call
  EXPECT_CALL(*mock_handler_, Scan(true)).Times(1).WillOnce(Return());
  EXPECT_TRUE(le_scanner_->StartScan(settings, filters));

  // These should succeed and result in a HAL call
  EXPECT_CALL(*mock_handler_, Scan(false)).Times(1).WillOnce(Return());
  EXPECT_TRUE(le_scanner_->StopScan());

  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
}

TEST_F(LowEnergyScannerPostRegisterTest, ScanRecord) {
  TestDelegate delegate;
  le_scanner_->SetDelegate(&delegate);

  EXPECT_EQ(0, delegate.scan_result_count());

  std::vector<uint8_t> kTestRecord0({0x02, 0x01, 0x00, 0x00});
  std::vector<uint8_t> kTestRecord1({0x00});
  std::vector<uint8_t> kTestRecord2(
      {0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
       0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
       0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
       0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
       0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
       0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00});
  const RawAddress kTestAddress = {{0x01, 0x02, 0x03, 0x0A, 0x0B, 0x0C}};
  const char kTestAddressStr[] = "01:02:03:0A:0B:0C";
  const int kTestRssi = 64;

  // Scan wasn't started. Result should be ignored.
  fake_hal_gatt_iface_->NotifyScanResultCallback(kTestAddress, kTestRssi,
                                                 kTestRecord0);
  EXPECT_EQ(0, delegate.scan_result_count());

  // Start a scan session for |le_scanner_|.
  EXPECT_CALL(mock_adapter_, IsEnabled()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mock_handler_, Scan(_))
      .Times(2)
      .WillOnce(Return())
      .WillOnce(Return());
  ScanSettings settings;
  std::vector<ScanFilter> filters;
  ASSERT_TRUE(le_scanner_->StartScan(settings, filters));

  fake_hal_gatt_iface_->NotifyScanResultCallback(kTestAddress, kTestRssi,
                                                 kTestRecord0);
  EXPECT_EQ(1, delegate.scan_result_count());
  EXPECT_EQ(kTestAddressStr, delegate.last_scan_result().device_address());
  EXPECT_EQ(kTestRssi, delegate.last_scan_result().rssi());
  EXPECT_EQ(3U, delegate.last_scan_result().scan_record().size());

  fake_hal_gatt_iface_->NotifyScanResultCallback(kTestAddress, kTestRssi,
                                                 kTestRecord1);
  EXPECT_EQ(2, delegate.scan_result_count());
  EXPECT_EQ(kTestAddressStr, delegate.last_scan_result().device_address());
  EXPECT_EQ(kTestRssi, delegate.last_scan_result().rssi());
  EXPECT_TRUE(delegate.last_scan_result().scan_record().empty());

  fake_hal_gatt_iface_->NotifyScanResultCallback(kTestAddress, kTestRssi,
                                                 kTestRecord2);
  EXPECT_EQ(3, delegate.scan_result_count());
  EXPECT_EQ(kTestAddressStr, delegate.last_scan_result().device_address());
  EXPECT_EQ(kTestRssi, delegate.last_scan_result().rssi());
  EXPECT_EQ(62U, delegate.last_scan_result().scan_record().size());

  le_scanner_->SetDelegate(nullptr);
}

}  // namespace
}  // namespace bluetooth
