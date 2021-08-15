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

#include "wificond/client_interface_impl.h"

#include <vector>

#include <android-base/logging.h>
#include <utils/Timers.h>

#include "wificond/client_interface_binder.h"
#include "wificond/logging_utils.h"
#include "wificond/net/mlme_event.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/offload/offload_service_utils.h"
#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/scan_utils.h"
#include "wificond/scanning/scanner_impl.h"

using android::net::wifi::IClientInterface;
using android::net::wifi::ISendMgmtFrameEvent;
using com::android::server::wifi::wificond::NativeScanResult;
using android::sp;
using android::wifi_system::InterfaceTool;

using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

MlmeEventHandlerImpl::MlmeEventHandlerImpl(ClientInterfaceImpl* client_interface)
    : client_interface_(client_interface) {
}

MlmeEventHandlerImpl::~MlmeEventHandlerImpl() {
}

void MlmeEventHandlerImpl::OnConnect(unique_ptr<MlmeConnectEvent> event) {
  if (!event->IsTimeout() && event->GetStatusCode() == 0) {
    client_interface_->is_associated_ = true;
    client_interface_->RefreshAssociateFreq();
    client_interface_->bssid_ = event->GetBSSID();
  } else {
    if (event->IsTimeout()) {
      LOG(INFO) << "Connect timeout";
    }
    client_interface_->is_associated_ = false;
    client_interface_->bssid_.fill(0);
  }
}

void MlmeEventHandlerImpl::OnRoam(unique_ptr<MlmeRoamEvent> event) {
  client_interface_->is_associated_ = true;
  client_interface_->RefreshAssociateFreq();
  client_interface_->bssid_ = event->GetBSSID();
}

void MlmeEventHandlerImpl::OnAssociate(unique_ptr<MlmeAssociateEvent> event) {
  if (!event->IsTimeout() && event->GetStatusCode() == 0) {
    client_interface_->is_associated_ = true;
    client_interface_->RefreshAssociateFreq();
    client_interface_->bssid_ = event->GetBSSID();
  } else {
    if (event->IsTimeout()) {
      LOG(INFO) << "Associate timeout";
    }
    client_interface_->is_associated_ = false;
    client_interface_->bssid_.fill(0);
  }
}

void MlmeEventHandlerImpl::OnDisconnect(unique_ptr<MlmeDisconnectEvent> event) {
  client_interface_->is_associated_ = false;
  client_interface_->bssid_.fill(0);
}

void MlmeEventHandlerImpl::OnDisassociate(unique_ptr<MlmeDisassociateEvent> event) {
  client_interface_->is_associated_ = false;
  client_interface_->bssid_.fill(0);
}


ClientInterfaceImpl::ClientInterfaceImpl(
    uint32_t wiphy_index,
    const std::string& interface_name,
    uint32_t interface_index,
    const std::array<uint8_t, ETH_ALEN>& interface_mac_addr,
    InterfaceTool* if_tool,
    NetlinkUtils* netlink_utils,
    ScanUtils* scan_utils)
    : wiphy_index_(wiphy_index),
      interface_name_(interface_name),
      interface_index_(interface_index),
      interface_mac_addr_(interface_mac_addr),
      if_tool_(if_tool),
      netlink_utils_(netlink_utils),
      scan_utils_(scan_utils),
      offload_service_utils_(new OffloadServiceUtils()),
      mlme_event_handler_(new MlmeEventHandlerImpl(this)),
      binder_(new ClientInterfaceBinder(this)),
      is_associated_(false),
      frame_tx_in_progress_(false),
      frame_tx_status_cookie_(0),
      on_frame_tx_status_event_handler_([](bool was_acked) {}) {
  netlink_utils_->SubscribeMlmeEvent(
      interface_index_,
      mlme_event_handler_.get());

  netlink_utils_->SubscribeFrameTxStatusEvent(
      interface_index,
      [this](uint64_t cookie, bool was_acked) {
        if (frame_tx_in_progress_ && frame_tx_status_cookie_ == cookie) {
          on_frame_tx_status_event_handler_(was_acked);
          frame_tx_in_progress_ = false;
          frame_tx_status_cookie_ = 0;
          on_frame_tx_status_event_handler_ = [](bool was_acked) {};
        }
      });

  if (!netlink_utils_->GetWiphyInfo(wiphy_index_,
                               &band_info_,
                               &scan_capabilities_,
                               &wiphy_features_)) {
    LOG(ERROR) << "Failed to get wiphy info from kernel";
  }
  LOG(INFO) << "create scanner for interface with index: "
            << (int)interface_index_;
  scanner_ = new ScannerImpl(interface_index_,
                             scan_capabilities_,
                             wiphy_features_,
                             this,
                             scan_utils_,
                             offload_service_utils_);
  // Need to set the interface up (especially in scan mode since wpa_supplicant
  // is not started)
  if_tool_->SetUpState(interface_name_.c_str(), true);
}

ClientInterfaceImpl::~ClientInterfaceImpl() {
  binder_->NotifyImplDead();
  scanner_->Invalidate();
  netlink_utils_->UnsubscribeFrameTxStatusEvent(interface_index_);
  netlink_utils_->UnsubscribeMlmeEvent(interface_index_);
  if_tool_->SetUpState(interface_name_.c_str(), false);
}

sp<android::net::wifi::IClientInterface> ClientInterfaceImpl::GetBinder() const {
  return binder_;
}

void ClientInterfaceImpl::Dump(std::stringstream* ss) const {
  *ss << "------- Dump of client interface with index: "
      << interface_index_ << " and name: " << interface_name_
      << "-------" << endl;
  *ss << "Max number of ssids for single shot scan: "
      << static_cast<int>(scan_capabilities_.max_num_scan_ssids) << endl;
  *ss << "Max number of ssids for scheduled scan: "
      << static_cast<int>(scan_capabilities_.max_num_sched_scan_ssids) << endl;
  *ss << "Max number of match sets for scheduled scan: "
      << static_cast<int>(scan_capabilities_.max_match_sets) << endl;
  *ss << "Maximum number of scan plans: "
      << scan_capabilities_.max_num_scan_plans << endl;
  *ss << "Max scan plan interval in seconds: "
      << scan_capabilities_.max_scan_plan_interval << endl;
  *ss << "Max scan plan iterations: "
      << scan_capabilities_.max_scan_plan_iterations << endl;
  *ss << "Device supports random MAC for single shot scan: "
      << wiphy_features_.supports_random_mac_oneshot_scan << endl;
  *ss << "Device supports low span single shot scan: "
      << wiphy_features_.supports_low_span_oneshot_scan << endl;
  *ss << "Device supports low power single shot scan: "
      << wiphy_features_.supports_low_power_oneshot_scan << endl;
  *ss << "Device supports high accuracy single shot scan: "
      << wiphy_features_.supports_high_accuracy_oneshot_scan << endl;
  *ss << "Device supports random MAC for scheduled scan: "
      << wiphy_features_.supports_random_mac_sched_scan << endl;
  *ss << "Device supports sending management frames at specified MCS rate: "
      << wiphy_features_.supports_tx_mgmt_frame_mcs << endl;
  *ss << "------- Dump End -------" << endl;
}

bool ClientInterfaceImpl::GetPacketCounters(vector<int32_t>* out_packet_counters) {
  StationInfo station_info;
  if (!netlink_utils_->GetStationInfo(interface_index_,
                                      bssid_,
                                      &station_info)) {
    return false;
  }
  out_packet_counters->push_back(station_info.station_tx_packets);
  out_packet_counters->push_back(station_info.station_tx_failed);

  return true;
}

bool ClientInterfaceImpl::SignalPoll(vector<int32_t>* out_signal_poll_results) {
  if (!IsAssociated()) {
    LOG(INFO) << "Fail RSSI polling because wifi is not associated.";
    return false;
  }

  StationInfo station_info;
  if (!netlink_utils_->GetStationInfo(interface_index_,
                                      bssid_,
                                      &station_info)) {
    return false;
  }
  out_signal_poll_results->push_back(
      static_cast<int32_t>(station_info.current_rssi));
  // Convert from 100kbit/s to Mbps.
  out_signal_poll_results->push_back(
      static_cast<int32_t>(station_info.station_tx_bitrate/10));
  // Association frequency.
  out_signal_poll_results->push_back(
      static_cast<int32_t>(associate_freq_));
  // Convert from 100kbit/s to Mbps.
  out_signal_poll_results->push_back(
      static_cast<int32_t>(station_info.station_rx_bitrate/10));

  return true;
}

const std::array<uint8_t, ETH_ALEN>& ClientInterfaceImpl::GetMacAddress() {
  return interface_mac_addr_;
}

bool ClientInterfaceImpl::SetMacAddress(const std::array<uint8_t, ETH_ALEN>& mac) {
  if (!if_tool_->SetWifiUpState(false)) {
    LOG(ERROR) << "SetWifiUpState(false) failed.";
    return false;
  }
  if (!if_tool_->SetMacAddress(interface_name_.c_str(), mac)) {
    LOG(ERROR) << "SetMacAddress(" << interface_name_ << ", "
               << LoggingUtils::GetMacString(mac) << ") failed.";
    return false;
  }
  if (!if_tool_->SetWifiUpState(true)) {
    LOG(ERROR) << "SetWifiUpState(true) failed.";
    return false;
  }
  LOG(DEBUG) << "Successfully SetMacAddress.";
  return true;
}

bool ClientInterfaceImpl::RefreshAssociateFreq() {
  // wpa_supplicant fetches associate frequency using the latest scan result.
  // We should follow the same method here before we find a better solution.
  std::vector<NativeScanResult> scan_results;
  if (!scan_utils_->GetScanResult(interface_index_, &scan_results)) {
    return false;
  }
  for (auto& scan_result : scan_results) {
    if (scan_result.associated) {
      associate_freq_ = scan_result.frequency;
    }
  }
  return false;
}

bool ClientInterfaceImpl::IsAssociated() const {
  return is_associated_;
}

void ClientInterfaceImpl::SendMgmtFrame(const vector<uint8_t>& frame,
    const sp<ISendMgmtFrameEvent>& callback, int32_t mcs) {
  if (mcs >= 0 && !wiphy_features_.supports_tx_mgmt_frame_mcs) {
    callback->OnFailure(
        ISendMgmtFrameEvent::SEND_MGMT_FRAME_ERROR_MCS_UNSUPPORTED);
    return;
  }

  uint64_t cookie;
  if (!netlink_utils_->SendMgmtFrame(interface_index_, frame, mcs, &cookie)) {
    callback->OnFailure(ISendMgmtFrameEvent::SEND_MGMT_FRAME_ERROR_UNKNOWN);
    return;
  }

  frame_tx_in_progress_ = true;
  frame_tx_status_cookie_ = cookie;
  nsecs_t start_time_ns = systemTime(SYSTEM_TIME_MONOTONIC);
  on_frame_tx_status_event_handler_ =
      [callback, start_time_ns](bool was_acked) {
        if (was_acked) {
          nsecs_t end_time_ns = systemTime(SYSTEM_TIME_MONOTONIC);
          int32_t elapsed_time_ms = static_cast<int32_t>(
              nanoseconds_to_milliseconds(end_time_ns - start_time_ns));
          callback->OnAck(elapsed_time_ms);
        } else {
          callback->OnFailure(
              ISendMgmtFrameEvent::SEND_MGMT_FRAME_ERROR_NO_ACK);
        }
      };
}

}  // namespace wificond
}  // namespace android
