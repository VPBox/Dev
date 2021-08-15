/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include <gtest/gtest.h>

#include "wificond/tests/mock_offload.h"
#include "wificond/tests/mock_offload_scan_callback_interface.h"
#include "wificond/tests/mock_offload_service_utils.h"
#include "wificond/tests/offload_hal_test_constants.h"
#include "wificond/tests/offload_test_utils.h"

#include "wificond/scanning/offload/offload_callback.h"
#include "wificond/scanning/offload/offload_callback_handlers.h"
#include "wificond/scanning/offload/offload_scan_manager.h"
#include "wificond/scanning/scan_result.h"

using android::hardware::Return;
using android::hardware::Void;
using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::OffloadStatus;
using android::hardware::wifi::offload::V1_0::OffloadStatusCode;
using android::hardware::wifi::offload::V1_0::ScanParam;
using android::hardware::wifi::offload::V1_0::ScanFilter;
using android::hardware::wifi::offload::V1_0::ScanStats;
using android::sp;
using com::android::server::wifi::wificond::NativeScanResult;
using com::android::server::wifi::wificond::NativeScanStats;
using testing::NiceMock;
using testing::_;
using testing::Invoke;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::bind;

using namespace std::placeholders;
using namespace android::wificond::offload_hal_test_constants;

namespace android {
namespace wificond {

sp<OffloadCallback> CaptureCallback(OffloadCallbackHandlers* handler,
                                    sp<OffloadCallback>* offload_callback) {
  *offload_callback = sp<OffloadCallback>(new OffloadCallback(handler));
  return *offload_callback;
}

OffloadDeathRecipient* CaptureDeathRecipient(
    OffloadDeathRecipientHandler handler,
    sp<OffloadDeathRecipient>* death_recipient_) {
  OffloadDeathRecipient* death_recipient = new OffloadDeathRecipient(handler);
  *death_recipient_ = sp<OffloadDeathRecipient>(death_recipient);
  return death_recipient;
}

Return<void> HidlStatusReturn(android::wificond::StatusCallback cb,
                              OffloadStatus* status) {
  cb(*status);
  return Void();
}

Return<void> ScanStatusReturn(android::wificond::OnScanStatsCallback cb,
                              OffloadStatus* status) {
  ScanStats stats;
  cb(*status, stats);
  return Void();
}

class OffloadScanManagerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    ON_CALL(*mock_offload_service_utils_, GetOffloadCallback(_))
        .WillByDefault(Invoke(bind(CaptureCallback, _1, &offload_callback_)));
    ON_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_))
        .WillByDefault(
            Invoke(bind(CaptureDeathRecipient, _1, &death_recipient_)));
    status = OffloadTestUtils::createOffloadStatus(OffloadStatusCode::OK);
    ON_CALL(*mock_offload_, configureScans(_, _, _))
        .WillByDefault(Invoke(bind(HidlStatusReturn, _3, &status)));
    ON_CALL(*mock_offload_, subscribeScanResults(_, _))
        .WillByDefault(Invoke(bind(HidlStatusReturn, _2, &status)));
    ON_CALL(*mock_offload_, getScanStats(_))
        .WillByDefault(Invoke(bind(ScanStatusReturn, _1, &status)));
  }

  void TearDown() override {
    offload_callback_.clear();
    death_recipient_.clear();
  }

  sp<NiceMock<MockOffload>> mock_offload_{new NiceMock<MockOffload>()};
  sp<OffloadCallback> offload_callback_;
  sp<OffloadDeathRecipient> death_recipient_;
  shared_ptr<NiceMock<MockOffloadServiceUtils>> mock_offload_service_utils_{
      new NiceMock<MockOffloadServiceUtils>()};
  shared_ptr<NiceMock<MockOffloadScanCallbackInterface>>
      mock_offload_scan_callback_interface_{
          new NiceMock<MockOffloadScanCallbackInterface>()};
  unique_ptr<OffloadScanManager> offload_scan_manager_;
  OffloadStatus status;
  vector<vector<uint8_t>> scan_ssids{kSsid1, kSsid2};
  vector<vector<uint8_t>> match_ssids{kSsid1, kSsid2};
  vector<uint8_t> security_flags{kNetworkFlags, kNetworkFlags};
  vector<uint32_t> frequencies{kFrequency1, kFrequency2};
  uint64_t cookie_ = reinterpret_cast<uint64_t>(mock_offload_.get());
};

/**
 * Testing OffloadScanManager for binder death with registered cookie
 */
TEST_F(OffloadScanManagerTest, BinderDeathRegisteredCookieAndService) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_scan_callback_interface_, OnOffloadError(_));
  death_recipient_->serviceDied(cookie_, mock_offload_);
  EXPECT_EQ(OffloadScanManager::kNoService,
            offload_scan_manager_->getOffloadStatus());
}

/**
 * Testing OffloadScanManager for binder death with invalid cookie
 */
TEST_F(OffloadScanManagerTest, BinderDeathUnregisteredCookie) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  death_recipient_->serviceDied(kDeathCode, mock_offload_);
  EXPECT_FALSE(OffloadScanManager::kNoService ==
               offload_scan_manager_->getOffloadStatus());
}

/**
 * Testing OffloadScanManager with no handle on Offloal HAL service
 * and no registered handler for Offload Scan results
 */
TEST_F(OffloadScanManagerTest, ServiceNotAvailableTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(nullptr));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_EQ(OffloadScanManager::kNoService,
            offload_scan_manager_->getOffloadStatus());
}

/**
 * Testing OffloadScanManager when service is available and valid handler
 * registered for Offload Scan results
 */
TEST_F(OffloadScanManagerTest, ServiceAvailableTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_EQ(OffloadScanManager::kNoError,
            offload_scan_manager_->getOffloadStatus());
}

/**
 * Testing OffloadScanManager when service is available and valid handler
 * is registered, test to ensure that registered handler is invoked when
 * scan results are available
 */
TEST_F(OffloadScanManagerTest, CallbackInvokedTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  EXPECT_CALL(*mock_offload_scan_callback_interface_, OnOffloadScanResult());
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  vector<ScanResult> dummy_scan_results_ =
      OffloadTestUtils::createOffloadScanResults();
  offload_callback_->onScanResult(dummy_scan_results_);
}

/**
 * Testing OffloadScanManager when service is available and valid handler
 * is registered, ensure that error callback is invoked
 */
TEST_F(OffloadScanManagerTest, ErrorCallbackInvokedTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  OffloadStatus status =
      OffloadTestUtils::createOffloadStatus(OffloadStatusCode::ERROR);
  EXPECT_CALL(*mock_offload_scan_callback_interface_, OnOffloadError(_));
  offload_callback_->onError(status);
  EXPECT_EQ(offload_scan_manager_->getOffloadStatus(),
            OffloadScanManager::kError);
}

/**
 * Testing OffloadScanManager for subscribing to the scan results from
 * Offload HAL when service is running without errors
 */
TEST_F(OffloadScanManagerTest, StartScanTestWhenServiceIsOk) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, subscribeScanResults(_, _));
  EXPECT_CALL(*mock_offload_, configureScans(_, _, _));
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, true);
}

/**
 * Testing OffloadScanManager for subscribing to the scan results from
 * Offload HAL when service is not available
 */
TEST_F(OffloadScanManagerTest, StartScanTestWhenServiceIsNotAvailable) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService()).Times(2);
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(nullptr));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, false);
  EXPECT_EQ(reason_code, OffloadScanManager::kNotAvailable);
}

/**
 * Testing OffloadScanManager for subscribing to the scan results from
 * Offload HAL when service is not working correctly
 */
TEST_F(OffloadScanManagerTest, StartScanTestWhenServiceIsNotConnected) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  OffloadStatus status =
      OffloadTestUtils::createOffloadStatus(OffloadStatusCode::NO_CONNECTION);
  offload_callback_->onError(status);
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, false);
  EXPECT_EQ(reason_code, OffloadScanManager::kNotAvailable);
}

/**
 * Testing OffloadScanManager for subscribing to the scan results from
 * Offload HAL twice when service is okay
 */
TEST_F(OffloadScanManagerTest, StartScanTwiceTestWhenServiceIsOk) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, subscribeScanResults(_, _)).Times(2);
  EXPECT_CALL(*mock_offload_, configureScans(_, _, _)).Times(2);
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, true);
  result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, true);
}

/**
 * Testing OffloadScanManager for unsubscribing to the scan results from
 * Offload HAL when service is ok
 */
TEST_F(OffloadScanManagerTest, StopScanTestWhenServiceIsOk) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, subscribeScanResults(_, _));
  EXPECT_CALL(*mock_offload_, configureScans(_, _, _));
  EXPECT_CALL(*mock_offload_, unsubscribeScanResults());
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, true);
  result = offload_scan_manager_->stopScan(&reason_code);
  EXPECT_EQ(result, true);
}

/**
 * Testing OffloadScanManager for unsubscribing to the scan results from
 * when service is not connected to the hardware
 */
TEST_F(OffloadScanManagerTest, StopScanTestWhenServiceIsNotConnectedAnymore) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, subscribeScanResults(_, _));
  EXPECT_CALL(*mock_offload_, configureScans(_, _, _));
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, true);
  OffloadStatus status =
      OffloadTestUtils::createOffloadStatus(OffloadStatusCode::NO_CONNECTION);
  offload_callback_->onError(status);
  result = offload_scan_manager_->stopScan(&reason_code);
  EXPECT_EQ(result, false);
}

/**
 * Testing OffloadScanManager for getting scan statistics when the
 * Offload HAL service is running without errors
 */
TEST_F(OffloadScanManagerTest, getScanStatsTestWhenServiceIsOk) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, getScanStats(_));
  NativeScanStats stats;
  bool result = offload_scan_manager_->getScanStats(&stats);
  EXPECT_EQ(result, true);
}

/**
 * Testing OffloadScanManager for getting scan statistics when the
 * Offload HAL service is not connected
 */
TEST_F(OffloadScanManagerTest, getScanStatsTestWhenServiceIsNotOk) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  OffloadStatus status =
      OffloadTestUtils::createOffloadStatus(OffloadStatusCode::NO_CONNECTION);
  offload_callback_->onError(status);
  EXPECT_CALL(*mock_offload_, getScanStats(_)).Times(0);
  NativeScanStats stats;
  bool result = offload_scan_manager_->getScanStats(&stats);
  EXPECT_EQ(result, false);
}

/**
 * Testing OffloadScanManager for subscribing to the scan results from
 * Offload HAL when service is running without errors, operation failure
 */
TEST_F(OffloadScanManagerTest, StartScanFailedTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  EXPECT_CALL(*mock_offload_, subscribeScanResults(_, _)).Times(0);
  EXPECT_CALL(*mock_offload_, configureScans(_, _, _)).Times(1);
  status = OffloadTestUtils::createOffloadStatus(OffloadStatusCode::ERROR);
  OffloadScanManager::ReasonCode reason_code = OffloadScanManager::kNone;
  bool result = offload_scan_manager_->startScan(
      kDisconnectedModeScanIntervalMs, kRssiThreshold, scan_ssids, match_ssids,
      security_flags, frequencies, &reason_code);
  EXPECT_EQ(result, false);
  EXPECT_EQ(reason_code, OffloadScanManager::kOperationFailed);
}

/**
 * Testing OffloadScanManager for getting scan statistics when the
 * Offload HAL service is running without errors, getting scan stats failure
 */
TEST_F(OffloadScanManagerTest, getScanStatsFailedTest) {
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadService());
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadCallback(_));
  EXPECT_CALL(*mock_offload_service_utils_, GetOffloadDeathRecipient(_));
  ON_CALL(*mock_offload_service_utils_, GetOffloadService())
      .WillByDefault(testing::Return(mock_offload_));
  offload_scan_manager_.reset(new OffloadScanManager(
      mock_offload_service_utils_, mock_offload_scan_callback_interface_));
  status = OffloadTestUtils::createOffloadStatus(OffloadStatusCode::TIMEOUT);
  EXPECT_CALL(*mock_offload_, getScanStats(_));
  NativeScanStats stats;
  bool result = offload_scan_manager_->getScanStats(&stats);
  EXPECT_EQ(result, false);
}

}  // namespace wificond
}  // namespace android
