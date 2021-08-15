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

#include "wificond/net/netlink_utils.h"

#include <array>
#include <algorithm>
#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <net/if.h>
#include <linux/netlink.h>

#include <android-base/logging.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/mlme_event_handler.h"
#include "wificond/net/nl80211_packet.h"

using std::array;
using std::make_pair;
using std::make_unique;
using std::map;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

namespace {

uint32_t k2GHzFrequencyLowerBound = 2400;
uint32_t k2GHzFrequencyUpperBound = 2500;

uint32_t k5GHzFrequencyLowerBound = 5000;
// This upper bound will exclude any 5.9Ghz channels which belong to 802.11p
// for "vehicular communication systems".
uint32_t k5GHzFrequencyUpperBound = 5850;

bool IsExtFeatureFlagSet(
    const std::vector<uint8_t>& ext_feature_flags_bytes,
    enum nl80211_ext_feature_index ext_feature_flag) {
  static_assert(NUM_NL80211_EXT_FEATURES <= SIZE_MAX,
                "Ext feature values doesn't fit in |size_t|");
  // TODO:This is an unsafe cast because this assumes that the values
  // are always unsigned!
  size_t ext_feature_flag_idx = static_cast<size_t>(ext_feature_flag);
  size_t ext_feature_flag_byte_pos = ext_feature_flag_idx / 8;
  size_t ext_feature_flag_bit_pos = ext_feature_flag_idx % 8;
  if (ext_feature_flag_byte_pos >= ext_feature_flags_bytes.size()) {
    return false;
  }
  uint8_t ext_feature_flag_byte =
      ext_feature_flags_bytes[ext_feature_flag_byte_pos];
  return (ext_feature_flag_byte & (1U << ext_feature_flag_bit_pos));
}
}  // namespace

WiphyFeatures::WiphyFeatures(uint32_t feature_flags,
                             const std::vector<uint8_t>& ext_feature_flags_bytes)
    : supports_random_mac_oneshot_scan(
            feature_flags & NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR),
        supports_random_mac_sched_scan(
            feature_flags & NL80211_FEATURE_SCHED_SCAN_RANDOM_MAC_ADDR) {
  supports_low_span_oneshot_scan =
      IsExtFeatureFlagSet(ext_feature_flags_bytes,
                          NL80211_EXT_FEATURE_LOW_SPAN_SCAN);
  supports_low_power_oneshot_scan =
      IsExtFeatureFlagSet(ext_feature_flags_bytes,
                          NL80211_EXT_FEATURE_LOW_POWER_SCAN);
  supports_high_accuracy_oneshot_scan =
      IsExtFeatureFlagSet(ext_feature_flags_bytes,
                          NL80211_EXT_FEATURE_HIGH_ACCURACY_SCAN);
  // TODO (b/112029045) check if sending frame at specified MCS is supported
  supports_tx_mgmt_frame_mcs = false;
  supports_ext_sched_scan_relative_rssi =
      IsExtFeatureFlagSet(ext_feature_flags_bytes,
                          NL80211_EXT_FEATURE_SCHED_SCAN_RELATIVE_RSSI);
}

NetlinkUtils::NetlinkUtils(NetlinkManager* netlink_manager)
    : netlink_manager_(netlink_manager) {
  if (!netlink_manager_->IsStarted()) {
    netlink_manager_->Start();
  }
  uint32_t protocol_features = 0;
  supports_split_wiphy_dump_ = GetProtocolFeatures(&protocol_features) &&
      (protocol_features & NL80211_PROTOCOL_FEATURE_SPLIT_WIPHY_DUMP);
}

NetlinkUtils::~NetlinkUtils() {}

bool NetlinkUtils::GetWiphyIndex(uint32_t* out_wiphy_index,
                                 const std::string& iface_name) {
  NL80211Packet get_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  get_wiphy.AddFlag(NLM_F_DUMP);
  if (!iface_name.empty()) {
    int ifindex = if_nametoindex(iface_name.c_str());
    get_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX, ifindex));
  }
  vector<unique_ptr<const NL80211Packet>> response;
  if (!netlink_manager_->SendMessageAndGetResponses(get_wiphy, &response))  {
    LOG(ERROR) << "NL80211_CMD_GET_WIPHY dump failed";
    return false;
  }
  if (response.empty()) {
    LOG(DEBUG) << "No wiphy is found";
    return false;
  }
  for (auto& packet : response) {
    if (packet->GetMessageType() == NLMSG_ERROR) {
      LOG(ERROR) << "Receive ERROR message: "
                 << strerror(packet->GetErrorCode());
      return false;
    }
    if (packet->GetMessageType() != netlink_manager_->GetFamilyId()) {
      LOG(ERROR) << "Wrong message type for new interface message: "
                 << packet->GetMessageType();
      return false;
    }
    if (packet->GetCommand() != NL80211_CMD_NEW_WIPHY) {
      LOG(ERROR) << "Wrong command in response to "
                 << "a wiphy dump request: "
                 << static_cast<int>(packet->GetCommand());
      return false;
    }
    if (!packet->GetAttributeValue(NL80211_ATTR_WIPHY, out_wiphy_index)) {
      LOG(ERROR) << "Failed to get wiphy index from reply message";
      return false;
    }
  }
  return true;
}

bool NetlinkUtils::GetWiphyIndex(uint32_t* out_wiphy_index) {
  return GetWiphyIndex(out_wiphy_index, "");
}

bool NetlinkUtils::GetInterfaces(uint32_t wiphy_index,
                                 vector<InterfaceInfo>* interface_info) {
  NL80211Packet get_interfaces(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());

  get_interfaces.AddFlag(NLM_F_DUMP);
  get_interfaces.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY, wiphy_index));
  vector<unique_ptr<const NL80211Packet>> response;
  if (!netlink_manager_->SendMessageAndGetResponses(get_interfaces, &response)) {
    LOG(ERROR) << "NL80211_CMD_GET_INTERFACE dump failed";
    return false;
  }
  if (response.empty()) {
    LOG(ERROR) << "No interface is found";
    return false;
  }
  for (auto& packet : response) {
    if (packet->GetMessageType() == NLMSG_ERROR) {
      LOG(ERROR) << "Receive ERROR message: "
                 << strerror(packet->GetErrorCode());
      return false;
    }
    if (packet->GetMessageType() != netlink_manager_->GetFamilyId()) {
      LOG(ERROR) << "Wrong message type for new interface message: "
                 << packet->GetMessageType();
      return false;
    }
    if (packet->GetCommand() != NL80211_CMD_NEW_INTERFACE) {
      LOG(ERROR) << "Wrong command in response to "
                 << "an interface dump request: "
                 << static_cast<int>(packet->GetCommand());
      return false;
    }

    // In some situations, it has been observed that the kernel tells us
    // about a pseudo interface that does not have a real netdev.  In this
    // case, responses will have a NL80211_ATTR_WDEV, and not the expected
    // IFNAME/IFINDEX. In this case we just skip these pseudo interfaces.
    uint32_t if_index;
    if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
      LOG(DEBUG) << "Failed to get interface index";
      continue;
    }

    // Today we don't check NL80211_ATTR_IFTYPE because at this point of time
    // driver always reports that interface is in STATION mode. Even when we
    // are asking interfaces infomation on behalf of tethering, it is still so
    // because hostapd is supposed to set interface to AP mode later.

    string if_name;
    if (!packet->GetAttributeValue(NL80211_ATTR_IFNAME, &if_name)) {
      LOG(WARNING) << "Failed to get interface name";
      continue;
    }

    array<uint8_t, ETH_ALEN> if_mac_addr;
    if (!packet->GetAttributeValue(NL80211_ATTR_MAC, &if_mac_addr)) {
      LOG(WARNING) << "Failed to get interface mac address";
      continue;
    }

    interface_info->emplace_back(if_index, if_name, if_mac_addr);
  }

  return true;
}

bool NetlinkUtils::SetInterfaceMode(uint32_t interface_index,
                                    InterfaceMode mode) {
  uint32_t set_to_mode = NL80211_IFTYPE_UNSPECIFIED;
  if (mode == STATION_MODE) {
    set_to_mode = NL80211_IFTYPE_STATION;
  } else {
    LOG(ERROR) << "Unexpected mode for interface with index: "
               << interface_index;
    return false;
  }
  NL80211Packet set_interface_mode(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_SET_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  // Force an ACK response upon success.
  set_interface_mode.AddFlag(NLM_F_ACK);

  set_interface_mode.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX, interface_index));
  set_interface_mode.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_IFTYPE, set_to_mode));

  if (!netlink_manager_->SendMessageAndGetAck(set_interface_mode)) {
    LOG(ERROR) << "NL80211_CMD_SET_INTERFACE failed";
    return false;
  }

  return true;
}

bool NetlinkUtils::GetProtocolFeatures(uint32_t* features) {
  NL80211Packet get_protocol_features(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_PROTOCOL_FEATURES,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  unique_ptr<const NL80211Packet> response;
  if (!netlink_manager_->SendMessageAndGetSingleResponse(get_protocol_features,
                                                         &response)) {
    LOG(ERROR) << "NL80211_CMD_GET_PROTOCOL_FEATURES failed";
    return false;
  }
  if (!response->GetAttributeValue(NL80211_ATTR_PROTOCOL_FEATURES, features)) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_PROTOCOL_FEATURES";
    return false;
  }
  return true;
}

bool NetlinkUtils::GetWiphyInfo(
    uint32_t wiphy_index,
    BandInfo* out_band_info,
    ScanCapabilities* out_scan_capabilities,
    WiphyFeatures* out_wiphy_features) {
  NL80211Packet get_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  get_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY, wiphy_index));
  if (supports_split_wiphy_dump_) {
    get_wiphy.AddFlagAttribute(NL80211_ATTR_SPLIT_WIPHY_DUMP);
    get_wiphy.AddFlag(NLM_F_DUMP);
  }
  vector<unique_ptr<const NL80211Packet>> response;
  if (!netlink_manager_->SendMessageAndGetResponses(get_wiphy, &response))  {
    LOG(ERROR) << "NL80211_CMD_GET_WIPHY dump failed";
    return false;
  }

  vector<NL80211Packet> packet_per_wiphy;
  if (supports_split_wiphy_dump_) {
    if (!MergePacketsForSplitWiphyDump(response, &packet_per_wiphy)) {
      LOG(WARNING) << "Failed to merge responses from split wiphy dump";
    }
  } else {
    for (auto& packet : response) {
      packet_per_wiphy.push_back(move(*(packet.release())));
    }
  }

  for (const auto& packet : packet_per_wiphy) {
    uint32_t current_wiphy_index;
    if (!packet.GetAttributeValue(NL80211_ATTR_WIPHY, &current_wiphy_index) ||
        // Not the wihpy we requested.
        current_wiphy_index != wiphy_index) {
      continue;
    }
    if (ParseWiphyInfoFromPacket(packet, out_band_info,
                                 out_scan_capabilities, out_wiphy_features)) {
      return true;
    }
  }

  LOG(ERROR) << "Failed to find expected wiphy info "
             << "from NL80211_CMD_GET_WIPHY responses";
  return false;
}

bool NetlinkUtils::ParseWiphyInfoFromPacket(
    const NL80211Packet& packet,
    BandInfo* out_band_info,
    ScanCapabilities* out_scan_capabilities,
    WiphyFeatures* out_wiphy_features) {
  if (packet.GetCommand() != NL80211_CMD_NEW_WIPHY) {
    LOG(ERROR) << "Wrong command in response to a get wiphy request: "
               << static_cast<int>(packet.GetCommand());
    return false;
  }
  if (!ParseBandInfo(&packet, out_band_info) ||
      !ParseScanCapabilities(&packet, out_scan_capabilities)) {
    return false;
  }
  uint32_t feature_flags;
  if (!packet.GetAttributeValue(NL80211_ATTR_FEATURE_FLAGS,
                                 &feature_flags)) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_FEATURE_FLAGS";
    return false;
  }
  std::vector<uint8_t> ext_feature_flags_bytes;
  if (!packet.GetAttributeValue(NL80211_ATTR_EXT_FEATURES,
                                &ext_feature_flags_bytes)) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_EXT_FEATURES";
  }
  *out_wiphy_features = WiphyFeatures(feature_flags,
                                      ext_feature_flags_bytes);
  return true;
}

bool NetlinkUtils::ParseScanCapabilities(
    const NL80211Packet* const packet,
    ScanCapabilities* out_scan_capabilities) {
  uint8_t max_num_scan_ssids;
  if (!packet->GetAttributeValue(NL80211_ATTR_MAX_NUM_SCAN_SSIDS,
                                   &max_num_scan_ssids)) {
    LOG(ERROR) << "Failed to get the capacity of maximum number of scan ssids";
    return false;
  }

  uint8_t max_num_sched_scan_ssids;
  if (!packet->GetAttributeValue(NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS,
                                 &max_num_sched_scan_ssids)) {
    LOG(ERROR) << "Failed to get the capacity of "
               << "maximum number of scheduled scan ssids";
    return false;
  }

  // Use default value 0 for scan plan capabilities if attributes are missing.
  uint32_t max_num_scan_plans = 0;
  packet->GetAttributeValue(NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS,
                            &max_num_scan_plans);
  uint32_t max_scan_plan_interval = 0;
  packet->GetAttributeValue(NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL,
                            &max_scan_plan_interval);
  uint32_t max_scan_plan_iterations = 0;
  packet->GetAttributeValue(NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS,
                            &max_scan_plan_iterations);

  uint8_t max_match_sets;
  if (!packet->GetAttributeValue(NL80211_ATTR_MAX_MATCH_SETS,
                                   &max_match_sets)) {
    LOG(ERROR) << "Failed to get the capacity of maximum number of match set"
               << "of a scheduled scan";
    return false;
  }
  *out_scan_capabilities = ScanCapabilities(max_num_scan_ssids,
                                            max_num_sched_scan_ssids,
                                            max_match_sets,
                                            max_num_scan_plans,
                                            max_scan_plan_interval,
                                            max_scan_plan_iterations);
  return true;
}

bool NetlinkUtils::ParseBandInfo(const NL80211Packet* const packet,
                                 BandInfo* out_band_info) {

  NL80211NestedAttr bands_attr(0);
  if (!packet->GetAttribute(NL80211_ATTR_WIPHY_BANDS, &bands_attr)) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_WIPHY_BANDS";
    return false;
  }
  vector<NL80211NestedAttr> bands;
  if (!bands_attr.GetListOfNestedAttributes(&bands)) {
    LOG(ERROR) << "Failed to get bands within NL80211_ATTR_WIPHY_BANDS";
    return false;
  }
  vector<uint32_t> frequencies_2g;
  vector<uint32_t> frequencies_5g;
  vector<uint32_t> frequencies_dfs;
  for (unsigned int band_index = 0; band_index < bands.size(); band_index++) {
    NL80211NestedAttr freqs_attr(0);
    if (!bands[band_index].GetAttribute(NL80211_BAND_ATTR_FREQS, &freqs_attr)) {
      LOG(DEBUG) << "Failed to get NL80211_BAND_ATTR_FREQS";
      continue;
    }
    vector<NL80211NestedAttr> freqs;
    if (!freqs_attr.GetListOfNestedAttributes(&freqs)) {
      LOG(ERROR) << "Failed to get frequencies within NL80211_BAND_ATTR_FREQS";
      continue;
    }
    for (auto& freq : freqs) {
      uint32_t frequency_value;
      if (!freq.GetAttributeValue(NL80211_FREQUENCY_ATTR_FREQ,
                                  &frequency_value)) {
        LOG(DEBUG) << "Failed to get NL80211_FREQUENCY_ATTR_FREQ";
        continue;
      }
      // Channel is disabled in current regulatory domain.
      if (freq.HasAttribute(NL80211_FREQUENCY_ATTR_DISABLED)) {
        continue;
      }
      if (frequency_value > k2GHzFrequencyLowerBound &&
            frequency_value < k2GHzFrequencyUpperBound) {
          frequencies_2g.push_back(frequency_value);
      } else if (frequency_value > k5GHzFrequencyLowerBound &&
            frequency_value < k5GHzFrequencyUpperBound) {
        // If this is an available/usable DFS frequency, we should save it to
        // DFS frequencies list.
        uint32_t dfs_state;
        if (freq.GetAttributeValue(NL80211_FREQUENCY_ATTR_DFS_STATE,
                                   &dfs_state) &&
            (dfs_state == NL80211_DFS_AVAILABLE ||
                 dfs_state == NL80211_DFS_USABLE)) {
          frequencies_dfs.push_back(frequency_value);
          continue;
        }

        // Put non-dfs passive-only channels into the dfs category.
        // This aligns with what framework always assumes.
        if (freq.HasAttribute(NL80211_FREQUENCY_ATTR_NO_IR)) {
          frequencies_dfs.push_back(frequency_value);
          continue;
        }

        // Otherwise, this is a regular 5g frequency.
        frequencies_5g.push_back(frequency_value);
      }

    }
  }
  *out_band_info = BandInfo(frequencies_2g, frequencies_5g, frequencies_dfs);
  return true;
}

bool NetlinkUtils::GetStationInfo(uint32_t interface_index,
                                  const array<uint8_t, ETH_ALEN>& mac_address,
                                  StationInfo* out_station_info) {
  NL80211Packet get_station(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_STATION,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  get_station.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX,
                                                 interface_index));
  get_station.AddAttribute(NL80211Attr<array<uint8_t, ETH_ALEN>>(
      NL80211_ATTR_MAC, mac_address));

  unique_ptr<const NL80211Packet> response;
  if (!netlink_manager_->SendMessageAndGetSingleResponse(get_station,
                                                         &response)) {
    LOG(ERROR) << "NL80211_CMD_GET_STATION failed";
    return false;
  }
  if (response->GetCommand() != NL80211_CMD_NEW_STATION) {
    LOG(ERROR) << "Wrong command in response to a get station request: "
               << static_cast<int>(response->GetCommand());
    return false;
  }
  NL80211NestedAttr sta_info(0);
  if (!response->GetAttribute(NL80211_ATTR_STA_INFO, &sta_info)) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_STA_INFO";
    return false;
  }
  int32_t tx_good, tx_bad;
  if (!sta_info.GetAttributeValue(NL80211_STA_INFO_TX_PACKETS, &tx_good)) {
    LOG(ERROR) << "Failed to get NL80211_STA_INFO_TX_PACKETS";
    return false;
  }
  if (!sta_info.GetAttributeValue(NL80211_STA_INFO_TX_FAILED, &tx_bad)) {
    LOG(ERROR) << "Failed to get NL80211_STA_INFO_TX_FAILED";
    return false;
  }
  int8_t current_rssi;
  if (!sta_info.GetAttributeValue(NL80211_STA_INFO_SIGNAL, &current_rssi)) {
    LOG(ERROR) << "Failed to get NL80211_STA_INFO_SIGNAL";
    return false;
  }
  NL80211NestedAttr tx_bitrate_attr(0);
  uint32_t tx_bitrate = 0;
  if (sta_info.GetAttribute(NL80211_STA_INFO_TX_BITRATE,
                            &tx_bitrate_attr)) {
    if (!tx_bitrate_attr.GetAttributeValue(NL80211_RATE_INFO_BITRATE32,
                                         &tx_bitrate)) {
      // Return invalid tx rate to avoid breaking the get station cmd
      tx_bitrate = 0;
    }
  }
  NL80211NestedAttr rx_bitrate_attr(0);
  uint32_t rx_bitrate = 0;
  if (sta_info.GetAttribute(NL80211_STA_INFO_RX_BITRATE,
                            &rx_bitrate_attr)) {
    if (!rx_bitrate_attr.GetAttributeValue(NL80211_RATE_INFO_BITRATE32,
                                         &rx_bitrate)) {
      // Return invalid rx rate to avoid breaking the get station cmd
      rx_bitrate = 0;
    }
  }
  *out_station_info = StationInfo(tx_good, tx_bad, tx_bitrate, current_rssi, rx_bitrate);
  return true;
}

// This is a helper function for merging split NL80211_CMD_NEW_WIPHY packets.
// For example:
// First NL80211_CMD_NEW_WIPHY has attribute A with payload 0x1234.
// Second NL80211_CMD_NEW_WIPHY has attribute A with payload 0x5678.
// The generated NL80211_CMD_NEW_WIPHY will have attribute A with
// payload 0x12345678.
// NL80211_ATTR_WIPHY, NL80211_ATTR_IFINDEX, and NL80211_ATTR_WDEV
// are used for filtering packets so we know which packets should
// be merged together.
bool NetlinkUtils::MergePacketsForSplitWiphyDump(
    const vector<unique_ptr<const NL80211Packet>>& split_dump_info,
    vector<NL80211Packet>* packet_per_wiphy) {
  map<uint32_t, map<int, BaseNL80211Attr>> attr_by_wiphy_and_id;

  // Construct the map using input packets.
  for (const auto& packet : split_dump_info) {
    uint32_t wiphy_index;
    if (!packet->GetAttributeValue(NL80211_ATTR_WIPHY, &wiphy_index)) {
      LOG(ERROR) << "Failed to get NL80211_ATTR_WIPHY from wiphy split dump";
      return false;
    }
    vector<BaseNL80211Attr> attributes;
    if (!packet->GetAllAttributes(&attributes)) {
      return false;
    }
    for (auto& attr : attributes) {
      int attr_id = attr.GetAttributeId();
      if (attr_id != NL80211_ATTR_WIPHY &&
          attr_id != NL80211_ATTR_IFINDEX &&
              attr_id != NL80211_ATTR_WDEV) {
          auto attr_id_and_attr =
              attr_by_wiphy_and_id[wiphy_index].find(attr_id);
          if (attr_id_and_attr == attr_by_wiphy_and_id[wiphy_index].end()) {
            attr_by_wiphy_and_id[wiphy_index].
                insert(make_pair(attr_id, move(attr)));
          } else {
            attr_id_and_attr->second.Merge(attr);
          }
      }
    }
  }

  // Generate output packets using the constructed map.
  for (const auto& wiphy_and_attributes : attr_by_wiphy_and_id) {
    NL80211Packet new_wiphy(0, NL80211_CMD_NEW_WIPHY, 0, 0);
    new_wiphy.AddAttribute(
        NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY, wiphy_and_attributes.first));
    for (const auto& attr : wiphy_and_attributes.second) {
      new_wiphy.AddAttribute(attr.second);
    }
    packet_per_wiphy->emplace_back(move(new_wiphy));
  }
  return true;
}

bool NetlinkUtils::GetCountryCode(string* out_country_code) {
  NL80211Packet get_country_code(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_REG,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  unique_ptr<const NL80211Packet> response;
  if (!netlink_manager_->SendMessageAndGetSingleResponse(get_country_code,
                                                         &response)) {
    LOG(ERROR) << "NL80211_CMD_GET_REG failed";
    return false;
  }
  if (!response->GetAttributeValue(NL80211_ATTR_REG_ALPHA2, out_country_code)) {
    LOG(ERROR) << "Get NL80211_ATTR_REG_ALPHA2 failed";
    return false;
  }
  return true;
}

bool NetlinkUtils::SendMgmtFrame(uint32_t interface_index,
    const vector<uint8_t>& frame, int32_t mcs, uint64_t* out_cookie) {

  NL80211Packet send_mgmt_frame(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_FRAME,
      netlink_manager_->GetSequenceNumber(),
      getpid());

  send_mgmt_frame.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX, interface_index));

  send_mgmt_frame.AddAttribute(
      NL80211Attr<vector<uint8_t>>(NL80211_ATTR_FRAME, frame));

  if (mcs >= 0) {
    // TODO (b/112029045) if mcs >= 0, add MCS attribute
  }

  unique_ptr<const NL80211Packet> response;
  if (!netlink_manager_->SendMessageAndGetSingleResponse(
      send_mgmt_frame, &response)) {
    LOG(ERROR) << "NL80211_CMD_FRAME failed";
    return false;
  }

  if (!response->GetAttributeValue(NL80211_ATTR_COOKIE, out_cookie)) {
    LOG(ERROR) << "Get NL80211_ATTR_COOKIE failed";
    return false;
  }

  return true;
}

void NetlinkUtils::SubscribeMlmeEvent(uint32_t interface_index,
                                      MlmeEventHandler* handler) {
  netlink_manager_->SubscribeMlmeEvent(interface_index, handler);
}

void NetlinkUtils::UnsubscribeMlmeEvent(uint32_t interface_index) {
  netlink_manager_->UnsubscribeMlmeEvent(interface_index);
}

void NetlinkUtils::SubscribeRegDomainChange(
    uint32_t wiphy_index,
    OnRegDomainChangedHandler handler) {
  netlink_manager_->SubscribeRegDomainChange(wiphy_index, handler);
}

void NetlinkUtils::UnsubscribeRegDomainChange(uint32_t wiphy_index) {
  netlink_manager_->UnsubscribeRegDomainChange(wiphy_index);
}

void NetlinkUtils::SubscribeStationEvent(uint32_t interface_index,
                                         OnStationEventHandler handler) {
  netlink_manager_->SubscribeStationEvent(interface_index, handler);
}

void NetlinkUtils::UnsubscribeStationEvent(uint32_t interface_index) {
  netlink_manager_->UnsubscribeStationEvent(interface_index);
}

void NetlinkUtils::SubscribeChannelSwitchEvent(uint32_t interface_index,
                                         OnChannelSwitchEventHandler handler) {
  netlink_manager_->SubscribeChannelSwitchEvent(interface_index, handler);
}

void NetlinkUtils::UnsubscribeChannelSwitchEvent(uint32_t interface_index) {
  netlink_manager_->UnsubscribeChannelSwitchEvent(interface_index);
}

void NetlinkUtils::SubscribeFrameTxStatusEvent(
    uint32_t interface_index, OnFrameTxStatusEventHandler handler) {
  netlink_manager_->SubscribeFrameTxStatusEvent(interface_index, handler);
}

void NetlinkUtils::UnsubscribeFrameTxStatusEvent(uint32_t interface_index) {
  netlink_manager_->UnsubscribeFrameTxStatusEvent(interface_index);
}

}  // namespace wificond
}  // namespace android
