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
#include "wificond/scanning/offload/offload_scan_utils.h"

#include <android-base/logging.h>
#include <utils/Timers.h>

#include "wificond/scanning/offload/scan_stats.h"
#include "wificond/scanning/scan_result.h"

using ::com::android::server::wifi::wificond::NativeScanResult;
using ::com::android::server::wifi::wificond::NativeScanStats;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::ScanParam;
using android::hardware::wifi::offload::V1_0::ScanFilter;
using android::hardware::wifi::offload::V1_0::ScanStats;
using android::hardware::wifi::offload::V1_0::NetworkInfo;
using android::hardware::hidl_vec;
using std::vector;

namespace android {
namespace wificond {

bool OffloadScanUtils::convertToNativeScanResults(
    const vector<ScanResult>& scan_result,
    vector<NativeScanResult>* native_scan_result) {
  if (native_scan_result == nullptr) return false;
  for (size_t i = 0; i < scan_result.size(); i++) {
    NativeScanResult single_scan_result;
    single_scan_result.ssid.assign(scan_result[i].networkInfo.ssid.begin(),
                                   scan_result[i].networkInfo.ssid.end());
    for (size_t j = 0; j < scan_result[i].bssid.elementCount(); j++) {
      single_scan_result.bssid.at(j) = scan_result[i].bssid[j];
    }
    single_scan_result.frequency = scan_result[i].frequency;
    single_scan_result.signal_mbm = scan_result[i].rssi;
    single_scan_result.tsf = systemTime(SYSTEM_TIME_MONOTONIC) / 1000;
    single_scan_result.capability = scan_result[i].capability;
    single_scan_result.associated = false;
    native_scan_result->push_back(std::move(single_scan_result));
  }
  return true;
}

ScanParam OffloadScanUtils::createScanParam(
    const vector<vector<uint8_t>>& ssid_list,
    const vector<uint32_t>& frequency_list, uint32_t scan_interval_ms) {
  ScanParam scan_param;
  scan_param.disconnectedModeScanIntervalMs = scan_interval_ms;
  scan_param.frequencyList = frequency_list;
  vector<hidl_vec<uint8_t>> ssid_list_tmp;
  for (const auto& ssid : ssid_list) {
    ssid_list_tmp.push_back(ssid);
  }
  scan_param.ssidList = ssid_list_tmp;
  return scan_param;
}

ScanFilter OffloadScanUtils::createScanFilter(
    const vector<vector<uint8_t>>& ssids, const vector<uint8_t>& flags,
    int8_t rssi_threshold) {
  ScanFilter scan_filter;
  vector<NetworkInfo> nw_info_list;
  size_t i = 0;
  scan_filter.rssiThreshold = rssi_threshold;
  // Note that the number of ssids should match the number of security flags
  for (const auto& ssid : ssids) {
    NetworkInfo nw_info;
    nw_info.ssid = ssid;
    if (i < flags.size()) {
      nw_info.flags = flags[i++];
    } else {
      continue;
    }
    nw_info_list.push_back(nw_info);
  }
  scan_filter.preferredNetworkInfoList = nw_info_list;
  return scan_filter;
}

NativeScanStats OffloadScanUtils::convertToNativeScanStats(
    const ScanStats& scanStats) {
  uint32_t num_channels_scanned = 0;
  uint32_t scan_duration_ms = 0;
  vector<uint8_t> histogram_channels;

  for (size_t i = 0; i < scanStats.scanRecord.size(); i++) {
    scan_duration_ms += scanStats.scanRecord[i].durationMs;
    num_channels_scanned += scanStats.scanRecord[i].numChannelsScanned;
  }
  for (size_t i = 0; i < scanStats.histogramChannelsScanned.size(); i++) {
    histogram_channels.push_back(scanStats.histogramChannelsScanned[i]);
  }

  NativeScanStats native_scan_stats(
      scanStats.numScansRequestedByWifi, scanStats.numScansServicedByWifi,
      scanStats.subscriptionDurationMs, scan_duration_ms, num_channels_scanned,
      histogram_channels);
  return native_scan_stats;
}

}  // namespace wificond
}  // namespace android
