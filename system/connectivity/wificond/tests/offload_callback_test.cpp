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

#include <gtest/gtest.h>

#include "wificond/scanning/offload/offload_callback.h"
#include "wificond/scanning/scan_result.h"
#include "wificond/tests/mock_offload_callback_handlers.h"
#include "wificond/tests/offload_test_utils.h"

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::OffloadStatus;
using android::hardware::wifi::offload::V1_0::OffloadStatusCode;
using android::hardware::hidl_vec;
using testing::NiceMock;

namespace android {
namespace wificond {

class OffloadCallbackTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    dummy_scan_results_ = OffloadTestUtils::createOffloadScanResults();
  }

  void TearDown() override { dummy_scan_results_.clear(); }

  std::vector<ScanResult> dummy_scan_results_;
  std::unique_ptr<OffloadCallback> offload_callback_;
  std::unique_ptr<NiceMock<MockOffloadCallbackHandlers>> handlers_;
};

/**
 * Testing OffloadCallback to invoke the registered callback handler
 * with the scan results when they are available
 */
TEST_F(OffloadCallbackTest, checkScanResultSize) {
  std::vector<ScanResult> scan_result;
  handlers_.reset(new NiceMock<MockOffloadCallbackHandlers>());
  ON_CALL(*handlers_, OnScanResultHandler(testing::_))
      .WillByDefault(testing::Invoke(
          [&scan_result](std::vector<ScanResult> scanResult) -> void {
            scan_result = scanResult;
          }));
  offload_callback_.reset(new OffloadCallback(handlers_.get()));
  hidl_vec<ScanResult> offloadScanResult(dummy_scan_results_);
  offload_callback_->onScanResult(offloadScanResult);
  EXPECT_EQ(dummy_scan_results_.size(), scan_result.size());
}

/**
 * Testing OffloadCallback to invoke the registered error handler
 */
TEST_F(OffloadCallbackTest, checkErrorStatus) {
  OffloadStatus status_;
  handlers_.reset(new NiceMock<MockOffloadCallbackHandlers>());
  ON_CALL(*handlers_, OnErrorHandler(testing::_))
      .WillByDefault(testing::Invoke(
          [&status_](OffloadStatus status) -> void { status_ = status; }));
  offload_callback_.reset(new OffloadCallback(handlers_.get()));
  OffloadStatus status =
      OffloadTestUtils::createOffloadStatus(OffloadStatusCode::ERROR);
  offload_callback_->onError(status);
  EXPECT_EQ(status_.code, OffloadStatusCode::ERROR);
}

}  // namespace wificond
}  // namespace android
