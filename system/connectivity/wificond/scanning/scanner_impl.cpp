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

#include "wificond/scanning/scanner_impl.h"

#include <set>
#include <string>
#include <vector>

#include <android-base/logging.h>

#include "wificond/client_interface_impl.h"
#include "wificond/scanning/offload/offload_scan_manager.h"
#include "wificond/scanning/offload/offload_service_utils.h"
#include "wificond/scanning/scan_utils.h"

using android::binder::Status;
using android::net::wifi::IPnoScanEvent;
using android::net::wifi::IScanEvent;
using android::net::wifi::IWifiScannerImpl;
using android::sp;
using com::android::server::wifi::wificond::NativeScanResult;
using com::android::server::wifi::wificond::PnoSettings;
using com::android::server::wifi::wificond::SingleScanSettings;

using std::string;
using std::vector;
using std::weak_ptr;
using std::shared_ptr;

using namespace std::placeholders;

namespace {
using android::wificond::WiphyFeatures;
bool IsScanTypeSupported(int scan_type, const WiphyFeatures& wiphy_features) {
  switch(scan_type) {
    case IWifiScannerImpl::SCAN_TYPE_LOW_SPAN:
      return wiphy_features.supports_low_span_oneshot_scan;
    case IWifiScannerImpl::SCAN_TYPE_LOW_POWER:
      return wiphy_features.supports_low_power_oneshot_scan;
    case IWifiScannerImpl::SCAN_TYPE_HIGH_ACCURACY:
      return wiphy_features.supports_high_accuracy_oneshot_scan;
    default:
      CHECK(0) << "Invalid scan type received: " << scan_type;
  }
  return {};
}

constexpr const int kPercentNetworksWithFreq = 30;
constexpr const int kPnoScanDefaultFreqs[] = {2412, 2417, 2422, 2427, 2432, 2437, 2447, 2452,
    2457, 2462, 5180, 5200, 5220, 5240, 5745, 5765, 5785, 5805};
} // namespace

namespace android {
namespace wificond {

ScannerImpl::ScannerImpl(uint32_t interface_index,
                         const ScanCapabilities& scan_capabilities,
                         const WiphyFeatures& wiphy_features,
                         ClientInterfaceImpl* client_interface,
                         ScanUtils* scan_utils,
                         weak_ptr<OffloadServiceUtils> offload_service_utils)
    : valid_(true),
      scan_started_(false),
      pno_scan_started_(false),
      offload_scan_supported_(false),
      pno_scan_running_over_offload_(false),
      pno_scan_results_from_offload_(false),
      interface_index_(interface_index),
      scan_capabilities_(scan_capabilities),
      wiphy_features_(wiphy_features),
      client_interface_(client_interface),
      scan_utils_(scan_utils),
      scan_event_handler_(nullptr) {
  // Subscribe one-shot scan result notification from kernel.
  LOG(INFO) << "subscribe scan result for interface with index: "
            << (int)interface_index_;
  scan_utils_->SubscribeScanResultNotification(
      interface_index_,
      std::bind(&ScannerImpl::OnScanResultsReady, this, _1, _2, _3, _4));
  // Subscribe scheduled scan result notification from kernel.
  scan_utils_->SubscribeSchedScanResultNotification(
      interface_index_,
      std::bind(&ScannerImpl::OnSchedScanResultsReady,
                this,
                _1, _2));
  std::shared_ptr<OffloadScanCallbackInterfaceImpl>
      offload_scan_callback_interface =
          offload_service_utils.lock()->GetOffloadScanCallbackInterface(this);
  offload_scan_manager_ = offload_service_utils.lock()->GetOffloadScanManager(
      offload_service_utils, offload_scan_callback_interface);
  offload_scan_supported_ = offload_service_utils.lock()->IsOffloadScanSupported();
}

ScannerImpl::~ScannerImpl() {}

void ScannerImpl::Invalidate() {
  LOG(INFO) << "Unsubscribe scan result for interface with index: "
            << (int)interface_index_;
  scan_utils_->UnsubscribeScanResultNotification(interface_index_);
  scan_utils_->UnsubscribeSchedScanResultNotification(interface_index_);
  valid_ = false;
}

bool ScannerImpl::CheckIsValid() {
  if (!valid_) {
    LOG(DEBUG) << "Calling on a invalid scanner object."
               << "Underlying client interface object was destroyed.";
  }
  return valid_;
}

Status ScannerImpl::getScanResults(vector<NativeScanResult>* out_scan_results) {
  if (!CheckIsValid()) {
    return Status::ok();
  }
  if (!scan_utils_->GetScanResult(interface_index_, out_scan_results)) {
    LOG(ERROR) << "Failed to get scan results via NL80211";
  }
  return Status::ok();
}

Status ScannerImpl::getPnoScanResults(
    vector<NativeScanResult>* out_scan_results) {
  if (!CheckIsValid()) {
    return Status::ok();
  }
  if (pno_scan_results_from_offload_) {
    if (!offload_scan_manager_->getScanResults(out_scan_results)) {
      LOG(ERROR) << "Failed to get scan results via Offload HAL";
    }
  } else {
    if (!scan_utils_->GetScanResult(interface_index_, out_scan_results)) {
      LOG(ERROR) << "Failed to get scan results via NL80211";
    }
  }
  return Status::ok();
}

Status ScannerImpl::scan(const SingleScanSettings& scan_settings,
                         bool* out_success) {
  if (!CheckIsValid()) {
    *out_success = false;
    return Status::ok();
  }

  if (scan_started_) {
    LOG(WARNING) << "Scan already started";
  }
  // Only request MAC address randomization when station is not associated.
  bool request_random_mac =
      wiphy_features_.supports_random_mac_oneshot_scan &&
      !client_interface_->IsAssociated();
  int scan_type = scan_settings.scan_type_;
  if (!IsScanTypeSupported(scan_settings.scan_type_, wiphy_features_)) {
    LOG(DEBUG) << "Ignoring scan type because device does not support it";
    scan_type = SCAN_TYPE_DEFAULT;
  }

  // Initialize it with an empty ssid for a wild card scan.
  vector<vector<uint8_t>> ssids = {{}};

  vector<vector<uint8_t>> skipped_scan_ssids;
  for (auto& network : scan_settings.hidden_networks_) {
    if (ssids.size() + 1 > scan_capabilities_.max_num_scan_ssids) {
      skipped_scan_ssids.emplace_back(network.ssid_);
      continue;
    }
    ssids.push_back(network.ssid_);
  }

  LogSsidList(skipped_scan_ssids, "Skip scan ssid for single scan");

  vector<uint32_t> freqs;
  for (auto& channel : scan_settings.channel_settings_) {
    freqs.push_back(channel.frequency_);
  }

  int error_code = 0;
  if (!scan_utils_->Scan(interface_index_, request_random_mac, scan_type,
                         ssids, freqs, &error_code)) {
    CHECK(error_code != ENODEV) << "Driver is in a bad state, restarting wificond";
    *out_success = false;
    return Status::ok();
  }
  scan_started_ = true;
  *out_success = true;
  return Status::ok();
}

Status ScannerImpl::startPnoScan(const PnoSettings& pno_settings,
                                 bool* out_success) {
  pno_settings_ = pno_settings;
  pno_scan_results_from_offload_ = false;
  LOG(VERBOSE) << "startPnoScan";
  if (offload_scan_supported_ && StartPnoScanOffload(pno_settings)) {
    // scanning over offload succeeded
    *out_success = true;
  } else {
    *out_success = StartPnoScanDefault(pno_settings);
  }
  return Status::ok();
}

bool ScannerImpl::StartPnoScanOffload(const PnoSettings& pno_settings) {
  OffloadScanManager::ReasonCode reason_code;
  vector<vector<uint8_t>> scan_ssids;
  vector<vector<uint8_t>> match_ssids;
  vector<uint8_t> match_security;
  // Empty frequency list: scan all frequencies.
  vector<uint32_t> freqs;

  ParsePnoSettings(pno_settings, &scan_ssids, &match_ssids, &freqs,
                   &match_security);
  pno_scan_running_over_offload_ = offload_scan_manager_->startScan(
      pno_settings.interval_ms_,
      // TODO: honor both rssi thresholds.
      pno_settings.min_5g_rssi_, scan_ssids, match_ssids, match_security, freqs,
      &reason_code);
  if (pno_scan_running_over_offload_) {
    LOG(VERBOSE) << "Pno scans requested over Offload HAL";
    if (pno_scan_event_handler_ != nullptr) {
      pno_scan_event_handler_->OnPnoScanOverOffloadStarted();
    }
  }
  return pno_scan_running_over_offload_;
}

void ScannerImpl::ParsePnoSettings(const PnoSettings& pno_settings,
                                   vector<vector<uint8_t>>* scan_ssids,
                                   vector<vector<uint8_t>>* match_ssids,
                                   vector<uint32_t>* freqs,
                                   vector<uint8_t>* match_security) {
  // TODO provide actionable security match parameters
  const uint8_t kNetworkFlagsDefault = 0;
  vector<vector<uint8_t>> skipped_scan_ssids;
  vector<vector<uint8_t>> skipped_match_ssids;
  std::set<int32_t> unique_frequencies;
  int num_networks_no_freqs = 0;
  for (auto& network : pno_settings.pno_networks_) {
    // Add hidden network ssid.
    if (network.is_hidden_) {
      // TODO remove pruning for Offload Scans
      if (scan_ssids->size() + 1 >
          scan_capabilities_.max_num_sched_scan_ssids) {
        skipped_scan_ssids.emplace_back(network.ssid_);
        continue;
      }
      scan_ssids->push_back(network.ssid_);
    }

    if (match_ssids->size() + 1 > scan_capabilities_.max_match_sets) {
      skipped_match_ssids.emplace_back(network.ssid_);
      continue;
    }
    match_ssids->push_back(network.ssid_);
    match_security->push_back(kNetworkFlagsDefault);

    // build the set of unique frequencies to scan for.
    for (const auto& frequency : network.frequencies_) {
      unique_frequencies.insert(frequency);
    }
    if (network.frequencies_.empty()) {
      num_networks_no_freqs++;
    }
  }

  // Also scan the default frequencies if there is frequency data passed down but more than 30% of
  // networks don't have frequency data.
  if (unique_frequencies.size() > 0 && num_networks_no_freqs * 100 / match_ssids->size()
      > kPercentNetworksWithFreq) {
    unique_frequencies.insert(std::begin(kPnoScanDefaultFreqs), std::end(kPnoScanDefaultFreqs));
  }
  for (const auto& frequency : unique_frequencies) {
    freqs->push_back(frequency);
  }
  LogSsidList(skipped_scan_ssids, "Skip scan ssid for pno scan");
  LogSsidList(skipped_match_ssids, "Skip match ssid for pno scan");
}

bool ScannerImpl::StartPnoScanDefault(const PnoSettings& pno_settings) {
  if (!CheckIsValid()) {
    return false;
  }
  if (pno_scan_started_) {
    LOG(WARNING) << "Pno scan already started";
  }
  // An empty ssid for a wild card scan.
  vector<vector<uint8_t>> scan_ssids = {{}};
  vector<vector<uint8_t>> match_ssids;
  vector<uint8_t> unused;
  // Empty frequency list: scan all frequencies.
  vector<uint32_t> freqs;

  ParsePnoSettings(pno_settings, &scan_ssids, &match_ssids, &freqs, &unused);
  // Only request MAC address randomization when station is not associated.
  bool request_random_mac = wiphy_features_.supports_random_mac_sched_scan &&
      !client_interface_->IsAssociated();
  // Always request a low power scan for PNO, if device supports it.
  bool request_low_power = wiphy_features_.supports_low_power_oneshot_scan;

  bool request_sched_scan_relative_rssi = wiphy_features_.supports_ext_sched_scan_relative_rssi;

  int error_code = 0;
  struct SchedScanReqFlags req_flags = {};
  req_flags.request_random_mac = request_random_mac;
  req_flags.request_low_power = request_low_power;
  req_flags.request_sched_scan_relative_rssi = request_sched_scan_relative_rssi;
  if (!scan_utils_->StartScheduledScan(interface_index_,
                                       GenerateIntervalSetting(pno_settings),
                                       pno_settings.min_2g_rssi_,
                                       pno_settings.min_5g_rssi_,
                                       req_flags,
                                       scan_ssids,
                                       match_ssids,
                                       freqs,
                                       &error_code)) {
    LOG(ERROR) << "Failed to start pno scan";
    CHECK(error_code != ENODEV) << "Driver is in a bad state, restarting wificond";
    return false;
  }
  string freq_string;
  if (freqs.empty()) {
    freq_string = "for all supported frequencies";
  } else {
    freq_string = "for frequencies: ";
    for (uint32_t f : freqs) {
      freq_string += std::to_string(f) + ", ";
    }
  }
  LOG(INFO) << "Pno scan started " << freq_string;
  pno_scan_started_ = true;
  return true;
}

Status ScannerImpl::stopPnoScan(bool* out_success) {
  if (offload_scan_supported_ && StopPnoScanOffload()) {
    // Pno scans over offload stopped successfully
    *out_success = true;
  } else {
    // Pno scans were not requested over offload
    *out_success = StopPnoScanDefault();
  }
  return Status::ok();
}

bool ScannerImpl::StopPnoScanOffload() {
  OffloadScanManager::ReasonCode reason_code;
  if (!pno_scan_running_over_offload_) {
    return false;
  }
  if (!offload_scan_manager_->stopScan(&reason_code)) {
    LOG(WARNING) << "Unable to unsubscribe to Offload scan results";
  }
  pno_scan_running_over_offload_ = false;
  LOG(VERBOSE) << "Pno scans over Offload stopped";
  return true;
}

bool ScannerImpl::StopPnoScanDefault() {
  if (!CheckIsValid()) {
    return false;
  }

  if (!pno_scan_started_) {
    LOG(WARNING) << "No pno scan started";
  }
  if (!scan_utils_->StopScheduledScan(interface_index_)) {
    return false;
  }
  LOG(INFO) << "Pno scan stopped";
  pno_scan_started_ = false;
  return true;
}

Status ScannerImpl::abortScan() {
  if (!CheckIsValid()) {
    return Status::ok();
  }

  if (!scan_started_) {
    LOG(WARNING) << "Scan is not started. Ignore abort request";
    return Status::ok();
  }
  if (!scan_utils_->AbortScan(interface_index_)) {
    LOG(WARNING) << "Abort scan failed";
  }
  return Status::ok();
}

Status ScannerImpl::subscribeScanEvents(const sp<IScanEvent>& handler) {
  if (!CheckIsValid()) {
    return Status::ok();
  }

  if (scan_event_handler_ != nullptr) {
    LOG(ERROR) << "Found existing scan events subscriber."
               << " This subscription request will unsubscribe it";
  }
  scan_event_handler_ = handler;
  return Status::ok();
}

Status ScannerImpl::unsubscribeScanEvents() {
  scan_event_handler_ = nullptr;
  return Status::ok();
}

Status ScannerImpl::subscribePnoScanEvents(const sp<IPnoScanEvent>& handler) {
  if (!CheckIsValid()) {
    return Status::ok();
  }

  if (pno_scan_event_handler_ != nullptr) {
    LOG(ERROR) << "Found existing pno scan events subscriber."
               << " This subscription request will unsubscribe it";
  }
  pno_scan_event_handler_ = handler;

  return Status::ok();
}

Status ScannerImpl::unsubscribePnoScanEvents() {
  pno_scan_event_handler_ = nullptr;
  return Status::ok();
}

void ScannerImpl::OnScanResultsReady(uint32_t interface_index, bool aborted,
                                     vector<vector<uint8_t>>& ssids,
                                     vector<uint32_t>& frequencies) {
  if (!scan_started_) {
    LOG(INFO) << "Received external scan result notification from kernel.";
  }
  scan_started_ = false;
  if (scan_event_handler_ != nullptr) {
    // TODO: Pass other parameters back once we find framework needs them.
    if (aborted) {
      LOG(WARNING) << "Scan aborted";
      scan_event_handler_->OnScanFailed();
    } else {
      scan_event_handler_->OnScanResultReady();
    }
  } else {
    LOG(WARNING) << "No scan event handler found.";
  }
}

void ScannerImpl::OnSchedScanResultsReady(uint32_t interface_index,
                                          bool scan_stopped) {
  if (pno_scan_event_handler_ != nullptr) {
    if (scan_stopped) {
      // If |pno_scan_started_| is false.
      // This stop notification might result from our own request.
      // See the document for NL80211_CMD_SCHED_SCAN_STOPPED in nl80211.h.
      if (pno_scan_started_) {
        LOG(WARNING) << "Unexpected pno scan stopped event";
        pno_scan_event_handler_->OnPnoScanFailed();
      }
      pno_scan_started_ = false;
    } else {
      LOG(INFO) << "Pno scan result ready event";
      pno_scan_results_from_offload_ = false;
      pno_scan_event_handler_->OnPnoNetworkFound();
    }
  }
}

SchedScanIntervalSetting ScannerImpl::GenerateIntervalSetting(
    const ::com::android::server::wifi::wificond::PnoSettings&
        pno_settings) const {
  bool support_num_scan_plans = scan_capabilities_.max_num_scan_plans >= 2;
  bool support_scan_plan_interval =
      scan_capabilities_.max_scan_plan_interval * 1000 >=
          pno_settings.interval_ms_ * PnoSettings::kSlowScanIntervalMultiplier;
  bool support_scan_plan_iterations =
      scan_capabilities_.max_scan_plan_iterations >=
                  PnoSettings::kFastScanIterations;

  uint32_t fast_scan_interval =
      static_cast<uint32_t>(pno_settings.interval_ms_);
  if (support_num_scan_plans && support_scan_plan_interval &&
      support_scan_plan_iterations) {
    return SchedScanIntervalSetting{
        {{fast_scan_interval, PnoSettings::kFastScanIterations}},
        fast_scan_interval * PnoSettings::kSlowScanIntervalMultiplier};
  } else {
    // Device doesn't support the provided scan plans.
    // Specify single interval instead.
    // In this case, the driver/firmware is expected to implement back off
    // logic internally using |pno_settings.interval_ms_| as "fast scan"
    // interval.
    return SchedScanIntervalSetting{{}, fast_scan_interval};
  }
}

void ScannerImpl::OnOffloadScanResult() {
  if (!pno_scan_running_over_offload_) {
    LOG(WARNING) << "Scan results from Offload HAL but scan not requested over "
                    "this interface";
    return;
  }
  LOG(INFO) << "Offload Scan results received";
  pno_scan_results_from_offload_ = true;
  if (pno_scan_event_handler_ != nullptr) {
    pno_scan_event_handler_->OnPnoNetworkFound();
  } else {
    LOG(WARNING) << "No scan event handler Offload Scan result";
  }
}

void ScannerImpl::OnOffloadError(
    OffloadScanCallbackInterface::AsyncErrorReason error_code) {
  if (!pno_scan_running_over_offload_) {
    // Ignore irrelevant error notifications
    LOG(WARNING) << "Offload HAL Async Error occured but Offload HAL is not "
                    "subscribed to";
    return;
  }
  LOG(ERROR) << "Offload Service Async Failure error_code=" << error_code;
  switch (error_code) {
    case OffloadScanCallbackInterface::AsyncErrorReason::BINDER_DEATH:
      LOG(ERROR) << "Binder death";
      if (pno_scan_event_handler_ != nullptr) {
        pno_scan_event_handler_->OnPnoScanOverOffloadFailed(
            net::wifi::IPnoScanEvent::PNO_SCAN_OVER_OFFLOAD_BINDER_FAILURE);
      }
      break;
    case OffloadScanCallbackInterface::AsyncErrorReason::REMOTE_FAILURE:
      LOG(ERROR) << "Remote failure";
      if (pno_scan_event_handler_ != nullptr) {
        pno_scan_event_handler_->OnPnoScanOverOffloadFailed(
            net::wifi::IPnoScanEvent::PNO_SCAN_OVER_OFFLOAD_REMOTE_FAILURE);
      }
      break;
    default:
      LOG(WARNING) << "Invalid Error code";
      break;
  }
  bool success = false;
  // Stop scans over Offload HAL and request them over netlink
  stopPnoScan(&success);
  if (success) {
    LOG(INFO) << "Pno scans stopped";
  }
  // Restart PNO scans over netlink interface
  success = StartPnoScanDefault(pno_settings_);
  if (success) {
    LOG(INFO) << "Pno scans restarted";
  } else {
    LOG(ERROR) << "Unable to fall back to netlink pno scan";
    pno_scan_event_handler_->OnPnoScanFailed();
  }
}

void ScannerImpl::LogSsidList(vector<vector<uint8_t>>& ssid_list,
                              string prefix) {
  if (ssid_list.empty()) {
    return;
  }
  string ssid_list_string;
  for (auto& ssid : ssid_list) {
    ssid_list_string += string(ssid.begin(), ssid.end());
    if (&ssid != &ssid_list.back()) {
      ssid_list_string += ", ";
    }
  }
  LOG(WARNING) << prefix << ": " << ssid_list_string;
}

}  // namespace wificond
}  // namespace android
