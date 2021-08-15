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

#ifndef WIFICOND_NET_NETLINK_UTILS_H_
#define WIFICOND_NET_NETLINK_UTILS_H_

#include <array>
#include <functional>
#include <string>
#include <vector>

#include <linux/if_ether.h>

#include <android-base/macros.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/netlink_manager.h"

namespace android {
namespace wificond {

struct InterfaceInfo {
  InterfaceInfo() = default;
  InterfaceInfo(uint32_t index,
                const std::string& name,
                const std::array<uint8_t, ETH_ALEN>& mac_address)
      : index(index),
        name(name),
        mac_address(mac_address) {}
  // Index of this interface.
  uint32_t index;
  // Name of this interface.
  std::string name;
  // MAC address of this interface.
  std::array<uint8_t, ETH_ALEN> mac_address;
};

struct BandInfo {
  BandInfo() = default;
  BandInfo(std::vector<uint32_t>& band_2g_,
           std::vector<uint32_t>& band_5g_,
           std::vector<uint32_t>& band_dfs_)
      : band_2g(band_2g_),
        band_5g(band_5g_),
        band_dfs(band_dfs_) {}
  // Frequencies for 2.4 GHz band.
  std::vector<uint32_t> band_2g;
  // Frequencies for 5 GHz band without DFS.
  std::vector<uint32_t> band_5g;
  // Frequencies for DFS.
  std::vector<uint32_t> band_dfs;
};

struct ScanCapabilities {
  ScanCapabilities() = default;
  ScanCapabilities(uint8_t max_num_scan_ssids_,
                   uint8_t max_num_sched_scan_ssids_,
                   uint8_t max_match_sets_,
                   uint32_t max_num_scan_plans_,
                   uint32_t max_scan_plan_interval_,
                   uint32_t max_scan_plan_iterations_)
      : max_num_scan_ssids(max_num_scan_ssids_),
        max_num_sched_scan_ssids(max_num_sched_scan_ssids_),
        max_match_sets(max_match_sets_),
        max_num_scan_plans(max_num_scan_plans_),
        max_scan_plan_interval(max_scan_plan_interval_),
        max_scan_plan_iterations(max_scan_plan_iterations_) {}
  // Number of SSIDs you can scan with a single scan request.
  uint8_t max_num_scan_ssids;
  // Number of SSIDs you can scan with a single scheduled scan request.
  uint8_t max_num_sched_scan_ssids;
  // Maximum number of sets that can be used with NL80211_ATTR_SCHED_SCAN_MATCH.
  uint8_t max_match_sets;
  // Maximum number of scan plans that can be specified.
  uint32_t max_num_scan_plans;
  // Maximum interval in seconds for a particular scan plan that can be
  // specified.
  uint32_t max_scan_plan_interval;
  // Maximum number of iterations for a particular scan plan that can be
  // specified.
  uint32_t max_scan_plan_iterations;
};

struct WiphyFeatures {
  WiphyFeatures()
      : supports_random_mac_oneshot_scan(false),
        supports_random_mac_sched_scan(false),
        supports_low_span_oneshot_scan(false),
        supports_low_power_oneshot_scan(false),
        supports_high_accuracy_oneshot_scan(false),
        supports_tx_mgmt_frame_mcs(false) {}
  WiphyFeatures(uint32_t feature_flags,
                const std::vector<uint8_t>& ext_feature_flags_bytes);
  // This device/driver supports using a random MAC address during scan
  // (while not associated).
  bool supports_random_mac_oneshot_scan;
  // This device/driver supports using a random MAC address for every
  // scan iteration during scheduled scan (while not associated).
  bool supports_random_mac_sched_scan;
  // This device/driver supports performing low-span/low-latency one-shot scans.
  bool supports_low_span_oneshot_scan;
  // This device/driver supports performing low-power one-shot scans.
  bool supports_low_power_oneshot_scan;
  // This device/driver supports performing high-accuracy one-shot scans.
  bool supports_high_accuracy_oneshot_scan;
  // This device/driver supports sending a management frame at a specified MCS.
  bool supports_tx_mgmt_frame_mcs;
  // This device/driver supports sched_scan for reporting BSSs
  // with better RSSI than the current connected BSS
  bool supports_ext_sched_scan_relative_rssi;
  // There are other flags included in NL80211_ATTR_FEATURE_FLAGS.
  // We will add them once we find them useful.
};

struct StationInfo {
  StationInfo() = default;
  StationInfo(uint32_t station_tx_packets_,
              uint32_t station_tx_failed_,
              uint32_t station_tx_bitrate_,
              int8_t current_rssi_,
              uint32_t station_rx_bitrate_)
      : station_tx_packets(station_tx_packets_),
        station_tx_failed(station_tx_failed_),
        station_tx_bitrate(station_tx_bitrate_),
        current_rssi(current_rssi_),
        station_rx_bitrate(station_rx_bitrate_) {}
  // Number of successfully transmitted packets.
  int32_t station_tx_packets;
  // Number of tramsmission failures.
  int32_t station_tx_failed;
  // Transimission bit rate in 100kbit/s.
  uint32_t station_tx_bitrate;
  // Current signal strength.
  int8_t current_rssi;
  // Last Received unicast packet bit rate in 100kbit/s.
  uint32_t station_rx_bitrate;
  // There are many other counters/parameters included in station info.
  // We will add them once we find them useful.
};

class MlmeEventHandler;
class NetlinkManager;
class NL80211Packet;

// Provides NL80211 helper functions.
class NetlinkUtils {
 public:
  // Currently we only support setting the interface to STATION mode.
  // This is used for cleaning up interface after KILLING hostapd.
  enum InterfaceMode{
      STATION_MODE
  };

  explicit NetlinkUtils(NetlinkManager* netlink_manager);
  virtual ~NetlinkUtils();

  // Get the wiphy index from kernel.
  // |*out_wiphy_index| returns the wiphy index from kernel.
  // Returns true on success.
  virtual bool GetWiphyIndex(uint32_t* out_wiphy_index);
  virtual bool GetWiphyIndex(uint32_t* out_wiphy_index,
                             const std::string& iface_name);

  // Get wifi interfaces info from kernel.
  // |wiphy_index| is the wiphy index we get using GetWiphyIndex().
  // |interface_info| returns a vector of InterfaceInfo structs with
  // information about all existing interfaces.
  // Returns true on success.
  virtual bool GetInterfaces(uint32_t wiphy_index,
                             std::vector<InterfaceInfo>* interface_info);

  // Set the mode of interface.
  // |interface_index| is the interface index.
  // |mode| is one of the values in |enum InterfaceMode|.
  // Returns true on success.
  virtual bool SetInterfaceMode(uint32_t interface_index,
                                InterfaceMode mode);

  // Get wiphy capability information from kernel.
  // Returns true on success.
  virtual bool GetWiphyInfo(uint32_t wiphy_index,
                            BandInfo* out_band_info,
                            ScanCapabilities* out_scan_capabilities,
                            WiphyFeatures* out_wiphy_features);

  // Get station info from kernel.
  // |*out_station_info]| is the struct of available station information.
  // Returns true on success.
  virtual bool GetStationInfo(uint32_t interface_index,
                              const std::array<uint8_t, ETH_ALEN>& mac_address,
                              StationInfo* out_station_info);

  // Get a bitmap for nl80211 protocol features,
  // i.e. features for the nl80211 protocol rather than device features.
  // See enum nl80211_protocol_features in nl80211.h for decoding the bitmap.
  // Returns true on success.
  virtual bool GetProtocolFeatures(uint32_t* features);

  // Get current alpha2 country code from kernel.
  // Returns true on success.
  virtual bool GetCountryCode(std::string* out_country_code);

  // Sign up to be notified when there is MLME event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  // NetlinkUtils is not going to take ownership of this pointer, and that it
  // is the caller's responsibility to make sure that the object exists for the
  // duration of the subscription.
  virtual void SubscribeMlmeEvent(uint32_t interface_index,
                                  MlmeEventHandler* handler);

  // Cancel the sign-up of receiving MLME event notification
  // from interface with index |interface_index|.
  virtual void UnsubscribeMlmeEvent(uint32_t interface_index);

  // Sign up to be notified when there is an regulatory domain change.
  // Only one handler can be registered per wiphy index.
  // New handler will replace the registered handler if they are for the
  // same wiphy index.
  virtual void SubscribeRegDomainChange(uint32_t wiphy_index,
                                        OnRegDomainChangedHandler handler);

  // Cancel the sign-up of receiving regulatory domain change notification
  // from wiphy with index |wiphy_index|.
  virtual void UnsubscribeRegDomainChange(uint32_t wiphy_index);

  // Sign up to be notified when there is a station event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeStationEvent(uint32_t interface_index,
                                     OnStationEventHandler handler);

  // Cancel the sign-up of receiving station events.
  virtual void UnsubscribeStationEvent(uint32_t interface_index);

  // Sign up to be notified when there is a channel switch event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeChannelSwitchEvent(uint32_t interface_index,
                                           OnChannelSwitchEventHandler handler);

  // Cancel the sign-up of receiving channel switch events.
  virtual void UnsubscribeChannelSwitchEvent(uint32_t interface_index);

  // Sign up to be notified of frame tx status events.
  virtual void SubscribeFrameTxStatusEvent(
      uint32_t interface_index, OnFrameTxStatusEventHandler handler);

  // Cancel the sign-up of receiving frame tx status events.
  virtual void UnsubscribeFrameTxStatusEvent(uint32_t interface_index);

  virtual bool SendMgmtFrame(uint32_t interface_index,
    const std::vector<uint8_t>& frame, int32_t mcs, uint64_t* out_cookie);

  // Visible for testing.
  bool supports_split_wiphy_dump_;

 private:
  bool ParseWiphyInfoFromPacket(
      const NL80211Packet& packet,
      BandInfo* out_band_info,
      ScanCapabilities* out_scan_capabilities,
      WiphyFeatures* out_wiphy_features);
  bool ParseBandInfo(const NL80211Packet* const packet,
                     BandInfo* out_band_info);
  bool ParseScanCapabilities(const NL80211Packet* const packet,
                             ScanCapabilities* out_scan_capabilities);

  bool MergePacketsForSplitWiphyDump(
      const std::vector<std::unique_ptr<const NL80211Packet>>& split_dump_info,
      std::vector<NL80211Packet>* packet_per_wiphy);

  NetlinkManager* netlink_manager_;

  DISALLOW_COPY_AND_ASSIGN(NetlinkUtils);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_NETLINK_UTILS_H_
