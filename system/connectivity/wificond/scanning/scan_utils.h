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

#ifndef WIFICOND_SCANNING_SCAN_UTILS_H_
#define WIFICOND_SCANNING_SCAN_UTILS_H_

#include <memory>
#include <vector>

#include <android-base/macros.h>

#include "wificond/net/netlink_manager.h"

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanResult;
class RadioChainInfo;

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

namespace android {
namespace wificond {

class NL80211NestedAttr;
class NL80211Packet;

struct SchedScanIntervalSetting {
  struct ScanPlan {
    uint32_t interval_ms;
    uint32_t n_iterations;
  };
  std::vector<ScanPlan> plans;
  // After |plans| has been exhausted, scan at every
  // |final_interval_ms|.
  uint32_t final_interval_ms{0};
};

struct SchedScanReqFlags {
  bool request_random_mac;
  bool request_low_power;
  bool request_sched_scan_relative_rssi;
};

// Provides scanning helper functions.
class ScanUtils {
 public:
  explicit ScanUtils(NetlinkManager* netlink_manager);
  virtual ~ScanUtils();

  // Send 'get scan results' request to kernel and get the latest scan results.
  // |interface_index| is the index of interface we want to get scan results
  // from.
  // A vector of ScanResult object will be returned by |*out_scan_results|.
  // Returns true on success.
  virtual bool GetScanResult(
      uint32_t interface_index,
      std::vector<::com::android::server::wifi::wificond::NativeScanResult>* out_scan_results);

  // Send scan request to kernel for interface with index |interface_index|.
  // - |request_random_mac| If true, request device/driver to use a random MAC
  // address during scan. Requires |supports_random_mac_sched_scan|
  // address during scan.
  // - |scan_type| Type of scan to perform. One of,
  // |SCAN_TYPE_LOW_SPAN| (prioritize to reduce latency over other scan
  // performance attributes),
  // |SCAN_TYPE_LOW_POWER| (prioritize to reduce power consumption over other
  // scan performance attributes),
  // |SCAN_TYPE_HIGH_ACCURACY| (prioritize to increase accuracy over other scan
  // performance atrributes) OR
  // |SCAN_TYPE_DEFAULT| (no prioritization).
  // - |ssids| is a vector of ssids we request to scan, which mostly is used
  // for hidden networks.
  // If |ssids| is an empty vector, it will do a passive scan.
  // If |ssids| contains an empty string, it will a scan for all ssids.
  // - |freqs| is a vector of frequencies we request to scan.
  // If |freqs| is an empty vector, it will scan all supported frequencies.
  // - |error_code| contains the errno kernel replied when this returns false.
  // Returns true on success.
  virtual bool Scan(uint32_t interface_index,
                    bool request_random_mac,
                    int scan_type,
                    const std::vector<std::vector<uint8_t>>& ssids,
                    const std::vector<uint32_t>& freqs,
                    int* error_code);

  // Send scan request to kernel for interface with index |interface_index|.
  // - |inteval_ms| is the expected scan interval in milliseconds.
  // - |rssi_threshold_2g| is the minimum RSSI threshold value as a filter for
  // 2GHz band.
  // - |rssi_threshold_5g| is the minimum RSSI threshold value as a filter for
  // 5GHz band.
  // - |scan_ssids| is a vector of ssids we request to scan, which is mostly
  // used for hidden networks.
  // - |request_random_mac| If true, request device/driver to use a random MAC
  // address during scan. Requires |supports_random_mac_sched_scan|
  // - |request_low_power|: If true, prioritize power consumption over
  // other scan performance attributes.
  // Requires |supports_low_power_oneshot_scan|.
  // - |request_sched_scan_relative_rssi| is sched_scan flag for better BSS's from connected BSS.
  // If |request_sched_scan_relative_rssi| is true, it will fill scan rssi adjust to
  // get BSS's with better RSSI from connected BSS.
  // - |scan_ssids| is the list of ssids to actively scan for.
  // If |scan_ssids| is an empty vector, it will do a passive scan.
  // If |scan_ssids| contains an empty string, it will a scan for all ssids.
  // - |match_ssids| is the list of ssids that we want to add as filters.
  // - |freqs| is a vector of frequencies we request to scan.
  // If |freqs| is an empty vector, it will scan all supported frequencies.
  // - |error_code| contains the errno kernel replied when this returns false.
  // Only BSSs match the |match_ssids| and |rssi_threshold| will be returned as
  // scan results.
  // Returns true on success.
  virtual bool StartScheduledScan(
      uint32_t interface_index,
      const SchedScanIntervalSetting& interval_setting,
      int32_t rssi_threshold_2g,
      int32_t rssi_threshold_5g,
      const SchedScanReqFlags& req_flags,
      const std::vector<std::vector<uint8_t>>& scan_ssids,
      const std::vector<std::vector<uint8_t>>& match_ssids,
      const std::vector<uint32_t>& freqs,
      int* error_code);

  // Stop existing scheduled scan on interface with index |interface_index|.
  // Returns true on success.
  // Returns false on error or when there is no scheduled scan running.
  virtual bool StopScheduledScan(uint32_t interface_index);

  // Abort ongoing single scan on interface with index |interface_index|.
  // Returns true on success.
  virtual bool AbortScan(uint32_t interface_index);

  // Visible for testing.
  // Get a timestamp for the scan result |bss| represents.
  // This timestamp records the time passed since boot when last time the
  // AP was seen.
  virtual bool GetBssTimestampForTesting(
      const NL80211NestedAttr& bss,
       uint64_t* last_seen_since_boot_microseconds);

  // Sign up to be notified when new scan results are available.
  // |handler| will be called when the kernel signals to wificond that a scan
  // has been completed on the given |interface_index|.  See the declaration of
  // OnScanResultsReadyHandler for documentation on the semantics of this
  // callback.
  virtual void SubscribeScanResultNotification(
      uint32_t interface_index,
      OnScanResultsReadyHandler handler);

  // Cancel the sign-up of receiving new scan result notification from
  // interface with index |interface_index|.
  virtual void UnsubscribeScanResultNotification(uint32_t interface_index);

  // Sign up to be notified when new scan results are available.
  // |handler| will be called when the kernel signals to wificond that a
  // scheduled scan has been completed on the given |interface_index|.
  // See the declaration of OnSchedScanResultsReadyHandler for documentation
  // on the semantics of this callback.
  virtual void SubscribeSchedScanResultNotification(
      uint32_t interface_index,
      OnSchedScanResultsReadyHandler handler);

  // Cancel the sign-up of receiving new scheduled scan result notification from
  // interface with index |interface_index|.
  virtual void UnsubscribeSchedScanResultNotification(uint32_t interface_index);

 private:
  bool GetBssTimestamp(const NL80211NestedAttr& bss,
                       uint64_t* last_seen_since_boot_microseconds);
  bool ParseRadioChainInfos(
      const NL80211NestedAttr& bss,
      std::vector<::com::android::server::wifi::wificond::RadioChainInfo>
        *radio_chain_infos);
  bool GetSSIDFromInfoElement(const std::vector<uint8_t>& ie,
                              std::vector<uint8_t>* ssid);
  // Converts a NL80211_CMD_NEW_SCAN_RESULTS packet to a ScanResult object.
  bool ParseScanResult(
      std::unique_ptr<const NL80211Packet> packet,
      ::com::android::server::wifi::wificond::NativeScanResult* scan_result);

  NetlinkManager* netlink_manager_;

  DISALLOW_COPY_AND_ASSIGN(ScanUtils);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_SCANNING_SCAN_UTILS_H_
