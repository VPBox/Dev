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

#include <vector>

#include "wificond/tests/offload_hal_test_constants.h"
#include "wificond/tests/offload_test_utils.h"

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::ScanStats;
using android::hardware::wifi::offload::V1_0::ScanRecord;
using android::hardware::wifi::offload::V1_0::OffloadStatus;
using android::hardware::wifi::offload::V1_0::OffloadStatusCode;

using ::com::android::server::wifi::wificond::NativeScanResult;
using ::com::android::server::wifi::wificond::NativeScanStats;

using namespace android::wificond::offload_hal_test_constants;

namespace android {
namespace wificond {

std::vector<ScanResult> OffloadTestUtils::createOffloadScanResults() {
  std::vector<ScanResult> scanResults;
  ScanResult scanResult;
  std::vector<uint8_t> ssid(kSsid1, kSsid1 + kSsid1_size);
  scanResult.tsf = kTsf;
  scanResult.rssi = kRssi;
  scanResult.frequency = kFrequency1;
  scanResult.capability = kCapability;
  memcpy(&scanResult.bssid[0], &kBssid[0], kBssidSize);
  scanResult.networkInfo.ssid = ssid;
  scanResult.networkInfo.flags = kNetworkFlags;
  scanResults.push_back(scanResult);
  return scanResults;
}

ScanStats OffloadTestUtils::createScanStats(NativeScanStats* nativeScanStats) {
  std::vector<ScanRecord> scan_records;
  std::vector<uint8_t> histogram_channels;
  uint32_t scan_duration_ms = 0;
  uint32_t num_channels_scanned = 0;
  ScanStats scan_stats;
  int numEntriesInScanRecord =
      sizeof(kNumChannelsScanned) / sizeof(kNumChannelsScanned[0]);
  for (int i = 0; i < numEntriesInScanRecord; i++) {
    ScanRecord scan_record;
    scan_record.durationMs = kScanDurationMs[i];
    scan_duration_ms += kScanDurationMs[i];
    scan_record.numChannelsScanned = kNumChannelsScanned[i];
    num_channels_scanned += kNumChannelsScanned[i];
    scan_record.numEntriesAggregated = 1;
    scan_records.push_back(scan_record);
  }
  scan_stats.scanRecord = scan_records;
  scan_stats.numScansRequestedByWifi = kDefaultNumScansRequestedByWifi;
  scan_stats.numScansServicedByWifi = kDefaultNumScansServicedByWifi;
  scan_stats.subscriptionDurationMs = kSubscriptionDurationMs;
  uint32_t skip_tmp = 256 / num_channels_scanned;
  for (size_t i = 0; i < 256; i++) {
    if (i % skip_tmp == 0) {
      scan_stats.histogramChannelsScanned[i] =
          kDefaultNumTimesAChannelsIsScanned;
      histogram_channels.push_back(kDefaultNumTimesAChannelsIsScanned);
    } else {
      scan_stats.histogramChannelsScanned[i] = kChannelNotScanned;
      histogram_channels.push_back(kChannelNotScanned);
    }
  }
  NativeScanStats native_scan_stats(kDefaultNumScansRequestedByWifi,
                                    kDefaultNumScansServicedByWifi,
                                    kSubscriptionDurationMs, scan_duration_ms,
                                    num_channels_scanned, histogram_channels);
  *nativeScanStats = native_scan_stats;
  return scan_stats;
}

OffloadStatus OffloadTestUtils::createOffloadStatus(OffloadStatusCode code) {
  return createOffloadStatus(code, "");
}

OffloadStatus OffloadTestUtils::createOffloadStatus(OffloadStatusCode code,
                                                    const std::string& desc) {
  return {code, desc};
}

}  // namespace wificond
}  // namespace android
