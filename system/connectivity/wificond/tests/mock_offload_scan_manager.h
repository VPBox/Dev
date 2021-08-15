/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_MANAGER_H_
#define WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_MANAGER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wificond/scanning/offload/offload_scan_manager.h"

namespace android {
namespace wificond {

class MockOffloadScanManager : public OffloadScanManager {
 public:
  MockOffloadScanManager(
      std::weak_ptr<OffloadServiceUtils> service_utils,
      std::shared_ptr<OffloadScanCallbackInterface> callback_interface);
  ~MockOffloadScanManager() override = default;

  MOCK_METHOD7(startScan,
               bool(uint32_t interval_ms, int32_t rssi_threshold,
                    const std::vector<std::vector<uint8_t>>& scan_ssids,
                    const std::vector<std::vector<uint8_t>>& match_ssids,
                    const std::vector<uint8_t>& match_security,
                    const std::vector<uint32_t>& frequencies,
                    OffloadScanManager::ReasonCode* reason_code));
  MOCK_METHOD1(stopScan, bool(OffloadScanManager::ReasonCode* reason_code));
  MOCK_METHOD1(getScanStats,
               bool(::com::android::server::wifi::wificond::NativeScanStats*
                        scan_stats));
  MOCK_CONST_METHOD0(getOffloadStatus, OffloadScanManager::StatusCode());
  MOCK_METHOD1(
      getScanResults,
      bool(std::vector<
           ::com::android::server::wifi::wificond::NativeScanResult>*));
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_MANAGER_H_
